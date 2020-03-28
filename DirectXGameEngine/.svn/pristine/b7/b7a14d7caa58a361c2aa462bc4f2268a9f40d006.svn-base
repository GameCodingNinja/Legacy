
/************************************************************************
*    FILE NAME:       hudmanager.h
*
*    DESCRIPTION:     Hud manager class
************************************************************************/

#ifndef __hud_manager_h__
#define __hud_manager_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Game lib dependencies
#include <xmlParser/xmlParser.h>

// Forward declaration(s)
class CUIControl;

class CHudManager
{
public:

    // Get the instance of the singleton class
    static CHudManager & Instance()
    {
        static CHudManager hudManager;
        return hudManager;
    }

    // Load the menu info from XML node
    void LoadListTable( const std::string & filePath );

    // Load the hud
    void LoadHud( const std::string & hudName );

    // Load the hud info from XML node
    void LoadHudFromNode( const XMLNode & node );

    // Get the hud control
    CUIControl * GetHudControl( const std::string & ctrlAction );

    // Reset the dynamic positions of menus
    void ResetDynamicOffset();

    // Unload the hud
    void Clear();

    // Transform the hud
    void Transform();

    // Update the hud elements
    void Update();

    // Do the render
    void Render();

private:

    // Constructor
    CHudManager();

    // Destructor
    virtual ~CHudManager();

    // Load the hud from file
    void LoadHudFromFile( const std::string & hudName, const std::string & filePath );

private:

    // Hud list table
    std::map<std::string, std::string> hudLstTable;
    std::map<std::string, std::string>::iterator lstTblIter; 

    // Map of hud controls
    boost::ptr_map<const std::string, CUIControl> spHudCtrlMap;
    boost::ptr_map<const std::string, CUIControl>::iterator hudMapIter;

};

#endif  // __hud_manager_h__


