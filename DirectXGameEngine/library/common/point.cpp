
/************************************************************************
*    FILE NAME:       point.h
*
*    DESCRIPTION:     3D Point class
************************************************************************/

// Physical component dependency
#include <common/point.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <common/matrix.h>
#include <common/pointint.h>
#include <common/worldpoint.h>

using namespace std;

/************************************************************************                                                             
*    desc:  Constructor
************************************************************************/
CPoint::CPoint()
      : x(0),
        y(0),
        z(0)
{
}	// Constructor

CPoint::CPoint( const CPoint & obj )
{
    *this = obj;

}	// Constructor

CPoint::CPoint( float _x, float _y, float _z )
      : x(_x),
        y(_y),
        z(_z)
{
}	// Constructor

CPoint::CPoint( float _x, float _y )
      : x(_x),
        y(_y),
        z(0)
{
}	// Constructor

CPoint::CPoint( const CWorldPoint & point )
{
    *this = point;

}	// Constructor


/************************************************************************                                                             
*    desc:  Set the point data
*
*	 param: float _x, _y, _z - values to set the point to
************************************************************************/
void CPoint::Set( float _x, float _y, float _z )
{
    x = _x;
    y = _y;
    z = _z;

}   // Set 


/************************************************************************                                                             
*    desc:  Set the point data
*
*	 param: float value - value to set the point to
************************************************************************/
void CPoint::Set( float value )
{
    x = value;
    y = value;
    z = value;

}   // Set


/************************************************************************                                                             
*    desc:  Normalize the point
************************************************************************/
void CPoint::Normalize()
{
    float length = GetLength();

    if( length != 0.0f )
    {
        float inverseLength = 1.f / length;

        x *= inverseLength;
        y *= inverseLength;
        z *= inverseLength;
    }

}	// Normalize


/************************************************************************                                                             
*    desc:  Normalize the point along the x and y axis
************************************************************************/
void CPoint::Normalize2D()
{
    float length = GetLength2D();

    if( length != 0.0f )
    {
        float inverseLength = 1.f / length;

        x *= inverseLength;
        y *= inverseLength;
        z = 0;
    }

}	// Normalize2D


/************************************************************************                                                             
*    desc:  Cap the values
*
*	 param: float value - value to cap at
************************************************************************/
void CPoint::Cap( float value )
{
    if( value > 0.0f )
    {
        if( x > value )
            x -= value;
        
        else if ( x < 0.0f )
            x += value;

        if( y > value )
            y -= value;
        
        else if ( y < 0.0f )
            y += value;

        if( z > value )
            z -= value;
        
        else if ( z < 0.0f )
            z += value;
    }
    else
    {
        if( x > value )
            x += value;
        
        else if ( x < 0.0f )
            x -= value;
        
        if( y > value )
            y += value;
        
        else if ( y < 0.0f )
            y -= value;
        
        if( z > value )
            z += value;
        
        else if ( z < 0.0f )
            z -= value;
    }

}   // Cap


/************************************************************************                                                             
*    desc:  Does this point not have any data? 
*
*    return: bool
************************************************************************/
bool CPoint::IsEmpty() const
{
    return ( x == 0.0f ) &&  ( y == 0.0f ) && ( z == 0.0f );

}   // IsEmpty


/************************************************************************                                                             
*    desc:  Flip the signs of the point
************************************************************************/
void CPoint::FlipSigns()
{
    x = -x;
    y = -y;
    z = -z;

}   // FlipSigns


/************************************************************************                                                             
*    desc:  Flip the signs of the x component
************************************************************************/
void CPoint::FlipSignX()
{
    x = -x;

}   // FlipSignX


/************************************************************************                                                             
*    desc:  Flip the signs of the y component
************************************************************************/
void CPoint::FlipSignY()
{
    y = -y;

}   // FlipSignY


/************************************************************************                                                             
*    desc:  Flip the signs of the z component
************************************************************************/
void CPoint::FlipSignZ()
{
    z = -z;

}   // FlipSignZ


