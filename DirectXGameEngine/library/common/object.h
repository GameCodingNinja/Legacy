/************************************************************************
*    FILE NAME:       object.h
*
*    DESCRIPTION:     Base object class
************************************************************************/

#ifndef __object_h__
#define __object_h__

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Game lib dependencies
#include <common/bitmask.h>
#include <common/point.h>
#include <common/matrix.h>
#include <common/worldpoint.h>

// Forward declaration(s)
class CTransformData;

class CObject : boost::noncopyable
{
public:
    
    enum
    {
        // No parameters
        NONE = 0,

        // Transform parameters
        TRANSFORM = 0x0001,
        TRANSLATE = 0x0002,
        ROTATE	  = 0x0004,
        SCALE	  = 0x0008
    };

    // Types of radii
    enum ERadiusType
    {
        ERT_MIN = 0,
        ERT_SCALED_RADIUS = 0,
        ERT_SCALED_RADIUS_SQUARED,
        ERT_UNSCALED_RADIUS,
        ERT_UNSCALED_RADIUS_SQUARED,
        ERT_MAX
    };
    
    CObject();
    virtual ~CObject();

    // Set the transform data
    void SetTransData( const CTransformData & transData );

    // Set-Get the object's position
    virtual void SetPos( const CPoint & position );
    virtual void SetPos( const CPointInt & position );
    virtual void SetPos( const CWorldPoint & position );
    const CWorldPoint & GetPos() const;
    virtual void SetDefaultPos( const CPoint & position );
    virtual void SetDefaultPos( const CPointInt & position );
    virtual void SetDefaultPos( const CWorldPoint & position );
    const CWorldPoint & GetDefaultPos() const;

    // Increment the object's position
    virtual void IncPos( const CPoint & position );
    virtual void IncPos( const CPointInt & position );
    virtual void IncPos( const CWorldPoint & position );

    // Set-Get the object's rotation
    virtual void SetRot( const CPoint & rotation );
    const CPoint & GetRot() const;

    // Increment the object's rotation
    virtual void IncRot( const CPoint & rotation );

    // Set the object's scale
    virtual void SetScale( float _scale );
    virtual void SetScale( const CPoint & _scale );

    // Increment the object's scale
    virtual void IncScale( float _scale );
    virtual void IncScale( const CPoint & _scale );

    // Get the object's scale
    const CPoint & GetScale() const;

    // Transform the object
    virtual void Transform();
    virtual void Transform( CMatrix & matrix );
    virtual void Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, const CWorldPoint & point );
    virtual void TransformToLocal();

    // Get the object's matrices
    const CMatrix & GetUnscaledMatrix() const;
    const CMatrix & GetScaledMatrix() const;

    // Get the object's transformed position
    const CWorldPoint & GetTransPos() const;

    // Set the object's visibility
    void SetVisible( bool value = true );

    // Is the object visible
    bool IsVisible() const;

    // Set and get the transformed flag
    const CBitmask & GetParameters() const;
    void SetTransformParameters( const CBitmask & transform, bool transFromParent = false );
    void ResetTransformParameters();

    // Set and get the radius
    void SetRadius( float value );
    float GetRadius( ERadiusType type = ERT_SCALED_RADIUS ) const;

    // Set the parent object
    void SetParent( CObject * pObj );

private:

    // Scale the radius
    void ScaleRadius();

private:

    // Object position
    CWorldPoint pos;
    CWorldPoint trans_pos;
    CWorldPoint defaultPos;

    // Object Rotation
    CPoint rot;

    // Object scale
    CPoint scale;

    // Object matrices
    CMatrix unscaledMatrix;
    CMatrix scaledMatrix;

    // Visibility flag
    bool visible;

    // Bitmask letting us know if the object needs to be transformed
    CBitmask parameters;

    // Array to hold the different forms of the radius
    float radiusArray[ERT_MAX];

    // A pointer to the parent object. This class does not
    // own this pointer
    CObject * pParent;
};

#endif  // __object_h__
