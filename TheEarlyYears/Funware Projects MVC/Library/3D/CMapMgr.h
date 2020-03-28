
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
#include "CMatrix.h"         // Matrix class
#include "types.h"           // misc type specifiers
#include <vector>
#include <algorithm>
#include "FreePtr.h"
#include "CGraphList.h"      // Graph template class
#include "CPathNode.h"       // Node class
#include "CPlayer.h"		 // player class
#include "CAnimMeshLst.h"
#include "deletefuncs.h"
#include "CMapLightLst.h"
#include "CBaseCamera.h"

#include "tagfileparser/datadefnode.h"
#include "tagfileparser/tokenizer.h"
#include "tagfileparser/tagfileparser.h"

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)


/////////////////////////////////////////////////
// The below class is to simplfy the managing
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

// Base class of a rooms and doors
// The doorLst is only used for room areas
class CArea
{
public:
    // Current area we are stepping on
    CMeshMap *pArea;
	
	// List of doorways seen from room 
    std::deque<CMeshMap *> doorLst;
	// list of path nodes in this room
	std::deque<CPathNode *> nodeLst;
    // List of objects associated with this room
    std::vector<CAnimMeshLst<CAnimMesh> *> objLst;
	// Light list for objects. Reusable objects need dynamic lighting
	CLightList *pLightList;
    
	// List of rooms & doors
    std::deque<CArea *> areaLst;

	CArea()
	{
		pArea = NULL;
		pLightList = NULL;
	}

	~CArea()
	{
		DeleteVectorPointers( objLst );
	}
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

    // List of rooms with the attached doors and rooms
    std::deque<CArea *> roomLst;
    // List of doors with the ajoining rooms
    std::deque<CArea *> doorLst;
    // List of rendered doors for checking if objects
	// in a given room can be seen through the door.
    std::deque<CMeshMap *> rendDoorLst;
	// List of rendered map area elements to
	// reset it's render flag
	std::deque<CMeshMap *> rendAreaLst;

    // counters
	int doorCount;
    int roomCount;
	int hallwayCount;
	int stairwayCount;
    int objCount;
    int totalCount;
    
    // We are standing in a room. Render that room with
    // it's doors and ajoining rooms and doors.
    void RenderRoom( int activeIndex, CMatrix &m, char *debug );
    // We are standing in a doorway. Render that doorway
    // with it's ajoining rooms and doors.
    void RenderDoor( int activeIndex, CMatrix &m, char *debug );
    // We are standing in a doorway. Render that doorway
    // with it's ajoining rooms and doors.
    int RenderObjects( CMatrix &m, CArea * pArea, int &objCount );
    int RenderObjectsPortal( CMatrix &m, CArea * pArea, 
                             int &objCount, CMeshMap *pMeshPortal = NULL );
    // Inc the counters
    void IncCounters( CMeshMap *pMesh ); 

public:

    CMapMgr();
    virtual ~CMapMgr();

    // Render only what we can see
    void Render( CMapType &mapType, CMatrix &m, char *debug );
    // Check for collision and react to it
    void ReactToCollision( CMapType &mapType,
    					   CMatrix &m, 
    					   float fpRadious,
						   double distFromFloor,
						   double distFromWall,
						   int buttonUpDown,
						   int buttonLeftRight );
    // Make the map ready for renderng
    void SetupMap();
    // Was this area rendered
    bool WasRendered( CMapType &mapType );
    // Is this bounding sphere visible through the list of rendered doorways?
    bool IsVisableThruRenderedDoorways( CMapType &objMapType, CMapType &playerMapType,
                                        float radius, CPoint *pCenter );
	// Set the spawn point
	bool SetSpawnPoint( CMapType &mapType,
						CMatrix &matrix,
                        EOT_OBJECT_TYPE spawnPoint,
                        int spawnIndex,
                        float dir );
	bool SetSpawnPoint( CMapType &mapType,
		 		        CMatrix &matrix,
		 		        CPoint trans,
		 		        CPoint rot,
						int floorId,
                        int mapId,
                        float dir );

	// Load the polygon object file
	void BotFloorCollision( CMatrix &matrix,
                            CMapType &mapType,
							CPoint &objPoint,
                            float radius );
	// Add the nodes to the rooms
	bool AddGraphNodesToRooms( CGraphList <CPathNode> & graph );
	// Get the node list
	std::deque<CPathNode *> &GetNodeLst( CMapType &mapType );
	// Get the mesh
	mesh_type *GetMesh( CMapType &mapType );
	// Set the objects to the rooms
	bool SetObjsToRooms( const std::string & tagFilename,
		 		         CAnimMeshLst<CAnimMesh> &mapObjects );
	// Set the list of light data to all meshes
    virtual void SetLightList( CLightList * pLiteLst );
	// Set the lights to the meshes
	bool SetLightsToMesh( std::vector<CMapLightList *> &mapLightLst );
	// Clear the mesh lights
	virtual void ClearMeshLights();
};


/************************************************************************
*    FUNCTION NAME:  Constructer 
************************************************************************/
template <class mesh_type>
CMapMgr<mesh_type>::CMapMgr()
{
}


/************************************************************************
*    DESCRIPTION:  Free the allocations
************************************************************************/
template <class mesh_type>
CMapMgr<mesh_type>::~CMapMgr()
{
	DeleteDequePointers( roomLst );
	DeleteDequePointers( doorLst );
}


