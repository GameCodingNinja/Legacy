
/************************************************************************
*    FILE NAME:       spritedata.cpp
*
*    DESCRIPTION:     Sprite Class
************************************************************************/

// Physical component dependency
#include <common/spritedata.h>

// Game lib dependencies
#include <xmlParser/xmlParser.h>
#include <utilities/xmlparsehelper.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSpriteData::CSpriteData()
           : useCamera(false)
{
}   // constructor


/************************************************************************
*    desc:  Load the sector data from node
************************************************************************/
void CSpriteData::LoadFromNode( const XMLNode & spriteDataNode )
{
    // Get the group this sprite belongs to
    group = spriteDataNode.getAttribute( "group" );

    // Get the type of object
    type = spriteDataNode.getAttribute( "type" );

    // Object's unique tag name, if it has one
    if( spriteDataNode.isAttributeSet("uniqueMeshTag") )
        uniqueMeshTag = spriteDataNode.getAttribute( "uniqueMeshTag" );

    // Object's name, if it has one
    if( spriteDataNode.isAttributeSet("name") )
        name = spriteDataNode.getAttribute( "name" );

    // Load the transform data
    transData = NParseHelper::LoadTransformData( spriteDataNode );

}	// LoadFromXML

 
/************************************************************************
*    desc:  Get the transform data
*
*	 ret:  const CTransformData & - data
************************************************************************/
const CTransformData & CSpriteData::GetTransformData() const
{
    return transData;

}	// GetTransformData


/************************************************************************
*    desc:  Set the group
*
*	 param: const std::string & str - group to set
************************************************************************/
void CSpriteData::SetGroup( const std::string & str)
{
    group = str;

}	// SetGroup


/************************************************************************
*    desc:  Get the group
*
*	 ret:	const std::string & - group
************************************************************************/
const std::string & CSpriteData::GetGroup() const
{
    return group;

}	// GetGroup


/************************************************************************
*    desc:  Set the type
*
*	 param: const std::string & str - type to set
************************************************************************/
void CSpriteData::SetType( const std::string & str)
{
    type = str;

}	// SetType


/************************************************************************
*    desc:  Get the type
*
*	 ret:	const std::string & - type
************************************************************************/
const std::string & CSpriteData::GetType() const
{
    return type;

}	// GetType


/************************************************************************
*    desc:  Set the unique mesh tag
*
*	 param: const std::string & str - tag to set
************************************************************************/
void CSpriteData::SetUniqueMeshTag( const std::string & str)
{
    uniqueMeshTag = str;

}	// SetUniqueMeshTag


/************************************************************************
*    desc:  Get the unique mesh tag
*
*	 ret:	const std::string & - mesh tag
************************************************************************/
const std::string & CSpriteData::GetUniqueMeshTag() const
{
    return uniqueMeshTag;

}	// GetUniqueMeshTag


/************************************************************************
*    desc:  Set the name
*
*	 param: const std::string & str - name to set
************************************************************************/
void CSpriteData::SetName( const std::string & str)
{
    name = str;

}	// SetName


/************************************************************************
*    desc:  Get the name
*
*	 ret:	const std::string & - name
************************************************************************/
const std::string & CSpriteData::GetName() const
{
    return name;

}	// GetName


/************************************************************************
*    desc:  Set whether the sprite uses the camera
*
*	 param:	bool value - value to set
************************************************************************/
void CSpriteData::SetUseCamera( bool value )
{
    useCamera = value;

}	// SetUseCamera


/************************************************************************
*    desc:  Does this sprite use the camera
*
*	 ret:	bool - result
************************************************************************/
bool CSpriteData::UseCamera() const
{
    return useCamera;

}	// UseCamera