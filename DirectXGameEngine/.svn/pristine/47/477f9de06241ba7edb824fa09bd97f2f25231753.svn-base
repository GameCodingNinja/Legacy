
/************************************************************************
*    FILE NAME:       transformdata.cpp
*
*    DESCRIPTION:     Class to hold position, rotation, and scale data,
*					  as well as a bitmask to tell if either is set
************************************************************************/

// Physical component dependency
#include <common/transformdata.h>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CTransformData::CTransformData()
              : scale(1,1,1)
{
}   // constructor


/************************************************************************
*    desc:  Set the position
*
*	 param:	CWorldPoint & _pos - position to set to
************************************************************************/
void CTransformData::SetPos( CWorldPoint & _pos )
{
    pos = _pos;
    parameters.Add( CTransformData::POSITION );

}	// SetPos


/************************************************************************
*    desc:  Get the position
*
*	 ret:	CWorldPoint & - position
************************************************************************/
const CWorldPoint & CTransformData::GetPos() const
{
    return pos;

}	// GetPos


/************************************************************************
*    desc:  Set the rotation
*
*	 param:	CPoint & _rot - rotation to set to
************************************************************************/
void CTransformData::SetRot( CPoint & _rot )
{
    rot = _rot;
    parameters.Add( CTransformData::ROTATION );

}	// SetRot


/************************************************************************
*    desc:  Get the rotation
*
*	 ret:	CPoint & - rotation
************************************************************************/
const CPoint & CTransformData::GetRot() const
{
    return rot;

}	// GetRot


/************************************************************************
*    desc:  Set the scale
*
*	 param:	CPoint & _scale - scale to set to
************************************************************************/
void CTransformData::SetScale( CPoint & _scale )
{
    scale = _scale;
    parameters.Add( CTransformData::SCALE );

}	// SetScale


/************************************************************************
*    desc:  Get the scale
*
*	 ret:	CPoint & - scale
************************************************************************/
const CPoint & CTransformData::GetScale() const
{
    return scale;

}	// GetScale


/************************************************************************
*    desc:  Get the parameters
*
*	 ret:	const CBitmask & - parameters of the transformations
************************************************************************/
const CBitmask & CTransformData::GetParameters() const
{
    return parameters;

}	// GetParameters