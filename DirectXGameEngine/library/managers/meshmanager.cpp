/************************************************************************
*    FILE NAME:       meshmanager.h
*
*    DESCRIPTION:     3D mesh loading manager class
************************************************************************/

// Physical component dependency
#include <managers/meshmanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <utilities/genfunc.h>
#include <3d/visualmesh3d.h>
#include <3d/collisionmesh3d.h>
#include <3d/jointanimationmesh3d.h>
#include <2d/visualmesh2d.h>
#include <2d/objectdata2d.h>
#include <3d/objectdata3d.h>
#include <utilities/exceptionhandling.h>

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CMeshMgr::CMeshMgr()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CMeshMgr::~CMeshMgr()
{
}   // Destructer


/************************************************************************
*    desc:  Load the mesh info from RSS file if it hasn't already
*           been loaded. 
*  
*    param: const CObjectData2D * pObjData - object data pointer
*    
*    return: CVisualMesh
************************************************************************/
CMesh3D * CMeshMgr::GetVisualMesh3D( const CObjectData3D * pObjData )
{
    // Create the map group if it doesn't already exist
    visualMesh3DMapMapIter = spVisualMesh3DMapMap.find( pObjData->GetGroup() );
    if( visualMesh3DMapMapIter == spVisualMesh3DMapMap.end() )
    {
        spVisualMesh3DMapMap.insert( pObjData->GetGroup(), new boost::ptr_map<const std::string, CVisualMesh3D> );
        visualMesh3DMapMapIter = spVisualMesh3DMapMap.find( pObjData->GetGroup() );
    }

    // See if the name of the mesh can be found
    visualMesh3DMapIter = visualMesh3DMapMapIter->second->find( pObjData->GetName() );

    // If it's not found, load the mesh and add it to the list
    if( visualMesh3DMapIter == visualMesh3DMapMapIter->second->end() )
    {
        CVisualMesh3D * pMesh = new CVisualMesh3D();

        visualMesh3DMapMapIter->second->insert( pObjData->GetName(), pMesh );

        pMesh->LoadFromRSS( pObjData );

        // NGenFunc::PostDebugMsg("Created Visual Mesh - Group: %s, Key: %s", pData->GetGroup().c_str(), pData->GetName() );

        return pMesh;
    }
    else
        return visualMesh3DMapIter->second;

}   // GetVisualMesh3D


/************************************************************************
*    desc:  Delete a specific mesh
*  
*    param: string & key
************************************************************************/
void CMeshMgr::DeleteVisualMeshGroup3D( const std::string & group )
{
    // Free the mesh group if it exists
    visualMesh3DMapMapIter = spVisualMesh3DMapMap.find( group );
    if( visualMesh3DMapMapIter != spVisualMesh3DMapMap.end() )
        spVisualMesh3DMapMap.erase( visualMesh3DMapMapIter );

}	// DeleteVisualMesh


