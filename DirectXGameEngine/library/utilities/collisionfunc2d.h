/************************************************************************
*    FILE NAME:       collisionfunc2d.h
*
*    DESCRIPTION:     2D collision functions
************************************************************************/  

#ifndef __collisionfunc_2d_h__
#define __collisionfunc_2d_h__

// Game lib dependencies
#include <common/point.h>
#include <common/pointint.h>
#include <common/worldpoint.h>

namespace NCollisionFunc2D
{
    //float SqaredDistance( const CPoint & p0, const CPoint & p1 );
    bool VectorsFacing( const CPoint & v0, const CPoint & v1 );

    bool SquaredRadiiIntersect( const CPoint & p0, float r0, CPoint & p1, float r1 );
    bool SqrtRadiiIntersect( const CPoint & p0, float r0, CPoint & p1, float r1 );
    bool BoxRadiiIntersect( const CPoint & p0, float r0, CPoint & p1, float r1 );
    bool BoxRadiiIntersect( const CWorldPoint & p0, float r0, const CWorldPoint & p1, float r1 );

    bool PointInRect( const CPoint & point, float top, float bottom, float left, float right );
    bool PointInRect( const CPointInt & point, int top, int bottom, int left, int right );

}

#endif  // __collisionfunc_2d_h__

