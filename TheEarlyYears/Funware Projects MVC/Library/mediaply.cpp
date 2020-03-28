
/************************************************************************
*
*    PRODUCT:         The Windows Media Player
*
*    FILE NAME:       mediaply.cpp
*
*    DESCRIPTION:     This class is used mainly if you want your program
*                     to play media files be it mid, wav, MP3 or what ever
*                     Windows supports. Also, if your program has music
*                     files located in a resource, you could load the resource
*                     and save it to a file and play it from there.
*
*    IMPLEMENTATION:  The constructure takes a handle to the main window
*                     and makes an invisible mediaplayer there. You then load
*                     and play files with this hidden child window. Trust me
*                     this is the easyest way to handle this. Once the window
*                     is created, you then play midi or wav files.
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
                             // Note: we are not using the #define WIN32_LEAN_AND_MEAN because we need multimedia MCI features
#include <stdlib.h>          // Declares several commonly used routines such as atoi, random, abs, malloc etc.
#include "mediaply.h"        // Header file for this *.cpp file.
#include "minTools.h"        // A misc group of useful functions
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf


// This quiets the compiler warnings about a bunch of functions that Microsoft has
// decided to deprecate, even though the C++ standardization committee has not done so.
#pragma warning(disable : 4996)

/************************************************************************
*    FUNCTION NAME:              Constructor                                                             
************************************************************************/
CMediaPlayer::CMediaPlayer()
{
    ErrorString[0] = 0;
    ResLoadedTmpFile[0] = 0;
    PlayError = false;
    hMediaWnd = NULL;

}   // Constructor


/************************************************************************
*    FUNCTION NAME:              Destructor                                                             
************************************************************************/
CMediaPlayer::~CMediaPlayer()
{
    if( hMediaWnd )
    {
        if( GetStatus() != PLAYER_CLOSED )
            Close();

        hMediaWnd = NULL;
    }

    DeleteResLoadedTmpFile();

}   // Destructor


/************************************************************************
*    DESCRIPTION:           Delete the resource loaded file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
************************************************************************/
bool CMediaPlayer::Init( HWND hwnd, HINSTANCE hInst, bool ShowErrorDlg, DWORD Flags )
{    
    PlayError = false;

    // Save the instance
    hInstance = hInst;

    // Setup the default path and name to the file created from the resource
    GetTheWindowsDirectory( ResFilePath );

    // Specify the media play with flags
    if( Flags )
    {
        hMediaWnd = MCIWndCreate( hwnd, hInst, Flags, NULL );
    }
    else
    {
        // Create a hidden media window with or without the error dialog boxes
        if( ShowErrorDlg )
            hMediaWnd = MCIWndCreate( hwnd, hInst, 
                    MCIWNDF_NOOPEN|MCIWNDF_NOPLAYBAR|MCIWNDF_NOMENU|MCIWNDF_NOAUTOSIZEWINDOW|MCIWNDF_NOAUTOSIZEMOVIE, NULL );
        else
            hMediaWnd = MCIWndCreate( hwnd, hInst, 
                    MCIWNDF_NOOPEN|MCIWNDF_NOERRORDLG|MCIWNDF_NOPLAYBAR|MCIWNDF_NOMENU|MCIWNDF_NOAUTOSIZEWINDOW|MCIWNDF_NOAUTOSIZEMOVIE, NULL );
    }

    // Did we get our MCI window?
    if( !hMediaWnd )
    {
        strcpy( ErrorString, "Error initilizing Windows Media Player." );
        PlayError = true;
    }

    return !PlayError;

}   // Init


/************************************************************************
*    DESCRIPTION:           Delete the resource loaded file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
************************************************************************/
void CMediaPlayer::DeleteResLoadedTmpFile()
{
    if( ResLoadedTmpFile[0] )
    {
        DeleteFile( ResLoadedTmpFile );
        ResLoadedTmpFile[0] = 0;
    }

}   // DeleteResLoadedTmpFile


/************************************************************************
*    DESCRIPTION:           Loads a file and plays it. The MCI functions
*                           return 0 if successful.
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName    - Path to file name
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
************************************************************************/
bool CMediaPlayer::Play( char *FileName )
{
    // Do we have a handle to the MCI window?
    if( hMediaWnd && !PlayError )
    {
		// Close the media device if it is open.
        // On some computers, if the device is not closed, windows
        // doesn't release the file so that is can be over written.
        Close();

        // Can we open this file
        if( Load( FileName ) )
        {
            // An internal error can be caused because the windows media player
            // is already in use. This type of error doesn't require an error to be known
            // Was there an error?
            if( MCIWndPlay( hMediaWnd ) != 0 )
            {
                // Only show the file name if it is there files and not the resource
                if( strcmp( FileName, ResFilePath ) != 0 )
                    sprintf( ErrorString, "ERROR PLAYING: %s", FileName );

                PlayError = true;
            }
        }           
    }

    return !PlayError;

}   // Play


/************************************************************************
*    DESCRIPTION:           Loads a file and makes it ready to play.
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName    - Path to file name
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
************************************************************************/
bool CMediaPlayer::Load( char *FileName )
{
    bool Result = false;

    // Do we have a handle to the MCI window?
    if( hMediaWnd && !PlayError )
    {
        // Can we open this file
        if( MCIWndOpen( hMediaWnd, FileName, 0 ) == 0 )
        {
            // Test to see if we can play this file
            // Play the file
            if( MCIWndCanPlay( hMediaWnd ) )
            {
                Result = true;
            }
        }
        
        // Was there an error?
        if( !Result )
        {
            // Only show the file name if it is there files and not the resource
            if( strcmp( FileName, ResFilePath ) != 0 )
                sprintf( ErrorString, "ERROR LOADING: %s", FileName );

            PlayError = true;
        }           
    }

    return !PlayError;

}   // Load


