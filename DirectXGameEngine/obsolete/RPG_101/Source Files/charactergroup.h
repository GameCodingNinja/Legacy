/************************************************************************
*    FILE NAME:       charactergroup.h
*
*    DESCRIPTION:     Class which holds a group of CCharacterLoadInfo
*					  objects.
************************************************************************/

#ifndef __charactergroup_h__
#define __charactergroup_h__

// Standard lib dependencies
#include <vector>

// Game lib dependencies
#include "characterloadinfo.h"

class CCharacterGroup
{
public:

    CCharacterGroup();
    ~CCharacterGroup();

    // Set-Get functions for the character group
    void SetGroup( std::vector<CCharacterLoadInfo *> & grp );
    std::vector<CCharacterLoadInfo *> & GetGroup();

    // Adds a single character to the group
    void AddCharacter( CCharacterLoadInfo * pCharacter );

private:

    // Vector of the inactive character info
    std::vector<CCharacterLoadInfo *> group;

};
#endif  // __charactergroup_h__