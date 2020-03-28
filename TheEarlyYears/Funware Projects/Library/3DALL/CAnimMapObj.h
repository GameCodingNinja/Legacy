/************************************************************************
*
*    PRODUCT:         Animated map Object
*
*    FILE NAME:       CAnimMapObj.h
*
*    DESCRIPTION:     Animated object that is map aware 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef _CANIM_MAP_H_
#define _CANIM_MAP_H_

#include "CAnimMeshLst.h"       // Animation specific mesh class

/************************************************************************
*
*    Class for handling lists of meshes
*
************************************************************************/

template <class mesh_type>
class CAnimMapObj : public CAnimMeshLst<mesh_type>
{
protected:

	// Map type and index of the area the
	// object is located
    CMapType mapType;
    // class for loading native map files
    CMapMgr<CMeshMap> * pMap;
    // Radius
    float radius;
	// floor offset
	float floorOffset;

public:

	CAnimMapObj();
    virtual ~CAnimMapObj();

    // Set the map the bot is in
	void SetMap( CMapMgr<CMeshMap> * map, float rad, float offset );
	// Render the animation frame
    virtual int Render( CMatrix &m );
	// Use a spawn point to position the animated object
	void SetToSpawnPoint( EOT_OBJECT_TYPE spawnPoint,
                          int spawnIndex );
};


/////////////////////////////////////////
//  Must include this here or you will
//  get a bunch of "Unresolved External"
//  errors. Templates are a pain to code
/////////////////////////////////////////
#include "CAnimMapObj.cpp"

#endif  // _CANIM_MAP_H_
