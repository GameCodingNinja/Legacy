//
// File name: PhongDM.CPP
//
// Description: Part of the Phong shading demo
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#include <DOS.H>
#include <Bios.H>
#include <Time.H>
#include <Conio.H>
#include <Stdlib.H>
#include <Iostream.H>

#include "32Bit.HPP"
#include "Palette.HPP"
#include "DrawPoly.HPP"

const int KEY_WAITING = 1;
const double MOVE = 0.03125;

void ProjectPoly ( Point2D *Poly2D, Point3D *Poly3D, int NP )
   {
   for ( short int N = 0; N < NP; N++ )
       {
       Poly2D [ N ].X = Poly3D [ N ].Wx * 120.0F / 
                        Poly3D [ N ].Wz + 160.0F;

       Poly2D [ N ].Y = Poly3D [ N ].Wy * -120.0F /
                        Poly3D [ N ].Wz + 100.0F;
       }
   }

void inline ShowPoly ( Point3D *Poly3D, 
                       int NP, unsigned char *Dest )
   {
   Point2D Poly2D [ 10 ];
   ProjectPoly ( Poly2D, Poly3D, NP );
   DrawPoly ( Poly2D, Poly3D, NP, Dest );
   }

void NormLight ()
   {
   double Mag;
   Mag = sqrt ( Light.X * Light.X +
                Light.Y * Light.Y +
                Light.Z * Light.Z );
   Light.X /= Mag;
   Light.Y /= Mag;
   Light.Z /= Mag;
   }

void SetPalette ()
   {
   short int Inten;
   for ( short int N = 0; N < 256; N++ )
       {
       Inten = ( short ) ( ( short ) N >> ( short ) 2 );
       setpalreg ( N, Inten, Inten, Inten );
       }
   }

void main ()
   {
   unsigned char *VidMem = VideoAddress ();
   long StartTime, EndTime, TotalTime, Ticks = 0;
   unsigned char Input = 0;
   unsigned char *Buffer = new unsigned char [ 64000 ];
   Point3D Poly1 [] = { { 0,     100, 150,   // The first 
                                             // vertex

                             0,  100, -50 }, // The first 
                                             // vertex' normal

                        { 100,  -100, 150,   // The second 
                                             // vertex

                            100, -100, -50 },// The second 
                                             // vertex' normal

                        { -100, -100, 150,   // The third
                                             // vertex

                         -100, -100, -50 } };// The third
                                             // vertex' normal

   // Normalize the light source vector to 1:
   NormLight ();

   // Set the video mode:
   SetVideo ( 0x13 );

   // Set the palette to shades of gray:
   SetPalette ();

   StartTime = clock ();
   while ( Input != 27 )
         {
         if ( _bios_keybrd ( _KEYBRD_READY ) )
            {
            Input = ( unsigned char ) _bios_keybrd ( _KEYBRD_READ );
            if ( ( Input == 'r' ) || ( Input == 'R' ) )
               {
               Light.X += MOVE;
               NormLight ();
               }
            else if ( ( Input == 'l' ) || ( Input == 'L' ) )
                    {
                    Light.X -= MOVE;
                    NormLight ();
                    }
            else if ( ( Input == 'u' ) || ( Input == 'U' ) )
                    {
                    Light.Y += MOVE;
                    NormLight ();
                    }
            else if ( ( Input == 'd' ) || ( Input == 'D' ) )
                    {
                    Light.Y -= MOVE;
                    NormLight ();
                    }
            }
         setmem ( Buffer, 64000, 0 );
         ShowPoly ( Poly1, 3, Buffer );
         memmove ( VidMem, Buffer, 64000 );
         ++Ticks;
         }
   EndTime = clock ();

   SetVideo ( 0x03 );
   TotalTime = ( double ) ( EndTime - StartTime ) / ( double ) CLK_TCK;
   cout << "Polygons per second = " 
        << ( 1.0F / ( ( double ) TotalTime / ( double ) Ticks ) );
   delete [] Buffer;
   }