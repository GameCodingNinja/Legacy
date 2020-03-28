/************************************************************************
*    FILE NAME:       collisioncheck.h
*
*    DESCRIPTION:     collision check class
************************************************************************/  

#ifndef __collision_check_h__
#define __collision_check_h__

// Game lib dependencies
#include <3d/collision.h>

enum
{
    EC_FLOOR,
    EC_WALL,
    EC_MAX
};

class CCollisionCheck
{
public:

    CCollisionCheck();
    ~CCollisionCheck();

    // Clear the collision flag
    void ClearCollisionFlag();

    // Reset the class data
    void Reset();

    // Was there a collision?
    bool IsCollision();

    // Set the collision flag
    void SetCollision( bool value );

    // Sort through the passed in collisions to find the closest ones
    void Sort( CCollisionCheck & colSort );

    // Get view collision offset
    CPoint & GetFinalViewOffset();

    // Get object collision offset
    CPoint & GetFinalObjectOffset();

    // Floor / wall collision array
    CCollision col[EC_MAX];

protected:

    // Offset of collision
    CPoint offset;

    // Conversion of offset
    CPoint offsetConversion;

    // flag to indicate collision
    bool collision;
};

#endif  // __collision_check_h__