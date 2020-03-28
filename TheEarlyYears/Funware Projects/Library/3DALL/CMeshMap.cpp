
/************************************************************************
*
*    FILE NAME:       CMeshMap.cpp
*
*    DESCRIPTION:     Mesh container for maps
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

#include "CMeshMap.h"        // Header file for this *.cpp file.


/************************************************************************
*    DESCRIPTION:   Constructer                                                             
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04             Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
CMeshMap::CMeshMap()
{
    // init class members
    memset( mapData, 0, sizeof(mapData) );
    collisionChecked = false;

}   // Constructer


/************************************************************************
*    DESCRIPTION:  Destructer                                                             
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
CMeshMap::~CMeshMap()
{

}   // Destructer


/************************************************************************
*    DESCRIPTION:  Virtual function for loading inherited class
*                  specific data
*
*    Input:    FILE *hFile - Handle to file
*
*    Output:   Output:   bool - true on success, false on fail
*
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CMeshMap::LoadExtraMeshData( FILE *hFile )
{
    // read the room indexes
    if( fread( mapData, sizeof( mapData ), 1, hFile ) != 1 )
        return false;

    return true;

}   // LoadExtraMeshData


/************************************************************************
*    DESCRIPTION:  Virtual function for savinging inherited class
*                  specific data
*
*    Input:    FILE *hFile - Handle to file
*
*    Output:   Output:   bool - true on success, false on fail
*
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CMeshMap::SaveExtraMeshData( FILE *hFile )
{
    // Write out the room indexes
    if( fwrite( mapData, sizeof( mapData ), 1, hFile ) != 1 )
        return false;

    return true;

}   // SaveExtraMeshData


/************************************************************************
*    DESCRIPTION:  Depending on the angle of the normal, set the polygon
*                  Collision type to wall or floor.
*
*    -------------------------------------------------------------------
*    H Rosenorn    03/18/05      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void CMeshMap::SetWallFloorCollisionType()
{
    // Only set collision if this object is one of these types
    if( type == EOT_MAP_ROOM ||
	    type == EOT_MAP_HALLWAY ||
	    type == EOT_MAP_STAIRWAY ||
        type == EOT_MAP_DOORWAY || 
        type == EOT_MAP_COLLISION ||
        type == EOT_MAP_OBJ_COL )
    {
        bool wallFloorColSet = false;

        // Check to see if a wall or floor has allready been set
        for( int i = 0; i < polyCount; ++i )
        {
            if( pList[i].GetCollisionType() == ECT_MAP_FLOOR ||
                pList[i].GetCollisionType() == ECT_MAP_WALL )
            {
                wallFloorColSet = true;
                break;
            }
        }

        // Only do the wall/floor collision if it has not already been set.
        if( wallFloorColSet == false )
        {
            CMesh::SetWallFloorCollisionType();
        }
    }

}   // SetWallFloorCollisionType