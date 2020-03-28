/************************************************************************
*    FILE NAME:       DoNothingDlg.cpp
*
*    DESCRIPTION:     Template Dialog
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "DoNothingDlg.h"    // Header for this *.cpp file


// Globals
CDoNothingDlg *pDoNothingDlg;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    DESCRIPTION: Create dialog box
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progam instance
*              HWND hwnd - Handle to the parent window
*
*    Output:   bool - true on success or false on fail
************************************************************************/
bool Display_DoNothingDlg( HINSTANCE hInst, HWND hwnd )
{
    // Create the object
	CDoNothingDlg doNothingDlg;
    pDoNothingDlg = &doNothingDlg;

    // because we create a dialog box, we don't return until the window is closed
    return doNothingDlg.CreateDialogWnd( hInst, hwnd );

}   // Display_DoNothingDlg


/************************************************************************
*    DESCRIPTION: Standard Windows dialog message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hDlg      - Handle of the window receiving the message
*              UINT uMsg      - Message being sent
*              WPARAM wParam  - Additional information included with message
*              LPARAM lParam  - Additional information included with message
*
*    Output:   LRESULT -
************************************************************************/
LRESULT CALLBACK DoNothingDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pDoNothingDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND,    pDoNothingDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_DESTROY,    pDoNothingDlg->OnDestroy );
        default:
            return false;
    }
}   // DoNothingDlg_Proc


/************************************************************************
*                        Dialog Object Functions
************************************************************************/


/************************************************************************
*    FUNCTION NAME:  Constructer                                                             
************************************************************************/
CDoNothingDlg::CDoNothingDlg()
{
    // Init class members

}   // Constructer


/************************************************************************
*    FUNCTION NAME:  Destructer                                                             
************************************************************************/
CDoNothingDlg::~CDoNothingDlg()
{
}   // Destructer


/************************************************************************
*    DESCRIPTION: Create the dialog Window
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message
*      LPCREATESTRUCT lpCreateStruct  - Structure used in window creation 
*
*    Output:   bool - true on success or false on fail
************************************************************************/
bool CDoNothingDlg::CreateDialogWnd( HINSTANCE hInstance, HWND hwnd )
{
    // Init class variables
    hInst = hInstance;
    hParentWnd = hwnd;

    return (DialogBox( hInst, "DLG_DO_NOTHING", hParentWnd, (DLGPROC)DoNothingDlg_Proc ) > 0);

}   // CreateDialogWnd


/************************************************************************
*    DESCRIPTION: Standard Windows OnInitDialog message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message
*      LPCREATESTRUCT lpCreateStruct  - Structure used in window creation 
*
*    Output:   bool - true on success or false on fail
************************************************************************/
bool CDoNothingDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{

    return true;

}   // OnInitDialog


/************************************************************************
*    DESCRIPTION:  Standard Windows OnCommand message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              int id          - Menu ID of the of the control.
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT codeNotify - The action taking place
************************************************************************/
void CDoNothingDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case MNU_EXIT:
            // The true is the return parameter. This can be an int.
            EndDialog( hwnd, true );        
        break;
    }

}   // OnCommand


/************************************************************************
*    DESCRIPTION: Standard Windows OnDestroy message handler.
*                 You could free memory and do other clean up.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd  - Handle of the window receiving the message 
************************************************************************/
void CDoNothingDlg::OnDestroy( HWND hwnd ) 
{

} // OnDestroy