
/************************************************************************
*    FILE NAME:       liminality.cpp
*
*    DESCRIPTION:     game class
************************************************************************/

// Game OS dependencies
                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

// Physical component dependency
#include "liminality.h"

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/lexical_cast.hpp>
#include <boost/scoped_array.hpp>

// Game lib dependencies
#include <system/xwindow.h>
#include <common/defs.h>
#include <utilities/highresolutiontimer.h>
#include <misc/settings.h>
#include <managers/actormanager.h>
#include <managers/shader.h>

// Game dependencies
#include "startupstate.h"
#include "runstate.h"
#include "igamestate.h"
#include "smartresolutionbtn.h"
#include "smartfullscreencheckbox.h"
#include "smartvsynccheckbox.h"
#include "smartexitbtn.h"
#include "smartapplysettingsbtn.h"
#include "smartkeybindbtn.h"
#include "smartcontrolbindinglist.h"
#include "smartbtncapturemenu.h"

// Required namespace(s)
using namespace std;

// disable warning about the use of the "this" pointer in the constructor list
#pragma warning(disable : 4355)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CLiminality::CLiminality()
     : movementActive(false),
       smartGuiCreateSubscriber( boost::bind(&CLiminality::PerformSmartGuiCreate, this, _1, _2) ),
       shaderInitSubscriber( boost::bind(&CLiminality::EnumerateShaderInit, this, _1) ),
       blurFadeInTime(0),
       blurFadeOutTime(0),

       soundUpdateTimer(500, true)
{
    // Turn on the subscribers
    smartGuiCreateSubscriber.Subscribe();
    shaderInitSubscriber.Subscribe();

    // Set the call back to the actor ai
    CActorManager::Instance().SetCreateAICallback( boost::bind(&CLiminality::ActorAICreateCallback, this, _1, _2 ) );

    // Load the settings file
    CSettings::Instance().LoadFromXML(string("data/settings/settings.cfg"));

    // Force an expire so that there's no initial wait
    soundUpdateTimer.SetExpired();

    // Set post process effects
    SetPostProcStrings(string("nullEffect"));
    EnablePostProcessing(false);

}   // Constructer */


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CLiminality::~CLiminality()
{
}   // Destructer */


