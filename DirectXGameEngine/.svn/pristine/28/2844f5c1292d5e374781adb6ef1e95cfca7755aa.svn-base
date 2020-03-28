
/************************************************************************
*    FILE NAME:       smartconfirmbtn.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Game OS dependencies
                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

// Physical component dependency
#include "smartconfirmbtn.h"

// Game lib dependencies
#include <gui/uicontrol.h>
#include <gui/uimenutree.h>
#include <gui/uimenu.h>
#include <controller/gamecontroller.h>

// Game dependencies
#include "smartexitbtn.h"
#include "smartresetkeybind.h"

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartConfirmBtn::CSmartConfirmBtn( CUIControl * _pUIControl )
                : CSmartGuiControl( _pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the control is executed
****************************************************************************/
void CSmartConfirmBtn::Execute()
{
    CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu("confirmation_menu");
    CUIControl * pYesBtn = pMenu->GetPtrToControl("yes_button");
    CUIControl * pMegLbl = pMenu->GetPtrToControl("message_label");

    CSmartGuiControl * pSmartGuiCtrl(NULL);
    std::string conformationMsg;
    std::string executionAction;
    NUIControl::EControlActionType actionType(NUIControl::ECAT_RETURN);

    if( pUIControl->GetName() == "exit_btn" )
    {
        pSmartGuiCtrl = new CSmartExitBtn( pYesBtn );
        conformationMsg = "Do you want to|quit the game?";
    }
    else if( pUIControl->GetName() == "Key_Binding_reset_btn" )
    {
        pSmartGuiCtrl = new CSmartResetKeyBindBtn( pYesBtn );
        conformationMsg = "Reset all key bindings|to their default settings?";
    }
    else if( pUIControl->GetName() == "main_menu_btn" )
    {
        conformationMsg = "Are you sure you|want to go back to|the main menu?";

        // Set the action type to kill the menu
        actionType = NUIControl::ECAT_BACK_TO_MAIN_MENU;
    }

    // Set the conformation menu
    pYesBtn->SetSmartGui( pSmartGuiCtrl );
    pYesBtn->SetActionType( actionType );
    pYesBtn->SetExecutionAction( executionAction );
    pMegLbl->DisplayFontString( conformationMsg );

}	// Execute







