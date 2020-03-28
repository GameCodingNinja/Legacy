
/************************************************************************
*
*    PRODUCT:         Animated mesh class
*
*    FILE NAME:       CAnimMesh.h
*
*    DESCRIPTION:     Mesh container for animation frames
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
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

	// Animated polygon class
	CAnimPolygon *pAnimPolyLst;

public:

    CAnimMesh();
    virtual ~CAnimMesh();

	// Render the 3D object on screen buffer
	void RenderFrame( CMatrix &m );

    /////////////////
    // Set functions
    /////////////////


    /////////////////
    // Get functions
    /////////////////

};

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

#endif // __CANIMATED_MESH_H__