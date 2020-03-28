
/************************************************************************
*    FILE NAME:       instancemeshmanager.cpp
*
*    DESCRIPTION:     Singleton to manage instance meshes
************************************************************************/

// Physical component dependency
#include <managers/instancemeshmanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <2d/instancemesh2d.h>
#include <2d/actorsprite2d.h>
#include <2d/spritegroup2d.h>
#include <2d/visualsprite2d.h>
#include <utilities/exceptionhandling.h>


/************************************************************************
*    desc:  Constructer
************************************************************************/
CInstanceMeshManager::CInstanceMeshManager()
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CInstanceMeshManager::~CInstanceMeshManager()
{
}	// destructer


/************************************************************************
*    desc:  Get the instance mesh pointer
*
*	 param: const string & meshName		   - name of the instance mesh
************************************************************************/
CInstanceMesh2D * CInstanceMeshManager::GetInstanceMeshPtr( const std::string & group )
{
    spInstanceMeshMapIter = spInstanceMeshMap.find( group );

    // If the group already exists, we give the sprite the pointer to the instance mesh
    if( spInstanceMeshMapIter != spInstanceMeshMap.end() )
        return spInstanceMeshMapIter->second;
        
    // If the group doesn't exist, throw an exception
    else
        throw NExcept::CCriticalException("Get Instance Mesh Pointer",
                boost::str( boost::format("Instance mesh name does not exist (%s).\n\n%s\nLine: %s") % group % __FUNCTION__ % __LINE__ ));
        
    return NULL;

}	// GetInstanceMeshPtr


/************************************************************************
*    desc:  Create an instance mesh
*
*	 param: const string & group		   - group name
************************************************************************/
void CInstanceMeshManager::CreateInstanceMesh( const std::string & group )
{
    CInstanceMesh2D * pInstanceMesh = new CInstanceMesh2D();
    pInstanceMesh->Init( group );

    spInstanceMeshMap.insert( group, pInstanceMesh );

}	// CreateInstanceMesh


void CInstanceMeshManager::CreateInstanceMesh( const std::string & group, const std::string & megatextureName )
{
    CInstanceMesh2D * pInstanceMesh = new CInstanceMesh2D();
    pInstanceMesh->Init( megatextureName );

    spInstanceMeshMap.insert( group, pInstanceMesh );

}	// CreateInstanceMesh


/************************************************************************
*    desc:  Initialize the passed in sprite to an instance mesh
*
*	 param:	const string & meshname - name of the instance mesh
*			CSpriteGroup2D/CVisualSprite2D/CActorSprite2D * pSprite
*				- sprite to initialize
************************************************************************/
void CInstanceMeshManager::InitInstanceSprite( const std::string & group, CActorSprite2D * pActorSprite )
{
    // Make sure the passed in sprite isn't null
    if( pActorSprite != NULL )
    {
        // Make all of the actor's sprites instance sprites
        for( uint i = 0; i < pActorSprite->GetSpriteGroupCount(); ++i )
            InitInstanceSprite( group, pActorSprite->GetSpriteGroup(i) );
    }

}	// InitInstanceSprite

void CInstanceMeshManager::InitInstanceSprite( const std::string & group, CSpriteGroup2D * pSpriteGrp )
{
    // Make sure the passed in sprite isn't null
    if( pSpriteGrp != NULL )
    {
        spInstanceMeshMapIter = spInstanceMeshMap.find( group );

        // If the group already exists, we give the sprite the pointer to the instance mesh
        if( spInstanceMeshMapIter != spInstanceMeshMap.end() )
            pSpriteGrp->SetInstanceMesh( spInstanceMeshMapIter->second );
        
        // If the group doesn't exist, throw an exception
        else
            throw NExcept::CCriticalException("Get Instance Mesh Pointer",
                boost::str( boost::format("Instance mesh name does not exist (%s).\n\n%s\nLine: %s") % group % __FUNCTION__ % __LINE__ ));
    }

}	// InitInstanceSprite


/************************************************************************
*    desc:  Render an instance mesh
*
*	 param:	const string & meshName - mesh to render
************************************************************************/
void CInstanceMeshManager::Render( const std::string & group )
{
    spInstanceMeshMapIter = spInstanceMeshMap.find( group );

    if( spInstanceMeshMapIter != spInstanceMeshMap.end() )
        spInstanceMeshMapIter->second->Render();

}	// Render


/************************************************************************
*    desc:  Render all instance meshes
************************************************************************/
void CInstanceMeshManager::RenderAll()
{
    for( spInstanceMeshMapIter = spInstanceMeshMap.begin();
             spInstanceMeshMapIter != spInstanceMeshMap.end();
             ++spInstanceMeshMapIter )
    {
        spInstanceMeshMapIter->second->Render();
    }

}	// RenderAll


/************************************************************************
*    desc:  Clear the render multi maps
************************************************************************/
void CInstanceMeshManager::ClearRender()
{
    spInstanceMeshMapIter = spInstanceMeshMap.begin();

    while( spInstanceMeshMapIter != spInstanceMeshMap.end() )
    {
        spInstanceMeshMapIter->second->Clear();
        ++spInstanceMeshMapIter;
    }

}	// ClearRender


/************************************************************************
*    desc:  Remove an instance mesh
*
*	 param:	string & meshName - mesh to remove
************************************************************************/
void CInstanceMeshManager::RemoveInstanceMesh( const std::string & group )
{
    spInstanceMeshMapIter = spInstanceMeshMap.find( group );

    if( spInstanceMeshMapIter != spInstanceMeshMap.end() )
        spInstanceMeshMap.erase( spInstanceMeshMapIter );

}	// RemoveInstanceMesh


/************************************************************************
*    desc:  Clear all of the entries in the manager
************************************************************************/
void CInstanceMeshManager::Clear()
{
    spInstanceMeshMap.clear();

    int sder = 56;

}	// Clear