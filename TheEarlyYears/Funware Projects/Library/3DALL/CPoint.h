
/************************************************************************
*    FILE NAME:       CPoint.h
*
*    DESCRIPTION:     3D Point and uv and rgb classe
*
************************************************************************/  

#ifndef __CPOINT_H__
#define __CPOINT_H__


/************************************************************************
*
*    Holds the 2D point values. The computer screen is a 2D surface so
*    the above classes translated 3D points (tx,ty,tz) calculated to a
*    two dimensional point for pixel plotting. This is also known as
*    projection.
*
************************************************************************/

class CPoint2D
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
    CPoint2D():x(0),y(0),z(0),U(0),V(0),r(0),g(0),b(0)
    {}
};


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
    bool operator == ( CPoint &V );
	// The inequality operator
	bool operator != ( CPoint &V );
};

// Does point = point?
bool CPoint::operator == ( CPoint &V )
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

// The inequality operator
bool CPoint::operator != ( CPoint &V )
{
  if( ( V.x != x ) || ( V.y != y ) || ( V.z != z ) )
  {
	return true;
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


/************************************************************************
*
*    Class for specifing the vertex buffer
*
************************************************************************/

class CDirectXVert
{
public:
    // vertices
    float x, y, z;

    // light for this vertex
    unsigned int color;

    // U, V
    float u, v;
};


#endif  // __CPOINT_H__