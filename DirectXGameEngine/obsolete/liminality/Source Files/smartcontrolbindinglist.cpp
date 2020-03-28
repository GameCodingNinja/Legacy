
/************************************************************************
*    FILE NAME:       smartcontrolbindinglist.cpp
*
*    DESCRIPTION:     Class CSmartCtrlBindLstBtn
************************************************************************/

// Physical component dependency
#include "smartcontrolbindinglist.h"

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
CSmartCtrlBindLstBtn::CSmartCtrlBindLstBtn( CUIControl * _pUIControl )
                     : CSmartGuiControl( _pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the control is created
****************************************************************************/
void CSmartCtrlBindLstBtn::Create()
{
    // Set the keyboard as the first active item in the list
    NGenFunc::DynCast<CUIButtonList *>(pUIControl)->SetActiveIndex( 0 );
}


/***************************************************************************
*    decs:  Called when the control is executed
****************************************************************************/
void CSmartCtrlBindLstBtn::Execute()
{
    string actionStr;
    string idStr;
    bool configurable;
    int controlCounter = 0;

    // Get the pointer to the menu
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

}	// Execute







