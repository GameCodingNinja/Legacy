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

#include "SSConfig.h"        // Project file header with all the #defines and structures.
#include "SSBase.h"          // The command center for all screen savers
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf

#include "TStrList.h"        // Header file for the string list class.
#include "BldBuf.h"          // Functions for updating and refreshing the buffer
#include "PrintCnl.h"        // Print Cancel dialog box
#include "Loader.h"          // Code for loading the graphics from the back of the file


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
        HANDLE_MSG( hDlg, WM_DESTROY,    pConfigDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pConfigDlg->OnNCDestroy );
        default:
            return FALSE;
    }
}   // ConfigDlgProc


/************************************************************************
*    FUNCTION NAME:         NewEditProc                                                             
*
*    DESCRIPTION:           Subclassing the Edit field to trap keyboard
*                           messages to keep specific keystrokes from being
*                           keyed into the edit field. We only the user to
*                           be able to type numbers, delete keys and the
*                           period key.
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

LRESULT CALLBACK ViewWnd_StaticProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    switch( Message )
    {
        HANDLE_MSG( hwnd, WM_PAINT, pConfigDlg->ViewWnd_OnPaint );
        default:
            return CallWindowProc( pConfigDlg->GetViewWndOldProc(), hwnd, Message, wParam, lParam );
    }
}   // NewStaticProc


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
    BOOL Result = FALSE;
    RECT DisplayRect;
    char TmpString[MAX_PATH+1];
    HMENU CfgMenu;
    int i;

    // Set the dialog windows caption icon to the app icon
    SetClassLong( hwnd, GCL_HICON, (LONG) LoadIcon( hInst, "AppIcon" ) );

    // Create the menu
    CfgMenu = LoadMenu( hInst, "ConFigMenu" );
    SetMenu( hwnd, CfgMenu );

    // First this to do is memset the AppData
    memset( &AppData, 0, sizeof( TApplicationData ) );

    // zero out all the structures
    memset( &ActProj, 0, sizeof(TActiveProjectData) );
    memset( &ProjPrefsData, 0, sizeof(TProjectPrefs) );
    memset( &FileHeader, 0, sizeof(TFileHeader) );
    memset( &AppData.ViewWndRect, 0, sizeof(RECT) );

    //Create the path to the temp sound file
    GetTheWindowsTmpDir( TmpString );
    sprintf( AppData.SoundFileTmpPath, DEFAULT_MUSIC_FILE_NAME, TmpString, rand() %  0xFFFFFFF );

    // Get a handel to the view window
    AppData.hViewWnd = GetDlgItem( hwnd, STC_VIEW_WND );

    // Get the instance
    AppData.hInst = hInst;

    // Subclass static control
    OldStaticWnd = (WNDPROC)SetWindowLong( AppData.hViewWnd, GWL_WNDPROC, LONG( ViewWnd_StaticProc ) );

    // Get the command line and parse it to be sure
    // there is no junk with it
    ParsePathFromCmdLine( GetCommandLine(), AppData.Path );

    // We are in debug mode and are loading data from the default file name.
    // This causes the program to open the other file on the hard drive
    // instead of itself which allows us to step through it in the debugger
    #ifdef VIEW_DEBUG

    char FilePath[MAX_PATH+1];

    // We are asuming this file has bee generated by the tool
    CropOutPath( AppData.Path, FilePath );
    strcat( FilePath, "\\" );
    strcat( FilePath, PRO_EXAMPLE_EXE_NAME );
    strcpy( AppData.Path, FilePath );

    #endif

    // Reads in the file header, the preferences and all the TPage data
    if( !Loader_OpenEXEandReadHeader( &FileHeader, &ProjPrefsData, &PageList, AppData.Path ) )
        goto INIT_ERROR;

    // Load all the ini file settings
    Settings.GetPrivateString();

    // Create the medis player
    mediaPlayer.Init( hwnd, AppData.hInst, USE_ERROR_DLG_BOXES );

    // Create the buffer for blitting to the little preview window
    if( !buffer.CreateDIBSectionBuffer( GetScreenWidth(), GetScreenHeight(), BIT_COUNT_24 ) )
        goto INIT_ERROR;

    // Setup the rect
    SetRect( &DisplayRect, 0, 0, GetScreenWidth(), GetScreenHeight() );

    // Color the buffer white
    buffer.ColorRect( &DisplayRect, WHITE_BRUSH );

    // Load the GIF graphic from resource
    dib.LoadFromBuffer( gif.LoadFromResource( "settings", "GIF", hInst ) );

    // Load the first page
    if( !Loader_LoadFile( hwnd, &MainDib, PageList.GetMainDataOffset(0), AppData.Path ) )
        goto INIT_ERROR;

    // The background can be a color so just return TRUE if it is
    if( PageList.GetBackDataType( 0 ) > NO_FILE_TYPE && PageList.GetBackDataType( 0 ) < FILE_TYPE_META )
    {
        // Load the first page
        if( !Loader_LoadFile( hwnd, &BackDib, PageList.GetBackDataOffset(0), AppData.Path ) )
            goto INIT_ERROR;
    }

    // See if they choose to size all images to fit
    if( Settings.GetSizeImageToFit() )
    {
        for( i = 0; i < PageList.Count(); ++i )
            PageList.SetSizeToFit( i, TRUE );
    }

    // Update the buffer
    BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, 0, &PageList );

    // Assign a bitmap to the buttons
    PostMessage( GetDlgItem( hwnd, BTN_FLIP_LEFT ), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, 
                 (LPARAM) (HANDLE)LoadBitmap( AppData.hInst, "FlipLeft" ) );

    // Assign a bitmap to the buttons
    PostMessage( GetDlgItem( hwnd, BTN_FLIP_RIGHT ), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, 
                 (LPARAM) (HANDLE)LoadBitmap( AppData.hInst, "FlipRight" ) );

    // Assign a bitmap to the buttons
    PostMessage( GetDlgItem( hwnd, BTN_SOUND_PLAY ), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, 
                 (LPARAM) (HANDLE)LoadBitmap( AppData.hInst, "play" ) );

    // Assign a bitmap to the buttons
    PostMessage( GetDlgItem( hwnd, BTN_SOUND_PAUSE ), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, 
                 (LPARAM) (HANDLE)LoadBitmap( AppData.hInst, "pause" ) );

    // Assign a bitmap to the buttons
    PostMessage( GetDlgItem( hwnd, BTN_SOUND_STOP ), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, 
                 (LPARAM) (HANDLE)LoadBitmap( AppData.hInst, "stop" ) );

    // Set the new text to the window
    SetWindowText( hwnd, ProjPrefsData.Title );

    // Update the controls
    UpdateControls( hwnd, PageList.GetCurrentPage(), PageList.Count() );

    // Set the menu item checks
    if( Settings.GetSoundOnOff() )
        CheckMenuItem( CfgMenu, MNU_SOUND_ON, MF_CHECKED );

    if( Settings.GetTransOnOff() )
        CheckMenuItem( CfgMenu, MNU_TRANS_ON, MF_CHECKED );

    if( Settings.GetSizeImageToFit() )
        CheckMenuItem( CfgMenu, MNU_SIZE_ALL_IMAGES, MF_CHECKED );

    if( Settings.GetPlayMusicRandOrder() )
        CheckMenuItem( CfgMenu, MNU_RANDOMIZE_MUSIC, MF_CHECKED );

    if( Settings.GetScreensRandomOrder() )
        CheckMenuItem( CfgMenu, MNU_RANDOMIZE_SCREENS, MF_CHECKED );

    if( Settings.GetBackRandOrder() )
        CheckMenuItem( CfgMenu, MNU_RANDOMIZE_BACKS, MF_CHECKED );

    Result = TRUE;

INIT_ERROR:;

    if( !Result )
        // Kill the program
        EndDialog( hwnd, 0 );

    return Result;

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

            // Kill the program
            EndDialog( hwnd, 0 );
        break;

        case BTN_SYSTEM_QUIT:
        case BTN_CANCEL:
            // Kill the program
            EndDialog( hwnd, 0 );
        break;

        case BTN_FLIP_LEFT:
            if( PageList.Count() && PageList.GetCurrentPage() > 0  )
                FlipPage( hwnd, PAGE_FLIP_LEFT );
        break;

        case BTN_FLIP_RIGHT:
            if( PageList.Count() && PageList.GetCurrentPage() < PageList.Count()-1 )
                FlipPage( hwnd, PAGE_FLIP_RIGHT );
        break;

        case BTN_SOUND_PLAY:
            if( PageList.Count() && !mediaPlayer.GetPlayError() && PageList.GetSoundDataType( PageList.GetCurrentPage() ) )
                PlaySoundFile( hwnd, PageList.GetCurrentPage() );
        break;

        case BTN_SOUND_PAUSE:
            if( PageList.Count() && !mediaPlayer.GetPlayError() )
            {
                // If a file is playing, pause it
                if( mediaPlayer.GetStatus() == PLAYER_PLAYING )    
                   mediaPlayer.Pause();

                // if the file is paused, play it
                else if( mediaPlayer.GetStatus() == PLAYER_PAUSED )
                    mediaPlayer.Play();
            }
        break;

        case BTN_SOUND_STOP:
            if( PageList.Count() && !mediaPlayer.GetPlayError() )
            {
                // If a file is playing, stop it by closing it.
                if( mediaPlayer.GetStatus() != PLAYER_CLOSED )
                {
                    mediaPlayer.Close();

                    // If this page doesn't have a sound file associated with it
                    // then disable the stop button
                    if( !PageList.GetSoundDataType( PageList.GetCurrentPage() ) )
                        Button_Enable( GetDlgItem( hwnd, BTN_SOUND_STOP ), DISABLE );
                }
            }
        break;

        case MNU_WEB_LINK:
            ShellExecute( NULL, "open", WEB_ADDRESS, NULL, NULL, SW_SHOWNORMAL );
        break;

        case MNU_SOUND_ON:
        case MNU_TRANS_ON:
        case MNU_RANDOMIZE_MUSIC:
        case MNU_RANDOMIZE_SCREENS:
        case MNU_RANDOMIZE_BACKS:
            CheckUncheckMenu( hwnd, id );
        break;

        case MNU_SIZE_ALL_IMAGES:
            CheckUncheckMenu( hwnd, id );

            if( IsMenuChecked( hwnd, MNU_SIZE_ALL_IMAGES ) )
            {
                for( int i = 0; i < PageList.Count(); ++i )
                    PageList.SetSizeToFit( i, TRUE );
            }
            else
            {
                for( int i = 0; i < PageList.Count(); ++i )
                    PageList.SetSizeToFit( i, PageList.GetSizeToFitBackup( i ) );
            }

            // Set the view window
            BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, PageList.GetCurrentPage(), &PageList );

            // Force the view window to repaint
            ViewWnd_Repaint();

        break;

        case MNU_PRINT_SETUP:
            if( ProjPrefsData.ProjRegMode == FULL_REG_VERSION )
                Printer.DisplayPrinterSetup();
            else
                PostMsg( "Printing", "Printing is only allowed when Screen Saver to Go has been registered." );
        break;

        case MNU_PRINT:
            if( ProjPrefsData.ProjRegMode == FULL_REG_VERSION )
                Print( hwnd );
            else
                PostMsg( "Printing", "Printing is only allowed when Screen Saver to Go has been registered." );
        break;
    }
}   // OnCommand


/************************************************************************
*    FUNCTION NAME:         CheckUncheckMenu                                                             
*
*    DESCRIPTION:           Check or uncheck the menu.
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

void TConfigDlg::CheckUncheckMenu( HWND hwnd, int MenuID )
{
    HMENU hMenu;
    UINT uState;

    // Get the menu
    hMenu = GetMenu( hwnd );

    // Get the state of the menu
    uState = GetMenuState( hMenu, MenuID, MF_BYCOMMAND );

    // Check or uncheck it
    if( uState & MFS_CHECKED )
        CheckMenuItem( hMenu, MenuID, MF_UNCHECKED );
    else
        CheckMenuItem( hMenu, MenuID, MF_CHECKED );

}   // CheckUncheckMenu


/************************************************************************
*    FUNCTION NAME:         OnDestroy                                                             
*
*    DESCRIPTION:           Standard Windows OnDestroy message handler.
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

void TConfigDlg::OnDestroy( HWND hwnd )
{
    // Give the static control back its window procedure
    if( OldStaticWnd )
        SetWindowLong( AppData.hViewWnd, GWL_WNDPROC, LONG( OldStaticWnd ) );

    // delete any temp files
    // Copy the path to add the extension
    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".mp3" );
    DeleteFile( AppData.SoundFileTmpPath );

    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".wav" );
    DeleteFile( AppData.SoundFileTmpPath );

    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".mid" );
    DeleteFile( AppData.SoundFileTmpPath );

    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".rmi" );
    DeleteFile( AppData.SoundFileTmpPath );

}   // OnDestroy


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
    dib.BlitToDC( PaintDC, abs((r.right - dib.width()) / 2), 10 );

    // Must be called
    EndPaint( hwnd, &paintStruct );

}   // OnPaint


/************************************************************************
*    FUNCTION NAME:         PlaySoundFile                                                             
*
*    DESCRIPTION:           Loads and plays the sound file
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    8/23/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TConfigDlg::PlaySoundFile( HWND hwnd, WORD PageToLoad )
{
    TLoadBuffer lb;

    if( mediaPlayer.GetStatus() == PLAYER_PAUSED )
        mediaPlayer.Play();
    else
    {
        // If the player is not closed, close it
        if( mediaPlayer.GetStatus() != PLAYER_CLOSED )
            mediaPlayer.Close();

        // Load the data into the buffer
        if( Loader_LoadFileDataFromEXEfile( PageList.GetSoundDataOffset( PageToLoad ), &lb, AppData.Path ) )
        {
            // Attach the correct file extension to the file name
            switch( lb.Type )
            {
                case FILE_TYPE_MP3:
                    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".mp3" );
                break;

                case FILE_TYPE_WAV:
                    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".wav" );
                break;

                case FILE_TYPE_MID:
                    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".mid" );
                break;

                case FILE_TYPE_RMI:
                    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".rmi" );
                break;                  
            }

            // Write the music data to a temp file
            if( WriteDataToFile( AppData.SoundFileTmpPath, lb.pData, lb.Size, CREATE_ALWAYS ) )
            {
                // Play the music
                mediaPlayer.Play( AppData.SoundFileTmpPath );
            }

            // Delete the buffer because we are done with it
            delete [] lb.pData;
        }
    }

}   // PlaySoundFile


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
*    FUNCTION NAME:         FlipPage                                                             
*
*    DESCRIPTION:           Flips the page left or right
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd     - Handle of the window receiving the message
*              int Direction - Flip the page left or flip the page right
*              BOOL LoadSpecificPage - Flag that indicates the page to be
*                            shown is already set in the PageList and if
*                            this flag is TRUE, the function will not
*                            inc or dec the page in this function.
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

void TConfigDlg::FlipPage( HWND hwnd, int Direction )
{
    int CurPage;

    // Are we just flipping pages or loading a specific page
    if( PageList.Count() )
    {
        // Flip the page
        if( Direction == PAGE_FLIP_RIGHT )
            PageList.NextPage();
        else
            PageList.PreviousPage();
    }

    // Get the current page
    CurPage = PageList.GetCurrentPage();
             
    // Load the next page
    if( PageList.Count() )
    {
        // Load the main image
        if( !Loader_LoadFile( hwnd, &MainDib, PageList.GetMainDataOffset( CurPage ), AppData.Path ) )
        {
            // If there was an error, disable the program
            ErrorDisableControls( hwnd );
            return;
        }

        // The background can be a color
        if( PageList.GetBackDataType( CurPage ) > NO_FILE_TYPE && 
            PageList.GetBackDataType( CurPage ) < FILE_TYPE_META )
        {
            // Load the first page
            if( !Loader_LoadFile( hwnd, &BackDib, PageList.GetBackDataOffset( CurPage ), AppData.Path ) )
            {
                // If there was an error, disable the program
                ErrorDisableControls( hwnd );
                return;
            }
        }
    }

    // Disable the buttons if there are no pages to show
    if( !PageList.Count() )
        // If there was an error, disable the program
        ErrorDisableControls( hwnd );
    else
        UpdateControls( hwnd, CurPage, PageList.Count() );

    // Set the view window
    BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, CurPage, &PageList );

    // Force the view window to repaint
    ViewWnd_Repaint();
            
}   // FlipPage


/************************************************************************
*    FUNCTION NAME:         UpdateControls                                                             
*
*    DESCRIPTION:           Updates the controls to reflect the  
*                           changing conditions from page to page
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

void TConfigDlg::UpdateControls( HWND hwnd, int CurrentPage, int TotalPages )
{
    char StrTmp[ 50 ];
    int i;

    // Set the static control that shows the current page and total number of pages
    sprintf( StrTmp, "Page %u of %u", CurrentPage+1, TotalPages );
    Static_SetText( GetDlgItem( hwnd, STC_TOTAL_PAGES ), StrTmp );

    if( CurrentPage == TotalPages-1 )
        Button_Enable( GetDlgItem( hwnd, BTN_FLIP_RIGHT ), DISABLE );
    else
        Button_Enable( GetDlgItem( hwnd, BTN_FLIP_RIGHT ), ENABLE );

    if( CurrentPage == 0 )
        Button_Enable( GetDlgItem( hwnd, BTN_FLIP_LEFT ), DISABLE );
    else
        Button_Enable( GetDlgItem( hwnd, BTN_FLIP_LEFT ), ENABLE );


    // See if there is a sound associated with this page
    if( PageList.GetSoundDataType( CurrentPage ) )
    {
        for( i = 0; i < NUM_TBSOUNDBUTTONS; ++i )
            Button_Enable( GetDlgItem( hwnd, BTN_SOUND_PLAY+i ), ENABLE );
    }
    else
    {
        if( !mediaPlayer.GetPlayError() && mediaPlayer.GetStatus() != PLAYER_PLAYING )
        {
            for( i = 0; i < NUM_TBSOUNDBUTTONS; ++i )
                Button_Enable( GetDlgItem( hwnd, BTN_SOUND_PLAY+i ), DISABLE );
        }
        // If they go to a new page and the media player is playing, don't disable the stop button
        else
        {
            Button_Enable( GetDlgItem( hwnd, BTN_SOUND_PLAY ), DISABLE );
            Button_Enable( GetDlgItem( hwnd, BTN_SOUND_PAUSE ), DISABLE );
            Button_Enable( GetDlgItem( hwnd, BTN_SOUND_STOP ), ENABLE );
        }
    }

}   // UpdateControls


/************************************************************************
*    FUNCTION NAME:         ErrorDisableControls                                                             
*
*    DESCRIPTION:           Disable the controls on an error
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

void TConfigDlg::ErrorDisableControls( HWND hwnd )
{
    // disable the flip buttons
    Button_SetState( GetDlgItem( hwnd, BTN_FLIP_RIGHT ), DISABLE );
    Button_SetState( GetDlgItem( hwnd, BTN_FLIP_LEFT ), DISABLE );

}   // ErrorDisableControls


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
    Settings.SetSoundOnOff( IsMenuChecked( hwnd, MNU_SOUND_ON ) );
    Settings.SetTransOnOff( IsMenuChecked( hwnd, MNU_TRANS_ON ) );
    Settings.SetPlayMusicRandOrder( IsMenuChecked( hwnd, MNU_RANDOMIZE_MUSIC ) );
    Settings.SetSizeImageToFit( IsMenuChecked( hwnd, MNU_SIZE_ALL_IMAGES ) );
    Settings.SetScreensRandomOrder( IsMenuChecked( hwnd, MNU_RANDOMIZE_SCREENS ) );
    Settings.SetBackRandOrder( IsMenuChecked( hwnd, MNU_RANDOMIZE_BACKS ) );

}   // RecordUserSettings


/************************************************************************
*    FUNCTION NAME:         Print                                                             
*
*    DESCRIPTION:           Print a page to the printer
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
*    h rosenorn    8/23/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TConfigDlg::Print( HWND hwnd )
{
    LPPRINTDLG ppdlg;
    int i, j;
    RECT rect;
    double PrintableW, PrintableH, MultiW, MultiH, Multiplyer;
    double ImageW, ImageH, MarginX, MarginY;
    HWND hPrintingWnd = NULL;
    char PageNumber[20];
    BOOL Result = FALSE;
    TBitmap PrintBuffer;

    if( PageList.Count() )
    {
        ppdlg = Printer.DisplayPrintDlg( PageList.GetCurrentPage()+1, PageList.Count() );

        if( ppdlg )
        {
            if( Printer.BeginDoc( "Print page(s)" ) )
            {
				// Init the creation of the printer buffer with the printer dc
				PrintBuffer.InitActiveDC( Printer.Handle() );

                // Make the printer buffer
                if( !PrintBuffer.CreateDIBSectionBuffer( GetScreenWidth(), GetScreenHeight(), BIT_COUNT_24 ) )
                {
                    PostMsg( "Printing Error", "Error allocating printer buffer." );
                    goto ERROR_JUMP;
                }

                // Using CreateDialog causes the dialog to return rightaway.
                hPrintingWnd = DisplayPrintCancelDlg( hInst, hwnd, "Printing Page...", TRUE );
                if( !hPrintingWnd )
                    goto ERROR_JUMP;

                // Set the printer abort function
                SetAbortProc( Printer.Handle(), (ABORTPROC)PrintCancelDlg_AbortFunc );
            
                for( i = ppdlg->nFromPage; i < ppdlg->nToPage+1; ++i )
                {
                    // Load the main image
                    if( !Loader_LoadFile( hwnd, &MainDib, PageList.GetMainDataOffset( i-1 ), AppData.Path ) )
                        goto ERROR_JUMP;

                    // The background can be a color
                    if( PageList.GetBackDataType( i-1 ) > NO_FILE_TYPE && 
                        PageList.GetBackDataType( i-1 ) < FILE_TYPE_META )
                    {
                        // Load the first page
                        if( !Loader_LoadFile( hwnd, &BackDib, PageList.GetBackDataOffset( i-1 ), AppData.Path ) )
                            goto ERROR_JUMP;
                    }

                    // Set the view window
                    BldBuffer_ResetBuffer( &PrintBuffer, &MainDib, &BackDib, i-1, &PageList );

                    // Update the printing status
                    PrintCancelDlg_UpdateStatus( "Printing Page (%u)", i );
                    Sleep(100);

                    // Setup the margins
                    MarginX = Printer.GetpixelsPerInchX() / 2;
                    MarginY = Printer.GetpixelsPerInchY() / 2;

                    PrintableW = Printer.width();
                    PrintableH = Printer.height();

                    // The width or the height of the print area can't be zero
                    if( !PrintableW || !PrintableH ) 
                    {
                        PostMsg( "Printing Error", "Print area has no size" );
                        goto ERROR_JUMP;
                    }

                    // Determine the greatest size to blit into
                    PrintableW -= (MarginX * 2);
                    PrintableH -= ( MarginY * 2);

                    // Center the bitmap on the page
                    ImageW = PrintBuffer.width();
                    ImageH = PrintBuffer.height();

                    // The width or the height of any bitmap can't be zero. There is a problem
                    if( !ImageW || !ImageH )
                    {
                        PostMsg( "Printing Error", "Bitmap has no size" );
                        goto ERROR_JUMP;
                    }

                    // Who is bigger. The image or the printing surface
                    // Divide the largest number against the smaller number
                    if( ImageW < PrintableW )
                        MultiW = PrintableW / ImageW;
                    else
                        MultiW = ImageW / PrintableW;

                    if( ImageH < PrintableH )
                        MultiH = PrintableH / ImageH;
                    else
                        MultiH = ImageH / PrintableH;                       

                    // If the image is smaller than the printable area
                    // Multiply the multiplyer. If it is bigger than the
                    // printable area then divide.
                    if( ImageW < PrintableW && ImageH < PrintableH ) // Enlarge to fit
                    {
                        // See if one side will enlarge past the window rect
                        // Add 1 to compensate for floating point math to the window sizes
                        if( ImageW * MultiW > PrintableW+1 || ImageH * MultiW > PrintableH+1 )
                            Multiplyer = MultiH;
                        else if( ImageW * MultiH > PrintableW+1 || ImageH * MultiH > PrintableH+1 )
                            Multiplyer = MultiW;
                        else
                        {
                            // Since none of the windows extends past the window rect
                            // take the larger multiplyer
                            if( MultiW > MultiH )
                                Multiplyer = MultiW;
                            else
                                Multiplyer = MultiH;
                        }

                        rect.right = ImageW * Multiplyer;
                        rect.bottom = ImageH * Multiplyer;
                    }
                    else  // Reduce to fit
                    {
                        // See if one side will enlarge past the window rect
                        // Add 1 to compensate for floating point math to the window sizes
                        if( ImageW / MultiW > PrintableW+1 || ImageH / MultiW > PrintableH+1 )
                            Multiplyer = MultiH;
                        else if( ImageW / MultiH > PrintableW+1 || ImageH / MultiH > PrintableH+1 )
                            Multiplyer = MultiW;
                        else
                        {
                            // Since none of the windows extends past the window rect
                            // take the smaller multiplyer
                            if( MultiW < MultiH )
                                Multiplyer = MultiW;
                            else
                                Multiplyer = MultiH;
                        }

                        rect.right = ImageW / Multiplyer;
                        rect.bottom = ImageH / Multiplyer;
                    }

                    // Center the image onto the page
                    rect.left = ((PrintableW - rect.right) / 2) + MarginX;
                    rect.top = ((PrintableH - rect.bottom) / 2) + MarginY;

                    // Make the font
                    Printer.SetFont( "Arial", 8 );

                    for( j = 0; j < ppdlg->nCopies; ++j )
                    {
                        // Jump out if the user has canceled the print job
                        if( !PrintCancelDlg_GetPrintCancel() ) 
                            goto ERROR_JUMP;

                        Printer.BeginPage();

                        // If we are using a custom font, we'll need to remake the other font as well
                        Printer.SetFont( "Arial", 8 );

                        // Print out the page number on the bottom center of the screen
                        sprintf( PageNumber, "(%u)", i );
                        Printer.PrintText( (Printer.width()-Printer.TextWidth( PageNumber )) / 2, 
                                   Printer.height() - Printer.TextHeight( PageNumber ) - (MarginY / 2), PageNumber );

                        Printer.PrintBitmap( &PrintBuffer, &rect );

                        Printer.StopPage();
                    }
                }
            }
        }
    }

    // If we made it this far, the printing went ok
    Result = TRUE;

ERROR_JUMP:;

    // Were done. Stop everything
    Printer.StopDoc();

    if( hPrintingWnd )
        DestroyWindow( hPrintingWnd );

    // Free all printer objects
    Printer.Free();

    return Result;

}   // Print


/************************************************************************
*    FUNCTION NAME:         ViewWnd_OnPaint()                                                             
*
*    DESCRIPTION:           Standard Windows OnPaint message handler.
*                           The OnPaint is not being used at this time.
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

void TConfigDlg::ViewWnd_OnPaint( HWND hwnd )
{
    PAINTSTRUCT paintStruct;
    HDC PaintDC;
    RECT DisplayRect;
    HPALETTE hOldPalette = NULL;

    // Allow PAINTSTRUCT structure to get inited by the BeginPaint API call.
    // The BeginPaint and EndPaint API calls can only be use in the WM_PAINT
    // message handler. You CAN NOT paint any other way. Even if you have
    // nothing to paint, WM_PAINT message handler has to have the BeginPaint 
    // and EndPaint API calls or bad things will happen,
    PaintDC = BeginPaint( hwnd, &paintStruct );

    // Make sure we are ready to blit
    if( !buffer.IsEmpty() )
    {
        // Setup the rect
        SetRect( &DisplayRect, 0, 0, VIEW_WND_W, VIEW_WND_H );

        // You have to realize the palette each time you get a WM_PAINT message
        // or the bitmap will be drawn without it's colors. This does not apply
        // if the computer is in a higher color depth than 256.
        if( GetDeviceCaps( PaintDC, BITSPIXEL ) <= 8 )
        {
            // Save the old palette so that we can select it back in before
            // deleting the palette we created. DeleteObject doesn't seem to
            // work if your trying to delete a palette that is still in the DC
            // We only use the main dib's pallete
            hOldPalette = SelectPalette( PaintDC, MainDib.GetPalette(), FALSE );                 
            RealizePalette( PaintDC );
        }

        // Blit the buffer to the screen
        buffer.StretchDraw( PaintDC, &DisplayRect );

       // Reselect the old palette back into the DC so that the created palette can be deleted
        if( hOldPalette )
            SelectPalette( PaintDC, hOldPalette, FALSE );
    }

    // Must be called
    EndPaint( hwnd, &paintStruct );

}   // ViewWnd_OnPaint


/************************************************************************
*    FUNCTION NAME:         ViewWnd_Repaint                                                             
*
*    DESCRIPTION:           Force the window to repaint itself
*
*    FUNCTION PARAMETERS:
*    Input:    var2type Var2Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    8/18/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TConfigDlg::ViewWnd_Repaint()
{
    RECT DisplayRect;

    // Get the rect of our view window
    GetClientRect( AppData.hViewWnd, &DisplayRect );

    // Invalidate the whole window so it all gets painted
    InvalidateRect( AppData.hViewWnd, &DisplayRect, TRUE );

}   // ViewWnd_Repaint()