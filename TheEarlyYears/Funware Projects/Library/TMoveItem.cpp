/************************************************************************
*
*    PRODUCT:         MoveItem Dialog
*
*    FILE NAME:       MoveItem.cpp
*
*    DESCRIPTION:     MoveItem Dialog
*
*    IMPLEMENTATION:  MoveItem Dialog
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include "GDefines.h"        // Header file with #defines
#include "TMoveItem.h"       // Header for this *.cpp file
#include MAIN_APP_TOOLS      // A misc group of useful functions

// Globals
PTMoveItemDlg pMoveItemDlg;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:        DisplayMoveItemDlg                                                             
*
*    DESCRIPTION:           Gets called by the base screen saver to
*                           display the dialog box
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HWND hwnd - Handle to window you want centered
*              int MoveItem - The Item that is being moved
*              int NumItems - Number of Items in this project
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

int DisplayMoveItemDlg( HINSTANCE hInst, HWND hwnd, HWND hCtrl, int MoveItem, int NumItems )
{
    int Result = MoveItem;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pMoveItemDlg = new TMoveItemDlg();

    // Create the dialog box
    if( pMoveItemDlg )
        Result = pMoveItemDlg->CreateDialogWnd( hInst, hwnd, hCtrl, MoveItem, NumItems );

    return Result;

}   // DisplayMoveItemDlg



/************************************************************************
*    FUNCTION NAME:         MoveItemDlg_SetupDlgProc
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

LRESULT CALLBACK MoveItemDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pMoveItemDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND,    pMoveItemDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_DESTROY,    pMoveItemDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pMoveItemDlg->OnNCDestroy );
        default:
            return FALSE;
    }
}   // MoveItem_SetupDlgProc


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

TMoveItemDlg::TMoveItemDlg()
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

TMoveItemDlg::~TMoveItemDlg()
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

int TMoveItemDlg::CreateDialogWnd( HINSTANCE hInst, HWND hwnd, HWND hCtrl, int MoveItem, int NumItems )
{
    ItemToBeMoved = MoveItem;
    TotalNumItems = NumItems;
    hParentWnd = hwnd;
	hListControl = hCtrl;

    return DialogBox( hInst, "DLG_MOVE_ITEM", hwnd, (DLGPROC)MoveItemDlg_Proc );

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

BOOL TMoveItemDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    int i;
    char StrTmp[100];
	RECT rect;

    // Center the window to the parent window
    GetWindowRect( hwnd, &rect );
    CenterWindow( hwnd, rect.right - rect.left, rect.bottom - rect.top, hParentWnd );

    // Fill the combo box with the Items that the selected Item can be moved
    for( i = 0; i < TotalNumItems; ++i )
    {
        if( i == ItemToBeMoved )
            strcpy( StrTmp, "Current Item To Be Moved" );
        else
		{
			if( hListControl )
				// Get the text from the list box control
				ListBox_GetText( hListControl, i, StrTmp );
			else
            	sprintf( StrTmp, "Move To Item No. %u", i+1 );
		}

        ComboBox_AddString( GetDlgItem( hwnd, CBO_MOVE_ITEM ), StrTmp );
    }

    // Have the combo box display the first item
    ComboBox_SetCurSel( GetDlgItem( hwnd, CBO_MOVE_ITEM ), ItemToBeMoved );

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

void TMoveItemDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    int ComboSelection;

    switch( id )
    {
        case BTN_CANCEL:
            
            // The FALSE is the return parameter        
            EndDialog( hwnd, -1 );
        break;

        case BTN_OK:

            ComboSelection = ComboBox_GetCurSel( GetDlgItem( hwnd, CBO_MOVE_ITEM ) );

            // The TRUE is the return parameter
            if( ComboSelection != ItemToBeMoved )
                EndDialog( hwnd, ComboSelection );
            else
                EndDialog( hwnd, -1 );        
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

void TMoveItemDlg::OnDestroy( HWND hwnd ) 
{

} // OnDestroy