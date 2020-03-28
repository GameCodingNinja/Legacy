
/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CMSBParser.cpp
*
*    DESCRIPTION:     Parse 3D OBJ files
*
*    IMPLEMENTATION:  Load in an MilkShape file and parse out the data.
*                     If there is a texture library to be made, there is a
*                     MLT file listed in the OBJ file.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

#include <string.h>          // string functions
#include "CMSBParser.h"      // Header file for this *.cpp file.
#include "CMeshBone.h"       // mesh bone class


// These are globals used by the 3D engine and must be setup
unsigned short int *gusiVBufer;
int giBufWidth, giBufHeight;
float gfBufWidthDiv2, gfBufHeightDiv2;
int *giZBuffer;
float gfMaxZDistance;
float gfMinZDistance;
unsigned short int *gusiShadeTbl;
float gfCameraScaleX;
float gfCameraScaleY;


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  08/20/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CMSBjParser::CMSBjParser()
{
    errorCode = NO_ERROR_DETECTED;
    parseState = EPM_NONE;

}   // Constructor


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/12/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CMSBjParser::~CMSBjParser()
{
}   // Destructor


/************************************************************************
*    FUNCTION NAME:         Decode_VertsFacesUVTextBones()                                                             
*
*    DESCRIPTION:           Decode Verts, Faces, UV, textures and bones.
*
*    FUNCTION PARAMETERS:
*    Input:    char *fileName - Path to load file
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/12/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::Decode_VertsFacesUVTextBones( char *filePath )
{
    bool result = false;
    char lineStr[350];
    int strCount, nullCount;
    FILE *hFile;

    // Reset the error file path
    errorFilePath[0] = '\0';

    // Save the file path to the first loaded file
    strcpy( fileName, filePath );

    // Open the file
    hFile = fopen( filePath, "rt" );
    if( hFile == NULL )
    {
        errorCode = ERROR_OBJ_FILE_HANDLE_FAIL;
        goto HANDLE_ERROR;
    }

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
        else if( strcmp( lineStr, "Bones:" ) == 0 )
        {
            parseState = EMP_BONES;
        }

        // Did we find a quote? It means different things 
        // durring different states
        else if( lineStr[0] == QUOTE_CHR )
        {
            if( parseState == EMP_MESH )
            {
                if( !Add_Group( groupLst, lineStr, strCount ) )
                    goto HANDLE_ERROR;
            }
            else if( parseState == EMP_MATERIALS )
            {
                // Do we have the file name of a bitmap
                if( lineStr[2] == '\\' )
                {
                    // Get the name of the bitmap
                    if( !Decode_TextureName( textLst, lineStr, strCount ) )
                        goto HANDLE_ERROR;
                }
            }
            else if( parseState == EMP_BONES )
            {
                if( !Add_Bone( hFile, boneLst, lineStr, strCount ) )
                    goto HANDLE_ERROR;
            }
        }
        // Did we find a number? It means different things 
        // durring different states
        else if( lineStr[0] >= 0x30 && lineStr[0] <= 0x39 )
        {
            // Decode the vertices, UV and bone index
            if( parseState == EMP_MESH && nullCount == VERT_NULL_COUNT )
            {
                if( groupLst.Count() == 0 )
                {
                    errorCode = ERROR_EMPTY_GROUP;
                    goto HANDLE_ERROR;
                }

                if( !Decode_VerticeUV( PCGroup(groupLst.GetObj( groupLst.Count()-1 ))->vertLst, 
                                       vertLst, lineStr, strCount ) )
                    goto HANDLE_ERROR;
            }

            // decode the triangle faces
            else if( parseState == EMP_MESH && nullCount == FACE_NULL_COUNT )
            {
                if( !Decode_Faces( PCGroup(groupLst.GetObj( groupLst.Count()-1 ))->vertLst,
                                   PCGroup(groupLst.GetObj( groupLst.Count()-1 ))->textureIndex, 
                                   faceLst, lineStr, strCount ) )
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
    if( !RemoveDupVerticies( vertLst ) )
        goto HANDLE_ERROR;

    // Match up the face vertices with the
    // trimmed down vertice list with the duplicate
    // vertices removed
    if( !MatchVerticies( vertLst, faceLst ) )
        goto HANDLE_ERROR;

    // Do we have a texture library file to parse?
    if( textLst.Count() )
    {
        if( !BuildTextureLibrary( textLib, textLst, fileName ) )
            goto HANDLE_ERROR;

        // Convert the non-specific coordinates to
        // coordinates specific to the library textures
        ConvertUVtoSpecificCoordinates( textLib, faceLst );
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
    {
        fclose( hFile );
    }
    
    return result;

}   // Decode_VertsFacesUVTextBones


/************************************************************************
*    FUNCTION NAME:         Decode_BonesAnimation()                                                             
*
*    DESCRIPTION:           Decode Verts, Faces, UV and textures
*                           has two files. The OBJ file refers to a
*                           MTL file for all the texture file information.
*
*    FUNCTION PARAMETERS:
*    Input:    char *fileName - Path to load file
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/12/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::Decode_BonesAnimation( char *filePath )
{
    bool result = false;
    char lineStr[350], animName[100];
    int strCount, nullCount;
    FILE *hFile;
    
    // Allocate a bone class
    PCAnim pAnim = new CAnim;

    // reset the state
    parseState = EPM_NONE;

    // check for allocations for errors
    if( pAnim == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // Pull the file name from the path
    CropFileNameFromPath( filePath, animName, false );

    // The name of the animation is the name of the file
    strcpy( pAnim->animName, animName );

    // Add the object to the list
    animLst.AddObj( pAnim );

    // Reset the error file path
    errorFilePath[0] = '\0';

    // Open the file
    hFile = fopen( filePath, "rt" );
    if( hFile == NULL )
    {
        errorCode = ERROR_OBJ_FILE_HANDLE_FAIL;
        goto HANDLE_ERROR;
    }

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
        else if( strcmp( lineStr, "Bones:" ) == 0 )
        {
            parseState = EMP_BONES;
        }
        // get misc info
        else if( strcmp( lineStr, "Frames:" ) == 0 )
        {
            if( !Decode_FrameCount( lineStr, strCount, pAnim->frameCount ) )
                goto HANDLE_ERROR;
        }
        else if( strcmp( lineStr, "Frame:" ) == 0 )
        {
            if( !Decode_CurrentFrame( lineStr, strCount, pAnim->currentFrame, pAnim->frameCount ) )
                goto HANDLE_ERROR;
        }

        // Did we find a quote? It means different things 
        // durring different states
        else if( lineStr[0] == QUOTE_CHR )
        {
            if( parseState == EMP_BONES )
            {
                if( !Add_BoneAnim( hFile, pAnim->boneAnimLst, lineStr, strCount ) )
                    goto HANDLE_ERROR;
            }
        }
        // Did we find a number? It means different things 
        // durring different states
        else if( lineStr[0] >= 0x30 && lineStr[0] <= 0x39 )
        {
            if( parseState == EMP_BONES && nullCount == BONE_OFFSET_NULL_COUNT )
            {
                if( !Decode_Animation( hFile, pAnim->boneAnimLst, lineStr, strCount ) )
                    goto HANDLE_ERROR;
            }
        }
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
    {
        fclose( hFile );
    }
    
    return result;

}   // Decode_BonesAnimation


/************************************************************************
*    FUNCTION NAME:         Decode_Animation()                                                             
*
*    DESCRIPTION:           Decode the animation
*
*    FUNCTION PARAMETERS:
*    Input:    char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::Decode_Animation( FILE *hFile, CObjList &boneAnimLst, char *strData, int strCount )
{
    bool result = false;
    int count = 0, tmpStrCount, nullCount, i, j, frames;
    char lineStr[350];

    // Get the most current bone
    PCBonesAnim pBoneAnim = (PCBonesAnim)boneAnimLst.GetObj( boneAnimLst.Count()-1 );

    // get the translations
    for( i = 0; i < 3; ++i )
    {
        // Jump past the null
        while( strData[count++] && count < strCount )
        {}

        pBoneAnim->intTran.vec[i] = atof( &strData[count] );
    }

    // get the rotations
    for( i = 0; i < 3; ++i )
    {
        // Jump past the null
        while( strData[count++] && count < strCount )
        {}

        pBoneAnim->intRot.vec[i] = atof( &strData[count] );
    }

    // Get the next line. It's the number of translations
    GetLine( hFile, lineStr, nullCount );

    // There are no NULLs here
    if( nullCount != 0 )
    {
        errorCode = ERROR_DATA_OUT_OF_RANGE;
        goto HANDLE_ERROR;
    }

    // How many frams we got?
    frames = atoi( lineStr );

    // Decode all the translations
    for( i = 0; i < frames; ++i )
    {
        count = 0;

        // Get the next line
        tmpStrCount = GetLine( hFile, lineStr, nullCount );

        if( nullCount != VECTOR_NULL_COUNT )
        {
            errorCode = ERROR_DATA_OUT_OF_RANGE;
            goto HANDLE_ERROR;
        }

        // Allocate a vector
        PCVector vector = new CVector;

        // Get the vector time
        vector->time = atof( &lineStr[count] );

        // get the translations
        for( j = 0; j < 3; ++j )
        {
            // Jump past the null
            while( lineStr[count++] && count < tmpStrCount )
            {}

            vector->vec[j] = atof( &lineStr[count] );
        }

        // Add it
        pBoneAnim->transLst.AddObj( vector );
    }

    // Get the next line. It's the number of rotations
    GetLine( hFile, lineStr, nullCount );

    // There are no NULLs here
    if( nullCount != 0 )
    {
        errorCode = ERROR_DATA_OUT_OF_RANGE;
        goto HANDLE_ERROR;
    }

    // How many frams we got?
    frames = atoi( lineStr );

    // Decode all the rotations
    for( i = 0; i < frames; ++i )
    {
        count = 0;

        // Get the next line
        tmpStrCount = GetLine( hFile, lineStr, nullCount );

        if( nullCount != VECTOR_NULL_COUNT )
        {
            errorCode = ERROR_DATA_OUT_OF_RANGE;
            goto HANDLE_ERROR;
        }

        // Allocate a vector
        PCVector vector = new CVector;

        // Get the vector time
        vector->time = atof( &lineStr[count] );

        // get the translations
        for( j = 0; j < 3; ++j )
        {
            // Jump past the null
            while( lineStr[count++] && count < tmpStrCount )
            {}

            vector->vec[j] = atof( &lineStr[count] );
        }

        // Add it
        pBoneAnim->rotLst.AddObj( vector );
    }

    result = true;

HANDLE_ERROR:;
    
    return result;

}   // Decode_Animation


/************************************************************************
*    FUNCTION NAME:         Decode_FrameCount()                                                             
*
*    DESCRIPTION:           Get the number of frames in this animation
*
*    FUNCTION PARAMETERS:
*    Input:    char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::Decode_FrameCount( char *strData, int strCount, int &frameCount )
{
    bool result = false;
    int count = 0;

    // Jump past the null
    while( strData[count++] && count < strCount )
    {}

    // Make sure we didn't over run our string array
    if( count < strCount )
    {
        // Decode the frame count
        frameCount = atoi( &strData[count] );

        if( frameCount <= 0 )
        {
            errorCode = ERROR_SAVE_NO_ANIMATION_FRAMES;;
            goto HANDLE_ERROR;
        }
    }
    else
    {
        errorCode = ERROR_SAVE_NO_ANIMATION_FRAMES;;
        goto HANDLE_ERROR;
    }

    result = true;

HANDLE_ERROR:;
    
    return result;

}   // Decode_FrameCount


/************************************************************************
*    FUNCTION NAME:         Decode_CurrentFrame()                                                             
*
*    DESCRIPTION:           Get the current frame.
*
*    FUNCTION PARAMETERS:
*    Input:    char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::Decode_CurrentFrame( char *strData, int strCount, int &currentFrame, int &frameCount )
{
    bool result = false;
    int count = 0;

    // Jump past the null
    while( strData[count++] && count < strCount )
    {}

    // Make sure we didn't over run our string array
    if( count < strCount )
    {
        // Decode the frame count
        currentFrame = atoi( &strData[count] );

        if( currentFrame < 0 || currentFrame > frameCount )
        {
            errorCode = ERROR_SAVE_INVALID_CURRENT_FRAME_NO;
            goto HANDLE_ERROR;
        }
    }
    else
    {
        errorCode = ERROR_SAVE_INVALID_CURRENT_FRAME_NO;;
        goto HANDLE_ERROR;
    }

    result = true;

HANDLE_ERROR:;
    
    return result;

}   // Decode_CurrentFrame


/************************************************************************
*    FUNCTION NAME:         GenerateNormalFile()                                                             
*
*    DESCRIPTION:           Read in the poly file and generate all the normals
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - File handle
*              char *Str - text read up to the new-line character
*
*    Output:   int - Number of characters read in
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/12/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMSBjParser::GenerateNormalFile()
{
    CMeshBone tmpBoneMsh;
    bool result = false;

    // Change the extension to the original file path
    RelpaceFileExtension( fileName, "ply" );

    // Load in the new polygon file to generate all the normals
    if( !tmpBoneMsh.LoadFromFile( fileName ) )
    {
        errorCode = ERROR_CAL_NORMALS_FILE;;
        goto HANDLE_ERROR;
    }

    // Change the extension to the normal file name
    RelpaceFileExtension( fileName, "nor" );

    // Load in the new polygon file to generate all the normals
    if( !tmpBoneMsh.SaveNormals( fileName ) )
    {
        errorCode = ERROR_SAVE_NORMALS_FILE;
        goto HANDLE_ERROR;
    }

	result = true;

HANDLE_ERROR:;
    
    return result;

}   // GenerateNormalFile


/************************************************************************
*    FUNCTION NAME:         GetLine()                                                             
*
*    DESCRIPTION:           Reads a line of text from a text file
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - File handle
*              char *Str - text read up to the new-line character
*              int &nullCount - number of NULL characters in this string
*
*    Output:   int - Number of characters read in
*
*    HISTORY:
*    Name          Date          Comment
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
*    FUNCTION NAME:         Decode_VerticeUV()                                                             
*
*    DESCRIPTION:           Decode the vertices, UV and bone index
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &vertLst - Object list to receive allocated vertices
*              char *strData     - Line of parsed characters
*              int strCount      - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::Decode_VerticeUV( CObjList &groupVertLst, CObjList &vertLst, char *strData, int strCount )
{
    bool result = false;
    int i, count = 0;

    // Allocate the class
    PCVertice pGroupVert = new CVertice;
    PCVertice pVert = new CVertice;

    // check for allocations for errors
    if( pVert == NULL || pGroupVert == NULL )
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
        pGroupVert->vert[i] = pVert->vert[i] = atof( &strData[count] );
    }

    // Swap the pos/neg sign of the z. This is format is a right hand
    // system and we need it to be a left hand system
    pVert->vert[2] = pGroupVert->vert[2] = -pVert->vert[2];

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
        pGroupVert->crd.uv[i] = atof( &strData[count] );

        // UV coordinates can't be a negative number or
        // greater then 1.0. This would cause us to try
        // to sample outside the scope of the texture.
        if( pGroupVert->crd.uv[i] < 0.0f )
            pGroupVert->crd.uv[i] = 0.0f;
        else if( pGroupVert->crd.uv[i] > 1.0f )
            pGroupVert->crd.uv[i] = 1.0f;

        pVert->crd.uv[i] = pGroupVert->crd.uv[i];
    }

    // Jump past the null
    while( strData[count++] && count < strCount )
    {}

    // Do a sanity check to ensure we are not reading junk
    if( strData[count] < 45 || strData[count] > 57 || count > strCount )
    {
        errorCode = ERROR_DATA_OUT_OF_RANGE;
        goto HANDLE_ERROR;
    }

    // Get the index to the bone
    pGroupVert->boneIndex = pVert->boneIndex = atoi( &strData[count] );

    // Add the object to our object list
    groupVertLst.AddObj( (PVOID) pGroupVert );
    vertLst.AddObj( (PVOID) pVert );

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
*    FUNCTION NAME:         Decode_Faces()                                                             
*
*    DESCRIPTION:           Decode the string of UV values
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &vertLst - Object list of vertices
*              int textureIndex  - index to texture
*              CObjList &faceLst - Object list of faces
*              char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::Decode_Faces( CObjList &vertLst, int textureIndex, CObjList &faceLst, char *strData, int strCount )
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

    // Save the texture index
    pFace->tIndex = textureIndex;

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
        pVert = (PCVertice)vertLst.GetObj( vIndex );

        // Make sure we have a vertex
        if( pVert == NULL )
        {
            errorCode = ERROR_DATA_OUT_OF_RANGE;
            goto HANDLE_ERROR;
        }

        // Get the verts for this face
        for( j = 0; j < 3; ++j )
            pFace->point[i].vert[j] = pVert->vert[j];

        // Get the uv for this face
        // Swap the first and last vertex index. This is format is a right hand
        // system and we need it to be a left hand system
        for( j = 0; j < 2; ++j )
        {
            pFace->crd[2-i].uv[j] = pVert->crd.uv[j];
        }
    }

    // Add the object in our object list
    faceLst.AddObj( (PVOID) pFace );

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
*    FUNCTION NAME:         Add_Group()                                                             
*
*    DESCRIPTION:           Add a new group to the list
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &groupLst - Group list object
*              char *strData    - Line of parsed characters
*              int strCount     - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::Add_Group( CObjList &groupLst, char *strData, int strCount )
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

    // Copy the name of the mesh
    strcpy( pGroup->groupName, strData );

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
    groupLst.AddObj( (PVOID) pGroup );

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
*    FUNCTION NAME:         Decode_TextureName()                                                             
*
*    DESCRIPTION:           Get the texture name and add it to the list.
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &textLst - Object list to receive allocation
*              char *strData - Line of parsed characters
*              int strCount  - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::Decode_TextureName( CObjList &textLst, char *strData, int strCount )
{
    bool result = false;
    int i, count = 0;

    // Allocate a face class
    PCTextLibrary pTxtLib = new CTextLibrary;

    // check for allocations for errors
    if( pTxtLib == NULL )
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
    
    // The format for the texture file name looks like this...
    // ".\filename.bmp"
    // It get's more complicated depending on where the bitmap
    // file is. To keep this code simple, we are just going
    // to assume all the textures are in the same directory
    // as the txt file
    for( i = strCount-1; i > 0; --i )
    {
        if( strData[i] == '\\' )
            break;
    }

    // terminate the last quote
    strData[strCount-1] = 0;

    // Copy over the material name
    strcpy( pTxtLib->txtFileName, &strData[i+1] );

    // Add it to the object
    textLst.AddObj( pTxtLib );

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // Decode_TextureName


/************************************************************************
*    FUNCTION NAME:         Add_Bone()                                                             
*
*    DESCRIPTION:           Add a new bone to the list.
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &textLst - Object list to receive allocation
*              char *strData - Line of parsed characters
*              int strCount  - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::Add_Bone( FILE *hFile, CObjList &boneLst, char *strData, int strCount )
{
    bool result = false;
    char lineStr[350];
    int tmpStrCount, nullCount, i;

    // Allocate a bone class
    PCBones pBone = new CBones;

    // check for allocations for errors
    if( pBone == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // Add back any spaces that was replaced with a null.
    for( i = 1; i < strCount; ++i )
    {
        if( strData[i] == '\0' )
            strData[i] = ' ';
    }

    // terminate the last quote
    strData[strCount-1] = 0;

    // Copy over the bone name adding one to jump over the first quote
    strcpy( pBone->boneName, &strData[1] );

    // the parent of this bone is the next line so get the next line
    tmpStrCount = GetLine( hFile, lineStr, nullCount );

    // Check for the root node. It will be a set of empty quotes
    if( tmpStrCount > 2 )
    {
        // Add back any spaces that was replaced with a null.
        for( i = 1; i < tmpStrCount; ++i )
        {
            if( lineStr[i] == '\0' )
                lineStr[i] = ' ';
        }

        // terminate the last quote
        lineStr[tmpStrCount-1] = 0;

        // Copy over the bone name adding one to jump over the first quote
        strcpy( pBone->parentName, &lineStr[1] );

        // Find the parent index to the parent of this bone
        for( i = 0; i < boneLst.Count(); ++i )
        {
            PCBones tmpBone = (PCBones)boneLst.GetObj( i );

            if( strcmp( pBone->parentName, tmpBone->boneName ) == 0 )
            {
                pBone->parentIndex = i;
                break;
            }
        }
    }

    // Add it to the object
    boneLst.AddObj( (PVOID)pBone );

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // Add_Bone


/************************************************************************
*    FUNCTION NAME:         RemoveDupVerticies()                                                             
*
*    DESCRIPTION:           Remove all the duplicate verticies from the
*                           vertice list
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &vertLst - Object list of vertices
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/14/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::RemoveDupVerticies( CObjList &vertLst )
{
    bool result = false;
    int i, j, offset, x=0;
    bool inOrder;
    PCVertice pVert1, pVert2, ptemp;

    // We have to have a list
    if( vertLst.Count() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Cycle through the list and delete all the duplicates
    for( i = 0; i < vertLst.Count(); ++i )
    {        
        // Get the vertex to check for duplicates
        pVert1 = (PCVertice)vertLst.GetObj( i );

        for( j = i+1; j < vertLst.Count(); ++j )
        {            
            // Get the vertex to check against
            pVert2 = (PCVertice)vertLst.GetObj( j );

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
    
    // Sort via bone index

    inOrder = false;
    offset = vertLst.Count();

    // Make sure we have something to sort
    if( offset > 1 )
    {
        while( x < offset && inOrder == false )
        {
            ++x;
            inOrder = true;
            for( j = 0; j < offset - x; ++j )
            {
                pVert1 = (PCVertice)vertLst.GetObj(j);
                pVert2 = (PCVertice)vertLst.GetObj(j+1);
                
                // Sort in an assending order
                if( pVert1->boneIndex > pVert2->boneIndex )
                {
                    inOrder = false;
                    ptemp = pVert1;
                    vertLst.SetObj( j, pVert2 );
                    vertLst.SetObj( j+1, ptemp );
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
*    FUNCTION NAME:         Add_BoneAnim()                                                             
*
*    DESCRIPTION:           Add bone animation.
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &textLst - Object list to receive allocation
*              char *strData - Line of parsed characters
*              int strCount  - Number of characters in strData
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::Add_BoneAnim( FILE *hFile, CObjList &boneAnimLst, char *strData, int strCount )
{
    bool result = false;
    char lineStr[350];
    int nullCount, i;

    // Allocate a bone class
    PCBonesAnim pBonesAnim = new CBonesAnim;

    // check for allocations for errors
    if( pBonesAnim == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // Add back any spaces that was replaced with a null.
    for( i = 1; i < strCount; ++i )
    {
        if( strData[i] == '\0' )
            strData[i] = ' ';
    }

    // terminate the last quote
    strData[strCount-1] = 0;

    // Copy over the bone name adding one to jump over the first quote
    strcpy( pBonesAnim->boneName, &strData[1] );

    // pull a line of test and burn it. It's the name of the
    // parent and we don'e need it now
    GetLine( hFile, lineStr, nullCount );

    // Find the index to the bone for future use
    for( i = 0; i < boneLst.Count(); ++i )
    {
        PCBones pBone = (PCBones)boneLst.GetObj( i );

        if( strcmp( pBonesAnim->boneName, pBone->boneName ) == 0 )
        {
            pBonesAnim->boneIndex = i;
            pBonesAnim->parentIndex = pBone->parentIndex;
            strcpy( pBonesAnim->parentName, pBone->parentName );
            break;
        }
    }

    // Something is wrong. We didn't find a bone
    if( pBonesAnim->boneIndex == -1 )
    {
        errorCode = ERROR_NO_BONE_INDEX_NOT_FOUND;
        goto HANDLE_ERROR;
    }

    // Add it to the object
    boneAnimLst.AddObj( pBonesAnim );

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // Add_BoneAnim


/************************************************************************
*    FUNCTION NAME:         MatchVerticies()                                                             
*
*    DESCRIPTION:           Match up all the vertices in the faces to
*                           the verstices in the list.
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &vertLst  - Object list of vertices
*              CObjList &faceLst  - Object list of faces
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/14/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::MatchVerticies( CObjList &vertLst, CObjList &faceLst )
{
    bool result = false;
    int i, pt, v;
    PCVertice pVert;
    PCFace pFace;
    bool fMatchFound;

    // We have to have a list
    if( faceLst.Count() == 0 || vertLst.Count() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Cycle through all the faces in the group
    for( i = 0; i < faceLst.Count(); ++i )
    {
        // Get a pointer to the face in the group
        pFace = (PCFace)faceLst.GetObj( i );

        // Check each point of this face
        for( pt = 0; pt < 3; ++pt )
        {
            fMatchFound = false;

            // Cycle through the vert list looking for a match
            for( v = 0; v < vertLst.Count(); ++v )
            {
                // Get the vertex to check against
                pVert = (PCVertice)vertLst.GetObj( v );

                // Do we have a match?
                if( pFace->point[pt].vert[0] == pVert->vert[0] &&
                    pFace->point[pt].vert[1] == pVert->vert[1] &&
                    pFace->point[pt].vert[2] == pVert->vert[2] )
                {
                    // Record the index into the vert list
                    pFace->vIndex[2-pt] = v;
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

}   // MatchVerticies


/************************************************************************
*    FUNCTION NAME:         SaveToFile()                                                             
*
*    DESCRIPTION:           Save polygon object data to file
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &objLst - Object list of vertices
*              CObjList &groupLst - Object list of groups
*              char fileName    - Output file name of polygon
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/15/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::SaveToFile()
{
    bool result = false;
    FILE *hFile;
    int vCount, triCount, animCount, boneCount, i;
    int faceDataSize, vertDataSize;
    PCFace pFace;

    // Change the extension to the original file path
    RelpaceFileExtension( fileName, "ply" );

    // Save the file path in case of problem
    strcpy( errorFilePath, fileName );

    // Create the file
    hFile = fopen ( fileName, "wb" );
    if( hFile == NULL )
    {
        errorCode = ERROR_PLY_FILE_HANDLE_FAIL;
        goto HANDLE_ERROR;
    }

    // Get the number of vertices and triangles
    vCount = vertLst.Count();
    triCount = faceLst.Count();
	boneCount = boneLst.Count();
	animCount = animLst.Count();

    // Write out the vertex count
    if( fwrite( &vCount, sizeof( vCount ), 1, hFile ) != 1 )
    {
        errorCode = ERROR_FILE_WRITE;
        goto HANDLE_ERROR;
    }

    // Write out the number of triangles
    if( fwrite( &triCount, sizeof( triCount ), 1, hFile ) != 1 )
    {
        errorCode = ERROR_FILE_WRITE;
        goto HANDLE_ERROR;
    }

    // Write out the number of bones
    if( fwrite( &boneCount, sizeof( boneCount ), 1, hFile ) != 1 )
    {
        errorCode = ERROR_FILE_WRITE;
        goto HANDLE_ERROR;
    }
    
    // Write out the number of bone animations
    if( fwrite( &animCount, sizeof( animCount ), 1, hFile ) != 1 )
    {
        errorCode = ERROR_FILE_WRITE;
        goto HANDLE_ERROR;
    }

    // 3 floats for the point and one int for the bone index
    vertDataSize = (sizeof( float ) * 3) + sizeof(int);

    // Write out the vertex data
    for( i = 0; i < vertLst.Count(); ++i )
    {
        if( fwrite( PCVertice( vertLst.GetObj( i ) )->vert, vertDataSize, 1, hFile ) != 1 )
        {
            errorCode = ERROR_FILE_WRITE;
            goto HANDLE_ERROR;
        }
    }

    // Compute the data size which is everything in the class but the vertices.
    faceDataSize = sizeof( CFace ) - (sizeof( CVertice ) * 3);

    // Save all the faces to the file
    for( i = 0; i < faceLst.Count(); ++i )
    {
        pFace = (PCFace)faceLst.GetObj( i );
        
        // Get a pointer to the face in the group
        if( fwrite( pFace, faceDataSize, 1, hFile ) != 1 )
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

}   // SaveToFile


/************************************************************************
*    FUNCTION NAME:         BuildTextureLibrary()                                                             
*
*    DESCRIPTION:           Build the texture library file
*
*    FUNCTION PARAMETERS:
*    Input:    CTextureLib &textLib - Texture library class
*              CObjList &noDupTxtLst - BMP file name list
*              char *filePath   - Path to where the texture library file
*                                 is to be saved
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::BuildTextureLibrary( CTextureLib &textLib, CObjList &textLst, char *filePath )
{
    char pathToBMP[500];
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
        // create the path to the BMP file
        CropOutPathAddFileName( filePath, pathToBMP, PCTextLibrary(textLst.GetObj( i ))->txtFileName );

        // Save the file path in case of problem
        strcpy( errorFilePath, pathToBMP );

        // Add the texture to the library
        if( !textLib.AddTextureToLib( i, pathToBMP, EBT_KEEP_BOTTOM_TO_SAME ) )
        {
            errorCode = ERROR_LOAD_TEXTURE;
            goto HANDLE_ERROR;
        }
    }

    // Replace the extension to the texture library file
    //RelpaceFileExtension( filePath, "b32" );
    
    // Save the library to file
    //textLib.SaveToFile_32( filePath );

    /////////////////////////////////////////////////////
    //   Build a 16 bit texture map library
    /////////////////////////////////////////////////////

    if( !textLib.Create16BitLibFrom32() )
    {
        errorCode = ERROR_CONVERT_TEXTLIB_TO_16;
        goto HANDLE_ERROR;
    }

    // Replace the extension to the texture library file
    RelpaceFileExtension( filePath, "b16" );
    
    // Save the library to file
    textLib.SaveToFile_16( filePath );

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // BuildTextureLibrary


/************************************************************************
*    FUNCTION NAME:         ConvertUVtoSpecificCoordinates()                                                             
*
*    DESCRIPTION:           Convert the non-specific coordinates to
*                           coordinates specific to the library textures
*
*    FUNCTION PARAMETERS:
*    Input:    CTextureLib &textLib - Texture library class
*              CObjList &groupLst  - face group list
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
void CMSBjParser::ConvertUVtoSpecificCoordinates( CTextureLib &textLib, CObjList &faceLst )
{
    // All the faces in this group use the same texture.
    // That's what defines a group, they all use the same material name
    for( int i = 0; i < faceLst.Count(); ++i )
    {
        // get the face in this group
        PCFace pFace = (PCFace)faceLst.GetObj( i );

        // Do the UV calculations depending on the texture width & height
        for( int j = 0; j < 3; ++j )
        {
            pFace->crd[j].uv[0] *= (textLib.pTMap[pFace->tIndex].width-1);
            pFace->crd[j].uv[1] *= (textLib.pTMap[pFace->tIndex].height-1);
        }
    }

}   // ConvertUVtoSpecificCoordinates


/************************************************************************
*    FUNCTION NAME:         Build_H_File()                                                             
*
*    DESCRIPTION:           Build a header file related to the image library.
*
*    FUNCTION PARAMETERS:
*    Input:    CTextureLib &noDupTxtLst - List of texture file names
*              CObjList &groupLst  - Object group list
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::Build_H_File()
{
    FILE *hFile; 
    bool result = false;
    char fileTmp[200], enumName[200];
    int i;

    char header[] = {"/*********************************************************\n* Bone Animation Specific Header File: %s\n*********************************************************/\n\n"};

    // Change the extension to the original file path
    RelpaceFileExtension( fileName, "h" );

    // Open the file
    hFile = fopen( fileName, "wt" );
    if( hFile == NULL )
    {
        errorCode = ERROR_H_FILE_HANDLE_FAIL;
        goto HANDLE_ERROR;
    }

    // Get the file name but keep the file extension
    CropFileNameFromPath( fileName, fileTmp );

    // Write the header file
    fprintf( hFile, header, fileTmp );

    // Get the file name but loose the file extension
    CropFileNameFromPath( fileName, fileTmp, false );

    // Replace all spaces with an underscore
    RelpaceChar( fileTmp, ' ', '_' );

    fprintf( hFile, "#ifndef __%s_H__\n", fileTmp );
    fprintf( hFile, "#define __%s_H__\n\n", fileTmp );

    fprintf( hFile, "enum EBA_%s\n{\n", fileTmp );

    for( i = 0; i < animLst.Count(); ++i )
    {
        // Get the file name but loose the file extension
        strcpy( enumName, PCAnim(animLst.GetObj( i ))->animName );

        // Replace all spaces with an underscore
        RelpaceChar( enumName, ' ', '_' );

        // Write the enum
        if( i == 0 )
            fprintf( hFile, "    EB_%s=0,\n", enumName );
        else
            fprintf( hFile, "    EB_%s,\n", enumName );
    }

    // Max Amount
    fprintf( hFile, "    EB_MAX_%s,\n", fileTmp );

    // Finish off the file
    fprintf( hFile, "};\n\n#endif\n" );

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );

    return result;

}   // Build_H_File


