/************************************************************************
*    FILE NAME:   configdialog.h
*
*    DESCRIPTION: For creating and managing a config dialog window singlton 
************************************************************************/

#ifndef __config_dialog_h__
#define __config_dialog_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include "misc\\defs.h"

class CConfigDialogWnd
{
public:

	// Create the parent Window
	bool CreateDialogWnd( HWND hwnd,
						  HINSTANCE hInstance,
						  std::string & resource,
						  winCallbackFuncPtrType pWinCallback );

	// Initilization function
    bool OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );

	// Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );

    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );

    // Standard Windows OnPaint message handler
    void OnPaint( HWND hwnd );

    // I'm using this to post a message to show the helpful hint. I do this so that the
    // config dialog box shows up and then the helpful hint box. Otherwise, the helpful
    // hint box will show before the dialog box will.
    void OnShowWindow( HWND hwnd, BOOL fShow, UINT status );

    // On HScroll message handler for the config dialog box
    void OnHScroll( HWND hwnd, HWND hwndCtl, UINT code, int pos );

    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };

protected:

	// Constructor
    CConfigDialogWnd();

	// Get the handle to the saver window
	HWND GetWndHandle()
	{ return hWnd; }

	// Get the screen saver's instance
	HINSTANCE GetAppInstance()
	{ return hInst; }

private:

	// handle to this window
    HWND hWnd;

	// handle to this apps instance
	HINSTANCE hInst;

};


#endif // __config_dialog_h__
