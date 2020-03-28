
/************************************************************************
*
*    FILE NAME:       CTSParser.cpp
*
*    DESCRIPTION:     Parse True Space 3D files
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

#include <string.h>          // string functions
#include "CTSParser.h"       // Header file for this *.cpp file.
#include "CMatrix.h"         // Matrix header
#include "CMeshList.h"       // mesh list class
#include "gTools.h"

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

// These are globals used by the 3D engine and must be setup
float gfMaxZDistance = -1000;
float gfMinZDistance = -1.0f;
int gCurrentID=0;
float gfSquarePercentage;
float gfFrustrumYRatio;
float gfAspectRatio;
int giRenderDevice;
int gQuadCounter;
float gfCameraScaleX;
float gfCameraScaleY;
float gfBufWidthDiv2;
float gfBufHeightDiv2;
int giBufWidth;
int giBufHeight;
unsigned int *gusiVBufer;
int *giZBuffer;
// DirectX may or maynot be turned on
#ifdef _USE_DIRECTX_9_
    LPDIRECT3DVERTEXBUFFER9 g_DirectX_VB = NULL;
    LPDIRECT3DDEVICE9 g_pDXDevice = NULL;
#endif
#ifdef _USE_DIRECTX_8_
    LPDIRECT3DVERTEXBUFFER8 g_DirectX_VB = NULL;
    LPDIRECT3DDEVICE8 g_pDXDevice = NULL;
#endif


/************************************************************************
*    DESCRIPTION:  Constructer                                                             
*
*    -------------------------------------------------------------------
*                  11/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
CTrueSpaceParser::CTrueSpaceParser()
{
	memset( typeCounter, 0, sizeof(typeCounter) );
	floorCounter = -1;
	maternialIndex = 0;

    errorCode = NO_ERROR_DETECTED;

}   // Constructor


/************************************************************************
*    DESCRIPTION:  Destructer                                                             
*
*    -------------------------------------------------------------------
*                  11/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
CTrueSpaceParser::~CTrueSpaceParser()
{
	DeleteVectorPointers( groupLst );
	DeleteVectorPointers( vertLst );
	DeleteVectorPointers( faceLst );

}   // Destructor


/************************************************************************
*    DESCRIPTION:  Load OBJ text file and parse it. This export
*                  has two files. The OBJ file refers to a
*                  MTL file for all the texture file information.
*
*    Input:    char *fileName - Path to load file
*
*    Output:   bool - true on success, false on error
************************************************************************/   
bool CTrueSpaceParser::LoadFromFile( const char *filePath )
{
    bool result = false;
    char lineStr[MAX_LINE], fileName[400];
    int strCount;
    FILE *hFile;
    SCollisionType colType;

    // Reset the error file path
    errorFilePath[0] = '\0';

    // We need a local copy of the file path because
    // we will be changing it.
    strcpy( fileName, filePath );

    // Open the file
    hFile = fopen( fileName, "rt" );
    if( hFile == NULL )
    {
        errorCode = ERROR_OBJ_FILE_HANDLE_FAIL;
        goto HANDLE_ERROR;
    }

    // Inc the floor counter
	++floorCounter;

    ///////////////////////////////////////
    // Make sure this is the ascii file
    ///////////////////////////////////////

    // Set the starting state
    state = EPS_NO_STATE;

    // Loop until we come to the end of the file
    while( 1 )
    {
        // Get a line of text
        strCount = GetLine( hFile, lineStr );

        // Did we reach the end of the file?
        if( strCount == EOF )
            break;

        // Is there nothing to look at?
        else if( strCount == 0 )
            continue;

        // Not the ascii file format. Jump out
        if( strcmp( lineStr, "Caligari" ) == 0 )
        {
            int count = 0;
            // Jump past the null
            while( lineStr[count++] )
            {}

            if( strcmp( &lineStr[count], "V00.01BLH" ) == 0 )
            {
                // no need to cause an error
                result = true;
            
                // Set the starting state
                state = EPS_NO_STATE;

                goto HANDLE_ERROR;
            }
        }
        else if( strcmp( lineStr, "Name" ) == 0 )
        {
            // Decode the center position of the object
            Decode_ObjectName( lineStr, strCount );

            // Set the starting state
            state = EPS_NO_STATE;
        }
        else if( strcmp( lineStr, "center" ) == 0 )
        {
            // Decode the center position of the object
            Decode_Center( hFile, lineStr, strCount );

            // init the counter
            matIndex = 0;

            // Set the starting state
            state = EPS_CENTER;
        }
        else if( strcmp( lineStr, "Transform" ) == 0 )
        {
            // init the counter
            matIndex = 0;

            // Set the starting state
            state = EPS_TRANSFORM;
        }
        else if( strcmp( lineStr, "World" ) == 0 )
        {
            // add a new group
            if( !AddGroup() )
                goto HANDLE_ERROR;

            // Set the starting state
            state = EPS_VERTEX;
        }
        else if( strcmp( lineStr, "Texture" ) == 0 )
        {
            // Set a space and set again
            lineStr[7] = ' ';

            // Refine the search
            if( strcmp( lineStr, "Texture Vertices" ) == 0 )
            {
                // Set the starting state
                state = EPS_UV;
            }
        }
        else if( strcmp( lineStr, "Faces" ) == 0 )
        {
            // Set the starting state
            state = EPS_NO_STATE;
        }
        else if( strcmp( lineStr, "Face" ) == 0 )
        {
            if( !AddFaceToGroup( lineStr, strCount ) )
                goto HANDLE_ERROR;

            // Set the starting state
            state = EPS_FACE;
        }
		else if( strcmp( lineStr, "mat#" ) == 0 )
		{
			int count = 0;
			// Jump past the null
	        while( lineStr[count++] )
	        {}

			// Get the material index
        	maternialIndex = atoi( &lineStr[count] );

			// Set the starting state
            state = EPS_NO_STATE;
		}
        else if( strcmp( lineStr, "texture:" ) == 0 )
        {
            if( !AddMateralNameToGroup( lineStr, strCount ) )
                goto HANDLE_ERROR;

            // Set the starting state
            state = EPS_NO_STATE;
        }
        else if( state == EPS_NO_STATE )
        {
            // Check for collision types
            for( int i = 0; i < ECT_MAX_TYPES; ++i )
            {
                if( strcmp( lineStr, colType.typeStr[i].c_str() ) == 0 )
                {
                    state = EPS_COLLISION_TYPE;
                    collisionType = i;

                    // Burn a line because it is not needed
                    GetLine( hFile, lineStr );

                    break;
                }
            }
        }
        
        else if( state == EPS_CENTER )
        {
            Decode_CenterMatrix( lineStr, strCount );

            // Set the starting state
            if( matIndex >= 9 )
                state = EPS_NO_STATE;
        }
        else if( state == EPS_TRANSFORM )
        {
            Decode_TransMatrix( lineStr, strCount );

            // Set the starting state
            if( matIndex >= 16 )
                state = EPS_NO_STATE;
        }
        else if( state == EPS_VERTEX )
        {
            if( !Decode_Vertice( lineStr, strCount ) )
                goto HANDLE_ERROR;
        }
        else if( state == EPS_UV )
        {
            if( !Decode_UV( lineStr, strCount ) )
                goto HANDLE_ERROR;
        }
        else if( state == EPS_FACE )
        {
            if( !Decode_Face( lineStr, strCount ) )
                goto HANDLE_ERROR;

            // Set the starting state
            state = EPS_NO_STATE;
        }
        else if( state == EPS_COLLISION_TYPE )
        {
            Decode_CollisionType( lineStr, strCount );
        }
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );

    return result;

}	// LoadFromFile


/************************************************************************
*    DESCRIPTION:  Build the map
*
*    Output:   bool - true on success, false on error
************************************************************************/   
bool CTrueSpaceParser::BuildMap( const char *filePath )
{
	bool result = false;

    char fileName[400];

    // We need a local copy of the file path because
    // we will be changing it.
    strcpy( fileName, filePath );

    ///////////////////////////////////////////////////
    // At this point, all the vertex data has been parsed 
    // out. Now what we need to do is to remove all the 
    // duplicate vertices from the vertice list. Then
    // go through each group and find the index for each
    // vertice within the vertice list.
    //
    // Last but not least, we build the texture library
    // if one is to be built. We have to parse the 
    // texture library file
    ///////////////////////////////////////////////////

    ///////////////////////////////////////////////////
    //  Build the map file which is a collection of
    //  room, doors and objects
    ///////////////////////////////////////////////////

    // Remove the duplicate vertices from the
    // groups and the global list
    if( !RemoveDupVerticies() )
        goto HANDLE_ERROR;
    
    // Match the local faces to the local verts
    if( !MatchVertsToLocalList() )
        goto HANDLE_ERROR;
    
    // Set the wall and floor collision type
    SetWallFloorCollisionType();

    // Calculaye the bounding sphere
	CalBoundingSphere();

    // Match the doors to the rooms
    if( !MatchDoorsToRooms() )
		goto HANDLE_ERROR;

    // Match the objects to rooms
    MatchObjectsToRooms();

    // Create global texture list
    CreateTextureList();

    // Do we have a texture library file to parse?
    if( textStrLst.size() > 0 )
    {
        // Match up the texture library indexes to the faces
        if( !MatchTextureIndexToFace() )
            goto HANDLE_ERROR;

        if( !BuildTextureLibrary( fileName ) )
            goto HANDLE_ERROR;

        // Sort the group faces by texture index
        for( uint i = 0; i < groupLst.size(); ++i )
        {
            SortByTextIndex( groupLst[ i ]->faceLst );
        }
    }

    // Save the polygon seperated object file
    if( !SaveToFile_MAP( fileName ) )
        goto HANDLE_ERROR;

    // Generate normals
    if( !GenerateNormals_MAP( fileName ) )
        goto HANDLE_ERROR;


    ///////////////////////////////////////////////////
    //  Build the standard ply file.
    ///////////////////////////////////////////////////

#ifdef GENERATE_PLY_FILE

    // Make a list of global verticies
    if( !MakeGlobalVertLst() )
        goto HANDLE_ERROR;

    // Make a list of global faces
    if( !MakeGlobalFaceLst() )
        goto HANDLE_ERROR;
    
    // Remove the duplicate vertices from the vertex list
    if( !RemoveDupVerticies() )
        goto HANDLE_ERROR;
        
    // Match the global faces to the global verticies
    if( !MatchVertsToGlobalList() )
        goto HANDLE_ERROR;

    // Save the polygon condenced object file
    if( !SaveToFile_PLY( fileName ) )
        goto HANDLE_ERROR;

    // Generate normals
    if( !GenerateNormals_PLY( fileName ) )
        goto HANDLE_ERROR;

#endif

    ///////////////////////////////////////////////////
    //  Generate the final map file
    ///////////////////////////////////////////////////

    // Build a map file to show all that we have done
    if( !BuildMapFile( fileName ) )
        goto HANDLE_ERROR;


    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;
    
    return result;

}   // LoadFromFile


