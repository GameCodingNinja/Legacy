
/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CPoint.h
*
*    DESCRIPTION:     3D and 2D Point classes
*
*    IMPLEMENTATION:  For these classes, we are using a class more so
*                     as a container and not so much for following good
*                     object design. In 3D, speed is everything. We need
*                     fast access to all data which is why everything
*                     is made public.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
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
	// value of 1/z
	float oneOverZ;
	// p is the calculated percentage of the lines length
	double p; 
	// UVI index of start and end
	int startI, endI;
	// Clipped flag
	bool fClipped;
	// The texture's position 1/Z fixed point value 
    int U, V;
	// The texture's intensity 1/Z fixed point value
    int I;

	// Fast init if class varaibles.
	CPoint2D():x(0),y(0),z(0),U(0),V(0),I(0),
	           oneOverZ(0),startI(0),endI(0),fClipped(false)
	{}
};


/************************************************************************
*
*    Holds the 3D point values. The orininal values from data loaded from
*    a file or otherwise, also known as "Local Coordinates" (x,y,z).
*    Translated points (tx,ty,tz) also known as "World Coordinates". The
*    translated points come from translating and rotating (x,y,z).
*
*    So in most cases, values loaded in (x,y,z) never change and the values
*    used to plot a point in space is the (tx,ty,tz).
*
************************************************************************/

class CPoint3D
{
    public:

    // Original point values. These are the values
    // that are used for translation and rotation.
    float x, y, z;
    // Translated versions of the original values.
    float tx, ty, tz;
	// Projected point
	int px, py;
	// value of 1/z
	float oneOverZ;
	// value of the 1/z	32 bit fix point
	int oneOverZfixPoint32bit;

    // Fast init if class varaibles.
    CPoint3D():x(0),y(0),z(0),tx(0),ty(0),tz(0),px(0),py(0),
              oneOverZ(0),oneOverZfixPoint32bit(0)
    {}

	// Function section:
	inline bool CPoint3D::operator == ( CPoint3D &V )
	{
	  bool RValue = false;
	  if( V.x == x )
	     if( V.y == y )
	        if( V.z == z )
	           RValue = true;
	  return RValue;
	}
};


/************************************************************************
*
*    Simple point class
*
************************************************************************/

class CPoint
{
    public:

    float x, y, z;
	float tx, ty, tz;

	CPoint():x(0),y(0),z(0),tx(0),ty(0),tz(0)
	{}
};


/************************************************************************
*
*    Simple class for holding U, V, I
*
************************************************************************/

class CUVI
{
public:
	float u, v;
	int i;

    // light normal for this vertex
	CPoint n;

	CUVI():u(0),v(0),i(0)
	{}
};


#endif  // __CPOINT_H__