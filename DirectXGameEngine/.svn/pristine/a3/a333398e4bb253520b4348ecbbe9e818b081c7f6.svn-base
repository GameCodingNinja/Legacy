
/************************************************************************
*    FILE NAME:       texturecomponent.h
*
*    DESCRIPTION:     Holds all of the texture data until for the mega texture
************************************************************************/

#ifndef __texture_component_h__
#define __texture_component_h__

// Game lib dependencies
#include <common/pointint.h>
#include <common/texture.h>

class CMegaTextureComponent
{
public:
        
    // Constructors
    CMegaTextureComponent():pTexture(NULL){}
    CMegaTextureComponent( NText::CTextureFor2D * pTex ){ pTexture = pTex; }

    // The texture of the component
    NText::CTextureFor2D * pTexture;

    // The position of the texture. This is not a uv position but will later
    // be converted into a uv position once all textures have a position
    CPointInt pos;

    // The UV location of the texture in the mega texture in compact form. Indexes
    // 0 and 1 make up the first UV, and 2 and 3 make up the UV diagnal from it
    float uv[4];
};

#endif  // __texture_component_h__


