
/************************************************************************
*
*    FILE NAME:       CMesh.cpp
*
*    DESCRIPTION:     base mesh class with all general mesh attributes
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

#include "CMesh.h"           // Header file for this *.cpp file.
#include "types.h"           // misc types

// Globals
extern float gfMaxZDistance;
extern float gfMinZDistance;
extern float gfFrustrumYRatio;
extern float gfSquarePercentage;
extern int giRenderDevice;
extern int gTriCounter;
extern int gSoftwareTextIndex;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    DESCRIPTION:  Constructer                                                             
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
    id = 0;
    type = EOT_NO_TYPE;
    rendered = false;
    cullTestDone = false;

}   // Constructer


/************************************************************************
*    DESCRIPTION:  Destructer                                                             
************************************************************************/
CMesh::~CMesh()
{
    Free();

}   // Destructer


/************************************************************************
*    DESCRIPTION:  Free all allocations
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
*    DESCRIPTION:  Calculate a bounding sphere & rect around this object.
*                  Handy for full object culling against the 
*                  view frustum.
************************************************************************/
void CMesh::CalBoundingSphereBox()
{
    long double x(0.0), y(0.0), z(0.0);
	long double inverseVertCount = 1.0 / (float)vCount;
	double maxDistance = 0.0;
    double maxDistanceX = 0.0;
    double maxDistanceY = 0.0;
    double curDistance;

    // Average out each vertex
    for( int i = 0; i < vCount; ++i )
	{
		x += vList[i].x;
		y += vList[i].y;
		z += vList[i].z;
	}

    // Record the center point
    centerPt.x = x * inverseVertCount;
    centerPt.y = y * inverseVertCount;
    centerPt.z = z * inverseVertCount;

    // init
    minPoint.x = vList[0].x;
    maxPoint.x = vList[0].x;
    minPoint.y = vList[0].y;
    maxPoint.y = vList[0].y;
    minPoint.z = vList[0].z;
    maxPoint.z = vList[0].z;

    // Find the closest and farthest distances
    // for each axis.
    for( int i = 1; i < vCount; ++i )
    {
        if( vList[i].x < minPoint.x )
            minPoint.x = vList[i].x;

        else if( vList[i].x > maxPoint.x )
            maxPoint.x = vList[i].x;

        if( vList[i].y < minPoint.y )
            minPoint.y = vList[i].y;

        else if( vList[i].y > maxPoint.y )
            maxPoint.y = vList[i].y;

        if( vList[i].z < minPoint.z )
            minPoint.z = vList[i].z;

        else if( vList[i].z > maxPoint.z )
            maxPoint.z = vList[i].z;
    }

    // Setup the bounding box
    boundingBox[0].x = minPoint.x;
    boundingBox[0].y = minPoint.y;
    boundingBox[0].z = minPoint.z;

    boundingBox[1].x = maxPoint.x;
    boundingBox[1].y = maxPoint.y;
    boundingBox[1].z = maxPoint.z;

    boundingBox[2].x = maxPoint.x;
    boundingBox[2].y = minPoint.y;
    boundingBox[2].z = minPoint.z;

    boundingBox[3].x = minPoint.x;
    boundingBox[3].y = maxPoint.y;
    boundingBox[3].z = minPoint.z;

    boundingBox[4].x = minPoint.x;
    boundingBox[4].y = minPoint.y;
    boundingBox[4].z = maxPoint.z;

    boundingBox[5].x = maxPoint.x;
    boundingBox[5].y = maxPoint.y;
    boundingBox[5].z = minPoint.z;

    boundingBox[6].x = minPoint.x;
    boundingBox[6].y = maxPoint.y;
    boundingBox[6].z = maxPoint.z;

    boundingBox[7].x = maxPoint.x;
    boundingBox[7].y = minPoint.y;
    boundingBox[7].z = maxPoint.z;
    
    // Reguardless of where the object is,
    // we need to calculate the radius as if the
    // object's center was at 0,0,0
    for( int i = 0; i < vCount; ++i )
    {
        CPoint pt;

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

        curDistance = (pt.x * pt.x) + (pt.z * pt.z);

        if( curDistance > maxDistanceX )
            maxDistanceX = curDistance;

        curDistance = (pt.y * pt.y) + (pt.z * pt.z);

        if( curDistance > maxDistanceY )
            maxDistanceY = curDistance;
    }

    // computer the radius
    radius = sqrt( maxDistance );
    radiusX = sqrt( maxDistanceX );
    radiusY = sqrt( maxDistanceY );

}   // CalBoundingSphereBox


/************************************************************************
*    DESCRIPTION:  Upon entering this function, the Surface Normal
*                  has been calculated for each polygon. We find
*                  all the shared vertices for a given vertex,
*                  add it all up, and average it out for one
*                  vertex. This is how we calculate a normal for
*                  each vertex. This is needed to do dynamic shading.
*
*                  Note: This is time consuming on large data sets.
*                  It is suggested you do all your normal and vertex
*                  normal calculations offline.
************************************************************************/
void CMesh::CalcVertexNormals( float range )
{
    // Addup all the normals per vertex for all polygons
    for( int pIndex = 0; pIndex < polyCount; ++pIndex )
    {           
		// Find shared vertices, add them up and average per vertex
    	for( int vIndex = 0; vIndex < pList[pIndex].GetVertexCount(); ++vIndex )
    	{
			std::vector<CNormAdd> sharedNormLst;

			// Collect the shared verticies
			FindSharedNorms( range, pList[pIndex].GetVPoint( vIndex ), sharedNormLst );

            // Must have atleast one
            if( sharedNormLst.empty() )
			{
				CNormAdd na;
				na.x = (long double)pList[pIndex].GetNormal()->x;
        		na.y = (long double)pList[pIndex].GetNormal()->y;
        		na.z = (long double)pList[pIndex].GetNormal()->z;
				sharedNormLst.push_back( na );
			}

            // Add the shared normals to create vertex normal
			AverageSharedNorms( pIndex, vIndex, sharedNormLst );
        }
    }

}   // CalcVertexNormals


/************************************************************************
*    DESCRIPTION:  Find shared indexes in vertex. This has been designed
*                  this way so that it can be used in mesh lists.
*
*    Input:    float range - How closely you need the verts to be compaired
*              CPoint * vPoint -  point to check it against
*              std::vector<int> &sharedIntLst - shared int list
*
*                  Note: This is time consuming on large data sets.
*                  It is suggested you do all your normal and vertex
*                  normal calculations offline.
************************************************************************/
void _fastcall CMesh::FindSharedNorms( float range, CPoint * vPoint, std::vector<CNormAdd> &sharedNormLst )
{
	// Search through the entire list of polygons looking 
    // for shared vertices and record it's index

	if( range == 0.0f )	// each vert is an exact match
	{
		// Check for an exact match for the point - fastest
		for( int i = 0; i < polyCount; ++i )
	    {
	    	if( pList[ i ].HasVert( *vPoint ) )
	        {
				CNormAdd na;
				na.x = (long double)pList[i].GetNormal()->x;
        		na.y = (long double)pList[i].GetNormal()->y;
        		na.z = (long double)pList[i].GetNormal()->z;
				sharedNormLst.push_back( na );
	        }
	    }
	}
	else if( range > 0.0f )	 // look for verts within a range
	{
		// Check two points within a range - slower but makes up for modeler errors
		for( int i = 0; i < polyCount; ++i )
	    {
	    	if( pList[ i ].HasCloseVert( *vPoint, range ) )
	        {
				CNormAdd na;
				na.x = (long double)pList[i].GetNormal()->x;
        		na.y = (long double)pList[i].GetNormal()->y;
        		na.z = (long double)pList[i].GetNormal()->z;
				sharedNormLst.push_back( na );
	        }
	    }
	}

}	// FindSharedIndexes


