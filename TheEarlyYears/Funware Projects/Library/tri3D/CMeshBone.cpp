/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CMeshBone.cpp
*
*    DESCRIPTION:     Complex polygon object class
*
*    IMPLEMENTATION:  For these classes, we are using a class more so
*                     as a container and not so much for following good
*                     object design. In 3D, speed is everything. We need
*                     fast access to all data which is why everything
*                     is made public.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "CMeshBone.h"       // Header file for this *.cpp file.


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    08/20/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CMeshBone::CMeshBone()
{
	pBoneAnimList = NULL;
	boneAnimCount = 0;
	boneCount = 0;
}


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    08/20/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CMeshBone::~CMeshBone()
{
	Free();

}   // Destructer


/************************************************************************
*    FUNCTION NAME:         Free()                                                             
*
*    DESCRIPTION:           Free all allocations
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
*    H Rosenorn    10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMeshBone::Free()
{
    CMeshTri::Free();

	boneAnimCount = 0;
	boneCount = 0;

	if( pBoneAnimList != NULL )
	{
		delete [] pBoneAnimList;
		pBoneAnimList = NULL;
	}

}   // Free


/************************************************************************
*    FUNCTION NAME:         LoadFromFile()                                                             
*
*    DESCRIPTION:           Load an object from a PLY binary file. This is
*                           our own simple file for quickly loading
*                           vertices, face indexes, uv and texture indexes
*                           into the texture library.
*
*    FUNCTION PARAMETERS:
*    Input:    char *filePath - Path to file to load
*              bool calNormalse - Calculate normals on the fly
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    08/12/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMeshBone::LoadFromFile( char *filePath, bool calNormals )
{
    bool result = false;
    FILE *hFile;
    int i, j, vIndex;

    // Free all allocations
    Free();

    // Open the file
    hFile = fopen( filePath, "rb" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    // Get the number of vertexes
    if( fread( &vCount, sizeof( vCount ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Get the number of triangles
    if( fread( &triCount, sizeof( triCount ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Get the number of bones
	//if( fread( &boneCount, sizeof( boneCount ), 1, hFile ) != 1 )
   //     goto HANDLE_ERROR;

	// Get the number of bone animations
	//if( fread( &boneAnimCount, sizeof( boneAnimCount ), 1, hFile ) != 1 )
    //    goto HANDLE_ERROR;

    // Allocate the vertex list
    vList = new CPoint3D[ vCount ];
    if( vList == NULL )
        goto HANDLE_ERROR;

    // Allocate the projected flag list
    fProjected = new bool[ vCount ];
    if( fProjected == NULL )
        goto HANDLE_ERROR;

    // Allocate the triangle list
    triList = new CTriangle[ triCount ];
    if( triList == NULL )
        goto HANDLE_ERROR;

    // Allocate the normal list
    nList = new CPoint[ triCount ];
    if( nList == NULL )
        goto HANDLE_ERROR;

    // Setup the normals with the pointers
    for( i = 0; i < triCount; ++i )
        triList[i].pNormal = &nList[i];

    // Load the global vertex list for this object
    for( i = 0; i < vCount; ++i )
    {
        if( fread( &vList[i].x, sizeof(vList[i].x), 1, hFile ) != 1 )
            goto HANDLE_ERROR;
            
        if( fread( &vList[i].y, sizeof(vList[i].y), 1, hFile ) != 1 )
            goto HANDLE_ERROR;
            
        if( fread( &vList[i].z, sizeof(vList[i].z), 1, hFile ) != 1 )
            goto HANDLE_ERROR;

		if( fread( &vList[i].boneIndex, sizeof(vList[i].boneIndex), 1, hFile ) != 1 )
            goto HANDLE_ERROR;
            
        // Make a copy to the transformed vertexes for startup inits
        vList[i].tx = vList[i].x;
        vList[i].ty = vList[i].y;
        vList[i].tz = vList[i].z;  
    }       

    // Load the triangle data
    for( i = 0; i < triCount; ++i )
    {
        // Load the vertex index and set the pointer
        for( j = 0; j < 3; ++j )
        {
            // Read in the vertex index
            if( fread( &vIndex, sizeof(vIndex), 1, hFile ) != 1 )
                goto HANDLE_ERROR;

            // Set the pointers
            triList[i].pVPoint[j] = &vList[vIndex];
            triList[i].pfProjected[j] = &fProjected[vIndex];
        }
        
        // Load the UV corridinates
        for( j = 0; j < 3; ++j )
        {
            if( fread( &triList[i].uvi[j].u, sizeof(triList[i].uvi[j].u), 1, hFile ) != 1 )
                goto HANDLE_ERROR;
                
            if( fread( &triList[i].uvi[j].v, sizeof(triList[i].uvi[j].v), 1, hFile ) != 1 )
                goto HANDLE_ERROR;
        }
        
        // Read in the texture index
        if( fread( &triList[i].tIndex, sizeof(triList[i].tIndex), 1, hFile ) != 1 )
            goto HANDLE_ERROR;

        // Calculate the Surface Normal(surface description)
        triList[ i ].CalcSurfaceNormal(); 
    }

    // Do we calculate the normals on the fly if we are not loading from file
    if( calNormals == true )
    {
        // Setup the shaded vertices with it's own averaged Surface Normal(surface description)
        CalVertexNormals();

        // Calculate the RGB color intensity per vertex. Only do this
        // here if we have a light list and we are NOT doing dynamic lightning.
        // Dynamic lighting is done on each render cycle so there's no need
        // to do it here on startup.
        if( pLightList != NULL && pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
        {
            for( i = 0; i < triCount; ++i )
                triList[i].CalcLightIntenStatic( pLightList );
        }
    }

    // If a light list was not specified, just apply an ambient color
    if( pLightList == NULL ) 
        SetAmbientLightLevel( 255, 255, 255 );

    // Calculate the bounding sphere for each triangle
    for( i = 0; i < triCount; ++i )
        triList[i].CalcBoundingTri();

    // Calculate a bounding sphere for this object
    CalBoundingSphere();

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );
    
    return result;

}   // LoadFromFile


/************************************************************************
*    FUNCTION NAME:         LoadFromPointer                                                             
*
*    DESCRIPTION:           Load an object from data pointer. This is
*                           our own simple file format for quickly loading
*                           vertices, face indexes, uv and texture indexes
*                           into the texture library.
*
*    FUNCTION PARAMETERS:
*    Input:    unsigned char *pData - data pointer
*              bool calNormalse - Calculate normals on the fly
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMeshBone::LoadFromPointer( unsigned char *pData, bool calNormals )
{
    bool result = false;
    int i, j, vIndex;

    // Free all allocations
    Free();

    // Get the number of vertices and triangles
    vCount = *(int *)pData;
    pData += sizeof( int );
    triCount = *(int *)pData;
    pData += sizeof( int );

    // Allocate the vertex list
    vList = new CPoint3D[ vCount ];
    if( vList == NULL )
        goto HANDLE_ERROR;

    // Allocate the projected flag list
    fProjected = new bool[ vCount ];
    if( fProjected == NULL )
        goto HANDLE_ERROR;

    // Allocate the triangle list
    triList = new CTriangle[ triCount ];
    if( triList == NULL )
        goto HANDLE_ERROR;

    // Allocate the normal list
    nList = new CPoint[ triCount ];
    if( nList == NULL )
        goto HANDLE_ERROR;

    // Setup the normals with the pointers
    for( i = 0; i < triCount; ++i )
        triList[i].pNormal = &nList[i];

    // Load the globat vertex list for this object
    for( i = 0; i < vCount; ++i )
    {
        vList[i].tx = vList[i].x = *(float *)pData;
        pData += sizeof( float );

        vList[i].ty = vList[i].y = *(float *)pData;
        pData += sizeof( float );

        vList[i].tz = vList[i].z = *(float *)pData;
        pData += sizeof( float );  
    }

    // Load the triangle data
    for( i = 0; i < triCount; ++i )
    {
        // Load the vertex index and set the pointer
        for( j = 0; j < 3; ++j )
        {
            // Read in the vertex index
            vIndex = *(int *)pData;
            pData += sizeof( int );

            // Set the pointers
            triList[i].pVPoint[j] = &vList[vIndex];
            triList[i].pfProjected[j] = &fProjected[vIndex];
        }
        
        // Load the UV corridinates
        for( j = 0; j < 3; ++j )
        {
            triList[i].uvi[j].u = *(float *)pData;
            pData += sizeof( float );

            triList[i].uvi[j].v = *(float *)pData;
            pData += sizeof( float );
        }
        
        // Read in the texture index
        triList[i].tIndex = *(int *)pData;
        pData += sizeof( int );
            
        // Calculate the surface description
        triList[ i ].CalcSurfaceNormal(); 
    }

    // Do we calculate the normals on the fly
    if( calNormals == true )
    {
        // Setup the shaded vertices with it's own averaged surface description
        CalVertexNormals();

        // Calculate the RGB color intensity per vertex. Only do this
        // here if we have a light list and we are NOT doing dynamic lightning.
        // Dynamic lighting is done on each render cycle so there's no need
        // to do it here on startup.
        if( pLightList != NULL && pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
        {
            for( i = 0; i < triCount; ++i )
                triList[i].CalcLightIntenStatic( pLightList );
        }
    }

    // If a light list was not specified, just apply an ambient color
    if( pLightList == NULL ) 
        SetAmbientLightLevel( 255, 255, 255 );

    // Calculate the bounding sphere for each triangle
    for( i = 0; i < triCount; ++i )
        triList[i].CalcBoundingTri();

    // Did we enable object culling?
    CalBoundingSphere();

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;
    
    return result;

}   // LoadFromPointer