
/************************************************************************
*    FILE NAME:       CPoint.h
*
*    DESCRIPTION:     3D Point and uv and rgb classe
*
************************************************************************/  

#ifndef __CPOINT_H__
#define __CPOINT_H__

#include <math.h>
#include <vector>


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

    // Copy constructor
    CPoint( CPoint &point ):x(point.x),y(point.y),z(point.z),tx(point.tx),ty(point.ty),tz(point.tz)
    {}

    // Copy constructor
    CPoint( float _x, float _y, float _z ):x(_x),y(_y),z(_z)
    {}

    /************************************************************************                                                             
    *    DESCRIPTION:  The equality operator 
    *
    *    Input:  CPoint &point - point to check
    *
    *    Output: bool - true or false
    ************************************************************************/
    bool operator == ( CPoint &point )
    {
        if( point.x == x )
        {
            if( point.y == y )
            {
                if( point.z == z )
                {
                    return true;
                }
            }
        }
        return false;

    }	// operator ==


    /************************************************************************                                                             
    *    DESCRIPTION:  The inequality operator 
    *
    *    Input:  CPoint &point - point to check
    *
    *    Output: bool - true or false
    ************************************************************************/
    bool operator != ( CPoint &point )
    {
        if( ( point.x != x ) || ( point.y != y ) || ( point.z != z ) )
        {
            return true;
        }
        return false;

    }	// operator !=


	/************************************************************************                                                             
    *    DESCRIPTION:  The subtraction operator 
    *
    *    Input:  CPoint &point - point to subtract
    *
    *    Output: CPoint - subtracted point
    ************************************************************************/
	CPoint operator - ( CPoint &point )
  	{
  		CPoint tmp;
  		tmp.x = x - point.x;
  		tmp.y = y - point.y;
  		tmp.z = z - point.z;

  		return tmp;

	}	// operator -


	/************************************************************************                                                             
    *    DESCRIPTION:  The addition operator 
    *
    *    Input:  CPoint &point - point to add
    *
    *    Output: CPoint - added point
    ************************************************************************/
	CPoint operator + ( CPoint &point )
  	{
  		CPoint tmp;
  		tmp.x = x + point.x;
  		tmp.y = y + point.y;
  		tmp.z = z + point.z;

  		return tmp;

	}	// operator +

    /************************************************************************                                                             
    *    DESCRIPTION:  The addition operator 
    *
    *    Input:  CPoint &point - point to add
    *
    *    Output: CPoint - added point
    ************************************************************************/
	CPoint operator += ( CPoint &point )
  	{
  		x += point.x;
  		y += point.y;
  		z += point.z;

		return *this;

	}	// operator +=

	/************************************************************************                                                             
    *    DESCRIPTION:  The multiplication operator 
    *
    *    Input:  CPoint &point * point to add
    *
    *    Output: CPoint - added point
    ************************************************************************/
	CPoint operator * ( CPoint &point )
  	{
  		CPoint tmp;
  		tmp.x = x * point.x;
  		tmp.y = y * point.y;
  		tmp.z = z * point.z;

  		return tmp;

	}	// operator *

    /************************************************************************                                                             
    *    DESCRIPTION:  The multiplication operator 
    *
    *    Input:  CPoint &point * point to add
    *
    *    Output: CPoint - added point
    ************************************************************************/
	CPoint operator *= ( CPoint &point )
  	{
  		x *= point.x;
  		y *= point.y;
  		z *= point.z;

		return *this;

	}	// operator *=

	/************************************************************************                                                             
    *    DESCRIPTION:  Copy point data
    ************************************************************************/
    void Init( std::vector<float> &floats )
    {
		x = tx = floats[0];
	    y = ty = floats[1];
	    z = tz = floats[2];

    }   // Init

    /************************************************************************                                                             
    *    DESCRIPTION:  Copy point data
    ************************************************************************/
    void Init( float _x, float _y, float _z )
    {
		x = tx = _x;
	    y = ty = _y;
	    z = tz = _z;

    }   // Init

    /************************************************************************                                                             
    *    DESCRIPTION:  Copy point data
    ************************************************************************/
    void Init( CPoint &point )
    {
		x = point.x;
	    y = point.y;
	    z = point.z;

		tx = point.tx;
		ty = point.ty;
		tz = point.tz;

    }   // Init

	/************************************************************************                                                             
    *    DESCRIPTION:  Copy transformed data to make perminate
    ************************************************************************/
    void MakePermanent()
    {
		x = tx;
	    y = ty;
	    z = tz;

    }   // MakePerminant

	/************************************************************************                                                             
    *    DESCRIPTION:  set all to 0.0
    ************************************************************************/
    void Reset()
    {
		x = tx = 0.0f;
	    y = ty = 0.0f;
	    z = tz = 0.0f;

    }   // Reset

    /************************************************************************                                                             
    *    DESCRIPTION:  Get the length between two untransformed points 
    *
    *    Input:  CPoint &point1 - first point
    *            CPoint &point2 - second point
    *
    *    Output: float - distance between two points
    ************************************************************************/
    float _fastcall GetLengthSt( CPoint &point )
    {
        // Calculate the distance between the center points of both objects
        return ( ((x - point.x) * (x - point.x)) +
                 ((y - point.y) * (y - point.y)) +
                 ((z - point.z) * (z - point.z)) );

    }   // GetLengthSt

    /************************************************************************                                                             
    *    DESCRIPTION:  Get the length between two transformed points 
    *
    *    Input:  CPoint &point1 - first point
    *            CPoint &point2 - second point
    *
    *    Output: float - distance between two points
    ************************************************************************/
    float _fastcall GetLengthDy( CPoint &point )
    {
        // Calculate the distance between the center points of both objects
        return ( (( tx - point.tx ) * ( tx - point.tx )) +
                 (( ty - point.ty ) * ( ty - point.ty )) +
                 (( tz - point.tz ) * ( tz - point.tz )) );

    }   // GetLengthDy

    /************************************************************************                                                             
    *    DESCRIPTION:  Get the length between two untransformed points X & Z
    *
    *    Input:  CPoint &point1 - first point
    *            CPoint &point2 - second point
    *
    *    Output: float - distance between two points
    ************************************************************************/
    float _fastcall GetLengthXZSt( CPoint &point )
    {
        // Calculate the distance between the center points of both objects
        return ( (( x - point.x ) * ( x - point.x )) +
                 (( z - point.z ) * ( z - point.z )) );

    }   // GetLengthSt

	/************************************************************************                                                             
    *    DESCRIPTION:  Get the length between two transformed points X & Z
    *
    *    Input:  CPoint &point1 - first point
    *            CPoint &point2 - second point
    *
    *    Output: float - distance between two points
    ************************************************************************/
    float _fastcall GetLengthXZDy( CPoint &point )
    {
        // Calculate the distance between the center points of both objects
        return ( (( tx - point.tx ) * ( tx - point.tx )) +
                 (( tz - point.tz ) * ( tz - point.tz )) );

    }   // GetLengthDy

    /************************************************************************                                                             
    *    DESCRIPTION:  Get the length of the points 
    *
    *    Output: float - length of point
    ************************************************************************/
    float GetLengthSt()
    {
        // Calculate the distance between the center points of both objects
        return ( x * x ) +
               ( y * y ) +
               ( z * z );

    }   // GetLengthSt

    /************************************************************************                                                             
    *    DESCRIPTION:  Get the length of the x z only 
    *
    *    Output: float - length of point
    ************************************************************************/
    float GetLengthXZSt()
    {
        // Calculate the distance between the center points of both objects
        return ( x * x ) +
               ( z * z );

    }   // GetLengthXZSt

	/************************************************************************                                                             
    *    DESCRIPTION:  Get the length of the x z only 
    *
    *    Output: float - length of point
    ************************************************************************/
    float GetLengthYZSt()
    {
        // Calculate the distance between the center points of both objects
        return ( y * y ) +
               ( z * z );

    }   // GetLengthXZSt

    /************************************************************************                                                             
    *    DESCRIPTION:  Get the length of the points 
    *
    *    Output: float - length of point
    ************************************************************************/
    float GetLengthDy()
    {
        // Calculate the distance between the center points of both objects
        return ( tx * tx ) +
               ( ty * ty ) +
               ( tz * tz );

    }   // GetLengthDy

    /************************************************************************                                                             
    *    DESCRIPTION:  Get the dot product 
    *
    *    Input:  CPoint &point
    *
    *    Output: float - distance between two points
    ************************************************************************/
    float _fastcall GetDotProductSt( CPoint &point )
    {
        // Calculate the dot product between two
        return ( ( x * point.x ) +
                 ( y * point.y ) +
                 ( z * point.z ) );

    }   // GetDotProductSt

    /************************************************************************                                                             
    *    DESCRIPTION:  use the point to get the length for the dot product 
    *
    *    Input:  CPoint &n - normal point
	*            CPoint &v - vertex point
    *
    *    Output: float - distance between two points
    ************************************************************************/
    float _fastcall GetLengthDotProductSt( CPoint &n, CPoint &v )
    {
        // Calculate the dot product between two
        return ( ( (v.x - x) * n.x ) +
                 ( (v.y - y) * n.y ) +
                 ( (v.z - z) * n.z ) );

    }   // GetDotProductSt

    /************************************************************************                                                             
    *    DESCRIPTION:  Get the dot product 
    *
    *    Input:  CPoint &point
    *
    *    Output: float - distance between two points
    ************************************************************************/
    float _fastcall GetDotProductDy( CPoint &point )
    {
        // Calculate the dot product between two
        return ( ( tx * point.tx ) +
                 ( ty * point.ty ) +
                 ( tz * point.tz ) );

    }   // GetDotProductDy

    /************************************************************************                                                             
    *    DESCRIPTION:  use the point to get the length for the dot product 
    *
    *    Input:  CPoint &point
    *
    *    Output: float - distance between two points
    ************************************************************************/
    float _fastcall GetLengthDotProductDy( CPoint &n, CPoint &v )
    {
        // Calculate the dot product between two
        return ( ( (v.tx - tx) * n.tx ) +
                 ( (v.ty - ty) * n.ty ) +
                 ( (v.tz - tz) * n.tz ) );

    }   // GetDotProductDy
};


