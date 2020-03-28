
/************************************************************************
*
*    PRODUCT:         Screen Saver Animation Base Class
*
*    FILE NAME:       SSThread.cpp
*
*    DESCRIPTION:     Thread that runs the screen saver
*
*    IMPLEMENTATION:  Designed to be used as a screen saver thread base.
*                     Here is where the screen saver is initilized,
*                     run it and then free all allocated memory and resources.
*                     New projects would use this like a library and not
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
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
                             // Note: we are not using the #define WIN32_LEAN_AND_MEAN because we need some multimedia functions
                             // WIN32_LEAN_AND_MEAN leaves out
#include "minTools.h"        // A misc group of useful functions
#include "SSBase.h"          // Header file for this *.cpp file.
#include <stdlib.h>          // Declares several commonly used routines such as atoi, random, abs, malloc etc.


// This quiets the compiler warnings about a bunch of functions that Microsoft has
// decided to deprecate, even though the C++ standardization committee has not done so.
#pragma warning(disable : 4996)

/************************************************************************
*    FUNCTION NAME:         Constructor                                                             
*
*    FUNCTION PARAMETERS:
*    Input:    PTSaverModeData ssData - screen saver data 
************************************************************************/
CScreenSaver::CScreenSaver( PSSaverModeData ssData )
{
    // copy over the pointer to the screen saver data
    pssData = ssData;
    errorStr[0] = 0;

}   // Constructor


/************************************************************************
*    DESCRIPTION:           Try to load the screen saver 
*
*    Output:   returntype - Description
************************************************************************/
bool CScreenSaver::Load()
{
    bool result = false;
    
    // Load your graphics and other initilizations
    if( SaverActive() )
    {
        // Set the flag that indicates we are loading the screen saver
        SetScreenSaverLoading( true );

        // Load the screen saver
        result = InitilizeScreenSaver();

        // We are done loading the screen saver.
        // The loading flag is used for waiting for the load process to finish
        // before we see if we need to wait for the thread to time out if need be.
        // This is needed for slower computers loading complicated screen savers.
        SetScreenSaverLoading( false );
    }

    return result;

}   // Load


/************************************************************************
*    DESCRIPTION:           Standard Screen Saver inilizations 
*
*    Output:   bool - true or false depending on if successful
************************************************************************/
bool CScreenSaver::LoadGeneralStartUpData()
{
    // Get the device context to the main screen
    SetScreenDC( GetDC( Handle() ) );

    // Make sure we have a screen DC
    if( GetScreenDC() == NULL )
    {
        strcpy( errorStr, "Can't get a DC from the window handle. Your system resources may be low so try restarting your computer." );
        return false;
    }

    // Get the bit depth of the system, be it 8, 16, 24, 32
    systemBitDepth = GetDeviceCaps( GetScreenDC(), BITSPIXEL );

    // Get the Width & Height of the desktop which could include more then one monitor.
    // Use init class variables for better speed in tight loops.
    desktopWidth = pssData->desktopWidth;
    desktopHeight = pssData->desktopHeight;

    // Get the Width & Height of the screen which includes only the first monitor.
    // Use init class variables for better speed in tight loops.
    screenWidth = pssData->screenWidth;
    screenHeight = pssData->screenHeight;

    // Save the rect to our windows for easy use with functions that require 
    // a rect as a paramater.
    SetRect( &windowRect, 0, 0, WindowWidth(), WindowHeight() );
    SetRect( &screenRect, 0, 0, screenWidth, screenHeight );
    SetRect( &desktopRect, 0, 0, desktopWidth, desktopHeight );

    // Give the canvas class the active DC
    canvas.InitActiveDC( GetScreenDC() );

    // Create the default font
    canvas.SetFont( "Arial", 12, FONT_BOLD, RGB( 255, 0, 0 ) );

    // Get a stock brush. Stock objects don't have to be freed
    blackBrush = (HBRUSH)GetStockObject( BLACK_BRUSH );

    // Things must be real bad if we can't get a stock object
    if( blackBrush == NULL )
    {
        strcpy( errorStr, "Can't allocate a black brush. Your system resources may be low so try restarting your computer." );
        return false;
    }

    return true;

}   // LoadGeneralStartUpData


