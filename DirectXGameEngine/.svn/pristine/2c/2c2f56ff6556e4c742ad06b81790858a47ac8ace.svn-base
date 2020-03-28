
/************************************************************************
*    FILE NAME:   ssindow.cpp
*
*    DESCRIPTION: For creating and managing a screen saver Window singlton 
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

// Physical component dependency
#include "sswindow.h"

// Game lib dependencies
#include "2d\\point2d.h"

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc: Constructer                                                             
************************************************************************/
CSSWindow::CSSWindow()
         : screenSaverMode(ESSM_NULL),
		   hWnd(NULL),
		   hInst(NULL),
		   className("saver_window"),
		   windowName("no name"),
		   multipleMonitors(false),
		   debugMode(true),
		   fullScreenDebugWnd(false),
		   debugWndScaler(.25f),
		   MOUSE_TICKS_TO_IGNOR(5),
		   mousePrevX(0),
		   mousePrevY(0),
		   mouseTicks(MOUSE_TICKS_TO_IGNOR),
		   timerReturnCode(0),
		   MOUSE_RESET_TIMER_ID(3),
		   saverRunning(false),
		   MINIMUM_MOUSE_MOVE(5)
{
}	// Constructer


/***************************************************************************
*   desc:  Create the parent Window
*
*   param:  HWND hwnd - Handle of the window receiving the message
*   param:  cmdParam - windows screen saver command parameter
*   param:  pWinCallback - Windows call back function pointer
*   param:  pTimerCallBack - Windows timer call back function pointer
*
*   ret:  bool - true on success or false on fail
 ****************************************************************************/
bool CSSWindow::CreateParentWnd( HINSTANCE hInstance,
							     string & cmdParam,
							     winCallbackFuncPtrType pWinCallback,
								 timerCallbackFuncPtrType pTimerCallBack )
{
	hInst = hInstance;

	// Check for full screen saver
	if( (cmdParam.find("S") != string::npos) || (cmdParam.find("s") != string::npos) ||
		debugMode )
	{
		if( CreateFullScreenSaver( pWinCallback, pTimerCallBack ) )
			return true;
	}

	// Check for preview window
	else if( (cmdParam.find("P") != string::npos) || (cmdParam.find("p") != string::npos) ||
		     (cmdParam.find("L") != string::npos) || (cmdParam.find("l") != string::npos) )
	{
		if( CreatePreviewWnd( cmdParam, pWinCallback ) )
			return true;
	}

	// Check for config file or run it if nothing else presents itself
	else if( (cmdParam.find("C") != string::npos) || (cmdParam.find("c") != string::npos) ||
		     (screenSaverMode == ESSM_NULL) )
	{
		// Once this function returns, the program quits.
		CreateConfigWnd( cmdParam );
	}

	return false;

}	// CreateParentWnd


/***************************************************************************
*   desc:  Create the full screen saver window
*
*   param:  pWinCallback - Windows call back function pointer
*   param:  pTimerCallBack - Windows timer call back function pointer
*
*   ret:  bool - true on success or false on fail
 ****************************************************************************/
bool CSSWindow::CreateFullScreenSaver( winCallbackFuncPtrType pWinCallback,
									   timerCallbackFuncPtrType pTimerCallBack )
{
	// Set the saver mode
	screenSaverMode = ESSM_SAVER;

	// If this is already running, exit
	if( DoExist( string("howiesfunwarescreensaverfull") ) )
		return false;

	// Register the window the screen saver will animate in
	if( !RegisterWnd( pWinCallback ) )
		return false;

	// Create the screen saver window
	if( !CreateWnd() )
		return false;

	// Create a timer to reset the mouseTicks variable
    timerReturnCode = SetTimer( hWnd, MOUSE_RESET_TIMER_ID, 5000, (TIMERPROC)pTimerCallBack );

	// Did we get a timer? We can live without one but add a little more to the MouseTicks variable
    if( timerReturnCode == 0 )
        mouseTicks += MOUSE_TICKS_TO_IGNOR;

	// Hide the mouse
    CursorShow( false );

    // Set the window to the forground
    SetForegroundWindow( hWnd );

	return true;

}	// CreateFullScreenSaver


