
/************************************************************************
*    FILE NAME:       objectscriptdata2d.cpp
*
*    DESCRIPTION:     Class containing the 2D object's scripting data
************************************************************************/

// Physical component dependency
#include <2d/objectscriptdata2d.h>

// Game lib dependencies
#include <utilities/xmlparsehelper.h>


/************************************************************************
*    desc:  Load thes object data from node
************************************************************************/
void CObjectScriptData2D::LoadFromNode( const XMLNode & objectNode )
{
    const XMLNode scriptNode = objectNode.getChildNode( "script" );

    if( !scriptNode.isEmpty() )
    {
        if( scriptNode.isAttributeSet("name") )
            name = scriptNode.getAttribute( "name" );


        if( scriptNode.isAttributeSet("command") )
            defaultCommand = scriptNode.getAttribute( "command" );
    }

}	// LoadFromNode


/************************************************************************
*    desc:  Get the name of the script
************************************************************************/
const std::string & CObjectScriptData2D::GetName() const
{
    return name;
}


/************************************************************************
*    desc:  Get the name of the default command
************************************************************************/
const std::string & CObjectScriptData2D::GetCommand() const
{
    return defaultCommand;
}
