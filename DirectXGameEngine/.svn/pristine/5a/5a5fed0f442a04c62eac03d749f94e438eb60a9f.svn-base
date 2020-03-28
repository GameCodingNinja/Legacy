/************************************************************************
*    FILE NAME:       commandscript.cpp
*
*    DESCRIPTION:     Class containing all of the commands in an xml
************************************************************************/

// Physical component dependency
#include <scripting/commandscript.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependency
#include <scripting/command.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCommandScript::CCommandScript()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCommandScript::~CCommandScript()
{
}   // Destructer


/************************************************************************
*    desc:  Load the script from node
*  
*    param: node
************************************************************************/
void CCommandScript::LoadFromNode( XMLNode & node )
{
    // Get the number of commands 
    int cmdCount = node.nChildNode("command");

    for( int i = 0; i < cmdCount; ++i )
    {
        XMLNode commandNode = node.getChildNode("command", i);

        string cmdName = commandNode.getAttribute("name");

        CCommand * pCommand = new CCommand();
        spCommandMap.insert( cmdName, pCommand );

        pCommand->LoadFromNode( commandNode );
    }

}	// LoadFromNode


/************************************************************************
*    desc:  Copy Constructer                                                             
************************************************************************/
CCommandScript::CCommandScript( CCommandScript * pCopy )
{
    for( commandMapIter = pCopy->spCommandMap.begin();
         commandMapIter != pCopy->spCommandMap.end();
         ++commandMapIter )
    {
        string cmdName = commandMapIter->first;

        CCommand * pTmpCommand = new CCommand(commandMapIter->second);
        spCommandMap.insert( cmdName, pTmpCommand );
    }

}   // Copy Constructer


/************************************************************************
*    desc:  Copy Constructer                                                             
************************************************************************/
CCommand * CCommandScript::GetCommand( const string & cmd )
{
    commandMapIter = spCommandMap.find( cmd );

    if( commandMapIter != spCommandMap.end() )
        return commandMapIter->second;

    return NULL;

}	// GetCommand
