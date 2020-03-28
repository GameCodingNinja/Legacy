
/************************************************************************
*    FILE NAME:       smartexitbtn.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Game OS dependencies
                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

// Physical component dependency
#include "smartexitbtn.h"

// Game lib dependencies
#include <system/xwindow.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartExitBtn::CSmartExitBtn( CUIControl * _pUIControl )
              : CSmartGuiControl( _pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the control is executed - quits the game
****************************************************************************/
void CSmartExitBtn::Execute()
{
    PostMessage( CXWindow::Instance().GetWndHandle(), WM_CLOSE, 0, 0 );

}	// Execute







