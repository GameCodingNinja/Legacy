
/************************************************************************
*
*    FILE NAME:       CLight.cpp
*
*    DESCRIPTION:     Class for holding light settings
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "CLight.h"          // Header file for this *.cpp file.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include "deletefuncs.h"
#include <cassert>

/************************************************************************
*    DESCRIPTION:  Constructer                                                             
************************************************************************/
CLight::CLight()
{
    // default light position
    pos.tx = pos.x = 10000.0F;
    pos.ty = pos.y = 7000.0F;
    pos.tz = pos.z = 4.0F;

    // Calculate the magnitude
    CalculateMag();

    // Set the color of the light
    SetColor( 255, 255, 255 );

    // Default intensity settings
    intRange = 255;
    intStart = 0;

    // Type of light
    lightType = ELT_INFINITE;
    shadeType = EShade_GOURAUD;

    // Spotlight radius
    spotRadius = 300;

}   // Constructer


/************************************************************************
*    DESCRIPTION:  Destructer
************************************************************************/
CLight::~CLight()
{

}   // Destructer


/************************************************************************
*    DESCRIPTION:  Set the point in space where light is comming from
*
*    Input:    float aX - X point in space
*              float aY - Y point in space
*              float aZ - Z point in space
************************************************************************/
void CLight::SetLightPt( float aX, float aY, float aZ )
{
    pos.tx = pos.x = aX; pos.ty = pos.y = aY; pos.tz = pos.z = aZ;

    // Calculate the magnitude
    CalculateMag();

}   // SetLight


/************************************************************************
*    DESCRIPTION:  Transform the position of the light source
*
*    Input:    CMatrix & matrix - 
************************************************************************/
void CLight::Transform( CMatrix & matrix )
{
    matrix.Transform( pos );

    // Calculate the magnitude
    CalculateMag();

}   // Transform


/************************************************************************
*    DESCRIPTION:  Calculate the magnitude from the transformed point
************************************************************************/
void CLight::CalculateMag()
{
    // Calculate the magnitude
    mag = sqrt( pos.GetLengthDy() );

    // Don't let Mag = 0
    // We will get a divide by zero error
    if( mag == 0.0F )
        mag = 0.00001F;

}   // CalculateMag


/************************************************************************
*    DESCRIPTION:  Calculate the vertex intensity
************************************************************************/
int CLight::CalVertIntenSt( CPoint &n, CPoint &v )
{
	int vertInten = 0;

	///////////////////////////////
    // Infinite Light Calculations
    ///////////////////////////////
    if( lightType == ELT_INFINITE )
    {                                                        
	    // Assign an intensity based on the vertex surface normal
	    double cosAngle = pos.GetDotProductSt( n ) /	mag;

	    // Calculate the intensity of the vertex from the angle of the poly to the light source
	    vertInten = (int)(( cosAngle * ( double ) intRange ) + intStart);
    }
	////////////////////////////////////
    // point infinite Light Calculations
    ////////////////////////////////////
    if( lightType == ELT_POINT_INFINITE )
    {                                                                            
        // Assign an intensity based on the vertex surface normal
        double cosAngle = pos.GetLengthDotProductSt( n, v ) / GetMagSt( v );

        // Calculate the intensity of the vertex from the angle of the poly to the light source
        vertInten = (int)(( cosAngle * ( double ) intRange ) + intStart);
    }
    ////////////////////////////////////
    // point radius Light Calculations
    ////////////////////////////////////
    else if( lightType == ELT_POINT_RADIUS )
    {                   
    	double dist = pos.GetLengthSt( v );

        if( dist < spotRadius )
		{
	        // Assign an intensity based on the vertex surface normal
        	double cosAngle = pos.GetLengthDotProductSt( n, v ) / GetMagSt( v );
	        		
	        // Cause the light to dim the further it is from the poin
			vertInten = (int)(cosAngle * (spotRadius / dist));
		}
    }

	return vertInten;

}   // CalVertIntenSt


/************************************************************************
*    DESCRIPTION:  Calculate the vertex intensity
************************************************************************/
int _fastcall CLight::CalVertIntenDy( CPoint &n, CPoint &v )
{
	int vertInten = 0;

	///////////////////////////////
    // Infinite Light Calculations
    ///////////////////////////////
    if( lightType == ELT_INFINITE )
    {                                                        
	    // Assign an intensity based on the vertex surface normal
	    double cosAngle = pos.GetDotProductDy( n ) / mag;

	    // Calculate the intensity of the vertex from the angle of the poly to the light source
	    vertInten = (int)(( cosAngle * ( double ) intRange ) + intStart);
    }
	////////////////////////////////////
    // point infinite Light Calculations
    ////////////////////////////////////
    if( lightType == ELT_POINT_INFINITE )
    {                                                                            
        // Assign an intensity based on the vertex surface normal
        double cosAngle = pos.GetLengthDotProductDy( n, v ) / GetMagDy( v );

        // Calculate the intensity of the vertex from the angle of the poly to the light source
        vertInten = (int)(( cosAngle * ( double ) intRange ) + intStart);
    }
    ////////////////////////////////////
    // point radius Light Calculations
    ////////////////////////////////////
    else if( lightType == ELT_POINT_RADIUS )
    {                   
    	double dist = pos.GetLengthDy( v );

        if( dist < spotRadius )
		{
	        // Assign an intensity based on the vertex surface normal
        	double cosAngle = pos.GetLengthDotProductDy( n, v ) / GetMagDy( v );
	        		
	        // Cause the light to dim the further it is from the point
			vertInten = (int)(cosAngle * (spotRadius / dist));
		}
    }

	return vertInten;

}   // CalVertIntenDy


