/************************************************************************
*    FILE NAME:       gameaction.cpp
*
*    DESCRIPTION:     Extension of the CGame class for game specific actions
************************************************************************/

// Physical component dependency
#include "liminality.h"

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <system/xwindow.h>
#include <common/defs.h>
#include <gui/uimenu.h>
#include <gui/uicheckbox.h>
#include <gui/uibuttonlist.h>
#include <controller/gamecontroller.h>
#include <utilities/genfunc.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Perform an action made from the ui control init
*
*	 param: uiControl - user interface control
************************************************************************/
void CLiminality::PerformUIMenuUserInput( CUIMenu * pMenu )
{
	if( pMenu->GetName() == "button_capture_overlay" )
		HandleMenuUserInput( pMenu );

	// Unsubscribe if we escaped to a different menu
	else
		menuUserInputSubscriber.Unsubscribe();

}	// PerformUIMenuUserInput


/************************************************************************
*    desc:  Perform an action made from the ui control init
*
*	 param: uiControl - user interface control
************************************************************************/
void CLiminality::PerformUIControlInit( CUIControl * pControl )
{
	// Check for the resolution button list
	if( pControl->GetExecutionAction() == "resolution_button_list" )
		SetResStringsToControl( pControl );

	// Set the keyboard as the first active item in the list
	else if( pControl->GetExecutionAction() == "control_device_list" )
		NGenFunc::DynCast<CUIButtonList *>(pControl)->SetActiveIndex( 0 );

}	// PerformUIControlInit


/************************************************************************
*    desc:  Perform an action made from the ui control
*
*	 param: uiControl - user interface control
************************************************************************/
void CLiminality::PerformUIControlAction( CUIControl * pControl )
{
	assert(pControl != NULL && "Callback control pointer is NULL!");

	// Exit the game
	if( pControl->GetExecutionAction() == "exit_game" )
		ExitGame();

	// Init the settings menu
	else if( pControl->GetExecutionAction() == "settings_menu" )
		InitSettingsMenu();

	// Save the game settings
	else if( pControl->GetExecutionAction() == "settings_apply_button" )
		SaveGameSettings();

	if( pControl->GetExecutionAction() == "controls_menu" )
		InitControlsMenu();

	// React to button list control change
	else if( pControl->GetExecutionAction() == "control_device_list" )
		InitControlsMenu();

	// React to button list control change
	else if( pControl->GetExecutionAction() == "button_capture_overlay" )
	{
		lastButtonID = -1;
		CUIMenuTree::Instance().SetDisableAllExceptEsc(true);
		menuUserInputSubscriber.Subscribe();
	}

	// See if any of the settings buttons have changed from their default values
	else if( (pControl->GetExecutionAction() == "resolution_button_list") || 
		     (pControl->GetExecutionAction() == "full_screen_check_box") ||
			 (pControl->GetExecutionAction() == "v-sync_check_box") )
	{
		EnableDisableSettingsApplyBtn();
	}

}	// PerformUIControlAction


/***************************************************************************
*    decs:  Enable/disable the settings apply btn
****************************************************************************/
void CLiminality::EnableDisableSettingsApplyBtn()
{
	// Get the settings menu
	CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu(string("settings_menu"));

	if( WasSettingsChanged() )
		pMenu->GetPtrToControl( string("settings_apply_button") )->SetState(NUIControl::ECS_INACTIVE);
	else
		pMenu->GetPtrToControl( string("settings_apply_button") )->SetState(NUIControl::ECS_DISABLED);

}	// EnableDisableSettingsApplyBtn


/***************************************************************************
*    decs:  Was the settings changed
****************************************************************************/
bool CLiminality::WasSettingsChanged()
{
	// Get the settings menu
	CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu(string("settings_menu"));

	// Get the control
	CUIControl * pControl;

	// Check for the resolution button list
	pControl = pMenu->GetPtrToControl( string("resolution_button_list") );
	if( resIndex != NGenFunc::DynCast<CUIButtonList *>(pControl)->GetActiveIndex() )
		return true;

	// Check for the full screen check box
	pControl = pMenu->GetPtrToControl( string("full_screen_check_box") );
	if( CSettings::Instance().GetFullScreen() != NGenFunc::DynCast<CUICheckBox *>(pControl)->GetToggleState() )
		return true;

	// Check for the v-sync check box
	pControl = pMenu->GetPtrToControl( string("v-sync_check_box") );
	if( CSettings::Instance().GetVSync() != NGenFunc::DynCast<CUICheckBox *>(pControl)->GetToggleState() )
		return true;

	return false;

}	// WasSettingsChanged


