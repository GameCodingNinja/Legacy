
/************************************************************************
*    FILE NAME:       point.h
*
*    DESCRIPTION:     3D Point class
************************************************************************/  

#ifndef __point_h__
#define __point_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/serialization/nvp.hpp>

// Forward declaration(s)
class CMatrix;
class CPointInt;
class CWorldPoint;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*	Class: CBoolPoint
*
*	Description: Class for remembering what xyz positions need to be set
************************************************************************/
class CBoolPoint
{
public:

    CBoolPoint() : x(false),y(false),z(false)
    {}

    bool x, y, z;
};

class CPoint
{
public:

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar 
            & BOOST_SERIALIZATION_NVP(x)
            & BOOST_SERIALIZATION_NVP(y)
            & BOOST_SERIALIZATION_NVP(z);
    }

    // point values. 
    float x, y, z;

    // Constructors
    CPoint();
    CPoint( const CPoint & obj );
    CPoint( float _x, float _y, float _z );
    CPoint( float _x, float _y );
    CPoint( const CWorldPoint & point );

    // Get functions
    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetZ() const { return z; }

    // Set the point data
    void Set( float _x, float _y, float _z );
    void Set( float value );

    // Clear functions
    void ClearX() { x = 0; }
    void ClearY() { y = 0; }
    void ClearZ() { z = 0; }
    void Clear()  { x = 0; y = 0; z = 0; }

    // Normalize the point
    void Normalize();
    void Normalize2D();

    // Cap the values
    void Cap( float value );

    // Is the point set to zero
    bool IsEmpty() const;

    // Flip the signs of the float's components
    void FlipSigns();
    void FlipSignX();
    void FlipSignY();
    void FlipSignZ();

    // Absolute value
    void Abs();
    CPoint GetAbs() const;

    // Get the length and length squared of the point
    float GetLength() const;
    float GetLength( const CPoint & point ) const;
    float GetLength2D() const;
    float GetLengthSquared() const;
    float GetLengthSquared( const CPoint & point ) const;
    float GetLengthSquared2D() const;

    // Calculate the dot product
    float GetDotProduct( const CPoint & point ) const;

    // Calculate the cross product
    CPoint GetCrossProduct( const CPoint & point ) const;

    // Print out the contents of the point
    std::string Dump() const;

    // Assignment operators
    void CPoint::operator = ( const CWorldPoint & point );
    void CPoint::operator = ( const float value );

    // Comparison operators
    bool CPoint::operator == ( const CPoint & point ) const;
    bool CPoint::operator != ( const CPoint & point ) const;

    // Operators to compare with a matrix
    CPoint CPoint::operator * ( const CMatrix & matrix ) const;
    CPoint CPoint::operator *= ( const CMatrix & matrix );

    // Addition operators
    CPoint CPoint::operator + (const CPoint & point ) const;
    CPoint CPoint::operator + ( const CPointInt & point ) const;
    CPoint CPoint::operator + ( const CWorldPoint & point ) const;
    CPoint CPoint::operator + ( const float value ) const;
    void CPoint::operator += ( const CPoint & point );
    void CPoint::operator += ( const CPointInt & point );
    void CPoint::operator += ( const CWorldPoint & point );
    void CPoint::operator += ( const float value );

    // Subtraction operators
    CPoint CPoint::operator - ( const CPoint & point ) const;
    CPoint CPoint::operator - ( const CPointInt & point ) const;
    CPoint CPoint::operator - ( const CWorldPoint & point ) const;
    CPoint CPoint::operator - ( const float value ) const;
    void CPoint::operator -= ( const CPoint & point );
    void CPoint::operator -= ( const CPointInt & point );
    void CPoint::operator -= ( const CWorldPoint & point );
    void CPoint::operator -= ( const float value );

    // Multiplication operators
    CPoint CPoint::operator * ( const CPoint & point ) const;
    CPoint CPoint::operator * ( const CPointInt & point ) const;
    CPoint CPoint::operator * ( const CWorldPoint & point ) const;
    CPoint CPoint::operator * ( const float value ) const;
    void CPoint::operator *= ( const CPoint & point );
    void CPoint::operator *= ( const CPointInt & point );
    void CPoint::operator *= ( const CWorldPoint & point );
    void CPoint::operator *= ( const float value );

    // Division operators
    CPoint CPoint::operator / ( const CPoint & point ) const;
    CPoint CPoint::operator / ( const CPointInt & point ) const;
    CPoint CPoint::operator / ( const CWorldPoint & point ) const;
    CPoint CPoint::operator / ( const float value ) const;
    void CPoint::operator /= ( const CPoint & point );
    void CPoint::operator /= ( const CPointInt & point );
    void CPoint::operator /= ( const CWorldPoint & point );
    void CPoint::operator /= ( const float value );

    // Negative operator
    CPoint CPoint::operator - () const;

};

#endif  // __point_h__

