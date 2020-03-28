//
// File name: Panel3D.CPP
//
// Description: The support files for the Panel3D.HPP header
//
// Author: John De Goes
//
// Project: Cutting SPoint 3D Game Programming
//

#include <Math.H>
#include <Stdlib.H>
#include <String.H>

#include "Panel3D.HPP"
#include "LineType.HPP"
#include "FixASM.HPP"
#include "3DCLass.HPP"
#include "LightP.HPP"
#include "TextType.HPP"
#include "PalShade.HPP"

// Declare the textures for the polygons:
ImageData TextDat;

// Delcare the texture shade look-up table:
ShadeDat TextShade;

void inline ClipHLine ( long &X1, long &X2, long &Z, long ZStep,
                        long &U, long UStep, long &V, long VStep,
                        long &I, long IStep )
   {
   long register Step;
   // Clip a horizontal "Z-buffered" line:
   if ( X1 < MINX )
      {
      Step = ( MINX - X1 );
      // Take advantage of the fact that ( a * ( b * f ) / f ) 
      // is equal to ( a * b );
      Z += ZStep * Step;
      U += UStep * Step;
      V += VStep * Step;
      I += IStep * Step;
      X1 = MINX;
      }
   else if ( X1 > MAXX )
           X1 = MAXX;
   if ( X2 < MINX )
      X2 = MINX;
   else if ( X2 > MAXX )
           X2 = MAXX;
   }

void Panel3D::CalcRadius ()
   {
   // Calculate the radius of the panel:

   // Initialize/Declare variables:
   Point3D TempPoint [ 4 ], Center;
   unsigned int Count;
   double Distance [ 4 ], Dist;
  
   Center.Lx = ( VPoint [ 0 ]->Lx +
                 VPoint [ 1 ]->Lx +
                 VPoint [ 2 ]->Lx +
                 VPoint [ 3 ]->Lx ) / 4.0F;

   Center.Ly = ( VPoint [ 0 ]->Ly +
                 VPoint [ 1 ]->Ly +
                 VPoint [ 2 ]->Ly +
                 VPoint [ 3 ]->Ly ) / 4.0F;

   Center.Lz = ( VPoint [ 0 ]->Lz +
                 VPoint [ 1 ]->Lz +
                 VPoint [ 2 ]->Lz +
                 VPoint [ 3 ]->Lz ) / 4.0F;

   // Translate the panel to the origin and store it in TempPoint:
   for ( Count = 0; Count < 4; Count++ )
       {
       TempPoint [ Count ] = ( *VPoint[ Count ] ) - Center;
       }

   // Calculate the distance to each of the vertices:
   for ( Count = 0; Count < 4; Count++ )
       {
       // Calculate distance to the panel:
       Dist = TempPoint [ Count ].Mag ();
       // Store it:
       Distance [ Count ] = Dist;
       }

   // Determine the maximum distance:
   Dist = Distance [ 0 ];
   for ( Count = 1; Count < 4; Count++ )
       {
       if ( Distance [ Count ] > Dist )
          Dist = Distance [ Count ];
       }
   Radius = Dist * 1.1;

   // Calculate the distance to each of the vertices:
   for ( Count = 0; Count < 4; Count++ )
       {
       // Calculate distance to object ON THE X/Z PLANE!
       Dist = 
       sqrt ( TempPoint [ Count ].Lx * TempPoint [ Count ].Lx +
              TempPoint [ Count ].Lz * TempPoint [ Count ].Lz );
       Distance [ Count ] = Dist;
       }

   // Determine the maximum distance:
   Dist = Distance [ 0 ];
   for ( Count = 1; Count < 4; Count++ )
       {
       if ( Distance [ Count ] > Dist )
          Dist = Distance [ Count ];
       }
   // Dist holds the maximum X/Z extent of the panel:
   RadiusXZ = Dist;

   // Calculate the distance to each of the vertices:
   for ( Count = 0; Count < 4; Count++ )
       {
       // Calculate distance to object ON THE Y AXIS!
       Dist = TempPoint [ Count ].Ly;
       Distance [ Count ] = Dist;
       }

   // Determine the maximum distance:
   Dist = Distance [ 0 ];
   for ( Count = 1; Count < 4; Count++ )
       {
       if ( Distance [ Count ] > Dist )
          Dist = Distance [ Count ];
       }
   // Dist holds the maximum Y extent of the panel:
   RadiusY = Dist;
   }

