/************************************************************************
*    FILE NAME:       texture.h
*
*    DESCRIPTION:     classes for keeping track of the textures
************************************************************************/  

#ifndef __texture_h__
#define __texture_h__

// Windows lib dependencies
#include <atlbase.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <utilities/deletefuncs.h>
#include <common/size.h>

namespace NText
{
    // Texture type
    enum ETextureType
    {
        ETT_NULL=0,
        ETT_DIFFUSE,
        ETT_NORMAL,
        ETT_SPECULAR,
        ETT_DISPLACEMENT,
        ETT_MAX_TEXT_TYPES,
    };


    // Class for specific needs of rendering 3D
    // NOTE: The below class doesn't own any of the pointers
    class CTextureFor3D
    {
    public:

        CTextureFor3D() : pTexture(NULL), type(ETT_NULL)
        { }

        // Texture buffer
        LPDIRECT3DTEXTURE9 pTexture;

        // Type of texture
        ETextureType type;
    };


    // Class for specific needs of rendering 2D
    class CTextureFor2D
    {
    public:

        CTextureFor2D()
        { }

        // Texture buffer in a safe pointer
        CComPtr<IDirect3DTexture9> spTexture;

        // Size of texture
        CSize<float> size;
    };


    // Class for managing more then one texture
    // NOTE: The below class doesn't own any of the pointers
    class CMaterial
    {
    public:

        // All other textures added to their material
        std::vector<NText::CTextureFor3D> textureLstVec;
    };

}

#endif  // __texture_h__