/************************************************************************
*    FUNCTION NAME:         RelpaceFileExtension                                                             
*
*    DESCRIPTION:           Strip the ".ext" of the end of a file path or
*                           file and replace with different extension
*
*    FUNCTION PARAMETERS:
*    Input:    char *filePath - file path
*              char *Ext - Extension string. 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  3/06/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMSBjParser::RelpaceFileExtension( char *filePath, char *Ext )
{
    int i, length;

    // Get the string length
    length = strlen( filePath );

    for( i = length; i > 0; --i )
    {
        if( filePath[i] == '.' )
        {
            filePath[i+1] = 0;
            break;
        }
    }

    // Add on the new file extension
    strcat( filePath, Ext );

}   // RelpaceFileExtension


/************************************************************************
*    FUNCTION NAME:         CropOutPathAddFileName                                                             
*
*    DESCRIPTION:           Strip the file name from the path and add
*                           new file name.
*
*    FUNCTION PARAMETERS:
*    Input:    char *sourcePath  - Source string
*              char *destPath    - Destination string
*              char *newFileName - new file name 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
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
*    FUNCTION NAME:         BuildMapFile()                                                             
*
*    DESCRIPTION:           Build a map file to show the data organization.
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &groupLst - Object list of groups
*              CObjList &vLst     - Object list of vertices
*              CObjList &tLst     - Object list of texture names
*              CTextureLib &textLib - Texture library
*              char *filePath - file path to save object file
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/14/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CMSBjParser::BuildMapFile()
{
    bool result = false;
    FILE *hFile;
    int i, j, w;
    PCVertice pVert;
    PCFace pFace;
    char filePath[400];

    // Create a local copy of the path to the first loaded file
    strcpy( filePath, fileName );

    // Change the extension to the original file path
    RelpaceFileExtension( filePath, "map" );

    // Open the file
    hFile = fopen( filePath, "wt" );
    if( hFile == NULL )
    {
        errorCode = ERROR_MAP_FILE_HANDLE_FAIL;
        goto HANDLE_ERROR;
    }    

    // File header
    fprintf( hFile, "Vertex List:\n" );
    fprintf( hFile, "Vertex Count: %d\n\n", vertLst.Count() );

    fprintf( hFile, "vertex: x, y, z,  bone index\n\n" );

    // List all the vertices
    for( i = 0; i < vertLst.Count(); ++i )
    {
        // Get the vertex data
        pVert = (PCVertice)vertLst.GetObj( i );

        if( i < 10 )
            fprintf( hFile, "%u:    %f, %f, %f,  %d\n", i, pVert->vert[0], pVert->vert[1], pVert->vert[2], pVert->boneIndex );
        else if( i < 100 )
            fprintf( hFile, "%u:   %f, %f, %f,  %d\n", i, pVert->vert[0], pVert->vert[1], pVert->vert[2], pVert->boneIndex );
        else if( i < 1000 )
            fprintf( hFile, "%u:  %f, %f, %f,  %d\n", i, pVert->vert[0], pVert->vert[1], pVert->vert[2], pVert->boneIndex );
        else if( i < 10000 )
            fprintf( hFile, "%u:  %f, %f, %f,  %d\n", i, pVert->vert[0], pVert->vert[1], pVert->vert[2], pVert->boneIndex );
        else
            fprintf( hFile, "%u: %f, %f, %f,  %d\n", i, pVert->vert[0], pVert->vert[1], pVert->vert[2], pVert->boneIndex );             
    }

    // Group/Triangle list
    fprintf( hFile, "\nFace List:\n" );
    fprintf( hFile, "Total Face Count: %d\n", faceLst.Count() );

    // Cycle through all the faces in the group
    for( i = 0; i < faceLst.Count(); ++i )
    {
        // Get a pointer to the face in the group
        pFace = (PCFace)faceLst.GetObj( i );

        // Face count
        fprintf( hFile, "    %d Face:\n", i+1 );

        // Display vertex index
        fprintf( hFile, "      vIndex: 1: %d, 2: %d, 3: %d\n", pFace->vIndex[0], 
                               pFace->vIndex[1], pFace->vIndex[2] );

        for( j = 0; j < 3; ++j )
        {
            // Display the UV coordinate
            fprintf( hFile, "      UV %d: u:%f, v:%f\n", j+1, pFace->crd[j].uv[0], pFace->crd[j].uv[1] );
        }

        // Texture index
        //fprintf( hFile, "      tIndex: %u, name: %s\n", pFace->tIndex, PCTextLibrary(textLst.GetObj( pFace->tIndex ))->txtFileName );
    }

    // texture map list
    fprintf( hFile, "\nTexture Map List:\n" );
    fprintf( hFile, "Texture Count: %d\n", textLib.Count() );

    // Cycle through the texture map list
    for( i = 0; i < textLib.Count(); ++i )
    {
        // Get the name of the texture
        fprintf( hFile, "\n  %d: %s\n", i, PCTextLibrary(textLst.GetObj( i ))->txtFileName );

        // Get the width and height of texture
        fprintf( hFile, "  width: %d, height: %d, shift: %u\n", textLib.pTMap[i].width, textLib.pTMap[i].height, textLib.pTMap[i].multiShift );
    }

    // Animation list
	fprintf( hFile, "\nBone Count: %d:\n", boneLst.Count() );
    fprintf( hFile, "\nAnimation List:\n" );
    fprintf( hFile, "Animation Count: %d\n", animLst.Count() );

    for( i = 0; i < animLst.Count(); ++i )
    {
        PCAnim pAnim = (PCAnim)animLst.GetObj(i);
        fprintf( hFile, "\n  Animation: %d\n", i+1 );
        fprintf( hFile, "    Animation Name: %s\n", pAnim->animName );
        fprintf( hFile, "    Frame Count: %d:\n", pAnim->frameCount );
        fprintf( hFile, "    Current Frame: %d:\n", pAnim->currentFrame );
        
        for( j = 0; j < pAnim->boneAnimLst.Count(); ++j )
        {
            PCBonesAnim pBoneAnim = (PCBonesAnim)pAnim->boneAnimLst.GetObj( j );
            fprintf( hFile, "\n      Bone Name: %s, Index: %d, Parent Name: %s, Parent Index: %d\n", 
                     pBoneAnim->boneName, pBoneAnim->boneIndex, pBoneAnim->parentName, pBoneAnim->parentIndex );
            fprintf( hFile, "        Bone Transition Offsets: x: %f, y: %f, z: %f\n", 
                     pBoneAnim->intTran.vec[0], pBoneAnim->intTran.vec[1], pBoneAnim->intTran.vec[2] );

            fprintf( hFile, "        Bone Rotation Offsets: x: %f, y: %f, z: %f\n", 
                     pBoneAnim->intRot.vec[0], pBoneAnim->intRot.vec[1], pBoneAnim->intRot.vec[2] );

            fprintf( hFile, "          Translation List Count: %d\n", pBoneAnim->transLst.Count() );

            // Show the time and translations
            for( w = 0; w < pBoneAnim->transLst.Count(); ++w )
            {
                PCVector pVec = (PCVector)pBoneAnim->transLst.GetObj( w );
                fprintf( hFile, "            time: %f, x: %f, y: %f, z: %f\n", pVec->time, pVec->vec[0], pVec->vec[1], pVec->vec[2] );
            }

            fprintf( hFile, "          Rotation List Count: %d\n", pBoneAnim->rotLst.Count() );

            // Show the time and rotations
            for( w = 0; w < pBoneAnim->rotLst.Count(); ++w )
            {
                PCVector pVec = (PCVector)pBoneAnim->rotLst.GetObj( w );
                fprintf( hFile, "            time: %f, x: %f, y: %f, z: %f\n", pVec->time, pVec->vec[0], pVec->vec[1], pVec->vec[2] );
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

}   // BuildMapFile


/************************************************************************
*    FUNCTION NAME:         CropFileNameFromPath                                                             
*
*    DESCRIPTION:           Strip the file name from the file path
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  3/06/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMSBjParser::CropFileNameFromPath( char *SourceFilePath, char *DestFileName, bool KeepExtension )
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
*    FUNCTION NAME:         RelpaceChar                                                             
*
*    DESCRIPTION:           Search a string and replace a character with
*                           a different character
*
*    FUNCTION PARAMETERS:
*    Input:    char *charString - string to search
*              char find - character to find
*              char replace - character to replace with found character 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  3/06/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMSBjParser::RelpaceChar( char *charString, char find, char replace )
{
    int i, length;

    // Get the string length
    length = strlen( charString );

    for( i = 0; i < length; ++i )
    {
        if( charString[i] == find )
            charString[i] = replace;
    }

}   // RelpaceChar