/************************************************************************
*    FILE NAME:       face.cpp
*
*    DESCRIPTION:     3D face class
************************************************************************/

// Physical component dependency
#include <common/face.h>

// Standard lib dependencies
#include <math.h>
#include <assert.h>


/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CFace::CFace()
     : radius(0.0f)
{
}   // Constructor


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CFace::~CFace()
{

}   // Destructor


/************************************************************************
*    desc:  Calculate the cross product that is then
*           normalized. This is the normal of the polygon face.
*           This function assumes all vertex points are unique.
*           Also, this calculation of the cross product produces
*           a normal with more precesion because it uses all 3 points
*           on the triangle surface.
************************************************************************/
void CFace::CalcSurfaceNormal()
{
    double X1( vert[ 0 ].x );
    double Y1( vert[ 0 ].y );
    double Z1( vert[ 0 ].z );

    double X2( vert[ 1 ].x );
    double Y2( vert[ 1 ].y );
    double Z2( vert[ 1 ].z );

    double X3( vert[ 2 ].x );
    double Y3( vert[ 2 ].y );
    double Z3( vert[ 2 ].z );

    // Calculate the cross product using all three points on the triangle
    double x( Y1 * ( Z2 - Z3 ) + Y2 * ( Z3 - Z1 ) + Y3 * ( Z1 - Z2 ) );
    double y( Z1 * ( X2 - X3 ) + Z2 * ( X3 - X1 ) + Z3 * ( X1 - X2 ) );
    double z( X1 * ( Y2 - Y3 ) + X2 * ( Y3 - Y1 ) + X3 * ( Y1 - Y2 ) );

    // this is to show how you can create the cross product with just two
    // points from the triangle face. Still works but not as precise.
    // May cause problems.
    //double testx = (Y1 * Z2) - (Y2 * Z1);
    //double testy = (Z1 * X2) - (Z2 * X1);
    //double testz = (X1 * Y2) - (X2 * Y1);

    // Calculate the length of the vector
    double length( sqrt( (x*x) + (y*y) + (z*z) ) );

    // length can't be zero otherwise you'll get a divided by 0 error.

    // The normalizing part is the (n.x / length)
    if( length != 0.0 )
    {
        // Normalize the normal and make it a point. Sounds silly hu?
        normal.x = (float)( x / length );
        normal.y = (float)( y / length );
        normal.z = (float)( z / length );
    }
    else
    {
        assert(!("Zero length vector!"));
    }

    length = 0;

}   // CalcSurfaceNormal


/************************************************************************
*    desc:  Calculate the center point of the polygon
************************************************************************/
void CFace::CalcPolyCenterPoint()
{
    // Average out each vertex
    for( int i = 0; i < 3; ++i )
    {
        center += vert[i];
    }

    center.x /= 3;
    center.y /= 3;
    center.z /= 3;

}	// CalcPolyCenterPoint


/************************************************************************
*    desc:  Calculate a bounding sphere around the polygon face
************************************************************************/
void CFace::CalcBoundingPoly()
{
    double curDistance;
    double maxDistance = 0.0;

    // Get the center point of the untransformed polygon
    CalcPolyCenterPoint();
    
    // Find the largest distance
    for( int i = 0; i < 3; ++i )
    {
        // Calculate a new position where the center is 0,0,0
        CPoint pt = vert[i] - center;
        
        // Get the length of this point
        curDistance = pt.GetLengthSquared();

        // Have we found the longest distance?
        if( curDistance > maxDistance )
            maxDistance = curDistance;
    }

    // compute the radius by getting the vector length
    radius = (float)maxDistance;

}   // CalcBoundingPoly


/************************************************************************
*	 desc:  Get the value distance between the given point and plane of
*			the face.
*
*	 param: CPoint & point - point to test against
*
*    ret: float - distance
************************************************************************/
float CFace::PointToPlaneDistance( const CPoint & point )
{
    // The dot product of the normal against the subtracted points is
    // equal to the equation of a plane. And because the normal of the
    // plane is normalized, this equals the distance between the point
    // and plane.
    return -normal.GetDotProduct( vert[0] - point );

}	// PointToPlaneDistance


