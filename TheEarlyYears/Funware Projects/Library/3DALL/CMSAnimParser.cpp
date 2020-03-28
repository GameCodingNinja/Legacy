
/************************************************************************
*
*    FILE NAME:       CMSAnimParser.cpp
*
*    DESCRIPTION:     Parse 3D ascii files in to an animation list
*
*    IMPLEMENTATION:  Load in an MilkShape file and parse out the data.
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

#include <string.h>          // string functions
#include "CMSAnimParser.h"   // Header file for this *.cpp file.
#include "minTools.h"        // min tools
#include "CAnimMeshLst.h"    // Header file for this *.cpp file

// These are globals used by the 3D engine and must be setup
float gfMaxZDistance = -1000;
float gfMinZDistance = -1.0f;
float gfAspectRatio;
int giRenderDevice;
float gfFrustrumYRatio;
float gfSquarePercentage;
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
*                  08/20/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
CMSBjParser::CMSBjParser()
{
    errorCode = NO_ERROR_DETECTED;
    parseState = EPM_NONE;
    animIndex = 0;
    frmIndex = 0;
    grpIndex = 0;

    // Don't have the global lists free data
    // because we are reusing pointers
    textStrLst.DontFreeData();

}   // Constructor


/************************************************************************
*    DESCRIPTION:  Destructer                                                             
*
*    -------------------------------------------------------------------
*                  09/12/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
CMSBjParser::~CMSBjParser()
{
}   // Destructor


/************************************************************************
*    DESCRIPTION:  Decode Verts, Faces, UV, textures and bones.
*
*    FUNCTION PARAMETERS:
*    Input:    char *fileName - Path to load file
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  9/12/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CMSBjParser::LoadFromFile( char *filePath )
{
    bool result = false;
    char lineStr[350];
    int strCount, nullCount;
    FILE *hFile;
    char animNameStr[100];
    PCAnim pAnim;
    PCFrame pFrame;

    // Reset the error file path
    errorFilePath[0] = '\0';

    // Set the parse state
    parseState = EPM_NONE;

    // Open the file
    hFile = fopen( filePath, "rt" );
    if( hFile == NULL )
    {
        errorCode = ERROR_OBJ_FILE_HANDLE_FAIL;
        goto HANDLE_ERROR;
    }

    // Build a file name using the name of the folder
    if( animLst.Count() == 0 )
    {
        char tmpFile[200];
        char fName[100];

        // Get the path
        CropOutPath( filePath, tmpFile );
        
        // add an extension to pretend it is a file name
        strcat( tmpFile, ".tmp" );

        // Now crop the name from the remaining path
        CropFileNameFromPath( tmpFile, fName );

        // Add the new name to the path we can change later
        CropOutPathAddFileName( filePath, fileName, fName );
    }

    // Get the animation name from the file name
    CropFileNameFromPath( filePath, animNameStr );
    RelpaceChar( animNameStr, '_', 0 );

    // Add a new animation to the list if the list is empty or the animation name is different
    if( animLst.Count() == 0 || strcmp( animLst.Get(animIndex)->animNameStr, animNameStr ) != 0 )
    {
        // Add a new animation it the list
        pAnim = new CAnim;
        if( pAnim == NULL )
        {
            errorCode = ERROR_ALLOCATION_FAIL;
            goto HANDLE_ERROR;
        }

        // Copy the name of the animation
        strcpy( pAnim->animNameStr, animNameStr );

        // Add the animation and update the index
        animLst.Add( pAnim );
        animIndex = animLst.Count() - 1;
    }

    // Add a new frame
    pFrame = new CFrame;
    if( pFrame == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // Add the animation and update the index
    animLst.Get(animIndex)->frameLst.Add( pFrame );
    frmIndex = animLst.Get(animIndex)->frameLst.Count() - 1;

    // Loop until we come to the end of the file
    while( 1 )
    {
        // Get a line of text
        strCount = GetLine( hFile, lineStr, nullCount );

        // Did we reach the end of the file?
        if( strCount == EOF )
            break;

        // Is there nothing to look at?
        else if( strCount == 0 )
            continue;

        // Set the states
        if( strcmp( lineStr, "Meshes:" ) == 0 )
        {
            parseState = EMP_MESH;
        }
        else if( strcmp( lineStr, "Materials:" ) == 0 )
        {
            parseState = EMP_MATERIALS;
        }

        // Did we find a quote? It means different things 
        // durring different states
        else if( lineStr[0] == QUOTE_CHR )
        {
            if( parseState == EMP_MESH )
            {
                if( !Add_Group( lineStr, strCount ) )
                    goto HANDLE_ERROR;
            }
            else if( parseState == EMP_MATERIALS )
            {
                // Do we have the file name of a bitmap
                if( lineStr[2] == '\\' || lineStr[2] == '.' )
                {
                    // Get the name of the bitmap
                    if( !Decode_TextureName( lineStr, strCount ) )
                        goto HANDLE_ERROR;
                }
            }
        }
        // Did we find a number? It means different things 
        // durring different states
        else if( lineStr[0] >= 0x30 && lineStr[0] <= 0x39 )
        {
            // Decode the vertices, UV and bone index
            if( parseState == EMP_MESH && nullCount == VERT_NULL_COUNT )
            {
                if( animLst.Get(animIndex)->frameLst.Get(frmIndex)->groupLst.Count() == 0 )
                {
                    errorCode = ERROR_EMPTY_GROUP;
                    goto HANDLE_ERROR;
                }

                if( !Decode_VerticeUV( lineStr, strCount ) )
                    goto HANDLE_ERROR;
            }

            // decode the triangle faces
            else if( parseState == EMP_MESH && nullCount == FACE_NULL_COUNT )
            {
                if( !Decode_Faces( lineStr, strCount ) )
                    goto HANDLE_ERROR;
            }
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

    // Remove the duplicate vertices
    if( !RemoveDupVerticies() )
        goto HANDLE_ERROR;

    // Match up the face vertices with the
    // trimmed down vertice list with the duplicate
    // vertices removed
    if( !MatchVerticies() )
        goto HANDLE_ERROR;

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
    {
        fclose( hFile );
    }
    
    return result;

}   // LoadFromFile


/************************************************************************
*    DESCRIPTION:  Add a new group to the list
*
*    Input:    char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/  
bool CMSBjParser::Add_Group( char *strData, int strCount )
{
    bool result = false;
    int count = 0;

    // Allocate a group class
    PCGroup pGroup = new CGroup;

    // check for allocations for errors
    if( pGroup == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // jump until we reach the second "
    while( strData[++count] != QUOTE_CHR && count < strCount )
    {}

    // Jump past the first null - Name of mesh
    while( strData[count++] && count < strCount )
    {}

    // Jump past the second null - some flag
    while( strData[count++] && count < strCount )
    {}

    // Make sure we didn't over run our string array
    if( count < strCount )
    {
        // Decode the texture index
        pGroup->textureIndex = atoi( &strData[count] );
    }

    // Add the object to our object list
    animLst.Get(animIndex)->frameLst.Get(frmIndex)->groupLst.Add( pGroup );
    grpIndex = animLst.Get(animIndex)->frameLst.Get(frmIndex)->groupLst.Count() - 1;

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

}   // Add_Group


/************************************************************************
*    DESCRIPTION:  Decode the vertices, UV and bone index
*
*    Input:    char *strData     - Line of parsed characters
*              int strCount      - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CMSBjParser::Decode_VerticeUV( char *strData, int strCount )
{
    bool result = false;
    int i, count = 0;

    // Allocate the class
    PCVertice pVert = new CVertice;

    // check for allocations for errors
    if( pVert == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // Decode each vertice
    for( i = 0; i < 3; ++i )
    {
        // Jump past the null
        while( strData[count++] && count < strCount )
        {}

        // Do a sanity check to ensure we are not reading junk
        if( strData[count] < 45 || strData[count] > 57 || count > strCount )
        {
            errorCode = ERROR_DATA_OUT_OF_RANGE;
            goto HANDLE_ERROR;
        }

        // Decode the three vertices
        pVert->vert[i] = atof( &strData[count] );
    }

    // Decode the uv
    for( i = 0; i < 2; ++i )
    {
        // Jump past the null
        while( strData[count++] && count < strCount )
        {}

        // Do a sanity check to ensure we are not reading junk
        if( strData[count] < 45 || strData[count] > 57 || count > strCount )
        {
            errorCode = ERROR_DATA_OUT_OF_RANGE;
            goto HANDLE_ERROR;
        }

        // Decode the two uv
        pVert->crd.uv[i] = atof( &strData[count] );

        // UV coordinates can't be a negative number or
        // greater then 1.0. This would cause us to try
        // to sample outside the scope of the texture.
        if( pVert->crd.uv[i] < 0.0f )
            pVert->crd.uv[i] = 0.0f;
        else if( pVert->crd.uv[i] > 1.0f )
            pVert->crd.uv[i] = 1.0f;
    }

    // Add the vert to our group list
    animLst.Get(animIndex)->frameLst.Get(frmIndex)->groupLst.Get(grpIndex)->vertLst.Add( pVert );

    // Add the vert to our global list
    animLst.Get(animIndex)->frameLst.Get(frmIndex)->gVertLst.Add( pVert );

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

}   // Decode_VerticeUV


/************************************************************************
*    DESCRIPTION:  Decode the string of UV values
*
*    Input:    char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CMSBjParser::Decode_Faces( char *strData, int strCount )
{
    bool result = false;
    int i, j, count = 0, vIndex;
    PCVertice pVert;

    // Allocate a face class
    PCFace pFace = new CFace;

    // check for allocations for errors
    if( pFace == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // Decode each face into the allocated class
    for( i = 0; i < 3; ++i )
    {
        // Jump past the null
        while( strData[count++] && count < strCount )
        {}

        // Do a sanity check to ensure we are not reading junk
        if( strData[count] < 0x30 || strData[count] > 0x39 )
        {
            errorCode = ERROR_DATA_OUT_OF_RANGE;
            goto HANDLE_ERROR;
        }

        // Get the index to triangles vertex
        vIndex = atoi( &strData[count] );

        // Get the vertex in question
        pVert = animLst.Get(animIndex)->frameLst.Get(frmIndex)->groupLst.Get(grpIndex)->vertLst.Get( vIndex );

        // Make sure we have a vertex
        if( pVert == NULL )
        {
            errorCode = ERROR_DATA_OUT_OF_RANGE;
            goto HANDLE_ERROR;
        }

        // Get the verts for this face
        for( j = 0; j < 3; ++j )
        {
            pFace->point[i].vert[j] = pVert->vert[j];
        }

        // Get the uv for this face
        // Swap the first and last vertex index. This is format is a right hand
        // system and we need it to be a left hand system
        for( j = 0; j < 2; ++j )
        {
            pFace->crd[2-i].uv[j] = pVert->crd.uv[j];
        }
    }

    // Add the object to our local face list
    animLst.Get(animIndex)->frameLst.Get(frmIndex)->groupLst.Get(grpIndex)->faceList.Add( pFace );

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

}   // Decode_Faces


/************************************************************************
*    DESCRIPTION:  Get the texture name and add it to the list.
*
*    Input:    char *strData - Line of parsed characters
*              int strCount  - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CMSBjParser::Decode_TextureName( char *strData, int strCount )
{
    bool result = false;
    int i, count = 0;
    bool found = false;

    // Allocate a string
    char *pStrPath = new char[100];

    // check for allocations for errors
    if( pStrPath == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // Add back any spaces that was replaced with a null.
    for( i = count; i < strCount; ++i )
    {
        if( strData[i] == '\0' )
            strData[i] = ' ';
    }

    // terminate the last quote
    strData[strCount-1] = 0;
    
    // The format for the texture file name looks like this...
    // ".\filename.bmp"
    // It get's more complicated depending on where the bitmap
    // file is. To keep this code simple, we are just going
    // to assume all the textures are in the same directory
    // as the txt file
    for( i = strCount-1; i > 0; --i )
    {
        if( strData[i] == '\\' )
        {
            // Copy over the material name
            strcpy( pStrPath, &strData[i+1] );

            break;
        }
    }

    // Save the name to the local list
    animLst.Get(animIndex)->frameLst.Get(frmIndex)->textStrLst.Add(pStrPath);

    // See if this one is already in there before trying to add it to the global list
    for( i = 0; i < textStrLst.Count(); ++i )
    {
        if( strcmp( pStrPath, textStrLst.Get(i) ) == 0 )
        {
            found = true;
            break;
        }
    }

    // Add the texture name to the global list
    if( found == false )
    {
        // Add it to the object
        textStrLst.Add( pStrPath );
    }   

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // Decode_TextureName


/************************************************************************
*    DESCRIPTION:   Remove all the duplicate verticies from the
*                   vertice list
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  9/14/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CMSBjParser::RemoveDupVerticies()
{
    bool result = false;
    int i, j;
    PCVertice pVert1, pVert2;

    // We have to have a list
    if( animLst.Get(animIndex)->frameLst.Get(frmIndex)->gVertLst.Count() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Cycle through the list and delete all the duplicates
    for( i = 0; i < animLst.Get(animIndex)->frameLst.Get(frmIndex)->gVertLst.Count(); ++i )
    {        
        // Get the vertex to check for duplicates
        pVert1 = animLst.Get(animIndex)->frameLst.Get(frmIndex)->gVertLst.Get( i );

        for( j = i+1; j < animLst.Get(animIndex)->frameLst.Get(frmIndex)->gVertLst.Count(); ++j )
        {            
            // Get the vertex to check against
            pVert2 = animLst.Get(animIndex)->frameLst.Get(frmIndex)->gVertLst.Get( j );

            // Do we have a match?
            if( pVert1->vert[0] == pVert2->vert[0] &&
                pVert1->vert[1] == pVert2->vert[1] &&
                pVert1->vert[2] == pVert2->vert[2] )
            {
                // Delete the vertice at this index
                animLst.Get(animIndex)->frameLst.Get(frmIndex)->gVertLst.Delete( j );

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
*    DESCRIPTION:  Match up all the vertices in the faces to
*                  the verstices in the list.
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  9/14/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CMSBjParser::MatchVerticies()
{
    bool result = false;
    int grp, fac, pt, v;
    PCVertice pVert;
    PCFace pFace;
    bool fMatchFound;

    // We have to have a global list of vertexes and a list of faces
    if( animLst.Get(animIndex)->frameLst.Get(frmIndex)->gVertLst.Count() == 0 || 
        animLst.Get(animIndex)->frameLst.Get(frmIndex)->groupLst.Count() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    for( grp = 0; grp < animLst.Get(animIndex)->frameLst.Get(frmIndex)->groupLst.Count(); ++grp )
    {
        // Cycle through all the faces in the group
        for( fac = 0; fac < animLst.Get(animIndex)->frameLst.Get(frmIndex)->groupLst.Get(grp)->faceList.Count(); ++fac )
        {
            // Get a pointer to the face in the group
            pFace = animLst.Get(animIndex)->frameLst.Get(frmIndex)->groupLst.Get(grp)->faceList.Get(fac);

            // Check each point of this face
            for( pt = 0; pt < 3; ++pt )
            {
                fMatchFound = false;

                // Cycle through the vert list looking for a match
                for( v = 0; v < animLst.Get(animIndex)->frameLst.Get(frmIndex)->gVertLst.Count(); ++v )
                {
                    // Get the vertex to check against
                    pVert = animLst.Get(animIndex)->frameLst.Get(frmIndex)->gVertLst.Get( v );

                    // Do we have a match?
                    if( pFace->point[pt].vert[0] == pVert->vert[0] &&
                        pFace->point[pt].vert[1] == pVert->vert[1] &&
                        pFace->point[pt].vert[2] == pVert->vert[2] )
                    {
                        // Record the index into the vert list
                        pFace->vIndex[2-pt] = (short int)v;
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
*    DESCRIPTION:  Match the group texture to the global texture
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  9/14/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::MatchGroupTextToGlobalText()
{
    int anim, frm, grp, txt, fac;
    int textIndex;
    bool found;

    for( anim = 0; anim < animLst.Count(); ++anim )
    {
        for( frm = 0; frm < animLst.Get(anim)->frameLst.Count(); ++frm )
        {
            for( grp = 0; grp < animLst.Get(anim)->frameLst.Get(frm)->groupLst.Count(); ++grp )
            {
                textIndex = animLst.Get(anim)->frameLst.Get(frm)->groupLst.Get(grp)->textureIndex;
                
                // Copy the name of the texture
                strcpy( animLst.Get(anim)->frameLst.Get(frm)->groupLst.Get(grp)->textStr,
                        animLst.Get(anim)->frameLst.Get(frm)->textStrLst.Get(textIndex) );
                        
                found = false;

                // find this texture in the global texture list
                for( txt = 0; txt < textStrLst.Count(); ++txt )
                {
                    if( strcmp( animLst.Get(anim)->frameLst.Get(frm)->groupLst.Get(grp)->textStr,
                                textStrLst.Get(txt) ) == 0 )
                    {
                        // Record the texture index of the global texture
                        animLst.Get(anim)->frameLst.Get(frm)->groupLst.Get(grp)->textureIndex = txt;

                        // Record the texture index for all the faces
                        for( fac = 0; fac < animLst.Get(anim)->frameLst.Get(frm)->groupLst.Get(grp)->faceList.Count(); ++fac )
                        {
                            animLst.Get(anim)->frameLst.Get(frm)->groupLst.Get(grp)->faceList.Get(fac)->tIndex = (unsigned char)txt;
                        }

                        found = true;
                        break;
                    }
                }
                
                // All textures have to match up
                if( found == false )
                {
                    errorCode = ERROR_MATCH_NOT_FOUND_TEXTURE;
                    return false;
                }   
            }
        }
    }

    return true;

}   // MatchGroupTextToGlobalText


/************************************************************************
*    DESCRIPTION:  Save polygon object data to file
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  9/15/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CMSBjParser::SaveToFile()
{
    bool result = false;
    FILE *hFile;
    int vCount, faceCount, animCount;
    int anim, frm, vert, grp, fac;
    char formatTypeMeshLst[FORMAT_SPEC_STR_COUNT];
    char formatTypeMeshRaw[FORMAT_SPEC_STR_COUNT];
    char formatTypeMeshAnim[FORMAT_SPEC_STR_COUNT];
    CAnimTable animTbl;
    int frameCount=0, dummy=0;
    CVertLoad *pVert;
    CAnimFaceLoad *pFace;

    // Setup the format id's
    memset( formatTypeMeshLst, 0, sizeof(formatTypeMeshLst) );
    memset( formatTypeMeshRaw, 0, sizeof(formatTypeMeshRaw) );
    memset( formatTypeMeshAnim, 0, sizeof(formatTypeMeshAnim) );
    strcpy( formatTypeMeshLst, MESH_LST_FORMAT_STR_ID );
    strcpy( formatTypeMeshRaw, MESH_RAW_FORMAT_STR_ID );
    strcpy( formatTypeMeshAnim, MESH_ANIM_TBL_FORMAT_STR_ID );

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

    // Write out the animated mesh format ID
    if( fwrite( formatTypeMeshAnim, sizeof( formatTypeMeshAnim ), 1, hFile ) != 1 )
    {
        errorCode = ERROR_FILE_WRITE;
        goto HANDLE_ERROR;
    }

    // get the number of animations
    animCount = animLst.Count();

    // Write out the vertex count
    if( fwrite( &animCount, sizeof( animCount ), 1, hFile ) != 1 )
    {
        errorCode = ERROR_FILE_WRITE;
        goto HANDLE_ERROR;
    }

    // Write out the animation table
    for( anim = 0; anim < animLst.Count(); ++anim )
    {
        memset( animTbl.animNameStr, 0, sizeof(animTbl.animNameStr) );
        strcpy( animTbl.animNameStr, animLst.Get(anim)->animNameStr );
        
        animTbl.frameStart = frameCount;
        animTbl.frameCount = animLst.Get(anim)->frameLst.Count();
        frameCount += animLst.Get(anim)->frameLst.Count();

        if( fwrite( &animTbl, sizeof( animTbl ), 1, hFile ) != 1 )
        {
            errorCode = ERROR_FILE_WRITE;
            goto HANDLE_ERROR;
        }
    }

    // Write out the mesh list format ID
    if( fwrite( formatTypeMeshLst, sizeof( formatTypeMeshLst ), 1, hFile ) != 1 )
    {
        errorCode = ERROR_FILE_WRITE;
        goto HANDLE_ERROR;
    }

    // Write out the total number of frames/meshes
    if( fwrite( &frameCount, sizeof( frameCount ), 1, hFile ) != 1 )
    {
        errorCode = ERROR_FILE_WRITE;
        goto HANDLE_ERROR;
    }

    // Write out the vertex and face data for all the frames/meshes
    for( anim = 0; anim < animLst.Count(); ++anim )
    {
        for( frm = 0; frm < animLst.Get(anim)->frameLst.Count(); ++frm )
        {
            // Write out the format ID
            if( fwrite( formatTypeMeshRaw, sizeof( formatTypeMeshRaw ), 1, hFile ) != 1 )
            {
                errorCode = ERROR_FILE_WRITE;
                goto HANDLE_ERROR;
            }
            
            // Write out the object type
            if( fwrite( &dummy, sizeof( dummy ), 1, hFile ) != 1 )
            {
                errorCode = ERROR_FILE_WRITE;
                goto HANDLE_ERROR;
            }

            // Write out the object ID
            if( fwrite( &dummy, sizeof( dummy ), 1, hFile ) != 1 )
            {
                errorCode = ERROR_FILE_WRITE;
                goto HANDLE_ERROR;
            }

            // Get the vert count
            vCount = animLst.Get(anim)->frameLst.Get(frm)->gVertLst.Count();

            faceCount = 0;

            for( grp = 0; grp < animLst.Get(anim)->frameLst.Get(frm)->groupLst.Count(); ++grp )
            {
                faceCount += animLst.Get(anim)->frameLst.Get(frm)->groupLst.Get(grp)->faceList.Count();
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
            for( vert = 0; vert < animLst.Get(anim)->frameLst.Get(frm)->gVertLst.Count(); ++vert )
            {
                // Get the vertex data
                pVert = animLst.Get(anim)->frameLst.Get(frm)->gVertLst.Get(vert);

                if( fwrite( pVert->vert, sizeof( CVertLoad ), 1, hFile ) != 1 )
                {
                    errorCode = ERROR_FILE_WRITE;
                    goto HANDLE_ERROR;
                }
            }

            // Write all the faces to the file
            for( grp = 0; grp < animLst.Get(anim)->frameLst.Get(frm)->groupLst.Count(); ++grp )
            {
                for( fac = 0; fac < animLst.Get(anim)->frameLst.Get(frm)->groupLst.Get(grp)->faceList.Count(); ++fac )
                {
                    pFace = animLst.Get(anim)->frameLst.Get(frm)->groupLst.Get(grp)->faceList.Get(fac);

                    if( fwrite( pFace, sizeof(CAnimFaceLoad), 1, hFile ) != 1 )
                    {
                        errorCode = ERROR_FILE_WRITE;
                        goto HANDLE_ERROR;
                    }
                }
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

}   // SaveToFile


/************************************************************************
*    DESCRIPTION:  Build the texture library file
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CMSBjParser::BuildTextureLibrary()
{
    bool result = false;
    int i;
    
    if( textStrLst.Count() )
    {
        // Tell the texture library how many textures we need
        if( !textLib.AllocateTextLibrary( textStrLst.Count() ) )
        {
            errorCode = ERROR_TEXT_LIB_ALLOC;
            goto HANDLE_ERROR;
        }

        // Now add each texture to the library
        for( i = 0; i < textLib.Count(); ++i )
        {
            // Save the file path in case of problem
            strcpy( errorFilePath, textStrLst.Get( i ) );

            // Add the texture to the library
            if( !textLib.AddTextureToLib( i, textStrLst.Get( i ), EBT_BOTTOM_TOP_FLIP ) )
            {
                errorCode = ERROR_LOAD_TEXTURE;
                goto HANDLE_ERROR;
            }
        }

        // Change the extension to the original file path
        RelpaceFileExtension( NULL, fileName, ".bt" );

        // Save the library to file
        textLib.SaveToFile( fileName );
    }
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
*    -------------------------------------------------------------------
*                  3/06/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void CMSBjParser::CropOutPathAddFileName( char *sourcePath, char *destPath, char *newFileName )
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
*    DESCRIPTION:  Read in the poly file and generate all the normals
*
*    Output:   int - Number of characters read in
*
*    -------------------------------------------------------------------
*                  9/12/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CMSBjParser::GenerateNormalFile()
{
    CAnimMeshLst<CAnimMesh> tmpAnimMshLst;
    bool result = false;

    // Change the extension to the original file path
    RelpaceFileExtension( NULL, fileName, ".ply" );

    // Load in the new polygon file to generate all the normals
    if( !tmpAnimMshLst.LoadMeshFromFile( fileName ) )
    {
        errorCode = ERROR_CAL_NORMALS_FILE;
        goto HANDLE_ERROR;
    }

    // calculate all the normals
    tmpAnimMshLst.CalNormals();

    // Save in the new polygon file with the 
    // mesh and normals in them
    if( !tmpAnimMshLst.SaveToFile( fileName ) )
    {
        errorCode = ERROR_SAVE_NORMALS_FILE;
        goto HANDLE_ERROR;
    }

    result = true;

HANDLE_ERROR:;
    
    return result;

}   // GenerateNormalFile


/************************************************************************
*    DESCRIPTION:  Reads a line of text from a text file
*
*    Input:    FILE *hFile - File handle
*              char *Str - text read up to the new-line character
*              int &nullCount - number of NULL characters in this string
*
*    Output:   int - Number of characters read in
*
*    -------------------------------------------------------------------
*                  9/12/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
int CMSBjParser::GetLine( FILE *hFile, char *str, int &nullCount )
{
    // Reads a line of text from a text file
    char nextChar;
    int index = 0;
    bool fComment = false;

    // Reads a line of text from a text file
    nextChar = (char)fgetc( hFile );

    // Clear the null count
    nullCount = 0;

    // Loop until we reach a new-line character
    while( nextChar != '\n' )
    {
        // Check for end of file:
        if( nextChar == EOF )
        {
            // If found, close off string
            // and return EOF:
            str[ index ] = '\0';
            return EOF;
        }

        // check to see if this line is a comment. If it is,
        // we need to make sure we don't Decode any info.
        if( nextChar == '/' )
            fComment = true;

        if( !fComment )
        {
            // Replace the space character with a null character
            if( nextChar == ' ' )
            {
               str[ index++ ] = '\0';
               ++nullCount;
            }
            // Record the text
            else if( nextChar != '\r' && nextChar != '\n' )
            {
               str[ index++ ] = nextChar;
            }
        }
            
        // Get the next character
        nextChar = (char)fgetc( hFile );
    }

    // Close off the string.
    if( index )
        str[ index ] = '\0';

    return index;

}   // GetLine


/************************************************************************
*    DESCRIPTION:  Build a map file to show the data organization.
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  9/14/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/   
bool CMSBjParser::BuildMapFile()
{
    bool result = false;
    FILE *hFile;
    int anim, frm, vert, grp, fac, txt, uv, count;
    int frameCount=0;
    PCVertice pVert;
    PCFace pFace;
    
    // Change the extension to the original file path
    RelpaceFileExtension( NULL, fileName, ".map" );

    // Open the file
    hFile = fopen( fileName, "wt" );
    if( hFile == NULL )
    {
        errorCode = ERROR_MAP_FILE_HANDLE_FAIL;
        goto HANDLE_ERROR;
    }
    
    fprintf( hFile, "Animation Count: %d\n\n", animLst.Count() );
    fprintf( hFile, "Animation Table:\n\n" );
    
    for( anim = 0; anim < animLst.Count(); ++anim )
    {
        fprintf( hFile, "  Animation: %s\n", animLst.Get(anim)->animNameStr );
        fprintf( hFile, "    Frame Count: %d,  %d to %d\n", animLst.Get(anim)->frameLst.Count(), frameCount,
                                                            frameCount + animLst.Get(anim)->frameLst.Count() - 1 );
        frameCount += animLst.Get(anim)->frameLst.Count();

        fprintf( hFile, "    Vertex Count: %d\n", animLst.Get(anim)->frameLst.Get(0)->gVertLst.Count() );

        count = 0;

        // Add up all the faces in the groups
        for( grp = 0; grp < animLst.Get(anim)->frameLst.Get(0)->groupLst.Count(); ++grp )
        {
            count += animLst.Get(anim)->frameLst.Get(0)->groupLst.Get(grp)->faceList.Count();
        }

        fprintf( hFile, "    Face Count: %d\n\n", count );
    }    

    // File header
    fprintf( hFile, "Vertex/Face List:\n" );
    fprintf( hFile, "vertex: x, y, z\n" );
    
    for( anim = 0; anim < animLst.Count(); ++anim )
    {
        fprintf( hFile, "\n  Animation: %s\n", animLst.Get(anim)->animNameStr );

        for( frm = 0; frm < animLst.Get(anim)->frameLst.Count(); ++frm )
        {
            fprintf( hFile, "\n    Frame: %d\n", frm + 1 );

            // List all the vertices
            for( vert = 0; vert < animLst.Get(anim)->frameLst.Get(frm)->gVertLst.Count(); ++vert )
            {
                // Get the vertex data
                pVert = animLst.Get(anim)->frameLst.Get(frm)->gVertLst.Get(vert);

                if( vert < 10 )
                    fprintf( hFile, "    %u:    %f, %f, %f\n", vert, pVert->vert[0], pVert->vert[1], pVert->vert[2] );
                else if( vert < 100 )
                    fprintf( hFile, "    %u:   %f, %f, %f\n", vert, pVert->vert[0], pVert->vert[1], pVert->vert[2] );
                else if( vert < 1000 )
                    fprintf( hFile, "    %u:  %f, %f, %f\n", vert, pVert->vert[0], pVert->vert[1], pVert->vert[2] );
                else if( vert < 10000 )
                    fprintf( hFile, "    %u:  %f, %f, %f\n", vert, pVert->vert[0], pVert->vert[1], pVert->vert[2] );
                else
                    fprintf( hFile, "    %u: %f, %f, %f\n", vert, pVert->vert[0], pVert->vert[1], pVert->vert[2] );             
            }

            count = 0;

            for( grp = 0; grp < animLst.Get(anim)->frameLst.Get(frm)->groupLst.Count(); ++grp )
            {
                count += animLst.Get(anim)->frameLst.Get(frm)->groupLst.Get(grp)->faceList.Count();
            }
            
            // Group/Triangle list
            fprintf( hFile, "\nFace List:\n" );
            fprintf( hFile, "Total Face Count: %d\n", count );

            // Cycle through all the faces in the group
            for( grp = 0; grp < animLst.Get(anim)->frameLst.Get(frm)->groupLst.Count(); ++grp )
            {
                // All the face in a group have the same texture index
                int textIndex = animLst.Get(anim)->frameLst.Get(frm)->groupLst.Get(grp)->textureIndex;
                
                // Texture index
                fprintf( hFile, "    Texture Index: %d; Texture Name: %s\n", textIndex, textStrLst.Get(textIndex) );

                for( fac = 0; fac < animLst.Get(anim)->frameLst.Get(frm)->groupLst.Get(grp)->faceList.Count(); ++fac )
                {
                    // Get a pointer to the face in the group
                    pFace = animLst.Get(anim)->frameLst.Get(frm)->groupLst.Get(grp)->faceList.Get(fac);

                    // Face count
                    fprintf( hFile, "    %d Face:\n", fac+1 );

                    // Display vertex index
                    fprintf( hFile, "      vIndex: 1: %d, 2: %d, 3: %d\n", pFace->vIndex[0], 
                                           pFace->vIndex[1], pFace->vIndex[2] );

                    for( uv = 0; uv < 3; ++uv )
                    {
                        // Display the UV coordinate
                        fprintf( hFile, "      UV %d: u:%f, v:%f\n", uv+1, pFace->crd[uv].uv[0], pFace->crd[uv].uv[1] );
                    }

                    // Texture index
                    //fprintf( hFile, "      tIndex: %u, name: %s\n", pFace->tIndex, PCTextLibrary(textLst.GetObj( pFace->tIndex ))->txtFileName );
                }
            }
        } 
    }

    // texture map list
    fprintf( hFile, "\nTexture Map List:\n" );
    fprintf( hFile, "Texture Count: %d\n\n", textStrLst.Count() );

    // Cycle through the texture map list
    for( txt = 0; txt < textStrLst.Count(); ++txt )
    {
        // Get the name of the texture
        fprintf( hFile, "  %d: %s\n", txt, textStrLst.Get( txt ) );

        // Get the width and height of texture
        //fprintf( hFile, "  width: %d, height: %d, shift: %u\n", textLib.pTMap[i].width, textLib.pTMap[i].height, textLib.pTMap[i].multiShift );
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );

    return result;

}   // BuildMapFile


/************************************************************************
*    DESCRIPTION:  Build a map file to show the data organization.
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  9/14/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::DoFinalErrorChecks()
{
    int anim, frm, grp, count, faceCount;

    // A character animation should only have a few texture
    // at the most. The max it 256.
    if( textStrLst.Count() > MAX_POSSIBLE_TEXTURES )
    {
        errorCode = ERROR_TOO_MANY_TEXTURES;
        return false;
    }

    // A character animation shouldn't have a lot of vertices
    for( anim = 0; anim < animLst.Count(); ++anim )
    {
        for( frm = 0; frm < animLst.Get(anim)->frameLst.Count(); ++frm )
        {
            if( animLst.Get(anim)->frameLst.Get(frm)->gVertLst.Count() > MAX_POSSIBLE_VERTICES )
            {
                errorCode = ERROR_TOO_MANY_VERTEXES;
                return false;
            }
        }
    }

    // The number of verts shouldn't change within the frames of an animation
    for( anim = 0; anim < animLst.Count(); ++anim )
    {
        // Get the number of global verts in the first frame
        count = animLst.Get(anim)->frameLst.Get(0)->gVertLst.Count();

        for( frm = 1; frm < animLst.Get(anim)->frameLst.Count(); ++frm )
        {
            if( animLst.Get(anim)->frameLst.Get(frm)->gVertLst.Count() != count )
            {
                errorCode = ERROR_FRAME_VERTEX_COUNT_MISMATCH;
                return false;
            }
        }
    }

    // The number of faces shouldn't change within the frames of an animation
    for( anim = 0; anim < animLst.Count(); ++anim )
    {
        faceCount = 0;

        // Add up the faces in the first frame to have a number to compare with the others
        for( grp = 0; grp < animLst.Get(anim)->frameLst.Get(0)->groupLst.Count(); ++grp )
        {
            faceCount += animLst.Get(anim)->frameLst.Get(0)->groupLst.Get(grp)->faceList.Count();
        }
        
        // Go through the remaining frames and add up all the faces
        for( frm = 1; frm < animLst.Get(anim)->frameLst.Count(); ++frm )
        {
            count = 0;
            for( grp = 0; grp < animLst.Get(anim)->frameLst.Get(frm)->groupLst.Count(); ++grp )
            {
                count += animLst.Get(anim)->frameLst.Get(frm)->groupLst.Get(grp)->faceList.Count();
            }
                
            if( faceCount != count )
            {
                errorCode = ERROR_FRAME_FACE_COUNT_MISMATCH;
                return false;
            }
        }
    }

    return true;

}   // DoFinalErrorChecks