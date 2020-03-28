
/************************************************************************
*
*    PRODUCT:         Screen Saver Thread
*
*    FILE NAME:       SSAnim.cpp
*
*    DESCRIPTION:     Thread that runs the screen saver
*
*    IMPLEMENTATION:  This thread is screen saver specific. Here is where
*                     you initilize your screen saver, run it and then
*                     free all allocated memory and resources. 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
                             // Note: we are not using the #define WIN32_LEAN_AND_MEAN because we need some multimedia functions
                             // WIN32_LEAN_AND_MEAN leaves out

#include "mintools.h"        // A misc group of useful functions
#include "ssAnim.h"          // Header file for this *.cpp file.
#include <stdlib.h>          // Declares several commonly used routines such as atoi, random, abs, malloc etc.
#include "SSConfig.h"        // Header file for the config dialog box.


int RotateClock[10] = { -1, -2, -3, -4, -5, -4, -3, -2, -1, 0 };
int RotateCounter[10] = { 1, 2, 3, 4, 5, 4, 3, 2, 1, 0 };
int shipMoveLR[SHIP_MOVE_SIDE_TO_SIDE] = { -3, -6, -9, -12, -15, -18, -21, -24, -26, -28, 
                                        -30, -31, -32, -33, -32, -31, -30, -28, -26, -24, -21, -18, -15,
                                        -12, -9, -6, -3, 0, 3, 6, 9, 12, 15,
                                         18, 21, 24, 26, 28, 30, 31, 32, 33, 32, 31, 30, 28, 
                                         26, 24, 21, 18, 15, 12, 9, 6, 3, 0 };

int shipMoveUD[SHIP_MOVE_UP_DOWN] = { -1, -2, -3, -4, -5, -6, -5, -4, -3, -2, -1, 0,
                                       1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, 0 };

int viewRotation[MAX_VIEWS] = { 0, 256, 512 };

// Externed from SSConfig.cpp
extern int ResCheck[MAX_RES_CHECK];
extern int BlitToScreen[MAX_RES_CHECK];


/************************************************************************
*    FUNCTION NAME:         Constructor                                                             
*
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CAnimateSaver::CAnimateSaver( PTSaverModeData ssData ) : CScreenSaver( ssData )
{
    // Init screen saver specific functions
    ZBuffer = NULL;
    ZBufferFlipFlopCounter = TRUE;
    CameraCounter = 0;
    ScreenResized = FALSE;
    rotate = 0;
    RotateTimer = 0;
    RotateCount = 0;
    shipLRCount = 0;
    shipUDCount = 0;
    // Pick a new rotation type
    RotateType = rand() % 2;
    viewCount = 0;
    pShadedTbl = NULL;
    colorTimer = 0;
    colorChangeCounter;
	cameraTimer = 0;
	scrnRenderTime = 0;

    // Init non-specific class members
    AnimCounter = 0;
    SaverTimer = 0;
    SaverSpeed = 0;
    ShareWareMode = false;
    ShareWareTimer = 0;
    MusicTimer = 0;
    SongCount = 0;
    hOldPalette = NULL;
    Error_Number = 0;
    PreviewCounter = 0;

}   // Constructor


/************************************************************************
*    FUNCTION NAME:         LoadControlPanelPrefs                                                             
*
*    DESCRIPTION:           Get the user control panel settings
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - True or False depending on if successful
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CAnimateSaver::LoadControlPanelPrefs()
{
    int RenderWidth, RenderHeight;

    // Get the user's settings
    // Load all the ini file settings
    Settings.GetPrivateString();

    // Check to see if this is the registered version
    /*if( strcmp( FULL_VERSION_NUMBER, Settings.GetRegNum() ) == 0 )
        ShareWareMode = FALSE;

    else // Check to see if one of the temp reg numbers are being used
    {
        // If the first tem reg number doesn't pan out, try the second number
        if( ValidateTempRegNum( FIRST_CHECK_FILE, SHAREWAEW_TMP_RUN_MAX ) )
            ShareWareMode = FALSE;
        else
        {
            if( ValidateTempRegNum( SECOND_CHECK_FILE, SHAREWAEW_TMP_RUN_MAX ) )
                ShareWareMode = FALSE;
        }
    }*/

    // Get the current res ID for the screen size
    int screenResID = GetScreenResSizeID( ScreenRect.right, ScreenRect.bottom );

    // Calculate the maximum render size
    if( Settings.GetCalcRenderRes() )
    {        
        for( int i = 0; i < MAX_RES_CHECK; ++i )
        {
            int renderTime = GetRenderTime( ResCheck[i], Instance() );

            // Make sure we at least set the first resolution if there wasn't an error
            if( renderTime <= (MAX_MIL_RENDER_WAIT - BlitToScreen[i]) )
                Settings.SetRenderRes( ResCheck[i] );
            else
                break;
        }

        // Make sure we don't save a render resolution higher then the screen size
        if( Settings.GetRenderRes() > screenResID )
            Settings.SetRenderRes( screenResID );

        // Set the calculate to false so we only do this once
        Settings.SetCalcRenderRes( FALSE );
        
        // Save all the ini file settings
        Settings.WritePrivateString();  
    }

    // Make sure we don't save a render resolution higher then the screen size
    if( Settings.GetRenderRes() > screenResID )
        Settings.SetRenderRes( screenResID );

    // What size should the screen be rendered?
    GetWidthHeightCamera( Settings.GetRenderRes(), RenderWidth, RenderHeight, CameraSize );

    #ifdef DEBUG
    ShareWareMode = FALSE;
    #endif

    // Set up the blit rects
    #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )

    SetRect( &RnderRect, abs(ScreenWidth - RenderWidth) / 2, 
                         abs(ScreenHeight - RenderHeight) / 2,
                         RenderWidth, RenderHeight );
    #else
    CameraSize = 200;
    RnderRect = WindowRect;
    #endif

    if( Mode() == ssPreview )
    {
        RnderRect = WindowRect;
        CameraSize = 100;
    }

    return TRUE;

}   // LoadControlPanelPrefs


