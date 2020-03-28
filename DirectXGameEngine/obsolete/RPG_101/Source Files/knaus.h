/************************************************************************
*    FILE NAME:       knaus.h
*
*    DESCRIPTION:     Knaus specific character class
************************************************************************/  

#ifndef __knaus_h__
#define __knaus_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include "nonplayercharacter.h"

// Forward declaration(s)


class CKnaus : public CNonPlayerCharacter
{
public:

    CKnaus();
    ~CKnaus();

    // Update the character's actions
    void Update(){};

    // Calculate destinations and actions the character will perform
    void Calculate(){};

    // Move the character in the direction of a target
    //void Move(){};

};

#endif  // __knaus_h__