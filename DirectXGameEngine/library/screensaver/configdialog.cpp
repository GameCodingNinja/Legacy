/************************************************************************
*    FILE NAME:   configdialog.cpp
*
*    DESCRIPTION: For creating and managing a config dialog window singlton 
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

// Physical component dependency
#include "configdialog.h"

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc: Constructer                                                             
************************************************************************/
CConfigDialogWnd::CConfigDialogWnd()
				: hWnd(NULL),
				  hInst(NULL)
{
}	// Constructer


/***************************************************************************
*   desc:  Create the config dialog Window
*
*   param:  hwnd - handle to parent window
*   param:  hInstance - Program instance
*   param:  resourcs - resource name of dialog
*   param:  pWinCallback - Windows call back function pointer
*
*   ret:  bool - true on success or false on fail
 ****************************************************************************/
bool CConfigDialogWnd::CreateDialogWnd( HWND hwnd,
										HINSTANCE hInstance,
										std::string & resource,
										winCallbackFuncPtrType pWinCallback )
{
	hInst = hInstance;

	return DialogBox( hInst, resource.c_str(), hwnd, (DLGPROC)pWinCallback );

}	// CreateDialogWnd


/***************************************************************************
*   desc:  Standard Windows OnInitDialog message handler
*
*   param:  hwnd - handle to this window
*   param:  hwndFocus - Handle of window receiving focus
*   param:  lParam - additional dialog window param
*
*   ret:  bool - true on success or false on fail
 ****************************************************************************/
bool CConfigDialogWnd::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
	hWnd = hwnd;

	return true;

}	// OnInitDialog