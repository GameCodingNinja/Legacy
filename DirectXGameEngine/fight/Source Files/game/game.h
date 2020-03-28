
/************************************************************************
*    FILE NAME:       gametemplate.h
*
*    DESCRIPTION:     CGame class
************************************************************************/

#ifndef __game_h__
#define __game_h__

// Physical component dependency
#include <system/gamewnd.h>

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>

// Game lib dependencies
#include <gui/controlevent.h>
#include <misc/shaderevent.h>
#include <utilities/timer.h>

// Forward declaration(s)
class iAIBase;
class iGameState;
class CEffectData;
class CActorSprite2D;

class CGame : public CGameWnd
{
public:

    // Get the instance
    static CGame & Instance()
    {
        static CGame game;
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

    // Check for collision and react to it
    virtual void ReactToCollision();

    // Update animations, Move sprites, Check for collision
    virtual void Update();

    // Transform game objects
    virtual void Transform();

    // Render to the shadow buffer
    virtual void RenderShadowBuffer();

    // 2D/3D Render of game content
    virtual void PreRender();

    // 2D/3D Render of post game content
    virtual void PostRender();

    // Handle the device reset
    virtual void HandleDeviceReset( const CSize<float> & res, bool vSync, bool windowed );

    // Convert the data associated with pixel shader "ConvertPixelsToTexels"
    bool SetConvertPixelsToTexels( std::string & effectId, std::string & convertPixelsId );

private:

    // Constructor
    CGame();

    // Destructor
    virtual ~CGame();

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

    // Call back function to create actor ai
    void ActorAICreateCallback( std::string & aiName, CActorSprite2D * pActorSprite );

    // Callback for when a GUI menu or control is created
    void PerformSmartGuiCreate( CUIControl * pUIControl, CUIMenu * pUIMenu );

private:

    // Needed publish/subscriber call backs
    CSubscriber<CSmartGuiCreateEvent, CSmartGuiCreateEvent::CallbackType> smartGuiCreateSubscriber;
    
    // Blur members
    float blurFadeInTime;
    float blurFadeOutTime;

    // scoped pointer Game State
    boost::scoped_ptr<iGameState> spGameState;

    // timer
    CTimer soundUpdateTimer;

};

#endif  // __game_template_h__
