/************************************************************************
*    FILE NAME:       charactermanager.h
*
*    DESCRIPTION:     Character manager class
************************************************************************/  

#ifndef __charactermanager_h__
#define __charactermanager_h__

// Standard lib dependencies
#include <string>
#include <vector>

// Game lib dependencies
//#include "3d\\matrix.h"
//#include "3d\\point.h"
//#include "3d\\animatedspritegroup.h"
#include "characterswap.h"
#include "nonplayercharacter.h"

// Forward declaration
class CActor;

class CCharacterManager
{
public:
    
    CCharacterManager();
    ~CCharacterManager();

    // Update the character manager
    void Update();

    // Return the current environment character group
    CCharacterGroup * ReturnCharacterGroup( std::string & stageName );

    // Change the characters
    void SwapCharacters( CActor * list, std::string & stageName );

    // Loads the character group lists from the xml
    void LoadCharacterGroupLists( std::string & filePath );

private:

    CCharacterSwap characterSwap;
    std::vector<CNonPlayerCharacter *> npcVector;

};	

#endif // __charactermanager_h__