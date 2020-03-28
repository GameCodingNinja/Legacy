/************************************************************************
*    FILE NAME:       collisionstats.cpp
*
*    DESCRIPTION:     CollisionStats class
************************************************************************/

// Physical component dependency
#include "collisionstats.h"

// DirectX lib dependencies
#include "d3dx9.h"

// Game lib dependencies
#include "xmlParser\\xmlParser.h"
#include "misc\\genfunc.h"
#include "misc\\deletefuncs.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCollisionStats::CCollisionStats()
                :floorPad(0.0f),
                 wallPad(0.0f)
{
}


/************************************************************************
*    desc:  Destructor                                                             
************************************************************************/
CCollisionStats::~CCollisionStats()
{
}

void CCollisionStats::AddColideWith( string & spriteName )
{
    colideWith.push_back( spriteName );
}

void CCollisionStats::SetFloorPad( float floor )
{
    floorPad = floor;
}

void CCollisionStats::SetWallPad( float wall )
{
    wallPad = wall;
}

vector<string> & CCollisionStats::GetColideWith()
{
    return colideWith;
}

float CCollisionStats::GetFloorPad()
{
    return floorPad;
}

float CCollisionStats::GetWallPad()
{
    return wallPad;
}