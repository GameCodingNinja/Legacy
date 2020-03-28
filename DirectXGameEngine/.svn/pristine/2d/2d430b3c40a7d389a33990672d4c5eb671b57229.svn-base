/************************************************************************
*    FILE NAME:       stagedata.cpp
*
*    DESCRIPTION:     Class that contains all objects for a single stage
************************************************************************/

// Physical component dependency
#include <common/stagedata.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <xmlParser/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/exceptionhandling.h>
#include <common/sectordata.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CStageData::CStageData()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CStageData::~CStageData()
{
}   // Destructer


/************************************************************************
*    desc:  Load the stage from xml file
*
*    param: String filePath - path to xml file
************************************************************************/
void CStageData::LoadFromXML( string & filePath )
{
	// This open and parse the XML file:
	XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "stage" );

	// Get the stage's lights
	//XMLNode lightsNode = mainNode.getChildNode( "lightList" );
	//lightsFilePath = lightsNode.getAttribute( "file" );

	XMLNode defcameraNode = mainNode.getChildNode( "defaultCamera" );

	// Get the default camera position
	cameraPos = NParseHelper::LoadPosition( defcameraNode );

	// Get the default camera rotation
	cameraRot = NParseHelper::LoadRotation( defcameraNode );


	/* CURRENTLY NOT BEING USED ****************************************************
	// Get the stage's sky mesh data											
	XMLNode skyNode = mainNode.getChildNode( "skyMesh" );					
	skyFilePath = skyNode.getAttribute( "file" );							
																				
	// Get the sky mesh's position												
	XMLNode skyPosNode = skyNode.getChildNode( "position" );					
	skyPos.x = atof( skyPosNode.getAttribute( "x" ) );					
	skyPos.y = atof( skyPosNode.getAttribute( "y" ) );
	skyPos.z = atof( skyPosNode.getAttribute( "z" ) );

	// Get the sky mesh's rotation
	XMLNode skyRotNode = skyNode.getChildNode( "rotation" );
	skyRot.x = atof( skyRotNode.getAttribute( "x" ) );
	skyRot.y = atof( skyRotNode.getAttribute( "y" ) );
	skyRot.z = atof( skyRotNode.getAttribute( "z" ) );

	// Get the default actors' file
	XMLNode actorNode = mainNode.getChildNode( "defaultActors" );
	defaultActorFile = actorNode.getAttribute( "file" );
	// *****************************************************************************/

	// Get the stage objects
	XMLNode stageSectorsNode = mainNode.getChildNode( "sectors" );
	
    for( int i = 0; i < stageSectorsNode.nChildNode(); ++i )
    {
		CSectorData * pTmpData = new CSectorData();

        // Put the sector file path into the vector 
		spSectorVec.push_back( pTmpData );
		
		XMLNode sectorNode = stageSectorsNode.getChildNode( "sector", i );

		// Get the sector's filepath
		XMLNode fileNode = sectorNode.getChildNode( "filePath" );
		if( !fileNode.isEmpty() )
			pTmpData->filePath = fileNode.getAttribute( "file" );

		// Get the sector's position
		pTmpData->position = NParseHelper::LoadPositionInt( sectorNode );
	}

}	// LoadFromXML


/************************************************************************
*    desc:  Get the number of sectors in the vector
*
*    ret: int - sector count
************************************************************************/
size_t CStageData::GetSectorCount()
{
	return spSectorVec.size();

}	// GetSectorCount


/************************************************************************
*    desc:  Get a sector
*
*    param: int index - index of the sector
************************************************************************/
CSectorData * CStageData::GetSectorData( unsigned int index )
{
	if(index >= spSectorVec.size())
		throw NExcept::CCriticalException("Sector Data get Data Error!",
			boost::str( boost::format("Sector index does not exsist (%d).\n\n%s\nLine: %s") % index % __FUNCTION__ % __LINE__ ));

	return &spSectorVec[index];

}	// GetSectorData