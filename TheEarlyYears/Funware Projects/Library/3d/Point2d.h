//
// File name: Point2D.h
//
// Description: The header file a 2D point structure
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef POINT2DHPP
#define POINT2DHPP

struct Point2D
{
    int X, Y;   // The screen X and Y of point
    int Z;      // The 1/Z value
    int U, V;   // The texture's position
    int I;      // The texture's intensity
    Point2D(){ X = Y = Z = U = V = I = 0; }
    int operator == ( Point2D &V )
    {
        return ( ( X == V.X ) && ( Y == V.Y ) );
    }
};

int UniqueVert( Point2D &V, Point2D *List, int Range );
int GetVertIndex( Point2D &V, Point2D *List, int Range );

#endif