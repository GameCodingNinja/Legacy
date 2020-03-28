/************************************************************************
*
*    PRODUCT:         PrintCancel Dialog
*
*    FILE NAME:       printcnl.cpp
*
*    DESCRIPTION:     PrintCancel Dialog
*
*    IMPLEMENTATION:  PrintCancel Dialog
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
#include "printcnl.h"        // Header for this *.cpp file
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf


// Globals
PTPrintCnlDlg pPrintCnlDlg;
HWND hgPrintCancelDld;
BOOL gPrinterContinuePrinting = TRUE;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:        DisplayPrintCancelDlg                                                             
*
*    DESCRIPTION:           Gets called by the base screen saver to
*                           display the dialog box
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd - Handle to window you want centered
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

HWND DisplayPrintCancelDlg( HINSTANCE hInst, HWND hwnd, char *StatusTxt, BOOL ACenterToParent )
{
    HWND Result = NULL;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pPrintCnlDlg = new TPrintCnlDlg();

    // Create the dialog box
    if( pPrintCnlDlg )
        Result = pPrintCnlDlg->CreateDialogWnd( hInst, hwnd, StatusTxt, ACenterToParent );

    // Save the handle in a global varaible
    hgPrintCancelDld = Result;

    return Result;

}   // DisplayPrintCancelDlg



/************************************************************************
*    FUNCTION NAME:         PrintCancelDlg_SetupDlgProc
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

LRESULT CALLBACK PrintCancelDlg_DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pPrintCnlDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND,    pPrintCnlDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_DESTROY,    pPrintCnlDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pPrintCnlDlg->OnNCDestroy );
        default:
            return FALSE;
    }
}   // PrintCancel_SetupDlgProc


/************************************************************************
*    FUNCTION NAME:         PrintCancelDlg_AbortFunc()                                                             
*
*    DESCRIPTION:           Printer Abort function.
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

BOOL CALLBACK PrintCancelDlg_AbortFunc( HDC hdc, int error )
{
    MSG msg;

    while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
        if( !IsDialogMessage( hgPrintCancelDld, &msg ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }

    return gPrinterContinuePrinting;

}   // PrintCancelDlg_AbortFunc


/************************************************************************
*    FUNCTION NAME:         PrintCancelDlg_UpdateStatus()                                                             
*
*    DESCRIPTION:           Update the printing status
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              char *StatusTxt - Status Text
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

void PrintCancelDlg_UpdateStatus( char *Text, ... )
{
    va_list argptr;
    char StrBuffer[256];

    va_start( argptr, Text );
    vprintf( Text, argptr );
    wvsprintf( StrBuffer, Text, argptr );

    // set the print message
    Static_SetText( GetDlgItem( hgPrintCancelDld, TXT_PRINTING ), StrBuffer );

}   // PrintCancelDlg_UpdateStatus


/************************************************************************
*    FUNCTION NAME:         PrintCancelDlg_GetPrintCancel()                                                             
*
*    DESCRIPTION:           Should the printing continue
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              char *StatusTxt - Status Text
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
 
BOOL PrintCancelDlg_GetPrintCancel()
{
    return gPrinterContinuePrinting;

}   // PrintCancelDlg_GetPrintCancel


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

TPrintCnlDlg::TPrintCnlDlg()
{
    gPrinterContinuePrinting = TRUE;

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

TPrintCnlDlg::~TPrintCnlDlg()
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

HWND TPrintCnlDlg::CreateDialogWnd( HINSTANCE hInst, HWND hwnd, char *StatusTxt, BOOL ACenterToParent )
{
    PrintStatus = StatusTxt;
    CenterToParent = ACenterToParent;
    hParentWnd = hwnd;
    gPrinterContinuePrinting = TRUE;

    hPrintCnlDlg = CreateDialog( hInst, "DLG_PRINTING", hwnd, (DLGPROC)PrintCancelDlg_DlgProc );

    return hPrintCnlDlg;

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

BOOL TPrintCnlDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    RECT rect;

    // Center the Helpful Hint window to the parent window
    if( CenterToParent )
    {
        GetWindowRect( hwnd, &rect );
        CenterWindow( hwnd, rect.right - rect.left, rect.bottom - rect.top, hParentWnd );
    }

    // set the print message
    Static_SetText( GetDlgItem( hwnd, TXT_PRINTING ), PrintStatus );
    
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

void TPrintCnlDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case BTN_CANCEL:
            gPrinterContinuePrinting = FALSE;
        break;
    }

}   // OnCommand


/************************************************************************
*    FUNCTION NAME:         PrintCancelDlg_OnDestroy                                                             
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

void TPrintCnlDlg::OnDestroy( HWND hwnd ) 
{

} // OnDestroy