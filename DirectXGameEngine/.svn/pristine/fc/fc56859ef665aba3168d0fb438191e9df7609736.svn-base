/************************************************************************
*    FILE NAME:       collision.h
*
*    DESCRIPTION:     collision class
************************************************************************/  

#ifndef __collision_h__
#define __collision_h__

#include <common/point.h>

#define MAX_DISTANCE 1000.0f

class CCollision
{
public:

    CCollision();
    ~CCollision();

    // Reset the class data
    void Reset();

    // Was there a collision? This only indicates if the 
    // bounding sphere check was true
    bool IsCollision();

    // This indicates that the plane being tested was pierced
    bool IsPlanePierce();

    // Smallest collision check distance between object and surface
    float distance;

    // index of face collided with
    int index;

    // amount the plane was pierced by
    float planePierceOffset;

    // translated normal of plane
    CPoint normal;

    // translated triangle center point
    CPoint center;

    // Offset of collision
    CPoint offset;
};

#endif  // __collision_h__