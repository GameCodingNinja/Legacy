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


/************************************************************************
*    desc:  Load the stage from xml file
*
*    param: String filePath - path to xml file
************************************************************************/
void CStageData::LoadFromXML( std::string & filePath )
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

    if( stageSectorsNode.isAttributeSet( "shaderEffect" ) )
        effect = stageSectorsNode.getAttribute( "shaderEffect" );
    
    for( int i = 0; i < stageSectorsNode.nChildNode(); ++i )
    {
        CSectorData * pSectorData = new CSectorData();

        // Put the sector file path into the vector 
        spSectorVec.push_back( pSectorData );

        // Load thes sector data from node
        pSectorData->LoadFromNode( stageSectorsNode.getChildNode( "sector", i ) );
    }

}	// LoadFromXML


/************************************************************************
*    desc:  Get sector data
*
*    param: int index - index of the sector
************************************************************************/
const CSectorData & CStageData::GetSectorData( uint index ) const
{
    if( index >= spSectorVec.size() )
        throw NExcept::CCriticalException("Sector Data get Data Error!",
            boost::str( boost::format("Sector index does not exsist (%d).\n\n%s\nLine: %s") % index % __FUNCTION__ % __LINE__ ));

    return spSectorVec[index];

}	// GetSectorData


/************************************************************************
*    desc:  Get the camera position
************************************************************************/
const CPoint & CStageData::CameraPos() const
{
    return cameraPos;

}	// CameraPos


/************************************************************************
*    desc:  Get the camera roataion
************************************************************************/
const CPoint & CStageData::CameraRot() const
{
    return cameraRot;

}	// CameraRot


/************************************************************************
*    desc:  Get the shader effect
************************************************************************/
const std::string & CStageData::GetEffect() const
{
    return effect;

}	// GetEffect


/************************************************************************
*    desc:  Get Sector count
************************************************************************/
size_t CStageData::GetSectorCount() const
{
    return spSectorVec.size();

}	// GetSectorCount