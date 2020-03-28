/************************************************************************
*    FILE NAME:       liminality.h
*
*    DESCRIPTION:     game class
************************************************************************/

#ifndef __liminality_h__
#define __liminality_h__

// Physical component dependency
#include <system/gamewnd.h>

// Boost lib dependencies
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>

// Game lib dependencies
#include <2d/stage2d.h>
#include <gui/uimenutree.h>
#include <xact/xact.h>
#include <gui/controlevent.h>
#include <misc/shaderevent.h>
#include <2d/fontsprite.h>
#include <2d/spritegroup2d.h>
#include <utilities/timer.h>

// Game dependencies
#include "worldmanager.h"

enum EDirection
{
    ED_UP,
    ED_DOWN,
    ED_LEFT,
    ED_RIGHT
};


// Forward declaration(s)
class iAIBase;
class iGameState;
class CEffectData;
class CActorSprite2D;

class CLiminality : public CGameWnd
{
public:

    // Get the instance
    static CLiminality & Instance()
    {
        static CLiminality game;
        return game;
    }

    // Load game resources here
    virtual void LoadGameObjects();

    // Delete the allocated game objects
    virtual void DeleteGameObjects();

    // Handle the state change
    virtual void DoStateChange();

    // Act upon what the user is doing
    virtual void GetUserInput();

    // Update animations, Move sprites, Check for collision
    virtual void Update();

    // Check for collision and react to it
    virtual void ReactToCollision();

    // Render to the shadow buffer
    virtual void RenderShadowBuffer();

    // 2D/3D Render of game content
    virtual void PreRender();

    // 2D/3D Render of post game content
    virtual void PostRender();

    // Handle the device reset
    virtual void HandleDeviceReset( CSize<float> & res, bool vSync, bool windowed );

    // Convert the data associated with pixel shader "ConvertPixelsToTexels"
    bool SetConvertPixelsToTexels( std::string & effectId, std::string & convertPixelsId );

private:

    // Constructor
    CLiminality();

    // Destructor
    virtual ~CLiminality();

    // Enumerate the shader init
    void EnumerateShaderInit( CEffectData * pEffectData );

    // generate weights for bell curve.
    void GenerateWeights( int maxWeights, float variance, float & weightInc, float & totalWeight );

    // Calculate the blur sample amount
    int CalcBlurSampleAmount();

    // pause blur in and out with color drain
    void PauseBlurInOutDrain();

    // See if we are to do any post process
    virtual void PostProcessDecision();

    // Handle the effects for the post process
    virtual void HandlePostProcessEffects();

    // Set the resolution strings to the control
    void SetResStringsToControl( CUIControl * pControl );

    // Call back function to create actor ai
    void ActorAICreateCallback( std::string & aiName, CActorSprite2D * pActorSprite );

    // Callback for when a GUI menu or control is created
    void PerformSmartGuiCreate( CUIControl * pUIControl, CUIMenu * pUIMenu );

private:

    // Needed publish/subscriber call backs
    CSubscriber<CSmartGuiCreateEvent, CSmartGuiCreateEvent::CallbackType> smartGuiCreateSubscriber;
    CSubscriber<CShaderInitEvent, CShaderInitEvent::CallbackType> shaderInitSubscriber;

    // flags that indicates movement
    bool movementActive;

    // Player
    CSpriteGroup2D player2d;
    
    // Blur members
    float blurFadeInTime;
    float blurFadeOutTime;

    // scoped pointer Game State
    boost::scoped_ptr<iGameState> spGameState;

    // Timer
    CTimer soundUpdateTimer;

    // Direction the player is facing
    EDirection playerFacing;

    bool playerMoving;

};

#endif  // __liminality_h__
