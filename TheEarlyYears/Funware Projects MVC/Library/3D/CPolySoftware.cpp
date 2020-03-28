/************************************************************************
*
*    FILE NAME:       CPolySoftware.cpp
*
*    DESCRIPTION:     Software Rendering functions
*
************************************************************************/

                           // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT             // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "types.h"         // Header file with #defines
#include "CPolygon.h"      // Header file for this *.cpp file.

// Globals
extern float gfMaxZDistance;
extern float gfMinZDistance;
extern float gfCameraScaleX;
extern float gfCameraScaleY;
extern float gfBufWidthDiv2;
extern float gfBufHeightDiv2;
extern int giBufWidth;
extern int giBufHeight;
extern unsigned int *gusiVBufer;
extern int *giZBuffer;
int gSoftwareTextIndex=-1;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

#ifdef _16_BIT_SOFTWARE_RENDERING_
extern unsigned short int *gusiShadeTbl;
#endif


/************************************************************************
*    DESCRIPTION:  bind a specific texture for rendering
************************************************************************/
void CPolygon::BindSoftwareText()
{
    gSoftwareTextIndex = tIndex;

}   // BindSoftwareText


/************************************************************************
*    DESCRIPTION:  Get the texture index
************************************************************************/
int CPolygon::GetTextIndex()
{
	if( gSoftwareTextIndex > -1 )
		return gSoftwareTextIndex;
	else
		return tIndex;

}   // GetTextIndex


/************************************************************************
*    DESCRIPTION:  Clip the polygon to the near z plane and
*                  project the points
************************************************************************/
bool CPolygon::IsVisable2D()
{
    bool result(false);

    // Make sure the panel has more than two points
    if( projCount > 2 )
    {
        int XMinInVis(0), XMaxInVis(0), YMinInVis(0), YMaxInVis(0);

        // Determine location of panel's 2D points
        for( int i = 0; i < projCount; ++i )
        {
            if( projectPoint[ i ].x < 0 ) // 0 was MINX
                ++XMinInVis;

            else if( projectPoint[ i ].x > giBufWidth )
                ++XMaxInVis;

            if( projectPoint[ i ].y < 0 )  // 0 was MINY
                ++YMinInVis;

            else if( projectPoint[ i ].y > giBufHeight )
                ++YMaxInVis;
        }

        if( projCount > XMinInVis && projCount > YMinInVis && 
            projCount > XMaxInVis && projCount > YMaxInVis )
            result = true;
    }

    return result;

}   // IsVisable2D


