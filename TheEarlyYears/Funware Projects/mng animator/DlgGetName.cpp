/************************************************************************
*
*    PRODUCT:         Get Name Dialog box
*
*    FILE NAME:       DlgGetName.cpp
*
*    DESCRIPTION:     Dialog box for getting a name
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "DlgGetName.h"      // Header for this *.cpp file
#include "GDefines.h"        // Header file with #defines
#include "gTools.h"          // A misc group of useful functions

// Globals
PCGetNameDlg pGetNameDlg;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:        Display_GetNameDlg                                                             
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

BOOL Display_GetNameDlg( HINSTANCE hInst, HWND hwnd, char *fileName, char *filePath, PTStringList pStringList )
{
    BOOL Result = FALSE;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pGetNameDlg = new CGetNameDlg();

    // Create the dialog box
    if( pGetNameDlg )
        Result = pGetNameDlg->CreateDialogWnd( hInst, hwnd, fileName, filePath, pStringList );

    return Result;

}   // Display_GetNameDlg


/************************************************************************
*    FUNCTION NAME:         GetNameDlg_Proc
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

LRESULT CALLBACK GetNameDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pGetNameDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND,    pGetNameDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_DESTROY,    pGetNameDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pGetNameDlg->OnNCDestroy );
        default:
            return FALSE;
    }
}   // GetNameDlg_Proc


/************************************************************************
*    FUNCTION NAME:         SubClassEdtProc                                                             
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

LRESULT CALLBACK SubClassEdtProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    return pGetNameDlg->OnEdit_GetName( hCtrl, Message, wParam, lParam );

}   // SubClassEdtProc


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

CGetNameDlg::CGetNameDlg()
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

CGetNameDlg::~CGetNameDlg()
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

BOOL CGetNameDlg::CreateDialogWnd( HINSTANCE hInstance, HWND hwnd, char *fName, char *path, PTStringList pStrList )
{
    // Init class variables
    hInst = hInstance;
    hParentWnd = hwnd;
    fileName = fName;
    filePath = path;
	pList = pStrList;

    return DialogBox( hInst, "DLG_GET_NAME", hParentWnd, (DLGPROC)GetNameDlg_Proc );

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

BOOL CGetNameDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    // Get the handle to this dialog box
    hDlgWnd = hwnd;

    // Subclass the edit control
    // Save the control handle for later use
    hSubEdtCtrl = GetDlgItem( hwnd, EDT_GET_SCRIPT );

    // Subclass the windows control
    OldSubEdtCtrlProc = (WNDPROC)SetWindowLong( hSubEdtCtrl, GWL_WNDPROC, 
                                                LONG( SubClassEdtProc ) );

    // Limit the number of characters they can enter into the edit field
    Edit_LimitText( hSubEdtCtrl, MAX_SCRIP_NAME_SIZE );

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

void CGetNameDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case BTN_CANCEL:
            // The TRUE is the return parameter. This can be an int.
            EndDialog( hwnd, FALSE );        
        break;

        case BTN_OK:

            // Make sure the file name works
            if( CheckFileName( hwnd ) )
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

void CGetNameDlg::OnDestroy( HWND hwnd ) 
{
    // Give all the subclassed controls their procedure back 
    // before destroying the dialog box
    if( OldSubEdtCtrlProc )
        SetWindowLong( hSubEdtCtrl, GWL_WNDPROC, LONG( OldSubEdtCtrlProc ) );

} // OnDestroy


/************************************************************************
*    FUNCTION NAME:         CheckFileName                                                             
*
*    DESCRIPTION:           Check the entered filename
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

BOOL CGetNameDlg::CheckFileName( HWND hwnd ) 
{
    BOOL Result = FALSE;
    int i;
    char tmpFileName[ MAX_SCRIP_NAME_SIZE + 1 ], tmpListName[ MAX_SCRIP_NAME_SIZE + 1 ];
	char path[MAX_PATH*2];

    // Check that they entered some text
    if( Edit_GetTextLength( hSubEdtCtrl ) )
    {
        // Get the text
        Edit_GetText( hSubEdtCtrl, tmpFileName, MAX_SCRIP_NAME_SIZE );

		// Build the path to the script	file
		wsprintf( path, "%s\\%s.sas", filePath, tmpFileName );
        
        // Check that the name is not already being used
        for( i = 0; i < pList->Count(); ++i )
        {
            // Make sure we didn't type in the same name
            if( strcmpi( path, pList->GetString( i ) ) == 0 )
            {
                PostMsg( "Ummm Du.", "The script name you typed is already in use for that folder (%s). You'll need to try again.", path );
                goto HANDLE_ERROR;
            }
        }
        
        // All is OK so save the script name
        strcpy( fileName, tmpFileName );       
    }
    else
        goto HANDLE_ERROR;

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

} // OnDestroy


/************************************************************************
*    FUNCTION NAME:         OnEdit_GetName                                                             
*
*    DESCRIPTION:           Message handler for the Create animation edit control    
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message 
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

LRESULT CGetNameDlg::OnEdit_GetName( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;

    // Keep illigal characters out of the edit field
    if( Message == WM_CHAR )
    {
        if( (wParam >= ' ' &&  wParam <= ',') || (wParam >= ':' &&  wParam <= '@') ||
            (wParam >= '[' &&  wParam <= '^') || (wParam >= '{' &&  wParam <= '~') ||
            wParam == '.' || wParam == '/' || wParam == '`' )
            return FALSE;
    }

    if( Message == WM_KEYUP && wParam == RETURN_KEY )
    {
        // Pretend we clicked on the OK button
        OnCommand( hDlgWnd, BTN_OK, NULL, 0 );
    }
    else
        Result = CallWindowProc( OldSubEdtCtrlProc, hCtrl, Message, wParam, lParam );

HANDLE_ERROR:;

    return Result;

}   // OnEdit_GetName