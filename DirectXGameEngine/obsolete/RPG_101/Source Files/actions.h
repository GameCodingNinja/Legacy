/************************************************************************
*    FILE NAME:       actions.h
*
*    DESCRIPTION:     Actions base class
************************************************************************/  

#ifndef __actions_h__
#define __actions_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include "character.h"

class CActions
{
public:

    CActions();
    virtual ~CActions();

    // Set/Get the target ID number
    void SetTargetNum( int num );
    int GetTargetNum();

    // Most, if not all, characters will be able to dodge and block
    //virtual void Dodge( CCharacter * pChar ) = 0;
    //virtual void Block( CCharacter * pChar ) = 0;

protected:

    // The ID number of the character's target
    int targetNum;

};

#endif  // __actions_h__