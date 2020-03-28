
/************************************************************************
*
*    PRODUCT:         Screen Saver Thread
*
*    FILE NAME:       SSAnim.cpp
*
*    DESCRIPTION:     Thread that runs the screen saver
*
*    IMPLEMENTATION:  This thread is screen saver specific. Here is where
*                     you initilize your screen saver, run it and then
*                     free all allocated memory and resources. 
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

#include "mintools.h"        // A misc group of useful functions
#include "ssAnim.h"          // Header file for this *.cpp file.
#include <stdlib.h>          // Declares several commonly used routines such as atoi, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf


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

CAnimateSaver::CAnimateSaver( PTSaverModeData ssData ) : CScreenSaver( ssData )
{
    // Init screen saver specific functions
    saverCounter = 0;
    saverRunning = false;
	currentSaverPath = NULL;

    // Init non-specific class members
    AnimCounter = 0;
    SaverTimer = 0;
    ShareWareMode = TRUE;
    ShareWareTimer = 0;
    MusicTimer = 0;
    SongCount = 0;
    hOldPalette = NULL;
    Error_Number = 0;
    PreviewCounter = 0;

}   // Constructor

/************************************************************************
*    FUNCTION NAME:         LoadControlPanelPrefs                                                             
*
*    DESCRIPTION:           Get the user control panel settings
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

BOOL CAnimateSaver::LoadControlPanelPrefs()
{    
    return TRUE;

}   // LoadControlPanelPrefs


/************************************************************************
*    FUNCTION NAME:         LoadGraphics                                                             
*
*    DESCRIPTION:           init buffers, load graphics etc.
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

BOOL CAnimateSaver::LoadGraphics()
{
    // Setup an animation for the preview screen
    if( Mode() == ssPreview )
    {
        // Init the classes using the screens DC. That way it doesn't have to make one
        buffer.InitActiveDC( GetScreenDC() );

        if( buffer.CreateStandardBuffer( WindowRect.right, WindowRect.bottom ) )
        {
            // Give the canvas class the active DC
            previewCanvas.InitActiveDC( buffer.Handle() );

            // Create the font
            previewCanvas.SetFont( "Arial", 12, FONT_BOLD, RGB( 120, 100, 255 ) );

            // Set the background mode to transparent for the text
            previewCanvas.SetBackgroundMode( TRANSPARENT );

            strcpy( previewTxt[0], "Howie's" );
            strcpy( previewTxt[1], "Saver" );
            strcpy( previewTxt[2], "Randomizer" );

            int tmpInt[2] = { 1, -1 };

            for( int i = 0; i < PREV_TXT_MAX; ++i )
            {
                GetTextMetrics( buffer.Handle(), &txtTm[i] );
                GetTextExtentPoint32( buffer.Handle(), previewTxt[i], strlen( previewTxt[i] ), &textSize[i] );

                x[i] = rand() % (WindowRect.right - textSize[i].cx);
                y[i] = rand() % (WindowRect.bottom - txtTm[i].tmHeight);

                vectorX[i] = tmpInt[ rand() % 2 ];
                vectorY[i] = tmpInt[ rand() % 2 ];
            }
        }
    }

    return true;

}   // LoadGraphics


/************************************************************************
*    FUNCTION NAME:         InitSaverData                                                             
*
*    DESCRIPTION:           Init screen saver data
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

BOOL CAnimateSaver::InitSaverData()
{    
    FillRect( GetScreenDC(), &WindowRect, BlackBrush );

    if( Mode() == ssSaver )
    {
        // Get the user's settings
        // Load all the ini file settings
        Settings.GetPrivateString();

        char saverListFile[MAX_PATH+1];
    
        // Get the windows directory
        GetTheWindowsDirectory( winDirStr, false );

        // Try to load the screen saver list file
        sprintf( saverListFile, SAVER_LIST_FILE, winDirStr );

        // See if we can load a file
        // We don't care if we error.
        StringList.LoadFromFile( saverListFile, OPEN_EXISTING );

        // Make sure we have something to animate
        if( StringList.Count() > 0 )
        {
            int tmp[] = { 1,2,3,5,10,15,20,25,30,45,60,120,180,240,300,0,0,0,0,0, };

            timeout = tmp[ Settings.GetTimeOut() ] * 60000;
            
            SaverTimer = timeGetTime() + timeout;

            if( Settings.GetRandomList() == true )
            {
                StringList.MixUp();
            }

            RunScreenSaver();
        }
        else
        {
            // Set the background mode to transparent for the text
            canvas.SetBackgroundMode( TRANSPARENT );

            canvas.DrawText( 5, 5, "You need to add screen savers to the 'List of Screen Savers To Play'" );
            canvas.DrawText( 5, 25, "in the Settings window of this screen saver." );
        
            // Set the background mode back to opaque
            canvas.SetBackgroundMode( OPAQUE );
        }
    }

    return true;

}   // InitSaverData


/************************************************************************
*    FUNCTION NAME:         InitMusicPlayer                                                             
*
*    DESCRIPTION:           Allocate the music player. If there is an error
*                           allocating the Windows Media player, we still
*                           allow the screen saver to run.
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

void CAnimateSaver::InitMusicPlayer()
{
}   // InitMusicPlayer


/************************************************************************
*    FUNCTION NAME:         DisplayBufferOnStartUp                                                             
*
*    DESCRIPTION:           First time display of the buffer to start
*                           the screen saver
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

void CAnimateSaver::DisplayBufferOnStartUp()
{
}   // DisplayBufferOnStartUp


/************************************************************************
*    FUNCTION NAME:         AnimateSaver                                                             
*
*    DESCRIPTION:           Animate the screen saver. This function is called
*                           by the thread.
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

void CAnimateSaver::AnimateSaver()
{
    // Check to see if we need to paint the rect the password dialog
    // box was at because it doesn't erase itself
    if( !PaintPasswordDialogRect() )
    {
        // Make sure the monitor is not asleep
        if( !GetMonitorIsAsleep() )
        {
            if( Mode() == ssSaver )
            {
                // Time out the screen saver only on the first ineration
                if( timeout != 0 && SaverTimer < timeGetTime() )
                {
                    Animate();
                }
                else
                {
                    // Check to see if our screen saver is still running. If it isn't
                    if( processInfo.hProcess != NULL && saverRunning == true )
                    {
                        DWORD exitCode = STILL_ACTIVE;
                        GetExitCodeProcess( processInfo.hProcess, &exitCode );

                        if( exitCode != STILL_ACTIVE )
                        {
                            MoveWindow( Handle(), 0, 0, DesktopWidth, DesktopHeight, false );
                            Sleep( 10 );

                            timeout = 0;
                            
                            PostMessage( Handle(), WM_CLOSE, 0, 0 );
                            saverRunning = false;
                        }
                    }

                    // Hand over our process. We don't need it.
                    Sleep(20);
                }
            }
            else if( Mode() == ssPreview )
            {
                if( SaverTimer < timeGetTime() )
                {
                    SaverTimer = timeGetTime() + 40;
                    AnimatePreview();
                }
            }
        }
    }
    else
    {
        // Resets the rect back to zero.
        ResetPasswordRect();
    }

}   // AnimateSaver


/************************************************************************
*    FUNCTION NAME:         PlayMusic                                                             
*
*    DESCRIPTION:           Plays the music that comes with this screen
*                           saver or play the users music
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

void CAnimateSaver::PlayMusic()
{
}   // PlayMusic


/************************************************************************
*    FUNCTION NAME:         DisplayMusicError                                                             
*
*    DESCRIPTION:           Let the user know there was an error
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

void CAnimateSaver::DisplayMusicError()
{
}   // DisplayMusicError


/************************************************************************
*    FUNCTION NAME:         Animate                                                             
*
*    DESCRIPTION:           Animates the stars. This is designed to have it
*                           do one thing at a time as it come through the
*                           function.
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

void CAnimateSaver::Animate()
{
    KillScreenSaver();
    RunScreenSaver();

}   // Animate


/************************************************************************
*    FUNCTION NAME:         RunScreenSaver                                                             
*
*    DESCRIPTION:           Run the screen saver.
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

void CAnimateSaver::RunScreenSaver()
{
    char saverPath[MAX_PATH+1];

    if( SaverActive() && StringList.Count() > 0 )
    {
        memset( &startInfo, 0, sizeof(startInfo) );
        memset( &processInfo, 0, sizeof(processInfo) );

        startInfo.cb = sizeof(startInfo);
        startInfo.dwFlags = STARTF_USESHOWWINDOW;
        startInfo.wShowWindow = SW_SHOWNORMAL;

        // Get the pointer to the screen saver to load
		currentSaverPath = StringList.GetString( saverCounter );

        sprintf( saverPath, "%s /S", currentSaverPath );

        if( CreateProcess( NULL, saverPath, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startInfo, &processInfo ) )
        {
            // Wait for the new process to init itself
            WaitForInputIdle( processInfo.hProcess, 5000 );
            
            SaverTimer = timeGetTime() + timeout;
            saverRunning = true;

            MoveWindow( Handle(), -10, 0, 5, 5, false );
        }
        else  // Error starting screen saver
        {
            MoveWindow( Handle(), 0, 0, DesktopWidth, DesktopHeight, false );
            FillRect( GetScreenDC(), &WindowRect, BlackBrush );
            
            // Set the background mode to transparent for the text
            canvas.SetBackgroundMode( TRANSPARENT );

            char saverStr[100];
            char errorString[300];

            // Build the error text
            CropFileNameFromPath( currentSaverPath, saverStr, false );
            sprintf( errorString, "ERROR: Unable to run screen saver %s", saverStr );

            // Draw the text
            canvas.DrawText( 5, 5, errorString );

            // Set the background mode back to opaque
            canvas.SetBackgroundMode( OPAQUE );

            saverRunning = false;
            MessageBeep(0);
			SaveErrorFile( "Will Not Start" );
            Sleep(5000);

            MoveWindow( Handle(), -10, 0, 5, 5, false );
        }

        // Inc to the next saver
        saverCounter = (saverCounter + 1) % StringList.Count();

        if( saverCounter == 0 && Settings.GetRandomList() == true )
        {
            StringList.MixUp();
        }
    }

}   // RunScreenSaver


/************************************************************************
*    FUNCTION NAME:         KillScreenSaver                                                             
*
*    DESCRIPTION:           Kill the screen saver.
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

void CAnimateSaver::KillScreenSaver()
{
    DWORD counter=0;

    if( processInfo.hProcess != NULL )
    {
        DWORD exitCode = STILL_ACTIVE;

        // Move the window so that it is behind the current screen saver
        MoveWindow( Handle(), 0, 0, DesktopWidth, DesktopHeight, false );

        // Send the thread ID the message
        EnumThreadWindows( processInfo.dwThreadId, EnumThreadProc, BUTTON_CLICK );

        WaitForSingleObject( processInfo.hProcess, 5000 );
        GetExitCodeProcess( processInfo.hProcess, &exitCode );

        // Wait until the process exits
        while( exitCode == STILL_ACTIVE && SaverActive() )
        {
            WaitForSingleObject( processInfo.hProcess, 1000 );
            GetExitCodeProcess( processInfo.hProcess, &exitCode );

            // Enumerate the thread windows to send it messages
			if( exitCode == STILL_ACTIVE && counter == 5 )
            {
                // Send the thread ID the message
                EnumThreadWindows( processInfo.dwThreadId, EnumThreadProc, MOUSE_CLICK );
            }
            else if( exitCode == STILL_ACTIVE && counter == 10 )
            {
                // Send the thread ID the message
                EnumThreadWindows( processInfo.dwThreadId, EnumThreadProc, CLOSE_APP );                     
            }
			else if( exitCode == STILL_ACTIVE && counter == 15 )
			{
				SaveErrorFile( "Will Not Exit" );
			}

            ++counter;
        }

        FillRect( GetScreenDC(), &WindowRect, BlackBrush );

        processInfo.hProcess = NULL;

        saverRunning = false;
    }

}   // KillScreenSaver


/************************************************************************
*    FUNCTION NAME:         SaveErrorFile                                                             
*
*    DESCRIPTION:           Save screen saver name to error file.
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

void CAnimateSaver::SaveErrorFile( char *error )
{
	char pathToFile[MAX_PATH];
	char pathToSaver[MAX_PATH];

	// Build the path to the error file
    sprintf( pathToFile, ERROR_LIST_FILE, winDirStr );
	sprintf( pathToSaver, "%s - %s\r\n", error, currentSaverPath );

	WriteDataToFile( pathToFile, pathToSaver, strlen(pathToSaver) );

}	// SaveErrorFile


/************************************************************************
*    FUNCTION NAME:         EraseTheBuffer                                                             
*
*    DESCRIPTION:           Erases the buffer in prep for the next 
*                           animation round
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

void CAnimateSaver::EraseTheBuffer()
{

}   // EraseTheBuffer


/************************************************************************
*    FUNCTION NAME:         UpdateAllAnimations                                                             
*
*    DESCRIPTION:           Updates all the animation positions
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

void CAnimateSaver::UpdateAllAnimations()
{

}   // UpdateAllAnimations


/************************************************************************
*    FUNCTION NAME:         BlitToTheBuffer                                                             
*
*    DESCRIPTION:           Blit all the animations to the buffer
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

void CAnimateSaver::BlitToTheBuffer()
{

}   // BlitToTheBuffer


/************************************************************************
*    FUNCTION NAME:         BlitFromBufferToScreen                                                             
*
*    DESCRIPTION:           Blit to the buffer from the screen
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

void CAnimateSaver::BlitFromBufferToScreen()
{
}   // BlitFromBufferToScreen


/************************************************************************
*    FUNCTION NAME:         ShowShareWareScreen                                                             
*
*    DESCRIPTION:           If we are in shareware mode and it has timed
*                           out, show the shareware screen
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

BOOL CAnimateSaver::ShowShareWareScreen()
{
    BOOL Result = TRUE;

    return Result;

}   // ShowShareWareScreen


/************************************************************************
*    FUNCTION NAME:         Destructor                                                             
*
*    DESCRIPTION:           Free up all allocated resources.
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

CAnimateSaver::~CAnimateSaver()
{
            
}   // Destructor


/************************************************************************
*    FUNCTION NAME:         PauseSound                                                             
*
*    DESCRIPTION:           Pause the sound
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

void CAnimateSaver::PauseSound()
{

}   // PauseSound


/************************************************************************
*    FUNCTION NAME:         AnimatePreview                                                             
*
*    DESCRIPTION:           Animate the preview
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

void CAnimateSaver::AnimatePreview()
{
    if( buffer.Handle() != 0 )
    {
        FillRect( buffer.Handle(), &WindowRect, BlackBrush );

        for( int i = 0; i < PREV_TXT_MAX; ++i )
        {
            if( x[i] <= 0 )
                vectorX[i] = 1;

            if( y[i] <= 0 )
                vectorY[i] = 1;
            
            if( x[i] + textSize[i].cx >= WindowRect.right )
                vectorX[i] = -1;
            
            if( y[i] + txtTm[i].tmHeight >= WindowRect.bottom )
                vectorY[i] = -1;

            x[i] += vectorX[i];
            y[i] += vectorY[i];

            previewCanvas.DrawText( x[i], y[i], previewTxt[i] );
        }

        buffer.Draw( GetScreenDC() );
    }
}   // AnimatePreview


/************************************************************************
*    FUNCTION NAME:         EnumThreadProc                                                             
*
*    DESCRIPTION:           Callback function to cycle through all the
*                           windows of the process. We'll send a message to
*                           each window in the process in the hopes we'll
*                           kill the screen saver.
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
*                  7/4/04        Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CALLBACK EnumThreadProc( HWND hwnd, LPARAM lParam )
{
    if( lParam == BUTTON_CLICK )
    {
        PostMessage( hwnd, WM_KEYDOWN, ESCAPE, 0 );
        PostMessage( hwnd, WM_KEYUP, ESCAPE, 0 );
    }
    else if( lParam == MOUSE_CLICK )
    {
        PostMessage( hwnd, WM_LBUTTONDOWN, 0, 0 );
        PostMessage( hwnd, WM_LBUTTONUP, 0, 0 );
    }
    else if( lParam == CLOSE_APP )
    {
        PostMessage( hwnd, WM_CLOSE, 0, 0 );
    }

    return true;

}   // EnumThreadProc