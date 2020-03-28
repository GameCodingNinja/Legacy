/************************************************************************
*    FILE NAME:       characterswap.cpp
*
*    DESCRIPTION:     Class which will decide what characters will be
*					  used in the current stage
************************************************************************/

// Physical component dependency
#include "characterswap.h"

// Game dependencies
#include "3d\\actor.h"
#include "nonplayercharacter.h"
#include "deletefuncs.h"
#include "gamestate.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer
************************************************************************/
CCharacterSwap::CCharacterSwap()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer
************************************************************************/
CCharacterSwap::~CCharacterSwap()
{
}   // Destructer


/************************************************************************
*    desc:  Figure out which decision function will execute        
*
*	 param: int state - state of the game
*			string & stageName - name of the current stage
************************************************************************/
CCharacterGroup * CCharacterSwap::UpdateCharacters( string & stageName )
{
    if( CGameState::Instance().GetGameState() == CGameState::E_STATE_BATTLE )
    {
        //BattleDecisions( stageName );
    }
    else if( CGameState::Instance().GetGameState() == CGameState::E_STATE_ENVIRONMENT )
    {
        return EnvironmentDecisions( stageName );
    }

}	// UpdateCharacters


/************************************************************************
*    desc:  Decide on what characters will be used for a battle
*
*	 param: string & stageName - name of the current stage
************************************************************************/
void CCharacterSwap::BattleDecisions( string & stageName )
{

}	// BattleDecisions


/************************************************************************
*    desc:  Decide on what characters will be used in the environment
*
*	 param: string & stageName - name of the current stage
************************************************************************/
CCharacterGroup * CCharacterSwap::EnvironmentDecisions( string & stageName )
{
    return characterMaps.GetEnvironmentMapElement(stageName)->GetCharacterGroupList()[0];

}	// EnvironmentDecisions


/************************************************************************
*    desc:  Load the maps from xml
*
*	 param: string & filePath - path to the xml file
************************************************************************/
void CCharacterSwap::LoadMaps( string & filePath )
{
    characterMaps.LoadFromXML( filePath );

}	// LoadMaps


/************************************************************************
*    desc:  Changes the active characters
*
*	 param:	CActor * pAnimList - list of animated sprites
*			string & stageName - name of the new stage
*			vector<CNonPlayerCharacter *> & npcVector - vector of NPCs
************************************************************************/
void CCharacterSwap::ChangeCharacters( CActor * pAnimList, string & stageName, vector<CNonPlayerCharacter *> & npcVector )
{
    vector<CCharacterLoadInfo *> tmpInfo = UpdateCharacters( stageName )->GetGroup();

    vector<CGroup *> tmpGroupVector;

    CNonPlayerCharacter * pNPC;

    for( unsigned int i = 0; i < tmpInfo.size(); i++ )
    {
        CGroup * pTmpGroup = new CAnimatedSpriteGroup();

        pTmpGroup->CreateGroupCopy( pAnimList->allObjectInfo[tmpInfo[i]->GetName()] );

        pTmpGroup->SetPos( tmpInfo[i]->GetPos() );

        pTmpGroup->SetPreTransRot( tmpInfo[i]->GetPreTransRot() );

        tmpGroupVector.push_back( pTmpGroup );
        
        pNPC = matching.MatchCharacter( tmpInfo[i]->GetName() );

        pNPC->SetOriginalPos( tmpInfo[i]->GetPos() );

        pNPC->SetCharacter( pTmpGroup );

        npcVector.push_back( pNPC );
    }

    pAnimList->ClearActorVectors();

    pAnimList->SetActorVector( tmpGroupVector );

    //pAnimList->SortActorVectors();

}	// ChangeCharacters