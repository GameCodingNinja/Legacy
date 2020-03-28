/************************************************************************
*    FILE NAME:       collisioncheck2d.h
*
*    DESCRIPTION:     Class to check and react to 2D collisions
************************************************************************/

#ifndef __collision_check_2d_h__
#define __collision_check_2d_h__

// Standard lib dependencies
#include <vector>

// Game lib dependencies
#include <common/point.h>
#include <2d/spritepath2d.h>

// Forward declarations
class CSpriteGroup2D;

class CCollisionCheck2D
{
public:

    CCollisionCheck2D();
    ~CCollisionCheck2D();

    // React the passed in sprite to the passed in vector of static sprites
    void ReactToStaticSprites( CSpriteGroup2D * pGroup, std::vector<CSpriteGroup2D *> & pStaticList );


protected:

    // Use the passed in sprite to find the sprites we could possibly be colliding with
    void FindPossibleSprites( CSpriteGroup2D * pGroup, std::vector<CSpriteGroup2D *> & pSpriteList );

    // Vector of points representing a vertex's pre and post transformation position
    CSpritePath2D spritePath;

    // The sprites we can possibly collide with
    std::vector<CCollisionSprite2D *> pPossibleSpritesList;

};

#endif  // __collision_check_2d_h__