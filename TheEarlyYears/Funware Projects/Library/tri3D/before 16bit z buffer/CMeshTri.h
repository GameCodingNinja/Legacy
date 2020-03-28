
/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CMeshObj.h
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
    EZ_ZBUFFER_TRI,
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

    // Dynamic light type
    EDyn_Light dynamicLight;
    // The vertex count
    int vCount;
    // The triangle count
    int triCount;
    // Bounding sphere radius
    float radius;
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
    // Cap for the intensity
    int intensityCap;
    // Intensity shift
    int intensityShift;
    // The render bit count
    int renderBitCount;
    // Type of rendering
    EZBuffer ZbufType;
    EShading shadeType;
    ERT_TypeOfRender renderType;
    // color used for 16 bit table
    unsigned short int rgbColor16;
	// Object center point for culling this object
    CPoint3D centerPt;
	// Flag that indicates this object was culled
    bool culled;
	// Distance of bounding sphere
    float distance;

    // Calculate a surface normal for each vertex.
    void CalVertexNormals();
    // Calculate a bounding sphere around this object.
    void CalBoundingSphere();

public:

    CMeshTri();
    ~CMeshTri();

    // Free all allocations
    void Free();    
    // Render to the buffer
    int Render( CMatrix &m, float cullPercent = 0.55 );
    // Load the polygon object file
    bool LoadFromFile( char *filePath, bool calNormals = true );
    // Load an object from data pointer
    bool LoadFromPointer( unsigned char *pData, bool calNormals = true );
	// Save a file with all the normals
    bool SaveNormals( char *filePath );
    // Load the normals from a file
    bool LoadNormals( char *filePath );
	// Was this object culled?
    bool WasCulled(){ return culled; };
	// Test position of object in frustrum to see if it can be culled
    void _fastcall CullMesh_BoundSphere( CMatrix &m, float cullPercent = 0.5 );

    /////////////////
    // Set functions
    /////////////////

    // Set the texture library
    void SetTextureLibrary( CTextureLib &textLib );
    // Set the pointer to the light list and if you want shadows recalculated on each frame
    void SetLightList( CLightList &lightList, EDyn_Light enableState );
    // Set an ambient light level for this object
    void SetAmbientLightLevel( unsigned int r, unsigned int g, unsigned int b );
    // Set the cap for the intensity
    void IntensityCap( int value ){ intensityCap = value; };
    // Set the render bit count
    void SetRenderBitCount( int value ){ renderBitCount = value; };
    // Set the render specs
    void SetRenderSpecs( EZBuffer bufType, EShading shType, ERT_TypeOfRender renType ){ ZbufType = bufType; shadeType = shType; renderType = renType; };
    // Set the color used for the 16 color render
    void SetRGBColor16( unsigned short int value ){ rgbColor16 = value; };

    /////////////////
    // Get functions
    /////////////////

    // Get the value of the radius
    float GetRadius(){ return radius; };
};

void Generate16BitShadedTable( unsigned short int *shadeTbl );

#endif // __CMESHTRI_H__