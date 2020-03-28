/************************************************************************
*
*    PRODUCT:         Ordering Options Dialog
*
*    FILE NAME:       OrderOpt.cpp
*
*    DESCRIPTION:     Simple dialog that asks if they want to order online 
*                     or by standard U.S. mail
*
*    IMPLEMENTATION:  Template Dialog
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include "GDefines.h"        // Header file with #defines, enums and structures
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size
#include "minTools.h"        // A misc group of useful functions
#include "OrderOpt.h"        // Header for this *.cpp file


// Globals
PTOrderOptDlg pOrderOptDlg;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:        DisplayTemplateDlg                                                             
*
*    DESCRIPTION:           Gets called by the base screen saver to
*                           display the dialog box
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HWND hwnd - Handle to window you want centered
*              char *InfoWndCaption - Text entered for the window caption
*              char *InfoWndTitle   - Text entered for the window title  
*              char *InfoWndTxt - The body of text
*
*    Output:   returntype - none
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/28/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int DisplayOrderOptionDlg( HINSTANCE hInst, HWND hwnd, BOOL ACenterToParent )
{
    int Result = NO_ORDER_OPTION_SELECTED;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pOrderOptDlg = new TOrderOptDlg();

    // Create the dialog box
    if( pOrderOptDlg )
        Result = pOrderOptDlg->CreateDialogWnd( hInst, hwnd, ACenterToParent );

    return Result;

}   // DisplayOrderOptionDlg



/************************************************************************
*    FUNCTION NAME:         TemplateDlg_SetupDlgProc
*
*    DESCRIPTION:           Standard Windows dialog message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hDlg      - Handle of the window receiving the message
*              UINT uMsg      - Message being sent
*              WPARAM wParam  - Additional information included with message
*              LPARAM lParam  - Additional information included with message
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

LRESULT CALLBACK OrderOption_DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pOrderOptDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND,    pOrderOptDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_DESTROY,    pOrderOptDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pOrderOptDlg->OnNCDestroy );
        default:
            return FALSE;
    }
}   // OrderOptionDlg_SetupDlgProc



/************************************************************************
*
*                About Dialog Object Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TOrderOptDlg::TOrderOptDlg()
{
}   // Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TOrderOptDlg::~TOrderOptDlg()
{
}   // Destructer



/************************************************************************
*    FUNCTION NAME:         CreateDialogWnd()                                                             
*
*    DESCRIPTION:           Create the dialog Window
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message
*      LPCREATESTRUCT lpCreateStruct  - Structure used in window creation 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int TOrderOptDlg::CreateDialogWnd( HINSTANCE hInst, HWND hwnd, BOOL ACenterToParent )
{
    CenterToParent = ACenterToParent;
    hParentWnd = hwnd;

    return DialogBox( hInst, "DLG_CHOOSE_ORDER_METHOD", hwnd, (DLGPROC)OrderOption_DlgProc );

}   // CreateDialogWnd


/************************************************************************
*    FUNCTION NAME:         OnInitDialog()                                                             
*
*    DESCRIPTION:           Standard Windows OnInitDialog message handler.
*                           Load the file the user has typed for their
*                           info window.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message
*      LPCREATESTRUCT lpCreateStruct  - Structure used in window creation 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TOrderOptDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    RECT rect;

    // Center the window to the parent window
    if( CenterToParent )
    {
        GetWindowRect( hwnd, &rect );
        CenterWindow( hwnd, rect.right - rect.left, rect.bottom - rect.top, hParentWnd );
    }

    return TRUE;

}   // OnInitDialog



/************************************************************************
*    FUNCTION NAME:         OnCommand()                                                             
*
*    DESCRIPTION:           Standard Windows OnCommand message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              int id          - Menu ID of the of the control.
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT codeNotify - The action taking place
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TOrderOptDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case BTN_CANCEL:
            
            // The FALSE is the return parameter        
            EndDialog( hwnd, NO_ORDER_OPTION_SELECTED );
        break;

        case BTN_ORDER_ONLINE:

            // The TRUE is the return parameter
            EndDialog( hwnd, ONLINE_WEB_ORDER );        
        break;

        case BTN_ORDER_THRO_MAIL:

            // The TRUE is the return parameter
            EndDialog( hwnd, STANDARD_US_MAIL_ORDER );        
        break;
    }

}   // OnCommand


/************************************************************************
*    FUNCTION NAME:         OnDestroy                                                             
*
*    DESCRIPTION:           Standard Windows OnDestroy message handler.
*                           You could free memory and do other clean up
*                           but it is best to keep it all in the OnClose
*                           message handler
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TOrderOptDlg::OnDestroy( HWND hwnd ) 
{

} // OnDestroy