/************************************************************************
*    DESCRIPTION:  Set the list of light data to all meshes
*
*    Input:    CLightList &lightList - Pointer to light list
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::SetLightList( CLightList * pLiteLst )
{
    for( int i = 0; i < meshCount; ++i )
    {
        // only set the light list if the pointer is null
        if( pMesh[i].GetLightList() == NULL )
		{
        	pMesh[i].SetLightList( pLiteLst );
		}
    }
}   // SetLightList


/************************************************************************                                                             
*    DESCRIPTION:  Render only what we can see. If we can see a
*                  doorway, then render the other possible room.
*
*    Input:    CMatrix &M - Matris to do all the translations
*              char *debug -  debug info
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::Render( CMapType &mapType, CMatrix &m, char *debug )
{
    #ifdef _render_all_
    RenderAll( m );
    #else

    // init the counters
    doorCount = 0;
    roomCount = 0;
    hallwayCount = 0;
    stairwayCount = 0;
    objCount = 0;
    totalCount = 0;
	CMatrix::InitTransCounter();

    // Clear out all the render flags
	for( unsigned int i = 0; i < rendAreaLst.size(); ++i )
	{
		rendAreaLst[i]->ResetRenderFlag();
		rendAreaLst[i]->ResetCullTestFlag();
		rendAreaLst[i]->ResetTransformedFlag();	
	}

	// free all rendered elements list
	rendAreaLst.clear();

    // free all saved rendered doors
    rendDoorLst.clear();

    // We are standing on a polygon in a doorway
    if( mapType.activeType == EOT_MAP_DOORWAY ||
        mapType.activeType == EOT_MAP_HALLDOORWAY )
    {        
        RenderDoor( mapType.activeIndex, m, debug );
    }
    // We are standing on a polygon in a room, hallway or stairway
    else
    {
        RenderRoom( mapType.activeIndex, m, debug );
    }

    sprintf( debug, "Door: %d;   hallway: %d;   Room: %d;   Stairway: %d;   Obj Count: %d;   PCount: %d;   VCount: %d",
             doorCount, hallwayCount, roomCount, stairwayCount, objCount, totalCount, CMatrix::GetTransCount() );
    #endif

}   // Render

/************************************************************************                                                             
*    DESCRIPTION:  We are standing in a room. Render that room with
*                  it's doors and ajoining rooms and doors.
*
*    Input:    CMeshMap *pMesh - Mest to be counted
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::IncCounters( CMeshMap *pMesh )
{
    // Inc the counters if not allready rendered
    if( !pMesh->WasRendered() )
    {
        if( pMesh->GetType() == EOT_MAP_HALLWAY )
        {
            ++hallwayCount;
        }
        else if( pMesh->GetType() == EOT_MAP_ROOM )
        {
            ++roomCount;
        }
        else if( pMesh->GetType() == EOT_MAP_STAIRWAY )
        {
            ++stairwayCount;
        }
    }
}   // IncCounters


/************************************************************************                                                             
*    DESCRIPTION:  We are standing in a room. Render that room with
*                  it's doors and ajoining rooms and doors.
*
*    Input:    Matrix3D &M - Matris to do all the translations
*              char *debug -  debug info
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::RenderRoom( int activeIndex, CMatrix &m, char *debug )
{
    // Render the active area first
    CMeshMap *pMesh = roomLst[ activeIndex ]->pArea;

    // Inc the debug counters
    IncCounters( pMesh );

	// Add to rendered list
	rendAreaLst.push_back( pMesh );

    // render this area
    totalCount += pMesh->Render( m );

    // Render all the objects in this room
    totalCount += RenderObjects( m, roomLst[ activeIndex ], objCount );

    // Get a reference to the door list
    std::deque<CMeshMap *> &tmpDoorLst = roomLst[ activeIndex ]->doorLst;

    // Transform all the doors in the room we are standing in.
    // This way we can test for it later and not reprocess
    // the same doors that are part of the ajoining rooms.
	for( std::deque<CMeshMap *>::iterator it = tmpDoorLst.begin(); it != tmpDoorLst.end(); ++it )
    {
        // Must transform the center point first before we do anything
        // because many functions rely on the translated center point.
        (*it)->TransformCenterPt( m );

        // Transfor the door so that it can be collision checked.
		// Have to do it anyways because we always need to test for it
        (*it)->Transform( m );

		// Add to rendered list
		rendAreaLst.push_back( (*it) );
    }

    // Find the doors that are connected to this room
    // and check if we can see them
    for( unsigned int i = 0; i < tmpDoorLst.size(); ++i )
    {
        // Get a pointer to the door mesh
        CMeshMap *pDoorMesh = tmpDoorLst[ i ];

        bool closeToDoorWay = false;

        // If the doorway was NOT culled, render it
        // along with the ajoining room
        if( !pDoorMesh->WasRendered() && !pDoorMesh->CullMesh_BoundSphere() )
        {
            // Add the rendered door to our list
            rendDoorLst.push_back( pDoorMesh );

            // Render the doorway
            totalCount += pDoorMesh->Render( m, false );
            ++doorCount;

            // Get the other room attached to this door
            CMeshMap *pRoomMesh = roomLst[ activeIndex ]->areaLst[ i ]->pArea;                

            // Inc the debug counters
            IncCounters( pRoomMesh );

			// Add to rendered list
			rendAreaLst.push_back( pRoomMesh );
            
            // Only render the polygons we can see through the doorway
            // Check to see how close we are to the doorway 
            if( pDoorMesh->GetDistance() > pDoorMesh->GetRadius() )
            {                
                // Two or more doors can connect to the same room so make sure we only
                // translate once.
                totalCount += pRoomMesh->Render( m,
                								 pDoorMesh->GetRadius(),
                                                 pDoorMesh->GetCenterPt(), 
                                                 !pRoomMesh->WasTransformed() );

                // Render all objects in this room
                totalCount += RenderObjectsPortal( m,
                								   roomLst[ activeIndex ]->areaLst[ i ], 
                                                   objCount, pDoorMesh );
            }
            // The portal is useless when your right on top of it so don't bother using
            // with the rooms in front and behind us.
            else
            {                
                totalCount += pRoomMesh->Render( m,
                								 !pRoomMesh->WasTransformed() );
                closeToDoorWay = true;

                // Render all objects in this room
                totalCount += RenderObjects( m,
                							 roomLst[ activeIndex ]->areaLst[ i ], 
                                             objCount );
            }

            // Get the doors attached to the inner room
            std::deque<CMeshMap *> &tmpInnerDoorLst = roomLst[ activeIndex ]->areaLst[ i ]->doorLst;

            // Try to render the doorways in this room
            for( unsigned int j = 0; j < tmpInnerDoorLst.size(); ++j )
            {
                CMeshMap *innerDoorMesh = tmpInnerDoorLst[ j ];

				// Add to rendered list
				rendAreaLst.push_back( innerDoorMesh );

                if( !innerDoorMesh->WasRendered() )
                {
                    // Must transform the center point first before we do anything
                    // because many functions rely on the translated center point.
                    innerDoorMesh->TransformCenterPt( m );

                    bool innerDoorCulled;

                    // Cull the doorway from the current doorway.
                    // Depending on how close we are to the doorway, we'll cull as if we are looking
                    // through the door or not.
                    if( !closeToDoorWay )
                    {
                        innerDoorCulled = innerDoorMesh->CullMesh_BoundSphere( pDoorMesh->GetRadius(),
                                                                               pDoorMesh->GetCenterPt() );
                    }
                    else
                    {
                        innerDoorCulled = innerDoorMesh->CullMesh_BoundSphere();
                    }

                    if( !innerDoorCulled )
                    {
                        // Don't render doors that were already transformed. All the doors connected
                        // to the room we are standing in have been pre-transformed for culling
                        // purposes. These doors are also part of the inner list and should not
                        // be rendered because we test for rendering of the closest doors.
                        if( !innerDoorMesh->WasTransformed() )
                        {
                            // Add the rendered door to our list
                            rendDoorLst.push_back( innerDoorMesh );

                            if( !closeToDoorWay )
                            {
                                totalCount += innerDoorMesh->Render( m,
                                									 pDoorMesh->GetRadius(),
                                                                     pDoorMesh->GetCenterPt() );
                            }
                            else
                            {
                                totalCount += innerDoorMesh->Render( m );
                            }

                            ++doorCount;
                        }

                        // render the room associated with this door.
                        // one of these rooms has already been rendered
                        for( int w = 0; w < EMD_MAX_ROOMS_PER_DOOR; ++w )
                        {
                            CMeshMap *pInnerRoomMesh = roomLst[ innerDoorMesh->GetRoomIndex(w) ]->pArea;

                            // Inc the debug counters
                            IncCounters( pInnerRoomMesh );

							// Add to rendered list
							rendAreaLst.push_back( pInnerRoomMesh );

                            pInnerRoomMesh->Render( m,
                            						innerDoorMesh->GetRadius(),
                                                    innerDoorMesh->GetCenterPt(),
                                                    !pInnerRoomMesh->WasTransformed() );

							// Render all objects in this room
                            totalCount += RenderObjectsPortal( m,
                        								   roomLst[ innerDoorMesh->GetRoomIndex(w) ],
                                                           objCount, innerDoorMesh );
                        }
                    }
                }
            }   
        }
    }

}   // RenderRoom


/************************************************************************                                                             
*    DESCRIPTION:  We are standing in a doorway. Render that doorway
*                  with it's ajoining rooms and doors.
*
*    Input:    Matrix3D &M - Matris to do all the translations
*              char *debug -  debug info
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::RenderDoor( int activeIndex, CMatrix &m, char *debug )
{
    // Render about the door we are standing in
    totalCount += doorLst[ activeIndex ]->pArea->Render( m );
    ++doorCount;

    // Add the rendered door to our list
    rendDoorLst.push_back( doorLst[ activeIndex ]->pArea );

	// Add to rendered list
	rendAreaLst.push_back( doorLst[ activeIndex ]->pArea );

    // Try to render the two rooms that join the doorway
    for( int i = 0; i < EMD_MAX_ROOMS_PER_DOOR; ++i )
    {        
        CMeshMap *pMesh = doorLst[ activeIndex ]->areaLst[ i ]->pArea;

        // Inc the debug counters
        IncCounters( pMesh );

		// Add to rendered list
		rendAreaLst.push_back( pMesh );

        // Render this area
        totalCount += pMesh->Render( m );

        // Render all objects in this room
        totalCount += RenderObjects( m,
        							 doorLst[ activeIndex ]->areaLst[ i ],
        							 objCount );

        // Get the pointer to the door list
        std::deque<CMeshMap *> &tmpDoorLst = doorLst[ activeIndex ]->areaLst[ i ]->doorLst;

        // render the doors you can see
        for( unsigned int j = 0; j < tmpDoorLst.size(); ++j )
        {
            // Get a pointer to the door mesh
            CMeshMap *pDoorMesh = tmpDoorLst[ j ];

			// Add to rendered list
			rendAreaLst.push_back( pDoorMesh );

            // If the doorway was NOT culled, see if it should be rendered
            if( !pDoorMesh->WasCullTested() && !pDoorMesh->WasRendered() )
            {
                // Must transform the center point first before we do anything
                // because many functions rely on the translated center point.
                pDoorMesh->TransformCenterPt( m );

                if( !pDoorMesh->CullMesh_BoundSphere() )
                {
                    // Add the rendered door to our list
                    rendDoorLst.push_back( pDoorMesh );

                    totalCount += pDoorMesh->Render( m );
                    ++doorCount;

                    // render the room associated with this door.
                    // one of these rooms has already been rendered
                    for( int w = 0; w < EMD_MAX_ROOMS_PER_DOOR; ++w )
                    {
                        CMeshMap *pInnerRoomMesh = roomLst[ pDoorMesh->GetRoomIndex(w) ]->pArea;

                        // Inc the debug counters
                        IncCounters( pInnerRoomMesh );

						// Add to rendered list
						rendAreaLst.push_back( pInnerRoomMesh );

                        totalCount += pInnerRoomMesh->Render( m,
                        									  pDoorMesh->GetRadius(),
                                                              pDoorMesh->GetCenterPt(),
                                                              !pInnerRoomMesh->WasTransformed() );

                        // Render all objects in this room
                        totalCount += RenderObjectsPortal( m,
                        								   roomLst[ pDoorMesh->GetRoomIndex(w) ],
                                                           objCount, pDoorMesh );
                    }
                }
            }
        }
    }

}   // RenderDoor


/************************************************************************                                                             
*    DESCRIPTION:  We are standing in a doorway. Render that doorway
*                  with it's ajoining rooms and doors.
*
*    Input:  CMatrix &M - Matris to do all the translations
*            CArea * pArea - Area that objects are in
*            int &objCount - number of objects
*            CMeshMap *pMeshPortal - Mesh to be used as a portal
*
*    Output: int - number of polygons rendered
************************************************************************/
template <class mesh_type>
int CMapMgr<mesh_type>::RenderObjects( CMatrix &m, CArea * pArea, int &objCount )
{
    int count = 0;

    // Render all the objects in this room
	for( std::vector<CAnimMeshLst<CAnimMesh> *>::iterator it = pArea->objLst.begin();
		it != pArea->objLst.end();
		++it )
    {
		int tmpCount;

		(*it)->DoWorldTransformation();
		(*it)->DoViewTransformation( m );

		// Are we rendering with dynamic lighting?
		if( pArea->pLightList != NULL && 
			pArea->pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
		{
			// Transform the light position
			pArea->pLightList->Transform( m );

			tmpCount = (*it)->Render( pArea->pLightList,
									  m,
									  pArea->pArea->GetMinPoint(),
									  pArea->pArea->GetMaxPoint() );
		}
		else
		{
			tmpCount = (*it)->Render();
		}

		count += tmpCount;

		if( tmpCount )
			++objCount;
    }

    return count;

}   // RenderObjects


/************************************************************************                                                             
*    DESCRIPTION:  We are standing in a doorway. Render that doorway
*                  with it's ajoining rooms and doors.
*
*    Input:    CMatrix &M - Matris to do all the translations
*              CArea * pArea - Area that objects are in
*              int &objCount - number of objects
*              bool transformCol - do transformations on collision objs
*              CMeshMap *pMeshPortal - Mesh to be used as a portal
*
*    Output: int - number of polygons rendered
************************************************************************/
template <class mesh_type>
int CMapMgr<mesh_type>::RenderObjectsPortal( CMatrix &m,
											 CArea * pArea,
                                             int &objCount,
                                             CMeshMap *pMeshPortal )
{
    int count = 0;

    // Render all the objects in this room
	for( std::vector<CAnimMeshLst<CAnimMesh> *>::iterator it = pArea->objLst.begin();
	     it != pArea->objLst.end();
	     ++it )
    {
        int tmpCount;

	    (*it)->DoWorldTransformation();
		(*it)->DoViewTransformation( m );

		// Are we rendering with dynamic lighting?
		if( pArea->pLightList != NULL && 
			pArea->pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
		{
			// Transform the light position
			pArea->pLightList->Transform( m );

			tmpCount = (*it)->Render( pArea->pLightList,
									  m,
									  pMeshPortal->GetRadius(),
	                                  pMeshPortal->GetCenterPt(),
	                                  pArea->pArea->GetMinPoint(),
	                                  pArea->pArea->GetMaxPoint() );
		}
		else
		{
			tmpCount = (*it)->Render( pMeshPortal->GetRadius(),
	                                  pMeshPortal->GetCenterPt() );
		}

		count += tmpCount;

		if( tmpCount )
			++objCount;
    }

    return count;

}   // RenderObjects


/************************************************************************                                                             
*    DESCRIPTION:  Check for collision and react to it. I feel
*                  that collision reaction is best when it is
*                  handled outside the engine because every
*                  situation is different. Let the engine indicate
*                  it has hit something, but that is it.
*
*    Input:  Matrix3D &M - Matrix to do all the translations
*            int buttonUpDown - button up down flag
*            nt buttonLeftRight  - button left right flag
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::ReactToCollision( CMapType &mapType, 
										   CMatrix &matrixYZ,
										   float fpRadious,
										   double distFromFloor,
										   double distFromWall,
										   int buttonUpDown,
										   int buttonLeftRight )
{
    int polyIndex, colType;
    float x, z, y, collisionOffset; 
    double dist;
    CPoint pt;
    CMeshMap *pFloor, *pWall;
    std::vector<CMeshMap *> objCheck;

    // Start the inital distances as far away so we can see if
    // we are getting closer. We only want to collide (do Intersection Collision)
    // with the closest polygons.
    double closestFloorDistance=distFromFloor*2;
	double closestWallDistance=distFromWall*2;
    int closestFloorIndex = -1;
    int closestWallIndex = -1;

    // Tune the wall distance for stairways
	if( mapType.activeType == EOT_MAP_STAIRWAY )
	{
		closestWallDistance = distFromWall * 5;
	}

    ////////////////////////////////////////
    // Make a temporary list of objects to
    // check for collision
    ////////////////////////////////////////
    
    // We are standing on a polygon in a doorway
    if( mapType.activeType == EOT_MAP_DOORWAY ||
        mapType.activeType == EOT_MAP_HALLDOORWAY )
    {
        // Add the doorway we were last known to be standing in
        objCheck.push_back( doorLst[ mapType.activeIndex ]->pArea );

        // Add the ajoining rooms attached to this doorway
        // to check for possible collision
        for( int i = 0; i < EMD_MAX_ROOMS_PER_DOOR; ++i )
        {
            objCheck.push_back( doorLst[ mapType.activeIndex ]->areaLst[ i ]->pArea );
        }
    }
    // We are standing on a polygon in a room
    else
    {
        // Add the room we were last known to be standing in
        objCheck.push_back( roomLst[ mapType.activeIndex ]->pArea );

        // Add all the doorways attached to this room to check
        // for possible collision if we are close
        for( unsigned int i = 0; i < roomLst[ mapType.activeIndex ]->doorLst.size(); ++i )
        {
            objCheck.push_back( roomLst[ mapType.activeIndex ]->doorLst[i] );
        }
    }

    // Check the differnt areas for collision
    for( std::vector<CMeshMap *>::iterator it = objCheck.begin(); it != objCheck.end(); ++it )
    {
        polyIndex = 0;

        do
        {
            // Check for a bounding sphere collision. This is fastest and we'll
            // use this as a means of finding only the closest polygons to do
            // our final test which is the intersection test.
            colType = (*it)->IsCollision_BoundSphere( polyIndex, dist, pt, fpRadious );

            ///////////////////////////
            // Did we hit a floor?
            ///////////////////////////
            if( colType == ECT_MAP_FLOOR )
            {
                // Find the closest floor polygon and save the index
                if( dist < closestFloorDistance )
                {
                    closestFloorDistance = dist;
                    closestFloorIndex = polyIndex;
                    pFloor = (*it);
                }
            }
            ///////////////////////////
            // Did we hit a wall?
            ///////////////////////////
            else if( colType == ECT_MAP_WALL )
            {
                // Find the closest wall polygon and save the index
                if( dist < closestWallDistance )
                {
                    closestWallDistance = dist;
                    closestWallIndex = polyIndex;
                    pWall = (*it);
                }
            }
        }
        while( (polyIndex > -1) && (++polyIndex < (*it)->GetPolyCount()) );
    }
            
    // react to the floor if we touched one
    if( closestFloorIndex > -1 )
    {
	    // Get a copy of the firct point
	    CPoint triVert( *pFloor->GetPolyList()[closestFloorIndex].GetVPoint(0) );
	    
	    // Get a copy of the normal 
	    CPoint normal( *pFloor->GetPolyList()[closestFloorIndex].GetNormal() );

	    // We need the points and the normal retransformed without 
	    // any x rotation because this function assumes gravity is being used.
	    matrixYZ.Transform( triVert );
	    matrixYZ.TransformVector( normal );

        // reset the active area index
        mapType.activeIndex = pFloor->GetID();
        mapType.activeType = pFloor->GetType();

        // Final check for floor collision. Slower then bounding sphere collision but very
        // accurate which is why we do it after the bounding sphere check.
        if( pFloor->IsCollision_Intersection( normal, triVert, pt, distFromFloor, collisionOffset ) )
        {
            // Compute the y delta
            if( normal.ty != 0 )
                y = (((normal.tx + normal.tz) - collisionOffset) / -normal.ty) + distFromFloor;
            else
                y = ((normal.tx + normal.tz) - collisionOffset) + distFromFloor;
            
            // Move the camera
            matrixYZ.Translate( 0, -y, 0 );
        }
    }

    // Only react to the wall if we are moving. If we hit the wall, do a wall glide
    if( (closestWallIndex > -1) && (buttonUpDown || buttonLeftRight) )
    {
	    // Get a copy of the firct point
	    CPoint triVert( *pWall->GetPolyList()[closestWallIndex].GetVPoint(0) );
	    
	    // Get a copy of the normal 
	    CPoint normal( *pWall->GetPolyList()[closestWallIndex].GetNormal() );

	    // We need the points and the normal retransformed without 
	    // any x rotation because this function assumes gravity is being used.
	    matrixYZ.Transform( triVert );
	    matrixYZ.TransformVector( normal );

        // Final check for wall collision. Will return
        // true if we are to go further.
        if( pWall->IsCollision_Intersection( normal, triVert, pt, distFromWall, collisionOffset ) )
        {
            z = x = 0;

            // If we are going in the opposite direction of the wall we are
            // touching, don't do collision reaction because we don't want the
            // wall to hold onto us like a tractor beam.
            if( (normal.tz <= 0 && buttonUpDown < 0) ||
                (normal.tz >= 0 && buttonUpDown > 0) ||
                (normal.tx <= 0 && buttonLeftRight < 0) ||
                (normal.tx >= 0 && buttonLeftRight > 0) )
            {
                // Glide along the wall directly in front or behind us
                if( fabs(normal.tz) > fabs(normal.tx) )
                {
                    // The wall is in front of us
                    if( normal.tz < 0 )
                    {
                        z = -(((normal.tx - collisionOffset) / (-normal.tz)) + distFromWall);
                        
                        // Only use the normal value if we are going
                        // forward and need to glide along the wall.
                        if( buttonUpDown )
                            x = normal.tx;

                        // Cap the speed so we don't snap to the wall
                        if( z > 0 )
                            z = 0;
                    }
                    // The wall is behind us
                    else
                    {
                        z = -(((normal.tx - collisionOffset) / (-normal.tz)) - distFromWall);
                        
                        // Only use the normal value if we are going
                        // backwards and need to glide along the wall.
                        if( buttonUpDown )
                            x = -normal.tx;

                        // Cap the speed so we don't snap to the wall
                        if( z < 0 )
                            z = 0;
                    }
                }
                // Glide along the wall to our right or left side
                else
                {
                    // The wall is on our left side
                    if( normal.tx < 0 )
                    {
                        // Only use the normal value if we are going
                        // sideways and need to glide along the wall.
                        if( buttonLeftRight )
                            z = -normal.tz;

                        x = -(((normal.tz - collisionOffset) / (-normal.tx)) + distFromWall);

                        // Cap the speed so we don't snap to the wall
                        if( x > 0 )
                            x = 0;
                            
                    }
                    // The wall is on our right side
                    else
                    {
                        // Only use the normal value if we are going
                        // sideways and need to glide along the wall.
                        if( buttonLeftRight )
                            z = -normal.tz;

                        x = -(((normal.tz - collisionOffset) / (-normal.tx)) - distFromWall);

                        // Cap the speed so we don't snap to the wall
                        if( x < 0 )
                            x = 0;                       
                    }                
                }
            }

           // Move it along the wall
           matrixYZ.Translate( x, 0, z );
        }
    }

}   // ReactToCollision


/************************************************************************
*    DESCRIPTION:  Regroup all the objects to help simplify
*                  the rendering and collision detection.
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::SetupMap()
{
    // Seperate the meshes into their lists
    for( int i = 0; i < meshCount; ++i )
    {
        // Allocate an area and add it to the list
        CArea *pTmp = new CArea;

        if( (pMesh[i].GetType() == EOT_MAP_ROOM) ||
            (pMesh[i].GetType() == EOT_MAP_HALLWAY) ||
            (pMesh[i].GetType() == EOT_MAP_STAIRWAY) )
        {
            // Copy the room pointer
            pTmp->pArea = &pMesh[i];

			// Copy the light list used for this mesh
			pTmp->pLightList = pMesh[i].GetLightList();
            
            // Add the room pointer
            roomLst.push_back( pTmp );
        }
        else if( pMesh[i].GetType() == EOT_MAP_DOORWAY ||
                 pMesh[i].GetType() == EOT_MAP_HALLDOORWAY )
        {
            // Use a smaller radius for thin doorway walls. Makes it easer
            // for wall collision detection. You don't wall glide to far after
            // going through the doorway
			if( pMesh[i].GetType() == EOT_MAP_DOORWAY )
			{
	            pMesh[i].AdjustRadius( 0.001f, ECT_MAP_WALL );
			}

            // Copy the door pointer
            pTmp->pArea = &pMesh[i];

			// Copy the light list used for this mesh
			pTmp->pLightList = pMesh[i].GetLightList();

            // Add the door pointer
            doorLst.push_back( pTmp );
        }
    }

    // Don't setup all the room and door because we might be trying to debug a problem
    #ifndef _render_all_

    // Go through all the rooms and find the doors
    // and other rooms that belong to them
    for( unsigned int i = 0; i < roomLst.size(); ++i )
    {
        for( unsigned int j = 0; j < doorLst.size(); ++j )
        {
            for( int w = 0; w < EMD_MAX_ROOMS_PER_DOOR; ++w )
            {
                // Add the door to the room if we find it
                if( doorLst[ j ]->pArea->GetRoomIndex( w ) == i )
                {
                    // Add the door connected to this room
                    roomLst[ i ]->doorLst.push_back( doorLst[ j ]->pArea );

                    // Add the room connected to this door
                    roomLst[ i ]->areaLst.push_back( roomLst[ doorLst[ j ]->pArea->GetRoomIndex( !w ) ] );

                    break;
                }
            }
        }
    }

    // Go through all the doors and add the 
    // rooms that they attach to.
    for( unsigned int i = 0; i < doorLst.size(); ++i )
    {
        for( int j = 0; j < EMD_MAX_ROOMS_PER_DOOR; ++j )
        {
			// Add the room connected to this door
            doorLst[ i ]->areaLst.push_back( roomLst[ doorLst[ i ]->pArea->GetRoomIndex( j ) ] );
        }
    }

    #endif

}   // SetupMap


/************************************************************************                                                             
*    DESCRIPTION:  Was this area rendered
*
*    Input:    CMapType &mapType -  map type and index
************************************************************************/
template <class mesh_type>
bool CMapMgr<mesh_type>::WasRendered( CMapType &mapType )
{   
    bool fRendered;

    // Object is in a doorway
    if( mapType.activeType == EOT_MAP_DOORWAY ||
        mapType.activeType == EOT_MAP_HALLDOORWAY )
    {
        fRendered = doorLst[ mapType.activeIndex ]->pArea->WasRendered();
    }
    // Object is in some type of room
    else
    {
        fRendered = roomLst[ mapType.activeIndex ]->pArea->WasRendered();
    }

    return fRendered;

}   // WasRendered


/************************************************************************                                                             
*    DESCRIPTION:  Is this bounding sphere visible through the list
*                  of rendered doorways?
*
*    Input:   CMapType &mapType -  map type and index of object to render
*              CMapType &playerMapType - player object and map type
*             float radius - radius of object being checked
*             CPoint *pCenter - translated center point of object
************************************************************************/
template <class mesh_type>
bool CMapMgr<mesh_type>::IsVisableThruRenderedDoorways( CMapType &objMapType,  CMapType &playerMapType,
                                                        float radius, CPoint *pCenter )
{
	// No sence doing any checks if we are in the same room or doorway as the object
    if( (objMapType.activeIndex == playerMapType.activeIndex) && 
        (objMapType.activeType == playerMapType.activeType) )
    {
        return true;
    }

    // Make sure the object is not in the room of the doorway we are standing in
    if( playerMapType.activeType == EOT_MAP_DOORWAY ||
        playerMapType.activeType == EOT_MAP_HALLDOORWAY )
    {
        if( objMapType.activeIndex == rendDoorLst[0]->GetRoomIndex( EMD_ROOM_1 ) ||
            objMapType.activeIndex == rendDoorLst[0]->GetRoomIndex( EMD_ROOM_2 ) )
        {
            return true;
        }
    }

    bool result = false;

    if( objMapType.activeType == EOT_MAP_DOORWAY ||
        objMapType.activeType == EOT_MAP_HALLDOORWAY )
    {
        // Check the door id's to see if it is the same as the one the object standing on
		for( std::deque<CMeshMap *>::iterator it = rendDoorLst.begin(); it != rendDoorLst.end() && !result; ++it )
        {
            if( (*it)->GetID() == objMapType.activeIndex )
            {
                return true;
            }
        }
    }
    else
    {
        // only check the door radius of the room the object is in to see
        // if it can be seen through that doorway
		for( std::deque<CMeshMap *>::iterator it = rendDoorLst.begin(); it != rendDoorLst.end() && !result; ++it )
        {
            if( objMapType.activeIndex == (*it)->GetRoomIndex( EMD_ROOM_1 ) ||
                objMapType.activeIndex == (*it)->GetRoomIndex( EMD_ROOM_2 ) )
            {
                result = !(*it)->CullMesh_BoundSphere( radius, pCenter );
            }
        }
    }

    return result;

}   // IsVisableThruRenderedDoorways


/************************************************************************
*    DESCRIPTION:  Set the spawn point
*
*    Input:  CMapType &mapType -  map type and index
*            Matrix3D &matrixYZ - Matrix to do all the translations            
*            EOT_OBJECT_TYPE spawnPoint - spawn point
*            int spawnIndex - index of spawn point
*            float dir - direction of matrix. Values are 1 or -1 used
*                        to change the sign of the x, y, z
*
*    Output: bool - true if spawn point was found and used
************************************************************************/
template <class mesh_type>
bool CMapMgr<mesh_type>::SetSpawnPoint( CMapType &mapType,
										CMatrix &matrix,
                                        EOT_OBJECT_TYPE spawnPoint,
                                        int spawnIndex,
                                        float dir )
{
    bool spawnFound = false;
    int meshIndex;
    int spawnCounter = 0;
    
    // Go through all the meshes and find the spawn point
    for( int i = 0; i < meshCount; ++i )
    {
        if( pMesh[i].GetType() == spawnPoint )
        {
            if( spawnCounter == spawnIndex )
            {
                mapType.activeIndex = pMesh[i].GetRoomIndex(EMD_ROOM_1);
                mapType.activeType = roomLst[ mapType.activeIndex ]->pArea->GetType();
 
                spawnFound = true;
                meshIndex = i;
                break;
            }
            ++spawnCounter;
        }
    }

    // spawn found. Translate to that position
    if( spawnFound == true )
    {
        matrix.Translate( pMesh[meshIndex].GetCenterPt()->x * dir,
                          pMesh[meshIndex].GetCenterPt()->y * dir, 
                          pMesh[meshIndex].GetCenterPt()->z * dir );
    }

	return spawnFound;

}   // SetToSpawnPoint


/************************************************************************
*    DESCRIPTION:  Set the spawn point
*
*    Input:  CMapType &mapType -  map type and index
*            Matrix3D &matrixYZ - Matrix to do all the translations            
*            EOT_OBJECT_TYPE spawnPoint - spawn point
*            int spawnIndex - index of spawn point
*            float dir - direction of matrix. Values are 1 or -1 used
*                        to change the sign of the x, y, z
*
*    Output: bool - true if spawn point was found and used
************************************************************************/
template <class mesh_type>
bool CMapMgr<mesh_type>::SetSpawnPoint( CMapType &mapType,
										CMatrix &matrix,
										CPoint trans,
										CPoint rot,
										int floorId,
                                        int mapId,
                                        float dir )
{
    bool spawnFound = false;
    
    // Go through all the meshes and find the spawn point
    for( int i = 0; i < meshCount && !spawnFound; ++i )
    {
        if( (pMesh[i].GetFloorID() == floorId) &&
            (pMesh[i].GetType() == mapType.activeType) &&
            (pMesh[i].GetMapId() == mapId) )
        {
        	mapType.activeIndex = pMesh[i].GetID();

			spawnFound = true;
        }
    }

    // spawn found. Translate to that position
    if( spawnFound == true )
    {
        matrix.Translate( trans.x * dir, trans.y * dir, trans.z * dir );
		matrix.Rotate( rot.x, rot.y, rot.z );
    }

	return spawnFound;

}   // SetToSpawnPoint


/************************************************************************                                                             
*    DESCRIPTION:  Floor collision for bots
*
*    Input:  Matrix3D &M - Matrix to do all the translations
*            CMapType &mapType -  map type and index
*            CPoint &objPoint -  point to do collision detection with
*            float radius - radius of objects
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::BotFloorCollision( CMatrix &matrix,
                                            CMapType &mapType,
                                            CPoint &objPoint,
                                            float radius )
{
    int polyIndex;
	ECT_COLLISION_TYPE colType;
    float y; 
    double dist;
    CPoint normal;
    CMeshMap *pFloor;
    std::vector<CMeshMap *> objCheck;

    // Start the inital distances as far away so we can see if
    // we are getting closer. We only want to collide (do Intersection Collision)
    // with the closest polygons.
    double closestFloorDistance=(radius*radius)*20;
    int closestFloorIndex = -1;

    ////////////////////////////////////////
    // Make a temporary list of objects to
    // check for collision
    ////////////////////////////////////////
    
    // We are standing on a polygon in a doorway
    if( mapType.activeType == EOT_MAP_DOORWAY ||
        mapType.activeType == EOT_MAP_HALLDOORWAY )
    {
        // Add the doorway we were last known to be standing in
        objCheck.push_back( doorLst[ mapType.activeIndex ]->pArea );

        // Add the ajoining rooms attached to this doorway
        // to check for possible collision
        for( int i = 0; i < EMD_MAX_ROOMS_PER_DOOR; ++i )
        {
            objCheck.push_back( doorLst[ mapType.activeIndex ]->areaLst[ i ]->pArea );
        }
    }
    // We are standing on a polygon in a room
    else
    {
        // Add the room we were last known to be standing in
        objCheck.push_back( roomLst[ mapType.activeIndex ]->pArea );

        // Add all the doorways attached to this room to check
        // for possible collision if we are close
        for( unsigned int i = 0; i < roomLst[ mapType.activeIndex ]->doorLst.size(); ++i )
        {
            objCheck.push_back( roomLst[ mapType.activeIndex ]->doorLst[i] );
        }
    }

    // Check the differnt areas for collision
	for( std::vector<CMeshMap *>::iterator it = objCheck.begin(); it != objCheck.end(); ++it )
    {
        polyIndex = 0;

        do
        {
            // Check for a bounding sphere collision. This is fastest and we'll
            // use this as a means of finding only the closest polygons to do
            // our final test which is the intersection test.
            colType = (*it)->IsCollision_BoundSphereSt( polyIndex, dist, objPoint, radius, ECT_MAP_FLOOR );

            ///////////////////////////
            // Did we hit a floor?
            ///////////////////////////
            if( colType == ECT_MAP_FLOOR )
            {
                // Find the closest floor polygon and save the index
                if( dist < closestFloorDistance )
                {
                    closestFloorDistance = dist;
                    closestFloorIndex = polyIndex;
                    pFloor = (*it);
                }
            }
        }
        while( (polyIndex > -1) && (++polyIndex < (*it)->GetPolyCount()) );
    }
            
    // react to the floor if we touched one
    if( closestFloorIndex > -1 )
    {
        // reset the active area index
        mapType.activeIndex = pFloor->GetID();
        mapType.activeType = pFloor->GetType();

        // get the average height of the polygon
        y = pFloor->GetHeightOfPolygon( closestFloorIndex );

		// Set the height
		matrix.SetY( y );
    }

}   // BotFloorCollision


/************************************************************************                                                             
*    DESCRIPTION:  Record the nodes
*
*    Input:  CGraphList <CPathNode> & graph
*
*    Output: bool - true if all nodes are found
************************************************************************/
template <class mesh_type>
bool CMapMgr<mesh_type>::AddGraphNodesToRooms( CGraphList <CPathNode> & graph )
{
	ECT_COLLISION_TYPE colType = ECT_NO_COLLISION;
	std::set<CPathNode *> & nodeLst = graph.GetUniqueNodeList();
	double dummy;
	int nodesFound = 0;

	// cycle through all the rooms
	for( std::vector<CRoom *>::iterator rit = roomLst.begin(); rit != roomLst.end(); ++rit )
	{
		// Check each node only once
		for( std::set<CPathNode *>::iterator nit = nodeLst.begin(); nit != nodeLst.end(); ++nit )
		{
			int polyIndex(0);

			// The node is the same position as the floor polygon so 
			// we can use a small radius of 1
			colType = (*rit)->pArea->IsCollision_BoundSphereSt( polyIndex, dummy, (*nit)->GetPos(), 1, ECT_MAP_FLOOR );

	        // if we collided with one of the floor polygons, record it
			if( colType == ECT_MAP_FLOOR )
			{
				(*rit)->nodeLst.push_back( (*nit) );
				++nodesFound;
			}
		}
	}

    return ( nodesFound == nodeLst.size() );
		
}	// AddGraphNodesToRooms


/************************************************************************                                                             
*    DESCRIPTION:  Get the node list
*
*    Input:  CMapType &mapType - get the node list based on the map type
*
*    Output: std::vector<CPathNode *> & - returns the path node vector
************************************************************************/
template <class mesh_type>
std::deque<CPathNode *> &CMapMgr<mesh_type>::GetNodeLst( CMapType &mapType )
{
	if( mapType.activeType == EOT_MAP_DOORWAY ||
	    mapType.activeType == EOT_MAP_HALLDOORWAY )
	{
		return doorLst[mapType.activeIndex]->nodeLst;
	}
	else // everything else is considered a room of some type
	{
		return roomLst[mapType.activeIndex]->nodeLst;
	}	 
}   // GetNodeLst


/************************************************************************                                                             
*    DESCRIPTION:  Get the mesh
*
*    Input:  CMapType &mapType - get the mesh based on the map type
*
*    Output: mesh_type * - pointer to mesh
************************************************************************/
template <class mesh_type>
mesh_type *CMapMgr<mesh_type>::GetMesh( CMapType &mapType )
{
	if( mapType.activeType == EOT_MAP_DOORWAY ||
	    mapType.activeType == EOT_MAP_HALLDOORWAY )
	{
		return doorLst[mapType.activeIndex]->pArea;
	}
	else // everything else is considered a room of some type
	{
		return roomLst[mapType.activeIndex]->pArea;
	}
}	// GetMesh


/************************************************************************
*    DESCRIPTION:  Set the objects to the rooms
*
*    Input:  const std::string & tagFilename -  config file
*            CAnimMeshLst<CAnimMesh> &mapObjects - Object animation list
*
*    Output: bool - true if spawn point was found and used
************************************************************************/
template <class mesh_type>
bool CMapMgr<mesh_type>::SetObjsToRooms( const std::string & tagFilename,
										 CAnimMeshLst<CAnimMesh> &mapObjects )
{
    bool roomFound = true;
	TDataDefNodeVector data;
	TFloatVector mapScale;
	CPoint mapScalePoint;
	SObjectType objTypeStruct;

	// Parse the config file.
	CDataDefNode * pTree = CTagFileParser::GetInstance().ParseFile( tagFilename );
    if( pTree == NULL )
		return false;

    CDataDefNode * pChild = pTree->GetChild("global");
    if( pChild == NULL )
		return false;

    pChild->GetDataByName("mapScale", mapScale);
	mapScalePoint.Init( mapScale );
    
    // Retrieve all of the spawn data.    
    if( !pTree->GetDataByName("objectList", data) )
		return false;

    if( data.size() == 0 )
		return false;
    
    // Go through all the meshes and find the room the object belongs in
	for( unsigned int i = 0; i < data.size() && roomFound; ++i )
	{
	    roomFound = false;

		TFloatVector trans;
    	TFloatVector rot;
		TFloatVector scaleObj;
		TIntVector offsetObj;
		TFloatVector shadowProject;
		TFloatVector lightOffset;
		std::string animName;
		std::string objTypeStr;
		EOT_OBJECT_TYPE objType;
		int floorId = 0;
    	int mapId = 0;
		int shadow = 0;
		int shadowInten = 0;
		CPoint tmpPt;

	    // Get the object data
		data[i]->GetDataByName("AnimName", animName);
		data[i]->GetDataByName("floorId", floorId);
		data[i]->GetDataByName("objectType", objTypeStr);
		objType = objTypeStruct.GetType( objTypeStr );
	    data[i]->GetDataByName("mapId", mapId);
		data[i]->GetDataByName("objOffset", offsetObj);
	    data[i]->GetDataByName("trans", trans);
	    data[i]->GetDataByName("rot", rot);
		data[i]->GetDataByName("scale", scaleObj);
		data[i]->GetDataByName("shadow", shadow);
		data[i]->GetDataByName("shadowInten", shadowInten);
		data[i]->GetDataByName("shadowProject", shadowProject);
		data[i]->GetDataByName("lightOffset", lightOffset);

	    for( unsigned int j = 0; j < roomLst.size() && !roomFound; ++j )
	    {
	        if( (roomLst[j]->pArea->GetFloorID() == floorId) &&
				(roomLst[j]->pArea->GetType() == objType) &&
	            (roomLst[j]->pArea->GetMapId() == mapId) )
	        {
				CMatrix mapScaleMatrix;
				CPoint transPoint;

				roomFound = true;

                // Calculate the transition point based on the map scale
				transPoint.Init( trans );
				mapScaleMatrix.Scale( mapScalePoint );
				mapScaleMatrix.Transform( transPoint );
				transPoint.MakePermanent();

				// Allocate the object with the copy contructor
				roomLst[j]->objLst.push_back( new CAnimMeshLst<CAnimMesh>(mapObjects) );

				// Set the animation
				roomLst[j]->objLst.back()->SetAnimation( (char *)animName.c_str() );

				// Set the size of the object
				if( scaleObj.size() > 0 )
				{					
					roomLst[j]->objLst.back()->SetScale( scaleObj[0], scaleObj[1], scaleObj[2] );
				}

				// Set the rotation on the map
				if( rot.size() > 0 )
				{
					roomLst[j]->objLst.back()->SetRot( rot[0], rot[1], rot[2] );
				}
				
				// Set the position on the map
				roomLst[j]->objLst.back()->SetPos( transPoint, offsetObj );
				
				// Set the shadow flag
				if( shadow )
				{
					roomLst[j]->objLst.back()->SetShadow( true );
				}

				roomLst[j]->objLst.back()->SetShadowInten( shadowInten );
				
				if( shadowProject.size() > 0 )
				{
					tmpPt.Init(shadowProject);
					roomLst[j]->objLst.back()->SetShadowProject( tmpPt );
				}
				
				if( lightOffset.size() > 0 )
				{
					tmpPt.Init(lightOffset);
					mapScaleMatrix.Transform( tmpPt );
					tmpPt.MakePermanent();
					roomLst[j]->objLst.back()->SetLightOffset( tmpPt );
				}				
	        }
	    }
	}

	return roomFound;

}   // SetObjsToRooms


/************************************************************************
*    DESCRIPTION:  Set the lights to the meshes
*
*    Input:  std::vector<CMapLightList *> mapLightLst - light list
*
*    Output: bool - true if spawn point was found and used
************************************************************************/
template <class mesh_type>
bool CMapMgr<mesh_type>::SetLightsToMesh( std::vector<CMapLightList *> &mapLightLst )
{
	bool roomFound = true;

	for( unsigned int i = 0; i < mapLightLst.size() && roomFound; ++i )
	{
	    roomFound = false;

	    for( int j = 0; j < meshCount && !roomFound; ++j )
		{
		    if( (pMesh[j].GetFloorID() == mapLightLst[i]->floorID) &&
				(pMesh[j].GetType() == mapLightLst[i]->objectType) &&
		        (pMesh[j].GetMapId() == mapLightLst[i]->mapID) )
		    {
				roomFound = true;
				pMesh[j].SetLightList( mapLightLst[i] );		
		    }
		}
	}

	return roomFound;

}   // SetLightsToMesh


/************************************************************************
*    DESCRIPTION:  Clear the mesh lights
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::ClearMeshLights()
{
	for( int i = 0; i < meshCount; i++ )
	{		
		// Only remove the EDL_NO_DYNAMIC_LIGHTNING lighting
		if( pMesh[i].GetLightList()->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
		{
			pMesh[i].ClearLightLst();
		}		
	}

}   // ClearMeshLights


#endif  // _CMAP_MGR_H_