
/************************************************************************
*    FILE NAME:       smartdeadzoneslider.cpp
*
*    DESCRIPTION:     Class CSmartVSyncCheckBox
************************************************************************/

// Physical component dependency
#include "smartdeadzoneslider.h"

// Game lib dependencies
#include <misc/settings.h>
#include <gui/uislider.h>
#include <utilities/genfunc.h>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartDeadZoneSlider::CSmartDeadZoneSlider( CUIControl * _pUIControl )
                    : CSmartSettingsMenuBtn( _pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the gui is to be displayed
****************************************************************************/
void CSmartDeadZoneSlider::Display()
{
    // Max setting for the controller deadzone is 10,000
    NGenFunc::DynCast<CUISlider *>(pUIControl)->SetSlider( CSettings::Instance().GetJoyPadStickDeadZone() / 100 );

}	// Display


/***************************************************************************
*    decs:  Called when the control is executed - Enable/disable the apply btn
****************************************************************************/
void CSmartDeadZoneSlider::Execute()
{
    EnableDisableApplyBtn();

}	// Execute







