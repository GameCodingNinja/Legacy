
/************************************************************************
*    FILE NAME:       smartbtncapturemenu.cpp
*
*    DESCRIPTION:     Class CSmartBtnCaptureLbl
************************************************************************/

// Physical component dependency
#include "smartbtncapturemenu.h"

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <gui/uimenutree.h>
#include <gui/uimenu.h>
#include <gui/uibuttonlist.h>
#include <controller/gamecontroller.h>
#include <utilities/genfunc.h>
#include <utilities/exceptionhandling.h>

// game dependencies
#include "smartkeybindscrollbox.h"

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
    // Get the ID of the last device used
    NDevice::EDeviceId deviceId =  CGameController::Instance().GetLastDevicedUsed();

    // Get the button ID of the last device
    int buttonID = CGameController::Instance().ScanDeviceForButtonPress( deviceId );

    // The Esc key is the only key allowd to cancel out of the key bindings
    // menu so trap it here and reset because this key can never be mapped.
    if( (deviceId == NDevice::KEYBOARD) && (buttonID == NDevice::KEY_ESCAPE) )
        buttonID = -1;

    // Wait for all buttons to be released before we're ready to bind a key
    if( lastButtonID == -1 && buttonID == -1 )
        keyBindReady = true;

    // Are we ready to bind a key?
    if( (lastButtonID > -1) && (buttonID == -1) && keyBindReady )
    {
        CUIControl * pScrollBoxCtrl = CUIMenuTree::Instance().GetPtrToMenu("key_bindings_menu")->GetPtrToControl("key_binding_scroll_box");

        if( pScrollBoxCtrl == NULL )
            throw NExcept::CCriticalException("Scroll Box Controll Error!",
                boost::str( boost::format("Control is NULL.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

        CUIControl * pActiveCtrl = pScrollBoxCtrl->GetActiveControl();

        if( (pActiveCtrl != NULL) &&
            CGameController::Instance().SetKeyBinding( deviceId, pActiveCtrl->GetFontString(), lastButtonID ) )
        {
            string componetIdStr = CGameController::Instance().GetDeviceComponetIdStr( deviceId, lastButtonID );

            if( deviceId == NDevice::JOYPAD )
                pActiveCtrl->GetSubControl(deviceId)->DisplayFontString( CGameController::Instance().GetJoypadDisplayStr(componetIdStr) );
            else
                pActiveCtrl->GetSubControl(deviceId)->DisplayFontString( componetIdStr );

            CGameController::Instance().SaveToFile();

            // Flag any conflicts
            NGenFunc::DynCast<CSmartKeyBindScrollBox *>(pScrollBoxCtrl->GetSmartGuiPtr())->FlagConflictedDeviceMappings();
        }

        // Transition out of the menu
        CUIMenuTree::Instance().SimulateEscapeActionOutOfMenu();
    }

    lastButtonID = buttonID;

}	// HandleUserInput






