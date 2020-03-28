
/************************************************************************
*    FILE NAME:       smartbtncapturemenu.cpp
*
*    DESCRIPTION:     Class CSmartBtnCaptureLbl
************************************************************************/

// Physical component dependency
#include "smartbtncapturemenu.h"

// Game lib dependencies
#include <gui/uimenutree.h>
#include <gui/uimenu.h>
#include <gui/uibuttonlist.h>
#include <controller/gamecontroller.h>
#include <utilities/genfunc.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartBtnCaptureMenu::CSmartBtnCaptureMenu( CUIMenu * _pUIMenu )
                     : CSmartGuiMenu( _pUIMenu )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the gui is to be displayed
****************************************************************************/
void CSmartBtnCaptureMenu::Display()
{
    lastButtonID = -1;
    keyBindReady = false;
    CUIMenuTree::Instance().SetDisableAllExceptEsc(true);

}	// Display


/***************************************************************************
*    decs:  Called during the handle user imput
****************************************************************************/
void CSmartBtnCaptureMenu::HandleUserInput()
{
    CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu(string("controls_menu"));

    CUIControl * pControl = pMenu->GetPtrToControl( string("control_device_list") );

    EDeviceId deviceId = static_cast<EDeviceId>(NGenFunc::DynCast<CUIButtonList *>(pControl)->GetActiveIndex());

    int buttonID = CGameController::Instance().ScanDeviceForButtonPress( deviceId );

    // The Esc key is the only key allowd to cancel out of the key bindings
    // menu so trap it here and reset because this key can never be mapped.
    if( (deviceId == KEYBOARD) && (buttonID == KEY_ESCAPE) )
        buttonID = -1;

    // Wait for all buttons to be released before we're ready to bind a key
    if( lastButtonID == -1 && buttonID == -1 )
        keyBindReady = true;

    if( (lastButtonID > -1) && (buttonID == -1) && keyBindReady )
    {
        pControl = pMenu->GetActiveControl();
        if( pControl != NULL )
        {
            string componetIdStr = CGameController::Instance().GetDeviceComponetIdStr( deviceId, lastButtonID );

            if( deviceId == JOYPAD )
                pControl->DisplayFontString( CGameController::Instance().GetJoypadDisplayStr(componetIdStr) );
            else
                pControl->DisplayFontString( componetIdStr );

            CGameController::Instance().SetKeyBinding( deviceId, pControl->GetID()-1, lastButtonID );
            CGameController::Instance().SaveToFile();
        }

        // Transition out of the menu
        CUIMenuTree::Instance().SimulateEscapeActionOutOfMenu();
    }

    lastButtonID = buttonID;

}	// HandleUserInput






