
/************************************************************************
*    FILE NAME:       worldvalue.h
*
*    DESCRIPTION:     World value class
************************************************************************/

#ifndef __world_value_h__
#define __world_value_h__

// Boost lib dependencies
#include <boost/serialization/nvp.hpp>


class CWorldValue
{
public:

    // Constructors
    CWorldValue();
    CWorldValue( const CWorldValue & value );
    CWorldValue( int _i );
    CWorldValue( float _f );
    CWorldValue( int _i, float _f );
    CWorldValue( float _f, int _i );

    // Serialize the value
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar 
            & BOOST_SERIALIZATION_NVP(i)
            & BOOST_SERIALIZATION_NVP(f);

    }	// serialize

    // Conform the world value to the sector size
    void ConformValue();
    CWorldValue GetConformedValue() const;

    // Convert the value in single float form
    void ToFloat();
    float GetFloat() const;
    operator float() const { return GetFloat(); }

    // Comparison Operators
    bool CWorldValue::operator == ( const CWorldValue & value ) const;
    bool CWorldValue::operator == ( const float value ) const;

    bool CWorldValue::operator != ( const CWorldValue & value ) const;
    bool CWorldValue::operator != ( const float value ) const;

    bool CWorldValue::operator < ( const CWorldValue & value ) const;
    bool CWorldValue::operator < ( const float value ) const;

    bool CWorldValue::operator > ( const CWorldValue & value ) const;
    bool CWorldValue::operator > ( const float value ) const;

    bool CWorldValue::operator <= ( const CWorldValue & value ) const;
    bool CWorldValue::operator <= ( const float value ) const;

    bool CWorldValue::operator >= ( const CWorldValue & value ) const;
    bool CWorldValue::operator >= ( const float value ) const;

    // Assignment Operator
    CWorldValue & CWorldValue::operator = ( const CWorldValue & value );

    // Addition Operators
    CWorldValue CWorldValue::operator + ( const CWorldValue & value ) const;
    CWorldValue CWorldValue::operator + ( const float value ) const;
    void CWorldValue::operator += ( const CWorldValue & value );
    void CWorldValue::operator += ( const float value );

    // Subtraction Operators
    CWorldValue CWorldValue::operator - ( const CWorldValue & value ) const;
    CWorldValue CWorldValue::operator - ( const float value ) const;
    void CWorldValue::operator -= ( const CWorldValue & value );
    void CWorldValue::operator -= ( const float value );

    // Multiplication Operators
    CWorldValue CWorldValue::operator * ( const CWorldValue & value ) const;
    CWorldValue CWorldValue::operator * ( const int value ) const;
    CWorldValue CWorldValue::operator * ( const float value ) const;
    void CWorldValue::operator *= ( const CWorldValue & value );
    void CWorldValue::operator *= ( const int value );
    void CWorldValue::operator *= ( const float value );

    // Division Operators
    CWorldValue CWorldValue::operator / ( const CWorldValue & value ) const;
    CWorldValue CWorldValue::operator / ( const float value ) const;
    void CWorldValue::operator /= ( const CWorldValue & value );
    void CWorldValue::operator /= ( const float value );
    
    // Negative Operator
    CWorldValue CWorldValue::operator - () const;

public:

    // The int and float values of the world point
    int i;
    float f;

    // sector size
    static const int SECTOR_SIZE;
    static const int HALF_SECTOR_SIZE;
};

#endif  // __classtemplate_h__


