
/************************************************************************
*    FILE NAME:       instancemeshmanager.h
*
*    DESCRIPTION:     Singleton to manage instance meshes
************************************************************************/

#ifndef __instance_mesh_manager_h__
#define __instance_mesh_manager_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Game lib dependencies
#include <2d/instancemesh2d.h>
#include <common/matrix.h>
#include <common/worldpoint.h>

// Forward declaration(s)
class CSpriteGroup2D;
class CVisualSprite2D;
class CActorSprite2D;


class CInstanceMeshManager
{
public:

    // Get the instance of the singleton class
    static CInstanceMeshManager & Instance()
    {
        static CInstanceMeshManager instanceMeshManager;
        return instanceMeshManager;
    }

    // Get the instance mesh pointer
    CInstanceMesh2D * GetInstanceMeshPtr( const std::string & group );

    // Create an instance mesh
    void CreateInstanceMesh( const std::string & group );
    void CreateInstanceMesh( const std::string & group, const std::string & megatextureName );

    // Initialize the passed in sprite to its appropriate instance mesh
    void InitInstanceSprite( const std::string & meshName, CActorSprite2D * pActorSprite );
    void InitInstanceSprite( const std::string & meshName, CSpriteGroup2D * pSpriteGrp );

    // Render a specific instance mesh
    void Render( const std::string & group );

    // Render all instance meshes
    void RenderAll();

    // Clear the render multi maps of the instance meshes
    void ClearRender();

    // Remove an instance mesh
    void RemoveInstanceMesh( const std::string & group );

    // Clear all of the entries in the manager
    void Clear();

private:

    // Constructor
    CInstanceMeshManager();

    // Destructor
    ~CInstanceMeshManager();

private:

    // Map to hold the instance meshes
    boost::ptr_map< const std::string, CInstanceMesh2D > spInstanceMeshMap;
    boost::ptr_map< const std::string, CInstanceMesh2D >::iterator spInstanceMeshMapIter;
};

#endif  // __instance_mesh_manager_h__