void Panel3D::CalcInten ()
   {
   double Mag = ( sqrt ( Light.X * Light.X +
                         Light.Y * Light.Y +
                         Light.Z * Light.Z ) );
   // Assign a shade based on normal:
   double CosA = ( ( Normal.X - VPoint [ 0 ]->Lx ) * Light.X +
                   ( Normal.Y - VPoint [ 0 ]->Ly ) * Light.Y +
                   ( Normal.Z - VPoint [ 0 ]->Lz ) * Light.Z ) / Mag;
   Color = ( ( CosA * ( double ) COLOR_RANGE ) + COLOR_START );
   }

void Panel3D::Project ()
   {
   // Perform front Z-clipping and project the panel's 3-dimensional points 
   // onto the screen:
   SPCount = 4;
   unsigned int Count, OutC = 0, StartI, EndI;
   double UOverZ, VOverZ, OneOverZ;
   Point3D ZClip [ 5 ]; // Maximum of 5 clipped Z points
   Detail3D NewA [ 5 ]; // Maximum of 5 clipped detail values

   // Initialize pointer to last vertex:
   StartI = SPCount - 1;

   // Loop through all edges of panel using S&H algorithm:
   for ( EndI = 0; EndI < SPCount; EndI++ )
       {
       if ( VPoint [ StartI ]->Wz >= MINZ )
          {
          if ( VPoint [ EndI ]->Wz >= MINZ )
             {
             // Entirely inside front view volume (case 1) - output unchanged 
             // vertex:
             ZClip [ OutC ].Wx = VPoint [ EndI ]->Wx;
             ZClip [ OutC ].Wy = VPoint [ EndI ]->Wy;
             ZClip [ OutC ].Wz = VPoint [ EndI ]->Wz;

             // Output unchanged detail values:
             NewA [ OutC ] = Attrib [ EndI ];

             // Update index:
             ++OutC;
             }
          else {
               // SPoint is leaving view volume (case 2) -
               // clip using parametric form of line:
               double DeltaZ = ( VPoint [ EndI ]->Wz - VPoint [ StartI ]->Wz );

               double t = ( MINZ - VPoint [ StartI ]->Wz ) / DeltaZ;

               ZClip [ OutC ].Wx = VPoint [ StartI ]->Wx + ( double )
                                   ( VPoint [ EndI ]->Wx - 
                                     VPoint [ StartI ]->Wx ) * t;
               ZClip [ OutC ].Wy = VPoint [ StartI ]->Wy + ( double )
                                   ( VPoint [ EndI ]->Wy - 
                                     VPoint [ StartI ]->Wy ) * t;
               ZClip [ OutC ].Wz = MINZ;

               // Calculate new surface detail values:
               NewA [ OutC ] = Attrib [ StartI ] + 
                               (  ( Attrib [ EndI ] - 
                                    Attrib [ StartI ] ) * t );
               // Update index:
               ++OutC;
               }
          }   
       else {
            if ( VPoint [ EndI ]->Wz >= MINZ ) 
               {
               // SPoint is entering view volume (case 3) - clip
               // using parametric form of line:
               double DeltaZ = ( VPoint [ EndI ]->Wz - VPoint [ StartI ]->Wz );

               double t = ( MINZ - VPoint [ StartI ]->Wz ) / DeltaZ;

               ZClip [ OutC ].Wx = VPoint [ StartI ]->Wx + ( double )
                                   ( VPoint [ EndI ]->Wx - 
                                     VPoint [ StartI ]->Wx )* t;
            
               ZClip [ OutC ].Wy = VPoint [ StartI ]->Wy + ( double )
                                   ( VPoint [ EndI ]->Wy - 
                                     VPoint [ StartI ]->Wy ) * t;
               ZClip [ OutC ].Wz =  MINZ;

               // Calculate new surface detail values:
               NewA [ OutC ] = Attrib [ StartI ] + 
                               (  ( Attrib [ EndI ] - 
                                    Attrib [ StartI ] ) * t );

               // Update index:
               ++OutC;

               // Add an extra edge to list:
               ZClip [ OutC ].Wx =  VPoint [ EndI ]->Wx;
               ZClip [ OutC ].Wy =  VPoint [ EndI ]->Wy;
               ZClip [ OutC ].Wz =  VPoint [ EndI ]->Wz;

               // Add an additional surface detail point to 
               // the list:
               NewA [ OutC ] = Attrib [ EndI ];

               // Update index:
               ++OutC;
               }
            else {
                 // No operation is necessary for case 4
                 }
            }   
       // Advance to next vertex:
       StartI = EndI;
       }

   // Store the number of vertices in OutC:
   SPCount = ( WORD ) OutC;

   // Project panel's points:
   for ( Count = 0; Count < OutC; Count++ )
       {
       OneOverZ = 1.0F / ZClip [ Count ].Wz;
       SPoint [ Count ].X = ZClip [ Count ].Wx * XSCALE *
                            OneOverZ + XCENTER;
       SPoint [ Count ].Y = ZClip [ Count ].Wy * YSCALE *
                            OneOverZ + YCENTER;
       SPoint [ Count ].Z = ( ( double ) OneOverZ *
                            ( ( double ) ( 1 << ZSTEP_PREC ) ) );

       UOverZ = ( double ) NewA [ Count ].U * OneOverZ;
       VOverZ = ( double ) NewA [ Count ].V * OneOverZ;

       SPoint [ Count ].U = UOverZ * ( double )
                            ( 1 << TSTEP_PREC );

       SPoint [ Count ].V = VOverZ * ( double )
                            ( 1 << TSTEP_PREC );
       long NewI = NewA [ Count ].I;

       // Perform depth shading:
       NewI = NewI + ZClip [ Count ].Wz / SHADE_DIV;
       if ( NewI > ( SHADE_COUNT - 1 ) )
          NewI = ( SHADE_COUNT - 1 );
       SPoint [ Count ].I = NewI << ISTEP_PREC;
       }
   }
   
