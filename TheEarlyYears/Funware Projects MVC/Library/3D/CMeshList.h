
/************************************************************************
*
*    FILE NAME:       CMeshList.h
*
*    DESCRIPTION:     Mesh List class template  
*
************************************************************************/

#ifndef _CMESH_LIST_H_
#define _CMESH_LIST_H_

#include "types.h"           // Misc type specifiers
#include "CMesh.h"           // Mesh class
#include "CMatrix.h"         // Matrix class

class CMesh;
class CMatrix;
class CLightList;

#define MESH_LST_FORMAT_STR_ID "mesh list"


/************************************************************************
*
*    Class for handling lists of meshes
*
************************************************************************/

template <class mesh_type>
class CMeshList
{
protected:

    // Mesh array
    typename mesh_type *pMesh;
    // Number of allocated meshes
    int meshCount;
	// allocation flag - if this class owns  
	// the mesh, it should delete it
	bool meshAllocated;

public:

    CMeshList();
	CMeshList( CMeshList &msh );
    virtual ~CMeshList(){ Free(); }

    // Free the allocations
    void Free();
    // Get the count
    int Count(){ return meshCount; };
    // Render all the meshes
    int RenderAll( CMatrix &m );
    // Load mesh and normal data from file
    virtual bool LoadFromFile( char *filePath );
    virtual bool LoadFromFile( FILE *hFile );
    // Load mesh from file
    virtual bool LoadMeshFromFile( char *filePath );
    virtual bool LoadMeshFromFile( FILE *hFile );
    // Save mesh and normal data to file
    virtual bool SaveToFile( char *filePath );
    virtual bool SaveToFile( FILE *hFile );
    // Calculate normals
    void CalNormals();
	// Calculate vertex normals
	void CalcVertexNormals( float range = 0.0f );
    // Set the list of light data to all meshes
    virtual void SetLightList( CLightList * pLiteLst );
    // Set the list of light data to a specific mesh
    void SetLightList( int index, CLightList * pLiteLst );
    // Scale the mesh classes
    void Scale( float x, float y, float z );
	void Scale( int index, int count, float x, float y, float z );
    // Set the texture library to all meshes
    void SetTextureLibrary( CTextureLib &textLib );
	// Set the colision type
	void SetWallFloorCollisionType();
	// Clear all related flags
	void ClearFlags();
	// get the mesh
	mesh_type * GetMesh(){ return pMesh; };
	// Adjust the radius depending on the need
	void AdjustRadius( float scaler, EOT_OBJECT_TYPE objType,
                       ECT_COLLISION_TYPE colType );
	// Clear the mesh lights
	virtual void ClearMeshLights();
};


/************************************************************************
*    DESCRIPTION:  Constructer                                                             
************************************************************************/
template <class mesh_type>
CMeshList<mesh_type>::CMeshList() 
                    : pMesh(NULL), meshCount(0), meshAllocated(false) 
{
}


/************************************************************************
*    DESCRIPTION:  Copy Constructer                                                             
************************************************************************/
template <class mesh_type>
CMeshList<mesh_type>::CMeshList( CMeshList &msh ) 
                    : pMesh(msh.GetMesh()),
                      meshCount(msh.Count()),
                      meshAllocated(false) 
{
}


/************************************************************************
*    DESCRIPTION:  Render all the meshes
*
*    Input:    Matrix3D &M - Matris to do all the translations
*
*    Output:   int - Number of polygons sent to be rendered
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
************************************************************************/
template <class mesh_type>
bool CMeshList<mesh_type>::LoadFromFile( char *filePath )
{
    bool result = false;

    // Open the file
    FILE * hFile;
	hFile = fopen( filePath, "rb" );
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

	// Set the flag that indicates this class allocated the mesh
	meshAllocated = true;

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

	// Set the flag that indicates this class allocated the mesh
	meshAllocated = true;

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
************************************************************************/
template <class mesh_type>
void CMeshList<mesh_type>::Free()
{
    if( pMesh != NULL )
    {
        if( meshAllocated )
        	delete [] pMesh;

        pMesh = NULL;
        meshCount = 0;
		meshAllocated = false;
    }
}   // Free


/************************************************************************
*    DESCRIPTION:  Calculate normals
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
*    DESCRIPTION:  Calculate vertex normals
************************************************************************/
template <class mesh_type>
void CMeshList<mesh_type>::CalcVertexNormals( float range )
{
    for( int i = 0; i < meshCount; ++i )
    {
        pMesh[i].CalcVertexNormals( range );
    }
}   // CalNormals


/************************************************************************
*    DESCRIPTION:  Set the list of light data to all meshes
*
*    Input:    CLightList * pLiteLst - Pointer to light list
************************************************************************/
template <class mesh_type>
void CMeshList<mesh_type>::SetLightList( CLightList * pLiteLst )
{
    for( int i = 0; i < meshCount; ++i )
    {
        pMesh[i].SetLightList( pLiteLst );
    }
}   // SetLightList


/************************************************************************
*    DESCRIPTION:  Set the list of light data to a specific mesh
*
*    Input:    int index - Index to set the list to
*              CLightList * pLiteLst - Pointer to light list
************************************************************************/
template <class mesh_type>
void CMeshList<mesh_type>::SetLightList( int index, CLightList * pLiteLst )
{
    if( index < meshCount )
    {
        pMesh[index].SetLightList( pLiteLst );
    }
}   // SetLightList


/************************************************************************
*    DESCRIPTION:  Scale the mesh classes.
*
*    Input:    float x, y, z: Scale amounts
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
*    DESCRIPTION:  Scale the mesh classes.
*
*    Input:   int index - mesh starting point
*             int count - number of meshes to scale 
*             float x, y, z: Scale amounts
************************************************************************/
template <class mesh_type>
void CMeshList<mesh_type>::Scale( int index, int count, float x, float y, float z )
{    
    for( int i = index; i < index + count; ++i )
    {
        pMesh[i].Scale( x, y, z );
    }
}   // Scale


/************************************************************************
*    DESCRIPTION:  Set the texture library to all meshes
*
*    Input:    CTextureLib &textLib - Texture library
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


/************************************************************************
*    DESCRIPTION:  Clear the mesh lights
************************************************************************/
template <class mesh_type>
void CMeshList<mesh_type>::ClearMeshLights()
{
	for( int i = 0; i < meshCount; i++ )
	{		
		// Remove the light list since it's been setup
		pMesh[i].ClearLightLst();		
	}

}   // ClearMeshLights


#endif  // _CMESH_LIST_H_