/************************************************************************
*    FILE NAME:       newworldpoint.h
*
*    DESCRIPTION:     World point class
************************************************************************/

#ifndef __new_world_point_h__
#define __new_world_point_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/serialization/nvp.hpp>

// Game lib dependencies
#include <common/worldvalue.h>

// Forward declaration(s)
class CPoint;
class CPointInt;
class CMatrix;

class CWorldPoint
{
public:

    // Constructor
    CWorldPoint();
    CWorldPoint( const CWorldPoint & point );
    CWorldPoint( const CPointInt & point );
    CWorldPoint( const CPoint & point );
    CWorldPoint( const CPointInt & point_i, const CPoint & point_f );
    CWorldPoint( const CPoint & point_f, const CPointInt & point_i );
    CWorldPoint( const CWorldValue & _x, const CWorldValue & _y, const CWorldValue & _z );

    // Serialize the point
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar 
            & BOOST_SERIALIZATION_NVP(x)
            & BOOST_SERIALIZATION_NVP(y)
            & BOOST_SERIALIZATION_NVP(z);

    }	// serialize

    // Get the int and float components separately
    CPointInt GetIntComponent() const;
    CPoint GetFloatComponent() const;

    // Get the length and squared length of the line from the origin to the point
    float GetLength() const;
    float GetLengthSquared() const;
    float GetLength( const CWorldPoint & point ) const;
    float GetLengthSquared( const CWorldPoint & point ) const;

    // Is the world point empty
    bool IsEmpty() const;

    // Clear the point
    void Clear();

    // Print out the contents of the world point
    std::string Dump() const;

    // Comparison Operators
    bool CWorldPoint::operator == ( const CWorldPoint & point ) const;
    bool CWorldPoint::operator == ( const CPointInt & point ) const;
    bool CWorldPoint::operator == ( const CPoint & point ) const;
    bool CWorldPoint::operator != ( const CWorldPoint & point ) const;
    bool CWorldPoint::operator != ( const CPointInt & point ) const;
    bool CWorldPoint::operator != ( const CPoint & point ) const;

    // Assignment Operators
    CWorldPoint & CWorldPoint::operator = ( const CPointInt & point );
    CWorldPoint & CWorldPoint::operator = ( const CPoint & point );
    CWorldPoint & CWorldPoint::operator = ( const float value );

    // Addition Operators
    CWorldPoint CWorldPoint::operator + ( const CWorldPoint & point ) const;
    CWorldPoint CWorldPoint::operator + ( const CPointInt & point ) const;
    CWorldPoint CWorldPoint::operator + ( const CPoint & point ) const;
    CWorldPoint CWorldPoint::operator + ( const float value ) const;
    void CWorldPoint::operator += ( const CWorldPoint & point );
    void CWorldPoint::operator += ( const CPointInt & point );
    void CWorldPoint::operator += ( const CPoint & point );
    void CWorldPoint::operator += ( const float value );

    // Subtraction Operators
    CWorldPoint CWorldPoint::operator - ( const CWorldPoint & point ) const;
    CWorldPoint CWorldPoint::operator - ( const CPointInt & point ) const;
    CWorldPoint CWorldPoint::operator - ( const CPoint & point ) const;
    CWorldPoint CWorldPoint::operator - ( const float value ) const;
    void CWorldPoint::operator -= ( const CWorldPoint & point );
    void CWorldPoint::operator -= ( const CPointInt & point );
    void CWorldPoint::operator -= ( const CPoint & point );
    void CWorldPoint::operator -= ( const float value );

    // Multiplication Operators
    CWorldPoint CWorldPoint::operator * ( const CWorldPoint & point ) const;
    CWorldPoint CWorldPoint::operator * ( const CPointInt & point ) const;
    CWorldPoint CWorldPoint::operator * ( const CPoint & point ) const;
    CWorldPoint CWorldPoint::operator * ( const float value ) const;
    void CWorldPoint::operator *= ( const CWorldPoint & point );
    void CWorldPoint::operator *= ( const CPointInt & point );
    void CWorldPoint::operator *= ( const CPoint & point );
    void CWorldPoint::operator *= ( const float value );

    // Division Operators
    CWorldPoint CWorldPoint::operator / ( const CWorldPoint & point ) const;
    CWorldPoint CWorldPoint::operator / ( const CPointInt & point ) const;
    CWorldPoint CWorldPoint::operator / ( const CPoint & point ) const;
    CWorldPoint CWorldPoint::operator / ( const float value ) const;
    void CWorldPoint::operator /= ( const CWorldPoint & point );
    void CWorldPoint::operator /= ( const CPointInt & point );
    void CWorldPoint::operator /= ( const CPoint & point );
    void CWorldPoint::operator /= ( const float value );

    // Negative Operator
    CWorldPoint CWorldPoint::operator - () const;

public:

    // Components of the world point
    CWorldValue x, y, z;

};

#endif	// __new_world_point_h__