
/************************************************************************
*
*    PRODUCT:         Map Manager
*
*    FILE NAME:       CMapMgr.cpp
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

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

#include "CMapMgr.h"         // Header file for this *.cpp file
#include "CMatrix.h"         // Matrix class
#include "types.h"           // misc type specifiers


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
CMapMgr<mesh_type>::CMapMgr()
{
    activeIndex = 0;
    activeType = EOT_MAP_ROOM;

    // We don't want the data to be freeded
    roomLst.DontFreeData();
    doorLst.DontFreeData();
    rendDoorLst.DontFreeData();
}


/************************************************************************
*    DESCRIPTION:           Free the allocations
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::Free()
{
}   // Free


/************************************************************************                                                             
*    DESCRIPTION:     Render only what we can see. If we can see a
*                     doorway, then render the other possible room.
*
*    Input:    Matrix3D &M - Matris to do all the translations
*              char *debug -  debug info
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::Render( CMatrix &m, char *debug )
{
    int doorCount = 0;
    int roomCount = 0;
    int totalCount = 0;
    CMeshMap *pDoorMesh, *pRoomMesh, *innerDoorMesh, *pInnerRoomMesh;
    CObjLst<CMeshMap> *pDoorLst, *pInnerDoorLst;

    // Clear out all the render flags
    ClearFlags();

    // free all saved rendered doors
    rendDoorLst.Free();
    
    // We are standing on a polygon in a room
    if( activeType == EOT_MAP_ROOM )
    {
        // Render the active area first
        totalCount += roomLst.Get( activeIndex )->pRoom->Render( m );
        ++roomCount;

        // Get the pointer to the door list
        pDoorLst = &roomLst.Get( activeIndex )->doorLst;

        // Transform all the points on all the doors in the room we are
        // standing in. This way we can test for it later and not reprocess
        // the same doors that are part of the ajoining room.
        for( int i = 0; i < pDoorLst->Count(); ++i )
        {
            // Get a pointer to the door mesh
            pDoorMesh = pDoorLst->Get( i );

            // Must transform the center point first before we do anything
            // because many functions rely on the translated center point.
            pDoorMesh->TransformCenterPt( m );

            // Transfor the door so that it can be collision checked
            pDoorMesh->Transform( m );
        }

        // Find the doors that are connected to this room
        // and check if we can see them
        for( int i = 0; i < pDoorLst->Count(); ++i )
        {
            // Get a pointer to the door mesh
            pDoorMesh = pDoorLst->Get( i );

            bool closeToDoorWay = false;

            // If the doorway was NOT culled, render it
            // along with the ajoining room
            if( !pDoorMesh->CullMesh_BoundSphere() )
            {
                // Add the rendered door to our list
                rendDoorLst.Add( pDoorMesh );

                // Render the doorway
                totalCount += pDoorMesh->Render( m, false );
                ++doorCount;

                // Get the other room attached to this door
                pRoomMesh = roomLst.Get( activeIndex )->roomDoorLst.Get( i )->pRoom;                

                // Inc the room counter
                if( !pRoomMesh->WasRendered() )
                {
                    ++roomCount;
                }
                
                // Only render the polygons we can see through the doorway 
                if( pDoorMesh->GetDistance() > pDoorMesh->GetRadius() )
                {
                    // Two or more doors can connect to the same room so make sure we only
                    // translate once.
                    totalCount += pRoomMesh->Render( m, pDoorMesh->GetRadius(),
                                                     pDoorMesh->GetCenterPt(), 
                                                     !pRoomMesh->WasRendered() );
                }
                // The portal is useless when your right on top of it so don't bother using
                // with the rooms in front and behind us.
                else
                {
                    totalCount += pRoomMesh->Render( m, !pRoomMesh->WasRendered() );
                    closeToDoorWay = true;
                }

                // Get the doors attached to the inner room
                pInnerDoorLst = &roomLst.Get( activeIndex )->roomDoorLst.Get( i )->doorLst;

                // Try to render the doorways in this room
                for( int j = 0; j < pInnerDoorLst->Count(); ++j )
                {
                    innerDoorMesh = pInnerDoorLst->Get( j );

                    // Must transform the center point first before we do anything
                    // because many functions rely on the translated center point.
                    if( !innerDoorMesh->WasTransformed() )
                    {
                        innerDoorMesh->TransformCenterPt( m );
                    }

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
                        if( !innerDoorMesh->WasTransformed() )
                        {
                            // Add the rendered door to our list
                            rendDoorLst.Add( innerDoorMesh );

                            if( !closeToDoorWay )
                            {
                                totalCount += innerDoorMesh->Render( m, pDoorMesh->GetRadius(),
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
                        for( int w = 0; w < MAX_ROOMS_PER_DOOR; ++w )
                        {
                            pInnerRoomMesh = roomLst.Get( innerDoorMesh->GetAttachedRoom(w) )->pRoom;

                            if( !pInnerRoomMesh->WasRendered() )
                            {
                                pInnerRoomMesh->Render( m, innerDoorMesh->GetRadius(),
                                                        innerDoorMesh->GetCenterPt() );
                                ++roomCount;
                                break;
                            }
                        }
                    }
                }   
            }
        }
    }
    // We are standing on a polygon in a doorway
    else if( activeType == EOT_MAP_DOORWAY )
    {        
        // Render about the door we are standing in
        totalCount += doorLst.Get( activeIndex )->pDoor->Render( m );
        ++doorCount;

        // Add the rendered door to our list
        rendDoorLst.Add( doorLst.Get( activeIndex )->pDoor );

        // Try to render the rooms that join the doorway
        for( int i = 0; i < MAX_ROOMS_PER_DOOR; ++i )
        {
            totalCount += doorLst.Get( activeIndex )->roomDoorLst.Get( i )->pRoom->Render( m );
            ++roomCount;

            // Get the pointer to the door list
            pDoorLst = &doorLst.Get( activeIndex )->roomDoorLst.Get( i )->doorLst;

            // render the doors you can see
            for( int j = 0; j < pDoorLst->Count(); ++j )
            {
                // Get a pointer to the door mesh
                pDoorMesh = pDoorLst->Get( j );

                // If the doorway was NOT culled, see if it should be rendered
                if( !pDoorMesh->WasCullTested() )
                {
                    // Must transform the center point first before we do anything
                    // because many functions rely on the translated center point.
                    pDoorMesh->TransformCenterPt( m );

                    if( !pDoorMesh->CullMesh_BoundSphere() )
                    {
                        // Add the rendered door to our list
                        rendDoorLst.Add( pDoorMesh );

                        totalCount += pDoorMesh->Render( m );
                        ++doorCount;

                        // render the room associated with this door.
                        // one of these rooms has already been rendered
                        for( int w = 0; w < MAX_ROOMS_PER_DOOR; ++w )
                        {
                            if( !roomLst.Get( pDoorMesh->GetAttachedRoom(w) )->pRoom->WasRendered() )
                            {
                                roomLst.Get( pDoorMesh->GetAttachedRoom(w) )->pRoom->Render( m, pDoorMesh->GetRadius(),
                                                                                             pDoorMesh->GetCenterPt() );
                                ++roomCount;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    
    sprintf( debug, "Door Count: %d;   Room Count: %d;   Total Poly Count: %d", doorCount, roomCount, totalCount );

}   // Render


/************************************************************************
*    DESCRIPTION:     Set the index of the area we are to be in
*
*    Input:    int index - active index of the room we are standing in
*              EOT_OBJECT_TYPE type - object type
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::SetActiveIndex( int index, EOT_OBJECT_TYPE type )
{
    if( index < meshCount )
    {
        activeIndex = index;
        activeType = type;
    }
}   // SetActiveIndex


/************************************************************************                                                             
*    DESCRIPTION:     Check for collision and react to it. I feel
*                     that collision reaction is best when it is
*                     handled outside the engine because every
*                     situation is different. Let the engine indicate
*                     it has hit something, but that is it.
*
*    Input:    Matrix3D &M - Matris to do all the translations
*              int buttonUpDown - button up down flag
*              nt buttonLeftRight  - button left right flag
*
*    -------------------------------------------------------------------
*                  12/29/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::ReactToCollision( CMatrix &matrixYZ, int buttonUpDown, int buttonLeftRight )
{
    int polyIndex, colType;
    float x, z, y, pointD; 
    double dist;
    CPoint pt, normal;
    CMeshMap *pFloor, *pWall, *pMesh;
    CObjLst<CMeshMap> objCheck;
    objCheck.DontFreeData();

    // Start the inital distances as far away so we can see if
    // we are getting closer. We only want to collide (do Intersection Collision)
    // with the closest polygons.
    double closestFloorDistance=DIST_FROM_FLOOR*20;
    double closestWallDistance=DIST_FROM_WALL*20;
    int closestFloorIndex = -1;
    int closestWallIndex = -1;

    ////////////////////////////////////////
    // Make a temporary list of objects to
    // check for collision
    ////////////////////////////////////////

    // We are standing on a polygon in a room
    if( activeType == EOT_MAP_ROOM )
    {
        // Add the room we were last known to be standing in
        objCheck.Add( roomLst.Get( activeIndex )->pRoom );

        // Add all the doorways attached to this room to check
        // for possible collision if we are close
        for( int i = 0; i < roomLst.Get( activeIndex )->doorLst.Count(); ++i )
        {
            objCheck.Add( roomLst.Get( activeIndex )->doorLst.Get(i) );
        }
    }
    // We are standing on a polygon in a doorway
    else if( activeType == EOT_MAP_DOORWAY )
    {
        // Add the doorway we were last known to be standing in
        objCheck.Add( doorLst.Get( activeIndex )->pDoor );

        // Add the ajoining rooms attached to this doorway
        // to check for possible collision
        for( int i = 0; i < MAX_ROOMS_PER_DOOR; ++i )
        {
            objCheck.Add( doorLst.Get( activeIndex )->roomDoorLst.Get( i )->pRoom );
        }
    }

    // Check the differnt areas for collision
    for( int i = 0; i < objCheck.Count(); ++i )
    {
        polyIndex = 0;

        // Get a pointer to the mesh we are checking
        pMesh = objCheck.Get( i );

        do
        {
            // Check for a bounding sphere collision. This is fastest and we'll
            // use this as a means of finding only the closest polygons to do
            // our final test which is the intersection test.
            colType = pMesh->IsCollision_BoundSphere( polyIndex, dist, pt, FP_RADIUS );

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
                    pFloor = pMesh;
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
                    pWall = pMesh;
                }
            }
        }
        while( (polyIndex > -1) && (++polyIndex < pMesh->GetPolyCount()) );
    }
            
    // react to the floor if we touched one
    if( closestFloorIndex > -1 )
    {
        // reset the active area index
        activeIndex = pFloor->GetID();
        activeType =  (EOT_OBJECT_TYPE)pFloor->GetType();

        // Final check for floor collision. Slower then bounding sphere collision but very
        // accurate which is why we do it after the bounding sphere check.
        if( pFloor->IsCollision_Intersection( matrixYZ, closestFloorIndex, DIST_FROM_FLOOR, 
                                              pt, normal, pointD ) )
        {
            // Compute the y delta
            if( normal.y != 0 )
                y = ((normal.x + normal.z + pointD) / -normal.y) + DIST_FROM_FLOOR;
            else
                y = (normal.x + normal.z + pointD) + DIST_FROM_FLOOR;
            
            // Move the camera
            matrixYZ.Translate( 0, -y, 0 );
        }
    }

    // Only react to the wall if we are moving. If we hit the wall, do a wall glide
    if( (closestWallIndex > -1) && (buttonUpDown || buttonLeftRight) )
    {
        // Final check for wall collision. Will return
        // true if we are to go further.
        if( pWall->IsCollision_Intersection( matrixYZ, closestWallIndex, DIST_FROM_WALL, 
                                             pt, normal, pointD ) )
        {
            z = x = 0;

            // If we are going in the opposite direction of the wall we are
            // touching, don't do collision reaction because we don't want the
            // wall to hold onto us like a tractor beam.
            if( (normal.z <= 0 && buttonUpDown < 0) ||
                (normal.z >= 0 && buttonUpDown > 0) ||
                (normal.x <= 0 && buttonLeftRight < 0) ||
                (normal.x >= 0 && buttonLeftRight > 0) )
            {
                // Glide along the wall directly in front or behind us
                if( fabs(normal.z) > fabs(normal.x) )
                {
                    // The wall is in front of us
                    if( normal.z < 0 )
                    {
                        z = -(((normal.x + pointD) / (-normal.z)) + DIST_FROM_WALL);
                        
                        // Only use the normal value if we are going
                        // forward and need to glide along the wall.
                        if( buttonUpDown )
                            x = normal.x;

                        // Cap the speed so we don't snap to the wall
                        if( z > 0 )
                            z = 0;
                    }
                    // The wall is behind us
                    else
                    {
                        z = -(((normal.x + pointD) / (-normal.z)) - DIST_FROM_WALL);
                        
                        // Only use the normal value if we are going
                        // backwards and need to glide along the wall.
                        if( buttonUpDown )
                            x = -normal.x;

                        // Cap the speed so we don't snap to the wall
                        if( z < 0 )
                            z = 0;
                    }
                }
                // Glide along the wall to our right or left side
                else
                {
                    // The wall is on our left side
                    if( normal.x < 0 )
                    {
                        // Only use the normal value if we are going
                        // sideways and need to glide along the wall.
                        if( buttonLeftRight )
                            z = -normal.z;

                        x = -(((normal.z + pointD) / (-normal.x)) + DIST_FROM_WALL);

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
                            z = -normal.z;

                        x = -(((normal.z + pointD) / (-normal.x)) - DIST_FROM_WALL);

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
*    DESCRIPTION:     Regroup all the objects to help simplify
*                     the rendering and collision detection.
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::SetupMap()
{
    // Seperate the meshes into their lists
    for( int i = 0; i < meshCount; ++i )
    {
        if( pMesh[i].GetType() == EOT_MAP_ROOM )
        {
            // Allocate a room and add it to the room list
            CRoom *pRoom = new CRoom;

            // Copy the room pointer
            pRoom->pRoom = &pMesh[i];
            
            // Add the room pointer
            roomLst.Add( pRoom );
        }
        else if( pMesh[i].GetType() == EOT_MAP_DOORWAY )
        {
            // Allocate a door and add it to the list
            CDoor *pDoor = new CDoor;

            // Use a smaller radius for the doorway walls. Makes it easer
			// for wall collision detection. You don't wall glide to far after
			// going through the doorway
            for( int j = 0; j < pMesh[i].GetPolyCount(); ++j )
            {
                if( pMesh[i].GetPolyList()[j].GetCollisionType() == ECT_MAP_WALL )
                {
                    pMesh[i].GetPolyList()[j].SetRadius( pMesh[i].GetPolyList()[j].GetRadius() * 0.75 );
                }
            }

            // Copy the door pointer
            pDoor->pDoor = &pMesh[i];

            // Add the door pointer
            doorLst.Add( pDoor );
        }
    }

    // Go through all the rooms and find the doors
    // and other rooms that belong to them
    for( int i = 0; i < roomLst.Count(); ++i )
    {
        for( int j = 0; j < doorLst.Count(); ++j )
        {
            for( int w = 0; w < MAX_ROOMS_PER_DOOR; ++w )
            {
                // Add the door to the room if we find it
                if( doorLst.Get( j )->pDoor->GetAttachedRoom( w ) == i )
                {
                    // Add the door connected to this room
                    roomLst.Get( i )->doorLst.Add( doorLst.Get( j )->pDoor );

                    // Allocate a room and add it to the room list
                    CRoomDoors *pRoomDoors = new CRoomDoors;

                    // Copy the pointer to the other room this door is attached to
                    pRoomDoors->pRoom = roomLst.Get( doorLst.Get( j )->pDoor->GetAttachedRoom( !w ) )->pRoom;

                    // now go through and find all the doors attached to this room
                    for( int k = 0; k < doorLst.Count(); ++k )
                    {
                        for( int m = 0; m < MAX_ROOMS_PER_DOOR; ++m )
                        {
                            // Be sure not to add the same door we are
                            // already attached to
                            if( j != k )
                            {
                                if( doorLst.Get( j )->pDoor->GetAttachedRoom( !w ) == 
                                    doorLst.Get( k )->pDoor->GetAttachedRoom( m ) )
                                {
                                    pRoomDoors->doorLst.Add( doorLst.Get( k )->pDoor );
                                    break;
                                }
                            }
                        }
                    }

                    // Add the room connected to this door
                    roomLst.Get( i )->roomDoorLst.Add( pRoomDoors );

                    break;
                }
            }
        }
    }

    // Go through all the doors and add the 
    // rooms that they attach to.
    for( int i = 0; i < doorLst.Count(); ++i )
    {
        for( int j = 0; j < MAX_ROOMS_PER_DOOR; ++j )
        {
            // Allocate a room and add it to the room list
            CRoomDoors *pRoomDoors = new CRoomDoors;

            // Copy the pointer to the other room this door is attached to
            pRoomDoors->pRoom = roomLst.Get( doorLst.Get( i )->pDoor->GetAttachedRoom( j ) )->pRoom;

            // now go through and find all the doors attached to this room
            for( int k = 0; k < doorLst.Count(); ++k )
            {
                for( int m = 0; m < MAX_ROOMS_PER_DOOR; ++m )
                {
                    // Be sure not to add the same door we are
                    // already attached to
                    if( i != k )
                    {
                        if( doorLst.Get( i )->pDoor->GetAttachedRoom( j ) == 
                            doorLst.Get( k )->pDoor->GetAttachedRoom( m ) )
                        {
                            pRoomDoors->doorLst.Add( doorLst.Get( k )->pDoor );
                            break;
                        }
                    }
                }
            }
            
            doorLst.Get( i )->roomDoorLst.Add( pRoomDoors );
        }
    }

}   // SetupMap


/************************************************************************                                                             
*    DESCRIPTION:     Was this area rendered
*
*    Input:    int objIndex - active index of the room we are standing in
*              EOT_OBJECT_TYPE objType - object type
*
*    -------------------------------------------------------------------
*                  12/29/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
bool CMapMgr<mesh_type>::WasRendered( int objIndex, EOT_OBJECT_TYPE objType )
{
    // Object is in a room
    if( objType == EOT_MAP_ROOM )
    {
        return roomLst.Get( objIndex )->pRoom->WasRendered();
    }
    // Object is in a doorway
    else if( objType == EOT_MAP_DOORWAY )
    {
        return doorLst.Get( objIndex )->pDoor->WasRendered();
    }

}   // WasRendered


/************************************************************************                                                             
*    DESCRIPTION:     Is this bounding sphere visible through the list
*                     of rendered doorways?
*
*    Input:    int objIndex - active index of the room we are standing in
*              EOT_OBJECT_TYPE objType - object type
*              float radius - radius of object being checked
*              CPoint *pCenter - translated center point of object
*
*    -------------------------------------------------------------------
*                  12/29/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
bool CMapMgr<mesh_type>::IsVisableThruRenderedDoorways( int objIndex, EOT_OBJECT_TYPE objType,
                                                        float radius, CPoint *pCenter )
{
    // No sence doing any checks if we are in the same room or doorway as the object
	if( (objIndex == activeIndex) && (objType == activeType) )
	{
		return true;
	}

    // Make sure the object is not in the room of the doorway we are standing in
    if( activeType == EOT_MAP_DOORWAY )
	{
		if( objIndex == rendDoorLst.Get(0)->GetAttachedRoom( 0 ) ||
		    objIndex == rendDoorLst.Get(0)->GetAttachedRoom( 1 ) )
		{
			return true;
		}
	}

    bool result = false;

	for( int i = 0; i < rendDoorLst.Count() && !result; ++i )
	{
		// only check the door of the room the object is in
		if( objIndex == rendDoorLst.Get(i)->GetAttachedRoom( 0 ) ||
		    objIndex == rendDoorLst.Get(i)->GetAttachedRoom( 1 ) )
		{
			result = !rendDoorLst.Get(i)->CullMesh_BoundSphere( radius, pCenter );
		}
	}

    return result;

}   // IsVisableThruRenderedDoorways


/************************************************************************
*    DESCRIPTION:     Load an object from a MAP binary file. This is
*                     our own simple file for quickly loading
*                     vertices, face indexes, uv and texture indexes
*                     into the texture library.
*
*    Input:    char *filePath - Path to file to load
*
*    Output:   bool - true on success, false on fail
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
bool CMapMgr<mesh_type>::LoadFromFile( char *filePath )
{
    bool result = CMeshList<mesh_type>::LoadFromFile( filePath );

    return result;

}   // LoadFromFile


/************************************************************************
*    DESCRIPTION:     Load an object from a MAP binary file. This is
*                     our own simple file for quickly loading
*                     vertices, face indexes, uv and texture indexes
*                     into the texture library.
*
*    Input:    FILE *hFile - Handle to file
*
*    Output:   bool - true on success, false on fail
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
bool CMapMgr<mesh_type>::LoadFromFile( FILE *hFile )
{    
    return CMeshList<mesh_type>::LoadFromFile( hFile );

}   // LoadFromFile