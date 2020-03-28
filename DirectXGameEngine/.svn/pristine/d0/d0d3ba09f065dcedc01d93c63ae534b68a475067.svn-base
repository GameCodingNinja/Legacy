
/************************************************************************
*    FILE NAME:       game.cpp
*
*    DESCRIPTION:     game class
************************************************************************/

// Game OS dependencies
                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

// Physical component dependency
#include "game.h"

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/bind.hpp>

// Game lib dependencies
#include <2d/objectdatalist2d.h>
#include <2d/fontmanager.h>
#include <2d/actorsprite2d.h>
#include <managers/stagedatalist.h>
#include <managers/shader.h>
#include <managers/actormanager.h>
#include <managers/physicsworldmanager.h>
#include <system/xdevice.h>
#include <controller/gamecontroller.h>
#include <scripting/scriptmanager.h>
#include <common/defs.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/genfunc.h>
#include <utilities/statcounter.h>
#include <utilities/criticalsection.h>
#include <utilities/exceptionhandling.h>
#include <misc/settings.h>
#include <xact/xact.h>
#include <gui/uimenutree.h>

// Game dependencies
#include "../state/startupstate.h"
#include "../state/titlescreenstate.h"
#include "../state/gameloadstate.h"
#include "../state/runstate.h"
#include "../state/igamestate.h"
#include "../ai/playerprojectileai.h"
#include "../ai/playershipai.h"
#include "../ai/enemyshipai.h"
#include "../smartGUI/smartresolutionbtn.h"
#include "../smartGUI/smartfullscreencheckbox.h"
#include "../smartGUI/smartvsynccheckbox.h"
#include "../smartGUI/smartconfirmbtn.h"
#include "../smartGUI/smartapplysettingsbtn.h"
#include "../smartGUI/smartkeybindbtn.h"
#include "../smartGUI/smartbtncapturemenu.h"
#include "../smartGUI/smartkeybindscrollbox.h"
#include "../smartGUI/smartnewloadscrollbox.h"
#include "../smartGUI/smartnewgamesavebtn.h"
#include "../smartGUI/smartloadgamesavebtn.h"
#include "../smartGUI/smartcontinuebtn.h"
#include "../smartGUI/smartdeadzoneslider.h"

// Game dependencies local
#include "savefile.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)
// disable warning about the use of the "this" pointer in the constructor list
#pragma warning(disable : 4355)


/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CGame::CGame()
     : smartGuiCreateSubscriber( boost::bind(&CGame::PerformSmartGuiCreate, this, _1, _2) ),
       blurFadeInTime(0),
       blurFadeOutTime(0),
       soundUpdateTimer(500, true)
{
    // Turn on the subscriber
    smartGuiCreateSubscriber.Subscribe();

    // Set post process effects
    SetPostProcStrings("nullEffect");
    EnablePostProcessing(false);

}   // Constructer */


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CGame::~CGame()
{
}   // Destructer */


/***************************************************************************
*    desc:  Load game resources here
*
*    ret:   bool - true on success or false on fail
****************************************************************************/
void CGame::LoadGameObjects()
{
    CGameWnd::LoadGameObjects();

    // Connect to shader signal
    CShader::Instance().Connect( boost::bind(&CGame::EnumerateShaderInit, this, _1) );

    // Set the call back to the actor ai
    CActorManager::Instance().Connect( boost::bind(&CGame::ActorAICreateCallback, this, _1, _2 ) );

    // Set the mouse position to the middle of the window
    SetCursorPos( mouseCenterPt.x, mouseCenterPt.y );

    // Create the physics worlds
    CPhysicsWorldManager::Instance().LoadFromXML( "data/objects/2d/physicsWorldList.lst" );

    // Create the startup state
    spGameState.reset( new CStartUpState );

    // Let it run
    StartGame();

}   // LoadGameObjects


/***************************************************************************
*    decs:  Enumerate the shader init to set it's default values
*
*    param: effectStr - effects string
****************************************************************************/
void CGame::EnumerateShaderInit( CEffectData * pEffectData )
{
    if( pEffectData->effectNameStr == "post_process" )
    {
        CShader::Instance().SetEffectValue( pEffectData, "bufferWidth", CXDevice::Instance().GetBufferWidth() );
        CShader::Instance().SetEffectValue( pEffectData, "bufferHeight", CXDevice::Instance().GetBufferHeight() );

        // Init the post process strings
        SetPostProcStrings( pEffectData, "nullEffect", "postProcessTexture" );
    }

}	// EnumerateShaderInit */


