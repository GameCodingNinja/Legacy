
/************************************************************************
*    FILE NAME:       smartapplysettingsbtn.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Physical component dependency
#include "smartapplysettingsbtn.h"

// Game lib dependencies
#include <system/xwindow.h>
#include <gui/uimenutree.h>
#include <gui/uicheckbox.h>
#include <gui/uibuttonlist.h>
#include <gui/uimenu.h>
#include <utilities/genfunc.h>
#include <managers/shader.h>

// Game dependencies
#include "smartresolutionbtn.h"
#include "liminality.h"

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
    CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu(string("settings_menu"));

    // Init as disabled
    pMenu->GetPtrToControl( string("settings_apply_button") )->SetState(NUIControl::ECS_DISABLED);

}	// Display


/***************************************************************************
*    decs:  Called when the control is executed - Applies the settings
****************************************************************************/
void CSmartApplySettingsBtn::Execute()
{
    bool settingsChangeMade(false);

    // Get the settings menu
    CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu(string("settings_menu"));

    // Get the control
    CUIControl * pControl;

    // Check for the resolution button list
    pControl = pMenu->GetPtrToControl( string("resolution_button_list") );
    CSmartResolutionBtn * pSmartGuiResBtn = NGenFunc::DynCast<CSmartResolutionBtn *>(pControl->GetSmartGuiPtr());
    if( pSmartGuiResBtn->WasResolutionChanged() )
    {
        pSmartGuiResBtn->SetResolutionChange();
        settingsChangeMade = true;
    }

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

    // See if we need to reset the device
    if( ShouldResetDevice() )
    {
        CLiminality::Instance().HandleDeviceReset(
            CSettings::Instance().GetSizeChange(),
            CSettings::Instance().GetVSyncChange(),
            !CSettings::Instance().GetFullScreenChange() );

        CXWindow::Instance().ResetWindowSize();
    }
    // Settings that don't require a device reset
    else if( CSettings::Instance().WasTextFilteringChanged() )
    {
        CEffectData * pEffectData = CShader::Instance().GetEffectData(string("gausian_lights"));

        CShader::Instance().SetEffectValue( 
            pEffectData, 
            string("textureFilter"), 
            CXWindow::Instance().GetTextureFiltering( CSettings::Instance().GetTextFilteringChange() ) );

        CShader::Instance().SetEffectValue( 
            pEffectData, 
            string("anisotropyRate"), 
            CXWindow::Instance().GetAnisotropicFiltering( CSettings::Instance().GetTextFilteringChange() ) );
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







