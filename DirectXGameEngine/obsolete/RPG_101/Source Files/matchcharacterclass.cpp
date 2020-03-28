/************************************************************************
*    FILE NAME:       matchcharacterclass.cpp
*
*    DESCRIPTION:     Matches an animated sprite group to a character
*					  specific class
************************************************************************/

// Physical component dependency
#include "matchcharacterclass.h"

// Game dependency
#include "knaus.h"
#include "nonplayercharacter.h"
#include "character.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CMatchCharacterClass::CMatchCharacterClass()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CMatchCharacterClass::~CMatchCharacterClass()
{
}   // Destructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CNonPlayerCharacter * CMatchCharacterClass::MatchCharacter( string & name )
{
    CNonPlayerCharacter * pNPC;

    if( name[0] == 'k' )
    {
        if( name == string("knaus") )
        {
            pNPC = new CKnaus();

            return pNPC;
        }
    }

    return NULL;
}	// ReturnCharacter