/************************************************************************
*    FILE NAME:       collisioncheck2d.cpp
*
*    DESCRIPTION:    Class to check and react to 2D collisions
************************************************************************/

// Physical component dependency
#include <2d/collisioncheck2d.h>

// Game lib dependencies
#include <utilities\\collisionfunc2d.h>
#include <2d\\spritegroup2d.h>
#include <2d\\collisionsprite2d.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

// Some padding when offseting sprites
const float PADDING = 0.1f;


/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCollisionCheck2D::CCollisionCheck2D()
{
}   // Constructer */


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCollisionCheck2D::~CCollisionCheck2D()
{
}   // Destructer */


/************************************************************************
*    desc:  React the passed in sprites to the collision of their 
*			surrounding sectors
*
*	 param: CSpriteGroup2D * pGroup                - sprite to check for
*													 collision
*			vector<CSpriteGroup2D *> & pStaticList - vector of sprites in
*													 the surrounding sectors
************************************************************************/
void CCollisionCheck2D::ReactToStaticSprites( CSpriteGroup2D * pGroup, vector<CSpriteGroup2D *> & pStaticList )
{
    // Set a sprite to a sprite path
    spritePath.SetSprite( pGroup );

    // Find the sprites we can possibly collide with
    FindPossibleSprites( pGroup, pStaticList );

    // React against static edges
    spritePath.ReactToStaticEdges();

    // If the sprite is still colliding after the previous checks, react to static vert collisions
    if( spritePath.Colliding() )
    {
        // Reset the transformation applied when reacting to the static edges
        spritePath.ResetSpriteTransformation();

        // React against static verts
        spritePath.ReactToStaticVerts();
    }

    // If the sprite is still colliding after the previous checks, react to both static edge and vert collisions
    if( spritePath.Colliding() )
    {
        spritePath.ResetSpriteTransformation();
        spritePath.ReactToAll();
    }

    // Reset the collision sprite's position
    pGroup->GetCollisionSprite().SetPos( pGroup->GetPos() );

}	// ReactToStaticSprites */


/************************************************************************
*    desc:  Use the passed in sprite to find the sprites we could possibly
*			be colliding with
*
*	 param: CSpriteGroup2D * pGroup - sprite to check against
*
*			vector<CSpriteGroup2D *> & pSpriteList - list of sprites to
*													 perform radius checks
*													 against
************************************************************************/
void CCollisionCheck2D::FindPossibleSprites( CSpriteGroup2D * pGroup, vector<CSpriteGroup2D *> & pSpriteList )
{
    // Keep track of the sprites we can possibly collide with
    pPossibleSpritesList.reserve( pSpriteList.size() );

    // Find the sprites whose radii intersect with the passed in sprite
    for( unsigned int colSprite = 0; colSprite < pSpriteList.size(); colSprite++ )
    {
        if( NCollisionFunc2D::BoxRadiiIntersect( pGroup->GetPos(), pGroup->GetCollisionSprite().GetRadiusSqrt(), 
                                                 pSpriteList[colSprite]->GetPos(), 
                                                 pSpriteList[colSprite]->GetCollisionSprite().GetRadiusSqrt() ) )
        {
            pPossibleSpritesList.push_back( &pSpriteList[colSprite]->GetCollisionSprite() );
        }
    }

    spritePath.SetPossibleSpriteList( pPossibleSpritesList );

    // Check for potential static edge collisions
    for( unsigned int i = 0; i < pPossibleSpritesList.size(); i++ )
        spritePath.FindPossibleEdgeCollisions();

    // Check for potential static vert collisions
    for( unsigned int i = 0; i < pPossibleSpritesList.size(); i++ )
        spritePath.FindPossibleVertCollisions();

}	// FindPossibleSprites */

