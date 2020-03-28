
/************************************************************************
*    FILE NAME:       enemyshipai.cpp
*
*    DESCRIPTION:     Class enemy ship AI
************************************************************************/

// Physical component dependency
#include "enemyshipai.h"

// Standard lib dependencies

// Boost lib dependencies

// Game lib dependencies
#include <2d/actorsprite2d.h>

// Game dependencies

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)
// disable warning about the use of the "this" pointer in the constructor list
#pragma warning(disable : 4355)

/************************************************************************
*    desc:  Constructer
************************************************************************/
CEnemyShipAI::CEnemyShipAI( CActorSprite2D * _pActor )
            : iAIBase(_pActor)
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CEnemyShipAI::~CEnemyShipAI()
{
}	// destructer





