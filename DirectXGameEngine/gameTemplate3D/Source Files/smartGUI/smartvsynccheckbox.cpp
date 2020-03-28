
/************************************************************************
*    FILE NAME:       smartvsynccheckbox.cpp
*
*    DESCRIPTION:     Class CSmartVSyncCheckBox
************************************************************************/

// Physical component dependency
#include "smartvsynccheckbox.h"

// Game lib dependencies
#include <misc/settings.h>
#include <gui/uicheckbox.h>
#include <utilities/genfunc.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartVSyncCheckBox::CSmartVSyncCheckBox( CUIControl * _pUIControl )
                    : CSmartSettingsMenuBtn( _pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the gui is to be displayed
****************************************************************************/
void CSmartVSyncCheckBox::Display()
{
    NGenFunc::DynCast<CUICheckBox *>(pUIControl)->SetToggleState( CSettings::Instance().GetVSync() );

}	// Display


/***************************************************************************
*    decs:  Called when the control is executed - Enable/disable the apply btn
****************************************************************************/
void CSmartVSyncCheckBox::Execute()
{
    EnableDisableApplyBtn();

}	// Execute







