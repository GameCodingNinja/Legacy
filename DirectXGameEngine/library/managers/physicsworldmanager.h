
/************************************************************************
*    FILE NAME:       physicsworld.h
*
*    DESCRIPTION:     Box2D b2World wrapper class
************************************************************************/

#ifndef __physics_world_manager_h__
#define __physics_world_manager_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Game lib dependencies
#include <2d/physicsworld.h>
#include <common/pointint.h>


class CPhysicsWorldManager
{
public:

    // Get the instance of the singleton class
    static CPhysicsWorldManager & Instance()
    {
        static CPhysicsWorldManager worldManager;
        return worldManager;
    }

    // Load the physics worlds from an XML
    void LoadFromXML( const std::string & filePath );

    // Get the physics world
    CPhysicsWorld * GetWorld( const std::string & name );

    // Delete a world
    void DeleteWorld( const std::string & name );

    // Clear the entire physics world manager
    void Clear();

private:

    // Constructor
    CPhysicsWorldManager();

    // Destructor
    ~CPhysicsWorldManager();

private:

    // Map of physics worlds
    boost::ptr_map< const std::string, CPhysicsWorld> spWorldMap;
    boost::ptr_map< const std::string, CPhysicsWorld>::iterator spWorldMapIter;

};

#endif  // __physics_world_manager_h__