
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
#include <gui/uislider.h>
#include <utilities/genfunc.h>
#include <misc/settings.h>

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
    CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu("settings_menu");

    if( WasSettingsButtonsChanged() )
        pMenu->GetPtrToControl( "settings_apply_btn" )->SetState(NUIControl::ECS_INACTIVE);
    else
        pMenu->GetPtrToControl( "settings_apply_btn" )->SetState(NUIControl::ECS_DISABLED);

}	// EnableDisableSettingsApplyBtn


/***************************************************************************
*    decs:  Were the buttons in the settings menu changed
****************************************************************************/
bool CSmartSettingsMenuBtn::WasSettingsButtonsChanged()
{
    // Get the settings menu
    CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu("settings_menu");

    // Get the control
    CUIControl * pControl;

    // Check for the resolution button list
    pControl = pMenu->GetPtrToControl( "resolution_btn_lst" );
    CSmartResolutionBtn * pSmartGuiResBtn = NGenFunc::DynCast<CSmartResolutionBtn *>(pControl->GetSmartGuiPtr());
    if( pSmartGuiResBtn->WasResolutionChanged() )
        return true;

    // Check for the anisotropic button list
    pControl = pMenu->GetPtrToControl( "anisotropic_btn_lst" );
    if( CSettings::Instance().GetTextFilterIndex() != NGenFunc::DynCast<CUIButtonList *>(pControl)->GetActiveIndex() )
        return true;

    // Check for the full screen check box
    pControl = pMenu->GetPtrToControl( "full_screen_check_box" );
    if( CSettings::Instance().GetFullScreen() != NGenFunc::DynCast<CUICheckBox *>(pControl)->GetToggleState() )
        return true;

    // Check for the v-sync check box
    pControl = pMenu->GetPtrToControl( "v-sync_check_box" );
    if( CSettings::Instance().GetVSync() != NGenFunc::DynCast<CUICheckBox *>(pControl)->GetToggleState() )
        return true;

    // Check for dead zone slider
    pControl = pMenu->GetPtrToControl( "settings_dead_zone_slider" );
    if( CSettings::Instance().GetJoyPadStickDeadZone() != ((int)NGenFunc::DynCast<CUISlider *>(pControl)->GetValue() * 100) )
        return true;

    return false;

}	// WasSettingsChanged







