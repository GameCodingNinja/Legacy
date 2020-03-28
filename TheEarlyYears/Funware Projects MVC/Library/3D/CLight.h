
/************************************************************************
*
*    FILE NAME:       CLight.h
*
*    DESCRIPTION:     Class for holding light settings
*
************************************************************************/

#ifndef _CLIGHT_H_
#define _CLIGHT_H_

#include <Math.H>
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include "CPoint.h"          // class holding all the point classes
#include "CMatrix.h"         // Matrix class
#include <string>
#include <vector>

#pragma warning(disable : 4996)

// Type of dynamic lighting
enum EDyn_Light
{
    EDL_NO_DYNAMIC_LIGHTNING=0,     // Only do light calcualtions on startup
    EDL_LOCAL_DYNAMIC_LIGHTNING,    // dynamic light calculations on none-translated normals
    EDL_WORLD_DYNAMIC_LIGHTNING,    // dynamic light calculations on translated normals
	EDL_MAX_TYPES
};

struct SDynamicType
{
    std::string typeStr[EDL_MAX_TYPES];

    SDynamicType()
    {
        typeStr[EDL_NO_DYNAMIC_LIGHTNING] = "no_dynamic_lights";
        typeStr[EDL_LOCAL_DYNAMIC_LIGHTNING] = "local_dynamic_lights";
        typeStr[EDL_WORLD_DYNAMIC_LIGHTNING] = "world_dynamic_lights";
    }

	EDyn_Light GetType( std::string &type )
	{
		EDyn_Light result = EDL_NO_DYNAMIC_LIGHTNING;

	    // find the type
	    for( int i = 0; i < EDL_MAX_TYPES; ++i )
	    {
	        if( typeStr[i] == type )
	        {
	            result = (EDyn_Light)i;
	            break;
	        }
	    }

		return result;
	}
};

// Type of light
enum ELT_lightType
{
    ELT_AMBIENT=0,      // all triangless are affected by color
    ELT_AMBIENT_POINT,  // confine the ambient light to an area
	ELT_INFINITE,       // infinite lighting
    ELT_POINT_INFINITE, // infinite point lighting
    ELT_POINT_RADIUS,   // Light bulb in a room
	ELT_MAX_TYPES
};


struct SLightDirType
{
    std::string typeStr[ELT_MAX_TYPES];

    SLightDirType()
    {
		typeStr[ELT_AMBIENT] = "ambient";
        typeStr[ELT_AMBIENT_POINT] = "amnient_point";
		typeStr[ELT_INFINITE] = "infinite";
        typeStr[ELT_POINT_INFINITE] = "point_infinite";		
		typeStr[ELT_POINT_RADIUS] = "point_radius";
    }

	ELT_lightType GetType( std::string &type )
	{
		ELT_lightType result = ELT_AMBIENT;

	    // find the type
	    for( int i = 0; i < ELT_MAX_TYPES; ++i )
	    {
	        if( typeStr[i] == type )
	        {
	            result = (ELT_lightType)i;
	            break;
	        }
	    }

		return result;
	}
};

enum EShade_Type
{
    EShade_NONE=0,
    EShade_FLAT,
    EShade_GOURAUD,
	EShade_MAX
};

struct SShadeType
{
    std::string typeStr[EShade_MAX];

    SShadeType()
    {
        typeStr[EShade_NONE] = "none";
        typeStr[EShade_FLAT] = "flat";
        typeStr[EShade_GOURAUD] = "gouraud";
    }

	EShade_Type GetType( std::string &type )
	{
		EShade_Type result = EShade_GOURAUD;

	    // find the type
	    for( int i = 0; i < EShade_MAX; ++i )
	    {
	        if( typeStr[i] == type )
	        {
	            result = (EShade_Type)i;
	            break;
	        }
	    }

		return result;
	}
};


/************************************************************************
*
*    Class for holding specific Light information
*
************************************************************************/

class CLight
{
protected:

    // Light point
    CPoint pos;
    // Magnitude of light source
    double mag;
    // radius of spot light
    float spotRadius;
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
    
public: 

    CLight();
    ~CLight();

    // Set the point in space where light is comming from
    void SetLightPt( float aX, float aY, float aZ );
    // Calculate the magnitude from the convirted points
    void CalculateMag();
	double GetMagSt( CPoint &v );
	double _fastcall GetMagDy( CPoint &v );
    // Set the color of the light via a packed DWORD
    void SetColor( unsigned int aColor );
    void SetColor( COLORREF aColor );
    // Set the color of the light via seperate RGB DWORS
    void SetColor( unsigned int red, unsigned int green, unsigned int blue );
    // Set the Intensity
    void SetIntenRangeStart( int range, int start );
    // Set the spot color radius
    void SetSpotRadius( float value ){ spotRadius = value; };
	// Transform the position of the light source
	void Transform( CMatrix & matrix );
	// Set the type of light.
	void SetLightType( ELT_lightType type, EShade_Type shade );
	// Get the position
	CPoint & GetPos(){ return pos; };
	// Calculate the vertex intensity
	int CalVertIntenSt( CPoint &n, CPoint &v );
	int _fastcall CalVertIntenDy( CPoint &n, CPoint &v );
	// Get the radius
	float GetRadius(){ return spotRadius; };
	// Get the light type
	ELT_lightType GetLightType(){ return lightType; };
	// Get the shade type
	EShade_Type GetShadeType(){ return shadeType; };
	// The colors
	unsigned int GetColorR(){ return rColor; };
	unsigned int GetColorG(){ return gColor; };
	unsigned int GetColorB(){ return bColor; };
};


/************************************************************************
*
*    Class for holding many lights
*
************************************************************************/

class CLightList
{
protected:

    // Allocated light class array
    std::vector<CLight *> light;
    // Dynamic light type
    EDyn_Light dynamicLight;
	// Flag to indicate class owns allocation
	bool allocated;

public:

    CLightList();
    ~CLightList();

    void Free();

    // Get the dynamic light type
    EDyn_Light GetDynamicLightType(){ return dynamicLight; };
    // Set the dynamic light type
    void SetDynamicLightType( EDyn_Light type );
	// Transform the position of the light source
	void Transform( CMatrix & matrix );
	// Allocate the number of needed lights
	void SetLightCount( int count );
	// Get light count
	int Count(){ return light.size(); };
	// get the light list
	std::vector<CLight *> & GetLightLst(){ return light; };
	// Get the light
	CLight * GetLight( int index ){ return light[index]; };
	// Add light to class
	void InsertLight( CLight *pLight );
};

#endif