/************************************************************************
*    DESCRIPTION:  Add the shared normals to create vertex normal
*
*    Input:    int pIndex - polygon index of vert to record averaged norms
*              int vIndex - vertex index of vert to record averaged norms
*              unsigned int sharedNormCount - Total number of shared verts
*
*                  Note: This is time consuming on large data sets.
*                  It is suggested you do all your normal and vertex
*                  normal calculations offline.
************************************************************************/
void _fastcall CMesh::AverageSharedNorms( int pIndex, int vIndex, std::vector<CNormAdd> &sharedNormLst )
{
	CNormAdd na;

	// Add up all the Surface Normals of all the shared vertices
	for( std::vector<CNormAdd>::iterator it = sharedNormLst.begin(); it != sharedNormLst.end(); ++it )
    {
        na.x += (*it).x;
        na.y += (*it).y;
        na.z += (*it).z;
    }

	// Average out the accumlated Surface Normals
    // to end up with a Surface Normal for each vertex
    CPoint n;
    n.tx = n.x = (float)(na.x / (long double)sharedNormLst.size());
    n.ty = n.y = (float)(na.y / (long double)sharedNormLst.size());
    n.tz = n.z = (float)(na.z / (long double)sharedNormLst.size());

    // Record the vertex normal for light calculations
    pList[ pIndex ].SetVertexNormal( vIndex, n );

}	// AddSharedNorms


/************************************************************************
*    DESCRIPTION:  Upon entering this function, the Surface Normal
*                  has been calculated for each polygon.
*
*                  This function is specific for a terrain grid.
*
*                  This function is optimized for fast vertex normals
*                  that happen to be in a grid like a terrain for example
*
*    Input:    int width - Width of grid in points
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
                nx += (long double)pList[ index ].GetNormal()->x;
                ny += (long double)pList[ index ].GetNormal()->y;
                nz += (long double)pList[ index ].GetNormal()->z;
            }

            // Average out the accumlated Surface Normals
            // to end up with a Surface Normal for each vertex
            CPoint n;
            n.tx = n.x = (float)(nx / (long double)sharedIndexCount);
            n.ty = n.y = (float)(ny / (long double)sharedIndexCount);
            n.tz = n.z = (float)(nz / (long double)sharedIndexCount);

            // Record the vertex normal for light calculations
            pList[ poly ].SetVertexNormal( vIndex, n );
        }
    }

}   // CalcGridVertexNormals


/************************************************************************
*    DESCRIPTION:  Load an object from a PLY binary file. This is
*                  our own simple file for quickly loading
*                  vertices, face indexes, uv and texture indexes
*                  into the texture library.
*
*    FUNCTION PARAMETERS:
*    Input:    char *filePath - Path to file to load
*
*    Output:   bool - true on success, false on fail
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
*    DESCRIPTION:  Load an object from a PLY binary file. This is
*                  our own simple file for quickly loading
*                  vertices, face indexes, uv and texture indexes
*                  into the texture library.
*
*    Input:    FILE *hFile - Handle to file
*
*    Output:   bool - true on success, false on fail
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
*    DESCRIPTION:           Load an object from a PLY binary file. This is
*                           our own simple file for quickly loading
*                           vertices, face indexes, uv and texture indexes
*                           into the texture library.
*
*    Input:    char *filePath - Path to file to load
*
*    Output:   bool - true on success, false on fail
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
*    DESCRIPTION:  Load an object from a PLY binary file. This is
*                  our own simple file for quickly loading
*                  vertices, face indexes, uv and texture indexes
*                  into the texture library.
*
*    Input:    FILE *hFile - Handle to file
*
*    Output:   bool - true on success, false on fail
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

	if( vCount <= 0 || polyCount <= 0 )
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
    CalBoundingSphereBox();

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;
    
    return result;

}   // LoadMeshFromFile


/************************************************************************
*    DESCRIPTION:  Virtual function for loading inherited class
*                  specific data
*
*    Input:    FILE *hFile - Handle to file
*
*    Output:   bool - true on success, false on fail
************************************************************************/

bool CMesh::LoadExtraMeshData( FILE *hFile )
{
    return true;

}   // LoadExtraMeshData


/************************************************************************
*    DESCRIPTION:  Allocate the polygon array. Virtual call so that an
*                  inherited class can load up it's own polygon class.
************************************************************************/
CPolygon * CMesh::AllocatePolygonArray()
{
    return new CPolygon[ polyCount ];

}   // AllocatePolygonArray


/************************************************************************
*    DESCRIPTION:  Save mesh and normal data to file. This is
*                  our own simple file for quickly loading
*                  vertices, face indexes, uv and texture indexes
*                  into the texture library.
*
*    Input:    char *filePath - Path to file to load
*
*    Output:   bool - true on success, false on fail
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
*    DESCRIPTION:  Save mesh and normal data to file. This is
*                  our own simple file for quickly loading
*                  vertices, face indexes, uv and texture indexes
*                  into the texture library.
*
*    Input:    FILE *hFile - Handle to file
*
*    Output:   bool - true on success, false on fail
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
*    DESCRIPTION:  Generate a terrain grid.
*
*    Input:    int x - Number of points X
*              int y - Number of points Y
*              int yOffset - y offset of points
*              float multiplyer - multiply the hight values
*              float size - spacing between each point
*              unsigned char * hightData -  height map data. Height map
*                                           data from an 8 bit bmp works well.
*
*    Output:   bool - true on success, false on fail
************************************************************************/
bool CMesh::GenerateGrid( int x, int z, int yOffset, float multiplyer,
                          float size, unsigned char * hightData )
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
*    DESCRIPTION:  Save the mesh to file.
*
*    Input:    char *filePath - Path to file to load
*
*    Output:   bool - true on success, false on fail
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
*    DESCRIPTION:  Save the mesh to file.
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - File handle
*
*    Output:   bool - true on success, false on fail
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
*    DESCRIPTION:  Virtual function for savinging inherited class
*                  specific data
*
*    Input:    FILE *hFile - Handle to file
*
*    Output:   bool - true on success, false on fail
************************************************************************/
bool CMesh::SaveExtraMeshData( FILE *hFile )
{
    return true;

}   // SaveExtraMeshData


/************************************************************************
*    DESCRIPTION:  Save all the normals to a file. Helpful for
*                  quick loading of large data sets.
*
*    Input:    char *filePath - Path to file to load
*
*    Output:   bool - true on success, false on fail
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
*    DESCRIPTION:  Save all the normals to a file. Helpful for
*                  quick loading of large data sets.
*
*    Input:    FILE *hFile - File handle
*
*    Output:   bool - true on success, false on fail
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
*    DESCRIPTION:  Load the normals from a file.
*
*    Input:    char *filePath - Path to file to load
*
*    Output:   bool - true on success, false on fail
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
*    DESCRIPTION:  Load the normals from a file.
*
*    Input:    FILE *hFile - File handle
*
*    Output:   bool - true on success, false on fail
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
*    DESCRIPTION:  Set an ambient light level for this object.
*
*    Input:    int rColor, gColor, bColor - RGB color
************************************************************************/
void CMesh::SetAmbientLightLevel( int rColor, int gColor, int bColor )
{
    int r, g, b;
    
    // As a safty precaution, make sure each channel was 
    // not set heigher then 255
    r = rColor & 0xFF;
    g = gColor & 0xFF;
    b = bColor & 0xFF;  

    if( giRenderDevice == ERD_OPENGL )
	{
	    float red, green, blue;

	    // convert to normal float
        if( r > 254 )
            red = 1.0;
        else
            red = (float)r * 0.0039215;

        if( g > 254 )
            green = 1.0;
        else
            green = (float)g * 0.0039215;

        if( b > 254 )
            blue = 1.0;
        else
            blue = (float)b * 0.0039215;

	    for( int i = 0; i < polyCount; ++i )
	    {        
	        // equil to r / 255.0f;   i/z conversion for fast lighting
	        pList[i].SetAmbientLightLevel( red, green, blue );
	    }
	}
	else
	{
		for( int i = 0; i < polyCount; ++i )
	    {
	        // equil to r / 255.0f;   i/z conversion for fast lighting
	        pList[i].SetAmbientLightLevel( (float)r, (float)g, (float)b );
	    }
	}

}   // SetAmbientLightLevel


