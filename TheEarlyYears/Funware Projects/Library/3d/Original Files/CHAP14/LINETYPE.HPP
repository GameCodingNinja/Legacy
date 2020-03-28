//
// File name: LineType.HPP
//
// Description: A fixed-point implementation of a polygon edge 
//             algorithm
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef LINETYPEHPP
#define LINETYPEHPP

const XSTEP_PREC = 10;

// TSTEP_PREC and ZSTEP_PREC must be equal:
const ZSTEP_PREC = 26;
const TSTEP_PREC = ZSTEP_PREC;
const LSTEP_PREC = 16;
const ISTEP_PREC = 8;
const TMASK      = ( 0xFFFFFFFF << LSTEP_PREC );
const IMASK      = ( 0xFFFFFFFF << ISTEP_PREC );

#include <Dos.H>
#include <Math.H>

#include "Point2D.HPP"
#include "3DClass.HPP"


// Ceiling step constant:
const long CEIL_FRACT = ( ( 1 << XSTEP_PREC ) - 1 );

// A class that steps on the ceiling of integers - always steps
// on Y, rendering it useless for anything but polygon
// rasterization:
class CeilLine {
protected:
  long X1, X2, Y1, Y2;
  long X, StepX, StepZ, Z, U, V, StepU, StepV, I, StepI;
  long EdgeHeight, Y;
  void inline Step ();
  void inline Step ( long Amount );
public:
  CeilLine () { EdgeHeight = 0; }  // The constructor
  inline void Init ( Point2D P1, Point2D P2 ); // The initialize function
  inline void ClipTop ( const int Top );  // Object-precision clip 
                                          // against top of viewport
  void operator ++ () { Step (); }        // Steps polygon edge
  void operator ++ ( int ) { Step (); }
  void operator += ( long Amount ) { Step ( Amount ); } // Steps edge by "Amount"
  inline Point2D operator + ( long Amount );   // Returns a point P + Amount
  long GetX () { return ( X >> XSTEP_PREC ); } // Returns the current X coordinate
  long GetY () { return Y; } // Returns the current Y coordinate
  long GetZ () { return Z; } // Returns the current 1/Z coordinate
  long GetU () { return U; } // Returns the current 1/U texture coordinate
  long GetV () { return V; } // Returns the current 1/V texture coordinate
  long GetI () { return I; } // Return the current intensity
  long Height () { return EdgeHeight; } // Returns the current poly height
};

inline void CeilLine::Init ( Point2D P1, Point2D P2 )
   {
   // Calculate initial values for polygon edge:
   long FWidth, DeltaZ, DeltaU, DeltaV, Z1, Z2, DeltaI;
   X1 = P1.X; X2 = P2.X;
   Y1 = P1.Y; Y2 = P2.Y;
   Z1 = P1.Z; Z2 = P2.Z;

   EdgeHeight = ( Y2 - Y1 );
   FWidth = ( X2 - X1 ) << XSTEP_PREC;
   DeltaZ = ( Z2 - Z1 );
   // Calculate U/Z and V/Z deltas:
   DeltaU = ( P2.U - P1.U );
   DeltaV = ( P2.V - P1.V );

   // Calculate intensity delta:
   DeltaI = ( P2.I - P1.I );

   X = ( X1 << XSTEP_PREC ) + CEIL_FRACT;
   Y = Y1;
   Z = Z1 + ZTrans;
   // Store U/Z and V/Z values:
   U = P1.U;
   V = P1.V;
   // Store intensity value:
   I = P1.I;

   if ( EdgeHeight )
      {
      StepX = FWidth / EdgeHeight;
      StepZ = DeltaZ / EdgeHeight;
      StepU = DeltaU / EdgeHeight;
      StepV = DeltaV / EdgeHeight;
      StepI = DeltaI / EdgeHeight;
      }
   else {
        StepX = 0;
        StepZ = 0;
        StepU = 0;
        StepV = 0;
        StepI = 0;
        }
   }

inline void CeilLine::Step ()
   {
   // Step the edge:
   X += StepX;
   ++Y; Z += StepZ;
   U += StepU;
   V += StepV;
   I += StepI;
   --EdgeHeight;
   }

inline void CeilLine::Step ( long Amount )
   {
   // Step the edge by "Amount":
   X += ( StepX * Amount );
   Y += ( Amount );
   Z += ( StepZ * Amount );
   U += ( StepU * Amount );
   V += ( StepV * Amount );
   I += ( StepI * Amount );
   EdgeHeight -= ( Amount );
   }

inline Point2D CeilLine::operator + ( long Amount )
   {
   // Return a point P + Amount:
   Point2D Temp;
   Temp.X = X + ( StepX * Amount );
   Temp.Y = Y + ( Amount );
   Temp.Z = Z + ( StepZ * Amount );
   Temp.U = U + ( StepU * Amount );
   Temp.V = V + ( StepV * Amount );
   Temp.I = I + ( StepI * Amount );
   return Temp;
   }

inline void CeilLine::ClipTop ( const int Top )
   {
   // Clip the polygon edge against the top of the viewport -
   // Note: must be called directly after edge initialization:
   double SlopeX;
   long Step, Height = Y2 - Y1;
   if ( ( Y < Top ) && ( Height ) )
      {
      Step = Top - Y;
      SlopeX = ( double ) ( X2 - X1 ) / ( double ) Height;
      X = ( X1 << XSTEP_PREC ) + 
           ( ( long ) ( SlopeX * ( double ) Step ) << XSTEP_PREC ) + 
          CEIL_FRACT;
      Y = Top;
      Z += StepZ * Step;
      U += StepU * Step;
      V += StepV * Step;
      I += StepI * Step;
      EdgeHeight -= Step;
      }
   }

#endif