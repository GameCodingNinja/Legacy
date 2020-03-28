/************************************************************************
*    FILE NAME:       objectdatalist3d.cpp
*
*    DESCRIPTION:    Class that holds a map of all object data used for
*					 later loading
************************************************************************/

// Physical component dependency
#include <3d/objectdatalist3d.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/xmlparsehelper.h>
#include <xmlParser/xmlParser.h>
#include <3d/objectdata3d.h>
#include <3d/spritegroup3d.h>
#include <scripting/scriptmanager.h>
#include <managers/meshmanager.h>
#include <managers/texturemanager.h>


/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CObjectDataList3D::CObjectDataList3D()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CObjectDataList3D::~CObjectDataList3D()
{
}   // Destructer


/************************************************************************
*    desc:  Load the data list table
*
*	 param:	string & filePath - file path of the object data list XML
************************************************************************/
void CObjectDataList3D::LoadListTable( const std::string & filePath )
{
    if( dataListTableMap.empty() )
    {
        // this open and parse the XML file:
        const XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "dataListTable" );

        for( int i = 0; i < mainNode.nChildNode(); ++i )
        {
            const XMLNode dataListNode = mainNode.getChildNode( "dataList", i );

            dataListTableMap.insert( std::make_pair( dataListNode.getAttribute( "groupName" ), 
                                                     dataListNode.getAttribute( "file" ) ) );
        }
    }

}	// LoadListTable


/************************************************************************
*    desc:  Load all of the meshes and materials of a specific data group
*
*    param: string & group - specified group of meshes and materials to load
************************************************************************/
void CObjectDataList3D::LoadDataGroup( const std::string & group )
{
    // Make sure the group we are looking has been defined in the list table file
    dataListTableIter = dataListTableMap.find( group );
    if( dataListTableIter == dataListTableMap.end() )
        throw NExcept::CCriticalException("Obj Data List 3D Load Group Data Error!",
                boost::str( boost::format("Object data list group name can't be found (%s).\n\n%s\nLine: %s") % group % __FUNCTION__ % __LINE__ ));

    // Load the group data if it doesn't already exist
    groupMapIter = spObjectDataMapMap.find( group );
    if( groupMapIter == spObjectDataMapMap.end() )
    {
        LoadFromXML( dataListTableIter->first, dataListTableIter->second );
        groupMapIter = spObjectDataMapMap.find( group );
    }

    // Load the script group
    CScriptMgr::Instance().LoadScriptGroup( group );

    // This loads the texture, visual mesh, collision mesh & vertex buffer
    for( dataMapIter = groupMapIter->second->begin();
         dataMapIter != groupMapIter->second->end();
         ++dataMapIter )
    {
        CSpriteGroup3D tmp( dataMapIter->second );
    }

}	// LoadDataGroup


/************************************************************************
*    desc:  Free all of the meshes and materials of a specific data group
*
*    param: string & group - specified group of meshes and materials to load
************************************************************************/
void CObjectDataList3D::FreeDataGroup( const std::string & group )
{
    // Make sure the group we are looking for exists
    dataListTableIter = dataListTableMap.find( group );
    if( dataListTableIter == dataListTableMap.end() )
        throw NExcept::CCriticalException("Obj Data List 2D Load Group Data Error!",
                boost::str( boost::format("Object data list group name can't be found (%s).\n\n%s\nLine: %s") % group % __FUNCTION__ % __LINE__ ));

    // See if this group is still loaded
    groupMapIter = spObjectDataMapMap.find( group );
    if( groupMapIter != spObjectDataMapMap.end() )
    {
        CTextureMgr::Instance().DeleteTextureGroupFor3D( group );
        CMeshMgr::Instance().DeleteVisualMeshGroup3D( group );
        CMeshMgr::Instance().DeleteJointMeshGroup3D( group );
        CMeshMgr::Instance().DeleteCollisionMeshGroup3D( group );
        CScriptMgr::Instance().DeleteScriptGroup( group );

        // Unload the group data
        spObjectDataMapMap.erase( groupMapIter );
    }

}	// FreeDataGroup


