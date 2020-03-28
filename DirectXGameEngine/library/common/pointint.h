/************************************************************************
*    FILE NAME:       pointint.h
*
*    DESCRIPTION:     3D Point class using integers
************************************************************************/  

#ifndef __point_int_h__
#define __point_int_h__

// Standard lib dependencies
#include <xhash>
#include <string>

// Boost lib dependencies
#include <boost/serialization/nvp.hpp>
#include <boost/format.hpp>

// Game lib dependencies
#include <common/point.h>

// Forward declaration(s)
class CPoint;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

class CPointInt
{
public:
    /*
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar 
            & BOOST_SERIALIZATION_NVP(x)
            & BOOST_SERIALIZATION_NVP(y)
            & BOOST_SERIALIZATION_NVP(z)

            ;
    }
    */
    // point values. 
    int x, y, z;

    // Fast init if class varaibles.
    CPointInt():x(0),y(0),z(0)
    {
    }

    // Copy constructor
    CPointInt( const CPointInt & obj )
    {
        *this = obj;
    }

    // Constructor
    CPointInt( int _x, int _y, int _z ):x(_x),y(_y),z(_z)
    {
    }

    // Operators to compare with a point
    CPointInt operator + ( const CPoint & point ) const;
    CPointInt operator += ( const CPoint & point );
    CPointInt operator - ( const CPoint & point ) const;
    CPointInt operator -= ( const CPoint & point );
    CPointInt operator * ( const CPoint & point ) const;
    CPointInt operator *= ( const CPoint & point );
    CPointInt operator / ( const CPoint & point ) const;
    CPointInt operator /= ( const CPoint & point );

    /************************************************************************                                                             
    *    desc:  Clear the x
    ************************************************************************/
    void ClearX()
    {
        x = 0;
    }

    /************************************************************************                                                             
    *    desc:  Clear the y
    ************************************************************************/
    void ClearY()
    {
        y = 0;
    }

    /************************************************************************                                                             
    *    desc:  Clear the z
    ************************************************************************/
    void ClearZ()
    {
        z = 0;
    }

    /************************************************************************                                                             
    *    desc:  Clear the values
    ************************************************************************/
    void Clear()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    /************************************************************************                                                             
    *    desc:  Set the values to absolute values
    ************************************************************************/
    void Abs()
    {
        x = abs(x);
        y = abs(y);
        z = abs(z);
    }


    /************************************************************************                                                             
    *    desc:  Check if the point components are all zero 
    *
    *    return: bool - result
    ************************************************************************/
    bool IsEmpty() const
    {
        return (x == 0 && y == 0 && z == 0);
    }


    /************************************************************************                                                             
    *    desc:  Convert to a float point 
    *
    *    return: CPoint - point int in float form
    ************************************************************************/
    CPoint ToCPoint()
    {
        CPoint tmp(x,y,z);
        return tmp;
    }


    /************************************************************************                                                             
    *    desc:  Get a string of the point's data
    *
    *	 ret:	string - string of the data
    ************************************************************************/
    std::string Dump() const
    {
        std::string printout = "I( " +
                                str( boost::format("%d") % x ) + "x, " +
                                str( boost::format("%d") % y ) + "y, " +
                                str( boost::format("%d") % z ) + "z" +
                                " )";

        return printout;

    }	// Dump


    /************************************************************************                                                             
    *    desc:  Convert to a float point 
    *
    *    param: const CPoint & point - value to assign
    ************************************************************************/
    void operator = ( const CPoint & point )
    {
        x = point.x;
        y = point.y;
        z = point.z;

    }	// operator =


    /************************************************************************                                                             
    *    desc:  The less than operator 
    *
    *    param:  CPointInt & point - point to compare
    *
    *    return: bool - true or false
    ************************************************************************/
    bool operator < ( const CPointInt & point ) const
    {
        if( x < point.x )
            return true;

        else if( y < point.y )
            return true;

        else if( z < point.z )
            return true;

        return false;

    }   // operator <


    /************************************************************************                                                             
    *    desc:  The greater than operator 
    *
    *    param:  CPointInt & point - point to compare
    *
    *    return: bool - true or false
    ************************************************************************/
    bool operator > ( const CPointInt & point ) const
    {
        if( x > point.x )
            return true;

        else if( y > point.y )
            return true;

        else if( z > point.z )
            return true;

        return false;

    }   // operator >


    /************************************************************************                                                             
    *    desc:  The equality operator 
    *
    *    param:  CPointInt & point - point to equate
    *
    *    return: bool - true or false
    ************************************************************************/
    bool operator == ( const CPointInt & point ) const
    {
        if( (point.x == x) && (point.y == y) && (point.z == z) )
        {
            return true;
        }
        return false;

    }   // operator ==

