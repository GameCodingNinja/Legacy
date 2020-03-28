/************************************************************************
*    FILE NAME:       xshadowface.h
*
*    DESCRIPTION:     3D DirectX shadow face class
************************************************************************/  

#ifndef __x_shadow_face_h__
#define __x_shadow_face_h__

#include "point.h"

class CXShadowFace
{
public:

    CXShadowFace(){};
    CXShadowFace( const CXShadowFace &obj )
    {
        *this = obj;
    }

    // Verts
    CPoint vert[3];

};

#endif  // __x_shadow_face_h__