/***************************************************************************
*    decs:  Call back function to create actor ai
*
*    param: aiName - ai name to chech for
*           pActor - actor pointer
****************************************************************************/
void CGame::ActorAICreateCallback( string & aiName, CActorSprite2D * pActorSprite )
{
    if( aiName == "player_ship" )
        pActorSprite->SetAI( new CPlayerShipAI( pActorSprite ) );

    else if( aiName == "player_projectile" )
        pActorSprite->SetAI( new CPlayerProjectileAI( pActorSprite ) );

    else if( aiName == "enemy_ship" )
        pActorSprite->SetAI( new CEnemyShipAI( pActorSprite ) );

}	// ActorAICreateCallback


/************************************************************************
*    desc:  Callback for when a GUI menu or control is created
*
*	 param: pUIControl - user interface control
*	        pUIMenu - user interface control
************************************************************************/
void CGame::PerformSmartGuiCreate( CUIControl * pUIControl, CUIMenu * pUIMenu )
{
    if( pUIControl != NULL )
    {
        if( pUIControl->GetExecutionAction() == "confirmation_menu" )
            pUIControl->SetSmartGui( new CSmartConfirmBtn( pUIControl ) );

        else if( pUIControl->GetExecutionAction() == "button_capture_menu" )
            pUIControl->SetSmartGui( new CSmartKeyBindBtn( pUIControl ) );

        else if( pUIControl->GetExecutionAction() == "new_load_game_scroll_box" )
            pUIControl->SetSmartGui( new CSmartNewLoadScrollBox( pUIControl ) );

        else if( pUIControl->GetName() == "resolution_btn_lst" )
            pUIControl->SetSmartGui( new CSmartResolutionBtn( pUIControl ) );

        else if( pUIControl->GetName() == "full_screen_check_box" )
            pUIControl->SetSmartGui( new CSmartScrnCheckBox( pUIControl ) );

        else if( pUIControl->GetName() == "v-sync_check_box" )
            pUIControl->SetSmartGui( new CSmartVSyncCheckBox( pUIControl ) );

        else if( pUIControl->GetName() == "settings_apply_btn" )
            pUIControl->SetSmartGui( new CSmartApplySettingsBtn( pUIControl ) );

        else if( pUIControl->GetName() == "key_binding_scroll_box" )
            pUIControl->SetSmartGui( new CSmartKeyBindScrollBox( pUIControl ) );

        else if( pUIControl->GetName() == "create_new_save_btn" )
            pUIControl->SetSmartGui( new CSmartNewGameSaveBtn( pUIControl ) );

        else if( pUIControl->GetName() == "load_game_save_btn" )
            pUIControl->SetSmartGui( new CSmartLoadGameSaveBtn( pUIControl ) );

        else if( pUIControl->GetName() == "continue_btn" )
            pUIControl->SetSmartGui( new CSmartContinueBtn( pUIControl ) );

        else if( pUIControl->GetName() == "settings_dead_zone_slider" )
            pUIControl->SetSmartGui( new CSmartDeadZoneSlider( pUIControl ) );
    }
    else if( pUIMenu != NULL )
    {
        if( pUIMenu->GetName() == "button_capture_menu" )
            pUIMenu->SetSmartGui( new CSmartBtnCaptureMenu( pUIMenu ) );
    }

}	// PerformSmartGuiCreate


/***************************************************************************
*    decs:  Delete game resources here
****************************************************************************/
void CGame::DeleteGameObjects()
{
    CGameWnd::DeleteGameObjects();

    // Save the game on the way out
    if( spGameState && (spGameState->GetState() == EGS_RUN) )
        CSaveFile::Instance().Save();

}	// DeleteGameObjcts */


