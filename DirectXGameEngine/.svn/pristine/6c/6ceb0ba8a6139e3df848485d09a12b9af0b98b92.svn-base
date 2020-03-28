/************************************************************************
*    FILE NAME:       collisionstats.h
*
*    DESCRIPTION:     CollisionStats class
************************************************************************/

#ifndef __collisionstats_h__
#define __collisionstats_h__

// Standard lib dependencies
#include <string>
#include <vector>

class CCollisionStats
{
public:

    CCollisionStats();
    ~CCollisionStats();
    
    void AddColideWith( std::string & spriteName );
    void SetFloorPad( float floor );
    void SetWallPad( float wall );

    std::vector<std::string> & GetColideWith();
    float GetFloorPad();
    float GetWallPad();

private:

    std::vector<std::string> colideWith;
    float floorPad;
    float wallPad;
};
#endif  // __collisionstats_h__