/************************************************************************
*    DESCRIPTION:           Loads a midi file from the resource by coping
*                           the file to the hard drive.
*
*    FUNCTION PARAMETERS:
*    Input:    char *name        - Name of the resource
*              char *type        - Resource Type Id
*
*    Output:   returntype - Description
************************************************************************/
bool CMediaPlayer::Load( char *name, char *type, char *tmpFileName )
{
    if( hMediaWnd && !PlayError )
    {
        if( tmpFileName )
        {
            // Copy over the file path
            strcpy( ResLoadedTmpFile, ResFilePath );

            // Add the file name to the path
            strcat( ResLoadedTmpFile, tmpFileName );
        }
        else
        {
            // Copy over the file path
            strcpy( ResLoadedTmpFile, ResFilePath );

            // Add the file name to the path
            strcat( ResLoadedTmpFile, DEFAULT_RES_FILE_NAME );
        }

        // Save the file to disk
        if( SaveResourceToDisk( hInstance, name, type, ResLoadedTmpFile ) )
        {
            if( !Load( ResLoadedTmpFile ) )
            {
                strcpy( ErrorString, "Error playing default music. There may be another application playing the same type of music file." );
                PlayError = true;
            }
        }
        else
            PlayError = true;
    }

    return !PlayError;

}   // Load


/************************************************************************
*    DESCRIPTION:           Loads a midi file from the resource by coping
*                           the file to the hard drive.
*
*    FUNCTION PARAMETERS:
*    Input:    char *name        - Name of the resource
*              char *type        - Resource Type Id
*
*    Output:   returntype - Description
************************************************************************/
bool CMediaPlayer::Play( char *name, char *type, char *tmpFileName )
{
    // Save the file to disk
    if( hMediaWnd && !PlayError )
    {
        // Close the media device if it is open.
        // On some computers, if the device is not closed, windows
        // doesn't release the file so that is can be over written.
        Close();

		// Load from resource and save to the hard drive
		if( Load( name, type, tmpFileName )	)
        {
	        if( !Play() )
	        {
	            strcpy( ErrorString, "Error playing default music. There may be another application playing the same type of music file." );
	            PlayError = true;
	        }
		}
    }

    return !PlayError;

}   // play


/************************************************************************
*    DESCRIPTION:           Just plays the currently loaded file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var2Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
************************************************************************/
bool CMediaPlayer::Play()
{
    bool Result = false;

    // Do we have a handle to the MCI window?
    if( hMediaWnd && !PlayError )
    {
        // An internal error can be caused because the windows media player
        // is already in use. This type of error doesn't require an error to be known
        if( MCIWndPlay( hMediaWnd ) == 0 )
            Result = true;           

        // Was there an error?
        if( !Result )
        {
            strcpy( ErrorString, "Error playing sound file. There may be another application playing the same type of music file." );

            PlayError = true;
        }
    }

    return !PlayError;

}   // Play


/************************************************************************
*    DESCRIPTION:           Pause the currently playing sound fileThe MCI
*                           functions return 0 if successful.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var2Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
************************************************************************/
bool CMediaPlayer::Pause()
{
    // Do we have a handle to the MCI window?
    if( hMediaWnd && !PlayError )
    {
        if( MCIWndPause( hMediaWnd ) != 0 )
            PlayError = true;
    }

    return !PlayError;

}   // Pause


/************************************************************************
*    DESCRIPTION:           Closes the currently playing sound fileThe MCI
*                           functions return 0 if successful.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var2Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
************************************************************************/

bool CMediaPlayer::Close()
{
    bool Result = false;

    // Do we have a handle to the MCI window?
    if( hMediaWnd )
    {
        if( MCIWndClose( hMediaWnd ) == 0 )
            Result = true;
    }

    return Result;

}   // Pause


/************************************************************************
*    DESCRIPTION:           Returns the status of the currently playing device
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var2Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
************************************************************************/
int CMediaPlayer::GetStatus()
{
    int Status = 0;

    // Do we have a handle to the MCI window?
    // The character buffer holds a string that says what the media player is
    // doing, like "Playing" or "Stopped"
    if( hMediaWnd )
        Status = MCIWndGetMode( hMediaWnd, ModeStr, sizeof(ModeStr) );

    return Status;

}   // GetStatus


/************************************************************************
*    DESCRIPTION:           Stop the media player
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var2Name - Description
*              var2type Var2Name - Description
************************************************************************/
void CMediaPlayer::Stop()
{
    if( hMediaWnd )
    { 
        // Stop the play back
        MCIWndStop( hMediaWnd );

        // Rewind it to the beginning
        MCIWndSeek( hMediaWnd, MCIWND_START );
    }

}   // Stop


/************************************************************************
*    DESCRIPTION:           Set media player to repeat
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var2Name - Description
*              var2type Var2Name - Description
************************************************************************/
void CMediaPlayer::SetRepeat( bool Value )
{
    if( hMediaWnd ) 
        MCIWndSetRepeat( hMediaWnd, Value );

}   // SetRepeat