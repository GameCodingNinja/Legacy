
/************************************************************************
*
*    PRODUCT:         OpenGL light classes
*
*    FILE NAME:       CLight.cpp
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
*    H Rosenorn    02/20/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CLight::CLight()
{
    // default light position
    pos.tx = pos.x = 10000.0F;
    pos.ty = pos.y = 7000.0F;
    pos.tz = pos.z = 4.0F;

    // Calculate the magnitude
    CalculateMag();

    // Set the intensity
    intensity = 255 >> 3;

    // Default intensity settings
    intRange = 80 >> 3;
    intStart = 170 >> 3;

    // Type of light
    lightType = ELT_POINT;
    shadeType = EShade_GOURAUD;

    // Spotlight radius
    spotRadius = 500;

    // Direction of spot light
    // the values are only -1, 0, 1
    dx = 0;
    dy = -1;
    dz = 0;

}   // Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CLight::~CLight()
{

}   // Destructer


/************************************************************************
*    FUNCTION NAME:         SetLightPt()                                                             
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
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLight::SetLightPt( float aX, float aY, float aZ )
{
    pos.tx = pos.x = aX; pos.ty = pos.y = aY; pos.tz = pos.z = aZ;

    // Calculate the magnitude
    CalculateMag();

}   // SetLight


/************************************************************************
*    FUNCTION NAME:         CalculateMag()                                                             
*
*    DESCRIPTION:           Calculate the magnitude from the transformed point
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
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLight::CalculateMag()
{
    // Calculate the magnitude
    mag = sqrt( (pos.tx * pos.tx) + (pos.ty * pos.ty) + (pos.tz * pos.tz) );

    // Don't let Mag = 0
    // We will get a divide by zero error
    if( mag == 0.0F )
        mag = 0.00001F;

}   // CalculateMag


/************************************************************************
*    FUNCTION NAME:         SetIntensity()                                                             
*
*    DESCRIPTION:           Set the intensity of the light
*
*    FUNCTION PARAMETERS:
*    Input:    unsigned int aInt - Intensity
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLight::SetIntensity( unsigned int aInt )
{
    intensity = aInt >> 3;

}   // SetIntensity


/************************************************************************
*    FUNCTION NAME:         CalDistToSurfaceDy()                                                             
*
*    DESCRIPTION:           Calculate the distance to the surface to the 
*                           light point. This function uses translated
*                           light points for dynamic runtime light
*                           calculations.
*
*    FUNCTION PARAMETERS:
*    Input:    CPoint *v - vertex point
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

float CLight::CalDistToSurfaceDy( CPoint3D *v )
{
    float x(v->tx - pos.tx);
    float y(v->ty - pos.ty);
    float z(v->tz - pos.tz);

    return (x * x) + (y * y) + (z * z);

}   // CalDistToSurfaceDy


/************************************************************************
*    FUNCTION NAME:         CalDistToSurfaceSt()                                                             
*
*    DESCRIPTION:           Calculate the distance to the surface to the
*                           light point. This function uses untranslated
*                           light points for startup light calculations.
*
*    FUNCTION PARAMETERS:
*    Input:    CPoint *v - vertex point
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

float CLight::CalDistToSurfaceSt( CPoint3D *v )
{
    float x(v->x - pos.tx);
    float y(v->y - pos.ty);
    float z(v->z - pos.tz);

    return (x * x) + (y * y) + (z * z);

}   // CalDistToSurfaceSt


/************************************************************************
*    FUNCTION NAME:         CalAngleToSurfaceDy()                                                             
*
*    DESCRIPTION:           Calculate the angle of the surface to the light
*                           point - uses translated points for dynamic lighting
*
*    FUNCTION PARAMETERS:
*    Input:    CPoint &n - vertex normal
*              CPoint *v - vertex point
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

double CLight::CalAngleToSurfaceDy( CPoint &n, CPoint3D *v )
{
    // Remember, we need to convert our normal that we packaged
    // as a point so that it can easily be translated. That is
    // why we subtract it from the point we added to it (n.tx - v->tx).
    return ( ((n.tx - v->tx) * pos.tx) + 
             ((n.ty - v->ty) * pos.ty) + 
             ((n.tz - v->tz) * pos.tz) ) / mag;

}   // CalAngleToSurfaceDy


/************************************************************************
*    FUNCTION NAME:         CalAngleToSurfaceDy()                                                             
*
*    DESCRIPTION:           Calculate the angle of the surface to the light
*                           point - uses translated points for dynamic lighting
*
*    FUNCTION PARAMETERS:
*    Input:    CPoint *n - vertex normal
*              CPoint *v - vertex point
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

double CLight::CalAngleToSurfaceDy( CPoint *n, CPoint3D *v )
{
    // Remember, we need to convert our normal that we packaged
    // as a point so that it can easily be translated. That is
    // why we subtract it from the point we added to it (n.tx - v->tx).
    return ( ((n->tx - v->tx) * pos.tx) + 
             ((n->ty - v->ty) * pos.ty) + 
             ((n->tz - v->tz) * pos.tz) ) / mag;

}   // CalAngleToSurfaceDy


/************************************************************************
*    FUNCTION NAME:         CalAngleToSurfaceDy()                                                             
*
*    DESCRIPTION:           Calculate the angle of the surface to the light
*                           point - uses untranslated points and normal
*
*    FUNCTION PARAMETERS:
*    Input:    CPoint &n - vertex normal
*              CPoint *v - vertex point
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

double CLight::CalAngleToSurfaceSt( CPoint &n, CPoint3D *v )
{
    // Remember, we need to convert our normal that we packaged
    // as a point so that it can easily be translated. That is
    // why we subtract it from the point we added to it (n.tx - v->tx).
    return ( ((n.x - v->x) * pos.tx) + 
             ((n.y - v->y) * pos.ty) + 
             ((n.z - v->z) * pos.tz) ) / mag;

}   // CalAngleToSurfaceDy


/************************************************************************
*    FUNCTION NAME:         CalAngleToSurfaceDy()                                                             
*
*    DESCRIPTION:           Calculate the angle of the surface to the light
*                           point - uses untranslated points and normal
*
*    FUNCTION PARAMETERS:
*    Input:    CPoint &n - vertex normal
*              CPoint *v - vertex point
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

double CLight::CalAngleToSurfaceSt( CPoint *n, CPoint3D *v )
{
    // Remember, we need to convert our normal that we packaged
    // as a point so that it can easily be translated. That is
    // why we subtract it from the point we added to it (n.tx - v->tx).
    return ( ((n->x - v->x) * pos.tx) + 
             ((n->y - v->y) * pos.ty) + 
             ((n->z - v->z) * pos.tz) ) / mag;

}   // CalAngleToSurfaceDy


/************************************************************************
*    FUNCTION NAME:         CalDirToSurfaceDy()                                                             
*
*    DESCRIPTION:           Calculate the direction to the surface  - uses
*                           translated points for dynamic lighting
*
*    FUNCTION PARAMETERS:
*    Input:    CPoint &n - vertex point
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

double CLight::CalDirToSurfaceDy( CPoint &v )
{
    return (v.tx * dx) + (v.ty * dy) + (v.tz * dz);

}   // CalDirToSurfaceDy


/************************************************************************
*    FUNCTION NAME:         CalDirToSurfaceDy()                                                             
*
*    DESCRIPTION:           Calculate the direction to the surface - uses
*                           untranslated points and normal
*
*    FUNCTION PARAMETERS:
*    Input:    CPoint &v - vertex point
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

double CLight::CalDirToSurfaceSt( CPoint &v )
{
    return (v.x * dx) + (v.y * dy) + (v.z * dz);

}   // CalDirToSurfaceDy



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
*    H Rosenorn    02/20/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CLightList::CLightList()
{
    count = 0;
    light = NULL;

    // Default lightning
    dynamicLight = EDL_NO_DYNAMIC_LIGHTNING;

}   // Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04       Function Created
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
*    H Rosenorn    02/20/04      Function Created
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
*    FUNCTION NAME:         SetLightCount()                                                             
*
*    DESCRIPTION:           Allocate the number of needed lights
*
*    FUNCTION PARAMETERS:
*    Input:    int lCount - number of lights
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CLightList::SetLightCount( int lCount )
{
    bool result = false;

    // Free any allocated memory
    Free();

    // Make sure the number of lights are not negitive
    if( lCount > 0 )
    {
        // Allocate the needed classes
        light = new CLight[ lCount ];

        if( light != NULL )
        {
            count = lCount;
            result = true;
        }
    }
    return result;

}   // SetLightCount


/************************************************************************
*    FUNCTION NAME:         SetLightType()                                                             
*
*    DESCRIPTION:           Set the type of light.
*
*    FUNCTION PARAMETERS:
*    Input:    int lIndex - light index to set
*              ELT_lightType type - Type of light
*              EShade_Type shade - shading type
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLightList::SetLightType( int lIndex, ELT_lightType type, EShade_Type shade )
{
    // check our bounds
    if( lIndex < count )
    {
        light[lIndex].lightType = type;
        light[lIndex].shadeType = shade;
    }
}   // SetLightType


/************************************************************************
*    FUNCTION NAME:         SetLightPoint()                                                             
*
*    DESCRIPTION:           Set the light point.
*
*    FUNCTION PARAMETERS:
*    Input:    int lIndex - light index to set
*              float x, y, z - light point
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLightList::SetLightPoint( int lIndex, float x, float y, float z )
{
    // check our bounds
    if( lIndex < count )
    {
        light[lIndex].SetLightPt( x, y, z );
    }

}   // SetLightPoint


/************************************************************************
*    FUNCTION NAME:         SetLightIntensity()                                                             
*
*    DESCRIPTION:           Set the light intensity.
*
*    FUNCTION PARAMETERS:
*    Input:    int lIndex - light index to set
*              unsigned int aInt - Intensity
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLightList::SetLightIntensity( int lIndex, unsigned int aInt )
{
    // check our bounds
    if( lIndex < count )
    {
        light[lIndex].SetIntensity( aInt );
    }

}   // SetLightColor


/************************************************************************
*    FUNCTION NAME:         SetLightRange()                                                             
*
*    DESCRIPTION:           Set the light range.
*
*    FUNCTION PARAMETERS:
*    Input:    int lIndex - light index to set
*              int range - Gouraud shaded range
*              int start - Starting shade range.
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLightList::SetLightRange( int lIndex, int range, int start )
{
    // check our bounds
    if( lIndex < count )
    {
        light[lIndex].SetIntenRangeStart( range, start );
    }

}   // SetLightRange


/************************************************************************
*    FUNCTION NAME:         SetSpotLightRadius()                                                             
*
*    DESCRIPTION:           Set the spot light radius.
*
*    FUNCTION PARAMETERS:
*    Input:    int lIndex - light index to set
*              float radius - radius of spot light
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLightList::SetSpotLightRadius( int lIndex, float radius )
{
    // check our bounds
    if( lIndex < count )
    {
        light[lIndex].SetSpotRadius( radius );
    }

}   // SetSpotLightRadius


/************************************************************************
*    FUNCTION NAME:         SetSpotLightDir()                                                             
*
*    DESCRIPTION:           Set the spot light direction
*
*    FUNCTION PARAMETERS:
*    Input:    int lIndex - light index to set
*              float x, y, z - spot light direction
*                              use 1, 0 or -1 to point the light down 
*                              the x,y,z axis.
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLightList::SetSpotLightDir( int lIndex, float x, float y, float z )
{
    // check our bounds
    if( lIndex < count )
    {
        light[lIndex].SetSpotDir( x, y, z );
    }

}   // SetSpotLightDir


/************************************************************************
*    FUNCTION NAME:         SetDynamicLightType()                                                             
*
*    DESCRIPTION:           Set the spot light direction
*
*    FUNCTION PARAMETERS:
*    Input:    EDyn_Light type - dynamic light type
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CLightList::SetDynamicLightType( EDyn_Light type )
{
    dynamicLight = type;

}   // SetDynamicLightType