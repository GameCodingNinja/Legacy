
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

#include "sstools.h"         // A misc group of useful functions
#include "ssAnim.h"          // Header file for this *.cpp file.
#include <stdlib.h>          // Declares several commonly used routines such as atoi, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf


int hightlightindex[MAX_LIGHTNING] = { 4,3,5,6,7,8,7,6,3,5,3,4,1,1,7,6,7,1,7,1,3,8,6,4 };
WORD rainShades[MAX_RAIN_SHADES] = { 0xFFFF, 0x77BD, 0x6F7B, 0x6739, 0x5EF7, 0x56B5 };
WORD flashColors[MAX_FLASH_SHADES] = { 0xFFFF, 0x458B, 0x420C, 0x3E11, 0x3DA9, 0x35AF };
DWORD lightTimeOutAmounts[MAX_LIGHT_SET] = { 30000, 15000, 8000, 3000 };
int groupHitRand[MAX_LIGHT_SET] = { 24, 12, 6, 3 };
int blackOutRand[MAX_LIGHT_SET] = { 8, 4, 2, 1 };

/*
     RED_GRAY:    = RGB( 138, 99, 90 );     0x458B
     GREEN_GRAY:  = RGB( 130, 132, 100 );   0x420C
     BLUE_GRAY:   = RGB( 120, 132, 140 );   0x3E11
     YELLOW_GRAY: = RGB( 121, 108, 79 );    0x3DA9
     PURPLE_GRAY: = RGB( 111, 106, 121 );   0x35AF
*/


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

