/************************************************************************
*
*    FILE NAME:       CAnimPolygon.cpp
*
*    DESCRIPTION:     Polygon with animation properties
*
************************************************************************/

                           // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT             // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>       // Windows header file for creating windows programs. This is a must have.
#include "CAnimPolygon.h"  // Header file for this *.cpp file.


// Globals
extern float gfMaxZDistance;
extern float gfMinZDistance;


/************************************************************************
*    DESCRIPTION:  Constructer                                                             
*
*    -------------------------------------------------------------------
*                  08/20/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
CAnimPolygon::CAnimPolygon()
{

}   // Constructer


/************************************************************************
*    DESCRIPTION:   Destructer                                                             
*
*    -------------------------------------------------------------------
*                  08/20/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
CAnimPolygon::~CAnimPolygon()
{

}   // Destructer


/************************************************************************
*    DESCRIPTION:  Load polygon data from file
*
*    Input:    FILE *hFile - File handle
*
*    Output:   bool - true on success, false on fail
*
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CAnimPolygon::LoadPolygonData( FILE *hFile, CPoint *vertLst )
{
    CAnimFaceLoad faceLoad;

    // Read in all the data at once
    if( fread( &faceLoad, sizeof(faceLoad), 1, hFile ) != 1 )
        return false;

    // Copy the texture index
    tIndex = (int)faceLoad.tIndex;
    
    // Use the vertex index to set the pointer
    for( int i = 0; i < TRI; ++i )
    {
        // Set the pointer to the vertex list
        pVPoint[i] = &vertLst[ faceLoad.vIndex[i] ];
    }
    
    // Copy the UV corridinates
    for( int i = 0; i < TRI; ++i )
    {
        uvrgb[i].u = faceLoad.crd[i].uv[0];
        uvrgb[i].v = faceLoad.crd[i].uv[1];
    }

    // Load the polygon data
    if( !LoadExtraPolygonData( hFile ) )
        return false;

    return true;

}   // LoadPolygonData


/************************************************************************
*    DESCRIPTION:  Save polygon data from file
*
*    Input:    FILE *hFile - File handle
*              int vertexCount - Number of vertexes in this polygon
*              CPoint *vertLst - pointer to vertex list
*
*    Output:   bool - true on success, false on fail
*
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CAnimPolygon::SavePolygonData( FILE *hFile, int vertexCount, CPoint *vertLst )
{
    CAnimFaceLoad faceLoad;

    // find the vertex index
    for( int i = 0; i < TRI; ++i )
    {
        // Go through the vertex list and find the index
        // of the point we are using
        for( int j = 0; j < vertexCount; ++j )
        {
            if( vertLst[j] == *pVPoint[i] )
            {
                faceLoad.vIndex[i] = (short int)j;
                break;
            }
        }
    }
    
    // Copy the UV corridinates
    for( int i = 0; i < TRI; ++i )
    {
        faceLoad.crd[i].uv[0] = uvrgb[i].u;
        faceLoad.crd[i].uv[1] = uvrgb[i].v;
    }

    // Copy the texture index
    faceLoad.tIndex = (unsigned char)tIndex;

    // Save all the data at once
    if( fwrite( &faceLoad, sizeof(faceLoad), 1, hFile ) != 1 )
        return false;
    
    // Save extra polygon data
    if( !SaveExtraPolygonData( hFile ) )
        return false;

    return true;

}   // SavePolygonData


/************************************************************************
*    DESCRIPTION:  Check if the polygon is visible. Perform 3D culling
*                  This assumes a 45 degree view area.
*
*    Output:   bool - true on success, false on fail
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool _fastcall CAnimPolygon::IsVisable()
{
    bool visible = false;
    
    ////////////////////////////////////////////
    // Eliminate the triangle if it is behind us
    ////////////////////////////////////////////

    for( int i = 0; i < vertCount && !visible; ++i )
    {    
        if( pVPoint[ i ]->tz < gfMinZDistance )
            visible = true;
    }

    if( visible == false )
        return visible;

    /////////////////////////////////////////////////
    // Eliminate the triangle is too far ahead to see
    /////////////////////////////////////////////////

    for( int i = 0; i < vertCount && !visible; ++i )
    {
        if( pVPoint[ i ]->tz > gfMaxZDistance )
            visible = true;
    }

    if( visible == false )
        return visible;

    /////////////////////////////////////////////////
    // Eliminate back facing polygons
    /////////////////////////////////////////////////

    // Keep in mind our normal is hidden and to get it we have to
    // pNormal->tx - pVPoint[ 0 ]->tx
    double direction = (pVPoint[ 0 ]->tx * ( pNormal->tx - pVPoint[ 0 ]->tx )) + 
                       (pVPoint[ 0 ]->ty * ( pNormal->ty - pVPoint[ 0 ]->ty )) + 
                       (pVPoint[ 0 ]->tz * ( pNormal->tz - pVPoint[ 0 ]->tz ));

    if( direction < 0.0 )
        return false;

    return true;

}   // IsVisable