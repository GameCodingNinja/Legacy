//
// File name: Point2D.HPP
//
// Description: The header file a 2D point structure
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef POINT2DHPP
#define POINT2DHPP

struct Point2D {
  long X, Y;   // The screen X and Y of point
  long Z;      // The 1/Z value
  long U, V;   // The texture's position
  long I;      // The texture's intensity
  int operator == ( Point2D &V )
    {
    return ( ( X == V.X ) && ( Y == V.Y ) );
    }
};

int UniqueVert ( Point2D &V, Point2D *List, int Range );
unsigned int GetVertIndex ( Point2D &V, Point2D *List,
                            unsigned int Range );

#endif