/************************************************************************
*    DESCRIPTION:  This function assumes bounding sphere's check
*                  has been done.
*
*                  Test position of object in frustum to see
*                  if it can be culled. This function is only called
*                  after bounding sphere. Requires data that is set in
*                  the bounding sphere check.
*
*    Output:   bool - True indicates object is outside frustum
************************************************************************/
bool CMesh::CullMesh_BoundBox()
{
    cullTestDone = true;
    culled = false;

    // If we are this close, then we can see the object
    if( GetDistance() > radius )
    {
        float farPointX( centerPt.tz * gfSquarePercentage );
        float farPointY( centerPt.tz * gfFrustrumYRatio );
        
        // Test the center point first. If the center point is in the view
        // frustum, then the mesh can be seen
        if( centerPt.tz > -gfMaxZDistance && centerPt.tz < -gfMinZDistance &&
            centerPt.tx > farPointX && centerPt.tx < -farPointX &&
            centerPt.ty > farPointY && centerPt.ty < -farPointY )
            return culled;

        culled = true;
        
        // if one of the points is found then the mesh is in view
        for( int i = 0; i < 8; ++i )
        {
            farPointX = boundingBox[i].tz * gfSquarePercentage;
            farPointY = boundingBox[i].tz * gfFrustrumYRatio;

            if( boundingBox[i].tz > -gfMaxZDistance &&
                boundingBox[i].tz < -gfMinZDistance &&
                boundingBox[i].tx > farPointX &&
                boundingBox[i].tx < -farPointX &&
                boundingBox[i].ty > farPointY &&
                boundingBox[i].ty < -farPointY )
            {
                culled = false;
                break;
            }
        }
    }

    return culled;

}   // CullMesh_BoundBox


/************************************************************************
*    DESCRIPTION:  Test position of object in frustum to see
*                  if it can be culled. This assumes a 45 degree
*                  view area.
*
*    Output:   bool - True indicates object is outside frustum
************************************************************************/
bool CMesh::CullMesh_BoundSphere()
{
	return CullMesh_BoundSphere( radius );

}	// CullMesh_BoundSphere


/************************************************************************
*    DESCRIPTION:  Test position of object in frustum to see
*                  if it can be culled. This assumes a 45 degree
*                  view area.
*
*    Output:   bool - True indicates object is outside frustum
************************************************************************/
bool CMesh::CullMesh_BoundSphere( float _radius )
{
    cullTestDone = true;

    // Set the flag that indicates the object was
    // culled in case we exit early
    culled = true;

    //////////////////////////////////////////
    // Test against the far and near plain
    //////////////////////////////////////////

    if( centerPt.tz + _radius < -gfMaxZDistance )
        return culled;

    if( centerPt.tz - _radius > -gfMinZDistance )
        return culled;

    //////////////////////////////////////////
    // Test against the Y side of the frustum
    //////////////////////////////////////////

    // Find the farthest Z point
    float farPoint( centerPt.tz * gfFrustrumYRatio );

    if( centerPt.ty + _radius < farPoint )
        return culled;

    if( centerPt.ty - _radius > -farPoint )
        return culled;

    //////////////////////////////////////////
    // Test against the X side of the frustum
    //////////////////////////////////////////

    farPoint = centerPt.tz * gfSquarePercentage;

    if( centerPt.tx + _radius < farPoint )
        return culled;

    if( centerPt.tx - _radius > -farPoint )
        return culled;

    // if we made it this far, the object was not culled
    culled = false;

    return culled;

}   // CullMesh_BoundSphere


/************************************************************************
*    DESCRIPTION:  Test position of object in supplied poryal to see if
*                  it can be culled.
*
*    Input:    float portRadius - x portal size
*              CPoint *pPortalCenter - Center of portal
*
*    Output:   bool - True indicates object is outside frustum
************************************************************************/
bool _fastcall CMesh::CullMesh_BoundSphere( float portRadius,
                                            CPoint *pPortalCenter )
{
	return CullMesh_BoundSphere( radius, portRadius, pPortalCenter );

}	// CullMesh_BoundSphere


/************************************************************************
*    DESCRIPTION:  Test position of object in supplied poryal to see if
*                  it can be culled.
*
*    Input:    float portRadius - x portal size
*              CPoint *pPortalCenter - Center of portal
*
*    Output:   bool - True indicates object is outside frustum
************************************************************************/
bool _fastcall CMesh::CullMesh_BoundSphere( float _radius,
											float portRadius,
                                            CPoint *pPortalCenter )
{
    cullTestDone = true;

    // Set the flag that indicates the object was
    // culled in case we exit early
    culled = true;

    //////////////////////////////////////////
    // Test against the Z side of the frustum
    //////////////////////////////////////////

    if( centerPt.tz + _radius < -gfMaxZDistance )
        return culled;

    if( centerPt.tz - _radius > -gfMinZDistance )
        return culled;

    //////////////////////////////////////////
    // Get the difference between the portal
    // and the mesh
    //////////////////////////////////////////

    float diff = centerPt.tz / pPortalCenter->tz;
    float portalRadius = portRadius * diff;

    //////////////////////////////////////////
    // Test against the X side of the frustum
    //////////////////////////////////////////

    float portalCenterX = pPortalCenter->tx * diff;

    if( centerPt.tx + _radius < portalCenterX - portalRadius )
        return culled;

    if( centerPt.tx - _radius > portalCenterX + portalRadius )
        return culled;

    //////////////////////////////////////////
    // Test against the Y side of the frustum
    //////////////////////////////////////////
    
    float portalCenterY = pPortalCenter->ty * diff;

    if( centerPt.ty + _radius < portalCenterY - portalRadius )
        return culled;

    if( centerPt.ty - _radius > portalCenterY + portalRadius )
        return culled;

    // if we made it this far, the object was not culled
    culled = false; 
    
    return culled;

}   // CullMesh_BoundSphere


/************************************************************************
*    DESCRIPTION:  Check for collision. A bounding sphere for
*                  the mesh is used to check for collision
*                  against the supplied mesh.
*
*    Input:    CMesh &mesh - mesh to check for collion
*
*    Output:   bool - true on success, false on fail
************************************************************************/
bool _fastcall CMesh::Collide_BoundSphere( CMesh *mesh )
{
    bool result = false;

    // Calculate the distance between the center points of both objects
	double dist = sqrt( centerPt.GetLengthDy( *mesh->GetCenterPt() ) );

    // Check for possible collision by using bounding sphere
    if( dist <= mesh->GetRadius() + radius )
    {
        result = true;
    }
    
    return result;

}   // Collide_BoundSphere


