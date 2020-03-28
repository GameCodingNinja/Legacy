
/************************************************************************
*    FILE NAME:       smartanisotropicbtn.cpp
*
*    DESCRIPTION:     Class CSmartAnisotropicBtn
************************************************************************/

// Physical component dependency
#include "smartanisotropicbtn.h"

// Game lib dependencies
#include <misc/settings.h>
#include <gui/uibuttonlist.h>
#include <utilities/genfunc.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartAnisotropicBtn::CSmartAnisotropicBtn( CUIControl * _pUIControl )
                    : CSmartSettingsMenuBtn( _pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the gui is to be displayed
****************************************************************************/
void CSmartAnisotropicBtn::Display()
{
    NGenFunc::DynCast<CUIButtonList *>(pUIControl)->SetActiveIndex( CSettings::Instance().GetTextFilterIndex() );

}	// Display


/***************************************************************************
*    decs:  Called when the control is to excute - Enable/disable the apply btn
****************************************************************************/
void CSmartAnisotropicBtn::Execute()
{
    EnableDisableApplyBtn();

}	// Execute







