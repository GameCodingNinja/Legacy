
/************************************************************************
*    FILE NAME:       scriptmanager.h
*
*    DESCRIPTION:     script manager class singleton
************************************************************************/  

#ifndef __script_manager_h__
#define __script_manager_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Game lib dependencies
#include <scripting/commandscript.h>

class CScriptMgr
{
public:

    // Get the instance of the singleton class
    static CScriptMgr & Instance()
    {
        static CScriptMgr scriptMgr;
        return scriptMgr;
    }

    // Load the list table
    void LoadListTable( const std::string & filePath );

    // Load all of the scripts of a specific group
    void LoadScriptGroup( const std::string & group );

    // Get the command script
    CCommandScript * GetCommandScript( const std::string & group, const std::string & name );

    // Delete scripts based on a group
    void DeleteScriptGroup( const std::string & group );

protected:

    // Load the animation end mode
    void SetEndMode( const std::string & filePath, const std::string & commandName, const std::string & endMode );

private:

    CScriptMgr();
    ~CScriptMgr();

    // Load the scripts from an xml
    void LoadFromXML( const std::string & group, const std::string & filePath );

private:

    // Holds the data list table map
    std::map<const std::string, const std::string> scriptListTableMap;
    std::map<const std::string, const std::string>::iterator scriptListTableIter;

    // Map of a map of command script pointers
    boost::ptr_map<const std::string, boost::ptr_map<const std::string, CCommandScript>> spCommandScriptMapMap;
    boost::ptr_map<const std::string, boost::ptr_map<const std::string, CCommandScript>>::iterator commandScriptMapMapIter;
    boost::ptr_map<const std::string, CCommandScript>::iterator cmdScriptMapIter;

};


#endif  // __script_manager_h__