/***************************************************************************
*   desc:  Create the preview Window
*
*   param:  cmdParam - windows screen saver command parameter
*   param:  pWinCallback - Windows call back function pointer
*
*   ret:  bool - true on success or false on fail
 ****************************************************************************/
bool CSSWindow::CreatePreviewWnd( string & cmdParam,
								  winCallbackFuncPtrType pWinCallback )
{
	// Set the saver mode
	screenSaverMode = ESSM_PREV;

	// If this is already running, exit
	if( DoExist( string("howiesfunwarescreensaverpreview") ) )
		return false;

	// Find the position of the handle identifier
	size_t found = cmdParam.find(":");
	if( found == string::npos )
		return false;

	// Turn string into a window handle
    HWND hPreviewParentWnd = reinterpret_cast<HWND>((atoi( cmdParam.substr(found + 1).c_str() )));

	// Register the window the screen saver will animate in
	if( !RegisterWnd( pWinCallback ) )
		return false;

	// Create the preview window
	if( !CreateWnd( hPreviewParentWnd ) )
		return false;

	return true;

}	// CreatePreviewWnd


/***************************************************************************
*   desc:  Create the config Window
*
*   param:  cmdParam - windows screen saver command parameter
*
*   ret:  bool - true on success or false on fail
 ****************************************************************************/
void CSSWindow::CreateConfigWnd( string & cmdParam )
{
	// Set the saver mode
	screenSaverMode = ESSM_CFG;

	// If this is already running, exit
	if( DoExist( string("howiesfunwarescreensaverconfig") ) )
		return;

	HWND hConfigParentWnd = NULL;

	// Find the position of the handle identifier
	size_t found = cmdParam.find(":");
	if( found != string::npos )
	{
		// Turn string into a window handle
		HWND hConfigParentWnd = reinterpret_cast<HWND>((atoi( cmdParam.substr(found + 1).c_str() )));
	}

	if( hConfigParentWnd == NULL )
		hConfigParentWnd = GetForegroundWindow();

	// Once this function returns, the program ends.
	DisplayConfigDialogBox( hInst, hConfigParentWnd );

}	// CreateConfigWnd


/***************************************************************************
*   desc:  Standard Windows OnCreate message handler
*
*   param:  HWND hwnd - Handle of the window receiving the message
*   param:  lpCreateStruct  - Structure used in window creation
*
*   ret:  bool - true on success or false on fail
 ****************************************************************************/
bool CSSWindow::OnCreate( HWND hwnd, LPCREATESTRUCT lpCreateStruct )
{
	if( screenSaverMode == ESSM_SAVER )
	{
		// Use PeekMessage to remove all the mouse messages from the Que
		MSG msg;
        PeekMessage( &msg, hwnd, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE );

        // no alt-tab or ctrl-alt-del any more
        SystemParametersInfo( SPI_SCREENSAVERRUNNING, true, NULL, 0 );
	}

	// Load screen saver resources here
	return LoadSaverObjects();

}	// OnCreate


/***************************************************************************
*   desc:  Standard Windows OnClose message handler
*
*   param:  HWND hwnd - Handle of the window receiving the message
 ****************************************************************************/
void CSSWindow::OnClose( HWND hwnd )
{
	if( screenSaverMode == ESSM_SAVER )
	{
		// enable every system key; alt-tab or ctrl-alt-del
        SystemParametersInfo( SPI_SCREENSAVERRUNNING, false, NULL, 0 );

        // Double check that we did get a timer. Don't want to kill a timer we didn't have
        // because sometimes windows runs out of timers and we can live without one if we
        // didn't get one but be sure we had one before deleting it.
        if( timerReturnCode != 0 )
            // Free the mouse reset timer
            KillTimer( hwnd, MOUSE_RESET_TIMER_ID );
	}

	// Unload the allocated screen saver objects
	UnloadSaverObjects();

	// Destroy the window which will call the OnDestroy message handler that
    // will quit the program.
    if( !DestroyWindow( hwnd ) )
        // Was there a problem destroying the window? Force a quit.
        PostQuitMessage(0);

}	// OnClose


