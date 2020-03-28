
/************************************************************************
*
*    PRODUCT:         Software Rendering 3D Engine based on triangles only.
*
*    FILE NAME:       CMeshObj.cpp
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
#include "CMeshObj.h"        // Header file for this *.cpp file.

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

CMeshObj::CMeshObj()
{
    vCount = 0;
    triCount = 0;
    vList = NULL;
    triList = NULL;
    nList = NULL;
    dynamicLight = EDL_NO_DYNAMIC_LIGHTNING;
    fObjectCulling = false;
    radius = 0.0;
    radiusX = 0.0;
    radiusY = 0.0;
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

CMeshObj::~CMeshObj()
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

void CMeshObj::Free()
{
    if( vList != NULL )
    {
        delete [] vList;
        vList = NULL;
        vCount = 0;
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

void CMeshObj::CalBoundingSphere()
{
    float maxDistanceXYZ = 0.0, maxDistanceX = 0.0, maxDistanceY = 0.0;
    float curDistance;

    for( int i = 0; i < vCount; ++i )
    {
        // Add the XZ vertex
        curDistance = (vList[i].x * vList[i].x) +
                      (vList[i].y * vList[i].y) +
                      (vList[i].z * vList[i].z);

        // Have we found the longest distance?
        if( curDistance > maxDistanceXYZ )
            maxDistanceXYZ = curDistance;

        // Add the XZ vertex
        curDistance = (vList[i].x * vList[i].x) +
                      (vList[i].z * vList[i].z);

        // Have we found the longest distance?
        if( curDistance > maxDistanceX )
            maxDistanceX = curDistance;

        // Add the YZ vertex
        curDistance = (vList[i].y * vList[i].y) +
                      (vList[i].z * vList[i].z);

        // Have we found the longest distance?
        if( curDistance > maxDistanceY )
            maxDistanceY = curDistance;
    }

    // computer the full radius
    radius = (float)sqrt( maxDistanceXYZ );

    // computer the radius X
    radiusX = (float)sqrt( maxDistanceX );

    // computer the radius Y
    radiusY = (float)sqrt( maxDistanceY );

}   // CalBoundingSphere


/************************************************************************
*    FUNCTION NAME:         CalGouraudVertexNormals()                                                             
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

void CMeshObj::CalGouraudVertexNormals()
{
    int sharedIndex[300], sharedIndexCount;
    CPoint N;
    long double nx, ny, nz;
    int tri, vIndex, i, j; 

    // Calculate normals per vertex for all polygons
    for( tri = 0; tri < triCount; ++tri )
    {           
        // Find shared vertices, add them up and average per vertex
        for( vIndex = 0; vIndex < 3; ++vIndex )
        {
            sharedIndexCount = 0;
            nx = ny = nz = 0.0;
            sharedIndex[ 0 ] = tri;

            // Search through the entire list of triangles looking 
            // for shared vertices and record it's index
            for( i = 0; i < triCount; ++i )
            {
                if( triList[ i ].HasVert( *triList[ tri ].pVPoint[ vIndex ] ) )
                    sharedIndex[ sharedIndexCount++ ] = i;
            }

            // Have to have atleast one
            if( !sharedIndexCount )
                sharedIndexCount = 1;

            // Add up all the Surface Normals of all the shared vertices
            for( j = 0; j < sharedIndexCount; ++j )
            {
                nx += (long double)triList[ sharedIndex[ j ] ].pNormal->x;
                ny += (long double)triList[ sharedIndex[ j ] ].pNormal->y;
                nz += (long double)triList[ sharedIndex[ j ] ].pNormal->z;
            }

            // Average out the accumlated Surface Normals
            // to end up with a Surface Normal for each vertex
            N.x = (float)(nx / (long double)sharedIndexCount);
            N.y = (float)(ny / (long double)sharedIndexCount);
            N.z = (float)(nz / (long double)sharedIndexCount);

            // Normalize the Surface Normals
            triList[ tri ].Normalize( N, vIndex );

            // Record the vertex normal for light calculations
            triList[ tri ].uvi[ vIndex ].N.tx = triList[ tri ].uvi[ vIndex ].N.x = N.x;
            triList[ tri ].uvi[ vIndex ].N.ty = triList[ tri ].uvi[ vIndex ].N.y = N.y;
            triList[ tri ].uvi[ vIndex ].N.tz = triList[ tri ].uvi[ vIndex ].N.z = N.z;
        }
    }

}   // CalGouraudVertexNormals


/************************************************************************
*    FUNCTION NAME:         SetObjectCulling()
*
*    DESCRIPTION:           Turn on or off object culling
*
*    FUNCTION PARAMETERS:
*    Input:    int tri - Triangle index
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    05/08/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMeshObj::SetObjectCulling( bool value )
{
    fObjectCulling = value;

    // See if we need to do the calculations
    //if( fObjectCulling && vCount != 0 && radiusX == 0.0 && radiusY == 0.0 )
        //CalBoundingSphere();

}   // SetObjectCulling


/************************************************************************
*    FUNCTION NAME:         LoadFromFile()                                                             
*
*    DESCRIPTION:           Load an object from a PLY binary file
*
*    FUNCTION PARAMETERS:
*    Input:    char *filePath - Path to file to load
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

bool CMeshObj::LoadFromFile( char *filePath )
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

    // Allocate the triangle list
    triList = new CTriangle[ triCount ];
    if( triList == NULL )
        goto HANDLE_ERROR;

    // Allocate the normal list
    nList = new CPoint3D[ triCount ];
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

            // Set the pointer
            triList[i].pVPoint[j] = &vList[vIndex];
        }
        
        // Load the UV corridinates
        for( j = 0; j < 3; ++j )
        {
            if( fread( &triList[i].uvi[j].U, sizeof(triList[i].uvi[j].U), 1, hFile ) != 1 )
                goto HANDLE_ERROR;
                
            if( fread( &triList[i].uvi[j].V, sizeof(triList[i].uvi[j].U), 1, hFile ) != 1 )
                goto HANDLE_ERROR;
        }
        
        // Read in the texture index
        if( fread( &triList[i].tIndex, sizeof(triList[i].tIndex), 1, hFile ) != 1 )
            goto HANDLE_ERROR;

        // Calculate the Surface Normal(surface description)
        triList[ i ].CalcSurfaceNormal(); 
    }

        // Setup the shaded vertices with it's own averaged Surface Normal(surface description)
    CalGouraudVertexNormals();

    // Normalise the Surface Normal(surface descriptions)
    // We have to wait for the Gouraud calculations
    // are done first before normalizing the triangle
    // surface.
    for( i = 0; i < triCount; ++i )
        triList[i].Normalize();

    // Calculate the RGB color intensity per vertex. Only do this
    // here if we have a light list and we are NOT doing dynamic lightning.
    // Dynamic lighting is done on each render cycle so there's no need
    // to do it here on startup.
    if( pLightList != NULL && dynamicLight == EDL_NO_DYNAMIC_LIGHTNING )
    {
        for( i = 0; i < triCount; ++i )
            triList[i].CalcLightInten( pLightList, intensityCap, intensityShift );
    }

    // If a light list was not specified, just apply an ambient color
    if( pLightList == NULL ) 
        SetAmbientLightLevel( 255, 255, 255 );

    // Calculate the bounding sphere for each triangle
    //for( i = 0; i < triCount; ++i )
        //triList[i].CalBoundingTri();

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
*    DESCRIPTION:           Load an object from data pointer
*
*    FUNCTION PARAMETERS:
*    Input:    unsigned char *pData - data pointer
*              var2type Var2Name - Description
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

bool CMeshObj::LoadFromPointer( unsigned char *pData )
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

    // Allocate the triangle list
    triList = new CTriangle[ triCount ];
    if( triList == NULL )
        goto HANDLE_ERROR;

    // Allocate the normal list
    nList = new CPoint3D[ triCount ];
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

            // Set the pointer
            triList[i].pVPoint[j] = &vList[vIndex];
        }
        
        // Load the UV corridinates
        for( j = 0; j < 3; ++j )
        {
            triList[i].uvi[j].U = *(float *)pData;
            pData += sizeof( float );

            triList[i].uvi[j].V = *(float *)pData;
            pData += sizeof( float );
        }
        
        // Read in the texture index
        triList[i].tIndex = *(int *)pData;
        pData += sizeof( int );
            
        // Calculate the surface description
        triList[ i ].CalcSurfaceNormal(); 
    }

    // Setup the shaded vertices with it's own averaged surface description
    CalGouraudVertexNormals();

    // Normalise the surface descriptions
    // We have to wait for the Gouraud calculations
    // are done first before normalizing the triangle
    // surface.
    for( i = 0; i < triCount; ++i )
        triList[i].Normalize();

    // Calculate the RGB color intensity per vertex. Only do this
    // here if we have a light list and we are NOT doing dynamic lightning.
    // Dynamic lighting is done on each render cycle so there's no need
    // to do it here on startup.
    if( pLightList != NULL && dynamicLight == EDL_NO_DYNAMIC_LIGHTNING )
    {
        for( i = 0; i < triCount; ++i )
            triList[i].CalcLightInten( pLightList, intensityCap, intensityShift );
    }

    // If a light list was not specified, just apply an ambient color
    if( pLightList == NULL ) 
        SetAmbientLightLevel( 255, 255, 255 );

    // Did we enable object culling?
    CalBoundingSphere();

    // If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;
    
    return result;

}   // LoadFromPointer


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

void CMeshObj::SetAmbientLightLevel( unsigned int rColor, unsigned int gColor, unsigned int bColor )
{
    unsigned int r, g, b;
    
    // As a safty precaution, make sure each channel was 
    // not set heigher then 255
    r = rColor & 0xFF;
    g = gColor & 0xFF;
    b = bColor & 0xFF;  

    for( int tri = 0; tri < triCount; ++tri )
    {
        for( int vIndex = 0; vIndex < 3; ++vIndex )
        {
            // record the color intensity for this vertex
            triList[tri].uvi[ vIndex ].rI = r;
            triList[tri].uvi[ vIndex ].gI = g;
            triList[tri].uvi[ vIndex ].bI = b;
            triList[tri].uvi[ vIndex ].I = ((r+g+b)/3) >> 3;
        }
    }

}   // SetAmbientLightLevel


/************************************************************************
*    FUNCTION NAME:         CullObject()                                                             
*
*    DESCRIPTION:           Test position of object in frustrum to see
*                           if it can be culled
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
*    H Rosenorn    07/03/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMeshObj::CullObject( CMatrix &m )
{
    CPoint3D centerPt;

    // Get the center point of this object
    m.GetObjPoint( centerPt );

    // Test all sides of the view frustrum
    if( centerPt.z - radius > gfMaxZDistance )
        return true;

    else if( centerPt.z + radius < gfMinZDistance )
        return true;

    else if( centerPt.x - radius > centerPt.z )
        return true;

    else if( centerPt.x + radius < -centerPt.z )
        return true;

    else if( centerPt.y - radius > centerPt.z )
        return true;

    else if( centerPt.y + radius < -centerPt.z )
        return true;

    return false;

}   // CullObject


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

int CMeshObj::Render( CMatrix &m )
{   
    int renderCount;
    
    // Do we want to cull this object?
    if( fObjectCulling )
    {
        // Returns true if the object can't be 
        // seen in the view frustrum
        if( CullObject( m ) )
            return 0;
    }

    // Transform all the vertices
    m.Transform( vList, vCount );

    // Transform all the normals
    m.Transform( nList, triCount );

    // Clear the projected flags
    for( int j = 0; j < vCount; ++j )
        vList[j].fProjected = false;

    // Render depending on the texture library
    if( renderBitCount == 16 )
    {
        renderCount = Render16( m );
    }
    else
    {
        renderCount = Render32( m );
    }

    return renderCount;

}   // Render


/************************************************************************
*    FUNCTION NAME:         Render32()                                                             
*
*    DESCRIPTION:           Render the 3D object in RGB on screen buffer.
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
*    H Rosenorn    08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int CMeshObj::Render32( CMatrix &m )
{
    int renderCount(0);

    // Display selected triangles in the off-screen buffer
    for( int i = 0; i < triCount; ++i )
    {
        // Eliminate triangles that are not seen or have their backs to us
        if( triList[i].CalcVisible3D() )
        {
            // Clip and project the points
            triList[i].ClipProjectXYZ();

            // See if the triangle is visible in the video buffer
            if( triList[i].CalcVisible2D() )
            {                                
                // Do we want to recalculate light on each render cycle
                // for this triangle?                    
                if( dynamicLight == EDL_LOCAL_DYNAMIC_LIGHTNING )
                {
                    // Calculate a new intensity
                    triList[i].CalcLightInten( pLightList, intensityCap, intensityShift );
                }
                else if( dynamicLight == EDL_WORLD_DYNAMIC_LIGHTNING )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( triList[i].uvi );
                    
                    // Calculate a new intensity
                    triList[i].CalcLightInten( pLightList, intensityCap, intensityShift );
                }

                // The projection of the UV can wait til the last moment
                triList[i].ClipProjectUVI();
                
                // render with Z buffering
                if( ZbufType == EZ_ZBUFFER_TRI )
                {
                    
                    /*if( renderType == ERT_WIRE_FRAME )
                    {

                    }
                    else*/ if( renderType == ERT_SOLID_LIGHT_RENDER )
                    {
                        triList[i].RenderSolid32_LiteZBufRGBShading();
                    }
                    /*else if( renderType == ERT_SOLID_TEXT_RENDER )
                    {
                    }*/
                    else if( renderType == ERT_TEXT_RENDER )
                    {
                        // RGB shading
                        if( shadeType == ES_RGB_SHADING )
                            triList[i].Render32_ZBufRGBShading( pTextLib );

                        // Grey scale shading
                        else if( shadeType == ES_GREY_SCALE_SHADING )    
                            triList[i].Render32_ZBufGreyShading( pTextLib );

                        // No shading
                        else if( shadeType == ES_NO_SHADING )
                            triList[i].Render32_ZBufNoShading( pTextLib );
                    }
                }
                // render without z buffering
                else
                {
                    /*
                    if( renderType == ERT_WIRE_FRAME )
                    {

                    }
                    else if( renderType == ERT_SOLID_LIGHT_RENDER )
                    {
                    }
                    else if( renderType == ERT_TEXT_RENDER )
                    {*/
                        // RGB shading
                        if( shadeType == ES_RGB_SHADING )
                            triList[i].Render32_RGBShading( pTextLib );

                        // Grey scale shading
                        else if( shadeType == ES_GREY_SCALE_SHADING )    
                            triList[i].Render32_GreyShading( pTextLib );

                        // No shading
                        else if( shadeType == ES_NO_SHADING )
                            triList[i].Render32_NoShading( pTextLib );
                    //}
                }
                
                // Count up the number of triangles sent to be rendered
                ++renderCount;
            }
        }
    }

    return renderCount;

}   // Render32


