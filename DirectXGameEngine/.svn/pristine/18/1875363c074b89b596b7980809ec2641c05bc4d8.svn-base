/************************************************************************
*    FILE NAME:       object.cpp
*
*    DESCRIPTION:     Base object class
************************************************************************/

// Physical component dependency
#include <common/object.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <common/pointint.h>
#include <common/transformdata.h>


/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CObject::CObject()
       : scale(1,1,1),
         visible(true),
         parameters(TRANSFORM),
         pParent(NULL)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CObject::~CObject()
{
}   // Destructer


/************************************************************************
*    desc:  Set the transform data
*  
*    param: const CTransformData & transData - data to set
************************************************************************/
void CObject::SetTransData( const CTransformData & transData )
{
    if( transData.GetParameters().IsSet( CTransformData::POSITION ) )
        SetPos( transData.GetPos() );

    if( transData.GetParameters().IsSet( CTransformData::ROTATION ) )
        SetRot( transData.GetRot() );

    if( transData.GetParameters().IsSet( CTransformData::SCALE ) )
        SetScale( transData.GetScale() );

}   // SetTransData


/************************************************************************
*    desc:  Get the object's world position
*  
*    ret:	const CWorldPoint & - object's world position
************************************************************************/
const CWorldPoint & CObject::GetPos() const
{
    if( pParent )
        return pParent->GetPos();

    return pos;

}   // GetPos


/************************************************************************
*    desc:  Get the object's rotation
*  
*    ret:	const CPoint & - pre-translation rotation
************************************************************************/
const CPoint & CObject::GetRot() const
{
    if( pParent )
        return pParent->GetRot();

    return rot;

}   // GetRot


/************************************************************************
*    desc:  Set the object's position
*  
*    param: const CPoint/CPointInt/CWorldPoint & position - pos to set
************************************************************************/
void CObject::SetPos( const CPoint & position )
{
    pos = position;
    parameters.Add( TRANSLATE | TRANSFORM );
    pParent = NULL;

}   // SetPos

void CObject::SetPos( const CPointInt & position )
{
    pos = position;
    parameters.Add( TRANSLATE | TRANSFORM );
    pParent = NULL;

}   // SetPos

void CObject::SetPos( const CWorldPoint & position )
{
    pos = position;
    parameters.Add( TRANSLATE | TRANSFORM );
    pParent = NULL;

}   // SetPos


/************************************************************************
*    desc:  Set the object's default position
*  
*    param: const CPoint/CPointInt/CWorldPoint & position - pos to set
************************************************************************/
void CObject::SetDefaultPos( const CPoint & position )
{
    defaultPos = position;

}   // SetDefaultPos

void CObject::SetDefaultPos( const CPointInt & position )
{
    defaultPos = position;

}   // SetDefaultPos

void CObject::SetDefaultPos( const CWorldPoint & position )
{
    defaultPos = position;

}   // SetDefaultPos


/************************************************************************
*    desc:  Get the object's default position
*  
*    ret:	const CWorldPoint & - object's default position
************************************************************************/
const CWorldPoint & CObject::GetDefaultPos() const
{
    if( pParent )
        return pParent->GetDefaultPos();

    return defaultPos;

}   // GetDefaultPos


/************************************************************************
*    desc:  Inc the object's float position
*  
*    param: const CPoint/CPointInt/CWorldPoint & position - pos to inc
************************************************************************/
void CObject::IncPos( const CPoint & position )
{
    if( !position.IsEmpty() )
    {
        pos += position;
        parameters.Add( TRANSLATE | TRANSFORM );
        pParent = NULL;
    }

}   // IncPos

void CObject::IncPos( const CPointInt & position )
{
    if( !position.IsEmpty() )
    {
        pos += position;
        parameters.Add( TRANSLATE | TRANSFORM );
        pParent = NULL;
    }

}   // IncPos

void CObject::IncPos( const CWorldPoint & position )
{
    if( !position.IsEmpty() )
    {
        pos += position;
        parameters.Add( TRANSLATE | TRANSFORM );
        pParent = NULL;
    }

}   // IncPos


/************************************************************************
*    desc:  Set the pre-translation matrix
*  
*    param: const CPoint & rotation - rotation amount
************************************************************************/
void CObject::SetRot( const CPoint & rotation )
{
    rot = rotation;
    parameters.Add( ROTATE | TRANSFORM );
    pParent = NULL;

}   // SetRot


/************************************************************************
*    desc:  Inc the pre-translation matrix
*  
*    param: const CPoint & rotation - rotation inc
************************************************************************/
void CObject::IncRot( const CPoint & rotation )
{
    if( !rotation.IsEmpty() )
    {
        rot += rotation;
        parameters.Add( ROTATE | TRANSFORM );
        pParent = NULL;

        rot.Cap(360.0f);
    }

}   // IncRot


