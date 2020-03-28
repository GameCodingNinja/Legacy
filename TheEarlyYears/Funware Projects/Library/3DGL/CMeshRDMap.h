
/************************************************************************
*
*    PRODUCT:         Mesh interior map class
*
*    FILE NAME:       CMeshRDMap.h
*
*    DESCRIPTION:     Mesh container for maps based on rooms and doors
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __CMESH_ROOM_DOOR_MAP_H__
#define __CMESH_ROOM_DOOR_MAP_H__

#include "CMesh.h"         // Base mesh class

#define MAX_ROOMS_PER_DOOR      2


class CMeshRDMap : public CMesh
{
protected:

    // room id's attached to this door
    int roomToDoor[MAX_ROOMS_PER_DOOR];

	// Virtual function for loading inherited class specific data
    virtual bool LoadExtraMeshData( FILE *hFile );
    // Virtual function for saving inherited class specific data
    virtual bool SaveExtraMeshData( FILE *hFile );
	// Allocate the polygon array
    virtual CPolygon * AllocatePolygonArray();

public:

    CMeshRDMap();
    virtual ~CMeshRDMap();

    /////////////////
    // Set functions
    /////////////////


    /////////////////
    // Get functions
    /////////////////

    // Get data value
    int GetAttachedRoom( int index ){ return roomToDoor[index]; };
};

#endif // __CMESH_ROOM_DOOR_MAP_H__