TAnimateSaver::TAnimateSaver( void *ssData ) : TScreenSaver( ssData )
{
    // Init screen saver specific functions
    lightCounter = 0;
    lightningActive = FALSE;
    flashActive = FALSE;
    blackoutActive = FALSE;
    blackoutTimer = 0;
    blackoutOnOffTimer = 0;
    blackoutOnOff = FALSE;
    updateBuffer = TRUE;
    updateCity = TRUE;
    updateNeighborhood = TRUE;
    multiLightCounter = 0;
    blackoutOnOffCounter = 0;
    blackoutOnOff = FALSE;
    flickerMax = 0;
    cityLightsOnIndex = 0;
    cityLightTimer = 0;
    neighborhoodLightTimer = 0;
    neighborhoodLightsOnOffIndex = 0;
    rainCount = 0;
    pRain = NULL;
    rainDirection = RAIN_STRAIGHT;
    ManySndDevices = FALSE;
    playLightningSnd = FALSE;
    lightSndTimer = 0;
    backSndTimer = 0;
    MoveX = 0;
    carDirection = 0;
    carIndex = 0;
    flashColorIndex = 0;
    carTimer = 0;

    // Init non-specific class members
    AnimCounter = 0;
    SaverTimer = 0;
    ShareWareMode = TRUE;
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

BOOL TAnimateSaver::LoadControlPanelPrefs()
{
    float floatScreenWidth;

    // Get the user's settings
    // Load all the ini file settings
    Settings.GetPrivateString();

    // Check to see if this is the registered version
    if( strcmp( FULL_VERSION_NUMBER, Settings.GetRegNum() ) == 0 )
        ShareWareMode = FALSE;

    else // Check to see if one of the temp reg numbers are being used
    {
        // If the first temp reg number doesn't pan out, try the second number
        if( ValidateTempRegNum( FIRST_CHECK_FILE, SHAREWAEW_TMP_RUN_MAX ) )
            ShareWareMode = FALSE;
        else
        {
            if( ValidateTempRegNum( SECOND_CHECK_FILE, SHAREWAEW_TMP_RUN_MAX ) )
                ShareWareMode = FALSE;
        }
    }

    // Force a smaller window for preview mode
    if( Mode() == ssPreview )
    {
        DesktopWidth = 640;
        DesktopHeight = 480;
        
        ScreenWidth = DesktopWidth;
        ScreenHeight = DesktopHeight;
        
        SetRect( &ScreenRect, 0, 0, ScreenWidth, ScreenHeight );
        SetRect( &DesktopRect, 0, 0, DesktopWidth, DesktopHeight );     
    }

    // Conver the width to a float
    floatScreenWidth = (float)ScreenWidth;

    switch( Settings.GetRainAmount() )
    {
        case RAIN_AMOUNT_SPRINKLE: rainCount = floatScreenWidth / 9.5f;  break;
        case RAIN_AMOUNT_SHOWER:   rainCount = floatScreenWidth / 7.5f;  break;
        case RAIN_AMOUNT_HEAVY:    rainCount = floatScreenWidth / 5.5f;   break;
        case RAIN_AMOUNT_DELUGE:   rainCount = floatScreenWidth / 4.0f;   break;
        case RAIN_AMOUNT_MONSOON:  rainCount = floatScreenWidth / 2.5f;   break;
        default: rainCount = floatScreenWidth / 5.5f;  break;
    }

    #ifdef DEBUG
    ShareWareMode = FALSE;
    #endif
    
    return TRUE;

}   // LoadControlPanelPrefs


/************************************************************************
*    FUNCTION NAME:         LoadGraphics                                                             
*
*    DESCRIPTION:           init buffers, load graphics etc.
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

BOOL TAnimateSaver::LoadGraphics()
{
    BOOL Result = FALSE;
    TBitmap ArtTmp;

    // Init the classes using the screens DC. That way it doesn't have to make one
    buffer.InitActiveDC( GetScreenDC() );

    // Set the error string in case we have an error loading the metafiles
    strcpy( errorStr, "Can't load metafile data. Your system resources may be low so try restarting your computer." );

    // Load the city metafiles
    if( !LoadMetaFileAnim( city, "city_%d", MAX_CITY ) )
        goto HANDLE_ERROR;

    // Load the city lights on metafiles
    if( !LoadMetaFileAnim( cityLightON, "city_LightsOn%d", MAX_CITY_LIGHTS_ON ) )
        goto HANDLE_ERROR;

    // Load the city lights off metafiles
    if( !LoadMetaFileAnim( cityLightOFF, "city_LightsOff%d", MAX_CITY_LIGHTS_OFF ) )
        goto HANDLE_ERROR;

    // Load the block highlights metafiles
    if( !LoadMetaFileAnim( blockHighLight, "Block_Highlight%d", MAX_HIGHLIGHT ) )
        goto HANDLE_ERROR;

    // Load the block window light metafiles
    if( !LoadMetaFileAnim( blockWindowLight, "Block_Lights%d", MAX_BLOCK_LIGHTS ) )
        goto HANDLE_ERROR;

    // Load the block window light metafiles
    if( !LoadMetaFileAnim( lightning, "lightning%d", MAX_LIGHTNING ) )
        goto HANDLE_ERROR;

    // Load the misc metafiles. the neighborhood and porch lights
    if( !LoadMetaFileAnim( blockMisc, "Block_Misc%d", MAX_MISC ) )
        goto HANDLE_ERROR;

    // Load the car metafiles.
    if( !LoadMetaFileAnim( car, "car%d", MAX_CARS ) )
        goto HANDLE_ERROR;

    // Setup the move rect
    SetRect( &moveRect, 0, 0, (float)ScreenWidth * CAR_MULTI, ScreenHeight );

    // calculate the width and height of the grid
    gridWidth = ScreenWidth / GRID_SIZE;

    // Make the buffer. We always need the buffer
    // Since we are going to be rendering the rain right to the buffer, we need the buffer to be top down
    // so that is why the height needs to be a negative number.
    if( !buffer.CreateDIBSectionBuffer( ScreenWidth, -ScreenHeight, BIT_COUNT_16 ) )
    {
        strcpy( errorStr, "Can't allocate background buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }


    Result = TRUE;

    HANDLE_ERROR:;

    return Result;

}   // LoadGraphics


/************************************************************************
*    FUNCTION NAME:         LoadSound                                                             
*
*    DESCRIPTION:           Loads the screen saver sound effects
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

BOOL TAnimateSaver::LoadSound()
{
    BOOL Result = FALSE;
    char tmpStr[20];
    int i;
    CWaveDevice testDevice;

    // Load the sound resources if we are to play sound
    if( SaverActive() && Mode() == ssSaver && Settings.GetSoundOn() )
    {
        // do a test to see if we can have many sound devices
        ManySndDevices = testDevice.TestMultiDeviceOpen();

        // Allocate all the devices we want if we can.
        if( ManySndDevices )
        {
            // Allocate all the objects to hold the sound data
            if( !sndMjr.AllocateWavLoad( MAX_REG_SOUNDS ) )
                goto HANDLE_ERROR;

            // Allocate all the devices to play the sound data
            if( !sndMjr.AllocateWavDevices( MAX_REG_SOUNDS ) )
                goto HANDLE_ERROR;

            // Load the regular sounds from resource
            // Pick the rain sound
            if( rainDirection == RAIN_STRAIGHT )
            {
                if( !sndMjr.LoadFromResource( Instance(), "sndRainStraight", SND_RAIN ) )
                    goto HANDLE_ERROR;
            }
            else
            {
                if( !sndMjr.LoadFromResource( Instance(), "sndLow0", SND_RAIN ) )
                    goto HANDLE_ERROR;
            }

            // Apply the sound to the devise
            if( !sndMjr.ApplyLoadToDevice( SND_RAIN, SND_RAIN ) )
                goto HANDLE_ERROR;
                    
            // Load the black out sound
            if( !sndMjr.LoadFromResource( Instance(), "sndBlackOut", SND_BLACK_OUT ) )
                goto HANDLE_ERROR;

            if( !sndMjr.ApplyLoadToDevice( SND_BLACK_OUT, SND_BLACK_OUT ) )
                goto HANDLE_ERROR;

            // Start playing the rain sound and make it loop forever
            sndMjr.Play( SND_RAIN, 0xFFFFFFFF );

            // Allocate all the objects to hold the sound data
            if( !sndThunderMjr.AllocateWavLoad( MAX_SND_THUNDER ) )
                goto HANDLE_ERROR;

            // Allocate all the devices to play the sound data
            if( !sndThunderMjr.AllocateWavDevices( MAX_SND_THUNDER ) )
                goto HANDLE_ERROR;

            // Load the thounder sounds from resource
            for( i = 0; i < MAX_SND_THUNDER; ++i )
            {
                wsprintf( tmpStr, "soundThunder%d", i );

                if( !sndThunderMjr.LoadFromResource( Instance(), tmpStr, i ) )
                    goto HANDLE_ERROR;

                if( !sndThunderMjr.ApplyLoadToDevice( i, i ) )
                    goto HANDLE_ERROR;
            }

            // Allocate all the objects to hold the sound data
            if( !sndBackground.AllocateWavLoad( MAX_BACK_SND ) )
                goto HANDLE_ERROR;

            // Allocate all the devices to play the sound data
            if( !sndBackground.AllocateWavDevices( MAX_BACK_SND ) )
                goto HANDLE_ERROR;

            // Load the thounder sounds from resource
            for( i = 0; i < MAX_BACK_SND; ++i )
            {
                wsprintf( tmpStr, "backSound%d", i );

                if( !sndBackground.LoadFromResource( Instance(), tmpStr, i ) )
                    goto HANDLE_ERROR;

                if( !sndBackground.ApplyLoadToDevice( i, i ) )
                    goto HANDLE_ERROR;
            }
        }
        else  // We can't have multiple devices
        {
            // Setup all the sounds
            if( !sndMjr.AllocateWavLoad( MAX_LOW_SOUNDS ) )
                goto HANDLE_ERROR;

            // All we get is one that all the sounds will have to share
            if( !sndMjr.AllocateWavDevices( 1 ) )
                goto HANDLE_ERROR;

            // Load the thounder sounds from resource
            for( i = 0; i < MAX_LOW_SOUNDS; ++i )
            {
                wsprintf( tmpStr, "sndLow%d", i );

                if( !sndMjr.LoadFromResource( Instance(), tmpStr, i ) )
                    goto HANDLE_ERROR;
            }

            // Kill the playing of any music because we can't have more then one device
            Settings.SetThemeMusicOn( FALSE );
            Settings.SetLoadYourOwn( FALSE );

            // Force a sound load. If there is any errors to be seen, it will help 
            // to cause the error to show up sooner.
            if( !sndMjr.ApplyLoadToDevice( SND_LOW_RAIN, 0 ) )
                goto HANDLE_ERROR;

            // Start playing the rain sound and make it loop forever
            if( !sndMjr.Play( 0, 0xFFFFFFFF ) )
                goto HANDLE_ERROR;
        }
    }
    else
        goto HANDLE_ERROR;

    Result = TRUE;

HANDLE_ERROR:;

    // If we had an error, kill the sound
    if( !Result )
        Settings.SetSoundOn( FALSE );   

    return Result;

}   // LoadSound


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

BOOL TAnimateSaver::InitSaverData()
{
    BOOL Result = FALSE;
    int i;

    // Randomize these here
    carDirection = rand() % 2;
    carIndex = rand() % MAX_CARS;
    flashColorIndex = rand() % MAX_FLASH_SHADES;

    // Setup the int list
    for( i = 0; i < MAX_LIGHTNING; ++i )
        lightningLst.Add( i );

    // Now mix up all the ints
    lightningLst.MixUp();

    // set the lightning timers
    lightTimer = timeGetTime() + lightTimeOutAmounts[Settings.GetLightHits()] + (rand() % RAND_LIGHTNING_TIMEOUT);
    flashTimer = lightTimer - FLASH_TIMEOUT;
    lightDurTimer = lightTimer + LIGHTNING_DUR_TIMEOUT;

    // Setup the rain
    if( !InitRain() )
    {
        strcpy( errorStr, "Can't allocate rain array. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // init to use the buffer
    errorCanvas.InitActiveDC( buffer.Handle() );

    // Create the default font
    errorCanvas.SetFont( "Arial", 12, FONT_BOLD, RGB( 255, 0, 0 ) );

    Result = TRUE;

    HANDLE_ERROR:;

    return Result;

}   // InitSaverData


/************************************************************************
*    FUNCTION NAME:         InitRain                                                             
*
*    DESCRIPTION:           Init rain data
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

BOOL TAnimateSaver::InitRain()
{
    BOOL Result = FALSE;
    int i;

    // Allocate the amount of rain data
    pRain = new SRain[rainCount];

    // Check all allocations
    if( !pRain )
    {
        rainCount = 0;
        goto HANDLE_ERROR;
    }

    // See if we need to randomise the rain direction
    if( Settings.GetRainType() == SET_RANDOM_RAIN )
        Settings.SetRainType( rand() % SET_RANDOM_RAIN );

    // Set the rain direction
    if( Settings.GetRainType() == SET_RAIN_STRAIGHT )
        rainDirection = RAIN_STRAIGHT;

    else if( Settings.GetRainType() == SET_DIAGONAL )
        rainDirection = RAIN_LEFT + (rand() % MAX_DIAGONAL_RAIN);

    for( i = 0; i < rainCount; ++i )
    {
        pRain[i].length = (ScreenHeight / 6) - (rand() % (ScreenHeight / 10));
        pRain[i].X1 = rand() % ScreenWidth;
        pRain[i].Y1 = rand() % ScreenHeight;
        pRain[i].X2 = pRain[i].X1;
        pRain[i].Y2 = pRain[i].Y1 - pRain[i].length;
        pRain[i].color = rainShades[rand() % MAX_RAIN_SHADES];

        // See the angle depending on the rain direction
        if( rainDirection == RAIN_LEFT )
            pRain[i].X2 += pRain[i].length;

        else if( rainDirection == RAIN_RIGHT )
            pRain[i].X2 -= pRain[i].length;
    }

    Result = TRUE;

    HANDLE_ERROR:;

    return Result;

}   // InitRain


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

void TAnimateSaver::InitMusicPlayer()
{
    // Allocate the media player for playing music
    if( SaverActive() && Mode() == ssSaver && (Settings.GetThemeMusicOn() || Settings.GetLoadYourOwn()) )
    {
        // Init the media player
        if( MPlayer.Init( Handle(), Instance(), DONT_USE_ERROR_DLG_BOXES ) )
        {
            // if they are loading their own music, load up all the sound file paths
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

void TAnimateSaver::DisplayBufferOnStartUp()
{
    // Blit the animations to the buffer and then display the buffer
    // This is only needed in full screen mode
    #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )
    if( Mode() == ssSaver && !ShareWareMode )
    {
        // Start the music playing before the screen saver shows. Some computers
        // take a while and grind for a second before the music starts playing. It's
        // best not to have the screen saver showing at this time because the animation will
        // be stopped solid in it's tracks waiting for the music to start.
        PlayMusic();
        
        // If this is a duel monitor senearo then we need to black out the screen
        if( IsDuelMonitor() )
            FillRect( GetScreenDC(), &WindowRect, BlackBrush );      
    }
    #endif

}   // DisplayBufferOnStartUp


/************************************************************************
*    FUNCTION NAME:         AnimateSaver                                                             
*
*    DESCRIPTION:           Animate the screen saver. This function is called
*                           by the thread.
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

void TAnimateSaver::AnimateSaver()
{
    // Check to see if we need to paint the rect the password dialog
    // box was at because it doesn't erase itself
    if( !PaintPasswordDialogRect() )
    {
        // Make sure the monitor is not asleep
        if( !GetMonitorIsAsleep() )
        {
            // Time out the screen saver only on the first ineration
            if( AnimCounter == 0 && SaverTimer < timeGetTime() )
            {
                Animate();
                SaverTimer = timeGetTime() + MAX_CYCLE_RENDER_WAIT;
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
        buffer.Draw( GetScreenDC(), GetPasswordWindowRect() );

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

void TAnimateSaver::PlayMusic()
{
    // Don't try to play any music if there has been any errors
    if( !Settings.GetMusicOff() && !MPlayer.GetPlayError() && MusicTimer < timeGetTime() )
    {
        // Get the status of the windows media player
        int PlayerStatus = MPlayer.GetStatus();

        // Do we play the included star song?
        if( Settings.GetThemeMusicOn() )
        {
        /*
            // This will play the song that has already been loaded
            if( PlayerStatus == PLAYER_STOPPED || PlayerStatus == PLAYER_PAUSED )
                MPlayer.Play();

            // This will load the song and play it for the first time
            else if( PlayerStatus == PLAYER_CLOSED )
                MPlayer.Play( "ThemeSong", "MIDI" );
        */
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

void TAnimateSaver::DisplayMusicError()
{
    if( MPlayer.GetPlayError() )
    {
        // Set the background mode to transparent for the text
        errorCanvas.SetBackgroundMode( TRANSPARENT );

        // Draw the text
        errorCanvas.DrawText( 0, 0, MPlayer.GetErrorStr() );

        // Set the background mode back to opaque
        errorCanvas.SetBackgroundMode( OPAQUE );
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

void TAnimateSaver::Animate()
{
    switch( AnimCounter )
    {
        // Do we need to play music
        case PLAY_MUSIC:       PlayMusic();               break;

        // Update all the animations
        case UPDATE_ANIM:      UpdateAllAnimations();     break;

        // Blit to buffer
        case BLIT_TO_BUFFER:   BlitToTheBuffer();         break;

        // Do the shareware check
        case SHAREWARE_NOTICE: ShowShareWareScreen();     break;

        // Blit to screen
        case BLIT_TO_SCREEN:   BlitFromBufferToScreen();  break;
    }

    // Inc the animation counter
    AnimCounter = ( AnimCounter + 1 ) % NUM_BLIT_OPERATIONS;

}   // Animate


/************************************************************************
*    FUNCTION NAME:         UpdateAllAnimations                                                             
*
*    DESCRIPTION:           Updates all the animation positions
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

void TAnimateSaver::UpdateAllAnimations()
{
    DWORD timer = timeGetTime();

    // Do we play any lightning sounds
    if( playLightningSnd && lightSndTimer < timer )
    {
        if( Settings.GetSoundOn() )
        {
            if( ManySndDevices )
            {
                // Play the lightning sound
                sndThunderMjr.PlayRandom();
            }
            else
            {
                // Load the thunder sound
                sndMjr.ApplyLoadToDevice( SND_LOW_THUNDER1 + (rand() % MAX_LOW_THUNDER_SOUNDS), 0 );

                // Play the sound
                sndMjr.Play();
            }
        }

        playLightningSnd = FALSE;
    }

    if( flashTimer < timer )
    {
        // Set the flag that says we have to flash the screen
        flashActive = TRUE;

        // Indicate a full buffer update is needed
        updateBuffer = TRUE;

        // Reset the timer so high that it doesn't
        flashTimer = timer + 50000;

        // randomly pick the flash color
        flashColorIndex = rand() % MAX_FLASH_SHADES;
    }

    // Is it time to animate the lightning
    else if( lightTimer < timer )
    {
        // Set the flag that says we have lightning on the screen
        lightningActive = TRUE;

        // Kill the flash
        flashActive = FALSE;

        // Reset the timer so high that it doesn't
        lightTimer = timer + 50000;

        // Inc the counter
        lightCounter = (lightCounter + 1) % MAX_LIGHTNING;

        // Now mix up all the ints
        if( lightCounter == 0 )
            lightningLst.MixUp();

        // Indicate a full buffer update is needed
        updateBuffer = TRUE;

        // Set the duration this lightning will stay on the screen
        lightDurTimer = timer + LIGHTNING_DUR_TIMEOUT;

        // Play the lightning sound
        playLightningSnd = TRUE;
        lightSndTimer = timer + LIGHTNING_SND_DELAY;
    }

    // Time out the lightning on the screen
    else if( lightDurTimer < timer )
    {
        // Indicate a full buffer update is needed
        updateBuffer = TRUE;

        // Set the duration real high so that it doesn't come 
        // through here again before the next real flash
        lightDurTimer = timer + 50000;

        // do multiple lightnings
        if( multiLightCounter < MAX_MULTI_LIGHT_HITS && (rand() % groupHitRand[Settings.GetGroupHits()]) == 0 )
        {
            lightTimer = 0;
            ++multiLightCounter;
        }
        else
        {
            // Reset the counter back to z
            multiLightCounter = 0;

	        // Set the flag that says we don't have lightning on the screen
	        lightningActive = FALSE;

            // reset the timer
            cityLightTimer = timer + CITY_LIGHT_TIMEOUT;

	        // Reset the time to the next lightning strike
	        lightTimer = timer + lightTimeOutAmounts[Settings.GetLightHits()] + (rand() % RAND_LIGHTNING_TIMEOUT);

	        // Reset the flash timer
	        flashTimer = lightTimer - FLASH_TIMEOUT;

            // Do we want a black out now?
            // do it on a white flash and if there was a delay from the last blackout
            if( !blackoutActive && (blackoutTimer + MAX_DELAY_TIL_NEXT_BLACKOUT < timer) &&
                (rand() % blackOutRand[Settings.GetBlackOuts()]) == 0 && flashColorIndex == FLASH_WHITE )
            {
                // Set the flag that says we have blackout on the screen
                blackoutActive = TRUE;

                // Set the duration this blackout will stay on the screen
                blackoutTimer = timer + BLACKOUT_MIN_TIMEOUT + (rand() % BLACKOUT_MAX_TIMEOUT);

                // Set the flicker timer
                blackoutOnOffTimer = timer + FLICKER_TIMEOUT;

                // Set the number of flicker intervals
                flickerMax = 5;

                // Reset the flicker counter
                blackoutOnOffCounter = 0;

                if( Settings.GetSoundOn() )
                {
                    if( ManySndDevices )
                    {
                        // Play the black out sound
                        sndMjr.Play( SND_BLACK_OUT );
                    }
                    else
                    {
                        // Only play if sound is rain
                        if( sndMjr.GetSoundIndex() == SND_LOW_RAIN )
                        {
                            // Load the thunder sound
                            sndMjr.ApplyLoadToDevice( SND_LOW_BLACK_OUT, 0 );

                            // Play the sound
                            sndMjr.Play();
                        }
                    }
                }
            }
        }
    }

    // Is it time to stop the blackout?
    if( blackoutActive && blackoutTimer < timer )
    {
        // Indicate a full buffer update is needed
        updateBuffer = TRUE;

        // Set the flag that says we don't have a blackout on the screen
        blackoutActive = FALSE;

        // reset the timers
        cityLightTimer = timer + CITY_LIGHT_TIMEOUT;
        neighborhoodLightTimer = timer + (rand() % MAX_NEIGHBORHOOD_LIGHT_TIMEOUT) + 
                                         MIN_NEIGHBORHOOD_LIGHT_TIMEOUT;

        // Set the flicker timer
        blackoutOnOffTimer = timer + FLICKER_TIMEOUT;

        // Set the number of flicker intervals
        flickerMax = 6;

        // Reset the flicker counter
        blackoutOnOffCounter = -1;

        if( Settings.GetSoundOn() )
        {
            if( ManySndDevices )
            {
                // Play the black out sound
                sndMjr.Play( SND_BLACK_OUT );
            }
            else
            {
                // Only play if sound is rain
                if( sndMjr.GetSoundIndex() == SND_LOW_RAIN )
                {
                    // Load the thunder sound
                    sndMjr.ApplyLoadToDevice( SND_LOW_BLACK_OUT, 0 );

                    // Play the sound
                    sndMjr.Play();
                }
            }
        }
    }

    // Blink the lights on and off
    if( blackoutOnOffCounter < flickerMax && blackoutOnOffTimer < timer )
    {
        // Inc the blackout on/off counter
        ++blackoutOnOffCounter;

        if( blackoutOnOffCounter % 2 )
            blackoutOnOff = TRUE;
        else
            blackoutOnOff = FALSE;

        // Update everything
        updateBuffer = TRUE;

        // Reset the flicker timer
        blackoutOnOffTimer = timer + 200;
    }

    // Is it time to flicker the city lights
    if( !blackoutActive && !lightningActive && cityLightTimer < timer )
    {
        // Pick the city light group
        cityLightsOnIndex = rand() % MAX_ON_OFF_LIGHTS;

        // Set the flag to indicate an update is due
        updateCity = TRUE;

        // reset the timer
        cityLightTimer = timer + CITY_LIGHT_TIMEOUT;
    }

    // Is it time to change the neighborhood lights
    if( !blackoutActive && !lightningActive && neighborhoodLightTimer < timer )
    {
        // Pick the neighborhood light to change
        neighborhoodLightsOnOffIndex = rand() % MAX_NEIGHBORHOOD_ON_OFF_LIGHTS;

        // Set the flag to indicate an update is due
        updateNeighborhood = TRUE;

        // reset the timer
        neighborhoodLightTimer = timer + (rand() % MAX_NEIGHBORHOOD_LIGHT_TIMEOUT) + 
                                         MIN_NEIGHBORHOOD_LIGHT_TIMEOUT;
    }

    // If we have to do a buffer update, then everything needs an update
    if( updateBuffer )
    {
        updateCity = TRUE;
        updateNeighborhood = TRUE;
    }

    // We have to keep the rain sound looping
    // So if this is a limited sound situation
    if( !ManySndDevices && Settings.GetSoundOn() && !sndMjr.IsSndError() && !sndMjr.IsPlaying() )
    {
        // Load the rain sound
        sndMjr.ApplyLoadToDevice( SND_LOW_RAIN, 0 );

        // Start playing the rain sound and make it loop forever
        sndMjr.Play( 0, 0xFFFFFFFF );
    }

    // Play any background sounds
    if( ManySndDevices && Settings.GetSoundOn() && backSndTimer < timer )
    {
        if( !sndThunderMjr.IsMjrPlaying() && !sndBackground.IsMjrPlaying() )
        {
            // Don't let the siren play so often
            if( sndBackground.GetNextDevToPlay() == SND_BACK_SIREN && (rand() % 3) )
                sndBackground.IncDevCounter();
            else
                sndBackground.PlayRandom();
        }

        // Reset the timer
        backSndTimer = timer + 1000;
    }

    // Move the car accross the screen
    if( carTimer < timer )
    {
        // movement counter
        MoveX = ( MoveX + 1 ) % (int)((float)GRID_SIZE * CAR_MULTI_MOVE);

        // move the car
        if( MoveX )
        {
            // Have the mouse walk from the left or the right
            if( carDirection )
            {
                moveRect.left = -(MoveX * gridWidth);
                moveRect.right = ((float)ScreenWidth * CAR_MULTI) + moveRect.left;
            }
            else
            {
                moveRect.right = (MoveX * gridWidth) - ((float)ScreenWidth * CAR_MULTI_MOVE);
                moveRect.left = ((float)ScreenWidth * CAR_MULTI) + moveRect.right;
            }
        }
        else
        {
            // Change the direction of the car
            carDirection  = rand() % 2;

            // Randomly pick the car we are animating
            carIndex = rand() % MAX_CARS;

            // Rese the timer
            carTimer = timer + (rand() % 10000) + 5000;
        }
    }

}   // UpdateAllAnimations


/************************************************************************
*    FUNCTION NAME:         BlitToTheBuffer                                                             
*
*    DESCRIPTION:           Blit all the animations to the buffer
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

void TAnimateSaver::BlitToTheBuffer()
{
    // Do we need to do a full buffer update
    if( updateBuffer )
    {
        // For now, just fill with black
        if( lightningActive )
            ClearVideoBuffer( 0x842 ); // RGB( 19, 21, 22 )

        else if( flashActive )
            ClearVideoBuffer( flashColors[flashColorIndex] );

        else
            ClearVideoBuffer( 0 );
    }
    else
        // Erase the rain if not a full update
        AnimateRain();

    // Animate the rain
    UpdateRain();

    // Animate the city
    AnimateCity();

    // Animate the lightning if it is time
    AnimateLightning();

    // Animate Neighborhood
    AnimateNeighborhood();

    // draw the car if we are moving
    if( MoveX )
        car[ carIndex ].Draw( buffer.Handle(), &moveRect );

    // Draw the lines of rain
    AnimateRain();

    // Reset all the flags
    updateBuffer = FALSE;
    updateCity = FALSE;
    updateNeighborhood = FALSE;

}   // BlitToTheBuffer


/************************************************************************
*    FUNCTION NAME:         AnimateRain                                                             
*
*    DESCRIPTION:           Draw the lines of rain
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

void TAnimateSaver::AnimateRain()
{
    int i;

    for( i = 0; i < rainCount; ++i )
        buffer.Line16bit( pRain[i].X1, pRain[i].Y1, pRain[i].X2, pRain[i].Y2, pRain[i].color, RENDER_XOR );

}   // AnimateRain


/************************************************************************
*    FUNCTION NAME:         UpdateRain                                                             
*
*    DESCRIPTION:           Update the rain animation
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

void TAnimateSaver::UpdateRain()
{
    int i;
    // predivision for speed
    int ScreenHeightDiv3(ScreenHeight / 3);
    int ScreenHeightDiv4(ScreenHeight / 4);
    int ScreenHeightDiv6(ScreenHeight / 6);
    int ScreenHeightDiv8(ScreenHeight / 8);
    int ScreenHeightDiv10(ScreenHeight / 10);

    int ScreenWidthDiv4(ScreenWidth / 4);
    int ScreenWidthDiv8(ScreenWidth / 8);
    int ScreenWidthDiv10(ScreenWidth / 10);

    // See the angle depending on the rain direction
    if( rainDirection == RAIN_STRAIGHT )
    {
        for( i = 0; i < rainCount; ++i )
        {
            pRain[i].Y1 += ScreenHeightDiv3;
            pRain[i].Y2 = pRain[i].Y1 - pRain[i].length;

            // See if we need to start the rain over
            if( pRain[i].Y2 > ScreenHeight )
            {  
                pRain[i].length = ScreenHeightDiv6 - (rand() % ScreenHeightDiv10);
                pRain[i].X1 = rand() % ScreenWidth;
                pRain[i].Y1 = rand() % ScreenHeightDiv3;
                pRain[i].X2 = pRain[i].X1;
                pRain[i].Y2 = pRain[i].Y1 - pRain[i].length;
                pRain[i].color = rainShades[rand() % MAX_RAIN_SHADES];
            }
        }
    }
    else if( rainDirection == RAIN_LEFT )
    {
        for( i = 0; i < rainCount; ++i )
        {
            pRain[i].X1 -= ScreenHeightDiv4;
            pRain[i].Y1 += ScreenHeightDiv4;
            pRain[i].X2 = pRain[i].X1 + pRain[i].length;
            pRain[i].Y2 = pRain[i].Y1 - pRain[i].length;

            // See if we need to start the rain over
            if( pRain[i].Y2 > ScreenHeight || pRain[i].X2 < 0 )
            {  
                pRain[i].length = ScreenHeightDiv6 - (rand() % ScreenHeightDiv8);
                pRain[i].Y1 = (rand() % (ScreenHeight + ScreenHeightDiv10))-ScreenHeightDiv10;
                pRain[i].X1 = rand() % (ScreenWidth + ScreenWidthDiv8);
             
                // If the rain is falling too much into the middle, force it 
                // back to the top or to the side
                if( (i % 4) && pRain[i].Y1 > ScreenHeightDiv3 && pRain[i].X1 < ScreenWidth - ScreenWidthDiv4 )
                {
                    if( i % 2 )
                        pRain[i].X1 = ScreenWidth - (rand() % ScreenWidthDiv4);
                    else
                        pRain[i].Y1 = rand() % ScreenHeightDiv4;
                }
                    
                pRain[i].X2 = pRain[i].X1 + pRain[i].length;
                pRain[i].Y2 = pRain[i].Y1 - pRain[i].length;
                pRain[i].color = rainShades[rand() % MAX_RAIN_SHADES];
            }
        }
    }
    else if( rainDirection == RAIN_RIGHT )
    {
        for( i = 0; i < rainCount; ++i )
        {
            pRain[i].X1 += ScreenHeightDiv4;
            pRain[i].Y1 += ScreenHeightDiv4;
            pRain[i].X2 = pRain[i].X1 - pRain[i].length;
            pRain[i].Y2 = pRain[i].Y1 - pRain[i].length;

            // See if we need to start the rain over
            if( pRain[i].Y2 > ScreenHeight || pRain[i].X2 > ScreenWidth )
            {  
                pRain[i].length = ScreenHeightDiv6 - (rand() % ScreenHeightDiv8);
                pRain[i].Y1 = (rand() % (ScreenHeight + ScreenHeightDiv10))-ScreenHeightDiv10;
                pRain[i].X1 = (rand() % (ScreenWidth + ScreenWidthDiv8)) - ScreenWidthDiv10;

                // If the rain is falling too much into the middle, force it 
                // back to the top or to the side
                if( (i % 4) && pRain[i].Y1 > ScreenHeightDiv3 && pRain[i].X1 > ScreenWidthDiv4 )
                {
                    if( i % 2 )
                        pRain[i].X1 = (rand() % ScreenWidthDiv4)-ScreenWidthDiv8;
                    else
                        pRain[i].Y1 = rand() % ScreenHeightDiv4;
                }

                pRain[i].X2 = pRain[i].X1 - pRain[i].length;
                pRain[i].Y2 = pRain[i].Y1 - pRain[i].length;
                pRain[i].color = rainShades[rand() % MAX_RAIN_SHADES];
            }
        }
    }  

}   // UpdateRain


/************************************************************************
*    FUNCTION NAME:         AnimateCity                                                             
*
*    DESCRIPTION:           Animate the city
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

void TAnimateSaver::AnimateCity()
{
    int i;

    // Do we need to redraw the city
    if( updateCity )
    {
        // Are we under blackout conditions?
        if( blackoutOnOff )
        {
            if( lightningActive )
            {
                // We only need to update the whole city if a full update is required
                if( updateBuffer )
                    city[ 1 ].Draw( buffer.Handle(), &ScreenRect );
            }
            else
            {
                // We only need to update the whole city if a full update is required
                if( updateBuffer )
                    city[ 2 ].Draw( buffer.Handle(), &ScreenRect );               
            }

            // Render the rest of the off lights
            for( i = 0; i < MAX_CITY_LIGHTS_OFF; ++i )
                cityLightOFF[ i ].Draw( buffer.Handle(), &ScreenRect );

        }
        else
        {
            if( lightningActive )
            {
                // We only need to update the whole city if a full update is required
                if( updateBuffer )
                    city[ 0 ].Draw( buffer.Handle(), &ScreenRect );
            }
            else
            {
                // We only need to update the whole city if a full update is required
                if( updateBuffer )
                    city[ 1 ].Draw( buffer.Handle(), &ScreenRect );
            }

            // Render the base lights
            cityLightON[ 0 ].Draw( buffer.Handle(), &ScreenRect );

            // Render the rest of the off lights
            for( i = 1; i < MAX_CITY_LIGHTS_ON; ++i )
            {
                // Turn the selected lights on
                if( i == cityLightsOnIndex )
                    cityLightON[ i ].Draw( buffer.Handle(), &ScreenRect );
                else
                    cityLightOFF[ i ].Draw( buffer.Handle(), &ScreenRect );
            }
        }       
    }

}   // AnimateCity


/************************************************************************
*    FUNCTION NAME:         AnimateLightning                                                             
*
*    DESCRIPTION:           Flash the lightning
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

void TAnimateSaver::AnimateLightning()
{
    RECT tmpRect;
         
    if( lightningActive && updateBuffer )
    {
        tmpRect = ScreenRect;

        // Render the lightning backwards half the time
        if( rand() % 2 )
        {
            tmpRect.left = ScreenRect.right;
            tmpRect.right = ScreenRect.left;
        }
        
        // Draw a bolt of lightning
        lightning[ lightningLst.GetInt( lightCounter ) ].Draw( buffer.Handle(), &tmpRect );
    }

}   // AnimateLightning


/************************************************************************
*    FUNCTION NAME:         AnimateNeighborhood                                                             
*
*    DESCRIPTION:           Animate the Neighborhood
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

void TAnimateSaver::AnimateNeighborhood()
{
    // Do we need to redraw the neighborhood
    if( updateNeighborhood )
    {
        // Are we under blackout conditions?
        if( blackoutOnOff )
        {
            if( lightningActive )
            {
                // We only need to update the block if the city update is required
                if( updateCity )
                {
                    // The black neighborhood fill
                    blockMisc[ 0 ].Draw( buffer.Handle(), &ScreenRect );
                    
                    // Top of neighbohood outline
                    blockHighLight[ hightlightindex[ lightningLst.GetInt( lightCounter ) ] ].Draw( buffer.Handle(), &ScreenRect );
                }                               
            }
            else
            {
                // We only need to update the block if the city update is required
                if( updateCity )
                {
                    // The black neighborhood fill
                    blockMisc[ 0 ].Draw( buffer.Handle(), &ScreenRect );
                    
                    // Top of neighbohood outline
                    blockHighLight[ 0 ].Draw( buffer.Handle(), &ScreenRect );
                }             
            }

            // All lights out
            blockWindowLight[ 1 ].Draw( buffer.Handle(), &ScreenRect );
        }
        else
        {
            if( lightningActive )
            {
                // We only need to update the block if the city update is required
                if( updateCity )
                {
                    // The black neighborhood fill
                    blockMisc[ 0 ].Draw( buffer.Handle(), &ScreenRect );
                    
                    // Porch lights
                    blockMisc[ 1 ].Draw( buffer.Handle(), &ScreenRect );

                    // Top of neighbohood outline
                    blockHighLight[ hightlightindex[ lightningLst.GetInt( lightCounter ) ] ].Draw( buffer.Handle(), &ScreenRect );
                }
            }
            else
            {
                // We only need to update the block if the city update is required
                if( updateCity )
                {
                    // The black neighborhood fill
                    blockMisc[ 0 ].Draw( buffer.Handle(), &ScreenRect );
                    
                    // Porch lights
                    blockMisc[ 1 ].Draw( buffer.Handle(), &ScreenRect );

                    // Top of neighbohood outline
                    blockHighLight[ 0 ].Draw( buffer.Handle(), &ScreenRect );
                }
            }

            // Base lights
            blockWindowLight[ 0 ].Draw( buffer.Handle(), &ScreenRect );

            // The added or removed light
            blockWindowLight[ 2 + neighborhoodLightsOnOffIndex ].Draw( buffer.Handle(), &ScreenRect );
        }
    }

}   // AnimateNeighborhood


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

void TAnimateSaver::BlitFromBufferToScreen()
{
    if( Mode() == ssSaver )
    {
        // Show an error screen if there is a problem playing music
        DisplayMusicError();
        DisplaySoundError();

        // Us the RepaintScreen function to indicate we have come back
        // from sleeping. Use this to restart the looping background sound.
        if( RepaintScreen() && Settings.GetSoundOn() )
            sndMjr.RestartDev();

        #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )

            // Blit the flame animation
            buffer.Draw( GetScreenDC() );

        #else
            buffer.StretchDraw( GetScreenDC(), &WindowRect );
        #endif
    }
    else
    {
        buffer.StretchDraw( GetScreenDC(), &WindowRect );
    }

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

BOOL TAnimateSaver::ShowShareWareScreen()
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

            // Blit the whole buffer to the screen
            buffer.Draw( GetScreenDC(), &DesktopRect );

            // Reset the default font to show errors
            canvas.SetFont( "Arial", 12, FONT_BOLD, RGB( 255, 0, 0 ) );
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

TAnimateSaver::~TAnimateSaver()
{
    // Do any cleanup before the buffer is freed 
    // because this canvas uses the buffer's HDC
    errorCanvas.Free();

    if( pRain )
    {
        delete [] pRain;
        pRain = NULL;
    }
        
                    
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

void TAnimateSaver::PauseSound()
{
    int PlayerStatus;

    // Don't try to play any music if there has been any errors
    if( !MPlayer.GetPlayError() )
    {
        PlayerStatus = MPlayer.GetStatus();

        if( PlayerStatus == PLAYER_PLAYING )
            MPlayer.Pause();
    }

    // Pause the looping fireworks background sound
    if( Settings.GetSoundOn() )
        sndMjr.PauseDev();

}   // PauseSound


/************************************************************************
*    FUNCTION NAME:         LoadMetaFileAnim                                                             
*
*    DESCRIPTION:           Load the metafile animation into the metafile array
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

BOOL TAnimateSaver::LoadMetaFileAnim( PTMetafile pMetaFile, char *ResName, int noOfAnim )
{
    BOOL Result = TRUE;
    int i;
    char metaResName[30];

    for( i = 0; i < noOfAnim; ++i )
    {
        // Generate the resource name
        sprintf( metaResName, ResName, i );

        // Init the metafile class with the active DC. We already have it so it doen't need to be made again
        pMetaFile[i].InitActiveDC( GetScreenDC() );

        // Load the metafile
        if( !pMetaFile[i].LoadFromResource( Instance(), metaResName, "WMF" ) )
        {
            Result = FALSE;
            break;
        }
    }

    return Result;

}   // LoadMetaFileAnim


/************************************************************************
*    FUNCTION NAME:              ClearVideoBuffer
*
*    DESCRIPTION:                Clear the video buffer with a specific color
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*
*    Output:   BOOL            - Returns TRUE or FALSE to indicate if
*                                the task was secessful.
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TAnimateSaver::ClearVideoBuffer( DWORD color )
{
    int i, size;
    DWORD register *vidBuff;
    DWORD wordColor;

    // get the size in DWORD length
    size = (buffer.width() * buffer.height()) >> 1;

    // We only need to update a third of the buffer
    size -= (size >> 2);

    // get the pointer to the buffer
    vidBuff = (DWORD *)buffer.GetPtrToDIBsectionBits();

    // Set the 16 bit color into both halfs of the 32 bit data
    wordColor = color << 16;
    wordColor |= color; 

    for( i = 0; i < size; ++i )
        vidBuff[i] = wordColor;

}   // ClearVideoBuffer


/************************************************************************
*    FUNCTION NAME:         DisplaySoundError                                                             
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

void TAnimateSaver::DisplaySoundError()
{
    if( sndMjr.IsSndError() )
    {
        // Set the background mode to transparent for the text
        errorCanvas.SetBackgroundMode( TRANSPARENT );

        // Draw the text
        errorCanvas.DrawText( 5, 5, sndMjr.GetErrorStr() );

        // Set the background mode back to opaque
        errorCanvas.SetBackgroundMode( OPAQUE );
    }
    else if( sndThunderMjr.IsSndError() )
    {
        // Set the background mode to transparent for the text
        errorCanvas.SetBackgroundMode( TRANSPARENT );

        // Draw the text
        errorCanvas.DrawText( 5, 5, sndThunderMjr.GetErrorStr() );

        // Set the background mode back to opaque
        errorCanvas.SetBackgroundMode( OPAQUE );
    }
    else if( sndBackground.IsSndError() )
    {
        // Set the background mode to transparent for the text
        errorCanvas.SetBackgroundMode( TRANSPARENT );

        // Draw the text
        errorCanvas.DrawText( 5, 5, sndBackground.GetErrorStr() );

        // Set the background mode back to opaque
        errorCanvas.SetBackgroundMode( OPAQUE );
    }

}   // DisplaySoundError