/************************************************************************
*    desc:  Set the object's scale
*
*    param: float/CPoint _scale - scale to set
************************************************************************/
void CObject::SetScale( float _scale )
{
    SetScale( CPoint(_scale,_scale,_scale) );
    ScaleRadius();
    parameters.Add( SCALE | TRANSFORM );
    pParent = NULL;

}   // SetScale

void CObject::SetScale( const CPoint & _scale )
{
    scale = _scale;
    ScaleRadius();
    parameters.Add( SCALE | TRANSFORM );
    pParent = NULL;

}   // SetScale


/************************************************************************
*    desc:  Inc the object's scale
*
*    param: float/CPoint _scale - scale to inc
************************************************************************/
void CObject::IncScale( float _scale )
{
    if( _scale != 0 )
    {
        scale += _scale;
        ScaleRadius();
        parameters.Add( SCALE | TRANSFORM );
        pParent = NULL;
    }

}   // IncScale 

void CObject::IncScale( const CPoint & _scale )
{
    if( !_scale.IsEmpty() )
    {
        scale += _scale;
        ScaleRadius();
        parameters.Add( SCALE | TRANSFORM );
        pParent = NULL;
    }

}   // IncScale


/************************************************************************
*    desc:  Get the object's scale
*  
*    ret:	const CPoint & - sprite scale
************************************************************************/
const CPoint & CObject::GetScale() const
{
    if( pParent )
        return pParent->GetScale();

    return scale;

}   // GetScale


/************************************************************************
*    desc:  Get the translated position
*  
*    ret:	const CWorldPoint & - translated position
************************************************************************/
const CWorldPoint & CObject::GetTransPos() const
{
    if( pParent )
        return pParent->GetTransPos();

    return trans_pos;

}   // GetTransPos


/************************************************************************
*    desc:  Transform the object
************************************************************************/
void CObject::Transform()
{
    // Reset the matrices
    unscaledMatrix.InitilizeMatrix();
    scaledMatrix.InitilizeMatrix();
    trans_pos = 0;

    // Apply the scale
    scaledMatrix.Scale( scale );

    // Apply the rotation
    unscaledMatrix.Rotate( rot );

    // Apply the rotation to the scaled matrix
    scaledMatrix *= unscaledMatrix;

    // The transformed position is the same as the regular position
    trans_pos = pos;

    // Reset the transformed flag
    ResetTransformParameters();

}	// Transform


/************************************************************************
*    desc:  Transform the object
*
*    param: CMatrix & matrix - passed in matrix class
************************************************************************/
void CObject::Transform( CMatrix & matrix )
{
    // Reset the matrices
    unscaledMatrix.InitilizeMatrix();
    scaledMatrix.InitilizeMatrix();
    trans_pos = 0;

    // Apply the scale
    scaledMatrix.Scale( scale );

    // Apply the rotation
    unscaledMatrix.Rotate( rot );

    // Apply the translation
    unscaledMatrix.Translate( pos );

    // Multiply with the passed in matrix
    unscaledMatrix *= matrix;

    // Apply the rotation, translation, and passed in matrix to the scaled matrix
    scaledMatrix *= unscaledMatrix;

    // Transform the center of the object
    CPoint tmpPos;
    unscaledMatrix.Transform( tmpPos, CPoint() );
    trans_pos = tmpPos;

    // Reset the transformed flag
    ResetTransformParameters();

}	// Transform

/************************************************************************
*    desc:  Transform the object
*
*    param: const CMatrix & _scaledMatrix   - passed in relative scaled matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled matrix
*			const CWorldPoint & point	    - passed in relative position
************************************************************************/
void CObject::Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, const CWorldPoint & point )
{
    if( !pParent )
    {
        // Reset the matrices and transformed position
        unscaledMatrix.InitilizeMatrix();
        scaledMatrix.InitilizeMatrix();
        trans_pos = 0;

        // Apply the translation
        CPoint tmpPos;
        _scaledMatrix.Transform3x3( tmpPos, pos );
        trans_pos = point + tmpPos;

        // Apply the scale
        scaledMatrix.Scale( scale );

        // Apply the rotation
        unscaledMatrix.Rotate( rot );
    
        // Apply the object's unscaled matrix and the passed in scaled matrix
        scaledMatrix *= unscaledMatrix * _scaledMatrix;
        unscaledMatrix *= _unscaledMatrix;
    }

    // Reset the transformed flag
    ResetTransformParameters();

}	// Transform