/***************************************************************************
*    desc:  Load game resources here
*
*    ret:   bool - true on success or false on fail
****************************************************************************/
void CLiminality::LoadGameObjects()
{
    CGameWnd::LoadGameObjects();

    // Load up the object data list
    //CObjectDataList2D::Instance().LoadFromXML( string("data\\objects\\2d\\object data list\\objectDataList.lst") );
    //CObjectDataList2D::Instance().LoadFromXML( string("data\\objects\\2d\\object data list\\menuDataList.lst") );
    //CScriptMgr::Instance().LoadFromXML( string("data\\objects\\2d\\scriptList.lst") );

    // Load fonts
    //CFontMgr::Instance().LoadFromXML(string("data\\materials\\fonts\\fonts.cfg"));

    // Load the UV lists
    

    // Load up stage data list
    //CStageDataList::Instance().LoadFromXML( string("data\\objects\\2d\\stageDataList.lst") );

    // Set the mouse position to the middle of the window
    //SetCursorPos( mouseCenterPt.x, mouseCenterPt.y );

    //CGameController::Instance().Init( CXWindow::Instance().GetWndHandle() );

    // add devices to the controller list
    //CGameController::Instance().AddToPollLst( KEYBOARD );
    //CGameController::Instance().AddToPollLst( MOUSE );
    //CGameController::Instance().AddToPollLst( JOYPAD );


    // load in the default controller mappings
    //CGameController::Instance().LoadActionFromXML( string("data\\settings\\controllerMapping.cfg") );

    // Load the global settings
    //CXAct::Instance().Init( string("data\\sounds\\gameTemplate.xgs") );

    // load the Wave Bank
    //CXAct::Instance().LoadWaveBank( string("data\\sounds\\sound_effects.xwb") );

    // load the Wave Bank
    //CXAct::Instance().LoadWaveBank( string("data\\sounds\\music.xwb") );

    // Load the sound Bank
    //CXAct::Instance().LoadSoundBank( string("data\\sounds\\sound_effects.xsb") );

    // Load the sound bank
    //CXAct::Instance().LoadSoundBank( string("data\\sounds\\music.xsb") );


    // Initialize the player
    //player2d.InitGroup( CObjectDataList2D::Instance().GetData( string("female_test") ) );
    //player2d.SetWorldPos( CWorldPoint( CPointInt(0,0,0), CPoint(0,0,0) ) );

    // Initialize the world manager
    //worldManager.Init();

    // Load shader and it's default values
    //CShader::Instance().LoadFromXML( string("data\\shaders\\shader_effects.cfg") );

    // Enumerate through all the shaders to set their default values
    //CShader::Instance().EnumerateShaderInit();

    // load the menu tree
    //CUIMenuTree::Instance().LoadFromXML( string("data\\objects\\2d\\menus\\mainTree.menu") );
    
    // Begin the game in the pause menu
    //CUIMenuTree::Instance().Activate( string("pause_menu") );


    // Set the mouse position to the middle of the window
    SetCursorPos( mouseCenterPt.x, mouseCenterPt.y );

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
void CLiminality::EnumerateShaderInit( CEffectData * pEffectData )
{
    if( pEffectData->effectNameStr == "post_process" )
    {
        CShader::Instance().SetEffectValue( pEffectData, string("bufferWidth"), CXWindow::Instance().GetBufferWidth() );
        CShader::Instance().SetEffectValue( pEffectData, string("bufferHeight"), CXWindow::Instance().GetBufferHeight() );
        CShader::Instance().SetEffectValue( pEffectData, string("blurSampleAmount"), CalcBlurSampleAmount() );

        // Init the post process strings
        SetPostProcStrings( pEffectData, string("nullEffect"), string("postProcessTexture") );
    }

}	// EnumerateShaderInit */


/***************************************************************************
*    decs:  Call back function to create actor ai
*
*    param: aiName - ai name to chech for
*           pActor - actor pointer
****************************************************************************/
void CLiminality::ActorAICreateCallback( string & aiName, CActorSprite2D * pActorSprite )
{
    //*ppAiPointer = new CAIBase( pActor );

}	// ActorAICreateCallback */


/***************************************************************************
*    decs:  Delete game resources here
****************************************************************************/
void CLiminality::DeleteGameObjects()
{
    CGameWnd::DeleteGameObjects();

}	// DeleteGameObjcts */


/***************************************************************************
*    decs:  Handle the state change
****************************************************************************/
void CLiminality::DoStateChange()
{
    if( spGameState->DoStateChange() )
    {
        // Free the state class forcing the distructor to
        // de-allocating before loading the next state
        spGameState.reset();
        spGameState.reset( new CRunState );
    }

}   // DoStateChange */


/***************************************************************************
*    decs:  Act upon what the user is doing
****************************************************************************/
void CLiminality::GetUserInput()
{
    spGameState->GetUserInput();

}   // GetUserInput */


/************************************************************************
*    desc:  Callback for when a GUI menu or control is created
*
*	 param: pUIControl - user interface control
*	        pUIMenu - user interface control
************************************************************************/
void CLiminality::PerformSmartGuiCreate( CUIControl * pUIControl, CUIMenu * pUIMenu )
{
    static uint keyBindBtnIndex(0);

    if( pUIControl != NULL )
    {
        if( pUIControl->GetExecutionAction() == "resolution_button_list" )
            pUIControl->SetSmartGui( new CSmartResolutionBtn( pUIControl ) );

        else if( pUIControl->GetExecutionAction() == "full_screen_check_box" )
            pUIControl->SetSmartGui( new CSmartScrnCheckBox( pUIControl ) );

        else if( pUIControl->GetExecutionAction() == "v-sync_check_box" )
            pUIControl->SetSmartGui( new CSmartVSyncCheckBox( pUIControl ) );

        else if( pUIControl->GetExecutionAction() == "exit_game" )
            pUIControl->SetSmartGui( new CSmartExitBtn( pUIControl ) );

        else if( pUIControl->GetExecutionAction() == "settings_apply_button" )
            pUIControl->SetSmartGui( new CSmartApplySettingsBtn( pUIControl ) );

        else if( pUIControl->GetExecutionAction() == "button_capture_overlay" )
            pUIControl->SetSmartGui( new CSmartKeyBindBtn( pUIControl, keyBindBtnIndex++ ) );

        else if( pUIControl->GetExecutionAction() == "control_device_list" )
            pUIControl->SetSmartGui( new CSmartCtrlBindLstBtn( pUIControl ) );
    }
    else if( pUIMenu != NULL )
    {
        if( pUIMenu->GetName() == "button_capture_overlay" )
            pUIMenu->SetSmartGui( new CSmartBtnCaptureMenu( pUIMenu ) );
    }

}	// PerformSmartGuiCreate


/***************************************************************************
*    decs:  Act upon what the user is doing
****************************************************************************/
/*void CLiminality::HandleGameInput()
{
    // get the elapsed time
    float elapsedTime = CHighResTimer::Instance().GetElapsedTime();
    float movement = 0;

    bool playerMoved = false;

    const float PLAYER_SPEED(0.4f);

    CDeviceMovement &devMovement = CGameController::Instance().GetMovement();

    if( CGameController::Instance().WasAction(string("Up")) == EAP_HOLD )
    {
        playerMoved = true;

        //if( (playerMoving && playerFacing != ED_DOWN) || !playerMoving )
        {
            movement = PLAYER_SPEED * elapsedTime;
            player2d.IncWorldPos( CPoint( 0, movement, 0) );
        }

        if( !playerMoving )
        {
            player2d.SetActiveCommand( string("runForward") );
            playerMoving = true;
            playerFacing = ED_UP;
        }
    }

    if( CGameController::Instance().WasAction(string("Down")) == EAP_HOLD )
    {
        playerMoved = true;

        //if( (playerMoving && playerFacing != ED_UP) || !playerMoving )
        {
            movement = -PLAYER_SPEED * elapsedTime;
            player2d.IncWorldPos( CPoint( 0, movement, 0) );
        }
        
        if( !playerMoving )
        {
            player2d.SetActiveCommand( string("runBackward") );
            playerMoving = true;
            playerFacing = ED_DOWN;
        }
    }

    if( CGameController::Instance().WasAction(string("Left")) == EAP_HOLD )
    {
        playerMoved = true;

        //if( (playerMoving && playerFacing != ED_RIGHT) || !playerMoving )
        {
            movement = -PLAYER_SPEED * elapsedTime;
            player2d.IncWorldPos( CPoint( movement, 0.0f, 0.0f) );
        }

        if( !playerMoving )
        {
            player2d.SetActiveCommand( string("runLeft") );
            playerMoving = true;
            playerFacing = ED_LEFT;
        }
    }

    if( CGameController::Instance().WasAction(string("Right")) == EAP_HOLD )
    {
        playerMoved = true;

        //if( (playerMoving && playerFacing != ED_LEFT) || !playerMoving )
        {
            movement = PLAYER_SPEED * elapsedTime;
            player2d.IncWorldPos( CPoint( movement, 0.0f, 0.0f) );
        }
        
        if( !playerMoving )
        {
            player2d.SetActiveCommand( string("runRight") );
            playerMoving = true;
            playerFacing = ED_RIGHT;
        }
    }

    if( !playerMoved )
    {
        playerMoving = false;

        if( playerFacing == ED_UP )
            player2d.SetActiveCommand( string("idleForward") );

        else if( playerFacing == ED_DOWN )
            player2d.SetActiveCommand( string("idleBackward") );

        else if( playerFacing == ED_LEFT )
            player2d.SetActiveCommand( string("idleLeft") );

        else if( playerFacing == ED_RIGHT )
            player2d.SetActiveCommand( string("idleRight") );
    }

    // TEST CODE ************************************************************
    //movement = PLAYER_SPEED * elapsedTime;
    //player2d.IncWorldPos( CPoint( 0, movement, 0) );
    //player2d.IncWorldPos( CPoint( movement, 0.0f, 0.0f) );
    // TEST CODE ***********************************************************

    if( CGameController::Instance().WasAction(string("music")) == EAP_DOWN )
    {
        if( !music.IsBusy() )
            music.Play();
    }

    if( CGameController::Instance().WasAction(string("effect1")) == EAP_DOWN )
    {
        CXAct::Instance().GetSoundCue( string("ammo_fire") ).Play();
    }

    if( CGameController::Instance().WasAction(string("effect2")) == EAP_DOWN )
    {
        CXAct::Instance().GetSoundCue( string("droid_destroyed") ).Play();
    }

}	// HandleGameInput */


/***************************************************************************
*    decs:  Update animations, Move sprites, Check for collision
****************************************************************************/
void CLiminality::Update()
{
    spGameState->Update();

    // Update the menus
    /*CUIMenuTree::Instance().Update();

    // Update the 2D stage and player
    player2d.Update();
    worldManager.Update( player2d.GetWorldPos().IPos() );*/

    // Don't update every frame
    if( soundUpdateTimer.Expired(true) )
        CXAct::Instance().Update();

}	// Update */


/************************************************************************
*    desc:  Check for collision and react to it.
************************************************************************/
void CLiminality::ReactToCollision()
{
    spGameState->ReactToCollision();

    //if( CUIMenuTree::Instance().GetState() == NUIMenu::EMTS_HIDDEN )
        //worldManager.ReactToCollision( &player2d );

}	// ReactToCollision */


/***************************************************************************
*    decs:  Decide if we are to do post process
****************************************************************************/
void CLiminality::PostProcessDecision()
{
    // Handle post processing based on menu
    if( CUIMenuTree::Instance().GetState() == NUIMenu::EMTS_HIDDEN )
    {
        SetPostProcStrings(string("nullEffect"));
        EnablePostProcessing(false);
        blurFadeInTime = 0;
        blurFadeOutTime = 0;
    }
    else
    {
        SetPostProcStrings(string("gaussianBlurColorDrain"));
        EnablePostProcessing();
    }

}	// PostProcessDecision


/****************************************************************************
*    decs:  Render to the shadow buffer
****************************************************************************/
void CLiminality::RenderShadowBuffer()
{
}	// RenderShadowBuffer */


/***************************************************************************
*    decs:  3D Render of game content
****************************************************************************/
void CLiminality::PreRender()
{
    spGameState->PreRender();

}	// GameRender3D */


/***************************************************************************
*    decs:  2D Render of content after post process effects
****************************************************************************/
void CLiminality::PostRender()
{
    spGameState->PostRender();

}	// PostGameRender2D  */


/************************************************************************
*    desc:  Handle the effects for the post process
************************************************************************/
void CLiminality::HandlePostProcessEffects()
{
    if( GetPostProcessTechIdStr() == "gaussianBlurColorDrain" )
        PauseBlurInOutDrain();

}	// HandlePostProcessEffects


/************************************************************************
*    desc:  pause blur in and out with color drain
************************************************************************/
void CLiminality::PauseBlurInOutDrain()
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
    CShader::Instance().SetEffectValue( GetEffectData(), string("weightInc"), weightInc );
    CShader::Instance().SetEffectValue( GetEffectData(), string("totalWeight"), totalWeight );
    CShader::Instance().SetEffectValue( GetEffectData(), string("weightVariance"), incValue );
    CShader::Instance().SetEffectValue( GetEffectData(), string("colorDrainDominate"), colorDrainDominate );
    CShader::Instance().SetEffectValue( GetEffectData(), string("colorDrainSubmissive"), incValue / 2.f );

}	// PauseBlurInOutDrain


