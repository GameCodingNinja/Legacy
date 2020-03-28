//
// File name: 3Dclass.CPP
//
// Description: The support file for the 3DClass header
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

// ------------------------------------------------------------
// | Global headers:                                          |
// ------------------------------------------------------------

#include <Math.h>

// ------------------------------------------------------------
// | Local headers:                                           |
// ------------------------------------------------------------

#include "3DClass.HPP"

// ------------------------------------------------------------
// | Global variables/constants:                              |
// ------------------------------------------------------------

float CosTable [ DEGREECOUNT ];
float SinTable [ DEGREECOUNT ];

long *ZBuffer;
long ZTrans;

// ------------------------------------------------------------
// | Function section:                                        |
// ------------------------------------------------------------

// Initiate math function - calculates trig tables:
void InitMath ()
   {
   long double Unit = ( long double ) ( PI * 2.0F ) /
                      ( long double ) DEGREECOUNT;
   // Loop through all DEGREECOUNT degrees:
   for ( unsigned short N = 0; N < DEGREECOUNT; N++ )
       {
       long double Degree = ( long double ) N;
		 CosTable [ N ] = float ( cos ( Unit * Degree ) );
		 SinTable [ N ] = float ( sin ( Unit * Degree ) );
       }
   }