/************************************************************************
*    FUNCTION NAME:         Render16()                                                             
*
*    DESCRIPTION:           Render the 3D object in 16-bit on screen buffer.
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
*    H Rosenorn    08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int CMeshObj::Render16( CMatrix &m )
{
    int renderCount(0);

    // Display selected triangles in the off-screen buffer
    for( int i = 0; i < triCount; ++i )
    {
        // Eliminate triangles that are not seen or have their backs to us
        if( triList[i].CalcVisible3D() )
        {
            // Clip and project the points
            triList[i].ClipProjectXYZ();

            // See if the triangle is visible in the video buffer
            if( triList[i].CalcVisible2D() )
            {                                
                // Do we want to recalculate light on each render cycle
                // for this triangle?                    
                if( dynamicLight == EDL_LOCAL_DYNAMIC_LIGHTNING )
                {
                    // Calculate a new intensity
                    triList[i].CalcLightInten( pLightList, intensityCap, intensityShift );
                }
                else if( dynamicLight == EDL_WORLD_DYNAMIC_LIGHTNING )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( triList[i].uvi );
                    
                    // Calculate a new intensity
                    triList[i].CalcLightInten( pLightList, intensityCap, intensityShift );
                }

                // The projection of the I can wait til the last moment
                triList[i].ClipProjectUVI();

                // render with Z buffering
                if( ZbufType == EZ_ZBUFFER_TRI )
                {
                    // Render wire frame
                    if( renderType == ERT_WIRE_FRAME )
                    {
                        //if( shadeType == ES_RGB_SHADING )
                        triList[i].Render16_WireFrame( pTextLib );
                    }
                    else if( renderType == ERT_SOLID_LIGHT_RENDER )
                    {
                        triList[i].Render16_LiteZBufShading( rgbColor16 );
                    }
                    /*else if( renderType == ERT_SOLID_TEXT_RENDER )
                    {
                    }*/
                    // Render with textures
                    else if( renderType == ERT_TEXT_RENDER )
                    {
                        //if( shadeType == ES_RGB_SHADING )
                        triList[i].Render16_ZBufShading( pTextLib );
                    }
                }
                // render without z buffering
                else
                {

                }

                // Count up the number of triangles sent to be rendered
                ++renderCount;
            }
        }
    }

    return renderCount;

}   // Render16


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

void CMeshObj::SetTextureLibrary( CTextureLib &textLib )
{
    // get the pointer to the texture library
    pTextLib = &textLib;

    // Set the intensity cap and render bit count depending
    // on the bit size of the library. 16 & 32 bit color supported.
    if( textLib.GetBitSize() == 16 )
    {
        intensityCap = 31;
        renderBitCount = 16;
        intensityShift = 5;
    }
    else
    {
        intensityCap = 255;
        renderBitCount = 32;
        intensityShift = 8;
    }

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

void CMeshObj::SetLightList( CLightList &lightList, EDyn_Light enableState )
{
    // Set the light list
    pLightList = &lightList;

    // Set the dynamic light type
    dynamicLight = enableState;

    // Set the intensity cap and render bit count depending
    // on the bit size of the library. 16 & 32 bit color supported.
    if( lightList.bitCount == 16 )
    {
        intensityCap = 31;
        renderBitCount = 16;
        intensityShift = 5;
    }
    else
    {
        intensityCap = 255;
        renderBitCount = 32;
        intensityShift = 8;
    }

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