/************************************************************************
*    DESCRIPTION:  Calculate the bounding sphere for each oject
************************************************************************/
void CTrueSpaceParser::CalBoundingSphere()
{
	for( uint grp = 0; grp < groupLst.size(); ++grp )
	{
		groupLst[grp]->CalBoundingSphere();
	}

}	// CalBoundingSphere


/************************************************************************
*    DESCRIPTION:  Set the collision types
************************************************************************/
void CTrueSpaceParser::SetWallFloorCollisionType()
{
	PCGroup pGroup;
	PCFace pFace;
	PCFace pFaceFloor;
	PCFace pFaceWall;

	for( uint grp = 0; grp < groupLst.size(); ++grp )
	{
		pGroup = groupLst[grp];

		// Only set collision if this object is one of these types
	    if( pGroup->objectType == EOT_MAP_ROOM ||
	        pGroup->objectType == EOT_MAP_HALLWAY ||
	        pGroup->objectType == EOT_MAP_STAIRWAY ||
	        pGroup->objectType == EOT_MAP_DOORWAY ||
	        pGroup->objectType == EOT_MAP_HALLDOORWAY || 
	        pGroup->objectType == EOT_MAP_COLLISION ||
	        pGroup->objectType == EOT_MAP_OBJ_COL )
	    {
	        bool wallFloorColSet = false;

	        // Check to see if a wall or floor has allready been set
	        for( uint i = 0; i < pGroup->faceLst.size(); ++i )
	        {
	            pFace = PCFace( pGroup->faceLst[i] );

	            if( pFace->collisionType == ECT_MAP_FLOOR ||
	                pFace->collisionType == ECT_MAP_WALL )
	            {
	                wallFloorColSet = true;
	                break;
	            }
	        }

	        // Only do the wall/floor collision if it has not already been set.
	        if( wallFloorColSet == false )
	        {
				// first set all the floors
			    for( uint i = 0; i < pGroup->faceLst.size(); ++i )
			    {        
			        pFaceFloor = PCFace( pGroup->faceLst[i] );

			        if( pFaceFloor->collisionType == ECT_NO_TYPE )
			        {
			            if( pFaceFloor->normal.y >= -1.0F && pFaceFloor->normal.y < -0.1 )
			            {
			                pFaceFloor->collisionType = ECT_MAP_FLOOR;
			            }
			        }
			    }

				// Now that the floors are tagged, tag only the walls the touch the floors
			    for( uint i = 0; i < pGroup->faceLst.size(); ++i )
			    {        
			        pFaceWall = PCFace( pGroup->faceLst[i] );

			        if( pFaceWall->collisionType == ECT_NO_TYPE )
			        {
			            if( fabs( pFaceWall->normal.y ) < 0.10f )
			            {
			                for( uint j = 0; j < pGroup->faceLst.size(); ++j )
			                {
			                	pFaceFloor = PCFace( pGroup->faceLst[j] );

			                	if( pFaceFloor->collisionType == ECT_MAP_FLOOR )
								{
			                		// See if this floor collides with this wall
			                		if( DoFacesCollide( pFaceFloor, pFaceWall ) )
									{
			                			pFaceWall->collisionType = ECT_MAP_WALL;
									}
								}
							}
			            }
			        }
			    }
	        }
	    }
	}
}	// SetWallFloorCollisionType


/************************************************************************
*    DESCRIPTION:  Match up all the doors with ajoining rooms.
************************************************************************/   
bool CTrueSpaceParser::MatchDoorsToRooms()
{   
	bool result = false;

    for( uint i = 0; i < groupLst.size(); ++i )
	{
		PCGroup pDoorGroup = groupLst[i];

		// If this is a doorway, search for the two areas it connects to.
        if( pDoorGroup->objectType == EOT_MAP_DOORWAY ||
            pDoorGroup->objectType == EOT_MAP_HALLDOORWAY )
        {
			CGroupLst tmpGroupLst;
			PCFace pDoorFace;

            int count = 0;

            // Find the floor face of the doorway
            for( uint face = 0; face < pDoorGroup->faceLst.size() && (count < 2); ++face )
            {
                // Get one of the faces of the door. One face will touch both rooms
                pDoorFace = PCFace( pDoorGroup->faceLst[face] );

				// Only use the floor polygons for the check
				if( pDoorFace->normal.y >= -1.0F && pDoorFace->normal.y < -0.1 )
				{
					break;
				}
			}

			// Do a bounding sphere collision test to determine what rooms might
			// be connected to this door and save it to the temp list
			for( uint j = 0; j < groupLst.size(); ++j )
			{
				PCGroup pRoomGroup = groupLst[j];

				// Make sure it's not a doorway
				if( ((pRoomGroup->objectType == EOT_MAP_ROOM) ||
                     (pRoomGroup->objectType == EOT_MAP_HALLWAY)) &&
				    // Make sure it's on the same floor
				    (pDoorGroup->mapData[EMD_MAP_FLOOR] == pRoomGroup->mapData[EMD_MAP_FLOOR]) )
				{
					// If we collide, add it to out temp list
					if( pDoorGroup->Collide_BoundSphere( pRoomGroup ) )
					{
						tmpGroupLst.push_back( pRoomGroup );
					}
				}
				else if( (pRoomGroup->objectType == EOT_MAP_STAIRWAY) &&
				         (abs(pDoorGroup->mapData[EMD_MAP_FLOOR] - pRoomGroup->mapData[EMD_MAP_FLOOR]) < 2 ) )
				{
					// If we collide, add it to out temp list
					if( pDoorGroup->Collide_BoundSphere( pRoomGroup ) )
					{
						tmpGroupLst.push_back( pRoomGroup );
					}
				}
			}
			
			// only check the tmp group for collisions
            for( uint grp = 0; grp < tmpGroupLst.size() && (count < 2); ++grp )
            {
                PCGroup pRoomGroup = tmpGroupLst[grp];
                 
                CFaceLst &tmpFaceLst = pRoomGroup->faceLst;

                // Go through all the faces and see if we have a collision
                for( uint w = 0; w < tmpFaceLst.size(); ++w )
                {
                    PCFace pRoomFace = PCFace( tmpFaceLst[ w ] );

                    // Only use the floor polygons for the check
                    if( pRoomFace->normal.y >= -1.0F && pRoomFace->normal.y < -0.1 )
					{
	                    if( DoFacesCollide( pRoomFace, pDoorFace ) )
	                    {
	                        if( pDoorGroup->mapData[ EMD_ROOM_1 ] != pRoomGroup->objectID )
	                        {
								// record the index to the room
	                            pDoorGroup->mapData[ count++ ] = (EMD_MAP_DATA)pRoomGroup->objectID;

	                            break;
	                        }
	                    }
					}
                }
            }

			// make sure we found rooms to this door
			if( pDoorGroup->mapData[ EMD_ROOM_1 ] < 0 || pDoorGroup->mapData[ EMD_ROOM_2 ] < 0 )
			{
				errorCode = ERROR_DOOR_NOT_ATTACHED_TO_ROOM;
                goto HANDLE_ERROR;
			}
		}
	}

	// If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

	return result;

}   // MatchDoorsToRooms


/************************************************************************
*    DESCRIPTION:  Match up all the objects with rooms.
************************************************************************/   
void CTrueSpaceParser::MatchObjectsToRooms()
{
    for( uint obj = 0; obj < groupLst.size(); ++obj )
    {
        PCGroup pObjectGroup = groupLst[obj];
		PCGroup pRoomGroup;

        // If this is an object, search for the two rooms it connects to.
        if( pObjectGroup->objectType == EOT_NO_TYPE ||
            pObjectGroup->objectType == EOT_MAP_OBJECT ||
            pObjectGroup->objectType == EOT_MAP_COLLISION ||
            pObjectGroup->objectType == EOT_MAP_PLAYER_SPAWN_POINT ||
            pObjectGroup->objectType == EOT_MAP_BOT_SPAWN_POINT )
        {
			CGroupLst tmpGroupLst;

			// Do a bounding sphere collision test to determine what rooms
			// have this object in it
			for( uint j = 0; j < groupLst.size(); ++j )
			{
				pRoomGroup = groupLst[j];

				    // Make sure it's not a doorway
				if( ((pRoomGroup->objectType == EOT_MAP_ROOM) ||
                     (pRoomGroup->objectType == EOT_MAP_HALLWAY) ||
                     (pRoomGroup->objectType == EOT_MAP_STAIRWAY)) &&
				    // Make sure it's on the same floor
				    (pObjectGroup->mapData[EMD_MAP_FLOOR] == pRoomGroup->mapData[EMD_MAP_FLOOR]) )
				{
					// If we collide, add it to out temp list
					if( pObjectGroup->Collide_BoundSphere( pRoomGroup ) )
					{
						tmpGroupLst.push_back( pRoomGroup );
					}
				}
			}

			int totalCount = 0;
			int count;

            // Find the rooms and see if the object collides
            for( uint room = 0; room < tmpGroupLst.size(); ++room )
            {
            	pRoomGroup = tmpGroupLst[room];
            	count = 0;

                // Go through all the faces and see and count up all the collisions
                for( uint i = 0; i < pRoomGroup->faceLst.size(); ++i )
                {
                    for( uint j = 0; j < pObjectGroup->faceLst.size(); ++j )
                    {
                        if( DoFacesCollide( PCFace( pRoomGroup->faceLst[i] ), 
                                            PCFace( pObjectGroup->faceLst[j] ) ) )
                        {                           
                            ++count;
                        }
                    }
                }

                // Based on which room gets the most collision hits determins
                // the room the object is in.
                if( count > totalCount )
                {
                    pObjectGroup->mapData[EMD_ROOM_1] = (EMD_MAP_DATA)pRoomGroup->objectID;
                    totalCount = count;
                }
            }

			// Add the no type to the room group
			if( pObjectGroup->objectType == EOT_NO_TYPE )
			{
				CombineNoTypeObjectsToRoomMesh( pObjectGroup, pRoomGroup );
			}
        }
    }
}   // MatchObjectsToRooms


