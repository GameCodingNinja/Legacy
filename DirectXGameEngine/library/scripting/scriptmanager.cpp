
/************************************************************************
*    FILE NAME:       scriptmanager.cpp
*
*    DESCRIPTION:     script class singleton
************************************************************************/

// Physical component dependency
#include <scripting/scriptmanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <scripting/commandscript.h>
#include <xmlParser/xmlParser.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CScriptMgr::CScriptMgr()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CScriptMgr::~CScriptMgr()
{
}   // Destructer


/************************************************************************
*    desc:  Load the list table
*
*	 param:	string & filePath - file path of the object data list XML
************************************************************************/
void CScriptMgr::LoadListTable( const string & filePath )
{
    if( scriptListTableMap.empty() )
    {
        // this open and parse the XML file:
        const XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "scriptListTable" );

        for( int i = 0; i < mainNode.nChildNode(); ++i )
        {
            const XMLNode dataListNode = mainNode.getChildNode( "scriptList", i );

            scriptListTableMap.insert( make_pair(
                dataListNode.getAttribute( "groupName" ), 
                dataListNode.getAttribute( "file" ) ) );
        }
    }

}	// LoadListTable


/************************************************************************
*    desc:  Get a command script from the map
*  
*    ret:	CCommandScript * - pointer to a command script
************************************************************************/
CCommandScript * CScriptMgr::GetCommandScript( const string & group, const string & name )
{
    // Create the map group if it doesn't already exist
    commandScriptMapMapIter = spCommandScriptMapMap.find( group );
    if( commandScriptMapMapIter != spCommandScriptMapMap.end() )
    {
        // See if this script has already been loaded
        cmdScriptMapIter = commandScriptMapMapIter->second->find( name );
    
        if( cmdScriptMapIter != commandScriptMapMapIter->second->end() )
            return cmdScriptMapIter->second;
    }

    throw NExcept::CCriticalException("Script Manager Error!",
                boost::str( boost::format("Script name could not be found (%s).\n\n%s\nLine: %s") % name.c_str() % __FUNCTION__ % __LINE__ ));

    return NULL;

}	// GetCommandScript


/************************************************************************
*    desc:  Load the scripts from an xml
*  
*    param: string & filePath - file path of the script list
************************************************************************/
void CScriptMgr::LoadFromXML( const string & group, const string & filePath )
{
    // this open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "scriptList" );

    XMLNode listGroupNode = mainNode.getChildNode( "listGroup" );
    string listGroup = listGroupNode.getAttribute( "name" );

    // Create the map group if it doesn't already exist
    commandScriptMapMapIter = spCommandScriptMapMap.find( listGroup );
    if( commandScriptMapMapIter == spCommandScriptMapMap.end() )
    {
        spCommandScriptMapMap.insert( listGroup, new boost::ptr_map<const std::string, CCommandScript> );
        commandScriptMapMapIter = spCommandScriptMapMap.find( listGroup );
    }

    for( int i = 0; i < mainNode.nChildNode("script"); ++i )
    {
        XMLNode stageNode = mainNode.getChildNode( "script", i );

        // Get the command script's name and file path
        string name = stageNode.getAttribute( "name" );
        string file = stageNode.getAttribute( "file" );

        // See if this script has already been loaded
        cmdScriptMapIter = commandScriptMapMapIter->second->find( name );

        // If it's not found, load the script and add it to the map
        if( cmdScriptMapIter == commandScriptMapMapIter->second->end() )
        {
            // Make a temporary command script
            CCommandScript * pCmdScript = new CCommandScript();

            // Insert the command script into the map
            commandScriptMapMapIter->second->insert( name, pCmdScript );

            // open and parse the XML file:
            XMLNode node = XMLNode::openFileHelper( file.c_str(), "commandScripts" );

            // Load the command script
            pCmdScript->LoadFromNode( node );
        }
    }

}	// LoadFromXML


/************************************************************************
*    desc:  Load all of the scripts of a specific group
*
*    param: string & group - specified group of scripts to load
************************************************************************/
void CScriptMgr::LoadScriptGroup( const string & group )
{
    // Make sure the group we are looking has been defined in the list table file
    scriptListTableIter = scriptListTableMap.find( group );
    if( scriptListTableIter != scriptListTableMap.end() )
    {
        // Load the group data if it doesn't already exist
        commandScriptMapMapIter = spCommandScriptMapMap.find( group );
        if( commandScriptMapMapIter == spCommandScriptMapMap.end() )
            LoadFromXML( scriptListTableIter->first, scriptListTableIter->second );
    }

}	// LoadScriptGroup


/************************************************************************
*    desc:  Delete scripts based on a group
*  
*    param: string & key
************************************************************************/
void CScriptMgr::DeleteScriptGroup( const string & group )
{
    // Free the vert buffer group if it exists
    commandScriptMapMapIter = spCommandScriptMapMap.find( group );
    if( commandScriptMapMapIter != spCommandScriptMapMap.end() )
        spCommandScriptMapMap.erase( commandScriptMapMapIter );

}	// DeleteVisualMeshGroup