/************************************************************************
*    DESCRIPTION:  Check for collision. A bounding sphere for
*                  the mesh is used to check for collision
*                  against the supplied point and radius.
*
*    Input:    CPoint &pt - point to check for collion
*              float ptRadius -  radius of supplied point
*
*    Output:   bool - true on success, false on fail
************************************************************************/
bool _fastcall CMesh::Collide_BoundSphere( CPoint &pt, float ptRadius )
{
    bool result = false;

    // Calculate the distance between the center points of both objects
	double dist = sqrt( centerPt.GetLengthDy( pt ) );

    // Check for possible collision by using bounding sphere
    if( dist <= ptRadius + radius )
    {
        result = true;
    }
    
    return result;

}   // Collide_BoundSphere


/************************************************************************
*    DESCRIPTION:  Render the 3D object to on screen buffer.
*
*    Input:    Matrix3D &M - Matrix to do all the translations
*
*    Output:   int - Number of polygons sent to be rendered
************************************************************************/
int CMesh::RenderShadow( CMatrix &m, CPoint &offset, bool transform )
{
    rendered = true;

    if( transform )
    {
        transformed = true;

        // Transform all the vertices
        m.Transform( vList, vCount );

        // Transform all the surface normals
        m.TransformVector( nList, polyCount );
    }

	if( giRenderDevice == ERD_DIRECTX )
	{
	    return RenderDirectXShadow( offset );
	}
	else if( giRenderDevice == ERD_OPENGL )
	{
	    return RenderOpenGLShadow( offset );
	}
	else if( giRenderDevice == ERD_SOFTWARE )
	{
	    return RenderSoftware( m );
	}

    return 0;

}   // Render


/************************************************************************
*    DESCRIPTION:  Render the 3D object to on screen buffer.
*
*    Input:    Matrix3D &M - Matrix to do all the translations
*
*    Output:   int - Number of polygons sent to be rendered
************************************************************************/
int CMesh::Render( CMatrix &m, bool transform )
{
    rendered = true;

    if( transform )
    {
        transformed = true;

        // Transform all the vertices
        m.Transform( vList, vCount );

        // Transform all the surface normals
        m.TransformVector( nList, polyCount );
    }

	if( giRenderDevice == ERD_DIRECTX )
	{
	    return RenderDirectX(m);
	}
	else if( giRenderDevice == ERD_OPENGL )
	{
	    return RenderOpenGL(m);
	}
	else if( giRenderDevice == ERD_SOFTWARE )
	{
	    return RenderSoftware(m);
	}

    return 0;

}   // Render


/************************************************************************
*    DESCRIPTION:  Render the 3D object to on screen buffer.
*
*    Input:    Matrix3D &m - Matrix to do all the translations
*              float portRadius - portal size
*              CPoint *pPortalCenter - Center of portal
*
*    Output:   int - Number of polygons sent to be rendered
************************************************************************/
int CMesh::Render( CMatrix &m, float portRadius, CPoint *pPortalCenter, bool transform, bool shadow )
{
    rendered = true;

    if( transform )
    {
        transformed = true;

        // Transform all the vertices
        m.Transform( vList, vCount );

        // Transform all the surface normals
        m.TransformVector( nList, polyCount );
    }
    
    if( giRenderDevice == ERD_DIRECTX )
    {
        return RenderDirectX( m, portRadius, pPortalCenter );
    }
    else if( giRenderDevice == ERD_OPENGL )
    {
        return RenderOpenGL( m, portRadius, pPortalCenter );
    }
    else if( giRenderDevice == ERD_SOFTWARE )
    {
        return RenderSoftware( m, portRadius, pPortalCenter );
    }

    return 0;

}   // Render


/************************************************************************
*    DESCRIPTION:  Transform the points
*
*    Input:    Matrix3D &m - Matris to do all the translations
************************************************************************/
void CMesh::Transform( CMatrix &m )
{
    transformed = true;

    // Transform all the vertices
    m.Transform( vList, vCount );

    // Transform all the surface normals
    m.TransformVector( nList, polyCount );

}   // Transform


/************************************************************************
*    DESCRIPTION:  Transform the center point
*                  This has to be done before bounding sphere or distance
*                  checks.
*
*    Input:    Matrix3D &m - Matris to do all the translations
************************************************************************/
void CMesh::TransformCenterPt( CMatrix &m )
{
    // Tansform the center point of the mesh
    m.Transform( centerPt );

}   // TransformCenterPt


/************************************************************************
*    DESCRIPTION:  Transform the bounding box
*                  This has to be done before bounding rect or distance
*                  checks.
*
*    Input:    Matrix3D &m - Matris to do all the translations
************************************************************************/
void CMesh::TransformBoundingBox( CMatrix &m )
{
    // Tansform the bounding box
    m.Transform( boundingBox, 8 );

}   // TransformBoundingBox


/************************************************************************
*    DESCRIPTION:  Translate the light list
************************************************************************/
void CMesh::TransformLightList( CMatrix &m )
{
	pLightList->Transform( m );

}	// TranslateLightList


/************************************************************************
*    DESCRIPTION:  Bind a texture into video memory
************************************************************************/
void CMesh::BindTexture( unsigned int polyIndex )
{
    if( pTextLib != NULL && polyIndex < (unsigned int)polyCount )
	{
	    // It is assumed each mesh only uses one texture
	    if( giRenderDevice == ERD_DIRECTX )
	    {
	        pList[polyIndex].BindDirectXText( pTextLib );
	    }
	    else if( giRenderDevice == ERD_OPENGL )
	    {
	        pList[polyIndex].BindOpenGLText( pTextLib );
	    }
	    else if( giRenderDevice == ERD_SOFTWARE )
	    {
			pList[polyIndex].BindSoftwareText();
	    }
	}

}   // BindTexture


/************************************************************************
*    DESCRIPTION:  Render the 3D object to on screen buffer.
*
*    Output:   int - Number of polygons sent to be rendered
************************************************************************/
int CMesh::RenderOpenGLShadow( CPoint &offset )
{
    int renderCount(0);
	CPoint shadowOffset;

    // Display selected polygons in the off-screen buffer
    for( int i = 0; i < polyCount; ++i )
    {
        // Eliminate polygons that are not seen or have their backs to us
        if( pList[i].IsShadowVisable() )
        {
            // Render
            pList[i].RenderOpenGLShadow( shadowOffset );

            shadowOffset += offset;

            // Count up the number of polygons sent to be rendered
            ++renderCount;
        }
    }

    return renderCount;

}   // RenderOpenGLShadow


/************************************************************************
*    DESCRIPTION:  Render the 3D object to on screen buffer.
*
*    Output:   int - Number of polygons sent to be rendered
************************************************************************/
int CMesh::RenderOpenGL( CMatrix &m )
{
    int renderCount(0);

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
                    pList[i].RenderOpenGLText( pTextLib );

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
                    pList[i].RenderOpenGLText( pTextLib );

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Transform the light list
			TransformLightList( m );

            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable() )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pList[i].GetUVRGB(), pList[i].GetVertexCount() );
                    
                    // Calculate a new intensity
                    pList[i].CalcLightIntenDynamic( pLightList );

                    // Render
                    pList[i].RenderOpenGLText( pTextLib );

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
                    pList[i].RenderOpenGL();

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
                    pList[i].RenderOpenGL();

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Transform the light list
			TransformLightList( m );
            
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable() )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pList[i].GetUVRGB(), pList[i].GetVertexCount() );
                    
                    // Calculate a new intensity
                    pList[i].CalcLightIntenDynamic( pLightList );

                    // Render
                    pList[i].RenderOpenGL();

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }
        }
    }

    return renderCount;

}   // RenderOpenGL


