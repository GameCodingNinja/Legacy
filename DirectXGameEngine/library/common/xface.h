
/************************************************************************
*    FILE NAME:       xface.h
*
*    DESCRIPTION:     3D DirectX face class
************************************************************************/  

#ifndef __x_face_h__
#define __x_face_h__

#include <common/vertex.h>

class CXFace
{
public:

    CXFace(){};
    CXFace( const CXFace &obj )
    {
        *this = obj;
    }

    // Verts
    CVertex vert[3];

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

#endif  // __x_face_h__

