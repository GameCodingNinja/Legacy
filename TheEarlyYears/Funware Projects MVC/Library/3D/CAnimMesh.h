
/************************************************************************
*
*    FILE NAME:       CAnimMesh.h
*
*    DESCRIPTION:     Mesh container for animation frames
*
************************************************************************/

#ifndef __CANIMATED_MESH_H__
#define __CANIMATED_MESH_H__

#include "CMesh.h"         // Base mesh class
#include "CAnimPolygon.h"    // Animated polygon class

#define MESH_ANIM_TBL_FORMAT_STR_ID   "mesh anim table"

class CAnimMesh : public CMesh
{
protected:

    // Allocate the polygon array
    virtual CPolygon * AllocatePolygonArray();
    // Load the polygon data
    virtual bool LoadPolygonData( FILE *hFile );
    // Save the polygon data
    virtual bool SavePolygonData( FILE *hFile );

	// Disable the Translate the light list
	virtual void TransformLightList( CMatrix &m ){};

    // Animated polygon class
    CAnimPolygon *pAnimPolyLst;

public:

    CAnimMesh();
    virtual ~CAnimMesh();


    /////////////////
    // Set functions
    /////////////////


    /////////////////
    // Get functions
    /////////////////

};

#pragma pack(1)
class CAnimTable
{
public:

    char animNameStr[50];
    int frameStart;
    int frameCount;

    CAnimTable()
    {
        animNameStr[0] = 0;
        frameStart = 0;
        frameCount = 0;
    }
};
#pragma pack()

#endif // __CANIMATED_MESH_H__