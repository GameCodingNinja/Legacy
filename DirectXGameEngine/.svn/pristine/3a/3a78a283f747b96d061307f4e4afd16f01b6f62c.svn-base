/************************************************************************
*    FILE NAME:       objectdata2d.h
*
*    DESCRIPTION:     Class that holds a 2D object's data for later
*					  loading
************************************************************************/

#ifndef __object_data_2d_h__
#define __object_data_2d_h__

// Standard lib dependencies
#include <string>
#include <utility>

// Game lib dependencies
#include <common/point.h>
#include <2d/objectvisualdata2d.h>
#include <2d/objectcollisiondata2d.h>
#include <2d/objectscriptdata2d.h>

class CObjectData2D
{
public:

    // Constructor
    CObjectData2D():scale(1,1,1) {}

    // Access functions for the visual data
    CObjectVisualData2D & GetVisualData() { return visualData; }
    const CObjectVisualData2D & GetVisualData() const { return visualData; }
    void SetVisualData( const CObjectVisualData2D & data ) { visualData = data; }

    // Access functions for the collision data
    CObjectCollisionData2D & GetCollisionData() { return collisionData; }
    const CObjectCollisionData2D & GetCollisionData() const { return collisionData; }
    void SetCollisionData( const CObjectCollisionData2D & data ) { collisionData = data; }

    // Access functions for the script data
    CObjectScriptData2D & GetScriptData() {return scriptData;}
    const CObjectScriptData2D & GetScriptData() const { return scriptData; }
    void SetScriptData( const CObjectScriptData2D & data ){ scriptData = data; }

    // Access functions for the data name
    void SetName( const std::string & _name ) { name = _name; }
    const std::string & GetName() const { return name; }

    // Access functions for the data group
    void SetGroup( const std::string & grp ) { group = grp; }
    const std::string & GetGroup() const { return group; }
    
    // Access functions for the scale
    void SetScale( const CPoint & _scale ) { scale = _scale; }
    const CPoint & GetScale() const { return scale; }

private:

    // Visual data of the object
    CObjectVisualData2D visualData;

    // Collision data of the object
    CObjectCollisionData2D collisionData;

    // Script data of the object
    CObjectScriptData2D scriptData;

    // The name of the object data
    std::string name;

    // The group the object data is in
    std::string group;

    // The initial scale of the object
    CPoint scale;
};

#endif  // __object_data_2d_h__
