/************************************************************************
*    FILE NAME:       sortfunc.h
*
*    DESCRIPTION:     General sort functions
************************************************************************/           

#ifndef __sortfunc_h__
#define __sortfunc_h__

// Standard lib dependencies
#include <string>
#include <vector>

// Forward declarations
class CSpriteGroup2D;
class CVisualSprite2D;

namespace NText
{
    class CTextureFor2D;
    class CMaterial;
}

namespace NSortFunc
{
    // Functions to sort pointers of sprites
    bool SpriteGroup2DSort( CSpriteGroup2D * pSpriteA, CSpriteGroup2D * pSpriteB );
    bool VisualSprite2DSort( CVisualSprite2D * pSpriteA, CVisualSprite2D * pSpriteB );

    // Function to sort pointers of 2d textures by height
    bool Texture2DSort( NText::CTextureFor2D * pTextureA, NText::CTextureFor2D * pTextureB );

    // Perform an insertion sort
    void SpriteGroup2DInsertSort( CSpriteGroup2D * pSprite, std::vector<CSpriteGroup2D *> & pSpriteVector );

}

#endif  // __sortfunc_h__