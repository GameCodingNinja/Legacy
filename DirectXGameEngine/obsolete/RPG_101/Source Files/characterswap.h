/************************************************************************
*    FILE NAME:       characterswap.h
*
*    DESCRIPTION:     Class which will decide what characters will be
*					  used in the current stage
************************************************************************/

#ifndef __characterswap_h__
#define __characterswap_h__

// Standard lib dependencies
#include <string>
#include <vector>

// Physical component dependency
//#include "3d\\objectmanager.h"

// Game dependencies
#include "matchcharacterclass.h"
#include "stagecharactermap.h"

// Forward declarations
class CActor;
class CNonPlayerCharacter;

class CCharacterSwap
{
public:

    CCharacterSwap();
    ~CCharacterSwap();

    // Figure out which decision function will execute
    CCharacterGroup * UpdateCharacters( std::string & stageName );

    // The way characters are chosen based on if the player is in a battle
    void BattleDecisions( std::string & stageName );

    // The way characters are chosen based on if the player is in the environment
    CCharacterGroup * EnvironmentDecisions( std::string & stageName );

    // Load the maps from xml
    void LoadMaps( std::string & filePath );

    // Changes the active characters
    void ChangeCharacters( CActor * pAnimList, std::string & stageName, 
                           std::vector<CNonPlayerCharacter *> & npcVector );

private:

    CStageCharacterMap characterMaps;

    CMatchCharacterClass matching;

};
#endif  // __characterswap_h__