
/************************************************************************
*
*    PRODUCT:         Map Manager
*
*    FILE NAME:       CMapMgr.h
*
*    DESCRIPTION:     Handle indoor maps. The maps are desiged with rooms
*                     and doors. One door seperates two rooms and if the
*                     door is seen in the view frustrum, the room is processes.
*                     This class has the ability of handling large maps. 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef _CMAP_MGR_H_
#define _CMAP_MGR_H_

#include "CMeshList.h"       // Mesh list class
#include "CMeshMap.h"        // Map specific mesh class
#include "CObjList.h"        // Header file for the template list object.


/////////////////////////////////////////////////
// The below classes are to simplfy the managing
// the rooms and doorways in respect to rendering
// and collision detection.
//
// The list of doors and rooms are put into two
// groups. The room group, you know the doors attached,
// the rooms attached to the doors and all the
// doors attached to those rooms. The door group,
// you know the rooms on both sides of the door
// and the doors that attach to them. 
////////////////////////////////////////////////

// Base class of a room and all the doors it holds.
class CRoomDoors
{
public:
    // Current room in
    CMeshMap *pRoom;
    // List of objects associated with this room
    CObjLst<CMeshMap> objLst;
	// List of objects associated with this room
	// that use alpha textures
    CObjLst<CMeshMap> alphaObjLst;
    // List of doorways seen from room 
    CObjLst<CMeshMap> doorLst;

    CRoomDoors()
    {
        // We don't want the data to be freeded
        // because this class doesn't own it.
        doorLst.DontFreeData();
        objLst.DontFreeData();
		alphaObjLst.DontFreeData();
    }
};


// Class to hold the room, it's list of doors,
// rooms attached to those doors and the doors
// in those rooms
class CRoom : public CRoomDoors
{
public:

    // List of rooms & doors
    CObjLst<CRoomDoors> roomDoorLst;
};


// Class that holds the door, the rooms
// that are attache to it and the doors
// attached to those rooms.
class CDoor
{
public:
    // Doorway
    CMeshMap *pDoor;
    // List of rooms & doors
    CObjLst<CRoomDoors> roomDoorLst;
};



/************************************************************************
*
*    Class for handling lists of meshes
*
************************************************************************/

template <class mesh_type>
class CMapMgr : public CMeshList<mesh_type>
{
protected:

    // index of area we are currently steping on
    int activeIndex;
    // type of area we are currently steping on
    EOT_OBJECT_TYPE activeType;
    // List of rooms with the attached doors and rooms
    CObjLst<CRoom> roomLst;
    // List of doors with the ajoining rooms
    CObjLst<CDoor> doorLst;
    // List of rendered doors for checking if objects
	// in a given room can be seen through the door.
    CObjLst<CMeshMap> rendDoorLst;

    // counters
	int doorCount;
    int roomCount;
	int hallwayCount;
	int stairwayCount;
    int objCount;
    int totalCount;
    
    // We are standing in a room. Render that room with
    // it's doors and ajoining rooms and doors.
    void RenderRoom( CMatrix &m, char *debug );
    // We are standing in a doorway. Render that doorway
    // with it's ajoining rooms and doors.
    void RenderDoor( CMatrix &m, char *debug );
    // We are standing in a doorway. Render that doorway
    // with it's ajoining rooms and doors.
    int RenderObjects( CMatrix &m, CObjLst<CMeshMap> *pObjLst, 
                       int &objCount, bool transformCol = true );
    int RenderObjectsPortal( CMatrix &m, CObjLst<CMeshMap> *pObjLst, 
                             int &objCount, CMeshMap *pMeshPortal = NULL );
    // Inc the counters
    void IncCounters( CMeshMap *pMesh ); 

public:

    CMapMgr();
    virtual ~CMapMgr(){ Free(); }

    // Free the allocations
    void Free();
    // Render only what we can see
    void Render( CMatrix &m, char *debug );
    // Check for collision and react to it
    void ReactToCollision( CMatrix &m, int buttonUpDown, int buttonLeftRight );
    // Make the map ready for renderng
    void SetupMap();
    // Was this area rendered
    bool WasRendered( CMapType &mapType );
    // Is this bounding sphere visible through the list of rendered doorways?
    bool IsVisableThruRenderedDoorways( CMapType &mapType,
                                        float radius, CPoint *pCenter );
	// Set the starting position to the spawn point
	void SetSpawnPoint( CMatrix &matrix );
	// Set the bot spawn point
	void SetToSpawnPoint( CMatrix &matrix,
                          CMapType &mapType,
                          EOT_OBJECT_TYPE spawnPoint,
                          int spawnIndex,
                          float dir );

    // Load the polygon object file
    virtual bool LoadFromFile( char *filePath );
    virtual bool LoadFromFile( FILE *hFile );

    int GetActiveIndex(){ return activeIndex; };
    EOT_OBJECT_TYPE GetActiveType(){ return activeType; };

	// Load the polygon object file
	void ReactToFloorCollisionOnly( CMatrix &matrix,
                                    CMapType &mapType,
									CPoint &objPoint,
                                    float radius,
                                    float offset );

	int CheckForWallCollision( CMapType &mapType,
							   CPoint * objPoint,
                               float radius );
};

/////////////////////////////////////////
//  Must include this here or you will
//  get a bunch of "Unresolved External"
//  errors. Templates are a pain to code
/////////////////////////////////////////
#include "CMapMgr.cpp"

#endif  // _CMAP_MGR_H_