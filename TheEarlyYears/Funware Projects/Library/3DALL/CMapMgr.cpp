
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
    #ifdef _render_all_
    RenderAll( m );
    #else

    doorCount = 0;
    roomCount = 0;
    hallwayCount = 0;
    stairwayCount = 0;
    objCount = 0;
    totalCount = 0;

    // Clear out all the render flags
    ClearFlags();

    // free all saved rendered doors
    rendDoorLst.Free();

    // We are standing on a polygon in a doorway
    if( activeType == EOT_MAP_DOORWAY )
    {        
        RenderDoor( m, debug );
    }
    // We are standing on a polygon in a room, hallway or stairway
    else
    {
        RenderRoom( m, debug );
    }

    sprintf( debug, "Door: %d;   hallway: %d;   Room: %d;   Stairway: %d;   Poly Count: %d;   Obj Count: %d",
             doorCount, hallwayCount, roomCount, stairwayCount, totalCount, objCount );
    #endif

}   // Render

/************************************************************************                                                             
*    DESCRIPTION:     We are standing in a room. Render that room with
*                     it's doors and ajoining rooms and doors.
*
*    Input:    CMeshMap *pMesh - Mest to be counted
*
*    -------------------------------------------------------------------
*    H Rosenorn    07/20/06      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
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
*    DESCRIPTION:     We are standing in a room. Render that room with
*                     it's doors and ajoining rooms and doors.
*
*    Input:    Matrix3D &M - Matris to do all the translations
*              char *debug -  debug info
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/17/05      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::RenderRoom( CMatrix &m, char *debug )
{
    // Render all the objects in this room
    totalCount += RenderObjects( m, &roomLst.Get( activeIndex )->objLst, objCount );

    // Render the active area first
    CMeshMap *pMesh = roomLst.Get( activeIndex )->pRoom;

    // Inc the debug counters
    IncCounters( pMesh );

    // render this area
    totalCount += pMesh->Render( m );

    // Render all alpha blended objects in this room
    totalCount += RenderObjects( m, &roomLst.Get( activeIndex )->alphaObjLst, objCount );

    // Get the pointer to the door list
    CObjLst<CMeshMap> *pDoorLst = &roomLst.Get( activeIndex )->doorLst;

    // Transform all the doors in the room we are standing in.
    // This way we can test for it later and not reprocess
    // the same doors that are part of the ajoining rooms.
    for( int i = 0; i < pDoorLst->Count(); ++i )
    {
        // Must transform the center point first before we do anything
        // because many functions rely on the translated center point.
        pDoorLst->Get( i )->TransformCenterPt( m );

        // Transfor the door so that it can be collision checked
        pDoorLst->Get( i )->Transform( m );
    }

    // Find the doors that are connected to this room
    // and check if we can see them
    for( int i = 0; i < pDoorLst->Count(); ++i )
    {
        // Get a pointer to the door mesh
        CMeshMap *pDoorMesh = pDoorLst->Get( i );

        bool closeToDoorWay = false;

        // If the doorway was NOT culled, render it
        // along with the ajoining room
        if( !pDoorMesh->WasRendered() && !pDoorMesh->CullMesh_BoundSphere() )
        {
            // Add the rendered door to our list
            rendDoorLst.Add( pDoorMesh );

            // Render the doorway
            totalCount += pDoorMesh->Render( m, false );
            ++doorCount;

            // Get the other room attached to this door
            CMeshMap *pRoomMesh = roomLst.Get( activeIndex )->roomDoorLst.Get( i )->pRoom;                

            // Inc the debug counters
            IncCounters( pRoomMesh );
            
            // Only render the polygons we can see through the doorway
            // Check to see how close we are to the doorway 
            if( pDoorMesh->GetDistance() > pDoorMesh->GetRadius() )
            {
                // Render all the objects in this room
                totalCount += RenderObjectsPortal( m, &roomLst.Get( activeIndex )->roomDoorLst.Get( i )->objLst, 
                                             objCount, pDoorMesh );
                
                // Two or more doors can connect to the same room so make sure we only
                // translate once.
                totalCount += pRoomMesh->Render( m, pDoorMesh->GetRadius(),
                                                 pDoorMesh->GetCenterPt(), 
                                                 !pRoomMesh->WasTransformed() );

                // Render all alpha blended objects in this room
                totalCount += RenderObjectsPortal( m, &roomLst.Get( activeIndex )->roomDoorLst.Get( i )->alphaObjLst, 
                                             objCount, pDoorMesh );
            }
            // The portal is useless when your right on top of it so don't bother using
            // with the rooms in front and behind us.
            else
            {
                // Render all the objects in this room
                totalCount += RenderObjects( m, &roomLst.Get( activeIndex )->roomDoorLst.Get( i )->objLst, 
                                             objCount, false );
                
                totalCount += pRoomMesh->Render( m, !pRoomMesh->WasTransformed() );
                closeToDoorWay = true;

                // Render all alpha blended objects in this room
                totalCount += RenderObjects( m, &roomLst.Get( activeIndex )->roomDoorLst.Get( i )->alphaObjLst, 
                                             objCount, false );
            }

            // Get the doors attached to the inner room
            CObjLst<CMeshMap> *pInnerDoorLst = &roomLst.Get( activeIndex )->roomDoorLst.Get( i )->doorLst;

            // Try to render the doorways in this room
            for( int j = 0; j < pInnerDoorLst->Count(); ++j )
            {
                CMeshMap *innerDoorMesh = pInnerDoorLst->Get( j );

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
                            CMeshMap *pInnerRoomMesh = roomLst.Get( innerDoorMesh->GetMapData(w) )->pRoom;

                            // Inc the debug counters
                            IncCounters( pInnerRoomMesh );

                            pInnerRoomMesh->Render( m, innerDoorMesh->GetRadius(),
                                                    innerDoorMesh->GetCenterPt(),
                                                    !pInnerRoomMesh->WasTransformed() );
                        }
                    }
                }
            }   
        }
    }

}   // RenderRoom


/************************************************************************                                                             
*    DESCRIPTION:     We are standing in a doorway. Render that doorway
*                     with it's ajoining rooms and doors.
*
*    Input:    Matrix3D &M - Matris to do all the translations
*              char *debug -  debug info
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/17/05      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::RenderDoor( CMatrix &m, char *debug )
{
    // Render about the door we are standing in
    totalCount += doorLst.Get( activeIndex )->pDoor->Render( m );
    ++doorCount;

    // Add the rendered door to our list
    rendDoorLst.Add( doorLst.Get( activeIndex )->pDoor );

    // Try to render the two rooms that join the doorway
    for( int i = 0; i < MAX_ROOMS_PER_DOOR; ++i )
    {
        // Render all the objects in this room
        totalCount += RenderObjects( m, &doorLst.Get( activeIndex )->roomDoorLst.Get( i )->objLst, objCount, false );
        
        CMeshMap *pMesh = doorLst.Get( activeIndex )->roomDoorLst.Get( i )->pRoom;

        // Inc the debug counters
        IncCounters( pMesh );

        // Render this area
        totalCount += pMesh->Render( m );

        // Render all alpha blended objects in this room
        totalCount += RenderObjects( m, &doorLst.Get( activeIndex )->roomDoorLst.Get( i )->alphaObjLst, objCount, false );

        // Get the pointer to the door list
        CObjLst<CMeshMap> *pDoorLst = &doorLst.Get( activeIndex )->roomDoorLst.Get( i )->doorLst;

        // render the doors you can see
        for( int j = 0; j < pDoorLst->Count(); ++j )
        {
            // Get a pointer to the door mesh
            CMeshMap *pDoorMesh = pDoorLst->Get( j );

            // If the doorway was NOT culled, see if it should be rendered
            if( !pDoorMesh->WasCullTested() && !pDoorMesh->WasRendered() )
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
                        CMeshMap *pInnerRoomMesh = roomLst.Get( pDoorMesh->GetMapData(w) )->pRoom;

                        // Inc the debug counters
                        IncCounters( pInnerRoomMesh );

                        // Render all the objects in this room
                        totalCount += RenderObjectsPortal( m, &roomLst.Get( pDoorMesh->GetMapData(w) )->objLst,
                                                     objCount, pDoorMesh );

                        totalCount += pInnerRoomMesh->Render( m, pDoorMesh->GetRadius(),
                                                              pDoorMesh->GetCenterPt(),
                                                              !pInnerRoomMesh->WasTransformed() );

                        // Render all alpha blended objects in this room
                        totalCount += RenderObjectsPortal( m, &roomLst.Get( pDoorMesh->GetMapData(w) )->alphaObjLst,
                                                     objCount, pDoorMesh );
                    }
                }
            }
        }
    }

}   // RenderDoor


/************************************************************************                                                             
*    DESCRIPTION:     We are standing in a doorway. Render that doorway
*                     with it's ajoining rooms and doors.
*
*    Input:    Matrix3D &M - Matris to do all the translations
*              CObjLst<CMeshMap> *pObjLst -  List of objects to render
*              int &objCount - number of objects
*              bool transformCol - do transformations on collision objs
*              CMeshMap *pMeshPortal - Mesh to be used as a portal
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/17/05      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
int CMapMgr<mesh_type>::RenderObjects( CMatrix &m, CObjLst<CMeshMap> *pObjLst,
                                       int &objCount, bool transformCol )
{
    int count = 0;

    // Render all the objects in this room
    for( int i = 0; i < pObjLst->Count(); ++i )
    {
        CMeshMap *pObjMesh = pObjLst->Get(i);

        // Only transform the collision object because it is not rendered.
        if( pObjMesh->GetType() == EOT_MAP_COLLISION )
        {
            if( transformCol )
            {
                pObjMesh->Transform( m );
            }
        }
        else  // Render the rest of the objects
        {
            // Must transform the center point for object culling.
            pObjMesh->TransformCenterPt( m );

            // Transfor objectr so that it can be collision checked
            if( pObjMesh->GetCollisionChecking() )
            {
                pObjMesh->Transform( m );
            }

            if( !pObjMesh->CullMesh_BoundSphere() )
            {
                count += pObjMesh->Render( m, !pObjMesh->WasTransformed() );

                if( count )
                {
                    ++objCount;
                }
            }
        }
    }

    return count;

}   // RenderObjects


/************************************************************************                                                             
*    DESCRIPTION:     We are standing in a doorway. Render that doorway
*                     with it's ajoining rooms and doors.
*
*    Input:    Matrix3D &M - Matris to do all the translations
*              CObjLst<CMeshMap> *pObjLst -  List of objects to render
*              int &objCount - number of objects
*              bool transformCol - do transformations on collision objs
*              CMeshMap *pMeshPortal - Mesh to be used as a portal
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/17/05      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
int CMapMgr<mesh_type>::RenderObjectsPortal( CMatrix &m, CObjLst<CMeshMap> *pObjLst,
                                             int &objCount, CMeshMap *pMeshPortal )
{
    int count = 0;

    // Render all the objects in this room
    for( int i = 0; i < pObjLst->Count(); ++i )
    {
        CMeshMap *pObjMesh = pObjLst->Get(i);

        // Only render seperate objects
        if( pObjMesh->GetType() == EOT_MAP_OBJECT ||
            pObjMesh->GetType() == EOT_MAP_OBJ_COL )
        {
            // Must transform the center point for object culling.
            pObjMesh->TransformCenterPt( m );

            if( !pObjMesh->CullMesh_BoundSphere( pMeshPortal->GetRadius(),
                                                 pMeshPortal->GetCenterPt() ) )
            {
                count += pObjMesh->Render( m, pMeshPortal->GetRadius(),
                                           pMeshPortal->GetCenterPt(), 
                                           !pObjMesh->WasTransformed() );

                if( count )
                {
                    ++objCount;
                }
            }
        }
    }

    return count;

}   // RenderObjects


/************************************************************************                                                             
*    DESCRIPTION:     Check for collision and react to it. I feel
*                     that collision reaction is best when it is
*                     handled outside the engine because every
*                     situation is different. Let the engine indicate
*                     it has hit something, but that is it.
*
*    Input:    Matrix3D &M - Matrix to do all the translations
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
    
    // We are standing on a polygon in a doorway
    if( activeType == EOT_MAP_DOORWAY )
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
    // We are standing on a polygon in a room
    else
    {
        // Add the room we were last known to be standing in
        objCheck.Add( roomLst.Get( activeIndex )->pRoom );

        // Add all the doorways attached to this room to check
        // for possible collision if we are close
        for( int i = 0; i < roomLst.Get( activeIndex )->doorLst.Count(); ++i )
        {
            objCheck.Add( roomLst.Get( activeIndex )->doorLst.Get(i) );
        }

        // Add all the objects attached to this room for collision detection
        for( int i = 0; i < roomLst.Get( activeIndex )->objLst.Count(); ++i )
        {
            // Only do collision detection on objects tagged for collision detection
            if( roomLst.Get( activeIndex )->objLst.Get(i)->GetType() != EOT_MAP_OBJECT )
            {
                objCheck.Add( roomLst.Get( activeIndex )->objLst.Get(i) );
            }
        }

        // Add all alpha blended objects attached to this room for collision detection
        for( int i = 0; i < roomLst.Get( activeIndex )->alphaObjLst.Count(); ++i )
        {
            // Only do collision detection on objects tagged for collision detection
            if( roomLst.Get( activeIndex )->alphaObjLst.Get(i)->GetType() != EOT_MAP_OBJECT )
            {
                objCheck.Add( roomLst.Get( activeIndex )->alphaObjLst.Get(i) );
            }
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
        activeType = pFloor->GetType();

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
        if( (pMesh[i].GetType() == EOT_MAP_ROOM) ||
            (pMesh[i].GetType() == EOT_MAP_HALLWAY) ||
            (pMesh[i].GetType() == EOT_MAP_STAIRWAY) )
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
                    pMesh[i].GetPolyList()[j].SetRadius( pMesh[i].GetPolyList()[j].GetRadius() * 0.5 );
                }
            }

            // Copy the door pointer
            pDoor->pDoor = &pMesh[i];

            // Add the door pointer
            doorLst.Add( pDoor );
        }
    }

    // Don't setup all the room and door because we might be trying to debug a problem
    #ifndef _render_all_

    // Go through all the meshes and add the objects to the rooms
    for( int i = 0; i < meshCount; ++i )
    {
        if( pMesh[i].GetType() == EOT_MAP_OBJECT ||
            pMesh[i].GetType() == EOT_MAP_OBJ_COL ||
            pMesh[i].GetType() == EOT_MAP_COLLISION )
        {
            // Check to see if this object uses alpha blended textures
            if( pMesh[i].GetMapData( 1 ) == OBJECT_WITH_ALPHA_TXT )
            {
                roomLst.Get( pMesh[i].GetMapData( 0 ) )->alphaObjLst.Add( &pMesh[i] );
            }
            else
            {
                roomLst.Get( pMesh[i].GetMapData( 0 ) )->objLst.Add( &pMesh[i] );
            }

            if( pMesh[i].GetType() == EOT_MAP_COLLISION )
            {
                pMesh[i].SetWallFloorCollisionType();
            }

            // determin if this object requires collision testing.
            // When in a room that has objects and they require collision detection
            // they always have to be transformed.
            for( int j = 0; j < pMesh[i].GetPolyCount(); ++j )
            {
                if( pMesh[i].GetPolyList()[j].GetCollisionType() > ECT_NO_TYPE )
                {
                    pMesh[i].SetCollisionChecking( true );
                    break;
                }
            }
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
                if( doorLst.Get( j )->pDoor->GetMapData( w ) == i )
                {
                    // Add the door connected to this room
                    roomLst.Get( i )->doorLst.Add( doorLst.Get( j )->pDoor );

                    // Allocate a room and add it to the room list
                    CRoomDoors *pRoomDoors = new CRoomDoors;

                    // Copy the pointer to the other room this door is attached to
                    pRoomDoors->pRoom = roomLst.Get( doorLst.Get( j )->pDoor->GetMapData( !w ) )->pRoom;

                    // Add the objects to the list
                    for( int k = 0; k < roomLst.Get( doorLst.Get( j )->pDoor->GetMapData( !w ) )->objLst.Count(); ++k )
                    {
                        pRoomDoors->objLst.Add( roomLst.Get( doorLst.Get( j )->pDoor->GetMapData( !w ) )->objLst.Get( k ) );
                    }

                    // Add the alpha objects to the list
                    for( int k = 0; k < roomLst.Get( doorLst.Get( j )->pDoor->GetMapData( !w ) )->alphaObjLst.Count(); ++k )
                    {
                        pRoomDoors->alphaObjLst.Add( roomLst.Get( doorLst.Get( j )->pDoor->GetMapData( !w ) )->alphaObjLst.Get( k ) );
                    }

                    // now go through and find all the doors attached to this room
                    for( int k = 0; k < doorLst.Count(); ++k )
                    {
                        for( int m = 0; m < MAX_ROOMS_PER_DOOR; ++m )
                        {
                            // Be sure not to add the same door we are
                            // already attached to
                            if( j != k )
                            {
                                if( doorLst.Get( j )->pDoor->GetMapData( !w ) == 
                                    doorLst.Get( k )->pDoor->GetMapData( m ) )
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
            pRoomDoors->pRoom = roomLst.Get( doorLst.Get( i )->pDoor->GetMapData( j ) )->pRoom;

            // Add the objects to the list
            for( int k = 0; k < roomLst.Get( doorLst.Get( i )->pDoor->GetMapData( j ) )->objLst.Count(); ++k )
            {
                pRoomDoors->objLst.Add( roomLst.Get( doorLst.Get( i )->pDoor->GetMapData( j ) )->objLst.Get( k ) );
            }

            // Add the alpha objects to the list
            for( int k = 0; k < roomLst.Get( doorLst.Get( i )->pDoor->GetMapData( j ) )->alphaObjLst.Count(); ++k )
            {
                pRoomDoors->alphaObjLst.Add( roomLst.Get( doorLst.Get( i )->pDoor->GetMapData( j ) )->alphaObjLst.Get( k ) );
            }

            // now go through and find all the doors attached to this room
            for( int k = 0; k < doorLst.Count(); ++k )
            {
                for( int m = 0; m < MAX_ROOMS_PER_DOOR; ++m )
                {
                    // Be sure not to add the same door we are
                    // already attached to
                    if( i != k )
                    {
                        if( doorLst.Get( i )->pDoor->GetMapData( j ) == 
                            doorLst.Get( k )->pDoor->GetMapData( m ) )
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

    #endif

}   // SetupMap


/************************************************************************                                                             
*    DESCRIPTION:     Was this area rendered
*
*    Input:    CMapType &mapType -  map type and index
*
*    -------------------------------------------------------------------
*                  12/29/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
bool CMapMgr<mesh_type>::WasRendered( CMapType &mapType )
{   
    bool fRendered;

    // Object is in a doorway
    if( mapType.activeType == EOT_MAP_DOORWAY )
    {
        fRendered = doorLst.Get( mapType.activeIndex )->pDoor->WasRendered();
    }
    // Object is in some type of room
    else
    {
        fRendered = roomLst.Get( mapType.activeIndex )->pRoom->WasRendered();
    }

    return fRendered;

}   // WasRendered


/************************************************************************                                                             
*    DESCRIPTION:     Is this bounding sphere visible through the list
*                     of rendered doorways?
*
*    Input:    CMapType &mapType -  map type and index
*              float radius - radius of object being checked
*              CPoint *pCenter - translated center point of object
*
*    -------------------------------------------------------------------
*                  12/29/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
bool CMapMgr<mesh_type>::IsVisableThruRenderedDoorways( CMapType &mapType,
                                                        float radius, CPoint *pCenter )
{
    // No sence doing any checks if we are in the same room or doorway as the object
    if( (mapType.activeIndex == activeIndex) && (mapType.activeType == activeType) )
    {
        return true;
    }

    // Make sure the object is not in the room of the doorway we are standing in
    if( activeType == EOT_MAP_DOORWAY )
    {
        if( mapType.activeIndex == rendDoorLst.Get(0)->GetMapData( 0 ) ||
            mapType.activeIndex == rendDoorLst.Get(0)->GetMapData( 1 ) )
        {
            return true;
        }
    }

    bool result = false;

    if( mapType.activeType == EOT_MAP_DOORWAY )
    {
        // Check the door id's to see if it is the same as the one the object standing on
        for( int i = 0; i < rendDoorLst.Count() && !result; ++i )
        {
            if( rendDoorLst.Get(i)->GetID() == mapType.activeIndex )
            {
                return true;
            }
        }
    }
    else
    {
        // only check the door radius of the room the object is in to see
        // if it can be seen through that doorway
        for( int i = 0; i < rendDoorLst.Count() && !result; ++i )
        {
            if( mapType.activeIndex == rendDoorLst.Get(i)->GetMapData( 0 ) ||
                mapType.activeIndex == rendDoorLst.Get(i)->GetMapData( 1 ) )
            {
                result = !rendDoorLst.Get(i)->CullMesh_BoundSphere( radius, pCenter );
            }
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


/************************************************************************
*    DESCRIPTION:     Set the starting position to the spawn point
*
*    Input:    Matrix3D &matrixYZ - Matrix to do all the translations
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::SetSpawnPoint( CMatrix &matrix )
{
    bool spawnFound = false;
    int spawnIndex;
    
    // Go through all the meshes and find the spawn point
    for( int i = 0; i < meshCount; ++i )
    {
        if( pMesh[i].GetType() == EOT_MAP_SPAWN_POINT )
        {
            activeIndex = pMesh[i].GetMapData(0);
            activeType = roomLst.Get( activeIndex )->pRoom->GetType();
            spawnFound = true;
            spawnIndex = i;
            break;
        }
    }

    // spawn found. Translate to that position
    if( spawnFound == true )
    {
        matrix.Translate( -pMesh[spawnIndex].GetCenterPt()->x,
                          -pMesh[spawnIndex].GetCenterPt()->y, 
                          -pMesh[spawnIndex].GetCenterPt()->z );
    }

}   // SetSpawnPoint


/************************************************************************
*    DESCRIPTION:     Set the bot spawn point
*
*    Input:  Matrix3D &matrixYZ - Matrix to do all the translations
*            CMapType &mapType -  map type and index
*            EOT_OBJECT_TYPE spawnPoint - spawn point
*            int spawnIndex - index of spawn point
*            float dir - direction of matrix
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::SetToSpawnPoint( CMatrix &matrix,
                                          CMapType &mapType,
                                          EOT_OBJECT_TYPE spawnPoint,
                                          int spawnIndex,
                                          float dir )
{
    bool spawnFound = false;
    int meshIndex;
    int spawnCounter = 0;
    
    // Go through all the meshes and find the bot spawn point
    for( int i = 0; i < meshCount; ++i )
    {
        if( pMesh[i].GetType() == spawnPoint )
        {
            if( spawnCounter == spawnIndex )
            {
                mapType.activeIndex = pMesh[i].GetMapData(0);
                mapType.activeType = roomLst.Get( activeIndex )->pRoom->GetType();
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

}   // SetToSpawnPoint


/************************************************************************                                                             
*    DESCRIPTION:     Check for collision and react to it. I feel
*                     that collision reaction is best when it is
*                     handled outside the engine because every
*                     situation is different. Let the engine indicate
*                     it has hit something, but that is it.
*
*    Input:  Matrix3D &M - Matrix to do all the translations
*            CMapType &mapType -  map type and index
*            float radius - radius of object
*
*    -------------------------------------------------------------------
*                  12/29/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
void CMapMgr<mesh_type>::ReactToFloorCollisionOnly( CMatrix &matrix,
                                                    CMapType &mapType,
                                                    CPoint &objPoint,
                                                    float radius,
                                                    float offset )
{
    int polyIndex, colType;
    float y, pointD; 
    double dist;
    CPoint normal;
    CMeshMap *pFloor, *pMesh;
    CObjLst<CMeshMap> objCheck;
    objCheck.DontFreeData();

    // Start the inital distances as far away so we can see if
    // we are getting closer. We only want to collide (do Intersection Collision)
    // with the closest polygons.
    double closestFloorDistance=radius*20;
    int closestFloorIndex = -1;

    ////////////////////////////////////////
    // Make a temporary list of objects to
    // check for collision
    ////////////////////////////////////////
    
    // We are standing on a polygon in a doorway
    if( mapType.activeType == EOT_MAP_DOORWAY )
    {
        MessageBeep(0);
        // Add the doorway we were last known to be standing in
        objCheck.Add( doorLst.Get( mapType.activeIndex )->pDoor );

        // Add the ajoining rooms attached to this doorway
        // to check for possible collision
        for( int i = 0; i < MAX_ROOMS_PER_DOOR; ++i )
        {
            objCheck.Add( doorLst.Get( mapType.activeIndex )->roomDoorLst.Get( i )->pRoom );
        }
    }
    // We are standing on a polygon in a room
    else
    {
        // Add the room we were last known to be standing in
        objCheck.Add( roomLst.Get( mapType.activeIndex )->pRoom );

        // Add all the doorways attached to this room to check
        // for possible collision if we are close
        for( int i = 0; i < roomLst.Get( mapType.activeIndex )->doorLst.Count(); ++i )
        {
            objCheck.Add( roomLst.Get( mapType.activeIndex )->doorLst.Get(i) );
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
            colType = pMesh->IsCollision_BoundSphereSt( polyIndex, dist, objPoint, radius, ECT_MAP_FLOOR );

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
        }
        while( (polyIndex > -1) && (++polyIndex < pMesh->GetPolyCount()) );
    }
            
    // react to the floor if we touched one
    if( closestFloorIndex > -1 )
    {
        // reset the active area index
        mapType.activeIndex = pFloor->GetID();
        mapType.activeType = pFloor->GetType();

        // Final check for floor collision. Slower then bounding sphere collision but very
        // accurate which is why we do it after the bounding sphere check.
        if( pFloor->IsCollision_IntersectionSt( matrix, closestFloorIndex, radius, 
                                                objPoint, normal, pointD ) )
        {
            // Compute the y delta
            if( normal.y != 0 )
                y = ((normal.x + normal.z + pointD) / -normal.y) + offset;
            else
                y = (normal.x + normal.z + pointD) + offset;
            
            // Move the camera
            matrix.Translate( 0, y, 0 );
        }
    }
}   // ReactToFloorCollisionOnly


/************************************************************************                                                             
*    DESCRIPTION:     Check for collision and react to it. I feel
*                     that collision reaction is best when it is
*                     handled outside the engine because every
*                     situation is different. Let the engine indicate
*                     it has hit something, but that is it.
*
*    Input:    Matrix3D &M - Matrix to do all the translations
*              int buttonUpDown - button up down flag
*              nt buttonLeftRight  - button left right flag
*
*    -------------------------------------------------------------------
*                  12/29/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
int CMapMgr<mesh_type>::CheckForWallCollision( CMapType &mapType,
                                               CPoint * objPoint,
                                               float radius )
{
    int polyIndex, colType, collisionCount(0);
    //float x, z, y, pointD; 
    double dist;
    CPoint normal;
    CMeshMap *pFloor, *pWall, *pMesh;
    CObjLst<CMeshMap> objCheck;
    objCheck.DontFreeData();
    bool collisionFound = false;

    // Start the inital distances as far away so we can see if
    // we are getting closer. We only want to collide (do Intersection Collision)
    // with the closest polygons.
    double closestWallDistance=DIST_FROM_WALL*20;
    int closestWallIndex = -1;

    ////////////////////////////////////////
    // Make a temporary list of objects to
    // check for collision
    ////////////////////////////////////////
    
    // We are standing on a polygon in a doorway
    if( mapType.activeType == EOT_MAP_DOORWAY )
    {
        // Add the doorway we were last known to be standing in
        objCheck.Add( doorLst.Get( mapType.activeIndex )->pDoor );

        // Add the ajoining rooms attached to this doorway
        // to check for possible collision
        for( int i = 0; i < MAX_ROOMS_PER_DOOR; ++i )
        {
            objCheck.Add( doorLst.Get( mapType.activeIndex )->roomDoorLst.Get( i )->pRoom );
        }
    }
    // We are standing on a polygon in a room
    else
    {
        // Add the room we were last known to be standing in
        objCheck.Add( roomLst.Get( mapType.activeIndex )->pRoom );

        // Add all the doorways attached to this room to check
        // for possible collision if we are close
        for( int i = 0; i < roomLst.Get( mapType.activeIndex )->doorLst.Count(); ++i )
        {
            objCheck.Add( roomLst.Get( mapType.activeIndex )->doorLst.Get(i) );
        }
    }

    // Check the differnt areas for collision
    for( int i = 0; i < objCheck.Count() && !collisionFound; ++i )
    {
        polyIndex = 0;

        // Get a pointer to the mesh we are checking
        pMesh = objCheck.Get( i );

        do
        {
            colType = pMesh->IsCollision_BoundSphereSt( polyIndex, dist, objPoint[0], radius, ECT_MAP_WALL );

            ///////////////////////////
            // Did we hit a wall?
            ///////////////////////////
            if( colType == ECT_MAP_WALL )
            {
                collisionFound = true;
                ++collisionCount;
            }
        }
        while( (polyIndex > -1) && (++polyIndex < pMesh->GetPolyCount()) );
    }

    collisionFound = false;

    // Check the differnt areas for collision
    for( int i = 0; i < objCheck.Count() && !collisionFound; ++i )
    {
        polyIndex = 0;

        // Get a pointer to the mesh we are checking
        pMesh = objCheck.Get( i );

        do
        {
            colType = pMesh->IsCollision_BoundSphereSt( polyIndex, dist, objPoint[1], radius, ECT_MAP_WALL );

            ///////////////////////////
            // Did we hit a wall?
            ///////////////////////////
            if( colType == ECT_MAP_WALL )
            {
                collisionFound = true;
                ++collisionCount;
            }
        }
        while( (polyIndex > -1) && (++polyIndex < pMesh->GetPolyCount()) );
    }

    return collisionCount;

}   // ReactToCollision