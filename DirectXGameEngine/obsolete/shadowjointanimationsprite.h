
/************************************************************************
*    FILE NAME:       shadowjointanimationsprite.h
*
*    DESCRIPTION:     3D shadow joint animation sprite class
************************************************************************/

#ifndef __shadow_joint_animation_sprite_h__
#define __shadow_joint_animation_sprite_h__

// Game lib dependencies
#include "jointmultilinklist.h"
#include "jointnode.h"

// Physical component dependency
#include "shadowsprite.h"

// Forward declaration(s)
class CShadowJointAnimMesh;

class CShadowJointAnimSprite : public CShadowSprite
{
public:

    CShadowJointAnimSprite();
    virtual ~CShadowJointAnimSprite();

    // Init the sprite with the path to the mesh file
    virtual bool Init( std::string & path );

    // Set the animation
    void SetAnimation( std::string & name, float frameCount = 3.f );

    // Update the animation
    void UpdateAnimation( CMatrix & matrix, CLightLst & lightLst );

protected:

    // Current active animation string
    std::string animNameStr;

    // The current animation pointer
    CJointLinkLst<CJointNode *> * pCurrentAnim;

    // time
    float time;

    // The current animation pointer
    CJointLinkLst<CJointNode *> * pTransToAnim;

    // transition time
    float transTime;

    // transition frame count
    float transFrameCount;

    // Face pointer
    CFace * pFace;

};

#endif  // __joint_animation_sprite_h__