/************************************************************************
*    desc:  Calculate the blur sample amount
************************************************************************/
int CLiminality::CalcBlurSampleAmount()
{
    const int DEFAULT_BLUR_SAMPLES(11);
    const int MIN_BLUR_SAMPLES(2);

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
void CLiminality::GenerateWeights( int halfWeights, float variance, float & weightInc, float & totalWeight )
{
    // A value to adjust the domain of the bell curve. Best left alone.
    const float DOMAIN_ADJUSTMENT = 4;

    // Calculate the inc with the full number of bell curve weights
    weightInc = (sqrt(variance) * DOMAIN_ADJUSTMENT) / float((halfWeights*2)+1);
    totalWeight = 0.0f;
    
    float x = 0;
    int inc(0);

    float weight = (float)(1 / sqrt(2 * PI * variance)) * exp( -(x)/(2 * variance));
    totalWeight += weight;
    
    for( int i = 0; i < halfWeights; i++ )
    {
        x += weightInc;

        weight = (float)(1 / sqrt(2 * PI * variance)) * exp( -(x)/(2 * variance));

        totalWeight += weight;
        totalWeight += weight;
    }

}	// GenerateWeights


/***************************************************************************
*    decs:  Recover from a device lost
****************************************************************************/
void CLiminality::HandleDeviceReset( CSize<float> & res, bool vSync, bool windowed )
{
    // Free the shader(s)
    CShader::Instance().Free();

    // All things created by the device that is not managed need 
    // to be Released before the device can be reset
    CXWindow::Instance().HandleDeviceReset( res, vSync, windowed );

    // Reload and Enumerate through all the shaders to set their default values
    CShader::Instance().LoadFromXML( string("data/shaders/startup_shader_effects.cfg") );
    CShader::Instance().LoadFromXML( string("data/shaders/shader_effects.cfg") );
    CShader::Instance().EnumerateShaderInit();

}	// HandleDeviceReset */

