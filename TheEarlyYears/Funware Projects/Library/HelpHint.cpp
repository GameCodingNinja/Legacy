/************************************************************************
*
*    PRODUCT:         Screen saver
*
*    FILE NAME:       SSHelpHint.h
*
*    DESCRIPTION:     Screen saver Helpful hint dialog box
*
*    IMPLEMENTATION:  Dialog box to inform users about screen saver specific
*                     information and settings. This information can also be
*                     checksumed to ensure the information has not been changed.
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
#include "HelpHint.h"        // Helpful hint window. The text is loaded from resource


// Globals
PTHelpHintDlg pHelpHintDlg;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         DisplayHelpHintDialogBox                                                             
*
*    DESCRIPTION:           Gets called by the base screen saver to
*                           display the dialog box
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HWND hwnd - Handle to window you want centered
*              char *ResName - Name of the resource holding the help text
*              char *Type    - The resource type  
*              char *HelpTitle - Title of the help window
*              BOOL *ShowHelp - Pointer that is set if this help is turned off
*              char *Buffer  - Load the text from a buffer if text is not in the resource
*              BOOL ACenterToParent - If this flag is TRUE, the helpful hint window
*                                    is centered to the parent window, otherwise it
*                                    is centered to the screen.
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

void DisplayHelpHintDialogBox( HINSTANCE hInst, HWND hwnd, char *ResName, 
                               char *Type, char *HelpWndCaption, char *HelpTitle, 
                               BOOL *ShowHelp, BOOL DoCheckSum, char *Buffer, DWORD BufSize,
                               BOOL ACenterToParent )
{
    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pHelpHintDlg = new THelpHintDlg( hInst, hwnd );

    // Create the dialog box
    if( pHelpHintDlg )
        pHelpHintDlg->CreateDialogWnd( ResName, Type, HelpWndCaption, HelpTitle, ShowHelp, 
                                       DoCheckSum, Buffer, BufSize, ACenterToParent );

}   // DisplayHelpHintDialogBox


/************************************************************************
*    FUNCTION NAME:         HelpHintDlgProc
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

LRESULT CALLBACK HelpHintDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pHelpHintDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND, pHelpHintDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_DESTROY, pHelpHintDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_NCDESTROY, pHelpHintDlg->OnNCDestroy );
        default:
            return FALSE;
    }

} // HelpHintDlgProc


/************************************************************************
*    FUNCTION NAME:         NewEditProc                                                             
*
*    DESCRIPTION:           Subclassing the Edit field to trap keyboard
*                           messages to keep keystrokes from changing the
*                           edit field because the readonly option doesn't
*                           look that good.
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

LRESULT CALLBACK HelpHintEditProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    // Kill the keyboard messages
    if( Message == WM_KEYDOWN || Message == WM_KEYUP || Message == WM_CHAR )
        return FALSE;
    else
        return CallWindowProc( pHelpHintDlg->GetEditProc(), hwnd, Message, wParam, lParam );

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

THelpHintDlg::THelpHintDlg( HINSTANCE hInstance, HWND hwnd )
{
    hParentWnd = hwnd;
    hInst = hInstance;
    TitleFont = NULL;
    BufferSize = 0;
    OldEditProc = NULL;

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

THelpHintDlg::~THelpHintDlg()
{

}   // Destructer



/************************************************************************
*    FUNCTION NAME:         CreateDialogWnd                                                             
*
*    DESCRIPTION:           Create the dialog box
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HWND hwnd - Handle to window you want centered
*              char *SoundDir - Returns the directory the sound file was selected
*              char *GetFileName - Returns the path to the sound file
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

BOOL THelpHintDlg::CreateDialogWnd( char *ResName, char *Type, char *HelpTitle, char *HelpWndCaption, 
                                    BOOL *ShowHelp, BOOL DoCheckSum, char *Buffer,
                                    DWORD BufSize, BOOL ACenterToParent )
{
    // init these variables
    pResName = ResName;
    pType = Type;
    pHelpTitle = HelpTitle;
    pHelpWndCaption = HelpWndCaption;    
    pShowHelp = ShowHelp;    
    CheckSum = DoCheckSum;
    pBuffer = Buffer;
    BufferSize = BufSize;
    CenterToParent = ACenterToParent;   

    return DialogBox( hInst, "DLG_HELPFUL_HINT", hParentWnd, (DLGPROC)HelpHintDlgProc );

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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL THelpHintDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    TLoadRes LoadRes;
    HDC hDC;
    RECT rect;

    OldEditProc = NULL;

    // Center the Helpful Hint window to the parent window
    if( CenterToParent )
    {
        GetWindowRect( hwnd, &rect );
        CenterWindow( hwnd, rect.right - rect.left, rect.bottom - rect.top, hParentWnd );
    }

    // Set the caption of the help window
    if( pHelpWndCaption )
        SetWindowText( hwnd, pHelpWndCaption );

    // Set the title of the help window
    if( pHelpTitle )
        Static_SetText( GetDlgItem( hwnd, TXT_HLP_TITLE ), pHelpTitle );

    // Make a special font for the title of the helpful hint window
    hDC = GetDC( GetDlgItem( hwnd, TXT_HLP_TITLE ) );
    TitleFont = MakeFont( hDC, "MS Sans Serif", 16, FONT_BOLD, RGB( 0, 0, 0 ) );
    ReleaseDC( GetDlgItem( hwnd, TXT_HLP_TITLE ), hDC );

    if( TitleFont )
        SetWindowFont( GetDlgItem( hwnd, TXT_HLP_TITLE ), TitleFont, FALSE );
    
    // Load the text from the resource
    if( pResName )
    {
        if( !LoadTheResource( hInst, pResName, pType, &LoadRes ) )
            return FALSE;
    }
    else if( pBuffer ) // Load the text from a buffer
    {
        // Just copy over the pointer. Nothing fancy needed here
        LoadRes.pData = pBuffer;
        LoadRes.Size = BufferSize;
    }

    // If this is touchy information, we may want to checksum it to be
    // sure it has not been changed by someone. If we fail the checksum then
    // just display a weird message because we don't want to clue in the guilty
    // party that we are onto their game
    if( CheckSum == DO_CHECK_SUM && LoadRes.pData && LoadRes.Size )
    {
        if( !CheckSumData( LoadRes.pData, LoadRes.Size ) )
        {
            // Display a meaning less error to confuse evil doers.
            PostMsg( "Error", "Error number 15432." );

            // Kill this dialog box
            EndDialog( hwnd, 0 );

            // Kill this dialog box
            return FALSE;
        }
    }

    // Load the text into the edit field
    if( LoadRes.pData )
    {
        Edit_SetText( GetDlgItem( hwnd, EDT_HELPFUL_HINT ), (char *)LoadRes.pData );

        // Subclass Edit box control
        OldEditProc = (WNDPROC)SetWindowLong( GetDlgItem( hwnd, EDT_HELPFUL_HINT ), 
                                              GWL_WNDPROC, LONG( HelpHintEditProc ) );
    }

    // If we have a NULL pointer, hide the check box
    if( pShowHelp == NULL )
        ShowWindow( GetDlgItem( hwnd, CHK_DONT_SHOW_AGAIN ), SW_HIDE );

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

void THelpHintDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case BTN_DONE:
        case DLG_QUIT_BOX:
            // Did they check the button to not show this dialog box
            if( Button_GetCheck( GetDlgItem( hwnd, CHK_DONT_SHOW_AGAIN ) ) == BST_CHECKED )
            {
                if( pShowHelp )
                    *pShowHelp = FALSE;
            }
            else
            {
                if( pShowHelp )
                    *pShowHelp = TRUE;
            }
                    
            EndDialog( hwnd, 0 );
        break;
    }

}   // OnCommand


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

void THelpHintDlg::OnDestroy( HWND hwnd ) 
{
    // Delete the font we created just for this window
    if( TitleFont )
    {
        DeleteFont( TitleFont );
        TitleFont = NULL;
    }

    // Give the edit field back its mesproc
    if( OldEditProc )
    {
        SetWindowLong( GetDlgItem( hwnd, EDT_HELPFUL_HINT ), GWL_WNDPROC, LONG( OldEditProc ) );
        OldEditProc = NULL;
    }

} // OnDestroy