/***************************************************************************
*    decs:  Handle the state change
****************************************************************************/
void CGame::DoStateChange()
{
    if( spGameState->DoStateChange() )
    {
        // Kill the pre-render pause
        SetPreRenderPause( false );

        // Get the game state we are moving to
        const EGameState curState = spGameState->GetState();

        // Get the game state we are moving to
        const EGameState nextState = spGameState->GetNextState();

        // Get the message to the next state
        const CStateMessage stateMessage = spGameState->GetMessage();

        // Free the state class forcing the distructor to
        // de-allocating before loading the next state
        spGameState.reset();

        if( nextState == EGS_TITLE_SCREEN )
            spGameState.reset( new CTitleScreenState );

        else if( nextState == EGS_GAME_LOAD )
            spGameState.reset( new CGameLoadState( stateMessage ) );

        else if( nextState == EGS_RUN )
            spGameState.reset( new CRunState );

        else
            throw NExcept::CCriticalException("Error Invalid game state",
                boost::str( boost::format("Next state not valid (cur %d, next %d).\n\n%s\nLine: %s") % curState % nextState % __FUNCTION__ % __LINE__ ));
    }

}   // DoStateChange */


/***************************************************************************
*    decs:  Act upon what the user is doing
****************************************************************************/
void CGame::GetUserInput()
{
    spGameState->GetUserInput( HasFocus() );

}   // GetUserInput */


/************************************************************************
*    desc:  Check for collision and react to it.
************************************************************************/
void CGame::ReactToCollision()
{
    spGameState->ReactToCollision();

}	// ReactToCollision */


/***************************************************************************
*    decs:  Update animations, Move sprites, Check for collision
****************************************************************************/
void CGame::Update()
{
    spGameState->Update();

    // Don't update every frame and don't update during the startup state
    if( soundUpdateTimer.Expired(true) && (spGameState->GetState() > EGS_STARTUP) )
        CXAct::Instance().Update();

}	// Update */


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CGame::Transform()
{
    spGameState->Transform();

}	// Transform */


/***************************************************************************
*    decs:  2D Render of game content
****************************************************************************/
void CGame::PreRender()
{
    spGameState->PreRender();

}	// GameRender2D */


/***************************************************************************
*    decs:  2D Render of content after post process effects
****************************************************************************/
void CGame::PostRender()
{
    spGameState->PostRender();

}	// PostGameRender2D  */


/***************************************************************************
*    decs:  Decide if we are to do post process
****************************************************************************/
void CGame::PostProcessDecision()
{
    if( spGameState->GetState() == EGS_RUN )
    {
        // Handle post processing based on menu
        if( CUIMenuTree::Instance().GetState() == NUIMenu::EMTS_HIDDEN )
        {
            SetPostProcStrings("nullEffect");
            EnablePostProcessing(false);
            blurFadeInTime = 0;
            blurFadeOutTime = 0;
        }
        else
        {
            SetPostProcStrings("gaussianBlurColorDrain");
            EnablePostProcessing();
        }
    }

}	// PostProcessDecision


/************************************************************************
*    desc:  Handle the effects for the post process
************************************************************************/
void CGame::HandlePostProcessEffects()
{
    if( GetPostProcessTechIdStr() == "gaussianBlurColorDrain" )
        PauseBlurInOutDrain();

}	// HandlePostProcessEffects


/************************************************************************
*    desc:  pause blur in and out with color drain
************************************************************************/
void CGame::PauseBlurInOutDrain()
{
    float startValue;
    float endValue;
    const float milSecTime(250);
    float incValue;
    float blurFadeTime;
    float incPerMilsec;
    bool blurFadeInFlag(true);

    // Get the number of samples needed for this game's resolution
    int blurSamples = CalcBlurSampleAmount();

    // Set values based on if we are transitioning in or out
    if( CUIMenuTree::Instance().GetState() == NUIMenu::EMTS_WAIT_TRANSITION_OUT_TOGGLE )
    {
        startValue = 0.5f;
        endValue = .001f;
        blurFadeOutTime += CHighResTimer::Instance().GetElapsedTime();
        blurFadeTime = blurFadeOutTime;
        blurFadeInFlag = false;
    }
    else
    {
        startValue = 0.001f;
        endValue = 0.5f;
        blurFadeInTime += CHighResTimer::Instance().GetElapsedTime();
        blurFadeTime = blurFadeInTime;
    }

    // Calculate the amount per milisecond
    incPerMilsec = (endValue-startValue) / milSecTime;

    // Set the value used to increment color drain components
    incValue = (incPerMilsec * blurFadeTime) + startValue;

    if( blurFadeInFlag )
    {
        if( incValue > endValue )
        {
            incValue = endValue;

            // Pause the per-render
            SetPreRenderPause();

            // Set the nullEffect for refreshing the display buffer
            // with the last result of the post process buffer
            SetPostProcStrings("nullEffect");
        }
    }
    else
    {
        if( incValue < endValue )
            incValue = endValue;
    }

    // Calculate the color drain amount
    float colorDrainDominate = 1.0f - incValue;

    // Generate the weights
    float weightInc, totalWeight;
    GenerateWeights( CalcBlurSampleAmount(), incValue, weightInc, totalWeight );

    // Set the shader values
    CShader::Instance().SetEffectValue( GetPostProcEffectDataPtr(), "weightInc", weightInc );
    CShader::Instance().SetEffectValue( GetPostProcEffectDataPtr(), "totalWeight", totalWeight );
    CShader::Instance().SetEffectValue( GetPostProcEffectDataPtr(), "weightVariance", incValue );
    CShader::Instance().SetEffectValue( GetPostProcEffectDataPtr(), "colorDrainDominate", colorDrainDominate );
    CShader::Instance().SetEffectValue( GetPostProcEffectDataPtr(), "colorDrainSubmissive", incValue / 2.f );

    // It redundant but this needs to be set here because EnumerateShaderInit is called before
    // the ratio is recalulated after a resolution change.
    CShader::Instance().SetEffectValue( GetPostProcEffectDataPtr(), "blurSampleAmount", CalcBlurSampleAmount() );

}	// PauseBlurInOutDrain