/************************************************************************
*    desc:  Create 2D visual mesh from size if it hasn't 
*			already been created.
*  
*    param: CObjectData2D & pObjData     - data of the object to get
*			const string & meshUniqueTag - the unique tag for the mesh
*    
*    return: CVisualMesh2D
************************************************************************/
CVisualMesh2D * CMeshMgr::GetVisualMesh2D( const CObjectData2D * pObjData, const std::string & meshUniqueTag )
{
    // Create the map group if it doesn't already exist
    visualMesh2DMapMapIter = spVisualMesh2DMapMap.find( pObjData->GetGroup() );
    if( visualMesh2DMapMapIter == spVisualMesh2DMapMap.end() )
    {
        spVisualMesh2DMapMap.insert( pObjData->GetGroup(), new boost::ptr_map<const std::string, CVisualMesh2D> );
        visualMesh2DMapMapIter = spVisualMesh2DMapMap.find( pObjData->GetGroup() );
    }

    std::string keyStr = meshUniqueTag;

    // See if this mesh has already been loaded with it
    if( !pObjData->GetName().empty() )
        keyStr += pObjData->GetName();

    // Convert the width and height to a string to use as the key for this mesh
    // Cast to int to limit any floating point error
    else
    {
        keyStr += boost::str( boost::format("%dx%d") % (int)pObjData->GetVisualData().GetSize().GetW() % (int)pObjData->GetVisualData().GetSize().GetH() );

        // Throw an exception is there is no size
        if( pObjData->GetVisualData().GetSize().IsEmpty() )
            throw NExcept::CCriticalException("Visual mesh key size of 0x0!",
                boost::str( boost::format("Visual mesh key has no size (%s, %s).\n\n%s\nLine: %s") % pObjData->GetGroup() % pObjData->GetName() % __FUNCTION__ % __LINE__ ));
    }

    visualMesh2DMapIter = visualMesh2DMapMapIter->second->find( keyStr );

    // If it's not found, load the mesh and add it to the list
    if( visualMesh2DMapIter == visualMesh2DMapMapIter->second->end() )
    {
        CVisualMesh2D * pMesh = new CVisualMesh2D();
        visualMesh2DMapMapIter->second->insert( keyStr, pMesh );

        pMesh->LoadFromData( pObjData, meshUniqueTag );

        // NGenFunc::PostDebugMsg("Created Visual Mesh - Group: %s, Key: %s", pData->GetGroup().c_str(), keyStr.c_str() );

        return pMesh;
    }
    else
        return visualMesh2DMapIter->second;

}   // GetVisualMesh2D


/************************************************************************
*    desc:  Delete a meshs based on a group
*  
*    param: string & key
************************************************************************/
void CMeshMgr::DeleteVisualMeshGroup2D( const std::string & group )
{
    // Free the mesh group if it exists
    visualMesh2DMapMapIter = spVisualMesh2DMapMap.find( group );
    if( visualMesh2DMapMapIter != spVisualMesh2DMapMap.end() )
        spVisualMesh2DMapMap.erase( visualMesh2DMapMapIter );

}	// DeleteVisualMeshGroup2D


/************************************************************************
*    desc:  Load the mesh info from RSS file if it hasn't already
*           been loaded. 
*  
*    param: const CObjectData3D * pObjData - object data pointer
*    
*    return: CCollisionMesh
************************************************************************/
CMesh3D * CMeshMgr::GetCollisionMesh3D( const CObjectData3D * pObjData )
{
    // Create the map group if it doesn't already exist
    collisionMesh3DMapMapIter = spCollisionMesh3DMapMap.find( pObjData->GetGroup() );
    if( collisionMesh3DMapMapIter == spCollisionMesh3DMapMap.end() )
    {
        spCollisionMesh3DMapMap.insert( pObjData->GetGroup(), new boost::ptr_map<const std::string, CCollisionMesh3D> );
        collisionMesh3DMapMapIter = spCollisionMesh3DMapMap.find( pObjData->GetGroup() );
    }

    // See if the name of the mesh can be found
    collisionMesh3DMapIter = collisionMesh3DMapMapIter->second->find( pObjData->GetName() );

    // If it's not found, load the mesh and add it to the list
    if( collisionMesh3DMapIter == collisionMesh3DMapMapIter->second->end() )
    {
        CCollisionMesh3D * pMesh = new CCollisionMesh3D;

        collisionMesh3DMapMapIter->second->insert( pObjData->GetName(), pMesh );

        pMesh->LoadFromRSS( pObjData );

        // NGenFunc::PostDebugMsg("Created Visual Mesh - Group: %s, Key: %s", pData->GetGroup().c_str(), pData->GetName() );

        return pMesh;
    }
    else
        return collisionMesh3DMapIter->second;

}   // GetCollisionMesh


