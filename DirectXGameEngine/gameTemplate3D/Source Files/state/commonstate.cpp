
/************************************************************************
*    FILE NAME:       classtemplate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "runstate.h"

// Game lib dependencies
#include <3d/worldcamera.h>
#include <3d/stage3d.h>
#include <2d/stage2d.h>
#include <controller/gamecontroller.h>
#include <xact/xact.h>
#include <gui/uimenutree.h>
#include <utilities/highresolutiontimer.h>
#include <common/camera.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCommonState::CCommonState()
         : JoypadCheckTimer(2000)
{
}	// Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCommonState::~CCommonState()
{

}   // Destructer


/***************************************************************************
*    desc:  Act upon what the user is doing
****************************************************************************/
void CCommonState::GetUserInput( bool hasFocus )
{
    CGameController::Instance().Poll( hasFocus );

    // Get the user input for the pause menu
    if( CUIMenuTree::Instance().IsVisible() )
        CUIMenuTree::Instance().HandleUserInput();
    else
        HandleGameInput();
        
    if( (CGameController::Instance().GetJoypadCount() == 0) &&
        JoypadCheckTimer.Expired(true) &&
        CGameController::Instance().CheckForGamepadControllers() )
    {
        CGameController::Instance().AddToPollLst( NDevice::JOYPAD );
        CGameController::Instance().LoadJoyPadActionFromXML();

        // Controller found and initialized. Timer no longer needed
        JoypadCheckTimer.Disable();
    }

}	// GetUserInput


/***************************************************************************
*    decs:  Act upon what the user is doing
****************************************************************************/
void CCommonState::HandleGameInput()
{
    // get the elapsed time
    float elapsedTime = CHighResTimer::Instance().GetElapsedTime();
    float fps = CHighResTimer::Instance().GetFPS();

	CPoint moveVec;
    bool input(false);

    const float KEY_MOVEMENT(0.03f);

    const CDeviceMovement &devMovement = CGameController::Instance().GetMovement();

    const float GAMEPAD_MOVEMENT_A1(0.0015f);
    if( devMovement.gamepad1Y != 0 || devMovement.gamepad1X != 0 )
    {
        float speed = GAMEPAD_MOVEMENT_A1 * elapsedTime;
        CPoint tmpVec( ((float)devMovement.gamepad1X * speed), 0, -((float)devMovement.gamepad1Y * speed) );
        float sinY = sin( CCamera::Instance().GetRotation().y * DEG_TO_RAD );
        float cosY = cos( CCamera::Instance().GetRotation().y * DEG_TO_RAD );
        moveVec.x = tmpVec.x * cosY + tmpVec.z * -sinY;
        moveVec.z = tmpVec.x * sinY + tmpVec.z * cosY;
    }

	// handle keyboard input
    if( CGameController::Instance().WasAction("Up") == NDevice::EAP_HOLD )
	{
		moveVec.z = (KEY_MOVEMENT * elapsedTime);
		input = true;
	}

    if( CGameController::Instance().WasAction("Down") == NDevice::EAP_HOLD )
    {
		moveVec.z = -(KEY_MOVEMENT * elapsedTime);
		input = true;
    }

    if( CGameController::Instance().WasAction("Left") == NDevice::EAP_HOLD )
	{
		moveVec.x = -(KEY_MOVEMENT * elapsedTime);
		input = true;
	}

    if( CGameController::Instance().WasAction("Right") == NDevice::EAP_HOLD )
    {
		moveVec.x = (KEY_MOVEMENT * elapsedTime);
		input = true;
    }

	if( input )
		CCamera::Instance().Translate( moveVec );


    if( CGameController::Instance().WasAction("music") == NDevice::EAP_DOWN )
    {
        if( !CXAct::Instance().IsBusy( "music" ) )
            CXAct::Instance().Play( "music" );
    }

    if( CGameController::Instance().WasAction("effect1") == NDevice::EAP_DOWN )
    {
        CXAct::Instance().Play( "ammo_fire" );
    }

    if( CGameController::Instance().WasAction("effect2") == NDevice::EAP_DOWN )
    {
        CXAct::Instance().Play( "droid_destroyed" );
    }
    

    const float MOUSE_MOVEMENT(0.0002f);
    const float GAMEPAD_MOVEMENT_A2(0.003f);

    if( devMovement.mouseY != 0 || devMovement.mouseX != 0 )
    {
        float speed = fps * MOUSE_MOVEMENT * elapsedTime;
        CCamera::Instance().Rotate( CPoint( -((float)devMovement.mouseY * speed), 
                                  -((float)devMovement.mouseX * speed), 0) );
    }

    if( devMovement.gamepad2Y != 0 || devMovement.gamepad2X )
    {
        float speed = GAMEPAD_MOVEMENT_A2 * elapsedTime;
        CCamera::Instance().Rotate( CPoint( -((float)devMovement.gamepad2Y * speed), 
                               -((float)devMovement.gamepad2X * speed), 0) );
    }

    // If the toggle menu action was called, toggle the menues off
    if( CGameController::Instance().WasAction( CUIMenuTree::Instance().GetToggleAction() ) == NDevice::EAP_DOWN ||
        CGameController::Instance().WasAction( CUIMenuTree::Instance().GetEscapeAction() ) == NDevice::EAP_DOWN )
    {
        CUIMenuTree::Instance().Activate( "pause_menu" );
    }

}	// HandleGameInput */


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CCommonState::Update()
{
    // Update the menus
    CUIMenuTree::Instance().Update();

    // Update the animations
    CStage3D::Instance().Update();

    // Update the 2D stage
    CStage2D::Instance().Update();

}	// Update


/***************************************************************************
*    desc:  Transform game objects
****************************************************************************/
void CCommonState::Transform()
{
    CStage2D::Instance().Transform();
    CUIMenuTree::Instance().Transform();

}	// Transform */


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CCommonState::PreRender()
{
    // Calc the finialized camera position
    CCamera::Instance().Finalize();
    CStage3D::Instance().Render(CCamera::Instance().GetFinalMatrix());

    CStage2D::Instance().Render();

}	// Render3D


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CCommonState::PostRender()
{
    CUIMenuTree::Instance().Render();

}	// PostRender2D






