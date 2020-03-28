
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
#include "Settings.h"        // User settings for this screen saver
#include "SSBase.h"          // Header file for ancestor class
#include "Defs.h"            // Screen saver defs file
#include "SoundMgr.h"		 //	Loads and plays wave files


class CAnimateSaver : public CScreenSaver
{
protected:

    /***********************************
    *  Screen saver specific Data Types
    ************************************/

    // Image buffers
    TBitmap buffer;
	// Art buffer
	TBitmap bubbleArt;
	// Bubble data
	PTBubble bubble;
	// Bubble count
	int bubbleCount;
	// Resource art rects
	PRECTS pBubbleArt, pPopArt;
	// Loaded sound resources
	CSoundMgr soundMgr;
	// Pop Timer
	DWORD popTimer;
	// Pop delay
	DWORD MinPopDelay;


    /***********************************
    *  Non specific Data Types
    ************************************/

    // Timer for main screen saver loop
    DWORD SaverTimer;
    // Speed of main screen saver loop
    DWORD SaverSpeed;
    // Shareware mode flag
    BOOL ShareWareMode;
    // Shareware nag screen timer
    DWORD ShareWareTimer;
    // Screen saver settings
    TSettings Settings;
    // Old palette for 256 color mode
    HPALETTE hOldPalette;
    //Animation loop Counter
    int AnimCounter;
    // Preview mode counter to space out preview redraws
    int PreviewCounter;

    /***********************************
    *  Screen saver specific functions
    ************************************/

	// Set the bubble to animate
	void SetBubble( int Index );
	// Animate the bubble to bounce
	BOOL Bubble_Bounce( int Index );
	// Animate the bubble to float
	BOOL Bubble_Float( int Index );
	// Set the bubble to pop
	void Bubble_Pop( int Index, DWORD tmpTimer );
	// Set the bubble to float from the bottom up
	BOOL Bubble_Champagne( int Index );


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
    // If we are in shareware mode and it has timed out, show the shareware screen
    BOOL ShowShareWareScreen();
    // Let the user know there was an error
    void DisplaySoundError();
    // Load the users prefferences that were selected in the config dialog
    virtual BOOL LoadControlPanelPrefs();
    // Loads the screen saver graphics
    virtual BOOL LoadGraphics();
    // Init screen saver data.
    virtual BOOL InitSaverData();
    // Loads the screen saver sound effects
    virtual BOOL LoadSound();

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