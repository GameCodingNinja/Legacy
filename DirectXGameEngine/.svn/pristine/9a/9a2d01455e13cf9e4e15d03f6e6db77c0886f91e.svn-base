
/************************************************************************
*    FILE NAME:       smartkeybindbtn.cpp
*
*    DESCRIPTION:     Class CSmartKeyBindBtn
************************************************************************/

// Physical component dependency
#include "smartkeybindbtn.h"

// Game lib dependencies
#include <gui/uibuttonlist.h>
#include <gui/uimenutree.h>
#include <gui/uimenu.h>
#include <utilities/genfunc.h>
#include <controller/gamecontroller.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartKeyBindBtn::CSmartKeyBindBtn( CUIControl * _pUIControl, uint btnIndex )
                 : CSmartGuiControl( _pUIControl ),
                   BUTTON_INDEX( btnIndex )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the control is created
****************************************************************************/
void CSmartKeyBindBtn::Create()
{
    string actionStr;
    string idStr;
    bool configurable;

    // Get the pointer to the menu
    CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu(string("controls_menu"));

    // Get the control's active index which mirrors EDeviceId
    CUIControl * pDevControl = pMenu->GetPtrToControl( string("control_device_list") );
    EDeviceId deviceId = static_cast<EDeviceId>(NGenFunc::DynCast<CUIButtonList *>(pDevControl)->GetActiveIndex());

    // See if we have a device action for this
    if( CGameController::Instance().GetDeviceActionStr(deviceId, BUTTON_INDEX, actionStr, idStr, configurable) )
    {
        pUIControl->DisplayFontString(idStr, 0);
        pUIControl->DisplayFontString(actionStr + ":", 1);
        pUIControl->SetVisible();

        NUIControl::EControlState state = NUIControl::ECS_DISABLED;

        if( configurable )
            state = NUIControl::ECS_ACTIVE;

        pUIControl->SetState(state);
        pUIControl->SetDefaultState(state);
    }
    else
        pUIControl->SetVisible(false);

}	// Create