/************************************************************************
*    DESCRIPTION:           Initilize all you screen saver specific code
*                           here. 
*
*    Output:   returntype - Description
************************************************************************/
bool CScreenSaver::LoadSpecificStartUpData()
{
    bool result = false;

    // Seed the random number generator
    srand ( timeGetTime () );

    errorNumber = 10; // Set the error number to help tell when the error happened

    // Load the user selected preferences
    // This always has to be first because we need to know what the user wants to do
    if( !LoadControlPanelPrefs() )
        goto INITILIZE_ERROR;

    // If the screen saver is being stopped early, just return true and it will fall 
    // through on it's own.
    if( SaverActive() == false )
        return true;

    errorNumber = 15; // Set the error number to help tell when the error happened

    // Load the graphics needed for this screen saver
    if( !LoadGraphics() )
        goto INITILIZE_ERROR;

    // If the screen saver is being stopped early, just return true and it will fall 
    // through on it's own.
    if( SaverActive() == false )
        return true;

    errorNumber = 20; // Set the error number to help tell when the error happened

    // Do last round if initilizations
    if( !InitSaverData() )
        goto INITILIZE_ERROR;

    // If the screen saver is being stopped early, just return true and it will fall 
    // through on it's own.
    if( SaverActive() == false )
        return true;

    // Load the sound effects. This can return an error and we can keep going
    LoadSound();

    // Allocate the media player for playing music. If there is an error
    // allocating the Windows Media player, we still allow the screen saver to run.
    // Computers are known for having sound problems so we should not stop the screen saver
    // just because of that.
    InitMusicPlayer();

    result = true;

INITILIZE_ERROR:;

    return result;

}   // LoadSpecificStartUpData


/************************************************************************
*    DESCRIPTION:           Initilize all you screen saver specific code
*                           here. LoadSpecificStartUpData is a virtual hook. 
*
*    Output:   returntype - Description
************************************************************************/
bool CScreenSaver::InitilizeScreenSaver()
{
    bool result = false;

    // Set the error number to help tell when the error happened
    errorNumber = 5;

    // Do the standard Initilizations. This has to be first. 
    if( !LoadGeneralStartUpData() ) 
        goto INITILIZE_ERROR;

    // Load the user selected preferences and screen saver specific Initilizations
    if( !LoadSpecificStartUpData() ) 
        goto INITILIZE_ERROR;

    result = true;

INITILIZE_ERROR:;

    // There was an error, Kill the screen saver
    if( !result )
        ScreenSaverErrorAbortSaver();

    return result;

}   // InitilizeScreenSaver


/************************************************************************
*    DESCRIPTION:           Use this function if there was an error and
*                           the screen saver has to be stopped.
************************************************************************/
void CScreenSaver::ScreenSaverErrorAbortSaver()
{
    char error[400];

    // Show the cursor so they can click the message box
    if( Mode() == ssSaver )
        ShowHideCursor( SHOW_CURSOR );

    // Build the error string
    wsprintf( error, "There has been an error initilizing the screen saver.  %s  Error Number %d", errorStr, errorNumber );

    // We need to call MessageBox and pass it the handle to our window so that the
    // screen saver executation will stop here until the user clicks OK.
    MessageBox( Handle(), error, "Error", MB_ICONINFORMATION );

    // Keeps the screen saver loop from executing
    StopScreenSaver();

    // Post a message to the main window to close
    PostMessage( Handle(), WM_CLOSE, 0, 0 );

}   // ScreenSaverErrorAbortSaver


/************************************************************************
*    DESCRIPTION:           Free up all allocated resources.                                                             
************************************************************************/
CScreenSaver::~CScreenSaver()
{
    // Free the canvas
    // Need to release all resouces related to the DC before
    // we delete the DC
    canvas.Free();

    // This must always be last
    // Release the main windows DC
    if( GetScreenDC() )
    {
        if( !ReleaseDC( Handle(), GetScreenDC() ) )
            MessageBox( 0, "Unable to release window DC", "Error", MB_ICONINFORMATION );

        SetScreenDC( NULL );
    }

}   // Destructor


/************************************************************************
*    DESCRIPTION:           Indicates a repaint of the whole screen is needed.
*                           Only for screen saver that don't refresh the whole
*                           screen on every cycle. 
*
*    Output:   bool - true or false depending on if successful
************************************************************************/
bool CScreenSaver::RepaintScreen()
{
    bool result = false;

    // Do we need to repaint the screen?
    if( pssData->repaintScreen )
    {
        // Reset the varaibles back to false
        while( pssData->repaintScreen )
            pssData->repaintScreen = false;

        result = true;
    }

    return result;

} // RepaintScreen