/************************************************************************
*    DESCRIPTION:  Render the 3D object to on screen buffer.
*
*    Output:   int - Number of polygons sent to be rendered
************************************************************************/
int CMesh::RenderSoftware( CMatrix &m )
{
    int renderCount(0);

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
                    // Project
                    pList[i].ClipProject( pTextLib );

                    // Do the 2d culling
                    if( pList[i].IsVisable2D() )
					{
	                    if( !pTextLib->pTMap[ pList[i].GetTextIndex() ].AlphaBlend() )
	                    {
	                        // Render
	                        if( pList[i].GetClipped() )
	                            pList[i].RenderClipped( pTextLib );
	                        else
	                            pList[i].Render( pTextLib );
	                    }
	                    else
	                    {
	                        // Render
	                        if( pList[i].GetClipped() )
	                            pList[i].RenderClippedAlpha( pTextLib );
	                        else
	                            pList[i].RenderAlpha( pTextLib );
	                    }
					}

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

                    // Project
                    pList[i].ClipProject( pTextLib );

                    // Do the 2d culling
                    if( pList[i].IsVisable2D() )
					{
	                    if( !pTextLib->pTMap[ pList[i].GetTextIndex() ].AlphaBlend() )
	                    {
	                        // Render
	                        if( pList[i].GetClipped() )
	                            pList[i].RenderClipped( pTextLib );
	                        else
	                            pList[i].Render( pTextLib );
	                    }
	                    else
	                    {
	                        // Render
	                        if( pList[i].GetClipped() )
	                            pList[i].RenderClippedAlpha( pTextLib );
	                        else
	                            pList[i].RenderAlpha( pTextLib );
	                    }
					}

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Transform the light list
			TransformLightList( m );

            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable() )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pList[i].GetUVRGB(), pList[i].GetVertexCount() );
                    
                    // Calculate a new intensity
                    pList[i].CalcLightIntenDynamic( pLightList );

                    // Project
                    pList[i].ClipProject( pTextLib );

                    // Do the 2d culling
                    if( pList[i].IsVisable2D() )
					{
	                    if( !pTextLib->pTMap[ pList[i].GetTextIndex() ].AlphaBlend() )
	                    {
	                        // Render
	                        if( pList[i].GetClipped() )
	                            pList[i].RenderClipped( pTextLib );
	                        else
	                            pList[i].Render( pTextLib );
	                    }
	                    else
	                    {
	                        // Render
	                        if( pList[i].GetClipped() )
	                            pList[i].RenderClippedAlpha( pTextLib );
	                        else
	                            pList[i].RenderAlpha( pTextLib );
	                    }
					}

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
                    // Project
                    pList[i].ClipProject();

                    // Do the 2d culling
                    if( pList[i].IsVisable2D() )
					{
	                    // Render
	                    pList[i].Render_WireFrame();
					}

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

                    // Project
                    pList[i].ClipProject();

                    // Do the 2d culling
                    if( pList[i].IsVisable2D() )
					{
	                    // Render
	                    pList[i].Render_WireFrame();
					}

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Transform the light list
			TransformLightList( m );

            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable() )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pList[i].GetUVRGB(), pList[i].GetVertexCount() );

                    // Project
                    pList[i].ClipProject();
                    
                    // Do the 2d culling
                    if( pList[i].IsVisable2D() )
					{
	                    // Calculate a new intensity
	                    pList[i].CalcLightIntenDynamic( pLightList );

	                    // Render
	                    pList[i].Render_WireFrame();
					}

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }
        }
    }

    gSoftwareTextIndex = -1;

    return renderCount;

}   // RenderSoftware


/************************************************************************
*    DESCRIPTION:  Render the 3D object to on screen buffer.
*
*    Output:   int - Number of polygons sent to be rendered
************************************************************************/
int CMesh::RenderDirectXShadow( CPoint &offset )
{
    int renderCount(0);
	CPoint shadowOffset;

	// reset the counter
    gTriCounter = 0;

    // Display selected polygons in the off-screen buffer
    for( int i = 0; i < polyCount; ++i )
    {
        // Eliminate polygons that are not seen or have their backs to us
        if( pList[i].IsShadowVisable() )
        {
            // Render
            pList[i].RenderDirectXShadow( shadowOffset );

            shadowOffset += offset;

            // Count up the number of polygons sent to be rendered
            ++renderCount;
        }
    }

	// Finish off any remaining rendering
    DrawDirectXPrimatives();

    return renderCount;

}   // RenderDirectXShadow


/************************************************************************
*    DESCRIPTION:  Render the 3D object to on screen buffer.
*
*    Output:   int - Number of polygons sent to be rendered
************************************************************************/
int CMesh::RenderDirectX( CMatrix &m )
{
    int renderCount(0);

    // reset the counter
    gTriCounter = 0;

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
                    pList[i].RenderDirectXText( pTextLib );

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
                    pList[i].RenderDirectXText( pTextLib );

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Transform the light list
			TransformLightList( m );

            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable() )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pList[i].GetUVRGB(), pList[i].GetVertexCount() );
                    
                    // Calculate a new intensity
                    pList[i].CalcLightIntenDynamic( pLightList );

                    // Render
                    pList[i].RenderDirectXText( pTextLib );

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
                    pList[i].RenderDirectX();

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
                    pList[i].RenderDirectX();

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Transform the light list
			TransformLightList( m );

            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable() )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pList[i].GetUVRGB(), pList[i].GetVertexCount() );
                    
                    // Calculate a new intensity
                    pList[i].CalcLightIntenDynamic( pLightList );

                    // Render
                    pList[i].RenderDirectX();

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }
        }
    }

    // Finish off any remaining rendering
    DrawDirectXPrimatives();

    return renderCount;

}   // RenderDirectX


/************************************************************************
*    DESCRIPTION:  Render the 3D object to on screen buffer.
*
*    Input:    float portRadius - portal size
*              CPoint *pPortalCenter - Center of portal
*
*    Output:   int - Number of polygons sent to be rendered
************************************************************************/
int CMesh::RenderDirectX( CMatrix &m, float portRadius, CPoint *pPortalCenter )
{
    int renderCount(0);

    // reset the counter
    gTriCounter = 0;

    // Do we have textures?
    if( pTextLib != NULL )
    {
        if( pLightList == NULL || pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Render
                    pList[i].RenderDirectXText( pTextLib );

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
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Calculate a new intensity
                    pList[i].CalcLightIntenStatic( pLightList );

                    // Render
                    pList[i].RenderDirectXText( pTextLib );

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Transform the light list
			TransformLightList( m );

            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pList[i].GetUVRGB(), pList[i].GetVertexCount() );
                    
                    // Calculate a new intensity
                    pList[i].CalcLightIntenDynamic( pLightList );

                    // Render
                    pList[i].RenderDirectXText( pTextLib );

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
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Render
                    pList[i].RenderDirectX();

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
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Calculate a new intensity
                    pList[i].CalcLightIntenStatic( pLightList );

                    // Render
                    pList[i].RenderDirectX();

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Transform the light list
			TransformLightList( m );

            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pList[i].GetUVRGB(), pList[i].GetVertexCount() );
                    
                    // Calculate a new intensity
                    pList[i].CalcLightIntenDynamic( pLightList );

                    // Render
                    pList[i].RenderDirectX();

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }
        }
    }

    // Finish off any remaining rendering
    DrawDirectXPrimatives();

    return renderCount;

}   // RenderDirectX


