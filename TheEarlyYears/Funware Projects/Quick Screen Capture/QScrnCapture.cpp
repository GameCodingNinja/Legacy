
/************************************************************************
*
*    PRODUCT:         Quick Screen Capture
*
*    FILE NAME:       QScrnCapture.cpp
*
*    DESCRIPTION:     Screen Capture Program
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "GDefines.h"        // Header file with #defines
#include "QScrnCapture.h"    // Header file for this *.cpp file.
#include "FreeImage.h"       // Free Image library header
#include "DlgPrefs.h"        // Dialog box for setting the program preferences
#include "HelpHint.h"        // Helpful hint window. The text is loaded from resource or a buffer

// Globals
PCQScrnCaptWnd pQScrnCaptWnd;

// Global virtual key array
int gGrabKey[21] = {
    VK_F1,             
    VK_F2,             
    VK_F3,             
    VK_F4,             
    VK_F5,             
    VK_F6,             
    VK_F7,             
    VK_F8,             
    VK_F9,             
    VK_F10,            
    VK_F11,            
    VK_F12,
    VK_SCROLL,
    VK_PAUSE,
    VK_INSERT,
    VK_HOME,
    VK_DELETE,
    VK_END,
    VK_SHIFT,       
    VK_CONTROL,     
    VK_MENU, };

// Virtual Key string array
LPSTR keyDescription[21] = { "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", 
                            "Scroll Lock", "Pause", "Insert", "Home", "Delete", "End", "Shift", "Control", "Alt" };           

// File extension array
LPSTR formatExt[MAX_FILE_FORMATS] = { ".bmp", ".png", ".tif", ".tif", ".jpg", ".jpg", ".jpg", ".jpg", ".jpg", ".jpg" };

// Free Image format array
int fif[MAX_FILE_FORMATS] = { FIF_BMP, FIF_PNG, FIF_TIFF, FIF_TIFF, FIF_JPEG, FIF_JPEG, FIF_JPEG, FIF_JPEG, FIF_JPEG, FIF_JPEG };

// Free image flag array
int flag[MAX_FILE_FORMATS] = { BMP_DEFAULT, PNG_DEFAULT, TIFF_DEFAULT, TIFF_ADOBE_DEFLATE, JPEG_QUALITYSUPERB, JPEG_QUALITYGOOD, JPEG_DEFAULT, JPEG_QUALITYNORMAL, JPEG_QUALITYAVERAGE, JPEG_QUALITYBAD };     
         

/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         WinMain                                                             
*
*    DESCRIPTION:           The entry point for all good windows programs
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HINSTANCE hPrevInstance - Of no value to 32 bit windows programs
*              LPSTR lpszCmdParam - The command line is sent by Windows. The command
*                                   originally has the path to this program but
*                                   Windows parses it out before it hands it over.
*                                   To get the command line with the path in it you
*                                   need to call GetCommandLine() API call.
*              int nCmdShow - Specifies how the window is to be shown.
*                             Example: SW_SHOWMINIMIZED, SW_SHOWMAXIMIZED   
*
*    Output:   int - Usually it's the last message wParam.
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInstance,
                            LPSTR lpszCmdParam, int nCmdShow )
{
    MSG Msg;
    BOOL Result = FALSE;
    HACCEL hAccel;

    if( strlen( lpszCmdParam ) == 0 )
    {
        // Create the parent window object
        pQScrnCaptWnd = new CQScrnCaptWnd();

        // Create the parent window from a dialog
        if( pQScrnCaptWnd )
            Result = pQScrnCaptWnd->CreateParentWnd( hInst, nCmdShow );

        // Kick out of the program if there has been an error
        if( !Result )
            return Result;

        // Load the accelerators
        hAccel = LoadAccelerators( hInst, "DIALOG_ACC" );
        if( !hAccel )
        {
            MessageBox( pQScrnCaptWnd->GetParentWnd(), "Error loading accelerator table", "Error", MB_OK );
            return FALSE;
        }

        // Standard windows message loop
        while( GetMessage( &Msg, NULL, 0, 0 ) )
        {
            if( !TranslateAccelerator( pQScrnCaptWnd->GetParentWnd(), hAccel, &Msg ) )
            {
                TranslateMessage( &Msg );
                DispatchMessage( &Msg );
            }
        }
    }
    else
    {
        // Parse the command line and see what we need to do
        ParseCmndLine( hInst, lpszCmdParam );

        Msg.wParam = 0;
    }

    return Msg.wParam;

}   // WinMain


/************************************************************************
*    FUNCTION NAME:         ParseCmndLine                                                             
*
*    DESCRIPTION:           Parse the command line
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

void ParseCmndLine( HINSTANCE hInst, char *CndLine )
{
    char cmdChunk[MAX_PATH * 2];
    int size, index=0, i;
    BOOL OnlyActWnd = FALSE, PlayClick = FALSE;
    TLoadRes clickSndRes;
    DWORD sndTimer, timeTaken;

    // Create the parent window object
    CQScrnCaptWnd QScrnCaptWnd;

    // Get the settings in the ini file
    QScrnCaptWnd.Settings.GetPrivateString();

    // Get the size of the command line
    size = strlen( CndLine );

    i = 0;


    ///////////////////////////////////////////
    //
    // Get the second parameter. The bit depth
    //
    ///////////////////////////////////////////

    while( index < size )
    {
        // Break out if we hit a space
        if( CndLine[ index ] != ' ' )
        {
            cmdChunk[ i++ ] = CndLine[ index ];
        }
        else
        {
            ++index;
            break;
        }
    
        ++index;
    }

    // Terminate
    cmdChunk[ i ] = 0;

    // do a string compare
    if( strcmpi( cmdChunk, "1" ) == 0 )
    {
        // Load the resource for the camera click
        LoadTheResource( hInst, "CameraClick", "WAV", &clickSndRes );

        if( clickSndRes.pData )
        {
            PlaySound( (LPCSTR)clickSndRes.pData, NULL, SND_MEMORY | SND_ASYNC );
            PlayClick = TRUE;
        }
    }
    else if( strcmpi( cmdChunk, "0" ) == 0 )
    {
        // do nothing
    }
    else
    {
        // If none of these fit, just jump to the screen grab
        goto JUMP_TO_GRAB;
    }

    

    i = 0;

    
    ///////////////////////////////////////////
    //
    // Get the first parameter. The Save Option
    //
    ///////////////////////////////////////////
    while( index < size )
    {
        // Break out if we hit a space
        if( CndLine[ index ] != ' ' )
        {
            cmdChunk[ i++ ] = CndLine[ index ];
        }
        else
        {
            ++index;
            break;
        }
    
        ++index;
    }

    // Terminate
    cmdChunk[ i ] = 0;

    // Check for the first parameter

    // do a string compare
    if( strcmpi( cmdChunk, "bmp" ) == 0 )
        QScrnCaptWnd.Settings.SetSaveOption( FORMAT_BMP );

    else if( strcmpi( cmdChunk, "png" ) == 0 )
        QScrnCaptWnd.Settings.SetSaveOption( FORMAT_PNG );

    else if( strcmpi( cmdChunk, "tif" ) == 0 )
        QScrnCaptWnd.Settings.SetSaveOption( FORMAT_TIF );

    else if( strcmpi( cmdChunk, "tifcmp" ) == 0 )
        QScrnCaptWnd.Settings.SetSaveOption( FORMAT_TIF_CMP );

    else if( strcmpi( cmdChunk, "jpgS" ) == 0 )
        QScrnCaptWnd.Settings.SetSaveOption( FORMAT_JPG_SUP );

    else if( strcmpi( cmdChunk, "jpgH" ) == 0 )
        QScrnCaptWnd.Settings.SetSaveOption( FORMAT_JPG_HI );

    else if( strcmpi( cmdChunk, "jpgG" ) == 0 )
        QScrnCaptWnd.Settings.SetSaveOption( FORMAT_JPG_GOOD );

    else if( strcmpi( cmdChunk, "jpgN" ) == 0 )
        QScrnCaptWnd.Settings.SetSaveOption( FORMAT_JPG_NOR );

    else if( strcmpi( cmdChunk, "jpgA" ) == 0 )
        QScrnCaptWnd.Settings.SetSaveOption( FORMAT_JPG_AVG );

    else if( strcmpi( cmdChunk, "jpgB" ) == 0 )
        QScrnCaptWnd.Settings.SetSaveOption( FORMAT_JPG_BAD );

    else
    {
        // If none of these fit, just jump to the screen grab
        goto JUMP_TO_GRAB;
    }

    i = 0;


    ///////////////////////////////////////////
    //
    // Get the second parameter. The bit depth
    //
    ///////////////////////////////////////////

    while( index < size )
    {
        // Break out if we hit a space
        if( CndLine[ index ] != ' ' )
        {
            cmdChunk[ i++ ] = CndLine[ index ];
        }
        else
        {
            ++index;
            break;
        }
    
        ++index;
    }

    // Terminate
    cmdChunk[ i ] = 0;

    // do a string compare
    if( strcmpi( cmdChunk, "8" ) == 0 )
        QScrnCaptWnd.Settings.SetBitDepth( BIT_DEPTH_8 );

    else if( strcmpi( cmdChunk, "16" ) == 0 )
        QScrnCaptWnd.Settings.SetBitDepth( BIT_DEPTH_16 );

    else if( strcmpi( cmdChunk, "24" ) == 0 )
        QScrnCaptWnd.Settings.SetBitDepth( BIT_DEPTH_24 );

    else
    {
        // If none of these fit, just jump to the screen grab
        goto JUMP_TO_GRAB;
    }

    i = 0;


    ///////////////////////////////////////////
    //
    // Get the third parameter. The file name
    //
    ///////////////////////////////////////////
    
    while( index < size )
    {
        // Break out if we hit a space
        if( CndLine[ index ] != ' ' )
        {
            cmdChunk[ i++ ] = CndLine[ index ];
        }
        else
        {
            ++index;
            break;
        }
    
        ++index;
    }

    // Terminate
    cmdChunk[ i ] = 0;

    // See if we have a file name
    if( cmdChunk[0] )
    {
        strcpy( QScrnCaptWnd.Settings.GetFileName(), cmdChunk );
    }
    else
    {
        goto JUMP_TO_GRAB;
    }

    i = 0;


    ///////////////////////////////////////////
    //
    // Get the forth parameter. The path
    //
    ///////////////////////////////////////////

    while( index < size )
    {
        // Break out if we hit a space
        if( CndLine[ index ] != ' ' )
        {
            cmdChunk[ i++ ] = CndLine[ index ];
        }
        else
        {
            ++index;
            break;
        }
    
        ++index;
    }

    // Terminate
    cmdChunk[ i ] = 0;

    // See if we have a filepath
    if( cmdChunk[0] )
    {
        strcpy( QScrnCaptWnd.Settings.GetSavePath(), cmdChunk );
    }
    else
    {
        goto JUMP_TO_GRAB;
    }

    i = 0;


    ///////////////////////////////////////////
    //
    // Get the last parameter. The active screen.
    // Any value will work
    //
    ///////////////////////////////////////////

    while( index < size )
    {
        // Break out if we hit a space
        if( CndLine[ index ] != ' ' )
        {
            cmdChunk[ i++ ] = CndLine[ index ];
        }
        else
        {
            ++index;
            break;
        }
    
        ++index;
    }

    // Terminate
    cmdChunk[ i ] = 0;

    // do a string compare
    if( strcmpi( cmdChunk, "1" ) == 0 )
        OnlyActWnd = TRUE;


JUMP_TO_GRAB:;

    // Get the time in milliseconds
    sndTimer = timeGetTime();

    // Grab the screen and save it
    QScrnCaptWnd.GrabScreen( OnlyActWnd );

    // Sleep depending on how long it took to take the screen capture.
    // Otherwise the screen grab sound gets cut off.
    if( PlayClick )
    {
        // Get the elaps time
        timeTaken = timeGetTime() - sndTimer;

        // Only sleep if the time taken is less then the required sound time
        if( timeTaken < MAX_SOUND_TIME )
            Sleep( MAX_SOUND_TIME - timeTaken );
    }

}   // ParseCmndLine


/************************************************************************
*    FUNCTION NAME:         WndProc                                                             
*
*    DESCRIPTION:           Standard Windows message handler.
*                           We are using this to call our object functions
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

LRESULT CALLBACK WndProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    switch( Message )
    {
        HANDLE_MSG( hwnd, WM_INITDIALOG, pQScrnCaptWnd->OnInitDialog );
        HANDLE_MSG( hwnd, WM_COMMAND,    pQScrnCaptWnd->OnCommand );
        HANDLE_MSG( hwnd, WM_DESTROY,    pQScrnCaptWnd->OnDestroy );
        HANDLE_MSG( hwnd, WM_NCDESTROY,  pQScrnCaptWnd->OnNCDestroy );
        default:
            return FALSE;
    }

} // WndProc


/************************************************************************
*    FUNCTION NAME:         CheckForKeyPressTimer                                                             
*
*    DESCRIPTION:           Callback timer function.
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

void CALLBACK CheckForKeyPressTimer( HWND hwnd, UINT uTimerMsg, UINT uTimerID, DWORD dwTime )
{
    // Reset the nimber of mouse ticks to ignore
    pQScrnCaptWnd->CheckForKeyPress();

}   // CheckForKeyPressTimer


/************************************************************************
*    FUNCTION NAME:         CheckForRunningTimer                                                             
*
*    DESCRIPTION:           Callback timer function.
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

void CALLBACK CheckForRunningTimer( HWND hwnd, UINT uTimerMsg, UINT uTimerID, DWORD dwTime )
{
    // Reset the nimber of mouse ticks to ignore
    pQScrnCaptWnd->RunningTimerScreenShot();

}   // CheckForRunningTimer


/************************************************************************
*
*                      Window Object Functions
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

CQScrnCaptWnd::CQScrnCaptWnd()
{
    clickActive = FALSE;
    hParentWnd = 0;
    keyPressTimerReturnCode = 0;
	runningTimerReturnCode = 0;
	fileCounter = 0;

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

CQScrnCaptWnd::~CQScrnCaptWnd()
{
}   // Destructer



/************************************************************************
*    FUNCTION NAME:         CreateParentWnd()                                                             
*
*    DESCRIPTION:           Create the parent Window
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

BOOL CQScrnCaptWnd::CreateParentWnd( HINSTANCE hInstance, int nCmdShow )
{
    BOOL Result = FALSE;

    // Save the program instance
    hInst = hInstance;

    // Create the parent window
    if( CreateWnd( hInst, nCmdShow ) )
    {
        // Set the dialog windows caption icon to the app icon
        //SetClassLong( hParentWnd, GCL_HICON, (LONG) LoadIcon( hInst, "AppIcon" ) );

        Result = TRUE;
    }
    else
	{
        // Let them know there was a problem
        MessageBox( 0, "Error creating parent window.", "Error", MB_OK );
	}

    return Result;

}   // CreateParentWnd 


/************************************************************************
*    FUNCTION NAME:         CreateWnd                                                             
*
*    DESCRIPTION:           Create the parent window from a dialog
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*
*              int nCmdShow    - Specifies how the window is to be shown.
*                                Example: SW_SHOWMINIMIZED, SW_SHOWMAXIMIZED
*
*    Output:   HWND - A handle to the newly created window
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CQScrnCaptWnd::CreateWnd( HINSTANCE hInstance, int nCmdShow )
{
    // Create the parent window & save the window handle
    hParentWnd = CreateDialog( hInstance, "DLG_QSCREEN_CAPTURE_WND", NULL, (DLGPROC)WndProc );

    // Make sure we have a handle to the window
    if( hParentWnd == NULL )
        return FALSE;

    return TRUE;

}   // CreateWnd


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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CQScrnCaptWnd::OnDestroy( HWND hwnd )
{
    // Save the INI information
    Settings.WritePrivateString();

    // Double check that we did get a timer. Don't want to kill a timer 
    // we didn't have because sometimes windows runs out of timers.
    if( keyPressTimerReturnCode != 0 )
	{
        // Free the timer
        KillTimer( hwnd, CHECK_FOR_KEYPRESS_TIMER );
	}

    if( runningTimerReturnCode != 0 )
	{
        // Free the timer
        KillTimer( hwnd, RUNNING_TIMER );
	}

    // This causes the WinMain to fall out of it's while loop
    PostQuitMessage( 0 );

} // OnDestroy


/************************************************************************
*    FUNCTION NAME:         QuitProgram                                                             
*
*    DESCRIPTION:           Quit the program by destroying the window
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

BOOL CQScrnCaptWnd::QuitProgram( HWND hwnd )
{
    // Save the state of the active window
    Settings.SetActiveWnd( Button_GetCheck( GetDlgItem( hwnd, CHK_GRB_ACTIVE_WND ) ) );

    // Save the settings
    Settings.WritePrivateString();
   
    // Destroy the window
    DestroyWindow( hwnd );

    return 0;

}   // OnClose


/************************************************************************
*    FUNCTION NAME:         OnCommand                                                             
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CQScrnCaptWnd::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case DLG_QUIT_BOX:
        case MNU_EXIT:
            QuitProgram( hwnd );        
        break;

        case MNU_WEB_LINK:
            ShellExecute( NULL, "open", WEB_ADDRESS, NULL, NULL, SW_SHOWNORMAL );
        break;

        case MNU_PREFS:
            Display_PrefsDlg( hInst, hwnd, &Settings );

            // Set the info text
            UpdateInfo( hwnd );
        break;

        case MNU_HELP:
            MnuClk_Help( hwnd );
        break;

		case CHK_TIMER_CAPTURE:
			CreateRunningTimer( hwnd );	
		break;
    }

}   // OnCommand


/************************************************************************
*    FUNCTION NAME:         CreateRunningTimer()                                                             
*
*    DESCRIPTION:           Create a running timer
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

void CQScrnCaptWnd::CreateRunningTimer( HWND hwnd )
{
	// Create the running timer if we don't have one
	if( runningTimerReturnCode == 0 )
	{
		// reset the file counter
		fileCounter = 0;

		DWORD interval = (Settings.GetTimerHour() * 3600000) +
						 (Settings.GetTimerMinute() * 60000) + 
						 (Settings.GetTimerSecond() * 1000);

		if( interval < 30000 )
		{
			MessageBox( hwnd, "With the current timer settings and depending on your available amount of hard drive space, you could end up depleting your hard drive space which could cause Windows not to load! Please use with caution!", "Warning!", MB_OK );
		}

		// Get a Windows timer
		if( !(runningTimerReturnCode = (int)SetTimer( hwnd, RUNNING_TIMER, interval, (TIMERPROC)CheckForRunningTimer )) )
		{
		    MessageBox( hwnd, "Your computer is out of Windows timers. The Quick Screen Capture program needs a Windows timer to work. Try restarting your computer.", "Error", MB_OK );
		}
	}
	else
	{
		KillTimer( hwnd, RUNNING_TIMER );
		runningTimerReturnCode = 0;
	}

}	// CreateRunningTimer


/************************************************************************
*    FUNCTION NAME:         MnuClk_Help()                                                             
*
*    DESCRIPTION:           Simple help screen
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

void CQScrnCaptWnd::MnuClk_Help( HWND hwnd )
{
    DisplayHelpHintDialogBox( hInst, hwnd, "help", "TXT", 
                              WND_CAPTION, "Quick Help", 
                              NULL, TRUE, NULL, 0, TRUE );
}   // MnuClk_Help


/************************************************************************
*    FUNCTION NAME:         OnInitDialog()                                                             
*
*    DESCRIPTION:           Standard Windows OnCreate message handler.
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

BOOL CQScrnCaptWnd::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    BOOL Result = FALSE;

    PostMessage(hwnd, WM_SETICON, (WPARAM)false, (LPARAM)LoadIcon( hInst, "AppIcon" ) );
    
    // Get the settings in the ini file
    Settings.GetPrivateString();

    // This clears out any previous key presses so that when the 
    // program starts, it doesn't take a screen capture on startup.
    GetAsyncKeyState( VK_SNAPSHOT );
    GetAsyncKeyState( gGrabKey[ Settings.GetGrabKey() ] );

    // Get a Windows timer
    if( !(keyPressTimerReturnCode = (int)SetTimer( hwnd, CHECK_FOR_KEYPRESS_TIMER, 100, (TIMERPROC)CheckForKeyPressTimer )) )
    {
        MessageBox( hwnd, "Your computer is out of Windows timers. The Quick Screen Capture program needs a Windows timer to work. Try restarting your computer.", "Error", MB_OK );
    
        goto HANDLE_ERROR;
    }

	// Load the resource for the camera click
	LoadTheResource( hInst, "CameraClick", "WAV", &clickSndRes );

    // Set the windows caption
    SetWindowText( hwnd, WND_CAPTION );

    // Set the active window checkbox
    Button_SetCheck( GetDlgItem( hwnd, CHK_GRB_ACTIVE_WND ), Settings.GetActiveWnd() );

    // Set the info text
    UpdateInfo( hwnd );

    Result = TRUE;

HANDLE_ERROR:;

    if( !Result )
        PostQuitMessage( 0 );

    return Result;

}   // Starter_OnCreate


/************************************************************************
*    FUNCTION NAME:         CheckForKeyPressTimer                                                             
*
*    DESCRIPTION:           Check for key press
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

void CQScrnCaptWnd::CheckForKeyPress()
{
    // Make sure we are not busy before processing screen grabs
    if( !clickActive )
    {
        // Set to true to keep key presses out until
        // screen capture is done
        clickActive = TRUE;

        // See if the key has been pressed
        if( GetAsyncKeyState( VK_SNAPSHOT ) || 
            GetAsyncKeyState( gGrabKey[ Settings.GetGrabKey() ] ) )
        {
            if( clickSndRes.pData && Settings.GetSoundEffect() )
			{
                PlaySound( (LPCSTR)clickSndRes.pData, NULL, SND_MEMORY | SND_ASYNC );
			}

            GrabScreen();

            Sleep( 300 );
        }

        // Allow key press captures again
        clickActive = FALSE;
    }
}   // CheckForKeyPress


/************************************************************************
*    FUNCTION NAME:         RunningTimerScreenShot                                                             
*
*    DESCRIPTION:           running timer
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

void CQScrnCaptWnd::RunningTimerScreenShot()
{
    // Make sure we are not busy before processing screen grabs
    if( !clickActive )
    {
    	if( clickSndRes.pData && Settings.GetSoundEffect() )
		{
            PlaySound( (LPCSTR)clickSndRes.pData, NULL, SND_MEMORY | SND_ASYNC );
		}

        GrabScreen();
    }
}   // RunningTimerScreenShot


/************************************************************************
*    FUNCTION NAME:         GrabScreen                                                             
*
*    DESCRIPTION:           Grab the windows desktop
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

void CQScrnCaptWnd::GrabScreen( BOOL OnlyActWnd )
{
    HDC hScrDC, hBmpDc = NULL;
    int X=0, Y=0, Width, Height, screenW, screenH;
    HBITMAP hBitmap = NULL, hOldBitmap = NULL;
    HWND hForgrndWnd;
    RECT rect;

    // Get the size of the screen. This is only the size of the 
    // first monitor if more then monitor is present
    Width = GetSystemMetrics( SM_CXSCREEN );
    Height = GetSystemMetrics( SM_CYSCREEN );

    // If we have duel monitor support, get the virtual desktop size.
    // The calls in this if statement are not supported under all Windows OS's. So
    // if the SM_CMONITORS returns true, then the other calls in this if statement
    // are supported.
    if( GetSystemMetrics( SM_CMONITORS ) > 1 )
    {
        // The virtual desktop can be offsetted
        Width = GetSystemMetrics( SM_CXVIRTUALSCREEN );
        Height = GetSystemMetrics( SM_CYVIRTUALSCREEN );
    }

    // Capture only the window they have selected
    if( OnlyActWnd || (hParentWnd && Button_GetCheck( GetDlgItem( hParentWnd, CHK_GRB_ACTIVE_WND ) )) )
    {
        // Save the width and height of the screen
        screenW = Width;
        screenH = Height;
        
        // Get the forground window handle
        hForgrndWnd = GetForegroundWindow();

        // Get the parent windows rect
        GetWindowRect( hForgrndWnd, &rect );

        // Get the width and height of the foreground window
        Width = rect.right - rect.left;
        Height = rect.bottom - rect.top;
        X = rect.left;
        Y = rect.top;

        // Clip the X and reset the width of the rect
        if( X < 0 )
        {               
            // Reset the width of the rect
            // This is the same as Width - X without the abs()
            Width += X;

            // Reset the X of the rect
            X = 0;
        }

        // Clip the Y and reset the height of the rect
        if( Y < 0 )
        {        
            // Reset the height of the rect
            // This is the same as Height - Y without the abs()
            Height += Y;

            // reset the Y of the rect
            Y = 0;
        }

        // Clip the width of the destination rect
        if( X + Width > screenW )
            Width = screenW - X;
            
        // Clip the height of the destination rect
        if( Y + Height > screenH )
            Height = screenH - Y;
    }

    // Check the width of the screen area
    if( Width < 1 )
	{
        MessageBox( hParentWnd, "Error, screen width value is 0 or less", "Error", MB_OK );
        goto HANDLE_ERROR;
    }

    // Check the width of the screen area
    if( Height < 1 )
	{
        MessageBox( hParentWnd, "Error, screen height value is 0 or less", "Error", MB_OK );
        goto HANDLE_ERROR;
    }

    // Get the DC to the Windows desktop
    if( !(hScrDC = GetDC( 0 )) )
    {
        MessageBox( hParentWnd, "Error getting desktop DC. Restart your computer and try again", "Error", MB_OK );
        goto HANDLE_ERROR;
    }

    // Create a bitmap compatible with the screen DC
    if( !(hBitmap = CreateCompatibleBitmap( hScrDC, Width, Height )) )
    {
        MessageBox( hParentWnd, "Error creating bitmap. Restart your computer and try again", "Error", MB_OK );
        goto HANDLE_ERROR;
    }

    // Create the compatible dc but check that we have a valid handle
    if( !(hBmpDc = CreateCompatibleDC( hScrDC )) )
    {
        MessageBox( hParentWnd, "Error creating compatible DC. Restart your computer and try again", "Error", MB_OK );
        goto HANDLE_ERROR;
    }

    // Select new bitmap into memory DC
    hOldBitmap = (HBITMAP) SelectObject( hBmpDc, hBitmap );

    // Make sure there were no errors selecting the bitmap into the DC
    if( hOldBitmap == (HBITMAP)NULL || hOldBitmap == (HBITMAP)GDI_ERROR )
    {
        MessageBox( hParentWnd, "Error selecting bitmap into DC. Restart your computer and try again", "Error", MB_OK );
        goto HANDLE_ERROR;
    }

    // blit screen DC to bitmap DC
    BitBlt( hBmpDc, 0, 0, Width, Height, hScrDC, X, Y, SRCCOPY );

    // Make the bitmap
    MakeBitmap( hScrDC, hBmpDc, hBitmap, Width, Height, GetDeviceCaps( hScrDC, BITSPIXEL ) );

HANDLE_ERROR:;
    
    if( hBmpDc )
    {
        // Select old bitmap back into memory DC
        if( hOldBitmap )
            SelectObject( hBmpDc, hOldBitmap );

        // Delete the created bitmap DC 
        DeleteDC( hBmpDc );
    }

    if( hBitmap )
        DeleteBitmap( hBitmap );

    // Release the handle to the desktop DC
    if( hScrDC )
        ReleaseDC( 0, hScrDC  );

}   // GrabScreen


/************************************************************************
*    FUNCTION NAME:         MakeBitmap                                                             
*
*    DESCRIPTION:           Make the bitmap
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

void CQScrnCaptWnd::MakeBitmap( HDC hScrDC, HDC hBmpDc, HBITMAP hBitmap, int Width, int Height, int bitsPerPixel )
{
    FIBITMAP *dib = NULL, *dibConvert = NULL;
    BYTE *pFBits;
    BITMAPINFO bi;
    char fileSave[ MAX_PATH * 2 ];
    int i = 0, UserBitDepth;
    BOOL fileFound;

	if( runningTimerReturnCode != 0 )
	{
		i = fileCounter;
	}

    // Create a freeimage bitmap. Everything is grabbed as a 24 bit image and converted from this point
    if( !(dib = FreeImage_Allocate( Width, Height, 24 )) )
    {
        MessageBox( hParentWnd, "Error allocating bitmap. Restart your computer and try again", "Error", MB_OK );

        goto HANDLE_ERROR;
    }

    // Get the bits to the free image data
    if( !(pFBits = FreeImage_GetBits( dib )) )
    {
        MessageBox( hParentWnd, "Error getting allocated bits. Restart your computer and try again", "Error", MB_OK );

        goto HANDLE_ERROR;
    }

    // Fill out the info header
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = Width;
    bi.bmiHeader.biHeight = Height;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 24;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biSizeImage = 0;
    bi.bmiHeader.biXPelsPerMeter = DEFAULT_METERS_PER_INCH;
    bi.bmiHeader.biYPelsPerMeter = DEFAULT_METERS_PER_INCH;
    bi.bmiHeader.biClrUsed = 0;
    bi.bmiHeader.biClrImportant = 0;

    if( !GetDIBits( hBmpDc, hBitmap, 0, Height, pFBits, (BITMAPINFO *)&bi, DIB_RGB_COLORS ) )
    {
        MessageBox( hParentWnd, "Error copying bits. Restart your computer and try again", "Error", MB_OK );

        goto HANDLE_ERROR;
    }

    do
    {
        if( strlen( Settings.GetSavePath() ) > 3 )
        {
            if( i < 9 )
                wsprintf( fileSave, "%s\\%s00%d%s", Settings.GetSavePath(), Settings.GetFileName(), i+1, formatExt[ Settings.GetSaveOption() ] );
            else if( i < 99 )
                wsprintf( fileSave, "%s\\%s0%d%s", Settings.GetSavePath(), Settings.GetFileName(), i+1, formatExt[ Settings.GetSaveOption() ] );
            else if( i < 999 )
                wsprintf( fileSave, "%s\\%s%d%s", Settings.GetSavePath(), Settings.GetFileName(), i+1, formatExt[ Settings.GetSaveOption() ] );
        }
        else
        {
            if( i < 9 )
                wsprintf( fileSave, "%s%s00%d%s", Settings.GetSavePath(), Settings.GetFileName(), i+1, formatExt[ Settings.GetSaveOption() ] );
            else if( i < 99 )
                wsprintf( fileSave, "%s%s0%d%s", Settings.GetSavePath(), Settings.GetFileName(), i+1, formatExt[ Settings.GetSaveOption() ] );
            else if( i < 999 )
                wsprintf( fileSave, "%s%s%d%s", Settings.GetSavePath(), Settings.GetFileName(), i+1, formatExt[ Settings.GetSaveOption() ] );
        }

        // See if the file name is already there
        fileFound = FileExist( fileSave, Settings.GetSavePath() );

        ++i;

		if( i > 998 )
		{
			break;
		}
    }
    while( fileFound );

	// Init the global file counter
	if( runningTimerReturnCode != 0 )
	{
		fileCounter = i;
	}

    // Is the file directory full?
	if( i > 999 )
	{
		goto HANDLE_ERROR;
	}

    // Get the selected bit depth. Depending on what they have chosen
    // this value will need to be changed because the file format doesn't
    // support it.
    UserBitDepth = Settings.GetBitDepth();

    // See what format they want depending on the system bit depth
    if( UserBitDepth == BIT_DEPTH_SYS_DEF )
    {
        if( bitsPerPixel == 8 )
            UserBitDepth = BIT_DEPTH_8;
        else if( bitsPerPixel == 16 )
            UserBitDepth = BIT_DEPTH_16;
    }

    // Depending on the file format they have choosen
    // not all bit depths are supported. Change the bit depth
    // to 24 bit if the desired bit depth is not supported under
    // the chosen file format
    // PNG only supports 8 and 24 bit
    if( Settings.GetSaveOption() == FORMAT_PNG &&
        UserBitDepth == BIT_DEPTH_16 )
    {
        UserBitDepth = BIT_DEPTH_24;    
    }
    // TIF only supports 8 and 24 bit
    else if( Settings.GetSaveOption() == FORMAT_TIF &&
             UserBitDepth == BIT_DEPTH_16 )
    {
        UserBitDepth = BIT_DEPTH_24;    
    }
    // JPG only supports 24 bit
    else if( Settings.GetSaveOption() >= FORMAT_JPG_SUP )
    {
        UserBitDepth = BIT_DEPTH_24;    
    }

    // Convert the bit depth to the selected bit depth
    if( UserBitDepth == BIT_DEPTH_8 )
        dibConvert = FreeImage_ColorQuantize( dib, FIQ_WUQUANT );

    else if( UserBitDepth == BIT_DEPTH_16 )
        dibConvert = FreeImage_ConvertTo16Bits555( dib );

    // Save the file. If there is an error
    if( !FreeImage_Save( (FREE_IMAGE_FORMAT)fif[ Settings.GetSaveOption() ], dibConvert ? dibConvert : dib, fileSave, flag[ Settings.GetSaveOption() ] ) )
        PostMsg( "Save Error", "There was an error saving the screen capture file to the hard drive. Check that you have enough hard drive space and that the folder you are trying to save it to exists." );

HANDLE_ERROR:;

    // This clears out any previous key presses.
    GetAsyncKeyState( VK_SNAPSHOT );
    GetAsyncKeyState( gGrabKey[ Settings.GetGrabKey() ] );

    if( dib )
        FreeImage_Free( dib );

    if( dibConvert )
        FreeImage_Free( dibConvert );

}   // MakeBitmap


/************************************************************************
*    FUNCTION NAME:         UpdateInfo                                                             
*
*    DESCRIPTION:           Update the info string
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

void CQScrnCaptWnd::UpdateInfo( HWND hwnd )
{
    char strTmp[100];

    wsprintf( strTmp, "To grab the screen, press %s or the Print Scrn key.", keyDescription[ Settings.GetGrabKey() ] );

    Static_SetText( GetDlgItem( hwnd, STC_DISPLAY_INFO ), strTmp );

	// see if we need to enable the timer options
	if( (Settings.GetTimerHour() > 0) ||
	 	(Settings.GetTimerMinute() > 0) ||
	 	(Settings.GetTimerSecond() > 0) )
	{
		ComboBox_Enable( GetDlgItem( hwnd, CHK_TIMER_CAPTURE ), true );
	}
	else
	{
		ComboBox_Enable( GetDlgItem( hwnd, CHK_TIMER_CAPTURE ), false );
	}

}   // UpdateInfo