/************************************************************************
*    desc:  Is this point facing the front of the plane
*
*	 param: CPoint & point - point to test against
*
*    ret: bool - true if facing plane
************************************************************************/
bool CFace::IsFacingPlane( const CPoint & point )
{
    if( PointToPlaneDistance( point ) < 0.0f )
    {
        return false;
    }
    else
    {
        return true;
    }

}	// IsFacingPlane


/************************************************************************
*	 desc:  Check if a given point is within a floor face.
*
*	 param: CPoint & point - point to test against
*
*    ret: bool
************************************************************************/
bool CFace::PointInFloor( const CPoint & point )
{
    // Set up the three vectors used to find u and v.
    CPoint w2 = point - vert[0];
    CPoint w0 = vert[1] - vert[0];
    CPoint w1 = vert[2] - vert[0];

    // Set the vector y components to zero because this is collision against
    // the floor.
    w0.y = 0;
    w1.y = 0;
    w2.y = 0;

    // Perform dot product equations before calculating u and v because
    // some dot products are used more than once.
    float w0w0 = w0.GetDotProduct(w0);
    float w1w1 = w1.GetDotProduct(w1);
    float w2w2 = w2.GetDotProduct(w2);
    float w0w1 = w0.GetDotProduct(w1);
    float w0w2 = w0.GetDotProduct(w2);
    float w1w2 = w1.GetDotProduct(w2);

    // Find u and v.
    float u = ( ( w1w1 * w0w2 ) - ( w0w1 * w1w2 ) ) / 
              ( ( w0w0 * w1w1 ) - ( w0w1 * w0w1 ) );

    float v = ( ( w0w0 * w1w2 ) - ( w0w1 * w0w2 ) ) /
              ( ( w0w0 * w1w1 ) - ( w0w1 * w0w1 ) );

    // If u or v is greather than 1 or less than 0, the point is outside 
    // of the triangle. Also if u + v is greater than 1, the point is outside 
    // of the triangle. If the point is ouside of the triangle, we return
    // false.
    if( u > 1 || u < 0 )
    {
        return false;
    }
    else if( v > 1 || v < 0 )
    {
        return false;
    }
    else if( v + u > 1 )
    {
        return false;
    }
    else
    {
        return true;
    }

}	// PointInFloor


/************************************************************************
*	 desc:  Check if a given point is within a wall face.
*
*	 param: CPoint & point - point to test against
*
*    ret: bool
************************************************************************/
bool CFace::PointInWall( const CPoint & point )
{
    // Set up the three vectors used to find u and v.
    CPoint w2 = point - vert[0];
    CPoint w0 = vert[1] - vert[0];
    CPoint w1 = vert[2] - vert[0];

    // Perform dot product equations before calculating u and v because
    // some dot products are used more than once.
    float w0w0 = w0.GetDotProduct(w0);
    float w1w1 = w1.GetDotProduct(w1);
    float w2w2 = w2.GetDotProduct(w2);
    float w0w1 = w0.GetDotProduct(w1);
    float w0w2 = w0.GetDotProduct(w2);
    float w1w2 = w1.GetDotProduct(w2);

    // Find u and v.
    float u = ( ( w1w1 * w0w2 ) - ( w0w1 * w1w2 ) ) / 
              ( ( w0w0 * w1w1 ) - ( w0w1 * w0w1 ) );

    float v = ( ( w0w0 * w1w2 ) - ( w0w1 * w0w2 ) ) /
              ( ( w0w0 * w1w1 ) - ( w0w1 * w0w1 ) );

    // If u or v is greather than 1 or less than 0, the point is outside 
    // of the triangle. Also if u + v is greater than 1, the point is outside 
    // of the triangle. If the point is ouside of the triangle, we return
    // false.
    if( u > 1 || u < 0 )
    {
        return false;
    }
    else if( v > 1 || v < 0 )
    {
        return false;
    }
    else if( v + u > 1 )
    {
        return false;
    }
    else
    {
        return true;
    }

}	// PointInWall