/************************************************************************
*    FILE NAME:       stagecharactermap.h
*
*    DESCRIPTION:     Loads in the list of character group information
************************************************************************/

#ifndef __stagecharactermap_h__
#define __stagecharactermap_h__

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Game dependencies
#include "characterlist.h"

class CStageCharacterMap
{
public:

    CStageCharacterMap();
    ~CStageCharacterMap();

    // Load the character lists xml file
    bool LoadFromXML(std::string & filePath);

    CCharacterList * GetEnvironmentMapElement( std::string & name );

    // A map of all environmental characters
    std::map<std::string, CCharacterList *> environmentMap;

private:

    

    // A map of all random battle character lists
    std::map<std::string, CCharacterList *> randomBattleMap;

    // A map of all specific battle character lists
    std::map<std::string, CCharacterList *> specificBattleMap;

    enum{E_ENVIRONMENT, E_RANDOM_BATTLE, E_SPECIFIC_BATTLE};

};
#endif  // __animatedspritelist_h__