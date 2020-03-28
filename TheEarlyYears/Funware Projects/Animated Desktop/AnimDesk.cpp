
/************************************************************************
*
*    PRODUCT:         Animated Desktop
*
*    FILE NAME:       AnimDesk.cpp
*
*    DESCRIPTION:     Program the runs small animations while the computer
*                     is on durring normal use 
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
#include "gTools.h"          // A misc group of useful functions
#include "AnimDesk.h"        // Header file for this *.cpp file.
#include "ConfigDlg.h"   // Config dialog box

// Globals
// Create the parent window object
CAnimDeskWnd animDeskWnd;
const UINT WM_ICON_NOT = WM_USER + TRAY_ICON_ID;


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
    BOOL Result;

    // Make sure the program is not already running
    if( DoExist( SEMAPHORE ) )
        return FALSE;

    // Create the parent window
    Result = animDeskWnd.CreateParentWnd( hInst, nCmdShow );

    // Kick out of the program if there has been an error
    if( !Result )
        return Result;

    // Standard windows message loop
    while( GetMessage( &Msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &Msg );
        DispatchMessage( &Msg );
    }

    return Msg.wParam;

}   // WinMain


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
        HANDLE_MSG( hwnd, WM_DESTROY, animDeskWnd.OnDestroy );
        HANDLE_MSG( hwnd, WM_COMMAND, animDeskWnd.OnCommand );
        HANDLE_MSG( hwnd, WM_CREATE,  animDeskWnd.OnCreate );
        case WM_ICON_NOT:
            animDeskWnd.OnIconNotic( hwnd, wParam, lParam );
        break;
        default:
            return DefWindowProc( hwnd, Message, wParam, lParam );
    }

    return 0;

} // WndProc


/************************************************************************
*    FUNCTION NAME:         SaverWndProc                                                             
*
*    DESCRIPTION:           Standard Windows message handler. Normally you
*                           would make use of WindowsX macros, but a screen
*                           doesn't require a lot of message cracking so
*                           only a few are being used.
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

LRESULT CALLBACK AnimatedWndProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
	switch( Message )
	{
		case WM_ERASEBKGND:
		//case WM_ICONERASEBKGND:
		//case WM_PAINTICON:
		//case WM_DISPLAYCHANGE:
		case WM_NCPAINT:
		//case WM_SETREDRAW:
			animDeskWnd.SelfErase = TRUE;
	    return FALSE;
	}
    	
    return DefWindowProc( hwnd, Message, wParam, lParam );

} // SaverWndProc


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

CAnimDeskWnd::CAnimDeskWnd()
{
    // Init class members
    hmenu = NULL;
	hAnimWnd = NULL;
	hAnimDC = NULL;
	SelfErase = FALSE;
	SelfEraceCounter = 0;

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

CAnimDeskWnd::~CAnimDeskWnd()
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

BOOL CAnimDeskWnd::CreateParentWnd( HINSTANCE hInstance, int nCmdShow )
{
    // Save the program instance
    hInst = hInstance;

    // Register the parent window with Windows
    if( !RegisterWnd( hInst ) )
        return FALSE;

    // Create the parent window
    if( !CreateWnd( hInst, nCmdShow ) )
        return FALSE;

    return TRUE;

}   // CreateParentWnd


/************************************************************************
*    FUNCTION NAME:         RegisterWnd                                                             
*
*    DESCRIPTION:           All windows created with CreateWindow
*                           must be registered first. This does not include
*                           windows controls like buttons, listboxex and such
*                           because they register themselves.
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*
*    Output:   BOOL            - Returns TRUE or FALSE to indicate if
*                                the task was secessful.
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CAnimDeskWnd::RegisterWnd( HINSTANCE hInstance )
{
    WNDCLASSEX w;

    w.cbSize = sizeof( WNDCLASSEX );
    w.style = CS_HREDRAW|CS_VREDRAW;
    w.lpfnWndProc = WndProc;
    w.cbClsExtra = 0;
    w.cbWndExtra = 0;
    w.hInstance = hInstance;
    w.hIcon = LoadIcon( hInst, "AppIcon" );
    w.hIconSm = NULL;
    w.hCursor = LoadCursor( NULL, IDC_ARROW );
    w.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
    w.lpszMenuName = NULL;
    w.lpszClassName = "AnimDesk";

    if( RegisterClassEx(&w) == 0 )
		return 0;

	w.lpfnWndProc = AnimatedWndProc;
	w.lpszClassName = "AnimatedWnd";

    return ( RegisterClassEx(&w) != 0 );

}   // Starter_RegisterWnd 


/************************************************************************
*    FUNCTION NAME:         CreateWnd                                                             
*
*    DESCRIPTION:           Create the parent window
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

BOOL CAnimDeskWnd::CreateWnd( HINSTANCE hInstance, int nCmdShow )
{
    // Create the parent window & save the window handle
    hParentWnd = CreateWindow( "AnimDesk", "The API Tray App", WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT,
                               CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL );

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

void CAnimDeskWnd::OnDestroy( HWND hwnd )
{
    // Delete the tray icon
    Shell_NotifyIcon( NIM_DELETE, &notIconData );

    // Delete the menu we created
    if( hmenu )
    {
        DestroyMenu( hmenu );
        hmenu = NULL;
    }

	if( hAnimDC )
		ReleaseDC( hAnimWnd, hAnimDC );

    // Kill the thread if it is still running
    killTheThread( hwnd );
    
    // This causes the WinMain to fall out of it's while loop
    PostQuitMessage( 0 );

} // OnDestroy


/************************************************************************
*    FUNCTION NAME:         killTheThread                                                             
*
*    DESCRIPTION:           Kills the thread
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CAnimDeskWnd::killTheThread( HWND hwnd )
{
    DWORD ThreadExitCode = 0;
    DWORD FalseTimer;
    DWORD ThreadWaitTimeAmount = FIVE_SECONDS;
    BOOL Result = TRUE;

    // Setup a timer to make sure we are never stuck in a loop
    FalseTimer = timeGetTime() + TWO_SECONDS;

    // Kill the screen saver by causeing the thread to fall through
    // Letting a thread return on it's own is the best way of terminating a thread
    // Since two processes share this data, make real sure we set it to false.
    // and just to be safe, set a timer so that it will fall out if there is a major problem
    while( tData.active && FalseTimer > timeGetTime() )
        tData.active = FALSE;

    // Get the exit code for the thread to see if it is still active.
    if( tData.hThread )
        GetExitCodeThread( tData.hThread, &ThreadExitCode );

    // Is the thread still active?
    if( ThreadExitCode == STILL_ACTIVE )
    {
        // Wait for the thread to finish freeing up it's resources before
        // exiting the screen saver. It's a good idea to timeout anytime we are waiting for.
        if( WaitForSingleObject( tData.hThread, ThreadWaitTimeAmount ) == WAIT_TIMEOUT )
        {
            MessageBox( hwnd, "Animated desktop thread time out error. Attempting to terminate thread.",
                        "Error", MB_ICONINFORMATION );

            // Lets try one more time to see if it is still active before we kill it the hard way.
            GetExitCodeThread( tData.hThread, &ThreadExitCode );

            // Set to false because we are not having a normal termination of the thread
            Result = FALSE;

            if( ThreadExitCode == STILL_ACTIVE )
            {
                // Try to terminate the thread. Use force if you have to.
                if( !TerminateThread( tData.hThread, FALSE ) )
                    // Fire in the hole! Woman and children first!
                    MessageBox( hwnd, "Unable to terminate the thread. It is advised that you restart your computer.",
                            "Error", MB_ICONINFORMATION );
            }

            // Free the thread handle
            CloseHandle( tData.hThread );

        }
        else
            // Free the thread handle
            CloseHandle( tData.hThread );
    }
    else 
    {
        // Free the thread handle
        if( tData.hThread )
            CloseHandle( tData.hThread );
    }

    return Result;

}   // killTheThread


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

void CAnimDeskWnd::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case MNU_PROPERTIES:
            Display_ConfigDlg( hInst, hwnd );
        break;
        
        case MNU_EXIT:
            // Destroy the parent window
            DestroyWindow( hwnd );
        break;
    }

}   // OnCommand


/************************************************************************
*    FUNCTION NAME:         OnIconNotic                                                             
*
*    DESCRIPTION:           Tray app icon notification message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
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

void CAnimDeskWnd::OnIconNotic( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
    switch( lParam )
    {
        case WM_LBUTTONDBLCLK:
            Display_ConfigDlg( hInst, hwnd );
        break;
        
        case WM_RBUTTONUP:
            // get the position of the cursor
            GetCursorPos( &cursorPos );

            // Show the menu
            TrackPopupMenuEx( hSubMenu, 0, cursorPos.x, cursorPos.y, hwnd, NULL );
        break;  
    }

}   // OnIconNotic


/************************************************************************
*    FUNCTION NAME:         OnCreate()                                                             
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

BOOL CAnimDeskWnd::OnCreate( HWND hwnd, LPCREATESTRUCT lpCreateStruct )
{
    BOOL Result = FALSE;

    // Make sure we can start the tray app
    if( !SetUpTrayApp( hwnd ) )
    {
        MessageBox( hwnd, "Could not create Tray Application. Try restarting your computer.", "Error", MB_OK );
        goto HANDLE_ERROR;
    }

    tData.animCycleTimeOut = 40L;
    tData.animTimeOut = 6000L;

	// Load the artwork
	tData.art.LoadFromFile( "c:\\temp\\ships.bmp" );
	tData.art.CreateBitmap( STANDARD_DIB );
	tData.art.DeleteBitmapBits();

	// create the buffers
	tData.clean.CreateStandardBuffer( 126, 134 );
	tData.dirty.CreateStandardBuffer( 126, 134 );

    // Create the screen saver window
    hAnimWnd = CreateWindowEx( WS_EX_TOPMOST, "AnimatedWnd", "AnimatedWnd", WS_VISIBLE|CS_OWNDC|WS_POPUP,
                           -50, 0, 50, 50, hwnd, NULL, hInst, NULL );
	if( !hAnimWnd )
    {
        MessageBox( hwnd, "Could not create animation window. Try restarting your computer.", "Error", MB_OK );
        goto HANDLE_ERROR;
    }

	hAnimDC = GetDC( hAnimWnd );

	if( !hAnimDC )
    {
        MessageBox( hwnd, "Could not create animation DC. Try restarting your computer.", "Error", MB_OK );
        goto HANDLE_ERROR;
    }

	
    // Let the thread begin. Create the thread
    tData.hThread = CreateThread( NULL, 0, ActiveThread, &tData, 0, &tData.threadID );

    if( tData.hThread )
    {
        // Set the thread to the lowest priority level
        SetThreadPriority( tData.hThread, THREAD_PRIORITY_LOWEST );
    }
    else
    {
        MessageBox( hwnd, "Could not create animation thread. Try restarting your computer.", "Error", MB_OK );
        goto HANDLE_ERROR;
    }

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // Starter_OnCreate


/************************************************************************
*    FUNCTION NAME:         SetUpTrayApp                                                             
*
*    DESCRIPTION:           Setup the tray app icon.
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CAnimDeskWnd::SetUpTrayApp( HWND hwnd )
{
    // Get the handle to our menu
    hmenu = LoadMenu( hInst, "Menu_1" );

    // Make sure we have a menu
    if( !hmenu )
        return FALSE;

    // Now get the sub menu
    hSubMenu = GetSubMenu( hmenu, 0 );

    // Make sure we have a sub menu
    if( !hSubMenu )
        return FALSE;

    // Fill out the icon data
    notIconData.cbSize = sizeof( NOTIFYICONDATA );
    notIconData.hWnd   = hwnd;
    notIconData.uID    = TRAY_ICON_ID;
    notIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    notIconData.uCallbackMessage = WM_ICON_NOT;
    notIconData.hIcon = LoadIcon( hInst, "AppIcon" );
    strcpy( notIconData.szTip, PRODUCT_NAME );

    // Add the icon to the tray app
    if( !Shell_NotifyIcon( NIM_ADD, &notIconData ) )
        return FALSE;
    
    return TRUE;

}   // SetUpTrayApp


/************************************************************************
*    FUNCTION NAME:         ActiveThread                                                             
*
*    DESCRIPTION:           Running thread for animation
*
*    FUNCTION PARAMETERS:
*    Input:    void *ssData  - Pointer to the screen saver data 
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

unsigned long _stdcall ActiveThread( void *ptData )
{
    PSThreadData pData = PSThreadData( ptData );
    DWORD animTimeOut, animTimer;

    // Init the cycle time out
    animTimeOut = timeGetTime() + pData->animTimeOut;

    // Cycle loop
    while( pData->active )
    {
        // Is it time to run the animation?
        if( animTimeOut < timeGetTime() )
        {
            // Setup the next animation
			animDeskWnd.SetupAnimation( pData );
            
            // Setup the timer for the animation cycle
            animTimer = timeGetTime() + pData->animCycleTimeOut;
            
            // Internal animation loop
            while( pData->active )
            {
                // Get the current time
                if( animTimer < timeGetTime() )
                {
                    // Animate until we return false
                    if( !animDeskWnd.Animate( pData ) )
                        break;

                    // Reinit the timer for the next animation cycle
                    animTimer = timeGetTime() + pData->animCycleTimeOut;
                }

                // Give up our time slice to other processes
                Sleep(0);
            }

            // Reinit the timer
            animTimeOut = timeGetTime() + pData->animTimeOut;
        }

        // Do a good amount of sleeping while we are
        // waiting to run the next animation
        Sleep( 50 );
    }

    return 0;
}   // ActiveThread


/************************************************************************
*    FUNCTION NAME:         SetupAnimation                                                             
*
*    DESCRIPTION:           Setup the animation
*
*    FUNCTION PARAMETERS:
*    Input:    void *ssData  - Pointer to the screen saver data 
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

void CAnimDeskWnd::SetupAnimation( PSThreadData pData )
{
	// Setup the animation
	pData->Dest.top = rand() % (GetScreenHeight() - pData->Source.bottom);
	pData->Dest.left = -pData->Source.right;

}	// SetupAnimation


/************************************************************************
*    FUNCTION NAME:         Animate                                                             
*
*    DESCRIPTION:           Run the animation
*
*    FUNCTION PARAMETERS:
*    Input:    void *ssData  - Pointer to the screen saver data 
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

BOOL CAnimDeskWnd::Animate( PSThreadData pData )
{
	RECT rect, source, dest;
	HDC ScreenDC;

	ScreenDC = GetDC(0);

    // inc the animation frame
    pData->FrameCounter = ( pData->FrameCounter + 1 ) % pData->NumFrames;

	// Clean the dirty buffer
	dest = pData->Source;
	source = pData->Source;
	source.right -= 3;
	source.left += 3;
	dest.right -= 3;
	pData->clean.Draw( pData->clean.Handle(), &dest, &source );
	dest.right = 3;
	dest.left = pData->Source.right - 3;
	source = pData->Dest;
	source.left += source.right;
	source.right = 3;
	
	BitBlt( pData->clean.Handle(), dest.left, dest.top, dest.right, dest.bottom, ScreenDC, source.left, source.top, SRCCOPY );
	
	pData->clean.Draw( pData->dirty.Handle() );

	pData->Dest.left += 3;

    pData->art.SetCopyMode( SRCAND );
    rect = pData->Source;
    rect.top += rect.bottom;
    source = pData->Source;
    source.left += source.right * pData->FrameCounter;
    rect.left += rect.right * pData->FrameCounter;
    pData->art.Draw( &pData->dirty, &pData->Source, &rect );
    pData->art.SetCopyMode( SRCPAINT );
    pData->art.Draw( &pData->dirty, &pData->Source, &source );

	//pData->dirty.Draw( animDeskWnd.hAnimDC );

	//MoveWindow( animDeskWnd.hAnimWnd, pData->Dest.left, pData->Dest.top, pData->Dest.right, pData->Dest.bottom, FALSE );
	//SetWindowPos( animDeskWnd.hAnimWnd, HWND_TOPMOST, pData->Dest.left, pData->Dest.top, pData->Dest.right, pData->Dest.bottom, SWP_FRAMECHANGED );

	//pData->dirty.Draw( animDeskWnd.hAnimDC );
	//pData->dirty.Draw( ScreenDC, pData->Dest.left, pData->Dest.top );

	ReleaseDC( 0, ScreenDC );

	if( animDeskWnd.SelfErase )
	{
		MessageBeep(0);
		if( animDeskWnd.SelfEraceCounter > pData->Source.right )
		{
			animDeskWnd.SelfErase = FALSE;
			animDeskWnd.SelfEraceCounter = 0;
		}
		else
			animDeskWnd.SelfEraceCounter += 3;
		
		pData->dirty.Draw( animDeskWnd.hAnimDC );
		MoveWindow( animDeskWnd.hAnimWnd, pData->Dest.left, pData->Dest.top, pData->Dest.right, pData->Dest.bottom, TRUE );
	}
	else
	{
		MoveWindow( animDeskWnd.hAnimWnd, pData->Dest.left, pData->Dest.top, pData->Dest.right, pData->Dest.bottom, FALSE );
		pData->dirty.Draw( animDeskWnd.hAnimDC );
	}
	
	if( pData->Dest.left >= GetScreenWidth() )
		return FALSE;
	else
		return TRUE;

}	// Animate