
/************************************************************************
*    FILE NAME:       xvertexbuffer.h
*
*    DESCRIPTION:     DirectX vertex buffer
************************************************************************/  

#ifndef __x_vertex_buffer_h__
#define __x_vertex_buffer_h__

// Standard lib dependencies
#include <vector>
#include <string>

// DirectX lib dependencies
#include <d3dx9.h>

// Game lib dependencies
#include <utilities/deletefuncs.h>
#include <common/texture.h>
#include <common/defs.h>

class CXVertBuff
{
public:

    CXVertBuff() : pVertBuf(NULL), fcount(0)
    { }

    CXVertBuff( const CXVertBuff &obj )
    { *this = obj; }

    ~CXVertBuff()
    { NDelFunc::Release(pVertBuf); }


    /************************************************************************                                                             
    *    desc:  Get the texture list
    ************************************************************************/
    NText::CMaterial & GetMaterial()
    {
        return material;
    }

    /************************************************************************                                                             
    *    desc:  Get the texture count
    ************************************************************************/
    size_t GetTextCount() const
    {
        return material.textureLstVec.size();
    }

    /************************************************************************                                                             
    *    desc:  Set the texture indexes
    ************************************************************************/
    void SetTexture( NText::CTextureFor3D & texture )
    {
        // Make sure the defuse texture is always first
        if( texture.type == NText::ETT_DIFFUSE )
        {
            material.textureLstVec.insert( material.textureLstVec.begin(), texture );
        }
        else
            material.textureLstVec.push_back(texture);
    }

    /************************************************************************                                                             
    *    desc:  Set the face count
    ************************************************************************/
    void SetFaceCount(int value)
    {
        fcount = value;
    }

    /************************************************************************                                                             
    *    desc:  Get the face count
    ************************************************************************/
    uint GetFaceCount() const
    {
        return fcount;
    }
    
    /************************************************************************                                                             
    *    desc:  Get the vert buffer
    ************************************************************************/
    LPDIRECT3DVERTEXBUFFER9 GetVertBuffer()
    {
        return pVertBuf;
    }

    /************************************************************************                                                             
    *    desc:  Get the pointer to the pointer of the vert buffer
    ************************************************************************/
    LPDIRECT3DVERTEXBUFFER9 * GetDblPtrVertBuffer()
    {
        return &pVertBuf;
    }

    /************************************************************************                                                             
    *    desc:  Set the vert buffer
    ************************************************************************/
    void SetVertBuffer(LPDIRECT3DVERTEXBUFFER9 pPointer)
    {
        pVertBuf = pPointer;
    }

private:

    // Vertex buffer
    LPDIRECT3DVERTEXBUFFER9 pVertBuf;

    // face count
    unsigned int fcount;

    // Material group
    NText::CMaterial material;
};

#endif  // __x_vertex_buffer_h__