/***************************************************************************
*    decs:  Init the settings menu
****************************************************************************/
void CLiminality::InitSettingsMenu()
{
	// Get the settings menu
	CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu(string("settings_menu"));

	// Get the control
	CUIControl * pControl;

	// Check for the full screen check box
	pControl = pMenu->GetPtrToControl( string("full_screen_check_box") );
	NGenFunc::DynCast<CUICheckBox *>(pControl)->SetToggleState( CSettings::Instance().GetFullScreen() );

	// Check for the v-sync check box
	pControl = pMenu->GetPtrToControl( string("v-sync_check_box") );
	NGenFunc::DynCast<CUICheckBox *>(pControl)->SetToggleState( CSettings::Instance().GetVSync() );

	// Check for the resolution button list
	pControl = pMenu->GetPtrToControl( string("resolution_button_list") );
		if( resIndex > -1 )
			NGenFunc::DynCast<CUIButtonList *>(pControl)->SetActiveIndex( resIndex );

	// Reset back to disabled
	pMenu->GetPtrToControl( string("settings_apply_button") )->SetState(NUIControl::ECS_DISABLED);

}	// InitSettingsMenu


/***************************************************************************
*    decs:  Init the controls menu
****************************************************************************/
void CLiminality::InitControlsMenu()
{
	string actionStr;
	string idStr;
	bool configurable;
	int controlCounter = 0;

	CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu(string("controls_menu"));

	// Get the control's active index which mirrors EDeviceId
	CUIControl * pControl = pMenu->GetPtrToControl( string("control_device_list") );
	EDeviceId deviceId = static_cast<EDeviceId>(NGenFunc::DynCast<CUIButtonList *>(pControl)->GetActiveIndex());

	for( uint i = 0; i < pMenu->GetControlCount(); i++ )
	{
		// Get the control
		CUIControl * pControl = pMenu->GetPtrToControl(i);

		if( pControl->GetExecutionAction() == "button_capture_overlay" )
		{
			if( CGameController::Instance().GetDeviceActionStr(deviceId, controlCounter++, actionStr, idStr, configurable) )
			{
				pControl->DisplayFontString(idStr, 0);
				pControl->DisplayFontString(actionStr + ":", 1);
				pControl->SetVisible();

				NUIControl::EControlState state = NUIControl::ECS_DISABLED;

				if( configurable )
					state = NUIControl::ECS_ACTIVE;

				pControl->SetState(state);
				pControl->SetDefaultState(state);
			}
			else
				pControl->SetVisible(false);
		}
	}

}	// InitControlsMenu


/***************************************************************************
*    decs:  Handle the menu user input
****************************************************************************/
void CLiminality::HandleMenuUserInput( CUIMenu * pMenu )
{
	if( pMenu->GetName() == "button_capture_overlay" )
	{
		CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu(string("controls_menu"));

		CUIControl * pControl = pMenu->GetPtrToControl( string("control_device_list") );

		EDeviceId deviceId = static_cast<EDeviceId>(NGenFunc::DynCast<CUIButtonList *>(pControl)->GetActiveIndex());

		int buttonID = CGameController::Instance().ScanDeviceForButtonPress( deviceId );

		// The Esc key is the only key allowd to cancel out of the key bindings
		// menu so trap it here and reset because this key can never be mapped.
		if( (deviceId == KEYBOARD) && (buttonID == KEY_ESCAPE) )
			buttonID = -1;

		// Wait for all buttons to be released before we're ready to bind a key
		if( lastButtonID == -1 && buttonID == -1 )
			keyBindReady = true;

		if( (lastButtonID > -1) && (buttonID == -1) && keyBindReady )
		{
			pControl = pMenu->GetActiveControl();
			if( pControl != NULL )
			{
				string componetIdStr = CGameController::Instance().GetDeviceComponetIdStr( deviceId, lastButtonID );

				if( deviceId == JOYPAD )
					pControl->DisplayFontString( CGameController::Instance().GetJoypadDisplayStr(componetIdStr) );
				else
					pControl->DisplayFontString( componetIdStr );

				CGameController::Instance().SetKeyBinding( deviceId, pControl->GetID()-1, lastButtonID );
				CGameController::Instance().SaveToFile();
			}

			// Unscribe so that we stop checking for the key press
			menuUserInputSubscriber.Unsubscribe();

			// Transition out of the menu
			CUIMenuTree::Instance().SimulateEscapeActionOutOfMenu();
		}

		lastButtonID = buttonID;
	}

}	// HandleMenuUserInput


