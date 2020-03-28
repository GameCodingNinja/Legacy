
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
#include "Metafile.h"        // Class for loading and displaying Windows metafiles and enhanced metafiles
#include "TIntList.h"        // Int list object
#include "SoundMgr.h"        // Loads and plays wave files


class CAnimateSaver : public CScreenSaver
{
protected:

    /***********************************
    *  Screen saver specific Data Types
    ************************************/

    // Image buffers
    TBitmap buffer;
    // City background metafiles
    TMetafile city[ MAX_CITY ];
    // City background lights on metafiles
    TMetafile cityLightON[ MAX_CITY_LIGHTS_ON ];
    // City background lights off metafiles
    TMetafile cityLightOFF[ MAX_CITY_LIGHTS_OFF ];
    // Neighborhood and porch lights metafile
    TMetafile blockMisc[ MAX_MISC ];
    // Neighborhood highlight metafile
    TMetafile blockHighLight[ MAX_HIGHLIGHT ];
    // Neighborhood window lights metafile
    TMetafile blockWindowLight[ MAX_BLOCK_LIGHTS ];
    // lightning metafile
    TMetafile lightning[ MAX_LIGHTNING ];
    // Car metafiles
    TMetafile car[ MAX_CARS ];
    // List of lightning indexes
    TIntList lightningLst;
    // Timer for lightning strikes
    DWORD lightTimer;
    // Lightning sound timer
    DWORD lightSndTimer;
    // Timer for the flash
    DWORD flashTimer;
    // Lightning duration timer
    DWORD lightDurTimer;
    // The lightning counter
    int lightCounter;
    // Flag indicating that the lightning is active
    BOOL lightningActive;
    // Flag indicating that the flash is active
    BOOL flashActive;
    // Flag indicating there is a blackout active
    BOOL blackoutActive;
    // Flickers the blackout
    BOOL blackoutOnOff;
    // Flickers the blackout
    BOOL blackoutOnOffCounter;
    // Blackout Timer
    DWORD blackoutTimer;
    // Blackout on off timer
    DWORD blackoutOnOffTimer;
    // full buffer update flag
    BOOL updateBuffer;
    // Update city only flag
    BOOL updateCity;
    // Update Neighborhood only flag
    BOOL updateNeighborhood;
    // multiple lightning hit counter
    int multiLightCounter;
    // Set the maximum flicker count;
    int flickerMax;
    // Timer for the city lights
    DWORD cityLightTimer;
    // Group index of lights to turn on
    int cityLightsOnIndex;
    // Timer for the neighborhood lights
    DWORD neighborhoodLightTimer;
    // Index of the neighborhood lights to change
    int neighborhoodLightsOnOffIndex;
    // Rain count
    int rainCount;
    // Rain array
    PSRain pRain;
    // The direction the rain will fall
    int rainDirection;
    // Flash color index
    int flashColorIndex;
    // Loaded sound resources
    CSoundMgr sndMjr, sndThunderMjr, sndBackground; 
    // Flag that indicates we can have many sound devices
    BOOL ManySndDevices;
    // Flag to play the lightning sound
    BOOL playLightningSnd;
    // Error string output
    TCanvas errorCanvas;
    // Background Sound Timer
    DWORD backSndTimer;
    // car move rect
    RECT moveRect;
    // Number of grid pieces per screen width
    int gridWidth;
    // Movement X counter
    int MoveX;
    // Flip Flop the way the mouse is walking
    int carDirection;
    // Car Timer
    DWORD carTimer;
    // Current animated car
    int carIndex;
    // Render the lightning regular or backwords
    BOOL renderLightningBackwards;


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

    // Load the metafile animation  into the metafile array
    BOOL LoadMetaFileAnim( PTMetafile pMetaFile, char *ResName, int noOfAnim );
    // Animate the city
    void AnimateCity();
    // Animate the lightning
    void AnimateLightning();
    // Animate the Neighborhood
    void AnimateNeighborhood();
    // Draw the lines of rain
    void AnimateRain();
    // Update the rain animation
    void UpdateRain();
    // Init rain data
    BOOL InitRain();
    // Clear the video buffer with a specific color
    void ClearVideoBuffer( WORD color );
    // Let the user know there was an error
    void DisplaySoundError();


    /***********************************
    *  Non Specific functions
    ************************************/    

    // Animates the stars
    void Animate();
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
    // Loads the screen saver graphics
    virtual BOOL LoadGraphics();
    // Init screen saver data.
    virtual BOOL InitSaverData();
    // Init music player.
    virtual void InitMusicPlayer();
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