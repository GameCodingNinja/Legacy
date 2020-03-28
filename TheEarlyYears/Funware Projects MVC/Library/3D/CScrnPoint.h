
/************************************************************************
*    FILE NAME:       CScrnPoint.h
*
*    DESCRIPTION:     2D screen point for software rendering
*
************************************************************************/  

#ifndef __CSCRNPOINT_H__
#define __CSCRNPOINT_H__

#include <math.h>

/************************************************************************
*    Holds the 2D point values software rendering. The computer screen is 
*    a 2D surface so the above classes translated 3D points (tx,ty,tz) 
*    calculated to a two dimensional point for pixel plotting. This is 
*    also known as projection.
************************************************************************/

class CScrnPoint
{
    public:

    // Projected screen point values
    // The screen X and Y of point
    int x, y;
    // The 1/Z fixed point value
    int z;
    // The texture's position 1/Z fixed point value 
    int U, V;
    // The texture's intensity 1/Z fixed point value
    int r, g, b;

    // Fast init if class varaibles.
    CScrnPoint():x(0),y(0),z(0),U(0),V(0),r(0),g(0),b(0)
    {}
};


#endif  // __CSCRNPOINT_H__