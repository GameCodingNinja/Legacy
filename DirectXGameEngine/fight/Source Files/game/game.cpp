
/************************************************************************
*    FILE NAME:       gametemplate.cpp
*
*    DESCRIPTION:     CGame class
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
#include <boost/scoped_array.hpp>

// Game lib dependencies
#include <system/xdevice.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/genfunc.h>
#include <utilities/criticalsection.h>
#include <utilities/exceptionhandling.h>
#include <xact/xact.h>
#include <misc/settings.h>
#include <gui/uimenutree.h>
#include <managers/shader.h>
#include <managers/actormanager.h>
#include <managers/hudmanager.h>
#include <managers/physicsworldmanager.h>
#include <2d/iaibase.h>
#include <controller/gamecontroller.h>
#include <3d/stage3d.h>

// Game dependencies
#include "../state/startupstate.h"
#include "../state/titlescreenstate.h"
#include "../state/gameloadstate.h"
#include "../state/runstate.h"
#include "../ai/playershipai.h"
#include "../ai/playerprojectileai.h"
#include "../smartGUI/smartresolutionbtn.h"
#include "../smartGUI/smartfullscreencheckbox.h"
#include "../smartGUI/smartvsynccheckbox.h"
#include "../smartGUI/smartexitbtn.h"
#include "../smartGUI/smartapplysettingsbtn.h"
#include "../smartGUI/smartkeybindbtn.h"
#include "../smartGUI/smartdeadzoneslider.h"
#include "../smartGUI/smartbtncapturemenu.h"
#include "../smartGUI/smartconfirmbtn.h"
#include "../smartGUI/smartkeybindscrollbox.h"

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

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CGame::~CGame()
{
}   // Destructer


/***************************************************************************
*    desc:  Load game resources here
****************************************************************************/
void CGame::LoadGameObjects()
{
    // Have the parent do it's thing
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
        CShader::Instance().SetEffectValue( pEffectData, "blurSampleAmount", CalcBlurSampleAmount() );

        // Init the post process strings
        SetPostProcStrings( pEffectData, "nullEffect", "postProcessTexture" );
    }

}	// EnumerateShaderInit


/***************************************************************************
*    decs:  Call back function to create actor ai
*
*    param: aiName - ai name to chech for
*           pActor - actor pointer
****************************************************************************/
void CGame::ActorAICreateCallback( std::string & aiName, CActorSprite2D * pActorSprite )
{
    if( aiName == "player_ship" )
        pActorSprite->SetAI( new CPlayerShipAI( pActorSprite ) );

    else if( aiName == "player_projectile" )
        pActorSprite->SetAI( new CPlayerProjectileAI( pActorSprite ) );

}	// ActorAICreateCallback


