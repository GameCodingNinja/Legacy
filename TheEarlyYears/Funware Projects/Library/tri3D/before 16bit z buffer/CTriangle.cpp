/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CTriangle.cpp
*
*    DESCRIPTION:     Triangle classes (or 3 point polygon class)
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
#include "CTriangle.h"       // Header file for this *.cpp file.
#include "GDefines.h"        // Header file with #defines
#include <Math.H>
#include <stdlib.h>

// Globals
extern float gfCameraScaleX;
extern float gfCameraScaleY;
extern unsigned int *gdVBufer;
extern int giBufWidth, giBufHeight;
extern float gfBufWidthDiv2, gfBufHeightDiv2;
extern int *giZBuffer;
extern float gfMaxZDistance;
extern float gfMinZDistance;
extern unsigned short int *guiShadeTbl;


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    08/20/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CTriangle::CTriangle()
{
    // init class members
    projCount = 0;

    // default to the first texture
    tIndex = 0;

}   // Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    08/20/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CTriangle::~CTriangle()
{

}   // Destructer


/************************************************************************
*    FUNCTION NAME:         CalcLightInten()
*
*    DESCRIPTION:           Calculate light intensity
*
*    FUNCTION PARAMETERS:
*    Input:    CLightList *pLightList - light list
*              int intensityCap - Dependant on color bit count
*              int intensityShift - Dependant on color bit count
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall CTriangle::CalcLightInten( CLightList *pLightList, int intensityCap, int intensityShift )
{
    int vertInten, i[3]={0,0,0};
    double cosAngle, dist;

    for( int lite = 0; lite < pLightList->count; ++lite )
    {
        for( int vIndex = 0; vIndex < 3; ++vIndex )
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
                    cosAngle = pLightList->light[lite].CalAngleToSurfaceSt( uvi[ vIndex ].n, pVPoint[vIndex] );
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
                if( pLightList->light[lite].CalDirToSurfaceSt( uvi[ vIndex ].n ) > 0 )
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
                i[ vIndex ] += pLightList->light[lite].intensity;
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
                    i[ vIndex ] += pLightList->light[lite].intensity;
                }
            }

            // Cap the top and bottom of the intensity because it
            // can't be greater then 255 or less then 0. The color will
            // wrap around or do other nasty things.
            if( vertInten > 31 )
                vertInten = 31;
            else if( vertInten < 0 )
                vertInten = 0;

            // Only use if we have an intensity
            if( vertInten > 0 )
            {
                // record the color intensity for this vertex
                //i[ vIndex ] += (pLightList->light[lite].intensity * vertInten) >> 5;
				i[ vIndex ] += vertInten;
            }
        }
    }

    // Combine all the intensities
    for( int vIndex = 0; vIndex < 3; ++vIndex )
    {
        // record the color intensity for this vertex
        if( i[ vIndex ] > 31 )
            uvi[ vIndex ].i = 31;
        else
            uvi[ vIndex ].i = i[ vIndex ];
    }


}  // CalcLightInten


/************************************************************************
*    FUNCTION NAME:         ClipProjectXYZ()                                                             
*
*    DESCRIPTION:           Clip the triangle to the near Z plain but only
*                           project points that have not been projected.
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
*    H Rosenorn    08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall CTriangle::ClipProjectXYZ()
{
    int outC(0), startI, endI;
    double deltaZ, p;
    CPoint3D tmp3D;
    
    // Reset the projection count
    projCount = 3; 

    // Initialize pointer to last vertex:
    startI = projCount - 1;

    // Loop through all edges of panel using S&H algorithm:
    for( endI = 0; endI < projCount; ++endI )
    {
        if( pVPoint[ startI ]->tz >= gfMinZDistance )
        {
            // (case 1) Entirely inside front view volume - output unchanged vertex
            if( pVPoint[ endI ]->tz >= gfMinZDistance )
            {                
                // See if this point hasn't been projected yet.
                // If the point hasn't been projected, project the point
                if( !*pfProjected[endI] )
                    Project( endI );

                // Copy over the projected points
                point2D[outC].x = pVPoint[endI]->px;
                point2D[outC].y = pVPoint[endI]->py;
                point2D[outC].z = pVPoint[endI]->oneOverZfixPoint32bit;

                // Copy over the data. We'll do the calculations later
                point2D[outC].oneOverZ = pVPoint[endI]->oneOverZ;
                point2D[outC].endI = endI;
                point2D[outC].fClipped = false;

                // Update index:
                ++outC;
            }

            // (case 2) Point is leaving view volume -
            // clip using parametric form of line:
            else
            {    
                // Equasion: p = (Z2 - gfMinZDistance)/(Z2 - Z1)
                // p is the calculated percentage of the lines length
                // that lies outside the boundary of permissible values (gfMinZDistance).
                deltaZ = pVPoint[ endI ]->tz - pVPoint[ startI ]->tz;                
                p = ( gfMinZDistance - pVPoint[ startI ]->tz ) / deltaZ;

                // Clip the new X
                tmp3D.tx = pVPoint[ startI ]->tx + ( double )
                           ( pVPoint[ endI ]->tx - pVPoint[ startI ]->tx ) * p;

                // Clip the new Y
                tmp3D.ty = pVPoint[ startI ]->ty + ( double )
                           ( pVPoint[ endI ]->ty - pVPoint[ startI ]->ty ) * p;

                // The new Z is the Near Clipping Plain
                tmp3D.tz = gfMinZDistance;

                // Project the clipped point
                Project( outC, tmp3D );

                // Copy over the data. We'll do the calculations later
                point2D[outC].endI = endI;
                point2D[outC].startI = startI;
                point2D[outC].p = p;
                point2D[outC].fClipped = true;

                // Update index
                ++outC;
            }
        }
        
        // (case 3) SPoint is entering view volume - 
        // clip using parametric form of line:   
        else
        {
            if( pVPoint[ endI ]->tz >= gfMinZDistance ) 
            {
                // Equasion: p = (Z2 - gfMinZDistance)/(Z2 - Z1)
                // p is the calculated percentage of the lines length
                // that lies outside the boundary of permissible values (gfMinZDistance).
                deltaZ = pVPoint[ endI ]->tz - pVPoint[ startI ]->tz;
                p = ( gfMinZDistance - pVPoint[ startI ]->tz ) / deltaZ;

                // Clip the new X
                tmp3D.tx = pVPoint[ startI ]->tx + ( double )
                           ( pVPoint[ endI ]->tx - pVPoint[ startI ]->tx ) * p;

                
                // Clip the new Y
                tmp3D.ty = pVPoint[ startI ]->ty + ( double )
                           ( pVPoint[ endI ]->ty - pVPoint[ startI ]->ty ) * p;

                // The new Z is the Near Clipping Plain
                tmp3D.tz = gfMinZDistance;

                // Project the clipped point
                Project( outC, tmp3D );

                // Copy over the data. We'll do the calculations later
                point2D[outC].endI = endI;
                point2D[outC].startI = startI;
                point2D[outC].p = p;
                point2D[outC].fClipped = true;

                // Update index
                ++outC;

                // See if this point hasn't been projected yet.
                // If the point hasn't been projected, project the point
                if( !*pfProjected[endI] )
                    Project( endI );

                // Copy over the projected points
                point2D[outC].x = pVPoint[endI]->px;
                point2D[outC].y = pVPoint[endI]->py;
                point2D[outC].z = pVPoint[endI]->oneOverZfixPoint32bit;

                // Copy over the data. We'll do the calculations later
                point2D[outC].oneOverZ = pVPoint[endI]->oneOverZ;
                point2D[outC].endI = endI;
                point2D[outC].fClipped = false;

                // Update index
                ++outC;                
            }
        }
           
        // Advance to next vertex
        startI = endI;
    }

    // Store the number of vertices in outC
    projCount = outC;

}   // ClipProjectXYZ


/************************************************************************
*    FUNCTION NAME:         Project()                                                             
*
*    DESCRIPTION:           Project the 3D points to 2D screen points to
*                           the points 2D class so that the projection
*                           can be reused and not recalculated.
*
*    FUNCTION PARAMETERS:
*    Input:    int index - Index of point
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CTriangle::Project( int endI )
{
    // Convert the Z to 1/Z. This allows us to multiply
    // the Z value vs. dividing it. Multiplying it is much faster.
    // Also Z is not linear in screen space and 1/Z is. 1/Z is also
    // needed for "Prespective-Correct" texture mapping.
    double oneOverZ( 1.0F / pVPoint[endI]->tz );

    // Calculate the pixel coordinates
    pVPoint[endI]->px = ((pVPoint[endI]->tx * oneOverZ) * gfCameraScaleX) + gfBufWidthDiv2;
    pVPoint[endI]->py = ((pVPoint[endI]->ty * oneOverZ) * gfCameraScaleY) + gfBufHeightDiv2;
    pVPoint[endI]->oneOverZ = oneOverZ;
    pVPoint[endI]->oneOverZfixPoint32bit = oneOverZ * FIX_SHIFT_26_FLOAT;

    // Set the flag to indicate this point was projected
    // It can now be reused
    *pfProjected[endI] = true;

}   // Project


/************************************************************************
*    FUNCTION NAME:         Project()                                                             
*
*    DESCRIPTION:           Project the 3D points to 2D screen points to
*                           the 2D point class. If a point has been clipped,
*                           it is always projected and never reused because
*                           of the possibility of an additional point added
*                           durring the clipping process.
*
*    FUNCTION PARAMETERS:
*    Input:    int index - Index of point
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CTriangle::Project( int outC, CPoint3D &clippedPoint )
{
    // Convert the Z to 1/Z. This allows us to multiply
    // the Z value vs. dividing it. Multiplying it is much faster.
    // Also Z is not linear in screen space and 1/Z is. 1/Z is also
    // needed for "Prespective-Correct" texture mapping.
    double oneOverZ( 1.0F / clippedPoint.tz );

    // Calculate the pixel coordinates
    point2D[outC].x = ((clippedPoint.tx * oneOverZ) * gfCameraScaleX) + gfBufWidthDiv2;
    point2D[outC].y = ((clippedPoint.ty * oneOverZ) * gfCameraScaleY) + gfBufHeightDiv2;
    point2D[outC].z = oneOverZ * FIX_SHIFT_26_FLOAT;
    point2D[outC].oneOverZ = oneOverZ;

}   // Project


/************************************************************************
*    FUNCTION NAME:         ClipProjectUVI()                                                             
*
*    DESCRIPTION:           Project the UVI
*
*    FUNCTION PARAMETERS:
*    Input:    int index - Index of point
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall CTriangle::ClipProjectUVI()
{
    for( int i = 0; i < projCount; ++i )
    {
        // Create a local copy for speed
        int endI( point2D[i].endI );
        
        if( point2D[i].fClipped )
        {
            // Create a local copy for speed
            int startI( point2D[i].startI );
            double p( point2D[i].p );

            // Calculate new U value:
            float U (( double ) uvi[ startI ].u + ( double ) 
                ( uvi[ endI ].u - uvi[ startI ].u ) * p);

            // Calculate new V value:
            float V (( double ) uvi[ startI ].v + ( double ) 
                ( uvi[ endI ].v - uvi[ startI ].v ) * p);

            // Calculate new color intensity value
            int I (( double ) uvi[ startI ].i + ( double ) 
                ( uvi[ endI ].i - uvi[ startI ].i ) * p);

            // The 1/Z is needed for perspective calculations
            point2D[i].U = (U * point2D[i].oneOverZ) * FIX_SHIFT_26_FLOAT;
            point2D[i].V = (V * point2D[i].oneOverZ) * FIX_SHIFT_26_FLOAT; 

            // Gouraud shading doesn't use perspective. It's straight interpolation.
            //point2D[i].I = I * FIX_SHIFT_16_FLOAT;
            point2D[i].I = I << FIX_SHIFT_15_INT;
        }
        else
        {
            // The 1/Z is needed for perspective calculations
            point2D[i].U = (uvi[endI].u * point2D[i].oneOverZ) * FIX_SHIFT_26_FLOAT;
            point2D[i].V = (uvi[endI].v * point2D[i].oneOverZ) * FIX_SHIFT_26_FLOAT; 

            // Gouraud shading doesn't use perspective. It's straight interpolation.
            point2D[i].I = uvi[endI].i << FIX_SHIFT_15_INT;
        }
    }

}   // ClipProjectUVI


/************************************************************************
*    FUNCTION NAME:         ClipProjectI()                                                             
*
*    DESCRIPTION:           Project the I
*
*    FUNCTION PARAMETERS:
*    Input:    int index - Index of point
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall CTriangle::ClipProjectI()
{
    for( int i = 0; i < projCount; ++i )
    {
        // Has this been clipped?        
        if( point2D[i].fClipped )
        {
            // Create a local copy for speed
            int startI( point2D[i].startI );

            int I( ( double ) uvi[ startI ].i + ( double ) 
                ( uvi[ point2D[i].endI ].i - uvi[ startI ].i ) * point2D[i].p ); 

            // Gouraud shading doesn't use perspective. It's straight interpolation.
            point2D[i].I = I << FIX_SHIFT_15_INT;
        }
        else
        {
            // Gouraud shading doesn't use perspective. It's straight interpolation.
            point2D[i].I = uvi[ point2D[i].endI ].i << FIX_SHIFT_15_INT;
        }
    }

}   // ClipProjectI


/************************************************************************
*    FUNCTION NAME:         CalcSurfaceNormal()                                                             
*
*    DESCRIPTION:           Calculate the Surface Normal which is later 
*                           normalized.
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
*    H Rosenorn    10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CTriangle::CalcSurfaceNormal()
{
    long double X1, Y1, Z1, X2, Y2, Z2;
    long double X3, Y3, Z3, x, y, z, length;

    X1 = pVPoint[ 0 ]->x;
    Y1 = pVPoint[ 0 ]->y;
    Z1 = pVPoint[ 0 ]->z;

    X2 = pVPoint[ 1 ]->x;
    Y2 = pVPoint[ 1 ]->y;
    Z2 = pVPoint[ 1 ]->z;

    X3 = pVPoint[ 2 ]->x;
    Y3 = pVPoint[ 2 ]->y;
    Z3 = pVPoint[ 2 ]->z;

    // Use plane equation to determine the orientation of the surface
    x = (float)(Y1 * ( Z2 - Z3 ) + Y2 * ( Z3 - Z1 ) + Y3 * ( Z1 - Z2 ));
    y = (float)(Z1 * ( X2 - X3 ) + Z2 * ( X3 - X1 ) + Z3 * ( X1 - X2 ));
    z = (float)(X1 * ( Y2 - Y3 ) + X2 * ( Y3 - Y1 ) + X3 * ( Y1 - Y2 ));

    // Calculate the length of the vector
    length = sqrtl( (x*x) + (y*y) + (z*z) );

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
        pNormal->tx = pNormal->x = (float)(( x / length ) + pVPoint[ 0 ]->x);
        pNormal->ty = pNormal->y = (float)(( y / length ) + pVPoint[ 0 ]->y);
        pNormal->tz = pNormal->z = (float)(( z / length ) + pVPoint[ 0 ]->z);
    }
    else
    {
        pNormal->tx = pNormal->x = pVPoint[ 0 ]->x;
        pNormal->ty = pNormal->y = pVPoint[ 0 ]->y;
        pNormal->tz = pNormal->z = pVPoint[ 0 ]->z;
    }

}   // CalcSurfaceNormal


/************************************************************************
*    FUNCTION NAME:         CalcBoundingTri()                                                             
*
*    DESCRIPTION:           Calculate a bounding sphere around the triangle.
*                           Calculating a bounding sphere around a mesh is
*                           a lot simpler because usually the center is 0,0,0
*                           because it makes it easer to translate it in the
*                           world. You just simply search for the farthest
*                           point in the mesh. Calculating a radius around
*                           a single triangle is a little more involved because
*                           you have to revert the triangle to it's center point.
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

void CTriangle::CalcBoundingTri()
{
    CPoint offsetPt;
    long double curDistance;
    long double maxDistance = 0.0;
    CPoint centerPt;

    // Average out each vertex
    // Multiplication is faster then division. This is why we do
    // * 0.33333333 instead of / 3
    offsetPt.x = (pVPoint[0]->tx + pVPoint[1]->tx + pVPoint[2]->tx) * 0.33333333;
    offsetPt.y = (pVPoint[0]->ty + pVPoint[1]->ty + pVPoint[2]->ty) * 0.33333333;
    offsetPt.z = (pVPoint[0]->tz + pVPoint[1]->tz + pVPoint[2]->tz) * 0.33333333;

    // Find the largest distance
    for( int i = 0; i < 3; ++i )
    {
        // Calculate a new position where the center is 0,0,0
        centerPt.x = pVPoint[i]->x - offsetPt.x;
        centerPt.y = pVPoint[i]->y - offsetPt.y;
        centerPt.z = pVPoint[i]->z - offsetPt.z;
        
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

}   // CalcBoundingTri


/************************************************************************
*    FUNCTION NAME:         CalcVisible3D()                                                             
*
*    DESCRIPTION:           Check if the poly is visible. Perform 3D culling
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
*    H Rosenorn    10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool _fastcall CTriangle::CalcVisible3D( float cullPercent )
{
    bool visible;

    // Eliminate triangle if it is behind us
    if( pVPoint[ 0 ]->tz > gfMinZDistance )
        visible = true;
    else if( pVPoint[ 1 ]->tz > gfMinZDistance )
        visible = true;
    else if( pVPoint[ 2 ]->tz > gfMinZDistance )
        visible = true;
    else
        return false;

    // Eliminate the triangle is too far ahead to see
    if( pVPoint[ 0 ]->tz < gfMaxZDistance )
        visible = true;
    else if( pVPoint[ 1 ]->tz < gfMaxZDistance )
        visible = true;
    else if( pVPoint[ 2 ]->tz < gfMaxZDistance )
        visible = true;
    else
        return false;


    // Test x & y sides of the view frustrum

    // Find the farthest Z point
    float farPoint( pVPoint[ 0 ]->tz );

    if( pVPoint[ 1 ]->tz > farPoint )
        farPoint = pVPoint[ 1 ]->tz;

    if( pVPoint[ 2 ]->tz > farPoint )
        farPoint = pVPoint[ 2 ]->tz;

    // Do the pre-calc
    float z_test = farPoint * cullPercent;

    // Calculate the x center
    float xCenter( (pVPoint[ 0 ]->tx + pVPoint[ 1 ]->tx + pVPoint[ 2 ]->tx) * 0.33333333 );  
    
    if( xCenter - radius > z_test )
        return false;
    
    if( xCenter + radius < -z_test )
        return false;
 
    z_test = farPoint * (cullPercent - 0.1);

    // Calculate the y center
    float yCenter( (pVPoint[ 0 ]->ty + pVPoint[ 1 ]->ty + pVPoint[ 2 ]->ty) * 0.33333333 );
    
    if( yCenter - radius > z_test )
        return false;

    if( yCenter + radius < -z_test )
        return false;

    // Check to see if the back is facing us
    double direction = (pVPoint[ 0 ]->tx * ( pNormal->tx - pVPoint[ 0 ]->tx )) + 
                       (pVPoint[ 0 ]->ty * ( pNormal->ty - pVPoint[ 0 ]->ty )) + 
                       (pVPoint[ 0 ]->tz * ( pNormal->tz - pVPoint[ 0 ]->tz ));

    if( direction > 0.0F )
        visible = false;
 

    return visible;

}   // CalcVisible3D


/************************************************************************
*    FUNCTION NAME:         CalcVisible2D()                                                             
*
*    DESCRIPTION:           Perform 2D culling
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
*    H Rosenorn    10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool _fastcall CTriangle::CalcVisible2D()
{
    bool result(false);

    // Make sure the panel has more than two points
    if( projCount > 2 )
    {
        int xMinInVis(0), xMaxInVis(0), yMinInVis(0), yMaxInVis(0);

        // Determine location of panel's 2D points
        for( int i = 0; i < projCount; ++i )
        {
            if( point2D[ i ].x < 0 )
                ++xMinInVis;

            if( point2D[ i ].x > giBufWidth )
                ++xMaxInVis;

            if( point2D[ i ].y < 0 )
                ++yMinInVis;

            if( point2D[ i ].y > giBufHeight )
                ++yMaxInVis;
        }

        if( projCount > xMinInVis && projCount > yMinInVis && 
            projCount > xMaxInVis && projCount > yMaxInVis )
            result = true;
    }

    return result;

}   // CalcVisible2D


/************************************************************************
*    FUNCTION NAME:         Render16_WireFrame()
*
*    DESCRIPTION:           Outline the polygon
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
*    H Rosenorn    08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall CTriangle::Render16_WireFrame( PCTextureLib pTextLib )
{
    int xSlope, ySlope, xStart, yStart, xInc, yInc, error, index, i;
    int yOffset, xOffset, yMax(giBufWidth * (giBufHeight-1)), xMax(giBufWidth), offset;
    unsigned short int register *pVidBuf = (unsigned short int *)gdVBufer;
    unsigned short int register color = pTextLib->pTMap[tIndex].pImage16[130];
    unsigned short int register *pShade = guiShadeTbl;
    int register *ZPtr = giZBuffer;
    int zStart, Z, stepZ;
    int I, stepI;

    for( i = 0; i < projCount; ++i )
    {
        // Init
        error = 0;
        
        // Set the start offsets
        xStart = point2D[ i ].x;
        yStart = point2D[ i ].y;
        zStart = point2D[ i ].z;
        I = point2D[ i ].I;

        // Compute the slope                         
        xSlope = point2D[ (i+1) % projCount ].x - xStart;
        ySlope = point2D[ (i+1) % projCount ].y - yStart;

        // init the offsets
        yOffset = yStart * xMax;
        xOffset = xStart;
        Z = zStart;

        // test the X direction of the slope
        if( xSlope >= 0 )
        {
            // line is moving right
            xInc = 1;
        }
        else
        {
            // line is moving left
            xInc = -1;

            // need an absolute value
            xSlope = -xSlope;
        }

        // test Y direction of slope
        if( ySlope >= 0 )
        {
            // line is moving down
            yInc = xMax;
        }
        else
        {           
            // line is moving up
            yInc = -xMax;

            // need an absolute value
            ySlope = -ySlope;
        }

        // Based on which slope is greater we can plot the path
        if( xSlope > ySlope )
        {
            // Calculate Z slope
            stepZ = (point2D[ (i+1) % projCount ].z - zStart) / xSlope;

            // calculate the slope of the intensity
            stepI = (point2D[ (i+1) % projCount ].I - I) / xSlope;
                   
            for( index = 0; index < xSlope; ++index )
            {
                // Clip the line
                if( yOffset > -1 && yOffset < yMax &&
                    xOffset > -1 && xOffset < xMax )
                {
                    // Calculate the offset into the buffer
                    offset = yOffset + xOffset; 

                    if( ZPtr[offset] < Z )
                    {
                        // Set the pixel color
                        pVidBuf[offset] = pShade[ ( I & 0xFFFF8000 ) + color ];

                        // Update the Z buffer
                        ZPtr[offset] = Z;
                    }
                }

                // adjust the error factor
                error += ySlope;

                // test if error overflowed
                if( error > xSlope )
                {
                    // Reset the error factor
                    error -= xSlope;

                    // Inc the Y movement
                    yOffset += yInc;
                }

                // Inc the X movement
                xOffset += xInc;

                // Inc the Z movement
                Z += stepZ;

                // Inc the Intensity
                I += stepI;
            }
        }
        else
        {
            // Calculate Z slope
            stepZ = (point2D[ (i+1) % projCount ].z - zStart) / ySlope;

            // calculate the slope of the intensity
            stepI = (point2D[ (i+1) % projCount ].I - I) / ySlope;

            for( index = 0; index < ySlope; ++index )
            {
                // Clip the line
                if( yOffset > -1 && yOffset < yMax &&
                    xOffset > -1 && xOffset < xMax )
                {
                    // Calculate the offset into the buffer
                    offset = yOffset + xOffset; 

                    if( ZPtr[offset] < Z )
                    {
                        // Set the pixel color
                        pVidBuf[offset] = pShade[ ( I & 0xFFFF8000 ) + color ];

                        // Update the Z buffer
                        ZPtr[offset] = Z;
                    }
                }

                // adjust the error factor
                error += xSlope;

                // test if error overflowed
                if( error > 0 )
                {
                    // Reset the error factor
                    error -= ySlope;

                    // Inc the X movement
                    xOffset += xInc;
                }

                // Inc the Y movement
                yOffset += yInc;

                // Inc the Y movement
                Z += stepZ;

                // Inc the Intensity
                I += stepI;
            }
        }           
    }

}   // Render16_WireFrame


/************************************************************************
*    FUNCTION NAME:         Render16_ZBufShading()
*
*    DESCRIPTION:           Grey shading.
*
*    FUNCTION PARAMETERS:
*    Input:    PCTextureLib pTextLib - Texture library
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    08/15/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall CTriangle::Render16_ZBufShading( PCTextureLib pTextLib )
{
    CCeilLine leftSeg, rightSeg;
    int top(0), yIndex, height, xStart, xEnd, width;
    int newRightPos, newLeftPos, runLoops, length;
    unsigned short int register *DPtr, *pText, *pShade;
    int register *ZPtr;
    unsigned short int *tmpVBuf = (unsigned short int *)gdVBufer;
    int *tmpZBuf = giZBuffer;  
    int edgeCount(projCount);
    int ScrnW(giBufWidth), ScrnH(giBufHeight);
    int U, V, RZ, Z, stepU, stepV, stepZ, step, Tx1, Ty1, Tx2, Ty2, offset;
    int I, stepI;
    int subUStep, subVStep, subZStep, TxStep, TyStep, Tx, Ty;
    unsigned int shiftXaddition = FIX_SHIFT_23_INT - pTextLib->pTMap[tIndex].multiShift;
    unsigned char shiftXadditionByte = (unsigned char)shiftXaddition;
    pText = pTextLib->pTMap[tIndex].pImage16;
    pShade = guiShadeTbl;
    
    // Search for lowest Y coordinate (top of polygon)
    for( int n = 1; n < edgeCount; ++n )
    {
        if( point2D[ n ].y < point2D[ top ].y )
            top = n;
    }

    // This will initilize faster
    int leftPos(top), rightPos(top);

    // Calculate the index into the video buffer
    yIndex = point2D[ top ].y * ScrnW;

    // Loop for all polygon edges:
    while( edgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( rightSeg.edgeHeight < 1 )
        {
            newRightPos = rightPos + 1;

            if( newRightPos >= projCount )
                newRightPos = 0;

            // Init the right side of the triangle
            rightSeg.Init( point2D[ rightPos ], point2D[ newRightPos ] );

            // Move it to the next point on the polygon
            rightPos = newRightPos;
            --edgeCount;

            // Perform object-precision clip on top of edge 
            // Is the top edge of the polygon above the Y of the screen?
            // If so, recalculate the X offset and set the Y to 0
            if( rightSeg.Y < 0 )
            {
                // Can not be the same otherwise you'll get a divide by zero error
                if( rightSeg.y2 != rightSeg.y1 )
                    rightSeg.ClipTop();

                yIndex = 0;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( leftSeg.edgeHeight < 1 )
        {
            newLeftPos = leftPos - 1;

            if( newLeftPos < 0 )
                newLeftPos = ( projCount - 1 );

            // Init the left side of the triangle
            leftSeg.Init( point2D[ leftPos ], point2D[ newLeftPos ] );

            // Move it to the next point on the polygon
            leftPos = newLeftPos;
            --edgeCount;

            // Perform object-precision clip on top of edge. 
            // Is the top edge of the polygon above the Y of the screen?
            // If so, recalculate the X offset and set the Y to 0
            if( leftSeg.Y < 0 )
            {
                // Can not be the same otherwise you'll get a divide by zero error
                if( leftSeg.y2 != leftSeg.y1 )
                    leftSeg.ClipTop();

                yIndex = 0;
            }
        }

        // Subdivide polygon into trapezoid:
        // Will the left side or the right side define
        // the starting point of the Y
        if( leftSeg.edgeHeight < rightSeg.edgeHeight )
        {
            // Number of scan lines to fill
            height = leftSeg.edgeHeight;

            // Make sure the height and the segment offset
            // don't go off the bottom of the screen
            if( ( leftSeg.Y + height ) > ScrnH )
            {
                // Reset the height because it will exceed
                // the height of our buffer
                height = ScrnH - leftSeg.Y;

                // Even if there are more edges, we can't see
                // them so set the edge count to zero because 
                // this is that last render height we can do
                edgeCount = 0;
            }
        }
        else 
        {
            // Number of scan lines to fill
            height = rightSeg.edgeHeight;

            // Make sure the height and the segment offset
            // don't go off the bottom of the screen
            if( ( rightSeg.Y + height ) > ScrnH )
            {
                // Reset the height because it will exceed
                // the height of our buffer                
                height = ScrnH - rightSeg.Y;

                // Even if there are more edges, we can't see
                // them so set the edge count to zero because 
                // this is that last render height we can do
                edgeCount = 0;
            }
        }

        ////////////////////////////////////////////
        // The real work is done from this point on.
        // This is where we spend most of our time
        ////////////////////////////////////////////

        // Loop for the height of the trapezoid:
        while( height-- > 0 )
        {
            // Calculate initial values:
            xStart = (leftSeg.X >> FIX_SHIFT_20_INT);
            xEnd = (rightSeg.X >> FIX_SHIFT_20_INT);
            width = xEnd - xStart;

            // Make sure we are within the bounds of the screen
            if( ( width > 0 ) && ( xEnd > 0 ) && ( xStart < ScrnW ) )
            {
                U = leftSeg.U;
                V = leftSeg.V;
                Z = leftSeg.Z;
                I = leftSeg.I;

                // Create the step amounts for the scan line
                stepU = (rightSeg.U - U) / width;
                stepV = (rightSeg.V - V) / width;
                stepZ = (rightSeg.Z - Z) / width;
                stepI = (rightSeg.I - I) / width;
                  
                // Clip the scan-line
                if( xStart < 0 )
                {
                    step = -xStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += stepZ * step;
                    U += stepU * step;
                    V += stepV * step;
                    I += stepI * step;
                    
                    xStart = 0;
                    width = xEnd;
                }

                if( xEnd > ScrnW )
                {
                    xEnd = ScrnW;
                    width = xEnd - xStart;
                }

                // Init the offset
                offset = yIndex + xStart;
                DPtr = &tmpVBuf[offset];
                ZPtr = &tmpZBuf[offset];

                // Calculate subdivision loops
                // runLoops = width / 16
                runLoops = width >> DIVIDE_BY_16_SHIFT;

                if( runLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    subUStep = stepU << DIVIDE_BY_16_SHIFT;
                    subVStep = stepV << DIVIDE_BY_16_SHIFT;
                    subZStep = stepZ << DIVIDE_BY_16_SHIFT;
                }

                // Do the first calculations
                fixdiv( Tx1, U, Z, shiftXadditionByte );
                fixdiv( Ty1, V, Z, FIX_SHIFT_23_INT );

                ////////////////////////////////////////////
                // This is where we spend all of our time
                ////////////////////////////////////////////

                // Loop for number of subdivisions:
                for( int count = 0; count < runLoops; ++count )
                {
                    length = RUN_LENGTH_16;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:
                    RZ = Z;

                    // Jump ahead to next subdivision:
                    U  += subUStep;
                    V  += subVStep;
                    RZ += subZStep;

                    // Calculate the next point to interpulate
                    fixdiv( Tx2, U, RZ, shiftXadditionByte );
                    fixdiv( Ty2, V, RZ, FIX_SHIFT_23_INT );

                    // Divide by 16
                    TxStep = ( Tx2 - Tx1 ) >> DIVIDE_BY_16_SHIFT;
                    TyStep = ( Ty2 - Ty1 ) >> DIVIDE_BY_16_SHIFT;

                    Tx = Tx1;
                    Ty = Ty1;
                    
                    // Loop for width of scan-line
                    while( length > 0 )
                    {
                        --length;

                        if( *ZPtr < Z )
                        {
                            *ZPtr = Z;

                            *DPtr = pShade[ ( I & 0xFFFF8000 ) + pText[ (( Ty & SHIFT_23_MASK ) + Tx) >> shiftXaddition ]];                            
                        }

                        Z += stepZ;
                        I += stepI;
                        Tx += TxStep;
                        Ty += TyStep;
                        ++DPtr;
                        ++ZPtr; 
                    }
                    
                    // Reuse the last calculations as the first
                    Tx1 = Tx2;
                    Ty1 = Ty2;
                } 

                // Calculate remainder of scan-line left to rasterize 16 - 1
                length = width & (RUN_LENGTH_16-1);

                if( length > 0 )
                {
                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:
                    RZ = Z;

                    // Jump ahead to next subdivision:
                    U  += ( stepU * length );
                    V  += ( stepV * length );
                    RZ += ( stepZ * length );

                    // Calculate the next point to interpulate
                    fixdiv( Tx2, U, RZ, shiftXadditionByte );
                    fixdiv( Ty2, V, RZ, FIX_SHIFT_23_INT );

                    TxStep = ( Tx2 - Tx1 ) / length;
                    TyStep = ( Ty2 - Ty1 ) / length;

                    Tx = Tx1;
                    Ty = Ty1;
                    
                    // Loop for width of scan-line
                    while( length > 0 )
                    {
                        --length;

                        if( *ZPtr < Z )
                        {
                            *ZPtr = Z;

                            *DPtr = pShade[ ( I & 0xFFFF8000 ) + pText[ (( Ty & SHIFT_23_MASK ) + Tx) >> shiftXaddition ]];
                        }

                        Z += stepZ;
                        I += stepI;
                        Tx += TxStep;
                        Ty += TyStep;
                        ++DPtr;
                        ++ZPtr; 
                    }
                    
                    // Reuse the last calculations as the first
                    Tx1 = Tx2;
                    Ty1 = Ty2;
                }
            }

            rightSeg.X += rightSeg.stepX;
            ++rightSeg.Y;
            rightSeg.Z += rightSeg.stepZ;
            rightSeg.U += rightSeg.stepU;
            rightSeg.V += rightSeg.stepV;
            rightSeg.I += rightSeg.stepI;
            --rightSeg.edgeHeight;

            leftSeg.X += leftSeg.stepX;
            ++leftSeg.Y;
            leftSeg.Z += leftSeg.stepZ;
            leftSeg.U += leftSeg.stepU;
            leftSeg.V += leftSeg.stepV;
            leftSeg.I += leftSeg.stepI;
            --leftSeg.edgeHeight;

            yIndex += ScrnW;
        }
    }        
}   // Render16_ZBufShading


/************************************************************************
*    FUNCTION NAME:         Render16_LiteZBufShading()
*
*    DESCRIPTION:           light shading.
*
*    FUNCTION PARAMETERS:
*    Input:    unsigned short int color - Color to use
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall CTriangle::Render16_LiteZBufShading( unsigned short int color )
{
    CCeilLine leftSeg, rightSeg;
    int top(0), yIndex, height, xStart, xEnd, width;
    int newRightPos, newLeftPos;
    unsigned short int register *DPtr, *pShade;
    int register *ZPtr;
    unsigned short int *tmpVBuf = (unsigned short int *)gdVBufer;
    int *tmpZBuf = giZBuffer;  
    int edgeCount(projCount);
    int ScrnW(giBufWidth), ScrnH(giBufHeight);
    int Z, stepZ, step, offset;
    int I, stepI;
    pShade = guiShadeTbl;
    
    // Search for lowest Y coordinate (top of polygon)
    for( int n = 1; n < edgeCount; ++n )
    {
        if( point2D[ n ].y < point2D[ top ].y )
            top = n;
    }

    // This will initilize faster
    int leftPos(top), rightPos(top);

    // Calculate the index into the video buffer
    yIndex = point2D[ top ].y * ScrnW;

    // Loop for all polygon edges:
    while( edgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( rightSeg.edgeHeight < 1 )
        {
            newRightPos = rightPos + 1;

            if( newRightPos >= projCount )
                newRightPos = 0;

            // Init the right side of the triangle
            rightSeg.Init( point2D[ rightPos ], point2D[ newRightPos ] );

            // Move it to the next point on the polygon
            rightPos = newRightPos;
            --edgeCount;

            // Perform object-precision clip on top of edge 
            // Is the top edge of the polygon above the Y of the screen?
            // If so, recalculate the X offset and set the Y to 0
            if( rightSeg.Y < 0 )
            {
                // Can not be the same otherwise you'll get a divide by zero error
                if( rightSeg.y2 != rightSeg.y1 )
                    rightSeg.ClipTop();

                yIndex = 0;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( leftSeg.edgeHeight < 1 )
        {
            newLeftPos = leftPos - 1;

            if( newLeftPos < 0 )
                newLeftPos = ( projCount - 1 );

            // Init the left side of the triangle
            leftSeg.Init( point2D[ leftPos ], point2D[ newLeftPos ] );

            // Move it to the next point on the polygon
            leftPos = newLeftPos;
            --edgeCount;

            // Perform object-precision clip on top of edge. 
            // Is the top edge of the polygon above the Y of the screen?
            // If so, recalculate the X offset and set the Y to 0
            if( leftSeg.Y < 0 )
            {
                // Can not be the same otherwise you'll get a divide by zero error
                if( leftSeg.y2 != leftSeg.y1 )
                    leftSeg.ClipTop();

                yIndex = 0;
            }
        }

        // Subdivide polygon into trapezoid:
        // Will the left side or the right side define
        // the starting point of the Y
        if( leftSeg.edgeHeight < rightSeg.edgeHeight )
        {
            // Number of scan lines to fill
            height = leftSeg.edgeHeight;

            // Make sure the height and the segment offset
            // don't go off the bottom of the screen
            if( ( leftSeg.Y + height ) > ScrnH )
            {
                // Reset the height because it will exceed
                // the height of our buffer
                height = ScrnH - leftSeg.Y;

                // Even if there are more edges, we can't see
                // them so set the edge count to zero because 
                // this is that last render height we can do
                edgeCount = 0;
            }
        }
        else 
        {
            // Number of scan lines to fill
            height = rightSeg.edgeHeight;

            // Make sure the height and the segment offset
            // don't go off the bottom of the screen
            if( ( rightSeg.Y + height ) > ScrnH )
            {
                // Reset the height because it will exceed
                // the height of our buffer                
                height = ScrnH - rightSeg.Y;

                // Even if there are more edges, we can't see
                // them so set the edge count to zero because 
                // this is that last render height we can do
                edgeCount = 0;
            }
        }

        ////////////////////////////////////////////
        // The real work is done from this point on.
        // This is where we spend most of our time
        ////////////////////////////////////////////

        // Loop for the height of the trapezoid:
        while( height-- > 0 )
        {
            // Calculate initial values:
            xStart = (leftSeg.X >> FIX_SHIFT_20_INT);
            xEnd = (rightSeg.X >> FIX_SHIFT_20_INT);
            width = xEnd - xStart;

            // Make sure we are within the bounds of the screen
            if( ( width > 0 ) && ( xEnd > 0 ) && ( xStart < ScrnW ) )
            {
                Z = leftSeg.Z;
                I = leftSeg.I;

                // Create the step amounts for the scan line
                stepZ = (rightSeg.Z - Z) / width;
                stepI = (rightSeg.I - I) / width;
                  
                // Clip the scan-line
                if( xStart < 0 )
                {
                    step = -xStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += stepZ * step;
                    I += stepI * step;
                    
                    xStart = 0;
                    width = xEnd;
                }

                if( xEnd > ScrnW )
                {
                    xEnd = ScrnW;
                    width = xEnd - xStart;
                }

                // Init the offset
                offset = yIndex + xStart;
                DPtr = &tmpVBuf[offset];
                ZPtr = &tmpZBuf[offset];

                ////////////////////////////////////////////
                // This is where we spend all of our time
                ////////////////////////////////////////////
                    
                // Loop for width of scan-line
                while( width-- > 0 )
                {
                    if( *ZPtr < Z )
                    {
                        *ZPtr = Z;

                        *DPtr = pShade[ (I & 0xFFFF8000) + color ];                            
                    }

                    Z += stepZ;
                    I += stepI;
                    ++DPtr;
                    ++ZPtr; 
                }
            }

            rightSeg.X += rightSeg.stepX;
            ++rightSeg.Y;
            rightSeg.Z += rightSeg.stepZ;
            rightSeg.I += rightSeg.stepI;
            --rightSeg.edgeHeight;

            leftSeg.X += leftSeg.stepX;
            ++leftSeg.Y;
            leftSeg.Z += leftSeg.stepZ;
            leftSeg.I += leftSeg.stepI;
            --leftSeg.edgeHeight;

            yIndex += ScrnW;
        }
    }        
}   // Render16_LiteZBufShading