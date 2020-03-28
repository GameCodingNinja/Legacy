
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
#include "TCamLst.h"         // Header file for camera list object

/*******************************
*    3D Engine include files 
*******************************/

#include "TextType.h"
#include "PalShade.h"
#include "PolyObj.h"

typedef struct tagTCameraArray
{
    WORD Count;
    TCameraWord camera[];
} TCameraArray, *PTCameraArray;


class CAnimateSaver : public CScreenSaver
{
protected:
    /***********************************
    *  Screen saver specific Data Types
    ************************************/

    // Image buffers
    TBitmap buffer;
    // Bitmap texture array class
    ImageData TextDat;
    // Polygon object
    TPolyObj Wormhole1Poly, Ship;
    // Z Buffer
    int *ZBuffer;
    // Matrix object
    Matrix3D CameraMatrix;
    // Motion matrix
    Matrix3D MoveMatrix;
    // Camera list of offsets
    PTCameraArray pCameraList;
    // Flip flop Z Buffer counter
    BOOL ZBufferFlipFlopCounter;
    // Counter for the camera list
    int CameraCounter;
    // Render Resolution Rect
    RECT RnderRect;
    // Size of the camera
    int CameraSize;
    // The startup device mode
    DEVMODE startupDevmode;
    // Flag to indicate the screen res was changed
    BOOL ScreenResized;
    // Movement offsets
    int RotateCount, RotateType, rotate, shipLRCount, shipUDCount, viewCount;
    DWORD RotateTimer;
	// 16 bit Shaded lookup table
	unsigned short int *pShadedTbl;
	// Pointers to color textures
	unsigned short int *color[3];
	// Color switch timer
	unsigned int colorTimer;
	// Camera timer
	unsigned int cameraTimer;
	// Color change counter
	int colorChangeCounter;
	// color index
	int oldColorIndex;
	int newoColorIndex;
	int newFlipFlop;
	int oldFlipFlop;
	unsigned int scrnRenderTime;

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

    // Set the screen resolution
    void SetScreenResolution();
	// Change the color
	void ChangeColor();
	// Calc a new color
	void CalcNewColor();
	// Set the color
	void SetColor( int flipFlop, int colorIndex );
	// Pick an alternate color
	void PickAltColor();


    /***********************************
    *  Non Specific functions
    ************************************/    

    // Animates the stars
    void Animate();
    // Render the buffer
    void RenderBuffer();
    // Blit to the buffer from the screen
    void BlitFromBufferToScreen();
    // Load the users prefferences that were selected in the config dialog
    virtual BOOL LoadControlPanelPrefs();
    // Loads the screen saver graphics
    virtual BOOL LoadGraphics();
    // Play Music
    void PlayMusic();
    // If we are in shareware mode and it has timed out, show the shareware screen
    BOOL ShowShareWareScreen();
    // Let the user know there was an error
    void DisplayMusicError();
    // Init screen saver data
    virtual BOOL InitSaverData();
    // Init music player.
    virtual void InitMusicPlayer();
    // Pause the sound
    void PauseSound();
	// Set the handle for the images
	void SetImageHandle();

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