/************************************************************************
*    desc:  Transform the object
************************************************************************/
void CObject::TransformToLocal()
{
    // reset the matrix
    unscaledMatrix.InitilizeMatrix();
    scaledMatrix.InitilizeMatrix();
    trans_pos = 0;

    // Scale the scaled matrix
    scaledMatrix.Scale( scale );

    // Apply the rotation
    unscaledMatrix.Rotate( rot );

    // Apply the translation
    unscaledMatrix.Translate( pos );

    // Apply translation and rotation to the scaled matrix
    scaledMatrix *= unscaledMatrix;

    // Transform the center of the object
    CPoint tmpPos;
    unscaledMatrix.Transform( tmpPos, CPoint() );
    trans_pos = tmpPos;

}	// Transform


/************************************************************************
*    desc:  Get the object's matrix that doesn't include scale
*  
*    ret:	const CMatrix & - matrix that doesn't contain the scale
************************************************************************/
const CMatrix & CObject::GetUnscaledMatrix() const
{
    if( pParent )
        return pParent->GetUnscaledMatrix();

    return unscaledMatrix;

}   // GetUnscaledMatrix


/************************************************************************
*    desc:  Get the object's matrix that includes scale
*  
*    ret:	const CMatrix & - matrix that contains the scale
************************************************************************/
const CMatrix & CObject::GetScaledMatrix() const
{
    if( pParent )
        return pParent->GetScaledMatrix();

    return scaledMatrix;

}   // GetScaledMatrix


/************************************************************************
*    desc:  Set the object visible
*  
*    param: bool value - value to set to
************************************************************************/
void CObject::SetVisible( bool value )
{
    visible = value;

}	// SetVisible


/************************************************************************
*    desc:  Is the object visible
*  
*    ret:	bool - visible flag
************************************************************************/
bool CObject::IsVisible() const
{
    return visible;

}	// IsVisible


/************************************************************************
*    desc:  Get the object's parameters
*  
*    ret:	const CBitmask & - object's parameters
************************************************************************/
const CBitmask & CObject::GetParameters() const
{
    return parameters;

}	// GetParameters


/************************************************************************
*    desc:  Set the transform required flag
*  
*    param: const CBitmask & transform - transformations to set to
*			bool transFromParent       - if what's being passed in is the
*								         object's parent's transformations,
*								         we handle things a bit differently
************************************************************************/
void CObject::SetTransformParameters( const CBitmask & transform, bool transFromParent )
{
    // The transformations of the parent are passed down to the child
    parameters.Add( transform );

    // If the passed in transformation is the transformations of the parent,
    // and they contain scale or rotation, then this child object must also
    // translate
    if( transFromParent && transform > TRANSLATE )
        parameters.Add( TRANSLATE );
     
}	// SetTransformParameters


/************************************************************************
*    desc:  Resets the transform required flag
************************************************************************/
void CObject::ResetTransformParameters()
{
    parameters.Clear();
     
}	// SetTransformParameters


/************************************************************************
*    desc:  Set the radius
*  
*    param: float value - value to set to
************************************************************************/
void CObject::SetRadius( float value )
{
    radiusArray[ERT_UNSCALED_RADIUS] = value;
    radiusArray[ERT_UNSCALED_RADIUS_SQUARED] = value * value;
    
    ScaleRadius();

}	// SetRadius


/************************************************************************
*    desc:  Get the radius
*  
*	 param:	ERadiusType type - type of radius to get
*
*    ret:	float - radius
************************************************************************/
float CObject::GetRadius( ERadiusType type ) const
{
    if( type < ERT_MIN || type >= ERT_MAX )
        throw NExcept::CCriticalException("CObject Error!",
                boost::str( boost::format("Index out of bounds of radius array.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

    return radiusArray[type];

}	// GetRadius


/************************************************************************
*    desc:  Scale the radius
************************************************************************/
void CObject::ScaleRadius()
{
    // If x is the largest dimension, scale by the x value
    if( scale.x >= scale.y && scale.x >= scale.z )
        radiusArray[ERT_SCALED_RADIUS] = scale.x * radiusArray[ERT_UNSCALED_RADIUS];
    
    // If y is the largest dimension, scale by the y value
    else if( scale.y >= scale.x && scale.y >= scale.z )
        radiusArray[ERT_SCALED_RADIUS] = scale.y * radiusArray[ERT_UNSCALED_RADIUS];
    
    // If z is the largest dimension, scale by the z value
    else
        radiusArray[ERT_SCALED_RADIUS] = scale.z * radiusArray[ERT_UNSCALED_RADIUS];

    // Calculate the scaled radius squared
    radiusArray[ERT_SCALED_RADIUS_SQUARED] = radiusArray[ERT_SCALED_RADIUS] * radiusArray[ERT_SCALED_RADIUS];

}	// ScaleRadius


/************************************************************************
*    desc:  Set the parent object
*
*	 param: CObject * pObj - parent of the object
************************************************************************/
void CObject::SetParent( CObject * pObj )
{
    pParent = pObj;

}	// SetParent