/************************************************************************
*    desc:  Delete a specific mesh
*  
*    param: string & key
************************************************************************/
void CMeshMgr::DeleteCollisionMeshGroup3D( const std::string & group )
{
    // Free the mesh group if it exists
    collisionMesh3DMapMapIter = spCollisionMesh3DMapMap.find( group );
    if( collisionMesh3DMapMapIter != spCollisionMesh3DMapMap.end() )
        spCollisionMesh3DMapMap.erase( collisionMesh3DMapMapIter );

}	// DeleteCollisionMeshGroup3D


/************************************************************************
*    desc:  Load the mesh info from RSS file if it hasn't already
*           been loaded. 
*  
*    param: const CObjectData3D * pData - object data
*    
*    return: CJointAnimMesh
************************************************************************/
CMesh3D * CMeshMgr::GetJointAnimMesh3D( const CObjectData3D * pObjData )
{
    // Create the map group if it doesn't already exist
    jointMesh3DMapMapIter = spJointMesh3DMapMap.find( pObjData->GetGroup() );
    if( jointMesh3DMapMapIter == spJointMesh3DMapMap.end() )
    {
        spJointMesh3DMapMap.insert( pObjData->GetGroup(), new boost::ptr_map<const std::string, CJointAnimMesh3D> );
        jointMesh3DMapMapIter = spJointMesh3DMapMap.find( pObjData->GetGroup() );
    }

    // See if the name of the mesh can be found
    jointMesh3DMapIter = jointMesh3DMapMapIter->second->find( pObjData->GetName() );

    // If it's not found, load the mesh and add it to the list
    if( jointMesh3DMapIter == jointMesh3DMapMapIter->second->end() )
    {
        CJointAnimMesh3D * pMesh = new CJointAnimMesh3D();

        jointMesh3DMapMapIter->second->insert( pObjData->GetName(), pMesh );

        pMesh->LoadFromRSS( pObjData );

        // NGenFunc::PostDebugMsg("Created Visual Mesh - Group: %s, Key: %s", pData->GetGroup().c_str(), pData->GetName() );

        return pMesh;
    }
    else
        return jointMesh3DMapIter->second;
    
}   // GetJointMesh


/************************************************************************
*    desc:  Delete a specific mesh
*  
*    param: string & key
************************************************************************/
void CMeshMgr::DeleteJointMeshGroup3D( const std::string & group )
{
    // Free the mesh group if it exists
    jointMesh3DMapMapIter = spJointMesh3DMapMap.find( group );
    if( jointMesh3DMapMapIter != spJointMesh3DMapMap.end() )
        spJointMesh3DMapMap.erase( jointMesh3DMapMapIter );

}	// DeleteJointMeshGroup3D


