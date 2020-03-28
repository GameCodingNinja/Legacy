
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
    // Matrix class
    Matrix3D earthMatrix, ringMatrix, sunMatrix;
    // Bitmap texture library
    ImageData earthTxt, ringTxt;
    // Polygon object
    TPolyObj earthPly, ringPly;
	// Rotation Counter
	DWORD rotationCounter;
	// Ring pingpong flag
	BOOL ringPingPong;
	// Ring counter pingpong
	float ringCounterPingPong;
	// Earth's axis
	int earthAxis;
	// earth's Camera X
	int	earthCameraX;
	// Z index
	int Zindex;
	// Camera X timer
	DWORD cameraTimer;
	// Y offset
	int earthOffsetY;
	// File texture load
	char *pFileTextureLoad;
	// Star array pointer
	SStar *pStar;
	// Number of alocated stars
	int StarCount;
	// Picked twinkle star
	int twinkleStar;
	// Rotation vector
	int rotVector;
	// Maximum allowed zoom
	int zoomMax;
	// 16 bit Shaded lookup table
	unsigned short int *pShadedTbl;

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

	// Load texture from file
	BOOL LoadTextureFromFile( TDIB &dib );



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

// Strip the file name from the file path
//void CropFileNameFromPath( char *SourceFilePath, char *DestFileName, BOOL KeepExtension = TRUE );
// A simplified use of SearchPath function
BOOL FileExist( char *SearchFilePath, char *SearchPathDir = NULL );

#endif  // __SSANIMATE_H__