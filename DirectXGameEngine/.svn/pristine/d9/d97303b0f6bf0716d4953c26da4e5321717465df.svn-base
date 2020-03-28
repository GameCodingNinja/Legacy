/************************************************************************
*    FILE NAME:       charactermanager.cpp
*
*    DESCRIPTION:     Character manager class
************************************************************************/

// Game lib dependencies
#include "3d\\actor.h"
#include "deletefuncs.h"

// Physical component dependency
#include "charactermanager.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCharacterManager::CCharacterManager()
{
}	// Constructor


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCharacterManager::~CCharacterManager()
{
    DeleteVectorPointers( npcVector );

}	// Destructor


/************************************************************************
*    desc:  Update the character manager                                                             
************************************************************************/
void CCharacterManager::Update()
{
    for( unsigned int i = 0; i < npcVector.size(); i++ )
    {
        npcVector[i]->Wander(50.f);
    }

}	// Constructor


/************************************************************************
*    desc:  Return the current environment character group
*
*	 ret:	CCharacterGroup * group - active group of characters
************************************************************************/
CCharacterGroup * CCharacterManager::ReturnCharacterGroup( string & stageName )
{
    return characterSwap.UpdateCharacters( stageName );

}	// ReturnCharacterGroup


/************************************************************************
*    desc:  Change the characters
*
*	 ret:	CActor * list - list of all animated sprites
            string & stageName - name of the current stage
************************************************************************/
void CCharacterManager::SwapCharacters( CActor * list, string & stageName )
{
    // Whenever swaping characters we want to get rid of the pointers to the previous ones
    DeleteVectorPointers( npcVector );
    npcVector.clear();

    characterSwap.ChangeCharacters( list, stageName, npcVector );

}	// SwapCharacters


/************************************************************************
*    desc:  Loads the character group lists from the xml
*
*	 ret:	CCharacterGroup * group - active group of characters
************************************************************************/
void CCharacterManager::LoadCharacterGroupLists( string & filePath )
{
    characterSwap.LoadMaps( filePath );

}	// LoadCharacterGroupLists