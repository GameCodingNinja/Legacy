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

// Globals
extern float gfCameraScaleX;
extern float gfCameraScaleY;
extern unsigned long *gdVBufer;
extern int giBufWidth, giBufHeight;
extern float gfBufWidthDiv2, gfBufHeightDiv2;
extern float *gfZBuffer;
extern float gfMaxZDistance;


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
                if( !pVPoint[endI]->proj2D.fProjected )
                    Project( endI );

                // Copy over the projected points
                point2D[outC] = pVPoint[endI]->proj2D;

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

                // Project the clipped point
                Project( outC, tmp3D );

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

                // Project the clipped point
                Project( outC, tmp3D );

                // Update index
                ++outC;

                // See if this point hasn't been projected yet.
                // If the point hasn't been projected, project the point
                if( !pVPoint[endI]->proj2D.fProjected )
                    Project( endI );

                // Copy over the projected points
                point2D[outC] = pVPoint[endI]->proj2D;
                /*
                // Add an extra edge to list:
                tmp3D.tx =  pVPoint[ endI ]->tx;
                tmp3D.ty =  pVPoint[ endI ]->ty;
                tmp3D.tz =  pVPoint[ endI ]->tz;

                Project( outC, tmp3D );
                */
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

void CTriangle::Project( int index )
{
    // Convert the Z to 1/Z. This allows us to multiply
    // the Z value vs. dividing it. Multiplying it is much faster.
    float oneOverZ( 1.0F / pVPoint[index]->tz );

    // Calculate the pixel coordinates
    pVPoint[index]->proj2D.x = ((pVPoint[index]->tx * gfCameraScaleX) * oneOverZ) + gfBufWidthDiv2;
    pVPoint[index]->proj2D.y = ((pVPoint[index]->ty * gfCameraScaleY) * oneOverZ) + gfBufHeightDiv2;

    // Set the flag to indicate this point was projected
    pVPoint[index]->proj2D.fProjected = true;

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

void CTriangle::Project( int index, CPoint3D &clippedPoint )
{
    // Convert the Z to 1/Z. This allows us to multiply
    // the Z value vs. dividing it. Multiplying it is much faster.
    float oneOverZ( 1.0F / clippedPoint.tz );

    // Calculate the pixel coordinates
    point2D[index].x = ((clippedPoint.tx * gfCameraScaleX) * oneOverZ) + gfBufWidthDiv2;
    point2D[index].y = ((clippedPoint.ty * gfCameraScaleY) * oneOverZ) + gfBufHeightDiv2;

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

void CTriangle::WireFrame()
{
    int xSlope, ySlope, xStart, yStart, xInc, yInc, error, index, i;
    int yOffset, xOffset, yMax(giBufWidth * (giBufHeight-1)), xMax(giBufWidth), offset, zInc;
    unsigned long color;
    unsigned long register *pVidBuf = gdVBufer;
    float register *ZPtr = gfZBuffer;
    bool negZslope;
    float zStart, zSlope, Z;

    color = 245;
    color |= 102 << 16;
    color |= 206 << 8;

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

}   // WireFrame


/************************************************************************
*    FUNCTION NAME:         Render()
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

void CTriangle::Render()
{
    CCeilLine leftSeg, rightSeg;
    int top(0), yIndex, height, xStart, xEnd, width;
    float step;
    int newRightPos, newLeftPos;
    unsigned long register *DPtr;
	unsigned long color;

    color = 245;
    color |= 102 << 16;
    color |= 206 << 8;

    /////  Use local varables for speed
    int edgeCount(projCount);
    /////  Use local varables for speed


    // Search for lowest Y coordinate (top of polygon)
    for( int n = 1; n < edgeCount; ++n )
    {
        if( point2D[ n ].y < point2D[ top ].y )
            top = n;
    }

    // This will initilize faster
    int leftPos(top), rightPos(top);

    // Calculate the index to the buffer
    yIndex = point2D[ top ].y * giBufWidth;

    // Loop for all polygon edges:
    while( edgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( rightSeg.edgeHeight <= 0 )
        {
            newRightPos = rightPos + 1;

            if( newRightPos >= projCount )
                newRightPos = 0;

            rightSeg.x1 = point2D[ rightPos ].x;
            rightSeg.x2 = point2D[ newRightPos ].x;
            rightSeg.y1 = point2D[ rightPos ].y;
            rightSeg.y2 = point2D[ newRightPos ].y;

            rightSeg.edgeHeight = ( rightSeg.y2 - rightSeg.y1 );

            rightSeg.x = rightSeg.x1 + CEIL_FRACT;
            rightSeg.y = rightSeg.y1;
            //rightSeg.z = SPoint[ rightPos ].Z;

            // Store U/Z and V/Z values:
            //rightSeg.U = SPoint[ RightPos ].U;
            //rightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            //RightSeg.I = SPoint[ RightPos ].I;

            if( rightSeg.edgeHeight )
            {
                rightSeg.stepX = ( rightSeg.x2 - rightSeg.x1 ) / rightSeg.edgeHeight;
                //RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                //RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                //RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                //RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                rightSeg.stepX = 0;
                //RightSeg.StepZ = 0;
                //RightSeg.StepU = 0;
                //RightSeg.StepV = 0;
                //RightSeg.StepI = 0;
            }

            rightPos = newRightPos;
            --edgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( rightSeg.y < 0 ) // 0 was MINY
            {
                if( rightSeg.y2 != rightSeg.y1 )
                {
                    step = -rightSeg.y;

                    rightSeg.x = rightSeg.x1 +
                                 ((( double ) ( rightSeg.x2 - rightSeg.x1 ) /
                                 ( double ) (rightSeg.y2 - rightSeg.y1)) *
                                 ( double ) step ) + CEIL_FRACT;

                    rightSeg.y = 0;
                    //rightSeg.Z += RightSeg.StepZ * Step;
                    //rightSeg.U += RightSeg.StepU * Step;
                    //rightSeg.V += RightSeg.StepV * Step;
                    //rightSeg.I += RightSeg.StepI * Step;
                    rightSeg.edgeHeight -= step;
                }

                yIndex = 0;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( leftSeg.edgeHeight <= 0 )
        {
            newLeftPos = leftPos - 1;

            if( newLeftPos < 0 )
                newLeftPos = ( projCount - 1 );

            leftSeg.x1 = point2D[ leftPos ].x;
            leftSeg.x2 = point2D[ newLeftPos ].x;
            leftSeg.y1 = point2D[ leftPos ].y;
            leftSeg.y2 = point2D[ newLeftPos ].y;

            leftSeg.edgeHeight = ( leftSeg.y2 - leftSeg.y1 );

            leftSeg.x = leftSeg.x1 + CEIL_FRACT;
            leftSeg.y = leftSeg.y1;
            //leftSeg.z = SPoint[ leftPos ].Z;

            // Store U/Z and V/Z values:
            //leftSeg.U = SPoint[ leftPos ].U;
            //leftSeg.V = SPoint[ leftPos ].V;

            // Store intensity value:
            //leftSeg.I = SPoint[ leftPos ].I;

            if( leftSeg.edgeHeight )
            {
                leftSeg.stepX = ( leftSeg.x2 - leftSeg.x1 ) / leftSeg.edgeHeight;
                //leftSeg.StepZ = ( SPoint[ NewleftPos ].Z - SPoint[ leftPos ].Z ) / leftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                //leftSeg.StepU = ( SPoint[ NewleftPos ].U - SPoint[ leftPos ].U ) / leftSeg.EdgeHeight;
                //leftSeg.StepV = ( SPoint[ NewleftPos ].V - SPoint[ leftPos ].V ) / leftSeg.EdgeHeight;
                // Calculate intensity delta
                //leftSeg.StepI = ( SPoint[ NewleftPos ].I - SPoint[ leftPos ].I ) / leftSeg.EdgeHeight;
            }
            else
            {
                leftSeg.stepX = 0;
                //leftSeg.StepZ = 0;
                //leftSeg.StepU = 0;
                //leftSeg.StepV = 0;
                //leftSeg.StepI = 0;
            }

            leftPos = newLeftPos;
            --edgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( leftSeg.y < 0 )
            {
                if( leftSeg.y2 != leftSeg.y1 )
                {
                    step = -leftSeg.y;

                    leftSeg.x = leftSeg.x1 +
                                 ((( double ) ( leftSeg.x2 - leftSeg.x1 ) /
                                 ( double ) (leftSeg.y2 - leftSeg.y1)) *
                                 ( double ) step ) + CEIL_FRACT;

                    leftSeg.y = 0;
                    //leftSeg.Z += leftSeg.StepZ * Step;
                    //leftSeg.U += leftSeg.StepU * Step;
                    //leftSeg.V += leftSeg.StepV * Step;
                    //leftSeg.I += leftSeg.StepI * Step;
                    leftSeg.edgeHeight -= step;
                }

                yIndex = 0;
            }
        }


        // Subdivide polygon into trapezoid:
        if( leftSeg.edgeHeight < rightSeg.edgeHeight )
        {
            height = leftSeg.edgeHeight;

            if( ( leftSeg.y + height ) > giBufHeight )
            {
                height = giBufHeight - leftSeg.y;
                edgeCount = 0;
            }
        }
        else 
        {
            height = rightSeg.edgeHeight;

            if( ( rightSeg.y + height ) > giBufHeight )
            {
                height = giBufHeight - rightSeg.y;
                edgeCount = 0;
            }
        }


         // Loop for the height of the trapezoid:
         while( height > 0 )
         {
            --height;

               // Calculate initial values:
               xStart = leftSeg.x;
               xEnd = rightSeg.x;
               width = xEnd - xStart;

               if( ( width > 0 ) && ( xEnd > 0 ) && ( xStart < giBufWidth ) )
               {
                  //Z = LeftSeg.GetZ ();
                  //DeltaZ = ( RightSeg.GetZ () - LeftSeg.GetZ () );
                  //ZStep = DeltaZ / Width;

                  // Clip the scan-line:
                    if( xStart < 0 )
                    {
                        //Step = -XStart;
                        // Take advantage of the fact that ( a * ( b * f ) / f ) 
                        // is equal to ( a * b );

                        //Z += ZStep * Step;
                        //U += UStep * Step;
                        //V += VStep * Step;
                        //I += IStep * Step;

                        xStart = 0;
                        width = xEnd;
                    }

                    if( xEnd > giBufWidth )
                    {
                        xEnd = giBufWidth;
                        width = xEnd - xStart;
                    }

                  width = xEnd - xStart;
                  DPtr = &gdVBufer[ yIndex + xStart ];
                  //ZPtr = &ZBuffer [ YIndex + XStart ];

                  // Loop for width of scan-line:
                  while( width > 0 )
                  {
                    -- width;
                        //if ( *ZPtr < Z )
                           //{
                           //*ZPtr = Z;
                           *DPtr = color;
                           //}
                        //Z += ZStep;
                        ++DPtr;
                        //++ZPtr;
                  }
            }

            rightSeg.x += rightSeg.stepX;
            ++rightSeg.y;
            //rightSeg.Z += rightSeg.StepZ;
            //rightSeg.U += rightSeg.StepU;
            //rightSeg.V += rightSeg.StepV;
            //rightSeg.I += rightSeg.StepI;
            --rightSeg.edgeHeight;

            leftSeg.x += leftSeg.stepX;
            ++leftSeg.y;
            //leftSeg.Z += leftSeg.StepZ;
            //LeftSeg.U += LeftSeg.StepU;
            //LeftSeg.V += LeftSeg.StepV;
            //LeftSeg.I += LeftSeg.StepI;
            --leftSeg.edgeHeight;

               yIndex += giBufWidth;
        }

    }

}   // Render