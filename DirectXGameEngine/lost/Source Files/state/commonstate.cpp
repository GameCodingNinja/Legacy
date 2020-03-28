
/************************************************************************
*    FILE NAME:       commonstate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "commonstate.h"

// Game lib dependencies
#include <controller/gamecontroller.h>
#include <gui/uimenutree.h>
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
    {
        CUIMenuTree::Instance().HandleUserInput();

        if( CUIMenuTree::Instance().GetState() == NUIMenu::EMTS_WAIT_INPUT )
            HandleCustomMenuToggle();
    }
    else
        HandleGameInput();
    
    if( (CGameController::Instance().GetJoypadCount() == 0) &&
        JoypadCheckTimer.Expired(CTimer::RESTART_ON_EXPIRE) &&
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
    else if( CGameController::Instance().WasAction( "Loadout Menu" ) == NDevice::EAP_DOWN )
    {
        CUIMenuTree::Instance().Activate( "loadout_menu" );
    }
    else if( CGameController::Instance().WasAction( "Map" ) == NDevice::EAP_DOWN )
    {
        CUIMenuTree::Instance().Activate( "map_menu" );
    }
    else if( CGameController::Instance().WasAction( "Craft" ) == NDevice::EAP_DOWN )
    {
        CUIMenuTree::Instance().Activate( "craft_menu" );
    }
    else if( CGameController::Instance().WasAction( "Inventory" ) == NDevice::EAP_DOWN )
    {
        CUIMenuTree::Instance().Activate( "inventory_menu" );
    }
    else if( CGameController::Instance().WasAction( "Mod Ship" ) == NDevice::EAP_DOWN )
    {
        CUIMenuTree::Instance().Activate( "mod_ship_menu" );
    }

}	// HandleGameInput */


/***************************************************************************
*    decs:  Act upon any custom menu toggle
****************************************************************************/
void CCommonState::HandleCustomMenuToggle()
{
    if( CGameController::Instance().WasAction( "Loadout Menu" ) == NDevice::EAP_DOWN )
    {
        CUIMenuTree::Instance().SetMenuReturn( "loadout_menu" );
    }
    else if( CGameController::Instance().WasAction( "Map" ) == NDevice::EAP_DOWN )
    {
        CUIMenuTree::Instance().SetMenuReturn( "map_menu" );
    }
    else if( CGameController::Instance().WasAction( "Craft" ) == NDevice::EAP_DOWN )
    {
        CUIMenuTree::Instance().SetMenuReturn( "craft_menu" );
    }
    else if( CGameController::Instance().WasAction( "Inventory" ) == NDevice::EAP_DOWN )
    {
        CUIMenuTree::Instance().SetMenuReturn( "inventory_menu" );
    }
    else if( CGameController::Instance().WasAction( "Mod Ship" ) == NDevice::EAP_DOWN )
    {
        CUIMenuTree::Instance().SetMenuReturn( "mod_ship_menu" );
    }

}	// HandleCustomMenuToggle */


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CCommonState::Update()
{
    // Update the menus
    CUIMenuTree::Instance().Update();

}	// Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CCommonState::Transform()
{
    CUIMenuTree::Instance().Transform();

}	// Transform */


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CCommonState::PreRender()
{
}	// Render3D


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CCommonState::PostRender()
{
    CUIMenuTree::Instance().Render();

}	// PostRender2D






