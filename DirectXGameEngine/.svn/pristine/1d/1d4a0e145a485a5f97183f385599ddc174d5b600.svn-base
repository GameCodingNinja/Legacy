
/************************************************************************
*    FILE NAME:       jointanimationsprite3d.h
*
*    DESCRIPTION:     3D joint animation sprite class
************************************************************************/

#ifndef __joint_animation_sprite_3d_h__
#define __joint_animation_sprite_3d_h__

// Physical component dependency
#include <3d/visualsprite3D.h>

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/scoped_array.hpp>

// Game lib dependencies
#include <3d/jointmultilinklist.h>
#include <3d/jointnode.h>
#include <3d/lightlst.h>

// Forward declaration(s)
class CXVertBuff;
class CObjectData3D;

class CJointAnimSprite3D : public CVisualSprite3D
{
public:

    CJointAnimSprite3D();
    CJointAnimSprite3D( CObjectData3D * pObjData );
    virtual ~CJointAnimSprite3D();

    // Init the sprite with the object data
    virtual void Init( const std::string & group, const std::string & type );

    // Set the animation
    void SetAnimation( const std::string & name, float frameCount = 3.f );

    // Get the active animation string name
    std::string & GetAnimation();

    // Inc the animation timer
    void IncAnimationTime();

protected:

    // Load the mesh animation from RSA file
    void LoadAnimFromRSA( std::string & name, float fps, std::string & animFilePath );

    // Update the shader prior to rendering
    virtual void UpdateShader( CMatrix & matrix );

    // Update the shader for light projection
    virtual void UpdateShadowMapShader();

    // Update the animation
    void UpdateAnimation();

    // Init the joint matrix array and send it up to the shader
    void InitJointMatrixArry();

private:

    // Init the sprite with the object data
    void Init();

private:

    // Current active animation string
    std::string animNameStr;

    // time
    float time;

    // transition time
    float transTime;

    // transition frame count
    float transFrameCount;

    // flag to indicate animation needs to be updated
    bool updateAnim;

    // map of joint animation multu-link list
    boost::ptr_map<std::string, CJointLinkLst<CJointNode *>> spJointLinkLstMap;
    boost::ptr_map<std::string, CJointLinkLst<CJointNode *>>::iterator jointLinkLstMapIter;

    // The animation transition pointer
    // NOTE: This class does not own this pointer.
    CJointLinkLst<CJointNode *> * pTransToAnim;

    // The current animation pointer
    // NOTE: This class does not own this pointer.
    CJointLinkLst<CJointNode *> * pCurrentAnim;

    // Allocated list of directX matrixes
    boost::scoped_array<D3DXMATRIX> pDXMatrix;

};

#endif  // __joint_animation_sprite_h__
