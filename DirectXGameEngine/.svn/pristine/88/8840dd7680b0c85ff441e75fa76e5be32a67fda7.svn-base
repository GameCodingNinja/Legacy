/************************************************************************
*    FILE NAME:       characterlist.h
*
*    DESCRIPTION:     Loads in the list of character group information
************************************************************************/

#ifndef __characterlist_h__
#define __characterlist_h__

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Game dependencies
#include "charactergroup.h"

class CCharacterList
{
public:

    CCharacterList();
    virtual ~CCharacterList();

    // Returns the list of character groups
    std::vector<CCharacterGroup *> & GetCharacterGroupList();

    // Load the character list xml file
    void LoadFromXML(std::string & filePath);

protected:

    std::vector<CCharacterGroup *> list;

};
#endif  // __animatedspritelist_h__