    /************************************************************************                                                             
    *    desc:  The inequality operator 
    *
    *    param:  CPointInt & point - point to check
    *
    *    return: bool - true or false
    ************************************************************************/
    bool operator != ( const CPointInt & point ) const
    {
        if( ( point.x != x ) || ( point.y != y ) || ( point.z != z ) )
        {
            return true;
        }
        return false;

    }   // operator !=


    /********************************************************************************************************                                                             
    *    SUBTRACTION
    ********************************************************************************************************/
    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    *
    *    param:  CPointInt & point - point to subtract
    *
    *    return: CPointInt - subtracted point
    ************************************************************************/
    CPointInt operator - ( const CPointInt & point ) const
    {
        CPointInt tmp;
        tmp.x = x - point.x;
        tmp.y = y - point.y;
        tmp.z = z - point.z;

        return tmp;

    }   // operator -

    /************************************************************************                                                             
    *    desc:  The subtraction operator against an int
    *
    *    param:  int * value - value to subtract
    *
    *    return: CPointInt - subtracted point
    ************************************************************************/
    CPointInt operator - ( int value ) const
    {
        CPointInt tmp;
        tmp.x = x - value;
        tmp.y = y - value;
        tmp.z = z - value;

        return tmp;

    }   // operator -


    /********************************************************************************************************                                                             
    *    ADDITION
    ********************************************************************************************************/
    /************************************************************************                                                             
    *    desc:  The addition operator 
    *
    *    param:  CPointInt & point - point to add
    *
    *    return: CPointInt - added point
    ************************************************************************/
    CPointInt operator + ( const CPointInt & point ) const
    {
        CPointInt tmp;
        tmp.x = x + point.x;
        tmp.y = y + point.y;
        tmp.z = z + point.z;

        return tmp;

    }   // operator +

    /************************************************************************                                                             
    *    desc:  The addition operator against an int
    *
    *    param:  int value - value to add
    *
    *    return: CPointInt - added point
    ************************************************************************/
    CPointInt operator + ( int value ) const
    {
        CPointInt tmp;
        tmp.x = x + value;
        tmp.y = y + value;
        tmp.z = z + value;

        return tmp;

    }   // operator +


    /********************************************************************************************************                                                             
    *    RELATIVE ADDITION
    ********************************************************************************************************/
    /************************************************************************                                                             
    *    desc:  The relative addition operator 
    *
    *    param:  CPointInt & point - point to add
    *
    *    return: CPointInt - added point
    ************************************************************************/
    CPointInt operator += ( const CPointInt & point )
    {
        x += point.x;
        y += point.y;
        z += point.z;

        return *this;

    }   // operator +=

    /************************************************************************                                                             
    *    desc:  The relative addition operator against an int
    *
    *    param:  int value - value to add
    *
    *    return: CPointInt - added point
    ************************************************************************/
    CPointInt operator += ( int value )
    {
        x += value;
        y += value;
        z += value;

        return *this;

    }   // operator +=


    /********************************************************************************************************                                                             
    *    RELATIVE SUBTRACTION
    ********************************************************************************************************/
    /************************************************************************                                                             
    *    desc:  The relative subtraction operator 
    *
    *    param:  CPointInt & point - point to add
    *
    *    return: CPointInt - added point
    ************************************************************************/
    CPointInt operator -= ( const CPointInt & point )
    {
        x -= point.x;
        y -= point.y;
        z -= point.z;

        return *this;

    }   // operator -=

    /************************************************************************                                                             
    *    desc:  The relative subtraction operator against an int
    *
    *    param:  int value - value to add
    *
    *    return: CPointInt - added point
    ************************************************************************/
    CPointInt operator -= ( int value )
    {
        x -= value;
        y -= value;
        z -= value;

        return *this;

    }   // operator -=


    /********************************************************************************************************                                                             
    *    MULTIPLICATION
    ********************************************************************************************************/
    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    *
    *    param:  CPointInt & point - point to multiply
    *
    *    return: CPointInt - multiplied point
    ************************************************************************/
    CPointInt operator * ( int value ) const
    {
        CPointInt tmp;
        tmp.x = x * value;
        tmp.y = y * value;
        tmp.z = z * value;

        return tmp;

    }   // operator *

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    *
    *    param:  CPoint & point - point to multiply
    *
    *    return: CPoint - multiplied point
    ************************************************************************/
    CPoint operator * ( float value ) const
    {
        CPoint tmp;
        tmp.x = (float)x * value;
        tmp.y = (float)y * value;
        tmp.z = (float)z * value;

        return tmp;

    }   // operator *


    /************************************************************************                                                             
    *    desc:  Flip the signs of the point
    ************************************************************************/
    void FlipSigns()
    {
        x *= -1;
        y *= -1;
        z *= -1;

    }	// FlipSigns

};


#endif  // __point_int_h__