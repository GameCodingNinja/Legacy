
/************************************************************************
*
*    FILE NAME:       CMeshList.cpp
*
*    DESCRIPTION:     Mesh List class template  
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

#include "CMeshList.h"       // Header file for this *.cpp file
#include "CMatrix.h"         // Matrix class


/************************************************************************
*    DESCRIPTION:  Constructer                                                             
*
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
*    DESCRIPTION:  Render all the meshes
*
*    Input:    Matrix3D &M - Matris to do all the translations
*
*    Output:   int - Number of polygons sent to be rendered
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
int CMeshList<mesh_type>::RenderAll( CMatrix &m )
{
    int polyCount = 0;

    // Clear out all the render flags
    ClearFlags();

    // Render the mesh objects
    for( int i = 0; i < meshCount; ++i )
    {
        polyCount += pMesh[i].Render( m );
    }

    return polyCount;

}   // RenderAll


/************************************************************************
*    DESCRIPTION:  Load an object from a PLY binary file. This is
*                  our own simple file format for quickly loading
*                  vertices, face indexes, uv and texture indexes
*                  into the texture library.
*
*    Input:    char *filePath - Path to file to load
*
*    Output:   bool - true on success, false on fail
*
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
*    DESCRIPTION:  Load an object from a PLY binary file. This is
*                  our own simple file for quickly loading
*                  vertices, face indexes, uv and texture indexes
*                  into the texture library.
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
*    DESCRIPTION:  Calling this function means the file only
*                  contains mesh data and we need to use this
*                  to calculate all our normals before we save
*                  the final file which is a combination of
*                  mesh and normal data.
*
*    Input:    char *filePath - Path to file to load
*
*    Output:   bool - true on success, false on fail
*
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
*    DESCRIPTION:   Calling this function means the file only
*                   contains mesh data and we need to use this
*                   to calculate all our normals before we save
*                   the final file which is a combination of
*                   mesh and normal data.
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
*    DESCRIPTION:  Save mesh and normal data to file. This is
*                  our own simple file format for quickly loading
*                  vertices, face indexes, uv and texture indexes
*                  into the texture library.
*
*    Input:    char *filePath - Path to file to load
*
*    Output:   bool - true on success, false on fail
*
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
*    DESCRIPTION:  Save mesh and normal data to file. This is
*                  our own simple file for quickly loading
*                  vertices, face indexes, uv and texture indexes
*                  into the texture library.
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
*    DESCRIPTION:  Free the allocations
*
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
*    DESCRIPTION:  Calculate normals
*
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
*    DESCRIPTION:  Set the list of light data to all meshes
*
*    Input:    CLightList &lightList - Pointer to light list
*
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
*    DESCRIPTION:  Set the list of light data to a specific mesh
*
*    Input:    int index - Index to set the list to
*              CLightList &lightList - Pointer to light list
*
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
*    DESCRIPTION:  Scale the mesh classes.
*
*    Input:    float x, y, z: Scale amounts
*
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
}   // Scale


/************************************************************************
*    DESCRIPTION:  Set the texture library to all meshes
*
*    Input:    CTextureLib &textLib - Texture library
*
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


/************************************************************************
*    DESCRIPTION:  Depending on the angle of the normal, set the polygon
*                  Collision type to wall or floor.
*
*    -------------------------------------------------------------------
*    H Rosenorn    03/18/05      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
void CMeshList<mesh_type>::SetWallFloorCollisionType()
{
	for( int i = 0; i < meshCount; ++i )
    {
        pMesh[i].SetWallFloorCollisionType();
    }

}	// SetWallFloorCollisionType


/************************************************************************
*    DESCRIPTION:  Clear all the render flags
*
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
void CMeshList<mesh_type>::ClearFlags()
{
    // Render the mesh objects
    for( int i = 0; i < meshCount; ++i )
    {
        pMesh[i].ResetRenderFlag();
		pMesh[i].ResetCullTestFlag();
		pMesh[i].ResetTransformedFlag();
    }

}   // ClearRenderFlags


/************************************************************************
*    DESCRIPTION: Adjust the radius depending on the need  
*
*    Input:  float scaler - Amount to scale the radius by
*            EOT_OBJECT_TYPE objType - mesh object type looking for
*            ECT_COLLISION_TYPE colType -  collision type looking for
*
*    -------------------------------------------------------------------
*    H Rosenorn    05/01/06      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
template <class mesh_type>
void CMeshList<mesh_type>::AdjustRadius( float scaler, EOT_OBJECT_TYPE objType,
                                         ECT_COLLISION_TYPE colType )
{
    // Find the type
    for( int i = 0; i < meshCount; ++i )
    {
        if( pMesh[i].GetType() == objType )
        {
            pMesh[i].AdjustRadius( scaler, colType );
        }
    }
}	// AdjustRadius