/************************************************************************
*    FILE NAME:       actions.cpp
*
*    DESCRIPTION:     Actions base class
************************************************************************/

// Game lib dependencies

// Physical component dependency
#include "actions.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CActions::CActions()
     : targetNum(-1) // A target of -1 will represent no target
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CActions::~CActions()
{
}   // Destructer


/************************************************************************
*    desc:  Set the target of the character's ID number. For the player
*			this is used to determine which enemy to strafe around. For
*			an NPC, this is used in AI calculations.
*
*	 param: int num - new ID number
************************************************************************/
void CActions::SetTargetNum( int num )
{
    targetNum = num;
}


/************************************************************************
*    desc:  Gets the target of the character's ID number. For the player
*			this is used to determine which enemy to strafe around. For
*			an NPC, this is used in AI calculations.
*
*	 ret:   int num - current ID number
************************************************************************/
int CActions::GetTargetNum()
{
    return targetNum;
}