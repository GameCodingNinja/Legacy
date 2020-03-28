
/************************************************************************
*    FILE NAME:       classtemplate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "runstate.h"

// Game lib dependencies
#include <3d/worldcamera.h>
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
void CCommonState::GetUserInput()
{
    CGameController::Instance().Poll();

    // Get the user input for the pause menu
    CUIMenuTree::Instance().HandleUserInput();

    if( CUIMenuTree::Instance().GetState() == NUIMenu::EMTS_HIDDEN )
        HandleGameInput();
        
    if( (CGameController::Instance().GetJoypadCount() == 0) &&
        JoypadCheckTimer.Expired(true) &&
        CGameController::Instance().CheckForGamepadControllers() )
    {
        CGameController::Instance().AddToPollLst( JOYPAD );
        CGameController::Instance().LoadJoyPadActionFromXML( string("data\\settings\\controllerMapping.cfg") );

        // Controller found and initialized. Timer no longer needed
        JoypadCheckTimer.Disable();
    }

}	// GetUserInput


/***************************************************************************
*    decs:  Act upon what the user is doing
****************************************************************************/
void CCommonState::HandleGameInput()
{
    if( CGameController::Instance().WasAction(string("music")) == EAP_DOWN )
    {
        if( !CXAct::Instance().IsBusy( string("music") ) )
            CXAct::Instance().Play( string("music") );
    }

    if( CGameController::Instance().WasAction(string("effect1")) == EAP_DOWN )
    {
        CXAct::Instance().Play( string("ammo_fire") );
    }

    if( CGameController::Instance().WasAction(string("effect2")) == EAP_DOWN )
    {
        CXAct::Instance().Play( string("droid_destroyed") );
    }

    // If the toggle menu action was called, toggle the menues off
    if( CGameController::Instance().WasAction( CUIMenuTree::Instance().GetToggleAction() ) == EAP_DOWN ||
        CGameController::Instance().WasAction( CUIMenuTree::Instance().GetEscapeAction() ) == EAP_DOWN )
    {
        CUIMenuTree::Instance().Activate( string("pause_menu") );
    }

}	// HandleGameInput */


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CCommonState::Update()
{
    // Update the menus
    CUIMenuTree::Instance().Update();

    // Update the 2D stage
    CStage2D::Instance().Update();

}	// Update


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CCommonState::PostRender()
{
    CUIMenuTree::Instance().Render();

}	// PostRender2D






