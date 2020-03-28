/************************************************************************
*    FILE NAME:       matchcharacterclass.h
*
*    DESCRIPTION:     Matches an animated sprite group to a character
*					  specific class
************************************************************************/  

#ifndef __matchcharacterclass_h__
#define __matchcharacterclass_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
//#include "nonplayercharacter.h"
//#include "knaus.h"

// Forward declaration(s)
class CKnaus;
class CNonPlayerCharacter;
class CCharacter;

class CMatchCharacterClass
{
public:

    CMatchCharacterClass();
    ~CMatchCharacterClass();

    CNonPlayerCharacter * MatchCharacter( std::string & name );

};

#endif  // __matchcharacterclass_h__