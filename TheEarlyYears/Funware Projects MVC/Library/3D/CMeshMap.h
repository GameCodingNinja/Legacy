
/************************************************************************
*
*    FILE NAME:       CMeshMap.h
*
*    DESCRIPTION:     Mesh container for maps
*
************************************************************************/

#ifndef __CMESH_MAP_H__
#define __CMESH_MAP_H__

#include "CMesh.h"         // Base mesh class

// enum room data
enum EMD_MAP_DATA
{
	EMD_ROOM_1,
	EMD_ROOM_2,
	EMD_MAX_ROOMS_PER_DOOR,
	EMD_MAP_ID = EMD_MAX_ROOMS_PER_DOOR, // index number given by modeler
	EMD_MAP_FLOOR,   // floor index
	EMD_MAX_MAP_DATA
};


class CMeshMap : public CMesh
{
protected:

    // Map related extra data
    EMD_MAP_DATA mapData[EMD_MAX_MAP_DATA];
	// flag indicating this object is to be collision checked
	bool collisionChecked;

    // Virtual function for loading inherited class specific data
    virtual bool LoadExtraMeshData( FILE *hFile );
    // Virtual function for saving inherited class specific data
    virtual bool SaveExtraMeshData( FILE *hFile );

public:

    CMeshMap();
    virtual ~CMeshMap();

    // Depending on the angle of the normal, set the polygon
    // Collision type to wall or floor.
    virtual void SetWallFloorCollisionType();

    /////////////////
    // Set functions
    /////////////////

    // Set collision test value
    void SetCollisionChecking( bool value ){ collisionChecked = value; };

    /////////////////
    // Get functions
    /////////////////

    // Get the door point for angle compare
    int GetRoomIndex( int index ){ return mapData[index]; };
	// Get collision test value
	bool GetCollisionChecking(){ return collisionChecked; };
	// Get the map id given by the modeler
    int GetMapId(){ return mapData[EMD_MAP_ID]; };
	// Get the floor id
    int GetFloorID(){ return mapData[EMD_MAP_FLOOR]; };
};

#endif // __CMESH_MAP_H__