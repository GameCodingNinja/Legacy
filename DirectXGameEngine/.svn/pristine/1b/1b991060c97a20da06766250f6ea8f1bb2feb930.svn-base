
/************************************************************************
*    FILE NAME:       classtemplate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "runstate.h"

// Game lib dependencies
#include <controller/gamecontroller.h>
#include <gui/uimenutree.h>
#include <2d/stage2d.h>
#include <utilities/highresolutiontimer.h>

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

    // Get the user input for the menu system
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

    // Update the 2D stage
    //CStage2D::Instance().Update();

}	// Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CCommonState::Transform()
{
    CUIMenuTree::Instance().Transform();
    //CStage2D::Instance().Transform();

}	// Transform */


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CCommonState::PreRender()
{
    //CStage2D::Instance().Render();

}	// Render3D


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CCommonState::PostRender()
{
    CUIMenuTree::Instance().Render();

}	// PostRender2D






