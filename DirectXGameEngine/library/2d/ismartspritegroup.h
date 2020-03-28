
/************************************************************************
*    FILE NAME:       ismartspritegroup.h
*
*    DESCRIPTION:     iSmart Sprite Group Interface
************************************************************************/

#ifndef __smart_sprite_group_h__
#define __smart_sprite_group_h__

// Forward declaration(s)
class CSpriteGroup2D;

class iSmartSpriteGroup
{
public:

    // Constructor
    // this class does not own the CVisualSprite2D pointer so don't delete it
    explicit iSmartSpriteGroup( CSpriteGroup2D * pSpriteGroup );

    // Destructor
    virtual ~iSmartSpriteGroup(){}

    // Update animations, Move sprites
    virtual void Update(){}

    // React to any collisions against the sprite group
    virtual void ReactToCollision(){}

protected:

    // Pointer to actor class.
    // NOTE: We do not own this pointer. Don't try to free it!
    CSpriteGroup2D * pSpriteGrp;
};

#endif  // __smart_sprite_group_h__


