
/************************************************************************
*    FILE NAME:       matuvlistmanager.h
*
*    DESCRIPTION:     Singleton class to manager the UV values of materials
************************************************************************/

#ifndef __mat_uv_list_manager_h__
#define __mat_uv_list_manager_h__

// Standard lib dependencies
#include <map>
#include <string>

// Game lib dependencies
#include "matuvlist.h"

// Boost lib dependencies

class CMatUVListManager
{
public:

    // get the instance
    static CMatUVListManager & Instance();

    // Load the UV lists from an XML
    bool LoadFromXML( std::string & filePath );

protected:

    // Constructor
    CMatUVListManager();

    // Destructor
    ~CMatUVListManager();

    // Map of UV lists
    std::map<std::string, CMatUVList> uvListMap;

};

#endif  // __mat_uv_list_manager_h__


