
/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CMeshTri.h
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

#ifndef __CMESHTRI_H__
#define __CMESHTRI_H__

#include "CTriangle.h"       // Triangle class
#include "CMatrix.h"         // Matrix class

#define SHADE_COUNT         32
#define COLOR_COUNT_16_BIT  0x8000
#define DONT_CALC_NORMALS  false

// Render types
enum ERT_TypeOfRender
{
    ERT_WIRE_FRAME=0,
    ERT_SOLID_LIGHT_RENDER,
    ERT_SOLID_TEXT_RENDER,
    ERT_TEXT_RENDER,
};

// Type of zbuffering
enum EZBuffer
{
    EZ_NO_ZBUFFERING=0,
    EZ_ZBUFFER_POS,
	EZ_ZBUFFER_NEG,
};


// Type of shading
enum EShading
{ 
    ES_NO_SHADING=0,
    ES_GREY_SCALE_SHADING,
    ES_RGB_SHADING,
};

class CMeshTri
{
protected:

    // The vertex count
    int vCount;
    // The triangle count
    int triCount;
    // Bounding sphere radius
    float radius;
	float radiusX;
	float radiusY;
    // Allocated list of vertices
    CPoint3D *vList;
    // Allocated list of triangles
    CTriangle *triList;
    // Allocated list of normals
    CPoint *nList;
	// Projected bool list
	bool *fProjected;
    // Texture library used for this mesh
    PCTextureLib pTextLib;
    // Pointer to the light list
    CLightList *pLightList;
    // The render bit count
    int renderBitCount;
    // Type of rendering
    EZBuffer ZbufType;
    EShading shadeType;
    ERT_TypeOfRender renderType;
    // color used for 16 bit table
    unsigned short int rgbColor16;
	// Object center point for culling this object
    CPoint centerPt;
	// Flag that indicates this object was culled
    bool culled;
	// Distance of bounding sphere
    float distance;
	// Index of this object
    int index;
    // What this object is
    int type;

    // Calculate a surface normal for each vertex.
    void CalVertexNormals();
    // Calculate a bounding sphere around this object.
    void CalBoundingSphere();

public:

    CMeshTri();
    ~CMeshTri();

    // Free all allocations
    virtual void Free();    
    // Render to the buffer
    int Render( CMatrix &m, float cullPercent = 0.55, EZBuffer ZBufferOption = EZ_ZBUFFER_POS );
    // Load the polygon object file
    virtual bool LoadFromFile( char *filePath, bool calNormals = true );
    // Load an object from data pointer
    virtual bool LoadFromPointer( unsigned char *pData, bool calNormals = true );
	// Save a file with all the normals
    bool SaveNormals( char *filePath );
    // Load the normals from a file
    bool LoadNormals( char *filePath );
	// Was this object culled?
    bool WasCulled(){ return culled; };
	// Test position of object in frustrum to see if it can be culled
    void _fastcall CullMesh_BoundSphere( CMatrix &m, float cullPercent = 0.5 );
	// Check for collision using bounding spheres
    bool _fastcall Collide_BoundSphere( CMeshTri *mesh );
    // Check for collision with point
    bool _fastcall Collide_BoundSphere( CPoint &pt, float rad );
	// Check for collision with triangles using bound spheres
    int IsCollision_BoundSphere( int &index, double &distance, CPoint &pt, float rad );
	// Final Check for collision using intersection
    bool IsCollision_Intersection( CMatrix &m, int triIndex, float padding, CPoint &pt,
                                   CPoint &normal, float &pointD );

    /////////////////
    // Set functions
    /////////////////

    // Set the texture library
    void SetTextureLibrary( CTextureLib &textLib );
    // Set the pointer to the light list and if you want shadows recalculated on each frame
    void SetLightList( CLightList &lightList );
    // Set an ambient light level for this object
    void SetAmbientLightLevel( unsigned int r, unsigned int g, unsigned int b );
    // Set the render bit count
    void SetRenderBitCount( int value ){ renderBitCount = value; };
    // Set the render specs
    void SetRenderSpecs( EZBuffer bufType, EShading shType, ERT_TypeOfRender renType ){ ZbufType = bufType; shadeType = shType; renderType = renType; };
    // Set the color used for the 16 color render
    void SetRGBColor16( unsigned short int value ){ rgbColor16 = value; };
	// Set the type dependant on the texture index
    void SetTriType( int *typeAry, int count );
	// Set the index
    void SetIndex( int value ){ index = value; };
    // Set the type
    void SetType( int value ){ type = value; };

    /////////////////
    // Get functions
    /////////////////

    // Get the value of the radius
    float GetRadius(){ return radius; };
	float GetRadiusX(){ return radiusX; };
	float GetRadiusY(){ return radiusY; };
	// Get triangle count
    int GetTriCount(){ return triCount; };
	// Get the index
    int GetIndex(){ return index; };
    // Get the type
    int GetType(){ return type; };
	// Get the object distance
    double GetDistance(){ return distance; };
	// Get the center point
    CPoint * GetCenterPt(){ return &centerPt; };
};

void Generate16BitShadedTable( unsigned short int *shadeTbl );

#endif // __CMESHTRI_H__