/************************************************************************
*    FILE NAME:       sector2d.cpp
*
*    DESCRIPTION:    Class that holds a map of all object data used for
*					 later loading
************************************************************************/

// Physical component dependency
#include <2d/sector2d.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <xmlParser/xmlParser.h>
#include <utilities/deletefuncs.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/exceptionhandling.h>
#include <common/point.h>
#include <2d/objectdatalist2d.h>
#include <2d/objectdata2d.h>
#include <2d/spritegroup2d.h>
#include <managers/instancemeshmanager.h>


/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CSector2D::CSector2D()
         : visible(true)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CSector2D::~CSector2D()
{
    NDelFunc::DeleteVectorPointers(pGroupVector);

}   // Destructer


/************************************************************************
*    desc:  load the meshs from xml file
*
*    param: String filePath - path to xml file
************************************************************************/
void CSector2D::LoadFromXML( const std::string & filePath )
{
    // this open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "sector" );
    XMLNode objectListNode = mainNode.getChildNode("objectLst");

    CInstanceMesh2D * pInstMesh(NULL);
    if( objectListNode.isAttributeSet("instance") )
        pInstMesh = CInstanceMeshManager::Instance().GetInstanceMeshPtr( objectListNode.getAttribute( "instance" ) );

    for( int i = 0; i < objectListNode.nChildNode(); ++i )
    {
        XMLNode objectNode = objectListNode.getChildNode( "object", i );

        // If there's no object node, return
        if( objectNode.isEmpty() )
            return;

        // If there's no object node, return false
        if( objectNode.isEmpty() )
            throw NExcept::CCriticalException("Sector2D Load Error!",
                boost::str( boost::format("XML node is empty (%s).\n\n%s\nLine: %s") % filePath.c_str() % __FUNCTION__ % __LINE__ ));

        // Get the type of object and what group it's in
        std::string objType = objectNode.getAttribute( "type" );
        std::string objGroup = objectNode.getAttribute( "group" );

        // Object's unique tag name, if it has one
        std::string objUniqueTag;
        if( objectNode.isAttributeSet("unique") )
            objUniqueTag = objectNode.getAttribute( "unique" );

        // Get the object's parent, if it has one.
        // NOTE: Currently not doing anything with this yet
        std::string objParent;
        if( objectNode.isAttributeSet("parent") )
            objParent = objectNode.getAttribute("parent");

        // The pointer is placed within a vector and map for all objects
        pGroupVector.push_back( 
            new CSpriteGroup2D( CObjectDataList2D::Instance().GetData( objGroup, objType ), true, objUniqueTag ) );

        // Set the instance mesh pointer
        pGroupVector.back()->SetInstanceMesh( pInstMesh );

        // Load the transform data
        pGroupVector.back()->SetTransData( NParseHelper::LoadTransformData( objectNode ) );

        // If the object has collision, place the pointer here
        if( pGroupVector.back()->GetCollisionData().GetFile() != "" )
            pCollisionVector.push_back( pGroupVector.back() );
    }

}	// LoadFromXML


/************************************************************************
*    desc:  Add a sprite to the sector
*
*	 param:	CSpriteGroup2D * pGroup - sprite group to add
*
*	 NOTE:	If this function is used, the sector is now responsible for
*			the cleanup of this sprite. The class that supplies this
*			pointer is not responsible for it anymore
************************************************************************/
void CSector2D::AddSprite( CSpriteGroup2D * pGroup )
{
    if( pGroup != NULL )
    {
        /** TEST CODE: In this test code we're going to have the object positions be based off of
        *			   sector's positition. So when an object is given to the sector, the object's
        *			   position is moved in relation to the sector. **/
        CPointInt tmp = pGroup->GetPos().GetIntComponent() + i_pos;
        pGroup->SetPos( tmp );
        /** TEST CODE **/

        pGroupVector.push_back( pGroup );

        if( pGroup->IsCollisionActive() )
            pCollisionVector.push_back( pGroup );
    }

}	// AddSprite


/************************************************************************
*    desc:  Removes the sprite from the sector
*
*	 param:	CSpriteGroup2D * pGroup  - sprite group to unload
*			bool deleteSprite = true - whether or not to delete the sprite
*									   after it is removed
************************************************************************/
void CSector2D::RemoveSprite( CSpriteGroup2D * pGroup, bool deleteSprite )
{
    if( pGroup != NULL )
    {
        // Remove the sprite from the group vector
        for( size_t i = 0; i < pGroupVector.size(); ++i )
        {
            if( pGroupVector[i] == pGroup )
            {
                pGroupVector.erase( pGroupVector.begin() + i );
                break;
            }
        }

        // Remove the sprite from the collision vector
        if( pGroup->IsCollisionActive() )
        {
            for( size_t i = 0; i < pCollisionVector.size(); ++i )
            {
                if( pCollisionVector[i] == pGroup )
                {
                    pCollisionVector.erase( pCollisionVector.begin() + i );
                    break;
                }
            }
        }

        if( deleteSprite )
            NDelFunc::Delete( pGroup );
    }

}	// UnloadSprite


