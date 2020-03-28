
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
#include "ssBase.h"          // Header file for ancestor class
#include "MediaPly.h"        // Header file for media player
#include "Settings.h"        // User settings for this screen saver
#include "TStrList.h"        // Header file for the string list class.
#include "Defs.h"            // Screen saver defs file


class CAnimateSaver : public CScreenSaver
{
protected:

    /***********************************
    *  Screen saver specific Data Types
    ************************************/
    
    // Tracks the number of fish currently animating
    int NumFishAnim;
    // Max number of fish to animate at one time
    int FishCount;
    // Max number of animating background fish
    int BackFishCount;
    // Max number of animating bubbles
    int BubbleCount;
    // Allocated fish animation array
    TFish *fish;
    // Allocated background fish animation array
    TFish *backFish;
    // Timer to pick the next fish if we are short
    DWORD FishPickTimer;
    // Timer to bring the boss fish in
    DWORD BossFishTimer;
    // Allocated bubble animation array
    PTBubble pBubble;
    // Resource loaded float data
    short *fishFloatData;
    // Resource loaded fish data
    PTFishDataArry pFishDataAry;
    // Image buffers
    TBitmap buffer, backBuffer, FishArt;
    // TCanvas for blitting the text
    TCanvas bufferCanvas;

    /***********************************
    *  Non specific Data Types
    ************************************/

    // Timer for main screen saver loop
    DWORD SaverTimer;
    // Music check timer
    DWORD MusicTimer;
    // Media player for music
    TMediaPlayer MPlayer;
    // String list for sound files
    TStringList StringList;
    // Screen saver settings
    TSettings Settings;
    // Old palette for 256 color mode
    HPALETTE hOldPalette;
    //Animation loop Counter
    int AnimCounter;
    // Preview mode counter to space out preview redraws
    int PreviewCounter;
    // Total number of loaded songs
    int SongCount;
    // Width of the first monitor if we have duel monitors
    int firstMonitorWidth;

    /***********************************
    *  Screen saver specific functions
    ************************************/

    // Init the fish data
    BOOL InitFishData();
    // Init the background fish data
    BOOL InitBackFishData();
    // Init the fish data
    BOOL InitBubbleData();
    // Update all the fish positions that are active
    void UpdateFishPositions();
    // Update all the background fish positions
    void UpdateBackFishPositions();
    // Update all the bubble positions
    void UpdateBubblePositions();
    // Picks the next fish to animate
    void PickNextFishToAnimate();
    // Setup the background with a solid color
    void BackSolidColor();
    // Is it time to bring the boss back in?
    void InitBossFish();
    // Update the positions of the boss fish
    void UpdateBossFishPos();
    // Build the background
    BOOL BuildBackground();


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
    virtual BOOL LoadControlPanelPrefs();
    // Loads the screen saver graphics
    virtual BOOL LoadGraphics();
    // Init screen saver data.
    virtual BOOL InitSaverData();
    // Init music player.
    virtual void InitMusicPlayer();

public:

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