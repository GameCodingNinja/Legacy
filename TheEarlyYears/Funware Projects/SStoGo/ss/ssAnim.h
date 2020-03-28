
/************************************************************************
*
*    PRODUCT:         Screen Saver Thread
*
*    FILE NAME:       SSAnimate.h
*
*    DESCRIPTION:     Thread that runs the screen saver
*
*    IMPLEMENTATION:  This is screen saver specific. Here is where you 
*                     initilize your screen saver specific code, run it 
*                     and then free all allocated memory and resources. 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __SSANIMATE_H__
#define __SSANIMATE_H__


#include "TBitmap.h"         // Dib and Bitmap class for displaying and animating bitmaps
#include "DecodGif.h"        // GIF decoder which can load GIFs from file or resource
#include "SSBase.h"          // Header file for ancestor class
#include "MediaPly.h"        // Header file for media player
#include "Settings.h"        // User settings for this screen saver
#include "TStrList.h"        // Header file for the string list class.
#include "S2GStrct.h"        // Screen Savers To Go structure header file
#include "TJPEG.h"           // JPEG Library
#include "BldBuf.h"          // Functions for updating and refreshing the buffer
#include "PageList.h"        // Dynamic Allocation of pages
#include "TIntList.h"        // Manages a list of ints.

#define SMALLEST_TRANS_SIZE   4
#define PREVIEW_WND_INTERVAL  500

enum
{
    TIMER_TYPE_DELAY=0,
    TIMER_TYPE_GROUP,
};

class CAnimateSaver : public CScreenSaver
{
protected:

    /***********************************
    *  Screen saver specific Data Types
    ************************************/

    // Project structors
    TFileHeader FileHeader;
    TApplicationData AppData;
    TProjectPrefs ProjPrefsData;    
    TBitmap MainDib, BackDib, buffer;

    TPageList PageList;
    // GIF decoder which can load GIFs from file or resource
    TGif gif;
    // JPEG Decoder             
    TJPEG jpeg;

    /***********************************
    *  Non specific Data Types
    ************************************/

    DWORD MusicTimer;
    DWORD ShareWareTimer;
    TMediaPlayer MPlayer;
    int AnimCounter, SoundFileCount;
    int BlitCounter, BackFileCount;
    TIntList SoundList;
    TIntList BackList;
    int ScreenBitDepth;
    HPALETTE hOldPalette;
    LPPOINT TransPt;
    int TransArraySize;
    int TransCounter;
    int PtHeight, PtWidth;
    DWORD TransTimer;
    DWORD TransTimerInterval;
    BOOL TimerType;
    int TimerTypeCounter;
    DWORD PreviewTimer;
    DWORD MainTimer;
    // Screen saver settings
    TSettings Settings;


    /***********************************
    *  Screen saver specific functions
    ************************************/

    // Transition function
    BOOL TransToNextScreen();
    // Setup the transition
    void SetupTransition();             


    /***********************************
    *  Non Specific functions
    ************************************/

    // Animates the stars
    void Animate();
    // Blit all the animations to the buffer
    void BlitToTheBuffer();
    // Blit to the buffer from the screen
    void BlitFromBufferToScreen();
    // Play Music
    void PlayMusic();
    // If we are in shareware mode and it has timed out, show the shareware screen
    BOOL ShowShareWareScreen();
    // Let the user know there was an error
    void DisplayMusicError();
    // Pause the sound
    void PauseSound();
	// Load the graphics for the current page
	BOOL LoadPageGraphics();
    // Load the users prefferences that were selected in the config dialog
    virtual BOOL LoadControlPanelPrefs();
    // Loads the screen saver graphics
    virtual BOOL LoadGraphics();
    // Init screen saver data.
    virtual BOOL InitSaverData();
    // Init music player.
    virtual void InitMusicPlayer();

public:
    // Common Functions

    // Constructor
    CAnimateSaver( PTSaverModeData ssData );
    // Destructor - Put all your cleanup code here
    ~CAnimateSaver();
    // Run your animations
    void AnimateSaver();
    // First time display of the buffer to start the screen saver
    virtual void DisplayBufferOnStartUp();
};

typedef CAnimateSaver *PCAnimateSaver;

#endif  // __SSANIMATE_H__