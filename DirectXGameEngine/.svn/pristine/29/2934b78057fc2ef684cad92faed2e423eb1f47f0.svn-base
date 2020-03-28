
/************************************************************************
*    FILE NAME:       newworldpoint.cpp
*
*    DESCRIPTION:     World point class
************************************************************************/

// Physical component dependency
#include <common/worldpoint.h>

// Boost lib dependencies
#include <boost/format.hpp>

//Game Lib Dependencies
#include <common/point.h>
#include <common/pointint.h>
#include <common/matrix.h>


/************************************************************************
*    desc:  Constructer
************************************************************************/
CWorldPoint::CWorldPoint()
{
}	// Constructer

CWorldPoint::CWorldPoint( const CWorldPoint & point )
{
    *this = point;

}	// Constructer

CWorldPoint::CWorldPoint( const CPointInt & point )
{
    x.i = point.x;
    y.i = point.y;
    z.i = point.z;

}	// Constructer

CWorldPoint::CWorldPoint( const CPoint & point )
{
    x.f = point.x;
    y.f = point.y;
    z.f = point.z;

}	// Constructer

CWorldPoint::CWorldPoint( const CPointInt & point_i, const CPoint & point_f )
{
    x.i = point_i.x; x.f = point_f.x;
    y.i = point_i.y; y.f = point_f.y;
    z.i = point_i.z; z.f = point_f.z;

}	// Constructer

CWorldPoint::CWorldPoint( const CPoint & point_f, const CPointInt & point_i )
{
    x.i = point_i.x; x.f = point_f.x;
    y.i = point_i.y; y.f = point_f.y;
    z.i = point_i.z; z.f = point_f.z;

}	// Constructer

CWorldPoint::CWorldPoint( const CWorldValue & _x, const CWorldValue & _y, const CWorldValue & _z )
{
    x = _x;
    y = _y;
    z = _z;

}	// Constructor

/************************************************************************
*    desc:  Get the integer components of the world point
*
*	 ret:	CPointInt - point int
************************************************************************/
CPointInt CWorldPoint::GetIntComponent() const
{
    return CPointInt(x.i, y.i, z.i);

}	// IPoint

/************************************************************************
*    desc:  Get the float components of the world point
*
*	 ret:	CPoint - point float
************************************************************************/
CPoint CWorldPoint::GetFloatComponent() const
{
    return CPoint(x.f, y.f, z.f);

}	// FPoint


/************************************************************************
*    desc:  Get the length of the line from the origin to the point
*
*	 ret:	float - length of the line
************************************************************************/
float CWorldPoint::GetLength() const
{
    return sqrt( GetLengthSquared() );

}	// GetLength


/************************************************************************
*    desc:  Get the length squared of the line from the origin to the point
*
*	 ret:	float - length of the line
************************************************************************/
float CWorldPoint::GetLengthSquared() const
{
    return ((x * x) + (y * y) + (z * z));

}	// GetLengthSquared


/************************************************************************
*    desc:  Get the length of the line from the origin to the point
*
*	 ret:	float - length of the line
************************************************************************/
float CWorldPoint::GetLength( const CWorldPoint & point ) const
{
    return sqrt( GetLengthSquared( point ) );

}	// GetLength


/************************************************************************
*    desc:  Get the length squared of the line from the origin to the point
*
*	 ret:	float - length of the line
************************************************************************/
float CWorldPoint::GetLengthSquared( const CWorldPoint & point ) const
{
    return (*this - point).GetLengthSquared();

}	// GetLengthSquared


/************************************************************************                                                             
*    desc:  Does this point not have any data? 
*
*    ret:	bool
************************************************************************/
bool CWorldPoint::IsEmpty() const
{
    return ( x == 0.f ) &&  ( y == 0.f ) && ( z == 0.f );

}   // IsEmpty


/************************************************************************                                                             
*    desc:  Clear the point
************************************************************************/
void CWorldPoint::Clear()
{
    x = 0;
    y = 0;
    z = 0;

}	// Clear


/************************************************************************                                                             
*    desc:  Get a string of the world point's data
*
*	 ret:	string - string of the data
************************************************************************/
std::string CWorldPoint::Dump() const
{
    std::string printout = "W[ I( " +
                            str( boost::format("%d") % x.i ) + "x, " +
                            str( boost::format("%d") % y.i ) + "y, " +
                            str( boost::format("%d") % z.i ) + "z" +
                            " ) F( " +
                            str( boost::format("%d") % x.f ) + "x, " +
                            str( boost::format("%d") % y.f ) + "y, " +
                            str( boost::format("%d") % z.f ) + "z ) ]";

    return printout;

}	// Dump


