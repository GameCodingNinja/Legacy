
/************************************************************************
*    FILE NAME:       face.h
*
*    DESCRIPTION:     3D face class
************************************************************************/  

#ifndef __face_h__
#define __face_h__

// Game lib dependencies
#include "common\\point.h"
#include "common\\normal.h"

class CFace
{
public:

    CFace();
    ~CFace();

    // Calculate the Surface Normal
    void CalcSurfaceNormal();

    // Calculate the center point of the polygon
    void CalcPolyCenterPoint();

    // Calculate a bounding sphere around the polygon face
    void CalcBoundingPoly();

    // Get the angle of the point to the plane
    float PointToPlaneDistance( const CPoint & point );

    // Is this point facing the plane
    bool IsFacingPlane( const CPoint & point );

    // Get the edge of the face
    //void GetEdge( int index, CEdge & edge );

    // Check if a given point is within a floor face
    bool PointInFloor( const CPoint & point );

    // Check if a given point is within a wall face
    bool PointInWall( const CPoint & point );

    // Verts
    CPoint vert[3];

    // Verts
    CPoint center;

    // radius
    float radius;

    // Surface normal
    CNormal normal;
};

#endif  // __cface_h__