/************************************************************************
*    DESCRIPTION:  Clip the polygon to the near z plane and
*                  project the points
************************************************************************/
void _fastcall CPolygon::ClipProject( CTextureLib *pTextLib )
{
    int inc, start, textIndex;
    CPoint clipXYZ[ MAX_CLIPPED_POINTS ];
    CUVRGB clipUVI[ MAX_CLIPPED_POINTS ];
    
    textIndex = tIndex;
    
    if( gSoftwareTextIndex > -1 )
	{
    	textIndex = gSoftwareTextIndex;
	}

    // Init
    start = vertCount - 1;
    inc = 0;

    // set the state to indicate if this polygon was clipped
    clipped = false;

    // Loop through all edges of polygon using S&H algorithm
    for( int end = 0; end < vertCount; ++end )
    {
        if( pVPoint[ start ]->tz <= -gfMinZDistance )
        {
            // Case 1
            // Entirely inside front view volume - output unchanged vertex
            if( pVPoint[ end ]->tz <= -gfMinZDistance )
            {                
                // Straight copy
                clipXYZ[ inc ].x = pVPoint[ end ]->tx;
                clipXYZ[ inc ].y = pVPoint[ end ]->ty;
                clipXYZ[ inc ].z = pVPoint[ end ]->tz;

                clipUVI[ inc ] = uvrgb[ end ];

                // We need to force more precision if we are too
                // close to the near z plane
                if( clipXYZ[ inc ].z > PRECISION_Z_PLANE )
                    clipped = true;

                ++inc;
            }

            // Case 2
            // Point is leaving view volume
            // clip using parametric form of line
            else
            {    
                // Calculate the percentage of the line length
                float deltaZ = pVPoint[ end ]->tz - pVPoint[ start ]->tz;                
                float p = ((-gfMinZDistance) - pVPoint[ start ]->tz) / deltaZ;

                // Clip the new X
                clipXYZ[ inc ].x = pVPoint[ start ]->tx +
                                   ((pVPoint[ end ]->tx - pVPoint[ start ]->tx) * p);

                // Clip the new Y
                clipXYZ[ inc ].y = pVPoint[ start ]->ty + 
                                   ((pVPoint[ end ]->ty - pVPoint[ start ]->ty) * p);

                // The new Z is the Near Clipping Plain
                clipXYZ[ inc ].z = -gfMinZDistance;

                // Calculate new U value
                clipUVI[ inc ].u = uvrgb[ start ].u + 
                                   ((uvrgb[ end ].u - uvrgb[ start ].u) * p);

                // Calculate new V value
                clipUVI[ inc ].v = uvrgb[ start ].v + 
                                   ((uvrgb[ end ].v - uvrgb[ start ].v) * p);

                // Calculate new R value
                // For software rendering, we are only using r for the intensity value
                clipUVI[ inc ].r = uvrgb[ start ].r + 
                                   ((uvrgb[ end ].r - uvrgb[ start ].r) * p);

                #ifndef _16_BIT_SOFTWARE_RENDERING_
                // Calculate new G value
                // For software rendering, we are only using r for the intensity value
                clipUVI[ inc ].g = uvrgb[ start ].g + 
                                   ((uvrgb[ end ].g - uvrgb[ start ].g) * p);

                // Calculate new B value
                // For software rendering, we are only using r for the intensity value
                clipUVI[ inc ].b = uvrgb[ start ].b + 
                                   ((uvrgb[ end ].b - uvrgb[ start ].b) * p);
                #endif

                ++inc;

                // This polygon has officially been clipped
                clipped = true;
            }
        }

        // case 3
        // Point is entering view volume 
        // clip using parametric form of line   
        else
        {
            if( pVPoint[ end ]->tz <= -gfMinZDistance ) 
            {
                // Calculate the percentage of the line length
                float deltaZ = pVPoint[ end ]->tz - pVPoint[ start ]->tz;                
                float p = ((-gfMinZDistance) - pVPoint[ start ]->tz) / deltaZ;

                // Clip the new X
                clipXYZ[ inc ].x = pVPoint[ start ]->tx + 
                                   ((pVPoint[ end ]->tx - pVPoint[ start ]->tx ) * p);

                // Clip the new Y
                clipXYZ[ inc ].y = pVPoint[ start ]->ty +
                                   ((pVPoint[ end ]->ty - pVPoint[ start ]->ty) * p);

                // The new Z is the Near Clipping Plain
                clipXYZ[ inc ].z = -gfMinZDistance;

                // Calculate new U value
                clipUVI[ inc ].u = uvrgb[ start ].u + 
                                   ((uvrgb[ end ].u - uvrgb[ start ].u ) * p);

                // Calculate new V value
                clipUVI[ inc ].v = uvrgb[ start ].v + 
                                   ((uvrgb[ end ].v - uvrgb[ start ].v ) * p);

                // Calculate new R value
                // For software rendering, we are only using r for the intensity value
                clipUVI[ inc ].r = uvrgb[ start ].r + 
                                   ((uvrgb[ end ].r - uvrgb[ start ].r) * p);

                #ifndef _16_BIT_SOFTWARE_RENDERING_
                // Calculate new G value
                // For software rendering, we are only using r for the intensity value
                clipUVI[ inc ].g = uvrgb[ start ].g + 
                                   ((uvrgb[ end ].g - uvrgb[ start ].g) * p);

                // Calculate new B value
                // For software rendering, we are only using r for the intensity value
                clipUVI[ inc ].b = uvrgb[ start ].b + 
                                   ((uvrgb[ end ].b - uvrgb[ start ].b) * p);
                #endif

                ++inc;

                // Add an extra edge to list
                clipXYZ[ inc ].x = pVPoint[ end ]->tx;
                clipXYZ[ inc ].y = pVPoint[ end ]->ty;
                clipXYZ[ inc ].z = pVPoint[ end ]->tz;

                clipUVI[ inc ] = uvrgb[ end ];

                ++inc;
                
                // This polygon has officially been clipped
                clipped = true;                
            }
        }

        // Advance to next vertex
        start = end;
    }

    // Store the number of vertices to project
    projCount = inc;

    // Project
    for( int i = 0; i < projCount; ++i )
    {
        // One over z because multiplication is faster the division
        float oneOverZ = 1.0 / clipXYZ[i].z;

        projectPoint[i].x = ((clipXYZ[i].x * gfCameraScaleX) * oneOverZ) + gfBufWidthDiv2;
        projectPoint[i].y = ((clipXYZ[i].y * gfCameraScaleY) * oneOverZ) + gfBufHeightDiv2;
        projectPoint[i].z = oneOverZ * FIX_SHIFT_26_FLOAT;

        projectPoint[i].U = ((clipUVI[i].u * (pTextLib->pTMap[textIndex].GetWidth()-1)) * oneOverZ) * FIX_SHIFT_26_FLOAT;
        projectPoint[i].V = ((clipUVI[i].v * (pTextLib->pTMap[textIndex].GetHeight()-1)) * oneOverZ) * FIX_SHIFT_26_FLOAT;

        // We need more precision if the polygon has been clipped
        #ifndef _16_BIT_SOFTWARE_RENDERING_
        if( clipped )
        {
            projectPoint[i].r = ((int)clipUVI[i].r) << FIX_SHIFT_23_INT;
            projectPoint[i].g = ((int)clipUVI[i].g) << FIX_SHIFT_23_INT;
            projectPoint[i].b = ((int)clipUVI[i].b) << FIX_SHIFT_23_INT;
        }
        else
        {
            projectPoint[i].r = ((int)clipUVI[i].r) << FIX_SHIFT_16_INT;
            projectPoint[i].g = ((int)clipUVI[i].g) << FIX_SHIFT_16_INT;
            projectPoint[i].b = ((int)clipUVI[i].b) << FIX_SHIFT_16_INT;
        }
        #else
        if( clipped )
        {
            projectPoint[i].r = (((int)clipUVI[i].r) >> 3) << FIX_SHIFT_26_INT;
        }
        else
        {
            projectPoint[i].r = (((int)clipUVI[i].r) >> 3) << FIX_SHIFT_15_INT;
        }
        #endif
    }
    
}   // ClipProject


