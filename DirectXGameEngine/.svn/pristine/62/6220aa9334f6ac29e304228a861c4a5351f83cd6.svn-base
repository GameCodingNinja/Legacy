
/************************************************************************
*    FILE NAME:       objectcollisiondata3d.h
*
*    DESCRIPTION:     Class containing the 3D object's file data
************************************************************************/

#ifndef __object_collision_data_3d_h__
#define __object_collision_data_3d_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <common/color.h>

// Forward Declarations
struct XMLNode;

class CObjectCollisionData3D
{
public:
    
    // Constructer
    CObjectCollisionData3D();

    // Load thes object data from node
    void LoadFromNode( const XMLNode & objectNode );

    // Get the file path
    const std::string & GetFile() const;

    // Get the mass
    float GetMass() const;

    // Get the inertia
    float GetInertia() const;

    // Get the color
    const CColor & GetColor() const;

private:

    // Mesh file
    std::string file;

    // Values used in physics calculations. Mass is the resistance to move, inertia
    // is the resistance to rotate, and restitution is the percetange of velocity retained
    // upon colliding with this object
    float mass, inertia, restitution;

    // Color of the collision (this is only used for testing purposes)
    CColor color;

};

#endif  // __object_collision_data_3d_h__