
/************************************************************************
*
*    PRODUCT:         The Windows Media Player
*
*    FILE NAME:       mediaply.h
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


#ifndef __MEDIAPLAYER_H__
#define __MEDIAPLAYER_H__

#include <vfw.h>             // All the stuff needed for the MCI window

#define PLAYER_CLOSED     MCI_MODE_NOT_READY
#define PLAYER_OPENED     MCI_MODE_OPEN
#define PLAYER_PAUSED     MCI_MODE_PAUSE
#define PLAYER_PLAYING    MCI_MODE_PLAY
#define PLAYER_RECORDING  MCI_MODE_RECORD
#define PLAYER_SEEKING    MCI_MODE_SEEK
#define PLAYER_STOPPED    MCI_MODE_STOP

#define USE_ERROR_DLG_BOXES         TRUE
#define DONT_USE_ERROR_DLG_BOXES    FALSE

#define DEFAULT_RES_FILE_NAME  "HowiesFunwareSong.mid"

class TMediaPlayer
{
protected:
    HWND hMediaWnd;
    // Used for creating a default file path for loading
    // midi files from the resource and saving them to the hard drive
    char ResFilePath[MAX_PATH+1];
    char ResLoadedTmpFile[MAX_PATH+1];
    // The character buffer holds a string that says what the
    // media player is doing, like "Playing" or "Stopped".
    char ModeStr[50];
    HINSTANCE hInstance;
    char ErrorString[MAX_PATH];
    BOOL PlayError;

public:
    // Constructor
    TMediaPlayer();
    // Destructor
    ~TMediaPlayer();
    // init the media player
    BOOL Init( HWND hwnd, HINSTANCE hInst, BOOL ShowErrorDlg = TRUE, DWORD Flags = 0 );
    // PLay a sound file
    BOOL Play( char *FileName );
    // Plays a midi file from the resource
    BOOL Play( char *name, char *type, char *tmpFileName = NULL );
    // Just plays the currently loaded file
    BOOL Play();
    // Loads a file and makes it ready to play
    BOOL Load( char *FileName );
    // Loads a file from resource and makes it ready to play
    BOOL Load( char *name, char *type, char *tmpFileName = NULL );
    // Pauses the currently loaded file
    BOOL Pause();
    // Stop the play of a sound file
    BOOL Close();
    // Get the status of the active device
    int GetStatus();
    // Returns the mode string
    char *GetModeString(){ return ModeStr; };
    // Return the error string
    char *GetErrorStr(){ return ErrorString; };
    // Check to see if there is an internal error.
    // Usually caused because the device is already in use
    BOOL GetPlayError(){ return PlayError; };
    // Get the handle to the media player
    HWND GetHandle(){ return hMediaWnd; };
    // Reste the play error
    void ResetPlayError(){ PlayError = FALSE; ErrorString[0] = 0; };
    // Set media player to repeat
    void SetRepeat( BOOL Value = TRUE );
    // Stop playing
    void Stop();
    // Delete the resource loaded file
    void DeleteResLoadedTmpFile();
    // Free the player window
    void Free(){ if( hMediaWnd ){ Close(); MCIWndDestroy( hMediaWnd ); hMediaWnd = NULL; } };
};

typedef TMediaPlayer *PTMediaPlayer;


#endif  /* __MEDIAPLAYER__ */