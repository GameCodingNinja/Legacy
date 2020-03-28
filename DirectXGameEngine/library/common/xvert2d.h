
/************************************************************************
*    FILE NAME:       xvert2d.h
*
*    DESCRIPTION:     2d DirectX vert class used for creating 2d sprites
************************************************************************/  

#ifndef __x_vert_2d_h__
#define __x_vert_2d_h__


class CXVert2D
{
public:

    CXVert2D() : x(0), y(0), z(0), rhw(0), u(0), v(0)
    {}

    // rhw is for defining a vertex position in 2D space
    float x, y, z, rhw;

    // Texcoord
    float u, v;
};

#endif  // __x_vert_2d_h__
