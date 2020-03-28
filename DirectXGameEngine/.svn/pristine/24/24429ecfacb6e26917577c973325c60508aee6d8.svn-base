
/************************************************************************
*    FILE NAME:       objectscriptdata2d.h
*
*    DESCRIPTION:     Class containing the 2D object's scripting data
************************************************************************/

#ifndef __object_script_data_2d_h__
#define __object_script_data_2d_h__

// Standard lib dependencies
#include <string>

// Forward Declarations
struct XMLNode;

class CObjectScriptData2D
{
public:

    // Load thes object data from node
    void LoadFromNode( const XMLNode & objectNode );
    
    // Get the name of the script
    const std::string & GetName() const;

    // Get the name of the default command
    const std::string & GetCommand() const;

private:

    // Name of the script 
    std::string name;

    // The name of the default command
    std::string defaultCommand;

};

#endif  // __object_script_data_2d_h__