/************************************************************************                                                             
*    desc:  Make the components of the point positive
************************************************************************/
void CPoint::Abs()
{
    abs(x);
    abs(y);
    abs(z);

}	// Abs


/************************************************************************                                                             
*    desc:  Get a copy of the point where the components of the point are
*			positive
*
*	 ret:	CPoint - positive point
************************************************************************/
CPoint CPoint::GetAbs() const
{
    CPoint tmp(x,y,z);
    tmp.Abs();

    return tmp;

}	// GetAbs


/************************************************************************                                                             
*    desc:  Get the length of the point from the origin 
*
*    ret:	float - length of point
************************************************************************/
float CPoint::GetLength() const
{
    if( !IsEmpty() )
        return sqrt( GetLengthSquared() );

    return 0;

}   // GetLengthSquared


/************************************************************************                                                             
*    desc:  Get the length between two points 
*
*    param: CPoint & point - point
*
*    ret:	float - distance between two points
************************************************************************/
float CPoint::GetLength( const CPoint & point ) const
{
    return sqrt( GetLengthSquared( point ) );

}   // GetLengthSquared


/************************************************************************                                                             
*    desc:  Get the length of the point from the origin along the x and y
*			axis
*
*    ret:	float - length of point
************************************************************************/
float CPoint::GetLength2D() const
{
    if( !IsEmpty() )
        return sqrt( GetLengthSquared2D() );

    return 0;

}   // GetLength2D


/************************************************************************                                                             
*    desc:  Get the squared length of the point from the origin 
*
*    ret:	float - length of point
************************************************************************/
float CPoint::GetLengthSquared() const
{
    return ( x * x ) + ( y * y ) + ( z * z );

}   // GetLengthSquared


/************************************************************************                                                             
*    desc:  Get the squared length between two points 
*
*    param: CPoint & point - point
*
*    ret:	float - squared distance between two points
************************************************************************/
float CPoint::GetLengthSquared( const CPoint & point ) const
{
    return (*this - point).GetLengthSquared();

}   // GetLengthSquared


/************************************************************************                                                             
*    desc:  Get the squared length of the point from the origin along the 
*			x and y axis
*
*    ret:	float - length of point
************************************************************************/
float CPoint::GetLengthSquared2D() const
{
    return ( x * x ) + ( y * y );

}   // GetLengthSquared2D


/************************************************************************                                                             
*    desc:  Calculate the dot product 
*
*    param: const CPoint & point - point to multiply against
*
*    ret:	float - dot product
************************************************************************/
float CPoint::GetDotProduct( const CPoint & point ) const
{
    return ( x * point.x ) + ( y * point.y ) + ( z * point.z );

}   // GetDotProduct


/************************************************************************                                                             
*    desc:  Calculate the cross product
*
*    param: const CPoint & point - point to multiply against
*
*    ret:	float - cross product
************************************************************************/
CPoint CPoint::GetCrossProduct( const CPoint & point ) const
{
    CPoint tmp;
    tmp.x = (y * point.z) - (point.y * z);
    tmp.y = (z * point.x) - (point.z * x);
    tmp.z = (x * point.y) - (point.x * y);
    tmp.Normalize();

    return tmp;
        
}	// GetCrossProduct


/************************************************************************                                                             
*    desc:  Get a string of the point's data
*
*	 ret:	string - string of the data
************************************************************************/
string CPoint::Dump() const
{
    std::string printout = "F( " +
                            str( boost::format("%d") % x ) + "x, " +
                            str( boost::format("%d") % y ) + "y, " +
                            str( boost::format("%d") % z ) + "z" +
                            " )";

    return printout;

}	// Dump


/***********************************************************************************
*	ASSIGNMENT OPERATORS
***********************************************************************************/
/************************************************************************                                                             
*    desc:  The assignment operator 
*
*    param: const & point - point to assign to
************************************************************************/
void CPoint::operator = ( const CWorldPoint & point )
{
    x = point.x;
    y = point.y;
    z = point.z;

}	// operator = 

void CPoint::operator = ( const float value )
{
    x = value;
    y = value;
    z = value;

}	// operator =


