
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

/*******************************
*    3D Engine include files 
*******************************/

#include "TextType.h"
#include "PalShade.h"
#include "PolyObj.h"


class CAnimateSaver : public CScreenSaver
{
protected:

    /***********************************
    *  Screen saver specific Data Types
    ************************************/

    // Image buffers
    TBitmap buffer;    
    // Z Buffer
    int *ZBuffer;
    // Bitmap texture library
    ImageData santaTxt, snowTxt, houseTxt, rudolfTxt;
    // Polygon object
    TPolyObj santaPly[MAX_SANTA_DANCES][MAX_SANTA_FRAMES], housePly, snowPly; 
    TPolyObj rudolfPly[MAX_RUDOLF_DANCES][MAX_RUDOLF_FRAMES];
	// Animation frame counter
	int frameCounter;
	// Ping pong direction
	int pingPongDir, rudolfPingPongDir;
	// The snow offsets and rotation
	SSnow snowOffset[MAX_SNOW];
	// The camera
	TCamera camera;
	// Camera view index
	int cameraCounter;
	// Camera View Counter
	int cameraFrameCounter;
    // The camera and dance int list
    TIntList viewList, danceList;
	// The camera index
	int cameraIndex;
	// Current Santa dance
	int danceIndex;
	// Dance counter
	int danceCounter;
	// Rudolf dance counter
	int rudolfFrameCounter;
	// Rudolf dance index
	int rudolfDanceIndex;
	// Nose glow counter
	int noseGlowCounter;


    /***********************************
    *  Non specific Data Types
    ************************************/

    // Size of the camera
    int CameraSize;
    // Render Resolution Rect
    RECT RnderRect;
    // Save the res that is less then 640x480
    BOOL ResLessThen640x480;
    // Timer for main screen saver loop
    DWORD SaverTimer;
    // Speed of main screen saver loop
    DWORD SaverSpeed;
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

	// Set the camera
	void SetCamera();


    /***********************************
    *  Non Specific functions
    ************************************/    

    // Animates the stars
    void Animate();
    // Render the polygon objects to the buffer
    void RenderBuffer();
    // Set the screen resolution
    void SetScreenResolution();
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