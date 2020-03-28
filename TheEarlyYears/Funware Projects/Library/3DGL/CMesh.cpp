
/************************************************************************
*
*    PRODUCT:         Mesh class
*
*    FILE NAME:       CMesh.cpp
*
*    DESCRIPTION:     base mesh class with all general mesh attributes
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

#include "CMesh.h"           // Header file for this *.cpp file.

// Globals
extern float gfMaxZDistance;
extern float gfMinZDistance;
extern float gfAspectRatio;

#include "gl/gl.h"           // standard OpenGL include
#include "gl/glu.h"          // OpenGL utilities


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04             Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CMesh::CMesh()
{
    // init class members
    vCount = 0;
    polyCount = 0;
    vList = NULL;
    pList = NULL;
    nList = NULL;
    radius = 0.0;
    pLightList = NULL;
    pTextLib = NULL;
    culled = false;
    distance = 0.0;
    id = 0;
    type = 0;

}   // Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CMesh::~CMesh()
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
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMesh::Free()
{
    if( vList != NULL )
    {
        delete [] vList;
        vList = NULL;
        vCount = 0;
    }

    if( pList != NULL )
    {
        delete [] pList;
        pList = NULL;
        polyCount = 0;
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
*                           Handy for full object culling against the 
*                           view frustum.
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
*    H Rosenorn    05/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMesh::CalBoundingSphere()
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
    centerPt.x = (closeDist.x + maxDist.x) * 0.5;
    centerPt.y = (closeDist.y + maxDist.y) * 0.5;
    centerPt.z = (closeDist.z + maxDist.z) * 0.5;
    
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
*    FUNCTION NAME:         GetRadiusX()                                                             
*
*    DESCRIPTION:           Calculate the translated runtime radius X
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   float - runtime x radius
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

float CMesh::GetRadiusX()
{
    long double maxDistance = 0.0;
    long double curDistance;
    CPoint pt;
    CPoint closeDist, maxDist;

    // init
    closeDist.tx = vList[0].tx;
    maxDist.tx = vList[0].tx;

    // Find the closest and farthest distances
    // for each axis.
    for( int i = 1; i < vCount; ++i )
    {
        if( vList[i].tx < closeDist.tx )
            closeDist.tx = vList[i].tx;

        else if( vList[i].tx > maxDist.tx )
            maxDist.tx = vList[i].tx;
    }

    // Record the center point
    centerPt.tx = (closeDist.tx + maxDist.tx) * 0.5;
    
    // Reguardless of where the object is,
    // we need to calculate the radius as if the
    // object's center was at 0,0,0
    for( int i = 0; i < vCount; ++i )
    {
        // Calculate a new position where the center is 0
        pt.tx = vList[i].tx - centerPt.tx;
        
        // Add the vertex
        curDistance = (pt.tx * pt.tx) + (pt.tx * pt.tx) + (pt.tx * pt.tx);

        // Have we found the longest distance?
        if( curDistance > maxDistance )
            maxDistance = curDistance;
    }

    // computer the radius
    return (float)sqrt( maxDistance );

}   // GetRadiusX


/************************************************************************
*    FUNCTION NAME:         GetRadiusY()                                                             
*
*    DESCRIPTION:           Calculate the translated runtime radius Y
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   float - runtime y radius
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    05/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

float CMesh::GetRadiusY()
{
    long double maxDistance = 0.0;
    long double curDistance;
    CPoint pt;
    CPoint closeDist, maxDist;

    // init
    closeDist.ty = vList[0].ty;
    maxDist.ty = vList[0].ty;

    // Find the closest and farthest distances
    // for each axis.
    for( int i = 1; i < vCount; ++i )
    {
        if( vList[i].ty < closeDist.ty )
            closeDist.ty = vList[i].ty;

        else if( vList[i].ty > maxDist.ty )
            maxDist.ty = vList[i].ty;
    }

    // Record the center point
    centerPt.ty = (closeDist.ty + maxDist.ty) * 0.5;
    
    // Reguardless of where the object is,
    // we need to calculate the radius as if the
    // object's center was at 0,0,0
    for( int i = 0; i < vCount; ++i )
    {
        // Calculate a new position where the center is 0
        pt.ty = vList[i].ty - centerPt.ty;
        
        // Add the vertex
        curDistance = (pt.ty * pt.ty) + (pt.ty * pt.ty) + (pt.ty * pt.ty);

        // Have we found the longest distance?
        if( curDistance > maxDistance )
            maxDistance = curDistance;
    }

    // computer the radius
    return (float)sqrt( maxDistance );

}   // GetRadiusY


/************************************************************************
*    FUNCTION NAME:         CalcVertexNormals()                                                             
*
*    DESCRIPTION:           Upon entering this function, the Surface Normal
*                           has been calculated for each polygon. We find
*                           all the shared vertices for a given vertex,
*                           add it all up, and average it out for one
*                           vertex. This is how we calculate a normal for
*                           each vertex. This is needed to do dynamic shading.
*
*                           Note: This is time consuming on large data sets.
*                           It is suggested you do all your normal and vertex
*                           normal calculations offline.
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
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMesh::CalcVertexNormals()
{
    int sharedIndex[300], sharedIndexCount, index;
    long double nx, ny, nz; 

    // Calculate normals per vertex for all polygons
    for( int poly = 0; poly < polyCount; ++poly )
    {           
        // Find shared vertices, add them up and average per vertex
        for( int vIndex = 0; vIndex < pList[poly].GetVertexCount(); ++vIndex )
        {
            sharedIndexCount = 0;
            nx = ny = nz = 0.0;

            // Search through the entire list of polygons looking 
            // for shared vertices and record it's index
            for( int i = 0; i < polyCount; ++i )
            {
                if( pList[ i ].HasVert( *pList[ poly ].GetVPoint( vIndex ) ) )
                {
                    sharedIndex[ sharedIndexCount++ ] = i;
                }
            }

            // Have to have atleast one
            if( sharedIndexCount == 0 )
            {
                sharedIndexCount = 1;
                sharedIndex[ 0 ] = poly;
            }

            // Add up all the Surface Normals of all the shared vertices
            // We are subtracting the vertex point from the normal to get the true
            // normal of the polygon.
            for( int i = 0; i < sharedIndexCount; ++i )
            {
                index = sharedIndex[ i ];
                nx += (long double)(pList[ index ].GetNormal()->x - pList[ index ].GetVPoint( 0 )->x);
                ny += (long double)(pList[ index ].GetNormal()->y - pList[ index ].GetVPoint( 0 )->y);
                nz += (long double)(pList[ index ].GetNormal()->z - pList[ index ].GetVPoint( 0 )->z);
            }

            // Average out the accumlated Surface Normals
            // to end up with a Surface Normal for each vertex
            // We also add in the point so that this normal can be
            // easily translated like a point.
            CPoint n;
            n.tx = n.x = (float)((nx / (long double)sharedIndexCount) + pList[ poly ].GetVPoint( vIndex )->x);
            n.ty = n.y = (float)((ny / (long double)sharedIndexCount) + pList[ poly ].GetVPoint( vIndex )->y);
            n.tz = n.z = (float)((nz / (long double)sharedIndexCount) + pList[ poly ].GetVPoint( vIndex )->z);

            // Record the vertex normal for light calculations
            pList[ poly ].SetVertexNormal( vIndex, n );
        }
    }

}   // CalcVertexNormals


/************************************************************************
*    FUNCTION NAME:         CalcGridVertexNormals()                                                             
*
*    DESCRIPTION:           Upon entering this function, the Surface Normal
*                           has been calculated for each polygon.
*
*                           This function is specific for a terrain grid.
*
*                           This function is optimized for fast vertex normals
*                           that happen to be in a grid like a terrain for example
*
*    FUNCTION PARAMETERS:
*    Input:    int width - Width of grid in points
*              var2type Var2Name - Description
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

void CMesh::CalcGridVertexNormals( int width )
{
    int sharedIndex[300], sharedIndexCount, index;
    long double nx, ny, nz; 

    // Calculate normals per vertex for all polygons
    for( int poly = 0; poly < polyCount; ++poly )
    {       
        // Find shared vertices, add them up and average per vertex
        for( int vIndex = 0; vIndex < 3; ++vIndex )
        {
            sharedIndexCount = 0;
            nx = ny = nz = 0.0;

            // Search only the suranding polygons 
            // for shared vertices and record it's index
            // Front half
            for( int i = poly-width-4; i < (poly-width)+4; ++i )
            {
                if( i > -1 && i < polyCount )
                {
                    if( pList[ i ].HasVert( *pList[ poly ].GetVPoint( vIndex ) ) )
                        sharedIndex[ sharedIndexCount++ ] = i;
                }
            }

            // Middle
            for( int i = poly-4; i < poly+4; ++i )
            {
                if( i > -1 && i < polyCount )
                {
                    if( pList[ i ].HasVert( *pList[ poly ].GetVPoint( vIndex ) ) )
                        sharedIndex[ sharedIndexCount++ ] = i;
                }
            }

            // Back
            for( int i = (poly+width)-4; i < poly+width+4; ++i )
            {
                if( i > -1 && i < polyCount )
                {
                    if( pList[ i ].HasVert( *pList[ poly ].GetVPoint( vIndex ) ) )
                        sharedIndex[ sharedIndexCount++ ] = i;
                }
            }

            // Have to have atleast one
            if( sharedIndexCount == 0 )
            {
                sharedIndexCount = 1;
                sharedIndex[ 0 ] = poly;
            }

            // Add up all the Surface Normals of all the shared vertices
            for( int i = 0; i < sharedIndexCount; ++i )
            {
                index = sharedIndex[ i ];
                nx += (long double)pList[ index ].GetNormal()->x - pList[ index ].GetVPoint( 0 )->x;
                ny += (long double)pList[ index ].GetNormal()->y - pList[ index ].GetVPoint( 0 )->y;
                nz += (long double)pList[ index ].GetNormal()->z - pList[ index ].GetVPoint( 0 )->z;
            }

            // Average out the accumlated Surface Normals
            // to end up with a Surface Normal for each vertex
            // We also add in the point so that this normal can be
            // easily translated like a point.
            CPoint n;
            n.tx = n.x = (float)((nx / (long double)sharedIndexCount) + pList[ poly ].GetVPoint( vIndex )->x);
            n.ty = n.y = (float)((ny / (long double)sharedIndexCount) + pList[ poly ].GetVPoint( vIndex )->y);
            n.tz = n.z = (float)((nz / (long double)sharedIndexCount) + pList[ poly ].GetVPoint( vIndex )->z);

            // Record the vertex normal for light calculations
            pList[ poly ].SetVertexNormal( vIndex, n );
        }
    }

}   // CalcGridVertexNormals


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
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    05/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMesh::LoadFromFile( char *filePath )
{
    bool result = false;

    // Open the file
    FILE * hFile = fopen( filePath, "rb" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    // Load the vertex and face info from file
    if( LoadMeshFromFile( hFile ) == false )
        goto HANDLE_ERROR;

    // Load the face and vertex normals from file
    if( LoadNormalsFromFile( hFile ) == false )
        goto HANDLE_ERROR;

    // If we made it this far, all is good
    result = true;
    
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
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMesh::LoadFromFile( FILE *hFile )
{
    bool result = false;

    // Load the vertex and face info from file
    if( LoadMeshFromFile( hFile ) == false )
        goto HANDLE_ERROR;

    // Load the face and vertex normals from file
    if( LoadNormalsFromFile( hFile ) == false )
        goto HANDLE_ERROR;

    // If we made it this far, all is good
    result = true;
    
HANDLE_ERROR:;
    
    return result;

}   // LoadFromFile


/************************************************************************
*    FUNCTION NAME:         LoadMeshFromFile()                                                             
*
*    DESCRIPTION:           Load an object from a PLY binary file. This is
*                           our own simple file for quickly loading
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
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMesh::LoadMeshFromFile( char *filePath )
{
    bool result = false;

    // Open the file
    FILE * hFile = fopen( filePath, "rb" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    // Load the vertex and face info from file
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
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMesh::LoadMeshFromFile( FILE *hFile )
{    
    bool result = false;
    int i;

    // Format data sizes
    CVertLoad vertLoad;
    char formatType[FORMAT_SPEC_STR_COUNT];

    // Free all allocations
    Free();

    // Load in the header so that we can check it
    if( fread( formatType, sizeof( formatType ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Check the header to make sure this is not random data
    if( strcmp( formatType, MESH_RAW_FORMAT_STR_ID ) != 0 )
        goto HANDLE_ERROR;

    // Read in the mesh type
    if( fread( &type, sizeof( type ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Read in the mesh id
    if( fread( &id, sizeof( id ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // load extra data
    if( !LoadExtraMeshData( hFile ) )
        goto HANDLE_ERROR;
    
    // Read in the vertex count
    if( fread( &vCount, sizeof( vCount ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Read in the polygon count
    if( fread( &polyCount, sizeof( polyCount ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Allocate the vertex list
    vList = new CPoint[ vCount ];
    if( vList == NULL )
        goto HANDLE_ERROR;

    // Allocate the polygon array
    pList = AllocatePolygonArray();
    if( pList == NULL )
        goto HANDLE_ERROR;

    // Allocate the normal list. We are not loading any
    // normals here but the normal count is the same as
    // the number of polygons.
    nList = new CPoint[ polyCount ];
    if( nList == NULL )
        goto HANDLE_ERROR;

    // Setup the normals with the pointers
    // 1 to 1 ratio
    for( i = 0; i < polyCount; ++i )
    {
        pList[i].SetNormal( &nList[i] );
    }

    // Load the global vertex list for this object. In a given mesh,
    // polygons share vertexs. This means the least amount of points
    // to translate.
    for( i = 0; i < vCount; ++i )
    {
        if( fread( &vertLoad, sizeof(vertLoad), 1, hFile ) != 1 )
            goto HANDLE_ERROR;
            
        // Make a copy to the transformed vertexes for startup inits
        vList[i].ty = vList[i].x = vertLoad.vert[0];
        vList[i].tx = vList[i].y = vertLoad.vert[1];
        vList[i].tz = vList[i].z = vertLoad.vert[2];  
    }       

    // Load the polygon data
    if( !LoadPolygonData( hFile ) )
        goto HANDLE_ERROR;

    // Calculate a bounding sphere for each polygon. Used for collision
    // detection and view frustum culling
    CalBoundingPoly();

    // Calculate the bounding sphere for this object. Useful for
    // doing full object culling.
    CalBoundingSphere();

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;
    
    return result;

}   // LoadMeshFromFile


/************************************************************************
*    FUNCTION NAME:         LoadExtraMeshData()                                                             
*
*    DESCRIPTION:           Virtual function for loading inherited class
*                           specific data
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
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMesh::LoadExtraMeshData( FILE *hFile )
{
    return true;

}   // LoadExtraMeshData


/************************************************************************
*    FUNCTION NAME:         AllocatePolygonArray()                                                             
*
*    DESCRIPTION:           Allocate the polygon array. Virtual call so that
*                           an inherited class can load up it's own polygon
*                           class.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var2Name - Description
*              var2type Var2Name - Description
*
*    Output:   CPolygon * - Pointer to polygon array
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CPolygon * CMesh::AllocatePolygonArray()
{
    return new CPolygon[ polyCount ];

}   // AllocatePolygonArray


/************************************************************************
*    FUNCTION NAME:         SaveToFile()                                                             
*
*    DESCRIPTION:           Save mesh and normal data to file. This is
*                           our own simple file for quickly loading
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
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMesh::SaveToFile( char *filePath )
{
    bool result = false;

    // Open the file
    FILE * hFile = fopen( filePath, "wb" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    // Load the vertex and face info from file
    if( SaveMeshToFile( hFile ) == false )
        goto HANDLE_ERROR;

    // Load the face and vertex normals from file
    if( SaveNormalsToFile( hFile ) == false )
        goto HANDLE_ERROR;

    // If we made it this far, all is good
    result = true;
    
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
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMesh::SaveToFile( FILE *hFile )
{
    bool result = false;

    // Load the vertex and face info from file
    if( SaveMeshToFile( hFile ) == false )
        goto HANDLE_ERROR;

    // Load the face and vertex normals from file
    if( SaveNormalsToFile( hFile ) == false )
        goto HANDLE_ERROR;

    // If we made it this far, all is good
    result = true;
    
HANDLE_ERROR:;
    
    return result;

}   // SaveToFile


/************************************************************************
*    FUNCTION NAME:         GenerateGrid()                                                             
*
*    DESCRIPTION:           Generate a terrain grid.
*
*    FUNCTION PARAMETERS:
*    Input:    int x - Number of points X
*              int y - Number of points Y
*              int yOffset - y offset of points
*              float multiplyer - multiply the hight values
*              float size - spacing between each point
*              unsigned char * hightData -  height map data. Height map
*                                           data from an 8 bit bmp works well.
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMesh::GenerateGrid( int x, int z, int yOffset, float multiplyer, float size, unsigned char * hightData )
{
    bool result = false;
    int i, j, vCounter=0, width;
    float xOffset, zOffset;

    // Free all allocations
    Free();

    vCount = x * z;
    width = (x-1) * 2;
    polyCount = width * (z-1);

    // Allocate the vertex list
    vList = new CPoint[ vCount ];
    if( vList == NULL )
        goto HANDLE_ERROR;

    // Allocate the polygon list
    pList = new CPolygon[ polyCount ];
    if( pList == NULL )
        goto HANDLE_ERROR;

    // Allocate the normal list
    nList = new CPoint[ polyCount ];
    if( nList == NULL )
        goto HANDLE_ERROR;

    // calculate the offsets
    xOffset = -(((size-1)/2) * (x-1));
    zOffset = -(((size-1)/2) * (z-1));

    // Set the vertices
    for( i = 0; i < z; ++i )
    {
        for( j = 0; j < x; ++j )
        {
            vList[(i*x) + j].x = xOffset + ((float)j * size);
            vList[(i*x) + j].y = ((float)hightData[(i*x) + j]*multiplyer)-yOffset;
            vList[(i*x) + j].z = -(zOffset + ((float)i * size));

            // Copy to the transformed data
            vList[(i*x) + j].tx = vList[(i*x) + j].x;
            vList[(i*x) + j].ty = vList[(i*x) + j].y;
            vList[(i*x) + j].tz = vList[(i*x) + j].z;
        }
    }

    // Load the polygon data
    for( i = 0; i < polyCount; ++i )
    {
        // Setup the normals with the pointers
        pList[i].SetNormal( &nList[i] );
        
        // Inc to the next line
        if( (i != 0) && ((i % width) == 0) )
            ++vCounter;
        
        // Set the pointer
        if( (i & 0x1) == 0 )
        {            
            pList[i].SetVPoint( 0, &vList[vCounter] );
            pList[i].SetVPoint( 1, &vList[vCounter+1] );
            pList[i].SetVPoint( 2, &vList[vCounter+x] );
        }
        else
        {
            pList[i].SetVPoint( 0, &vList[vCounter+1] );
            pList[i].SetVPoint( 1, &vList[vCounter+1+x] );
            pList[i].SetVPoint( 2, &vList[vCounter+x] );

            ++vCounter;
        }

        // Calculate the Surface Normal(surface description)
        pList[ i ].CalcSurfaceNormal();
    }

    // Setup the shaded vertices with it's own averaged Surface Normal(surface description)
    CalcGridVertexNormals( width );

    // Calculate the RGB color intensity per vertex. Only do this
    // here if we have a light list and we are NOT doing dynamic lightning.
    // Dynamic lighting is done on each render cycle so there's no need
    // to do it here on startup.
    if( pLightList != NULL && pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
    {
        for( i = 0; i < polyCount; ++i )
            pList[i].CalcLightIntenStatic( pLightList );
    }

    for( i = 0; i < polyCount; ++i )
        pList[i].CalcBoundingPoly();

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;
    
    return result;

}   // GenerateGrid


/************************************************************************
*    FUNCTION NAME:         SaveMeshToFile()                                                             
*
*    DESCRIPTION:           Save the mesh to file.
*
*    FUNCTION PARAMETERS:
*    Input:    char *filePath - Path to file to load
*              var1type Var1Name - Description
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

bool CMesh::SaveMeshToFile( char *filePath )
{
    bool result = false;

    // Open the file to append it
    FILE *hFile = hFile = fopen ( filePath, "wb" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    result = SaveMeshToFile( hFile );

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );
    
    return result;

}   // SaveMeshToFile


/************************************************************************
*    FUNCTION NAME:         SaveMeshToFile()                                                             
*
*    DESCRIPTION:           Save the mesh to file.
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - File handle
*              var1type Var1Name - Description
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

bool CMesh::SaveMeshToFile( FILE *hFile )
{
    bool result = false;
    int i;

    // Format data sizes
    CVertLoad vertLoad;
    char formatTypeMeshRaw[FORMAT_SPEC_STR_COUNT];

    // Setup the format id's
    memset( formatTypeMeshRaw, 0, sizeof(formatTypeMeshRaw) );
    strcpy( formatTypeMeshRaw, MESH_RAW_FORMAT_STR_ID );

    // Write out the format ID
    if( fwrite( formatTypeMeshRaw, sizeof( formatTypeMeshRaw ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Write out the object type
    if( fwrite( &type, sizeof( type ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Write out the object ID
    if( fwrite( &id, sizeof( id ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Save extra data
    if( !SaveExtraMeshData( hFile ) )
        goto HANDLE_ERROR;

    // Write out the vertex count
    if( fwrite( &vCount, sizeof( vCount ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Write out the number of polygons
    if( fwrite( &polyCount, sizeof( polyCount ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Save the vertex list
    for( i = 0; i < vCount; ++i )
    {            
        vertLoad.vert[0] = vList[i].x;
        vertLoad.vert[1] = vList[i].y;
        vertLoad.vert[2] = vList[i].z;
        
        if( fwrite( &vertLoad, sizeof(vertLoad), 1, hFile ) != 1 )
            goto HANDLE_ERROR;
    }  
    
    // Save the polygon data
    if( !SavePolygonData( hFile ) )
        goto HANDLE_ERROR;

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // SaveMeshToFile


/************************************************************************
*    FUNCTION NAME:         SaveExtraMeshData()                                                             
*
*    DESCRIPTION:           Virtual function for savinging inherited class
*                           specific data
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
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMesh::SaveExtraMeshData( FILE *hFile )
{
    return true;

}   // SaveExtraMeshData


/************************************************************************
*    FUNCTION NAME:         SaveNormalsToFile()                                                             
*
*    DESCRIPTION:           Save all the normals to a file. Helpful for
*                           quick loading of large data sets.
*
*    FUNCTION PARAMETERS:
*    Input:    char *filePath - Path to file to load
*              var1type Var1Name - Description
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

bool CMesh::SaveNormalsToFile( char *filePath )
{
    bool result = false;

    // Open the file to append it
    FILE *hFile = hFile = fopen ( filePath, "wb" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    result = SaveNormalsToFile( hFile );

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );
    
    return result;

}   // SaveNormals


/************************************************************************
*    FUNCTION NAME:         SaveNormalsToFile()                                                             
*
*    DESCRIPTION:           Save all the normals to a file. Helpful for
*                           quick loading of large data sets.
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - File handle
*              var1type Var1Name - Description
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

bool CMesh::SaveNormalsToFile( FILE *hFile )
{
    bool result = false;
    int i;
    char formatTypeNormRaw[FORMAT_SPEC_STR_COUNT];

    // Setup the format id's
    memset( formatTypeNormRaw, 0, sizeof(formatTypeNormRaw) );
    strcpy( formatTypeNormRaw, NORMAL_RAW_FORMAT_STR_ID );

    // Write out the format ID
    if( fwrite( formatTypeNormRaw, sizeof( formatTypeNormRaw ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Write the number of polygons
    if( fwrite( &polyCount, sizeof( polyCount ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    for( i = 0; i < polyCount; ++i )
    {
        // Write the surface and vertex normals
        if( !pList[i].SaveNormalsToFile( hFile ) )
            goto HANDLE_ERROR;
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

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
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMesh::LoadNormalsFromFile( char *filePath )
{
    bool result = false;

    // Create the file
    FILE *hFile = fopen ( filePath, "rb" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    result = LoadNormalsFromFile( hFile );

HANDLE_ERROR:;
    
    // Close the file
    if( hFile != NULL )
        fclose( hFile );

    return result;

}   // LoadNormals


/************************************************************************
*    FUNCTION NAME:         LoadNormals()                                                             
*
*    DESCRIPTION:           Load the normals from a file.
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - File handle
*              var1type Var1Name - Description
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMesh::LoadNormalsFromFile( FILE *hFile )
{
    bool result = false;
    int i, tmpPolyCount;
    char formatType[FORMAT_SPEC_STR_COUNT];

    // Load in the header so that we can check it
    if( fread( formatType, sizeof( formatType ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Check the header to make sure this is not random data
    if( strcmp( formatType, NORMAL_RAW_FORMAT_STR_ID ) != 0 )
        goto HANDLE_ERROR;

    // Read the number of polygons
    if( fread( &tmpPolyCount, sizeof( tmpPolyCount ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Make sure we have the same number of polygons here
    if( tmpPolyCount != polyCount )
        goto HANDLE_ERROR;

    for( i = 0; i < polyCount; ++i )
    {
        // Read the surface and vertex normals
        if( !pList[i].LoadNormalsFromFile( hFile )   )
            goto HANDLE_ERROR;
    }

    // Calculate the RGB color intensity per vertex. Only do this
    // here if we have a light list and we are NOT doing dynamic lightning.
    // Dynamic lighting is done on each render cycle so there's no need
    // to do it here on startup.
    if( pLightList != NULL && pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
    {
        for( i = 0; i < polyCount; ++i )
            pList[i].CalcLightIntenStatic( pLightList );
    }

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    return result;

}   // LoadNormals


/************************************************************************
*    FUNCTION NAME:         LoadFromPointer                                                             
*
*    DESCRIPTION:           Load an object from data pointer. This is
*                           our own simple file format for quickly loading
*                           vertices, face indexes, uv and texture indexes
*                           into the texture library.
*
*    FUNCTION PARAMETERS:
*    Input:    unsigned char *pData - Pointer to mesh data
*              bool calNormalse - Calculate normals on the fly
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
/*
bool CMesh::LoadFromPointer( unsigned char *pData )
{
    bool result = false;
    int i, j, vIndex;

    // Free all allocations
    Free();

    // Get the number of vertices and polygons
    vCount = *(int *)pData;
    pData += sizeof( int );
    polyCount = *(int *)pData;
    pData += sizeof( int );

    // Allocate the vertex list
    vList = new CPoint[ vCount ];
    if( vList == NULL )
        goto HANDLE_ERROR;

    // Allocate the polygon list
    pList = new CPolygon[ polyCount ];
    if( pList == NULL )
        goto HANDLE_ERROR;

    // Allocate the normal list
    nList = new CPoint[ polyCount ];
    if( nList == NULL )
        goto HANDLE_ERROR;

    // Setup the normals with the pointers
    for( i = 0; i < polyCount; ++i )
        pList[i].pNormal = &nList[i];

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

    // Load the polygon data
    for( i = 0; i < polyCount; ++i )
    {
        // Load the vertex index and set the pointer
        for( j = 0; j < QUAD; ++j )
        {
            // Read in the vertex index
            vIndex = *(int *)pData;
            pData += sizeof( unsigned int );

            // Set the pointer
            pList[i].pVPoint[j] = &vList[vIndex];
        }
        
        // Load the UV corridinates
        for( j = 0; j < QUAD; ++j )
        {
            pList[i].uvrgb[j].u = *(float *)pData;
            pData += sizeof( float );

            pList[i].uvrgb[j].v = *(float *)pData;
            pData += sizeof( float );
        }
        
        // Read in the texture index
        pList[i].tIndex = *(unsigned short int *)pData;
        pData += sizeof( unsigned short int );

        // Read in the texture index
        pList[i].vertCount = *(char *)pData;
        pData += sizeof( char );

        // Set some class members depending on the number of verts
        if( pList[i].vertCount == TRI )
        {
            pList[i].glType = GL_TRIANGLES;
            pList[i].multi = 0.33333333;
        }
        else if( pList[i].vertCount == QUAD )
        {
            pList[i].glType = GL_QUADS;
            pList[i].multi = 0.25;
        } 
    }

    // Calculate a bounding sphere for each polygon. Used for collision
    // detection and view frustum culling
    CalBoundingPoly();

    // Calculate the bounding sphere for this object. Useful for
    // doing full object culling.
    CalBoundingSphere();

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;
    
    return result;

}   // LoadFromPointer
*/

