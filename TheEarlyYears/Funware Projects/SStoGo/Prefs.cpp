/************************************************************************
*
*    PRODUCT:         Art 4 Sale
*
*    FILE NAME:       Preferences.cpp
*
*    DESCRIPTION:     Preferences dialog box
*
*    IMPLEMENTATION:  Dialog box for users to fill in their project info
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include "GDefines.h"        // Header file with #defines
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "Prefs.h"           // Header file for this *.cpp file.
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include "gTools.h"          // A misc group of useful functions

// Globals
PTPrefsDlg pPrefsDlg;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         DisplayPrefsDialogBox                                                             
*
*    DESCRIPTION:           Gets called by the base screen saver to
*                           display the dialog box
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HWND hwnd - Handle to parent window
*
*    Output:   returntype - none
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL DisplayPrefsDialogBox( HINSTANCE hInst, HWND hwnd, PTProjectPrefs ProjPrefs, PTActiveProjectData ActProj )
{
    BOOL Result = FALSE;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pPrefsDlg = new TPrefsDlg();

    // Create the dialog box
    if( pPrefsDlg )
        Result = pPrefsDlg->CreateDialogWnd( hInst, hwnd, ProjPrefs, ActProj );

    return Result;
}


/************************************************************************
*    FUNCTION NAME:         PrefsDlgProc
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
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

LRESULT CALLBACK PrefsDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pPrefsDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND,    pPrefsDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_DESTROY,    pPrefsDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pPrefsDlg->OnNCDestroy );
        default:
            return FALSE;
    }
}   // PrefsDlgDlgProc


/************************************************************************
*    FUNCTION NAME:         SubClassProc                                                             
*
*    DESCRIPTION:           Subclassing windows controls
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

LRESULT CALLBACK SubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    return pPrefsDlg->HandleSubClassProc( hCtrl, Message, wParam, lParam );

}   // NewEditProc


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

TPrefsDlg::TPrefsDlg()
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

TPrefsDlg::~TPrefsDlg()
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

BOOL TPrefsDlg::CreateDialogWnd( HINSTANCE hInst, HWND hwnd, PTProjectPrefs ProjPrefs, PTActiveProjectData ActProj )
{
    // Init class variables
    pProjPrefsData = ProjPrefs;
    pActProj = ActProj;
    hParentWnd = hwnd;

    // Create the dialog box
    return DialogBox( hInst, "DLG_PROJECT_PREFS", hwnd, (DLGPROC)PrefsDlgProc );

}   // CreateDialogWnd


/************************************************************************
*    FUNCTION NAME:         OnInitDialog()                                                             
*
*    DESCRIPTION:           Standard Windows OnInitDialog message handler.
*                           Here we are creating a window to paint our frames
*                           on to and a double buffer to compose our frames.
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
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TPrefsDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    RECT rect;

    // Center the window to the parent window
    GetWindowRect( hwnd, &rect );
    CenterWindow( hwnd, rect.right - rect.left, rect.bottom - rect.top, hParentWnd );

    // Limit Window title
    Edit_LimitText( GetDlgItem( hwnd, EDT_APP_TITLE ), MAX_TITLE );
    // Limit Name of exe file
    Edit_LimitText( GetDlgItem( hwnd, EDT_EXE_NAME ), MAX_EXE_NAME );

    // Init the members of the preferences dialog box
    Edit_SetText( GetDlgItem( hwnd, EDT_APP_TITLE ), pProjPrefsData->Title );
    Edit_SetText( GetDlgItem( hwnd, EDT_EXE_NAME ), pProjPrefsData->ExeName );

    // Subclass the two edit controls
    hSubCtrl[ S_SAVER_NAME ] = GetDlgItem( hwnd, EDT_APP_TITLE );
    hSubCtrl[ INSTALLER_NAME ] = GetDlgItem( hwnd, EDT_EXE_NAME );

    OldSubCtrlProc[ S_SAVER_NAME ] = (WNDPROC)SetWindowLong( hSubCtrl[ S_SAVER_NAME ], 
                                              GWL_WNDPROC, LONG( SubClassProc ) );
    OldSubCtrlProc[ INSTALLER_NAME ] = (WNDPROC)SetWindowLong( hSubCtrl[ INSTALLER_NAME ], 
                                                GWL_WNDPROC, LONG( SubClassProc ) );

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
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPrefsDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch(id)
    {
        case BTN_OK:
            // Save the select preferences
            SaveProjectPrefs( hwnd );

            // Set the flag to say that this is an active project
            pActProj->LiveProject = TRUE;

            // Kill this dialog
            EndDialog( hwnd, TRUE );  
        break;

        case BTN_CANCEL:
        case DLG_QUIT_BOX:
            // Kill this dialog
            EndDialog( hwnd, FALSE );
        break;
    }
}   // PrefsDlg_OnCommand


/************************************************************************
*    FUNCTION NAME:         SaveProjectPrefs                                                             
*
*    DESCRIPTION:           Save information recorded in the prefs dialog box
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd     - Handle of the window receiving the message
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    4/02/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPrefsDlg::SaveProjectPrefs( HWND hwnd )
{
    char TempStr[50];
    int Length, i;

    // Window Title
    Edit_GetText( GetDlgItem( hwnd, EDT_APP_TITLE ), pProjPrefsData->Title, sizeof(char)*(MAX_TITLE+1) );

    // Name of exe file
    Edit_GetText( GetDlgItem( hwnd, EDT_EXE_NAME ), pProjPrefsData->ExeName, sizeof(char)*(MAX_EXE_NAME+1) );

    // See if they included a proper *.scr extension
    // Get the file extension
    CropExtensionFromPath( pProjPrefsData->ExeName, TempStr );

    // Upper case the file extension for compairison
    strupr( TempStr );

    // Do we have a proper extension?
    if( strcmp( TempStr, "EXE" ) != 0 )
    {
        // Get the length of the string
        Length = strlen( pProjPrefsData->ExeName );

        // See if we find the "."
        for( i = 0; i < Length; ++i )
        {
            // Terminate at the '.' if there is one
            // They could have entered more then one
            if( pProjPrefsData->ExeName[i] == '.' )
            {
                if( i > 0 )
                    pProjPrefsData->ExeName[i] = 0;
                else
                    // Just incase the first entry is a "." then the whole thing is wrong
                    strcpy( pProjPrefsData->ExeName, PRO_EXAMPLE_EXE_NAME );

                break;
            }   
        }

        // add in the approate extension
        strcat( pProjPrefsData->ExeName, ".exe" );
    }

}   // SaveProjectPrefs
             

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

void TPrefsDlg::OnDestroy( HWND hwnd ) 
{
    // Give all the subclassed controls their procedure back 
    // before destroying the dialog box
    if( OldSubCtrlProc[ S_SAVER_NAME ] )
        SetWindowLong( hSubCtrl[ S_SAVER_NAME ], GWL_WNDPROC, LONG( OldSubCtrlProc[ S_SAVER_NAME ] ) );

    if( OldSubCtrlProc[ INSTALLER_NAME ] )
        SetWindowLong( hSubCtrl[ INSTALLER_NAME ], GWL_WNDPROC, LONG( OldSubCtrlProc[ INSTALLER_NAME ] ) );

} // OnDestroy


/************************************************************************
*    FUNCTION NAME:         HandleSubClassProc                                                             
*
*    DESCRIPTION:           Handles the subclassing of controls. Check the
*                           window handle to see which control is being trapped.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message 
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

LRESULT TPrefsDlg::HandleSubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    // Keep illigal characters out of the edit field
    if( Message == WM_CHAR )
    {
        if( (wParam >= '!' && wParam <= ',') || (wParam >= ':' && wParam <= '@') ||
            (wParam >= '[' && wParam <= '^') || (wParam >= '{' && wParam <= '}') ||
            (wParam == '/') || (wParam == '`') || (hSubCtrl[ S_SAVER_NAME ] == hCtrl && wParam == '.') )
            return FALSE;
    }

    // Find the window handle that is recieving the maessage
    if( hSubCtrl[ S_SAVER_NAME ] == hCtrl )
        return CallWindowProc( OldSubCtrlProc[ S_SAVER_NAME ], hCtrl, Message, wParam, lParam );

    else
        return CallWindowProc( OldSubCtrlProc[ INSTALLER_NAME ], hCtrl, Message, wParam, lParam );

}   // HandleSubClassProc