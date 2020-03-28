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
************************************************************************/
CAnimPolygon::CAnimPolygon()
{

}   // Constructer


/************************************************************************
*    DESCRIPTION:   Destructer                                                             
************************************************************************/
CAnimPolygon::~CAnimPolygon()
{

}   // Destructer


/************************************************************************
*    DESCRIPTION:  Load polygon data from file
*
*    Input:    FILE *hFile - File handle
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