/************************************************************************
*    FUNCTION NAME:         LoadGraphics                                                             
*
*    DESCRIPTION:           Get the user control panel settings
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - True or False depending on if successful
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CAnimateSaver::LoadGraphics()
{
    BOOL Result = FALSE;
    TLoadRes LoadRes;

    // Allocate the Z Buffer
    ZBuffer = new int[ RnderRect.right * RnderRect.bottom ];
    if( !ZBuffer )
        goto HANDLE_ERROR;

    // Zero out the ZBuffer
    memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );

    // One time initilization of the cos and sin tables
    CameraMatrix.InitCosSinTables();

	// Set the light source
    //Wormhole1Poly.SetLightSource( (rand() % 500000) + 1, (rand() % 500000) + 1, (rand() % 500000) + 1 );

    // Load the wormhole polygon object from resource
    if( !LoadTheResource( Instance(), "Wormhole1Poly", "POLY", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the wormhole polygon object 
    if( !Wormhole1Poly.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the ship polygon object from resource
    //if( !LoadTheResource( Instance(), "ship1Poly", "POLY", &LoadRes ) )
        //goto HANDLE_ERROR;

    // Load the ship polygon object 
    //if( !Ship.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
        //goto HANDLE_ERROR;

    // Load the texture library from resource
    if( !LoadTheResource( Instance(), "texture", "BMPTXT", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the texture library 
    if( !TextDat.LoadFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    //if( !LoadTheResource( Instance(), "shadedTableRGB", "BMPSHD", &LoadRes ) )
        //goto HANDLE_ERROR;

    // Allocate the shaded lookup table
    pShadedTbl = new unsigned short int[ SHADE_COUNT * COLOR_COUNT_16_BIT ];
    if( pShadedTbl == NULL )
    {
        strcpy( errorStr, "Can't allocate shaded lookup table. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Generate the 16 bit shaded table
    Generate16BitShadedTable( pShadedTbl );

    // Load the 16 bit shaded table 
    Wormhole1Poly.SetShadeLookUpTable( pShadedTbl );
    //Ship.SetShadeLookUpTable( (WORD *)LoadRes.pData );

    
    // Init the classes using the screens DC. That way it doesn't have to make one
    buffer.InitActiveDC( GetScreenDC() );

    if( !buffer.CreateDIBSectionBuffer( RnderRect.right, -RnderRect.bottom, BIT_COUNT_16, TextDat.TMap[ 0 ].Palette ) )
        goto HANDLE_ERROR;

    // Set the ZBuffer
    Wormhole1Poly.SetZBuffer( ZBuffer );
    //Ship.SetZBuffer( ZBuffer );

    // Set the size of the rendering area
    Wormhole1Poly.SetRenderSize( RnderRect.right, RnderRect.bottom );
    //Ship.SetRenderSize( RnderRect.right, RnderRect.bottom );

    // Set the maxium distance
    Wormhole1Poly.SetMaxZDistance( 78000 );
    Wormhole1Poly.SetMaxLightDistance( 400000 );
    //Ship.SetMaxZDistance( 400000 );
    //Ship.SetMaxLightDistance( 400000 );

    // Set the scale
    Wormhole1Poly.SetCameraScale( CameraSize );
    //Ship.SetCameraScale( CameraSize+50 );

    // Set the video buffer
    Wormhole1Poly.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );
    //Ship.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );

    // Set the texture data
    Wormhole1Poly.SetTextures( &TextDat );
    //Ship.SetTextures( &TextDat );

	// Init with the default texture because we don't have a
    // coordinate file
    //Wormhole1Poly.InitDefText();

    // Load the ship polygon object from resource
    if( !LoadTheResource( Instance(), "Wormhole1Cord", "CORD", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the ship1 polygon object 
    Wormhole1Poly.LoadCrdFromPointer( (BYTE *)LoadRes.pData );

    // Load the ship polygon object from resource
    /*if( !LoadTheResource( Instance(), "ship1Cord", "CORD", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the ship1 polygon object 
    Ship.LoadCrdFromPointer( (BYTE *)LoadRes.pData ); */

    // Set the textures to the different polygon handles
    SetImageHandle();

    // Get the pointers to the colors
    color[0] = (unsigned short int *)TextDat.TMap[0].GetTexturePtr();
    color[1] = (unsigned short int *)TextDat.TMap[1].GetTexturePtr();
    color[2] = (unsigned short int *)TextDat.TMap[2].GetTexturePtr();

    // Calculate a new color
    CalcNewColor();

    // Randomize a flip flop
    newFlipFlop = rand() % MAX_FLIP_FLOP;

    // Set the color
    SetColor( newFlipFlop, newoColorIndex );

    // Try to pick an alternate color
    PickAltColor();

	//Wormhole1Poly.SetLightSource( (rand() % 500000) + 1, (rand() % 500000) + 1, (rand() % 500000) + 1 );

    // Load the texture library from resource
    if( !LoadTheResource( Instance(), "CameraMove1", "MOVEMENT", &LoadRes ) )
        goto HANDLE_ERROR;

    // Saver the pointer to the camera data
    pCameraList = PTCameraArray( LoadRes.pData );

	// Set the timer
    colorTimer = timeGetTime() + COLOR_CHANGE_TIMEOUT;

    Result = TRUE;

    HANDLE_ERROR:;

    return Result;
}   // LoadGraphics


