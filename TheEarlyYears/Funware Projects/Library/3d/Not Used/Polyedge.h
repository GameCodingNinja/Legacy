//
// File name: PolyEdge.h
//
// Description: Header file for a polygon edge class
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//
/*
#ifndef POINT2DHPP
  #define POINT2DHPP
  #include "Point2D.h"
#endif

class PolyEdge {
protected:
long X, Y, StepX, StepY, Numerator, Denominator, ErrorTerm;
long EdgeHeight;
long Zr, StepZ;
public:
void inline Step ();
PolyEdge () { }
PolyEdge ( Point2D &P1, Point2D &P2 )
   {
   Initialize ( P1, P2 );
   }
void Initialize ( Point2D &P1, Point2D &P2 );
long Height () { return EdgeHeight; }
long GetX () { return X; }
long GetY () { return Y; }
long GetZr () { return Zr; }
long GetStepY() { return StepY; }
void operator ++ () { Step (); }
void operator ++ (int) { Step (); }
void operator += ( int Amount );
};

// A structure representing a screen edge:
struct EdgeScreen {
long X;
long Z;
} Left [ 200 ], Right [ 200 ];  */