/************************************************************************
*    desc:  Calculate the blur sample amount
************************************************************************/
int CGame::CalcBlurSampleAmount()
{
    const int DEFAULT_BLUR_SAMPLES(4);
    const int MIN_BLUR_SAMPLES(1);

    // Calculate the blur sample amount based on the screen ratio
    int blurSamples = DEFAULT_BLUR_SAMPLES * CSettings::Instance().GetScreenRatio().w;

    // Do a range check
    if( blurSamples < MIN_BLUR_SAMPLES )
        blurSamples = MIN_BLUR_SAMPLES;

    return blurSamples;

}	// CalcBlurSampleAmount


/************************************************************************
*    desc: generate weights for bell curve.
*
*    param: halfWeights - half of the weights. The real number of weight
*                         is (halfWeights * 2) + 1.
*           variance -	value greater than 0
*						higher variance = less blury
*						lower variance = more blury
************************************************************************/
void CGame::GenerateWeights( int halfWeights, float variance, float & weightInc, float & totalWeight )
{
    // A value to adjust the domain of the bell curve. Best left alone.
    const float DOMAIN_ADJUSTMENT = 4;

    // Calculate the inc with the full number of bell curve weights
    weightInc = (sqrt(variance) * DOMAIN_ADJUSTMENT) / float((halfWeights*2)+1);
    totalWeight = 0.0f;
    
    float x = 0;
    int inc(0);

    float weight = (float)(1 / sqrt(2 * M_PI * variance)) * exp( -(x)/(2 * variance));
    totalWeight += weight;
    
    for( int i = 0; i < halfWeights; ++i )
    {
        x += weightInc;

        weight = (float)(1 / sqrt(2 * M_PI * variance)) * exp( -(x)/(2 * variance));

        totalWeight += weight;
        totalWeight += weight;
    }

}	// GenerateWeights


/***************************************************************************
*    decs:  Recover from a device lost
****************************************************************************/
void CGame::HandleDeviceReset( const CSize<float> & res, bool vSync, bool windowed )
{
    // Free the shader(s)
    CShader::Instance().Free();

    // All things created by the device that is not managed need 
    // to be Released before the device can be reset
    CXDevice::Instance().HandleDeviceReset( res, vSync, windowed );

    // Reload and Enumerate through all the shaders to set their default values
    CShader::Instance().LoadFromXML("data/shaders/startup_shader_effects.cfg");
    CShader::Instance().LoadFromXML("data/shaders/shader_effects.cfg");
    CShader::Instance().EnumerateShaderInit();

    // Kill the pre render pause
    SetPreRenderPause( false );

}	// HandleDeviceReset */


/***************************************************************************
*   desc:  Do the rendering
****************************************************************************/
void CGame::Render()
{
    // Set values based on if we are transitioning in or out
    if( IsPreRenderPause() && CUIMenuTree::Instance().GetState() == NUIMenu::EMTS_WAIT_TRANSITION_OUT_TOGGLE )
    {
        // Allow pre rendering 
        SetPreRenderPause( false );

        // Re-enable the the color drain again
        SetPostProcStrings("gaussianBlurColorDrain");
    }

    CGameWnd::Render();

}	// Render
