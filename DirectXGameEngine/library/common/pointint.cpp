/************************************************************************
*    FILE NAME:       pointint.cpp
*
*    DESCRIPTION:     3D Point class using integers
************************************************************************/  

// Physical component dependency
#include <common/pointint.h>

// Game lib dependencies
#include <common/point.h>


/************************************************************************                                                             
*    desc:  The addition operator 
*
*    param:  const CPointInt & point - point to add
*
*    return: CPoint - added point
************************************************************************/
CPointInt CPointInt::operator + ( const CPoint & point ) const
{
    CPointInt tmp;

    tmp.x = x + point.x;
    tmp.y = y + point.y;
    tmp.z = z + point.z;

    return tmp;

}   // operator +

/************************************************************************                                                             
*    desc:  The addition operator 
*
*    param:  const CPointInt & point - point to add
*
*    return: CPoint - added point
************************************************************************/
CPointInt CPointInt::operator += ( const CPoint & point )
{
    x += point.x;
    y += point.y;
    z += point.z;

    return *this;

}   // operator +=


/************************************************************************                                                             
*    desc:  The subtraction operator 
*
*    param:  const CPointInt & point - point to subtract
*
*    return: CPoint - subtracted point
************************************************************************/
CPointInt CPointInt::operator - ( const CPoint & point ) const
{
    CPointInt tmp;

    tmp.x = x - point.x;
    tmp.y = y - point.y;
    tmp.z = z - point.z;

    return tmp;

}   // operator -

/************************************************************************                                                             
*    desc:  The subtraction operator 
*
*    param:  const CPointInt & point - point to subtract
*
*    return: CPoint - subtracted point
************************************************************************/
CPointInt CPointInt::operator -= ( const CPoint & point )
{
    x -= point.x;
    y -= point.y;
    z -= point.z;

    return *this;

}   // operator -=


/************************************************************************                                                             
*    desc:  The multiplication operator 
*
*    param:  const CPointInt & point - point to multiply
*
*    return: CPoint - multiplied point
************************************************************************/
CPointInt CPointInt::operator * ( const CPoint & point ) const
{
    CPointInt tmp;

    tmp.x = x * point.x;
    tmp.y = y * point.y;
    tmp.z = z * point.z;

    return tmp;

}   // operator *

/************************************************************************                                                             
*    desc:  The multiplication operator 
*
*    param:  const CPointInt & point - point to multiply
*
*    return: CPoint - multiplied point
************************************************************************/
CPointInt CPointInt::operator *= ( const CPoint & point )
{
    x *= point.x;
    y *= point.y;
    z *= point.z;

    return *this;

}   // operator *=


/************************************************************************                                                             
*    desc:  The division operator 
*
*    param:  const CPointInt & point - point to divide
*
*    return: CPoint - divided point
************************************************************************/
CPointInt CPointInt::operator / ( const CPoint & point ) const
{
    CPointInt tmp;

    tmp.x = x / point.x;
    tmp.y = y / point.y;
    tmp.z = z / point.z;

    return tmp;

}   // operator /

/************************************************************************                                                             
*    desc:  The division operator 
*
*    param:  const CPointInt & point - point to divide
*
*    return: CPoint - divided point
************************************************************************/
CPointInt CPointInt::operator /= ( const CPoint & point )
{
    x /= point.x;
    y /= point.y;
    z /= point.z;

    return *this;

}   // operator /=