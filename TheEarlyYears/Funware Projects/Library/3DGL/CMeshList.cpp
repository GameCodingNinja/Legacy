
/************************************************************************
*
*    PRODUCT:         Mesh List class template
*
*    FILE NAME:       CMeshList.h
*
*    DESCRIPTION:     This list class is designed to only call members
*                     of the base mesh class. Inherit from this class
*                     to call specialized inherited mesh class members.  
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

#include "CMeshList.h"       // Header file for this *.cpp file
#include "CMatrix.h"         // Matrix class


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
CMeshList<mesh_type>::CMeshList()
{
    // Init class members
    meshCount = 0;
    pMesh = NULL;
}


/************************************************************************
*    FUNCTION NAME:         RenderAll()                                                             
*
*    DESCRIPTION:           Render all the meshes
*
*    FUNCTION PARAMETERS:
*    Input:    Matrix3D &M - Matris to do all the translations
*              var2type Var2Name - Description
*
*    Output:   int - Number of polygons sent to be rendered
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
int CMeshList<mesh_type>::RenderAll( CMatrix &m )
{
    int polyCount = 0;

    // Render the mesh objects
    for( int i = 0; i < meshCount; ++i )
    {
        polyCount += pMesh[i].Render( m );
    }

    return polyCount;

}   // RenderAll


/************************************************************************
*    FUNCTION NAME:         LoadFromFile()                                                             
*
*    DESCRIPTION:           Load an object from a PLY binary file. This is
*                           our own simple file format for quickly loading
*                           vertices, face indexes, uv and texture indexes
*                           into the texture library.
*
*    FUNCTION PARAMETERS:
*    Input:    char *filePath - Path to file to load
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
bool CMeshList<mesh_type>::LoadFromFile( char *filePath )
{
    bool result = false;

    // Open the file
    FILE * hFile = fopen( filePath, "rb" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    result = LoadFromFile( hFile );

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );
    
    return result;

}   // LoadFromFile


/************************************************************************
*    FUNCTION NAME:         LoadFromFile()                                                             
*
*    DESCRIPTION:           Load an object from a PLY binary file. This is
*                           our own simple file for quickly loading
*                           vertices, face indexes, uv and texture indexes
*                           into the texture library.
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - Handle to file
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
bool CMeshList<mesh_type>::LoadFromFile( FILE *hFile )
{
    bool result = false;
    int i;
    char formatType[FORMAT_SPEC_STR_COUNT];

    Free();

    // Load in the header so that we can check it
    if( fread( formatType, sizeof( formatType ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Check the header to make sure this is not random data
    if( strcmp( formatType, MESH_LST_FORMAT_STR_ID ) != 0 )
        goto HANDLE_ERROR;

    // Read in the mesh count
    if( fread( &meshCount, sizeof( meshCount ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Allocate the mesh list
    pMesh = new mesh_type[ meshCount ];
    if( pMesh == NULL )
        goto HANDLE_ERROR;

    for( i = 0; i < meshCount; ++i )
    {
        result = pMesh[i].LoadFromFile( hFile );

        if( result == false )
            goto HANDLE_ERROR;
    }

HANDLE_ERROR:;
    
    return result;

}   // LoadFromFile


/************************************************************************
*    FUNCTION NAME:         LoadMeshFromFile()                                                             
*
*    DESCRIPTION:           Calling this function means the file only
*                           contains mesh data and we need to use this
*                           to calculate all our normals before we save
*                           the final file which is a combination of
*                           mesh and normal data.
*
*    FUNCTION PARAMETERS:
*    Input:    char *filePath - Path to file to load
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
bool CMeshList<mesh_type>::LoadMeshFromFile( char *filePath )
{
    bool result = false;

    // Open the file
    FILE * hFile = fopen( filePath, "rb" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    result = LoadMeshFromFile( hFile );

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );
    
    return result;

}   // LoadMeshFromFile


/************************************************************************
*    FUNCTION NAME:         LoadMeshFromFile()                                                             
*
*    DESCRIPTION:           Calling this function means the file only
*                           contains mesh data and we need to use this
*                           to calculate all our normals before we save
*                           the final file which is a combination of
*                           mesh and normal data.
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - Handle to file
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
bool CMeshList<mesh_type>::LoadMeshFromFile( FILE *hFile )
{
    bool result = false;
    int i;
    char formatType[FORMAT_SPEC_STR_COUNT];

    Free();

    // Load in the header so that we can check it
    if( fread( formatType, sizeof( formatType ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Check the header to make sure this is not random data
    if( strcmp( formatType, MESH_LST_FORMAT_STR_ID ) != 0 )
        goto HANDLE_ERROR;

    // Read in the mesh count
    if( fread( &meshCount, sizeof( meshCount ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Allocate the mesh list
    pMesh = new mesh_type[ meshCount ];
    if( pMesh == NULL )
        goto HANDLE_ERROR;

    // Load the vertex and face info from file
    for( i = 0; i < meshCount; ++i )
    {
        result = pMesh[i].LoadMeshFromFile( hFile );

        if( result == false )
            goto HANDLE_ERROR;
    }
    
HANDLE_ERROR:;
    
    return result;

}   // LoadMeshFromFile


/************************************************************************
*    FUNCTION NAME:         SaveToFile()                                                             
*
*    DESCRIPTION:           Save mesh and normal data to file. This is
*                           our own simple file format for quickly loading
*                           vertices, face indexes, uv and texture indexes
*                           into the texture library.
*
*    FUNCTION PARAMETERS:
*    Input:    char *filePath - Path to file to load
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
bool CMeshList<mesh_type>::SaveToFile( char *filePath )
{
    bool result = false;

    // Open the file
    FILE * hFile = fopen( filePath, "wb" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    result = SaveToFile( hFile );
    
HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );
    
    return result;

}   // SaveToFile


/************************************************************************
*    FUNCTION NAME:         SaveToFile()                                                             
*
*    DESCRIPTION:           Save mesh and normal data to file. This is
*                           our own simple file for quickly loading
*                           vertices, face indexes, uv and texture indexes
*                           into the texture library.
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - Handle to file
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
bool CMeshList<mesh_type>::SaveToFile( FILE *hFile )
{
    bool result = false;
    char formatType[FORMAT_SPEC_STR_COUNT];
    int i;

    memset( formatType, 0, sizeof(formatType) );
    strcpy( formatType, MESH_LST_FORMAT_STR_ID );

    // Write out the format ID
    if( fwrite( formatType, sizeof( formatType ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Write out the group count
    if( fwrite( &meshCount, sizeof( meshCount ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    for( i = 0; i < meshCount; ++i )
    {
        result = pMesh[i].SaveToFile( hFile );

        if( result == false )
            goto HANDLE_ERROR;
    }
    
HANDLE_ERROR:;
    
    return result;

}   // SaveToFile


/************************************************************************
*    FUNCTION NAME:         Free()                                                             
*
*    DESCRIPTION:           Free the allocations
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
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
void CMeshList<mesh_type>::Free()
{
    if( pMesh != NULL )
    {
        delete [] pMesh;
        pMesh = NULL;
        meshCount = 0;
    }
}   // Free


/************************************************************************
*    FUNCTION NAME:         CalNormals()                                                             
*
*    DESCRIPTION:           Calculate normals
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
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
void CMeshList<mesh_type>::CalNormals()
{
    for( int i = 0; i < meshCount; ++i )
    {
        pMesh[i].CalNormals();
    }
}   // CalNormals


/************************************************************************
*    FUNCTION NAME:         SetLightList()                                                             
*
*    DESCRIPTION:           Set the list of light data to all meshes
*
*    FUNCTION PARAMETERS:
*    Input:    CLightList &lightList - Pointer to light list
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
void CMeshList<mesh_type>::SetLightList( CLightList &lightList )
{
    for( int i = 0; i < meshCount; ++i )
    {
        pMesh[i].SetLightList( lightList );
    }
}   // SetLightList


/************************************************************************
*    FUNCTION NAME:         SetLightList()                                                             
*
*    DESCRIPTION:           Set the list of light data to a specific mesh
*
*    FUNCTION PARAMETERS:
*    Input:    int index - Index to set the list to
*              CLightList &lightList - Pointer to light list
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
void CMeshList<mesh_type>::SetLightList( int index, CLightList &lightList )
{
    if( index < meshCount )
    {
        pMesh[index].SetLightList( lightList );
    }
}   // SetLightList


/************************************************************************
*    FUNCTION NAME:         Scale()                                                             
*
*    DESCRIPTION:           Scale the mesh classes.
*
*    FUNCTION PARAMETERS:
*    Input:    float x, y, z: Scale amounts
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
void CMeshList<mesh_type>::Scale( float x, float y, float z )
{
    for( int i = 0; i < meshCount; ++i )
    {
        pMesh[i].Scale( x, y, z );
    }
}   // SetLightList


/************************************************************************
*    FUNCTION NAME:         SetTextureLibrary()                                                             
*
*    DESCRIPTION:           Set the texture library to all meshes
*
*    FUNCTION PARAMETERS:
*    Input:    CTextureLib &textLib - Texture library
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
void CMeshList<mesh_type>::SetTextureLibrary( CTextureLib &textLib )
{
    for( int i = 0; i < meshCount; ++i )
    {
        pMesh[i].SetTextureLibrary( textLib );
    }
}   // SetTextureLibrary