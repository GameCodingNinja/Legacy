
/************************************************************************
*
*    PRODUCT:         OpenGL light classes
*
*    FILE NAME:       CLight.h
*
*    DESCRIPTION:     Class for holding light settings
*
*    IMPLEMENTATION:  In 3D, speed is everything. We need
*                     fast access to all data which is why everything
*                     is made public.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef _CLIGHT_H_
#define _CLIGHT_H_

#include <Math.H>
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include "CPoint.h"          // class holding all the point classes


// Type of dynamic lighting
enum EDyn_Light
{
    EDL_NO_DYNAMIC_LIGHTNING=0,     // Only do light calcualtions on startup
    EDL_LOCAL_DYNAMIC_LIGHTNING,    // dynamic light calculations on none-translated normals
    EDL_WORLD_DYNAMIC_LIGHTNING,    // dynamic light calculations on translated normals
};

// Type of light
enum ELT_lightType
{
    ELT_AMBIENT=0,      // all triangless are affected by color
    ELT_AMBIENT_POINT,  // confine the ambient light to an area
    ELT_POINT,          // infinite point lighting
    ELT_SPOT,           // Just as it says, a spot light
};

enum EShade_Type
{
    EShade_NONE=0,
    EShade_FLAT,
    EShade_GOURAUD,
};


/************************************************************************
*
*    Class for holding specific Light information about the triangle
*
************************************************************************/

class CLight
{
public:
    
    // Light point
    CPoint pos;
    // Direction of light - only for spot lights
    float dx, dy, dz;
    // Magnitude of light source
    double mag;
    // radius of spot light
    float spotRadius;
    // Color of the light
    unsigned int intensity;
    // Intensity range
    int intRange;
    // Intensity start point. Indicates the darkest point
    int intStart;
    // Type of light: Ambient, Infinite or point
    ELT_lightType lightType;
    // The type of shading to do on this mesh: Flat Gouraud
    EShade_Type shadeType; 

    CLight();
    ~CLight();

    // Set the point in space where light is comming from
    void SetLightPt( float aX, float aY, float aZ );
    // Calculate the magnitude from the convirted points
    void CalculateMag();
    // Set the intensity of the light
    void SetIntensity( unsigned int aInt );
    // Set the Intensity
    void SetIntenRangeStart( int range, int start ){ intRange = range >> 3; intStart = start >> 3; };
    // Calculate the angle of the surface to the light point - uses translated points for dynamic lighting
    double CalAngleToSurfaceDy( CPoint &n, CPoint3D *v );
    double CalAngleToSurfaceDy( CPoint * n, CPoint3D *v );
    // Calculate the angle of the surface to the light point - uses untranslated points
    double CalAngleToSurfaceSt( CPoint &n, CPoint3D *v );
    double CalAngleToSurfaceSt( CPoint * n, CPoint3D *v );
    // Calculate the distance to the surface to the light point
    float CalDistToSurfaceDy( CPoint3D *v );
    float CalDistToSurfaceSt( CPoint3D *v );
    // Calculate the direction to the surface.
    double CalDirToSurfaceDy( CPoint &v );
    double CalDirToSurfaceSt( CPoint &v );
    // Set the spot color radius
    void SetSpotRadius( float value ){ spotRadius = value; };
    // Set the direction of the spot light
    void SetSpotDir( float x, float y, float z ){ dx = x; dy = y; dz = z; };
};


/************************************************************************
*
*    Class for holding many lights
*
************************************************************************/

class CLightList
{
public:
    // number of lights
    unsigned int count;
    // Allocated light class array
    CLight *light;
    // Dynamic light type
    EDyn_Light dynamicLight;

    CLightList();
    ~CLightList();

    void Free();

    // Set the type of light
    void SetLightType( int lIndex, ELT_lightType type, EShade_Type shade );
    // Set the number of lights
    bool SetLightCount( int lCount );
    // Set the light point
    void SetLightPoint( int lIndex, float x, float y, float z );
    // Set the light intensity
    void SetLightIntensity( int lIndex, unsigned int aInt );
    // Set the light range
    void SetLightRange( int lIndex, int range, int start );
    // Set the spot light radius
    void SetSpotLightRadius( int lIndex, float radius );
    // Set the spot light dir
    void SetSpotLightDir( int lIndex, float x, float y, float z );
    // Get the dynamic light type
    EDyn_Light GetDynamicLightType(){ return dynamicLight; };
    // Set the dynamic light type
    void SetDynamicLightType( EDyn_Light type );
};

#endif