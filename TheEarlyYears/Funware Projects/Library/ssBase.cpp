
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
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
                             // Note: we are not using the #define WIN32_LEAN_AND_MEAN because we need some multimedia functions
                             // WIN32_LEAN_AND_MEAN leaves out
#include "minTools.h"        // A misc group of useful functions
#include "SSBase.h"          // Header file for this *.cpp file.
#include <stdlib.h>          // Declares several commonly used routines such as atoi, random, abs, malloc etc.
#include "Settings.h"        // User settings for this screen saver
#include "Metafile.h"        // Class for loading and displaying Windows metafiles and enhanced metafiles

// Turn off warnings for deprecated calls
#pragma warning(disable : 4996)


/************************************************************************
*    FUNCTION NAME:         Constructor                                                             
*
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

CScreenSaver::CScreenSaver( PTSaverModeData ssData )
{
    // copy over the pointer to the screen saver data
    pssData = ssData;
    errorStr[0] = 0;

}   // Constructor


/************************************************************************
*    FUNCTION NAME:         Load                                                             
*
*    DESCRIPTION:           Try to load the screen saver
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

BOOL CScreenSaver::Load()
{
    BOOL result = FALSE;
    
    // Load your graphics and other initilizations
    if( SaverActive() )
    {
        // Set the flag that indicates we are loading the screen saver
        SetScreenSaverLoading( TRUE );

        // Load the screen saver
        result = InitilizeScreenSaver();

        // We are done loading the screen saver.
        // The loading flag is used for waiting for the load process to finish
        // before we see if we need to wait for the thread to time out if need be.
        // This is needed for slower computers loading complicated screen savers.
        SetScreenSaverLoading( FALSE );
    }

    return result;

}   // Load


/************************************************************************
*    FUNCTION NAME:         LoadGeneralStartUpData                                                             
*
*    DESCRIPTION:           Standard Screen Saver inilizations
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

BOOL CScreenSaver::LoadGeneralStartUpData()
{
    // Get the device context to the main screen
    SetScreenDC( GetDC( Handle() ) );

    // Make sure we have a screen DC
    if( GetScreenDC() == NULL )
    {
        strcpy( errorStr, "Can't get a DC from the window handle. Your system resources may be low so try restarting your computer." );
        return FALSE;
    }

    // Get the bit depth of the system, be it 8, 16, 24, 32
    SystemBitDepth = GetDeviceCaps( GetScreenDC(), BITSPIXEL );

    // Get the Width & Height of the desktop which could include more then one monitor.
    // Use init class variables for better speed in tight loops.
    DesktopWidth = pssData->desktopWidth;
    DesktopHeight = pssData->desktopHeight;

    // Get the Width & Height of the screen which includes only the first monitor.
    // Use init class variables for better speed in tight loops.
    ScreenWidth = pssData->screenWidth;
    ScreenHeight = pssData->screenHeight;

    // Save the rect to our windows for easy use with functions that require 
    // a rect as a paramater.
    SetRect( &WindowRect, 0, 0, WindowWidth(), WindowHeight() );
    SetRect( &ScreenRect, 0, 0, ScreenWidth, ScreenHeight );
    SetRect( &DesktopRect, 0, 0, DesktopWidth, DesktopHeight );

    // Give the canvas class the active DC
    canvas.InitActiveDC( GetScreenDC() );

    // Create the default font
    canvas.SetFont( "Arial", 12, FONT_BOLD, RGB( 255, 0, 0 ) );

    // Get a stock brush. Stock objects don't have to be freed
    BlackBrush = (HBRUSH)GetStockObject( BLACK_BRUSH );

    // Things must be real bad if we can't get a stock object
    if( BlackBrush == NULL )
    {
        strcpy( errorStr, "Can't allocate a black brush. Your system resources may be low so try restarting your computer." );
        return FALSE;
    }

    return TRUE;

}   // LoadGeneralStartUpData


/************************************************************************
*    FUNCTION NAME:         LoadSpecificStartUpData                                                             
*
*    DESCRIPTION:           Initilize all you screen saver specific code
*                           here.
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

BOOL CScreenSaver::LoadSpecificStartUpData()
{
    BOOL Result = FALSE;

    // Seed the random number generator
    #ifdef MSVC_COMPILER
      srand ( timeGetTime () );
    #else
      randomize();
    #endif

    Error_Number = 10; // Set the error number to help tell when the error happened

    // Load the user selected preferences
    // This always has to be first because we need to know what the user wants to do
    if( !LoadControlPanelPrefs() )
        goto INITILIZE_ERROR;

    // If the screen saver is being stopped early, just return true and it will fall 
    // through on it's own.
    if( SaverActive() == FALSE )
        return TRUE;

    Error_Number = 15; // Set the error number to help tell when the error happened

    // Load the graphics needed for this screen saver
    if( !LoadGraphics() )
        goto INITILIZE_ERROR;

    // If the screen saver is being stopped early, just return true and it will fall 
    // through on it's own.
    if( SaverActive() == FALSE )
        return TRUE;

    Error_Number = 20; // Set the error number to help tell when the error happened

    // Do last round if initilizations
    if( !InitSaverData() )
        goto INITILIZE_ERROR;

    // If the screen saver is being stopped early, just return true and it will fall 
    // through on it's own.
    if( SaverActive() == FALSE )
        return TRUE;

    // Load the sound effects. This can return an error and we can keep going
    LoadSound();

    // Allocate the media player for playing music. If there is an error
    // allocating the Windows Media player, we still allow the screen saver to run.
    // Computers are known for having sound problems so we should not stop the screen saver
    // just because of that.
    InitMusicPlayer();

    Result = TRUE;

INITILIZE_ERROR:;

    return Result;

}   // LoadSpecificStartUpData


/************************************************************************
*    FUNCTION NAME:         InitilizeScreenSaver                                                             
*
*    DESCRIPTION:           Initilize all you screen saver specific code
*                           here. LoadSpecificStartUpData is a virtual hook.
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

BOOL CScreenSaver::InitilizeScreenSaver()
{
    BOOL Result = FALSE;

    // Set the error number to help tell when the error happened
    Error_Number = 5;

    // Do the standard Initilizations. This has to be first. 
    if( !LoadGeneralStartUpData() ) 
        goto INITILIZE_ERROR;

    // Load the user selected preferences and screen saver specific Initilizations
    if( !LoadSpecificStartUpData() ) 
        goto INITILIZE_ERROR;

    Result = TRUE;

INITILIZE_ERROR:;

    // There was an error, Kill the screen saver
    if( !Result )
        ScreenSaverErrorAbortSaver();

    return Result;

}   // InitilizeScreenSaver


/************************************************************************
*    FUNCTION NAME:         ScreenSaverErrorAbortSaver                                                             
*
*    DESCRIPTION:           Use this function if there was an error and
*                           the screen saver has to be stopped.
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

void CScreenSaver::ScreenSaverErrorAbortSaver()
{
    char error[400];

    // Show the cursor so they can click the message box
    if( Mode() == ssSaver )
        ShowHideCursor( SHOW_CURSOR );

    // Build the error string
    wsprintf( error, "There has been an error initilizing the screen saver.  %s  Error Number %d", errorStr, Error_Number );

    // We need to call MessageBox and pass it the handle to our window so that the
    // screen saver executation will stop here until the user clicks OK.
    MessageBox( Handle(), error, "Error", MB_ICONINFORMATION );

    // Keeps the screen saver loop from executing
    StopScreenSaver();

    // Post a message to the main window to close
    PostMessage( Handle(), WM_CLOSE, 0, 0 );

}   // ScreenSaverErrorAbortSaver


/************************************************************************
*    FUNCTION NAME:         Destructor
*
*    DESCRIPTION:           Free up all allocated resources.                                                             
*
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
*    FUNCTION NAME:         ValidateTempRegNum                                                             
*
*    DESCRIPTION:           Check to see if the temporary registration
*                           number is still useable or in use
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName - Secret file name to check
*              unsigned long CountMax - Description 
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

BOOL CScreenSaver::ValidateTempRegNum( char *FileName, unsigned long CountMax )
{
    char FilePath[MAX_PATH];
    unsigned long bytesWritten, Count;
    SECURITY_ATTRIBUTES FileSec;
    HANDLE hFile;
    BOOL Result = FALSE;

    FileSec.nLength = sizeof( SECURITY_ATTRIBUTES );
    FileSec.lpSecurityDescriptor = NULL;
    FileSec.bInheritHandle = FALSE;

    // Get the windows directory
    GetTheWindowsDirectory( FilePath );

    // Add the file name to the path
    strcat( FilePath, FileName );

    // Create the file
    hFile = CreateFile( FilePath, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ,
                         &FileSec, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    if( hFile == INVALID_HANDLE_VALUE )
        goto FREE_MEMORY;

    // Read the number writen to the file
    ReadFile( hFile, &Count, sizeof( Count ), &bytesWritten, NULL );

    // Check to see if the number is greater than zero and less then the max number
    if( Count > 0 && Count <= CountMax && bytesWritten == sizeof( Count ) )
    {
        // Set the file pointer back to the start of the file
        SetFilePointer( hFile, 0, 0, FILE_BEGIN );
        --Count;
        // Write the new value to the file
        WriteFile( hFile, &Count, sizeof( Count ), &bytesWritten, NULL );
        Result = TRUE;
    }

    FREE_MEMORY:;

    if( hFile >= 0 )
        CloseHandle( hFile );

    return Result;

}   // ValidateTempRegNum


/************************************************************************
*    FUNCTION NAME:         DisplayShareWareScreen
*
*    DESCRIPTION:           Loads the strings, and metafile to make the
*                           shareware screen.                                                             
*
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - Indicates the user canceled the screen saver if FALSE
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CScreenSaver::DisplayShareWareScreen( HDC outputDC, int width, int height, int mainMsg, int bottomInfoMsg, BOOL timeOutScreen )
{
    TMetafile Metafile;
    char MsgCount[10];
    char Message[300];
    char bottomMsgStr[700];
    int MsgFontSize, HowToFontSize;
    DWORD SleepTimer;
    RECT rect;
    BOOL Result = TRUE;
    TCanvas tmpCanvas;

    // init the parameters
    if( outputDC == NULL )
        outputDC = GetScreenDC();

    if( width == 0 )
        width = ScreenWidth;

    if( height == 0 )
        height = ScreenHeight;

    // Set the rect of the area that needs to be blacked out
    SetRect( &rect, 0, 0, width, height );

    // Paint the DC black
    FillRect( outputDC, &rect, BlackBrush );

    // Init the metafile
    Metafile.InitActiveDC( GetScreenDC() );

    // init the canvas
    tmpCanvas.InitActiveDC( outputDC );

    // Load the metafile logo
    if( !Metafile.LoadFromResource( Instance(), "funwareLogo", "WMF" ) )
        goto HANDLE_ERROR;

    // Do we use the shareware mesages or a custom message
    if( mainMsg == 0 )
    {
        // Get the number of message strings currently available
        if( !LoadString( Instance(), GET_NUM_MESSAGES, MsgCount, sizeof( MsgCount ) ) )
            goto HANDLE_ERROR;

        // Use rand() % atio( MsgCount ) to randomly pick one of the strings to load
        if( !LoadString( Instance(), (rand() % atoi( MsgCount )) + (GET_NUM_MESSAGES+1), Message, sizeof( Message ) ) )
            goto HANDLE_ERROR;
    }
    else
    {
        // Load a specific string from the resource
        if( !LoadString( Instance(), mainMsg, Message, sizeof( Message ) ) )
            goto HANDLE_ERROR;
    }

    // Do we use the shareware mesages or a custom message
    if( bottomInfoMsg == 0 )
    {
        // Load the "How To Register" message string
        if( !LoadString( Instance(), HOW_TO_REG_MSG, bottomMsgStr, sizeof( bottomMsgStr ) ) )
            goto HANDLE_ERROR;
    }
    else
    {
        // Load a specific string from the resource
        if( !LoadString( Instance(), bottomInfoMsg, bottomMsgStr, sizeof( bottomMsgStr ) ) )
            goto HANDLE_ERROR;
    }

    // select a font size suitable for the screen resolution
    if( width <= 200 )
    {
        MsgFontSize   = 5;
        HowToFontSize = 4;
    }
    else if( width <= 320 )
    {
        MsgFontSize   = 9;
        HowToFontSize = 7;
    }
    else if( width <= 400 )
    {
        MsgFontSize   = 12;
        HowToFontSize = 9;
    }
    else if( width <= 512 )
    {
        MsgFontSize   = 14;
        HowToFontSize = 11;
    }
    else if( width <= 640 )
    {
        MsgFontSize   = 18;
        HowToFontSize = 12;
    }
    else if( width <= 800 )
    {
        MsgFontSize   = 22;
        HowToFontSize = 14;
    }
    else if( width <= 1152 )
    {
        MsgFontSize   = 30;
        HowToFontSize = 18;
    }
    else if( width <= 1280 )
    {
        MsgFontSize   = 35;
        HowToFontSize = 20;
    }
    else
    {
        MsgFontSize   = 45;
        HowToFontSize = 26;
    }

    // set the coordinates for the Funware logo
    // this helps to position it to be about the same on any display resolution
    rect.left = width / 20;
    rect.top  = height / 20;
    rect.right = width / 3;
    rect.bottom = rect.right;

    // Set the background mode to transparent for the text
    tmpCanvas.SetBackgroundMode( TRANSPARENT );

    // Set the font for the message
    tmpCanvas.SetFont( "Arial", MsgFontSize, FONT_BOLD, RGB( 128, 128, 255 ) );

    // Print the Message text
    tmpCanvas.WrapText( Message, rect.left + rect.right, rect.top * 4,
                        width-((rect.left * 2) + rect.right) );

    // Set the font for the How To Reg message
    tmpCanvas.SetFont( "Arial", HowToFontSize, FONT_REGULAR, RGB( 192, 192, 192 ) );

    // Print the How To Reg text
    tmpCanvas.WrapText( bottomMsgStr, rect.left, height-(tmpCanvas.TextHeight( bottomMsgStr ) * 7),
                        width-(rect.left * 2) );

    // Set the background mode back to opaque
    tmpCanvas.SetBackgroundMode( OPAQUE );

    // Draw the metafile to the screen
    Metafile.Draw( outputDC, &rect );

    // Do we wait for the time out?
    if( timeOutScreen )
    {
        // Setup our own timer that we can break out of if need be
        SleepTimer = timeGetTime() + SHAREWARE_MESSAGE_SLEEP;

        while( SleepTimer > timeGetTime() )
        {
            if( !SaverActive() )
            {
                Result = FALSE;
                break;
            }  
        }
    }

HANDLE_ERROR:;

    return Result;

}   // DisplayShareWareScreen


/************************************************************************
*    FUNCTION NAME:         RepaintScreen                                                             
*
*    DESCRIPTION:           Indicates a repaint of the whole screen is needed.
*                           Only for screen saver that don't refresh the whole
*                           screen on every cycle.
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

BOOL CScreenSaver::RepaintScreen()
{
    BOOL Result = FALSE;

    // Do we need to repaint the screen?
    if( pssData->RepaintScreen )
    {
        // Reset the varaibles back to false
        while( pssData->RepaintScreen )
            pssData->RepaintScreen = FALSE;

        Result = TRUE;
    }

    return Result;

} // RepaintScreen