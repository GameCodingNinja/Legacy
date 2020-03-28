/************************************************************************
*    FILE NAME:       sectordata.cpp
*
*    DESCRIPTION:     Class that contains all objects for a single sector
************************************************************************/

// Physical component dependency
#include <common/sectordata.h>

// Game lib dependencies
#include <xmlParser/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <common/sectordata.h>


/************************************************************************
*    desc:  Load the sector data from node
************************************************************************/
void CSectorData::LoadFromNode( const XMLNode & sectorNode )
{
    // Get the sector's filepath
    XMLNode fileNode = sectorNode.getChildNode( "filePath" );
    if( !fileNode.isEmpty() )
        file = fileNode.getAttribute( "file" );

    // Get the sector's position
    position = NParseHelper::LoadPositionInt( sectorNode );

}	// LoadFromXML


/************************************************************************
*    desc:  Get the file path
************************************************************************/
const std::string & CSectorData::GetFile() const
{
    return file;

}	// GetFile


/************************************************************************
*    desc:  World position of the sector
************************************************************************/
const CPointInt & CSectorData::GetPos() const
{
    return position;

}	// GetPos