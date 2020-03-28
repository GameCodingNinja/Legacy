/************************************************************************
*
*    PRODUCT:         3D polygon
*
*    FILE NAME:       CPolygon.h
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __CPOLYGON_H__
#define __CPOLYGON_H__

#include "CPoint.h"    // class holding all the point classes
#include "CTextLib.h"  // Texture library class
#include "CLight.h"    // Header file for this *.cpp file.

class CPolygon
{
protected:

    // OpenGl type GL_TRIANGLES or GL_QUADS
    int glType;
    // multiplier instead of dividing
    float multi;
    // radius of polygon
    float radius;
    // Texture Index
    int tIndex;
    // collision type - so that we know what we are hitting
    int collisionType;
    // Number of vertexes for this polygon. Could be a tri or a quad
    int vertCount;
    // Pointer to the surface normal
    CPoint *pNormal;
    // 4 pointers to a vertex list
    CPoint *pVPoint[QUAD];
    // UN coordinates, vertex normal and rgb color per vertex
    CUVRGB uvrgb[QUAD];

    // Load extra polygon data
    virtual bool LoadExtraPolygonData( FILE *hFile );
    // Save extra polygon data
    virtual bool SaveExtraPolygonData( FILE *hFile );

public:
    
    CPolygon();
    virtual ~CPolygon();

    // Send the info to OpenGL for rendering.
    void _fastcall RenderText( PCTextureLib pTextLib );
    void _fastcall Render();
    // Calculate light intensity
    void _fastcall CalcLightIntenStatic( CLightList *pLightList );
    void _fastcall CalcLightIntenDynamic( CLightList *pLightList );
    // Use plane equation to determine the orientation of the surface
    void CalcSurfaceNormal();
    // Calculate a bounding sphere around the polygon
    void CalcBoundingPoly();
    // Check for collision using bounding spheres
    int _fastcall Collide_BoundSphere( CPoint &pt, double &distance, float rad );
    // Get the a specific point of the polygon
    void CopyPoint( CPoint &pt, int index );
    // Get a copy of the normal
    void CopyNormal( CPoint &pt );
    // Is the supplied vertice the same
    bool HasVert( const CPoint &P );
    // Load the polygon data
    virtual bool LoadPolygonData( FILE *hFile, CPoint *vertLst );
    // Load all the normals from file
    bool LoadNormalsFromFile( FILE *hFile );
    // Save the polygon data
    virtual bool SavePolygonData( FILE *hFile, int vertexCount, CPoint *vertLst );
    // Save all the normals to a file
    bool SaveNormalsToFile( FILE *hFile );
    // Check if the polygon is visible. Perform 3D culling
    bool _fastcall IsVisable();
    bool _fastcall IsVisable( float portRadiusX, float portRadiusY, CPoint *pPortalCenter );
    // Convert the normal to it's pure self
    void ConvertToPureNormals();
    // Convert pure normals to vector
    void ConvertPureNormalsToVectors();
	// Copy the polygon
	void CopyPolygon( CPolygon &poly, CPoint *vertLst, int vertexCount );

    /////////////////
    // Set functions
    /////////////////

    // Set the normal
    void SetNormal( CPoint * pNor ){ pNormal = pNor; };
    // Set the vertex point
    void SetVPoint( int index, CPoint * pVert ){ pVPoint[index] = pVert; };
    // Set vertex normal
    void SetVertexNormal( int index, CPoint &vNorm ){ uvrgb[index].n = vNorm; };
    // Set an ambient light level for this polygon
    void SetAmbientLightLevel( float rColor, float gColor, float bColor );
	// Set the vertex count
	void SetVertexCount( int value ){ vertCount = value; };


    /////////////////
    // Get functions
    /////////////////

    // Get the collision type
    int GetCollisionType(){ return collisionType; };
    // Get the number of vertexes for this polygon
    int GetVertexCount(){ return vertCount; };
	// Get the texture index
    int GetTextIndex(){ return tIndex; };
    // Get the normal
    const CPoint * GetNormal(){ return pNormal; };
    // Get the vertex point
    const CPoint *GetVPoint( int index ){ return pVPoint[index]; };
    // Get the uvrgb
    CUVRGB * GetUVRGB(){ return uvrgb; };
	// Get the multiplier
	float GetMulti(){ return multi; };
	// Get the glType
	int GetGLType(){ return glType; };
	// Get the radius
	float GetRadius(){ return radius; };
};


/************************************************************************
*
*    Simple classes for loading/saving of data from binary files.
*
************************************************************************/

class CUVLoad
{
public:
    // U, V assumed
    float uv[2];

    CUVLoad(){ uv[0]=0.0; uv[1]=0.0; }
};


class CFaceLoad
{
public:
    // index into main vertice list
    int vIndex[QUAD];
    // U, V assumed
    CUVLoad crd[QUAD];
    // texture index
    int tIndex;
    // Vert count
    unsigned char vertCount;
    // Collsion type flag
    unsigned char collisionType;

    CFaceLoad()
    {
        vIndex[0] = vIndex[1] = vIndex[2] = vIndex[3] = 0;
        tIndex = 0;
        vertCount = 3;
        collisionType = 0;
    }
};

class CNormal
{
public:
    // x, y, z is assumed
    float norm[3];

    CNormal(){ norm[0]=0.0; norm[1]=0.0; norm[2]=0.0; }
};

class CNormalLoad
{
public:
    // x, y, z is assumed
    float norm[3];

    CNormal vertNor[QUAD];

    CNormalLoad(){ norm[0]=0.0; norm[1]=0.0; norm[2]=0.0; }
};


#endif // __CPOLYGON_H__