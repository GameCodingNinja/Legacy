
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

    // Dirty Image buffers
    TBitmap dirtyBuffer;
    // Clean Image buffers
    TBitmap cleanBuffer;
    // Screen saver artwork
    TBitmap snowArt;
    // The count of rows needed to tile across the bottom
    int rowCount;
    // Total count of blinkers
    int BlinkerCount;
	// Total number of smoke animations
	int SmokeCount;
    // Blinker Pointer
    PTBlinker pBlinker;
    // Pointer to snow data
    PTSnow pSnow;
	// Smoke pointer
	PSSmoke pSmoke;
    // Count of all snow
    int snowCount;
    // people structors
    SPeople people[MAX_PEOPLE_ANIM];


    /***********************************
    *  Non specific Data Types
    ************************************/

    // Timer for main screen saver loop
    DWORD SaverTimer;
    // Shareware nag screen timer
    DWORD ShareWareTimer;
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
    // Timer for stating the people animations
    DWORD PeoplePickTimer;
	// Smoke Timer
	DWORD SmokeTimer;
	// Santa Timer
	DWORD SantaTimer;
	// Santa data
	SSanta santa;

    /***********************************
    *  Screen saver specific functions
    ************************************/

    // Build the background
    void BuildBackground();
    // Setup the blinking houses, trees and star
    void SetupBlinkers();
    // Setup the snow
    BOOL SetupSnow();
    // Setup people
    void SetupPeople();
    // Update all the people positions that are active
    void UpdatePeoplePositions( DWORD tmpTimer );
    // Picks the next person to animate
    void PickNextPersonToAnimate( DWORD tmpTimer );
    // Updates all the blinkers
    void UpdateBlinkers( DWORD tmpTimer );
    // Updates all the snow
    void UpdateSnow( DWORD tmpTimer );
    // Sort all the people by height pos in the screen
    void Sort();
    // Blit the blinkers
    void BlitBlinkersToTheBuffer();
    // Blit the people to the buffer
    void BlitPeopleToTheBuffer();
    // Blit the snow to the buffer
    void BlitSnowToTheBuffer();
    // Blit the people to the screen
    void BlitPeopleToScreen();
    // Blit the snow to the screen
    void BlitSnowToScreen();
    // Blit the changed blinkers to the screen
    void BlitChangedBlinkersToScreen();
	// Setup the smoke
	void SetupSmoke();
	// Blit the smoke to the buffer
	void BlitSmokeToTheBuffer();
	// Blit the smoke to the screen
	void BlitSmokeToScreen();
	// Setup the santa
	void SetupSanta();
	// Update Santa
	void UpdateSanta( DWORD tmpTimer );
	// Blit the santa to the buffer
	void BlitSantaToTheBuffer();
	// Blit the Santa to the screen
	void BlitSantaToScreen();


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