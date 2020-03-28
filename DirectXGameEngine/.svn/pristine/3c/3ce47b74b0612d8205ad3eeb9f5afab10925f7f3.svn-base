/************************************************************************
*    FILE NAME:       commandscript.h
*
*    DESCRIPTION:     Class containing all of the commands in an xml
************************************************************************/  

#ifndef __command_script_h__
#define __command_script_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_map.hpp>

// Game lib dependencies
#include <xmlParser/xmlParser.h>

// Forward declarations
class CCommand;

class CCommandScript : public boost::noncopyable
{
public:

    CCommandScript();
    CCommandScript( CCommandScript * pCopy );
    ~CCommandScript();

    // Load the script from node
    void LoadFromNode( XMLNode & node );

    // Get the command
    CCommand * CCommandScript::GetCommand( const std::string & cmd );

private:

    // Map of commands
    boost::ptr_map<std::string, CCommand> spCommandMap;
    boost::ptr_map<std::string, CCommand>::iterator commandMapIter;

};

#endif	// __command_script_h__