/***************************************************************************
*   desc:  Standard Windows OnMouseMove message handler
*
*   param:  HWND hwnd - Handle of the window receiving the message
*   param:  x - x mouse movement
*   param:  y - y mouse movement
*   param:  keyFlags - mouse flags
 ****************************************************************************/
void CSSWindow::OnMouseMove( HWND hwnd, int x, int y, UINT keyFlags )
{
	if( screenSaverMode == ESSM_SAVER )
	{
		if( (abs( mousePrevX - x ) > MINIMUM_MOUSE_MOVE) || 
            (abs( mousePrevY - y ) > MINIMUM_MOUSE_MOVE) )
		{
			if( mouseTicks > 0 )
				mouseTicks--;
			else
				CloseScreenSaver();
		}

		// Reset the mouse positions
		mousePrevX = x;
		mousePrevY = y;
	}

}	// OnMouseMove


/***************************************************************************
*   desc:  Handles mouse or button presses
*
*   param:  HWND hwnd - Handle of the window receiving the message
 ****************************************************************************/
void CSSWindow::OnButton( HWND hwnd )
{
	if( screenSaverMode == ESSM_SAVER )
		CloseScreenSaver();

}	// OnButton


/***************************************************************************
*   desc:  Time to close the screen saver
 ****************************************************************************/
void CSSWindow::CloseScreenSaver()
{
	// Don't let any more calls to this function get through while we are 
    // in the process of checking if we can close the screen saver.
    // We also wouldn't want anything to get through if we were in the
    // process of quitting the program and freeing up allocated memory
    if( IsSaverRunning() )
	{
		// Stop the render loop
		SetSaverRunning(false);

		// Show the cursor
		CursorShow(true);

		// Looks like we're closing it up now. Post the Close message.
        // The OnClose message handler is where we free up all memory and resources
        PostMessage( hWnd, WM_CLOSE, 0, 0 );
	}

}	// CloseScreenSaver


/***************************************************************************
*   desc:  Standard Windows OnDestroy message handler
*
*   param:  HWND hwnd - Handle of the window receiving the message
 ****************************************************************************/
void CSSWindow::OnDestroy( HWND hwnd )
{
	PostQuitMessage(0);

}	// OnDestroy


/***************************************************************************
*   desc:  See if a semaphore already exists
*
*   param:  semaphoreName - Name of semaphore
 ****************************************************************************/
bool CSSWindow::DoExist( string & semaphoreName )
{
    bool result = false;
    HANDLE hSemaphore;

    // Create a semaphore
    hSemaphore = CreateSemaphore( NULL, 0, 1, semaphoreName.c_str() );

    // Now, check to see if the semaphore exists
    if( hSemaphore != NULL && GetLastError() == ERROR_ALREADY_EXISTS )
    {
        CloseHandle( hSemaphore );
        result = true;
    }

    return result;

}   // DoExist


/***************************************************************************
*   desc:  All windows created with CreateWindow must be registered first.
*          This does not include windows controls like buttons, listboxex 
*          and such because they register themselves.
*
*   param:  pWinCallback - Windows call back function
*
*   ret:  bool - true on success or false on fail
****************************************************************************/
bool CSSWindow::RegisterWnd( winCallbackFuncPtrType pWinCallback )
{
    WNDCLASSEX w;

    w.cbSize = sizeof( WNDCLASSEX );
    w.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC|CS_BYTEALIGNCLIENT;
    w.lpfnWndProc = pWinCallback;
    w.cbClsExtra = 0;
    w.cbWndExtra = 0;
    w.hInstance = hInst;
    w.hIcon = NULL;
    w.hIconSm = NULL;
    w.hCursor = LoadCursor( NULL, IDC_ARROW );
    w.hbrBackground = NULL;
    w.lpszMenuName = NULL;
	w.lpszClassName = className.c_str();

    return ( RegisterClassEx( &w ) != 0 );

}   // RegisterWnd


