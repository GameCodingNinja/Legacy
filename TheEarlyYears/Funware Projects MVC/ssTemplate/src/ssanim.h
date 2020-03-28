
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


#include "bitmap.h"          // Dib and Bitmap class for displaying and animating bitmaps
#include "DecodGif.h"        // GIF decoder which can load GIFs from file or resource
#include "ssBase.h"          // Header file for ancestor class
#include "MediaPly.h"        // Header file for media player
#include "Settings.h"        // User settings for this screen saver
#include "Defs.h"            // Screen saver defs file
#include <vector>
#include <string>


class CAnimateSaver : public CScreenSaver
{
protected:

    /***********************************
    *  Screen saver specific Data Types
    ************************************/

    // Image buffers
    CBitmap buffer;


    /***********************************
    *  Non specific Data Types
    ************************************/

    // Timer for main screen saver loop
    DWORD saverTimer;
    // Music check timer
    DWORD musicTimer;
    // Media player for music
    CMediaPlayer mPlayer;
    // Screen saver settings
    CSettings settings;
    // Old palette for 256 color mode
    HPALETTE hOldPalette;
    //Animation loop Counter
    int animCounter;
    // Preview mode counter to space out preview redraws
    int previewCounter;
    // Total number of loaded songs
    int songCount;
	// File name string list
	std::vector<string> stringLst;

    /***********************************
    *  Screen saver specific functions
    ************************************/



    /***********************************
    *  Non Specific functions
    ************************************/    

    // Animates the stars
    void Animate();
    // Erases the buffer
    void EraseTheBuffer();
    // Updates all the animation positions
    void UpdateAllAnimations();
    // Blit all the animations to the buffer
    void BlitToTheBuffer();
    // Blit to the buffer from the screen
    void BlitFromBufferToScreen();
    // Play Music
    void PlayMusic();
    // Let the user know there was an error
    void DisplayMusicError();
	// Pause the sound
	void PauseSound();
    // Load the users prefferences that were selected in the config dialog
    virtual bool LoadControlPanelPrefs();
    // Loads the screen saver graphics
    virtual bool LoadGraphics();
    // Init screen saver data.
    virtual bool InitSaverData();
    // Init music player.
    virtual void InitMusicPlayer();

public:

    // Constructor
    CAnimateSaver( PSSaverModeData ssData );
    // Destructor - Put all your cleanup code here
    ~CAnimateSaver();
    // Run your animations
    void AnimateSaver();
    // First time display of the buffer to start the screen saver
    virtual void DisplayBufferOnStartUp();
};

typedef CAnimateSaver *PCAnimateSaver;

#endif  // __SSANIMATE_H__