/************************************************************************
*    desc:  Load all object information from an xml
*
*	 param:	string & filePath - file path of the object data list XML
************************************************************************/
void CObjectDataList3D::LoadFromXML( const std::string & group, const std::string & filePath )
{
    // As a sanity check, see if this object has already been loaded
    groupMapIter = spObjectDataMapMap.find( group );

    // Load if it hasn't been already loaded
    if( groupMapIter == spObjectDataMapMap.end() )
    {
        // this open and parse the XML file:
        const XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "objectDataList" );

        // Create a new map inside of our map and get an iterator into it
        spObjectDataMapMap.insert( group, new boost::ptr_map<const std::string, CObjectData3D> );
        groupMapIter = spObjectDataMapMap.find( group );

        // Get the node to the default object data
        const XMLNode defaultNode = mainNode.getChildNode( "default" );

        // Pointer to hold a data list's default data.
        boost::scoped_ptr< CObjectData3D > spDefaultData;

        // If there's no default node then we just use the defaults set in the data classes
        if( !defaultNode.isEmpty() )
        {
            spDefaultData.reset( new CObjectData3D );

            // Load in the object data
            LoadObjectFromNode( defaultNode, spDefaultData.get() );
        }

        XMLNode objectListNode = mainNode.getChildNode( "objectList" );

        for( int i = 0; i < objectListNode.nChildNode(); ++i )
        {
            // Get the object data node
            const XMLNode objectNode = objectListNode.getChildNode( "object", i );

            // Place the object in the map
            CObjectData3D * pObjectData = new CObjectData3D();
        
            // Get the object's name
            std::string objName = objectNode.getAttribute( "name" );

            // Add the object data to the map
            groupMapIter->second->insert( objName, pObjectData );

            // If the default data is set, copy the whole thing
            if( spDefaultData )
                *pObjectData = *spDefaultData;

            // Load in the object data
            LoadObjectFromNode( objectNode, pObjectData );

            // Set the object's name and group before loading the rest of the data
            pObjectData->SetName( objName );
            pObjectData->SetGroup( group );
        }
    }
    else
    {
        throw NExcept::CCriticalException("Obj Data List 2D load Error!",
                boost::str( boost::format("Object data list group name has alread been loaded (%s - %s).\n\n%s\nLine: %s") % group % filePath % __FUNCTION__ % __LINE__ ));
    }

}	// LoadFromXML


/************************************************************************
*    desc:  Load the object information from the passed in node
*
*	 param:	const XMLNode & objectNode  - node of the object
*			CObjectData2D * pObjectData - data to set to
************************************************************************/
void CObjectDataList3D::LoadObjectFromNode( const XMLNode & objectNode, 
                                            CObjectData3D * pObjectData )
{
    // Load the visual data
    pObjectData->GetVisualData().LoadFromNode( objectNode );

    // Load the collision data
    pObjectData->GetCollisionData().LoadFromNode( objectNode );

    // Load the shadow data
    pObjectData->GetShadowData().LoadFromNode( objectNode );

}	// LoadFromXML


/************************************************************************
*    desc:  Get a specific 3D object's data
*
*    param: string & group - the group the 3D object belongs to
*			string & name  - name of the 3D object
*
*	 ret:	CObjectData3D * - 2D object
************************************************************************/
CObjectData3D * CObjectDataList3D::GetData( const std::string & group, const std::string & name )
{
    groupMapIter = spObjectDataMapMap.find( group );
    if( groupMapIter != spObjectDataMapMap.end() )
    {
        dataMapIter = groupMapIter->second->find( name );

        if( dataMapIter != groupMapIter->second->end() )
            return dataMapIter->second;

        throw NExcept::CCriticalException("Obj Data List 3D Get Data Error!",
                boost::str( boost::format("Object data list object name can't be found (%s).\n\n%s\nLine: %s") % name % __FUNCTION__ % __LINE__ ));
    }

    throw NExcept::CCriticalException("Obj Data List 3D Get Data Error!",
                boost::str( boost::format("Object data list group can't be found (%s).\n\n%s\nLine: %s") % group % __FUNCTION__ % __LINE__ ));

    return NULL;

}	// GetData
