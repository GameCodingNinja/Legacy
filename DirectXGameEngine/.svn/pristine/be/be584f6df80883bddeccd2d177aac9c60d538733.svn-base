/************************************************************************
*    FILE NAME:       mathfunc.h
*
*    DESCRIPTION:     Math utility functions
************************************************************************/  

#ifndef __math_func_2d_h__
#define __math_func_2d_h__

// Game lib dependencies
#include <common/point.h>
#include <common/worldpoint.h>

namespace NMathFunc
{
    // 2D dot product
    float DotProduct2D( const CPoint & p1, const CPoint & p2 );
    CWorldValue DotProduct2D( const CWorldPoint & p1, const CWorldPoint & p2 );
    CWorldValue DotProduct2D( const CWorldPoint & p1, const CPoint & p2 );

    // 2D cross products
    float CrossProduct2D( const CPoint & p1, const CPoint & p2 );
    CPoint CrossProduct2D( const CPoint & p, float s );
    CPoint CrossProduct2D( float s, const CPoint & p );

    // Perform a modulus operation on the passed in floats
    float Modulus( float v1, float v2 );
}

#endif  // __math_func_2d_h__

