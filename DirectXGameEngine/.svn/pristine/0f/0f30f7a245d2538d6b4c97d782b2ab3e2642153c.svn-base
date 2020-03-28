
/************************************************************************
*    FILE NAME:       actordata.cpp
*
*    DESCRIPTION:     CActorData Class
************************************************************************/

// Physical component dependency
#include <common/actordata.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <utilities/xmlparsehelper.h>
#include <2d/objectdatalist2d.h>
#include <utilities/exceptionhandling.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CActorData::CActorData()
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CActorData::~CActorData()
{
}	// destructer


/************************************************************************
*    desc:  Load thes object data from node
************************************************************************/
void CActorData::LoadFromNode( const XMLNode & actorDataNode )
{
    // Get the stage's name and file path
    aiName = actorDataNode.getAttribute( "aiName" );
    std::string file = actorDataNode.getAttribute( "file" );

    if( actorDataNode.isAttributeSet("instance") )
        instance = actorDataNode.getAttribute( "instance" );

    // Load the actor data from xml file
    LoadFromXML( file );
    
}	// LoadFromNode


/************************************************************************
*    desc:  load the data from xml file
*
*    param: String filePath - path to xml file
************************************************************************/
void CActorData::LoadFromXML( const string & filePath )
{
    // this open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "actor" );
    XMLNode spriteListNode = mainNode.getChildNode("spriteDataList");

    // If there's no sprite data node, return false
    if( spriteListNode.isEmpty() )
        throw NExcept::CCriticalException("Actor Sprite Data Load Error!",
            boost::str( boost::format("Object node empty (%s).\n\n%s\nLine: %s") % filePath.c_str() % __FUNCTION__ % __LINE__ ));

    // Find out if the actor uses the camera or not
    bool useCamera = false;
    XMLNode cameraNode = mainNode.getChildNode( "camera" );

    if( !cameraNode.isEmpty() )
        if( string( cameraNode.getAttribute( "use" ) ) == "true" )
            useCamera = true;

    // Get the position of this actor
    pos = NParseHelper::LoadPosition( mainNode );

    for( int i = 0; i < spriteListNode.nChildNode(); ++i )
    {
        XMLNode spriteDataNode = spriteListNode.getChildNode( "spriteData", i );

        // If there's no sprite data node, return false
        if( spriteDataNode.isEmpty() )
            throw NExcept::CCriticalException("Actor Sprite Data Load Error!",
                boost::str( boost::format("Object node empty (%s).\n\n%s\nLine: %s") % filePath.c_str() % __FUNCTION__ % __LINE__ ));

        // Allocate the pointer
        spSpriteDataVec.push_back( new CSpriteData );

        // Load from node
        spSpriteDataVec.back().LoadFromNode( spriteDataNode );

        // Set the use camera bool
        spSpriteDataVec.back().SetUseCamera( useCamera );
    }

    // Load the collision data
    colData.LoadFromNode( mainNode );

}	// LoadFromXML


/************************************************************************
*    desc:  Get the position
************************************************************************/
const CWorldPoint & CActorData::GetPos() const
{
    return pos;
}


/************************************************************************
*    desc:  Get the ai name
************************************************************************/
const std::string & CActorData::GetAIName() const
{
    return aiName;
}


/************************************************************************
*    desc:  Get the instance name
************************************************************************/
const std::string & CActorData::GetInstance() const
{
    return instance;
}


/************************************************************************
*    desc:  Get sprite Data size
************************************************************************/
size_t CActorData::GetSpriteDataSize() const
{
    return spSpriteDataVec.size();
}


/************************************************************************
*    desc:  Get sprite Data - Allow outside functions to change data
************************************************************************/
CSpriteData & CActorData::GetSpriteData( size_t index )
{
    return spSpriteDataVec[index];
}


/************************************************************************
*    desc:  Get the collision data
************************************************************************/
const CObjectCollisionData2D & CActorData::GetCollisionData() const
{
    return colData;
}
