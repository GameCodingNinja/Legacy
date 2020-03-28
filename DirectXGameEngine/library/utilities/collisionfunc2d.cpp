/************************************************************************
*    FILE NAME:       collisionfunc2d.cpp
*
*    DESCRIPTION:     2D collision functions
************************************************************************/  

// Physical component dependency
#include <utilities/collisionfunc2d.h>

namespace NCollisionFunc2D
{
    /************************************************************************
    *    desc:  Get the squared distance of two points         
    *
    *	 param: CPoint & p0	- point 0
    *			CPoint & p1 - point 1
    *
    *	 ret:	float - squared distance between the two points
    ************************************************************************/
    float SquaredDistance( const CPoint & p0, const CPoint & p1 )
    {
        return ( pow( p0.x - p1.x, 2 ) + pow( p0.y - p1.y, 2 ) );

    }	// SquaredDistance


    /************************************************************************
    *    desc:  Check to see if the two vectors are facing each other         
    *
    *	 param: CPoint & v0	- vector 0
    *			CPoint & v1 - vector 1
    *
    *	 ret:	bool - whether or not the two vectors are facing each other
    ************************************************************************/
    bool VectorsFacing( const CPoint & v0, const CPoint & v1 )
    {
        float value = v0.GetDotProduct( v1 );

        if( !(value > 0) )
            return true;
        
        return false;

    }	// VectorsFacing


    /************************************************************************
    *    desc:  Check if two objects' radii intersect              
    *
    *	 param: CPoint & p0	- the center point of object 0
    *			float r0    - the radius of object 0
    *			CPoint & p1 - the center point of object 1
    *			float r1	- the radius of object 1
    *
    *	 ret:	bool - whether or not the radii intersect
    ************************************************************************/
    bool SquaredRadiiIntersect( const CPoint & p0, float r0, const CPoint & p1, float r1 )
    {
        float distance = SquaredDistance( p0, p1 );

        if( distance <= r0 + r1 )
            return true;

        return false;

    }	// SquaredRadiiIntersect


    /************************************************************************
    *    desc:  Check if two objects' radii intersect                
    *
    *	 param: CPoint & p0	- the center point of object 0
    *			float r0    - the radius of object 0
    *			CPoint & p1 - the center point of object 1
    *			float r1	- the radius of object 1
    *
    *	 ret:	bool - whether or not the radii intersect
    ************************************************************************/
    bool SqrtRadiiIntersect( const CPoint & p0, float r0, const CPoint & p1, float r1 )
    {
        float distance = sqrt( pow( p0.x - p1.x, 2 ) + pow( p0.y - p1.y, 2 ) );

        if( distance <= r0 + r1 )
            return true;

        return false;

    }	// SqrtRadiiIntersect


    /************************************************************************
    *    desc:  Check if two objects' bounding boxes intersect                
    *
    *	 param: CPoint & p0	- the center point of object 0
    *			float r0    - the radius of object 0
    *			CPoint & p1 - the center point of object 1
    *			float r1	- the radius of object 1
    *
    *	 ret:	bool - whether or not the bounding boxes intersect
    ************************************************************************/
    bool BoxRadiiIntersect( const CPoint & p0, float r0, const CPoint & p1, float r1 )
    {
        float distance(0), maxDistance(0);
        maxDistance = r0 + r1;

        // Check the x distance
        distance = abs(p0.x - p1.x);

        if( distance > maxDistance )
            return false;
        
        // Check the y distance. Z distance is ignored.
        distance = abs(p0.y - p1.y);

        if( distance > maxDistance )
            return false;

        return true;

    }	// SqrtRadiiIntersect


    /************************************************************************
    *    desc:  Check if two objects' bounding boxes intersect                
    *
    *	 param: CWorldPoint & p0	- the center point of object 0
    *			float r0			- the radius of object 0
    *			CWorldPoint & p1	- the center point of object 1
    *			float r1			- the radius of object 1
    *
    *	 ret:	bool - whether or not the bounding boxes intersect
    ************************************************************************/
    bool BoxRadiiIntersect( const CWorldPoint & p0, float r0, const CWorldPoint & p1, float r1 )
    {
        float distance(0), maxDistance(0);
        maxDistance = r0 + r1;

        CWorldPoint pointDiffer = p1 - p0;

        // Check the x distance
        distance = abs( pointDiffer.x );

        if( distance > maxDistance )
            return false;
        
        // Check the y distance. Z distance is ignored.
        distance = abs( pointDiffer.y );

        if( distance > maxDistance )
            return false;

        return true;

    }	// SqrtRadiiIntersect


    /************************************************************************
    *    desc:  Check if a point is within a sprite              
    *
    *	 param: CPoint & point - the point to use
    *			float top	   - top side of the rect
    *			float bottom   - bottom side of the rect
    *			float left     - left side of the rect
    *			float right    - right side of the rect
    *
    *	 ret:	bool - whether or not the point is in the rect
    ************************************************************************/
    bool PointInRect( const CPoint & point, float top, float bottom, float left, float right )
    {
        if( point.x > left   && point.x < right && 
            point.y > bottom && point.y < top )
            return true;

        return false;

    }	// PointInRect


    /************************************************************************
    *    desc:  Check if a point is within a sprite              
    *
    *	 param: CPointInt & point - the point to use
    *			int top		      - top side of the rect
    *			int bottom        - bottom side of the rect
    *			int left          - left side of the rect
    *			int right         - right side of the rect
    *
    *	 ret:	bool - whether or not the point is in the rect
    ************************************************************************/
    bool PointInRect( const CPointInt & point, int top, int bottom, int left, int right )
    {
        if( point.x > left   && point.x < right && 
            point.y > bottom && point.y < top )
            return true;

        return false;

    }	// PointInRect

}