/************************************************************************
*    desc:  Callback for when a GUI menu or control is created
*
*	 param: pUIControl - user interface control
*	        pUIMenu - user interface control
************************************************************************/
void CGame::PerformSmartGuiCreate( CUIControl * pUIControl, CUIMenu * pUIMenu )
{
    static uint keyBindBtnIndex(0);

    if( pUIControl != NULL )
    {
        if( pUIControl->GetExecutionAction() == "confirmation_menu" )
            pUIControl->SetSmartGui( new CSmartConfirmBtn( pUIControl ) );

        else if( pUIControl->GetName() == "resolution_btn_lst" )
            pUIControl->SetSmartGui( new CSmartResolutionBtn( pUIControl ) );

        else if( pUIControl->GetName() == "full_screen_check_box" )
            pUIControl->SetSmartGui( new CSmartScrnCheckBox( pUIControl ) );

        else if( pUIControl->GetName() == "v-sync_check_box" )
            pUIControl->SetSmartGui( new CSmartVSyncCheckBox( pUIControl ) );

        else if( pUIControl->GetName() == "settings_apply_btn" )
            pUIControl->SetSmartGui( new CSmartApplySettingsBtn( pUIControl ) );

        else if( pUIControl->GetName() == "settings_dead_zone_slider" )
            pUIControl->SetSmartGui( new CSmartDeadZoneSlider( pUIControl ) );

        else if( pUIControl->GetName() == "key_binding_scroll_box" )
            pUIControl->SetSmartGui( new CSmartKeyBindScrollBox( pUIControl ) );

        else if( pUIControl->GetExecutionAction() == "button_capture_menu" )
            pUIControl->SetSmartGui( new CSmartKeyBindBtn( pUIControl ) );
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

}	// DeleteGameObjcts


/***************************************************************************
*    decs:  Handle the state change
****************************************************************************/
void CGame::DoStateChange()
{
    if( spGameState->DoStateChange() )
    {
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

}   // DoStateChange


/***************************************************************************
*    decs:  Act upon what the user is doing
****************************************************************************/
void CGame::GetUserInput()
{
    spGameState->GetUserInput( HasFocus() );

}   // GetUserInput


/************************************************************************
*    desc:  Check for collision and react to it.
************************************************************************/
void CGame::ReactToCollision()
{
    spGameState->ReactToCollision();

}	// ReactToCollision


/***************************************************************************
*    decs:  Update animations
****************************************************************************/
void CGame::Update()
{
    spGameState->Update();

    /* Example of positioning a sound in 3D space
    if( music.IsBusy() )
    {
        CCamera::Instance().Finalize();
        CPoint point = CCamera::Instance().GetFinalMatrix().GetMatrixPoint();
        point.Invert();
        CXAct::Instance().PositionCue( point, music.pCue );
    }*/

    // Don't update every frame  and don't update during the startup state
    if( soundUpdateTimer.Expired(true) && (spGameState->GetState() > EGS_STARTUP) )
        CXAct::Instance().Update();

}	// Update


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


/***************************************************************************
*    desc:  Transform game objects
****************************************************************************/
void CGame::Transform()
{
    spGameState->Transform();

}	// Transform


/****************************************************************************
*    decs:  Render to the shadow buffer
****************************************************************************/
void CGame::RenderShadowBuffer()
{
}	// RenderShadowBuffer


/***************************************************************************
*    decs:  3D/2D Render of game content
****************************************************************************/
void CGame::PreRender()
{
    spGameState->PreRender();

}	// GameRender3D


/***************************************************************************
*    decs:  3D/2D Render of content after post process effects
****************************************************************************/
void CGame::PostRender()
{
    spGameState->PostRender();

}	// PostGameRender2D 


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

    // Get the number of samples needed for this game's resolution
    int blurSamples = CalcBlurSampleAmount();

    // Set values based on if we are transitioning in or out
    if( CUIMenuTree::Instance().GetState() == NUIMenu::EMTS_WAIT_TRANSITION_OUT_TOGGLE )
    {
        startValue = 0.5f;
        endValue = .001f;
        blurFadeOutTime += CHighResTimer::Instance().GetElapsedTime();
        blurFadeTime = blurFadeOutTime;
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

    // Inc the value based on time
    if( blurFadeTime < milSecTime )
        incValue = (incPerMilsec * blurFadeTime) + startValue;
    else
        incValue = endValue;

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

}	// PauseBlurInOutDrain


/************************************************************************
*    desc:  Calculate the blur sample amount
************************************************************************/
int CGame::CalcBlurSampleAmount()
{
    const int DEFAULT_BLUR_SAMPLES(5);
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
*    decs:  Convert the data associated with pixel shader "ConvertPixelsToTexels"
****************************************************************************/
bool CGame::SetConvertPixelsToTexels( std::string & effectId, std::string & convertPixelsId )
{
    LPD3DXEFFECT pEffect = CShader::Instance().GetEffectData( effectId )->spEffect;

    if( pEffect == NULL )
        return false;

    D3DXHANDLE hParamToConvert;
    D3DXHANDLE hAnnotation;
    UINT uParamIndex = 0;

    // Go through the parameter list to find the data we need to convert
    while( NULL != ( hParamToConvert = pEffect->GetParameter( NULL, uParamIndex++ ) ) )
    {
        if( NULL != ( hAnnotation = pEffect->GetAnnotationByName( hParamToConvert, convertPixelsId.c_str() ) ) )
        {
            LPCSTR szSource;
            pEffect->GetString( hAnnotation, &szSource );
            D3DXHANDLE hConvertSource = pEffect->GetParameterByName( NULL, szSource );

            if( hConvertSource )
            {
                // Retrieve the kernel size
                D3DXPARAMETER_DESC desc;
                pEffect->GetParameterDesc( hConvertSource, &desc );

                // Each element has 2 floats
                DWORD cKernel = desc.Bytes / ( 2 * sizeof( float ) );

                // Allocate the vector
                boost::scoped_array<D3DXVECTOR4> pvKernel( new D3DXVECTOR4[cKernel] );

                pEffect->GetVectorArray( hConvertSource, pvKernel.get(), cKernel );

                // Convert
                for( DWORD i = 0; i < cKernel; ++i )
                {
                    pvKernel[i].x = pvKernel[i].x / CXDevice::Instance().GetBufferWidth();
                    pvKernel[i].y = pvKernel[i].y / CXDevice::Instance().GetBufferHeight();
                }

                // Set the converted data
                pEffect->SetVectorArray( hParamToConvert, pvKernel.get(), cKernel );
            }
        }
    }

    return true;

}	// SetConvertPixelsToTexels


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
    CShader::Instance().LoadFromXML( "data/shaders/startup_shader_effects.cfg" );
    CShader::Instance().LoadFromXML( "data/shaders/shader_effects.cfg" );
    CShader::Instance().EnumerateShaderInit();

}	// HandleDeviceReset

