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

#define MINX    10
#define MAXX    309
#define MINY    10
#define MAXY    189
#define WIDTH   320
#define HEIGHT  200
#define XCENTER 160
#define YCENTER 100
#define MINZ    100.0F
#define MAXZ    10000.0F
#define XSCALE  120
#define YSCALE -120
#define COLOR_RANGE 120
#define COLOR_START 125

void inline ClipHLine ( long &X1, long &X2, long &Z, long ZStep,
                        long &I, long IStep )
   {
   // Clip a horizontal "Z-buffered" line:
   if ( X1 < MINX )
      {
      // Take advantage of the fact that ( a * ( b * f ) / f ) 
      // is equal to ( a * b );
      Z += ZStep * ( MINX - X1 );
      I += IStep * ( MINX - X1 );
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

   // Create a temporary vertex list:
   for ( Count = 0; Count < 4; Count++ )
       {
       TempPoint [ Count ] = *VPoint[ Count ];
       }

   // Calculate center of polygon:
   for ( Count = 0; Count < 4; Count++ )
       {
       Center += TempPoint [ Count ];
       }
   Center /= 4.0F;

   // Translate polygon to it's center:
   for ( Count = 0; Count < 4; Count++ )
       {
       TempPoint [ Count ] -= Center;
       }

   // Calculate the distance to each of the vertices:
   for ( Count = 0; Count < 4; Count++ )
       {
       Dist = TempPoint [ Count ].Mag ();
       Distance [ Count ] = Dist;
       }

   // Determine the maximum distance:
   Dist = Distance [ 0 ];
   for ( Count = 1; Count < 4; Count++ )
       {
       if ( Distance [ Count ] > Dist )
          Dist = Distance [ Count ];
       }
   // Dist holds the maximum radius of the panel:
   Radius = Dist;
   }

void Panel3D::CalcInten ()
   {
   double Mag = ( sqrt ( Light.X * Light.X +
                         Light.Y * Light.Y +
                         Light.Z * Light.Z ) );
   // Assign a color based on normal:
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
   double OneOverZ;
   Point3D ZClip [ 5 ]; // Maximum of 5 clipped Z points
   WORD NewInten [ 5 ];      // Maximum of 5 clipped intensity values

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

             // Output unchanged intensity value:
             NewInten [ OutC ] = Inten [ EndI ];

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

               // Calculate new intensity value:
               NewInten [ OutC ] = ( double ) Inten [ StartI ] + ( double ) 
                                   ( Inten [ EndI ] - Inten [ StartI ] ) * t; 

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

               // Calculate new intensity value:
               NewInten [ OutC ] = ( double ) Inten [ StartI ] + ( double ) 
                                   ( Inten [ EndI ] - Inten [ StartI ] ) * t;

               // Update index:
               ++OutC;
               
               // Add an extra edge to list:
               ZClip [ OutC ].Wx =  VPoint [ EndI ]->Wx;
               ZClip [ OutC ].Wy =  VPoint [ EndI ]->Wy;
               ZClip [ OutC ].Wz =  VPoint [ EndI ]->Wz;

               // Add an extra intensity:
               NewInten [ OutC ] = Inten [ EndI ];

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
       SPoint [ Count ].X  = ZClip [ Count ].Wx * XSCALE * 
                             OneOverZ + 160.0F;
       SPoint [ Count ].Y  = ZClip [ Count ].Wy * YSCALE * 
                             OneOverZ + 100.0F;
       SPoint [ Count ].Z  = ( ( double ) OneOverZ * 
                             ( ( double ) ( 1 << ZSTEP_PREC ) ) );
       SPoint [ Count ].Inten = NewInten [ Count ];
       }
   }
   
void Panel3D::CalcNormal ()
   {
   // Calculate the normal of the panel:
   long double X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3, Distance, A, B, C;
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

   // Get the distance to the vector:
   Distance = sqrt ( A*A + B*B + C*C );
   
   // Normalize the normal to 1 and create a point:
   Normal.X = ( A / Distance ) + VPoint [ 0 ]->Lx;
   Normal.Y = ( B / Distance ) + VPoint [ 0 ]->Ly;
   Normal.Z = ( C / Distance ) + VPoint [ 0 ]->Lz;
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
       Invis = ( WORD ) ( abs ( CalcCenterZ () ) / 50 );
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

void Panel3D::Display ( unsigned char *Dest )
   {
   // Display the panel in the screen buffer "Dest":
   unsigned char RColor, *DPtr;
   CeilLine LeftSeg, RightSeg;
   long Top = 0, N, RightPos, LeftPos, NewRightPos, NewLeftPos, 
        Height, EdgeCount, YIndex, Width, XStart, XEnd, DeltaZ, 
        ZStep, I, IStep, DeltaI;
   long *ZPtr, Z;
   RColor = ( unsigned char ) Color; RColor;
   EdgeCount = SPCount;

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
               if ( Width > 0 )
                  {
                  I = LeftSeg.GetI ();
                  Z = LeftSeg.GetZ ();

                  DeltaZ = ( RightSeg.GetZ () - LeftSeg.GetZ () );
                  DeltaI = ( RightSeg.GetI () - LeftSeg.GetI () );

                  ZStep = DeltaZ / Width;
                  IStep = DeltaI / Width;

                  // Clip the scan-line:
                  ClipHLine ( XStart, XEnd, Z, ZStep, I, IStep );
                  Width = XEnd - XStart;
                  DPtr = &Dest [ YIndex + XStart ];
                  ZPtr = &ZBuffer [ YIndex + XStart ];

                  // Loop for width of scan-line:
                  while ( Width-- > 0 )
                        {
                        if ( *ZPtr < Z )
                           {
                           *ZPtr = Z;
                           *DPtr = ( unsigned char ) ( I >> ISTEP_PREC );
                           }
                        Z += ZStep;
                        I += IStep;
                        ++DPtr;
                        ++ZPtr;
                        }
                  }
               ++RightSeg;
               ++LeftSeg;
               YIndex += 320;
               }
         }
   }
