//
// File name: PalShade.CPP
//
// Description: The support file for a palette shading class
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#include <Math.H>

#include "PalShade.HPP"

void ShadeDat::GenTable ( double R2, double G2, double B2,
                          RGBQUAD *Pal )
   {
   long Count, N;
   double R, G, B, R1, G1, B1, DeltaR, DeltaG,
          DeltaB, StepR, StepG, StepB;

   for ( Count = 0; Count < COLOR_COUNT; Count++ )
       {
       R1 = Pal [ Count ].rgbRed;
       G1 = Pal [ Count ].rgbGreen;
       B1 = Pal [ Count ].rgbBlue;
       DeltaR = R2 - R1;
       DeltaG = G2 - G1;
       DeltaB = B2 - B1;
       R = R1;
       G = G1;
       B = B1;
       StepR = DeltaR / ( double ) SHADE_COUNT;
       StepG = DeltaG / ( double ) SHADE_COUNT;
       StepB = DeltaB / ( double ) SHADE_COUNT;
       for ( N = 0; N < SHADE_COUNT; N++ )
           {
           Shade [ ( N * COLOR_COUNT ) + Count ] = ( BYTE )
                                 GetColor ( ( long ) R,
                                            ( long ) G,
                                            ( long ) B, Pal );
           R += StepR;
           G += StepG;
           B += StepB;
           }
       }
   }

int ShadeDat::LoadTable ( char *FileName )
  {
  FILE *InFile;
  if ( ( InFile = fopen ( FileName, "rb" ) ) == 0 )
     return 0;
  fread ( &Shade, sizeof Shade, 1, InFile );
  fclose ( InFile );
  return 1;
  }

int ShadeDat::SaveTable ( char *FileName )
  {
  FILE *OutFile;
  if ( ( OutFile = fopen ( FileName, "wb" ) ) == 0 )
     return 0;
  fwrite ( &Shade, sizeof Shade, 1, OutFile );
  fclose ( OutFile );
  return 1;
  }

int GetColor ( int Red, int Green, int Blue, RGBQUAD *Pal )
  {
  double Dist [ COLOR_COUNT ], DRed, DGreen, DBlue, DistC;
  int Count, CloseC;
  for ( Count = 0; Count < COLOR_COUNT; Count++ )
      {
      DRed   = ( Red   - Pal [ Count ].rgbRed );
      DGreen = ( Green - Pal [ Count ].rgbGreen );
      DBlue  = ( Blue  - Pal [ Count ].rgbBlue );
      Dist [ Count ] = sqrt ( DRed   * DRed +
                              DGreen * DGreen +
                              DBlue  * DBlue );
      }
  CloseC = 0;
  DistC  = Dist [ 0 ];
  for ( Count = 0; Count < COLOR_COUNT; Count++ )
      {
      if ( Dist [ Count ] < DistC )
         {
         DistC  = Dist [ Count ];
         CloseC = Count;
         }
      }
  return CloseC;
  }