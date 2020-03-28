
/************************************************************************
*
*    PRODUCT:         Screen saver Window Class
*
*    FILE NAME:       SSWnd.cpp
*
*    DESCRIPTION:     Screen saver window
*
*    IMPLEMENTATION:  Designed to be used as a screen saver window. New
*                     projects would use this like a library and not
*                     alter the code unless adding a new feature 
*                     ( that is NOT screen saver specific ) that
*                     all the screen savers would use or making fixes.
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include "GDefines.h"        // Header file with #defines, enums and structures
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "SSWnd.h"           // Header file for this *.cpp file.

#include "mintools.h"        // A misc group of useful functions
#include "SSConfig.h"        // The config dialog box
#include "ssAnim.h"          // Header file for desendant class of TScreenSaver
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.


// Globals
PCSSaverWnd pSSaverWnd;


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
    MSG msg;

    // Set wParam to 0 in case we show the config dialog box
    // or the password window because then we need to return 0.
    msg.wParam = 0;

    // Create the parent window object
    CSSaverWnd SSaverWnd;
    pSSaverWnd = &SSaverWnd;

    // Create the screen saver window
    if( SSaverWnd.CreateParentWnd( hInst, lpszCmdParam ) )
    {
        // Create a screen saver thread
        // The screen saver class is created from within the class
        if( SSaverWnd.CreateScreenSaverThread( (void *)SSaverWnd.GetSSDataPtr() ) )
        {
            // Standard windows message loop
            while( GetMessage( &msg, NULL, 0, 0 ) )
            {
                // Change the format of certain messages
                TranslateMessage( &msg );
                // Pass the message to the WinProc for processing
                DispatchMessage( &msg );
            }
        }
    }

    return msg.wParam;

} // WINAPI WinMain


/************************************************************************
*    FUNCTION NAME:         ScreenSaverThread                                                             
*
*    DESCRIPTION:           This is the thread that animates the screen
*                           saver. The thread function is designed to be
*                           very general. Screen saver specific code should
*                           not be placed in here.
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

unsigned long _stdcall ScreenSaverThread( void *pSSaver )
{
    // The screen saver is created here.
    // Init the screen saver class with the Screen Saver Data structure.
    CAnimateSaver screenSaver( (PTSaverModeData)pSSaver );

    // Load the screen saver. Was the load successful and
    // is the screen saver ready to run
    if( screenSaver.Load() )
    {
        // First time display of the buffer to start the screen saver.
        // This is an empty virtual function
        if( screenSaver.SaverActive() )
            screenSaver.DisplayBufferOnStartUp();

        // Screen saver execution loop
        while( screenSaver.SaverActive() )
        {
            // run the animation
            screenSaver.AnimateSaver();
        }
    }

    return 0;

}   // ScreenSaverThread


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

LRESULT CALLBACK SaverWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch( Message )
    {
        HANDLE_MSG( hwnd, WM_CREATE,    pSSaverWnd->OnCreate );
        HANDLE_MSG( hwnd, WM_MOUSEMOVE, pSSaverWnd->OnMouseMove );
        HANDLE_MSG( hwnd, WM_CLOSE,     pSSaverWnd->OnClose );
        HANDLE_MSG( hwnd, WM_DESTROY,   pSSaverWnd->OnDestroy );
        HANDLE_MSG( hwnd, WM_NCPAINT,   pSSaverWnd->OnNCPaint );
        HANDLE_MSG( hwnd, WM_SHOWWINDOW, pSSaverWnd->OnShowWindow );
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_KEYDOWN:      
                                        pSSaverWnd->OnButton( hwnd );
        break;

        // Trap message so that screen saver doesn't start up more than once
        case WM_SYSCOMMAND:
            if( wParam == SC_SCREENSAVE || wParam==SC_CLOSE )
                return FALSE;

            // Check if the monitor is powering off
            if( wParam == SC_MONITORPOWER )
                pSSaverWnd->PutScreenSaverToSleep();
        break;

        // Don't allow the screen to paint itself. We control all painting
        case WM_ERASEBKGND:
            return FALSE;
    }

    return DefWindowProc( hwnd, Message, wParam, lParam );

} // SaverWndProc


/************************************************************************
*    FUNCTION NAME:         PreviewWndProc                                                             
*
*    DESCRIPTION:           Standard Windows message handler. I added another
*                           message handler just for the preview window
*                           to help keep things simple. The main windows
*                           message handler requires much more processing
*                           and resources and I would have to make too many
*                           exceptions in the code to handle both cases.
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

LRESULT CALLBACK PreviewWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
   switch( Message )
   {
        HANDLE_MSG( hwnd, WM_CREATE,    pSSaverWnd->OnCreate );
        HANDLE_MSG( hwnd, WM_CLOSE,     pSSaverWnd->OnClose );
        HANDLE_MSG( hwnd, WM_DESTROY,   pSSaverWnd->OnDestroy );

        // Trap message so that screen saver doesn't start up more than once
        // This is not fail safe which is why I also use the DoExist( SEMAPHORE ) function
        case WM_SYSCOMMAND:
            if ( wParam == SC_SCREENSAVE || wParam==SC_CLOSE )
                return FALSE;
            break;

        // Don't allow the screen to paint itself. We control all painting
        case WM_ERASEBKGND:
            return FALSE;
   }

   return DefWindowProc( hwnd, Message, wParam, lParam );

} // PreviewWndProc


/************************************************************************
*    FUNCTION NAME:         MouseResetTimer                                                             
*
*    DESCRIPTION:           Callback timer function that resets the mouse
*                           ticks.
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

void CALLBACK MouseResetTimer( HWND hwnd, UINT uTimerMsg, UINT uTimerID, DWORD dwTime )
{
    // Use the on off test to see if your screen saver is returning the resources
    // it has allocated. By setting the Windows screen saver delay to 1 minute,
    // it will cause the screen saver to run and stop over and over and it doesn't
    // even wait for a minute to go by before restarting. It's about 5 seconds between
    // starts and stops
    #ifdef ON_OFF_TEST
    pSSaverWnd->CloseIfOk( hwnd );
    #endif

    // Reset the nimber of mouse ticks to ignore
    pSSaverWnd->SetMouseTicks( TICKS_TO_IGNORE );

}   // MouseResetTimer


/************************************************************************
*
*                      Object Related Functions
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

CSSaverWnd::CSSaverWnd()
{
    // Init structure data
    memset( &ssData, 0, sizeof( TSaverModeData ) );

    // Init class data
    PreviousX = 0;
    PreviousY = 0;
    MouseTicks = TICKS_TO_IGNORE;
    CheckingForClose = FALSE;
    TimerReturnCode = 0;

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

CSSaverWnd::~CSSaverWnd()
{

}   // Destructer


/************************************************************************
*    FUNCTION NAME:         CreateScreenSaverThread                                                             
*
*    DESCRIPTION:           Create the thread to run the screen saver
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

BOOL CSSaverWnd::CreateScreenSaverThread( void *pScreenSaver )
{
    BOOL result = FALSE;

    // Let the screen saver begin. Create the thread
    ssData.hSSThread = CreateThread( NULL, 0, ScreenSaverThread, 
                                     pScreenSaver, 0, &ssData.ssThreadID );

    // Do we have a thread?
    if( ssData.hSSThread != NULL )
    {
        // Set the flag
        result = TRUE;
        
        // Set the thread to the lowest priority level for the small preview window
        if( ssData.SaverMode == ssPreview )            
            SetThreadPriority( ssData.hSSThread, THREAD_PRIORITY_LOWEST );
    }

    return result;

}   // CreateScreenSaverThread


/************************************************************************
*    FUNCTION NAME:         CreateParentWnd()                                                             
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

BOOL CSSaverWnd::CreateParentWnd( HINSTANCE hInst, LPSTR lpszCmdParam )
{
    HWND hWnd, hSaverWnd;
    BOOL result = FALSE;
    
    // Parse the command line to see what we need to do
    // Getting the command line from lpszCmdParam means that it
    // has the path to the app already parsed out
    // If this is the ssConfig, we're getting the window handle
    // from the command line to make the config dialog box a child
    // of the Display Properties box 
    ssData.SaverMode = ParseCommandLine( lpszCmdParam, &hWnd );

    // If we are debugging, force it to run in full screen saver mode
    // because it will be resized down to a small window so that we can
    // see what it is doing while we step through the code
    #ifdef DEBUG
        ssData.SaverMode = ssSaver;
    #endif

    // Init Saver data structure
    ssData.hInst = hInst;
    ssData.Active = TRUE;
    
    // How are we running?
    if( ssData.SaverMode == ssConfig )
        // We got the window handle form the command line   
        DisplayConfigDialogBox( hInst, hWnd );

    else if( ssData.SaverMode == ssPassword )
        RunPasswordMode( hWnd );
    
    else if( ssData.SaverMode == ssPreview || ssData.SaverMode == ssSaver )
    {
        // Indicates the screen saver should run in a window
        result = TRUE;

        // Check to see if we are already running. If we are, kill it. That last
        // thing you need is a screen saver keeps running instances of itself
        // until the computer runs out of system resources and crashes.
        if( ssData.SaverMode == ssSaver )
            if( DoExist( SEMAPHORE ) )
                return FALSE;
        
        // Register the window the screen saver will animate in
        if( !RegisterWnd( hInst, ssData.SaverMode ) )
            return FALSE;
        
        // Create the window the screen saver will animate in
        hSaverWnd = CreateWnd( hInst, ssData.SaverMode, hWnd );
        if( hSaverWnd == NULL )
            return FALSE;

        if( result )
        {
            if( ssData.SaverMode == ssSaver )
            {
                // Hide the mouse
                ShowHideCursor( HIDE_CURSOR );

                // Set the window to the forground
                SetForegroundWindow( hSaverWnd );
            }
        }
    }

    return result;

}   // CreateParentWnd


/************************************************************************
*    FUNCTION NAME:         RegisterWnd                                                             
*
*    DESCRIPTION:           All windows created with CreateWindow
*                           must be registered first.
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

BOOL CSSaverWnd::RegisterWnd( HINSTANCE hInst, int aSaverMode )
{
    WNDCLASSEX w;
    
    // Set for full screen saver or for preview
    if( aSaverMode == ssSaver )
        w.lpfnWndProc = SaverWndProc;
    else
        w.lpfnWndProc = PreviewWndProc;

    w.cbSize        = sizeof( WNDCLASSEX );
    w.style         = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    w.cbClsExtra    = 0;
    w.cbWndExtra    = 0;
    w.hInstance     = hInst;
    w.hIcon         = NULL;
    w.hIconSm       = NULL;
    w.hCursor       = LoadCursor( NULL, IDC_ARROW );
    w.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
    w.lpszMenuName  = NULL;
    w.lpszClassName = WINDOW_CLASS;

    return ( RegisterClassEx( &w ) != 0 );

} // RegisterWnd


/************************************************************************
*    FUNCTION NAME:         CreateWnd                                                             
*
*    DESCRIPTION:           If we are running the preview or a full screen
*                           window, the window is created here.
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              int aSaverMode  - The screen saver mode which would be
*                                preview or full screen
*              HWND aPreview   - A handle to the preview window which you
*                                inturn create a child window of the same
*                                size. Full screen doesn't require a handle.
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

HWND CSSaverWnd::CreateWnd( HINSTANCE hInst, int aSaverMode, HWND aPreview )
{
    RECT rect;
    DWORD style, exstyle;
    int Width, Height, x = 0, y = 0;

    // Create the window as a child of the preview window
    if( aSaverMode == ssPreview )
    {
        // Get the parents window rect and our window rect will 
        // be the same size and position in that parent
        GetClientRect( aPreview, &rect );

        // Save the size of the small preview window
        Width = rect.right;
        Height = rect.bottom;

        // In preview mode, all we want is the screen size even if it is duel monitors
        ssData.desktopWidth = GetScreenWidth();
        ssData.desktopHeight = GetScreenHeight();
        ssData.screenWidth = ssData.desktopWidth;
        ssData.screenHeight = ssData.desktopHeight;

        // Define the style needed for a preview window
        style = WS_CHILD;
        exstyle = 0;
    }
    // Create the full screen saver window
    // aPreview will be NULL
    else if( aSaverMode == ssSaver )
    {
        // Get the size of the screen. This is only the size of the 
        // first monitor if more then monitor is present
        ssData.screenWidth = GetScreenWidth();
        ssData.screenHeight = GetScreenHeight();

        // Make the screen saver window fill the screen or the desktop if more then
        // one monitor is present.
        // If we have duel monitor support, get the virtual desktop size.
        // The calls in this if statement are not supported under all Windows OS's. So
        // if the SM_CMONITORS returns true, then the other calls in this if statement
        // are supported.
        if( GetSystemMetrics( SM_CMONITORS ) > 1 )
        {
            // The virtual desktop can be offsetted
            x = GetSystemMetrics( SM_XVIRTUALSCREEN );
            y = GetSystemMetrics( SM_YVIRTUALSCREEN );

            // Get the width and height of the desktop which
            // will be the combined size of the monitors on this computer
            Width = GetDesktopWidth();
            Height = GetDesktopHeight();
            ssData.desktopWidth = Width;
            ssData.desktopHeight = Height;

            // Set the duel monitor flag to true
            ssData.duelMonitor = TRUE;
        }
        else
        {
            // The desktop and screen size are the same size
            // in this situation.
            Width = ssData.screenWidth;
            Height = ssData.screenHeight;
            ssData.desktopWidth = Width;
            ssData.desktopHeight = Height;
        }

        // This allows the screen saver to run in a small window
        // on the bottom right corner of your screen. Used for stepping through
        // the code while debugging the screen saver
        #if defined( DEBUG ) && !defined( FULL_SCREEN_DEBUG )
        x = Width - DEBUG_WIDTH - DEBUG_OFFSET;
        y = Height - DEBUG_HEIGHT - DEBUG_OFFSET;
        Width = DEBUG_WIDTH;
        Height = DEBUG_HEIGHT;
        #endif

        // Define the style needed for a screen saver window
        style = WS_POPUP;
        exstyle = WS_EX_TOPMOST;

        // This is already null because there is no parent with the full screen saver mode.
        // I'm setting it to NULL here for readability.
        aPreview = NULL;
    }

    // Init Saver data structure
    ssData.windowWidth = Width;
    ssData.windowHeight = Height;

    // Create the screen saver window
    return CreateWindowEx( exstyle, WINDOW_CLASS, WINDOW_TITLE, style|WS_VISIBLE,
                           x, y, Width, Height, aPreview, NULL, hInst, NULL );

} // CreateWnd


/************************************************************************
*    FUNCTION NAME:         OnCreate()                                                             
*
*    DESCRIPTION:           Standard Windows OnCreate message handler. Here
*                           we are creating a windows call back function
*                           timer to reset mouse tick count variable.
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CSSaverWnd::OnCreate( HWND hwnd, LPCREATESTRUCT lpCreateStruct )
{
    BOOL Result = TRUE;
    MSG Msg;

    // Init Saver data structure
    ssData.hwnd = hwnd;

    if( ssData.SaverMode == ssSaver )
    {
        // Use PeekMessage to remove all the mouse messages from the Que
        PeekMessage( &Msg, hwnd, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE );

        // no alt-tab or ctrl-alt-del any more
        SystemParametersInfo( SPI_SCREENSAVERRUNNING, TRUE, NULL, 0 );

        #ifdef ON_OFF_TEST
            // Use a faster interval for the on off test
            TimerReturnCode = (int)SetTimer( hwnd, MOUSE_RESET_TIMER, 1500L, (TIMERPROC)MouseResetTimer );
        #else
            // Resets the mouseTicks variable
            TimerReturnCode = (int)SetTimer( hwnd, MOUSE_RESET_TIMER, FIVE_SECONDS, (TIMERPROC)MouseResetTimer );
        #endif

        // Did we get a timer? We can live without one but add a little more to the MouseTicks variable
        if( TimerReturnCode == 0 )
            MouseTicks += TICKS_TO_IGNORE;
    }

    return Result;

}   // OnCreate


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

void CSSaverWnd::OnShowWindow( HWND hwnd, BOOL fShow, UINT status )
{

}   // OnShowWindow


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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CSSaverWnd::OnDestroy( HWND hwnd )
{         
    // Quit the program. Notice, this is sending a Windows message and returns
    // right away. The program still continues after this point and is just sitting
    // around waiting for windows to give it the kiss of death.
    PostQuitMessage(0);

} // OnDestroy


/************************************************************************
*    FUNCTION NAME:         OnClose                                                             
*
*    DESCRIPTION:           Standard Windows OnClose message handler.
*                           Free your memory and other clean up here
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

void CSSaverWnd::OnClose( HWND hwnd )
{
    DWORD ThreadExitCode = 0;
    DWORD FalseTimer;
    DWORD ThreadWaitTimeAmount = TWENTY_SECONDS;

    // Setup a timer to make sure we are never stuck in a loop
    FalseTimer = timeGetTime() + TWO_SECONDS;

    // Kill the screen saver by causeing the thread to fall through.
    // Letting a thread return on it's own is the best way of terminating a thread.
    // Since two processes share this data, make real sure we set it to false.
    // and just to be safe, set a timer so that it will fall out if there is a major problem
    while( ssData.Active && FalseTimer > timeGetTime() )
        ssData.Active = FALSE;

    // Reset the false timer to wait for the loading to finish
    FalseTimer = timeGetTime() + TEN_SECONDS;

    // If we are trying to quit but the screen saver is still loading, allow
    // for more time to wait for the thread to return
    while( ssData.Loading && FalseTimer > timeGetTime() )
    {}

    // Give them the ability to ctrl-alt-del and kill the timer if this is the full screen saver 
    if( ssData.SaverMode == ssSaver )
    {
        // enable every Win95 system key; alt-tab or ctrl-alt-del
        SystemParametersInfo( SPI_SCREENSAVERRUNNING, FALSE, NULL, 0 );

        // Double check that we did get a timer. Don't want to kill a timer we didn't have
        // because sometimes windows runs out of timers and we can live without one if we
        // didn't get one but be sure we had one before deleting it.
        if( TimerReturnCode != 0 )
            // Free the mouse reset timer
            KillTimer( hwnd, MOUSE_RESET_TIMER );
    }

    // Get the exit code for the thread to see if it is still active.
    if( ssData.hSSThread )
        GetExitCodeThread( ssData.hSSThread, &ThreadExitCode );

    // Is the thread still active?
    if( ThreadExitCode == STILL_ACTIVE )
    {
        // Wait for the thread to finish freeing up it's resources before
        // exiting the screen saver. It's a good idea to timeout anytime we are waiting for.
        if( WaitForSingleObject( ssData.hSSThread, ThreadWaitTimeAmount ) == WAIT_TIMEOUT )
        {
            MessageBox( hwnd, "Screen saver thread time out error. Attempting to terminate thread.",
                        "Error", MB_ICONINFORMATION );

            // Lets try one more time to see if it is still active before we kill it the hard way.
            GetExitCodeThread( ssData.hSSThread, &ThreadExitCode );

            if( ThreadExitCode == STILL_ACTIVE )
            {
                // Try to terminate the thread. Use force if you have to.
                if( !TerminateThread( ssData.hSSThread, FALSE ) )
                    // Fire in the hole! Woman and children first!
                    MessageBox( hwnd, "Unable to terminate the thread. It is advised that you restart your computer.",
                            "Error", MB_ICONINFORMATION );
            }

            // Free the thread handle
            CloseHandle( ssData.hSSThread );

        }
        else
            // Free the thread handle
            CloseHandle( ssData.hSSThread );
    }
    else 
    {
        // Free the thread handle
        if( ssData.hSSThread )
            CloseHandle( ssData.hSSThread );
    }

    // If the screen resolution has been resized by the screen saver, reset it back.
    // XP doesn't like it if you try to do this in the thread.
    if( ssData.ScreenResResized )
        ChangeDisplaySettings( NULL, 0 );

    // Destroy the window which will call the OnDestroy message handler that
    // will quit the program.
    if( !DestroyWindow( hwnd ) )
    {
        // Was there a problem destroying the window? Force a quit.
        PostQuitMessage(0);
    }

} // OnClose


/************************************************************************
*    FUNCTION NAME:         OnButton                                                             
*
*    DESCRIPTION:           Stop the screen saver if someone has pressed
*                           a key or clicked the mouse button.
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

void CSSaverWnd::OnButton( HWND hwnd )
{
    #ifndef DEBUG
    CloseIfOk( hwnd );
    #endif

} // OnButton


/************************************************************************
*    FUNCTION NAME:         OnNCPaint                                                             
*
*    DESCRIPTION:           Standard Windows OnPaint message handler.
*                           If a passward dialog box shows over the screen,
*                           use the onPaint message to capture it's rect so that
*                           you can paint that rect.
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

void CSSaverWnd::OnNCPaint( HWND hwnd, HRGN hrgn )
{
    // Only if we have moved the mouse to cause the 
    // password dialog box to show
    if( CheckingForClose )
        GetRgnBox( hrgn, &ssData.PassRect );

}  // OnNCPaint


/************************************************************************
*    FUNCTION NAME:         OnMouseMove                                                             
*
*    DESCRIPTION:           Here we trap the mouse move events. We need to
*                           be very careful because some mouse drivers will
*                           send a ton of mouse move messages and will cause
*                           the screen saver to die before it has even
*                           started. That is why we are checking to see if
*                           the X, Y's have really changed because if they
*                           are not changing, the mouse is not really moving.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message 
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

void CSSaverWnd::OnMouseMove( HWND hwnd, int x, int y, UINT keyFlags )
{   
    if( abs( PreviousX-x ) > MINIMUM_MOUSE_MOVE || 
        abs( PreviousY-y ) > MINIMUM_MOUSE_MOVE )
        MouseEvent( hwnd );

    // See if we need to waik the sleeping monitor but only if the mouse did move
    if( abs( PreviousX-x ) || abs( PreviousY-y ) )
        WakeSleepingScreenSaver();

    PreviousX = x;
    PreviousY = y;
     
} // OnMouseMove


/************************************************************************
*    FUNCTION NAME:         MouseEvent                                                             
*
*    DESCRIPTION:           Every time the mouse moves, the mouse tick gets
*                           decreased. This is so that the screen saver
*                           doesn't stop because someone bumped their desk.
*                           When the count down reaches zero, the screen
*                           saver is ended.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message 
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

void CSSaverWnd::MouseEvent( HWND hwnd )
{
    if( MouseTicks > 0 )
        MouseTicks--;
    else
        CloseIfOk( hwnd );

} // MouseEvent


/************************************************************************
*    FUNCTION NAME:         CloseIfOk                                                             
*
*    DESCRIPTION:           Checks to see if a password is needed before
*                           quiting the application
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

void CSSaverWnd::CloseIfOk( HWND hwnd )
{
    MSG Msg;

    // Don't let any more calls to this function get through while we are 
    // in the process of checking if we can close the screen saver.
    // We also wouldn't want anything to get through if we were in the
    // process of quitting the program and freeing up allocated memory
    if( !CheckingForClose )
    {
        CheckingForClose = TRUE;

        // Show the cursor. We may have the password window up
        ShowHideCursor( SHOW_CURSOR );

        // Do we close the screen saver or keep it going
        // If the password option is being used, this shows the password dialog box
        if( VerifyPassword( hwnd ) )
        {
            // Looks like we're closing it up now. Post the Close message.
            // The OnClose message handler is where we free up all memory and resources
            // In preview mode, Windows calls the OnClose message handler for us
            PostMessage( hwnd, WM_CLOSE, 0, 0 );
        }
        else // Looks like we are not closing the screen saver
        {            
            ShowHideCursor( HIDE_CURSOR );

            // Use PeekMessage to remove all the mouse messages from the Que
            // If you don't, clicking on the cancel button sends a message to the
            // main window and password dialog box shows up again.
            PeekMessage( &Msg, hwnd, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE );

            // Set flag back to false and allow messages to come through again
            CheckingForClose = FALSE;
        }
    }
}   // CloseIfOk


/************************************************************************
*    FUNCTION NAME:         ParseCommandLine                                                             
*
*    DESCRIPTION:           Parses the command line to see what needs to 
*                           be done. Some command lines come with a handle
*                           to a window like the passward window and the
*                           preview window
*
*    FUNCTION PARAMETERS:
*    Input:    char *CmdLine  - Pre-parsed command line
*              HWND *hwnd     - Pointer to handle to be inited
*
*    Output:   int - The screen saver mode
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int CSSaverWnd::ParseCommandLine( char *CmdLine, HWND *hwnd )
{
    int SaverMode = ssNone;

    *hwnd = NULL;

    // Check if there is any spaces, just to be safe
    while( *CmdLine != 0 && *CmdLine == ' ' )
        CmdLine++;

    // If there's nothing here, just run the config window
    if( *CmdLine == 0 )
    {
        SaverMode = ssConfig;
    }
    else
    {
        // There is usually a '-' or a '/' before the code we need
        if( *CmdLine == '-' || *CmdLine == '/' )
            CmdLine++;

        // Do we want a preview window?
        if( *CmdLine == 'p' || *CmdLine == 'P' || *CmdLine == 'l' || *CmdLine == 'L' )
        {
            CmdLine++;
            while( *CmdLine ==' ' || *CmdLine == ':' )
                CmdLine++;

            // Turn string into a window handle
            *hwnd = (HWND)atoi( CmdLine );

            SaverMode = ssPreview;
        }
        // Do we want a full screen saver?
        else if( *CmdLine == 's' || *CmdLine == 'S')
            SaverMode = ssSaver;

        // Do we want a config dialog box?
        else if( *CmdLine == 'c' || *CmdLine == 'C')
        {
            CmdLine++;
            while( *CmdLine == ' ' || *CmdLine == ':' )
                CmdLine++;

            if( *CmdLine == 0 )
                *hwnd = GetForegroundWindow();
            else
                *hwnd = (HWND)atoi( CmdLine );

            SaverMode = ssConfig;
        }

        // Do we want a password window?
        else if( *CmdLine == 'a' || *CmdLine == 'A' )
        {
            CmdLine++;
            while( *CmdLine == ' ' || *CmdLine == ':' )
               CmdLine++;

            // Turn string into a window handle
            *hwnd = (HWND)atoi( CmdLine );
            SaverMode = ssPassword;
        }
    }

    return SaverMode;

}  // ParseCommandLine


/************************************************************************
*    FUNCTION NAME:         RunPasswordMode                                                             
*
*    DESCRIPTION:           Displays the change password dialog box so  
*                           the user can change their password.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd   - Handle to this programs window
*
*    Output:   none
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CSSaverWnd::RunPasswordMode( HWND hwnd )
{
    BOOL result = FALSE;
    PWDCHANGEPASSWORD PwdChangePassword;

    // This only ever gets called under '9x, when started with the /a option.
    // Try to load the library
    HINSTANCE hPasswrdChange = ::LoadLibrary("MPR.DLL");

    if( hPasswrdChange == NULL )
        goto HANDLE_ERROR;;  // Can't use a goto here.

    // Get the address to the function call
    PwdChangePassword = (PWDCHANGEPASSWORD)::GetProcAddress( hPasswrdChange, "PwdChangePasswordA" );

    // Do we have the function? Hope so. It would be nice.
    if( PwdChangePassword == NULL )
        goto HANDLE_ERROR;

    // Display the password dialog box for the user to changer in their password.
    PwdChangePassword( "SCRSAVE", hwnd, 0, 0 );

    // We're done. Free the library
    FreeLibrary( hPasswrdChange );

    result = TRUE;

    HANDLE_ERROR:;

    if( !result )
        MessageBox( NULL, "Unable to change password", "Error", MB_ICONINFORMATION );

    if( hPasswrdChange )
        FreeLibrary( hPasswrdChange );

}  // RunPasswordMode

 
/************************************************************************
*    FUNCTION NAME:         VerifyPassword                                                             
*
*    DESCRIPTION:           Displays displays the password box for the user 
*                           enter in their password. The screen saver will
*                           not quit if the users password is incorrect.
*
*                           Under NT, we return TRUE immediately. This lets
*                           the saver quit, and the system manages passwords.
*                           Under '95, we call VerifyScreenSavePwd. This 
*                           checks the appropriate registry key and, if 
*                           necessary, pops up a verify dialog
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd   - Handle to this programs window
*
*    Output:   BOOL        - Returns true if the password matchs or password
*                            protection is turned off
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CSSaverWnd::VerifyPassword( HWND hwnd )
{ 
    OSVERSIONINFO osv;
    HINSTANCE hPasswrdCheck = NULL;
    BOOL result = TRUE;
    VERIFYSCREENSAVEPWD VerifyScreenSavePwd;

    // Init the structure to check the password version
    osv.dwOSVersionInfoSize = sizeof( osv ); 
    GetVersionEx( &osv );

    // Check the OS version because we could be running NT
    if( osv.dwPlatformId != VER_PLATFORM_WIN32_NT ) 
    {
        // if dll not found, simply close the saver
        hPasswrdCheck = ::LoadLibrary( "PASSWORD.CPL" );
        if( hPasswrdCheck == NULL)
            goto HANDLE_ERROR;

        // show the password verification dialog
        VerifyScreenSavePwd = (VERIFYSCREENSAVEPWD)GetProcAddress( hPasswrdCheck, "VerifyScreenSavePwd" );
        if( VerifyScreenSavePwd != NULL )
            result = (BOOL)VerifyScreenSavePwd( hwnd );
    }

    HANDLE_ERROR:;

    if( hPasswrdCheck )
        FreeLibrary( hPasswrdCheck );

    return result;

} // VerifyPassword


/************************************************************************
*    FUNCTION NAME:         WakeSleepingScreenSaver                                                             
*
*    DESCRIPTION:           Wake up the sleeping screen saver
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd   - Handle to this programs window
*
*    Output:   none
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CSSaverWnd::WakeSleepingScreenSaver()
{
    // If the monitor is asleep and we get a paint message, then tell
    // the screen saver that we need to repaint the whole screen because the monitor is waking up
    if( GetMonitorIsAsleep() )
    {
        // Set the flag that indicates the monitor is not asleep
        while( GetMonitorIsAsleep() )
            SetMonitorIsAsleep( FALSE );

        // Set the flag that indicates the whole screen needs to be repainted
        SetRepaintScreen();
    }
}   // WakeSleepingScreenSaver


/************************************************************************
*    FUNCTION NAME:         PutScreenSaverToSleep                                                             
*
*    DESCRIPTION:           Put screen saver to sleep
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd   - Handle to this programs window
*
*    Output:   none
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CSSaverWnd::PutScreenSaverToSleep()
{
    // Pause the sound only once
    if( !GetMonitorIsAsleep() )
        ssData.PauseSound = TRUE;

    // Set the flag that indicates the monitor is asleep
    while( !GetMonitorIsAsleep() )
        SetMonitorIsAsleep( TRUE );

}   // PutScreenSaverToSleep