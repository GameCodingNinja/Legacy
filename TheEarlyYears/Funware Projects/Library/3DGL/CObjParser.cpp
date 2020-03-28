
/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CObjParser.cpp
*
*    DESCRIPTION:     Parse 3D OBJ files
*
*    IMPLEMENTATION:  Load in an WaveFront OBJ file and parse out the data.
*                     If there is a texture library to be made, there is a
*                     MLT file listed in the OBJ file.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

#include <string.h>                // string functions
#include "..\3dgl\CObjParser.h"    // Header file for this *.cpp file.
#include "CMeshTri.h"              // Opengl mesh class

// These are globals used by the 3D engine and must be setup
float gfMaxZDistance = -1000;
float gfMinZDistance = -1.0f;


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

CObjParser::CObjParser()
{
    errorCode = NO_ERROR_DETECTED;

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

CObjParser::~CObjParser()
{
}   // Destructor


/************************************************************************
*    FUNCTION NAME:         LoadFromFile()                                                             
*
*    DESCRIPTION:           Load OBJ text file and parse it. This export
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
   
bool CObjParser::LoadFromFile( char *filePath )
{
    bool result = false;
    char lineStr[150], txtFileName[150] = "", fileName[400];
    int strCount;
    FILE *hFile;
    CObjList vertLst, uvLst, groupLst, textLst, noDupTxtLst;
    CTextureLib textLib;

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

        /////////////////////////////////////////////////////
        // The first string decides what the data is.
        //
        // v      = vertice
        // vt     = texture UV coordinate
        // vn     = face normal (we computer our own)
        // f      = face index into the above lists
        // usemtl = materal name in obj file
        // mtllib = file name of texture library
        /////////////////////////////////////////////////////

        // Do we have a vertice?
        if( strcmp( lineStr, "v" ) == 0 )
        {
            if( !Decode_Vertice( vertLst, lineStr, strCount ) )
                goto HANDLE_ERROR;
        }
        // Do we have a texture coordinate?
        else if( strcmp( lineStr, "vt" ) == 0 )
        {
            if( !Decode_UV( uvLst, lineStr, strCount ) )
                goto HANDLE_ERROR;
        }
        // Do we have the texture library file name
        else if( strcmp( lineStr, "mtllib" ) == 0 )
        {
            // get the name of the texture library file
            Decode_Name( txtFileName, lineStr, strCount );
        }
        // Do we have a group name
        else if( strcmp( lineStr, "g" ) == 0 )
        {
            // add a new group
            if( !AddGroup( groupLst, lineStr, strCount ) )
                goto HANDLE_ERROR;
        }
        // Do we have a material name
        else if( strcmp( lineStr, "usemtl" ) == 0 )
        {
            // add a new group
            if( !AddMateralNameToGroup( groupLst, lineStr, strCount ) )
                goto HANDLE_ERROR;
        }
        // Do we have a face to Decode for the active group?
        else if( strcmp( lineStr, "f" ) == 0 )
        {
            if( !AddFaceToGroup( groupLst, vertLst, uvLst, lineStr, strCount ) )
                goto HANDLE_ERROR;
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
    if( !MatchVerticies( vertLst, groupLst ) )
        goto HANDLE_ERROR;

    // Do we have a texture library file to parse?
    if( txtFileName[0] )
    {
        // Open the texture library file
        if( !ParseTextLibFile( textLst, fileName, txtFileName ) )
            goto HANDLE_ERROR;

        // There's a problem if our object list is empty
        if( textLst.Count() == 0 )
            goto HANDLE_ERROR;

        // Flag any duplicate BMP files used in multiple materials
        // Build a list of non-duplicate file names
        if( !FlagDuplicateTexturess( textLst, noDupTxtLst ) )
            goto HANDLE_ERROR;

        // Match up the texture library indexes to the faces
        if( !MatchTextureIndexToFace( groupLst, textLst, noDupTxtLst ) )
            goto HANDLE_ERROR;

        if( !BuildTextureLibrary( textLib, noDupTxtLst, fileName ) )
            goto HANDLE_ERROR;

        // Convert the non-specific coordinates to
        // coordinates specific to the library textures
        // we don't do that for this version
        //ConvertUVtoSpecificCoordinates( textLib, groupLst );

        if( !Build_H_File( noDupTxtLst, fileName ) )
            goto HANDLE_ERROR;
    }

    // Change the extension to the original file path
    RelpaceFileExtension( fileName, "ply" );

    // Save the polygon object file
    if( !SaveToFile( vertLst, groupLst, fileName ) )
        goto HANDLE_ERROR;

    // Build a map file to show all that we have done
    if( !BuildMapFile( groupLst, vertLst, textLst, textLib, noDupTxtLst, fileName ) )
        goto HANDLE_ERROR;

    // Generate normal file
    GenerateNormalFile( fileName );
    
    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );
    
    return result;

}   // LoadFromFile


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

bool CObjParser::GenerateNormalFile( char *fileName )
{
    CMeshTri tmpMsh;
    bool result = false;
    FILE *hFile = NULL;

    // Change the extension to the original file path
    RelpaceFileExtension( fileName, "ply" );

    // Load in the new polygon file to generate all the normals
    if( !tmpMsh.LoadFromFile( fileName ) )
    {
        errorCode = ERROR_CAL_NORMALS_FILE;;
        goto HANDLE_ERROR;
    }

    // Change the extension to the normal file name
    RelpaceFileExtension( fileName, "nor" );

    // Load in the new polygon file to generate all the normals
    if( !tmpMsh.SaveNormals( fileName ) )
    {
        errorCode = ERROR_SAVE_NORMALS_FILE;;
        goto HANDLE_ERROR;
    }

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );
    
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

int CObjParser::GetLine( FILE *hFile, char *str )
{
    // Reads a line of text from a text file
    char nextChar;
    int index = 0;
    bool fComment = false;

    // Reads a line of text from a text file
    nextChar = (char)fgetc( hFile );

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
        if( nextChar == '#' )
            fComment = true;

        if( !fComment )
        {
            // Record the text
            if( nextChar == ' ' )
               str[ index++ ] = '\0';
            else if( nextChar != '\r' && nextChar != '\n' )
               str[ index++ ] = nextChar;
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
*    FUNCTION NAME:         Decode_Name()                                                             
*
*    DESCRIPTION:           Get the name. Could be a file name, object
*                           name or material name. They are all handled
*                           the same.
*
*    FUNCTION PARAMETERS:
*    Input:    char *strName - String to fill with parsed name
*              char *strData - Line of parsed characters
*              int strCount  - Number of characters in strData
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
void CObjParser::Decode_Name( char *strName, char *strData, int strCount )
{
    int i, count = 0;

    // Jump past the first null
    while( strData[count++] )
    {}
    
    // Add back the spaces that was replaced with a null.
    // This could be a file name or other type of name that uses spaces.
    for( i = count; i < strCount; ++i )
    {
        if( strData[i] == '\0' )
            strData[i] = ' ';
    }

    // copy the name over
    strcpy( strName, &strData[count] );

}   // Decode_Name


/************************************************************************
*    FUNCTION NAME:         Decode_Vertice()                                                             
*
*    DESCRIPTION:           Decode the string of vertices
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &objLst - Object list to receive allocation
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
   
bool CObjParser::Decode_Vertice( CObjList &objLst, char *strData, int strCount )
{
    bool result = false;
    int i, count = 0;

    // Allocate a vertice class
    PCVertice pVert = new CVertice;

    // check for allocations for errors
    if( pVert == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // Decode each vertice into the allocated class
    for( i = 0; i < 3; ++i )
    {
        // Jump past the null
        while( strData[count++] )
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

    // Add the object to our object list
    objLst.AddObj( (PVOID) pVert );

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

}   // Decode_Vertice


/************************************************************************
*    FUNCTION NAME:         Decode_UV()                                                             
*
*    DESCRIPTION:           Decode the string of UV values
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &objLst - Object list to receive allocation
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
   
bool CObjParser::Decode_UV( CObjList &objLst, char *strData, int strCount )
{
    bool result = false;
    int i, count = 0;

    // Allocate a vertice class
    PCUV pUV = new CUV;

    // check for allocations for errors
    if( pUV == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // Decode each UV into the allocated class
    for( i = 0; i < 2; ++i )
    {
        // Jump past the null
        while( strData[count++] )
        {}

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
    }

    // Add the object in our object list
    objLst.AddObj( (PVOID) pUV );

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
*    FUNCTION NAME:         AddGroup()                                                             
*
*    DESCRIPTION:           Add a new group to the list
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
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CObjParser::AddGroup( CObjList &objLst, char *strData, int strCount )
{
    bool result = false;

    // Allocate a group class
    PCGroup pGroup = new CGroup;

    // check for allocations for errors
    if( pGroup == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // Get the name of the material
    Decode_Name( pGroup->groupName, strData, strCount );

    // Add the object to our object list
    objLst.AddObj( (PVOID) pGroup );

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
*    FUNCTION NAME:         AddMateralNameToGroup()                                                             
*
*    DESCRIPTION:           Add the material name to the group
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
*                  9/13/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CObjParser::AddMateralNameToGroup( CObjList &groupLst, char *strData, int strCount )
{
    bool result = false;
    PCGroup tmpGroup;

    // We are supposed to have at least one group in the list.
    // Make sure that we do
    if( groupLst.Count() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Get a pointer to the most reciently added group
    tmpGroup = (PCGroup)groupLst.GetObj( groupLst.Count()-1 );

    // Get the name of the material
    Decode_Name( tmpGroup->materialName, strData, strCount );

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // AddMateralNameToGroup


/************************************************************************
*    FUNCTION NAME:         AddFaceToGroup()                                                             
*
*    DESCRIPTION:           Setup the group list with the vertices and uv
*                           data.
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
*                  9/14/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CObjParser::AddFaceToGroup( CObjList &groupLst, CObjList &vLst, CObjList &uvLst, char *strData, int strCount )
{
    bool result = false;
    PCGroup tmpGroup;
    PCVertice pTmpVert;
    PCUV pTmpUV;
    PCFace pFace;
    int vertIndex[3] = {-1,-1,-1};
    int uvIndex[3] = {-1,-1,-1};
    int count = 0, index, i, j;
    char sample[20];
    bool noUVinfo = false;
    int testCount = 0;

    // Check the null count. This format can also be used for quads.
    for( i = 0; i < strCount; ++i )
    {
        if( strData[i] == '\0' )
            ++testCount;
    }

    // If we have more or less then 3 nulls, there is a problem.
    if( testCount != 3 )
    {
        errorCode = ERROR_DATA_OUT_OF_RANGE;
        goto HANDLE_ERROR;
    }

    // Jump past the first null
    while( strData[count++] )
    {}

    for( i = 0; i < 3; ++i )
    {
        index = 0;
        
        // Get the first character from the text string
        sample[index++] = strData[count++];

        // Loop until we reach a delimiter: / or NULL
        while( strData[count] != '/' && strData[count] != '\0' )
        {            
            // Get the next character
            sample[index++] = strData[count++];
        }

        // Close off the string.
        sample[index] = '\0';

        // Save the vertice index
        // Swap the first and last vertex index. This is format is a right hand
        // system and we need it to be a left hand system
        vertIndex[2-i] = (atoi( sample ) - 1);

        // if the delimiter is a null, there is no UV info
        if( strData[count] == '\0' )
        {
            ++count;
            noUVinfo = true;
            continue;
        }

        // If the next character is also a / then there is no UV data.
        // just speed past to the next null
        if( strData[++count] == '/' )
        {
            while( strData[count++] )
                {}

            noUVinfo = true;
            continue;
        }

        index = 0;
        
        // Get the first character from the text string
        sample[index++] = strData[count++];

        // Loop until we reach a delimiter: / or NULL
        while( strData[count] != '/' && strData[count] != '\0' )
        {            
            // Get the next character
            sample[index++] = strData[count++];
        }

        // Close off the string.
        sample[index] = '\0';

        // Save the vertice index
        uvIndex[2-i] = (atoi( sample ) - 1);

        // No need to search for a null on the last pass
        if( i < 2 )
        {
            // Nothing left so speed to the next null
            while( strData[count++] )
            {}
        }
    }

    ///////////////////////////////////////////////////
    // Make sure none of our indexes are less then zero
    // or greater then the counts in each list. 
    // That would be bad.
    ///////////////////////////////////////////////////
    if( vertIndex[0] < 0 || vertIndex[1] < 0 || vertIndex[2] < 0 ||
        vertIndex[0] >= vLst.Count() || vertIndex[1] >= vLst.Count() || vertIndex[2] >= vLst.Count() )
    {
        errorCode = ERROR_DATA_OUT_OF_RANGE;
        goto HANDLE_ERROR;
    }

    // We may not have UV info in this OBJ file
    if( !noUVinfo )
    {
        if( uvIndex[0] < 0 || uvIndex[1] < 0 || uvIndex[2] < 0 ||
            uvIndex[0] >= uvLst.Count() || uvIndex[1] >= uvLst.Count() || uvIndex[2] >= uvLst.Count() )
        {
            errorCode = ERROR_DATA_OUT_OF_RANGE;
            goto HANDLE_ERROR;
        }
    }

    // We are supposed to have at least one group in the list.
    // Make sure that we do
    if( groupLst.Count() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Get a pointer to the most reciently added group
    tmpGroup = (PCGroup)groupLst.GetObj( groupLst.Count()-1 );

    // Allocate a face class
    pFace = new CFace;

    // check for allocations for errors
    if( pFace == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // init the face class with the vertice info
    for( i = 0; i < 3; ++i )
    {
        // Get the pointer to the vertice list item
        pTmpVert = (PCVertice)vLst.GetObj( vertIndex[i] );
        
        // copy the vertex data over
        for( j = 0; j < 3; ++j )
            pFace->point[i].vert[j] = pTmpVert->vert[j];
    }

    // We may not have UV info in this OBJ file
    if( !noUVinfo )
    {
        // init the face class with the UV info
        for( i = 0; i < 3; ++i )
        {
            // Get the pointer to the uv list item
            pTmpUV = (PCUV)uvLst.GetObj( uvIndex[i] );
            
            // copy the uv data over
            for( j = 0; j < 2; ++j )
                pFace->crd[i].uv[j] = pTmpUV->uv[j];
        }
    }

    // Add the filled out face to the group face list
    tmpGroup->faceLst.AddObj( (PVOID)pFace );      

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // AddFaceToGroup


/************************************************************************
*    FUNCTION NAME:         RemoveDupVerticies()                                                             
*
*    DESCRIPTION:           Remove all the duplicate verticies from the
*                           vertice list
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &objLst - Object list of vertices
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
   
bool CObjParser::RemoveDupVerticies( CObjList &objLst )
{
    bool result = false;
    int i, j;
    PCVertice pVert1, pVert2;

    // We have to have a list
    if( objLst.Count() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Cycle through the list and delete all the duplicates
    for( i = 0; i < objLst.Count(); ++i )
    {        
        // Get the vertex to check for duplicates
        pVert1 = (PCVertice)objLst.GetObj( i );

        for( j = i+1; j < objLst.Count(); ++j )
        {            
            // Get the vertex to check against
            pVert2 = (PCVertice)objLst.GetObj( j );

            // Do we have a match?
            if( pVert1->vert[0] == pVert2->vert[0] &&
                pVert1->vert[1] == pVert2->vert[1] &&
                pVert1->vert[2] == pVert2->vert[2] )
            {
                // Delete the vertice at this index
                objLst.Delete( j );

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
*    FUNCTION NAME:         MatchVerticies()                                                             
*
*    DESCRIPTION:           Match up all the vertices in the faces to
*                           the verstices in the list.
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &vertLst  - Object list of vertices
*              CObjList &groupLst - Object list of groups
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
   
bool CObjParser::MatchVerticies( CObjList &vertLst, CObjList &groupLst )
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
        pGroup = (PCGroup)groupLst.GetObj( i );

        // Cycle through all the faces in the group
        for( j = 0; j < pGroup->faceLst.Count(); ++j )
        {
            // Get a pointer to the face in the group
            pFace = (PCFace)pGroup->faceLst.GetObj( j );

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
   
bool CObjParser::SaveToFile( CObjList &vertLst, CObjList &groupLst, char *fileName )
{
    bool result = false;
    FILE *hFile;
    unsigned int vCount, triCount = 0;
    int i, j;
    PCGroup pGroup;
    int faceDataSize;
    CObjList tmpFaceLst;
    PCFace pFace;

    // We don't want this data to be freed because it is being shared
    tmpFaceLst.DontFreeData();

    // Compute the data size which is everything in the class but the vertices.
    faceDataSize = sizeof( CFace ) - (sizeof( CVertice ) * 3);

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

    // Cycle through the group list and
    // count up the number of triangles
    for( i = 0; i < groupLst.Count(); ++i )
        triCount += PCGroup( groupLst.GetObj( i ) )->faceLst.Count();

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

    // Write out the vertex data
    for( i = 0; i < vertLst.Count(); ++i )
    {
        if( fwrite( PCVertice( vertLst.GetObj( i ) )->vert, sizeof( CVertice ), 1, hFile ) != 1 )
        {
            errorCode = ERROR_FILE_WRITE;
            goto HANDLE_ERROR;
        }
    }

    // Create a face list
    for( i = 0; i < groupLst.Count(); ++i )
    {
        // Get a pointer to the group
        pGroup = (PCGroup)groupLst.GetObj( i );

        // Cycle through all the faces in the group
        for( j = 0; j < pGroup->faceLst.Count(); ++j )
        {            
            tmpFaceLst.AddObj( pGroup->faceLst.GetObj( j ) );            
        }
    }

    // Sort this list
    SortByTextIndex( tmpFaceLst );

    // Save all the faces to the file
    for( i = 0; i < tmpFaceLst.Count(); ++i )
    {
        pFace = (PCFace)tmpFaceLst.GetObj( i );
        
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
*    FUNCTION NAME:         SortByTextIndex()                                                             
*
*    DESCRIPTION:           Sorting by texture index will case for less
*                           state changes in OpenGL which should help speed.
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &tmpFaceLst  - Object list of vertices
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
   
void CObjParser::SortByTextIndex( CObjList &tmpFaceLst )
{
    int offset, j, x=0;
    bool inOrder;
    PCFace pFace1, pFace2, ptemp;

    inOrder = false;
    offset = tmpFaceLst.Count();

    // Make sure we have something to sort
    if( offset > 1 )
    {
        while( x < offset && inOrder == false )
        {
            ++x;
            inOrder = true;
            for( j = 0; j < offset - x; ++j )
            {
                pFace1 = (PCFace)tmpFaceLst.GetObj(j);
                pFace2 = (PCFace)tmpFaceLst.GetObj(j+1);
                
                // Sort in an assending order
                if( pFace1->tIndex > pFace2->tIndex )
                {
                    inOrder = false;
                    ptemp = pFace1;
                    tmpFaceLst.SetObj( j, pFace2 );
                    tmpFaceLst.SetObj( j+1, ptemp );
                }
            }
        }
    }

}   // SortByTextIndex


/************************************************************************
*    FUNCTION NAME:         ParseTextLibFile()                                                             
*
*    DESCRIPTION:           Parse the texture library class
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &objLst - Object list to receive allocation
*              char *filePath   - Oringinal file path of OBJ file
*              char *fileName   - Texture library file name (mtl)
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/16/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool CObjParser::ParseTextLibFile( CObjList &objLst, char *filePath, char *fileName )
{
    bool result = false;
    FILE *hFile;
    char pathToLibFile[500], lineStr[300];
    int strCount;

    // Build the path to the texture library file. It's expected to be in
    // the same directory as the OBJ file.
    CropOutPathAddFileName( filePath, pathToLibFile, fileName );

    // Save the file path in case of problem
    strcpy( errorFilePath, pathToLibFile );
    
    // Open the file
    hFile = fopen( pathToLibFile, "rt" );
    if( hFile == NULL )
    {
        errorCode = ERROR_TXT_LIB_FILE_HANDLE_FAIL;
        goto HANDLE_ERROR;
    }

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

        /////////////////////////////////////////////////////
        // The first string decides what the data is.
        //
        // newmtl = material name in mtl file
        // map_Kd = texture file name in mtl file
        /////////////////////////////////////////////////////

        // Do we have a material name?
        if( strcmp( lineStr, "newmtl" ) == 0 )
        {
            // Get the name of the material name associated with the texture
            if( !Decode_MaterialName( objLst, lineStr, strCount ) )
                goto HANDLE_ERROR;

        }
        // Do we have a bitmap file name?
        else if( strcmp( lineStr, "map_Kd" ) == 0 )
        {
            // Get the name of the bitmap
            if( !Decode_TextureName( objLst, lineStr, strCount ) )
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

}   // ParseTextLibFile


/************************************************************************
*    FUNCTION NAME:         Decode_MaterialName()                                                             
*
*    DESCRIPTION:           Get the material name and add it to the list.
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &objLst - Object list to receive allocation
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
   
bool CObjParser::Decode_MaterialName( CObjList &objLst, char *strData, int strCount )
{
    bool result = false;
    int i, count = 0;
    PCTextLibrary pTxtLib;

    // Jump past the first null
    while( strData[count++] )
    {}
    
    // Add back the spaces that was replaced with a null.
    for( i = count; i < strCount; ++i )
    {
        if( strData[i] == '\0' )
            strData[i] = ' ';
    }

    // Allocate a face class
    pTxtLib = new CTextLibrary;

    // check for allocations for errors
    if( pTxtLib == NULL )
    {
        errorCode = ERROR_ALLOCATION_FAIL;
        goto HANDLE_ERROR;
    }

    // Copy over the material name
    strcpy( pTxtLib->materialName, &strData[count] );

    // Add it to the object
    objLst.AddObj( pTxtLib );

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // Decode_MaterialName


/************************************************************************
*    FUNCTION NAME:         Decode_TextureName()                                                             
*
*    DESCRIPTION:           Get the texture name and add it to the list.
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &objLst - Object list to receive allocation
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
   
bool CObjParser::Decode_TextureName( CObjList &objLst, char *strData, int strCount )
{
    bool result = false;
    int i, count = 0;
    PCTextLibrary pTxtLib;

    // We are supposed to have at least one item in the list.
    // Make sure that we do
    if( objLst.Count() == 0 )
    {
        errorCode = ERROR_EMPTY_GROUP;
        goto HANDLE_ERROR;
    }

    // Get the texture library pointer
    pTxtLib = (PCTextLibrary)objLst.GetObj( objLst.Count()-1 );

    // Jump past the first null
    while( strData[count++] )
    {}
    
    // Add back the spaces that was replaced with a null.
    for( i = count; i < strCount; ++i )
    {
        if( strData[i] == '\0' )
            strData[i] = ' ';
    }

    // The format for the texture file name looks like this...
    // map_Kd .\filename.bmp
    // It get's more complicated depending on where the bitmap
    // file is. To keep this code simple, we are just going
    // to assume all the textures are in the same directory
    // as the OBJ file
    for( i = strCount-1; i > count; --i )
    {
        if( strData[i] == '\\' )
            break;
    }

    // Copy over the material name
    strcpy( pTxtLib->txtFileName, &strData[i+1] );

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // Decode_TextureName


/************************************************************************
*    FUNCTION NAME:         FlagDuplicateTexturess()                                                             
*
*    DESCRIPTION:           Check to see if we have any duplicate textures
*                           in the mix. Depending on the modeling program,
*                           adding in the same BMP as a material is not
*                           stopped. So we'll take a moment to stop it here.
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &objLst - Object list to receive allocation
*              var2type Var2Name - Description
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
   
bool CObjParser::FlagDuplicateTexturess( CObjList &tLst, CObjList &noDupTxtLst )
{
    int i, j;
    PCTextLibrary pTextLib1, pTextLib2;
    bool result = false;
    char *ptmpStr;

    // Cycle through the list and delete all the duplicates
    for( i = 0; i < tLst.Count(); ++i )
    {        
        // Get the bitmap file name to check for duplicates
        pTextLib1 = (PCTextLibrary)tLst.GetObj( i );

        for( j = i+1; j < tLst.Count(); ++j )
        {            
            // Get the bitmap file name to check against
            pTextLib2 = (PCTextLibrary)tLst.GetObj( j );

            // Do we have a match?
            if( strcmp( pTextLib1->txtFileName, pTextLib2->txtFileName ) == 0 )
            {
                // Set the duplicate flag
                pTextLib2->duplicate = true;
            }
        }
    }

    // Build a bmp list of names that are not duplicates
    for( i = 0; i < tLst.Count(); ++i )
    {
        if( !PCTextLibrary( tLst.GetObj( i ) )->duplicate )
        {
            // Allocate a string to hold the name
            ptmpStr = new char [ strlen( PCTextLibrary( tLst.GetObj( i ) )->txtFileName )+1 ];
            if( ptmpStr == NULL )
            {
                errorCode = ERROR_ALLOCATION_FAIL;
                goto HANDLE_ERROR;
            }

            // Copy the string
            strcpy( ptmpStr, PCTextLibrary( tLst.GetObj( i ) )->txtFileName );

            // Add to list
            noDupTxtLst.AddObj( ptmpStr );
        }
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // FlagDuplicateTexturess


/************************************************************************
*    FUNCTION NAME:         MatchTextureIndexToFace()                                                             
*
*    DESCRIPTION:           Match texture index to face
*
*    FUNCTION PARAMETERS:
*    Input:    CObjList &groupLst  - Object group list
*              CObjList &textLst - Texture list
*              CObjList &noDupTxtLst - List of bitmap file names with all
*                                      duplicates stripped out
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
   
bool CObjParser::MatchTextureIndexToFace( CObjList &groupLst, CObjList &textLst, CObjList &noDupTxtLst )
{
    bool result = false;
    int i, j, textIndex, w;
    PCGroup pGroup;
    PCTextLibrary pTextLib;
    bool fMatchFound;

    // Cycle through the group list
    for( i = 0; i < groupLst.Count(); ++i )
    {        
        // Get a pointer to the group
        pGroup = (PCGroup)groupLst.GetObj( i );

        fMatchFound = false;

        // Cycle through the texture list looking for a match
        for( j = 0; j < textLst.Count(); ++j )
        {
            // Get the vertex to check against
            pTextLib = (PCTextLibrary)textLst.GetObj( j );

            // Do we have a match?
            if( strcmp( pGroup->materialName, pTextLib->materialName ) == 0 )
            {
                // Now search for the texture in the non duplicate list
                for( w = 0; w < noDupTxtLst.Count(); ++w )
                {
                    if( strcmp( (char *)noDupTxtLst.GetObj( w ), pTextLib->txtFileName ) == 0 )
                    {
                        // Get the index into the texture library minus the duplicates
                        textIndex = w;
                        // Set the flag that indicates this texture is being used
                        pTextLib->used = true;
                        // Set the sanity flag
                        fMatchFound = true;
                        break;
                    }
                }

                // our work here is done
                break;
            }
        }

        // Do a sanity check. We have to have a match.
        if( !fMatchFound )
        {
            errorCode = ERROR_MATCH_NOT_FOUND_TEXTURE;
            goto HANDLE_ERROR;
        }

        // All the faces in this group use the same texture.
        // That's what defines a group, they all use the same material name
        for( j = 0; j < pGroup->faceLst.Count(); ++j )
            PCFace( pGroup->faceLst.GetObj( j ) )->tIndex = textIndex;
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // MatchTextureIndexToFace


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
   
bool CObjParser::BuildTextureLibrary( CTextureLib &textLib, CObjList &noDupTxtLst, char *filePath )
{
    char pathToBMP[500];
    bool result = false;
    int i;

    /////////////////////////////////////////////////////
    //   Build a 32 bit texture map library
    /////////////////////////////////////////////////////

    // Tell the texture library how many textures we need
    if( !textLib.AllocateTextLibrary( noDupTxtLst.Count() ) )
    {
        errorCode = ERROR_TEXT_LIB_ALLOC;
        goto HANDLE_ERROR;
    }

    // Now add each texture to the library
    for( i = 0; i < textLib.Count(); ++i )
    {
        // create the path to the BMP file
        CropOutPathAddFileName( filePath, pathToBMP, (char *)noDupTxtLst.GetObj( i ) );

        // Save the file path in case of problem
        strcpy( errorFilePath, pathToBMP );

        // Add the texture to the library
        if( !textLib.AddTextureToLib( i, pathToBMP ) )
        {
            errorCode = ERROR_LOAD_TEXTURE;
            goto HANDLE_ERROR;
        }
    }

    // Replace the extension to the texture library file
    RelpaceFileExtension( filePath, "bt" );
    
    // Save the library to file
    textLib.SaveToFile( filePath );

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
   
void CObjParser::ConvertUVtoSpecificCoordinates( CTextureLib &textLib, CObjList &groupLst )
{
    int i, j, w;
    PCGroup pGroup;
    PCFace pFace;

    // Cycle through the group list
    for( i = 0; i < groupLst.Count(); ++i )
    {        
        // Get a pointer to the group
        pGroup = (PCGroup)groupLst.GetObj( i );

        // All the faces in this group use the same texture.
        // That's what defines a group, they all use the same material name
        for( j = 0; j < pGroup->faceLst.Count(); ++j )
        {
            // get the face in this group
            pFace = (PCFace)pGroup->faceLst.GetObj( j );

            // Do the UV calculations depending on the texture width & height
            for( w = 0; w < 3; ++w )
            {
                pFace->crd[w].uv[0] *= (textLib.pTMap[pFace->tIndex].GetWidth()-1);
                pFace->crd[w].uv[1] *= (textLib.pTMap[pFace->tIndex].GetHeight()-1);
            }
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
   
bool CObjParser::Build_H_File( CObjList &noDupTxtLst, char *filePath )
{
    FILE *hFile; 
    bool result = false;
    char fileName[200], enumName[200];
    int i;

    char header[] = {"/*********************************************************\n* Texture Library Specific Header File: %s\n*********************************************************/\n\n"};

    // Change the extension to the original file path
    RelpaceFileExtension( filePath, "h" );

    // Open the file
    hFile = fopen( filePath, "wt" );
    if( hFile == NULL )
    {
        errorCode = ERROR_H_FILE_HANDLE_FAIL;
        goto HANDLE_ERROR;
    }

    // Get the file name but keep the file extension
    CropFileNameFromPath( filePath, fileName );

    // Write the header file
    fprintf( hFile, header, fileName );

    // Get the file name but loose the file extension
    CropFileNameFromPath( filePath, fileName, false );

    // Replace all spaces with an underscore
    RelpaceChar( fileName, ' ', '_' );

    fprintf( hFile, "#ifndef __%s_H__\n", fileName );
    fprintf( hFile, "#define __%s_H__\n\n", fileName );

    fprintf( hFile, "enum ETL_%s\n{\n", fileName );

    for( i = 0; i < noDupTxtLst.Count(); ++i )
    {
        // Get the file name but loose the file extension
        CropFileNameFromPath( (char *)noDupTxtLst.GetObj( i ), enumName, false );

        // Replace all spaces with an underscore
        RelpaceChar( enumName, ' ', '_' );

        // Write the enum
        if( i == 0 )
            fprintf( hFile, "    ET_%s=0,\n", enumName );
        else
            fprintf( hFile, "    ET_%s,\n", enumName );
    }

    // Max Amount
    fprintf( hFile, "    ET_MAX_%s,\n", fileName );

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

void CObjParser::RelpaceFileExtension( char *filePath, char *Ext )
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

void CObjParser::CropOutPathAddFileName( char *sourcePath, char *destPath, char *newFileName )
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
   
bool CObjParser::BuildMapFile( CObjList &groupLst, CObjList &vLst, CObjList &tLst, 
                               CTextureLib &textLib, CObjList &noDupTxtLst, char *filePath )
{
    FILE *hFile;
    int i, j, w, faceCount = 0, unusedTextCount = 0, counter = 0;
    PCVertice pVert;
    PCFace pFace;
    PCGroup pGroup;
    bool result = false;

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
    fprintf( hFile, "Vertex count: %d\n\n", vLst.Count() );

    // List all the vertices
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

    // Count up all the faces
    for( i = 0; i < groupLst.Count(); ++i )
        faceCount += PCGroup( groupLst.GetObj( i ) )->faceLst.Count();

    // Group/Triangle list
    fprintf( hFile, "\nGroup/Face List:\n" );
    fprintf( hFile, "Group count: %d\n", groupLst.Count() );
    fprintf( hFile, "Total face count: %d\n", faceCount );

    // Cycle through the group list
    for( i = 0; i < groupLst.Count(); ++i )
    {        
        // Get a pointer to the group
        pGroup = (PCGroup)groupLst.GetObj( i );

        // Display group name
        fprintf( hFile, "\n  %u Group: %s - %s\n", i+1, pGroup->groupName, pGroup->materialName );
        fprintf( hFile, "  Face count: %d\n", pGroup->faceLst.Count() );

        // Cycle through all the faces in the group
        for( j = 0; j < pGroup->faceLst.Count(); ++j )
        {
            // Get a pointer to the face in the group
            pFace = (PCFace)pGroup->faceLst.GetObj( j );

            // Face count
            fprintf( hFile, "    %d Face:\n", j+1 );

            // Display vertex index
            fprintf( hFile, "      vIndex: x:%d, y:%d, z:%d\n", pFace->vIndex[0], 
                                   pFace->vIndex[1], pFace->vIndex[2] );

            for( w = 0; w < 3; ++w )
            {
                // Display the UV coordinate
                fprintf( hFile, "      UV: u:%f, v:%f\n", pFace->crd[w].uv[0], pFace->crd[w].uv[1] );
            }

            // Texture index
            fprintf( hFile, "      tIndex: %u, name: %s\n", pFace->tIndex, (char *)noDupTxtLst.GetObj( pFace->tIndex ) );
        }
    }

    // texture map list
    fprintf( hFile, "\nTexture Map List:\n" );
    fprintf( hFile, "Texture count: %d\n", textLib.Count() );

    // Cycle through the texture map list
    for( i = 0; i < textLib.Count(); ++i )
    {
        // Get the name of the texture
        fprintf( hFile, "\n  %d: %s\n", i, (char *)noDupTxtLst.GetObj( i ) );

        // Get the width and height of texture
        fprintf( hFile, "  width: %d, height: %d", textLib.pTMap[i].GetWidth(), textLib.pTMap[i].GetHeight() );
    }

    // Add up all the unused textures
    for( i = 0; i < tLst.Count(); ++i )
    {
        if( !PCTextLibrary( tLst.GetObj( i ) )->duplicate && !PCTextLibrary( tLst.GetObj( i ) )->used )
            ++unusedTextCount;
    }

    // Display all the unused textures
    if( unusedTextCount != 0 )
    {
        // unused texture map list
        fprintf( hFile, "\nUnused Texture Map List: (Textures in library not currently being referenced)\n" );
        fprintf( hFile, "Unused texture count: %d\n", unusedTextCount );

        // List all the textures not being used
        for( i = 0; i < tLst.Count(); ++i )
        {
            if( !PCTextLibrary( tLst.GetObj( i ) )->duplicate && !PCTextLibrary( tLst.GetObj( i ) )->used )
            {
                ++counter;
                fprintf( hFile, "  %d: %s, %s\n", counter, PCTextLibrary( tLst.GetObj( i ) )->txtFileName,
                                                           PCTextLibrary( tLst.GetObj( i ) )->materialName );
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

void CObjParser::CropFileNameFromPath( char *SourceFilePath, char *DestFileName, bool KeepExtension )
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

void CObjParser::RelpaceChar( char *charString, char find, char replace )
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