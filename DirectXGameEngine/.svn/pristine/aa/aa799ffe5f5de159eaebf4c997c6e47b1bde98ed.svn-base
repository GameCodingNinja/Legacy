
/************************************************************************
*    FILE NAME:       xvertexshadowbuffer.h
*
*    DESCRIPTION:     DirectX vertex shadow buffer
************************************************************************/  

#ifndef __x_vertex_shadow_buffer_h__
#define __x_vertex_shadow_buffer_h__

// DirectX lib dependencies
#include "d3dx9.h"

// Game lib dependencies
#include "xshadowface.h" // not used here directly but will be needed
                         // where this class is used

class CXVertShadowBuff
{
public:

    CXVertShadowBuff() : xVertBuf(NULL), fcount(0), backCapRadius(0.0f)
    {
    }

    CXVertShadowBuff( const CXVertShadowBuff &obj )
    {
        *this = obj;
    }

    ~CXVertShadowBuff()
    {
        if( NULL != xVertBuf )
        {
            xVertBuf->Release();
        }

        xVertBuf = NULL;
        fcount = 0;
    }

    // Vertex buffer
    LPDIRECT3DVERTEXBUFFER9 xVertBuf;

    // face count
    unsigned int fcount;

    // back center radius of the shadow volume
    CPoint center;

    // Radius of the back cap
    float backCapRadius;

};

#endif  // __x_vertex_shadow_buffer_h__

