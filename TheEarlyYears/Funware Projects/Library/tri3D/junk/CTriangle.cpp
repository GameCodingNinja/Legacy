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
#include <Math.H>
#include <stdlib.h>

// Globals
extern float gfCameraScaleX;
extern float gfCameraScaleY;
extern unsigned long *gdVBufer;
extern int giBufWidth, giBufHeight;
extern float gfBufWidthDiv2, gfBufHeightDiv2;
extern unsigned long *gdZBuffer;
extern float gfMaxZDistance;
extern unsigned long *gdTexture;


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

CTriangle::CTriangle()
{
    // init class members
    projCount = 0;

    color = (rand() % 256);
    color |= (rand() % 256) << 16;
    color |= (rand() % 256) << 8;

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

CTriangle::~CTriangle()
{

}   // Destructer


/************************************************************************
*    FUNCTION NAME:         Clip()                                                             
*
*    DESCRIPTION:           Project the 3D points to 2D points
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
*                  08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CTriangle::ClipProject()
{
    int outC(0), startI, endI;
    double deltaZ, p;
    CPoint3D tmp3D;
    CUVI tmpuvi;
    
    // Reset the projection count
    projCount = 3; 

    // Initialize pointer to last vertex:
    startI = projCount - 1;

    // Loop through all edges of panel using S&H algorithm:
    for( endI = 0; endI < projCount; ++endI )
    {
        if( pVPoint[ startI ]->tz >= MINZ )
        {
            // (case 1) Entirely inside front view volume - output unchanged vertex
            if( pVPoint[ endI ]->tz >= MINZ )
            {                
                // See if this point hasn't been projected yet.
                // If the point hasn't been projected, project the point
                if( !pVPoint[endI]->fProjected )
                    Project( endI );

                // Copy over the projected points
                point2D[outC].x = pVPoint[endI]->px;
                point2D[outC].y = pVPoint[endI]->py;
                point2D[outC].z = pVPoint[endI]->oneOverZ;

                // The 1/Z is needed for perspective calculations
                point2D[outC].U = (behavior.uvi[endI].U * point2D[outC].z) * FIX_SHIFT_24;
                point2D[outC].V = (behavior.uvi[endI].V * point2D[outC].z) * FIX_SHIFT_24;

                // Gouraud shading doesn't use perspective. It's straight interpolation.
                point2D[outC].I = behavior.uvi[endI].I;

                // Update index:
                ++outC;
            }

            // (case 2) Point is leaving view volume -
            // clip using parametric form of line:
            else
            {    
                // Equasion: p = (Z2 - MINZ)/(Z2 - Z1)
                // p is the calculated percentage of the lines length
                // that lies outside the boundary of permissible values (MINZ).
                deltaZ = pVPoint[ endI ]->tz - pVPoint[ startI ]->tz;                
                p = ( MINZ - pVPoint[ startI ]->tz ) / deltaZ;

                // Clip the new X
                tmp3D.tx = pVPoint[ startI ]->tx + ( double )
                           ( pVPoint[ endI ]->tx - pVPoint[ startI ]->tx ) * p;

                // Clip the new Y
                tmp3D.ty = pVPoint[ startI ]->ty + ( double )
                           ( pVPoint[ endI ]->ty - pVPoint[ startI ]->ty ) * p;

                // The new Z is the Near Clipping Plain
                tmp3D.tz = MINZ;

                // Calculate new U value:
                tmpuvi.U = (( double ) behavior.uvi[ startI ].U + ( double ) 
                           ( behavior.uvi[ endI ].U - behavior.uvi[ startI ].U ) * p) * FIX_SHIFT_24;

                // Calculate new V value:
                tmpuvi.V = (( double ) behavior.uvi[ startI ].V + ( double ) 
                           ( behavior.uvi[ endI ].V - behavior.uvi[ startI ].V ) * p) * FIX_SHIFT_24;

                // Calculate new intensity value:
                tmpuvi.I = ( double ) behavior.uvi[ startI ].I + ( double ) 
                           ( behavior.uvi[ endI ].I - behavior.uvi[ startI ].I ) * p;

                // Project the clipped point
                Project( outC, tmp3D, tmpuvi );

                // Update index
                ++outC;
            }
        }
        
        // (case 3) SPoint is entering view volume - 
        // clip using parametric form of line:   
        else
        {
            if( pVPoint[ endI ]->tz >= MINZ ) 
            {
                // Equasion: p = (Z2 - MINZ)/(Z2 - Z1)
                // p is the calculated percentage of the lines length
                // that lies outside the boundary of permissible values (MINZ).
                deltaZ = pVPoint[ endI ]->tz - pVPoint[ startI ]->tz;
                p = ( MINZ - pVPoint[ startI ]->tz ) / deltaZ;

                // Clip the new X
                tmp3D.tx = pVPoint[ startI ]->tx + ( double )
                           ( pVPoint[ endI ]->tx - pVPoint[ startI ]->tx ) * p;

                
                // Clip the new Y
                tmp3D.ty = pVPoint[ startI ]->ty + ( double )
                           ( pVPoint[ endI ]->ty - pVPoint[ startI ]->ty ) * p;

                // The new Z is the Near Clipping Plain
                tmp3D.tz = MINZ;

                // Calculate new U value:
                tmpuvi.U = (( double ) behavior.uvi[ startI ].U + ( double ) 
                           ( behavior.uvi[ endI ].U - behavior.uvi[ startI ].U ) * p) * FIX_SHIFT_24;

                // Calculate new V value:
                tmpuvi.V = (( double ) behavior.uvi[ startI ].V + ( double ) 
                           ( behavior.uvi[ endI ].V - behavior.uvi[ startI ].V ) * p) * FIX_SHIFT_24;

                // Calculate new intensity value
                tmpuvi.I = ( double ) behavior.uvi[ startI ].I + ( double ) 
                           ( behavior.uvi[ endI ].I - behavior.uvi[ startI ].I ) * p;

                // Project the clipped point
                Project( outC, tmp3D, tmpuvi );

                // Update index
                ++outC;

                // See if this point hasn't been projected yet.
                // If the point hasn't been projected, project the point
                if( !pVPoint[endI]->fProjected )
                    Project( endI );

                // Copy over the projected points
                point2D[outC].x = pVPoint[endI]->px;
                point2D[outC].y = pVPoint[endI]->py;
                point2D[outC].z = pVPoint[endI]->oneOverZ;

                // The 1/Z is needed for perspective calculations
                point2D[outC].U = behavior.uvi[endI].U * point2D[outC].z;
                point2D[outC].V = behavior.uvi[endI].V * point2D[outC].z;

                // Gouraud shading doesn't use perspective. It's straight interpolation.
                point2D[outC].I = behavior.uvi[endI].I;

                // Update index
                ++outC;                
            }
        }
           
        // Advance to next vertex
        startI = endI;
    }

    // Store the number of vertices in outC
    projCount = outC;

}   // ClipProject


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
*                  08/20/03      Function Created
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
    pVPoint[endI]->px = ((pVPoint[endI]->tx * gfCameraScaleX) * oneOverZ) + gfBufWidthDiv2;
    pVPoint[endI]->py = ((pVPoint[endI]->ty * gfCameraScaleY) * oneOverZ) + gfBufHeightDiv2;
    pVPoint[endI]->oneOverZ = oneOverZ;

    // Set the flag to indicate this point was projected
    pVPoint[endI]->fProjected = true;

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
*                  08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CTriangle::Project( int outC, CPoint3D &clippedPoint, CUVI &clippedUVI )
{
    // Convert the Z to 1/Z. This allows us to multiply
    // the Z value vs. dividing it. Multiplying it is much faster.
    // Also Z is not linear in screen space and 1/Z is. 1/Z is also
    // needed for "Prespective-Correct" texture mapping.
    double oneOverZ( 1.0F / clippedPoint.tz );

    // Calculate the pixel coordinates
    point2D[outC].x = ((clippedPoint.tx * gfCameraScaleX) * oneOverZ) + gfBufWidthDiv2;
    point2D[outC].y = ((clippedPoint.ty * gfCameraScaleY) * oneOverZ) + gfBufHeightDiv2;
    point2D[outC].z = oneOverZ;

    // The 1/Z is needed for perspective calculations
    point2D[outC].U = clippedUVI.U * oneOverZ;
    point2D[outC].V = clippedUVI.V * oneOverZ;

    // Gouraud shading doesn't use perspective. It's straight interpolation.
    point2D[outC].I = clippedUVI.I;

}   // Project


/************************************************************************
*    FUNCTION NAME:         CalcNormal()                                                             
*
*    DESCRIPTION:           Calculate the normal of the triangle
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
   
void CTriangle::CalcNormal()
{
    long double X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3;
    long double Distance, A, B, C;

    X1 = pVPoint[ 0 ]->x;
    Y1 = pVPoint[ 0 ]->y;
    Z1 = pVPoint[ 0 ]->z;

    X2 = pVPoint[ 1 ]->x;
    Y2 = pVPoint[ 1 ]->y;
    Z2 = pVPoint[ 1 ]->z;

    X3 = pVPoint[ 2 ]->x;
    Y3 = pVPoint[ 2 ]->y;
    Z3 = pVPoint[ 2 ]->z;

    // Use plane equation to determine plane's orientation:
    A = Y1 * ( Z2 - Z3 ) + Y2 * ( Z3 - Z1 ) + Y3 * ( Z1 - Z2 );
    B = Z1 * ( X2 - X3 ) + Z2 * ( X3 - X1 ) + Z3 * ( X1 - X2 );
    C = X1 * ( Y2 - Y3 ) + X2 * ( Y3 - Y1 ) + X3 * ( Y1 - Y2 );

    // Get the distance to the vector:
    Distance = sqrt( (A*A) + (B*B) + (C*C) );

    // Normalize the normal to 1 and create a point:
    pNormal->x = ( A / Distance ) + X1;
    pNormal->y = ( B / Distance ) + Y1;
    pNormal->z = ( C / Distance ) + Z1;

}   // CalcNormal


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
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CTriangle::CalcVisible3D()
{
    bool visible;

    // Eliminate panel if it is behind us

    if( pVPoint[ 0 ]->tz > MINZ )
        visible = true;
    else if( pVPoint[ 1 ]->tz > MINZ )
        visible = true;
    else if( pVPoint[ 2 ]->tz > MINZ )
        visible = true;
    else
        visible = false;

    // Check if the panel is too far ahead to see
    if( visible )
    {
        float nearPoint( pVPoint[ 0 ]->tz );

        // Unroll the for loop
        if( pVPoint[ 1 ]->tz < nearPoint )
            nearPoint = pVPoint[ 1 ]->tz;

        if( pVPoint[ 2 ]->tz < nearPoint )
            nearPoint = pVPoint[ 2 ]->tz;

        // Panel is too far ahead to see
        if( nearPoint > gfMaxZDistance )
            visible = false;

        // Perform 3D culling:
        if( visible )
        {
            // Check to see if the back is facing us
            double direction = (pVPoint[ 0 ]->tx * ( pNormal->tx - pVPoint[ 0 ]->tx )) + 
                               (pVPoint[ 0 ]->ty * ( pNormal->ty - pVPoint[ 0 ]->ty )) + 
                               (pVPoint[ 0 ]->tz * ( pNormal->tz - pVPoint[ 0 ]->tz ));

            if( direction > 0.0F )
                visible = false;
        }
    }

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
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CTriangle::CalcVisible2D()
{
    bool result(true);

    // Make sure the panel has more than two points
    if( projCount > 2 )
    {
        int xMinInVis(0), xMaxInVis(0), yMinInVis(0), yMaxInVis(0), i(0);

        // Determine location of panel's 2D points
        for( ;i < projCount; ++i )
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

        if( xMinInVis >= projCount || yMinInVis >= projCount || 
            xMaxInVis >= projCount || yMaxInVis >= projCount )
            result = false;
    }
    else
        result = false;

    return result;

}   // CalcVisible2D


/************************************************************************
*    FUNCTION NAME:         WireFrame()
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
*                  08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CTriangle::Render_ZBufWireFrameGrouraudShading()
{
    int xSlope, ySlope, xStart, yStart, xInc, yInc, error, index, i;
    int yOffset, xOffset, yMax(giBufWidth * (giBufHeight-1)), xMax(giBufWidth), offset, zInc;
    unsigned long register *pVidBuf = gdVBufer;
    //float register *ZPtr = gdZBuffer;
    bool negZslope;
    float zStart, zSlope, Z;

    for( i = 0; i < projCount; ++i )
    {
        // Init
        error = 0;
        negZslope = false;
        zInc = 0;
        
        // Set the start offsets
        xStart = point2D[ i ].x;
        yStart = point2D[ i ].y;
        //zStart = pVPoint[ i ]->z;

        // Compute the slope                         
        xSlope = point2D[ (i+1) % projCount ].x - xStart;
        ySlope = point2D[ (i+1) % projCount ].y - yStart;
        //zSlope = pVPoint[ (i+1) % 3 ]->z - zStart;

        // init the offsets
        yOffset = yStart * xMax;
        xOffset = xStart;
        //Z = zStart;

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
        /*
        // test Z direction of slope
        if( zSlope < 0 )
        {
            // need an absolute value
            zSlope = -zSlope;
            negZslope = true;
        }
        */
        
        // Convert slope to a float
        //zFloat = (float)zSlope / (float) TSTEP_PREC_SL1;

        // Based on which slope is greater we can plot the path
        if( xSlope > ySlope )
        {
            /*
            // Calculate the inc of the Z and push to fixed point int
            if( zSlope && xSlope )
            {                
                // Divide the float versions of the slopes and convert it to fixed point int
                zInc = (zFloat / (float)xSlope) * (float)TSTEP_PREC_SL1;

                if( negZslope )
                    zInc = -zInc;
            }
            */
                   
            for( index = 0; index < xSlope; ++index )
            {
                // Clip the line
                if( yOffset > -1 && yOffset < yMax &&
                    xOffset > -1 && xOffset < xMax )
                {
                    // Calculate the offset into the buffer
                    offset = yOffset + xOffset; 

                    //if( ZPtr[offset] < Z )
                    //{
                        // Set the pixel color
                        pVidBuf[offset] = color;

                        // Update the Z buffer
                        //ZPtr[offset] = Z;
                    //}
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
                //Z += zInc;
            }
        }
        else
        {
            /*
            // Calculate the inc of the Z
            if( zSlope && ySlope )
            {
                // Divide the float versions of the slopes and convert it to fixed point int
                zInc = (zFloat / (float)ySlope) * (float)TSTEP_PREC_SL1;

                if( negZslope )
                    zInc = -zInc;
            }
            */

            for( index = 0; index < ySlope; ++index )
            {
                // Clip the line
                if( yOffset > -1 && yOffset < yMax &&
                    xOffset > -1 && xOffset < xMax )
                {
                    // Calculate the offset into the buffer
                    offset = yOffset + xOffset; 

                    //if( ZPtr[offset] < Z )
                    //{
                        // Set the pixel color
                        pVidBuf[offset] = color;

                        // Update the Z buffer
                        //ZPtr[offset] = Z;
                    //}
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
                //Z += zInc;
            }
        }           
    }

}   // Render_ZBufWireFrameGrouraudShading


/************************************************************************
*    FUNCTION NAME:         Render_ZBufTextGrouraudShading()
*
*    DESCRIPTION:           Z Buffered rendering, texture mapping and
*                           grouraud shading.
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
*                  08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CTriangle::Render_ZBufTextGrouraudShading()
{
    CCeilLine leftSeg, rightSeg;
    int top(0), yIndex, height, xStart, xEnd, width;
    float step, U, V, Z, I, stepI, stepU, stepV, stepZ, oneOverW;
    int newRightPos, newLeftPos;
    unsigned long register *DPtr, *ZPtr;
    unsigned long *pVBuffer = gdVBufer;//, *ZBuffer = gdZBuffer;
    int edgeCount(projCount);
    int ScrnW(giBufWidth), ScrnH(giBufHeight);
    unsigned long fixZ, fixU, fixV, fixI, fixStepZ, fixStepU, fixStepV, fixStepI, Tx, Ty;
    unsigned long volatile textel, textel_R, textel_G, textel_B, offset, fixItoInt;

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
            xStart = leftSeg.X;
            xEnd = rightSeg.X;
            width = xEnd - xStart;

            // Make sure we are within the bounds of the screen
            if( ( width > 0 ) && ( xEnd > 0 ) && ( xStart < ScrnW ) )
            {
                U = leftSeg.U;
                V = leftSeg.V;
                Z = leftSeg.Z;
                I = leftSeg.I;

                // Normally the below would be divided by the width:
                // Example: stepU = (rightSeg.U - U) / width
                // Division is slower but if we convert the width,
                // we can then do a multi instead.
                oneOverW = 1.0f / (float)width;

                stepU = (rightSeg.U - U) * oneOverW;
                stepV = (rightSeg.V - V) * oneOverW;
                stepZ = (rightSeg.Z - Z) * oneOverW;
                stepI = (rightSeg.I - I) * oneOverW;
                  
                // Clip the scan-line:
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

                offset = yIndex + xStart;
                DPtr = &pVBuffer[ offset ];
                //ZPtr = &ZBuffer[ offset ];

                // Force all the important stuff into fixed point
                fixZ = Z * FIX_SHIFT_24;
                fixU = U * FIX_SHIFT_24;
                fixV = V * FIX_SHIFT_24;
                fixI = I * FIX_SHIFT_16;
                fixStepZ = stepZ * FIX_SHIFT_24;
                fixStepU = stepU * FIX_SHIFT_24;
                fixStepV = stepV * FIX_SHIFT_24;
                fixStepI = stepI * FIX_SHIFT_16;

                // Loop for width of scan-line
                while( width-- > 0 )
                {
                    //if ( *ZPtr < Z )
                    //{
                        //*ZPtr = Z;
                        
                        // Floating point division automatically
                        // truncates the fraction part.
                        Tx = fixU / fixZ;
                        Ty = (fixV / fixZ) << 6;

                        // Convert to int from fixed foint math
                        fixItoInt = fixI >> 15;

                        // Seperate out each color
						textel = gdTexture[ Ty + Tx ];
                        textel_R = textel & 0xFF0000;
                        textel_G = textel & 0xFF00;
                        textel_B = textel & 0xFF;

                        // multiply the textels while in there respective places.
                        textel_R *= fixItoInt;
                        textel_G *= fixItoInt;
                        textel_B *= fixItoInt;
                        
                        // The formula here is to multiply the color by the intensity,
                        // then divide by the max color amount.
                        // color = (channel * intensity) / max_colors
                        // Below is a creative way to do the least amount of shifting
                        // to pack the final color back into 32-bit
                        *DPtr = ((textel_B >> 8) | 
                                ((textel_G >> 8) & 0xFF00) | 
                                ((textel_R >> 8) & 0xFF0000));                      
                    //}

                    ++DPtr;
                    fixZ += fixStepZ;
                    fixU += fixStepU;
                    fixV += fixStepV;
                    fixI += fixStepI; 
                      
                    //++ZPtr;
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
        
}   // Render_ZBufTextGrouraudShading


/************************************************************************
*    FUNCTION NAME:         Render_ZBufTextOnly()
*
*    DESCRIPTION:           Z Buffered rendering with texture mapping. 
*                           No shading.
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
*                  08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CTriangle::Render_ZBufTextOnly()
{
    CCeilLine leftSeg, rightSeg;
    int top(0), yIndex, height, xStart, xEnd, width;
    float step, Z, stepU, stepV, stepZ, oneOverW;
    int newRightPos, newLeftPos;
    unsigned long register *DPtr, *ZPtr;
    unsigned long *pVBuffer = gdVBufer;//, *ZBuffer = gdZBuffer;
    int edgeCount(projCount);
    int ScrnW(giBufWidth), ScrnH(giBufHeight);
    unsigned long fixZ, fixU, fixV, fixStepZ, fixStepU, fixStepV, Tx, Ty, offset, U, V;

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
            xStart = leftSeg.X;
            xEnd = rightSeg.X;
            width = xEnd - xStart;

            // Make sure we are within the bounds of the screen
            if( ( width > 0 ) && ( xEnd > 0 ) && ( xStart < ScrnW ) )
            {
                U = leftSeg.U;
                V = leftSeg.V;
                Z = leftSeg.Z;

                // Normally the below would be divided by the width:
                // Example: stepU = (rightSeg.U - U) / width
                // Division is slower but if we convert the width,
                // we can then do a multi instead.
                //oneOverW = 1.0f / (float)width;

                stepU = (rightSeg.U - U) / width;
                stepV = (rightSeg.V - V) / width;
                stepZ = (rightSeg.Z - Z) / width;
                  
                // Clip the scan-line:
                if( xStart < 0 )
                {
                    step = -xStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += stepZ * step;
                    U += stepU * step;
                    V += stepV * step;
                    
                    xStart = 0;
                    width = xEnd;
                }

                if( xEnd > ScrnW )
                {
                    xEnd = ScrnW;
                    width = xEnd - xStart;
                }

                offset = yIndex + xStart;
                DPtr = &pVBuffer[ offset ];
                //ZPtr = &ZBuffer[ offset ];
				
                // Force all the important stuff into fixed point
                fixZ = Z * FIX_SHIFT_24;
                //fixU = U * FIX_SHIFT_24;
                //fixV = V * FIX_SHIFT_24;
                fixStepZ = stepZ * FIX_SHIFT_24;
                //fixStepU = stepU * FIX_SHIFT_24;
                //fixStepV = stepV * FIX_SHIFT_24;

                // Loop for width of scan-line
                while( width-- > 0 )
                {
                    //if ( *ZPtr < Z )
                    //{
                        //*ZPtr = Z;
                        
                        // Floating point division automatically
                        // truncates the fraction part.
                        Tx = U / fixZ;
                        //Ty = (V / fixZ) << 6;
						Ty = (V / fixZ);

                        // Write the texture
                        *DPtr = gdTexture[ Ty + Tx ];
                        
                    //}

                    ++DPtr;
                    fixZ += fixStepZ;
                    U += stepU;
                    V += stepV; 
                      
                    //++ZPtr;
                }
            }

            rightSeg.X += rightSeg.stepX;
            ++rightSeg.Y;
            rightSeg.Z += rightSeg.stepZ;
            rightSeg.U += rightSeg.stepU;
            rightSeg.V += rightSeg.stepV;
            --rightSeg.edgeHeight;

            leftSeg.X += leftSeg.stepX;
            ++leftSeg.Y;
            leftSeg.Z += leftSeg.stepZ;
            leftSeg.U += leftSeg.stepU;
            leftSeg.V += leftSeg.stepV;
            --leftSeg.edgeHeight;

            yIndex += ScrnW;
        }
    }
        
}   // Render_ZBufTextOnly


/************************************************************************
*    FUNCTION NAME:         CalcLocalInten()
*
*    DESCRIPTION:           Calculate light intensity depending on local
*                           offsets
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
*                  08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CTriangle::CalcLocalInten()
{
   double Mag = ( sqrt ( 1000.0F * 1000.0F +
                         100.0F * 100.0F +
                         4.0F * 4.0F ) );
   // Assign a color based on normal:
   double CosA = ( ( pNormal->x - pVPoint[ 0 ]->x ) * 1000.0F +
                   ( pNormal->y - pVPoint[ 0 ]->y ) * 100.0F +
                   ( pNormal->z - pVPoint[ 0 ]->z ) * 4.0F ) / Mag;

   //intensity = ( CosA * ( double ) 256 ) + 120;
   intensity = ( CosA * ( double ) 220 ) + 150;

}  // CalcLocalInten


/************************************************************************
*    FUNCTION NAME:         CalcLocalInten()
*
*    DESCRIPTION:           Calculate light intensity depending on world
*                           offsets
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
*                  08/20/03      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CTriangle::CalcWorldInten()
{
   double Mag = ( sqrt ( 1000.0F * 1000.0F +
                         100.0F * 100.0F +
                         4.0F * 4.0F ) );
   // Assign a color based on normal:
   double CosA = ( ( pNormal->tx - pVPoint[ 0 ]->tx ) * 1000.0F +
                   ( pNormal->ty - pVPoint[ 0 ]->ty ) * 100.0F +
                   ( pNormal->tz - pVPoint[ 0 ]->tz ) * 4.0F ) / Mag;

   intensity = ( CosA * ( double ) 200 ) + 120;
}