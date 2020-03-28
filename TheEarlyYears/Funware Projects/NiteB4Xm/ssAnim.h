
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
#include "TIntList.h"        // Int list object
#include "Metafile.h"        // Class for loading and displaying Windows metafiles and enhanced metafiles


class CAnimateSaver : public CScreenSaver
{
protected:
    /***********************************
    *  Screen saver specific Data Types
    ************************************/

    // Image buffers
    TBitmap dirtyBuffer;
    // Image buffers
    TBitmap cleanBuffer;
    // Fire animation counter
    int fireAnimCounter;
    // Tree light animation counter
    int treeAnimCounter;
    // Santa eyes counter
    int santaEyesCounter;
    // Santa feet counter
    int santaFeetCounter;
    // Cat sleep counter
    int catCounter;
    // Mouse walk counter
    int mouseWalkCounter;
    // Index of the selected present
    int presentIndex;
    // Fire animation metafiles
    TMetafile metaFire[ MAX_FIRE_ANIM ];
    // Tree light animation metafiles
    TMetafile metaTreeLights[ MAX_TREE_LIGHT_ANIM ];
    // Santa eyes animation metafiles
    TMetafile santaEyes[ MAX_SANTA_EYES_ANIM ];
    // Santa legs animation metafiles
    TMetafile santaLegs[ MAX_SANTA_LEGS_ANIM ];
    // Cat sleep animation metafiles
    TMetafile catSleep[ MAX_CAT_SLEEP_ANIM ];
    // Cat yawn animation metafiles
    TMetafile catYawn[ MAX_CAT_YAWN_ANIM ];
    // Cat eye open animation metafiles
    TMetafile catEyeOpen[ MAX_CAT_EYEOPEN_ANIM ];
    // Mouse walk animation metafiles
    TMetafile mouseWalk[ MAX_MOUSE_WALK_FRAMES ];
    // Present Art metafiles
    TMetafile presents[ MAX_PRESENTS ];
    // Fire animation Timer
    DWORD fireTimer;
    // Tree animation Timer
    DWORD treeTimer;
    // Santa eyes Time
    DWORD santaEyesTimer;
    // Santa feet Time
    DWORD santaFeetTimer;
    // Cat Sleep Timer
    DWORD catSleepTimer;
    // Mouse walk timer
    DWORD mouseWalkTimer;
    // Mouse and gift move rect
    RECT moveRect;
    // Number of grid pieces per screen width and height
    int gridWidth;
    int gridHeight;
    // Movement X counter
    int MoveX;
    // Flip Flop the way the mouse is walking
    int mouseDirection;
    // The presents int list
    TIntList presList;
    // The state the cat is in
    int catState;


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

    /***********************************
    *  Screen saver specific functions
    ************************************/

    // Load the metafile animation  into the metafile array
    BOOL LoadMetaFileAnim( PTMetafile pMetaFile, char *ResName, int noOfAnim );


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