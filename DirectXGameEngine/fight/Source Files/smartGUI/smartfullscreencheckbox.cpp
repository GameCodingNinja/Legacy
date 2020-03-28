
/************************************************************************
*    FILE NAME:       smartfullscreencheckbox.cpp
*
*    DESCRIPTION:     Class CSmartScrnCheckBox
************************************************************************/

// Physical component dependency
#include "smartfullscreencheckbox.h"

// Game lib dependencies
#include <misc/settings.h>
#include <gui/uicheckbox.h>
#include <utilities/genfunc.h>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartScrnCheckBox::CSmartScrnCheckBox( CUIControl * _pUIControl )
                   : CSmartSettingsMenuBtn( _pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the gui is to be displayed
****************************************************************************/
void CSmartScrnCheckBox::Display()
{
    NGenFunc::DynCast<CUICheckBox *>(pUIControl)->SetToggleState( CSettings::Instance().GetFullScreen() );

}	// Display


/***************************************************************************
*    decs:  Called when the control is executed - Enable/disable the apply btn
****************************************************************************/
void CSmartScrnCheckBox::Execute()
{
    EnableDisableApplyBtn();

}	// Execute