/************************************************************************
*    DESCRIPTION:  Clip the polygon to the near z plane and
*                  project the points
************************************************************************/
void CPolygon::ClipProject()
{
    int inc, start;
    CPoint clipXYZ[ MAX_CLIPPED_POINTS ];
    CUVRGB clipUVI[ MAX_CLIPPED_POINTS ]; 

    // Init
    start = vertCount - 1;
    inc = 0;

    // set the state to indicate if this polygon was clipped
    clipped = false;

    // Loop through all edges of polygon using S&H algorithm
    for( int end = 0; end < vertCount; ++end )
    {
        if( pVPoint[ start ]->tz <= -gfMinZDistance )
        {
            // Case 1
            // Entirely inside front view volume - output unchanged vertex
            if( pVPoint[ end ]->tz <= -gfMinZDistance )
            {                
                // Straight copy
                clipXYZ[ inc ].x = pVPoint[ end ]->tx;
                clipXYZ[ inc ].y = pVPoint[ end ]->ty;
                clipXYZ[ inc ].z = pVPoint[ end ]->tz;

                clipUVI[ inc ] = uvrgb[ end ];

                // We need to force more precision if we are too
                // close to the near z plane
                if( clipXYZ[ inc ].z > PRECISION_Z_PLANE )
                    clipped = true;

                ++inc;
            }

            // Case 2
            // Point is leaving view volume
            // clip using parametric form of line
            else
            {    
                // Calculate the percentage of the line length
                float deltaZ = pVPoint[ end ]->tz - pVPoint[ start ]->tz;                
                float p = ((-gfMinZDistance) - pVPoint[ start ]->tz) / deltaZ;

                // Clip the new X
                clipXYZ[ inc ].x = pVPoint[ start ]->tx +
                                   ((pVPoint[ end ]->tx - pVPoint[ start ]->tx) * p);

                // Clip the new Y
                clipXYZ[ inc ].y = pVPoint[ start ]->ty + 
                                   ((pVPoint[ end ]->ty - pVPoint[ start ]->ty) * p);

                // The new Z is the Near Clipping Plain
                clipXYZ[ inc ].z = -gfMinZDistance;

                // Calculate new R value
                // For software rendering, we are only using r for the intensity value
                clipUVI[ inc ].r = uvrgb[ start ].r + 
                                   ((uvrgb[ end ].r - uvrgb[ start ].r) * p);

                #ifndef _16_BIT_SOFTWARE_RENDERING_
                // Calculate new G value
                // For software rendering, we are only using r for the intensity value
                clipUVI[ inc ].g = uvrgb[ start ].g + 
                                   ((uvrgb[ end ].g - uvrgb[ start ].g) * p);

                // Calculate new B value
                // For software rendering, we are only using r for the intensity value
                clipUVI[ inc ].b = uvrgb[ start ].b + 
                                   ((uvrgb[ end ].b - uvrgb[ start ].b) * p);
                #endif

                ++inc;

                // This polygon has officially been clipped
                clipped = true;
            }
        }

        // case 3
        // Point is entering view volume 
        // clip using parametric form of line   
        else
        {
            if( pVPoint[ end ]->tz <= -gfMinZDistance ) 
            {
                // Calculate the percentage of the line length
                float deltaZ = pVPoint[ end ]->tz - pVPoint[ start ]->tz;                
                float p = ((-gfMinZDistance) - pVPoint[ start ]->tz) / deltaZ;

                // Clip the new X
                clipXYZ[ inc ].x = pVPoint[ start ]->tx + 
                                   ((pVPoint[ end ]->tx - pVPoint[ start ]->tx ) * p);

                // Clip the new Y
                clipXYZ[ inc ].y = pVPoint[ start ]->ty +
                                   ((pVPoint[ end ]->ty - pVPoint[ start ]->ty) * p);

                // The new Z is the Near Clipping Plain
                clipXYZ[ inc ].z = -gfMinZDistance;

                // Calculate new R value
                // For software rendering, we are only using r for the intensity value
                clipUVI[ inc ].r = uvrgb[ start ].r + 
                                   ((uvrgb[ end ].r - uvrgb[ start ].r) * p);

                #ifndef _16_BIT_SOFTWARE_RENDERING_
                // Calculate new G value
                // For software rendering, we are only using r for the intensity value
                clipUVI[ inc ].g = uvrgb[ start ].g + 
                                   ((uvrgb[ end ].g - uvrgb[ start ].g) * p);

                // Calculate new B value
                // For software rendering, we are only using r for the intensity value
                clipUVI[ inc ].b = uvrgb[ start ].b + 
                                   ((uvrgb[ end ].b - uvrgb[ start ].b) * p);
                #endif

                ++inc;

                // Add an extra edge to list
                clipXYZ[ inc ].x = pVPoint[ end ]->tx;
                clipXYZ[ inc ].y = pVPoint[ end ]->ty;
                clipXYZ[ inc ].z = pVPoint[ end ]->tz;

                clipUVI[ inc ] = uvrgb[ end ];

                ++inc;
                
                // This polygon has officially been clipped
                clipped = true;                
            }
        }

        // Advance to next vertex
        start = end;
    }

    // Store the number of vertices to project
    projCount = inc;

    // Project
    for( int i = 0; i < projCount; ++i )
    {
        // One over z because multiplication is faster the division
        float oneOverZ = 1.0 / clipXYZ[i].z;

        projectPoint[i].x = ((clipXYZ[i].x * gfCameraScaleX) * oneOverZ) + gfBufWidthDiv2;
        projectPoint[i].y = ((clipXYZ[i].y * gfCameraScaleY) * oneOverZ) + gfBufHeightDiv2;
        projectPoint[i].z = oneOverZ * FIX_SHIFT_26_FLOAT;

        // We need more precision if the polygon has been clipped
        #ifndef _16_BIT_SOFTWARE_RENDERING_
        if( clipped )
        {
            projectPoint[i].r = ((int)clipUVI[i].r) << FIX_SHIFT_23_INT;
            projectPoint[i].g = ((int)clipUVI[i].g) << FIX_SHIFT_23_INT;
            projectPoint[i].b = ((int)clipUVI[i].b) << FIX_SHIFT_23_INT;
        }
        else
        {
            projectPoint[i].r = ((int)clipUVI[i].r) << FIX_SHIFT_16_INT;
            projectPoint[i].g = ((int)clipUVI[i].g) << FIX_SHIFT_16_INT;
            projectPoint[i].b = ((int)clipUVI[i].b) << FIX_SHIFT_16_INT;
        }
        #else
        if( clipped )
        {
            projectPoint[i].r = (((int)clipUVI[i].r) >> 3) << FIX_SHIFT_26_INT;
        }
        else
        {
            projectPoint[i].r = (((int)clipUVI[i].r) >> 3) << FIX_SHIFT_15_INT;
        }
        #endif
    }
    
}   // ClipProject


