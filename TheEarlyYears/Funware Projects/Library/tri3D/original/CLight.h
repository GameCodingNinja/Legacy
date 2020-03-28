
/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CLight.h
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

#ifndef _CLIGHT_H_
#define _CLIGHT_H_

#include <Math.H>
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include "CPoint.h"          // class holding all the point classes


// Type of dynamic lighting
enum EDyn_Light
{
    EDL_NO_DYNAMIC_LIGHTNING=0,
    EDL_LOCAL_DYNAMIC_LIGHTNING,
    EDL_WORLD_DYNAMIC_LIGHTNING,
};

// Type of light
enum ELT_lightType
{
    ELT_AMBIENT=0,
    ELT_INFINITE,
    ELT_POINT,
    ELT_SPOT,
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
    float x, y, z;
    // Translated versions of the original values.
    float tx, ty, tz;
    double mag;
    // Color of the light
    unsigned int rgbcolor;
    unsigned int rColor;
    unsigned int gColor;
    unsigned int bColor;
    // Intensity range
    int intRange;
    // Intensity start point. Indicates the darkest point
    int intStart;
    // Type of light: Ambient, Infinite or point
    ELT_lightType lightType;
    // The type of shading to do on this mesh: Flat Gouraud
    EShade_Type shadeType;
    // Bit count of light
    int bitCount; 

    CLight();
    ~CLight();

    // Set the point in space where light is comming from
    void SetLightPt( float aX, float aY, float aZ );
    // Calculate the magnitude from the convirted points
    void CalculateMag();
    // Set the color of the light via a packed DWORD
    void SetColor( unsigned int aColor );
    void SetColor( COLORREF aColor );
    // Set the color of the light via seperate RGB DWORS
    void SetColor( unsigned int red, unsigned int green, unsigned int blue );
    // Set the Intensity
    void SetIntenRangeStart( int range, int start );
    // Calculate point light for mag
    void CalculatePointMag( CPoint3D *v );
    // Calculate the angle of the surface to the light point
    double CalAngleToSurface( CPoint &n ){ return ( (n.tx * tx) + (n.ty * ty) + (n.tz * tz) ) / mag; };
    double CalAngleToSurface( CPoint &n, CPoint3D *v ){ return ( ((n.tx-v->tx) * tx) + ((n.ty-v->ty) * ty) + ((n.tz-v->tz) * tz) ) / mag; };
    double CalAngleToSurface( CPoint3D * n, CPoint3D *v ){ return ( ((n->tx-v->tx) * tx) + ((n->ty-v->ty) * ty) + ((n->tz-v->tz) * tz) ) / mag; };
};


/************************************************************************
*
*    Class for holding many lights
*
************************************************************************/

class CLightList
{
public:
    int count;
    CLight *light;
    int bitCount;

    CLightList();
    ~CLightList();

    void Free();

    // Set the type of light
    void SetLightType( int lIndex, ELT_lightType type, EShade_Type shade );
    // Set the number of lights
    bool SetLightCount( int lCount, int bitCt = 16 );
    // Set the light point
    void SetLightPoint( int lIndex, float x, float y, float z );
    // Set the light color
    void SetLightColor( int lIndex, unsigned int r, unsigned int g, unsigned int b );
    void SetLightColor( int lIndex, unsigned int aColor );
    void SetLightColor( int lIndex, COLORREF aColor );
    // Set the light range
    void SetLightRange( int lIndex, int range, int start );
};

#endif