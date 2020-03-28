
/************************************************************************
*
*    PRODUCT:         Software Rendering 3D Engine based on triangles only.
*
*    FILE NAME:       CMeshTri.cpp
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
#include "CMeshTri.h"        // Header file for this *.cpp file.

// Globals
extern float gfMaxZDistance;
extern float gfMinZDistance;
extern float gfCameraScaleX;
extern float gfCameraScaleY;


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

CMeshTri::CMeshTri()
{
    vCount = 0;
    triCount = 0;
    vList = NULL;
	fProjected = NULL;
    triList = NULL;
    nList = NULL;
    dynamicLight = EDL_NO_DYNAMIC_LIGHTNING;
    radius = 0.0;
    pTextLib = NULL;
    pLightList = NULL;
    // Default to 16 bit color
    intensityCap = 31;
    intensityShift = 5;
    renderBitCount = 16;
    ZbufType = EZ_ZBUFFER_TRI;
    shadeType = ES_NO_SHADING;
    renderType = ERT_TEXT_RENDER;
    rgbColor16 = 0;
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

CMeshTri::~CMeshTri()
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

void CMeshTri::Free()
{
    if( vList != NULL )
    {
        delete [] vList;
        vList = NULL;
        vCount = 0;
    }

	if( fProjected != NULL )
    {
        delete [] fProjected;
        fProjected = NULL;
    }

    if( triList != NULL )
    {
        delete [] triList;
        triList = NULL;
        triCount = 0;
    }

    if( nList != NULL )
    {
        delete [] nList;
        nList = NULL;
    }

}   // Free


/************************************************************************
*    FUNCTION NAME:         CalBoundingSphere()                                                             
*
*    DESCRIPTION:           Calculate a bounding sphere around this object.
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
*    H Rosenorn    09/24/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMeshTri::CalBoundingSphere()
{
    long double maxDistance = 0.0;
    long double curDistance;
    CPoint pt;
    CPoint closeDist, maxDist;

    // init
    closeDist.x = vList[0].x;
    maxDist.x = vList[0].x;
    closeDist.y = vList[0].y;
    maxDist.y = vList[0].y;
    closeDist.z = vList[0].z;
    maxDist.z = vList[0].z;

    // Find the closest and farthest distances
    // for each axis.
    for( int i = 1; i < vCount; ++i )
    {
        if( vList[i].x < closeDist.x )
            closeDist.x = vList[i].x;

        else if( vList[i].x > maxDist.x )
            maxDist.x = vList[i].x;

        if( vList[i].y < closeDist.y )
            closeDist.y = vList[i].y;

        else if( vList[i].y > maxDist.y )
            maxDist.y = vList[i].y;

        if( vList[i].z < closeDist.z )
            closeDist.z = vList[i].z;

        else if( vList[i].z > maxDist.z )
            maxDist.z = vList[i].z;
    }

    // Record the center point
    centerPt.x = (closeDist.x + maxDist.x) / 2;
    centerPt.y = (closeDist.y + maxDist.y) / 2;
    centerPt.z = (closeDist.z + maxDist.z) / 2;
    
    // Reguardless of where the object is,
    // we need to calculate the radius as if the
    // object's center was at 0,0,0
    for( int i = 0; i < vCount; ++i )
    {
        // Calculate a new position where the center is 0,0,0
        pt.x = vList[i].x - centerPt.x;
        pt.y = vList[i].y - centerPt.y;
        pt.z = vList[i].z - centerPt.z;
        
        // Add the vertex
        curDistance = (pt.x * pt.x) +
                      (pt.y * pt.y) +
                      (pt.z * pt.z);

        // Have we found the longest distance?
        if( curDistance > maxDistance )
            maxDistance = curDistance;
    }

    // computer the radius
    radius = (float)sqrt( maxDistance );

}   // CalBoundingSphere


/************************************************************************
*    FUNCTION NAME:         CalVertexNormals()                                                             
*
*    DESCRIPTION:           Upon entering this function, the Surface Normal
*                           has been calculated for each triangle. Before it is
*                           normalized, we find all the shared vertices for
*                           a given vertex and add up all the Surface Normals
*                           to average it out for that one vertex. We then
*                           normalize it. This is how we calculate a normal
*                           for each vertex. This is needed to do dynamic
*                           Gouraud shading.
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
*    H Rosenorn    06/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMeshTri::CalVertexNormals()
{
    int sharedIndex[300], sharedIndexCount, index;
    float x, y, z;
    long double nx, ny, nz; 

    // Calculate normals per vertex for all polygons
    for( int tri = 0; tri < triCount; ++tri )
    {           
        // Find shared vertices, add them up and average per vertex
        for( int vIndex = 0; vIndex < 3; ++vIndex )
        {
            sharedIndexCount = 0;
            nx = ny = nz = 0.0;

            // Search through the entire list of triangles looking 
            // for shared vertices and record it's index
            for( int i = 0; i < triCount; ++i )
            {
                if( triList[ i ].HasVert( *triList[ tri ].pVPoint[ vIndex ] ) )
                    sharedIndex[ sharedIndexCount++ ] = i;
            }

            // Have to have atleast one
            if( !sharedIndexCount )
            {
                sharedIndexCount = 1;
                sharedIndex[ 0 ] = tri;
            }

            // Add up all the Surface Normals of all the shared vertices
            for( int i = 0; i < sharedIndexCount; ++i )
            {
                index = sharedIndex[ i ];
                nx += (long double)triList[ sharedIndex[ i ] ].pNormal->x - triList[ index ].pVPoint[ 0 ]->x;
                ny += (long double)triList[ sharedIndex[ i ] ].pNormal->y - triList[ index ].pVPoint[ 0 ]->y;
                nz += (long double)triList[ sharedIndex[ i ] ].pNormal->z - triList[ index ].pVPoint[ 0 ]->z;
            }

            // Average out the accumlated Surface Normals
            // to end up with a Surface Normal for each vertex
            x = (float)((nx / (long double)sharedIndexCount) + triList[ tri ].pVPoint[ vIndex ]->x);
            y = (float)((ny / (long double)sharedIndexCount) + triList[ tri ].pVPoint[ vIndex ]->y);
            z = (float)((nz / (long double)sharedIndexCount) + triList[ tri ].pVPoint[ vIndex ]->z);

            // Record the vertex normal for light calculations
            triList[ tri ].uvi[ vIndex ].n.tx = triList[ tri ].uvi[ vIndex ].n.x = x;
            triList[ tri ].uvi[ vIndex ].n.ty = triList[ tri ].uvi[ vIndex ].n.y = y;
            triList[ tri ].uvi[ vIndex ].n.tz = triList[ tri ].uvi[ vIndex ].n.z = z;
        }
    }

}   // CalVertexNormals


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

bool CMeshTri::LoadFromFile( char *filePath, bool calNormals )
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

    // Read in the first two ints for the number panels and verticies
    if( fread( &vCount, sizeof( vCount ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    if( fread( &triCount, sizeof( triCount ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

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
        if( pLightList != NULL && dynamicLight == EDL_NO_DYNAMIC_LIGHTNING )
        {
            for( i = 0; i < triCount; ++i )
                triList[i].CalcLightInten( pLightList, intensityCap, intensityShift );
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

bool CMeshTri::LoadFromPointer( unsigned char *pData, bool calNormals )
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
        if( pLightList != NULL && dynamicLight == EDL_NO_DYNAMIC_LIGHTNING )
        {
            for( i = 0; i < triCount; ++i )
                triList[i].CalcLightInten( pLightList, intensityCap, intensityShift );
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


/************************************************************************
*    FUNCTION NAME:         SaveNormals()                                                             
*
*    DESCRIPTION:           Save all the normals to a file. Helpful for
*                           quick loading of large data sets.
*
*    FUNCTION PARAMETERS:
*    Input:    char *filePath - Path to file to load
*              var1type Var1Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMeshTri::SaveNormals( char *filePath )
{
    bool result = false;
    FILE *hFile;
    int i, j;

    // Create the file
    hFile = fopen ( filePath, "wb" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    // Write the number of triangles
    if( fwrite( &triCount, sizeof( triCount ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    for( i = 0; i < triCount; ++i )
    {
        // Write the surface normals
        if( fwrite( &nList[i].x, sizeof( float ), 3, hFile ) != 3 )
            goto HANDLE_ERROR;

        // Write the vertex normals
        for( j = 0; j < 3; ++j )
        {
            if( fwrite( &triList[i].uvi[j].n.x, sizeof( float ), 3, hFile ) != 3 )
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
}   // SaveNormals


/************************************************************************
*    FUNCTION NAME:         LoadNormals()                                                             
*
*    DESCRIPTION:           Load the normals from a file.
*
*    FUNCTION PARAMETERS:
*    Input:    char *filePath - Path to file to load
*              var1type Var1Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMeshTri::LoadNormals( char *filePath )
{
    bool result = false;
    FILE *hFile;
    int tmpTriCount, i, j;

    // Create the file
    hFile = fopen ( filePath, "rb" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    // Read the number of triangles
    if( fread( &tmpTriCount, sizeof( tmpTriCount ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Make sure we have the same number of triangles here
    if( tmpTriCount != triCount )
        goto HANDLE_ERROR;

    for( i = 0; i < triCount; ++i )
    {
        // Read the surface normals
        if( fread( &nList[i].x, sizeof( float ), 3, hFile ) != 3 )
            goto HANDLE_ERROR;

        // Make a copy to the transformed normals for startup inits
        nList[i].tx = nList[i].x;
        nList[i].ty = nList[i].y;
        nList[i].tz = nList[i].z;

        for( j = 0; j < 3; ++j )
        {
            if( fread( &triList[i].uvi[j].n.x, sizeof( float ), 3, hFile ) != 3 )
                goto HANDLE_ERROR;

            // Make a copy to the transformed normals for startup inits
            triList[i].uvi[j].n.tx = triList[i].uvi[j].n.x;
            triList[i].uvi[j].n.ty = triList[i].uvi[j].n.y;
            triList[i].uvi[j].n.tz = triList[i].uvi[j].n.z;
        }
    }

    // Calculate the RGB color intensity per vertex. Only do this
    // here if we have a light list and we are NOT doing dynamic lightning.
    // Dynamic lighting is done on each render cycle so there's no need
    // to do it here on startup.
	if( pLightList != NULL && dynamicLight == EDL_NO_DYNAMIC_LIGHTNING )
        {
            for( i = 0; i < triCount; ++i )
                triList[i].CalcLightInten( pLightList, intensityCap, intensityShift );
        }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );

    return result;

}   // LoadNormals


/************************************************************************
*    FUNCTION NAME:         SetAmbientLightLevel()
*
*    DESCRIPTION:           Set an ambient light level for this object.
*                           If your setting all colors to their full
*                           brightness, ie. 255, it would be faster
*                           to just blit the object with no shading.
*
*    FUNCTION PARAMETERS:
*    Input:    unsigned int rColor - Red color
*              unsigned int gColor - Green color
*              unsigned int bColor - Blue Color
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

void CMeshTri::SetAmbientLightLevel( unsigned int rColor, unsigned int gColor, unsigned int bColor )
{
    for( int tri = 0; tri < triCount; ++tri )
    {
        for( int vIndex = 0; vIndex < 3; ++vIndex )
        {
            // record the color intensity for this vertex
            triList[tri].uvi[ vIndex ].i = 31;
        }
    }

}   // SetAmbientLightLevel


/************************************************************************
*    FUNCTION NAME:         CullMesh_BoundSphere()                                                             
*
*    DESCRIPTION:           Test position of object in frustrum to see
*                           if it can be culled. This assumes a 45 degree
*                           view area.
*
*    FUNCTION PARAMETERS:
*    Input:    Matrix3D &M - Matris to do all the translations
*              var2type Var2Name - Description
*
*    Output:   bool - True indicates object is outsidefrustrum
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall CMeshTri::CullMesh_BoundSphere( CMatrix &m, float cullPercent )
{
    // Set the flag that indicates the object was
    // culled in case we exit early
    culled = true;

    // Set 0.0 to indicate the object was culled
    distance = 0.0;

    // Tansform the center point of the mesh
    m.Transform( centerPt );

    // Test all sides of the view frustrum
    if( centerPt.tz + radius < gfMaxZDistance )
        return;

    if( centerPt.tz - radius > gfMinZDistance )
        return;


    // Find the farthest Z point
    float farPoint = ( centerPt.tz - radius ) * cullPercent;

    if( centerPt.tx + radius < farPoint )
        return;

    if( centerPt.tx - radius > -farPoint )
        return;

    farPoint = ( centerPt.tz - radius ) * (cullPercent - 0.1);
    
    if( centerPt.ty + radius < farPoint )
        return;

    if( centerPt.ty - radius > -farPoint )
        return;

    // if we made it this far, the object was not culled
    culled = false;

    // Calculate the distance of the center point
    distance = fabs( sqrt( ( centerPt.tx  *  centerPt.tx ) +
                       ( centerPt.ty  *  centerPt.ty ) +
                       ( centerPt.tz  *  centerPt.tz ) ) );

}   // CullMesh_BoundSphere


/************************************************************************
*    FUNCTION NAME:         Render()                                                             
*
*    DESCRIPTION:           Render the 3D object to a screen buffer.
*
*    FUNCTION PARAMETERS:
*    Input:    Matrix3D &M - Matris to do all the translations
*              var2type Var2Name - Description
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

int CMeshTri::Render( CMatrix &m, float cullPercent )
{   
    int renderCount(0);

    // Transform all the vertices
    m.Transform( vList, vCount );

    // Transform all the normals
    m.Transform( nList, triCount );

    // Clear the projected flags
    memset( fProjected, 0, sizeof(bool) * vCount );

    // Do we have textures?
    if( pTextLib != NULL )
    {
        if( pLightList == NULL || pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
        {            
            // Display selected triangles in the off-screen buffer
            for( int i = 0; i < triCount; ++i )
            {
	            // Eliminate triangles that are not seen or have their backs to us
		        if( triList[i].CalcVisible3D( cullPercent ) )
		        {
		            // Clip and project the points
		            triList[i].ClipProjectXYZ();

		            // See if the triangle is visible in the video buffer
		            if( triList[i].CalcVisible2D() )
		            {
						// The projection of the UVI can wait til the last moment
                		triList[i].ClipProjectUVI();

						triList[i].Render16_ZBufShading( pTextLib );

						// Count up the number of triangles sent to be rendered
                    	++renderCount;
					}
				}
			}
        }                   
        else if( pLightList->GetDynamicLightType() == EDL_LOCAL_DYNAMIC_LIGHTNING )
        {
       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {

        }
    }
    else   // Render with no textures
    {
        if( pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
        {

        }                   
        else if( pLightList->GetDynamicLightType() == EDL_LOCAL_DYNAMIC_LIGHTNING )
        {
       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {

        }
    }

    return renderCount;

}   // Render


/************************************************************************
*    FUNCTION NAME:         SetTextureLibrary()                                                             
*
*    DESCRIPTION:           Set the texture library.
*
*    FUNCTION PARAMETERS:
*    Input:    CTextureLib &textLib - Set the texture library
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    08/01/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMeshTri::SetTextureLibrary( CTextureLib &textLib )
{
    // get the pointer to the texture library
    pTextLib = &textLib;

}   // SetTextureLibrary


/************************************************************************
*    FUNCTION NAME:         SetLightList()                                                             
*
*    DESCRIPTION:           Set the light list.
*
*    FUNCTION PARAMETERS:
*    Input:    CLightList &lightList - Light list
*              EDyn_Light enableState - Dynamic light type
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

void CMeshTri::SetLightList( CLightList &lightList, EDyn_Light enableState )
{
    // Set the light list
    pLightList = &lightList;

    // Set the dynamic light type
    dynamicLight = enableState;

}   // SetLightList    


/************************************************************************
*    FUNCTION NAME:         Generate16BitShadedTable()                                                             
*
*    DESCRIPTION:           Generate a 16 bit shaded table
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

void Generate16BitShadedTable( unsigned short int *shadeTbl )
{
    int textel_R, textel_G, textel_B;

    for( int color = 0; color < COLOR_COUNT_16_BIT; ++color )
    {
        for( int i = 0; i < SHADE_COUNT; ++i )
        {
            // Calculate the shaded 16 bit RGB color
            textel_R = (color & 0x7C00) >> 10;
            textel_G = (color & 0x3E0) >> 5;
            textel_B = color & 0x1F;
            
            // Inc the 16 bit RGB color
            textel_R -= i;
            textel_G -= i;
            textel_B -= i;
            
            if( textel_R < 0 )
                textel_R = 0;
                
            if( textel_G < 0 )
                textel_G = 0;

            if( textel_B < 0 )
                textel_B = 0; 
             
            // Save the color to the shaded table
            shadeTbl[ ( ((SHADE_COUNT-1)-i) * COLOR_COUNT_16_BIT ) + color ] = (textel_R << 10) | (textel_G << 5) | textel_B;
        }
    }
}   // Generate16BitShadedTable