
/*****************************************************************************
*    FILE NAME:       winmain.cpp
*
*    DESCRIPTION:     window's main function
****************************************************************************/

// Game OS dependencies

// Enables strict compiler error checking. Without this, the compiler doesn't
// know the difference between many Windows data types. Very bad to leave out.
#define STRICT
#include <windows.h>   // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>  // Macros the makes windows programming easer and more readable. Doesn't add to code size

// Physical component dependency
#include "winmain.h"
#include "liminality.h"

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <common/defs.h>

/***************************************************************************
*   desc:   The entry point for all good windows programs
*
*   param:  HINSTANCE hInst - This progams instance
*           HINSTANCE hPrevInstance - Of no value to 32 bit windows programs
*           LPSTR lpszCmdParam - The command line is sent by Windows. The command
*                               originally has the path to this program but
*                               Windows parses it out before it hands it over.
*                               To get the command line with the path in it you
*                               need to call GetCommandLine() API call.
*           int nCmdShow - Specifies how the window is to be shown.
*                          Example: SW_SHOWMINIMIZED, SW_SHOWMAXIMIZED
*                          We are not currently using it.
*
*   ret:  int - Usually it's the last message wParam.
****************************************************************************/
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInstance,
                    LPSTR lpszCmdParam, int nCmdShow )
{
    MSG msg;

    // Set wParam to 0 in case need to return right away.
    msg.wParam = 0;

    try
    {
        // Create the parent window singleton
        CLiminality::Instance().CreateParentWnd( hInst, WndProc );

        // A game requires an infinite loop
        while( true )
        {
            // Check for waiting messages
            if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
            {
                // Change the format of certain messages
                TranslateMessage( &msg );

                // Pass the message to WndProc() for processing
                DispatchMessage( &msg );

                // Is it time to quit
                if( msg.message == WM_QUIT )
                    break;
            }

            // Call the game loop
            if( CLiminality::Instance().IsGameRunning() )		
                CLiminality::Instance().GameLoop();
        }
    }
    catch( NExcept::CCriticalException & ex )
    {
        MessageBox( GetActiveWindow(), ex.GetErrorMsg().c_str(), ex.GetErrorTitle().c_str(), MB_ICONERROR );
        NGenFunc::PostDebugMsg("Critical Exception: %s, %s", ex.GetErrorTitle().c_str(), ex.GetErrorMsg().c_str() );
        CLiminality::Instance().ExceptionCleanUp();
    }
    catch( std::exception const & ex )
    {
        MessageBox( GetActiveWindow(), ex.what(), "Standard Exception", MB_ICONERROR );
        NGenFunc::PostDebugMsg("Standard Exception: %s", ex.what() );
        CLiminality::Instance().ExceptionCleanUp();
    }
    catch(...)
    {
        MessageBox( GetActiveWindow(), "Something bad happened and I'm not sure what it was.", "Unhandled Error", MB_ICONERROR );
        NGenFunc::PostDebugMsg("Something bad happened and I'm not sure what it was." );
        CLiminality::Instance().ExceptionCleanUp();
    }

    return msg.wParam;

}   // WinMain


/***************************************************************************
*   decs:   Standard Windows message handler.
*           We are using this to call our object functions
*
*   param:  HWND hwnd      - Handle of the window receiving the message
*           UINT Message   - Message being sent 
*           WPARAM wParam  - Additional information included with message
*           LPARAM lParam  - Additional information included with message
*
*   ret:    LRESULT - What the message returned
****************************************************************************/
LRESULT CALLBACK WndProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    switch( Message )
    {
        HANDLE_MSG( hwnd, WM_DESTROY,       CLiminality::Instance().OnDestroy );
        HANDLE_MSG( hwnd, WM_COMMAND,       CLiminality::Instance().OnCommand );
        HANDLE_MSG( hwnd, WM_CREATE,        CLiminality::Instance().OnCreate );
        HANDLE_MSG( hwnd, WM_SIZE,          CLiminality::Instance().OnSize );
        HANDLE_MSG( hwnd, WM_CLOSE,         CLiminality::Instance().OnClose );
        HANDLE_MSG( hwnd, WM_LOADING,       CLiminality::Instance().OnLoading );
        HANDLE_MSG( hwnd, WM_LOAD_COMPLETE, CLiminality::Instance().OnLoadComplete );

        case WM_NCPAINT:
            return CLiminality::Instance().OnNcPaint( hwnd, Message, wParam, lParam );

        default:
            return DefWindowProc( hwnd, Message, wParam, lParam );
    }

}  // WndProc