/************************************************************************
*    DESCRIPTION:  Render the 3D object to on screen buffer.
*
*    Input:    float portRadius - portal size
*              CPoint *pPortalCenter - Center of portal
*
*    Output:   int - Number of polygons sent to be rendered
************************************************************************/
int CMesh::RenderSoftware( CMatrix &m, float portRadius, CPoint *pPortalCenter )
{
    int renderCount(0);

    // Do we have textures?
    if( pTextLib != NULL )
    {
        if( pLightList == NULL || pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Project
                    pList[i].ClipProject( pTextLib );

                    // Do the 2d culling
                    if( pList[i].IsVisable2D() )
					{
	                    if( !pTextLib->pTMap[ pList[i].GetTextIndex() ].AlphaBlend() )
	                    {
	                        // Render
	                        if( pList[i].GetClipped() )
	                            pList[i].RenderClipped( pTextLib );
	                        else
	                            pList[i].Render( pTextLib );
	                    }
	                    else
	                    {
	                        // Render
	                        if( pList[i].GetClipped() )
	                            pList[i].RenderClippedAlpha( pTextLib );
	                        else
	                            pList[i].RenderAlpha( pTextLib );
	                    }
					}

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
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Calculate a new intensity
                    pList[i].CalcLightIntenStatic( pLightList );

                    // Project
                    pList[i].ClipProject( pTextLib );

					// Do the 2d culling
					if( pList[i].IsVisable2D() )
					{
	                    if( !pTextLib->pTMap[ pList[i].GetTextIndex() ].AlphaBlend() )
	                    {
	                        // Render
	                        if( pList[i].GetClipped() )
	                            pList[i].RenderClipped( pTextLib );
	                        else
	                            pList[i].Render( pTextLib );
	                    }
	                    else
	                    {
	                        // Render
	                        if( pList[i].GetClipped() )
	                            pList[i].RenderClippedAlpha( pTextLib );
	                        else
	                            pList[i].RenderAlpha( pTextLib );
	                    }
					}

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Transform the light list
			TransformLightList( m );

            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pList[i].GetUVRGB(), pList[i].GetVertexCount() );
                    
                    // Calculate a new intensity
                    pList[i].CalcLightIntenDynamic( pLightList );

                    // Project
                    pList[i].ClipProject( pTextLib );

                    // Do the 2d culling
                    if( pList[i].IsVisable2D() )
					{
	                    if( !pTextLib->pTMap[ pList[i].GetTextIndex() ].AlphaBlend() )
	                    {
	                        // Render
	                        if( pList[i].GetClipped() )
	                            pList[i].RenderClipped( pTextLib );
	                        else
	                            pList[i].Render( pTextLib );
	                    }
	                    else
	                    {
	                        // Render
	                        if( pList[i].GetClipped() )
	                            pList[i].RenderClippedAlpha( pTextLib );
	                        else
	                            pList[i].RenderAlpha( pTextLib );
	                    }
					}

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
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Project
                    pList[i].ClipProject();

                    // Do the 2d culling
                    if( pList[i].IsVisable2D() )
					{
	                    // Render
	                    pList[i].Render_WireFrame();
					}

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
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Calculate a new intensity
                    pList[i].CalcLightIntenStatic( pLightList );

                    // Project
                    pList[i].ClipProject();

                    // Do the 2d culling
                    if( pList[i].IsVisable2D() )
					{
	                    // Render
	                    pList[i].Render_WireFrame();
					}

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Transform the light list
			TransformLightList( m );

            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pList[i].GetUVRGB(), pList[i].GetVertexCount() );

                    // Project
                    pList[i].ClipProject();

                    // Do the 2d culling
                    if( pList[i].IsVisable2D() )
					{
	                    // Calculate a new intensity
                    	pList[i].CalcLightIntenDynamic( pLightList );

	                    // Render
	                    pList[i].Render_WireFrame();
					}

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }
        }
    }

	gSoftwareTextIndex = -1;

    return renderCount;

}   // RenderSoftware


/************************************************************************
*    DESCRIPTION:  Render the 3D object to on screen buffer.
*
*    Input:    float portRadius - portal size
*              CPoint *pPortalCenter - Center of portal
*
*    Output:   int - Number of polygons sent to be rendered
************************************************************************/
int CMesh::RenderOpenGL( CMatrix &m, float portRadius, CPoint *pPortalCenter )
{
    int renderCount(0);

    // Do we have textures?
    if( pTextLib != NULL )
    {
        if( pLightList == NULL || pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Render
                    pList[i].RenderOpenGLText( pTextLib );

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
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Calculate a new intensity
                    pList[i].CalcLightIntenStatic( pLightList );

                    // Render
                    pList[i].RenderOpenGLText( pTextLib );

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Transform the light list
			TransformLightList( m );

            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pList[i].GetUVRGB(), pList[i].GetVertexCount() );
                    
                    // Calculate a new intensity
                    pList[i].CalcLightIntenDynamic( pLightList );

                    // Render
                    pList[i].RenderOpenGLText( pTextLib );

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
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Render
                    pList[i].RenderOpenGL();

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
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Calculate a new intensity
                    pList[i].CalcLightIntenStatic( pLightList );

                    // Render
                    pList[i].RenderOpenGL();

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Transform the light list
			TransformLightList( m );

            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pList[i].IsVisable( portRadius, pPortalCenter ) )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pList[i].GetUVRGB(), pList[i].GetVertexCount() );
                    
                    // Calculate a new intensity
                    pList[i].CalcLightIntenDynamic( pLightList );

                    // Render
                    pList[i].RenderOpenGL();

                    // Count up the number of polygons sent to be rendered
                    ++renderCount;
                }
            }
        }
    }

    return renderCount;

}   // RenderOpenGL


/************************************************************************
*    DESCRIPTION:  Check for collision. Once a collision is made
*                  we break from the loop to allow the game to
*                  process the polygon we collided with. The game
*                  usually keeps calling this function intil all
*                  the polygons have been checked.
*
*    Input:    int &index - Index to start searching from.
*              double &colDistance - Distance from collision
*              CPoint &pt - Center point of the translated object.
*                             Usually the vertex gotten from the matrix
*                             being used to translate the object.
*              float rad - radius of the object the center point belongs to.
*              ECT_COLLISION_TYPE checkSpecificType - Specific type to check for
*
*    Output:   int - Index of the texture being used. This is a way to tell
*                    what we are colliding with.
************************************************************************/
ECT_COLLISION_TYPE _fastcall CMesh::IsCollision_BoundSphere( int &index, double &colDistance, CPoint &pt,
                                                             float rad, ECT_COLLISION_TYPE checkSpecificType )
{
    ECT_COLLISION_TYPE result(ECT_NO_COLLISION);
    int i(index);
    
    // Check the required collision types if no type is specified
    if( checkSpecificType == ECT_NO_TYPE )
	{
	    // cycle through until we find a collision
	    for( ; i < polyCount; ++i )
	    {
	        // Only check those that have a type
	        if( pList[i].GetCollisionType() > ECT_NO_TYPE )
	        {
	            result = pList[i].Collide_BoundSphere( pt, colDistance, rad );
	            
	            // Break here and return if we found something
	            if( result > ECT_NO_TYPE )
	                break;
	        }
	    }
	}
	else
	{
	    // cycle through until we find a collision
	    for( ; i < polyCount; ++i )
	    {
	        // Only check those that have a type
	        if( pList[i].GetCollisionType() == checkSpecificType )
	        {
	            result = pList[i].Collide_BoundSphere( pt, colDistance, rad );
	            
	            // Break here and return if we found something
	            if( result > ECT_NO_TYPE )
	                break;
	        }
	    }
	}

    index = i;

    return result;

}   // IsCollision_BoundSphere


