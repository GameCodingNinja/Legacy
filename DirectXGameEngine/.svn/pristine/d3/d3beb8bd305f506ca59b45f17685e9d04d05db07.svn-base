/************************************************************************
*    FILE NAME:       worldcamera.cpp
*
*    DESCRIPTION:     Class that holds the world camera position and 
*					  rotation
************************************************************************/

// Physical component dependency
#include <3d/worldcamera.h>


/************************************************************************
*    desc:  Constructor                                                             
************************************************************************/
CWorldCamera::CWorldCamera()
{
}   // Constructor


/************************************************************************
*    desc:  Destructor                                                             
************************************************************************/
CWorldCamera::~CWorldCamera()
{
}   // Destructor


/************************************************************************
*    desc:  Set the camera's world position
*
*	 param:	CWorldPoint & pos - position to set to
************************************************************************/  
void CWorldCamera::SetPos( const CWorldPoint & pos )
{
    worldPos = -pos;

}	// SetPos


/************************************************************************
*    desc:  Get the camera's world position
*
*	 ret:	CWorldPoint & - camera's position
************************************************************************/  
const CWorldPoint & CWorldCamera::GetPos() const
{
    return worldPos;

}	// GetPos


/************************************************************************
*    desc:  Increment the camera's world position
*
*	 param:	CPoint & pos - amount to increment
************************************************************************/  
void CWorldCamera::IncPos( const CPoint & pos )
{
    worldPos -= pos;

}	// IncPos


/************************************************************************
*    desc:  Increment the camera's world position
*
*	 param:	CPointInt & pos - amount to increment
************************************************************************/ 
void CWorldCamera::IncPos( const CPointInt & pos )
{
    worldPos -= pos;

}	// IncPos


/************************************************************************
*    desc:  Increment the camera's world position
*
*	 param:	CWorldPoint & pos - amount to increment
************************************************************************/ 
void CWorldCamera::IncPos( const CWorldPoint & pos )
{
    worldPos -= pos;

}	// IncPos


/************************************************************************
*    desc:  Set the camera's rotation
*
*	 param:	CPoint & rot - rotation to set to
************************************************************************/ 
void CWorldCamera::SetRotation( const CPoint & rot )
{
    rotation = rot;

    rotMatrix.InitilizeMatrix();
    rotMatrix.Rotate( rot );

}	// SetRotation


/************************************************************************
*    desc:  Get the camera's rotation
*
*	 ret:	CPoint & - camera's rotation
************************************************************************/ 
const CPoint & CWorldCamera::GetRotation() const
{
    return rotation;

}	// GetRotation


/************************************************************************
*    desc:  Get the camera's rotation matrix
*
*	 ret:	CMatrix & - camera's rotation matrix
************************************************************************/ 
const CMatrix & CWorldCamera::GetRotationMatrix() const
{
    return rotMatrix;

}	// GetRotationMatrix


/************************************************************************
*    desc:  Increment the camera's rotation
*
*	 param:	CPoint & rot - amount to increment
************************************************************************/ 
void CWorldCamera::IncRotation( const CPoint & rot )
{
    rotation += rot;

}	// IncRotation


/************************************************************************
*    desc:  Transform the passed in point
*
*	 param:	CPoint & dest              - transformed point
*			const CWorldPoint & source - point to transform
************************************************************************/ 
void CWorldCamera::Transform( CPoint & dest, const CWorldPoint & source ) const
{
    CWorldPoint resultPoint = source + worldPos;

    rotMatrix.Transform3x3( dest, resultPoint );

}	// Transform
