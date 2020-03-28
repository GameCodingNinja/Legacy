
/************************************************************************
*    FILE NAME:       craftitemlist.h
*
*    DESCRIPTION:     Class for loading craft data
************************************************************************/

#ifndef __craft_item_list_h__
#define __craft_item_list_h__

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Game lib dependencies
#include <common/defs.h>

// Game dependencies local
#include "craftitem.h"

class CCraftItemList
{
public:

    // Get the instance of the singleton class
    static CCraftItemList & Instance()
    {
        static CCraftItemList craftItemList;
        return craftItemList;
    }

    // Load information from an xml
    void LoadFromXML( const std::string & filePath );

private:

    // Constructor
    CCraftItemList()
    {}

    // Map of craft items
    boost::ptr_map<const std::string, CCraftItem> spCraftItemMap;
    boost::ptr_map<const std::string, CCraftItem>::iterator CraftItemMapIter;

};

#endif  // __craft_item_list_h__


