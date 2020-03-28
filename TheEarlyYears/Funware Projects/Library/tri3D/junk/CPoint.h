
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

#define CEIL_FRACT			 0.9999999999999999999999999999999

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
    int x, y;     // The screen X and Y of point
    float z;      // The 1/Z value
    float U, V;   // The texture's position	1/Z value
    float I;      // The texture's intensity

	// Fast init if class varaibles.
	CPoint2D():x(0),y(0),z(0),U(0),V(0),I(0)
	{};
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
	// value of the 1/z
	float oneOverZ;

	// Flag indicating point has been projected
	bool fProjected;

    // Fast init if class varaibles.
    CPoint3D():x(0),y(0),z(0),tx(0),ty(0),tz(0),px(0),py(0),oneOverZ(0),fProjected(false)
    {};

    // The equality operator:
    inline bool operator == ( CPoint3D &V );
};

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


/************************************************************************
*
*    CeilLine class for plotting your way down the edge of the triangle
*
************************************************************************/

class CCeilLine
{
public:
    int Y, x1, x2, y1, y2;
	float X, Z, I;
    float stepX, stepZ, stepI;
    int edgeHeight, U, V, stepU, stepV;

    // The constructor
    CCeilLine():x1(0), x2(0), y1(0), y2(0), edgeHeight(0), X(0), Y(0), Z(0),
    I(0), U(0), V(0), stepX(0), stepZ(0), stepI(0), stepU(0), stepV(0)
    {
    }

	// Init the calss with the two points that form a line down the triangle
	inline void Init( CPoint2D &point2D_rightPos, CPoint2D &point2D_newRightPos );
    // Perform object-precision clip on top of edge. 
    // Is the top edge of the polygon above the Y of the screen?
    // If so, recalculate the X offset and set the Y to 0
	inline void ClipTop();
};

// Init the class with the two points that form a line down the triangle
inline void CCeilLine::Init( CPoint2D &point2D_Pos, CPoint2D &point2D_newPos )
{
    // The two points that makeup a line
    x1 = point2D_Pos.x;
    x2 = point2D_newPos.x;
    y1 = point2D_Pos.y;
    y2 = point2D_newPos.y;

    // Height of the two points
    edgeHeight = ( y2 - y1 );

    // Push the X to the Ceiling of the number so that
    // the number isn't truncated.
    X = (float)x1 + CEIL_FRACT;
    Y = y1;
    Z = point2D_Pos.z;

    // Store U/Z and V/Z values:
    U = point2D_Pos.U;
    V = point2D_Pos.V;

    // Store intensity value:
    I = point2D_Pos.I;

    if( edgeHeight )
    {
        // Create the step amount
        stepX = (float)( x2 - x1 ) / edgeHeight;
        stepZ = ( point2D_newPos.z - point2D_Pos.z ) / edgeHeight;

        // Calculate U/Z and V/Z deltas:
        stepU = ( point2D_newPos.U - point2D_Pos.U ) / edgeHeight;
        stepV = ( point2D_newPos.V - point2D_Pos.V ) / edgeHeight;

        // Calculate intensity delta                
        stepI = ( point2D_newPos.I - point2D_Pos.I ) / edgeHeight;
    }
    else
    {
        stepX = 0;
        stepZ = 0;
        stepU = 0;
        stepV = 0;
        stepI = 0;
    }
}	// init


// Perform object-precision clip on top of edge. 
// Is the top edge of the polygon above the Y of the screen?
// If so, recalculate the X offset and set the Y to 0
inline void CCeilLine::ClipTop()
{
    // Save the offset of the y that is off screen
    float step( -Y );

    // calculate a new x offset based on the y being zero
    X = (( double )x1 +
        ((( double )( x2 - x1 ) /
        ( double )(y2 - y1)) *
        ( double ) step )) + CEIL_FRACT;

    // Set to zero
    Y = 0;
    Z += stepZ * step;
    U += stepU * step;
    V += stepV * step;
    I += stepI * step;

    // Take the current edge height and subtract the
    // original offscreen y offset
    edgeHeight -= step;

}	// ClipTop


/************************************************************************
*
*    Simple class for holding U, V, I
*
************************************************************************/

class CUVI
{
public:
	float U, V, I;

	CUVI():U(0),V(0),I(0)
	{}
};


/************************************************************************
*
*    Class for holding specific behavior information about the triangle
*
************************************************************************/

class CBehavior
{
public:
	CUVI uvi[3];
};


#endif  // __CPOINT_H__