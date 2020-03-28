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

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    DESCRIPTION:  Constructer                                                             
************************************************************************/
CPolygon::CPolygon()
{
    // Init class members
    vertCount = TRI;
	inverseVertCount = 1.0 / (float)vertCount;
    tIndex = 0;
    collisionType = ECT_NO_TYPE;
    collideRadius = 0;
	viewRadius = 0;
    pNormal = NULL;
    projCount = 0;

}   // Constructer


/************************************************************************
*    DESCRIPTION:   Destructer                                                             
************************************************************************/
CPolygon::~CPolygon()
{

}   // Destructer


/************************************************************************
*    DESCRIPTION:  Calculate the Surface Normal that is then
*                  normalized. This is the normal of the polygon.
*                  This function assumes all vertex points are unique.
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

    // this is to show how you can create the cross product with just two
    // points from the triangle face. Still works but not as precise.
    /*x = ( (Y1 * Z2) - (Z1 * Y2) );
    y = -( (Z1 * X2) - (X1 * Z2) );
    z = ( (X1 * Y2) - (Y1 * X2) );*/

    // Calculate the length of the vector
    double length( sqrt( (x*x) + (y*y) + (z*z) ) );

    // length can't be zero otherwise you'll get a divided by 0 error.

    // The normalizing part is the (n.x / length)
    if( length != 0.0 )
    {
        // Normalize the normal and make it a point. Sounds silly hu?
        pNormal->tx = pNormal->x = ( x / length );
        pNormal->ty = pNormal->y = ( y / length );
        pNormal->tz = pNormal->z = ( z / length );
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
************************************************************************/
void CPolygon::CalcBoundingPoly()
{
    CPoint pt;
    double curDistance;
    double maxDistance = 0.0;
    double maxDistanceX = 0.0;
    double maxDistanceY = 0.0;
    CPoint centerPt;

    // Get the center point of the untransformed polygon
    CalcPolyCenterPointSt( centerPt );
    
    // Find the largest distance
    for( int i = 0; i < vertCount; ++i )
    {
        // Calculate a new position where the center is 0,0,0
        pt = *pVPoint[i] - centerPt;
        
        // Get the length of this point
        curDistance = pt.GetLengthSt();

        // Have we found the longest distance?
        if( curDistance > maxDistance )
            maxDistance = curDistance;
    }

    // compute the radius by getting the vector length
    collideRadius = viewRadius = sqrt( maxDistance );

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
************************************************************************/
ECT_COLLISION_TYPE _fastcall CPolygon::Collide_BoundSphere( CPoint &pt, double &distance, float rad )
{
    // Don't collision check back facing polys
    if( pVPoint[0]->GetDotProductDy( *pNormal ) < 0.0F )
        return ECT_NO_COLLISION;

    // Get the center of the transformed polygon
	CPoint centerPt;
	CalcPolyCenterPointDy( centerPt );

    // Calculate the distance between the center points of both objects
	float dist = sqrt( centerPt.GetLengthDy( pt ) );

    // Check for possible collision by using bounding sphere
    if( dist <= rad + collideRadius )
    {
        // Record the distance
        distance = dist;

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
************************************************************************/
ECT_COLLISION_TYPE _fastcall CPolygon::Collide_BoundSphereSt( CPoint &pt, double &distance, float rad )
{
    // Don't collision check back facing polys
    if( pVPoint[0]->GetDotProductSt( *pNormal ) < 0.0F )
        return ECT_NO_COLLISION;

    // Get the center of the untransformed polygon
	CPoint centerPt;
	CalcPolyCenterPointSt( centerPt );

    // Calculate the distance between the center points of both objects
	float dist = sqrt( centerPt.GetLengthSt( pt ) );

    // Check for possible collision by using bounding sphere
    if( dist <= rad + collideRadius )
    {
        // Record the distance
        distance = dist;

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
************************************************************************/
void _fastcall CPolygon::CalcLightIntenDynamic( CLightList *pLightList )
{
    int vertInten, r[QUAD]={0,0,0,0}, g[QUAD]={0,0,0,0}, b[QUAD]={0,0,0,0};

    for( int lite = 0; lite < pLightList->Count(); ++lite )
    {
        for( int vIndex = 0; vIndex < vertCount; ++vIndex )
        {
            vertInten = 0;
            
            /////////////////////////////////////////////
            // point, infinite & spot Light Calculations
            /////////////////////////////////////////////
            if( pLightList->GetLight(lite )->GetLightType() >= ELT_INFINITE )
            {
                if( pLightList->GetLight(lite )->GetShadeType() == EShade_GOURAUD )
                {                                                                            
                    vertInten = pLightList->GetLight(lite )->CalVertIntenDy( uvrgb[ vIndex ].n, *pVPoint[vIndex] );
                }
                else if( pLightList->GetLight(lite )->GetShadeType() == EShade_FLAT && vIndex == 0 )
                {
                    vertInten = pLightList->GetLight(lite )->CalVertIntenDy( *pNormal, *pVPoint[vIndex] );                    
                }
            }
            ///////////////////////////////
            // Ambient Light Calculations
            /////////////////////////////// 
            else if( pLightList->GetLight(lite )->GetLightType() == ELT_AMBIENT )
            {
                // record the color for this vertex
                r[ vIndex ] += pLightList->GetLight(lite )->GetColorR();
                g[ vIndex ] += pLightList->GetLight(lite )->GetColorG();
                b[ vIndex ] += pLightList->GetLight(lite )->GetColorB();
            }
            ///////////////////////////////
            // Ambient Point Light Calculations
            /////////////////////////////// 
            else if( pLightList->GetLight(lite )->GetLightType() == ELT_AMBIENT_POINT )
            {
                // Is the light distance within range
                if( pLightList->GetLight(lite )->GetPos().GetDotProductDy( *pVPoint[vIndex] ) < pLightList->GetLight(lite )->GetRadius() )
                {
                    // record the color for this vertex
                    r[ vIndex ] += pLightList->GetLight(lite )->GetColorR();
                    g[ vIndex ] += pLightList->GetLight(lite )->GetColorG();
                    b[ vIndex ] += pLightList->GetLight(lite )->GetColorB();
                }
            }

            // Cap the top and bottom of the intensity because it
            // can't be greater then 255 or less then 0. The color will
            // wrap around or do other nasty things.
            if( vertInten > 255 )
                vertInten = 255;

            // Only use if we have an intensity
            if( vertInten > 0 )
            {
                // record the color intensity for this vertex
                r[ vIndex ] += (pLightList->GetLight(lite )->GetColorR() * vertInten) >> 8;
                g[ vIndex ] += (pLightList->GetLight(lite )->GetColorG() * vertInten) >> 8;
                b[ vIndex ] += (pLightList->GetLight(lite )->GetColorB() * vertInten) >> 8;
            }
        }
    }

    // Combine all the lights  (inten / 255.0f) 1/z conversion for fast lighting
    if( giRenderDevice == ERD_OPENGL )
    {
    	for( int vIndex = 0; vIndex < vertCount; ++vIndex )
        {
            // record the color intensity for this vertex
            if( r[ vIndex ] > 254 )
                uvrgb[ vIndex ].r = 1.0;
            else
                uvrgb[ vIndex ].r = (float)r[ vIndex ] * 0.0039215;

            if( g[ vIndex ] > 254 )
                uvrgb[ vIndex ].g = 1.0;
            else
                uvrgb[ vIndex ].g = (float)g[ vIndex ] * 0.0039215;

            if( b[ vIndex ] > 254 )
                uvrgb[ vIndex ].b = 1.0;
            else
                uvrgb[ vIndex ].b = (float)b[ vIndex ] * 0.0039215;
        }
    }
    else
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

}  // CalcLightIntenDynamic


/************************************************************************
*    DESCRIPTION:  Calculate light intensity with load-time
*                  normal and vertices
*
*    Input:    CLightList *pLightList - Light list object
************************************************************************/
void _fastcall CPolygon::CalcLightIntenStatic( CLightList *pLightList )
{
    int vertInten, r[QUAD]={0,0,0,0}, g[QUAD]={0,0,0,0}, b[QUAD]={0,0,0,0};

    for( int lite = 0; lite < pLightList->Count(); ++lite )
    {
        for( int vIndex = 0; vIndex < vertCount; ++vIndex )
        {
            vertInten = 0;
            
            /////////////////////////////////////////////
            // point, infinite & spot Light Calculations
            /////////////////////////////////////////////
            if( pLightList->GetLight(lite )->GetLightType() >= ELT_INFINITE )
            {
                if( pLightList->GetLight(lite )->GetShadeType() == EShade_GOURAUD )
                {                                                                            
                    vertInten = pLightList->GetLight(lite )->CalVertIntenSt( uvrgb[ vIndex ].n, *pVPoint[vIndex] );
                }
                else if( pLightList->GetLight(lite )->GetShadeType() == EShade_FLAT && vIndex == 0 )
                {
                    vertInten = pLightList->GetLight(lite )->CalVertIntenSt( *pNormal, *pVPoint[vIndex] );                    
                }
            }
            ///////////////////////////////
            // Ambient Light Calculations
            /////////////////////////////// 
            else if( pLightList->GetLight(lite )->GetLightType() == ELT_AMBIENT )
            {
                // record the color for this vertex
                r[ vIndex ] += pLightList->GetLight(lite )->GetColorR();
                g[ vIndex ] += pLightList->GetLight(lite )->GetColorG();
                b[ vIndex ] += pLightList->GetLight(lite )->GetColorB();
            }
            ///////////////////////////////
            // Ambient Point Light Calculations
            /////////////////////////////// 
            else if( pLightList->GetLight(lite )->GetLightType() == ELT_AMBIENT_POINT )
            {
                // Is the light distance within range
                if( pLightList->GetLight(lite )->GetPos().GetDotProductSt( *pVPoint[vIndex] ) < pLightList->GetLight(lite )->GetRadius() )
                {
                    // record the color for this vertex
                    r[ vIndex ] += pLightList->GetLight(lite )->GetColorR();
                    g[ vIndex ] += pLightList->GetLight(lite )->GetColorG();
                    b[ vIndex ] += pLightList->GetLight(lite )->GetColorB();
                }
            }

            // Cap the top and bottom of the intensity because it
            // can't be greater then 255 or less then 0. The color will
            // wrap around or do other nasty things.
            if( vertInten > 255 )
                vertInten = 255;

            // Only use if we have an intensity
            if( vertInten > 0 )
            {
                // record the color intensity for this vertex
                r[ vIndex ] += (pLightList->GetLight(lite )->GetColorR() * vertInten) >> 8;
                g[ vIndex ] += (pLightList->GetLight(lite )->GetColorG() * vertInten) >> 8;
                b[ vIndex ] += (pLightList->GetLight(lite )->GetColorB() * vertInten) >> 8;
            }
        }
    }

    // Combine all the lights  (inten / 255.0f) 1/z conversion for fast lighting
    if( giRenderDevice == ERD_OPENGL )
    {
    	for( int vIndex = 0; vIndex < vertCount; ++vIndex )
        {
            // record the color intensity for this vertex
            if( r[ vIndex ] > 254 )
                uvrgb[ vIndex ].r = 1.0;
            else
                uvrgb[ vIndex ].r = (float)r[ vIndex ] * 0.0039215;

            if( g[ vIndex ] > 254 )
                uvrgb[ vIndex ].g = 1.0;
            else
                uvrgb[ vIndex ].g = (float)g[ vIndex ] * 0.0039215;

            if( b[ vIndex ] > 254 )
                uvrgb[ vIndex ].b = 1.0;
            else
                uvrgb[ vIndex ].b = (float)b[ vIndex ] * 0.0039215;
        }
    }
    else
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

}  // CalcLightIntenStatic


/************************************************************************
*    DESCRIPTION:   Get the a specific point of the polygon
*
*    Input:    CPoint &pt - Point class to copy normal to
*              int index - index to copy form
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
************************************************************************/
void CPolygon::SetNormals( CPolygon &polygon )
{
	pNormal->tx = pNormal->x;
    pNormal->ty = pNormal->y;
    pNormal->tz = pNormal->z;

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
************************************************************************/
bool CPolygon::IsVisable()
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

    if( pVPoint[0]->GetDotProductDy( *pNormal ) < 0.0 )
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
        
    if( xCenter + viewRadius < farPointX )
        return false;
    
    if( xCenter - viewRadius > -farPointX )
        return false;

    //////////////////////////////////////////
    // Test against the Y side of the frustum
    //////////////////////////////////////////

    // Height is always half the distance
    float farPointY( farPoint * gfFrustrumYRatio );

    // Calculate the center
    // below = yCenter /= vertCount    
    yCenter *= inverseVertCount;

    if( yCenter + viewRadius < farPointY )
        return false;

    if( yCenter - viewRadius > -farPointY )
        return false;
   
    return visible;

}   // IsVisable


/************************************************************************
*    DESCRIPTION:   Check if the polygon is visible. Perform 3D culling
*                   This assumes a 45 degree view area.
************************************************************************/
bool CPolygon::IsShadowVisable()
{
    bool visible = true;

    /////////////////////////////////////////////////
    // Eliminate back facing polygons
    /////////////////////////////////////////////////

    if( pVPoint[0]->GetDotProductDy( *pNormal ) < 0.0 )
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

    /////////////////////////////////////////////////
    // Eliminate back facing polygons
    /////////////////////////////////////////////////

    if( pVPoint[ 0 ]->GetDotProductDy( *pNormal ) < 0.0F )
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

    if( (xCenter + viewRadius) < portalCenter - portalRadius )
        return false;
    
    if( (xCenter - viewRadius) > portalCenter + portalRadius )
        return false;

    //////////////////////////////////////////
    // Test against the Y side of the frustum
    //////////////////////////////////////////

    portalCenter = pPortalCenter->ty * diff;

    // Calculate the center
	// below = yCenter /= vertCount
    yCenter *= inverseVertCount;

    if( yCenter + viewRadius < portalCenter - portalRadius )
        return false;

    if( yCenter - viewRadius > portalCenter + portalRadius )
        return false;
    
    return visible;

}   // IsVisable


/************************************************************************
*    DESCRIPTION:  Is the supplied vertice the same
************************************************************************/
bool _fastcall CPolygon::HasVert( CPoint &P )
{
    for( int i = 0; i < vertCount; ++i )
    {
        if( ( *pVPoint[i] ) == P )
            return true;
    }

    return false;

}   // HasVert


/************************************************************************
*    DESCRIPTION:  Is the supplied vertice similar within a range
************************************************************************/
bool _fastcall CPolygon::HasCloseVert( CPoint &P, float range )
{
    for( int i = 0; i < vertCount; ++i )
    {
        if( pVPoint[i]->GetLengthSt( P ) < range )
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
    viewRadius = poly.GetViewRadius();
	collideRadius = poly.GetCollideRadius();

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
************************************************************************/
bool CPolygon::SaveExtraPolygonData( FILE *hFile )
{
    return true;

}   // SaveExtraPolygonData


/************************************************************************
*    DESCRIPTION:  Save all the normals to a file.
*
*    Input:    FILE *hFile - File handle
*
*    Output:   bool - true on success, false on error
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
*    DESCRIPTION:  Calculate the center point of the polygon
************************************************************************/
void CPolygon::CalcPolyCenterPointSt( CPoint &centerPt )
{
    // Average out each vertex
    for( int i = 0; i < vertCount; ++i )
    {
        centerPt.x += pVPoint[i]->x;
        centerPt.y += pVPoint[i]->y;
        centerPt.z += pVPoint[i]->z;
    }

    // below = centerPx.# /= vertCount
    centerPt.tx = centerPt.x *= inverseVertCount;
    centerPt.ty = centerPt.y *= inverseVertCount;
    centerPt.tz = centerPt.z *= inverseVertCount;

}	// CalcPolyCenterPoint


/************************************************************************
*    DESCRIPTION:  Calculate the center point of the polygon
************************************************************************/
void CPolygon::CalcPolyCenterPointDy( CPoint &centerPt )
{
    // Average out each vertex
    for( int i = 0; i < vertCount; ++i )
    {
        centerPt.x += pVPoint[i]->tx;
        centerPt.y += pVPoint[i]->ty;
        centerPt.z += pVPoint[i]->tz;
    }

    // below = centerPx.# /= vertCount
    centerPt.tx = centerPt.x *= inverseVertCount;
    centerPt.ty = centerPt.y *= inverseVertCount;
    centerPt.tz = centerPt.z *= inverseVertCount;

}	// CalcPolyCenterPoint


/************************************************************************
*    DESCRIPTION:  Set an ambient light level for this polygon.
*
*    Input:    float red - red channel color
*              float green - green channel color
*              float green - green channel color
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
************************************************************************/
void CPolygon::AdjustViewRadius( float scaler )
{
	viewRadius *= scaler;

}	// AdjustViewRadius


/************************************************************************
*    DESCRIPTION: Adjust the radius depending on the need  
*
*    Input:  float scaler - Amount to scale the radius by
************************************************************************/
void CPolygon::AdjustCollideRadius( float scaler )
{
	collideRadius *= scaler;

}	// AdjustCollideRadius


/************************************************************************
*    DESCRIPTION:  Get the average height of the polygon
************************************************************************/
float CPolygon::GetAverageHeightSt()
{
    CPoint tmpPoint;

    // Average out each vertex
    for( int i = 0; i < vertCount; ++i )
    {
        tmpPoint.y += pVPoint[i]->y;
    }

    // below = centerPx.# /= vertCount
    tmpPoint.y *= inverseVertCount;

	return tmpPoint.y;

}	// CalcPolyCenterPoint


/************************************************************************
*    DESCRIPTION:  do we have a match
*
*    Input:    CPolygon &polygon - polygon being tested
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