void Panel3D::CalcNormal ()
   {
   // Calculate the normal of the panel:
   long double X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3,
               Distance, A, B, C, D;
   Point3D UniqueVerts [ 4 ], TVert;
   unsigned short Count, Range = 0;
   // Create a list of unique vertices:
   for ( Count = 0; Count < 4; Count++ )
       {
       TVert = *VPoint [ Count ];
       if ( Range == 0 )
          UniqueVerts [ Range++ ] = TVert;
       else if ( UniqueVert ( TVert, UniqueVerts, Range ) )
               {
               UniqueVerts [ Range++ ] = TVert;
               }
       }

   X1 = UniqueVerts [ 0 ].Lx;
   Y1 = UniqueVerts [ 0 ].Ly;
   Z1 = UniqueVerts [ 0 ].Lz;

   X2 = UniqueVerts [ 1 ].Lx;
   Y2 = UniqueVerts [ 1 ].Ly;
   Z2 = UniqueVerts [ 1 ].Lz;

   X3 = UniqueVerts [ 2 ].Lx;
   Y3 = UniqueVerts [ 2 ].Ly;
   Z3 = UniqueVerts [ 2 ].Lz;

   // Use plane equation to determine plane's orientation:
   A = Y1 * ( Z2 - Z3 ) + Y2 * ( Z3 - Z1 ) + Y3 * ( Z1 - Z2 );
   B = Z1 * ( X2 - X3 ) + Z2 * ( X3 - X1 ) + Z3 * ( X1 - X2 );
   C = X1 * ( Y2 - Y3 ) + X2 * ( Y3 - Y1 ) + X3 * ( Y1 - Y2 );
   D = -X1 * ( Y2 * Z3 - Y3 * Z2 )  -X2 * ( Y3 * Z1 - Y1 * Z3 )
       -X3 * ( Y1 * Z2 - Y2 * Z1 );

   // Get the distance to the vector:
   Distance = sqrt ( A*A + B*B + C*C );

   // Normalize the normal to 1 and create a point:
   Normal.X = ( A / Distance ) + VPoint [ 0 ]->Lx;
   Normal.Y = ( B / Distance ) + VPoint [ 0 ]->Ly;
   Normal.Z = ( C / Distance ) + VPoint [ 0 ]->Lz;
   Normal.D = ( D );/// Distance );
   Normal.Td = Distance;
   }

int inline Panel3D::CalcBFace ()
  {
  // Determine if polygon is a backface:
  int Visible = 1; Invis = 0;
  Point3D V = *VPoint [ 0 ];
  double Direction = ( V.Wx * ( Normal.Tx - VPoint [ 0 ]->Wx ) + 
                       V.Wy * ( Normal.Ty - VPoint [ 0 ]->Wy ) + 
                       V.Wz * ( Normal.Tz - VPoint [ 0 ]->Wz ) );
  if ( Direction > 0.0F )
     {
     // Get the cosine of the angle between the viewer and the polygon 
     // normal:
     Direction /= V.Mag ();
     // Assume panel will remain a time proportional to the angle between
     // the viewer to the polygon normal:
     Invis = ( double ) Direction * ( double ) 25.0F;
     // Set the invisible flag for this frame:
     Visible = 0;
     }
  return Visible;
  }


int Panel3D::CalcVisible3D ()
  {
  // Perform 3D culling:

  // Assume panel is visible:
  int Visible;

  // Perform a back-face culling operation:
  Visible = CalcBFace ();
   
  // If panel still visible, perform extent test:
  if ( Visible )
     Visible = CheckExtents ();
  return Visible;
  }