/***************************************************************************
*   desc:  If we are running the preview or a full screen window, the 
*          window is created here.
*
*   param:  hPreviewWnd - Handle to preview window.
*
*   ret:  bool - true on success or false on fail
****************************************************************************/
bool CSSWindow::CreateWnd( HWND hPreviewWnd )
{
    DWORD style;
	DWORD exstyle;
    CPoint2D<int> point;

    // Create the window as a child of the preview window
	// The render window and display window will be different sizes
    if( screenSaverMode == ESSM_PREV )
    {
        // Get the parents window rect and our window rect will 
        // be the same size and position in that parent
		RECT rect;
        GetClientRect( hPreviewWnd, &rect );

        // Save the size of the small preview window
        displayWndSize.w = rect.right;
        displayWndSize.h = rect.bottom;

        // In preview mode, all we want is the screen size even if it is duel monitors
        renderWndSize.w = GetSystemMetrics( SM_CXSCREEN );
        renderWndSize.h = GetSystemMetrics( SM_CYSCREEN );

        // Define the style needed for a preview window
        style = WS_CHILD;
        exstyle = 0;
    }
    // Create the full screen saver window
    else if( screenSaverMode == ESSM_SAVER )
    {
        // Get the size of the screen. This is only the size of the 
        // first monitor if more then one monitor is present
        renderWndSize.w = GetSystemMetrics( SM_CXSCREEN );
        renderWndSize.h = GetSystemMetrics( SM_CYSCREEN );

        // Make the screen saver window fill the screen or the desktop if more then
        // one monitor is present.
        // If we have duel monitor support, get the virtual desktop size.
        // The calls in this if statement are not supported under all Windows OS's. So
        // if the SM_CMONITORS returns true, then the other calls in this if statement
        // are supported.
        if( GetSystemMetrics( SM_CMONITORS ) > 1 )
        {
            // The virtual desktop can be offsetted
            point.x = GetSystemMetrics( SM_XVIRTUALSCREEN );
            point.y = GetSystemMetrics( SM_YVIRTUALSCREEN );

            // Get the width and height of the desktop which
            // will be the combined size of the monitors on this computer
            renderWndSize.w = GetSystemMetrics( SM_CXVIRTUALSCREEN );
            renderWndSize.h = GetSystemMetrics( SM_CYVIRTUALSCREEN );

            // Set the duel monitor flag to true
            multipleMonitors = true;
        }

		// Display and render windows are the same size
		displayWndSize = renderWndSize;

        // This allows the screen saver to run in a small window
        // on the bottom right corner of your screen. Used for stepping through
        // the code while debugging the screen saver
		if( debugMode && !fullScreenDebugWnd )
		{
			displayWndSize = renderWndSize * debugWndScaler;
			point.x = renderWndSize.w - displayWndSize.w;
			point.y = renderWndSize.h - displayWndSize.h;
		}

        // Define the style needed for a screen saver window
        style = WS_POPUP;
        exstyle = WS_EX_TOPMOST;
    }

    // Create the screen saver window
    hWnd = CreateWindowEx( exstyle, className.c_str(), windowName.c_str(), style|WS_VISIBLE,
                           point.x, point.y, displayWndSize.w, displayWndSize.h, hPreviewWnd, NULL, hInst, NULL );

	return (hWnd != NULL);

}   // CreateWnd


/***************************************************************************
*   desc:  The ShowCursor API function increments or decrements the internal
*          display counter. The cursor is displayed only if the display count
*          is greater than or equal to 0. So here, we are counting up how many
*          times the cursor has to be hidden so that we can show it the same
*          number of times when it needs to be shown. This way the screen saver
*          will leave the internal display counter just as it found it.
*
*   param:  value - Show/Hide the windows cursor
****************************************************************************/
void CSSWindow::CursorShow( bool value )
{
	if( value )
    {
        // Return value has to be 0 or greater to show
        while( ShowCursor( value ) < 0 )
        {}
    }
	else
    {
        // Return value has to be -1 or less to hide
        while( ShowCursor( value ) > -1 )
        {}
    }
    
}   // ShowHideCursor