/************************************************************************
*    DESCRIPTION:  Outline the polygon
************************************************************************/
void _fastcall CPolygon::Render_WireFrame()
{
    int xSlope, ySlope, xStart, yStart, xInc, yInc, error, index, i;
    int yOffset, xOffset, yMax(giBufWidth * (giBufHeight-1)), xMax(giBufWidth), offset;
    int register *ZPtr = giZBuffer;
    int zStart, Z, stepZ;

    #ifdef _16_BIT_SOFTWARE_RENDERING_
    unsigned short int register *pVidBuf = (unsigned short int *)gusiVBufer;
    unsigned short int register color = 0xffff;
    #else
    unsigned int register *pVidBuf = gusiVBufer;
    unsigned int register color = 0xffffff;

	if( collisionType == ECT_MAP_FLOOR )
		color = 0x00ffff;
	else if( collisionType == ECT_MAP_WALL )
		color = 0xffff00;
		 
    #endif

    for( i = 0; i < projCount; ++i )
    {
        // Init
        error = 0;
        
        // Set the start offsets
        xStart = projectPoint[ i ].x;
        yStart = projectPoint[ i ].y;
        zStart = projectPoint[ i ].z;

        // Compute the slope                         
        xSlope = projectPoint[ (i+1) % projCount ].x - xStart;
        ySlope = projectPoint[ (i+1) % projCount ].y - yStart;

        // Values can't be zero
        if( xSlope == 0 )
            xSlope = 1;

        if( ySlope == 0 )
            ySlope = 1;

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
            stepZ = (projectPoint[ (i+1) % projCount ].z - zStart) / xSlope;
                   
            for( index = 0; index < xSlope; ++index )
            {
                // Clip the line
                if( yOffset > -1 && yOffset < yMax &&
                    xOffset > -1 && xOffset < xMax )
                {
                    // Calculate the offset into the buffer
                    offset = yOffset + xOffset; 

                    if( ZPtr[offset] > Z )
                    {
                        // Set the pixel color
                        pVidBuf[offset] = color;

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
            }
        }
        else
        {
            // Calculate Z slope
            stepZ = (projectPoint[ (i+1) % projCount ].z - zStart) / ySlope;

            for( index = 0; index < ySlope; ++index )
            {
                // Clip the line
                if( yOffset > -1 && yOffset < yMax &&
                    xOffset > -1 && xOffset < xMax )
                {
                    // Calculate the offset into the buffer
                    offset = yOffset + xOffset; 

                    if( ZPtr[offset] > Z )
                    {
                        // Set the pixel color
                        pVidBuf[offset] = color;

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
            }
        }           
    }

}   // Render_WireFrame


/************************************************************************
*    DESCRIPTION:  RGB shaded texture mapping. Do some different things
*                  because this polygon was clipped to the near Z plane
*                  and we need more precision.
*
*    Input:    PCTextureLib pTextLib - Texture library
************************************************************************/
void _fastcall CPolygon::RenderClipped( CTextureLib *pTextLib )
{
    CCeilLine leftSeg, rightSeg;
    int top(0), yIndex, height, xStart, xEnd, width;
    int newRightPos, newLeftPos, runLoops, length;
    int register *ZPtr;
    int *tmpZBuf = giZBuffer;  
    int edgeCount(projCount);
    int ScrnW(giBufWidth), ScrnH(giBufHeight);
    int U, V, RZ, Z, stepU, stepV, stepZ, step, Tx1, Ty1, Tx2, Ty2, offset;
    int r, g, b, stepR, stepG, stepB;
    int subUStep, subVStep, subZStep, TxStep, TyStep, Tx, Ty;
	int textIndex = tIndex;
    
    if( gSoftwareTextIndex > -1 )
	{
    	textIndex = gSoftwareTextIndex;
	}

    unsigned int shiftXaddition = FIX_SHIFT_23_INT - pTextLib->pTMap[textIndex].GetMultiShift();
    unsigned char shiftXadditionByte = (unsigned char)shiftXaddition;

    #ifndef _16_BIT_SOFTWARE_RENDERING_
        unsigned int register *DPtr, *pText;
        pText = (unsigned int *)pTextLib->pTMap[textIndex].GetBits();
        unsigned int textel;
        unsigned int *tmpVBuf = (unsigned int *)gusiVBufer;
    #else
        unsigned short int register *DPtr, *pText, *pShade;
        pText = (unsigned short int *)pTextLib->pTMap[textIndex].GetBits();
        unsigned short int *tmpVBuf = (unsigned short int *)gusiVBufer;
        pShade = gusiShadeTbl;
    #endif
    
    // Search for lowest Y coordinate (top of polygon)
    for( int n = 1; n < edgeCount; ++n )
    {
        if( projectPoint[ n ].y < projectPoint[ top ].y )
            top = n;
    }

    // This will initilize faster
    int leftPos(top), rightPos(top);

    // Calculate the index into the video buffer
    yIndex = projectPoint[ top ].y * ScrnW;

    // Loop for all polygon edges
    while( edgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing
        if( rightSeg.edgeHeight < 1 )
        {
            newRightPos = rightPos + 1;

            if( newRightPos >= projCount )
                newRightPos = 0;

            // Init the right side of the triangle
            rightSeg.Init( projectPoint[ rightPos ], projectPoint[ newRightPos ] );

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
        // (re)initializing
        if( leftSeg.edgeHeight < 1 )
        {
            newLeftPos = leftPos - 1;

            if( newLeftPos < 0 )
                newLeftPos = ( projCount - 1 );

            // Init the left side of the triangle
            leftSeg.Init( projectPoint[ leftPos ], projectPoint[ newLeftPos ] );

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

        // Subdivide polygon into trapezoid
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

        // Loop for the height of the trapezoid
        while( height-- > 0 )
        {
            // Calculate initial values
            xStart = leftSeg.X_float;
            xEnd = rightSeg.X_float;
            width = xEnd - xStart;

            // Make sure we are within the bounds of the screen
            if( ( width > 0 ) && ( xEnd > 0 ) && ( xStart < ScrnW ) )
            {
                U = leftSeg.U;
                V = leftSeg.V;
                Z = leftSeg.Z;
                r = leftSeg.r;
                #ifndef _16_BIT_SOFTWARE_RENDERING_
                g = leftSeg.g;
                b = leftSeg.b;
                #endif

                // Create the step amounts for the scan line
                stepU = (rightSeg.U - U) / width;
                stepV = (rightSeg.V - V) / width;
                stepZ = (rightSeg.Z - Z) / width;
                stepR = (rightSeg.r - r) / width;
                #ifndef _16_BIT_SOFTWARE_RENDERING_
                stepG = (rightSeg.g - g) / width;
                stepB = (rightSeg.b - b) / width;
                #endif
                  
                // Clip the scan-line
                if( xStart < 0 )
                {
                    step = -xStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += stepZ * step;
                    U += stepU * step;
                    V += stepV * step;
                    r += stepR * step;
                    #ifndef _16_BIT_SOFTWARE_RENDERING_
                    g += stepG * step;
                    b += stepB * step;
                    #endif
                    
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
                    // Premultiply steps for subdivision
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

                // Loop for number of subdivisions
                for( int count = 0; count < runLoops; ++count )
                {
                    length = RUN_LENGTH_16;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision
                    RZ = Z;

                    // Jump ahead to next subdivision
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
                        
                        if( *ZPtr > Z )
                        {
                            *ZPtr = Z;

                            #ifdef _16_BIT_SOFTWARE_RENDERING_
                            *DPtr = pShade[ ( (r >> 11) & SHIFT_15_MASK ) + pText[ (( Ty & SHIFT_23_MASK ) + Tx) >> shiftXaddition ]];
                            #else

                            // Get the textile color
                            textel = pText[ (( Ty & SHIFT_23_MASK ) + Tx) >> shiftXaddition ];

                            // Seperate out each color and
                            // multiply the textels while in there respective places.                            
                            // The formula here is to multiply the color by the intensity,
                            // then divide by the max color amount.
                            // color = (channel * intensity) / (0 to max_colors)
                            // Below is a creative way to do the least amount of shifting
                            // to pack the final color back into 32-bit
                            *DPtr = (((b >> FIX_SHIFT_23_INT) * (textel & 0xFF) & 0xFF00) | 
                                     ((g >> FIX_SHIFT_23_INT) * (textel & 0xFF00) & 0xFF0000) | 
                                     ((r >> FIX_SHIFT_23_INT) * (textel & 0xFF0000) & 0xFF000000)) >> 8;
                            #endif                            
                        }

                        Z += stepZ;
                        r += stepR;
                        #ifndef _16_BIT_SOFTWARE_RENDERING_
                        g += stepG;
                        b += stepB;
                        #endif
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
                    // based on perspective subdivision
                    RZ = Z;

                    // Jump ahead to next subdivision
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

                        if( *ZPtr > Z )
                        {
                            *ZPtr = Z;

                            #ifdef _16_BIT_SOFTWARE_RENDERING_
                            *DPtr = pShade[ ( (r >> 11) & SHIFT_15_MASK ) + pText[ (( Ty & SHIFT_23_MASK ) + Tx) >> shiftXaddition ]];
                            #else

                            // Seperate out each color
                            textel = pText[ (( Ty & SHIFT_23_MASK ) + Tx) >> shiftXaddition ];

                            // Seperate out each color and
                            // multiply the textels while in there respective places.                            
                            // The formula here is to multiply the color by the intensity,
                            // then divide by the max color amount.
                            // color = (channel * intensity) / (0 to max_colors)
                            // Below is a creative way to do the least amount of shifting
                            // to pack the final color back into 32-bit
                            *DPtr = (((b >> FIX_SHIFT_23_INT) * (textel & 0xFF) & 0xFF00) | 
                                     ((g >> FIX_SHIFT_23_INT) * (textel & 0xFF00) & 0xFF0000) | 
                                     ((r >> FIX_SHIFT_23_INT) * (textel & 0xFF0000) & 0xFF000000)) >> 8;
                            #endif
                        }

                        Z += stepZ;
                        r += stepR;
                        #ifndef _16_BIT_SOFTWARE_RENDERING_
                        g += stepG;
                        b += stepB;
                        #endif
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

            rightSeg.X_float += rightSeg.stepX_float;
            ++rightSeg.Y;
            rightSeg.Z += rightSeg.stepZ;
            rightSeg.U += rightSeg.stepU;
            rightSeg.V += rightSeg.stepV;
            rightSeg.r += rightSeg.stepR;
            #ifndef _16_BIT_SOFTWARE_RENDERING_
            rightSeg.g += rightSeg.stepG;
            rightSeg.b += rightSeg.stepB;
            #endif
            --rightSeg.edgeHeight;

            leftSeg.X_float += leftSeg.stepX_float;
            ++leftSeg.Y;
            leftSeg.Z += leftSeg.stepZ;
            leftSeg.U += leftSeg.stepU;
            leftSeg.V += leftSeg.stepV;
            leftSeg.r += leftSeg.stepR;
            #ifndef _16_BIT_SOFTWARE_RENDERING_
            leftSeg.g += leftSeg.stepG;
            leftSeg.b += leftSeg.stepB;
            #endif
            --leftSeg.edgeHeight;

            yIndex += ScrnW;
        }
    }        
}   // RenderClipped


/************************************************************************
*    DESCRIPTION:  RGB shaded texture mapping.
*
*    Input:    PCTextureLib pTextLib - Texture library
************************************************************************/
void _fastcall CPolygon::Render( CTextureLib *pTextLib )
{
    CCeilLine leftSeg, rightSeg;
    int top(0), yIndex, height, xStart, xEnd, width;
    int newRightPos, newLeftPos, runLoops, length;
    int register *ZPtr;
    int *tmpZBuf = giZBuffer;  
    int edgeCount(projCount);
    int ScrnW(giBufWidth), ScrnH(giBufHeight);
    int U, V, RZ, Z, stepU, stepV, stepZ, step, Tx1, Ty1, Tx2, Ty2, offset;
    int r, g, b, stepR, stepG, stepB;
    int subUStep, subVStep, subZStep, TxStep, TyStep, Tx, Ty;
	int textIndex = tIndex;
    
    if( gSoftwareTextIndex > -1 )
	{
    	textIndex = gSoftwareTextIndex;
	}

    unsigned int shiftXaddition = FIX_SHIFT_23_INT - pTextLib->pTMap[textIndex].GetMultiShift();
    unsigned char shiftXadditionByte = (unsigned char)shiftXaddition;
    
    #ifndef _16_BIT_SOFTWARE_RENDERING_
        unsigned int register *DPtr, *pText;
        pText = (unsigned int *)pTextLib->pTMap[textIndex].GetBits();
        unsigned int textel;
        unsigned int *tmpVBuf = (unsigned int *)gusiVBufer;
    #else
        unsigned short int register *DPtr, *pText, *pShade;
        pText = (unsigned short int *)pTextLib->pTMap[textIndex].GetBits();
        unsigned short int *tmpVBuf = (unsigned short int *)gusiVBufer;
        pShade = gusiShadeTbl;
    #endif
    
    // Search for lowest Y coordinate (top of polygon)
    for( int n = 1; n < edgeCount; ++n )
    {
        if( projectPoint[ n ].y < projectPoint[ top ].y )
            top = n;
    }

    // This will initilize faster
    int leftPos(top), rightPos(top);

    // Calculate the index into the video buffer
    yIndex = projectPoint[ top ].y * ScrnW;

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
            rightSeg.Init( projectPoint[ rightPos ], projectPoint[ newRightPos ] );

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
            leftSeg.Init( projectPoint[ leftPos ], projectPoint[ newLeftPos ] );

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
            xStart = leftSeg.X_int >> FIX_SHIFT_20_INT;
            xEnd = rightSeg.X_int >> FIX_SHIFT_20_INT;
            width = xEnd - xStart;

            // Make sure we are within the bounds of the screen
            if( ( width > 0 ) && ( xEnd > 0 ) && ( xStart < ScrnW ) )
            {
                U = leftSeg.U;
                V = leftSeg.V;
                Z = leftSeg.Z;
                r = leftSeg.r;
                #ifndef _16_BIT_SOFTWARE_RENDERING_
                g = leftSeg.g;
                b = leftSeg.b;
                #endif

                // Create the step amounts for the scan line
                stepU = (rightSeg.U - U) / width;
                stepV = (rightSeg.V - V) / width;
                stepZ = (rightSeg.Z - Z) / width;
                stepR = (rightSeg.r - r) / width;
                #ifndef _16_BIT_SOFTWARE_RENDERING_
                stepG = (rightSeg.g - g) / width;
                stepB = (rightSeg.b - b) / width;
                #endif
                  
                // Clip the scan-line
                if( xStart < 0 )
                {
                    step = -xStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += stepZ * step;
                    U += stepU * step;
                    V += stepV * step;
                    r += stepR * step;
                    #ifndef _16_BIT_SOFTWARE_RENDERING_
                    g += stepG * step;
                    b += stepB * step;
                    #endif
                    
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
                        
                        if( *ZPtr > Z )
                        {
                            *ZPtr = Z;

                            #ifdef _16_BIT_SOFTWARE_RENDERING_
                            *DPtr = pShade[ ( r & SHIFT_15_MASK ) + pText[ (( Ty & SHIFT_23_MASK ) + Tx) >> shiftXaddition ]];
                            #else

                            // Get the textile color
                            textel = pText[ (( Ty & SHIFT_23_MASK ) + Tx) >> shiftXaddition ];

                            // Seperate out each color and
                            // multiply the textels while in there respective places.                            
                            // The formula here is to multiply the color by the intensity,
                            // then divide by the max color amount.
                            // color = (channel * intensity) / (0 to max_colors)
                            // Below is a creative way to do the least amount of shifting
                            // to pack the final color back into 32-bit
                            *DPtr = (((b >> FIX_SHIFT_16_INT) * (textel & 0xFF) & 0xFF00) | 
                                     ((g >> FIX_SHIFT_16_INT) * (textel & 0xFF00) & 0xFF0000) | 
                                     ((r >> FIX_SHIFT_16_INT) * (textel & 0xFF0000) & 0xFF000000)) >> 8;
                            #endif                            
                        }

                        Z += stepZ;
                        r += stepR;
                        #ifndef _16_BIT_SOFTWARE_RENDERING_
                        g += stepG;
                        b += stepB;
                        #endif
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

                        if( *ZPtr > Z )
                        {
                            *ZPtr = Z;

                            #ifdef _16_BIT_SOFTWARE_RENDERING_
                            *DPtr = pShade[ ( r & SHIFT_15_MASK ) + pText[ (( Ty & SHIFT_23_MASK ) + Tx) >> shiftXaddition ]];
                            #else

                            // Seperate out each color
                            textel = pText[ (( Ty & SHIFT_23_MASK ) + Tx) >> shiftXaddition ];

                            // Seperate out each color and
                            // multiply the textels while in there respective places.                            
                            // The formula here is to multiply the color by the intensity,
                            // then divide by the max color amount.
                            // color = (channel * intensity) / (0 to max_colors)
                            // Below is a creative way to do the least amount of shifting
                            // to pack the final color back into 32-bit
                            *DPtr = (((b >> FIX_SHIFT_16_INT) * (textel & 0xFF) & 0xFF00) | 
                                     ((g >> FIX_SHIFT_16_INT) * (textel & 0xFF00) & 0xFF0000) | 
                                     ((r >> FIX_SHIFT_16_INT) * (textel & 0xFF0000) & 0xFF000000)) >> 8;
                            #endif
                        }

                        Z += stepZ;
                        r += stepR;
                        #ifndef _16_BIT_SOFTWARE_RENDERING_
                        g += stepG;
                        b += stepB;
                        #endif
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

            rightSeg.X_int += rightSeg.stepX_int;
            ++rightSeg.Y;
            rightSeg.Z += rightSeg.stepZ;
            rightSeg.U += rightSeg.stepU;
            rightSeg.V += rightSeg.stepV;
            rightSeg.r += rightSeg.stepR;
            #ifndef _16_BIT_SOFTWARE_RENDERING_
            rightSeg.g += rightSeg.stepG;
            rightSeg.b += rightSeg.stepB;
            #endif
            --rightSeg.edgeHeight;

            leftSeg.X_int += leftSeg.stepX_int;
            ++leftSeg.Y;
            leftSeg.Z += leftSeg.stepZ;
            leftSeg.U += leftSeg.stepU;
            leftSeg.V += leftSeg.stepV;
            leftSeg.r += leftSeg.stepR;
            #ifndef _16_BIT_SOFTWARE_RENDERING_
            leftSeg.g += leftSeg.stepG;
            leftSeg.b += leftSeg.stepB;
            #endif
            --leftSeg.edgeHeight;

            yIndex += ScrnW;
        }
    }        
}   // Render


/************************************************************************
*    DESCRIPTION:  RGB shaded texture mapping.
*
*    Input:    PCTextureLib pTextLib - Texture library
************************************************************************/
void _fastcall CPolygon::RenderAlpha( CTextureLib *pTextLib )
{
    CCeilLine leftSeg, rightSeg;
    int top(0), yIndex, height, xStart, xEnd, width;
    int newRightPos, newLeftPos, runLoops, length;
    int register *ZPtr;
    int *tmpZBuf = giZBuffer;  
    int edgeCount(projCount);
    int ScrnW(giBufWidth), ScrnH(giBufHeight);
    int U, V, RZ, Z, stepU, stepV, stepZ, step, Tx1, Ty1, Tx2, Ty2, offset;
    int r, g, b, stepR, stepG, stepB;
    int subUStep, subVStep, subZStep, TxStep, TyStep, Tx, Ty;
	int textIndex = tIndex;
    
    if( gSoftwareTextIndex > -1 )
	{
    	textIndex = gSoftwareTextIndex;
	}

    unsigned int shiftXaddition = FIX_SHIFT_23_INT - pTextLib->pTMap[textIndex].GetMultiShift();
    unsigned char shiftXadditionByte = (unsigned char)shiftXaddition;
    unsigned int alphaFix, dest, destB, destG, destR;
    
    unsigned int register *DPtr, *pText;
    pText = (unsigned int *)pTextLib->pTMap[textIndex].GetBits();
    unsigned int textel;
    unsigned int *tmpVBuf = (unsigned int *)gusiVBufer;
    
    // Search for lowest Y coordinate (top of polygon)
    for( int n = 1; n < edgeCount; ++n )
    {
        if( projectPoint[ n ].y < projectPoint[ top ].y )
            top = n;
    }

    // This will initilize faster
    int leftPos(top), rightPos(top);

    // Calculate the index into the video buffer
    yIndex = projectPoint[ top ].y * ScrnW;

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
            rightSeg.Init( projectPoint[ rightPos ], projectPoint[ newRightPos ] );

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
            leftSeg.Init( projectPoint[ leftPos ], projectPoint[ newLeftPos ] );

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
            xStart = leftSeg.X_int >> FIX_SHIFT_20_INT;
            xEnd = rightSeg.X_int >> FIX_SHIFT_20_INT;
            width = xEnd - xStart;

            // Make sure we are within the bounds of the screen
            if( ( width > 0 ) && ( xEnd > 0 ) && ( xStart < ScrnW ) )
            {
                U = leftSeg.U;
                V = leftSeg.V;
                Z = leftSeg.Z;
                r = leftSeg.r;
                g = leftSeg.g;
                b = leftSeg.b;

                // Create the step amounts for the scan line
                stepU = (rightSeg.U - U) / width;
                stepV = (rightSeg.V - V) / width;
                stepZ = (rightSeg.Z - Z) / width;
                stepR = (rightSeg.r - r) / width;
                stepG = (rightSeg.g - g) / width;
                stepB = (rightSeg.b - b) / width;
                  
                // Clip the scan-line
                if( xStart < 0 )
                {
                    step = -xStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += stepZ * step;
                    U += stepU * step;
                    V += stepV * step;
                    r += stepR * step;
                    g += stepG * step;
                    b += stepB * step;
                    
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
                        
                        if( *ZPtr > Z )
                        {
                            *ZPtr = Z;

                            // Seperate out each color
                            textel = pText[ (( Ty & SHIFT_23_MASK ) + Tx) >> shiftXaddition ];

                            // get the fixed value alpha
                            alphaFix = (textel & 0xFF000000) >> 24;
                            dest = *DPtr;

                            // multiply the alpha by the destination color
                            destB = ((dest & 0xFF) * alphaFix) >> FIX_SHIFT_6_INT;
                            destG = ((dest & 0xFF00) * alphaFix) >> FIX_SHIFT_6_INT;
                            destR = ((dest & 0xFF0000) * alphaFix) >> FIX_SHIFT_6_INT;

                            // Seperate out each color and
                            // multiply the textels while in there respective places.                            
                            // The formula here is to multiply the color by the intensity,
                            // then divide by the max color amount.
                            // color = (channel * intensity) / (0 to max_colors)
                            // Below is a creative way to do the least amount of shifting
                            // to pack the final color back into 32-bit
                            *DPtr = (((b >> FIX_SHIFT_16_INT) * ((textel & 0xFF) + destB) & 0xFF00) | 
                                     ((g >> FIX_SHIFT_16_INT) * ((textel & 0xFF00) + destG) & 0xFF0000) | 
                                     ((r >> FIX_SHIFT_16_INT) * ((textel & 0xFF0000) + destR) & 0xFF000000)) >> 8;                          
                        }

                        Z += stepZ;
                        r += stepR;
                        g += stepG;
                        b += stepB;
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

                        if( *ZPtr > Z )
                        {
                            *ZPtr = Z;

                            // Seperate out each color
                            textel = pText[ (( Ty & SHIFT_23_MASK ) + Tx) >> shiftXaddition ];

                            // get the fixed value alpha
                            alphaFix = (textel & 0xFF000000) >> 24;
                            dest = *DPtr;

                            // multiply the alpha by the destination color
                            destB = ((dest & 0xFF) * alphaFix) >> FIX_SHIFT_6_INT;
                            destG = ((dest & 0xFF00) * alphaFix) >> FIX_SHIFT_6_INT;
                            destR = ((dest & 0xFF0000) * alphaFix) >> FIX_SHIFT_6_INT;

                            // Seperate out each color and
                            // multiply the textels while in there respective places.                            
                            // The formula here is to multiply the color by the intensity,
                            // then divide by the max color amount.
                            // color = (channel * intensity) / (0 to max_colors)
                            // Below is a creative way to do the least amount of shifting
                            // to pack the final color back into 32-bit
                            *DPtr = (((b >> FIX_SHIFT_16_INT) * ((textel & 0xFF) + destB) & 0xFF00) | 
                                     ((g >> FIX_SHIFT_16_INT) * ((textel & 0xFF00) + destG) & 0xFF0000) | 
                                     ((r >> FIX_SHIFT_16_INT) * ((textel & 0xFF0000) + destR) & 0xFF000000)) >> 8;

                        }

                        Z += stepZ;
                        r += stepR;
                        g += stepG;
                        b += stepB;
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

            rightSeg.X_int += rightSeg.stepX_int;
            ++rightSeg.Y;
            rightSeg.Z += rightSeg.stepZ;
            rightSeg.U += rightSeg.stepU;
            rightSeg.V += rightSeg.stepV;
            rightSeg.r += rightSeg.stepR;
            rightSeg.g += rightSeg.stepG;
            rightSeg.b += rightSeg.stepB;
            --rightSeg.edgeHeight;

            leftSeg.X_int += leftSeg.stepX_int;
            ++leftSeg.Y;
            leftSeg.Z += leftSeg.stepZ;
            leftSeg.U += leftSeg.stepU;
            leftSeg.V += leftSeg.stepV;
            leftSeg.r += leftSeg.stepR;
            leftSeg.g += leftSeg.stepG;
            leftSeg.b += leftSeg.stepB;
            --leftSeg.edgeHeight;

            yIndex += ScrnW;
        }
    }        
}   // RenderAlpha


/************************************************************************
*    DESCRIPTION:  RGB shaded texture mapping. Do some different things
*                  because this polygon was clipped to the near Z plane
*                  and we need more precision.
*
*    Input:    PCTextureLib pTextLib - Texture library
************************************************************************/
void _fastcall CPolygon::RenderClippedAlpha( CTextureLib *pTextLib )
{
    CCeilLine leftSeg, rightSeg;
    int top(0), yIndex, height, xStart, xEnd, width;
    int newRightPos, newLeftPos, runLoops, length;
    int register *ZPtr;
    int *tmpZBuf = giZBuffer;  
    int edgeCount(projCount);
    int ScrnW(giBufWidth), ScrnH(giBufHeight);
    int U, V, RZ, Z, stepU, stepV, stepZ, step, Tx1, Ty1, Tx2, Ty2, offset;
    int r, g, b, stepR, stepG, stepB;
    int subUStep, subVStep, subZStep, TxStep, TyStep, Tx, Ty;
	int textIndex = tIndex;
    
    if( gSoftwareTextIndex > -1 )
	{
    	textIndex = gSoftwareTextIndex;
	}

    unsigned int shiftXaddition = FIX_SHIFT_23_INT - pTextLib->pTMap[textIndex].GetMultiShift();
    unsigned char shiftXadditionByte = (unsigned char)shiftXaddition;
    unsigned int alphaFix, dest, destB, destG, destR;

    unsigned int register *DPtr, *pText;
    pText = (unsigned int *)pTextLib->pTMap[textIndex].GetBits();
    unsigned int textel;
    unsigned int *tmpVBuf = (unsigned int *)gusiVBufer;

    
    // Search for lowest Y coordinate (top of polygon)
    for( int n = 1; n < edgeCount; ++n )
    {
        if( projectPoint[ n ].y < projectPoint[ top ].y )
            top = n;
    }

    // This will initilize faster
    int leftPos(top), rightPos(top);

    // Calculate the index into the video buffer
    yIndex = projectPoint[ top ].y * ScrnW;

    // Loop for all polygon edges
    while( edgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing
        if( rightSeg.edgeHeight < 1 )
        {
            newRightPos = rightPos + 1;

            if( newRightPos >= projCount )
                newRightPos = 0;

            // Init the right side of the triangle
            rightSeg.Init( projectPoint[ rightPos ], projectPoint[ newRightPos ] );

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
        // (re)initializing
        if( leftSeg.edgeHeight < 1 )
        {
            newLeftPos = leftPos - 1;

            if( newLeftPos < 0 )
                newLeftPos = ( projCount - 1 );

            // Init the left side of the triangle
            leftSeg.Init( projectPoint[ leftPos ], projectPoint[ newLeftPos ] );

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

        // Subdivide polygon into trapezoid
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

        // Loop for the height of the trapezoid
        while( height-- > 0 )
        {
            // Calculate initial values
            xStart = leftSeg.X_float;
            xEnd = rightSeg.X_float;
            width = xEnd - xStart;

            // Make sure we are within the bounds of the screen
            if( ( width > 0 ) && ( xEnd > 0 ) && ( xStart < ScrnW ) )
            {
                U = leftSeg.U;
                V = leftSeg.V;
                Z = leftSeg.Z;
                r = leftSeg.r;
                g = leftSeg.g;
                b = leftSeg.b;

                // Create the step amounts for the scan line
                stepU = (rightSeg.U - U) / width;
                stepV = (rightSeg.V - V) / width;
                stepZ = (rightSeg.Z - Z) / width;
                stepR = (rightSeg.r - r) / width;
                stepG = (rightSeg.g - g) / width;
                stepB = (rightSeg.b - b) / width;
                  
                // Clip the scan-line
                if( xStart < 0 )
                {
                    step = -xStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += stepZ * step;
                    U += stepU * step;
                    V += stepV * step;
                    r += stepR * step;
                    g += stepG * step;
                    b += stepB * step;
                    
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
                    // Premultiply steps for subdivision
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

                // Loop for number of subdivisions
                for( int count = 0; count < runLoops; ++count )
                {
                    length = RUN_LENGTH_16;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision
                    RZ = Z;

                    // Jump ahead to next subdivision
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
                        
                        if( *ZPtr > Z )
                        {
                            *ZPtr = Z;

                            // Get the textile color
                            textel = pText[ (( Ty & SHIFT_23_MASK ) + Tx) >> shiftXaddition ];

                            // get the fixed value alpha
                            alphaFix = (textel & 0xFF000000) >> 24;
                            dest = *DPtr;

                            // multiply the alpha by the destination color
                            destB = ((dest & 0xFF) * alphaFix) >> FIX_SHIFT_6_INT;
                            destG = ((dest & 0xFF00) * alphaFix) >> FIX_SHIFT_6_INT;
                            destR = ((dest & 0xFF0000) * alphaFix) >> FIX_SHIFT_6_INT;

                            // Seperate out each color and
                            // multiply the textels while in there respective places.                            
                            // The formula here is to multiply the color by the intensity,
                            // then divide by the max color amount.
                            // color = (channel * intensity) / (0 to max_colors)
                            // Below is a creative way to do the least amount of shifting
                            // to pack the final color back into 32-bit
                            *DPtr = (((b >> FIX_SHIFT_23_INT) * ((textel & 0xFF) + destB) & 0xFF00) | 
                                     ((g >> FIX_SHIFT_23_INT) * ((textel & 0xFF00) + destG) & 0xFF0000) | 
                                     ((r >> FIX_SHIFT_23_INT) * ((textel & 0xFF0000) + destR) & 0xFF000000)) >> 8;                            
                        }

                        Z += stepZ;
                        r += stepR;
                        g += stepG;
                        b += stepB;
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
                    // based on perspective subdivision
                    RZ = Z;

                    // Jump ahead to next subdivision
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

                        if( *ZPtr > Z )
                        {
                            *ZPtr = Z;

                            // Seperate out each color
                            textel = pText[ (( Ty & SHIFT_23_MASK ) + Tx) >> shiftXaddition ];

                            // get the fixed value alpha
                            alphaFix = (textel & 0xFF000000) >> 24;
                            dest = *DPtr;

                            // multiply the alpha by the destination color
                            destB = ((dest & 0xFF) * alphaFix) >> FIX_SHIFT_6_INT;
                            destG = ((dest & 0xFF00) * alphaFix) >> FIX_SHIFT_6_INT;
                            destR = ((dest & 0xFF0000) * alphaFix) >> FIX_SHIFT_6_INT;

                            // Seperate out each color and
                            // multiply the textels while in there respective places.                            
                            // The formula here is to multiply the color by the intensity,
                            // then divide by the max color amount.
                            // color = (channel * intensity) / (0 to max_colors)
                            // Below is a creative way to do the least amount of shifting
                            // to pack the final color back into 32-bit
                            *DPtr = (((b >> FIX_SHIFT_23_INT) * ((textel & 0xFF) + destB) & 0xFF00) | 
                                     ((g >> FIX_SHIFT_23_INT) * ((textel & 0xFF00) + destG) & 0xFF0000) | 
                                     ((r >> FIX_SHIFT_23_INT) * ((textel & 0xFF0000) + destR) & 0xFF000000)) >> 8;
                        }

                        Z += stepZ;
                        r += stepR;
                        g += stepG;
                        b += stepB;
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

            rightSeg.X_float += rightSeg.stepX_float;
            ++rightSeg.Y;
            rightSeg.Z += rightSeg.stepZ;
            rightSeg.U += rightSeg.stepU;
            rightSeg.V += rightSeg.stepV;
            rightSeg.r += rightSeg.stepR;
            rightSeg.g += rightSeg.stepG;
            rightSeg.b += rightSeg.stepB;
            --rightSeg.edgeHeight;

            leftSeg.X_float += leftSeg.stepX_float;
            ++leftSeg.Y;
            leftSeg.Z += leftSeg.stepZ;
            leftSeg.U += leftSeg.stepU;
            leftSeg.V += leftSeg.stepV;
            leftSeg.r += leftSeg.stepR;
            leftSeg.g += leftSeg.stepG;
            leftSeg.b += leftSeg.stepB;
            --leftSeg.edgeHeight;

            yIndex += ScrnW;
        }
    }        
}   // RenderClippedAlpha