/************************************************************************
*    DESCRIPTION:  Calculate the magnitude
************************************************************************/
double CLight::GetMagSt( CPoint &v )
{    
    // Calculate the magnitude
    double length = sqrt( pos.GetLengthSt( v ) );

    // Don't let Mag = 0
    // We will get a divide by zero error
    if( length == 0.0F )
        length = 0.00001F;

	return length;

}   // GetMagSt


/************************************************************************
*    DESCRIPTION:  Calculate the magnitude
************************************************************************/
double _fastcall CLight::GetMagDy( CPoint &v )
{
    // Calculate the magnitude
    double length = sqrt( pos.GetLengthDy( v ) );

    // Don't let Mag = 0
    // We will get a divide by zero error
    if( length == 0.0F )
        length = 0.00001F;

	return length;

}   // GetMagDy


/************************************************************************
*    DESCRIPTION:  Set the color of the light via a packed DWORD
*
*    Input:    unsigned int aColor - RGB packed DWORD
************************************************************************/
void CLight::SetColor( unsigned int aColor )
{
    rgbcolor = aColor;
    rColor = ((aColor >> 16) & 0xFF);
    gColor = ((aColor >> 8) & 0xFF);
    bColor = (aColor & 0xFF);

}   // SetColor


/************************************************************************
*    DESCRIPTION:  Set the color of the light via a Windows
*                  packed color reference.
*
*    Input:    COLORREF aColor - RGB packed DWORD
************************************************************************/
void CLight::SetColor( COLORREF aColor )
{   
    rColor = (aColor & 0xFF);
    gColor = ((aColor >> 8) & 0xFF);
    bColor = ((aColor >> 16) & 0xFF);
    
    rgbcolor =  rColor << 16;
    rgbcolor |= gColor << 8;
    rgbcolor |= bColor;

}   // SetColor


/************************************************************************
*    DESCRIPTION:  Set the color of the light
*
*    Input:    unsigned int red - red channel color
*              unsigned int green - green channel color
*              unsigned int green - green channel color
************************************************************************/
void CLight::SetColor( unsigned int red, unsigned int green, unsigned int blue )
{
    // Make sure the value doesent exceed 255
    rColor = red & 0xFF;
    gColor = green & 0xFF;
    bColor = blue & 0xFF;

    rgbcolor =  red << 16;
    rgbcolor |= green << 8;
    rgbcolor |= blue;

}   // SetColor


/************************************************************************
*    DESCRIPTION:  Calculate the angle of the surface to the light
*                  point - uses untranslated points and normal
*
*    Input:    CPoint &n - vertex normal
*              CPoint *v - vertex point
*
*    Output:   double - Angle to surface
************************************************************************/
void CLight::SetIntenRangeStart( int range, int start )
{
    intRange = range;
    intStart = start;

}   // CalAngleToSurfaceDy


/************************************************************************
*    DESCRIPTION:  Set the type of light.
*
*    Input:    int lIndex - light index to set
*              ELT_lightType type - Type of light
*              EShade_Type shade - shading type
************************************************************************/
void CLight::SetLightType( ELT_lightType type, EShade_Type shade )
{
	lightType = type;
    shadeType = shade;

}   // SetLightType


/************************************************************************
*
*    Class for holding many lights
*
************************************************************************/


/************************************************************************
*    DESCRIPTION:  Constructer                                                             
************************************************************************/
CLightList::CLightList()
{
    // Default lightning
    dynamicLight = EDL_NO_DYNAMIC_LIGHTNING;
	allocated = false;

}   // Constructer


/************************************************************************
*    DESCRIPTION:  Destructer                                                             
************************************************************************/
CLightList::~CLightList()
{
    // Free any allocated memory
    Free();

}   // Destructer


/************************************************************************
*    DESCRIPTION:  Free allocated memory
************************************************************************/
void CLightList::Free()
{
    if( allocated )
	{
    	DeleteVectorPointers( light );
	}
	else
	{
		light.clear();
	}

	allocated = false;

}   // Free


/************************************************************************
*    DESCRIPTION:  Allocate the number of needed lights
*
*    Input:    int count - number of lights
************************************************************************/
void CLightList::SetLightCount( int count )
{
    // Free any allocated memory
    Free();

    // Make sure the number of lights are not negitive
    if( count > 0 )
    {
        allocated = true;

        // Allocate the needed classes
		for( int i = 0; i < count; i++ )
		{
        	light.push_back( new CLight );
		}
    }

}   // SetLightCount


/************************************************************************
*    DESCRIPTION:  Add light to class
*
*    Input:    CLight *pLight - light pointer
************************************************************************/
void CLightList::InsertLight( CLight *pLight )
{
	assert( pLight != NULL );

    // Make sure the number of lights are not negitive
    if( pLight != NULL )
    {
    	light.push_back( pLight );
    }

}   // InsertLight


/************************************************************************
*    DESCRIPTION:  Set the spot light direction
*
*    Input:    EDyn_Light type - dynamic light type
*              var2type Var2Name - Description
************************************************************************/
void CLightList::SetDynamicLightType( EDyn_Light type )
{
    dynamicLight = type;

}   // SetDynamicLightType


/************************************************************************
*    DESCRIPTION:  Transform the position of the light source
*
*    Input:    CMatrix & matrix - 
************************************************************************/
void CLightList::Transform( CMatrix & matrix )
{
    for( unsigned int i = 0; i < light.size(); i++ )
	{
	    matrix.Transform( light[i]->GetPos() );

	    // Calculate the magnitude
	    light[i]->CalculateMag();
	}

}   // Transform