
/************************************************************************
*
*    PRODUCT:         Status Bas window
*
*    FILE NAME:       status.cpp
*
*    DESCRIPTION:     Status Bar Dialog
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "statusdefs.h"
#include "Status.h"
#include "minTools.h"        // A misc group of useful functions
#include <commctrl.h>        // Header file for Windows 95 controls
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf


// Globals
PTStatusDlg pStatusDlg = FALSE;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/

/************************************************************************
*    FUNCTION NAME:        DisplayAboutDlg                                                             
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

HWND DisplayStatusWnd( HINSTANCE Instance, HWND hwnd, BOOL ACenterToParent )
{
    HWND Result = NULL;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pStatusDlg = new TStatusDlg();

    // Create the dialog box
    if( pStatusDlg )
        Result = pStatusDlg->CreateDialogWnd( Instance, hwnd, ACenterToParent );

    return Result;

}   // DisplayAboutDlg


/************************************************************************
*    FUNCTION NAME:         StatusWnd_Proc                                                             
*
*    DESCRIPTION:           Standard Windows message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message
*              UINT Message   - Message being sent 
*              WPARAM wParam  - Additional information included with message
*              LPARAM lParam  - Additional information included with message
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

LRESULT CALLBACK StatusWnd_Proc( HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam )
{
    switch( Message )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pStatusDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_DESTROY,    pStatusDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pStatusDlg->OnNCDestroy );
        default:
            return FALSE;
    }
}   // StatusWnd_Proc



/************************************************************************
*    FUNCTION NAME:         StatusWnd_SetStatusTxt                                                             
*
*    DESCRIPTION:           Update the status text
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void StatusWnd_SetStatusTxt( char *T, ... )
{
    if( pStatusDlg, pStatusDlg->GetHandle() )
    {
        char StrArg[256];
        va_list argptr;

        va_start( argptr, T );
        vprintf( T, argptr );
        wvsprintf( StrArg, T, argptr );

        Static_SetText( GetDlgItem( pStatusDlg->GetHandle(), STC_PROGBAR_TXT ), StrArg );

        va_end( argptr );
    }

}   // StatusWnd_SetStatusTxt



/************************************************************************
*    FUNCTION NAME:         StatusWnd_SetStatusBar                                                             
*
*    DESCRIPTION:           Set the progress bar
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void StatusWnd_SetStatusBar( int StartPos, int MaxPos, int Inc )
{
    if( pStatusDlg )
        pStatusDlg->SetProgressBar( StartPos, MaxPos, Inc );

}   // StatusWnd_SetStatusBar



/************************************************************************
*    FUNCTION NAME:         StatusWnd_MoveStatusBar                                                             
*
*    DESCRIPTION:           Update the position of the progress bar
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void StatusWnd_MoveStatusBar()
{
    if( pStatusDlg )
        pStatusDlg->MoveProgressBar();

}   // StatusWnd_MoveStatusBar



/************************************************************************
*    FUNCTION NAME:         StatusWnd_BarDance                                                             
*
*    DESCRIPTION:           Just males the bar move
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void StatusWnd_BarDance()
{
    if( pStatusDlg )
        pStatusDlg->MakeBarDance();

}   // StatusWnd_BarDance



/************************************************************************
*    FUNCTION NAME:         StatusWnd_HideStatusWnd                                                             
*
*    DESCRIPTION:           Hides the status window
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void StatusWnd_HideStatusWnd()
{
    if( pStatusDlg )
        DestroyWindow( pStatusDlg->GetHandle() );

}   // StatusWnd_HideStatusWnd



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

TStatusDlg::TStatusDlg()
{
    IncProgBar = 0;
    hStatusWnd = NULL;

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

TStatusDlg::~TStatusDlg()
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

HWND TStatusDlg::CreateDialogWnd( HINSTANCE Instance, HWND hwnd, BOOL ACenterToParent )
{
    hParentWnd = hwnd;
    CenterToParent = ACenterToParent;

    hStatusWnd = CreateDialog( Instance, "DLG_STATUS_WND", hwnd, (DLGPROC)StatusWnd_Proc );

    return hStatusWnd;

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

BOOL TStatusDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    RECT rect;

    // Center the Helpful Hint window to the parent window
    if( CenterToParent )
    {
        GetWindowRect( hwnd, &rect );
        CenterWindow( hwnd, rect.right - rect.left, rect.bottom - rect.top, hParentWnd );
    }
    
    return TRUE;

}   // OnInitDialog


/************************************************************************
*    FUNCTION NAME:         StatusWnd_OnDestroy                                                             
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

void TStatusDlg::OnDestroy( HWND hwnd )
{

}   // StatusWnd_OnDestroy


/************************************************************************
*    FUNCTION NAME:         MoveProgressBar()                                                             
*
*    DESCRIPTION:           Move the progress bar
*
*    FUNCTION PARAMETERS: 
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

void TStatusDlg::MoveProgressBar()
{
    if( hStatusWnd )
        SendMessage( GetDlgItem( hStatusWnd, PROG_BAR ), PBM_STEPIT, 0, 0 );

}   // MoveProgressBar


/************************************************************************
*    FUNCTION NAME:         SetProgressBar()                                                             
*
*    DESCRIPTION:           Set the progress bar
*
*    FUNCTION PARAMETERS: 
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

void TStatusDlg::SetProgressBar( int StartPos, int MaxPos, int Inc )
{
    if( hStatusWnd )
    {
        SendMessage( GetDlgItem( hStatusWnd, PROG_BAR ),
                     PBM_SETRANGE, 0, MAKELONG( StartPos, MaxPos ) );

        SendMessage( GetDlgItem( hStatusWnd, PROG_BAR ),
                     PBM_SETSTEP, WPARAM( Inc ), 0 );
    }

}   // SetProgressBar


/************************************************************************
*    FUNCTION NAME:         StatusWnd_BarDance                                                             
*
*    DESCRIPTION:           Just males the bar move
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void TStatusDlg::MakeBarDance()
{
    if( IncProgBar > 10 )
        IncProgBar = 0;

    if( !IncProgBar )
        SetProgressBar( 0, 100, 10 );

    MoveProgressBar();

    IncProgBar++;

}   // StatusWnd_BarDance
