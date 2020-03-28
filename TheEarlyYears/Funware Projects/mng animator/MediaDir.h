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
#include "TStoryBoardList.h" // List object for handling the list of animations
#include "TStrList.h"        // List object for storing lists of strings


// Function for creating the media sound directory dialog box
BOOL MediaDir_GetSoundDirectory( HINSTANCE hInst, HWND hwnd, char *SoundDir, HWND hList, PCSoundListAry pLst, PTStringList pSoundList );
// Hook Message handler function for common dialog box mods
UINT APIENTRY MediaDirDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );
// Call back for subclassing any of the controls procedure
LRESULT CALLBACK SndSubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam );


class TMediaDirDlg
{
protected:
    // Windows Media Player
    TMediaPlayer MediaPlayer;
    // Program instance
    HINSTANCE hInst;
    // Handle to parent dialog window
    HWND hParentWnd;
    // Storyboard array
    PCSoundListAry pSoundLst;
    // List of script paths
    PTStringList pSoundLstStr;
    // Handle to the frame list box
    HWND hFrameList;
    // Subclassed window control handles
    HWND hSubCtrl[ MAX_SND_SUBCLASS_CTRLS ];
    // Matching windows procedure for subclassed controls
    WNDPROC OldSubCtrlProc[ MAX_SND_SUBCLASS_CTRLS ];
    // Selected Index
    int activeIndex;
    // The sound directory
    char *pSoundDir;

    // Message handler for the volume edit control
    LRESULT OnEdit_VolumeCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for the channel edit control
    LRESULT OnEdit_ChannelCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for the sound list
    LRESULT OnLst_SoundCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );

    // Handle the list clicks
    void LstClk_SoundList( HWND hwnd, HWND hwndCtl, UINT codeNotify );

    // Add a sound to the list
    void BtnClk_AddSound( HWND hwnd );
    // Play the selected sound
    void BtnClk_PlaySound( HWND hwnd );

public:
    // Constructor
    TMediaDirDlg();
    // Destructor
    ~TMediaDirDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    BOOL CreateDialogWnd( HINSTANCE hInst, HWND hwnd, char *SoundDir, HWND hList, PCSoundListAry pLstAry, PTStringList pSoundList );
    // Called from the subclassed control
    LRESULT HandleSubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TMediaDirDlg *PTMediaDirDlg;


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