/***********************************************************************************
*	COMPARISION OPERATORS
***********************************************************************************/
/************************************************************************                                                             
*    desc:  The equality operator 
*
*    param:  CPoint & point - point to equate
*
*    return: bool - true or false
************************************************************************/
bool CPoint::operator == ( const CPoint & point ) const
{
    if( (point.x == x) && (point.y == y) && (point.z == z) )
        return true;

    return false;

}   // operator ==

/************************************************************************                                                             
*    desc:  The inequality operator 
*
*    param:  CPoint & point - point to check
*
*    return: bool - true or false
************************************************************************/
bool CPoint::operator != ( const CPoint & point ) const
{
    if( ( point.x != x ) || ( point.y != y ) || ( point.z != z ) )
        return true;

    return false;

}   // operator !=


/***********************************************************************************
*	MATRIX OPERATIONS
***********************************************************************************/
/************************************************************************                                                             
*    desc:  The multiplication operator 
*
*    param:  CMatrix & matrix - matrix to multiply
*
*    return: CPoint - multiplied point
************************************************************************/
CPoint CPoint::operator * ( const CMatrix & matrix ) const
{
    CPoint tmp;

    tmp.x = x * matrix[0] + y * matrix[4] + z * matrix[8];
    tmp.y = x * matrix[1] + y * matrix[5] + z * matrix[9];
    tmp.z = x * matrix[2] + y * matrix[6] + z * matrix[10];

    return tmp;

}   // operator *=

/************************************************************************                                                             
*    desc:  The multiplication operator 
*
*    param:  CMatrix & matrix - matrix to multiply
*
*    return: CPoint - multiplied point
************************************************************************/
CPoint CPoint::operator *= ( const CMatrix & matrix )
{
    CPoint tmp;

    tmp.x = x * matrix[0] + y * matrix[4] + z * matrix[8];
    tmp.y = x * matrix[1] + y * matrix[5] + z * matrix[9];
    tmp.z = x * matrix[2] + y * matrix[6] + z * matrix[10];

    x = tmp.x;
    y = tmp.y;
    z = tmp.z;

    return *this;

}   // operator *=



/***********************************************************************************
*	ADDITION OPERATORS
***********************************************************************************/
/************************************************************************                                                             
*    desc:  The addition operator 
*
*    param:  const & point - point to add
*
*    return: CPoint - added point
************************************************************************/
CPoint CPoint::operator + ( const CPoint & point ) const
{
    return CPoint( x + point.x, y + point.y, z + point.z );

}   // operator +

CPoint CPoint::operator + ( const CPointInt & point ) const
{
    return CPoint( x + point.x, y + point.y, z + point.z );

}   // operator +

CPoint CPoint::operator + ( const CWorldPoint & point ) const
{
    return CPoint( x + point.x, y + point.y, z + point.z );

}   // operator +

CPoint CPoint::operator + ( const float value ) const
{
    return CPoint( x + value, y + value, z + value );

}   // operator +

/************************************************************************                                                             
*    desc:  The addition operator 
*
*    param:  const & point - point to add
************************************************************************/
void CPoint::operator += ( const CPoint & point )
{
    *this = *this + point;

}   // operator +=

void CPoint::operator += ( const CPointInt & point )
{
    *this = *this + point;

}   // operator +=

void CPoint::operator += ( const CWorldPoint & point )
{
    *this = *this + point;

}   // operator +=

void CPoint::operator += ( const float value )
{
    *this = *this + value;

}   // operator +=



/***********************************************************************************
*	SUBTRACTION OPERATORS
***********************************************************************************/
/************************************************************************                                                             
*    desc:  The subtraction operator 
*
*    param:  const & point - point to subtract
*
*    return: CPoint - subtracted point
************************************************************************/
CPoint CPoint::operator - ( const CPoint & point ) const
{
    return CPoint( x - point.x, y - point.y, z - point.z );

}   // operator -

CPoint CPoint::operator - ( const CPointInt & point ) const
{
    return CPoint( x - point.x, y - point.y, z - point.z );

}   // operator -

