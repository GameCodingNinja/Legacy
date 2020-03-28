/************************************************************************
*
*    PRODUCT:         Art4Sale
*
*    FILE NAME:       CaptionWnd.h
*
*    DESCRIPTION:     Caption window setup dialog
*
*    IMPLEMENTATION:  Dialog box that helps the users add captions to the images
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

#include MAIN_APP_TOOLS      // A misc group of useful functions
#include "Caption.h"         // Header for this *.cpp file


// Globals
PTCaptionDlg pCaptionDlg;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:        DisplayInfoTxtSetupDialogBox                                                             
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

BOOL DisplayCaptionDlg( HINSTANCE hInst, HWND hwnd, char *pCaption )
{
    BOOL Result = FALSE;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pCaptionDlg = new TCaptionDlg();

    // Create the dialog box
    if( pCaptionDlg )
        Result = pCaptionDlg->CreateDialogWnd( hInst, hwnd, pCaption );

    return Result;

}   // DisplayInfoTxtSetupDialogBox



/************************************************************************
*    FUNCTION NAME:         CaptionDlg_Proc
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

LRESULT CALLBACK CaptionDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pCaptionDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND,    pCaptionDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pCaptionDlg->OnNCDestroy );
        default:
            return FALSE;
    }
}   // InfoTxtDlg_SetupDlgProc


/************************************************************************
*
*                     Dialog Object Related Functions
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

TCaptionDlg::TCaptionDlg()
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

TCaptionDlg::~TCaptionDlg()
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

BOOL TCaptionDlg::CreateDialogWnd( HINSTANCE hInst, HWND hwnd, char *pCaption )
{
    // init these variables
    pDlgCaption = pCaption;
    hParentWnd = hwnd;

    return DialogBox( hInst, "DLG_ADD_CAPTION", hwnd, (DLGPROC)CaptionDlg_Proc );

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

BOOL TCaptionDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    RECT rect;

    // Center the window to the parent window
    GetWindowRect( hwnd, &rect );
    CenterWindow( hwnd, rect.right - rect.left, rect.bottom - rect.top, hParentWnd );

    // Set the text limits for the caption edit field
    Edit_LimitText( GetDlgItem( hwnd, EDIT_CAPTION ), MAX_PAGE_CAPTION );

    // Display the text for this window
    SetWindowText( GetDlgItem( hwnd, EDIT_CAPTION ), pDlgCaption );

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

void TCaptionDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case BTN_SYSTEM_QUIT:
        case BTN_CANCEL:
            
            // The FALSE is the return parameter        
            EndDialog( hwnd, FALSE );
        break;

        case BTN_OK:

            // Save the text from the edit box to the string buffer for the caption and the title text strings
            Edit_GetText( GetDlgItem( hwnd, EDIT_CAPTION ), pDlgCaption, sizeof(char)*(MAX_PAGE_CAPTION+1) );

            // The TRUE is the return parameter
            EndDialog( hwnd, TRUE );        
        break;
    }

}   // OnCommand