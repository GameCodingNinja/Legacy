
/************************************************************************
*
*    FILE NAME:       CAnimMesh.cpp
*
*    DESCRIPTION:     Mesh container for animation frames
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

#include "CAnimMesh.h"       // Header file for this *.cpp file.


/************************************************************************
*    DESCRIPTION:  Constructer                                                             
************************************************************************/
CAnimMesh::CAnimMesh()
{
}   // Constructer


/************************************************************************
*    DESCRIPTION:  Destructer                                                             
************************************************************************/
CAnimMesh::~CAnimMesh()
{

}   // Destructer


/************************************************************************
*    DESCRIPTION:  Allocate the polygon array. Virtual call so that
*                  an inherited class can load up it's own polygon
*                  class.
*
*    Output:   CPolygon - Pointer to polygon array
************************************************************************/
CPolygon * CAnimMesh::AllocatePolygonArray()
{
    pAnimPolyLst = new CAnimPolygon[ polyCount ];
    return pAnimPolyLst;

}   // AllocatePolygonArray


/************************************************************************
*    DESCRIPTION:  Load the polygon data
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - Handle to file
*
*    Output:   bool - true on success, false on fail
************************************************************************/
bool CAnimMesh::LoadPolygonData( FILE *hFile )
{
    // Cast to our animated poly class
    CAnimPolygon *pAnimPolyLst = (CAnimPolygon *)pList;

    // Load the polygon data
    for( int i = 0; i < polyCount; ++i )
    {
        if( !pAnimPolyLst[i].LoadPolygonData( hFile, vList ) )
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
bool CAnimMesh::SavePolygonData( FILE *hFile )
{
    // Cast to our animated poly class
    CAnimPolygon *pAnimPolyLst = (CAnimPolygon *)pList;
    
    // Save the polygon data
    for( int i = 0; i < polyCount; ++i )
    {
        if( !pAnimPolyLst[i].SavePolygonData( hFile, vCount, vList ) )
            return false;         
    }

    return true;

}   // SavePolygonData