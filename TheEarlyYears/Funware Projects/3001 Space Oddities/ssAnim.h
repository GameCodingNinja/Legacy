
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
#include "starDefs.h"         // Star screen saver defs file


class CAnimateSaver : public CScreenSaver
{
protected:

    /***********************************
    *  Screen saver specific Data Types
    ************************************/

    // Image buffers
    TBitmap buffer, stars, cloudBuffer, ships;
    // Star data pointer
    PTStar pStarData;
    // Planet data array
    TPlanet PlanetData[ NUM_PLANETS ];
    // Ship data array
    TShip ShipData[ NUM_SHIPS ];
    // Number of stars
    int StarCount;
    // Planet timer
    DWORD PlanetPickTimer;
    // Ship Timer
    DWORD ShipPickTimer;
    // Number of planets to animate
    int NumPlanetsAnim;
    // Number of ships to animate
    int NumShipsAnim;
    // Star movement direction
    int StarMovement;


    /***********************************
    *  Non specific Data Types
    ************************************/

    // Timer for main screen saver loop
    DWORD SaverTimer;
    // Shareware mode flag
    BOOL ShareWareMode;
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


    /***********************************
    *  Screen saver specific functions
    ************************************/

    // Initilize the data array
    BOOL InitStarData();
    // Initilize the data array
    BOOL InitPlanetData();
    // Initilize the data array
    BOOL InitShipData();
    // Takes the pointer to the rect, a source and adds and subtracts the amout given
    void AddSubtractRect( RECT *dest, RECT *source, int Amount, int Type = ERASE );
    // This resets the star structure for a new star
    void ResetStarStructure( PTStar pStar );
    // This resets the planet structure for a new planet
    void ResetPlanetStructure( PTPlanet pPlanet );
    // This resets the ship structure for a new planet
    void ResetShipStructure( PTShip pShip );
    // Updates all the star positions
    void UpdateStarPositions();
    // Update all the planet positions that are active
    void UpdatePlanetPositions();
    // Update all the ship positions that are active
    void UpdateShipPositions();


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
    // If we are in shareware mode and it has timed out, show the shareware screen
    BOOL ShowShareWareScreen();
    // Let the user know there was an error
    void DisplayMusicError();
    // Pause the sound
    void PauseSound();
    // Load the users prefferences that were selected in the config dialog
    virtual BOOL LoadControlPanelPrefs();
    // Init screen saver data.
    virtual BOOL InitSaverData();
    // Loads the screen saver graphics
    virtual BOOL LoadGraphics();
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