/************************************************************************
*    DESCRIPTION:  Check for collision. Once a collision is made
*                  we break from the loop to allow the game to
*                  process the polygon we collided with. The game
*                  usually keeps calling this function intil all
*                  the polygons have been checked.
*
*                  This checks untransformed points
*
*    Input:    int &index - Index to start searching from.
*              double &colDistance - Distance from collision
*              CPoint &pt - Center point of the translated object.
*                             Usually the vertex gotten from the matrix
*                             being used to translate the object.
*              float rad - radius of the object the center point belongs to.
*              ECT_COLLISION_TYPE checkSpecificType - Specific type to check for
*
*    Output:   int - Index of the texture being used. This is a way to tell
*                    what we are colliding with.
************************************************************************/
ECT_COLLISION_TYPE _fastcall CMesh::IsCollision_BoundSphereSt( int &index, double &colDistance, CPoint &pt,
                                                               float rad, ECT_COLLISION_TYPE checkSpecificType )
{
    ECT_COLLISION_TYPE result(ECT_NO_COLLISION);
    int i(index);
    
    // Check the required collision types if no type is specified
    if( checkSpecificType == ECT_NO_TYPE )
	{
	    // cycle through until we find a collision
	    for( ; i < polyCount; ++i )
	    {
	        // Only check those that have a type
	        if( pList[i].GetCollisionType() > ECT_NO_TYPE )
	        {
	            result = pList[i].Collide_BoundSphereSt( pt, colDistance, rad );
	            
	            // Break here and return if we found something
	            if( result > ECT_NO_TYPE )
	                break;
	        }
	    }
	}
	else
	{
	    // cycle through until we find a collision
	    for( ; i < polyCount; ++i )
	    {
	        // Only check those that have a type
	        if( pList[i].GetCollisionType() == checkSpecificType )
	        {
	            result = pList[i].Collide_BoundSphereSt( pt, colDistance, rad );
	            
	            // Break here and return if we found something
	            if( result > ECT_NO_TYPE )
	                break;
	        }
	    }
	}

    index = i;

    return result;

}   // IsCollision_BoundSphereSt


