/************************************************************************
*    FILE NAME:       character.cpp
*
*    DESCRIPTION:     Character base class
************************************************************************/

// Game lib dependencies
#include "3d\\animatedspritegroup.h"

// Physical component dependency
#include "character.h"
#include "actions.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCharacter::CCharacter()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCharacter::~CCharacter()
{
}   // Destructer


/************************************************************************
*    desc:  Set the animated sprite group for the character
*
*	 param: CAnimatedSpriteGroup * _character - new character
************************************************************************/
void CCharacter::SetCharacter( CGroup * _pCharacter )
{
    pCharacter = _pCharacter;
}


/************************************************************************
*    desc:  Set the animated sprite group for the character
*
*	 ret:  CAnimatedSpriteGroup _character - new character
************************************************************************/
CGroup * CCharacter::GetCharacter()
{
    return pCharacter;
}