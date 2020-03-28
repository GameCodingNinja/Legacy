
/************************************************************************
*    FILE NAME:       smartsettingsmenubtn.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Physical component dependency
#include "smartsettingsmenubtn.h"

// Game lib dependencies
#include <gui/uimenutree.h>
#include <gui/uicheckbox.h>
#include <gui/uibuttonlist.h>
#include <gui/uimenu.h>
#include <utilities/genfunc.h>

// Game dependencies
#include "smartresolutionbtn.h"

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartSettingsMenuBtn::CSmartSettingsMenuBtn( CUIControl * _pUIControl )
                      : CSmartGuiControl( _pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Enable/disable the apply btn
****************************************************************************/
void CSmartSettingsMenuBtn::EnableDisableApplyBtn()
{
    // Get the settings menu
    CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu(string("settings_menu"));

    if( WasSettingsButtonsChanged() )
        pMenu->GetPtrToControl( string("settings_apply_button") )->SetState(NUIControl::ECS_INACTIVE);
    else
        pMenu->GetPtrToControl( string("settings_apply_button") )->SetState(NUIControl::ECS_DISABLED);

}	// EnableDisableSettingsApplyBtn


/***************************************************************************
*    decs:  Were the buttons in the settings menu changed
****************************************************************************/
bool CSmartSettingsMenuBtn::WasSettingsButtonsChanged()
{
    // Get the settings menu
    CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu(string("settings_menu"));

    // Get the control
    CUIControl * pControl;

    // Check for the resolution button list
    pControl = pMenu->GetPtrToControl( string("resolution_button_list") );
    CSmartResolutionBtn * pSmartGuiResBtn = NGenFunc::DynCast<CSmartResolutionBtn *>(pControl->GetSmartGuiPtr());
    if( pSmartGuiResBtn->WasResolutionChanged() )
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







