/************************************************************************
*    FILE NAME:       collisionvertex.h
*
*    DESCRIPTION:     Collision vertex class
************************************************************************/  

#ifndef __collision_vertex_h__
#define __collision_vertex_h__

// Game lib dependencies
#include <common/point.h>
#include <common/normal.h>
#include <common/worldpoint.h>

// Forward declarations
class CSpriteGroup2D;
class CSpritePath2D;

class CCollisionVertex
{
public:

    // Set-Get the vertex
    void SetPos( const CPoint & point );
    void SetPos( const CWorldPoint & point );
    CWorldPoint & GetPos();
    const CWorldPoint & GetPos() const;

    // Increment the vert position
    void IncPos( const CPoint & point );
    void IncPos( const CPointInt & point );
    void IncPos( const CWorldPoint & point );

    // Set-Get the vertex normal
    void SetNormal( const CNormal & norm );
    CNormal & GetNormal();
    const CNormal & GetNormal() const;

protected:

    // Vertex position
    CWorldPoint pos;

    // Vertex normal
    CNormal normal;

};

#endif  // __collision_vertex_h__
