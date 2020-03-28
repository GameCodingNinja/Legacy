/************************************************************************
*
*    PRODUCT:         Art4Sale
*
*    FILE NAME:       InfoWndSetup.cpp
*
*    DESCRIPTION:     Info window setup dialog
*
*    IMPLEMENTATION:  Dialog box that helps the user setup their info windows
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
#include "InfoWnd.h"         // Header for this *.cpp file

// Globals
PTInfoSetupDlg pInfoSetupDlg;


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

BOOL DisplayInfoTxtSetupDlg( HINSTANCE hInst, HWND hwnd, char *DlgCaption, 
                               char *InfoWndCaption, char *InfoWndTitle, char *InfoWndTxtFilePath )
{
    BOOL Result = FALSE;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pInfoSetupDlg = new TInfoSetupDlg();

    // Create the dialog box
    if( pInfoSetupDlg )
        Result = pInfoSetupDlg->CreateDialogWnd( hInst, hwnd, DlgCaption, InfoWndCaption, 
                                                 InfoWndTitle, InfoWndTxtFilePath );
    return Result;

}   // DisplayInfoTxtSetupDialogBox



/************************************************************************
*    FUNCTION NAME:         InfoTxtDlg_Proc
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

LRESULT CALLBACK InfoTxtDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pInfoSetupDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND,    pInfoSetupDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pInfoSetupDlg->OnNCDestroy );
        default:
            return FALSE;
    }
}   // InfoTxtDlg_Proc


/************************************************************************
*
*                  Dialog Object Related Functions
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

TInfoSetupDlg::TInfoSetupDlg()
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

TInfoSetupDlg::~TInfoSetupDlg()
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

BOOL TInfoSetupDlg::CreateDialogWnd( HINSTANCE hInst, HWND hwnd, char *DlgCaption,
                                 char *InfoWndCaption, char *InfoWndTitle, char *InfoWndTxtFilePath )
{
    // init these variables
    pInfoWndCaption = InfoWndCaption;
    pInfoWndTitle = InfoWndTitle;
    pDlgCaption = DlgCaption;
    pInfoWndTxtFilePath = InfoWndTxtFilePath;
    hParentWnd = hwnd;

    return DialogBox( hInst, "DLG_INFOTXT_SETUP", hwnd, (DLGPROC)InfoTxtDlg_Proc );

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

BOOL TInfoSetupDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long bytesReadWrite;
    HANDLE hFile;
    int FileSize;
    char *txtBuffer = NULL;
    BOOL FileReadWriteResult;
    RECT rect;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    // Center the window to the parent window
    GetWindowRect( hwnd, &rect );
    CenterWindow( hwnd, rect.right - rect.left, rect.bottom - rect.top, hParentWnd );

    // Set the text limits for the two edit fields
    Edit_LimitText( GetDlgItem( hwnd, EDT_INFOWND_CAPTION ), MAX_INFO );
    Edit_LimitText( GetDlgItem( hwnd, EDT_INFOWND_TITLE ), MAX_INFO );

    // Open the file on the hard drive
    // This only opens the file, it doesn't create one. Fails if a file doesn't exsist.
    hFile = CreateFile( pInfoWndTxtFilePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ,
                           &FileSecurity, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hFile != INVALID_HANDLE_VALUE )
    {
        // get the size of the file
        FileSize = GetFileSize( hFile, NULL );

        // Allocate memory and hold text in a buffer
        if( FileSize > 0 )
        {
            txtBuffer = new char[FileSize];

            // Check all allocations
            if( !txtBuffer )
            {
                PostMsg( "Error", "Unable to allocate memory for text buffer (%s).", pDlgCaption );
                goto FREE_MEMORY;
            }

            // Read in the info window text
            FileReadWriteResult = ReadFile( hFile, txtBuffer, sizeof(char)*FileSize, &bytesReadWrite, NULL );
            if( !FileReadWriteResult || bytesReadWrite != sizeof(char)*FileSize )
            {
                PostMsg( "Error", "Error reading in file data (%s).", pDlgCaption );
                goto FREE_MEMORY;
            }

            // Set the text box with the users text
            if( bytesReadWrite )
                SetWindowText( GetDlgItem( hwnd, EDT_INFOWND_TEXT ), txtBuffer );
        }
    }

FREE_MEMORY:;

    // Set the caption of the help window
    SetWindowText( hwnd, pDlgCaption );

    // Set these text boxes with the users text they have typed in
    SetWindowText( GetDlgItem( hwnd, EDT_INFOWND_CAPTION ), pInfoWndCaption );
    SetWindowText( GetDlgItem( hwnd, EDT_INFOWND_TITLE ), pInfoWndTitle );

    // Close the file handle
    if( hFile != INVALID_HANDLE_VALUE )
        CloseHandle( hFile );

    if( txtBuffer )
        delete txtBuffer;

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

void TInfoSetupDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case BTN_SYSTEM_QUIT:
        case BTN_CANCEL:
            
            // The FALSE is the return parameter        
            EndDialog( hwnd, FALSE );
        break;

        case BTN_OK:

            // Save the text file to the hard drive
            // This will also delete the file if there is no text to save
            SaveTextFile( hwnd );

            // Save the text from the edit box to the string buffer for the caption and the title text strings
            Edit_GetText( GetDlgItem( hwnd, EDT_INFOWND_CAPTION ), pInfoWndCaption, sizeof(char)*(MAX_INFO+1) );
            Edit_GetText( GetDlgItem( hwnd, EDT_INFOWND_TITLE ), pInfoWndTitle, sizeof(char)*(MAX_INFO+1) );

            // The TRUE is the return parameter
            EndDialog( hwnd, TRUE );        
        break;
    }

}   // OnCommand



/************************************************************************
*    FUNCTION NAME:         SaveTextFile                                                             
*
*    DESCRIPTION:           Saves the text file to the hard drive
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

void TInfoSetupDlg::SaveTextFile( HWND hwnd )
{
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long bytesReadWrite;
    HANDLE hFile;
    int BufferSize;
    char *txtBuffer = NULL;
    BOOL FileReadWriteResult;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    // This always creates a new file by over writing the current file with a new one
    hFile = CreateFile( pInfoWndTxtFilePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_WRITE,
                           &FileSecurity, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hFile == INVALID_HANDLE_VALUE )
    {
        PostMsg( "Error", "Error opening file for saving file data to disk (%s).", pDlgCaption );
        goto FREE_MEMORY;
    }

    // Get the size of the text in the text box
    BufferSize = Edit_GetTextLength( GetDlgItem( hwnd, EDT_INFOWND_TEXT ) );

    // Allocate memory and hold text in a buffer
    if( BufferSize > 0 )
    {
        // Add 1 for the terminator
        txtBuffer = new char[BufferSize+1];

        // Check all allocations
        if( !txtBuffer )
        {
            PostMsg( "Error", "Unable to allocate memory for text buffer (%s).", pDlgCaption );
            goto FREE_MEMORY;
        }

        // Save the text from the edit window to the allocated buffer
        Edit_GetText( GetDlgItem( hwnd, EDT_INFOWND_TEXT ), txtBuffer, sizeof(char)*(BufferSize+1) );

        // Terminate the buffer
        txtBuffer[ BufferSize ] = 0;

        // Save the text file to disk
        FileReadWriteResult = WriteFile( hFile, txtBuffer, sizeof(char)*(BufferSize+1), &bytesReadWrite, NULL );
        if( !FileReadWriteResult || bytesReadWrite != sizeof(char)*(BufferSize+1) )
        {
            PostMsg( "Error", "Error saving file to disk (%s).", pDlgCaption );
            goto FREE_MEMORY;
        }
    }
    else
    {
        // Close the file so that it can be deleted
        CloseHandle( hFile );
        hFile = INVALID_HANDLE_VALUE;

        // delete the file if it is no longer in use
        DeleteFile( pInfoWndTxtFilePath );
    }

FREE_MEMORY:;

    // Close the file handle
    if( hFile != INVALID_HANDLE_VALUE )
        CloseHandle( hFile );

    // Delete the text buffer
    if( txtBuffer )
        delete txtBuffer;

}   // SaveTextFile