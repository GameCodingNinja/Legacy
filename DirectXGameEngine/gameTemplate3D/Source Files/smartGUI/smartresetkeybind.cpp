
/************************************************************************
*    FILE NAME:       smartresetkeybind.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Game OS dependencies
                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

// Physical component dependency
#include "smartresetkeybind.h"

// Game lib dependencies
#include <controller/gamecontroller.h>
#include <gui/uimenutree.h>
#include <gui/uimenu.h>
#include <gui/uiscrollbox.h>
#include <utilities/genfunc.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartResetKeyBindBtn::CSmartResetKeyBindBtn( CUIControl * _pUIControl )
                     : CSmartGuiControl( _pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the control is executed - quits the game
****************************************************************************/
void CSmartResetKeyBindBtn::Execute()
{
    // Reset the key bindings for all controls and save
    CGameController::Instance().ResetKeyBindingsToDefault();

    CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu("key_bindings_menu");
    CUIScrollBox * pScrollBoxCtrl = NGenFunc::DynCast<CUIScrollBox *>(pMenu->GetPtrToControl("key_binding_scroll_box"));

    for( uint i = 0; i < pScrollBoxCtrl->GetScrollCtrlCount(); ++i )
        pScrollBoxCtrl->GetScrollCtrl(i)->SmartCreate();

}	// Execute







