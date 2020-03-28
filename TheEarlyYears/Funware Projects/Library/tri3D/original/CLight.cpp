
/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CLight.cpp
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

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "CLight.h"          // Header file for this *.cpp file.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf

/************************************************************************
*
*    Class for holding specific Light information about the triangle
*
************************************************************************/

/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CLight::CLight()
{
    // default light
    tx = x = 1000.0F;
    ty = y = 100.0F;
    tz = z = 4.0F;

    // Calculate the magnitude
	CalculateMag();

    // Set the color of the light
    SetColor( 255, 255, 255 );

    // Default intensity settings
    intRange = 80;
    intStart = 170;

    // Type of light
    lightType = ELT_INFINITE;
    shadeType = EShade_GOURAUD;

}   // Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CLight::~CLight()
{


}   // Destructer


/************************************************************************
*    FUNCTION NAME:         SetLight()                                                             
*
*    DESCRIPTION:           Set the point in space where light is comming from
*
*    FUNCTION PARAMETERS:
*    Input:    float aX - X point in space
*              float aY - Y point in space
*              float aZ - Z point in space
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLight::SetLightPt( float aX, float aY, float aZ )
{
    tx = x = aX; ty = y = aY; tz = z = aZ;

    // Calculate the magnitude
    CalculateMag();

}   // SetLight


/************************************************************************
*    FUNCTION NAME:         CalculateMag()                                                             
*
*    DESCRIPTION:           Calculate the magnitude from the convirted points
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLight::CalculateMag()
{
    // Calculate the magnitude
    mag = sqrt( (tx * tx) + (ty * ty) + (tz * tz) );

    // Don't let Mag = 0
    // We will get a divide by zero error
    if( mag == 0.0F )
        mag = 0.00001F;

}   // CalculateMag


/************************************************************************
*    FUNCTION NAME:         SetIntenRangeStart()                                                             
*
*    DESCRIPTION:           Set the color of the light via a packed DWORD
*
*    FUNCTION PARAMETERS:
*    Input:    unsigned int aColor - RGB packed DWORD
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLight::SetIntenRangeStart( int range, int start )
{
	if( bitCount == 16 )
	{
		// Convert to 16 bit rgb values
		intRange = (range & 0xFF) >> 3;
		intStart = (start & 0xFF) >> 3;
	}
	else
	{
		intRange = range & 0xFF;
		intStart = start & 0xFF;
	}

}	// SetIntenRangeStart


/************************************************************************
*    FUNCTION NAME:         SetColor()                                                             
*
*    DESCRIPTION:           Set the color of the light via a packed DWORD
*
*    FUNCTION PARAMETERS:
*    Input:    unsigned int aColor - RGB packed DWORD
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLight::SetColor( unsigned int aColor )
{
    rgbcolor = aColor;
    rColor = ((aColor >> 16) & 0xFF);
    gColor = ((aColor >> 8) & 0xFF);
    bColor = (aColor & 0xFF);

    if( bitCount == 16 )
	{
		// Convert to 16 bit rgb values
		rColor = rColor >> 3;
		gColor = gColor >> 3;
		bColor = bColor >> 3; 

	    rgbcolor =  rColor << 10;
	    rgbcolor |= gColor << 5;
	    rgbcolor |= bColor;
	}

}   // SetColor


/************************************************************************
*    FUNCTION NAME:         SetColor()                                                             
*
*    DESCRIPTION:           Set the color of the light via a Windows
*                           packed color reference.
*
*    FUNCTION PARAMETERS:
*    Input:    unsigned int aColor - RGB packed DWORD
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLight::SetColor( COLORREF aColor )
{   
    rColor = (aColor & 0xFF);
    gColor = ((aColor >> 8) & 0xFF);
    bColor = ((aColor >> 16) & 0xFF);

    if( bitCount == 16 )
	{
		// Convert to 16 bit rgb values
		rColor = rColor >> 3;
		gColor = gColor >> 3;
		bColor = bColor >> 3; 

	    rgbcolor =  rColor << 10;
	    rgbcolor |= gColor << 5;
	    rgbcolor |= bColor;
	}
	else if( bitCount == 32 )
	{
	    rgbcolor =  rColor << 16;
	    rgbcolor |= gColor << 8;
	    rgbcolor |= bColor;
	}

}   // SetColor


/************************************************************************
*    FUNCTION NAME:         SetColor()                                                             
*
*    DESCRIPTION:           Set the color of the light via seperate RGB DWORS
*
*    FUNCTION PARAMETERS:
*    Input:    unsigned int red - red channel color
*              unsigned int green - green channel color
*              unsigned int green - green channel color
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLight::SetColor( unsigned int red, unsigned int green, unsigned int blue )
{
	// Make sure the value doesent exceed 255
	rColor = red & 0xFF;
	gColor = green & 0xFF;
	bColor = blue & 0xFF;

    if( bitCount == 16 )
	{
		// Convert to 16 bit rgb values
		rColor = rColor >> 3;
		gColor = gColor >> 3;
		bColor = bColor >> 3; 

	    rgbcolor =  rColor << 10;
	    rgbcolor |= gColor << 5;
	    rgbcolor |= bColor;
	}
	else if( bitCount == 32 )
	{
	    rgbcolor =  rColor << 16;
	    rgbcolor |= gColor << 8;
	    rgbcolor |= bColor;
	}

}   // SetColor


/************************************************************************
*    FUNCTION NAME:         CalculatePointMag()                                                             
*
*    DESCRIPTION:           Calculate the magnitude from the convirted points.
*                           For this, the vertex distance is take into effect.
*
*                           The below function is a fast way to replace sqrt
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLight::CalculatePointMag( CPoint3D *v )
{
    #define SWAP(a,b,t) {t=a; a=b; b=t;}
    int temp;  // used for swaping
    int x,y,z; // used for algorithm

    // the below code replaces: sqrtf( v->tx*tx + v->ty*ty + v->tz*tz )
    // The code has an 8% error factor but it's still close enough because
    // if this function is used, it could be called many time per render cycle.

    //fast_sqrt( float x, float y, float z );

    // make sure values are all positive
    x = (int)fabs(v->tx - tx) * 1024;
    y = (int)fabs(v->ty - ty) * 1024;
    z = (int)fabs(v->tz - tz) * 1024;

    // sort values
    if( y < x )
        SWAP( x, y, temp )

    if( z < y )
        SWAP( y, z, temp )

    if( y < x )
        SWAP( x, y, temp )

    int dist = (z + 11 * (y >> 5) + (x >> 2) );

    // compute distance with 8% error
    mag = ((double)(dist >> 10));

    // Can't be zero
    if( mag == 0.0F )
        mag = 0.00001F;

}   // CalculatePointMag


/************************************************************************
*
*    Class for holding many lights
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CLightList::CLightList()
{
    count = 0;
    light = NULL;
	bitCount = 16;

}   // Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CLightList::~CLightList()
{
    // Free any allocated memory
    Free();

}   // Destructer


/************************************************************************
*    FUNCTION NAME:         Free()                                                             
*
*    DESCRIPTION:           Free allocated memory
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLightList::Free()
{
    if( light != NULL )
    {
        delete [] light;
        light = NULL;
        count = 0;
    }

}   // Free


/************************************************************************
*    FUNCTION NAME:         CalculateMag()                                                             
*
*    DESCRIPTION:           Calculate the magnitude from the convirted points
*
*    FUNCTION PARAMETERS:
*    Input:    int lCount - Number of lights
*              int bitSize - Bit count of light
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CLightList::SetLightCount( int lCount, int bitCt )
{
    bool result = false;

    // Free any allocated memory
    Free();

    // Make sure the number of lights are not negitive
    if( lCount > 0 )
    {
        // Light bit count
        bitCount = bitCt;
        
        // Allocate the needed classes
        light = new CLight[ lCount ];

        if( light != NULL )
        {
            count = lCount;

			// Set the bit count of the lights
			for( int i = 0; i < count; ++i )
				light[i].bitCount = bitCt;

            result = true;
        }
    }

    return result;

}   // SetLightCount


/************************************************************************
*    FUNCTION NAME:         SetLightType()                                                             
*
*    DESCRIPTION:           Set the type of light
*
*    FUNCTION PARAMETERS:
*    Input:    int lIndex - Index to light
*              int type - Type of light
*              int shade -  shade type
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLightList::SetLightType( int lIndex, ELT_lightType type, EShade_Type shade )
{
	if( lIndex > -1 && lIndex < count )
	{
		light[lIndex].lightType = type; 
		light[lIndex].shadeType = shade;
	}

}	// SetLightType


/************************************************************************
*    FUNCTION NAME:         SetLightPoint()                                                             
*
*    DESCRIPTION:           Set the point is space the light will generate from
*
*    FUNCTION PARAMETERS:
*    Input:    int lIndex - Index to light
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLightList::SetLightPoint( int lIndex, float x, float y, float z )
{
	if( lIndex > -1 && lIndex < count )
	{
		light[lIndex].SetLightPt( x, y, z );
	}

}	// SetLightPoint


/************************************************************************
*    FUNCTION NAME:         SetLightColor()                                                             
*
*    DESCRIPTION:           Set the color of the light
*
*    FUNCTION PARAMETERS:
*    Input:    int lIndex - Index to light
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLightList::SetLightColor( int lIndex, unsigned int r, unsigned int g, unsigned int b )
{
	if( lIndex > -1 && lIndex < count )
	{
		light[lIndex].SetColor( r, g, b );
	}

}	// SetLightColor


/************************************************************************
*    FUNCTION NAME:         SetLightColor()                                                             
*
*    DESCRIPTION:           Set the color of the light
*
*    FUNCTION PARAMETERS:
*    Input:    int lIndex - Index to light
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLightList::SetLightColor( int lIndex, unsigned int aColor )
{
	if( lIndex > -1 && lIndex < count )
	{
		light[lIndex].SetColor( aColor );
	}

}	// SetLightColor


/************************************************************************
*    FUNCTION NAME:         SetLightColor()                                                             
*
*    DESCRIPTION:           Set the color of the light
*
*    FUNCTION PARAMETERS:
*    Input:    int lIndex - Index to light
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLightList::SetLightColor( int lIndex, COLORREF aColor )
{
	if( lIndex > -1 && lIndex < count )
	{
		light[lIndex].SetColor( aColor );
	}

}	// SetLightColor


/************************************************************************
*    FUNCTION NAME:         SetLightRange()                                                             
*
*    DESCRIPTION:           Set the range of the light
*
*    FUNCTION PARAMETERS:
*    Input:    int lIndex - Index to light
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  09/27/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLightList::SetLightRange( int lIndex, int range, int start )
{
	if( lIndex > -1 && lIndex < count )
	{
		light[lIndex].SetIntenRangeStart( range, start );
	}

}	// SetLightRange