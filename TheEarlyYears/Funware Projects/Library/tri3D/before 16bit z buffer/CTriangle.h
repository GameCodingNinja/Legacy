/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CTriangle.h
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

#ifndef __CTRIANGLE_H__
#define __CTRIANGLE_H__

#include "CPoint.h"      // class holding all the point classes
#include "CTextLib.h"    // Texture library class
#include "CLight.h"          // Header file for this *.cpp file.

#define MAX_CLIPPED_POINTS   4

#define FIX_SHIFT_26_FLOAT   0x4000000   // Float to fix multiplier amount
#define FIX_SHIFT_26_INT     26          // Fix to int shift right amount

#define FIX_SHIFT_23_FLOAT   0x800000    // Float to fix multiplier amount
#define FIX_SHIFT_23_INT     23          // Fix to int shift right amount
#define SHIFT_23_MASK        0xFF800000  // Mask out the top 9 bits for a 32 bit shift int

#define FIX_SHIFT_20_INT     20
#define CEIL_FRACT           (FIX_SHIFT_20_INT-1)
#define FIX_SHIFT_16_FLOAT   0x10000
#define FIX_SHIFT_16_INT     16
#define FIX_SHIFT_15_FLOAT   0x8000
#define FIX_SHIFT_15_INT     15
#define DIVIDE_BY_16_SHIFT   4
#define RUN_LENGTH_16        16

//Tx1 = ((double)U / (double)Z) * FIX_SHIFT_23_FLOAT;
//Ty1 = ((double)V / (double)Z) * FIX_SHIFT_23_FLOAT;
// Below replaces above

#define fixdiv( Result, Num, Denom, Prec )\
  { _asm { mov  eax, Num }\
    _asm { cdq }\
    _asm { mov  cl, Prec }\
    _asm { shld  edx, eax, cl }\
    _asm { sal  eax, cl }\
    _asm { idiv Denom }\
    _asm { mov  Result, eax } }


class CTriangle
{
protected:
    // triangle projected point count
    int projCount;

    // Project the 3D points to 2D points
    void Project( int endI );
    void Project( int outC, CPoint3D &clippedPoint );

public:

    // 3 pointers to a vertex list
    CPoint3D *pVPoint[3];
    // Projected flag pointer
    bool *pfProjected[3];
    // 2D projected points
    CPoint2D point2D[MAX_CLIPPED_POINTS];
    // Pointer to the normal
    CPoint *pNormal;
    // UNI and vertex normal
    CUVI uvi[3];
    // Texture Index
    int tIndex;
    // radius of triangle
    float radius;

    CTriangle();
    ~CTriangle();

    // Clip and project the 3D point vertices
    void _fastcall ClipProjectXYZ();
    // Clip and project the UVI
    void _fastcall ClipProjectUVI();
    // Clip and project the I
    void _fastcall ClipProjectI();
    // Check if the poly is visible. Perform 3D culling
    bool _fastcall CalcVisible3D( float cullPercent );
    // Check if the poly is visible. Perform dD culling
    bool _fastcall CalcVisible2D();
    // Calculate a bounding sphere around the triangle
    void CalcBoundingTri();
    // Calculate the Cross Product which is used to give us our normal
    void CalcSurfaceNormal();
    // Calculate light intensity
    void _fastcall CalcLightInten( CLightList *pLightList, int intensityCap, int intensityShift );

    ///////////////////////////////////
    //  16 bit Texture Mapping
    ///////////////////////////////////
    // texture Grouraud shading.
    void _fastcall Render16_ZBufShading( PCTextureLib pTextLib );
    // light Grouraud chading
    void _fastcall Render16_LiteZBufShading( unsigned short int color );
    // Z Buffered wireframe rendering, and grouraud shading.
    void _fastcall Render16_WireFrame( PCTextureLib pTextLib );

    // Is the supplied vertice the same
    inline bool HasVert( CPoint3D &P )
    {
        if( ( *pVPoint[0] ) == P )
           return true;
        if( ( *pVPoint[1] ) == P )
           return true;
        if( ( *pVPoint[2] ) == P )
           return true;

        return false;
    }
};


/************************************************************************
*
*    CeilLine class for plotting your way down the edge of the triangle
*
************************************************************************/

class CCeilLine
{
public:
    int X, Y, Z, U, V, I, x1, x2, y1, y2;
    int stepX, stepZ, stepI, stepU, stepV, edgeHeight;

    // The constructor
    CCeilLine():x1(0), x2(0), y1(0), y2(0), edgeHeight(0), X(0), Y(0), Z(0),
    U(0), V(0), stepX(0), stepZ(0), stepU(0), stepV(0),
    I(0), stepI(0)
    {
    }

    // Init the class with the two points that form a line down the triangle
    inline void CCeilLine::Init( CPoint2D &point2D_Pos, CPoint2D &point2D_newPos )
    {
        // The two points that makeup a line
        x1 = point2D_Pos.x;
        x2 = point2D_newPos.x;
        y1 = point2D_Pos.y;
        y2 = point2D_newPos.y;

        // Height of the two points
        edgeHeight = ( y2 - y1 );

        // Push the X to the Ceiling of the number so that
        // the number isn't truncated.
        X = (x1 << FIX_SHIFT_20_INT) + CEIL_FRACT;
        Y = y1;
        Z = point2D_Pos.z;

        // Store U/Z and V/Z values:
        U = point2D_Pos.U;
        V = point2D_Pos.V;

        // Store color intensity value:
        I = point2D_Pos.I;

        if( edgeHeight )
        {
            // Create the step amount
            stepX = (( x2 - x1 ) << FIX_SHIFT_20_INT) / edgeHeight;
            stepZ = ( point2D_newPos.z - point2D_Pos.z ) / edgeHeight;

            // Calculate U/Z and V/Z deltas
            stepU = ( point2D_newPos.U - point2D_Pos.U ) / edgeHeight;
            stepV = ( point2D_newPos.V - point2D_Pos.V ) / edgeHeight;

            // Calculate color intensity delta                
            stepI = ( point2D_newPos.I - point2D_Pos.I ) / edgeHeight;
        }
        else
        {
            stepX = 0;
            stepZ = 0;
            stepU = 0;
            stepV = 0;
            stepI = 0;
        }
    }   // init

    // Perform object-precision clip on top of edge. 
    // Is the top edge of the polygon above the Y of the screen?
    // If so, recalculate the X offset and set the Y to 0
    inline void CCeilLine::ClipTop()
    {
        // Save the offset of the y that is off screen
        float step( -Y );

        // calculate a new x offset based on the y being zero
        X = ( x1 << FIX_SHIFT_20_INT ) + 
            ( ( int ) ( (( double ) ( x2 - x1 ) / 
            ( double ) (y2 - y1)) * 
            ( double ) step ) << FIX_SHIFT_20_INT ) + CEIL_FRACT;

        // Set to zero
        Y = 0;
        Z += stepZ * step;
        U += stepU * step;
        V += stepV * step;
        I += stepI * step;

        // Take the current edge height and subtract the
        // original offscreen y offset
        edgeHeight -= step;

    }   // ClipTop
};

#endif // 