/************************************************************************
*    FUNCTION NAME:         InitSaverData                                                             
*
*    DESCRIPTION:           Init screen saver data
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - True or False depending on if successful
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CAnimateSaver::InitSaverData()
{
    BOOL Result = FALSE;
    int i;

    #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )
    if( Mode() == ssSaver && 
        Settings.GetSizeToFit() && 
        Settings.GetRenderRes() != GetScreenResSizeID( ScreenRect.right, ScreenRect.bottom ) &&
        SystemBitDepth > 8 )
    {
        SetScreenResolution();
    }
	#endif

    // Randomize the position in the worm hole
    CameraCounter = rand() % pCameraList->Count;

    // Fast forwared to that spot
    for( i = 0; i < CameraCounter; ++i )
    {
        CameraMatrix.Rotate( pCameraList->camera[i].XRot, pCameraList->camera[i].YRot, pCameraList->camera[i].ZRot );
        CameraMatrix.Translate( pCameraList->camera[i].XPos, pCameraList->camera[i].YPos, pCameraList->camera[i].ZPos );
    }

	// Display a message if the computer is in 8-bit or less color
    if( SystemBitDepth < 16 )
        DisplayShareWareScreen( buffer.Handle(), buffer.width(), buffer.height(), NEED_HIGHER_COLOR_MODE, HOW_TO_CHANGE_HIGHER_COLOR, false );

    cameraTimer = timeGetTime() + 10000;
    
    Result = TRUE;

    HANDLE_ERROR:;

    return Result;

}   // InitSaverData


/************************************************************************
*    FUNCTION NAME:         InitMusicPlayer                                                             
*
*    DESCRIPTION:           Allocate the music player. If there is an error
*                           allocating the Windows Media player, we still
*                           allow the screen saver to run.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - True or False depending on if successful
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::InitMusicPlayer()
{
    // Allocate the media player for playing music
    if( SaverActive() && Mode() == ssSaver && (Settings.GetThemeMusicOn() || Settings.GetLoadYourOwn()) )
    {
        // Allocate the media player
        if( MPlayer.Init( Handle(), Instance(), DONT_USE_ERROR_DLG_BOXES ) )
        {
            // if they are loading their own music, allocate the string list
            // and load up all the sound file paths
            if( Settings.GetLoadYourOwn() )
            {
                StringList.GetAllFileNamesInDir( Settings.GetSoundDir(), "*.wav" );
                StringList.GetAllFileNamesInDir( Settings.GetSoundDir(), "*.mid" );
                StringList.GetAllFileNamesInDir( Settings.GetSoundDir(), "*.rmi" );
                StringList.GetAllFileNamesInDir( Settings.GetSoundDir(), "*.mp3" );

                // Make sure there has been music added to the sound directory
                if( StringList.Count() )
                {
                    if( !Settings.GetRandomMusic() )
                        StringList.Sort();
                }
            }
        }
    }

}   // InitMusicPlayer


/************************************************************************
*    FUNCTION NAME:         DisplayBufferOnStartUp                                                             
*
*    DESCRIPTION:           First time display of the buffer to start
*                           the screen saver
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::DisplayBufferOnStartUp()
{
    // Blit the animations to the buffer and then display the buffer
    // This is only needed in full screen mode
    #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )
    if( Mode() == ssSaver )
    {
        // Start the music playing before the screen saver shows. Some computers
        // take a while and grind for a second before the music starts playing. It's
        // best not to have the screen saver showing at this time because the animation will
        // be stopped solid in it's tracks waiting for the music to start.
        PlayMusic();

        FillRect( GetScreenDC(), &DesktopRect, (HBRUSH)GetStockObject( BLACK_BRUSH ) );   
    }
    #endif

}   // DisplayBufferOnStartUp


/************************************************************************
*    FUNCTION NAME:         AnimateSaver                                                             
*
*    DESCRIPTION:           Animate the screen saver
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::AnimateSaver()
{
    // Check to see if we need to paint the rect the password dialog
    // box was at because it doesn't erase itself
    if( !PaintPasswordDialogRect() )
    {
        // Make sure the monitor is not asleep
        if( !GetMonitorIsAsleep() )
        {
            // Time out the screen saver
            if( AnimCounter == 0 && SaverTimer < timeGetTime() )
            {
                Animate();
                SaverTimer = timeGetTime() + MAX_MIL_RENDER_WAIT;
            }
            else if( AnimCounter )
                Animate();
        }
        // see if we need to pause the sound
        else if( GetSoundPause() )
        {
            // Pause the sound because the monitor is asleep
            PauseSound();

            // Set the pause state back to false so that we don't keep
            // trying to pause the sound over and over again.
            SetSoundPause( FALSE );
        }
    }
    else
    {
        // Erase the password dialog box by blitting over it.
        //buffer.Draw( GetScreenDC(), GetPasswordWindowRect() );

        // Resets the rect back to zero.
        ResetPasswordRect();
    }

}   // AnimateSaver


/************************************************************************
*    FUNCTION NAME:         PlayMusic                                                             
*
*    DESCRIPTION:           Plays the music that comes with this screen
*                           saver or play the users music
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::PlayMusic()
{
    int PlayerStatus;

    // Don't try to play any music if there has been any errors
    if( !Settings.GetMusicOff() && !MPlayer.GetPlayError() && MusicTimer < timeGetTime() )
    {
        PlayerStatus = MPlayer.GetStatus();

        // Do we play the included star song?
        if( Settings.GetThemeMusicOn() )
        {
            // This will play the song that has already been loaded
            if( PlayerStatus == PLAYER_STOPPED || PlayerStatus == PLAYER_PAUSED )
                MPlayer.Play();

            // This will load the song and play it
            else if( PlayerStatus == PLAYER_CLOSED )
                MPlayer.Play( "ThemeSong", "MIDI" );
        }

        // Or do we play music from the users drive
        else if( Settings.GetLoadYourOwn() )
        {
            if( PlayerStatus == PLAYER_STOPPED || PlayerStatus == PLAYER_CLOSED )
            {
                if( SongCount == 0 )
                {
                    // If they choose to randomize the music, mix it up
                    if( Settings.GetRandomMusic() )
                        StringList.MixUp();
                }

                // Play the song
                MPlayer.Play( StringList.GetString( SongCount ) );
                
                // Inc to the next song
                SongCount = (SongCount + 1) % StringList.Count();
            }
            else if( PlayerStatus == PLAYER_PAUSED )
                MPlayer.Play();   
        }

        // Reste the timer to check if the music is done playing
        MusicTimer = timeGetTime() + CHECK_PLAYBACK_DONE_INTERVAL;
    }

}   // PlayMusic


/************************************************************************
*    FUNCTION NAME:         DisplayMusicError                                                             
*
*    DESCRIPTION:           Let the user know there was an error
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::DisplayMusicError()
{
    if( MPlayer.GetPlayError() )
    {
        // Set the background mode to transparent for the text
        canvas.SetBackgroundMode( TRANSPARENT );

        // Draw the text
        canvas.DrawText( 0, 0, MPlayer.GetErrorStr() );

        // Set the background mode back to opaque
        canvas.SetBackgroundMode( OPAQUE );
    }
}   // DisplayMusicError


/************************************************************************
*    FUNCTION NAME:         Animate                                                             
*
*    DESCRIPTION:           Animates the stars. This is designed to have it
*                           do one thing at a time as it come through the
*                           function.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::Animate()
{
    switch( AnimCounter )
    {
        // Do we need to play music
        case PLAY_MUSIC:       if( SystemBitDepth > 8 ) PlayMusic();              break;

        // Erasing the buffer
        case RENDER_BUFFER:    if( SystemBitDepth > 8 ) RenderBuffer();           break;

        // Do the shareware check
        case SHAREWARE_NOTICE: if( SystemBitDepth > 8 ) ShowShareWareScreen();    break;

        // Blit to screen
        case BLIT_TO_SCREEN:  BlitFromBufferToScreen();  break;
    }

    // Inc the animation counter
    AnimCounter = ( AnimCounter + 1 ) % NUM_BLIT_OPERATIONS;

}   // Animate


/************************************************************************
*    FUNCTION NAME:         RenderBuffer                                                             
*
*    DESCRIPTION:           Erases the buffer in prep for the next 
*                           animation round
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::RenderBuffer()
{
    int i, polyCount;
    Matrix3D FinalMatrix, shipMatrix;
	bool resetCamera = false;

    //memset( buffer.GetPtrToDIBsectionBits(), 35, (RnderRect.right * RnderRect.bottom) << 1 );

    for( i = 0; i < Settings.GetRenderSpeed()+1; ++i )
    {
        if( CameraCounter == 0 )
        {
            // Reset the camera matrix because we are starting over
            CameraMatrix.InitilizeMatrix();

            // reset the position of the graphice
            SetImageHandle();

			resetCamera = true;
        }

        CameraMatrix.Rotate( pCameraList->camera[CameraCounter].XRot, pCameraList->camera[CameraCounter].YRot, pCameraList->camera[CameraCounter].ZRot );
        CameraMatrix.Translate( pCameraList->camera[CameraCounter].XPos, pCameraList->camera[CameraCounter].YPos, pCameraList->camera[CameraCounter].ZPos );

        // Inc the camera list counter
        CameraCounter = (CameraCounter + 1) % pCameraList->Count;
    }

	// Do we allow for camera changes?
	if( Settings.GetRandomCamera() )
	{
	    // Change camera angle
	    if( (viewCount == 0) && (resetCamera || (cameraTimer < timeGetTime())) )
	    {
	        viewCount = (viewCount + 1) % MAX_VIEWS;
			cameraTimer = timeGetTime() + SIDE_CAMERA_TIME;
	    }
	    else if( (viewCount == 1) && (resetCamera || (cameraTimer < timeGetTime())) )
	    {
	        viewCount = (viewCount + 1) % MAX_VIEWS;
			cameraTimer = timeGetTime() + FRONT_BACK_CAMERA_TIME;
	    }
	    else if( (viewCount == 2) && (resetCamera || (cameraTimer < timeGetTime())) )
	    {
	        viewCount = (viewCount + 1) % MAX_VIEWS;
			cameraTimer = timeGetTime() + FRONT_BACK_CAMERA_TIME;
	    }
	}

    // Do we allow for rotations?
    if( Settings.GetRandomRot() && RotateTimer < timeGetTime() )
    {
        // Start a rotation if zero
        if( rotate == 0 )
        {
            // Pick a new rotation type
            RotateType = (RotateType + 1) % MAX_ROTATION_TYPES;

            // Reset the counter
            RotateCount = 0;
        }

        // inc the rotation
        if( RotateType == ROTATE_CLOCK )
        {
            rotate = RotateClock[ RotateCount ];            
        }
        else if( RotateType == ROTATE_COUNTER )
        {
            rotate = RotateCounter[ RotateCount ];          
        }

        if( rotate == 0 )
            RotateTimer = timeGetTime() + (rand() % 5000) + 3000;
        else if( abs(rotate) >= 1 && abs(rotate) <= 4  )
            RotateTimer = timeGetTime() + 500;
        else if( abs(rotate) == 5 )
            RotateTimer = timeGetTime() + (rand() % 5000) + 5000;

        ++RotateCount;
    }


    //MoveMatrix.Translate( DeltaX, DeltaY, 0 );
    MoveMatrix.Rotate(  0, 0, rotate );

    // Merge the matrix
    FinalMatrix.MergeMatrix( CameraMatrix.Matrix );
    FinalMatrix.MergeMatrix( MoveMatrix.Matrix );
    FinalMatrix.Rotate( 0, viewRotation[viewCount], 0 );

    if( viewCount == 1 )
    {
        FinalMatrix.Translate( 0, 0, 2000 );
    }

    /*shipMatrix.Translate( shipMoveLR[shipLRCount], -30 + shipMoveUD[shipUDCount], 0 );
    //shipMatrix.Translate( 0, -30 + shipMoveUD[shipUDCount], 0 );
    shipMatrix.Rotate(  0, viewRotation[viewCount], 0 );
    shipLRCount = (shipLRCount + 1) % SHIP_MOVE_SIDE_TO_SIDE;
    shipUDCount = (shipUDCount + 1) % SHIP_MOVE_UP_DOWN;*/

    if( colorTimer < timeGetTime() )
    {
        ChangeColor();
    }

    #ifdef FULL_SCREEN_DEBUG
    DWORD startTime, endTime;
    startTime = timeGetTime();
    #endif

    if( ZBufferFlipFlopCounter )
    {
        // Paint the buffer
        
        //Ship.PaintVBuffer8To16( shipMatrix, POS_ZBUFFER );
        polyCount = Wormhole1Poly.PaintVBuffer16To16( FinalMatrix, POS_ZBUFFER );

        ZBufferFlipFlopCounter = FALSE;
    }
    else
    {
        // Paint the buffer
        //Ship.PaintVBuffer8To16( shipMatrix, NEG_ZBUFFER );
        polyCount = Wormhole1Poly.PaintVBuffer16To16( FinalMatrix, NEG_ZBUFFER );

        ZBufferFlipFlopCounter = TRUE;
    }

    #ifdef FULL_SCREEN_DEBUG
    endTime = timeGetTime() - startTime;

    // Draw the text
    canvas.DrawFormatedText( 0, 0, "mil per frame:  %u;   poly Count:  %d;   Camera count:  %d;   Scrn Render:  %u", endTime, polyCount, CameraCounter, scrnRenderTime );
    #endif


}   // RenderBuffer


