/************************************************************************
*
*    PRODUCT:         Configeration dialog box
*
*    FILE NAME:       ConfigDlg.cpp
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines                             
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "ConfigDlg.h"       // Header for this *.cpp file


// Globals
PCConfigDlg pConfigDlg = NULL;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:        Display_ConfigDlg                                                             
*
*    DESCRIPTION:           Create dialog box
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progam instance
*              HWND hwnd - Handle to the parent window
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

BOOL Display_ConfigDlg( HINSTANCE hInst, HWND hwnd )
{
    BOOL Result = FALSE;

    // Make sure we don't run more then once
    if( !pConfigDlg )
    {
        // Create the object
        // It's best to not create the dialog box in the objects constructor
        pConfigDlg = new CConfigDlg();

        // Create the dialog box
        if( pConfigDlg )
            Result = pConfigDlg->CreateDialogWnd( hInst, hwnd );
    }

    // Since we are already running, bring the window forward
    else
    {
        SetForegroundWindow( pConfigDlg->GetWnd() );
        SetActiveWindow( pConfigDlg->GetWnd() );
    }

    return Result;

}   // Display_ConfigDlg


/************************************************************************
*    FUNCTION NAME:         ConfigDlg_Proc
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

LRESULT CALLBACK ConfigDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pConfigDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND,    pConfigDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_DESTROY,    pConfigDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pConfigDlg->OnNCDestroy );
        default:
            return FALSE;
    }
}   // ConfigDlg_Proc


/************************************************************************
*
*                        Dialog Object Functions
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

CConfigDlg::CConfigDlg()
{
    // Init class members

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

CConfigDlg::~CConfigDlg()
{
    pConfigDlg = NULL;

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

BOOL CConfigDlg::CreateDialogWnd( HINSTANCE hInstance, HWND hwnd )
{
    // Init class variables
    hInst = hInstance;
    hParentWnd = hwnd;

    return DialogBox( hInst, "DLG_CONFIG", hParentWnd, (DLGPROC)ConfigDlg_Proc );

}   // CreateDialogWnd


/************************************************************************
*    FUNCTION NAME:         OnInitDialog()                                                             
*
*    DESCRIPTION:           Standard Windows OnInitDialog message handler.
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

BOOL CConfigDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    // Set the dialog windows caption icon to the app icon
    SetClassLong( hwnd, GCL_HICON, (LONG) LoadIcon( hInst, "AppIcon" ) );

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

void CConfigDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case DLG_QUIT_BOX:
        case BTN_CANCEL:
            // The TRUE is the return parameter. This can be an int.
            EndDialog( hwnd, TRUE );        
        break;
    }

}   // OnCommand


/************************************************************************
*    FUNCTION NAME:         OnDestroy                                                             
*
*    DESCRIPTION:           Standard Windows OnDestroy message handler.
*                           You could free memory and do other clean up.
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

void CConfigDlg::OnDestroy( HWND hwnd ) 
{

} // OnDestroy