
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

#ifndef __CMESHOBJ_H__
#define __CMESHOBJ_H__

#include "CTriangle.h"       // Triangle class
#include "CMatrix.h"         // Matrix class

#define SHADE_COUNT         32
#define COLOR_COUNT_16_BIT  0x8000

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

class CMeshObj
{
protected:

    // Dynamic light type
    EDyn_Light dynamicLight;
    // The vertex count
    int vCount;
    // The triangle count
    int triCount;
    // Bounding sphere radius
    float radius, radiusX, radiusY;
    // Object Culling flag
    bool fObjectCulling;
    // Allocated list of vertices
    CPoint3D *vList;
    // Allocated list of triangles
    CTriangle *triList;
    // Allocated list of normals
    CPoint3D *nList;
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

    // Calculate a surface normal for each vertex.
    void CalGouraudVertexNormals();
    // Calculate a bounding sphere around this object.
    void CalBoundingSphere();
    // Test position of object in frustrum to see if it can be culled
    bool CullObject( CMatrix &m );
    // Render to the buffer
    int Render32( CMatrix &m );
    int Render16( CMatrix &m );

public:

    CMeshObj();
    ~CMeshObj();

    // Free all allocations
    void Free();    
    // Render to the buffer
    int Render( CMatrix &m );
    // Load the polygon object file
    bool LoadFromFile( char *filePath );
    // Load an object from data pointer
    bool LoadFromPointer( unsigned char *pData );

    /////////////////
    // Set functions
    /////////////////

    // Turn on or off object culling
    void SetObjectCulling( bool value );
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
	float GetRadiusX(){ return radiusX; };
	float GetRadiusY(){ return radiusY; };
};

void Generate16BitShadedTable( unsigned short int *shadeTbl );

#endif // __CMESHOBJ_H__