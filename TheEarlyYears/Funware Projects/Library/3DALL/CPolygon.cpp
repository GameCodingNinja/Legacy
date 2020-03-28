/************************************************************************
*
*    FILE NAME:       CPolygon.cpp
*
*    DESCRIPTION:     Polygon class
*
*
************************************************************************/

                           // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT             // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>       // Windows header file for creating windows programs. This is a must have.
#include "CPolygon.h"      // Header file for this *.cpp file.
#include <Math.H>


// Globals
extern float gfMaxZDistance;
extern float gfMinZDistance;
extern float gfFrustrumYRatio;
extern float gfSquarePercentage;
extern int giRenderDevice;


/************************************************************************
*    DESCRIPTION:  Constructer                                                             
*
*    -------------------------------------------------------------------
*                  08/20/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
CPolygon::CPolygon()
{
    // Init class members
    vertCount = TRI;
	inverseVertCount = 1.0 / (float)vertCount;
    tIndex = 0;
    collisionType = ECT_NO_TYPE;
    radius = 0;
    radiusX = 0;
    radiusY = 0;
    pNormal = NULL;
    projCount = 0;

}   // Constructer


/************************************************************************
*    DESCRIPTION:   Destructer                                                             
*
*    -------------------------------------------------------------------
*                  08/20/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
CPolygon::~CPolygon()
{

}   // Destructer


/************************************************************************
*    DESCRIPTION:  Calculate the Surface Normal that is then
*                  normalized. This is the normal of the polygon.
*                  This function assumes all vertex points are unique.
*
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

    // Calculate the cross product
    double x( Y1 * ( Z2 - Z3 ) + Y2 * ( Z3 - Z1 ) + Y3 * ( Z1 - Z2 ) );
    double y( Z1 * ( X2 - X3 ) + Z2 * ( X3 - X1 ) + Z3 * ( X1 - X2 ) );
    double z( X1 * ( Y2 - Y3 ) + X2 * ( Y3 - Y1 ) + X3 * ( Y1 - Y2 ) );

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
*    DESCRIPTION:  Calculate a bounding sphere around the polygon.
*                  Calculating a bounding sphere around a mesh is
*                  a lot simpler because usually the center is 0,0,0
*                  because it makes it easer to translate it in the
*                  world. You just simply search for the farthest
*                  point in the mesh. Calculating a radius around
*                  a single polygon is a little more involved because
*                  you have to revert the polygon to it's center point.
*
*    -------------------------------------------------------------------
*                  09/24/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void CPolygon::CalcBoundingPoly()
{
    CPoint pt;
    double curDistance;
    double maxDistance = 0.0;
    double maxDistanceX = 0.0;
    double maxDistanceY = 0.0;
    CPoint centerPt;

    // Average out each vertex
    for( int i = 0; i < vertCount; ++i )
    {
        centerPt.x += pVPoint[i]->x;
        centerPt.y += pVPoint[i]->y;
        centerPt.z += pVPoint[i]->z;
    }

    // below = centerPx.# /= vertCount
    centerPt.x *= inverseVertCount;
    centerPt.y *= inverseVertCount;
    centerPt.z *= inverseVertCount;

    // Find the largest distance
    for( int i = 0; i < vertCount; ++i )
    {
        // Calculate a new position where the center is 0,0,0
        pt.x = pVPoint[i]->x - centerPt.x;
        pt.y = pVPoint[i]->y - centerPt.y;
        pt.z = pVPoint[i]->z - centerPt.z;
        
        // Add the vertex
        curDistance = (pt.x * pt.x) +
                      (pt.y * pt.y) +
                      (pt.z * pt.z);

        // Have we found the longest distance?
        if( curDistance > maxDistance )
            maxDistance = curDistance;

        curDistance = (pt.x * pt.x) + (pt.z * pt.z);

        if( curDistance > maxDistanceX )
            maxDistanceX = curDistance;

        curDistance = (pt.y * pt.y) + (pt.z * pt.z);

        if( curDistance > maxDistanceY )
            maxDistanceY = curDistance;
    }

    // compute the radius by getting the vector length
    radius = sqrt( maxDistance );
    radiusX = sqrt( maxDistanceX );
    radiusY = sqrt( maxDistanceY );

}   // CalcBoundingPoly


/************************************************************************
*    DESCRIPTION:  Check for collision. A bounding sphere for
*                  this polygon is used to check for collision
*                  against the supplied pt and it's radius.
*
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
*    -------------------------------------------------------------------
*                  01/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
ECT_COLLISION_TYPE _fastcall CPolygon::Collide_BoundSphere( CPoint &pt, double &distance, float rad )
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

    // below = centerPT.# /= vertCount
    centerPt.x *= inverseVertCount;
    centerPt.y *= inverseVertCount;
    centerPt.z *= inverseVertCount;

    // Calculate the distance between the center points of both objects
    dist = sqrt( (( centerPt.x - pt.tx ) * ( centerPt.x - pt.tx )) +
                 (( centerPt.y - pt.ty ) * ( centerPt.y - pt.ty )) +
                 (( centerPt.z - pt.tz ) * ( centerPt.z - pt.tz )) );

    // Check for possible collision by using bounding sphere
    if( dist <= rad + radius )
    {
        // Get the shortest possible distance by subtracting it from the radus
        distance = dist - radius;

        // Return the collision type so we know what we are hitting
        return collisionType;
    }
    else
        return ECT_NO_COLLISION;

}   // Collide_BoundSphere


/************************************************************************
*    DESCRIPTION:  Check for collision. A bounding sphere for
*                  this polygon is used to check for collision
*                  against the supplied pt and it's radius.
*
*                  This checks untransformed points
*
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
*    -------------------------------------------------------------------
*                  01/18/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
ECT_COLLISION_TYPE _fastcall CPolygon::Collide_BoundSphereSt( CPoint &pt, double &distance, float rad )
{
    CPoint centerPt;
    double dist;

    // Get the center of the polygon
    for( int i = 0; i < vertCount; ++i )
    {
        centerPt.x += pVPoint[i]->x;
        centerPt.y += pVPoint[i]->y;
        centerPt.z += pVPoint[i]->z;
    }

    // below = centerPT.# /= vertCount
    centerPt.x *= inverseVertCount;
    centerPt.y *= inverseVertCount;
    centerPt.z *= inverseVertCount;

    // Calculate the distance between the center points of both objects
    dist = sqrt( (( centerPt.x - pt.tx ) * ( centerPt.x - pt.tx )) +
                 (( centerPt.y - pt.ty ) * ( centerPt.y - pt.ty )) +
                 (( centerPt.z - pt.tz ) * ( centerPt.z - pt.tz )) );

    // Check for possible collision by using bounding sphere
    if( dist <= rad + radius )
    {
        // Get the shortest possible distance by subtracting it from the radus
        distance = dist - radius;

        // Return the collision type so we know what we are hitting
        return collisionType;
    }
    else
        return ECT_NO_COLLISION;

}   // Collide_BoundSphere


/************************************************************************
*    DESCRIPTION:  Calculate light intensity with the translated
*                  normal and vertices.
*
*    Input:    CLightList *pLightList - Light list object
*
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

    if( giRenderDevice == ERD_OPENGL )
    {
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
    }
    else
    {
        // Combine all the lights
        if( pLightList->count > 1 )
        {
            for( int vIndex = 0; vIndex < vertCount; ++vIndex )
            {
                // record the color intensity for this vertex
                if( r[ vIndex ] > 254 )
                    uvrgb[ vIndex ].r = 255.0f;
                else
                    uvrgb[ vIndex ].r = (float)r[ vIndex ];

                if( g[ vIndex ] > 254 )
                    uvrgb[ vIndex ].g = 255.0f;
                else
                    uvrgb[ vIndex ].g = (float)g[ vIndex ];

                if( b[ vIndex ] > 254 )
                    uvrgb[ vIndex ].b = 255.0f;
                else
                    uvrgb[ vIndex ].b = (float)b[ vIndex ];
            }
        }
        else
        {
            for( int vIndex = 0; vIndex < vertCount; ++vIndex )
            {
                // record the color intensity for this vertex
                uvrgb[ vIndex ].r = (float)r[ vIndex ];
                uvrgb[ vIndex ].g = (float)g[ vIndex ];
                uvrgb[ vIndex ].b = (float)b[ vIndex ];
            }
        }
    }

}  // CalcLightIntenDynamic


/************************************************************************
*    DESCRIPTION:  Calculate light intensity with load-time
*                  normal and vertices
*
*    Input:    CLightList *pLightList - Light list object
*
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
    if( giRenderDevice == ERD_OPENGL )
    {
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
    }
    else
    {
        // Combine all the lights
        if( pLightList->count > 1 )
        {
            for( int vIndex = 0; vIndex < vertCount; ++vIndex )
            {
                // record the color intensity for this vertex
                if( r[ vIndex ] > 254 )
                    uvrgb[ vIndex ].r = 255.0f;
                else
                    uvrgb[ vIndex ].r = (float)r[ vIndex ];

                if( g[ vIndex ] > 254 )
                    uvrgb[ vIndex ].g = 255.0f;
                else
                    uvrgb[ vIndex ].g = (float)g[ vIndex ];

                if( b[ vIndex ] > 254 )
                    uvrgb[ vIndex ].b = 255.0f;
                else
                    uvrgb[ vIndex ].b = (float)b[ vIndex ];
            }
        }
        else
        {
            for( int vIndex = 0; vIndex < vertCount; ++vIndex )
            {
                // record the color intensity for this vertex
                uvrgb[ vIndex ].r = (float)r[ vIndex ];
                uvrgb[ vIndex ].g = (float)g[ vIndex ];
                uvrgb[ vIndex ].b = (float)b[ vIndex ];
            }
        }
    }

}  // CalcLightIntenStatic


/************************************************************************
*    DESCRIPTION:   Get the a specific point of the polygon
*
*    Input:    CPoint &pt - Point class to copy normal to
*              int index - index to copy form
*t
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
*    DESCRIPTION:  Get a copy of the normal
*
*    Input:    CPoint &pt - Point class to copy normal to
*
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
*    DESCRIPTION:  copy the normals	from another polygon
*
*    Input:    CPolygon &polygon - Polygon to get normals from
*
*    -------------------------------------------------------------------
*                  11/39/05      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void CPolygon::SetNormals( CPolygon &polygon )
{
	pNormal->tx = pNormal->x = polygon.GetNormal()->x;
    pNormal->ty = pNormal->y = polygon.GetNormal()->y;
    pNormal->tz = pNormal->z = polygon.GetNormal()->z;

    // now the vertex normal
	for( int i = 0; i < vertCount; ++i )
	{
		uvrgb[i].n.tx = uvrgb[i].n.x = polygon.GetUVRGB()[i].n.x;
        uvrgb[i].n.ty = uvrgb[i].n.y = polygon.GetUVRGB()[i].n.y;
        uvrgb[i].n.tz = uvrgb[i].n.z = polygon.GetUVRGB()[i].n.z;
	}

}	// SetNormals


/************************************************************************
*    DESCRIPTION:   Check if the polygon is visible. Perform 3D culling
*                   This assumes a 45 degree view area.
*
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
        if( pVPoint[ i ]->tz < -gfMinZDistance )
            visible = true;
    }

    if( visible == false )
        return visible;

    /////////////////////////////////////////////////
    // Eliminate the triangle is too far ahead to see
    /////////////////////////////////////////////////

    for( int i = 0; i < vertCount && !visible; ++i )
    {
        if( pVPoint[ i ]->tz > -gfMaxZDistance )
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

    
    /////////////////////////////////////////////
    // Add up the X & Y points to find the center
    /////////////////////////////////////////////
    
    float farPoint( pVPoint[ 0 ]->tz );
    float xCenter( pVPoint[ 0 ]->tx );
    float yCenter( pVPoint[ 0 ]->ty );

    for( int i = 1; i < vertCount; ++i )
    {
        // Find the farthest Z point
        if( pVPoint[ i ]->tz < farPoint )
            farPoint = pVPoint[ i ]->tz;

        // Add up the centers
        xCenter += pVPoint[ i ]->tx;
        yCenter += pVPoint[ i ]->ty;
    }

    //////////////////////////////////////////
    // Test against the X side of the frustum
    //////////////////////////////////////////

    // Add in the aspect ration for the width
    float farPointX( farPoint * gfSquarePercentage );

    // Calculate the center
	// below = xCenter /= vertCount
    xCenter *= inverseVertCount;
        
    if( xCenter + radius < farPointX )
        return false;
    
    if( xCenter - radius > -farPointX )
        return false;

    //////////////////////////////////////////
    // Test against the Y side of the frustum
    //////////////////////////////////////////

    // Height is always half the distance
    float farPointY( farPoint * gfFrustrumYRatio );

    // Calculate the center
    // below = yCenter /= vertCount    
    yCenter *= inverseVertCount;

    if( yCenter + radius < farPointY )
        return false;

    if( yCenter - radius > -farPointY )
        return false;
   
    return visible;

}   // IsVisable


/************************************************************************
*    DESCRIPTION:  Check if the polygon is visible. Perform 3D 
*                  culling using a portal.
*                          
*    FUNCTION PARAMETERS:
*    Input:    float portRadius - portal size
*              CPoint *pPortalCenter - Center of portal
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool _fastcall CPolygon::IsVisable( float portRadius, CPoint *pPortalCenter )
{
    bool visible = false;
    
    // Eliminate the triangle if it is behind the near Z plane
    for( int i = 0; i < vertCount && !visible; ++i )
    {    
        if( pVPoint[ i ]->tz < -gfMinZDistance )
            visible = true;
    }

    if( visible == false )
        return visible;

    // Eliminate the triangle if it in front of the far Z plane
    for( int i = 0; i < vertCount && !visible; ++i )
    {
        if( pVPoint[ i ]->tz > -gfMaxZDistance )
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
    
    /////////////////////////////////////////
    // Test x & y sides of the view frustrum
    /////////////////////////////////////////


    /////////////////////////////////////////////
    // Add up the X & Y points to find the center
    /////////////////////////////////////////////
    
    float farPoint( pVPoint[ 0 ]->tz );
    float xCenter( (double)pVPoint[ 0 ]->tx );
    float yCenter( (double)pVPoint[ 0 ]->ty );

    for( int i = 1; i < vertCount; ++i )
    {
        // Find the closest Z point
        if( pVPoint[ i ]->tz > farPoint )
            farPoint = pVPoint[ i ]->tz;

        // Add up the centers
        xCenter += pVPoint[ i ]->tx;
        yCenter += pVPoint[ i ]->ty;
    }

    //////////////////////////////////////////
    // Test against the X side of the frustum
    //////////////////////////////////////////

    // Calculate the difference between the portal and the polys
    float diff = farPoint / pPortalCenter->tz;

    float portalRadius = portRadius * diff;
    float portalCenter = pPortalCenter->tx * diff;

    // Calculate the center
	// below = xcenter /= vertCount
    xCenter *= inverseVertCount;

    if( (xCenter + radius) < portalCenter - portalRadius )
        return false;
    
    if( (xCenter - radius) > portalCenter + portalRadius )
        return false;

    //////////////////////////////////////////
    // Test against the Y side of the frustum
    //////////////////////////////////////////

    portalCenter = pPortalCenter->ty * diff;

    // Calculate the center
	// below = yCenter /= vertCount
    yCenter *= inverseVertCount;

    if( yCenter + radius < portalCenter - portalRadius )
        return false;

    if( yCenter - radius > portalCenter + portalRadius )
        return false;
    
    return visible;

}   // IsVisable


/************************************************************************
*    DESCRIPTION:  Is the supplied vertice the same
*                          
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
*    DESCRIPTION:  Load polygon data from file
*
*    Input:    FILE *hFile - File handle
*              CPoint *vertLst - Vertex list object
*
*    Output:   bool - true on success, false on error
*
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
	inverseVertCount = 1.0 / (float)vertCount;

    // Copy the type
    collisionType = (ECT_COLLISION_TYPE)faceLoad.collisionType;
    
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
*    DESCRIPTION:  Save polygon data from file
*
*    Input:    FILE *hFile - File handle
*              int vertexCount - Number of vertexes in this polygon
*              CPoint *vertLst - pointer to vertex list
*
*    Output:   bool - true on success, false on error
*
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
*    DESCRIPTION:  Copy the supplied polygon
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - File handle
*              int vertexCount - Number of vertexes in this polygon
*              CPoint *vertLst - pointer to vertex list
*
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
	inverseVertCount = 1.0 / (float)vertCount;

    // Copy the type
    collisionType = poly.GetCollisionType();

    // Copy the radius
    radius = poly.GetRadius();

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
*    DESCRIPTION:   Load extra polygon data. Virtual call so that
*                   an inherited class can load up it's own polygon
*                   class data.
*
*    Input:    FILE *hFile - File handle
*
*    Output:   bool - true on success, false on error
*
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
*    DESCRIPTION:  Save polygon data. Virtual call so that
*                  an inherited class can save it's own polygon
*                  class data.
*
*    Input:    FILE *hFile - File handle
*
*    Output:   bool - true on success, false on error
*
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
*    DESCRIPTION:  Convert the normal to it's pure self
*
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
*    DESCRIPTION:  Convert the normal to it's pure self
*
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
*    DESCRIPTION:  Save all the normals to a file.
*
*    Input:    FILE *hFile - File handle
*
*    Output:   bool - true on success, false on error
*
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
*    DESCRIPTION:  Load all the normals from file. Helpful for
*                  quick loading of large data sets.
*
*    Input:    FILE *hFile - File handle
*
*    Output:   bool - true on success, false on error
*
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
*    DESCRIPTION:  Set an ambient light level for this polygon.
*
*    Input:    float red - red channel color
*              float green - green channel color
*              float green - green channel color
*
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


/************************************************************************
*    DESCRIPTION: Adjust the radius depending on the need  
*
*    Input:  float scaler - Amount to scale the radius by
*            ECT_COLLISION_TYPE colType -  collision type looking for
*
*    -------------------------------------------------------------------
*    H Rosenorn    05/01/06      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void CPolygon::AdjustRadius( float scaler, ECT_COLLISION_TYPE colType )
{
	if( collisionType == colType )
    {
        radius *= scaler;
		radiusX *= scaler;
		radiusY *= scaler;
    }
}	// AdjustRadius


/************************************************************************
*    DESCRIPTION:  do we have a match
*
*    Input:    CPolygon &polygon - polygon being tested
*
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CPolygon::operator == ( CPolygon &polygon )
{
    // first check the vert count
	if( vertCount != polygon.GetVertexCount() )
	{
		return false;
	}

    for( int i = 0; i < vertCount; ++i )
	{
		if( *pVPoint[i] != *polygon.GetVPoint(i) )
		{
			return false;
		}
	}

    return true;
}