long Panel3D::CalcCenterZ ()
   {
   // Calculate the polygon's center Z:
   long SummedComponents = ( double ) VPoint [ 0 ] -> Wz + 
                           ( double ) VPoint [ 1 ] -> Wz +
                           ( double ) VPoint [ 2 ] -> Wz +
                           ( double ) VPoint [ 3 ] -> Wz;
   long CenterZ = ( SummedComponents  >> 2 );
   return CenterZ;
   }

int Panel3D::CalcVisible2D ()
  {
  // Perform 2D culling:

  // Assume panel is visible:
  long XMinInVis = 0, XMaxInVis = 0, YMinInVis = 0, YMaxInVis = 0;
  long Visible = 1, AveX = 0, AveY = 0, N;
  Invis = 0;

  // Make sure the panel has more than two points:
  if ( SPCount < 3 )
     {
     // If not, flag panel as invisible:
     Visible = 0;

     // Assume panel will remain invisible for four more frames:
     Invis = 4;
     return Visible;
     }

   // Determine location of panel's 2D points:
   for ( N = 0; N < SPCount; N++ )
       {
       if ( SPoint [ N ].X < MINX )
          ++XMinInVis;

       if ( SPoint [ N ].X > MAXX )
          ++XMaxInVis;

       if ( SPoint [ N ].Y < MINY )
          ++YMinInVis;

       if ( SPoint [ N ].Y > MAXY )
          ++YMaxInVis;

       AveX += SPoint [ N ].X;
       AveY += SPoint [ N ].Y;
       }

   if ( XMinInVis >= SPCount )
      {
      // Assume panel will remain invisible for a time 
      // proportional to the distance from the edge of the 
      // panel to the edge of the viewport:
      AveX /= SPCount;
      Invis = ( WORD ) ( abs ( AveX ) / ( 320 * 30 ) );
      Visible = 0;
      }
   else if ( YMinInVis >= SPCount )
           {
           // Assume panel will remain invisible for a time 
           // proportional to the distance from the edge of the 
           // panel to the edge of the viewport:
           AveY /= SPCount;
           Invis = ( WORD ) ( abs ( AveY ) / ( 200 * 30 ) );
           Visible = 0;
           }
   else if ( XMaxInVis >= SPCount )
           {
           // Assume panel will remain invisible for a time 
           // proportional to the distance from the edge of the 
           // panel to the edge of the viewport:
           AveX /= SPCount;
           Invis = ( WORD ) ( ( AveX - MAXX ) / ( 320 * 30 ) );
           Visible = 0;
           }
   else if ( YMaxInVis >= SPCount )
           {
           // Assume panel will remain invisible for a time 
           // proportional to the distance from the edge of the 
           // panel to the edge of the viewport:
           AveY /= SPCount;
           Invis = ( WORD ) ( ( AveY - MAXY ) / ( 200 * 30 ) );
           Visible = 0;
           }
  return Visible;
  }
   
int Panel3D::CheckExtents ()
  {
  // Determine if panel is in the range of MINZ to MAXZ:
  long Visible = 0, Count;
  double MinZ;
  for ( Count = 0; Count < 4; Count++ )
      {
      if ( VPoint [ Count ]->Wz > MINZ )
         {
         Visible = 1;
         Invis = 0;
         break;
         }
      }
  if ( Visible )
     {
     MinZ = VPoint [ 0 ]->Wz;
     for ( Count = 1; Count < 4; Count++ )
         {
         if ( VPoint [ Count ]->Wz < MinZ )
            MinZ = VPoint [ Count ]->Wz;
         }
     if ( MinZ > MAXZ )
        {
        // Set the invisible flag for this frame:
        Visible = 0;

        // Assume panel will remain invisible for a time 
        // proportional to the distance from the viewer:
        Invis = ( WORD ) ( ( MinZ - MAXZ ) / 50 );
        }
     }
  else {
       // Assume panel will remain invisible for a time 
       // proportional to the distance from the viewer:
       Invis = ( WORD ) ( abs ( CalcCenterZ () + MINZ ) / 50 );
       }
  return Visible;
  }

void Panel3D::Update ( Matrix3D &M )
   {
   // Transform the normal:
   M.Transform ( Normal );

   // Update the invisibility flag:
   if ( Invis > 0 )
      --Invis;
   }

