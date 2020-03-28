/************************************************************************
*    FILE NAME:       sortfunc.cpp
*
*    DESCRIPTION:     General sort functions
************************************************************************/           

// Physical component dependency
#include <utilities/sortfunc.h>

// Game lib dependencies
#include <2d/spritegroup2d.h>
#include <2d/visualsprite2d.h>
#include <common/texture.h>

using namespace std;

namespace NSortFunc
{
    /************************************************************************
    *    desc:  Sort 2d sprite groups
    *
    *	 param: CSpriteGroup2D * pGroupA - sprite to compare
    *			CSpriteGroup2D * pGroupB - sprite to compare
    *
    *	 ret:	bool - return value
    ************************************************************************/
    bool SpriteGroup2DSort( CSpriteGroup2D * pGroupA, CSpriteGroup2D * pGroupB ) 
    {
        if( pGroupA->GetPos().z > pGroupB->GetPos().z )
            return true;

        return false;

    }	// SpriteGroup2DSort


    /************************************************************************
    *    desc:  Sort 2d visual sprites
    *
    *	 param: CVisualSprite2D * pSpriteA - sprite to compare
    *			CVisualSprite2D * pSpriteB - sprite to compare
    *
    *	 ret:	bool - return value
    ************************************************************************/
    bool VisualSprite2DSort( CVisualSprite2D * pSpriteA, CVisualSprite2D * pSpriteB ) 
    {
        if( pSpriteA->GetPos().z > pSpriteB->GetPos().z )
            return true;

        return false;

    }	// VisualSprite2DSort


    /************************************************************************
    *    desc:  Function to sort pointers of 2d textures by width
    *
    *	 param: CTextureFor2D * pTextureA - texture to compare
    *			CTextureFor2D * pTextureB - texture to compare
    *
    *	 ret:	bool - return value
    ************************************************************************/
    bool Texture2DSort( NText::CTextureFor2D * pTextureA, NText::CTextureFor2D * pTextureB )
    {
        if( pTextureA->size.w > pTextureB->size.w )
            return true;

        return false;

    }	// Texture2DSort


    /************************************************************************
    *    desc:  Function to sort pointers of 2d textures by width
    *
    *	 param: CSpriteGroup2D * pSprite				 - sprite to insert
    *			vector<CSpriteGroup2D *> & pSpriteVector - vector to insert into
    ************************************************************************/
    void SpriteGroup2DInsertSort( CSpriteGroup2D * pSprite, vector<CSpriteGroup2D *> & pSpriteVector )
    {
        // Make sure we're not trying to insert a null sprite
        if( pSprite != NULL )
        {
            // Place in the vector to insert the sprite
            int place = -1;

            // The number of sprites in the vector before and after the pivot
            int prevCount = static_cast<int>((pSpriteVector.size() + 1) >> 1);
            int nextCount = static_cast<int>(pSpriteVector.size() - prevCount);

            // The pivot is a space in between sprites of the vector
            int pivot = prevCount;

            // If the vector is empty, just place the sprite in it
            if( pSpriteVector.size() == 0 )
                pSpriteVector.push_back( pSprite );
            
            // If there's one sprite in the vector, simply figure out if the added sprite goes 
            // before or after it
            else if( pSpriteVector.size() == 1 )
            {
                if( pSprite->GetPos().z > pSpriteVector[0]->GetPos().z )
                    pSpriteVector.insert( pSpriteVector.begin(), pSprite );
    
                else
                    pSpriteVector.push_back( pSprite );
            }
            else
            {
                // Begin finding a place for the sprite to be inserted
                while( place == -1 )
                {
                    if( pivot == 0 || pivot == pSpriteVector.size() )
                        place = pivot;
                    else
                    {
                        // If the passed in sprite has a z value less than or equal to the sprite before the pivot's
                        // z value, we go in here
                        if( pSpriteVector[pivot-1]->GetPos().z >= pSprite->GetPos().z )
                        {
                            // If the passed in sprite has a z value greater than the sprite after the pivot's z value,
                            // we found the spot to put our sprite
                            if( pSpriteVector[pivot]->GetPos().z < pSprite->GetPos().z )
                            {
                                place = pivot;
                            }
                            // Otherwise, find the next pivot to test
                            else
                            {
                                int tmpCount = nextCount;
                                prevCount = (tmpCount + 1) >> 1;
                                nextCount = tmpCount - prevCount;
                                pivot = pivot + prevCount;

                                // This may not be needed, but for saftey it's here
                                if( prevCount == 0 )
                                    place = pivot;
                            }
                        }
                        // Otherwise, find the next pivot to test
                        else
                        {
                            int tmpCount = prevCount;
                            prevCount = (tmpCount + 1) >> 1;
                            nextCount = tmpCount - prevCount;
                            pivot = pivot - nextCount;

                            // This may not be needed, but for saftey it's here
                            if( nextCount == 0 )
                                place = pivot;
                        }
                    }
                }

                // Add the sprite to the place found
                if( static_cast<uint>(place) < pSpriteVector.size() )
                    pSpriteVector.insert( pSpriteVector.begin()+place, pSprite );
                else
                    pSpriteVector.push_back( pSprite );
            }
        }

    }	// VisualSprite2DInsertSort

}	// NSortFunc