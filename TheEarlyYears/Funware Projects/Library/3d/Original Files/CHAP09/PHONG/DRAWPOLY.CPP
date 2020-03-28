//
//
//
//
//
//

#include <Mem.h>
#include <Dos.h>
#include <Math.h>

#include <Conio.h>
#include <IOStream.H>

#include "LineType.HPP"

LightSource Light = { 0.0F, 0.0F, -10.0F };

inline unsigned char CalcColor ( double Nx, double Ny, double Nz )
   {
   unsigned char Color;
   double Mag, CosA;
   Mag = sqrt ( Nx * Nx +
                Ny * Ny +
                Nz * Nz );
   CosA = ( Light.X * Nx + Light.Y * Ny + Light.Z * Nz ) / Mag;
   Color = CosA * 120.0F + 125.0F;

   return Color;
   }

void DrawPoly ( Point2D *SPoint, Point3D *List3D, int SPCount, unsigned char *Dest )
   {
      // Display the panel in the screen buffer "Dest":
   unsigned char RColor, *DPtr;
   CeilLine LeftSeg, RightSeg;
   long Top = 0, N, RightPos, LeftPos, NewRightPos, NewLeftPos, 
        Height, EdgeCount, YIndex, Width, XStart, XEnd;
   double Nx, Ny, Nz, StepNx, StepNy, StepNz;
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
            RightSeg.Init ( SPoint [ RightPos ], List3D [ RightPos ],
                            SPoint [ NewRightPos ], List3D [ NewRightPos ] );
            RightPos = NewRightPos;
            --EdgeCount;
            }
         // Determine if the left side of the polygon needs 
         // (re)initializing:
         if ( LeftSeg.Height () <= 0 )
            {
            NewLeftPos = LeftPos - 1;
            if ( NewLeftPos < 0 )
               NewLeftPos = ( SPCount - 1 );
            LeftSeg.Init ( SPoint [ LeftPos ], List3D [ LeftPos ],
                           SPoint [ NewLeftPos ], List3D [ NewLeftPos ] );
            LeftPos = NewLeftPos;
            --EdgeCount;
            }
         // Subdivide polygon into trapezoid:
         if ( LeftSeg.Height () < RightSeg.Height () )
            {
            Height = LeftSeg.Height ();
            }
         else {
              Height = RightSeg.Height ();
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
                  Nx = LeftSeg.GetNx ();
                  Ny = LeftSeg.GetNy ();
                  Nz = LeftSeg.GetNz ();

                  StepNx = ( RightSeg.GetNx () - LeftSeg.GetNx ())  /
                           ( double ) Width;
                  StepNy = ( RightSeg.GetNy () - LeftSeg.GetNy () ) /
                           ( double ) Width;
                  StepNz = ( RightSeg.GetNz () - LeftSeg.GetNz () ) /
                           ( double ) Width;

                  DPtr = &Dest [ YIndex + XStart ];

                  // Loop for width of scan-line:
                  while ( Width-- > 0 )
                        {
                        RColor = CalcColor ( Nx, Ny, Nz );
                        *DPtr = ( unsigned char ) RColor;
                        ++DPtr;
                        Nx += StepNx;
                        Ny += StepNy;
                        Nz += StepNz;
                        }
                  }
               ++RightSeg;
               ++LeftSeg;
               YIndex += 320;
               }
         }
   }