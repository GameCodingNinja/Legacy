/************************************************************************
*
*    PRODUCT:         Template Dialog
*
*    FILE NAME:       DlgTemplate.cpp
*
*    DESCRIPTION:     Template Dialog
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

#include MAIN_APP_TOOLS      // A misc group of useful functions
#include "RegNumDlg.h"       // Header for this *.cpp file

#include MAIN_APP_HEADER     // Header file for this project.



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

BOOL DisplayTemplateDlg( HINSTANCE hInst, HWND hwnd )
{

    return DialogBox( hInst, MAKEINTRESOURCE( DLG_REG_NUMBER ), hwnd, (DLGPROC)TemplateDlg_SetupDlgProc );

}   // DisplayTemplateDlg



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

LRESULT CALLBACK TemplateDlg_SetupDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, TemplateDlg_OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND, TemplateDlg_OnCommand );
        HANDLE_MSG( hDlg, WM_DESTROY, TemplateDlg_OnDestroy );
        default:
            return FALSE;
    }
}   // Template_SetupDlgProc



/************************************************************************
*    FUNCTION NAME:         TemplateDlg_OnInitDialog()                                                             
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

BOOL TemplateDlg_OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{

    return TRUE;

}   // TemplateDlg_OnInitDialog



/************************************************************************
*    FUNCTION NAME:         TemplateDlg_OnCommand()                                                             
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

void TemplateDlg_OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case BTN_CANCEL:
            
            // The FALSE is the return parameter        
            EndDialog( hwnd, FALSE );
        break;

        case BTN_OK:

            // The TRUE is the return parameter
            EndDialog( hwnd, TRUE );        
        break;
    }

}   // TemplateDlg_OnCommand


/************************************************************************
*    FUNCTION NAME:         TemplateDlg_OnDestroy                                                             
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

void TemplateDlg_OnDestroy( HWND hwnd ) 
{

} // OnDestroy