/************************************************************************
*    desc:  Dump out informatrion
************************************************************************/
void CMeshMgr::Dump()
{
    NGenFunc::PostDebugMsg( "Mesh Manager Dump" );

    if( !spVisualMesh3DMapMap.empty() )
    {
        NGenFunc::PostDebugMsg( "Visual Mesh Map 3D - Group Count: %d", spVisualMesh3DMapMap.size() );

        for( visualMesh3DMapMapIter = spVisualMesh3DMapMap.begin();
             visualMesh3DMapMapIter != spVisualMesh3DMapMap.end();
             ++visualMesh3DMapMapIter )
        {
            NGenFunc::PostDebugMsg( "  Group: %s, count: %d", visualMesh3DMapMapIter->first.c_str(), visualMesh3DMapMapIter->second->size() );

            for( visualMesh3DMapIter = visualMesh3DMapMapIter->second->begin();
                 visualMesh3DMapIter != visualMesh3DMapMapIter->second->end();
                 ++visualMesh3DMapIter )
            {
                NGenFunc::PostDebugMsg( "    Name: %s", visualMesh3DMapIter->first.c_str() );
            }
        }
    }

    if( !spJointMesh3DMapMap.empty() )
    {
        NGenFunc::PostDebugMsg( "Animated Joint Mesh Map 3D - Group Count: %d", spJointMesh3DMapMap.size() );

        for( jointMesh3DMapMapIter = spJointMesh3DMapMap.begin();
             jointMesh3DMapMapIter != spJointMesh3DMapMap.end();
             ++jointMesh3DMapMapIter )
        {
            NGenFunc::PostDebugMsg( "  Group: %s, count: %d", visualMesh3DMapMapIter->first.c_str(), visualMesh3DMapMapIter->second->size() );

            for( jointMesh3DMapIter = jointMesh3DMapMapIter->second->begin();
                 jointMesh3DMapIter != jointMesh3DMapMapIter->second->end();
                 ++jointMesh3DMapIter )
            {
                NGenFunc::PostDebugMsg( "    Name: %s", jointMesh3DMapIter->first.c_str() );
            }
        }
    }

    if( !spCollisionMesh3DMapMap.empty() )
    {
        NGenFunc::PostDebugMsg( "Collision Mesh Map 3D - Group Count: %d", spCollisionMesh3DMapMap.size() );

        for( collisionMesh3DMapMapIter = spCollisionMesh3DMapMap.begin();
             collisionMesh3DMapMapIter != spCollisionMesh3DMapMap.end();
             ++collisionMesh3DMapMapIter )
        {
            NGenFunc::PostDebugMsg( "  Group: %s, count: %d", collisionMesh3DMapMapIter->first.c_str(), collisionMesh3DMapMapIter->second->size() );

            for( collisionMesh3DMapIter = collisionMesh3DMapMapIter->second->begin();
                 collisionMesh3DMapIter != collisionMesh3DMapMapIter->second->end();
                 ++collisionMesh3DMapIter )
            {
                NGenFunc::PostDebugMsg( "    Name: %s", collisionMesh3DMapIter->first.c_str() );
            }
        }
    }

    if( !spVisualMesh2DMapMap.empty() )
    {
        NGenFunc::PostDebugMsg( "Visual Mesh Map 2D - Group Count: %d", spVisualMesh2DMapMap.size() );

        for( visualMesh2DMapMapIter = spVisualMesh2DMapMap.begin();
             visualMesh2DMapMapIter != spVisualMesh2DMapMap.end();
             ++visualMesh2DMapMapIter )
        {
            NGenFunc::PostDebugMsg( "  Group: %s, count: %d", visualMesh2DMapMapIter->first.c_str(), visualMesh2DMapMapIter->second->size() );

            for( visualMesh2DMapIter = visualMesh2DMapMapIter->second->begin();
                 visualMesh2DMapIter != visualMesh2DMapMapIter->second->end();
                 ++visualMesh2DMapIter )
            {
                NGenFunc::PostDebugMsg( "    Name: %s", visualMesh2DMapIter->first.c_str() );
            }
        }
    }
    /*
    if( !spCollisionMesh2DMapMap.empty() )
    {
        NGenFunc::PostDebugMsg( "Collision Mesh Map 2D - Group Count: %d", spCollisionMesh2DMapMap.size() );

        for( collisionMesh2DMapMapIter = spCollisionMesh2DMapMap.begin();
             collisionMesh2DMapMapIter != spCollisionMesh2DMapMap.end();
             ++collisionMesh2DMapMapIter )
        {
            NGenFunc::PostDebugMsg( "  Group: %s, count: %d", collisionMesh2DMapMapIter->first.c_str(), collisionMesh2DMapMapIter->second->size() );

            for( collisionMesh2DMapIter = collisionMesh2DMapMapIter->second->begin();
                 collisionMesh2DMapIter != collisionMesh2DMapMapIter->second->end();
                 ++collisionMesh2DMapIter )
            {
                NGenFunc::PostDebugMsg( "    Name: %s", collisionMesh2DMapIter->first.c_str() );
            }
        }
    }
    */

}	// Dump




