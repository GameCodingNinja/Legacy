/************************************************************************
*
*    FILE NAME:       CAnimPolygon.h
*
*    DESCRIPTION:     Polygon with animation properties
*
************************************************************************/

#ifndef __CANIM_POLYGON_H__
#define __CANIM_POLYGON_H__

#include "CPolygon.h"  // polygon class

class CAnimPolygon : public CPolygon
{
public:

    CAnimPolygon();
    virtual ~CAnimPolygon();

    // Load the polygon data
    virtual bool LoadPolygonData( FILE *hFile, CPoint *vertLst );
    // Save the polygon data
    virtual bool SavePolygonData( FILE *hFile, int vertexCount, CPoint *vertLst );
};


// Character models have to be slim by design
#pragma pack(1)
class CAnimFaceLoad
{
public:
    // index into main vertice list
    short int vIndex[TRI];
    // U, V assumed
    CUVLoad crd[TRI];
    // texture index
    unsigned char tIndex;

    CAnimFaceLoad()
    {
        vIndex[0] = vIndex[1] = vIndex[2] = 0;
        tIndex = 0;
    }
};
#pragma pack()


#endif // __CANIM_POLYGON_H__