/************************************************************************
*    FUNCTION NAME:         SetAmbientLightLevel()
*
*    DESCRIPTION:           Set an ambient light level for this object.
*
*    FUNCTION PARAMETERS:
*    Input:    int rColor, gColor, bColor - RGB color
*              var2type Var2Name - Description
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

void CMesh::SetAmbientLightLevel( int rColor, int gColor, int bColor )
{
    int r, g, b;
    
    // As a safty precaution, make sure each channel was 
    // not set heigher then 255
    r = rColor & 0xFF;
    g = gColor & 0xFF;
    b = bColor & 0xFF;  

    for( int i = 0; i < polyCount; ++i )
    {
        // equil to r / 255.0f;   i/z conversion for fast lighting
        pList[i].SetAmbientLightLevel( (float)r * 0.0039215, 
                                          (float)g * 0.0039215,
                                          (float)b * 0.0039215 );
    }
}   // SetAmbientLightLevel


/************************************************************************
*    FUNCTION NAME:         CullMesh_BoundSphere()                                                             
*
*    DESCRIPTION:           Test position of object in frustum to see
*                           if it can be culled. This assumes a 45 degree
*                           view area.
*
*    FUNCTION PARAMETERS:
*    Input:    Matrix3D &M - Matris to do all the translations
*              var2type Var2Name - Description
*
*    Output:   bool - True indicates object is outside frustum
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool _fastcall CMesh::CullMesh_BoundSphere( CMatrix &m )
{
    // Set the flag that indicates the object was
    // culled in case we exit early
    culled = true;

    // Set 0.0 to indicate the object was culled
    distance = 0.0;

    // Tansform the center point of the mesh
    m.Transform( centerPt );

    //////////////////////////////////////////
    // Test against the far and near plain
    //////////////////////////////////////////

    if( centerPt.tz + radius < gfMaxZDistance )
        return culled;

    if( centerPt.tz - radius > gfMinZDistance )
        return culled;

    //////////////////////////////////////////
    // Test against the Y side of the frustum
    //////////////////////////////////////////

    // Find the farthest Z point
    float farPointY( (centerPt.tz - radius) * 0.5 );

    if( centerPt.ty + radius < farPointY )
        return culled;

    if( centerPt.ty - radius > -farPointY )
        return culled;

    //////////////////////////////////////////
    // Test against the X side of the frustum
    //////////////////////////////////////////

    // Find the farthest Z point
    float farPointX( farPointY * gfAspectRatio );

    if( centerPt.tx + radius < farPointX )
        return culled;

    if( centerPt.tx - radius > -farPointX )
        return culled;

    // if we made it this far, the object was not culled
    culled = false;

    // Calculate the distance of the center point
    distance = sqrt( ( centerPt.tx  *  centerPt.tx ) +
                     ( centerPt.ty  *  centerPt.ty ) +
                     ( centerPt.tz  *  centerPt.tz ) );

    return culled;

}   // CullMesh_BoundSphere


/************************************************************************
*    FUNCTION NAME:         CullMesh_BoundSphere()                                                             
*
*    DESCRIPTION:           Test position of object in supplied frustum 
*                           to see if it can be culled. This assumes a 
*                           45 degree view area.
*
*    FUNCTION PARAMETERS:
*    Input:    Matrix3D &M - Matris to do all the translations
*              var2type Var2Name - Description
*
*    Output:   bool - True indicates object is outside frustum
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool _fastcall CMesh::CullMesh_BoundSphere( CMatrix &m, float portRadiusX, float portRadiusY,
                                            CPoint *pPortalCenter )
{
    // Set the flag that indicates the object was
    // culled in case we exit early
    culled = true;

    // Set 0.0 to indicate the object was culled
    distance = 0.0;

    // Tansform the center point of the mesh
    m.Transform( centerPt );

    //////////////////////////////////////////
    // Test against the far and near plain
    //////////////////////////////////////////

    if( centerPt.tz + radius < gfMaxZDistance )
        return culled;

    if( centerPt.tz - radius > gfMinZDistance )
        return culled;

    //////////////////////////////////////////
    // Test against the Y side of the frustum
    //////////////////////////////////////////

    float farPointY( ( centerPt.tz - radius ) * 0.5 );

    // Calculate the difference between the portal and
    // far point of the bounding sphere
    double diff = farPointY / (double)pPortalCenter->tz;

    double portalRadiusY = (double)portRadiusY * diff;
    double portalCenterY = (double)pPortalCenter->ty * diff;

    if( centerPt.ty + radius < portalCenterY - portalRadiusY )
        return culled;

    if( centerPt.ty - radius > portalCenterY + portalRadiusY )
        return culled;

    //////////////////////////////////////////
    // Test against the X side of the frustum
    //////////////////////////////////////////

    // Find the farthest Z point
    float farPointX( farPointY * gfAspectRatio );

    // Calculate the difference between the portal and
    // far point of the bounding sphere
    diff = farPointX / (double)pPortalCenter->tz;

    double portalRadiusX = (double)portRadiusX * diff;
    double portalCenterX = (double)pPortalCenter->tx * diff;

    if( centerPt.tx + radius < portalCenterX - portalRadiusX )
        return culled;

    if( centerPt.tx - radius > portalCenterX + portalRadiusX )
        return culled;

    // if we made it this far, the object was not culled
    culled = false;

    // Calculate the distance of the center point
    distance = sqrt( ( centerPt.tx  *  centerPt.tx ) +
                     ( centerPt.ty  *  centerPt.ty ) +
                     ( centerPt.tz  *  centerPt.tz ) );

    return culled;

}   // CullMesh_BoundSphere


/************************************************************************
*    FUNCTION NAME:         Collide_BoundSphere()                                                             
*
*    DESCRIPTION:           Check for collision. A bounding sphere for
*                           the mesh is used to check for collision
*                           against the supplied mesh.
*
*    FUNCTION PARAMETERS:
*    Input:    CMesh &mesh - mesh to check for collion
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  08/07/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool _fastcall CMesh::Collide_BoundSphere( CMesh *mesh )
{
    double dist;
    bool result = false;

    // Calculate the distance between the center points of both objects
    dist = sqrt( (( centerPt.tx - mesh->GetCenterPt()->tx ) * ( centerPt.tx - mesh->GetCenterPt()->tx )) +
                 (( centerPt.ty - mesh->GetCenterPt()->ty ) * ( centerPt.ty - mesh->GetCenterPt()->ty )) +
                 (( centerPt.tz - mesh->GetCenterPt()->tz ) * ( centerPt.tz - mesh->GetCenterPt()->tz )) );

    // Check for possible collision by using bounding sphere
    if( dist <= mesh->GetRadius() + radius )
    {
        result = true;
    }
    
    return result;

}   // Collide_BoundSphere


/************************************************************************
*    FUNCTION NAME:         Collide_BoundSphere()                                                             
*
*    DESCRIPTION:           Check for collision. A bounding sphere for
*                           the mesh is used to check for collision
*                           against the supplied point and radius.
*
*    FUNCTION PARAMETERS:
*    Input:    CPoint &pt - point to check for collion
*              float ptRadius -  radius of supplied point
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  08/07/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool _fastcall CMesh::Collide_BoundSphere( CPoint &pt, float ptRadius )
{
    double dist;
    bool result = false;

    // Calculate the distance between the center points of both objects
    dist = sqrt( (( centerPt.tx - pt.tx ) * ( centerPt.tx - pt.tx )) +
                 (( centerPt.ty - pt.ty ) * ( centerPt.ty - pt.ty )) +
                 (( centerPt.tz - pt.tz ) * ( centerPt.tz - pt.tz )) );

    // Check for possible collision by using bounding sphere
    if( dist <= ptRadius + radius )
    {
        result = true;
    }
    
    return result;

}   // Collide_BoundSphere


/************************************************************************
*    FUNCTION NAME:         Render()                                                             
*
*    DESCRIPTION:           Render the 3D object on screen buffer.
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
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int CMesh::Render( CMatrix &m )
{
    int renderCount(0);

    // Transform all the vertices
    m.Transform( vList, vCount );

    // Transform all the surface normals
    m.Transform( nList, polyCount );

    // Do we have textures?
    if( pTextLib != NULL )
    {
        if( pLightList == NULL || pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable() )
                {
                    // Render
                    pList[i].RenderText( pTextLib );

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }
        }                   
        else if( pLightList->GetDynamicLightType() == EDL_LOCAL_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable() )
                {
                    // Calculate a new intensity
                    pList[i].CalcLightIntenStatic( pLightList );

                    // Render
                    pList[i].RenderText( pTextLib );

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable() )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pList[i].GetUVRGB() );
                    
                    // Calculate a new intensity
                    pList[i].CalcLightIntenDynamic( pLightList );

                    // Render
                    pList[i].RenderText( pTextLib );

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }
        }
    }
    else   // Render with no textures
    {
        if( pLightList == NULL || pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable() )
                {
                    // Render
                    pList[i].Render();

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }
        }                   
        else if( pLightList->GetDynamicLightType() == EDL_LOCAL_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable() )
                {
                    // Calculate a new intensity
                    pList[i].CalcLightIntenStatic( pLightList );

                    // Render
                    pList[i].Render();

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable() )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pList[i].GetUVRGB() );
                    
                    // Calculate a new intensity
                    pList[i].CalcLightIntenDynamic( pLightList );

                    // Render
                    pList[i].Render();

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }
        }
    }

    return renderCount;

}   // Render


/************************************************************************
*    FUNCTION NAME:         Render()                                                             
*
*    DESCRIPTION:           Render the 3D object on screen buffer.
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
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int CMesh::Render( CMatrix &m, float portRadiusX, float portRadiusY, CPoint *pPortalCenter  )
{
    int renderCount(0);

    // Transform all the vertices
    m.Transform( vList, vCount );

    // Transform all the surface normals
    m.Transform( nList, polyCount );

    // Do we have textures?
    if( pTextLib != NULL )
    {
        if( pLightList == NULL || pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable( portRadiusX, portRadiusY, pPortalCenter ) )
                {
                    // Render
                    pList[i].RenderText( pTextLib );

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }
        }                   
        else if( pLightList->GetDynamicLightType() == EDL_LOCAL_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable( portRadiusX, portRadiusY, pPortalCenter ) )
                {
                    // Calculate a new intensity
                    pList[i].CalcLightIntenStatic( pLightList );

                    // Render
                    pList[i].RenderText( pTextLib );

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable( portRadiusX, portRadiusY, pPortalCenter ) )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pList[i].GetUVRGB() );
                    
                    // Calculate a new intensity
                    pList[i].CalcLightIntenDynamic( pLightList );

                    // Render
                    pList[i].RenderText( pTextLib );

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }
        }
    }
    else   // Render with no textures
    {
        if( pLightList == NULL || pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable( portRadiusX, portRadiusY, pPortalCenter ) )
                {
                    // Render
                    pList[i].Render();

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }
        }                   
        else if( pLightList->GetDynamicLightType() == EDL_LOCAL_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable( portRadiusX, portRadiusY, pPortalCenter ) )
                {
                    // Calculate a new intensity
                    pList[i].CalcLightIntenStatic( pLightList );

                    // Render
                    pList[i].Render();

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable( portRadiusX, portRadiusY, pPortalCenter ) )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pList[i].GetUVRGB() );
                    
                    // Calculate a new intensity
                    pList[i].CalcLightIntenDynamic( pLightList );

                    // Render
                    pList[i].Render();

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }
        }
    }

    return renderCount;

}   // Render


/************************************************************************
*    FUNCTION NAME:         IsCollision_BoundSphere()                                                             
*
*    DESCRIPTION:           Check for collision. Once a collision is made
*                           we break from the loop to allow the game to
*                           process the polygon we collided with. The game
*                           usually keeps calling this function intil all
*                           the polygons have been checked.
*
*    FUNCTION PARAMETERS:
*    Input:    
*              int &index - Index to start searching from.
*              CPoint &pt - Center point of the translated object.
*                             Usually the vertex gotten from the matrix
*                             being used to translate the object.
*              float rad - radius of the object the center point belongs to.
*
*    Output:   int - Index of the texture being used. This is a way to tell
*                    what we are colliding with.
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int CMesh::IsCollision_BoundSphere( int &index, double &distance, CPoint &pt, float rad )
{
    int result(-1), i(index);
    
    // cycle through until we find a collision
    for( ; i < polyCount; ++i )
    {
        // Only check those that have a type
        if( pList[i].GetCollisionType() > 0 )
        {
            result = pList[i].Collide_BoundSphere( pt, distance, rad );
            
            if( result > -1 )
                break;
        }
    }

    index = i;

    return result;

}   // IsCollision_BoundSphere


/************************************************************************
*    FUNCTION NAME:         IsCollision_Intersection()                                                             
*
*    DESCRIPTION:           Last check for collision. Much more accurate
*                           then bounding spheres and also much slower
*                           which is why this check is performed after
*                           a bounding sphere's tests are finished. Also,
*                           intersection collision is testing if the plane
*                           was pierced by a point. We are talking a plain,
*                           not a polygon. So the test is not confined to
*                           the size of the polygon, but the plain it is on.
*                           A plain is infanit.
*
*                           The resulting calculations are reused for
*                           handing the collision be it gravity or
*                           wall plaining.
*
*    FUNCTION PARAMETERS:
*    Input:    Matrix3D &M - Matris to do all the translations
*              int triIndex - Index of polygon to check
*              float padding - distance between point and polygon
*              CPoint &pt - point to check.
*              *NOTE: If in First Persion point of view, x, y, z is
*              not the current point on the map. use 0,0,0 or 1,1,1
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    03/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMesh::IsCollision_Intersection( CMatrix &m, int triIndex, float padding, CPoint &pt,
                                      CPoint &normal, float &pointD )
{
    CPoint norm;
    CPoint tri;

    // Get a copy of the points
    pList[triIndex].CopyPoint( tri, 0 );

    // Get a copy of the normal
    pList[triIndex].CopyNormal( norm );

    // We need the points and the normal retransformed without 
    // any x rotation for the equations to work correctly.
    m.Transform( tri );
    m.Transform( norm );
    
    // Get the x, y and z normals
    // Keep in mind our normal is hidden and to get it we have to
    // norm.tx - tri.tx
    normal.x = norm.tx - tri.tx;
    normal.y = norm.ty - tri.ty;
    normal.z = norm.tz - tri.tz;
    pointD = -( (normal.x * tri.tx) + (normal.y * tri.ty) + (normal.z * tri.tz) );

    // Confirm/reject collision by calculating which side of the plane
    // the point will be on. In other words, did we pierce the plain
    float finalCheck = (normal.x * pt.tx) + (normal.y * pt.ty) + (normal.z * pt.tz) + pointD + -padding;

    // Final check of intersection collision.
    // Did we perce the plain?
    if( finalCheck <= 0.0f )
    {        
        // The eagle has landed.
        return true;
    }

    return false;

}   // IsCollision_Intersection


/************************************************************************
*    FUNCTION NAME:         Scale()                                                             
*
*    DESCRIPTION:           Scale this mesh.
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

void CMesh::Scale( float x, float y, float z )
{
    CMatrix m;

    // convert all the normals from a vector to a normal.
    // if this seems odd, see CalcVertexNormals or CPolygon.cpp | CalcSurfaceNormal
    // for more info why we do this
    for( int i = 0; i < polyCount; ++i )
    {
        pList[i].ConvertToPureNormals();
    }

    // Create a matrix with the scaling factors
    m.Scale( x, y, z );

    // Transform all the vertices
    m.Transform( vList, vCount );
    
    // Copy it over and make it perminate
    for( int i = 0; i < vCount; ++i )
    {        
        vList[i].x = vList[i].tx;
        vList[i].y = vList[i].ty;
        vList[i].z = vList[i].tz;                 
    }

    // Reset the normals back to a vector for easy translation
    for( int i = 0; i < polyCount; ++i )
    {
        pList[i].ConvertPureNormalsToVectors();
    }

    // Recalculate the bounding polygons. Used for collision
    // detection and view frustum culling
    CalBoundingPoly();

    // Recalculate the bounding sphere for this object. Useful for
    // doing full object culling.
    CalBoundingSphere();

}   // Scale


/************************************************************************
*    FUNCTION NAME:         CalBoundingPoly()                                                             
*
*    DESCRIPTION:           Calculate bounding polygon
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
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMesh::CalBoundingPoly()
{
    // Calculate a bounding sphere for each polygon. Used for collision
    // detection and view frustum culling
    for( int i = 0; i < polyCount; ++i )
    {
        pList[i].CalcBoundingPoly();
    }

}   // CalBoundingPoly


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
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMesh::CalNormals()
{
    // Calculate the Surface Normal(surface description)
    for( int i = 0; i < polyCount; ++i )
    {
        pList[ i ].CalcSurfaceNormal();
    }

    // Setup the shaded vertices with it's own averaged
    // Surface Normal(surface description)
    CalcVertexNormals();

}   // CalNormals


/************************************************************************
*    FUNCTION NAME:         SetLightList()                                                             
*
*    DESCRIPTION:           Set the list of light data
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
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMesh::SetLightList( CLightList &lightList )
{
    pLightList = &lightList;

    // Calculate the RGB color intensity per vertex. Only do this
    // here if we have a light list and we are NOT doing dynamic lightning.
    // Dynamic lighting is done on each render cycle so there's no need
    // to do it here on startup.
    if( pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
    {
        for( int i = 0; i < polyCount; ++i )
        {
            pList[i].CalcLightIntenStatic( pLightList );
        }
    }

}   // SetLightList


/************************************************************************
*    FUNCTION NAME:         LoadPolygonData()                                                             
*
*    DESCRIPTION:           Load the polygon data
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
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMesh::LoadPolygonData( FILE *hFile )
{
    // Load the polygon data
    for( int i = 0; i < polyCount; ++i )
    {
        if( !pList[i].LoadPolygonData( hFile, vList ) )
            return false;     
    }

    return true;

}   // LoadPolygonData


/************************************************************************
*    FUNCTION NAME:         SavePolygonData()                                                             
*
*    DESCRIPTION:           Save the polygon data
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
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMesh::SavePolygonData( FILE *hFile )
{
    // Save the polygon data
    for( int i = 0; i < polyCount; ++i )
    {
        if( !pList[i].SavePolygonData( hFile, vCount, vList ) )
            return false;         
    }

    return true;

}   // SavePolygonData


/************************************************************************
*    FUNCTION NAME:         CopyMesh()                                                             
*
*    DESCRIPTION:           Copy the supplied mesh
*
*    FUNCTION PARAMETERS:
*    Input:    CMatrix &mesh - Mesh to copy
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    12/21/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMesh::CopyMesh( CMesh &mesh )
{
    // Free all allocations
    Free();

    vCount = mesh.GetVertCount();
    polyCount = mesh.GetPolyCount();
    pTextLib = mesh.GetTextLibary();
    pLightList = mesh.GetLightList();

    // Allocate the vertex list
    vList = new CPoint[ vCount ];
    if( vList == NULL )
        return false;

    // Allocate the polygon array
    pList = AllocatePolygonArray();
    if( pList == NULL )
        return false;

    // Allocate the normal list. We are not loading any
    // normals here but the normal count is the same as
    // the number of polygons.
    nList = new CPoint[ polyCount ];
    if( nList == NULL )
        return false;

    // Load the global vertex list for this object.
    for( int i = 0; i < vCount; ++i )
    {            
        vList[i] = mesh.GetVertexList()[i];  
    }

    // Setup the normals with the values and pointers
    // 1 to 1 ratio
    for( int i = 0; i < polyCount; ++i )
    {
        nList[i] = mesh.GetNormalList()[i];
        pList[i].SetNormal( &nList[i] );

        // Copy the polygon
        pList[i].CopyPolygon( mesh.GetPolyList()[i], vList, vCount );
    }

    return true;

}   // CopyMesh


/************************************************************************
*    FUNCTION NAME:         InterpulateMesh()                                                             
*
*    DESCRIPTION:           Merge two meshes into this one. Assumes same
*                           number of vertexes and polygons
*
*    FUNCTION PARAMETERS:
*    Input:    CMatrix &mesh - Mesh to copy
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    12/21/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMesh::InterpulateMesh( CMesh &mesh1, CMesh &mesh2, float amount )
{
    // Merge the verts
    for( int i = 0; i < vCount; ++i )
    {
        vList[i].x = ((1-amount) * mesh1.GetVertexList()[i].x) +
                     (amount * mesh2.GetVertexList()[i].x);

        vList[i].y = ((1-amount) * mesh1.GetVertexList()[i].y) +
                     (amount * mesh2.GetVertexList()[i].y);

        vList[i].z = ((1-amount) * mesh1.GetVertexList()[i].z) +
                     (amount * mesh2.GetVertexList()[i].z);
    }

    // There's no way around it. We have to recalculate normals
    // Interpulating the normals doesn't work.
    for( int i = 0; i < polyCount; ++i )
    {
        pList[i].CalcSurfaceNormal();
    }

    // If there is no dynamic lighting, interpulate the RGB values
    if( pLightList == NULL || pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
    {
        // Merge the rgb
        for( int i = 0; i < polyCount; ++i )
        {
            CPolygon *poly1 = &mesh1.GetPolyList()[i];
            CPolygon *poly2 = &mesh2.GetPolyList()[i];

            for( int j = 0; j < pList[i].GetVertexCount(); ++j )
            {
                pList[i].GetUVRGB()[j].r = ((1-amount) * poly1->GetUVRGB()[j].r) +
                         (amount * poly2->GetUVRGB()[j].r);

                pList[i].GetUVRGB()[j].g = ((1-amount) * poly1->GetUVRGB()[j].g) +
                         (amount * poly2->GetUVRGB()[j].g);

                pList[i].GetUVRGB()[j].b = ((1-amount) * poly1->GetUVRGB()[j].b) +
                         (amount * poly2->GetUVRGB()[j].b);
            }
        }
    }
    // If there is dynamic lightning, interpulate the vertex normals. This doesn't
    // have to be perfect.
    else if( pLightList->GetDynamicLightType() != EDL_NO_DYNAMIC_LIGHTNING )
    {
        // Merge the rgb
        for( int i = 0; i < polyCount; ++i )
        {
            CPolygon *poly1 = &mesh1.GetPolyList()[i];
            CPolygon *poly2 = &mesh2.GetPolyList()[i];

            for( int j = 0; j < pList[i].GetVertexCount(); ++j )
            {
                pList[i].GetUVRGB()[j].n.x = ((1-amount) * poly1->GetUVRGB()[j].n.x) +
                         (amount * poly2->GetUVRGB()[j].n.x);

                pList[i].GetUVRGB()[j].n.y = ((1-amount) * poly1->GetUVRGB()[j].n.y) +
                         (amount * poly2->GetUVRGB()[j].n.y);

                pList[i].GetUVRGB()[j].n.z = ((1-amount) * poly1->GetUVRGB()[j].n.z) +
                         (amount * poly2->GetUVRGB()[j].n.z);
            }
        }
    }

}   // InterpulateMesh