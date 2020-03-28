
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
#include "CMatrix.h"         // Matrix header.
#include "CMeshList.h"       // mesh list class
#include "gTools.h"

// These are globals used by the 3D engine and must be setup
float gfMaxZDistance = -1000;
float gfMinZDistance = -1.0f;
int gCurrentID=0;
float gfSquarePercentage;
float gfFrustrumYRatio;
float gfAspectRatio;
int giRenderDevice;
unsigned int gOpenGLTextCurrentID=0;
int gTriCounter;
int gQuadCounter;
float gfCameraScaleX;
float gfCameraScaleY;
float gfBufWidthDiv2;
float gfBufHeightDiv2;
int giBufWidth;
int giBufHeight;
unsigned int *gusiVBufer;
int *giZBuffer;


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
}   // Destructor


/************************************************************************
*    DESCRIPTION:  Load OBJ text file and parse it. This export
*                  has two files. The OBJ file refers to a
*                  MTL file for all the texture file information.
*
*    Input:    char *fileName - Path to load file
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CTrueSpaceParser::LoadFromFile( char *filePath )
{
    bool result = false;
    char lineStr[MAX_LINE], fileName[400];
    int strCount;
    FILE *hFile;
    CTextureLib textLib;
    CGroupLst groupLst;
    CFaceLst faceLst;
    CVertLst vertLst;
    SCollisionType colType;

    CTextStrLst textLst;

    // We don't want this data to be freed because it is being shared
    textLst.DontFreeData();

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

    ///////////////////////////////////////
    // Make sure this is the ascii file
    ///////////////////////////////////////

    // Set the starting state
    state = EPS_NO_STATE;
    memset( typeCounter, 0, sizeof(typeCounter) );

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
            if( !AddGroup( groupLst ) )
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
            if( !AddFaceToGroup( groupLst, lineStr, strCount ) )
                goto HANDLE_ERROR;

            // Set the starting state
            state = EPS_FACE;
        }
        else if( strcmp( lineStr, "texture:" ) == 0 )
        {
            if( !AddMateralNameToGroup( groupLst, lineStr, strCount ) )
                goto HANDLE_ERROR;

            // Set the starting state
            state = EPS_NO_STATE;
        }
        else if( state == EPS_NO_STATE )
        {
            // Check for collision types
            for( int i = 0; i < ECT_MAX_TYPES; ++i )
            {
                if( strcmp( lineStr, colType.typeStr[i] ) == 0 )
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
            if( !Decode_Vertice( groupLst, lineStr, strCount ) )
                goto HANDLE_ERROR;
        }
        else if( state == EPS_UV )
        {
            if( !Decode_UV( groupLst, lineStr, strCount ) )
                goto HANDLE_ERROR;
        }
        else if( state == EPS_FACE )
        {
            if( !Decode_Face( groupLst, lineStr, strCount ) )
                goto HANDLE_ERROR;

            // Set the starting state
            state = EPS_NO_STATE;
        }
        else if( state == EPS_COLLISION_TYPE )
        {
            Decode_CollisionType( groupLst, lineStr, strCount );
        }
    }

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
    if( !RemoveDupVerticies( groupLst ) )
        goto HANDLE_ERROR;
    
    // Match the local faces to the local verts
    if( !MatchVertsToLocalList( groupLst ) )
        goto HANDLE_ERROR;

    // Match the doors to the rooms
    MatchDoorsToRooms( groupLst );

    // Match the objects to rooms
    MatchObjectsToRooms( groupLst );

    // Combine none-type objects to the room mesh
    CombineNoTypeObjectsToRoomMesh( groupLst );

    // Create global texture list
    CreateTextureList( groupLst, textLst );

    // Do we have a texture library file to parse?
    if( textLst.Count() > 0 )
    {
        // Match up the texture library indexes to the faces
        if( !MatchTextureIndexToFace( groupLst, textLst ) )
            goto HANDLE_ERROR;

        if( !BuildTextureLibrary( textLib, textLst, fileName ) )
            goto HANDLE_ERROR;

        // Sort the group faces by texture index
        for( int i = 0; i < groupLst.Count(); ++i )
        {
            SortByTextIndex( groupLst.Get( i )->faceLst );
        }
    }

    // Save the polygon seperated object file
    if( !SaveToFile_Seperated( groupLst, fileName ) )
        goto HANDLE_ERROR;


    ///////////////////////////////////////////////////
    //  Build the standard ply file.
    ///////////////////////////////////////////////////

    // Make a list of global verticies
    if( !MakeGlobalVertLst( groupLst, vertLst ) )
        goto HANDLE_ERROR;

    // Make a list of global faces
    if( !MakeGlobalFaceLst( groupLst, faceLst ) )
        goto HANDLE_ERROR;
    
    // Remove the duplicate vertices from the vertex list
    if( !RemoveDupVerticies( vertLst ) )
        goto HANDLE_ERROR;
        
    // Match the global faces to the global verticies
    if( !MatchVertsToGlobalList( vertLst, faceLst ) )
        goto HANDLE_ERROR;

    // Save the polygon condenced object file
    if( !SaveToFile_Condenced( vertLst, faceLst, fileName ) )
        goto HANDLE_ERROR;

    // Generate normal file
    if( !GenerateNormals( fileName ) )
        goto HANDLE_ERROR;

    

    // Build a map file to show all that we have done
    if( !BuildMapFile( groupLst, textLst, textLib, fileName ) )
        goto HANDLE_ERROR;


    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );
    
    return result;

}   // LoadFromFile


/************************************************************************
*    DESCRIPTION:  Match up all the doors with ajoining rooms.
*
*    Input:    CGroupLst &groupLst - Group list
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
void CTrueSpaceParser::MatchDoorsToRooms( CGroupLst &groupLst )
{   
    PCFace pDoorFace;
    PCFace pRoomFace;
    CFaceLst *pFaceLst;
    PCGroup pDoorGroup;
    PCGroup pRoomGroup;
    int count;

    for( int i = 0; i < groupLst.Count(); ++i )
    {
        pDoorGroup = groupLst.Get(i);

        // If this is a doorway, search for the two rooms it connects to.
        if( pDoorGroup->objectType == EOT_MAP_DOORWAY )
        {
            count = 0;

            // keep going until one of the faces collides with two rooms
            for( int face = 0; face < pDoorGroup->faceLst.Count() && (count < 2); ++face )
            {
                // Get one of the faces of the door. One face will touch both rooms
                pDoorFace = PCFace( pDoorGroup->faceLst.Get(face) );

                for( int grp = 0; grp < groupLst.Count() && (count < 2); ++grp )
                {
                    pRoomGroup = groupLst.Get(grp);
                     
                    // If this is a room see if we have matching verts?
                    if( (pRoomGroup->objectType == EOT_MAP_ROOM) ||
                        (pRoomGroup->objectType == EOT_MAP_HALLWAY) ||
                        (pRoomGroup->objectType == EOT_MAP_STAIRWAY) )
                    {
                        pFaceLst = &pRoomGroup->faceLst;

                        // Go through all the faces and see if we have a collision
                        for( int w = 0; w < pFaceLst->Count(); ++w )
                        {
                            pRoomFace = PCFace( pFaceLst->Get( w ) );

                            if( DoFacesCollide( pRoomFace, pDoorFace ) )
                            {
                                if( pDoorGroup->mapData[ 0 ] != pRoomGroup->objectID )
                                {
                                    pDoorGroup->mapData[ count++ ] = pRoomGroup->objectID;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}   // MatchDoorsToRooms


/************************************************************************
*    DESCRIPTION:  Match up all the objects with rooms.
*
*    Input:    CGroupLst &groupLst - Group list
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
void CTrueSpaceParser::MatchObjectsToRooms( CGroupLst &groupLst )
{
    int count, totalCount;

    for( int obj = 0; obj < groupLst.Count(); ++obj )
    {
        // If this is an object, search for the two rooms it connects to.
        if( groupLst.Get(obj)->objectType == EOT_NO_TYPE ||
            groupLst.Get(obj)->objectType == EOT_MAP_OBJECT ||
            groupLst.Get(obj)->objectType == EOT_MAP_COLLISION ||
            groupLst.Get(obj)->objectType == EOT_MAP_SPAWN_POINT ||
            groupLst.Get(obj)->objectType == EOT_MAP_BOT_SPAWN_POINT )
        {
            totalCount = 0;

            // Find the rooms and see if the object collides
            for( int room = 0; room < groupLst.Count(); ++room )
            {                 
                // If this is a room see if we have matching verts?
                if( (groupLst.Get(room)->objectType == EOT_MAP_ROOM) ||
                    (groupLst.Get(room)->objectType == EOT_MAP_HALLWAY) ||
                    (groupLst.Get(room)->objectType == EOT_MAP_STAIRWAY) )
                {
                    count = 0;

                    // Go through all the faces and see and count up all the collisions
                    for( int i = 0; i < groupLst.Get(room)->faceLst.Count(); ++i )
                    {
                        for( int j = 0; j < groupLst.Get(obj)->faceLst.Count(); ++j )
                        {
                            if( DoFacesCollide( PCFace( groupLst.Get(room)->faceLst.Get(i) ), 
                                                PCFace( groupLst.Get(obj)->faceLst.Get(j) ) ) )
                            {                           
                                ++count;
                            }
                        }
                    }

                    // Based on which room gets the most collision hits determins
                    // the room the object is in.
                    if( count > totalCount )
                    {
                        groupLst.Get(obj)->mapData[0] = groupLst.Get(room)->objectID;
                        totalCount = count;
                    }
                }
            }
        }
    }
}   // MatchObjectsToRooms


/************************************************************************
*    DESCRIPTION:  Objects of no specific type are to be added to the
*                  room mesh they are associated with.
*
*    Input:    CGroupLst &groupLst - Group list
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CTrueSpaceParser::CombineNoTypeObjectsToRoomMesh( CGroupLst &groupLst )
{
    bool result = false;

    // Find the no type objects and the room they are associated with
    // and add the object data to the room data
    for( int obj = 0; obj < groupLst.Count(); ++obj )
    {
        if( groupLst.Get(obj)->objectType == EOT_NO_TYPE )
        {
            for( int room = 0; room < groupLst.Count(); ++room )
            {
                if( ((groupLst.Get(room)->objectType == EOT_MAP_ROOM) ||
                     (groupLst.Get(room)->objectType == EOT_MAP_HALLWAY) ||
                     (groupLst.Get(room)->objectType == EOT_MAP_STAIRWAY)) &&
                    groupLst.Get(obj)->mapData[0] == groupLst.Get(room)->objectID )
                {
                    // Get the number of verts in this object
                    int vCount = groupLst.Get( room )->vertLst.Count();

                    // Add the objects verts to the rooms vert list
                    for( int i = 0; i < groupLst.Get(obj)->vertLst.Count(); ++i )
                    {
                        // Allocate a vertice class
                        PCVertice pVert = new CVertLoad;

                        // check for allocations for errors
                        if( pVert == NULL )
                        {
                            errorCode = ERROR_ALLOCATION_FAIL;
                            goto HANDLE_ERROR;
                        }

                        pVert->vert[0] = groupLst.Get(obj)->vertLst.Get(i)->vert[0];
                        pVert->vert[1] = groupLst.Get(obj)->vertLst.Get(i)->vert[1];
                        pVert->vert[2] = groupLst.Get(obj)->vertLst.Get(i)->vert[2];

                        // Add the verts to the room verts
                        groupLst.Get( room )->vertLst.Add( pVert );
                    }

                    // Get the number of textures in this object
                    int tCount = groupLst.Get( room )->textLst.Count();

                    // Add the objects face to the rooms face list
                    for( int i = 0; i < groupLst.Get(obj)->faceLst.Count(); ++i )
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
                        pFace->vertCount = groupLst.Get(obj)->faceLst.Get(i)->vertCount;

                        // Get the collision type
                        pFace->collisionType = groupLst.Get(obj)->faceLst.Get(i)->collisionType;

                        // The index to the textures is now at the end of the room list so take
                        // the texture count of the room and add it to the objets texture index
                        pFace->tIndex = groupLst.Get(obj)->faceLst.Get(i)->tIndex + tCount;

                        // copy over the face data
                        for( int j = 0; j < pFace->vertCount; ++j )
                        {
                            // The index to the verts is now at the end of the room list so take
                            // the vert count of the room and add it to the objects vert index
                            pFace->vIndex[j] = groupLst.Get(obj)->faceLst.Get(i)->vIndex[j] + vCount;

                            // Copy over the uv
                            pFace->crd[j] = groupLst.Get(obj)->faceLst.Get(i)->crd[j];

                            // Copy over the verts
                            pFace->point[j] = groupLst.Get(obj)->faceLst.Get(i)->point[j];
                        }

                        // Add the face to the room faces
                        groupLst.Get( room )->faceLst.Add( pFace );
                    }

                    // Add the objects textures to the room's texture list
                    for( int i = 0; i < groupLst.Get(obj)->textLst.Count(); ++i )
                    {
                        // allocate a string
                        char *pPath = new char[ strlen(groupLst.Get(obj)->textLst.Get(i)) + 1 ];

                        // check for allocations for errors
                        if( pPath == NULL )
                        {
                            errorCode = ERROR_ALLOCATION_FAIL;
                            goto HANDLE_ERROR;
                        }

                        // copy the string
                        strcpy( pPath, groupLst.Get(obj)->textLst.Get(i) );

                        groupLst.Get( room )->textLst.Add( pPath );
                    }
                }
            }
        }
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
*    Input:    CGroupLst &groupLst - Group list
*              char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/22/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CTrueSpaceParser::Decode_CollisionType( CGroupLst &groupLst, char *strData, int strCount )
{
    bool result = false;
    int count = 0, faceIndex;
    CFaceLst *pFaceLst;
    PCFace pFace;

    // We are supposed to have at least one group in the list.
    // Make sure that we do
    if( groupLst.Count() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // There has to be a null first to qualify
    if( strData[0] == 0 )
    {
        pFaceLst = &groupLst.Get( groupLst.Count()-1 )->faceLst;

        // Jump past the null
        while( strData[count++] )
        {}

        do
        {
            // Get the index to the face list
            faceIndex = atoi( &strData[count] );

            // Check that we are not out of range
            if( faceIndex < 0 || faceIndex >= pFaceLst->Count() )
            {
                errorCode = ERROR_DATA_OUT_OF_RANGE;
                goto HANDLE_ERROR;
            }

            // Get the face
            pFace = pFaceLst->Get( faceIndex );

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
*
*    Input:    CGroupLst &groupLst - Group list
*              CTextStrLst &textLst - Texture list
*
*    -------------------------------------------------------------------
*                  11/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/  
void CTrueSpaceParser::CreateTextureList( CGroupLst &groupLst, CTextStrLst &textLst )
{
    PCGroup pGroup;
    char *pGrpPath, *ptxtPath;
    CTextStrLst alphaTextLst;
    alphaTextLst.DontFreeData();
    bool found;

    for( int i = 0; i < groupLst.Count(); ++i )
    {
        pGroup = groupLst.Get( i );

        for( int j = 0; j < pGroup->textLst.Count(); ++j )
        {
            found = false;

            pGrpPath = pGroup->textLst.Get( j );

            // Make sure we don't add duplicates to the list
            for( int w = 0; w < textLst.Count() && !found; ++w )
            {
                ptxtPath = textLst.Get( w );

                if( strcmp( pGrpPath, ptxtPath ) == 0 )
                {
                    found = true;
                }
            }

            // Make sure we don't add duplicates to the list
            for( int w = 0; w < alphaTextLst.Count() && !found; ++w )
            {
                ptxtPath = alphaTextLst.Get( w );

                if( strcmp( pGrpPath, ptxtPath ) == 0 )
                {
                    found = true;
                }
            }

            bool tgaFileFound = (GetFileType( pGrpPath ) == FILE_TYPE_TGA);

            // Flag this object because it has alpha blended textures
            if( tgaFileFound && pGroup->objectType == EOT_MAP_OBJECT )
            {
                pGroup->mapData[1] = OBJECT_WITH_ALPHA_TXT;
            }

            // Add the texture to the global list
            if( found == false )
            {
                // TGA files are assumed to be alpha blended files.
                // added these to a different list so that it can
                // be added to the end
                if( tgaFileFound )
                {
                    alphaTextLst.Add( pGrpPath );
                }
                else
                {
                    textLst.Add( pGrpPath );
                }
            }
        }
    }

    // Add all the alpha textures to the end of the global texture list
    for( int i = 0; i < alphaTextLst.Count(); ++i )
    {
        textLst.Add( alphaTextLst.Get(i) );
    }

}   // CreateTextureList


/************************************************************************
*    DESCRIPTION:  Add a new group to the list
*
*    Input:    CGroupLst &groupLst - Group to add to
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CTrueSpaceParser::AddGroup( CGroupLst &groupLst )
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

    // Save the object type
    pGroup->objectType = objectType;

	// Save the object id
	// Force the room areas to count up as one group
	if( objectType == EOT_MAP_ROOM || 
        objectType == EOT_MAP_HALLWAY ||
        objectType == EOT_MAP_STAIRWAY )
	{
		pGroup->objectID = typeCounter[EOT_MAP_ROOM]++;
	}
	else
	{
		pGroup->objectID = typeCounter[objectType]++;
	}    

    // Add the object to our object list
    groupLst.Add( pGroup );

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

    // Jump past the null
    while( strData[count++] )
    {}

    if( strData[count] )
    {
        // Check for object types
        for( int i = 0; i < EOT_MAX_TYPES; ++i )
        {
            if( strcmp( &strData[count], objTypeStruct.typeStr[i] ) == 0 )
            {
                // Set the type
                objectType = (EOT_OBJECT_TYPE)i;

                break;
            }
        }
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
*    Input:    CGroupLst &objLst - Object list to receive allocation
*              char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/  
bool CTrueSpaceParser::Decode_Vertice( CGroupLst &groupLst, char *strData, int strCount )
{
    bool result = false;
    int i, count = 0;
    PCVertice pVert;
    CMatrix matrix;
    CPoint point;

    // We are supposed to have at least one group in the list.
    // Make sure that we do
    if( groupLst.Count() == 0 )
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
        objectType == EOT_MAP_DOORWAY )
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

    point.x = point.tx;
    point.y = point.ty;
    point.z = point.tz;

	// Set the transform matrix and transform
	matrix.SetMatrix( transMat );
    matrix.Transform( point );

    point.x = point.tx;
    point.y = point.ty;
    point.z = point.tz;

    // Do the center matrix again and transform
    matrix.SetMatrix3to4( centerMat );

    // The z which is actually the y is off if it is a room or doorway
	if( objectType == EOT_MAP_ROOM || 
        objectType == EOT_MAP_HALLWAY ||
        objectType == EOT_MAP_STAIRWAY ||
        objectType == EOT_MAP_DOORWAY )
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
    groupLst.Get( groupLst.Count()-1 )->vertLst.Add( pVert );  

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
*    Input:    CGroupLst &objLst - Object list to receive allocation
*              char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/  
bool CTrueSpaceParser::Decode_UV( CGroupLst &groupLst, char *strData, int strCount )
{
    bool result = false;
    int i, count = 0;
    PCUV pUV;

    // We are supposed to have at least one group in the list.
    // Make sure that we do
    if( groupLst.Count() == 0 )
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
        if( pUV->uv[i] < 0.0f )
            pUV->uv[i] = 0.0f;
        else if( pUV->uv[i] > 1.0f )
            pUV->uv[i] = 1.0f;

        // Jump past the null
        while( strData[count++] )
        {}
    }

    // Add the object to our object list
    groupLst.Get( groupLst.Count()-1 )->uvLst.Add( pUV );

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
*    Input:    CGroupLst &objLst - Object list to receive allocation
*              char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/03/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CTrueSpaceParser::AddFaceToGroup( CGroupLst &groupLst, char *strData, int strCount )
{
    bool result = false;
    int vertCount, count = 0, i;
    PCFace pFace;

    // We are supposed to have at least one group in the list.
    // Make sure that we do
    if( groupLst.Count() == 0 )
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
    groupLst.Get( groupLst.Count()-1 )->faceLst.Add( pFace );  

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
*    Input:    CGroupLst &objLst - Object list to receive allocation
*              char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CTrueSpaceParser::Decode_Face( CGroupLst &groupLst, char *strData, int strCount )
{
    bool result = false;
    int i, j, count = 0, index;
    PCGroup pGroup;
    PCFace pFace;
    PCVertice pVert;
    PCUV pUV;

    // We are supposed to have at least one group in the list.
    // Make sure that we do
    if( groupLst.Count() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Get the face list
    pGroup = groupLst.Get( groupLst.Count()-1 );

    // Get the current face
    pFace = pGroup->faceLst.Get( pGroup->faceLst.Count()-1 );  

    // Decode each vertice & UV and set the face
    for( i = 0; i < pFace->vertCount; ++i )
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
        if( index < 0 || index >= pGroup->vertLst.Count() )
        {
            errorCode = ERROR_DATA_OUT_OF_RANGE;
            goto HANDLE_ERROR;
        }

        // Get the pointer to the vert class
        pVert = pGroup->vertLst.Get( index );

        // Copy over the verts
        for( j = 0; j < 3; ++j )
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
        if( index < 0 || index >= pGroup->uvLst.Count() )
        {
            errorCode = ERROR_DATA_OUT_OF_RANGE;
            goto HANDLE_ERROR;
        }

        // Get the pointer to the uv class
        pUV = pGroup->uvLst.Get( index );

        // Copy over the uv's
        for( j = 0; j < 2; ++j )
        {
            pFace->crd[i].uv[j] = pUV->uv[j];
        }

        // Jump past the null
        while( strData[count++] )
        {}
    }

    // calculate the radius of the face
    pFace->Radius();

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
*    Input:    CGroupLst &objLst - Object list to receive allocation
*              char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CTrueSpaceParser::AddMateralNameToGroup( CGroupLst &groupLst, char *strData, int strCount )
{
    bool result = false, found = false;
    int count = 0, i;
    char *pPath;
    int length;

    // We are supposed to have at least one group in the list.
    // Make sure that we do
    if( groupLst.Count() == 0 )
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

    // Get the length of the string
    length = strlen( &strData[count] );

    // allocate a string
    pPath = new char[ length + 1 ];
      
    // check for allocations for errors
    if( pPath == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }
        
    // Copy the string
    strcpy( pPath, &strData[count] );

    // Add the string to the list
    groupLst.Get( groupLst.Count()-1 )->textLst.Add( pPath );    

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // AddMateralNameToGroup


/************************************************************************
*    DESCRIPTION:  Make a global vert list
*
*    Input:    CGroupLst &groupLst - Group list
*              CVertLst &vertLst  - Vertex list
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CTrueSpaceParser::MakeGlobalVertLst( CGroupLst &groupLst, CVertLst &vertLst )
{
    bool result = false;
    CVertLst *pVertLst;
    PCVertice pVert, pVertTmp;

    // Make the global vertex list
    for( int i = 0; i < groupLst.Count(); ++i )
    {
        pVertLst = &groupLst.Get( i )->vertLst;

        for( int j = 0; j < pVertLst->Count(); ++j )
        {
            // Allocate a vertice class
            pVertTmp = new CVertLoad;

            // check for allocations for errors
            if( pVertTmp == NULL )
            {
                errorCode = ERROR_ALLOCATION_FAIL;
                goto HANDLE_ERROR;
            }
            
            pVert = pVertLst->Get( j );

            memcpy( pVertTmp, pVert, sizeof(CVertLoad) );
            
            vertLst.Add( pVertTmp );
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
*    Input:    CGroupLst &groupLst - Group list
*              CFaceLst &faceLst - Face list
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CTrueSpaceParser::MakeGlobalFaceLst( CGroupLst &groupLst, CFaceLst &faceLst )
{
    bool result = false;
    CFaceLst *pFaceLst;
    PCFace pFace, pFaceTmp;

    // Make the global vertex list
    for( int i = 0; i < groupLst.Count(); ++i )
    {
        pFaceLst = &groupLst.Get( i )->faceLst;

        for( int j = 0; j < pFaceLst->Count(); ++j )
        {
            // Allocate a vertice class
            pFaceTmp = new CFace;

            // check for allocations for errors
            if( pFaceTmp == NULL )
            {
                errorCode = ERROR_ALLOCATION_FAIL;
                goto HANDLE_ERROR;
            }
            
            pFace = pFaceLst->Get( j );

            memcpy( pFaceTmp, pFace, sizeof(CFace) );
            
            faceLst.Add( pFaceTmp );
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
*    Input:    CGroupLst &groupLst - group list
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/  
bool CTrueSpaceParser::RemoveDupVerticies( CGroupLst &groupLst )
{
    bool result = false;
    int i, j, obj;
    PCVertice pVert1, pVert2;
    CVertLst *pVertLst;

    // We have to have a list
    if( groupLst.Count() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Remove the duplicates in each group
    for( obj = 0; obj < groupLst.Count(); ++obj )
    {
        pVertLst = &groupLst.Get( obj )->vertLst;

        // Cycle through the list and delete all the duplicates
        for( i = 0; i < pVertLst->Count(); ++i )
        {        
            // Get the vertex to check for duplicates
            pVert1 = (PCVertice)pVertLst->Get( i );

            for( j = i+1; j < pVertLst->Count(); ++j )
            {            
                // Get the vertex to check against
                pVert2 = (PCVertice)pVertLst->Get( j );

                // Do we have a match?
                if( pVert1->vert[0] == pVert2->vert[0] &&
                    pVert1->vert[1] == pVert2->vert[1] &&
                    pVert1->vert[2] == pVert2->vert[2] )
                {
                    // Delete the vertice at this index
                    pVertLst->Delete( j );

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
    int i, j;
    PCVertice pVert1, pVert2;

    if( vertLst.Count() == 0 )
    {
        errorCode = ERROR_DATA_OUT_OF_RANGE;
        goto HANDLE_ERROR;
    }

    // Cycle through the global list and delete all the duplicates
    for( i = 0; i < vertLst.Count(); ++i )
    {        
        // Get the vertex to check for duplicates
        pVert1 = vertLst.Get( i );

        for( j = i+1; j < vertLst.Count(); ++j )
        {            
            // Get the vertex to check against
            pVert2 = vertLst.Get( j );

            // Do we have a match?
            if( pVert1->vert[0] == pVert2->vert[0] &&
                pVert1->vert[1] == pVert2->vert[1] &&
                pVert1->vert[2] == pVert2->vert[2] )
            {
                // Delete the vertice at this index
                vertLst.Delete( j );

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
bool CTrueSpaceParser::GenerateNormals( char *fileName )
{
    CMesh tmpMsh;
    CMeshList<CMeshMap> tmpMshLst;
    bool result = false;
    bool matchFound;
    int msh, ply, i;

    // Change the extension to the original file path
    RelpaceFileExtension( NULL, fileName, ".ply" );

    // Load in the new polygon file
    if( !tmpMsh.LoadMeshFromFile( fileName ) )
    {
        errorCode = ERROR_CAL_NORMALS_FILE;
        goto HANDLE_ERROR;
    }

    // Change the extension to the original file path
    RelpaceFileExtension( NULL, fileName, ".map" );

    // Load in the new polygon file to generate all the normals
    if( !tmpMshLst.LoadMeshFromFile( fileName ) )
    {
        errorCode = ERROR_CAL_NORMALS_FILE;
        goto HANDLE_ERROR;
    }

    // generate all the normals
    tmpMsh.CalNormals();

    // Go through all the meshes and match up the faces with the big list
    // to copy the normal data. This way the vertex normal is consistant throught.
    for( msh = 0; msh < tmpMshLst.Count(); ++msh )
    {
        for( ply = 0; ply < tmpMshLst.GetMesh()[msh].GetPolyCount(); ++ply )
        {
            matchFound = false;

            for( i = 0; i < tmpMsh.GetPolyCount() && !matchFound; ++i )
            {
                if( tmpMshLst.GetMesh()[msh].GetPolyList()[ply] == tmpMsh.GetPolyList()[i] )
                {
                    tmpMshLst.GetMesh()[msh].GetPolyList()[ply].SetNormals( tmpMsh.GetPolyList()[i] );
                    matchFound = true;
                }
            }
        }

        // All polygons must be found
        if( matchFound == false )
        {
            errorCode = ERROR_SAVE_NORMALS_FILE;
            goto HANDLE_ERROR;
        }
    }

    // Change the extension to the original file path
    RelpaceFileExtension( NULL, fileName, ".ply" );

    // Save in the new polygon file with the 
    // mesh and normals in them
    if( !tmpMsh.SaveToFile( fileName ) )
    {
        errorCode = ERROR_SAVE_NORMALS_FILE;
        goto HANDLE_ERROR;
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

}   // GenerateNormals_Condenced


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
*    Input:    CGroupLst &groupLst  - Group list
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CTrueSpaceParser::MatchVertsToLocalList( CGroupLst &groupLst )
{
    bool result = false;
    int i, j, pt, v;
    PCVertice pVert;
    PCFace pFace;
    PCGroup pGroup;
    bool fMatchFound;

    // We have to have a list
    if( groupLst.Count() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Cycle through the group list
    for( i = 0; i < groupLst.Count(); ++i )
    {        
        // Get a pointer to the group
        pGroup = groupLst.Get( i );

        // Cycle through all the faces in the group
        for( j = 0; j < pGroup->faceLst.Count(); ++j )
        {
            // Get a pointer to the face in the group
            pFace = (PCFace)pGroup->faceLst.Get( j );

            // Check each point of this face
            for( pt = 0; pt < pFace->vertCount; ++pt )
            {
                fMatchFound = false;

                // Cycle through the vert list looking for a match
                for( v = 0; v < pGroup->vertLst.Count(); ++v )
                {
                    // Get the vertex to check against
                    pVert = pGroup->vertLst.Get( v );

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
*    Input:    CVertLst &vertLst  - Object list of vertices
*              CFaceLst &groupLst - Object list of groups
*
*    Output:   bool - true on success, false on error
*t
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CTrueSpaceParser::MatchVertsToGlobalList( CVertLst &vertLst, CFaceLst &faceLst )
{
    bool result = false;
    int i, j, pt;
    PCVertice pVert;
    PCFace pFace;
    bool fMatchFound;

    // We have to have a list
    if( vertLst.Count() == 0 || faceLst.Count() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Cycle through all the faces in the group
    for( i = 0; i < faceLst.Count(); ++i )
    {
        // Get a pointer to the face in the group
        pFace = faceLst.Get( i );

        // Check each point of this face
        for( pt = 0; pt < pFace->vertCount; ++pt )
        {
            fMatchFound = false;

            // Cycle through the vert list looking for a match
            for( j = 0; j < vertLst.Count(); ++j )
            {
                // Get the vertex to check against
                pVert = vertLst.Get( j );

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
*    Input:    CVertLst &vertLst - list of vertices
*              CFaceLst &faceLst - List of faces
*              char fileName    - Output file name of polygon file
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CTrueSpaceParser::SaveToFile_Condenced( CVertLst &vertLst, CFaceLst &faceLst, 
                                             char *fileName )
{
    bool result = false;
    FILE *hFile;
    int vCount, faceCount;
    int i, objType = 0, objID = 0;
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
    vCount = vertLst.Count();

    // get the number of faces
    faceCount = faceLst.Count();

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
    for( i = 0; i < vertLst.Count(); ++i )
    {
        if( fwrite( vertLst.Get( i )->vert, sizeof( CVertLoad ), 1, hFile ) != 1 )
        {
            errorCode = ERROR_FILE_WRITE;
            goto HANDLE_ERROR;
        }
    }

    // Save all the faces to the file
    for( i = 0; i < faceLst.Count(); ++i )
    {
        pFaceLoad = dynamic_cast<CFaceLoad *>(faceLst.Get( i ));

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

}   // SaveToFile_Condenced


/************************************************************************
*    DESCRIPTION:  Save polygon object data to file
*
*    Input:    CGroupLst &groupLst - Object list of groups
*              char *fileName    - Output file name of polygon
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CTrueSpaceParser::SaveToFile_Seperated( CGroupLst &groupLst, char *fileName )
{
    bool result = false;
    FILE *hFile;
    int vCount, faceCount, groupCount;
    int i, j, objType, objID, noTypeCount(0);
    int roomToDoor[MAX_ROOMS_PER_DOOR];
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
    for( i = 0; i < groupLst.Count(); ++i )
    {
        if( groupLst.Get(i)->objectType == EOT_NO_TYPE &&
            groupLst.Get(i)->mapData[0] > -1 )
        {
            ++noTypeCount;
        }
    }

    // Get the group count
    groupCount = groupLst.Count() - noTypeCount;

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

    for( i = 0; i < groupLst.Count(); ++i )
    {
        pGroup = groupLst.Get( i );

        // groups that are of no type and associated with a room
        // are not to be added to the map file.
        if( pGroup->objectType == EOT_NO_TYPE &&
            pGroup->mapData[0] > -1 )
        {
            continue;
        }

        CVertLst *pVertLst = &pGroup->vertLst;
        CFaceLst *pFaceLst = &pGroup->faceLst;

        // Get the object type
        objType = pGroup->objectType;

        // Get the object type
        objID = pGroup->objectID;
    
        // Get the number of vertices
        vCount = pVertLst->Count();

        // get the number of faces
        faceCount = pFaceLst->Count();

        // Save the misc data
        roomToDoor[0] = pGroup->mapData[0];
        roomToDoor[1] = pGroup->mapData[1];

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
        if( fwrite( roomToDoor, sizeof( roomToDoor ), 1, hFile ) != 1 )
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
        for( j = 0; j < pVertLst->Count(); ++j )
        {
            if( fwrite( pVertLst->Get( j )->vert, sizeof( CVertLoad ), 1, hFile ) != 1 )
            {
                errorCode = ERROR_FILE_WRITE;
                goto HANDLE_ERROR;
            }
        }

        // Write all the faces to the file
        for( j = 0; j < pFaceLst->Count(); ++j )
        {
            pFaceLoad = dynamic_cast<CFaceLoad *>(pFaceLst->Get( j ));

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

}   // SaveToFile_Seperated


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
    offset = faceLst.Count();

    // Make sure we have something to sort
    if( offset > 1 )
    {
        while( x < offset && inOrder == false )
        {
            ++x;
            inOrder = true;
            for( j = 0; j < offset - x; ++j )
            {
                pFace1 = faceLst.Get(j);
                pFace2 = faceLst.Get(j+1);
                
                // Sort in an assending order
                if( pFace1->tIndex > pFace2->tIndex )
                {
                    inOrder = false;
                    ptemp = pFace1;
                    faceLst.Set( j, pFace2 );
                    faceLst.Set( j+1, ptemp );
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
*    Input:    CObjList &groupLst  - Object group list
*              CObjList &textLst - Texture list
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CTrueSpaceParser::MatchTextureIndexToFace( CGroupLst &groupLst, CTextStrLst &textLst )
{
    bool result = false;
    int i, j, w;
    PCGroup pGroup;
    PCFace pFace;
    char *pGrpPath, *pTxtPath;
    bool fMatchFound;

    // Cycle through the group list
    for( i = 0; i < groupLst.Count(); ++i )
    {        
        // Get a pointer to the group
        pGroup = groupLst.Get( i );

        // Make sure this group has textures associated with it
        if( pGroup->textLst.Count() > 0 )
        {
            for( j = 0; j < pGroup->faceLst.Count(); ++j )
            {
                // Get the face
                pFace = pGroup->faceLst.Get( j );

                // Check that we are not out of range
                if( pFace->tIndex >= pGroup->textLst.Count() )
                {
                    errorCode = ERROR_DATA_OUT_OF_RANGE;
                    goto HANDLE_ERROR;
                }
                
                // Get the path to the group texture this face uses
                pGrpPath = pGroup->textLst.Get( pFace->tIndex );

                fMatchFound = false;

                // Cycle through the texture list looking for a match
                for( w = 0; w < textLst.Count() && !fMatchFound; ++w )
                {
                    // Get the texture path to check against
                    pTxtPath = textLst.Get( w );

                    // Do we have a match?
                    if( strcmp( pGrpPath, pTxtPath ) == 0 )
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
*    Input:    CTextureLib &textLib - Texture library class
*              CObjList &textLst - BMP file name list
*              char *filePath   - Path to where the texture library file
*                                 is to be saved
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CTrueSpaceParser::BuildTextureLibrary( CTextureLib &textLib, CTextStrLst &textLst, char *filePath )
{
    bool result = false;
    int i;

    /////////////////////////////////////////////////////
    //   Build a 32 bit texture map library
    /////////////////////////////////////////////////////

    // Tell the texture library how many textures we need
    if( !textLib.AllocateTextLibrary( textLst.Count() ) )
    {
        errorCode = ERROR_TEXT_LIB_ALLOC;
        goto HANDLE_ERROR;
    }

    // Now add each texture to the library
    for( i = 0; i < textLib.Count(); ++i )
    {
        // Save the file path in case of problem
        strcpy( errorFilePath, textLst.Get( i ) );

        // Add the texture to the library
        if( !textLib.AddTextureToLib( i, textLst.Get( i ), EBT_KEEP_BOTTOM_TO_SAME ) )
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
*    Input:    CObjList &groupLst - Object list of groups
*              CObjList &vLst     - Object list of vertices
*              CObjList &tLst     - Object list of texture names
*              CTextureLib &textLib - Texture library
*              char *filePath - file path to save object file
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  11/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CTrueSpaceParser::BuildMapFile( CGroupLst &groupLst, CTextStrLst &textLst, 
                               CTextureLib &textLib, char *filePath )
{
    FILE *hFile;
    int i, j, w, obj;
    int faceCount=0, vertCount=0, noTypeCount(0);
    PCVertice pVert;
    PCFace pFace;
    bool result = false;
    SCollisionType colType;
    PCGroup pGroup;
    SObjectType objTypeStruct;
    int textureDataSize(0); 

    for( int i = 0; i < groupLst.Count(); ++i )
    {
        // Add up the no type groups
        if( groupLst.Get(i)->objectType == EOT_NO_TYPE &&
            groupLst.Get(i)->mapData[0] > -1 )
        {
            ++noTypeCount;
        }
        else // Add up everything else
        {
            vertCount += groupLst.Get( i )->vertLst.Count();
            faceCount += groupLst.Get( i )->faceLst.Count();
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
    fprintf( hFile, "Group Count: %d\n", groupLst.Count() );
    fprintf( hFile, "No Type Group Count: %d\n", noTypeCount );
    fprintf( hFile, "Total Vertex Count: %d\n", vertCount );
    fprintf( hFile, "Total Face Count: %d\n", faceCount );

    for( obj = 0; obj < groupLst.Count(); ++obj )
    {
        pGroup = groupLst.Get( obj );

        CVertLst *pVertLst = &pGroup->vertLst;
        CFaceLst *pFaceLst = &pGroup->faceLst;

        fprintf( hFile, "\nGroup Type: %s\n", objTypeStruct.typeStr[ pGroup->objectType ] );
        fprintf( hFile, "Group Id: %d\n", pGroup->objectID );
        fprintf( hFile, "data: %d\n", pGroup->mapData[0] );
        fprintf( hFile, "data: %d\n\n", pGroup->mapData[1] );

        fprintf( hFile, "Vertex List:\n" );
        fprintf( hFile, "Vertex Count: %d\n\n", pVertLst->Count() );

        // List all the vertices
        for( i = 0; i < pVertLst->Count(); ++i )
        {
            // Get the vertex data
            pVert = (PCVertice)pVertLst->Get( i );

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

        // Face list
        fprintf( hFile, "\nFace List:\n" );
        fprintf( hFile, "Face Count: %d\n", pFaceLst->Count() );

        // Cycle through all the faces in the group
        for( j = 0; j < pFaceLst->Count(); ++j )
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
            fprintf( hFile, "      tIndex: %u, name: %s\n", pFace->tIndex, textLst.Get( pFace->tIndex ) );
        }
    }
    /*
    fprintf( hFile, "\nGlobal Vertex List:\n" );
    fprintf( hFile, "Global Vertex Count: %d\n\n", vLst.Count() );

    // Global vertex list
    for( i = 0; i < vLst.Count(); ++i )
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
    for( j = 0; j < fLst.Count(); ++j )
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

        for( w = 0; w < pFace->vertCount; ++w )
        {
            // Display the UV coordinate
            fprintf( hFile, "      UV: u:%f, v:%f\n", pFace->crd[w].uv[0], pFace->crd[w].uv[1] );
        }

        // Texture index
        fprintf( hFile, "      tIndex: %u, name: %s\n", pFace->tIndex, (char *)tLst.GetObj( pFace->tIndex ) );
    }
    */
    // texture map list
    fprintf( hFile, "\nTexture Map List:\n" );
    fprintf( hFile, "Texture count: %d\n", textLib.Count() );

    // Cycle through the texture map list
    for( i = 0; i < textLib.Count(); ++i )
    {
        // Get the name of the texture
        fprintf( hFile, "\n  %d: %s\n", i, textLst.Get( i ) );

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