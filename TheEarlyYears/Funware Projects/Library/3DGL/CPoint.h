
/************************************************************************
*
*    PRODUCT:         3D Point Class
*
*    FILE NAME:       CPoint.h
*
*    DESCRIPTION:     3D Point and uv and rgb classe
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/  

#ifndef __CPOINT_H__
#define __CPOINT_H__


#define TRI                    3
#define QUAD                   4
#define FORMAT_SPEC_STR_COUNT  20


/************************************************************************
*
*    Holds the 3D point values. The orininal values from data loaded from
*    a file or otherwise, also known as "Local Coordinates" (x,y,z).
*    Translated points (tx,ty,tz) also known as "World Coordinates". The
*    translated points come from translating and rotating (x,y,z).
*
************************************************************************/

class CPoint
{
    public:

    // Original point values. These are the values
    // that are used for translation and rotation.
    float x, y, z;

    // Translated versions of the original values.
    float tx, ty, tz;

    // Fast init if class varaibles.
    CPoint():x(0),y(0),z(0),tx(0),ty(0),tz(0)
    {}

    // Does point = point?
    bool operator == ( const CPoint &V );
};

// Does point = point?
bool CPoint::operator == ( const CPoint &V )
{
    if( V.x == x )
    {
        if( V.y == y )
        {
            if( V.z == z )
            {
                return true;
            }
        }
    }
    return false;
}


/************************************************************************
*
*    Simple class for holding u, v, r, g, b and normal
*
************************************************************************/

class CUVRGB
{
public:
    // UV coordinates and RGB color values for each vertex
    float u, v, r, g, b;

    // light normal for this vertex
    CPoint n;

    CUVRGB():u(0),v(0),r(1),g(1),b(1)
    {}
};


#endif  // __CPOINT_H__