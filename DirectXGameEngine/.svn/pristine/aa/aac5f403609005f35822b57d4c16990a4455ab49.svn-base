/************************************************************************
*    FILE NAME:       objectdata3d.h
*
*    DESCRIPTION:     Class that holds an object's data for later
*					  loading
************************************************************************/

#ifndef __object_data_3d_h__
#define __object_data_3d_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <misc/settings.h>
#include <common/color.h>
#include <3d/objectvisualdata3d.h>
#include <3d/objectcollisiondata3d.h>
#include <3d/objectshadowdata3d.h>

class CObjectData3D
{
public:

    // Access functions for the visual data
    CObjectVisualData3D & GetVisualData() { return visualData; }
    const CObjectVisualData3D & GetVisualData() const { return visualData; }
    void SetVisualData( const CObjectVisualData3D & data ) { visualData = data; }

    // Access functions for the collision data
    CObjectCollisionData3D & GetCollisionData() { return collisionData; }
    const CObjectCollisionData3D & GetCollisionData() const { return collisionData; }
    void SetCollisionData( const CObjectCollisionData3D & data ) { collisionData = data; }

    // Access functions for the collision data
    CObjectShadowData3D & GetShadowData() { return shadowData; }
    const CObjectShadowData3D & GetShadowData() const { return shadowData; }
    void SetShadowData( const CObjectShadowData3D & data ) { shadowData = data; }

    // Access functions for the data name
    const std::string & GetName() const { return name; }
    void SetName( const std::string & _name ) { name = _name; }

    // Access functions for the data group
    const std::string & GetGroup() const { return group; }
    void SetGroup( const std::string & grp ) { group = grp; }

private:

    // Visual data of the object
    CObjectVisualData3D visualData;

    // Collision data of the object
    CObjectCollisionData3D collisionData;

    // Shadow data of object
    CObjectShadowData3D shadowData;

    // The name of the object data
    std::string name;

    // The group the object data is in
    std::string group;
};

#endif  // __object_data_h__
