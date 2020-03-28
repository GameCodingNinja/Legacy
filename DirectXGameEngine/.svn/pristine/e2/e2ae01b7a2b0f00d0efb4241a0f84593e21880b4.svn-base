/************************************************************************
*    FILE NAME:       character.h
*
*    DESCRIPTION:     Character base class
************************************************************************/  

#ifndef __character_h__
#define __character_h__

// Standard lib dependencies
#include <string>

// Forward declaration(s)
class CActions;
class CGroup;

class CCharacter
{
public:

    CCharacter();
    virtual ~CCharacter();

    // Set/Get the character
    void SetCharacter(CGroup * _pCharacter);
    CGroup * GetCharacter();

protected:

    // A pointer to the animated sprite group the class uses
    CGroup * pCharacter;

    CActions * pActions;
};

#endif  // __character_h__