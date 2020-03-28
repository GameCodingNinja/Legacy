/************************************************************************
*
*    PRODUCT:         Screen saver
*
*    FILE NAME:       MediaDir.h
*
*    DESCRIPTION:     Screen saver Media directory dialog box
*
*    IMPLEMENTATION:  This is not a full dialog box but an extension to the
*                     common file open dialog box. The end result a string
*                     to the directory containing sound files.
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __MEDIADIR_H__
#define __MEDIADIR_H__

#include "mediaply.h"        // Header file for media player

// Function for creating the media sound directory dialog box
bool MediaDir_GetSoundDirectory( HINSTANCE hInst, HWND hwnd, char *SoundDir, char *GetFileName = NULL );
// Hook Message handler function for common dialog box mods
UINT APIENTRY MediaDirDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );


class CMediaDirDlg
{
protected:
    // Windows Media Player
    CMediaPlayer MediaPlayer;
    // Program instance
    HINSTANCE hInst;

public:
    // Constructor
    CMediaDirDlg();
    // Destructor
    ~CMediaDirDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    bool OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    bool CreateDialogWnd( HINSTANCE hInst, HWND hwnd, char *SoundDir, char *GetFileName );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef CMediaDirDlg *PCMediaDirDlg;


/*
DLG_MEDIA_DIR DIALOG 0, 0, 262, 35
STYLE DS_3DLOOK | DS_CONTROL | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS
FONT 8, "MS Sans Serif"
{
 CONTROL "Test Selected Sound File", BTN_PLAY_SOUND, "BUTTON", BS_PUSHBUTTON | BS_CENTER | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 54, 16, 155, 14
 CONTROL "Be sure to test each sound file before using it with the program", TXT_MEDIA_DIR, "static", SS_CENTER | WS_CHILD | WS_VISIBLE, 8, 3, 248, 10, 0
}
*/


#endif  /* __MEDIADIR__ */