void Panel3D::DisplayNorm ( unsigned char *Dest )
   {
   // Display the panel in the screen buffer "Dest":
   unsigned char register *DPtr, *IPtr;
   long register *ZPtr, Z;
   long register Tx, Ty;
   long NewTMask;
   long TxStep, TyStep, IStep, I;
   long Top = 0, N, RightPos, LeftPos, NewRightPos, NewLeftPos, 
        Height, EdgeCount, YIndex, Width, XStart, XEnd,  
        ZStep, U, V, UStep, VStep, IPos,
        RunLoops, Count, Tx1, Ty1, Length, 
        Tx2, Ty2, SubUStep, SubVStep, SubZStep, 
        RZ, PreMultShift;
   const RunLen = 16, RunShift = 4;
   CeilLine LeftSeg, RightSeg;

   // Create a pointer to the texture map:
   IPtr = TextDat.TMap [ TextHan ].Image;

   EdgeCount = SPCount;

   // Pre-multiply the texture mask:
   NewTMask = TMASK * TextDat.TMap [ TextHan ].Width;

   switch ( TextDat.TMap [ TextHan ].Width )
          {
          case ( 2 ): PreMultShift = 1; break;
          case ( 4 ): PreMultShift = 2; break;
          case ( 8 ): PreMultShift = 3; break;
          case ( 16 ): PreMultShift = 4; break;
          case ( 32 ): PreMultShift = 5; break;
          case ( 64 ): PreMultShift = 6; break;
          case ( 128 ): PreMultShift = 7; break;
          case ( 256 ): PreMultShift = 8; break;
          case ( 512 ): PreMultShift = 9; break;
          }

   // Search for lowest Y coordinate (top of polygon):
   for ( N = 1; N < SPCount; N++ )
       {
       if ( SPoint [ N ].Y < SPoint [ Top ].Y )
          Top = N;
       }
   RightPos = Top;
   LeftPos = Top;

   // Calculate the index to the buffer:
   YIndex = SPoint [ Top ].Y * 320;

   // Loop for all polygon edges:
   while ( EdgeCount > 0 )
         {
         // Determine if the right side of the polygon needs 
         // (re)initializing:
         if ( RightSeg.Height () <= 0 )
            {
            NewRightPos = RightPos + 1;
            if ( NewRightPos >= SPCount )
               NewRightPos = 0;
            RightSeg.Init ( SPoint [ RightPos ], SPoint [ NewRightPos ] );
            RightPos = NewRightPos;
            --EdgeCount;
            // Perform object-precision clip on top of edge 
            // (if necessary):
            if ( RightSeg.GetY () < MINY )
               {
               RightSeg.ClipTop ( MINY );
               YIndex = MINY * 320;
               }
            }
         // Determine if the left side of the polygon needs 
         // (re)initializing:
         if ( LeftSeg.Height () <= 0 )
            {
            NewLeftPos = LeftPos - 1;
            if ( NewLeftPos < 0 )
               NewLeftPos = ( SPCount - 1 );
            LeftSeg.Init ( SPoint [ LeftPos ], SPoint [ NewLeftPos ] );
            LeftPos = NewLeftPos;
            --EdgeCount;
            // Perform object-precision clip on top of edge 
            // (if necessary):
            if ( LeftSeg.GetY () < MINY )
               {
               LeftSeg.ClipTop ( MINY );
               YIndex = MINY * 320;
               }
            }
         // Subdivide polygon into trapezoid:
         if ( LeftSeg.Height () < RightSeg.Height () )
            {
            Height = LeftSeg.Height ();
            if ( ( LeftSeg.GetY () + Height ) > MAXY )
               {
               Height = MAXY - LeftSeg.GetY ();
               EdgeCount = 0;
               }
            }
         else {
              Height = RightSeg.Height ();
              if ( ( RightSeg.GetY () + Height ) > MAXY )
                 {
                 Height = MAXY - RightSeg.GetY ();
                 EdgeCount = 0;
                 }
              }

         // Loop for the height of the trapezoid:
         while ( Height-- > 0 )
               {
               // Calculate initial values:
               XStart = LeftSeg.GetX ();
               XEnd = RightSeg.GetX ();
               Width = XEnd - XStart;
               if ( ( Width > 0 ) && ( XEnd > MINX ) && 
                    ( XStart < MAXX ) )
                  {
                  U = LeftSeg.GetU ();
                  V = LeftSeg.GetV ();
                  Z = LeftSeg.GetZ ();
                  I = LeftSeg.GetI ();

                  // Note: The following steps are linear in 
                  // screen space and are constant throughout 
                  // the entire polygon. Given enough Z 
                  // precision (which reduces the efficiency
                  // of the clear-reduction algorithm) they 
                  // can be calculated a single time and 
                  // used throughout the remainder of the 
                  // rasterization:
                  UStep = ( RightSeg.GetU () - LeftSeg.GetU () )
                            / Width;
                  VStep = ( RightSeg.GetV () - LeftSeg.GetV () )
                            / Width;
                  ZStep =  ( RightSeg.GetZ () - LeftSeg.GetZ () )
                            / Width;
                  
                  IStep = ( RightSeg.GetI () - LeftSeg.GetI () )
                            / Width;

                  // Clip the scan-line:
                  ClipHLine ( XStart, XEnd, Z, ZStep, U, UStep, 
                              V, VStep, I, IStep );
                  Width = XEnd - XStart;
                  DPtr = &Dest    [ YIndex + XStart ];
                  ZPtr = &ZBuffer [ YIndex + XStart ];

                  // Calculate subdivision loops:
                  RunLoops = Width >> RunShift;

                  if ( Width > 0 )
                     {
                     // Premultiply steps for subdivision:
                     SubUStep = UStep << RunShift;
                     SubVStep = VStep << RunShift;
                     SubZStep = ZStep << RunShift;
                     }

                  // Loop for number of subdivisions:
                  for ( Count = 0; Count < RunLoops; Count++ )
                      {
                      Length = RunLen;

                      // Calculate affine mapping coordinates
                      // based on perspective subdivision:
                      RZ  = Z - ZTrans;
                      fixdiv ( Tx1, U, RZ, LSTEP_PREC );
                      fixdiv ( Ty1, V, RZ, LSTEP_PREC );

                      // Jump ahead to next subdivision:
                      U  += SubUStep;
                      V  += SubVStep;
                      RZ += SubZStep;

                      fixdiv ( Tx2, U, RZ, LSTEP_PREC );
                      fixdiv ( Ty2, V, RZ, LSTEP_PREC );

                      TxStep = ( Tx2 - Tx1 ) >> RunShift;
                      TyStep = ( Ty2 - Ty1 ) >> RunShift;

                      Tx = Tx1;
                      // Premultiply texture's Y coordinate:
                      Ty = Ty1 << PreMultShift;

                      // Premultiply the texture's Y step:
                      TyStep <<= PreMultShift;

                      while ( Length-- > 0 )
                            {
                            if ( *ZPtr < Z )
                               {
                               *ZPtr = Z;
                               IPos = ( ( Ty & NewTMask ) + Tx ) 
                                      >> LSTEP_PREC;
                               *DPtr = TextShade.Shade [ 
                                         ( I & IMASK ) 
                                       + IPtr [ IPos ] ];
                               }
                            Z  += ZStep;
                            I  += IStep;
                            Tx += TxStep;
                            Ty += TyStep;
                            ++DPtr;
                            ++ZPtr;
                            }
                      }
                  // Calculate remainder of scan-line left:
                  // to rasterize:
                  Length = Width & ( RunLen - 1 );
                  if ( Length > 0 )
                     {
                     RZ  = Z - ZTrans;
                     fixdiv ( Tx1, U, RZ, LSTEP_PREC );
                     fixdiv ( Ty1, V, RZ, LSTEP_PREC );

                     // Jump ahead to next subdivision:
                     U  += ( UStep * Length );
                     V  += ( VStep * Length );
                     RZ += ( ZStep * Length );

                     fixdiv ( Tx2, U, RZ, LSTEP_PREC );
                     fixdiv ( Ty2, V, RZ, LSTEP_PREC );

                     TxStep = ( Tx2 - Tx1 ) / Length;
                     TyStep = ( Ty2 - Ty1 ) / Length;

                     Tx = Tx1;
                     Ty = Ty1;

                     // Premultiply the texture's Y coordinate:
                     Ty     <<= PreMultShift;

                     // Premultiply the texture's Y step:
                     TyStep <<= PreMultShift;

                     while ( Length-- > 0 )
                           {
                           if ( *ZPtr < Z )
                              {
                              *ZPtr = Z;
                              IPos = ( ( Ty & NewTMask ) + Tx ) 
                                      >> LSTEP_PREC;
                              *DPtr = TextShade.Shade [ 
                                         ( I & IMASK )
                                       + IPtr [ IPos ] ];
                              }
                           Z  += ZStep;
                           I  += IStep;
                           Tx += TxStep;
                           Ty += TyStep;
                           ++DPtr;
                           ++ZPtr;
                           }
                     }
                  }
               ++RightSeg;
               ++LeftSeg;
               YIndex += 320;
               }
         }
   }

