/************************************************************************
*    FILE NAME:		spritepath2d.h
*
*    DESCRIPTION:   Class to hold the information of a moving collision
*					sprite
************************************************************************/

#ifndef __collision_sprite_path_2d_h__
#define __collision_sprite_path_2d_h__

// Standard lib dependencies
#include <vector>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Game lib dependencies
#include <common/point.h>
#include <2d/vertexpath.h>

// Forward declarations
class CSpriteGroup2D;
class CCollisionVertex;

class CSpritePath2D : public boost::noncopyable
{
public:

    CSpritePath2D();

    // Find potential edges and vertices we could be colliding with
    void FindPossibleEdgeCollisions();
    void FindPossibleVertCollisions();

    // React the sprite's vertices to the static edges
    void ReactToStaticEdges();

    // React the sprite's edges to the static verts
    void ReactToStaticVerts();

    // React the sprite's verts and edges to the static verts and edges
    void ReactToAll();

    // Check to see if the moving sprite is colliding with anything
    bool Colliding();

    // Set-Get the moving object's sprite
    void SetSprite( CSpriteGroup2D * pGroup );
    CSpriteGroup2D * GetSprite();

    // Get the moving sprite's radius
    float GetRadiusSqrt();

    // Get the moving sprite's center point
    CPoint & GetCenter();

    // Get the sprite path vector
    CPoint & GetPathVector();

    // Add a sprite to the possible sprite list
    void SetPossibleSpriteList( std::vector<CCollisionSprite2D *> & pSpriteList );

    // Transform the collision sprite using the initial translation position
    void ResetSpriteTransformation();

protected:

    // Create the vertex path list
    void CreateVertPathList();

    // A vector of the vertex paths
    std::vector<CVertexPath> vertPathList;

    // A vector of the collision sprites that can be potentially collided with
    std::vector<CCollisionSprite2D *> pPossibleSpritesList;

    // List of static vertices that we can possibly collide with
    std::vector<CCollisionVertex *> pStaticVertList;

    // A pointer to the moving sprite
    CSpriteGroup2D * pGroup;

    // The position of the sprite's post transformation position.
    // This position is unrelated to transformations via collision offsets.
    CPoint initialTransPos;

    // Radius of the moving sprite
    float radiusSqrt;

    // Center point of the moving sprite
    CPoint center;

    // The vector that the sprite travels
    CPoint pathVector;

};

#endif  // __collision_sprite_path_2d_h__