/************************************************************************
*
*    PRODUCT:         Screen saver
*
*    FILE NAME:       SSConfig.cpp
*
*    DESCRIPTION:     Screen saver config dialog box
*
*    IMPLEMENTATION:  Dialog box for users to configure the screen saver
*                     Every project will need to have it's own different
*                     dialog box. This *.cpp and *.h are project specific
*                     and a new one for each screen saver project is required.
*
*    Copyright(c) 2000 by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "gtools.h"          // A misc group of useful functions
#include "Settings.h"        // User settings for this screen saver
#include "dib.h"             // Dib and Bitmap class for displaying and animating bitmaps
#include "DecodGif.h"        // GIF decoder which can load GIFs from file or resource
#include "HelpHint.h"        // Helpful hint window. The text is loaded from resource or a buffer
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf

#include "SSConfig.h"        // Header file for this *.cpp file.

// Globles
PTConfigDlg pConfigDlg;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         DisplayConfigDialogBox                                                             
*
*    DESCRIPTION:           Gets called by the base screen saver to
*                           display the dialog box
*
*    FUNCTION PARAMETERS:
*    Input:     HINSTANCE hInst - This progams instance
*               HWND hwnd - Handle to window you want centered
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

BOOL DisplayConfigDialogBox( HINSTANCE hInst, HWND hwnd )
{
    BOOL Result = FALSE;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pConfigDlg = new TConfigDlg();

    // Create the dialog box
    if( pConfigDlg )
        Result = pConfigDlg->CreateDialogWnd( hInst, hwnd );

    return Result;

}   // DisplayConfigDialogBox

                
/************************************************************************
*    FUNCTION NAME:         ConfigDlgProc
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

LRESULT CALLBACK ConfigDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pConfigDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND,    pConfigDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_PAINT,      pConfigDlg->OnPaint );
        HANDLE_MSG( hDlg, WM_SHOWWINDOW, pConfigDlg->OnShowWindow );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pConfigDlg->OnNCDestroy );
        default:
            return FALSE;
    }
}   // ConfigDlgProc


/************************************************************************
*
*                     Object Related Functions
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

TConfigDlg::TConfigDlg()
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

TConfigDlg::~TConfigDlg()
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

int TConfigDlg::CreateDialogWnd( HINSTANCE hInstance, HWND hwnd )
{
    hInst = hInstance;

    return DialogBox( hInst, "DLG_CONFIG", hwnd, (DLGPROC)ConfigDlgProc );

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

BOOL TConfigDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    TGif gif;   // GIF decoder which can load GIFs from file or resource
    char winDirStr[MAX_PATH+1];
    char fileName[MAX_PATH+1];
    char saverListFile[MAX_PATH+1];
    TStringList tmpSaverList;

    // Save the parent window handle
    hParentWnd = hwnd;
    focusWnd = hwndFocus;

    // Get the handle to the list boxs
    hAvailSavers = GetDlgItem( hwnd, LST_AVAILABLE_SAVERS );
    hToRunSavers = GetDlgItem( hwnd, LST_SAVERS_TO_RUN );

    // Load all the ini file settings
    Settings.GetPrivateString();
    
    // Load the GIF graphic from resource
    dib.LoadFromBuffer( gif.LoadFromResource( "settings", "GIF", hInst ) );

    // Set the window title
    SetWindowText( hwnd, PRODUCT_VERSION );

    // Get the windows directory
    GetTheWindowsDirectory( winDirStr, false );

    // Try to load the screen saver list file
    sprintf( saverListFile, SAVER_LIST_FILE, winDirStr );

    // See if we can load a file
    if( tmpSaverList.LoadFromFile( saverListFile, OPEN_EXISTING ) )
    {
        // Add all the strings to the list box
        for( int i = 0; i < tmpSaverList.Count(); ++i )
        {
            // Check to see if the file is there before adding it to the disk
            if( FileExist( tmpSaverList.GetString( i ) ) )
            {
                CropFileNameFromPath( tmpSaverList.GetString( i ), fileName, false );
                ListBox_AddString( hToRunSavers, fileName );

                selSaverList.Add( tmpSaverList.GetString( i ) );
            }
        }   
    }

    // Get all the available screen savers on the computer  
    availSaverList.GetAllFileNamesInDir( winDirStr, "*.scr" );
    strcat( winDirStr, "\\SYSTEM" );
    availSaverList.GetAllFileNamesInDir( winDirStr, "*.scr" );

    // Sort by the file name and not the whole path
    availSaverList.SortByFileName();

    // Find the name of our screen saver and remove it from the list
    for( int i = 0; i < availSaverList.Count(); ++i )
    {
        CropFileNameFromPath( availSaverList.GetString( i ), fileName, false );

        if( strcmp( fileName, INSTALLED_FILE_NAME ) == 0 )
        {
            availSaverList.Delete( i );
            break;
        }
    }
    
    // Add all the strings to the list box
    for( int i = 0; i < availSaverList.Count(); ++i )
    {
        CropFileNameFromPath( availSaverList.GetString( i ), fileName, false );
        ListBox_AddString( hAvailSavers, fileName );
    }

    // Load the combo box
    ComboBox_AddString( GetDlgItem( hwnd, CBO_LIST ), "After 1 Minute" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_LIST ), "After 2 Minutes" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_LIST ), "After 3 Minutes" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_LIST ), "After 5 Minutes" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_LIST ), "After 10 Minutes" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_LIST ), "After 15 Minutes" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_LIST ), "After 20 Minutes" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_LIST ), "After 25 Minutes" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_LIST ), "After 30 Minutes" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_LIST ), "After 45 Minutes" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_LIST ), "After 1 Hour" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_LIST ), "After 2 Hours" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_LIST ), "After 3 Hours" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_LIST ), "After 4 Hours" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_LIST ), "After 5 Hours" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_LIST ), "Never" );

    ComboBox_SetCurSel( GetDlgItem( hwnd, CBO_LIST ), Settings.GetTimeOut() );

    Button_SetCheck( GetDlgItem( hwnd, CHK_RANDOMISE ), Settings.GetRandomList() );

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

void TConfigDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch(id)
    {
        case BTN_OK:
            // Go through all the controls and record their settings
            RecordUserSettings( hwnd );

            // Saver all the ini file settings
            Settings.WritePrivateString();

            // Free all the allocated objects
            FreeAllAlocations();

            // Kill the program
            EndDialog( hwnd, 0 );
        break;

        case BTN_CANCEL:
            // Save off only the helful hint settings
            Settings.WriteHelpHintSettings();

            // Free all the allocated objects
            FreeAllAlocations();

            // Kill the program
            EndDialog( hwnd, 0 );
        break;

        case SHOW_HELPHINT_WND:
            // Show the helpful hint window
            if( Settings.GetIntroHelpWnd() )
            {
                Sleep( 400 );
                BOOL ShowNextTime;
                DisplayHelpHintDialogBox( hInst, hwnd, "settings", "TXT", "Screen Saver Settings", "Helpful Hint", &ShowNextTime, DO_CHECK_SUM );
                Settings.SetIntroHelpWnd( ShowNextTime );
            }
            
        break;

        case BTN_ADD:
            btn_AddSaverToList( hwnd );
        break;

        case BTN_REMOVE:
            btn_RemoveSaverFromList( hwnd );
        break;

        case BTN_WEB_SITE:
            ShellExecute( NULL, "open", WEB_ADDRESS, NULL, NULL, SW_SHOWNORMAL );
        break;

        case BTN_PREVIEW_TOTAL:
        case BTN_PREVIEW_PLAY:
        case BTN_CFG:
            btn_Run( hwnd, id );
        break;
    }
}   // OnCommand


/************************************************************************
*    FUNCTION NAME:         btn_Run()                                                             
*
*    DESCRIPTION:           run the selected screen saver.    
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

void TConfigDlg::btn_Run( HWND hwnd, int runType )
{
    char pathToSaver[MAX_PATH+1];
    HWND tmpHandle;
    char paramStr[20] = "/S";
    TStringList *pStringList;

    if( runType == BTN_PREVIEW_TOTAL )
    {
        pStringList = &availSaverList;
        tmpHandle = hAvailSavers;
    }
    else if( runType == BTN_PREVIEW_PLAY )
    {
        pStringList = &selSaverList;
        tmpHandle = hToRunSavers;
    }
    else if( runType == BTN_CFG )
    {
        tmpHandle = hToRunSavers;
        pStringList = &selSaverList;
        strcpy( paramStr, "/C" );
    }

    // Run the first screen saver in the list
    for( int i = 0; i < pStringList->Count(); ++i )
    {
        // Take the first selection
        if( ListBox_GetSel( tmpHandle, i ) > 0 )
        {
            STARTUPINFO startInfo;
            PROCESS_INFORMATION processInfo;

            memset( &startInfo, 0, sizeof(startInfo) );
            memset( &processInfo, 0, sizeof(processInfo) );

            startInfo.cb = sizeof(startInfo);
            startInfo.dwFlags = STARTF_USESHOWWINDOW;
            startInfo.wShowWindow = SW_SHOWNORMAL;

            sprintf( pathToSaver, "%s %s", pStringList->GetString( i ), paramStr );
            CreateProcess( NULL, pathToSaver, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startInfo, &processInfo );

            break;
        }
    }

}   // btn_Run


/************************************************************************
*    FUNCTION NAME:         btn_AddSaverToList()                                                             
*
*    DESCRIPTION:           Add saver to active list.    
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

void TConfigDlg::btn_AddSaverToList( HWND hwnd )
{   
    char nameToAdd[MAX_PATH+1];
    bool duplicateFound;

    for( int i = 0; i < availSaverList.Count(); ++i )
    {
        // Is this item selected?
        if( ListBox_GetSel( hAvailSavers, i ) > 0 )
        {
            // Set the flag
            duplicateFound = false;

            // See if the string is already in the list
            for( int j = 0; j < selSaverList.Count(); ++j )
            {                
                // Compair it to the one we want to add
                if( strcmp( availSaverList.GetString( i ), selSaverList.GetString( j ) ) == 0 )
                {
                    duplicateFound = true;
                    break;
                }   
            }
            
            // If not, add it
            if( !duplicateFound )
            {
                // Make the short name of the screen saver
                CropFileNameFromPath( availSaverList.GetString( i ), nameToAdd, false );

                // Add it to the list
                ListBox_AddString( hToRunSavers, nameToAdd );

                // Add the full path to the list
                selSaverList.Add( availSaverList.GetString( i ) );
            }
        }
    }
}   // btn_AddSaverToList


/************************************************************************
*    FUNCTION NAME:         btn_RemoveSaverFromList()                                                             
*
*    DESCRIPTION:           Remove saver from the list.    
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

void TConfigDlg::btn_RemoveSaverFromList( HWND hwnd )
{
    // Get the number of items in the list box
    int count = selSaverList.Count();

    for( int i = 0; i < count; ++i )
    {
        // Is this item selected?
        if( ListBox_GetSel( hToRunSavers, i ) > 0 )
        {
            ListBox_DeleteString( hToRunSavers, i );
			selSaverList.Delete( i );
            --count;
            --i;
        }
    }

}   // btn_RemoveSaverFromList


/************************************************************************
*    FUNCTION NAME:         OnPaint()                                                             
*
*    DESCRIPTION:           Standard Windows OnPaint message handler.    
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

void TConfigDlg::OnPaint( HWND hwnd )
{
    PAINTSTRUCT paintStruct;
    HDC PaintDC;
    RECT r;

    // Get the rect of our parent window
    GetClientRect( hwnd, &r );

    // Allow PAINTSTRUCT structure to get inited by the BeginPaint API call.
    // The BeginPaint and EndPaint API calls can only be use in the WM_PAINT
    // message handler. You CAN NOT paint any other way. Even if you have
    // nothing to paint, WM_PAINT message handler has to have the BeginPaint 
    // and EndPaint API calls or bad things will happen,
    PaintDC = BeginPaint( hwnd, &paintStruct );

    // Blit the graphic to the screen
    // The class also takes care of the realizing the palette if in 256 colors
    dib.BlitToDC( PaintDC, abs((r.right - dib.width()) / 2), 15 );

    // Must be called
    EndPaint( hwnd, &paintStruct );

}   // OnPaint


/************************************************************************
*    FUNCTION NAME:         OnShowWindow                                                             
*
*    DESCRIPTION:           Standard Windows OnShowWindowe message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              BOOL fShow      - show/hide flag
*              UINT status     - status flag
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

void TConfigDlg::OnShowWindow( HWND hwnd, BOOL fShow, UINT status )
{
    // Post a message to show the Helpful hint window
    // This way it doesn't show up before the config window does
    PostMessage( hwnd, (UINT)WM_COMMAND, (WPARAM)MAKELONG( SHOW_HELPHINT_WND, 0 ), (LPARAM)0 );

}   // OnShowWindow


/************************************************************************
*    FUNCTION NAME:         RecordUserSettings                                                             
*
*    DESCRIPTION:           Go through all the controls and record the
*                           settings they hold
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

void TConfigDlg::RecordUserSettings( HWND hwnd )
{
    char winDirStr[MAX_PATH+1];
    char saverListFile[MAX_PATH+1];

    // Get the windows directory
    GetTheWindowsDirectory( winDirStr, false );

    // Try to load the screen saver list file
    sprintf( saverListFile, SAVER_LIST_FILE, winDirStr );

    // Do we have any to save?
    if( selSaverList.Count() > 0 )
    {
        selSaverList.SaveToFile( saverListFile, CREATE_ALWAYS );
    }
    else
    {
        DeleteFile( saverListFile );
    }

    Settings.SetTimeOut( ComboBox_GetCurSel( GetDlgItem( hwnd, CBO_LIST ) ) );
    Settings.SetRandomList( Button_GetCheck( GetDlgItem( hwnd, CHK_RANDOMISE ) ) );

}   // RecordUserSettings


/************************************************************************
*    FUNCTION NAME:         FreeAllAlocations                                                             
*
*    DESCRIPTION:           Free all the allocations
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - True or False depending on if successful
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TConfigDlg::FreeAllAlocations()
{

}   // FreeAllAlocations
