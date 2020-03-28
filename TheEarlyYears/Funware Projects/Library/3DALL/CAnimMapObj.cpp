/************************************************************************
*
*    PRODUCT:         Animated map Object
*
*    FILE NAME:       CAnimMapObj.cpp
*
*    DESCRIPTION:     Animated object that is map aware 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#include "CAnimMapObj.h"

/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    -------------------------------------------------------------------
*    H Rosenorn    05/02/05      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
CAnimMapObj<mesh_type>::CAnimMapObj()
{
    radius = 0.0f;
	floorOffset = 0.0f;
    pMap = NULL;
}


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    -------------------------------------------------------------------
*    H Rosenorn    05/02/05      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
CAnimMapObj<mesh_type>::~CAnimMapObj()
{
}


/************************************************************************                                                             
*    DESCRIPTION:           Set the map that's currently being used
*
*    CMapMgr<CMeshMap> * map - Set the loaded map
*    float radt - radius of object for world view
*    float offset - object offset from floor
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/19/05      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
void CAnimMapObj<mesh_type>::SetMap( CMapMgr<CMeshMap> * map, float rad, float offset )
{
    pMap = map;
    radius = rad;
	floorOffset = offset;

}   // SetMap


/************************************************************************                                                             
*    DESCRIPTION:    Use a spawn point to position the animated object
*
*            EOT_OBJECT_TYPE spawnPoint - spawn point
*            int spawnIndex - index of spawn point
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/19/05      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
void CAnimMapObj<mesh_type>::SetToSpawnPoint( EOT_OBJECT_TYPE spawnPoint,
                                              int spawnIndex )
{
    if( pMap != NULL )
    {
        pMap->SetToSpawnPoint( matrix, mapType,
                               spawnPoint, spawnIndex, 1 );
    }
}   // SetMap


/************************************************************************                                                            
*    DESCRIPTION:           Render the animated frame
*
*    Input:    CMatrix &m - move matrix
*
*    Output:   int - 0 or 1 for counting active character meshes
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
int CAnimMapObj<mesh_type>::Render( CMatrix &m )
{
    int result = 0;

    // If we have a map loaded, use it to determin if we are to render the animation.
    // If the area this animation is located was NOT rendered, then it can't be seen
    // and we can jump out now. Very quick, simple check
    if( !pMap->WasRendered( mapType ) )
        return result;

	// Do the final view transformation
	DoViewTransformation( m );

	// Render only if it can be seen through the doorway. If the animation and the
    // view are in the same room, this function returns true.
    // Only check if animation is in a room. If it was in a doorway then the WasRendered
    // is the only check we need because if the doorway was rendered then the animation
    // should automatically be rendered too.
    if( pMap->IsVisableThruRenderedDoorways( mapType, 
                                             pMesh[frameIndex].GetRadius(),
                                             pMesh[frameIndex].GetCenterPt() ) )
	{
		// HAve the parent class handle the render
    	result = CAnimMeshLst<mesh_type>::Render( m );
	}

    return result;

}   // Render