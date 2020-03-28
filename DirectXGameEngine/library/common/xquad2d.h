
/************************************************************************
*    FILE NAME:       xquad2d.h
*
*    DESCRIPTION:     2d DirectX quad class used for creating 2d sprites
************************************************************************/  

#ifndef __x_quad_2d_h__
#define __x_quad_2d_h__

// Game lib dependencies
#include <common/xvert2d.h>

class CXQuad2D
{
public:

    // Constructors
    CXQuad2D(){}

    CXQuad2D( float width, float height )
    {
        vert[0].x   = -0.5f;
        vert[0].y   = -0.5f;
        vert[0].z   =  1.0f;
        vert[0].rhw =  1.0f;
        vert[0].u   =  0.0f;
        vert[0].v   =  0.0f;

        vert[1].x   = width-0.5f;
        vert[1].y   = -0.5f;
        vert[1].z   =  1.0f;
        vert[1].rhw =  1.0f;
        vert[1].u   =  1.0f;
        vert[1].v   =  0.0f;

        vert[2].x   = -0.5f;
        vert[2].y   = height-0.5f;
        vert[2].z   =  1.0f;
        vert[2].rhw =  1.0f;
        vert[2].u   =  0.0f;
        vert[2].v   =  1.0f;

        vert[3].x   = width-0.5f;
        vert[3].y   = height-0.5f;
        vert[3].z   =  1.0f;
        vert[3].rhw =  1.0f;
        vert[3].u   =  1.0f;
        vert[3].v   =  1.0f;
    }

    // Quad defined by 4 2D verts
    CXVert2D vert[4];

};

#endif  // __x_quad_2d_h__
