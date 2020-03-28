/************************************************************************
*    FILE NAME:       collisionvertex.cpp
*
*    DESCRIPTION:     Collision vertex class
************************************************************************/

// Physical component dependency
#include <common/collisionvertex.h>

// Game lib dependencies
#include <common/edge.h>
#include <2d/spritegroup2d.h>
#include <utilities/collisionfunc2d.h>
#include <utilities/genfunc.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

// Padding for offsets
const float PADDING = 0.1f;

/************************************************************************
*    desc:  Set the vertex
*
*	 param:	CPoint & point - point to set the vertex to
************************************************************************/
void CCollisionVertex::SetPos( const CPoint & point )
{
    pos = point;

}	// SetPos */


/************************************************************************
*    desc:  Set the vertex
*
*	 param:	CWorldPoint & point - point to set the vertex to
************************************************************************/
void CCollisionVertex::SetPos( const CWorldPoint & point )
{
    pos = point;

}	// SetPos */


/************************************************************************
*    desc:  Get the vertex
*
*	 ret:	CPoint & - collision vertex
************************************************************************
CPoint CCollisionVertex::GetVert()
{
    return vert.GetPointFloat();

}	// GetVert 

CPoint CCollisionVertex::GetVert() const
{
    return vert.GetPointFloat();

}	// GetVert */


/************************************************************************
*    desc:  Get the vertex
*
*	 ret:	CWorldPoint & - collision vertex
************************************************************************/
CWorldPoint & CCollisionVertex::GetPos()
{
    return pos;

}	// GetPos */

const CWorldPoint & CCollisionVertex::GetPos() const
{
    return pos;

}	// GetPos */


/************************************************************************
*    desc:  Increment the positino of the vertex
*
*	 param:	CPoint & point - amount to increment by
************************************************************************/
void CCollisionVertex::IncPos( const CPoint & point )
{
    pos += point;

}	// IncPos */


/************************************************************************
*    desc:  Increment the positino of the vertex
*
*	 param:	CPointInt & point - amount to increment by
************************************************************************/
void CCollisionVertex::IncPos( const CPointInt & point )
{
    pos += point;

}	// IncPos */


/************************************************************************
*    desc:  Increment the positino of the vertex
*
*	 param:	CWorldPoint & point - amount to increment by
************************************************************************/
void CCollisionVertex::IncPos( const CWorldPoint & point )
{
    pos += point;

}	// IncPos */


/************************************************************************
*    desc:  Set the vertex normal
*
*	 param:	CNormal & norm - normal to set the vertex normal to
************************************************************************/
void CCollisionVertex::SetNormal( const CNormal & norm )
{
    normal = norm;

}	// SetNormal */


/************************************************************************
*    desc:  Get the vertex normal
*
*	 ret:	CNormal & - collision vertex normal
************************************************************************/
CNormal & CCollisionVertex::GetNormal()
{
    return normal;

}	// GetNormal */

const CNormal & CCollisionVertex::GetNormal() const
{
    return normal;

}	// GetNormal */