/************************************************************************
*    DESCRIPTION:  Last check for collision. Much more accurate
*                  then bounding spheres and also much slower
*                  which is why this check is performed after
*                  a bounding sphere's tests are finished. Also,
*                  intersection collision is testing if the plane
*                  was pierced by a point. We are talking a plain,
*                  not a polygon. So the test is not confined to
*                  the size of the polygon, but the plain it is on.
*                  A plain is infanit.
*
*                  The resulting calculations are reused for
*                  handing the collision be it gravity or
*                  wall plaining.
*
*    Input:    CPoint &triVert - tirangle point              
*              float padding - distance between point and polygon
*              CPoint &pt - point to check.
*              *NOTE: If in First Persion point of view, x, y, z is
*              not the current point on the map. use 0,0,0 or 1,1,1
*
*    Output:   bool - true on success, false on fail
************************************************************************/
bool CMesh::IsCollision_Intersection( CPoint &normal, CPoint &triVert, CPoint &pt,
                                      float padding, float &pointD )
{
    // Here we define the plane for a polygon edge. This can also be used
	// as the offset for pushing back against the floor or wall
	pointD = normal.GetDotProductDy( triVert );

    // Confirm/reject collision by calculating which side of the plane
    // the point will be on. In other words, did we pierce the plain
	float finalCheck = normal.GetDotProductDy( pt ) + pointD + -padding;

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
*    DESCRIPTION:  Last check for collision. Much more accurate
*                  then bounding spheres and also much slower
*                  which is why this check is performed after
*                  a bounding sphere's tests are finished. Also,
*                  intersection collision is testing if the plane
*                  was pierced by a point. We are talking a plain,
*                  not a polygon. So the test is not confined to
*                  the size of the polygon, but the plain it is on.
*                  A plain is infanit.
*
*                  The resulting calculations are reused for
*                  handing the collision be it gravity or
*                  wall plaining.
*
*    Input:    Matrix3D &M - Matris to do all the translations
*              int triIndex - Index of polygon to check
*              float padding - distance between point and polygon
*              CPoint &pt - point to check.
*              *NOTE: If in First Persion point of view, x, y, z is
*              not the current point on the map. use 0,0,0 or 1,1,1
*
*    Output:   bool - true on success, false on fail
************************************************************************/
bool CMesh::IsCollision_IntersectionSt( int triIndex, float padding, CPoint &pt,
                                        CPoint &normal, float &pointD )
{
    // Get a copy of the firct point
    CPoint tri( *pList[triIndex].GetVPoint(0) );
    
    // Get a copy of the normal 
    normal.Init( *pList[triIndex].GetNormal() );

    // Here we define the plane for a polygon edge. This can also be used
	// as the offset for pushing back against the floor or wall
	pointD = normal.GetDotProductSt( tri );

	// Confirm/reject collision by calculating which side of the plane
    // the point will be on. In other words, did we pierce the plain
	float finalCheck = normal.GetDotProductSt( pt ) + pointD + -padding;

    // Final check of intersection collision.
    // Did we perce the plain?
    if( finalCheck <= 0.0f )
    {        
        // The eagle has landed.
        return true;
    }

    return false;

}   // IsCollision_IntersectionSt


/************************************************************************
*    DESCRIPTION:  Get the average height of the polygon
*
*    Input:    int triIndex -  index of the triangle to get the height of
*
*    Output:   float - average height of the polygon
************************************************************************/
float CMesh::GetHeightOfPolygon( int triIndex )
{
	return pList[triIndex].GetAverageHeightSt();

}	// GetHeightOfPoint


/************************************************************************
*    DESCRIPTION:  Scale this mesh.
*
*    Input:    float x, y, z: Scale amounts
*
*    Output:   returntype - Description
************************************************************************/
void CMesh::Scale( float x, float y, float z )
{
    CMatrix m;

    // Create a matrix with the scaling factors
    m.Scale( x, y, z );

    // Transform all the vertices
    m.Transform( vList, vCount );
    
    // Copy it over and make it perminate
    for( int i = 0; i < vCount; ++i )
    {        
        vList[i].MakePermanent();                 
    }

    // Recalculate the bounding polygons. Used for collision
    // detection and view frustum culling
    CalBoundingPoly();

    // Recalculate the bounding sphere for this object. Useful for
    // doing full object culling.
    CalBoundingSphereBox();

}   // Scale


/************************************************************************
*    DESCRIPTION:  Calculate bounding polygon
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
*    DESCRIPTION:  Calculate normals
************************************************************************/
void CMesh::CalNormals()
{
    // Calculate the Surface Normal(surface description)
    for( int i = 0; i < polyCount; ++i )
    {
        pList[ i ].CalcSurfaceNormal();
    }

}   // CalNormals


/************************************************************************
*    DESCRIPTION:  Set the list of light data
*
*    Input:    CLightList &lightList - Pointer to light list
************************************************************************/
void CMesh::SetLightList( CLightList * pLiteLst )
{
    pLightList = pLiteLst;

    // Calculate the RGB color intensity per vertex. Only do this
    // here if we have a light list and we are NOT doing dynamic lightning.
    // Dynamic lighting is done on each render cycle so there's no need
    // to do it here on startup.
    if( pLiteLst != NULL && pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
    {
        for( int i = 0; i < polyCount; ++i )
        {
            pList[i].CalcLightIntenStatic( pLightList );
        }
    }

}   // SetLightList


/************************************************************************
*    DESCRIPTION:  Load the polygon data
*
*    Input:    FILE *hFile - Handle to file
*
*    Output:   bool - true on success, false on fail
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
*    DESCRIPTION:  Save the polygon data
*
*    Input:    FILE *hFile - Handle to file
*
*    Output:   bool - true on success, false on fail
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
*    DESCRIPTION:  Copy the supplied mesh
*
*    Input:    CMatrix &mesh - Mesh to copy
*
*    Output:   returntype - Description
************************************************************************/
bool CMesh::CopyMesh( CMesh &mesh )
{
    // Free all allocations
    Free();

    vCount = mesh.GetVertCount();
    polyCount = mesh.GetPolyCount();
    pTextLib = mesh.GetTextLibary();
    pLightList = mesh.GetLightList();
	minPoint.Init( mesh.GetMinPoint() );
	maxPoint.Init( mesh.GetMaxPoint() );

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
*    DESCRIPTION:  Merge two meshes into this one. Assumes same
*                  number of vertexes and polygons
*
*    Input:    CMatrix &mesh1 - First mest
*              CMatrix &mesh2 - Second Mesh
*              float amount - Interpulate amount
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


/************************************************************************
*    DESCRIPTION:  Depending on the angle of the normal, set the polygon
*                  Collision type to wall or floor.
************************************************************************/
void CMesh::SetWallFloorCollisionType()
{
    // first set all the floors
    for( int i = 0; i < polyCount; ++i )
    {        
        if( pList[i].GetCollisionType() == ECT_NO_TYPE )
        {
            float normY = pList[i].GetNormal()->y;

            if( normY >= -1.0F && normY < -0.1 )
            {
                pList[i].SetCollisionType( ECT_MAP_FLOOR );
            }
        }
    }

    // Now that the floors are tagged, tag only the walls the touch the floors
    for( int i = 0; i < polyCount; ++i )
    {        
        if( pList[i].GetCollisionType() == ECT_NO_TYPE )
        {
            float normY = pList[i].GetNormal()->y;

            if( fabs( normY ) < 0.10f )
            {
                for( int j = 0; j < polyCount; ++j )
                {
                	if( pList[j].GetCollisionType() == ECT_MAP_FLOOR )
					{
                		CPoint centerPt;
						double dummy;

						// Get the center of the untransformed polygon
						pList[j].CalcPolyCenterPointSt( centerPt );

                		// See if this floor collides with this wall
                		if( pList[i].Collide_BoundSphereSt( centerPt, dummy, pList[j].GetCollideRadius() ) != ECT_NO_COLLISION )
						{
                			pList[i].SetCollisionType( ECT_MAP_WALL );
						}
					}
				}
            }
        }
    }

}   // SetWallFloorCollisionType


/************************************************************************
*    DESCRIPTION:  Calculate the distance of the center point
************************************************************************/
float CMesh::GetDistance()
{
    return sqrt( centerPt.GetLengthDy() );

}   // GetDistance


/************************************************************************
*    DESCRIPTION: Adjust the radius depending on the need  
*
*    Input:  float scaler - Amount to scale the radius by
*            ECT_COLLISION_TYPE colType -  collision type looking for
************************************************************************/
void CMesh::AdjustRadius( float scaler, ECT_COLLISION_TYPE colType )
{
	for( int i = 0; i < polyCount; ++i )
    {
        if( pList[i].GetCollisionType() == colType )
		{
        	pList[i].AdjustCollideRadius( scaler );
		}
    }
}	// AdjustRadius


/************************************************************************
*    DESCRIPTION: Adjust the radius depending on the need  
*
*    Input:  float rad - new radius to set to a specific type of polygon
*            ECT_COLLISION_TYPE colType -  collision type looking for
************************************************************************/
void CMesh::SetRadius( float rad, ECT_COLLISION_TYPE colType )
{
	for( int i = 0; i < polyCount; ++i )
    {
        if( pList[i].GetCollisionType() == colType )
		{
        	pList[i].AdjustCollideRadius( rad );
		}
    }
}	// AdjustRadius


/************************************************************************
*    DESCRIPTION: Set the texture index  
*
*    Input:  int index - index of texture
************************************************************************/
void CMesh::SetTextIndex( int index )
{
	for( int i = 0; i < polyCount; ++i )
    {
        pList[i].SetTextIndex( index );
    }
}	// SetTextIndex


/************************************************************************
*    DESCRIPTION:    Generate a 16 bit shaded table
*
*    Input:    unsigned short int *shadeTbl - Pointer to shaded table
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


/************************************************************************
*    DESCRIPTION: Find the closest light  
************************************************************************/
CLight * CMesh::FindClosestLight( CLightList * pLiteLst )
{
	CLight *light = NULL;

	if( pLiteLst != NULL )
	{		
		bool lightFound = false;
		float dist;
		float shortDist;

		// Find the closest point light
		for( int i = 0; i < pLiteLst->Count() && !lightFound; i++ )
		{
			// Find the first point light in the list
			if( pLiteLst->GetLight( i )->GetLightType() > ELT_INFINITE )
			{
				shortDist = centerPt.GetLengthDy( pLiteLst->GetLight( i )->GetPos() );
				light = pLiteLst->GetLight( i );

				// Go through the rest of the lights looking for the closest light
				for( int j = i+1; j < pLiteLst->Count(); j++ )
				{
					if( pLiteLst->GetLight( j )->GetLightType() > ELT_INFINITE )
					{
						dist = centerPt.GetLengthDy( pLiteLst->GetLight( j )->GetPos() );

						if( dist < shortDist )
						{
							shortDist = dist;
							light = pLiteLst->GetLight( j );
						}
					}
				}

				lightFound = true;
			}
		}
	}

	return light;

}	// FindClosestLight


/************************************************************************
*    DESCRIPTION: Convert the transformed point to a flat shadow  
************************************************************************/
void CMesh::ConvertToShadow( CMesh &mesh,
							 CMatrix &m,
							 CPoint &light,
							 CPoint &projectDir,
							 CPoint &areaMinPoint,
							 CPoint &areaMaxPoint )
{
	float shadowColor = 0;
	float projSurface;

	if( projectDir.y > 0 )
	{
		m.Transform( maxPoint );
		projSurface = maxPoint.ty;
	}
	else if( projectDir.y < 0 )
	{
		m.Transform( minPoint );
		projSurface = minPoint.ty;
	}
	else if( projectDir.x > 0 )
	{
		m.Transform( maxPoint );
		projSurface = maxPoint.tx;
	}
	else if( projectDir.x < 0 )
	{
		m.Transform( minPoint );
		projSurface = minPoint.tx;
	}
	else if( projectDir.z > 0 )
	{
		m.Transform( maxPoint );
		projSurface = maxPoint.tz;
	}
	else if( projectDir.z < 0 )
	{
		m.Transform( minPoint );
		projSurface = minPoint.tz;
	}

	// Merge the verts
	if( projectDir.y != 0 )
	{
		for( int i = 0; i < vCount; ++i )
		{
		    float ray = -(light.y - projSurface) / (mesh.GetVertexList()[i].ty - light.y);

		    vList[i].y = projSurface;
			vList[i].x = light.x + (ray * (mesh.GetVertexList()[i].tx - light.x));
			vList[i].z = light.z + (ray * (mesh.GetVertexList()[i].tz - light.z));
		}
	}
	else if( projectDir.x != 0 )
	{
		for( int i = 0; i < vCount; ++i )
		{
		    float ray = -(light.x - projSurface) / (mesh.GetVertexList()[i].tx - light.x);

		    vList[i].x = projSurface;
			vList[i].y = light.y + (ray * (mesh.GetVertexList()[i].ty - light.y));
			vList[i].z = light.z + (ray * (mesh.GetVertexList()[i].tz - light.z));
		}
	}
	else if( projectDir.z != 0 )
	{
		for( int i = 0; i < vCount; ++i )
		{
		    float ray = -(light.z - projSurface) / (mesh.GetVertexList()[i].tz - light.z);

		    vList[i].z = projSurface;
			vList[i].x = light.x + (ray * (mesh.GetVertexList()[i].tx - light.x));
			vList[i].y = light.y + (ray * (mesh.GetVertexList()[i].ty - light.y));
		}
	}

	for( int i = 0; i < polyCount; ++i )
	{
	    pList[i].CalcSurfaceNormal();
	}

}	// ConvertToShadow