
/************************************************************************
*    FILE NAME:       xjface.h
*
*    DESCRIPTION:     3D DirectX face class
************************************************************************/  

#ifndef __x_j_face_h__
#define __x_j_face_h__

#include <common/jvertex.h>

class CXJFace
{
public:

    CXJFace(){};
    CXJFace( const CXJFace &obj )
    {
        *this = obj;
    }

    // Verts
    CJVertex vert[3];

    /************************************************************************
    *    desc:  Calculate the center point of the polygon
    ************************************************************************/
    float GetCenterZ() const
    {
        float centerZ;

        centerZ = vert[0].vert.z;
        centerZ += vert[1].vert.z;
        centerZ += vert[2].vert.z;

        centerZ /= 3;

        return centerZ;

    }	// GetCenterZ

};

#endif  // __x_j_face_h__

