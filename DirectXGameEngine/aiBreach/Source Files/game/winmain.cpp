
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
#include "game.h"

// Boost lib dependencies
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <misc/settings.h>

// Game-specific sector size
const int CWorldValue::SECTOR_SIZE(1024);
const int CWorldValue::HALF_SECTOR_SIZE( CWorldValue::SECTOR_SIZE / 2 );

// Function prototypes - Not to be called except from here 
// which is why it's not defined in the header file
void SetupUserFolders();

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

    // Setup the user folders for game saves and config files
    SetupUserFolders();

    try
    {
        // Load the settings file
        CSettings::Instance().LoadFromXML("data/settings/settings.cfg");

        // Create the game singleton
        CGame::Instance().Create( hInst, WndProc );

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
            if( CGame::Instance().IsGameRunning() )
                CGame::Instance().GameLoop();
        }
    }
    catch( NExcept::CCriticalException & ex )
    {
        MessageBox( GetActiveWindow(), ex.GetErrorMsg().c_str(), ex.GetErrorTitle().c_str(), MB_ICONERROR );
        NGenFunc::PostDebugMsg("Critical Exception: %s, %s", ex.GetErrorTitle().c_str(), ex.GetErrorMsg().c_str() );
        CGame::Instance().ExceptionCleanUp();
    }
    catch( std::exception const & ex )
    {
        MessageBox( GetActiveWindow(), ex.what(), "Standard Exception", MB_ICONERROR );
        NGenFunc::PostDebugMsg("Standard Exception: %s", ex.what() );
        CGame::Instance().ExceptionCleanUp();
    }
    catch(...)
    {
        MessageBox( GetActiveWindow(), "Something bad happened and I'm not sure what it was.", "Unhandled Error", MB_ICONERROR );
        NGenFunc::PostDebugMsg("Something bad happened and I'm not sure what it was." );
        CGame::Instance().ExceptionCleanUp();
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
        HANDLE_MSG( hwnd, WM_CREATE,        CGame::Instance().OnCreate );
        HANDLE_MSG( hwnd, WM_SIZE,          CGame::Instance().OnSize );
        HANDLE_MSG( hwnd, WM_CLOSE,         CGame::Instance().OnClose );
        HANDLE_MSG( hwnd, WM_LOADING,       CGame::Instance().OnLoading );
        HANDLE_MSG( hwnd, WM_LOAD_COMPLETE, CGame::Instance().OnLoadComplete );
        HANDLE_MSG( hwnd, WM_ACTIVATEAPP,   CGame::Instance().OnActivateApp );

        case WM_NCPAINT:
            return CGame::Instance().OnNcPaint( hwnd, Message, wParam, lParam );

        default:
            return DefWindowProc( hwnd, Message, wParam, lParam );
    }

}  // WndProc


/***************************************************************************
*   decs:   Setup the user folders for game saves and config files
****************************************************************************/
void SetupUserFolders()
{
    // Create a shortened namespace name
    namespace fs = boost::filesystem;

    if( !CSettings::Instance().GetFolderPath().empty() )
    {
        // Append the development name to the Windows Documents folder path
        CSettings::Instance().AppendToFolderPath("/RabidSquirrelGames");

        try
        {
            // Create the development name folder
            if( !fs::is_directory( CSettings::Instance().GetFolderPath() ) )
                fs::create_directory( CSettings::Instance().GetFolderPath() );

            // Append the game name to the path
            CSettings::Instance().AppendToFolderPath("/aiBreach");

            // Create the game name folder
            if( !fs::is_directory( CSettings::Instance().GetFolderPath() ) )
                fs::create_directory( CSettings::Instance().GetFolderPath() );

            // Append a slash so that loading files from documents folder
            // or exe folder remain the same.
            CSettings::Instance().AppendToFolderPath("/");

            // Create the data folder
            if( !fs::is_directory( CSettings::Instance().GetFolderPath() + "data" ) )
                fs::create_directory( CSettings::Instance().GetFolderPath() + "data" );

            // Create the settings folder
            if( !fs::is_directory( CSettings::Instance().GetFolderPath() + "data/settings" ) )
                fs::create_directory( CSettings::Instance().GetFolderPath() + "data/settings" );

            // Copy the settings config file
            if( !fs::is_regular_file( CSettings::Instance().GetFolderPath() + "data/settings/settings.cfg" ) )
            {
                fs::path from(fs::current_path().string() + "/data/settings/settings.cfg");
                fs::path to(CSettings::Instance().GetFolderPath() + "data/settings/settings.cfg");

                fs::copy_file( from, to );
            }

            // Copy the controller mapping config file
            if( !fs::is_regular_file( CSettings::Instance().GetFolderPath() + "data/settings/controllerMapping.cfg" ) )
            {
                fs::path from(fs::current_path().string() + "/data/settings/controllerMapping.cfg");
                fs::path to(CSettings::Instance().GetFolderPath() + "data/settings/controllerMapping.cfg");

                fs::copy_file( from, to );
            }
        }
        catch(fs::filesystem_error const & e)
        {
            NGenFunc::PostDebugMsg( "Configuration file error (%s)", e.what() );

            // there was an error so clear the path
            CSettings::Instance().ClearFolderPath();
        }
    }

}	// SetupUserFolders
