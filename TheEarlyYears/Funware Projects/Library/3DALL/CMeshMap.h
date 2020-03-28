
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

#define MAX_ROOMS_PER_DOOR      2
#define MAX_MAP_DATA			MAX_ROOMS_PER_DOOR


class CMeshMap : public CMesh
{
protected:

    // Map related extra data
    int mapData[MAX_MAP_DATA];
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

    // Get data value
    int GetMapData( int index ){ return mapData[index]; };
	// Get collision test value
	bool GetCollisionChecking(){ return collisionChecked; };
};

#endif // __CMESH_MAP_H__