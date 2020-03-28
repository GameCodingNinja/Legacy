
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
#include "Defs.h"            // Screen saver defs file
#include "TIntList.h"        // Int list object

/*******************************
*    3D Engine include files 
*******************************/

#include "3d/TextType.h"
#include "3d/PalShade.h"
#include "3d/PolyObj.h"


class CAnimateSaver : public CScreenSaver
{
protected:

    /***********************************
    *  Screen saver specific Data Types
    ************************************/

    // Image buffers
    TBitmap buffer;
    // Bitmap texture library
    ImageData AttackShipTxt, MotherShipTxt, DefendShipTxt, AttackLaserTxt, DefendLaserTxt, ExplodeTxt;
    // Polygon object
    TPolyObj AttackShip, MotherShip, DefendShip, AttackLaser, AttackLaserOnly, DefendLaser, DefendLaserOnly, Explode, *pShip, *pLaser;
    // Z Buffer
    int *ZBuffer;
    // Matrix object
    Matrix3D attackMatrix[MAX_ATTACK_SHIPS], motherMatrix, defendMatrix[MAX_DEFEND_SHIPS], laserMatrix[MAX_DEFEND_SHIPS];
    // Size of the camera
    int CameraSize;
    // Render Resolution Rect
    RECT RnderRect;
    // Animation scene state
    int animSceneState;
    // Animation scene counter
    int animSceneCounter;
    // Animation Scene Max Count
    int animSceneMaxCount;
    // X, Y, Z axis move counter
    int X_Move, Y_Move, Z_Move, Move_1, Move_2, Move_3, AnimIndex, MoveAmount, Rotate, RotDir, CrashOffset;
    // X, Y, Z move arrays
    int X_MoveAry[MAX_ATTACK_SHIPS];
    int Y_MoveAry[MAX_ATTACK_SHIPS];
    int Z_MoveAry[MAX_ATTACK_SHIPS];
    int MoveAry1[MAX_ATTACK_SHIPS];
    int MoveAry2[MAX_ATTACK_SHIPS];
    int MoveAry3[MAX_ATTACK_SHIPS];
    int ExplodeY[MAX_EXPLODE_PER_SHIP];
    int ExplodeZ[MAX_EXPLODE_PER_SHIP];
	// Explode Flag
	BOOL fExplode;
    // Pick to see if we run the fight scene or explode scene
    int PickFightExplode;
    // Pick to see if we run the top or bottom scene
    int PickTopOrBottom;
    // Laser on and off switch
    int LaserCounter[MAX_ATTACK_SHIPS];
    // Flag that indicates the scene is to be setup
    BOOL setUpScene;
    // Start background buffer array
    BYTE *starBackground;
    // Cloud buffer offset
    int cloudBufferOffset;
    // Save the res that is less then 640x480
    BOOL ResLessThen640x480;
    // The presents int list
    TIntList AttackList, OverviewList;
    // Attack scene counter
    int AttackSceneCounter, OverviewCounter;
    // Map direction
    int Direction, Movement;


    /***********************************
    *  Non specific Data Types
    ************************************/

    // Timer for main screen saver loop
    DWORD saverTimer;
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

    //////////////////////////////////////////////
    // Below are all the different scene functions
    //////////////////////////////////////////////

    // Front approach of the mother ship
    void Scene_FrontMotherShipApproach();
    // Camera pan under the mother ship
    void Scene_UnderMotherShipPan();
    // Camera pan side of the mother ship
    void Scene_SideMotherShipPan();
    // Camera pan top of the mother ship
    void Scene_TopMotherShipPan();
    // Camera pan back of the mother ship
    void Scene_BackMotherShipPan();
    // Camera pan back of the mother ship
    void Scene_AttackShipFlyBy();
    // Mother ship can be see way off in the distance
    // while the attacking ships approach
    void Scene_ApproachMotherShip();
    // The attacking ships swarm the mother ships
    void Scene_SwarmMotherShip();
    // The emerging of the defender ships
    void Scene_DefenderShipEmerge();
    // Camera top fly by
    void Scene_AttackFlyTop();
    // Camera top fly by
    void Scene_AttackFlyBottom();
    // Camera left side fly by
    void Scene_AttackFlySide();
    // Over view shot of the battle
    void Scene_OverViewAttack();
    // Camera left side fly by explode one of the fighters
    void Scene_FlySideExplode();
    // Camera top fly by explode one of the fighters
    void Scene_FlyTopExplode();
    // Camera bottom fly by explode one of the fighters
    void Scene_FlyBottomExplode();
	// Camera close up on ship
	void Scene_CloseUpFlyOver();


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