/***********************************************************************************
*	COMPARISON OPERATORS
***********************************************************************************/
/************************************************************************                                                             
*    desc:  The equality operator 
*
*    param:  const & point - point to check
*
*    return: bool - true or false
************************************************************************/
bool CWorldPoint::operator == ( const CWorldPoint & point ) const
{
    if( x == point.x && y == point.y && z == point.z )
        return true;
    
    return false;

}   // operator ==

bool CWorldPoint::operator == ( const CPointInt & point ) const
{
    if( x.i == point.x && y.i == point.y && z.i == point.z )
        return true;
    
    return false;

}   // operator ==

bool CWorldPoint::operator == ( const CPoint & point ) const
{
    if( x == point.x && y == point.y && z == point.z )
        return true;
    
    return false;

}   // operator ==

/************************************************************************                                                             
*    desc:  The inequality operator 
*
*    param:  const & point - point to check
*
*    return: bool - true or false
************************************************************************/
bool CWorldPoint::operator != ( const CWorldPoint & point ) const
{
    if( x != point.x && y != point.y && z != point.z )
        return true;

    return false;

}   // operator !=

bool CWorldPoint::operator != ( const CPointInt & point ) const
{
    if( x.i != point.x && y.i != point.y && z.i != point.z )
        return true;

    return false;

}   // operator !=

bool CWorldPoint::operator != ( const CPoint & point ) const
{
    if( x != point.x && y != point.y && z != point.z )
        return true;

    return false;

}   // operator !=



/***********************************************************************************
*	ASSIGNMENT OPERATORS
***********************************************************************************/
/************************************************************************
*    desc:  Assignment operator
*
*	 param: const point - point assign
************************************************************************/
CWorldPoint & CWorldPoint::operator = ( const CPointInt & point )
{
    x.i = point.x; x.f = 0;
    y.i = point.y; y.f = 0;
    z.i = point.z; z.f = 0;

    return *this;

}	// operator =

CWorldPoint & CWorldPoint::operator = ( const CPoint & point )
{
    x = point.x;
    y = point.y;
    z = point.z;

    return *this;

}	// operator =

CWorldPoint & CWorldPoint::operator = ( const float value )
{
    x = value;
    y = value;
    z = value;

    return *this;

}	// operator =



/***********************************************************************************
*	ADDITION OPERATORS
***********************************************************************************/
/************************************************************************
*    desc:  Addition operator
*
*	 param: const point - point to add
*
*	 ret:	CWorldPoint - summation of points
************************************************************************/
CWorldPoint CWorldPoint::operator + ( const CWorldPoint & point ) const
{
    return CWorldPoint( x + point.x, y + point.y, z + point.z );

}	// operator +

CWorldPoint CWorldPoint::operator + ( const CPointInt & point ) const
{
    return CWorldPoint( CPointInt(x.i + point.x, y.i + point.y, z.i + point.z), GetFloatComponent() );

}	// operator +

CWorldPoint CWorldPoint::operator + ( const CPoint & point ) const
{
    return CWorldPoint( x + point.x, y + point.y, z + point.z );

}	// operator +

CWorldPoint CWorldPoint::operator + ( const float value ) const
{
    return CWorldPoint( x + value, y + value, z + value );

}	// operator +

/************************************************************************
*    desc:  Addition operator
*
*	 param: const point - point to add
************************************************************************/
void CWorldPoint::operator += ( const CWorldPoint & point )
{
    *this = *this + point;

}	// operator +=

void CWorldPoint::operator += ( const CPointInt & point )
{
    *this = *this + point;

}	// operator +=

void CWorldPoint::operator += ( const CPoint & point )
{
    *this = *this + point;

}	// operator += 

void CWorldPoint::operator += ( const float value )
{
    *this = *this + value;

}	// operator +=



/***********************************************************************************
*	SUBTRACTION OPERATORS
***********************************************************************************/
/************************************************************************
*    desc:  Subtraction operator
*
*	 param: const point - point to subtract
*
*	 ret:	CWorldPoint - subtraction of points
************************************************************************/
CWorldPoint CWorldPoint::operator - ( const CWorldPoint & point ) const
{
    return CWorldPoint( x - point.x, y - point.y, z - point.z );

}	// operator -

