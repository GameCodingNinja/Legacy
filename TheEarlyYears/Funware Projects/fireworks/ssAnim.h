
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
#include "Particle.h"        // Particle Class
#include "SoundMgr.h"        // Loads and plays wave files

/*******************************
*    3D Engine include files 
*******************************/

#include "TextType.h"
#include "PalShade.h"
#include "PolyObj.h"


// Structure to manage the fire works
class CFirework
{
public:
    // Matrix object
    Matrix3D fwMatrix1[ MAX_PARTICLE_BURST ];
    // Particle class
    CParticle cParticle;
    // Lifetime of particle burst counter
    int LifeTimeBurstCounter;
    // Max of lifetime
    int MaxLifeTime;
    // 3D Burst Position
    int X, Y, Z;
    // Color offset
    int colorOffset;
    // The number of particles bein used
    int activeCount;
    // Point movement to burst
    SParticlePoint3D sIPoint3D;

    // Slope X direction
    int SlopeX;
    // Slope Y direction             
    int SlopeY;
    // Index of the slope             
    int SlopeIndex;
    // speed of set path         
    int AnimSpeed;
    // Slope error
    int error;

    // Init the burst data
    void InitBurst( int MaxX, int MaxY, int MaxZ, int Yoffset, int RandZ, int LifeCount );
    // Plots the line and updates the offsets
    bool UpdatePoint();
    // Get the color offset
    int GetColorOffset(){ return colorOffset; };
    // Set the color offset
    void SetColorOffset( int value ){ colorOffset = value; };
    // Get the active number of particles
    int GetActiveCount(){ return activeCount; };
    // Set the active number of particles
    void SetActiveCount( int value ){ activeCount = value; };          

    // Conctructor
    CFirework();
};
typedef CFirework *PCFirework;


class CAnimateSaver : public CScreenSaver
{
protected:

    /***********************************
    *  Screen saver specific Data Types
    ************************************/

    // Image buffers
    TBitmap buffer;
    // Bitmap texture library
    ImageData boxTxt;
    // Polygon object
    TPolyObj box;
    // Size of the camera
    int CameraSize;
    // Render Resolution Rect
    RECT RnderRect;
    // Save the res that is less then 640x480
    BOOL ResLessThen640x480;
    // background buffer array
    BYTE *Background;
    // Star array pointer
    SStar *pStar;
    // Number of alocated stars
    int StarCount;
    // Picked twinkle star
    int star;
    // Maximum number of scan lines for moving the background
    int CloudMaxLines;
    // The counter for moving the scan lines
    int CloudCounter;
    // Firework array
    PCFirework pcFirework;
    // Color randomizer
    TIntList colors;
    // Color Counter
    int colorCounter;
    // 3D max offsets
    int MaxX, MaxY, MaxZ, RandZ, Ystart;
    // Angle of the viewer
    float Xangle;
    // Life time of the burst
    int MaxLifeCount;
    // Loaded sound resources
    CSoundMgr sndExplode, sndShoot, sndSimple;
	// One looping sound
	CWaveDevice loopDev;
	CWaveLoad loopLoad;
	// Croud sounds
	CWaveDevice croudDev;
	CWaveLoad croudLoad;
	// Flag that indicates we can have many sound devices
	BOOL ManySndDevices;
	// Error string output
	TCanvas errorCanvas;


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
    // Total number of loaded songs
    int SongCount;

    /***********************************
    *  Screen saver specific functions
    ************************************/

    // Render the polygon objects to the buffer
    void RenderBuffer();
    // Set the screen resolution
    void SetScreenResolution();
    // PlaySoundEffects
    void PlaySoundEffects();
    // Let the user know there was an error
    void DisplaySoundError();


    /***********************************
    *  Non Specific functions
    ************************************/    

    // Animates the stars
    void Animate();
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