void Panel3D::DisplaySel ( unsigned char *Dest )
   {
      // Display the panel in the screen buffer "Dest":
   unsigned char register *DPtr, *IPtr;
   long register *ZPtr, Z;
   long Tx, Ty, NewTMask, I;
   long TxStep, TyStep, IStep;
   long Top = 0, N, RightPos, LeftPos, NewRightPos, NewLeftPos, 
        Height, EdgeCount, YIndex, Width, XStart, XEnd,  
        ZStep, U, V, UStep, VStep, IPos,
        RunLoops, Count, Tx1, Ty1, Length, 
        Tx2, Ty2, SubUStep, SubVStep, SubZStep, RZ, 
        PreMultShift;
   const RunLen = 16, RunShift = 4;
   CeilLine LeftSeg, RightSeg;

   // Create a pointer to the texture map:
   IPtr = TextDat.TMap [ TextHan ].Image;

   EdgeCount = SPCount;

   // Pre-multiply the texture mask:
   NewTMask = TMASK * TextDat.TMap [ TextHan ].Width;

   switch ( TextDat.TMap [ TextHan ].Width )
          {
          case ( 2 ): PreMultShift = 1; break;
          case ( 4 ): PreMultShift = 2; break;
          case ( 8 ): PreMultShift = 3; break;
          case ( 16 ): PreMultShift = 4; break;
          case ( 32 ): PreMultShift = 5; break;
          case ( 64 ): PreMultShift = 6; break;
          case ( 128 ): PreMultShift = 7; break;
          case ( 256 ): PreMultShift = 8; break;
          case ( 512 ): PreMultShift = 9; break;
          }

   // Search for lowest Y coordinate (top of polygon):
   for ( N = 1; N < SPCount; N++ )
       {
       if ( SPoint [ N ].Y < SPoint [ Top ].Y )
          Top = N;
       }
   RightPos = Top;
   LeftPos = Top;

   // Calculate the index to the buffer:
   YIndex = SPoint [ Top ].Y * 320;

   // Loop for all polygon edges:
   while ( EdgeCount > 0 )
         {
         // Determine if the right side of the polygon needs 
         // (re)initializing:
         if ( RightSeg.Height () <= 0 )
            {
            NewRightPos = RightPos + 1;
            if ( NewRightPos >= SPCount )
               NewRightPos = 0;
            RightSeg.Init ( SPoint [ RightPos ], SPoint [ NewRightPos ] );
            RightPos = NewRightPos;
            --EdgeCount;
            // Perform object-precision clip on top of edge 
            // (if necessary):
            if ( RightSeg.GetY () < MINY )
               {
               RightSeg.ClipTop ( MINY );
               YIndex = MINY * 320;
               }
            }
         // Determine if the left side of the polygon needs 
         // (re)initializing:
         if ( LeftSeg.Height () <= 0 )
            {
            NewLeftPos = LeftPos - 1;
            if ( NewLeftPos < 0 )
               NewLeftPos = ( SPCount - 1 );
            LeftSeg.Init ( SPoint [ LeftPos ], SPoint [ NewLeftPos ] );
            LeftPos = NewLeftPos;
            --EdgeCount;
            // Perform object-precision clip on top of edge 
            // (if necessary):
            if ( LeftSeg.GetY () < MINY )
               {
               LeftSeg.ClipTop ( MINY );
               YIndex = MINY * 320;
               }
            }
         // Subdivide polygon into trapezoid:
         if ( LeftSeg.Height () < RightSeg.Height () )
            {
            Height = LeftSeg.Height ();
            if ( ( LeftSeg.GetY () + Height ) > MAXY )
               {
               Height = MAXY - LeftSeg.GetY ();
               EdgeCount = 0;
               }
            }
         else {
              Height = RightSeg.Height ();
              if ( ( RightSeg.GetY () + Height ) > MAXY )
                 {
                 Height = MAXY - RightSeg.GetY ();
                 EdgeCount = 0;
                 }
              }

         // Loop for the height of the trapezoid:
         while ( Height-- > 0 )
               {
               // Calculate initial values:
               XStart = LeftSeg.GetX ();
               XEnd = RightSeg.GetX ();
               Width = XEnd - XStart;
               if ( ( Width > 0 ) && ( XEnd > MINX ) && 
                    ( XStart < MAXX ) )
                  {
                  U = LeftSeg.GetU ();
                  V = LeftSeg.GetV ();
                  Z = LeftSeg.GetZ ();
                  I = LeftSeg.GetI ();

                  // Note: The following steps are linear in 
                  // screen space and are constant throughout 
                  // the entire polygon. Given enough Z 
                  // precision (which reduces the efficiency
                  // of the clear-reduction algorithm) they 
                  // can be calculated a single time and 
                  // used throughout the remainder of the 
                  // rasterization:
                  UStep = ( RightSeg.GetU () - LeftSeg.GetU () )
                            / Width;
                  VStep = ( RightSeg.GetV () - LeftSeg.GetV () )
                            / Width;
                  ZStep =  ( RightSeg.GetZ () - LeftSeg.GetZ () )
                            / Width;
                  
                  IStep = ( RightSeg.GetI () - LeftSeg.GetI () )
                            / Width;

                  // Clip the scan-line:
                  ClipHLine ( XStart, XEnd, Z, ZStep, U, UStep, 
                              V, VStep, I, IStep );
                  Width = XEnd - XStart;
                  DPtr = &Dest    [ YIndex + XStart ];
                  ZPtr = &ZBuffer [ YIndex + XStart ];

                  // Calculate subdivision loops:
                  RunLoops = Width >> RunShift;

                                    if ( Width > 0 )
                     {
                     // Premultiply steps for subdivision:
                     SubUStep = UStep << RunShift;
                     SubVStep = VStep << RunShift;
                     SubZStep = ZStep << RunShift;
                     }

                  // Loop for number of subdivisions:
                  for ( Count = 0; Count < RunLoops; Count++ )
                      {
                      Length = RunLen;

                      // Calculate affine mapping coordinates
                      // based on perspective subdivision:
                      RZ  = Z - ZTrans;
                      fixdiv ( Tx1, U, RZ, LSTEP_PREC );
                      fixdiv ( Ty1, V, RZ, LSTEP_PREC );

                      // Jump ahead to next subdivision:
                      U  += SubUStep;
                      V  += SubVStep;
                      RZ += SubZStep;

                      fixdiv ( Tx2, U, RZ, LSTEP_PREC );
                      fixdiv ( Ty2, V, RZ, LSTEP_PREC );

                      TxStep = ( Tx2 - Tx1 ) >> RunShift;
                      TyStep = ( Ty2 - Ty1 ) >> RunShift;

                      Tx = Tx1;
                      // Premultiply texture's Y coordinate:
                      Ty = Ty1 << PreMultShift;

                      // Premultiply the texture's Y step:
                      TyStep <<= PreMultShift;

                      while ( Length-- > 0 )
                            {
                            if ( *ZPtr < Z )
                               {
                               *ZPtr = Z;
                               IPos = ( ( Ty & NewTMask ) + Tx ) 
                                      >> LSTEP_PREC;
                               *DPtr = TextShade.Shade [ 
                                         ( I & IMASK )
                                       + IPtr [ IPos ] ];
                               }
                            Z  += ZStep;
                            I  += IStep;
                            Tx += TxStep;
                            Ty += TyStep;
                            ++DPtr;
                            ++ZPtr;
                            }
                      }
                  // Calculate remainder of scan-line left:
                  // to rasterize:
                  Length = Width & ( RunLen - 1 );
                  if ( Length > 0 )
                     {
                     RZ  = Z - ZTrans;
                     fixdiv ( Tx1, U, RZ, LSTEP_PREC );
                     fixdiv ( Ty1, V, RZ, LSTEP_PREC );

                     // Jump ahead to next subdivision:
                     U  += ( UStep * Length );
                     V  += ( VStep * Length );
                     RZ += ( ZStep * Length );

                     fixdiv ( Tx2, U, RZ, LSTEP_PREC );
                     fixdiv ( Ty2, V, RZ, LSTEP_PREC );

                     TxStep = ( Tx2 - Tx1 ) / Length;
                     TyStep = ( Ty2 - Ty1 ) / Length;

                     Tx = Tx1;
                     Ty = Ty1;

                     // Premultiply the texture's Y coordinate:
                     Ty     <<= PreMultShift;

                     // Premultiply the texture's Y step:
                     TyStep <<= PreMultShift;

                     while ( Length-- > 0 )
                           {
                           if ( *ZPtr < Z )
                              {
                              *ZPtr = Z;
                              IPos = ( ( Ty & NewTMask ) + Tx ) 
                                      >> LSTEP_PREC;
                              *DPtr = TextShade.Shade [ 
                                         ( I & IMASK )
                                       + IPtr [ IPos ] ];
                              }
                           Z  += ZStep;
                           I  += IStep;
                           Tx += TxStep;
                           Ty += TyStep;
                           ++DPtr;
                           ++ZPtr;
                           }
                     }
                  *( DPtr - 1 ) = 1;
                  Dest [ YIndex + XStart ] = 1;
                  }
               ++RightSeg;
               ++LeftSeg;
               YIndex += 320;
               }
         }
   }