/************************************************************************
*    DESCRIPTION:  Objects of no specific type are to be added to the
*                  room mesh they are associated with.
*
*    Input:    PCGroup pObjectGroup - object to be added to a group
*              PCGroup pRoomGroup - room to add object data to
*
*    Output:   bool - true on success, false on error
************************************************************************/   
bool CTrueSpaceParser::CombineNoTypeObjectsToRoomMesh( PCGroup pObjectGroup,
                                                       PCGroup pRoomGroup )
{
    bool result = false;

    // Get the number of verts in this object
    int vCount = pRoomGroup->vertLst.size();

    // Add the objects verts to the rooms vert list
    for( uint i = 0; i < pObjectGroup->vertLst.size(); ++i )
    {
        // Allocate a vertice class
        PCVertice pVert = new CVertLoad;

        // check for allocations for errors
        if( pVert == NULL )
        {
            errorCode = ERROR_ALLOCATION_FAIL;
            goto HANDLE_ERROR;
        }

        pVert->vert[0] = pObjectGroup->vertLst[i]->vert[0];
        pVert->vert[1] = pObjectGroup->vertLst[i]->vert[1];
        pVert->vert[2] = pObjectGroup->vertLst[i]->vert[2];

        // Add the verts to the room verts
        pRoomGroup->vertLst.push_back( pVert );
    }

    // Get the number of textures in this object
    int tCount = pRoomGroup->textStrMap.size();

    // Add the objects face to the rooms face list
    for( uint i = 0; i < pObjectGroup->faceLst.size(); ++i )
    {
        // Allocate a face class
        PCFace pFace = new CFace;

        // check for allocations for errors
        if( pFace == NULL )
        {
            errorCode = ERROR_ALLOCATION_FAIL;
            goto HANDLE_ERROR;
        }

        // Save the vert count for this polygon
        pFace->vertCount = pObjectGroup->faceLst[i]->vertCount;

        // Get the collision type
        pFace->collisionType = pObjectGroup->faceLst[i]->collisionType;

        // The index to the textures is now at the end of the room list so take
        // the texture count of the room and add it to the objets texture index
        pFace->tIndex = pObjectGroup->faceLst[i]->tIndex + tCount;

        // copy over the face data
        for( int j = 0; j < pFace->vertCount; ++j )
        {
            // The index to the verts is now at the end of the room list so take
            // the vert count of the room and add it to the objects vert index
            pFace->vIndex[j] = pObjectGroup->faceLst[i]->vIndex[j] + vCount;

            // Copy over the uv
            pFace->crd[j] = pObjectGroup->faceLst[i]->crd[j];

            // Copy over the verts
            pFace->point[j] = pObjectGroup->faceLst[i]->point[j];
        }

        // Add the face to the room faces
        pRoomGroup->faceLst.push_back( pFace );
    }

    // Add the objects textures to the room's texture list
	for( map<int, std::string>::iterator iter = pObjectGroup->textStrMap.begin();
	     iter != pObjectGroup->textStrMap.end();
	     ++iter )
	{
		pRoomGroup->textStrMap.insert( make_pair(iter->first + tCount, iter->second) ); 
	}

HANDLE_ERROR:;

    return result;

}  // CombineNoTypeObjectsToRoomMesh


