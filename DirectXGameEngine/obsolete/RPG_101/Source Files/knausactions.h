/************************************************************************
*    FILE NAME:       knausactions.h
*
*    DESCRIPTION:     Knaus specific actions
************************************************************************/  

#ifndef __knausactions_h__
#define __knausactions_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include "actions.h"

class CKnausActions : public CActions
{
public:

    CKnausActions();
    ~CKnausActions();

    // Most, if not all, characters will be able to dodge and block
    void Dodge( CCharacter * pChar );
    void Block( CCharacter * pChar );

protected:

};

#endif  // __knausactions_h__