CPoint CPoint::operator - ( const CWorldPoint & point ) const
{
    return CPoint( x - point.x, y - point.y, z - point.z );

}   // operator -

CPoint CPoint::operator - ( const float value ) const
{
    return CPoint( x - value, y - value, z - value );

}   // operator -

/************************************************************************                                                             
*    desc:  The subtraction operator 
*
*    param:  const CPointInt & point - point to subtract
*
*    return: CPoint - subtracted point
************************************************************************/
void CPoint::operator -= ( const CPoint & point )
{
    *this = *this - point;

}   // operator -=

void CPoint::operator -= ( const CPointInt & point )
{
    *this = *this - point;

}   // operator -=

void CPoint::operator -= ( const CWorldPoint & point )
{
    *this = *this - point;

}   // operator -=

void CPoint::operator -= ( const float value )
{
    *this = *this - value;

}   // operator -=



/***********************************************************************************
*	MULTIPLICATION OPERATORS
***********************************************************************************/
/************************************************************************                                                             
*    desc:  The multiplication operator 
*
*    param:  const & point - point to multiply
*
*    return: CPoint - multiplied point
************************************************************************/
CPoint CPoint::operator * ( const CPoint & point ) const
{
    return CPoint( x * point.x, y * point.y, z * point.z );

}   // operator *

CPoint CPoint::operator * ( const CPointInt & point ) const
{
    return CPoint( x * point.x, y * point.y, z * point.z );

}   // operator *

CPoint CPoint::operator * ( const CWorldPoint & point ) const
{
    return CPoint( x * point.x, y * point.y, z * point.z );

}   // operator *

CPoint CPoint::operator * ( const float value ) const
{
    return CPoint( x * value, y * value, z * value );

}   // operator *

/************************************************************************                                                             
*    desc:  The multiplication operator 
*
*    param:  const & point - point to multiply
************************************************************************/
void CPoint::operator *= ( const CPoint & point )
{
    *this = *this * point;

}   // operator *=

void CPoint::operator *= ( const CPointInt & point )
{
    *this = *this * point;

}   // operator *=

void CPoint::operator *= ( const CWorldPoint & point )
{
    *this = *this * point;

}   // operator -=

void CPoint::operator *= ( const float value )
{
    *this = *this * value;

}   // operator *=



/***********************************************************************************
*	DIVISION OPERATORS
***********************************************************************************/
/************************************************************************                                                             
*    desc:  The division operator 
*
*    param:  const & point - point to divide
*
*    return: CPoint - divided point
************************************************************************/
CPoint CPoint::operator / ( const CPoint & point ) const
{
    return CPoint( x / point.x, y / point.y, z / point.z );

}   // operator /

CPoint CPoint::operator / ( const CPointInt & point ) const
{
    return CPoint( x / point.x, y / point.y, z / point.z );

}   // operator /

CPoint CPoint::operator / ( const CWorldPoint & point ) const
{
    return CPoint( x / point.x, y / point.y, z / point.z );

}   // operator /

CPoint CPoint::operator / ( const float value ) const
{
    return CPoint( x / value, y / value, z / value );

}   // operator /

/************************************************************************                                                             
*    desc:  The division operator 
*
*    param:  const & point - point to divide
*
*    return: CPoint - divided point
************************************************************************/
void CPoint::operator /= ( const CPoint & point )
{
    *this = *this / point;

}   // operator /=

void CPoint::operator /= ( const CPointInt & point )
{
    *this = *this / point;

}   // operator /=

void CPoint::operator /= ( const CWorldPoint & point )
{
    *this = *this / point;

}   // operator /=

void CPoint::operator /= ( const float value )
{
    *this = *this / value;

}   // operator /=


/***********************************************************************************
*	NEGATIVE OPERATOR
***********************************************************************************/
/************************************************************************
*    desc:  Return a copy of the point with its signs flipped
*
*	 ret:	CPoint - point with signs flipped
************************************************************************/
CPoint CPoint::operator - () const
{
    return CPoint(-x,-y,-z);

}	// operator -