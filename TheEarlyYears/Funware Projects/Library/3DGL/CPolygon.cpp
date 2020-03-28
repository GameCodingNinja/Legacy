/************************************************************************
*
*    PRODUCT:         3D polygon
*
*    FILE NAME:       CPolygon.cpp
*
*    DESCRIPTION:     Polygon class
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                           // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT             // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>       // Windows header file for creating windows programs. This is a must have.
#include "CPolygon.h"      // Header file for this *.cpp file.
#include <Math.H>

#include "gl/gl.h"         // standard OpenGL include
#include "gl/glu.h"        // OpenGL utilities


// Globals
extern float gfMaxZDistance;
extern float gfMinZDistance;
extern float gfAspectRatio;
unsigned int gTextureCurrentID=0;


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  08/20/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CPolygon::CPolygon()
{
    // Init class members
    vertCount = TRI;
    glType = GL_TRIANGLES;
    multi = 0.33333333;
    tIndex = 0;
    collisionType = 0;
    radius = 0;
    pNormal = NULL;

}   // Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  08/20/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CPolygon::~CPolygon()
{

}   // Destructer


/************************************************************************
*    FUNCTION NAME:         CalcSurfaceNormal()                                                             
*
*    DESCRIPTION:           Calculate the Surface Normal that is then
*                           normalized. This is the normal of the polygon.
*                           This function assumes all vertex points are unique.
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
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CPolygon::CalcSurfaceNormal()
{
    double X1( pVPoint[ 0 ]->x );
    double Y1( pVPoint[ 0 ]->y );
    double Z1( pVPoint[ 0 ]->z );

    double X2( pVPoint[ 1 ]->x );
    double Y2( pVPoint[ 1 ]->y );
    double Z2( pVPoint[ 1 ]->z );

    double X3( pVPoint[ 2 ]->x );
    double Y3( pVPoint[ 2 ]->y );
    double Z3( pVPoint[ 2 ]->z );

    // Use plane equation to determine the orientation of the surface
    float x( Y1 * ( Z2 - Z3 ) + Y2 * ( Z3 - Z1 ) + Y3 * ( Z1 - Z2 ) );
    float y( Z1 * ( X2 - X3 ) + Z2 * ( X3 - X1 ) + Z3 * ( X1 - X2 ) );
    float z( X1 * ( Y2 - Y3 ) + X2 * ( Y3 - Y1 ) + X3 * ( Y1 - Y2 ) );

    // Calculate the length of the vector
    float length( sqrt( (x*x) + (y*y) + (z*z) ) );

    // length can't be zero otherwise you'll get a divided by 0 error.

    // The normalizing part is the (n.x / length). Notice we are also
    // adding in the point (pVPoint[ vIndex ]->x). What this does is
    // allow you to translate the normal like a point. A true normalized
    // value has the value of -1 to 1. After we translate our new type
    // of normal we just subtract it from the same point to get our
    // true normal value back (pNormal->tx - pVPoint[ 0 ]->tx). Cool hu?
    if( length != 0.0 )
    {
        // Normalize the normal and make it a point. Sounds silly hu?
        pNormal->tx = pNormal->x = ( x / length ) + pVPoint[ 0 ]->x;
        pNormal->ty = pNormal->y = ( y / length ) + pVPoint[ 0 ]->y;
        pNormal->tz = pNormal->z = ( z / length ) + pVPoint[ 0 ]->z;
    }
    else
    {
        pNormal->tx = pNormal->x = pVPoint[ 0 ]->x;
        pNormal->ty = pNormal->y = pVPoint[ 0 ]->y;
        pNormal->tz = pNormal->z = pVPoint[ 0 ]->z;
    }

}   // CalcSurfaceNormal


/************************************************************************
*    FUNCTION NAME:         RenderText()
*
*    DESCRIPTION:           Send the info to OpenGL for rendering.
*
*    FUNCTION PARAMETERS:
*    Input:    unsigned int textureID - OpenGL texture index
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall CPolygon::RenderText( PCTextureLib pTextLib )
{
    // Only switch states selecting a new texture if the needed texture is different
    if( gTextureCurrentID != pTextLib->pTMap[tIndex].textureID )
    {
        glBindTexture( GL_TEXTURE_2D, pTextLib->pTMap[tIndex].textureID );
        gTextureCurrentID = pTextLib->pTMap[tIndex].textureID;
    }

    // Tell OpenGL we are going to send it some data
    glBegin( glType );

    for( int i = 0; i < vertCount; ++i )
    {
        glTexCoord2f( uvrgb[i].u, uvrgb[i].v );
        glColor3f( uvrgb[i].r, uvrgb[i].g, uvrgb[i].b );
        glVertex3f( pVPoint[i]->tx, pVPoint[i]->ty, pVPoint[i]->tz );
    }

    // Tell OpenGL we are done sending polygon data
    glEnd();

}   // RenderText


/************************************************************************
*    FUNCTION NAME:         Render()
*
*    DESCRIPTION:           Send the info to OpenGL for rendering.
*
*    FUNCTION PARAMETERS:
*    Input:    unsigned int textureID - OpenGL texture index
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall CPolygon::Render()
{    
    // Tell OpenGL we are going to send it some data
    glBegin( glType );

    for( int i = 0; i < vertCount; ++i )
    {
        glColor3f( uvrgb[i].r, uvrgb[i].g, uvrgb[i].b );
        glVertex3f( pVPoint[i]->tx, pVPoint[i]->ty, pVPoint[i]->tz );
    }

    // Tell OpenGL we are done sending polygon data
    glEnd();

}   // RenderText


/************************************************************************
*    FUNCTION NAME:         CalcBoundingPoly()                                                             
*
*    DESCRIPTION:           Calculate a bounding sphere around the polygon.
*                           Calculating a bounding sphere around a mesh is
*                           a lot simpler because usually the center is 0,0,0
*                           because it makes it easer to translate it in the
*                           world. You just simply search for the farthest
*                           point in the mesh. Calculating a radius around
*                           a single polygon is a little more involved because
*                           you have to revert the polygon to it's center point.
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
*                  09/24/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CPolygon::CalcBoundingPoly()
{
    CPoint offsetPt;
    long double curDistance;
    long double maxDistance = 0.0;
    CPoint centerPt;

    // Average out each vertex
    for( int i = 0; i < vertCount; ++i )
    {
        offsetPt.x += pVPoint[i]->tx;
        offsetPt.y += pVPoint[i]->ty;
        offsetPt.z += pVPoint[i]->tz;
    }

    // Multiplication is faster then division.
    offsetPt.x *= multi;
    offsetPt.y *= multi;
    offsetPt.z *= multi;

    // Find the largest distance
    for( int i = 0; i < vertCount; ++i )
    {
        // Calculate a new position where the center is 0,0,0
        centerPt.x = pVPoint[i]->tx - offsetPt.x;
        centerPt.y = pVPoint[i]->ty - offsetPt.y;
        centerPt.z = pVPoint[i]->tz - offsetPt.z;
        
        // Add the vertex
        curDistance = (centerPt.x * centerPt.x) +
                      (centerPt.y * centerPt.y) +
                      (centerPt.z * centerPt.z);

        // Have we found the longest distance?
        if( curDistance > maxDistance )
            maxDistance = curDistance;
    }

    // compute the radius by getting the vector length
    radius = (float) sqrt( maxDistance );

}   // CalcBoundingPoly


/************************************************************************
*    FUNCTION NAME:         Collide_BoundSphere()                                                             
*
*    DESCRIPTION:           Check for collision. A bounding sphere for
*                           this polygon is used to check for collision
*                           against the supplied pt and it's radius.
*
*    FUNCTION PARAMETERS:
*    Input:    CPoint &pt - Center point of the translated object.
*                           we are checking that we collided with.
*              double &distance - distance of the collision. The game can
*                                 find the closest distance from the collisions
*                                 it finds to see which one to process.
*              float rad - radius of the object the center point belongs to.
*
*    Output:   int - Index of the texture being used. This is a way to tell
*                    what we are colliding with.
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  01/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int _fastcall CPolygon::Collide_BoundSphere( CPoint &pt, double &distance, float rad )
{
    CPoint centerPt;
    double dist;

    // Get the center of the polygon
    for( int i = 0; i < vertCount; ++i )
    {
        centerPt.x += pVPoint[i]->tx;
        centerPt.y += pVPoint[i]->ty;
        centerPt.z += pVPoint[i]->tz;
    }

    centerPt.x *= multi;
    centerPt.y *= multi;
    centerPt.z *= multi;

    // Calculate the distance between the center points of both objects
    dist = sqrt( (( centerPt.x - pt.tx ) * ( centerPt.x - pt.tx )) +
                 (( centerPt.y - pt.ty ) * ( centerPt.y - pt.ty )) +
                 (( centerPt.z - pt.tz ) * ( centerPt.z - pt.tz )) );

    // Check for possible collision by using bounding sphere
    if( dist <= rad + radius )
    {
        // Get the shortest possible distance by subtracting it from the radus
        distance = fabs( dist ) - radius;

        // Return the collision type so we know what we are hitting
        return collisionType;
    }
    else
        return -1;

}   // Collide_BoundSphere


/************************************************************************
*    FUNCTION NAME:         CalcLightIntenDynamic()
*
*    DESCRIPTION:           Calculate light intensity with the translated
*                           normal and vertices.
*
*    FUNCTION PARAMETERS:
*    Input:    CLightList *pLightList - Light list object
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall CPolygon::CalcLightIntenDynamic( CLightList *pLightList )
{
    int vertInten, r[QUAD]={0,0,0,0}, g[QUAD]={0,0,0,0}, b[QUAD]={0,0,0,0};
    double cosAngle;

    for( int lite = 0; lite < pLightList->count; ++lite )
    {
        for( int vIndex = 0; vIndex < vertCount; ++vIndex )
        {
            vertInten = 0;
            
            ///////////////////////////////
            // Infinite Light Calculations
            ///////////////////////////////
            if( pLightList->light[lite].lightType == ELT_POINT )
            {    
                if( pLightList->light[lite].shadeType == EShade_GOURAUD )
                {                                                        
                    // Assign an intensity based on the vertex surface normal
                    cosAngle = pLightList->light[lite].CalAngleToSurfaceDy( uvrgb[ vIndex ].n, pVPoint[vIndex] );
                }
                else if( pLightList->light[lite].shadeType == EShade_FLAT && vIndex == 0 )
                {
                    // Assign an intensity based on the vertex surface normal
                    cosAngle = pLightList->light[lite].CalAngleToSurfaceDy( pNormal, pVPoint[vIndex] );                    
                }

                // Calculate the intensity of the vertex from the angle of the poly to the light source
                vertInten = (int)(( cosAngle * ( double ) pLightList->light[lite].intRange ) + pLightList->light[lite].intStart);
            }
            ///////////////////////////////
            // Spot Light Calculations
            ///////////////////////////////
            else if( pLightList->light[lite].lightType == ELT_SPOT )
            {
                // Get the direction of the light
                if( pLightList->light[lite].CalDirToSurfaceDy( uvrgb[ vIndex ].n ) > 0 )
                {
                    vertInten = (int)(pLightList->light[lite].spotRadius / 
                                     // Calculate the distance from the light source
                                     pLightList->light[lite].CalDistToSurfaceDy( pVPoint[vIndex] )) * 
                                ( double ) pLightList->light[lite].intRange;
                }
            }
            ///////////////////////////////
            // Ambient Light Calculations
            /////////////////////////////// 
            else if( pLightList->light[lite].lightType == ELT_AMBIENT )
            {
                // record the color for this vertex
                r[ vIndex ] += pLightList->light[lite].rColor;
                g[ vIndex ] += pLightList->light[lite].gColor;
                b[ vIndex ] += pLightList->light[lite].bColor;
            }
            ///////////////////////////////
            // Ambient Point Light Calculations
            /////////////////////////////// 
            else if( pLightList->light[lite].lightType == ELT_AMBIENT_POINT )
            {
                // Is the light distance within range
                if( pLightList->light[lite].CalDistToSurfaceDy( pVPoint[vIndex] ) < pLightList->light[lite].spotRadius )
                {
                    // record the color for this vertex
                    r[ vIndex ] += pLightList->light[lite].rColor;
                    g[ vIndex ] += pLightList->light[lite].gColor;
                    b[ vIndex ] += pLightList->light[lite].bColor;
                }
            }

            // Cap the top and bottom of the intensity because it
            // can't be greater then 255 or less then 0. The color will
            // wrap around or do other nasty things.
            if( vertInten > 255 )
                vertInten = 255;
            else if( vertInten < 0 )
                vertInten = 0;

            // Only use if we have an intensity
            if( vertInten > 0 )
            {
                // record the color intensity for this vertex
                r[ vIndex ] += (pLightList->light[lite].rColor * vertInten) >> 8;
                g[ vIndex ] += (pLightList->light[lite].gColor * vertInten) >> 8;
                b[ vIndex ] += (pLightList->light[lite].bColor * vertInten) >> 8;
            }
        }
    }

    // Combine all the lights
    if( pLightList->count > 1 )
    {
        for( int vIndex = 0; vIndex < vertCount; ++vIndex )
        {
            // record the color intensity for this vertex
            if( r[ vIndex ] > 254 )
                uvrgb[ vIndex ].r = 1.0;
            else
                uvrgb[ vIndex ].r = (float)r[ vIndex ] * 0.0039215;/// 255.0f;  i/z conversion for fast lighting

            if( g[ vIndex ] > 254 )
                uvrgb[ vIndex ].g = 1.0;
            else
                uvrgb[ vIndex ].g = (float)g[ vIndex ] * 0.0039215;/// 255.0f;

            if( b[ vIndex ] > 254 )
                uvrgb[ vIndex ].b = 1.0;
            else
                uvrgb[ vIndex ].b = (float)b[ vIndex ] * 0.0039215;/// 255.0f;
        }
    }
    else
    {
        for( int vIndex = 0; vIndex < vertCount; ++vIndex )
        {
            // record the color intensity for this vertex
            uvrgb[ vIndex ].r = (float)r[ vIndex ] * 0.0039215;/// 255.0f;
            uvrgb[ vIndex ].g = (float)g[ vIndex ] * 0.0039215;/// 255.0f;
            uvrgb[ vIndex ].b = (float)b[ vIndex ] * 0.0039215;/// 255.0f;
        }
    }

}  // CalcLightIntenDynamic


/************************************************************************
*    FUNCTION NAME:         CalcLightIntenStatic()
*
*    DESCRIPTION:           Calculate light intensity with load-time
*                           normal and vertices
*
*    FUNCTION PARAMETERS:
*    Input:    CLightList *pLightList - Light list object
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall CPolygon::CalcLightIntenStatic( CLightList *pLightList )
{
    int vertInten, r[QUAD]={0,0,0,0}, g[QUAD]={0,0,0,0}, b[QUAD]={0,0,0,0};
    double cosAngle, dist;

    for( int lite = 0; lite < pLightList->count; ++lite )
    {
        for( int vIndex = 0; vIndex < vertCount; ++vIndex )
        {
            vertInten = 0;
            
            ///////////////////////////////
            // Infinite Light Calculations
            ///////////////////////////////
            if( pLightList->light[lite].lightType == ELT_POINT )
            {    
                if( pLightList->light[lite].shadeType == EShade_GOURAUD )
                {                                                        
                    // Assign an intensity based on the vertex surface normal
                    cosAngle = pLightList->light[lite].CalAngleToSurfaceSt( uvrgb[ vIndex ].n, pVPoint[vIndex] );
                }
                else if( pLightList->light[lite].shadeType == EShade_FLAT && vIndex == 0 )
                {
                    // Assign an intensity based on the vertex surface normal
                    cosAngle = pLightList->light[lite].CalAngleToSurfaceSt( pNormal, pVPoint[vIndex] );                    
                }

                // Calculate the intensity of the vertex from the angle of the poly to the light source
                vertInten = (int)(( cosAngle * ( double ) pLightList->light[lite].intRange ) + pLightList->light[lite].intStart);
            }
            ///////////////////////////////
            // Spot Light Calculations
            ///////////////////////////////
            else if( pLightList->light[lite].lightType == ELT_SPOT )
            {
                // Get the direction of the light
                if( pLightList->light[lite].CalDirToSurfaceSt( uvrgb[ vIndex ].n ) > 0 )
                {                   
                    dist = pLightList->light[lite].CalDistToSurfaceSt( pVPoint[vIndex] );

                    if( dist )
                    {
                        vertInten = (int)(pLightList->light[lite].spotRadius / 
                                         // Calculate the distance from the light source
                                         dist ) * ( double ) pLightList->light[lite].intRange;
                    }
                }
            }
            ///////////////////////////////
            // Ambient Light Calculations
            /////////////////////////////// 
            else if( pLightList->light[lite].lightType == ELT_AMBIENT )
            {
                // record the color for this vertex
                r[ vIndex ] += pLightList->light[lite].rColor;
                g[ vIndex ] += pLightList->light[lite].gColor;
                b[ vIndex ] += pLightList->light[lite].bColor;
            }
            ///////////////////////////////
            // Ambient Point Light Calculations
            /////////////////////////////// 
            else if( pLightList->light[lite].lightType == ELT_AMBIENT_POINT )
            {
                // Is the light distance within range
                if( pLightList->light[lite].CalDistToSurfaceSt( pVPoint[vIndex] ) < pLightList->light[lite].spotRadius )
                {
                    // record the color for this vertex
                    r[ vIndex ] += pLightList->light[lite].rColor;
                    g[ vIndex ] += pLightList->light[lite].gColor;
                    b[ vIndex ] += pLightList->light[lite].bColor;
                }
            }

            // Cap the top and bottom of the intensity because it
            // can't be greater then 255 or less then 0. The color will
            // wrap around or do other nasty things.
            if( vertInten > 255 )
                vertInten = 255;
            else if( vertInten < 0 )
                vertInten = 0;

            // Only use if we have an intensity
            if( vertInten > 0 )
            {
                // record the color intensity for this vertex
                r[ vIndex ] += (pLightList->light[lite].rColor * vertInten) >> 8;
                g[ vIndex ] += (pLightList->light[lite].gColor * vertInten) >> 8;
                b[ vIndex ] += (pLightList->light[lite].bColor * vertInten) >> 8;
            }
        }
    }

    // Combine all the lights
    if( pLightList->count > 1 )
    {
        for( int vIndex = 0; vIndex < vertCount; ++vIndex )
        {
            // record the color intensity for this vertex
            if( r[ vIndex ] > 254 )
                uvrgb[ vIndex ].r = 1.0;
            else
                uvrgb[ vIndex ].r = (float)r[ vIndex ] * 0.0039215;/// 255.0f;  i/z conversion for fast lighting

            if( g[ vIndex ] > 254 )
                uvrgb[ vIndex ].g = 1.0;
            else
                uvrgb[ vIndex ].g = (float)g[ vIndex ] * 0.0039215;/// 255.0f;

            if( b[ vIndex ] > 254 )
                uvrgb[ vIndex ].b = 1.0;
            else
                uvrgb[ vIndex ].b = (float)b[ vIndex ] * 0.0039215;/// 255.0f;
        }
    }
    else
    {
        for( int vIndex = 0; vIndex < vertCount; ++vIndex )
        {
            // record the color intensity for this vertex
            uvrgb[ vIndex ].r = (float)r[ vIndex ] * 0.0039215;/// 255.0f;
            uvrgb[ vIndex ].g = (float)g[ vIndex ] * 0.0039215;/// 255.0f;
            uvrgb[ vIndex ].b = (float)b[ vIndex ] * 0.0039215;/// 255.0f;
        }
    }

}  // CalcLightIntenStatic


/************************************************************************
*    FUNCTION NAME:         CopyPoint()                                                             
*
*    DESCRIPTION:           Get the a specific point of the polygon
*
*    FUNCTION PARAMETERS:
*    Input:    CPoint &pt - Point class to copy normal to
*              int index - index to copy form
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CPolygon::CopyPoint( CPoint &pt, int index )
{
    pt.x = pVPoint[index]->x;
    pt.y = pVPoint[index]->y;
    pt.z = pVPoint[index]->z;

    pt.tx = pVPoint[index]->tx;
    pt.ty = pVPoint[index]->ty;
    pt.tz = pVPoint[index]->tz;

}   // CopyPoint


/************************************************************************
*    FUNCTION NAME:         CopyNormal()                                                             
*
*    DESCRIPTION:           Get a copy of the normal
*
*    FUNCTION PARAMETERS:
*    Input:    CPoint &pt - Point class to copy normal to
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CPolygon::CopyNormal( CPoint &pt )
{
    pt.x = pNormal->x;
    pt.y = pNormal->y;
    pt.z = pNormal->z;

    pt.tx = pNormal->tx;
    pt.ty = pNormal->ty;
    pt.tz = pNormal->tz;

}   // CopyNormal


/************************************************************************
*    FUNCTION NAME:         IsVisable()                                                             
*
*    DESCRIPTION:           Check if the polygon is visible. Perform 3D culling
*                           This assumes a 45 degree view area.
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
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool _fastcall CPolygon::IsVisable()
{
    bool visible = false;
    
    ////////////////////////////////////////////
    // Eliminate the triangle if it is behind us
    ////////////////////////////////////////////

    for( int i = 0; i < vertCount && !visible; ++i )
    {    
        if( pVPoint[ i ]->tz < gfMinZDistance )
            visible = true;
    }

    if( visible == false )
        return visible;

    /////////////////////////////////////////////////
    // Eliminate the triangle is too far ahead to see
    /////////////////////////////////////////////////

    for( int i = 0; i < vertCount && !visible; ++i )
    {
        if( pVPoint[ i ]->tz > gfMaxZDistance )
            visible = true;
    }

    if( visible == false )
        return visible;

    /////////////////////////////////////////////////
    // Eliminate back facing polygons
    /////////////////////////////////////////////////

    // Keep in mind our normal is hidden and to get it we have to
    // pNormal->tx - pVPoint[ 0 ]->tx
    double direction = (pVPoint[ 0 ]->tx * ( pNormal->tx - pVPoint[ 0 ]->tx )) + 
                       (pVPoint[ 0 ]->ty * ( pNormal->ty - pVPoint[ 0 ]->ty )) + 
                       (pVPoint[ 0 ]->tz * ( pNormal->tz - pVPoint[ 0 ]->tz ));

    if( direction < 0.0 )
        return false;
    
    /////////////////////////////////////////
    // Test x & y sides of the view frustrum
    /////////////////////////////////////////
    
    float farPointY( pVPoint[ 0 ]->tz );
    double xCenter( (double)pVPoint[ 0 ]->tx );
    double yCenter( (double)pVPoint[ 0 ]->ty );

    for( int i = 1; i < vertCount; ++i )
    {
        // Find the farthest Z point
        if( pVPoint[ i ]->tz < farPointY )
            farPointY = pVPoint[ i ]->tz;

        // Add up the centers
        xCenter += (double)pVPoint[ i ]->tx;
        yCenter += (double)pVPoint[ i ]->ty;
    }

    // Height is always half the distance
    farPointY *= 0.5;

    // Calculate the center    
    yCenter *= (double)multi;

    if( yCenter + radius < farPointY )
        return false;

    if( yCenter - radius > -farPointY )
        return false;

    // Add in the aspect ration for the width
    float farPointX( farPointY * gfAspectRatio );

    // Calculate the center
    xCenter *= (double)multi;
        
    if( xCenter + radius < farPointX )
        return false;
    
    if( xCenter - radius > -farPointX )
        return false;

    return visible;

}   // IsVisable


/************************************************************************
*    FUNCTION NAME:         IsVisable()                                                             
*
*    DESCRIPTION:           Check if the polygon is visible. Perform 3D 
*                           culling using a portal.
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
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool _fastcall CPolygon::IsVisable( float portRadiusX, float portRadiusY, CPoint *pPortalCenter )
{
    bool visible = false;
    
    // Eliminate the triangle if it is behind us
    for( int i = 0; i < vertCount && !visible; ++i )
    {    
        if( pVPoint[ i ]->tz < gfMinZDistance )
            visible = true;
    }

    if( visible == false )
        return visible;

    // Eliminate the triangle is too far ahead to see
    for( int i = 0; i < vertCount && !visible; ++i )
    {
        if( pVPoint[ i ]->tz > gfMaxZDistance )
            visible = true;
    }

    if( visible == false )
        return visible;

    // Check to see if the back is facing us
    // Keep in mind our normal is hidden and to get it we have to
    // pNormal->tx - pVPoint[ 0 ]->tx
    double direction = (pVPoint[ 0 ]->tx * ( pNormal->tx - pVPoint[ 0 ]->tx )) + 
                       (pVPoint[ 0 ]->ty * ( pNormal->ty - pVPoint[ 0 ]->ty )) + 
                       (pVPoint[ 0 ]->tz * ( pNormal->tz - pVPoint[ 0 ]->tz ));

    if( direction < 0.0F )
        return false;
    
    ////////////////////////////////////////
    // Test x & y sides of the view frustrum
    ///////////////////////////////////////
    
    float farPoint( pVPoint[ 0 ]->tz );
    double xCenter( (double)pVPoint[ 0 ]->tx );
    double yCenter( (double)pVPoint[ 0 ]->ty );

    for( int i = 1; i < vertCount; ++i )
    {
        // Find the farthest Z point
        if( pVPoint[ i ]->tz < farPoint )
            farPoint = pVPoint[ i ]->tz;

        // Add up the centers
        xCenter += (double)pVPoint[ i ]->tx;
        yCenter += (double)pVPoint[ i ]->ty;
    }

    // Calculate the difference between the portal and the polys
    double diff = farPoint / (double)pPortalCenter->tz;

    double portalRadius = (double)portRadiusX * diff;
    double portalCenter = (double)pPortalCenter->tx * diff;

    // Calculate the centers
    xCenter *= (double)multi;

    if( xCenter + radius < portalCenter - portalRadius )
        return false;
    
    if( xCenter - radius > portalCenter + portalRadius )
        return false;

    portalRadius = (double)portRadiusY * diff;
    portalCenter = (double)pPortalCenter->ty * diff;

    // Calculate the centers
    yCenter *= (double)multi;

    if( yCenter + radius < portalCenter - portalRadius )
        return false;

    if( yCenter - radius > portalCenter + portalRadius )
        return false;

    return visible;

}   // IsVisable


/************************************************************************
*    FUNCTION NAME:         HasVert()                                                             
*
*    DESCRIPTION:           Is the supplied vertice the same
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
*                  11/04/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CPolygon::HasVert( const CPoint &P )
{
    for( int i = 0; i < vertCount; ++i )
    {
        if( ( *pVPoint[i] ) == P )
            return true;
    }

    return false;

}   // HasVert


/************************************************************************
*    FUNCTION NAME:         LoadPolygonData()                                                             
*
*    DESCRIPTION:           Load polygon data from file
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - File handle
*              var1type Var1Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CPolygon::LoadPolygonData( FILE *hFile, CPoint *vertLst )
{
    CFaceLoad faceLoad;

    // Read in all the data at once
    if( fread( &faceLoad, sizeof(faceLoad), 1, hFile ) != 1 )
        return false;

    // Copy the texture index
    tIndex = faceLoad.tIndex;

    // Get the vert count
    vertCount = (int)faceLoad.vertCount;

    // Copy the type
    collisionType = (int)faceLoad.collisionType;

    // Set some class members depending on the number of verts
    if( vertCount == TRI )
    {
        glType = GL_TRIANGLES;
        multi = 0.33333333;
    }
    else if( vertCount == QUAD )
    {
        glType = GL_QUADS;
        multi = 0.25;
    }
    else  // There's a big problem if this is out of range
    {   
        return false;
    }
    
    // Use the vertex index to set the pointer
    for( int i = 0; i < vertCount; ++i )
    {
        // Set the pointer to the vertex list
        pVPoint[i] = &vertLst[ faceLoad.vIndex[i] ];
    }
    
    // Copy the UV corridinates
    for( int i = 0; i < vertCount; ++i )
    {
        uvrgb[i].u = faceLoad.crd[i].uv[0];
        uvrgb[i].v = faceLoad.crd[i].uv[1];
    }

    // Load the polygon data
    if( !LoadExtraPolygonData( hFile ) )
        return false;

    return true;

}   // LoadPolygonData


/************************************************************************
*    FUNCTION NAME:         SavePolygonData()                                                             
*
*    DESCRIPTION:           Save polygon data from file
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - File handle
*              var1type Var1Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CPolygon::SavePolygonData( FILE *hFile, int vertexCount, CPoint *vertLst )
{
    CFaceLoad faceLoad;

    // find the vertex index
    for( int i = 0; i < vertCount; ++i )
    {
        // Go through the vertex list and find the index
        // of the point we are using
        for( int j = 0; j < vertexCount; ++j )
        {
            if( vertLst[j] == *pVPoint[i] )
            {
                faceLoad.vIndex[i] = j;
                break;
            }
        }
    }
    
    // Copy the UV corridinates
    for( int i = 0; i < QUAD; ++i )
    {
        faceLoad.crd[i].uv[0] = uvrgb[i].u;
        faceLoad.crd[i].uv[1] = uvrgb[i].v;
    }

    // Copy the texture index
    faceLoad.tIndex = tIndex;

    // Get the vert count
    faceLoad.vertCount = (unsigned char)vertCount;

    // Copy the type
    faceLoad.collisionType = (unsigned char)collisionType;

    // Save all the data at once
    if( fwrite( &faceLoad, sizeof(faceLoad), 1, hFile ) != 1 )
        return false;
    
    // Save extra polygon data
    if( !SaveExtraPolygonData( hFile ) )
        return false;

    return true;

}   // SavePolygonData


/************************************************************************
*    FUNCTION NAME:         CopyPolygon()                                                             
*
*    DESCRIPTION:           Copy the supplied polygon
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - File handle
*              var1type Var1Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CPolygon::CopyPolygon( CPolygon &poly, CPoint *vertLst, int vertexCount )
{
    // Copy the texture index
    tIndex = poly.GetTextIndex();

    // Get the vert count
    vertCount = poly.GetVertexCount();

    // Copy the type
    collisionType = poly.GetCollisionType();

    // Copy the radius
    radius = poly.GetRadius();

    // Set some class members depending on the number of verts
    glType = poly.GetGLType();
    multi = poly.GetMulti();

    // find the vertex index
    for( int i = 0; i < vertCount; ++i )
    {
        // Go through the vertex list and find the index
        // of the point we are using
        for( int j = 0; j < vertexCount; ++j )
        {
            if( vertLst[j] == *poly.GetVPoint(i) )
            {
                // Set the pointer to the vertex list
                pVPoint[i] = &vertLst[j];
                break;
            }
        }
    }

    // Copy the UV corridinates
    for( int i = 0; i < vertCount; ++i )
    {
        uvrgb[i] = poly.GetUVRGB()[i];
    }

}   // CopyPolygon


/************************************************************************
*    FUNCTION NAME:         LoadPolygonData()                                                             
*
*    DESCRIPTION:           Load extra polygon data. Virtual call so that
*                           an inherited class can load up it's own polygon
*                           class data.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var2Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CPolygon::LoadExtraPolygonData( FILE *hFile )
{
    return true;

}   // LoadPolygonData


/************************************************************************
*    FUNCTION NAME:         SaveExtraPolygonData()                                                             
*
*    DESCRIPTION:           Save polygon data. Virtual call so that
*                           an inherited class can save it's own polygon
*                           class data.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var2Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CPolygon::SaveExtraPolygonData( FILE *hFile )
{
    return true;

}   // SaveExtraPolygonData


/************************************************************************
*    FUNCTION NAME:         ConvertToPureNormals()                                                             
*
*    DESCRIPTION:           Convert the normal to it's pure self
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var1type Var1Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CPolygon::ConvertToPureNormals()
{
    pNormal->x -= pVPoint[ 0 ]->x;
    pNormal->y -= pVPoint[ 0 ]->y;
    pNormal->z -= pVPoint[ 0 ]->z;

    for( int i = 0; i < vertCount; ++i )
    {
        uvrgb[ i ].n.x -= pVPoint[ i ]->x;
        uvrgb[ i ].n.y -= pVPoint[ i ]->y;
        uvrgb[ i ].n.z -= pVPoint[ i ]->z;
    }

}   // ConvertNormalsToPure


/************************************************************************
*    FUNCTION NAME:         ConvertToPureNormals()                                                             
*
*    DESCRIPTION:           Convert the normal to it's pure self
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var1type Var1Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CPolygon::ConvertPureNormalsToVectors()
{
    pNormal->x += pVPoint[ 0 ]->x;
    pNormal->y += pVPoint[ 0 ]->y;
    pNormal->z += pVPoint[ 0 ]->z;

    for( int i = 0; i < vertCount; ++i )
    {
        uvrgb[ i ].n.x += pVPoint[ i ]->x;
        uvrgb[ i ].n.y += pVPoint[ i ]->y;
        uvrgb[ i ].n.z += pVPoint[ i ]->z;
    }

}   // ConvertNormalsToPure


/************************************************************************
*    FUNCTION NAME:         SaveNormalsToFile()                                                             
*
*    DESCRIPTION:           Save all the normals to a file.
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - File handle
*              var1type Var1Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CPolygon::SaveNormalsToFile( FILE *hFile )
{
    CNormalLoad normalLoad;

    normalLoad.norm[0] = pNormal->x;
    normalLoad.norm[1] = pNormal->y;
    normalLoad.norm[2] = pNormal->z;

    for( int i = 0; i < vertCount; ++i )
    {
        normalLoad.vertNor[i].norm[0] = uvrgb[i].n.x;
        normalLoad.vertNor[i].norm[1] = uvrgb[i].n.y;
        normalLoad.vertNor[i].norm[2] = uvrgb[i].n.z;
    }

    // Write the surface and vertex normals in one shot
    if( fwrite( &normalLoad, sizeof( CNormal ), vertCount + 1, hFile ) != (vertCount + 1) )
        return false;

    return true;

}   // SaveNormalsToFile


/************************************************************************
*    FUNCTION NAME:         LoadNormalsFromFile()                                                             
*
*    DESCRIPTION:           Load all the normals from file. Helpful for
*                           quick loading of large data sets.
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - File handle
*              var1type Var1Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CPolygon::LoadNormalsFromFile( FILE *hFile )
{
    CNormalLoad normalLoad;

    // Read the surface and vertex normals in one shot
    if( fread( &normalLoad, sizeof( CNormal ), vertCount + 1, hFile ) != (vertCount + 1) )
        return false;

    // Make a copy to the transformed normals for startup inits
    pNormal->tx = pNormal->x = normalLoad.norm[0];
    pNormal->ty = pNormal->y = normalLoad.norm[1];
    pNormal->tz = pNormal->z = normalLoad.norm[2];

    for( int i = 0; i < vertCount; ++i )
    {
        // Make a copy to the transformed normals for startup inits
        uvrgb[i].n.tx = uvrgb[i].n.x = normalLoad.vertNor[i].norm[0];
        uvrgb[i].n.ty = uvrgb[i].n.y = normalLoad.vertNor[i].norm[1];
        uvrgb[i].n.tz = uvrgb[i].n.z = normalLoad.vertNor[i].norm[2];
    }

    return true;

}   // LoadNormalsFromFile


/************************************************************************
*    FUNCTION NAME:         SetAmbientLightLevel()
*
*    DESCRIPTION:           Set an ambient light level for this polygon.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var2Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CPolygon::SetAmbientLightLevel( float rColor, float gColor, float bColor )
{
    for( int i = 0; i < vertCount; ++i )
    {
        // record the color intensity for this vertex
        uvrgb[ i ].r = rColor;
        uvrgb[ i ].g = gColor;
        uvrgb[ i ].b = bColor;
    }

}   // SetAmbientLightLevel