CWorldPoint CWorldPoint::operator - ( const CPointInt & point ) const
{
    return CWorldPoint( CPointInt(x.i - point.x, y.i - point.y, z.i - point.z), GetFloatComponent() );

}	// operator -

CWorldPoint CWorldPoint::operator - ( const CPoint & point ) const
{
    return CWorldPoint( x - point.x, y - point.y, z - point.z );

}	// operator -

CWorldPoint CWorldPoint::operator - ( const float value ) const
{
    return CWorldPoint( x - value, y - value, z - value );

}	// operator -

/************************************************************************
*    desc:  Subtraction operator
*
*	 param: const point - point to subtract
************************************************************************/
void CWorldPoint::operator -= ( const CWorldPoint & point )
{
    *this = *this - point;

}	// operator -=

void CWorldPoint::operator -= ( const CPointInt & point )
{
    *this = *this - point;

}	// operator -=

void CWorldPoint::operator -= ( const CPoint & point )
{
    *this = *this - point;

}	// operator -= 

void CWorldPoint::operator -= ( const float value )
{
    *this = *this - value;

}	// operator -=


/***********************************************************************************
*	MULTIPLICATION OPERATORS
***********************************************************************************/
/************************************************************************
*    desc:  Multiplication operator
*
*	 param: const point - point to multiply
*
*	 ret:	CWorldPoint - product of points
************************************************************************/
CWorldPoint CWorldPoint::operator * ( const CWorldPoint & point ) const
{
    return CWorldPoint( x * point.x, y * point.y, z * point.z );

}	// operator *

CWorldPoint CWorldPoint::operator * ( const CPointInt & point ) const
{
    return *this * CWorldPoint(point);

}	// operator *

CWorldPoint CWorldPoint::operator * ( const CPoint & point ) const
{
    return CWorldPoint( x * point.x, y * point.y, z * point.z );

}	// operator * 

CWorldPoint CWorldPoint::operator * ( const float value ) const
{
    return CWorldPoint( x * value, y * value, z * value );

}	// operator * 

/************************************************************************
*    desc:  Multiplication operator
*
*	 param: const point - point to multiply
************************************************************************/
void CWorldPoint::operator *= ( const CWorldPoint & point )
{
    *this = *this * point;

}	// operator *=

void CWorldPoint::operator *= ( const CPointInt & point )
{
    *this = *this * point;

}	// operator *=

void CWorldPoint::operator *= ( const CPoint & point )
{
    *this = *this * point;

}	// operator *= 

void CWorldPoint::operator *= ( const float value )
{
    *this = *this * value;

}	// operator *= 



/***********************************************************************************
*	DIVISION OPERATORS
***********************************************************************************/
/************************************************************************
*    desc:  Division operator
*
*	 param: const point - point to divided
*
*	 ret:	CWorldPoint - division of points
************************************************************************/
CWorldPoint CWorldPoint::operator / ( const CWorldPoint & point ) const
{
    return CWorldPoint( x / point.x, y / point.y, z / point.z );

}	// operator /

CWorldPoint CWorldPoint::operator / ( const CPointInt & point ) const
{
    return *this / CWorldPoint(point);

}	// operator /

CWorldPoint CWorldPoint::operator / ( const CPoint & point ) const
{
    return CWorldPoint( x / point.x, y / point.x, z / point.z );

}	// operator / 

CWorldPoint CWorldPoint::operator / ( const float value ) const
{
    return CWorldPoint( x / value, y / value, z / value );

}	// operator /

/************************************************************************
*    desc:  Division operator
*
*	 param: const point - point to divided
************************************************************************/
void CWorldPoint::operator /= ( const CWorldPoint & point )
{
    *this = *this / point;

}	// operator /=

void CWorldPoint::operator /= ( const CPointInt & point )
{
    *this = *this / point;

}	// operator /=

void CWorldPoint::operator /= ( const CPoint & point )
{
    *this = *this / point;

}	// operator /= 

void CWorldPoint::operator /= ( const float value )
{
    *this = *this / value;

}	// operator /=



/***********************************************************************************
*	NEGATIVE OPERATOR
***********************************************************************************/
/************************************************************************
*    desc:  Return a copy of the point with its signs flipped
*
*	 ret:	CWorldPoint - point with signs flipped
************************************************************************/
CWorldPoint CWorldPoint::operator - () const
{
    return CWorldPoint(-x,-y,-z);

}	// operator -
