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

#define XSTEP_PREC 10
#define ZSTEP_PREC 28
#define ISTEP_PREC 18

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
  long X1, X2, Y1, Y2, Z1, Z2;
  long X, StepX, StepZ, Z, I, StepI;
  long EdgeHeight, Y;
  void inline Step ();
  void inline Step ( long Amount );
public:
  CeilLine () { EdgeHeight = 0; }  // The constructor
  void inline Init ( Point2D P1, Point2D P2 ); // The initialize function
  void inline ClipTop ( const int Top );  // Object-precision clip 
                                          // against top of viewport
  void operator ++ () { Step (); }        // Steps polygon edge
  void operator ++ ( int ) { Step (); }
  void operator += ( long Amount ) { Step ( Amount ); } // Steps edge by "Amount"
  Point2D inline operator + ( long Amount );   // Returns a point P + Amount
  long GetX () { return ( X >> XSTEP_PREC ); } // Returns the current X coordinate
  long GetY () { return Y; } // Returns the current Y coordinate
  long GetZ () { return Z; } // Returns the current Z coordinate
  long GetI () { return I; } // Returns the current intensity
  long Height () { return EdgeHeight; } // Returns the current poly height
};

void inline CeilLine::Init ( Point2D P1, Point2D P2 )
   {
   // Calculate initial values for polygon edge:
   long FWidth, DeltaZ, DeltaI;
   X1 = P1.X; X2 = P2.X;
   Y1 = P1.Y; Y2 = P2.Y;
   Z1 = P1.Z; Z2 = P2.Z;

   EdgeHeight = ( Y2 - Y1 );
   FWidth = ( X2 - X1 ) << XSTEP_PREC;
   DeltaZ = ( Z2 - Z1 );
   DeltaI = ( P2.Inten - P1.Inten ) << ISTEP_PREC;
   X = ( X1 << XSTEP_PREC ) + CEIL_FRACT;
   Y = Y1;
   Z = Z1 + ZTrans;
   I = P1.Inten << ISTEP_PREC;
   if ( EdgeHeight )
      {
      StepX = FWidth / EdgeHeight;
      StepZ = DeltaZ / EdgeHeight;
      StepI = DeltaI / EdgeHeight;
      }
   else {
        StepX = 0;
        StepZ = 0;
        StepI = 0;
        }
   }

void inline CeilLine::Step ()
   {
   // Step the edge:
   X += StepX;
   ++Y; Z += StepZ;
   I += StepI;
   --EdgeHeight;
   }

void inline CeilLine::Step ( long Amount )
   {
   // Step the edge by "Amount":
   X += ( StepX * Amount );
   Y += ( Amount );
   Z += ( StepZ * Amount );
   I += ( StepI * Amount );
   EdgeHeight -= ( Amount );
   }

Point2D inline CeilLine::operator + ( long Amount )
   {
   // Return a point P + Amount:
   Point2D Temp;
   Temp.X = X + ( StepX * Amount );
   Temp.Y = Y + ( Amount );
   Temp.Z = Z + ( StepZ * Amount );
   Temp.Inten = I + ( StepI * Amount );
   return Temp;
   }

void CeilLine::ClipTop ( const int Top )
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
      I += StepI * Step;
      EdgeHeight -= Step;
      }
   }

#endif