/************************************************************************
*    FUNCTION NAME:         CalcNewColor                                                             
*
*    DESCRIPTION:           Calculate a new color
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::CalcNewColor()
{
    int tmpColor;
    int volatile textel_R;
    int volatile textel_G;
    int volatile textel_B;
    int volatile value;
    
    do
    {
        tmpColor = (rand() % COLOR_COUNT_16_BIT);
        
        // Break out the color to check it
        // to see if it is too dark
        textel_R = (tmpColor & 0x7C00) >> 10;
        textel_G = (tmpColor & 0x3E0) >> 5;
        textel_B = tmpColor & 0x1F;

        // Get the combined value to see if it is in range
        value = textel_R + textel_G + textel_B;
    }
	while( value < MAX_COLOR_VALUE_RANGE );
    
    // Save the new color
    newoColorIndex = tmpColor;

}   // CalcNewColor


/************************************************************************
*    FUNCTION NAME:         PickAltColor                                                             
*
*    DESCRIPTION:           Pick an alternate color
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::PickAltColor()
{
    // Set one of the colors to be random every now and then
    if( rand() % 5 == 0 )
    {
        *color[rand() % 3] = pShadedTbl[(rand() % COLOR_COUNT_16_BIT)];
    }

}   // PickAltColor


/************************************************************************
*    FUNCTION NAME:         ChangeColor                                                             
*
*    DESCRIPTION:           Change the colors
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::ChangeColor()
{
    
    if( colorChangeCounter == 0 )
    {
        oldFlipFlop = newFlipFlop;
        newFlipFlop = rand() % MAX_FLIP_FLOP;
        
        // Save the old color index and pick a new one
        oldColorIndex = newoColorIndex;

        CalcNewColor(); 
    }

    switch( colorChangeCounter )
    {
        case 0:
        case 6:
        case 10:
            SetColor( newFlipFlop, newoColorIndex );
        break;

        case 3:
        case 8:
            SetColor( oldFlipFlop, oldColorIndex );
        break;
    }

    ++colorChangeCounter;

    if( colorChangeCounter >= 11 )
    {
        colorTimer = timeGetTime() + COLOR_CHANGE_TIMEOUT;
        colorChangeCounter = 0;

        // Try to pick an alternate color
        PickAltColor();
    }

}   // ChangeColor


/************************************************************************
*    FUNCTION NAME:         SetColor                                                             
*
*    DESCRIPTION:           Set the new color
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::SetColor( int flipFlop, int colorIndex )
{
    switch( flipFlop )
    {
        case 0:
            *color[0] = pShadedTbl[colorIndex];
            *color[1] = pShadedTbl[colorIndex + (8 * COLOR_COUNT_16_BIT)];
            *color[2] = pShadedTbl[colorIndex + (16 * COLOR_COUNT_16_BIT)];
        break;

        case 1:
            *color[1] = pShadedTbl[colorIndex];
            *color[2] = pShadedTbl[colorIndex + (8 * COLOR_COUNT_16_BIT)];
            *color[0] = pShadedTbl[colorIndex + (16 * COLOR_COUNT_16_BIT)];
        break;
        
        case 2:
            *color[2] = pShadedTbl[colorIndex];
            *color[0] = pShadedTbl[colorIndex + (8 * COLOR_COUNT_16_BIT)];
            *color[1] = pShadedTbl[colorIndex + (16 * COLOR_COUNT_16_BIT)];
        break;
        
        case 3:
            *color[2] = pShadedTbl[colorIndex];
            *color[1] = pShadedTbl[colorIndex + (8 * COLOR_COUNT_16_BIT)];
            *color[0] = pShadedTbl[colorIndex + (16 * COLOR_COUNT_16_BIT)];
        break;

        case 4:
            *color[0] = pShadedTbl[colorIndex];
            *color[2] = pShadedTbl[colorIndex + (8 * COLOR_COUNT_16_BIT)];
            *color[1] = pShadedTbl[colorIndex + (16 * COLOR_COUNT_16_BIT)];
        break;

        case 5:
            *color[1] = pShadedTbl[colorIndex];
            *color[0] = pShadedTbl[colorIndex + (8 * COLOR_COUNT_16_BIT)];
            *color[2] = pShadedTbl[colorIndex + (16 * COLOR_COUNT_16_BIT)];
        break;
    }

}   // DoFlipFlop


/************************************************************************
*    FUNCTION NAME:         SetImageHandle                                                             
*
*    DESCRIPTION:           Set the handle for the images
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::SetImageHandle()
{
    unsigned int count = Wormhole1Poly.GetTotalPolyCount(), i;

    switch( rand() % 7 )
    {
        case 0:
            for( i = 0; i < count; ++i )
            {
                if( (rand() % 2) == 0 )
                {
                    if( (rand() % 15) == 0 )
                    {
                        Wormhole1Poly.GetPoly( i )->SetTextureHandle( 2 );
                    }
                    else
                    {
                        Wormhole1Poly.GetPoly( i )->SetTextureHandle( 1 );
                    }
                }
                else
                {
                    Wormhole1Poly.GetPoly( i )->SetTextureHandle( 0 );
                }
            }
        break;

        case 1:
            for( i = 0; i < count; ++i )
            {
                if( (rand() % 3) == 0 )
                {
                    if( (rand() % 15) == 0 )
                    {
                        Wormhole1Poly.GetPoly( i )->SetTextureHandle( 2 );
                    }
                    else
                    {
                        Wormhole1Poly.GetPoly( i )->SetTextureHandle( 1 );
                    }
                }
                else
                {
                    Wormhole1Poly.GetPoly( i )->SetTextureHandle( 0 );
                }
            }
        break;

        case 2:
            for( i = 0; i < count; ++i )
            {
                if( (rand() % 4) == 0 )
                {
                    if( (rand() % 15) == 0 )
                    {
                        Wormhole1Poly.GetPoly( i )->SetTextureHandle( 2 );
                    }
                    else
                    {
                        Wormhole1Poly.GetPoly( i )->SetTextureHandle( 1 );
                    }
                }
                else
                {
                    Wormhole1Poly.GetPoly( i )->SetTextureHandle( 0 );
                }
            }
        break;

        case 3:
            for( i = 0; i < count; ++i )
            {
                if( (rand() % 5) == 0 )
                {
                    if( (rand() % 15) == 0 )
                    {
                        Wormhole1Poly.GetPoly( i )->SetTextureHandle( 2 );
                    }
                    else
                    {
                        Wormhole1Poly.GetPoly( i )->SetTextureHandle( 1 );
                    }
                }
                else
                {
                    Wormhole1Poly.GetPoly( i )->SetTextureHandle( 0 );
                }
            }
        break;

        case 4:
            for( i = 0; i < count; ++i )
            {
                if( (i & 1) == 1 )
                {
                    if( (rand() % 15) == 0 )
                    {
                        Wormhole1Poly.GetPoly( i )->SetTextureHandle( 2 );
                    }
                    else
                    {
                        Wormhole1Poly.GetPoly( i )->SetTextureHandle( 1 );
                    }
                }
                else
                {
                    Wormhole1Poly.GetPoly( i )->SetTextureHandle( 0 );
                }
            }
        break;

        case 5:
            for( i = 0; i < count; ++i )
            {
                if( (i & 2) == 2 )
                {
                    if( (rand() % 15) == 0 )
                    {
                        Wormhole1Poly.GetPoly( i )->SetTextureHandle( 2 );
                    }
                    else
                    {
                        Wormhole1Poly.GetPoly( i )->SetTextureHandle( 1 );
                    }
                }
                else
                {
                    Wormhole1Poly.GetPoly( i )->SetTextureHandle( 0 );
                }
            }
        break;

        case 6:
            for( i = 0; i < count; ++i )
            {
                if( (i & 3) == 3 )
                {
                    if( (rand() % 15) == 0 )
                    {
                        Wormhole1Poly.GetPoly( i )->SetTextureHandle( 2 );
                    }
                    else
                    {
                        Wormhole1Poly.GetPoly( i )->SetTextureHandle( 1 );
                    }
                }
                else
                {
                    Wormhole1Poly.GetPoly( i )->SetTextureHandle( 0 );
                }
            }
        break;
    }
}   // SetImageHandle


/************************************************************************
*    FUNCTION NAME:         BlitFromBufferToScreen                                                             
*
*    DESCRIPTION:           Blit to the buffer from the screen
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::BlitFromBufferToScreen()
{
    if( Mode() == ssSaver )
    {
        #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )

        #ifdef FULL_SCREEN_DEBUG
        DWORD startTime = timeGetTime();
        #endif
        
        if( Settings.GetSizeToFit() )
            buffer.Draw( GetScreenDC() );
        else
            buffer.Draw( GetScreenDC(), RnderRect.left, RnderRect.top );

        #else
        // Now blit the buffer to the screen only if not in shareware mode
        buffer.Draw( GetScreenDC(), &RnderRect );
        #endif

        #ifdef FULL_SCREEN_DEBUG
        scrnRenderTime = timeGetTime() - startTime;
        #endif
    }
    else
    {
        buffer.Draw( GetScreenDC() );
    }

    // Show an error screen if there is a problem playing music
    DisplayMusicError();

}   // BlitFromBufferToScreen


/************************************************************************
*    FUNCTION NAME:         ShowShareWareScreen                                                             
*
*    DESCRIPTION:           If we are in shareware mode and it has timed
*                           out, show the shareware screen
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CAnimateSaver::ShowShareWareScreen()
{
    BOOL Result = TRUE;

    // If this is the shareware version, show the shareware screen when ready
    if( ShareWareMode && ShareWareTimer < timeGetTime() && Mode() == ssSaver )
    {
        // Display the shareware screen
        // Don't do the below if the screen savers been killed
        if( DisplayShareWareScreen() )
        {
            ShareWareTimer = timeGetTime() + SHAREWARE_MESSAGE_DELAY;

            // RESET THE SCREEN HERE
            FillRect( GetScreenDC(), &DesktopRect, BlackBrush );
        }
        else
            Result = FALSE;
    }

    return Result;

}   // ShowShareWareScreen



/************************************************************************
*    FUNCTION NAME:         Destructor                                                             
*
*    DESCRIPTION:           Free up all allocated resources.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CAnimateSaver::~CAnimateSaver()
{
    if( ScreenResized )
        ChangeDisplaySettings( NULL, 0 );

    if( ZBuffer != NULL )
        delete [] ZBuffer;

    if( pShadedTbl != NULL )
		delete [] pShadedTbl;
            
}   // Destructor


/************************************************************************
*    FUNCTION NAME:         PauseSound                                                             
*
*    DESCRIPTION:           Pause the sound
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::PauseSound()
{
    int PlayerStatus;

    // Don't try to play any music if there has been any errors
    if( !MPlayer.GetPlayError() )
    {
        PlayerStatus = MPlayer.GetStatus();

        if( PlayerStatus == PLAYER_PLAYING )
            MPlayer.Pause();
    }
}   // PauseSound


/************************************************************************
*    FUNCTION NAME:         SetScreenResolution                                                             
*
*    DESCRIPTION:           Set the screen resolution
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::SetScreenResolution()
{
    DEVMODE dm;
    DWORD mode = 0;

    // Set the size of the device mode structure
    dm.dmSize = sizeof( DEVMODE );

    // search for the mode we are looking for
    while( EnumDisplaySettings( NULL, mode, &dm ) )
    {
        if( SystemBitDepth == dm.dmBitsPerPel && GetScreenResSizeID( dm.dmPelsWidth, dm.dmPelsHeight ) == Settings.GetRenderRes() )
        {
            // We found our device mode so change it
            if( ChangeDisplaySettings( &dm, 0 ) == DISP_CHANGE_SUCCESSFUL )
            {
                // Set the flag to true 
                ScreenResized = TRUE;

                // Reset the screen width and height
                // This is mainly for properly drawing the shareware screens
                ScreenWidth = dm.dmPelsWidth;
                ScreenHeight = dm.dmPelsHeight;
            }
            else
            {
                ChangeDisplaySettings( NULL, 0 );
            }

            // We are done here so break out
            break;
        }
    
        ++mode;
    }

    // If setting the resolution didn't work, reset the flag
    if( !ScreenResized )
        Settings.SetSizeToFit( FALSE );

}   // SetScreenResolution