/***************************************************************************
*    decs:  Save the game settings
****************************************************************************/
void CLiminality::SaveGameSettings()
{
	// Get the settings menu
	CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu(string("settings_menu"));

	bool settingsChangeMade(false);

	// Get the control
	CUIControl * pControl;

	// Check for the full screen check box
	pControl = pMenu->GetPtrToControl( string("full_screen_check_box") );
	if( CSettings::Instance().GetFullScreen() != NGenFunc::DynCast<CUICheckBox *>(pControl)->GetToggleState() )
	{
		CSettings::Instance().SetFullScreenChange( NGenFunc::DynCast<CUICheckBox *>(pControl)->GetToggleState() );
		settingsChangeMade = true;
	}

	// Check for the v-sync check box
	pControl = pMenu->GetPtrToControl( string("v-sync_check_box") );
	if( CSettings::Instance().GetVSync() != NGenFunc::DynCast<CUICheckBox *>(pControl)->GetToggleState() )
	{
		CSettings::Instance().SetVSyncChange( NGenFunc::DynCast<CUICheckBox *>(pControl)->GetToggleState() );
		settingsChangeMade = true;
	}

	// Check for the resolution button list
	pControl = pMenu->GetPtrToControl( string("resolution_button_list") );
	if( resIndex != NGenFunc::DynCast<CUIButtonList *>(pControl)->GetActiveIndex() )
	{
		resIndex = NGenFunc::DynCast<CUIButtonList *>(pControl)->GetActiveIndex();

		pair<int, int> resPair = resVec[resIndex];
		CSettings::Instance().SetChangeSize( CSize<float>(resPair.first, resPair.second) );

		settingsChangeMade = true;
	}

	// See if we need to reset the device
	if( ShouldResetDevice() )
	{
		HandleDeviceReset( CSettings::Instance().GetSizeChange(),
						   CSettings::Instance().GetVSyncChange(),
						   !CSettings::Instance().GetFullScreenChange() );

		CXWindow::Instance().ResetWindowSize();

		// reposition the title screen menu
		if( CSettings::Instance().WasResolutionChanged() )
		{
			CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu( string("title_screen_menu") );
			CSize<float> offset = CSettings::Instance().GetDefaultSizeHalf() - (pMenu->GetSize() * .5f);
			pMenu->SetPos( CPoint(offset.w, -offset.h, pMenu->GetPos().z) - CPoint(100, -100, 0) );
			pMenu->Transform();
		}
	}

	// Only save if a change was actually made
	if( settingsChangeMade )
		CSettings::Instance().SaveSettings();

}	// SaveGameSettings


/***************************************************************************
*    decs:  Exit the game
****************************************************************************/
void CLiminality::ExitGame()
{
	StopGame();
	PostMessage( CXWindow::Instance().GetWndHandle(), WM_CLOSE, 0, 0 );
}


/***************************************************************************
*    decs:  Set the resolution strings to the control
****************************************************************************/
void CLiminality::SetResStringsToControl( CUIControl * pControl )
{
    DEVMODE dm;
	memset(&dm, 0, sizeof(DEVMODE));
	dm.dmSize = sizeof(DEVMODE);
	DWORD mode = 0;

	DEVMODE dmCur;
	memset(&dmCur, 0, sizeof(DEVMODE));
	dmCur.dmSize = sizeof(DEVMODE);

	HMONITOR hMonitor = MonitorFromWindow(CXWindow::Instance().GetWndHandle(), MONITOR_DEFAULTTOPRIMARY);
    MONITORINFOEX monInfo;
    memset(&monInfo, 0, sizeof(MONITORINFOEX));
    monInfo.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(hMonitor, &monInfo);

	// Get the current display size from the settings
	CSize<float> & size = CSettings::Instance().GetSize();

	int counter(0);
	resIndex = -1;

	// Add all the display resolutions to the button list
	if( EnumDisplaySettings(monInfo.szDevice, ENUM_CURRENT_SETTINGS, &dmCur) )
	{
		// Use a set to store only unique resolution strings
		vector<string> resStrVec;

		// search for the mode we are looking for
		mode = 0;
		while( EnumDisplaySettings( NULL, mode++, &dm ) )
		{
			// We only care about 32-bit displays of the same frequency
			if( (dm.dmBitsPerPel == dmCur.dmBitsPerPel) &&
				(dm.dmDisplayFrequency == dmCur.dmDisplayFrequency) )
			{
				string resStr = boost::str( boost::format("%d x %d") % dm.dmPelsWidth % dm.dmPelsHeight );

				// Keep out the duplicates
				if( find(resStrVec.begin(), resStrVec.end(), resStr) == resStrVec.end() )
				{
					resStrVec.push_back(resStr);
					pair<int, int> resPair(dm.dmPelsWidth, dm.dmPelsHeight);
					resVec.push_back( resPair );

					if( size.w == dm.dmPelsWidth && size.h == dm.dmPelsHeight )
						resIndex = counter;

					counter++;
				}
			}
		}

		// Set the unique resolution strings
		for( vector<string>::iterator it = resStrVec.begin(); it != resStrVec.end(); it++ )
			pControl->SetToFontStringList( (*it) );
	}

	if( resIndex > -1 )
		NGenFunc::DynCast<CUIButtonList *>(pControl)->SetActiveIndex( resIndex );

}	// SetResStringsToControl
