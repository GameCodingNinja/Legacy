/************************************************************************
*    FILE NAME:       sector3D.cpp
*
*    DESCRIPTION:    Class that holds a map of all object data used for
*					 later loading
************************************************************************/

// Physical component dependency
#include <3d/sector3d.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Game lib dependencies
#include <xmlParser/xmlParser.h>
#include <common/point.h>
#include <3d/objectdatalist3d.h>
#include <3d/objectdata3d.h>
#include <3d/spritegroup3d.h>
#include <3d/animatedspritegroup3d.h>
#include <3d/light.h>
#include <utilities/deletefuncs.h>
#include <utilities/xmlparsehelper.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CSector3D::CSector3D()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CSector3D::~CSector3D()
{
    NDelFunc::DeleteVectorPointers(allGroupsVector);

}   // Destructer


/************************************************************************
*    desc:  load the meshs from xml file
*
*    param: String filePath - path to xml file
************************************************************************/
void CSector3D::LoadFromXML( const string & filePath )
{
    // this open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "sector" );
    XMLNode objectListNode = mainNode.getChildNode("objectList");

    for( int i = 0; i < objectListNode.nChildNode(); ++i )
    {
        XMLNode objectNode = objectListNode.getChildNode( "object", i );

        // If there's no object node, return
        if( objectNode.isEmpty() )
            return;

        // Get the type of object and what group it's in
        string objType = objectNode.getAttribute( "type" );
        string objGroup = objectNode.getAttribute( "group" );

        // Get the object data
        CObjectData3D * pObjData = CObjectDataList3D::Instance().GetData( objGroup, objType );

        // Get the object's parent, if it has one.
        // NOTE: Currently not doing anything with this yet
        string objParent;
        if( objectNode.isAttributeSet("parent") )
            objParent = objectNode.getAttribute("parent");

        // Allocates a sprite group
        allGroupsVector.push_back( new CSpriteGroup3D( pObjData ) );


        /***********************************************************
        *	OBJECT SHADOW
        ***********************************************************/
        if( pObjData->GetShadowData().GetCastShadow() )
        {
            const CObjectShadowData3D & shadowData = pObjData->GetShadowData();

            if( shadowData.GetProjectionType() == CSettings::EPT_PERSPECTIVE )
            {
                allGroupsVector.back()->GetVisualSprite().SetPerspectiveProjMatrix( 
                    shadowData.GetMinZDist(), shadowData.GetMaxZDist(), shadowData.GetBias() );
            }
            else if( shadowData.GetProjectionType() == CSettings::EPT_ORTHOGRAPHIC )
            {
                allGroupsVector.back()->GetVisualSprite().SetOrthographicProjMatrix( 
                    shadowData.GetMinZDist(), shadowData.GetMaxZDist(), shadowData.GetOrthoScrnPercent(), shadowData.GetBias() );
            }
        }

        // Apply the position and rotations to the pTmpSpriteGroup
        allGroupsVector.back()->SetPos( CPoint( NParseHelper::LoadPosition( objectNode ) ) );
        allGroupsVector.back()->SetRot( NParseHelper::LoadRotation( objectNode ) );
        allGroupsVector.back()->SetScale( NParseHelper::LoadScale( objectNode ) );

        // If the object has collision, place the pointer here
        if( pObjData->GetCollisionData().GetFile() != "" )
            allColVector.push_back( allGroupsVector.back() );

        // If the object collides with the player, place the pointer here
        //if( pObjData->GetCollisionData().GetPlayerCollision() )
            //colPlayerVector.push_back( allGroupsVector.back() );

        // If the object collides with other objects, place the pointer here
        //if( pObjData->GetCollisionData().GetObjectCollision() )
            //colObjVector.push_back( allGroupsVector.back() );

        // If the object reacts to gravity, place the pointer here
        //if( pObjData->GetCollisionData().GetGravity() )
            //gravityVector.push_back( allGroupsVector.back() );

        // If the object contains alpha gradiant textures that need to be sorted, place the pointer here
        if( pObjData->GetVisualData().GetFile() != "" )
        {
            if( pObjData->GetVisualData().IsAlpha() )
                alphaVector.push_back( allGroupsVector.back() );
            // Otherwise the pointer is placed here
            else
                nonAlphaVector.push_back( allGroupsVector.back() );
        }

        // If the object is animated, place the pointer here
        if( pObjData->GetVisualData().IsAnimated() )
        {
            allGroupsVector.back()->SetAnimation( pObjData->GetVisualData().GetDefaultAnim() );
            animatedVector.push_back( allGroupsVector.back() );
        }

        // Add objects to be rendered to the shadow map buffer
        if( pObjData->GetShadowData().GetCastShadow() )
            shadowCastVector.push_back( allGroupsVector.back() );
    }


    /**********************************************************
    *	LIGHTS
    ***********************************************************/

    // Load the lights in the xml
    XMLNode lightListNode = mainNode.getChildNode("lightList");

    for( int i = 0; i < lightListNode.nChildNode(); ++i )
    {
        // Make a temporary light
        CLight * pTmpLight;

        // get a node to the light
        XMLNode lightNode = lightListNode.getChildNode("light", i);
        
        // Get the type of light
        string type = lightNode.getAttribute("type");

        // Allocate and set the type
        if( type == "directional" )
            pTmpLight = new CLight( ELT_DIRECTIONAL );

        else if( type == "point_infinite" )
            pTmpLight = new CLight( ELT_POINT_INFINITE );

        else if( type == "point_radius" )
            pTmpLight = new CLight( ELT_POINT_RADIUS );

        // Load the light from xml
        pTmpLight->LoadFromXML( lightNode ); 

        // Add the light to the light map
        lightMap.insert(make_pair(lightNode.getAttribute("name"), pTmpLight));
    }

}	// LoadFromXML


