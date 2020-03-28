/************************************************************************
*    FILE NAME:       collision.cpp
*
*    DESCRIPTION:     collision class
************************************************************************/

// Physical component dependency
#include <3d/collision.h>

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCollision::CCollision()
{
    Reset();

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCollision::~CCollision()
{
}   // Destructer


/************************************************************************
*    desc:  Reset the class data                                                             
************************************************************************/
void CCollision::Reset()
{
    index = -1;
    distance = MAX_DISTANCE;
    planePierceOffset = 0.0f;
    normal.Clear();
    center.Clear();
    offset.Clear();
}


/************************************************************************
*    desc:  Was there a collision? This only indicates if the bounding
*           sphere check was true because the index value would point
*           to a face index
*
*    ret:  bool - true if there was a collision
************************************************************************/
bool CCollision::IsCollision()
{
    return (index > -1);
}


/************************************************************************
*    desc:  This indicates that the plane being tested was pierced
*
*    ret:  bool - true if plane was pierced
************************************************************************/
bool CCollision::IsPlanePierce()
{
    return (planePierceOffset < 0.0f);
}