/************************************************************************
*    DESCRIPTION:  Check to see if faces collide
*
*    Input:    PCFace pFace1 - First face to chesk
*              PCFace pFace2 - Second face to check
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/22/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CTrueSpaceParser::DoFacesCollide( PCFace pFace1, PCFace pFace2 )
{
    double dist;

    // Calculate the distance between the center points of both objects
    dist = sqrt( (( pFace1->offsetPt.x - pFace2->offsetPt.x ) * ( pFace1->offsetPt.x - pFace2->offsetPt.x )) +
                 (( pFace1->offsetPt.y - pFace2->offsetPt.y ) * ( pFace1->offsetPt.y - pFace2->offsetPt.y )) +
                 (( pFace1->offsetPt.z - pFace2->offsetPt.z ) * ( pFace1->offsetPt.z - pFace2->offsetPt.z )) );

    // Check for possible collision by using bounding sphere
    if( dist <= pFace1->radius + pFace2->radius )
        return true;
    else
        return false;

}   // DoFacesCollide


/************************************************************************
*    DESCRIPTION:  Decode the collision type
*
*    Input:    char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
************************************************************************/  
bool CTrueSpaceParser::Decode_CollisionType( char *strData, int strCount )
{
    bool result = false;
    uint count = 0, faceIndex;
    PCFace pFace;

    // We are supposed to have at least one group in the list.
    // Make sure that we do
    if( groupLst.size() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // There has to be a null first to qualify
    if( strData[0] == 0 )
    {
        CFaceLst &tmpFaceLst = groupLst[ groupLst.size()-1 ]->faceLst;

        // Jump past the null
        while( strData[count++] )
        {}

        do
        {
            // Get the index to the face list
            faceIndex = atoi( &strData[count] );

            // Check that we are not out of range
            if( faceIndex < 0 || faceIndex >= tmpFaceLst.size() )
            {
                errorCode = ERROR_DATA_OUT_OF_RANGE;
                goto HANDLE_ERROR;
            }

            // Get the face
            pFace = tmpFaceLst[ faceIndex ];

            // save the collision type
            pFace->collisionType = collisionType;

            // Jump past the null
            while( strData[count++] )
            {}
        }
        while( strData[count] );
    }
    else
    {
        // Set the starting state
        state = EPS_NO_STATE;
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // Decode_CollisionType


/************************************************************************
*    DESCRIPTION:  Create a global texture list
************************************************************************/  
void CTrueSpaceParser::CreateTextureList()
{
    PCGroup pGroup;
	std::string grpPathStr, txtPathStr;
    CTextStrLst alphaTextStrLst;
    bool found;

    for( uint i = 0; i < groupLst.size(); ++i )
    {
        pGroup = groupLst[ i ];

        for( uint j = 0; j < pGroup->textStrMap.size(); ++j )
        {
            found = false;

            grpPathStr = pGroup->textStrMap[ j ];

            // Make sure we don't add duplicates to the list
            for( uint w = 0; w < textStrLst.size() && !found; ++w )
            {
                txtPathStr = textStrLst[ w ];

                if( grpPathStr == txtPathStr )
                {
                    found = true;
                }
            }

            // Make sure we don't add duplicates to the list
            for( uint w = 0; w < alphaTextStrLst.size() && !found; ++w )
            {
                txtPathStr = alphaTextStrLst[ w ];

                if( grpPathStr == txtPathStr )
                {
                    found = true;
                }
            }

            bool tgaFileFound = (GetFileType( (char *)grpPathStr.c_str() ) == FILE_TYPE_TGA);

            // Flag this object because it has alpha blended textures
            if( tgaFileFound && pGroup->objectType == EOT_MAP_OBJECT )
            {
                pGroup->mapData[EMD_ROOM_2] = (EMD_MAP_DATA)OBJECT_WITH_ALPHA_TXT;
            }

            // Add the texture to the global list
            if( found == false )
            {
                // TGA files are assumed to be alpha blended files.
                // added these to a different list so that it can
                // be added to the end
                if( tgaFileFound )
                {
                    alphaTextStrLst.push_back( grpPathStr );
                }
                else
                {
                    textStrLst.push_back( grpPathStr );
                }
            }
        }
    }

    // Add all the alpha textures to the end of the global texture list
    for( uint i = 0; i < alphaTextStrLst.size(); ++i )
    {
        textStrLst.push_back( alphaTextStrLst[i] );
    }

}   // CreateTextureList


/************************************************************************
*    DESCRIPTION:  Add a new group to the list
*
*    Output:   bool - true on success, false on error
************************************************************************/   
bool CTrueSpaceParser::AddGroup()
{
    bool result = false;
    PCGroup pGroup;

    // Allocate a group class
    pGroup = new CGroup;

    // check for allocations for errors
    if( pGroup == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // Save the object type and id
    pGroup->objectType = objectType;

	// Save the object id
	// Force the room areas to count up as one group that way
	// the id will match the index into the room list
	if( objectType == EOT_MAP_ROOM || 
        objectType == EOT_MAP_HALLWAY ||
        objectType == EOT_MAP_STAIRWAY )
	{
		pGroup->objectID = typeCounter[EOT_MAP_ROOM]++;
	}
	// Force the doorways to count up as one group that way
	// the id will match the index into the doorway list
	else if( objectType == EOT_MAP_DOORWAY || 
             objectType == EOT_MAP_HALLDOORWAY )
	{
		pGroup->objectID = typeCounter[EOT_MAP_DOORWAY]++;
	}
	else
	{
		pGroup->objectID = typeCounter[objectType]++;
	}
	
	// This is the id given by the modler
	pGroup->mapData[EMD_MAP_ID] = (EMD_MAP_DATA)mapID;
	
	// Record the floor index
	pGroup->mapData[EMD_MAP_FLOOR] = (EMD_MAP_DATA)floorCounter;    

    // Add the object to our object list
    groupLst.push_back( pGroup );

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // cleanup on error
    if( !result )
    {
        if( pGroup != NULL )
            delete pGroup;
    }

    return result;

}   // AddGroup


/************************************************************************
*    DESCRIPTION:  Decode the matrix
*
*    Input:    char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    -------------------------------------------------------------------
*                  11/05/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
void CTrueSpaceParser::Decode_TransMatrix( char *strData, int strCount )
{
    int count = 0;

    for( int i = 0; i < 4; ++i )
    {
        transMat[matIndex++] = atof( &strData[count] );

        // Jump past the null
        while( strData[count++] )
        {}
    }

}   // Decode_TransMatrix


/************************************************************************
*    DESCRIPTION:  Decode the center matrix
*
*    Input:    char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    -------------------------------------------------------------------
*                  11/05/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
void CTrueSpaceParser::Decode_CenterMatrix( char *strData, int strCount )
{
    int count = 0;

    // Jump past the null
    while( strData[count++] )
    {}

    // Jump past the null
    while( strData[count++] )
    {}

    for( int i = 0; i < 3; ++i )
    {
        centerMat[matIndex++] = atof( &strData[count] );

        // Jump past the null
        while( strData[count++] )
        {}
    }

}   // Decode_CenterMatrix


/************************************************************************
*    DESCRIPTION:  Decode the object name
*
*    Input:    char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    -------------------------------------------------------------------
*                  11/05/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
void CTrueSpaceParser::Decode_ObjectName( char *strData, int strCount )
{
    int count = 0;
    SObjectType objTypeStruct;

    objectType = EOT_NO_TYPE;
	mapID = 0;

    // Jump past the null
    while( strData[count++] )
    {}

    if( strData[count] )
    {
        // Check for object types
        for( int i = 0; i < EOT_MAX_TYPES; ++i )
        {
            if( strcmp( &strData[count], objTypeStruct.typeStr[i].c_str() ) == 0 )
            {
                // Set the type
                objectType = (EOT_OBJECT_TYPE)i;

                break;
            }
        }
    }

	// Jump past the null
	while( strData[count++] )
	{}

	// Get the map id
	if( strData[count] )
	{
		mapID = atof( &strData[count] );
	}


}   // Decode_ObjectName


/************************************************************************
*    DESCRIPTION:  Decode the center
*
*    Input:    char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    -------------------------------------------------------------------
*                  11/05/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
void CTrueSpaceParser::Decode_Center( FILE *hFile, char *strData, int strCount )
{
    int count = 0;
    /*CPoint axis;
    float finialAxis[3];*/

    // Jump past the null
    while( strData[count++] )
    {}

    centerPoint.x = atof( &strData[count] );

    // Jump past the null
    while( strData[count++] )
    {}

    centerPoint.y = atof( &strData[count] );

    // Jump past the null
    while( strData[count++] )
    {}

    centerPoint.z = atof( &strData[count] );

    // get each of the three axises
    /*for( int i = 0; i < 3; ++i )
    {
        GetLine( hFile, strData );

        count = 0;
        // Jump past the null
        while( strData[count++] )
        {}
        while( strData[count++] )
        {}

        axis.x = atof( &strData[count] );

        while( strData[count++] )
        {}

        axis.y = atof( &strData[count] );

        while( strData[count++] )
        {}

        axis.z = atof( &strData[count] );

        finialAxis[i] = sqrt( (axis.x * axis.x) +
                              (axis.y * axis.y) +
                              (axis.z * axis.z) );
    }

    centerPoint.x += finialAxis[0];
    centerPoint.y += finialAxis[1];
    centerPoint.z += finialAxis[2];*/

}   // Decode_Center


/************************************************************************
*    DESCRIPTION:  Decode the string of vertices
*
*    Input:    char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
************************************************************************/  
bool CTrueSpaceParser::Decode_Vertice( char *strData, int strCount )
{
    bool result = false;
    int i, count = 0;
    PCVertice pVert;
    CMatrix matrix;
    CPoint point;

    // We are supposed to have at least one group in the list.
    // Make sure that we do
    if( groupLst.size() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Allocate a vertice class
    pVert = new CVertLoad;

    // check for allocations for errors
    if( pVert == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // Decode each vertice into the allocated class
    for( i = 0; i < 3; ++i )
    {
        // Do a sanity check to ensure we are not reading junk
        if( strData[count] < 45 || strData[count] > 57 || count > strCount )
        {
            errorCode = ERROR_DATA_OUT_OF_RANGE;
            goto HANDLE_ERROR;
        }

        // Decode the three vertices
        pVert->vert[i] = atof( &strData[count] );

        // Jump past the null
        while( strData[count++] )
        {}
    }

    // Don't want the rooms or doors to have any rotations... Just to be safe
    if( objectType == EOT_MAP_ROOM || 
        objectType == EOT_MAP_HALLWAY ||
        objectType == EOT_MAP_STAIRWAY ||
        objectType == EOT_MAP_DOORWAY ||
        objectType == EOT_MAP_HALLDOORWAY )
	{
		if( centerMat[0] != 1 || centerMat[1] != 0 || centerMat[2] != 0 ||
		    centerMat[3] != 0 || centerMat[4] != 1 || centerMat[5] != 0 ||
		    centerMat[6] != 0 || centerMat[7] != 0 || centerMat[8] != 1 )
		{
			errorCode = ERROR_OBJ_CENTER_AXIS_USED;
	        goto HANDLE_ERROR;
		}
	}

    // Copy the vertex to the point
    point.x = pVert->vert[0];
    point.y = pVert->vert[1];
    point.z = pVert->vert[2];

	// Set the center matrix and transform
	matrix.SetMatrix3to4( centerMat );
    matrix.Transform( point );

    point.MakePermanent();

	// Set the transform matrix and transform
	matrix.SetMatrix( transMat );
    matrix.Transform( point );

    point.MakePermanent();

    // Do the center matrix again and transform
    matrix.SetMatrix3to4( centerMat );

    // The z which is actually the y is off if it is a room or doorway
	if( objectType == EOT_MAP_ROOM || 
        objectType == EOT_MAP_HALLWAY ||
        objectType == EOT_MAP_STAIRWAY ||
        objectType == EOT_MAP_DOORWAY ||
        objectType == EOT_MAP_HALLDOORWAY )
	{
		// This matrix element needs to be added in for inverted objects
		matrix.Translate( centerPoint.x, centerPoint.y, centerPoint.z + transMat[11] );
	}
	else
	{
		matrix.Translate( centerPoint.x, centerPoint.y, centerPoint.z );
	}
	
	// Flip it around so that it's not upside down and backwards
	matrix.Rotate( 0, 180, 180 );
    matrix.Transform( point );    
    
    // Invert the values an switch the y and z
    pVert->vert[0] = -point.tx;
    pVert->vert[1] = -point.tz;
    pVert->vert[2] = -point.ty;

    // Add the object to our object list
    groupLst[ groupLst.size()-1 ]->vertLst.push_back( pVert );  

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // cleanup on error
    if( !result )
    {
        if( pVert != NULL )
        {
            delete pVert;
        }
    }

    return result;

}   // Decode_Vertice


/************************************************************************
*    DESCRIPTION:  Decode the string of UV values
*
*    Input:    char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
************************************************************************/  
bool CTrueSpaceParser::Decode_UV( char *strData, int strCount )
{
    bool result = false;
    int i, count = 0;
    PCUV pUV;

    // We are supposed to have at least one group in the list.
    // Make sure that we do
    if( groupLst.size() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Allocate a vertice class
    pUV = new CUVLoad;

    // check for allocations for errors
    if( pUV == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // Decode each UV into the allocated class
    for( i = 0; i < 2; ++i )
    {
        // Do a sanity check to ensure we are not reading junk
        if( strData[count] < 45 || strData[count] > 57 || count > strCount )
        {
            errorCode = ERROR_DATA_OUT_OF_RANGE;
            goto HANDLE_ERROR;
        }

        // Decode the three vertices
        pUV->uv[i] = atof( &strData[count] );

        // UV coordinates can't be a negative number or
        // greater then 1.0. This would cause us to try
        // to sample outside the scope of the texture.

		// Also the modeler does a lousy job of texture mapping a 
		// square so cap values that should be 0 oe 1
        if( pUV->uv[i] < 0.1f )
            pUV->uv[i] = 0.0f;
        else if( pUV->uv[i] > 0.9f )
            pUV->uv[i] = 1.0f;

        // Jump past the null
        while( strData[count++] )
        {}
    }

    // Add the object to our object list
    groupLst[ groupLst.size()-1 ]->uvLst.push_back( pUV );

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // cleanup on error
    if( !result )
    {
        if( pUV != NULL )
            delete pUV;
    }

    return result;

}   // Decode_UV


/************************************************************************
*    DESCRIPTION:  Add a new face to the group. Check the number
*                  of vertices to make sure it is in range.
*
*    Input:    char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
************************************************************************/   
bool CTrueSpaceParser::AddFaceToGroup( char *strData, int strCount )
{
    bool result = false;
    int vertCount, count = 0, i;
    PCFace pFace;

    // We are supposed to have at least one group in the list.
    // Make sure that we do
    if( groupLst.size() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Allocate a face class
    pFace = new CFace;

    // check for allocations for errors
    if( pFace == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // Jump over two nulls to get the vert count
    for( i = 0; i < 2; ++i )
    {
        while( strData[count++] )
        {}
    }

    // Get the vertex count for this face
    vertCount = atoi( &strData[count] );

    if( vertCount < TRI || vertCount > QUAD )
    {
        errorCode = ERROR_VERTEX_OUT_OF_RANGE;
        goto HANDLE_ERROR;
    }

    // Save the vert count for this polygon
    pFace->vertCount = (char)vertCount;

    // Jump over 4 nulls to get the texture index
    for( i = 0; i < 4; ++i )
    {
        while( strData[count++] )
        {}
    }

    // Get the texture index
    pFace->tIndex = (unsigned short int)atoi( &strData[count] );

    // Add the object to our group list
    groupLst[ groupLst.size()-1 ]->faceLst.push_back( pFace );  

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // cleanup on error
    if( !result )
    {
        if( pFace != NULL )
            delete pFace;
    }

    return result;

}   // AddFaceToGroup


/************************************************************************
*    DESCRIPTION:  Decode the string of face
*
*    Input:    char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
************************************************************************/   
bool CTrueSpaceParser::Decode_Face( char *strData, int strCount )
{
    bool result = false;
    int count = 0;
	uint index;
    PCGroup pGroup;
    PCFace pFace;
    PCVertice pVert;
    PCUV pUV;

    // We are supposed to have at least one group in the list.
    // Make sure that we do
    if( groupLst.size() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Get the face list
    pGroup = groupLst[ groupLst.size()-1 ];

    // Get the current face
    pFace = pGroup->faceLst[ pGroup->faceLst.size()-1 ];  

    // Decode each vertice & UV and set the face
    for( int i = 0; i < pFace->vertCount; ++i )
    {
        // Do a sanity check to ensure we are not reading junk
        if( strData[count] < 48 || strData[count] > 57 || count > strCount )
        {
            errorCode = ERROR_DATA_OUT_OF_RANGE;
            goto HANDLE_ERROR;
        }
        
        // Get the index to the vertex list
        index = atoi( &strData[count] );
        
        // Do a sanity check to ensure we are not reading junk
        if( index < 0 || index >= pGroup->vertLst.size() )
        {
            errorCode = ERROR_DATA_OUT_OF_RANGE;
            goto HANDLE_ERROR;
        }

        // Get the pointer to the vert class
        pVert = pGroup->vertLst[ index ];

        // Copy over the verts
        for( uint j = 0; j < 3; ++j )
        {
            pFace->point[i].vert[j] = pVert->vert[j];
        }

        // Jump past the null
        while( strData[count++] )
        {}

        // Do a sanity check to ensure we are not reading junk
        if( strData[count] < 48 || strData[count] > 57 || count > strCount )
        {
            errorCode = ERROR_DATA_OUT_OF_RANGE;
            goto HANDLE_ERROR;
        }

        // Get the index to the uv list
        index = atoi( &strData[count] );
        
        // Do a sanity check to ensure we are not reading junk
        if( index < 0 || index >= pGroup->uvLst.size() )
        {
            errorCode = ERROR_DATA_OUT_OF_RANGE;
            goto HANDLE_ERROR;
        }

        // Get the pointer to the uv class
        pUV = pGroup->uvLst[ index ];

        // Copy over the uv's
        for( uint j = 0; j < 2; ++j )
        {
            pFace->crd[i].uv[j] = pUV->uv[j];
        }

        // Jump past the null
        while( strData[count++] )
        {}
    }

    // calculate the radius of the face
    pFace->Radius();

    // Calculate the normal
    pFace->CalcSurfaceNormal();

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // cleanup on error
    if( !result )
    {
        if( pVert != NULL )
            delete pVert;
    }

    return result;

}   // Decode_Face


/************************************************************************
*    DESCRIPTION:  Add the material name to the group
*
*    Input:    char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
************************************************************************/   
bool CTrueSpaceParser::AddMateralNameToGroup( char *strData, int strCount )
{
    bool result = false, found = false;
    int count = 0, i;
    string pathStr;

    // We are supposed to have at least one group in the list.
    // Make sure that we do
    if( groupLst.size() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Jump past the null
    while( strData[count++] )
    {}

    // Find the ":"
    for( i = count; i < strCount && !found; ++i )
    {
        if( strData[i] == ':' )
        {
            count = i - 1;
            found = true;
        }
    }

    // Do a sanity check to ensure we are not reading junk
    if( found == false )
    {
        errorCode = ERROR_DATA_OUT_OF_RANGE;
        goto HANDLE_ERROR;
    }

    // Put the spaces back into the string
    RelpaceChar( strData, 0, ' ', strCount - 2 );

    // Add the string to the list
    groupLst[ groupLst.size()-1 ]->textStrMap.insert( make_pair(maternialIndex,std::string(&strData[count])) );    

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // AddMateralNameToGroup


/************************************************************************
*    DESCRIPTION:  Make a global vert list
*
*    Input:    CVertLst &vertLst  - Vertex list
*
*    Output:   bool - true on success, false on error
************************************************************************/   
bool CTrueSpaceParser::MakeGlobalVertLst()
{
    bool result = false;
    PCVertice pVertTmp;

    // Make the global vertex list
    for( uint i = 0; i < groupLst.size(); ++i )
    {
        for( uint j = 0; j < groupLst[ i ]->vertLst.size(); ++j )
        {
            // Allocate a vertice class
            pVertTmp = new CVertLoad;

            // check for allocations for errors
            if( pVertTmp == NULL )
            {
                errorCode = ERROR_ALLOCATION_FAIL;
                goto HANDLE_ERROR;
            }
            
            memcpy( pVertTmp, groupLst[ i ]->vertLst[ j ], sizeof(CVertLoad) );
            
            vertLst.push_back( pVertTmp );
        }
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // MakeGlobalVertLst


/************************************************************************
*    DESCRIPTION:           Make a global face list
*
*    Input:    CFaceLst &faceLst - Face list
*
*    Output:   bool - true on success, false on error
************************************************************************/   
bool CTrueSpaceParser::MakeGlobalFaceLst()
{
    bool result = false;
    PCFace pFaceTmp;

    // Make the global vertex list
    for( uint i = 0; i < groupLst.size(); ++i )
    {
        CFaceLst &tmpFaceLst = groupLst[ i ]->faceLst;

        for( uint j = 0; j < tmpFaceLst.size(); ++j )
        {
            // Allocate a vertice class
            pFaceTmp = new CFace;

            // check for allocations for errors
            if( pFaceTmp == NULL )
            {
                errorCode = ERROR_ALLOCATION_FAIL;
                goto HANDLE_ERROR;
            }

            memcpy( pFaceTmp, tmpFaceLst[j], sizeof(CFace) );
            
            faceLst.push_back( pFaceTmp );
        }
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // MakeGlobalFaceLst


/************************************************************************
*    DESCRIPTION:  Remove all the duplicate verticies from the vertice list
*
*    Output:   bool - true on success, false on error
************************************************************************/  
bool CTrueSpaceParser::RemoveDupVerticies()
{
    bool result = false;
    uint i, j, obj;
    PCVertice pVert1, pVert2;

    // We have to have a list
    if( groupLst.size() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Remove the duplicates in each group
    for( obj = 0; obj < groupLst.size(); ++obj )
    {
        // Cycle through the list and delete all the duplicates
        for( i = 0; i < groupLst[ obj ]->vertLst.size(); ++i )
        {        
            // Get the vertex to check for duplicates
            pVert1 = (PCVertice)groupLst[ obj ]->vertLst[ i ];

            for( j = i+1; j < groupLst[ obj ]->vertLst.size(); ++j )
            {            
                // Get the vertex to check against
                pVert2 = (PCVertice)groupLst[ obj ]->vertLst[ j ];

                // Do we have a match?
                if( pVert1->vert[0] == pVert2->vert[0] &&
                    pVert1->vert[1] == pVert2->vert[1] &&
                    pVert1->vert[2] == pVert2->vert[2] )
                {
                    // Delete the vertice at this index
					Delete( groupLst[ obj ]->vertLst[j] );

                    groupLst[ obj ]->vertLst.erase( groupLst[ obj ]->vertLst.begin() + j );

                    // Step it back one because now
                    // it will skip over the next one
                    // that we want to test
                    --j;
                }
            }
        }
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // RemoveDupVerticies


/************************************************************************
*    DESCRIPTION:  Remove all the duplicate verticies from the vertice list
*
*    Input:    CVertLst &vertLst - Global vertex list
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/  
bool CTrueSpaceParser::RemoveDupVerticies( CVertLst &vertLst )
{
    bool result = false;
    uint i, j;
    PCVertice pVert1, pVert2;

    if( vertLst.size() == 0 )
    {
        errorCode = ERROR_DATA_OUT_OF_RANGE;
        goto HANDLE_ERROR;
    }

    // Cycle through the global list and delete all the duplicates
    for( i = 0; i < vertLst.size(); ++i )
    {        
        // Get the vertex to check for duplicates
        pVert1 = vertLst[ i ];

        for( j = i+1; j < vertLst.size(); ++j )
        {            
            // Get the vertex to check against
            pVert2 = vertLst[ j ];

            // Do we have a match?
            if( pVert1->vert[0] == pVert2->vert[0] &&
                pVert1->vert[1] == pVert2->vert[1] &&
                pVert1->vert[2] == pVert2->vert[2] )
            {
                // Delete the vertice at this index
				Delete( vertLst[j] );
                vertLst.erase( vertLst.begin() + j );

                // Step it back one because now
                // it will skip over the next one
                // that we want to test
                --j;
            }
        }
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // RemoveDupVerticies


/************************************************************************
*    DESCRIPTION:  Read in the poly file and generate all the normals
*
*    Input:    char *fileName - Path to load file
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CTrueSpaceParser::GenerateNormals_PLY( char *fileName )
{
    CMesh tmpMsh;
    bool result = false;

    // Change the extension to the original file path
    RelpaceFileExtension( NULL, fileName, ".ply" );

    // Load in the new polygon file
    if( !tmpMsh.LoadMeshFromFile( fileName ) )
    {
        errorCode = ERROR_CAL_NORMALS_FILE;
        goto HANDLE_ERROR;
    }

    // scale the map
	tmpMsh.Scale(50,50,50);

    // generate all the normals
	// use a range to allow for when the modeler is slightly off
	tmpMsh.CalNormals();
	tmpMsh.CalcVertexNormals( VERT_NORM_RANGE );

    // Change the extension to the original file path
    RelpaceFileExtension( NULL, fileName, ".ply" );

    // Save in the new polygon file with the 
    // mesh and normals in them
    if( !tmpMsh.SaveToFile( fileName ) )
    {
        errorCode = ERROR_SAVE_NORMALS_FILE;
        goto HANDLE_ERROR;
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;
    
    return result;

}   // GenerateNormals_PLY


/************************************************************************
*    DESCRIPTION:  Read in the map file and generate all the normals
*
*    Input:    char *fileName - Path to load file
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CTrueSpaceParser::GenerateNormals_MAP( char *fileName )
{
    CMeshList<CMeshMap> tmpMshLst;
    bool result = false;

    // Change the extension to the original file path
    RelpaceFileExtension( NULL, fileName, ".map" );

    // Load in the new polygon file to generate all the normals
    if( !tmpMshLst.LoadMeshFromFile( fileName ) )
    {
        errorCode = ERROR_CAL_NORMALS_FILE;
        goto HANDLE_ERROR;
    }

    // scale the map
	tmpMshLst.Scale(50,50,50);

    // Quick flat shading for map testing
	tmpMshLst.CalNormals();
	//tmpMshLst.CalcVertexNormals( 0.2f );

    for( int msh = 0; msh < tmpMshLst.Count(); ++msh )
	{
		std::vector<CMeshMap *> meshLst;

		// Record the mesh in question
		CMeshMap * mesh = &tmpMshLst.GetMesh()[msh];

        // Add our mesh to the mesh list
		meshLst.push_back( mesh );

        // If this is a room or doorway mesh, find whats connects to it
        if( mesh->GetType() < EOT_MAP_OBJECT )
		{
			for( int i = 0; i < tmpMshLst.Count(); ++i )
			{
				CMeshMap * tmpMesh = &tmpMshLst.GetMesh()[i];

				if( mesh->GetType() == EOT_MAP_ROOM ||
	        		mesh->GetType() == EOT_MAP_HALLWAY ||
	        		mesh->GetType() == EOT_MAP_STAIRWAY )
				{
					if( tmpMesh->GetType() == EOT_MAP_DOORWAY ||
	        	        tmpMesh->GetType() == EOT_MAP_HALLDOORWAY )
					{
						if( (abs(mesh->GetFloorID() - tmpMesh->GetFloorID()) < 2) &&
						    ((mesh->GetID() == tmpMesh->GetRoomIndex( EMD_ROOM_1 )) ||
						     (mesh->GetID() == tmpMesh->GetRoomIndex( EMD_ROOM_2 )) ) )
						{
							meshLst.push_back( tmpMesh );
						}			
					}
				}
				else if( mesh->GetType() == EOT_MAP_DOORWAY ||
	        	         mesh->GetType() == EOT_MAP_HALLDOORWAY )
				{
					if( tmpMesh->GetType() == EOT_MAP_ROOM ||
		        		tmpMesh->GetType() == EOT_MAP_HALLWAY ||
		        		tmpMesh->GetType() == EOT_MAP_STAIRWAY )
					{
						// Doors connected to rooms are always on the same floor
						if( (mesh->GetFloorID() == tmpMesh->GetFloorID()) &&
						    ((tmpMesh->GetID() == mesh->GetRoomIndex( EMD_ROOM_1 )) ||
						     (tmpMesh->GetID() == mesh->GetRoomIndex( EMD_ROOM_2 )) ) )
						{
							meshLst.push_back( tmpMesh );
						}
					}
				}
			}

			// There always need to be something added
			if( meshLst.empty() )
		    {
		        errorCode = ERROR_SAVE_NORMALS_FILE;
		        goto HANDLE_ERROR;
		    }

			// Addup all the normals per vertex for all polygons
		    for( int pIndex = 0; pIndex < mesh->GetPolyCount(); ++pIndex )
		    {
				// Find shared vertices, add them up and average per vertex
			    for( int vIndex = 0; vIndex < mesh->GetPolyList()[pIndex].GetVertexCount(); ++vIndex )
			    {
					std::vector<CNormAdd> sharedNormLst;

					// Compair this vert to all the verts in the mesh list
					for( unsigned int lst = 0; lst < meshLst.size(); ++lst )
		    		{           
						CMeshMap * tmpMesh = meshLst[lst];

						// Collect the shared normals
						tmpMesh->FindSharedNorms( VERT_NORM_RANGE, mesh->GetPolyList()[pIndex].GetVPoint( vIndex ), sharedNormLst );
					}

			        // Add the shared normals to create vertex normal
					mesh->AverageSharedNorms( pIndex, vIndex, sharedNormLst );
			    }
		    }
		}
	}

    // Change the extension to the original file path
    RelpaceFileExtension( NULL, fileName, ".map" );

    // Save in the new polygon file with the 
    // mesh and normals in them
    if( !tmpMshLst.SaveToFile( fileName ) )
    {
        errorCode = ERROR_SAVE_NORMALS_FILE;
        goto HANDLE_ERROR;
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;
    
    return result;

}   // GenerateNormals_MAP


/************************************************************************
*    DESCRIPTION:  Reads a line of text from a text file
*
*    Input:    FILE *hFile - File handle
*              char *Str - text read up to the new-line character
*
*    Output:   int - Number of characters read in
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
int CTrueSpaceParser::GetLine( FILE *hFile, char *str )
{
    // Reads a line of text from a text file
    char nextChar;
    int index = 0;

    // Reads a line of text from a text file
    nextChar = (char)fgetc( hFile );

    // Loop until we reach a new-line character
    while( nextChar != '\n' && index < MAX_LINE-2 )
    {
        // Check for end of file:
        if( nextChar == EOF )
        {
            // If found, close off string
            // and return EOF:
            str[ index ] = '\0';
            return EOF;
        }

        // Record the text
        if( nextChar == ' ' || nextChar == ',' || nextChar == '|' )
           str[ index++ ] = '\0';
        else if( nextChar != '\r' && nextChar != '\n' &&
                 nextChar != '<' && nextChar != '>' )
           str[ index++ ] = nextChar;
            
        // Get the next character
        nextChar = (char)fgetc( hFile );
    }

    // Close off the string.
    if( index )
    {
        str[ index++ ] = '\0';
        str[ index ] = '\0';
    }

    return index;

}   // GetLine


/************************************************************************
*    DESCRIPTION:  Match up all the vertices in the faces to
*                  the verstices in the local list.
*
*    Output:   bool - true on success, false on error
************************************************************************/   
bool CTrueSpaceParser::MatchVertsToLocalList()
{
    bool result = false;
    uint i, j, pt, v;
    PCVertice pVert;
    PCFace pFace;
    PCGroup pGroup;
    bool fMatchFound;

    // We have to have a list
    if( groupLst.size() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Cycle through the group list
    for( i = 0; i < groupLst.size(); ++i )
    {        
        // Get a pointer to the group
        pGroup = groupLst[ i ];

        // Cycle through all the faces in the group
        for( j = 0; j < pGroup->faceLst.size(); ++j )
        {
            // Get a pointer to the face in the group
            pFace = (PCFace)pGroup->faceLst[ j ];

            // Check each point of this face
            for( pt = 0; pt < pFace->vertCount; ++pt )
            {
                fMatchFound = false;

                // Cycle through the vert list looking for a match
                for( v = 0; v < pGroup->vertLst.size(); ++v )
                {
                    // Get the vertex to check against
                    pVert = pGroup->vertLst[ v ];

                    // Do we have a match?
                    if( pFace->point[pt].vert[0] == pVert->vert[0] &&
                        pFace->point[pt].vert[1] == pVert->vert[1] &&
                        pFace->point[pt].vert[2] == pVert->vert[2] )
                    {
                        // Record the index into the vert list
                        pFace->vIndex[pt] = v;
                        // Set the sanity flag
                        fMatchFound = true;
                        // out work here is done
                        break;
                    }
                }

                // Do a sanity check. We have to have a match.
                if( !fMatchFound )
                {
                    errorCode = ERROR_MATCH_NOT_FOUND_VERTICES;
                    goto HANDLE_ERROR;
                }
            }
        }
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // MatchVerticies


/************************************************************************
*    DESCRIPTION:  Match up all the vertices in the faces to
*                  the verstices in the list.
*
*    Output:   bool - true on success, false on error
************************************************************************/   
bool CTrueSpaceParser::MatchVertsToGlobalList()
{
    bool result = false;
    uint i, j, pt;
    PCVertice pVert;
    PCFace pFace;
    bool fMatchFound;

    // We have to have a list
    if( vertLst.size() == 0 || faceLst.size() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Cycle through all the faces in the group
    for( i = 0; i < faceLst.size(); ++i )
    {
        // Get a pointer to the face in the group
        pFace = faceLst[ i ];

        // Check each point of this face
        for( pt = 0; pt < pFace->vertCount; ++pt )
        {
            fMatchFound = false;

            // Cycle through the vert list looking for a match
            for( j = 0; j < vertLst.size(); ++j )
            {
                // Get the vertex to check against
                pVert = vertLst[ j ];

                // Do we have a match?
                if( pFace->point[pt].vert[0] == pVert->vert[0] &&
                    pFace->point[pt].vert[1] == pVert->vert[1] &&
                    pFace->point[pt].vert[2] == pVert->vert[2] )
                {
                    // Record the index into the vert list
                    pFace->vIndex[pt] = j;
                    // Set the sanity flag
                    fMatchFound = true;
                    // out work here is done
                    break;
                }
            }

            // Do a sanity check. We have to have a match.
            if( !fMatchFound )
            {
                errorCode = ERROR_MATCH_NOT_FOUND_VERTICES;
                goto HANDLE_ERROR;
            }
        }
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // MatchVertsToGlobalList


/************************************************************************
*    DESCRIPTION:  Save polygon object data to file
*
*    Input:    char fileName    - Output file name of polygon file
*
*    Output:   bool - true on success, false on error
************************************************************************/   
bool CTrueSpaceParser::SaveToFile_PLY( char *fileName )
{
    bool result = false;
    FILE *hFile;
    int vCount, faceCount;
    int objType = 0, objID = 0;
    CFaceLoad *pFaceLoad;
    char formatTypeMeshRaw[FORMAT_SPEC_STR_COUNT];

    strcpy( formatTypeMeshRaw, MESH_RAW_FORMAT_STR_ID );

    // Change the extension to the original file path
    RelpaceFileExtension( NULL, fileName, ".ply" );

    // Save the file path in case of problem
    strcpy( errorFilePath, fileName );

    // Create the file
    hFile = fopen ( fileName, "wb" );
    if( hFile == NULL )
    {
        errorCode = ERROR_PLY_FILE_HANDLE_FAIL;
        goto HANDLE_ERROR;
    }

    // Get the number of vertices
    vCount = vertLst.size();

    // get the number of faces
    faceCount = faceLst.size();

    // Write out the format ID
    if( fwrite( formatTypeMeshRaw, sizeof( formatTypeMeshRaw ), 1, hFile ) != 1 )
    {
        errorCode = ERROR_FILE_WRITE;
        goto HANDLE_ERROR;
    }

    // Write out the object type even though there really isn't
    // one for this. Just keeping the file format working
    if( fwrite( &objType, sizeof( objType ), 1, hFile ) != 1 )
    {
        errorCode = ERROR_FILE_WRITE;
        goto HANDLE_ERROR;
    }

    // Write out the object ID
    if( fwrite( &objID, sizeof( objID ), 1, hFile ) != 1 )
    {
        errorCode = ERROR_FILE_WRITE;
        goto HANDLE_ERROR;
    }

    // Write out the vertex count
    if( fwrite( &vCount, sizeof( vCount ), 1, hFile ) != 1 )
    {
        errorCode = ERROR_FILE_WRITE;
        goto HANDLE_ERROR;
    }

    // Write out the number of polygons
    if( fwrite( &faceCount, sizeof( faceCount ), 1, hFile ) != 1 )
    {
        errorCode = ERROR_FILE_WRITE;
        goto HANDLE_ERROR;
    }

    // Write out the vertex data
    for( uint i = 0; i < vertLst.size(); ++i )
    {
        if( fwrite( vertLst[ i ]->vert, sizeof( CVertLoad ), 1, hFile ) != 1 )
        {
            errorCode = ERROR_FILE_WRITE;
            goto HANDLE_ERROR;
        }
    }

    // Save all the faces to the file
    for( uint i = 0; i < faceLst.size(); ++i )
    {
        pFaceLoad = dynamic_cast<CFaceLoad *>(faceLst[ i ]);

        if( pFaceLoad == NULL )
        {
            errorCode = ERROR_ALLOCATION_FAIL;
            goto HANDLE_ERROR;
        }
        
        // Get a pointer to the face in the group
        if( fwrite( pFaceLoad, sizeof(CFaceLoad), 1, hFile ) != 1 )
        {
            errorCode = ERROR_FILE_WRITE;
            goto HANDLE_ERROR;
        }
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );

    return result;

}   // SaveToFile_PLY


/************************************************************************
*    DESCRIPTION:  Save polygon object data to file
*
*    Input:    char *fileName    - Output file name of polygon
*
*    Output:   bool - true on success, false on error
************************************************************************/   
bool CTrueSpaceParser::SaveToFile_MAP( char *fileName )
{
    bool result = false;
    FILE *hFile;
    int vCount, faceCount, groupCount;
    int objType, objID, noTypeCount(0);
    CFaceLoad *pFaceLoad;
    PCGroup pGroup;
    char formatTypeMeshLst[FORMAT_SPEC_STR_COUNT];
    char formatTypeMeshRaw[FORMAT_SPEC_STR_COUNT];

    // Setup the format id's
    memset( formatTypeMeshLst, 0, sizeof(formatTypeMeshLst) );
    memset( formatTypeMeshRaw, 0, sizeof(formatTypeMeshRaw) );
    strcpy( formatTypeMeshLst, MESH_LST_FORMAT_STR_ID );
    strcpy( formatTypeMeshRaw, MESH_RAW_FORMAT_STR_ID );

    // Change the extension to the original file path
    RelpaceFileExtension( NULL, fileName, ".map" );

    // Save the file path in case of problem
    strcpy( errorFilePath, fileName );

    // Create the file
    hFile = fopen ( fileName, "wb" );
    if( hFile == NULL )
    {
        errorCode = ERROR_PLY_FILE_HANDLE_FAIL;
        goto HANDLE_ERROR;
    }

    // Add up the no type groups
    for( uint i = 0; i < groupLst.size(); ++i )
    {
        if( groupLst[i]->objectType == EOT_NO_TYPE &&
            groupLst[i]->mapData[EMD_ROOM_1] > -1 )
        {
            ++noTypeCount;
        }
    }

    // Get the group count
    groupCount = groupLst.size() - noTypeCount;

    // Write out the format ID
    if( fwrite( formatTypeMeshLst, sizeof( formatTypeMeshLst ), 1, hFile ) != 1 )
    {
        errorCode = ERROR_FILE_WRITE;
        goto HANDLE_ERROR;
    }

    // Write out the group count
    if( fwrite( &groupCount, sizeof( groupCount ), 1, hFile ) != 1 )
    {
        errorCode = ERROR_FILE_WRITE;
        goto HANDLE_ERROR;
    }

    for( uint i = 0; i < groupLst.size(); ++i )
    {
        pGroup = groupLst[ i ];

        // groups that are of no type and associated with a room
        // are not to be added to the map file.
        if( pGroup->objectType == EOT_NO_TYPE &&
            pGroup->mapData[EMD_ROOM_1] > -1 )
        {
            continue;
        }

        CVertLst &tmpVertLst = pGroup->vertLst;
        CFaceLst &tmpFaceLst = pGroup->faceLst;

        // Get the object type
        objType = pGroup->objectType;

        // Get the object type
        objID = pGroup->objectID;
    
        // Get the number of vertices
        vCount = tmpVertLst.size();

        // get the number of faces
        faceCount = tmpFaceLst.size();

        // Write out the format ID
        if( fwrite( formatTypeMeshRaw, sizeof( formatTypeMeshRaw ), 1, hFile ) != 1 )
        {
            errorCode = ERROR_FILE_WRITE;
            goto HANDLE_ERROR;
        }

        // Write out the object type
        if( fwrite( &objType, sizeof( objType ), 1, hFile ) != 1 )
        {
            errorCode = ERROR_FILE_WRITE;
            goto HANDLE_ERROR;
        }

        // Write out the object ID
        if( fwrite( &objID, sizeof( objID ), 1, hFile ) != 1 )
        {
            errorCode = ERROR_FILE_WRITE;
            goto HANDLE_ERROR;
        }

        // Write out the room to door data
        if( fwrite( pGroup->mapData, sizeof( pGroup->mapData ), 1, hFile ) != 1 )
        {
            errorCode = ERROR_FILE_WRITE;
            goto HANDLE_ERROR;
        }

        // Write out the vertex count
        if( fwrite( &vCount, sizeof( vCount ), 1, hFile ) != 1 )
        {
            errorCode = ERROR_FILE_WRITE;
            goto HANDLE_ERROR;
        }

        // Write out the number of polygons
        if( fwrite( &faceCount, sizeof( faceCount ), 1, hFile ) != 1 )
        {
            errorCode = ERROR_FILE_WRITE;
            goto HANDLE_ERROR;
        }

        // Write out the vertex data
        for( uint j = 0; j < tmpVertLst.size(); ++j )
        {
            if( fwrite( tmpVertLst[ j ]->vert, sizeof( CVertLoad ), 1, hFile ) != 1 )
            {
                errorCode = ERROR_FILE_WRITE;
                goto HANDLE_ERROR;
            }
        }

        // Write all the faces to the file
        for( uint j = 0; j < tmpFaceLst.size(); ++j )
        {
            pFaceLoad = dynamic_cast<CFaceLoad *>(tmpFaceLst[ j ]);

            if( pFaceLoad == NULL )
            {
                errorCode = ERROR_ALLOCATION_FAIL;
                goto HANDLE_ERROR;
            }
            
            // Get a pointer to the face in the group
            if( fwrite( pFaceLoad, sizeof(CFaceLoad), 1, hFile ) != 1 )
            {
                errorCode = ERROR_FILE_WRITE;
                goto HANDLE_ERROR;
            }
        }
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );

    return result;

}   // SaveToFile_MAP


/************************************************************************
*    DESCRIPTION:  Sorting by texture index will case for less
*                  state changes in which should help speed.
*
*    Input:    CObjList &faceLst  - Object list of vertices
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
void CTrueSpaceParser::SortByTextIndex( CFaceLst &faceLst )
{
    int offset, j, x=0;
    bool inOrder;
    PCFace pFace1, pFace2, ptemp;
    
    inOrder = false;
    offset = faceLst.size();

    // Make sure we have something to sort
    if( offset > 1 )
    {
        while( x < offset && inOrder == false )
        {
            ++x;
            inOrder = true;
            for( j = 0; j < offset - x; ++j )
            {
                pFace1 = faceLst[j];
                pFace2 = faceLst[j+1];
                
                // Sort in an assending order
                if( pFace1->tIndex > pFace2->tIndex )
                {
                    inOrder = false;
                    ptemp = pFace1;
                    faceLst[ j ] = pFace2;
                    faceLst[ j+1 ] = ptemp;
                }
            }
        }
    }

}   // SortByTextIndex


/************************************************************************
*    DESCRIPTION:  Sort the objects by object name
*
*    Input:    CObjList &tmpFaceLst  - Object list of vertices
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
/*   
void CTrueSpaceParser::SortByObjectName( CObjList &groupLst )
{
    int offset, j, x=0;
    bool inOrder;
    PCGroup pObj1, pObj2, ptemp;
    
    inOrder = false;
    offset = groupLst.Count();

    // Make sure we have something to sort
    if( offset > 1 )
    {
        while( x < offset && inOrder == false )
        {
            ++x;
            inOrder = true;
            for( j = 0; j < offset - x; ++j )
            {
                pObj1 = (PCGroup)groupLst.GetObj(j);
                pObj2 = (PCGroup)groupLst.GetObj(j+1);
                
                // Sort in an assending order
                if( strcmp( pObj1->name, pObj2->name ) > 0 )
                {
                    inOrder = false;
                    ptemp = pObj1;
                    groupLst.SetObj( j, pObj2 );
                    groupLst.SetObj( j+1, ptemp );
                }
            }
        }
    }

}   // SortByObjectName  */


/************************************************************************
*    DESCRIPTION:  Match texture index to face
*
*    Output:   bool - true on success, false on error
************************************************************************/   
bool CTrueSpaceParser::MatchTextureIndexToFace()
{
    bool result = false;
    uint i, j, w;
    PCGroup pGroup;
    PCFace pFace;
	string grpPathStr, txtPathStr;
    bool fMatchFound;

    // Cycle through the group list
    for( i = 0; i < groupLst.size(); ++i )
    {        
        // Get a pointer to the group
        pGroup = groupLst[ i ];

        // Make sure this group has textures associated with it
        if( pGroup->textStrMap.size() > 0 )
        {
            for( j = 0; j < pGroup->faceLst.size(); ++j )
            {
                // Get the face
                pFace = pGroup->faceLst[ j ];

                // Check that we are not out of range
                if( (uint)pFace->tIndex >= pGroup->textStrMap.size() )
                {
                    errorCode = ERROR_DATA_OUT_OF_RANGE;
                    goto HANDLE_ERROR;
                }
                
                // Get the path to the group texture this face uses
                grpPathStr = pGroup->textStrMap[ pFace->tIndex ];

                fMatchFound = false;

                // Cycle through the texture list looking for a match
                for( w = 0; w < textStrLst.size() && !fMatchFound; ++w )
                {
                    // Get the texture path to check against
                    txtPathStr = textStrLst[ w ];

                    // Do we have a match?
                    if( grpPathStr == txtPathStr )
                    {
                        // Set the index into the global texture library
                        pFace->tIndex = (unsigned short int)w;

                        // Set the sanity flag
                        fMatchFound = true;
                    }
                }

                // Do a sanity check. We must have a match.
                if( !fMatchFound )
                {
                    errorCode = ERROR_MATCH_NOT_FOUND_TEXTURE;
                    goto HANDLE_ERROR;
                }
            }
        }       
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // MatchTextureIndexToFace


/************************************************************************
*    DESCRIPTION:  Build the texture library file
*
*    Input:    char *filePath   - Path to where the texture library file
*                                 is to be saved
*
*    Output:   bool - true on success, false on error
************************************************************************/   
bool CTrueSpaceParser::BuildTextureLibrary( char *filePath )
{
    bool result = false;
    int i;

    /////////////////////////////////////////////////////
    //   Build a 32 bit texture map library
    /////////////////////////////////////////////////////

    // Tell the texture library how many textures we need
    if( !textLib.AllocateTextLibrary( textStrLst.size() ) )
    {
        errorCode = ERROR_TEXT_LIB_ALLOC;
        goto HANDLE_ERROR;
    }

    // Now add each texture to the library
    for( i = 0; i < textLib.Count(); ++i )
    {
        // Save the file path in case of problem
        strcpy( errorFilePath, textStrLst[ i ].c_str() );

        // Add the texture to the library
        if( !textLib.AddTextureToLib( i, (char *)textStrLst[ i ].c_str(), EBT_KEEP_BOTTOM_TO_SAME ) )
        {
            errorCode = ERROR_LOAD_TEXTURE;
            goto HANDLE_ERROR;
        }
    }

    // Replace the extension to the texture library file
    RelpaceFileExtension( NULL, filePath, ".bt" );
    
    // Save the library to file
    textLib.SaveToFile( filePath );

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // BuildTextureLibrary


/************************************************************************
*    DESCRIPTION:  Strip the file name from the path and add
*                  new file name.
*
*    Input:    char *sourcePath  - Source string
*              char *destPath    - Destination string
*              char *newFileName - new file name 
*
*    Output:   returntype - Description
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void CTrueSpaceParser::CropOutPathAddFileName( char *sourcePath, char *destPath, char *newFileName )
{
    int i, length;
    bool fExtensionCheck = false;

    if( sourcePath )
        strcpy( destPath, sourcePath );

    // Get the string length
    length = strlen( destPath );

    for( i = length; i > 0; --i )
    {
        // Set the flag if we found the dot extension
        if( destPath[i] == '.' )
            fExtensionCheck = true;

        if( destPath[i] == '\\' )
            break;
    }

    // Ternamate only if there was an extension
    if( fExtensionCheck )
        destPath[i+1] = 0;

    // Append the file name to the path
    strcat( destPath, newFileName );

}   // CropOutPathAddFileName


/************************************************************************
*    DESCRIPTION:  Build a map file to show the data organization.
*
*    Input:    char *filePath - file path to save object file
*
*    Output:   bool - true on success, false on error
************************************************************************/   
bool CTrueSpaceParser::BuildMapFile( char *filePath )
{
    FILE *hFile;
    int faceCount=0, vertCount=0, noTypeCount(0);
    bool result = false;
    SCollisionType colType;
    PCGroup pGroup;
    SObjectType objTypeStruct;
    int textureDataSize(0); 

    for( uint i = 0; i < groupLst.size(); ++i )
    {
        // Add up the no type groups
        if( groupLst[i]->objectType == EOT_NO_TYPE &&
            groupLst[i]->mapData[EMD_ROOM_1] > -1 )
        {
            ++noTypeCount;
        }
        else // Add up everything else
        {
            vertCount += groupLst[ i ]->vertLst.size();
            faceCount += groupLst[ i ]->faceLst.size();
        }
    }

    // Change the extension to the original file path
    RelpaceFileExtension( NULL, filePath, ".txt" );

    // Open the file
    hFile = fopen( filePath, "wt" );
    if( hFile == NULL )
    {
        errorCode = ERROR_MAP_FILE_HANDLE_FAIL;
        goto HANDLE_ERROR;
    }    

    // File header
    fprintf( hFile, "Group Count: %d\n", groupLst.size() );
    fprintf( hFile, "No Type Group Count: %d\n", noTypeCount );
    fprintf( hFile, "Total Vertex Count: %d\n", vertCount );
    fprintf( hFile, "Total Face Count: %d\n", faceCount );

    for( uint obj = 0; obj < groupLst.size(); ++obj )
    {
        pGroup = groupLst[ obj ];

        CVertLst &tmpVertLst = pGroup->vertLst;
        CFaceLst &tmpFaceLst = pGroup->faceLst;

        fprintf( hFile, "\nGroup Type: %s\n", objTypeStruct.typeStr[ pGroup->objectType ].c_str() );
        fprintf( hFile, "Group Id: %d\n", pGroup->objectID );
        fprintf( hFile, "data: %d\n", pGroup->mapData[EMD_ROOM_1] );
        fprintf( hFile, "data: %d\n", pGroup->mapData[EMD_ROOM_2] );
		fprintf( hFile, "map Id: %d\n", pGroup->mapData[EMD_MAP_ID] );
		fprintf( hFile, "floor Index: %d\n\n", pGroup->mapData[EMD_MAP_FLOOR] );

        fprintf( hFile, "Vertex List:\n" );
        fprintf( hFile, "Vertex Count: %d\n\n", tmpVertLst.size() );

        // List all the vertices
        for( uint i = 0; i < tmpVertLst.size(); ++i )
        {
            // Get the vertex data
            PCVertice pVert = tmpVertLst[ i ];

            if( i < 10 )
                fprintf( hFile, "%u:    %f, %f, %f\n", i, pVert->vert[0], pVert->vert[1], pVert->vert[2] );
            else if( i < 100 )
                fprintf( hFile, "%u:   %f, %f, %f\n", i, pVert->vert[0], pVert->vert[1], pVert->vert[2] );
            else if( i < 1000 )
                fprintf( hFile, "%u:  %f, %f, %f\n", i, pVert->vert[0], pVert->vert[1], pVert->vert[2] );
            else if( i < 10000 )
                fprintf( hFile, "%u:  %f, %f, %f\n", i, pVert->vert[0], pVert->vert[1], pVert->vert[2] );
            else
                fprintf( hFile, "%u: %f, %f, %f\n", i, pVert->vert[0], pVert->vert[1], pVert->vert[2] );             
        }

		/*
        // Face list
        fprintf( hFile, "\nFace List:\n" );
        fprintf( hFile, "Face Count: %d\n", pFaceLst->Count() );

        // Cycle through all the faces in the group
        for( uint j = 0; j < pFaceLst->Count(); ++j )
        {
            // Get a pointer to the face in the group
            pFace = pFaceLst->Get( j );

            // Face count
            fprintf( hFile, "    %d Face:\n", j+1 );

            // Collision type
            fprintf( hFile, "      Collision Type: %s\n", colType.typeStr[ pFace->collisionType ] );

            // Vertex count
            fprintf( hFile, "      Vertex Count: %d\n", (int)pFace->vertCount );

            if( pFace->vertCount == TRI )
            {
                // Display vertex index
                fprintf( hFile, "      vIndex: %d, %d, %d\n", pFace->vIndex[0], 
                         pFace->vIndex[1], pFace->vIndex[2] );
            }
            else if( pFace->vertCount == QUAD )
            {
                // Display vertex index
                fprintf( hFile, "      vIndex: %d, %d, %d, %d\n", pFace->vIndex[0], 
                         pFace->vIndex[1], pFace->vIndex[2], pFace->vIndex[3] );
            }

            for( w = 0; w < pFace->vertCount; ++w )
            {
                // Display the UV coordinate
                fprintf( hFile, "      UV: u:%f, v:%f\n", pFace->crd[w].uv[0], pFace->crd[w].uv[1] );
            }

            // Texture index
			if( textStrLst.size() > pFace->tIndex )
			{
				fprintf( hFile, "      tIndex: %u, name: %s\n", pFace->tIndex, textStrLst[ pFace->tIndex ] );
			}
        }*/
    }
    
    /*fprintf( hFile, "\nGlobal Vertex List:\n" );
    fprintf( hFile, "Global Vertex Count: %d\n\n", vLst.Count() );

    // Global vertex list
    for( uint i = 0; i < vLst.Count(); ++i )
    {
        // Get the vertex data
        pVert = (PCVertice)vLst.GetObj( i );

        if( i < 10 )
            fprintf( hFile, "%u:    %f, %f, %f\n", i, pVert->vert[0], pVert->vert[1], pVert->vert[2] );
        else if( i < 100 )
            fprintf( hFile, "%u:   %f, %f, %f\n", i, pVert->vert[0], pVert->vert[1], pVert->vert[2] );
        else if( i < 1000 )
            fprintf( hFile, "%u:  %f, %f, %f\n", i, pVert->vert[0], pVert->vert[1], pVert->vert[2] );
        else if( i < 10000 )
            fprintf( hFile, "%u:  %f, %f, %f\n", i, pVert->vert[0], pVert->vert[1], pVert->vert[2] );
        else
            fprintf( hFile, "%u: %f, %f, %f\n", i, pVert->vert[0], pVert->vert[1], pVert->vert[2] );             

    }

    fprintf( hFile, "\nGlobal Face List:\n" );
    fprintf( hFile, "Global Face Count: %d\n\n", fLst.Count() );

    // Cycle through all the faces in the group
    for( uint j = 0; j < fLst.Count(); ++j )
    {
        // Get a pointer to the face in the group
        pFace = (PCFace)fLst.GetObj( j );

        // Face count
        fprintf( hFile, "    %d Face:\n", j+1 );

        // Collision type
        fprintf( hFile, "      Collision Type: %s\n", colType.typeStr[ pFace->collisionType ] );

        // Vertex count
        fprintf( hFile, "      Vertex Count: %d\n", (int)pFace->vertCount );

        if( pFace->vertCount == TRI )
        {
            // Display vertex index
            fprintf( hFile, "      vIndex: %d, %d, %d\n", pFace->vIndex[0], 
                     pFace->vIndex[1], pFace->vIndex[2] );
        }
        else if( pFace->vertCount == QUAD )
        {
            // Display vertex index
            fprintf( hFile, "      vIndex: %d, %d, %d, %d\n", pFace->vIndex[0], 
                     pFace->vIndex[1], pFace->vIndex[2], pFace->vIndex[3] );
        }

        for( uint w = 0; w < pFace->vertCount; ++w )
        {
            // Display the UV coordinate
            fprintf( hFile, "      UV: u:%f, v:%f\n", pFace->crd[w].uv[0], pFace->crd[w].uv[1] );
        }

        // Texture index
        fprintf( hFile, "      tIndex: %u, name: %s\n", pFace->tIndex, (char *)tLst.GetObj( pFace->tIndex ) );
    }*/
    
    // texture map list
    fprintf( hFile, "\nTexture Map List:\n" );
    fprintf( hFile, "Texture count: %d\n", textLib.Count() );

    // Cycle through the texture map list
    for( int i = 0; i < textLib.Count(); ++i )
    {
        // Get the name of the texture
        fprintf( hFile, "\n  %d: %s\n", i, textStrLst[ i ].c_str() );

        // Get the width and height of texture
        fprintf( hFile, "      width: %d, height: %d", textLib.pTMap[i].GetWidth(), textLib.pTMap[i].GetHeight() );

        textureDataSize += textLib.pTMap[i].GetWidth() * textLib.pTMap[i].GetHeight() * 4;
    }

    fprintf( hFile, "\n\nTexture Mem Size: %f  megs\n", (float)textureDataSize / 1048576.0f );

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );

    return result;

}   // BuildMapFile


/************************************************************************
*    DESCRIPTION:  Strip the file name from the file path
*
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void CTrueSpaceParser::CropFileNameFromPath( char *SourceFilePath, char *DestFileName, bool KeepExtension )
{
    int i, Length;

    // Get the string length
    Length = strlen( SourceFilePath );

    for( i = Length; i > -1; --i )
    {
        if( SourceFilePath[i] == '\\' )
            break;
    }

    ++i;

    strcpy( DestFileName, (SourceFilePath + i) );

    // Remove the extension
    if( !KeepExtension )
    {
        // Get the string length
        Length = strlen( DestFileName );

        for( i = Length; i > -1; --i )
        {
            if( DestFileName[i] == '.' )
            {
                DestFileName[i] = 0;
                break;
            }
        }
    }

}   // CropFileNameFromPath


/************************************************************************
*    DESCRIPTION:  Search a string and replace a character with
*                  a different character
*
*    Input:    char *charString - string to search
*              char find - character to find
*              char replace - character to replace with found character 
*
*    Output:   returntype - Description
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void CTrueSpaceParser::RelpaceChar( char *charString, char find, char replace, int strCount )
{
    int i;

    for( i = 0; i < strCount; ++i )
    {
        if( charString[i] == find )
            charString[i] = replace;
    }

}   // RelpaceChar