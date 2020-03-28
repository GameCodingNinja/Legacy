
/************************************************************************
*
*    FILE NAME:       Starter.cpp
*
*    DESCRIPTION:     The begining of all great API windows programs
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "Starter.h"         // Header file for this *.cpp file.
#include "DoNothingDlg.h"    // The do nothing dialog box for example sake

#include <iostream>
#include <sstream>
#include <map>

using namespace std;

// Globals
CStarterWnd *pStarterWnd;


/************************************************************************
*                       Non Object Related Functions
************************************************************************/


/************************************************************************
*    DESCRIPTION: The entry point for all good windows programs
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
************************************************************************/
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInstance,
                    LPSTR lpszCmdParam, int nCmdShow )
{
    MSG Msg;

    // Set wParam to 0 in case need to return right away.
    Msg.wParam = 0;

    // Create the parent window object
    CStarterWnd StarterWnd;
	pStarterWnd = &StarterWnd;

    // Create the parent window
    if( StarterWnd.CreateParentWnd( hInst, nCmdShow ) )
    {
	    // Standard windows message loop
	    while( GetMessage( &Msg, NULL, 0, 0 ) )
	    {
	        TranslateMessage( &Msg );
	        DispatchMessage( &Msg );
	    }
	}

    return Msg.wParam;

}   // WinMain


/************************************************************************
*    DESCRIPTION:  Standard Windows message handler.
*                  We are using this to call our object functions
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message
*              UINT Message   - Message being sent 
*              WPARAM wParam  - Additional information included with message
*              LPARAM lParam  - Additional information included with message
*
*    Output:   LRESULT - Description
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
*                      Window Object Functions
************************************************************************/


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
************************************************************************/
CStarterWnd::CStarterWnd()
{
    // Init class members

}   // Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
************************************************************************/
CStarterWnd::~CStarterWnd()
{
}   // Destructer


/************************************************************************
*    DESCRIPTION: Create the parent Window
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd   - Handle of the window receiving the message
*      LPCREATESTRUCT lpCreateStruct  - Structure used in window creation 
*
*    Output:  bool - true on success or false on fail
************************************************************************/
bool CStarterWnd::CreateParentWnd( HINSTANCE hInstance, int nCmdShow )
{
    // Save the program instance
    hInst = hInstance;

    // Register the parent window with Windows
    if( !RegisterWnd( hInst ) )
        return false;

    // Create the parent window
    if( !CreateWnd( hInst, nCmdShow ) )
        return false;

    return true;

}   // CreateParentWnd


/************************************************************************
*    DESCRIPTION: All windows created with CreateWindow
*                 must be registered first. This does not include
*                 windows controls like buttons, listboxex and such
*                 because they register themselves.
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*
*    Output:   bool - true on success or false on fail
************************************************************************/
bool CStarterWnd::RegisterWnd( HINSTANCE hInstance )
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
*    DESCRIPTION: Create the parent window
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*
*              int nCmdShow    - Specifies how the window is to be shown.
*                                Example: SW_SHOWMINIMIZED, SW_SHOWMAXIMIZED
*
*    Output:   HWND - A handle to the newly created window
************************************************************************/
bool CStarterWnd::CreateWnd( HINSTANCE hInstance, int nCmdShow )
{
    // Create the parent window & save the window handle
    hParentWnd = CreateWindow( "Starter", "The Starter API App", WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                               CW_USEDEFAULT, CW_USEDEFAULT,
                               CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL );

    // Make sure we have a handle to the window
    if( hParentWnd == NULL )
        return false;

    return true;

}   // CreateWnd


/************************************************************************
*    DESCRIPTION: Standard Windows OnDestroy message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message 
************************************************************************/
void CStarterWnd::OnDestroy( HWND hwnd )
{
    // This causes the WinMain to fall out of it's while loop
    PostQuitMessage( 0 );

} // OnDestroy


/************************************************************************
*    DESCRIPTION: Standard Windows OnCommand message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              int id          - Menu ID of the of the control.
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT codeNotify - The action taking place
************************************************************************/
void CStarterWnd::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
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

        case MNU_CREATE_DLG:
            Display_DoNothingDlg( hInst, hwnd );
        break;
    }

}   // OnCommand


/************************************************************************
*    DESCRIPTION: Standard Windows OnCreate message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd  - Handle of the window receiving the message
*      LPCREATESTRUCT lpCreateStruct  - Structure used in window creation 
*
*    Output:   bool - true on success or false on fail
************************************************************************/
bool CStarterWnd::OnCreate( HWND hwnd, LPCREATESTRUCT lpCreateStruct )
{
	stringstream temp;
	temp << "50" << endl;
    return true;

}   // Starter_OnCreate