/************************************************************************
*    desc:  Transform the sector's components
************************************************************************/
void CSector2D::Transform()
{
    for( size_t i = 0; i < pGroupVector.size(); ++i )
        pGroupVector[i]->Transform();
    
}	// Transform


/************************************************************************
*    desc:  Transform the sector's components
*
*	 param: const CMatrix & _scaledMatrix  - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point      - passed in relative position
************************************************************************/
void CSector2D::Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                           const CWorldPoint & point )
{
    for( size_t i = 0; i < pGroupVector.size(); ++i )
        pGroupVector[i]->Transform( _scaledMatrix, _unscaledMatrix, point );
    
}	// Transform


/************************************************************************
*    desc:  Inc the animation timer
************************************************************************/
void CSector2D::Update()
{
    for( size_t i = 0; i < pGroupVector.size(); ++i )
        pGroupVector[i]->Update();

}	// Update


/************************************************************************
*    desc:  Renders the sector's sprites
************************************************************************/
void CSector2D::Render()
{
    if( visible ) 
        for( size_t i = 0; i < pGroupVector.size(); ++i )
            pGroupVector[i]->Render();

}	// Render


/************************************************************************
*    desc:  Renders all the frames of the sector's sprites in one shot
************************************************************************/
void CSector2D::RenderAllFrames()
{
    if( visible )
    {
        for( size_t i = 0; i < pGroupVector.size(); ++i )
        {
            for( int j = 0; j < pGroupVector[i]->GetFrameCount(); ++j )
            {
                pGroupVector[i]->SetCurrentFrame( j );
                pGroupVector[i]->Render();
            }
        }
    }

}	// Render


/************************************************************************
*    desc:  Sets the position of the sector
*
*	 param:	CPointInt & pos - position to set
************************************************************************/
void CSector2D::SetPosition( const CPointInt & pos )
{
    /** TEST CODE: In this test code we're going to have the object positions be based off of
    *			   sector's positition. So if the sector moves, all other objects move. **/
    CPointInt diff = pos - i_pos;

    for( size_t i = 0; i < pGroupVector.size(); ++i )
        pGroupVector[i]->IncPos( diff );
    
    /** TEST CODE **/

    i_pos = pos;

}	// SetPosition


/************************************************************************
*    desc:  Gets the position of the sector
*
*	 param:	CPointInt & - position of the sector
************************************************************************/
CPointInt & CSector2D::GetPosition()
{
    return i_pos;

}	// GetPosition


/************************************************************************
*    desc:  Clear all the data so new data can be loaded
************************************************************************/
void CSector2D::ClearSector()
{
    // Delete the pointers
    NDelFunc::DeleteVectorPointers(pGroupVector);

    // Clear all object vectors
    pCollisionVector.clear();

}	// ClearSector


/************************************************************************
*    desc:  Get all the sprite groups in the sector
*
*	 ret:	vector<CSpriteGroup2D *> & - group vector
************************************************************************/
std::vector<CSpriteGroup2D *> & CSector2D::GetGroupVector()
{
    return pGroupVector;

}	// GetGroupVector


/************************************************************************
*    desc:  Get a sprite group in the sector
*
*	 ret:	CSpriteGroup2D * - group to get
************************************************************************/
CSpriteGroup2D * CSector2D::GetGroup( int i )
{
    return pGroupVector[i];

}	// GetGroup


/************************************************************************
*    desc:  Get all the sprite groups with collision
*
*	 ret:	vector<CSpriteGroup2D *> & - collision vector
************************************************************************/
std::vector<CSpriteGroup2D *> & CSector2D::GetCollisionVector()
{
    return pCollisionVector;

}	// GetCollisionVector


/************************************************************************
*    desc:  Set the sector's visibility
*  
*    param: int value - frame index
************************************************************************/
void CSector2D::SetVisible( bool value )
{
    visible = value;

}	// SetVisible


/************************************************************************
*    desc:  Is the sector visible
*  
*    ret: bool - frame index
************************************************************************/
bool CSector2D::IsVisible()
{
    return visible;

}	// IsVisible