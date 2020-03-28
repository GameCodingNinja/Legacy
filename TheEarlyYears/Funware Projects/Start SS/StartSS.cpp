
/************************************************************************
*
*    PRODUCT:         Starter
*
*    FILE NAME:       Starter.cpp
*
*    DESCRIPTION:     The begining of all great API windows programs
*
*    IMPLEMENTATION: How it's done 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "StartSS.h"         // Header file for this *.cpp file.


// Globals
PTStarterWnd pStarterWnd;


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

    // Create the parent window object
    pStarterWnd = new TStarterWnd();

    // Create the parent window
    if( pStarterWnd )
        Result = pStarterWnd->CreateParentWnd( hInst, nCmdShow );

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
        HANDLE_MSG( hwnd, WM_DESTROY, pStarterWnd->OnDestroy );
        HANDLE_MSG( hwnd, WM_COMMAND, pStarterWnd->OnCommand );
        HANDLE_MSG( hwnd, WM_CREATE,  pStarterWnd->OnCreate );
        default:
            return DefWindowProc( hwnd, Message, wParam, lParam );
    }

} // WndProc


/************************************************************************
*    FUNCTION NAME:         TimerCallBack                                                             
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

void CALLBACK TimerCallBack( HWND hwnd, UINT uTimerMsg, UINT uTimerID, DWORD dwTime )
{
    // Play the animations
    pStarterWnd->TimerCallback( hwnd );

}   // TimerCallBack


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

TStarterWnd::TStarterWnd()
{
    // Init class members
	TimerReturnCode = 0;
	intervalCounter = 0;

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

TStarterWnd::~TStarterWnd()
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

BOOL TStarterWnd::CreateParentWnd( HINSTANCE hInstance, int nCmdShow )
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

BOOL TStarterWnd::RegisterWnd( HINSTANCE hInstance )
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
    w.lpszMenuName = "Menu_1";
    w.lpszClassName = "Starter";

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

BOOL TStarterWnd::CreateWnd( HINSTANCE hInstance, int nCmdShow )
{
    // Create the parent window & save the window handle
    hParentWnd = CreateWindow( "Starter", "Start Screen Saver", WS_OVERLAPPEDWINDOW|WS_VISIBLE,
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

void TStarterWnd::OnDestroy( HWND hwnd )
{
	// Kill the timer if it is still running
	if( TimerReturnCode )
	{
		KillTimer( hwnd, 3 );
		TimerReturnCode = 0;
	}

    // This causes the WinMain to fall out of it's while loop
    PostQuitMessage( 0 );

} // OnDestroy


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

void TStarterWnd::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case MNU_ABOUT:
            MessageBox( hwnd, "Simple do nothing windows program!", "Starter", MB_OK );
        break;

        case MNU_EXIT:
            // Destroy the parent window
            DestroyWindow( hwnd );
        break;

		case MNU_START_SS:
			if( !TimerReturnCode )
			{
			    // Write the new INI file settings
    			GetPrivateProfileString( "boot", "SCRNSAVE.EXE", "", ssName, sizeof(ssName), "system.ini" );

				if( ssName[0] )
					TimerReturnCode = (int)SetTimer( hwnd, 3, 3000, (TIMERPROC)TimerCallBack );
			}
			else
			{
				KillTimer( hwnd, 3 );
				TimerReturnCode = 0;
			}

		break;

		case MNU_RESET_COUNTER:
			intervalCounter = 0;

			// Show the count
			Edit_DisplayValue( hStatic, intervalCounter );
		break;
    }

}   // OnCommand


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

BOOL TStarterWnd::OnCreate( HWND hwnd, LPCREATESTRUCT lpCreateStruct )
{
	// Center it
    CenterWindow( hwnd, 300, 150 );

	// Get the windows dir
	GetWindowsDirectory( winDir, sizeof( winDir ) );

	// Create a static to show the interval count
	hStatic = CreateWindow( "static", "0", WS_CHILD|WS_VISIBLE|SS_LEFT, 5, 5, 50, 14, hwnd, NULL, hInst, NULL );
    
    return TRUE;

}   // Starter_OnCreate


/************************************************************************
*    FUNCTION NAME:         TimerCallback                                                             
*
*    DESCRIPTION:           Timer call back function.
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

void TStarterWnd::TimerCallback( HWND hwnd )
{
	++intervalCounter;

	// Show the count
	Edit_DisplayValue( hStatic, intervalCounter );

	ShellExecute( hwnd, "open", ssName, "/S", winDir, SW_SHOWNORMAL );

}   // TimerCallback


/************************************************************************
*    FUNCTION NAME:         Edit_DisplayValue                                                             
*
*    DESCRIPTION:           Display an int in an edit control.
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

void TStarterWnd::Edit_DisplayValue( HWND hCtrl, int Value )
{
    char StrTmp[20];

    // Convert the int to a string
    wsprintf( StrTmp, "%d", Value );

    // Have the control display the amount
    Edit_SetText( hCtrl, StrTmp );

}   // Edit_DisplayValue


/************************************************************************
*    FUNCTION NAME:         CenterWindow()                                                             
*
*    DESCRIPTION:           Sizing and centering a window. If you send it
*                           a 0 width or 0 height, the width or height will
*                           be the size of the computers resolution.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd     - Handle of the window receiving the message
*              int Width     - Width of the window
*              int Height    - Height of the window     
*
*    Output:   long - Returns the error code if any
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void TStarterWnd::CenterWindow( HWND hwnd, int Width, int Height, HWND hParentWnd, BOOL IsChildWnd )
{
    int X, Y, ScreenW, ScreenH, ParentW, ParentH;
    RECT rect;

    // Get the size the the display screen
    ScreenW = GetSystemMetrics( SM_CXSCREEN );
    ScreenH = GetSystemMetrics( SM_CYSCREEN );

    // Are we centering inside of a parent
    if( hParentWnd )
    {
        // Get the parent windows rect
        GetWindowRect( hParentWnd, &rect );

        // Get the width and height of the parent window
        ParentW = rect.right - rect.left;
        ParentH = rect.bottom - rect.top;

        X = (ParentW - Width) / 2;
        Y = (ParentH - Height) / 2;

        // If this is not a child window add in the parents XY's
        if( !IsChildWnd )
        {
            X += rect.left;
            Y += rect.top;

            // Make sure we are not hidden in the window
            if( X + Width > ScreenW )
                X = ScreenW - Width;
            if( Y + Height > ScreenH )
                Y = ScreenH - Height;
        }
        else
        {
            if( X < 0 ) X = 0;
            if( Y < 0 ) Y = 0;

            // Make sure we are not hidden in the window
            if( X + Width > ParentW )
                X = ParentW - Width;
            if( Y + Height > ParentH )
                Y = ParentH - Height;
        }
    }
    // Center across the whole screen
    else
    {
        if (Width == 0)
        {
            Width = ScreenW;
            X = 0;
        }
        else
            X = (ScreenW - Width) / 2;

        if (Height == 0)
        {
            Height = ScreenH;
            Y = 0;
        }
        else
            Y = (ScreenH - Height) / 2;
    }

    // Put a window at a given position and size
    MoveWindow( hwnd, X, Y, Width, Height, TRUE );

}   // CenterWindow