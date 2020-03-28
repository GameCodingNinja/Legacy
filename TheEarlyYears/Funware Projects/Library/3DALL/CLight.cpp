
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


/************************************************************************
*    DESCRIPTION:  Constructer                                                             
*
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

    // Set the color of the light
    SetColor( 255, 255, 255 );

    // Default intensity settings
    intRange = 80;
    intStart = 170;

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
*    DESCRIPTION:  Destructer                                                             
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
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
*
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
*    DESCRIPTION:  Calculate the magnitude from the transformed point
*
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
*    DESCRIPTION:  Set the color of the light via a packed DWORD
*
*    Input:    unsigned int aColor - RGB packed DWORD
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
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
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
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
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
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
*    DESCRIPTION:  Calculate the distance to the surface to the light 
*                  point. This function uses translated light points for
*                  dynamic runtime light calculations.
*
*    Input:    CPoint *v - vertex point
*
*    Output:   float - Distance to surface
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
float CLight::CalDistToSurfaceDy( CPoint *v )
{
    float x(v->tx - pos.tx);
    float y(v->ty - pos.ty);
    float z(v->tz - pos.tz);

    return (x * x) + (y * y) + (z * z);

}   // CalDistToSurfaceDy


/************************************************************************
*    DESCRIPTION:  Calculate the distance to the surface to the light 
*                  point. This function uses translated light points for
*                  dynamic runtime light calculations.
*
*    Input:    CPoint *v - vertex point
*              var2type Var2Name - Description
*
*    Output:   float - Distance to surface
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
float CLight::CalDistToSurfaceSt( CPoint *v )
{
    float x(v->x - pos.tx);
    float y(v->y - pos.ty);
    float z(v->z - pos.tz);

    return (x * x) + (y * y) + (z * z);

}   // CalDistToSurfaceSt


/************************************************************************
*    DESCRIPTION:  Calculate the angle of the surface to the light
*                  point - uses translated points for dynamic lighting
*
*    Input:    CPoint &n - vertex normal
*              CPoint *v - vertex point
*
*    Output:   double - Angle to surface
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
double CLight::CalAngleToSurfaceDy( CPoint &n, CPoint *v )
{
    // Remember, we need to convert our normal that we packaged
    // as a point so that it can easily be translated. That is
    // why we subtract it from the point we added to it (n.tx - v->tx).
    return ( ((n.tx - v->tx) * pos.tx) + 
             ((n.ty - v->ty) * pos.ty) + 
             ((n.tz - v->tz) * pos.tz) ) / mag;

}   // CalAngleToSurfaceDy


/************************************************************************
*    DESCRIPTION:  Calculate the angle of the surface to the light
*                  point - uses translated points for dynamic lighting
*
*    Input:    CPoint *n - vertex normal
*              CPoint *v - vertex point
*
*    Output:   double - Angle to surface
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
double CLight::CalAngleToSurfaceDy( CPoint *n, CPoint *v )
{
    // Remember, we need to convert our normal that we packaged
    // as a point so that it can easily be translated. That is
    // why we subtract it from the point we added to it (n.tx - v->tx).
    return ( ((n->tx - v->tx) * pos.tx) + 
             ((n->ty - v->ty) * pos.ty) + 
             ((n->tz - v->tz) * pos.tz) ) / mag;

}   // CalAngleToSurfaceDy


/************************************************************************
*    DESCRIPTION:  Calculate the angle of the surface to the light
*                  point - uses untranslated points and normal
*
*    Input:    CPoint &n - vertex normal
*              CPoint *v - vertex point
*
*    Output:   double - Angle to surface
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
double CLight::CalAngleToSurfaceSt( CPoint &n, CPoint *v )
{
    // Remember, we need to convert our normal that we packaged
    // as a point so that it can easily be translated. That is
    // why we subtract it from the point we added to it (n.tx - v->tx).
    return ( ((n.x - v->x) * pos.tx) + 
             ((n.y - v->y) * pos.ty) + 
             ((n.z - v->z) * pos.tz) ) / mag;

}   // CalAngleToSurfaceDy


/************************************************************************
*    DESCRIPTION:  Calculate the angle of the surface to the light
*                  point - uses untranslated points and normal
*
*    Input:    CPoint &n - vertex normal
*              CPoint *v - vertex point
*
*    Output:   double - Angle to surface
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
double CLight::CalAngleToSurfaceSt( CPoint *n, CPoint *v )
{
    // Remember, we need to convert our normal that we packaged
    // as a point so that it can easily be translated. That is
    // why we subtract it from the point we added to it (n.tx - v->tx).
    return ( ((n->x - v->x) * pos.tx) + 
             ((n->y - v->y) * pos.ty) + 
             ((n->z - v->z) * pos.tz) ) / mag;

}   // CalAngleToSurfaceDy


/************************************************************************
*    DESCRIPTION:  Calculate the direction to the surface  - uses
*                  translated points for dynamic lighting
*
*    Input:    CPoint &n - vertex point
*              var2type Var2Name - Description
*
*    Output:   double - Direction to surface
*
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
*    DESCRIPTION:  Calculate the direction to the surface - uses
*                  untranslated points and normal
*
*    Input:    CPoint &v - vertex point
*              var2type Var2Name - Description
*
*    Output:   double - Direction to surface
*
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
*    DESCRIPTION:  Constructer                                                             
*
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
*    DESCRIPTION:  Destructer                                                             
*
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
*    DESCRIPTION:  Free allocated memory
*
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
*    DESCRIPTION:  Allocate the number of needed lights
*
*    Input:    int lCount - number of lights
*
*    Output:   bool - true for success, false for fail
*
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
*    DESCRIPTION:  Set the type of light.
*
*    Input:    int lIndex - light index to set
*              ELT_lightType type - Type of light
*              EShade_Type shade - shading type
*
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
*    DESCRIPTION:  Set the light point.
*
*    Input:    int lIndex - light index to set
*              float x, y, z - light point
*
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
*    DESCRIPTION:  Set the light color.
*
*    Input:    int lIndex - light index to set
*              unsigned int r, g, b - color
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void CLightList::SetLightColor( int lIndex, unsigned int r, unsigned int g, unsigned int b )
{
    // check our bounds
    if( lIndex < count )
    {
        light[lIndex].SetColor( r, g, b );
    }

}   // SetLightColor


/************************************************************************
*    DESCRIPTION:  Set the light color.
*
*    Input:    int lIndex - light index to set
*              unsigned int aColor - color
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void CLightList::SetLightColor( int lIndex, unsigned int aColor )
{
    // check our bounds
    if( lIndex < count )
    {
        light[lIndex].SetColor( aColor );
    }

}   // SetLightColor


/************************************************************************
*    DESCRIPTION:  Set the light color.
*
*    Input:    int lIndex - light index to set
*              COLORREF aColor - color
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void CLightList::SetLightColor( int lIndex, COLORREF aColor )
{
    // check our bounds
    if( lIndex < count )
    {
        light[lIndex].SetColor( aColor );
    }

}   // SetLightColor


/************************************************************************
*    DESCRIPTION:  Set the light range.
*
*    Input:    int lIndex - light index to set
*              int range - Gouraud shaded range
*              int start - Starting shade range.
*
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
*    DESCRIPTION:  Set the spot light radius.
*
*    Input:    int lIndex - light index to set
*              float radius - radius of spot light
*
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
*    DESCRIPTION:  Set the spot light direction
*
*    Input:    int lIndex - light index to set
*              float x, y, z - spot light direction
*                              use 1, 0 or -1 to point the light down 
*                              the x,y,z axis.
*
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
*    DESCRIPTION:  Set the spot light direction
*
*    Input:    EDyn_Light type - dynamic light type
*              var2type Var2Name - Description
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/20/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void CLightList::SetDynamicLightType( EDyn_Light type )
{
    dynamicLight = type;

}   // SetDynamicLightType