/************************************************************************
*    desc:  Renders the sector's non-alpha objects
*
*	 param:	CMatrix & matrix - passed in matrix class
************************************************************************/
void CSector3D::Render( CMatrix & matrix )
{
    for( size_t i = 0; i < nonAlphaVector.size(); ++i )
        nonAlphaVector[i]->GetVisualSprite().Render( matrix );

}	// Render


/************************************************************************
*    desc:  Renders the sector's alpha objects
*
*	 param:	CMatrix & matrix - passed in matrix class
************************************************************************/
void CSector3D::RenderAlpha( CMatrix & matrix )
{
    for( size_t i = 0; i < alphaVector.size(); ++i )
        alphaVector[i]->GetVisualSprite().Render( matrix );

}	// RenderAlpha


/************************************************************************
*    desc:  Render to shadow map
*
*	 param:	CMatrix & matrix - passed in matrix class
************************************************************************/
void CSector3D::RenderShadowMap( CMatrix & matrix )
{
    for( size_t i = 0; i < shadowCastVector.size(); ++i )
        shadowCastVector[i]->GetVisualSprite().RenderShadowMap( matrix );

}	// RenderShadow


/************************************************************************
*    desc:  Allow objects suseptible to gravity to fall
*
*	 param:	float speed - speed of gravity
************************************************************************/
void CSector3D::ReactToGravity( float speed, float elapsedTime )
{
    for( size_t i = 0; i < gravityVector.size(); ++i )
    {
        gravityVector[i]->IncPos( 0, speed * elapsedTime, 0 );
    }

}	// ReactToGravity


/************************************************************************
*    desc:  Inc the animation timer
************************************************************************/
void CSector3D::Update()
{
    for( size_t i = 0; i < animatedVector.size(); ++i )
    {
        animatedVector[i]->IncAnimationTime();
    }

}	// IncAnimationTime


/************************************************************************
*    desc:  Returns the light list
*
*	 ret:	CLightLst - light list
************************************************************************/
map<std::string, CLight *> & CSector3D::GetLightMap()
{
    return lightMap;

}	// GetLightMap


/************************************************************************
*    desc:  Sets the position of the sector
*
*	 param:	CPointInt & pos - position to set
************************************************************************/
void CSector3D::SetPosition( const CPointInt & pos )
{
    position = pos;

}	// SetPosition


/************************************************************************
*    desc:  Gets the position of the sector
*
*	 param:	CPointInt & - position of the sector
************************************************************************/
CPointInt & CSector3D::GetPosition()
{
    return position;

}	// GetPosition


/************************************************************************
*    desc:  Clear all the data so new data can be loaded
************************************************************************/
void CSector3D::ClearSector()
{
    // Delete the pointers
    NDelFunc::DeleteVectorPointers(allGroupsVector);

    // Clear all object vectors
    allGroupsVector.clear();
    colObjVector.clear();
    colPlayerVector.clear();
    gravityVector.clear();
    allColVector.clear();
    alphaVector.clear();
    nonAlphaVector.clear();
    animatedVector.clear();
    shadowCastVector.clear();

    // Clear the light map
    lightMap.clear();

}	// ClearSector