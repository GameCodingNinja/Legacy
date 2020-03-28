
/************************************************************************
*    FILE NAME:       smartapplysettingsbtn.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Physical component dependency
#include "smartapplysettingsbtn.h"

// Game lib dependencies
#include <system/xdevice.h>
#include <gui/uimenutree.h>
#include <gui/uicheckbox.h>
#include <gui/uibuttonlist.h>
#include <gui/uimenu.h>
#include <gui/uislider.h>
#include <managers/hudmanager.h>
#include <utilities/genfunc.h>
#include <managers/shader.h>
#include <controller/gamecontroller.h>

// Game dependencies
#include "../game/game.h"

// Game dependencies local
#include "smartresolutionbtn.h"

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartApplySettingsBtn::CSmartApplySettingsBtn( CUIControl * _pUIControl )
                       : CSmartSettingsMenuBtn( _pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the gui is to be displayed
****************************************************************************/
void CSmartApplySettingsBtn::Display()
{
    // Get the settings menu
    CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu("settings_menu");

    // Init as disabled
    pMenu->GetPtrToControl( "settings_apply_btn" )->SetState(NUIControl::ECS_DISABLED);

}	// Display


/***************************************************************************
*    decs:  Called when the control is executed - Applies the settings
****************************************************************************/
void CSmartApplySettingsBtn::Execute()
{
    bool settingsChangeMade(false);

    // Get the settings menu
    CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu("settings_menu");

    // Get the control
    CUIControl * pControl;

    // Check for the resolution button list
    pControl = pMenu->GetPtrToControl( "resolution_btn_lst" );
    CSmartResolutionBtn * pSmartGuiResBtn = NGenFunc::DynCast<CSmartResolutionBtn *>(pControl->GetSmartGuiPtr());
    if( pSmartGuiResBtn->WasResolutionChanged() )
    {
        pSmartGuiResBtn->SetResolutionChange();
        settingsChangeMade = true;
    }

    // Check for the anisotropic button list
    pControl = pMenu->GetPtrToControl( "anisotropic_btn_lst" );
    if( CSettings::Instance().GetTextFilterIndex() != NGenFunc::DynCast<CUIButtonList *>(pControl)->GetActiveIndex() )
    {
        CSettings::Instance().SetTextFilterByIndex( NGenFunc::DynCast<CUIButtonList *>(pControl)->GetActiveIndex() );
        settingsChangeMade = true;
    }

    // Check for the full screen check box
    pControl = pMenu->GetPtrToControl( "full_screen_check_box" );
    if( CSettings::Instance().GetFullScreen() != NGenFunc::DynCast<CUICheckBox *>(pControl)->GetToggleState() )
    {
        CSettings::Instance().SetFullScreenChange( NGenFunc::DynCast<CUICheckBox *>(pControl)->GetToggleState() );
        settingsChangeMade = true;
    }

    // Check for the v-sync check box
    pControl = pMenu->GetPtrToControl( "v-sync_check_box" );
    if( CSettings::Instance().GetVSync() != NGenFunc::DynCast<CUICheckBox *>(pControl)->GetToggleState() )
    {
        CSettings::Instance().SetVSyncChange( NGenFunc::DynCast<CUICheckBox *>(pControl)->GetToggleState() );
        settingsChangeMade = true;
    }

    // Check for dead zone slider
    pControl = pMenu->GetPtrToControl( "settings_dead_zone_slider" );
    if( CSettings::Instance().GetJoyPadStickDeadZone() != ((int)NGenFunc::DynCast<CUISlider *>(pControl)->GetValue() * 100) )
    {
        CSettings::Instance().SetJoyPadStickDeadZone( (int)NGenFunc::DynCast<CUISlider *>(pControl)->GetValue() * 100 );
        CGameController::Instance().SetJoypadDeadZone( CSettings::Instance().GetJoyPadStickDeadZone() );
        settingsChangeMade = true;
    }

    // See if we need to reset the device
    if( ShouldResetDevice() )
    {
        CGame::Instance().HandleDeviceReset(
            CSettings::Instance().GetSizeChange(),
            CSettings::Instance().GetVSyncChange(),
            !CSettings::Instance().GetFullScreenChange() );

        CGame::Instance().ResetWindowSize();

        // Reset the dynamic positions
        CUIMenuTree::Instance().ResetDynamicOffset();
        CHudManager::Instance().ResetDynamicOffset();
    }
    // Settings that don't require a device reset
    else if( CSettings::Instance().WasTextFilteringChanged() )
    {
        CEffectData * pEffectData = CShader::Instance().GetEffectData("gausian_lights");

        CShader::Instance().SetEffectValue( 
            pEffectData, 
            "textureFilter", 
            CXDevice::Instance().GetTextureFiltering( CSettings::Instance().GetTextFilteringChange() ) );

        CShader::Instance().SetEffectValue( 
            pEffectData, 
            "anisotropyRate", 
            CXDevice::Instance().GetAnisotropicFiltering( CSettings::Instance().GetTextFilteringChange() ) );
    }

    // Only save if a change was actually made
    if( settingsChangeMade )
        CSettings::Instance().SaveSettings();

}	// Execute


/************************************************************************
*    desc:  Should the DirectX device be reset
************************************************************************/
bool CSmartApplySettingsBtn::ShouldResetDevice()
{
    bool directXReset(false);

    if( CSettings::Instance().WasScreenModeChanged() ||
        CSettings::Instance().WasResolutionChanged() ||
        CSettings::Instance().WasVSyncChanged() )
    {
        directXReset = true;
    }

    return directXReset;

}	// ShouldResetDevice