/************************************************************************
*
*    Holds the 2D point values. This is the 2D companion to 3d.
*
************************************************************************/
class CPoint2D
{
    public:

    float x, z;

    // Fast init if class varaibles.
    CPoint2D():x(0),z(0)
    {};

    CPoint2D( float _x, float _z ):x(_x),z(_z)
    {};


    /************************************************************************                                                             
    *    DESCRIPTION: Normalize the point 
    ************************************************************************/
    void Normalize()
    {
        float length = sqrt( (x*x) + (z*z) );
        x /= length;
        z /= length;
    }

    /************************************************************************                                                             
    *    DESCRIPTION:  Get the angle between the two points 
    *
    *    Output: float - returns angle in degrees
    ************************************************************************/
    static double GetAngle( CPoint2D &point1, CPoint2D &point2 )
    {
        return ((acos( GetDotProduct( point1, point2 ) ) * 180) / 3.141592654);

    }   // GetAngle


    /************************************************************************                                                             
    *    DESCRIPTION:  Get the Dot Product 
    *
    *    Output: float - returns dot product
    ************************************************************************/
    static double GetDotProduct( CPoint2D &point1, CPoint2D &point2 )
    {
        return (point1.x * point2.x) + (point1.z * point2.z);

    }   // GetAngle


    /************************************************************************                                                             
    *    DESCRIPTION:  Get the length between the two points 
    *
    *    Output: float - returns angle in degrees
    ************************************************************************/
    static float GetLength( CPoint2D &point1, CPoint2D &point2 )
    {
        return (point1.x - point2.x) * (point1.z - point2.z);

    }   // GetAngle
};


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