
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


// Externed from SSConfig.cpp
extern int ResCheck[MAX_RES_CHECK];
extern int BlitToScreen[MAX_RES_CHECK];

int frameCount[MAX_SANTA_DANCES] = { 10,11,9,11,9,16,24,9,11,11,11,11 };
int animType[MAX_SANTA_DANCES] = { 0,0,0,0,0,0,1,0,0,0,0,0 };
int santaOffsetY[MAX_SANTA_DANCES] = { 0,0,0,0,60,0,0,0,0,0,0,0 };
int noseGlow[MAX_GLOW_COUNT] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,21,22,23,23,23,23,23,22,21,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

int rudolfFrameCount[MAX_RUDOLF_DANCES] = { 7,13,16 };
int rudolfAnimType[MAX_RUDOLF_DANCES] = { 0,0,0 };

int polyAllow[MAX_CAMERA_VIEWS] = { 128,128,80,47,72,72,72,46,46,45,
                                    58,46,47,47,125,118,48,117,72,72,
                                    72,128,128,60,72,72 };


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
    cameraIndex = 0;
    frameCounter = 0;
    pingPongDir = 1;
    rudolfPingPongDir = 1;
    danceIndex = 0;
    cameraCounter = 0;
    cameraFrameCounter = 0;
    danceCounter = 0;
    rudolfFrameCounter = 0;
    rudolfDanceIndex = 0;
    noseGlowCounter = 0;

    // Init non-specific class members
    ResLessThen640x480 = FALSE;
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
    int RenderWidth, RenderHeight, ScreenResID, i;
    DWORD RenderTime;

    // Get the user's settings
    // Load all the ini file settings
    Settings.GetPrivateString();

    // Check to see if this is the registered version
    /*if( strcmp( FULL_VERSION_NUMBER, Settings.GetRegNum() ) == 0 )
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
    }*/

    // Get the current res ID for the screen size
    ScreenResID = GetScreenResSizeID( ScreenRect.right, ScreenRect.bottom );

    // Calculate the maximum render size
    if( Settings.GetCalcRenderRes() )
    {        
        for( i = 0; i < MAX_RES_CHECK; ++i )
        {
            RenderTime = GetRenderTime( ResCheck[i], Instance() );

            // Make sure we at least set the first resolution if there wasn't an error
            if( RenderTime <= (MAX_MIL_RENDER_WAIT - BlitToScreen[i]) )
                Settings.SetRenderRes( ResCheck[i] );
            else
                break;
        }

        // Make sure we don't save a render resolution higher then the screen size
        if( Settings.GetRenderRes() > ScreenResID )
            Settings.SetRenderRes( ScreenResID );

        // Set the calculate to false so we only do this once
        Settings.SetCalcRenderRes( FALSE );
        
        // Save all the ini file settings
        Settings.WritePrivateString();  
    }

    // Make sure we don't save a render resolution higher then the screen size
    if( Settings.GetRenderRes() > ScreenResID )
        Settings.SetRenderRes( ScreenResID );

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
    CameraSize = 180;
    RnderRect = WindowRect;
    #endif

    if( Mode() == ssPreview )
    {
        RnderRect = WindowRect;
        CameraSize = 87;
    }

    // Reset the bit depth because we only want to
    // allow 8 or 16 bit screen depths
    if( SystemBitDepth <= 8 && !(Settings.GetForce16BitColor() && Mode() == ssSaver) )
        SystemBitDepth = 8;
    else
        SystemBitDepth = 16;

    
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

BOOL CAnimateSaver::LoadGraphics()
{
    BOOL Result = FALSE;
    char ResName[30];
    TLoadRes LoadRes;
    int i, j, offset[2] = { -1,1 };
    Matrix3D tmpMatrix;

    // Init the classes using the screens DC. That way it doesn't have to make one
    buffer.InitActiveDC( GetScreenDC() );

    // Allocate the Z Buffer
    ZBuffer = new int[ RnderRect.right * RnderRect.bottom ];
    if( !ZBuffer )
    {
        strcpy( errorStr, "Can't allocate Z buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Zero out the ZBuffer
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // One time initilization of the cos and sin tables
    // Makes no difference who calls it because the tables are static
    tmpMatrix.InitCosSinTables();


    // Set the random start pos and offsets for all the snow
    for( i = 0; i < MAX_SNOW; ++i )
    {
        snowOffset[i].XPos = (rand() % MAX_SNOW_X) * offset[rand() % 2];
        snowOffset[i].YPos = (rand() % MAX_Y_SNOW_HEIGHT) + Y_BOT_OFFSET;
        snowOffset[i].ZPos = (rand() % MAX_SNOW_Z) * offset[rand() % 2];

        // Push the snow out of the way so that it doesn't fall through Santa and Rudolf
        if( snowOffset[i].XPos > -SNOW_CHAR_OFFSET_X && snowOffset[i].XPos < SNOW_CHAR_OFFSET_X &&
            snowOffset[i].ZPos > -SNOW_CHAR_OFFSET_Z && snowOffset[i].ZPos < SNOW_CHAR_OFFSET_Z )
        {
            // Randomize a new postion out of range
            if( rand() % 2 )
                snowOffset[i].XPos = ((rand() % (MAX_SNOW_X - SNOW_CHAR_OFFSET_X)) + SNOW_CHAR_OFFSET_X) * offset[rand() % 2];
            else
                snowOffset[i].ZPos = ((rand() % (MAX_SNOW_Z - SNOW_CHAR_OFFSET_Z)) + SNOW_CHAR_OFFSET_Z) * offset[rand() % 2];
        }

        snowOffset[i].ZRot = rand() & (DEGREECOUNT-1);
        snowOffset[i].artIndex = i % MAX_SNOW_FLAKE_ART;
    }

    //////////////////////////////////////////////////////
    //
    //            Load Polygon Objects
    //
    //////////////////////////////////////////////////////

    // Load all of santa's dances
    for( i = 0; i < MAX_SANTA_DANCES; ++i )
    {
        for( j = 0; j < frameCount[i]; ++j )
        {
            // Generate the resource name
            sprintf( ResName, "SantaDance%d_%d", i, j );

            // Load the polygon object from resource
            if( !LoadTheResource( Instance(), ResName, "POLY", &LoadRes ) )
                goto HANDLE_ERROR;

            // Load the polygon object 
            if( !santaPly[i][j].LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
                goto HANDLE_ERROR;
        }
    }

    // Load all of Rudolf's frames
    for( i = 0; i < MAX_RUDOLF_DANCES; ++i )
    {
        for( j = 0; j < rudolfFrameCount[i]; ++j )
        {
            // Generate the resource name
            sprintf( ResName, "RudolfFrame%d_%d", i, j );

            // Load the polygon object from resource
            if( !LoadTheResource( Instance(), ResName, "POLY", &LoadRes ) )
                goto HANDLE_ERROR;

            // Load the polygon object 
            if( !rudolfPly[i][j].LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
                goto HANDLE_ERROR;
        }
    }

    // Load the polygon object from resource
    if( !LoadTheResource( Instance(), "housePly", "POLY", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the polygon object 
    if( !housePly.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the polygon object from resource
    if( !LoadTheResource( Instance(), "snowPly", "POLY", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the polygon object 
    if( !snowPly.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    //////////////////////////////////////////////////////
    //
    //            Load Texture Libraries
    //
    //////////////////////////////////////////////////////

    // Load the texture library from resource
    if( !LoadTheResource( Instance(), "SantaTxt", "BTXT", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the texture library 
    if( !santaTxt.LoadFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the texture library from resource
    if( !LoadTheResource( Instance(), "RudolfTxt", "BTXT", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the texture library 
    if( !rudolfTxt.LoadFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the texture library from resource
    if( !LoadTheResource( Instance(), "snowTxt", "BTXT", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the texture library 
    if( !snowTxt.LoadFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the texture library from resource
    if( !LoadTheResource( Instance(), "houseTxt", "BTXT", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the texture library 
    if( !houseTxt.LoadFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    //////////////////////////////////////////////////////
    //
    //   Load the Shaded Table into the polygon Objects
    //
    //////////////////////////////////////////////////////

    // Load the shaded table from resource
    if( SystemBitDepth == 8 )
    {
        if( !LoadTheResource( Instance(), "ShadedTable8bit", "STBL", &LoadRes ) )
            goto HANDLE_ERROR;

        // Load the 8 bit shaded table 
        for( i = 0; i < MAX_SANTA_DANCES; ++i )
        {
            for( j = 0; j < frameCount[i]; ++j )
            { 
                santaPly[i][j].SetShadeLookUpTable( (BYTE *)LoadRes.pData );
            }
        }

        for( i = 0; i < MAX_RUDOLF_DANCES; ++i )
        {
            for( j = 0; j < rudolfFrameCount[i]; ++j )
            { 
                rudolfPly[i][j].SetShadeLookUpTable( (BYTE *)LoadRes.pData );
            }
        }

        housePly.SetShadeLookUpTable( (BYTE *)LoadRes.pData );
        snowPly.SetShadeLookUpTable( (BYTE *)LoadRes.pData );
    }
    else
    {
        if( !LoadTheResource( Instance(), "ShadedTable16bit", "STBL", &LoadRes ) )
            goto HANDLE_ERROR;

        // Load the 16 bit shaded table 
        for( i = 0; i < MAX_SANTA_DANCES; ++i )
        {
            for( j = 0; j < frameCount[i]; ++j )
            { 
                santaPly[i][j].SetShadeLookUpTable( (WORD *)LoadRes.pData );
            }
        }

        for( i = 0; i < MAX_RUDOLF_DANCES; ++i )
        {
            for( j = 0; j < rudolfFrameCount[i]; ++j )
            { 
                rudolfPly[i][j].SetShadeLookUpTable( (WORD *)LoadRes.pData );
            }
        }

        housePly.SetShadeLookUpTable( (WORD *)LoadRes.pData );
        snowPly.SetShadeLookUpTable( (WORD *)LoadRes.pData );
    }

    
    // Create the video buffer depending on the screen color depth
    if( !buffer.CreateDIBSectionBuffer( RnderRect.right, -RnderRect.bottom, SystemBitDepth, santaTxt.TMap[ 0 ].Palette ) )
        goto HANDLE_ERROR;
    
    for( i = 0; i < MAX_SANTA_DANCES; ++i )
    {
        for( j = 0; j < frameCount[i]; ++j )
        { 
            // Set the ZBuffer
            santaPly[i][j].SetZBuffer( ZBuffer );
            // Set the size of the rendering area
            santaPly[i][j].SetRenderSize( RnderRect.right, RnderRect.bottom );
            // Set the maxium Z and Light distance
            santaPly[i][j].SetMaxZDistance( MAX_Z_DIS );
            santaPly[i][j].SetMaxLightDistance( MAX_LIGHT_DIS );
            // Set the camera scale
            santaPly[i][j].SetCameraScale( CameraSize );
            // Set the video buffer
            santaPly[i][j].SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );
            // Set the texture libraries
            santaPly[i][j].SetTextures( &santaTxt );
        }
    }

    for( i = 0; i < MAX_RUDOLF_DANCES; ++i )
    {
        for( j = 0; j < rudolfFrameCount[i]; ++j )
        { 
            // Set the ZBuffer
            rudolfPly[i][j].SetZBuffer( ZBuffer );
            // Set the size of the rendering area
            rudolfPly[i][j].SetRenderSize( RnderRect.right, RnderRect.bottom );
            // Set the maxium Z and Light distance
            rudolfPly[i][j].SetMaxZDistance( MAX_Z_DIS );
            rudolfPly[i][j].SetMaxLightDistance( MAX_LIGHT_DIS );
            // Set the camera scale
            rudolfPly[i][j].SetCameraScale( CameraSize );
            // Set the video buffer
            rudolfPly[i][j].SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );
            // Set the texture libraries
            rudolfPly[i][j].SetTextures( &rudolfTxt );
        }
    }

    // Set the ZBuffer
    housePly.SetZBuffer( ZBuffer );
    // Set the size of the rendering area
    housePly.SetRenderSize( RnderRect.right, RnderRect.bottom );
    // Set the maxium Z and Light distance
    housePly.SetMaxZDistance( MAX_Z_DIS );
    housePly.SetMaxLightDistance( MAX_LIGHT_DIS );
    // Set the camera scale
    housePly.SetCameraScale( CameraSize );
    // Set the video buffer
    housePly.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );
    // Set the texture libraries
    housePly.SetTextures( &houseTxt );

    // Set the ZBuffer
    snowPly.SetZBuffer( ZBuffer );
    // Set the size of the rendering area
    snowPly.SetRenderSize( RnderRect.right, RnderRect.bottom );
    // Set the maxium Z and Light distance
    snowPly.SetMaxZDistance( MAX_Z_DIS );
    snowPly.SetMaxLightDistance( MAX_LIGHT_DIS );
    // Set the camera scale
    snowPly.SetCameraScale( CameraSize );
    // Set the video buffer
    snowPly.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );
    // Set the texture libraries
    snowPly.SetTextures( &snowTxt );

    //////////////////////////////////////////////////////
    //
    //   Load Texture Coordinates
    //
    //   Loading the coordinates inits all the polygons
    //   in the polygon object file which is why it's the
    //   last item to load.
    //
    //////////////////////////////////////////////////////

    // Load the texture coordinates from resource
    if( !LoadTheResource( Instance(), "SantaCrd", "CRD", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the texture coordinates
    for( i = 0; i < MAX_SANTA_DANCES; ++i )
    {
        for( j = 0; j < frameCount[i]; ++j )
        { 
            santaPly[i][j].LoadCrdFromPointer( (BYTE *)LoadRes.pData );
            santaPly[i][j].SetLightSource( LIGHT_OFFSET_X, LIGHT_OFFSET_Y, LIGHT_OFFSET_Z );
        }
    }

    // Load the texture coordinates from resource
    if( !LoadTheResource( Instance(), "RudolfCrd", "CRD", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the texture coordinates
    for( i = 0; i < MAX_RUDOLF_DANCES; ++i )
    {
        for( j = 0; j < rudolfFrameCount[i]; ++j )
        { 
            rudolfPly[i][j].LoadCrdFromPointer( (BYTE *)LoadRes.pData );
            rudolfPly[i][j].SetLightSource( LIGHT_OFFSET_X, LIGHT_OFFSET_Y, LIGHT_OFFSET_Z );
        }
    }

    // Load the texture coordinates from resource
    if( !LoadTheResource( Instance(), "houseCrd", "CRD", &LoadRes ) )
        goto HANDLE_ERROR;

    housePly.LoadCrdFromPointer( (BYTE *)LoadRes.pData );
    housePly.SetLightSource( LIGHT_OFFSET_X, LIGHT_OFFSET_Y, LIGHT_OFFSET_Z );

    // Load the texture coordinates from resource
    if( !LoadTheResource( Instance(), "snowCrd", "CRD", &LoadRes ) )
        goto HANDLE_ERROR;

    snowPly.LoadCrdFromPointer( (BYTE *)LoadRes.pData );

    // Load the palette if we are in 8 bit or less mode
    // Init the screens palette
    if( SystemBitDepth == 8 )
    {
        // Saver the old palette to select back into the dc. If this doesn't happen, the newly created
        // palette will never get deleted
        hOldPalette = SelectPalette( GetScreenDC(), buffer.GetPalette(), FALSE );
        RealizePalette( GetScreenDC() );
    }

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
    BOOL Result = FALSE, resetScnRes = FALSE;
    int i, resID;

    #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )
    if( Mode() == ssSaver )
    {
        // If not size to fit, and greater then 16 bit color, do a reset
        if( !Settings.GetSizeToFit() && Settings.GetForce16BitColor() &&
            (GetDeviceCaps( GetScreenDC(), BITSPIXEL ) != 16) )
        {
            // Set the res id to the current screen size
            resID = GetScreenResSizeID( ScreenRect.right, ScreenRect.bottom );

            if( resID != RES_NO_RES_SELECTED )
            {
                Settings.SetRenderRes( resID );
                resetScnRes = TRUE;
            }
        }

        else if( Settings.GetSizeToFit() && Settings.GetRenderRes() != GetScreenResSizeID( ScreenRect.right, ScreenRect.bottom ) )
            resetScnRes = TRUE;

        // Reset the screen res and color depth
        if( resetScnRes )
            SetScreenResolution();

    }
    #endif

    // Setup the int list
    for( i = 0; i < MAX_CAMERA_VIEWS; ++i )
        viewList.Add( i );

    // Now mix up all the ints
    viewList.MixUp();

    // Setup the int list
    for( i = 0; i < MAX_SANTA_DANCES; ++i )
        danceList.Add( i );

    // Now mix up all the ints
    danceList.MixUp();

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
    char songName[20];
    int i;

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
            else if( Settings.GetThemeMusicOn() )
            {
                // Load the song names
                for( i = 0; i < MAX_NUMBER_OF_SONGS; ++i )
                {
                    sprintf( songName, "Song%d", i );

                    // Load the metafile
                    StringList.Add( songName );
                }

                // Mix it up
                StringList.MixUp();
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

        FillRect( GetScreenDC(), &DesktopRect, BlackBrush );   
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

void CAnimateSaver::AnimateSaver()
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

void CAnimateSaver::PlayMusic()
{
    // Don't try to play any music if there has been any errors
    if( !Settings.GetMusicOff() && !MPlayer.GetPlayError() && MusicTimer < timeGetTime() )
    {
        // Get the status of the windows media player
        int PlayerStatus = MPlayer.GetStatus();

        // Do we play the included star song?
        if( Settings.GetThemeMusicOn() )
        {
            // This will load the song and play it
            if( PlayerStatus == PLAYER_CLOSED || PlayerStatus == PLAYER_STOPPED )
            {
                if( SongCount == 0 )
                {
                    // If they choose to randomize the music, mix it up
                    if( Settings.GetRandomMusic() )
                        StringList.MixUp();
                }

                MPlayer.Play( StringList.GetString( SongCount ), "MIDI" );

                // Inc to the next song
                SongCount = (SongCount + 1) % StringList.Count();
            }
            // Player could have been paused by the monitor going asleep
            else if( PlayerStatus == PLAYER_PAUSED )
                MPlayer.Play();
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
        case PLAY_MUSIC:       PlayMusic();               break;

        // Erasing the buffer
        case RENDER_BUFFER:    RenderBuffer();            break;

        // Do the shareware check
        case SHAREWARE_NOTICE: ShowShareWareScreen();     break;

        // Blit to screen
        case BLIT_TO_SCREEN:   BlitFromBufferToScreen();  break;
    }

    // Inc the animation counter
    AnimCounter = ( AnimCounter + 1 ) % NUM_BLIT_OPERATIONS;

}   // Animate


/************************************************************************
*    FUNCTION NAME:         RenderBuffer                                                             
*
*    DESCRIPTION:           Render the selected scene to the buffer
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
    int i, offset[2] = { -1,1 }, snowX;
    Matrix3D snowMatrix, santaMatrix, houseMatrix, rudolfMatrix;

    // Zero out the ZBuffer
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Reset the camera if need be
    SetCamera();

    // init with the default offsets
    santaMatrix.Translate( -CHAR_X_OFFSET, santaOffsetY[danceIndex], 0 );
    houseMatrix.Translate( 0, HOUSE_Y_OFFSET, HOUSE_Z_OFFSET );
    rudolfMatrix.Translate( CHAR_X_OFFSET, 0, 0 );

    // Set to the camera offsets
    santaMatrix.Translate( camera.XPos, camera.YPos, camera.ZPos );
    santaMatrix.Rotate( camera.XRot, camera.YRot, camera.ZRot );
    
    houseMatrix.Translate( camera.XPos, camera.YPos, camera.ZPos );
    houseMatrix.Rotate( camera.XRot, camera.YRot, camera.ZRot );
    
    rudolfMatrix.Translate( camera.XPos, camera.YPos, camera.ZPos );
    rudolfMatrix.Rotate( camera.XRot, camera.YRot, camera.ZRot );

    // Make Rudolf's nose glow
    for( i = 0; i < MAX_NOSE_POLYS; ++i )
        rudolfPly[rudolfDanceIndex][rudolfFrameCounter].GetPoly( i + NOSE_GLOW_OFFSET )->SetIntensity( noseGlow[noseGlowCounter] );

    // inc the counter
    noseGlowCounter = (noseGlowCounter + 1) & (MAX_GLOW_COUNT-1);

    // Paint the buffer
    if( SystemBitDepth == 8 )
    {
        // Clear out the video buffer
        memsetQuad( buffer.GetPtrToDIBsectionBits(), 0, (RnderRect.right * RnderRect.bottom) >> 2 );

        // Paint Santa to the buffer
        santaPly[danceIndex][frameCounter].PaintVBuffer8To8( santaMatrix, POS_ZBUFFER );

        // Paint Rudolf to the buffer
        rudolfPly[rudolfDanceIndex][rudolfFrameCounter].PaintVBuffer8To8( rudolfMatrix, POS_ZBUFFER );
        
        // Paint the house to the buffer
        housePly.PaintVBuffer8To8( houseMatrix, POS_ZBUFFER, 0, polyAllow[cameraIndex] );
    }
    else
    {
        // Clear out the video buffer
        memsetQuad( buffer.GetPtrToDIBsectionBits(), 0, (RnderRect.right * RnderRect.bottom) >> 1 );

        // Paint Santa to the buffer
        santaPly[danceIndex][frameCounter].PaintVBuffer8To16( santaMatrix, POS_ZBUFFER );

        // Paint Rudolf to the buffer
        rudolfPly[rudolfDanceIndex][rudolfFrameCounter].PaintVBuffer8To16( rudolfMatrix, POS_ZBUFFER );

        // Paint the house to the buffer
        housePly.PaintVBuffer8To16( houseMatrix, POS_ZBUFFER, 0, polyAllow[cameraIndex] );
    }

    // The sign of the X rotation has to be changed depending
    // on the position of the Z for it to billboard properly.
    if( camera.ZPos > 0 )
        snowX = camera.XRot * -1;
    else
        snowX = camera.XRot;
        
    // Move and animate the snow
    for( i = 0; i < MAX_SNOW; ++i )
    {
        // Zero out the metrix
        snowMatrix.InitilizeMatrix();

        // Set the snow rotation & Translation
        snowMatrix.Rotate( 0, 0, snowOffset[i].ZRot );
        snowMatrix.Rotate( snowX, -camera.YRot, 0 );
        snowMatrix.Translate( snowOffset[i].XPos + camera.XPos, snowOffset[i].YPos + camera.YPos, snowOffset[i].ZPos + camera.ZPos );
        snowMatrix.Rotate( camera.XRot, camera.YRot, camera.ZRot );

        // Set the texture index
        snowPly.SetTextHandle( 0, snowOffset[i].artIndex );

        // Render the snow
        if( SystemBitDepth == 8 )
            snowPly.PaintVBuffer8To8( snowMatrix, POS_ZBUFFER, 0, 0, FALSE );
        else
            snowPly.PaintVBuffer8To16( snowMatrix, POS_ZBUFFER, 0, 0, FALSE, PAINT_OR );
        
        // Float the snow down
        snowOffset[i].YPos -= SNOW_FLOAT_SPEED;
        
        // Inc the rotation
        if( i & 1 )
            snowOffset[i].ZRot -= SNOW_ROTATION_SPEED;
        else
            snowOffset[i].ZRot += SNOW_ROTATION_SPEED;

        // Reint the snow to start all over again
        if( snowOffset[i].YPos < Y_BOT_OFFSET )
        {
            snowOffset[i].XPos = (rand() % MAX_SNOW_X) * offset[rand() % 2];
            snowOffset[i].YPos = Y_TOP_OFFSET;
            snowOffset[i].ZPos = (rand() % MAX_SNOW_Z) * offset[rand() % 2];

            // Push the snow out of the way so that it doesn't fall through Santa and Rudolf
            if( snowOffset[i].XPos > -SNOW_CHAR_OFFSET_X && snowOffset[i].XPos < SNOW_CHAR_OFFSET_X &&
                snowOffset[i].ZPos > -SNOW_CHAR_OFFSET_Z && snowOffset[i].ZPos < SNOW_CHAR_OFFSET_Z )
            {
                // Randomize a new postion out of range
                if( rand() % 2 )
                    snowOffset[i].XPos = ((rand() % (MAX_SNOW_X - SNOW_CHAR_OFFSET_X)) + SNOW_CHAR_OFFSET_X) * offset[rand() % 2];
                else
                    snowOffset[i].ZPos = ((rand() % (MAX_SNOW_Z - SNOW_CHAR_OFFSET_Z)) + SNOW_CHAR_OFFSET_Z) * offset[rand() % 2];
            }
        }
    }

    // Inc the santa animation
    if( animType[ danceIndex ] == PING_POMG )
    {
        frameCounter += pingPongDir;

        if( frameCounter == frameCount[danceIndex]-1 )
            pingPongDir = -1;
        else if( frameCounter == 0 )
            pingPongDir = 1;
    }
    else
    {
        frameCounter = ( frameCounter + 1 ) % frameCount[danceIndex];
    }

    // Inc the rudolf animation
    if( rudolfAnimType[ rudolfDanceIndex ] == PING_POMG )
    {
        rudolfFrameCounter += rudolfPingPongDir;

        if( rudolfFrameCounter == rudolfFrameCount[rudolfDanceIndex]-1 )
            rudolfPingPongDir = -1;
        else if( rudolfFrameCounter == 0 )
            rudolfPingPongDir = 1;
    }
    else
    {
        rudolfFrameCounter = ( rudolfFrameCounter + 1 ) % rudolfFrameCount[rudolfDanceIndex];
    }


}   // RenderBuffer


/************************************************************************
*    FUNCTION NAME:         SetCamera                                                             
*
*    DESCRIPTION:           Set the camera
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

void CAnimateSaver::SetCamera()
{

    // Dec the counter
    --cameraFrameCounter;

    // When the counter runs out, change the view
    if( cameraFrameCounter <= 0 )
    {
        // Inc the counter
        cameraCounter = (cameraCounter + 1) % MAX_CAMERA_VIEWS;
        
        // Now mix up all the ints
        if( cameraCounter == 0 )
            viewList.MixUp();

        // init to the next camera index
        cameraIndex = viewList.GetInt( cameraCounter );

        // change the dance
        if( cameraCounter % 2 )
        {
            // Inc the counter
            danceCounter = (danceCounter + 1) % MAX_SANTA_DANCES;
            rudolfDanceIndex = (rudolfDanceIndex + 1) % MAX_RUDOLF_DANCES;
            
            // Now mix up all the ints
            if( danceCounter == 0 )
                danceList.MixUp();

            // init to the next camera index
            danceIndex = danceList.GetInt( danceCounter );
            
            // Reset the frame counters
            frameCounter = 0;
            rudolfFrameCounter = 0;

            // Reset the ping pong direction
            pingPongDir = 1;
            rudolfPingPongDir = 1;
        }
    }

    switch( cameraIndex )
    {
        // Straight ahead slow zoom in
        case 0:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 200;

                camera.XPos = 0; camera.YPos = -218; camera.ZPos = -2000;
                camera.XRot = -16; camera.YRot = 513; camera.ZRot = 0;
            }

            if( cameraFrameCounter > 120 )
                camera.ZPos += 10;
            else if( cameraFrameCounter > 90 )
                camera.ZPos += 7;
            else if( cameraFrameCounter > 50 )
                camera.ZPos += 4;
            else if( cameraFrameCounter > 30 )
                camera.ZPos += 2;
        break;

        // Side pan and rotate
        case 1:
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 300;

                camera.XPos = -1663; camera.YPos = -69; camera.ZPos = -690;
                camera.XRot = -8; camera.YRot = 373; camera.ZRot = 0;
            }

            camera.XPos += 10;

            if( cameraFrameCounter > 199 && cameraFrameCounter < 206 )
                camera.YRot += 2;
            else if( cameraFrameCounter > 70 && cameraFrameCounter < 200 )
                camera.YRot += 2;
            else if( cameraFrameCounter > 64 && cameraFrameCounter < 71 )
                camera.YRot += 1;

        break;

        // Pan view next to the chimny
        case 2:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 150;

                camera.XPos = 1700; camera.YPos = 100; camera.ZPos = 780;
                camera.XRot = 3; camera.YRot = -124; camera.ZRot = 0;
            }

            camera.XPos -= 5;
        break;

        // Behind slow pan
        case 3:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = -276; camera.YPos = -14; camera.ZPos = 520;
                camera.XRot = 9; camera.YRot = 96; camera.ZRot = 0;
            }

            camera.XPos -= 2;
        break;

        // Overhead look down
        case 4:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = -356; camera.YPos = -844; camera.ZPos = 360;
                camera.XRot = -185; camera.YRot = 58; camera.ZRot = 0;
            }

            camera.XPos += 5;
        break;

        // Hehind pull back
        case 5:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = -146; camera.YPos = -164; camera.ZPos = 480;
                camera.XRot = -27; camera.YRot = 45; camera.ZRot = 0;
            }

            camera.ZPos += 5;
        break;

        // Side pan rotate
        case 6:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 120;

                camera.XPos = 784; camera.YPos = -104; camera.ZPos = 420;
                camera.XRot = -5; camera.YRot = -154; camera.ZRot = 0;
            }

            camera.ZPos -= 10;
            camera.YRot -= 2;
            camera.YPos -= 5;
            camera.XRot -= 1;
        break;

        // Front view pull back
        case 7:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = 320; camera.YPos = -70; camera.ZPos = -630;
                camera.XRot = 8; camera.YRot = -444; camera.ZRot = 0;
            }

            camera.ZPos -= 3;

        break;

        // Front view pull back
        case 8:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = 100; camera.YPos = -120; camera.ZPos = -650;
                camera.XRot = -2; camera.YRot = -491; camera.ZRot = 0;
            }

            camera.ZPos -= 3;

        break;

        // Front view pull back
        case 9:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = -169; camera.YPos = -92; camera.ZPos = -650;
                camera.XRot = 4; camera.YRot = -561; camera.ZRot = 0;
            }

            camera.ZPos -= 3;

        break;

        // Front view pull back
        case 10:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = -360; camera.YPos = -79; camera.ZPos = -570;
                camera.XRot = 3; camera.YRot = -608; camera.ZRot = 0;
            }

            camera.ZPos -= 3;

        break;

        // Front below view pan to side
        case 11:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = 100; camera.YPos = 166; camera.ZPos = -870;
                camera.XRot = 77; camera.YRot = -529; camera.ZRot = 0;
            }

            camera.XPos -= 3;

        break;

        // Front below view pan to side
        case 12:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 90;

                camera.XPos = -138; camera.YPos = -454; camera.ZPos = -700;
                camera.XRot = -90; camera.YRot = -562; camera.ZRot = 0;
            }

            camera.YPos -= 3;

        break;

        // Side and up pan
        case 13:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = -628; camera.YPos = -404; camera.ZPos = -140;
                camera.XRot = -79; camera.YRot = -747; camera.ZRot = 0;
            }

            camera.XPos -= 3;
            camera.YPos -= 3;

        break;

        // Side pan in
        case 14:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = -2058; camera.YPos = -424; camera.ZPos = -120;
                camera.XRot = -51; camera.YRot = -755; camera.ZRot = 0;
            }

            camera.XPos += 15;

        break;

        // Side pan
        case 15:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = -414; camera.YPos = -134; camera.ZPos = 1620;
                camera.XRot = -1; camera.YRot = 86; camera.ZRot = 0;
            }

            camera.XPos -= 5;

        break;

        // Side pan
        case 16:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = -914; camera.YPos = -44; camera.ZPos = 70;
                camera.XRot = 10; camera.YRot = 176; camera.ZRot = 0;
            }

            camera.XPos += 7;
            camera.ZPos -= 10;
            camera.YRot += 3;

        break;

        // front zoom
        case 17:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = -594; camera.YPos = -524; camera.ZPos = -1180;
                camera.XRot = -75; camera.YRot = 426; camera.ZRot = 0;
            }

            camera.ZPos += 5;
            camera.XPos += 2;

        break;

        // front pand down
        case 18:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = -300; camera.YPos = -784; camera.ZPos = -880;
                camera.XRot = -128; camera.YRot = 435; camera.ZRot = 0;
            }

            camera.YPos += 5;
            camera.XRot += 1;

        break;

        // front pand down
        case 19:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = 650; camera.YPos = -784; camera.ZPos = -930;
                camera.XRot = -96; camera.YRot = 575; camera.ZRot = 0;
            }

            camera.YPos += 7;
            camera.XRot += 1;
            camera.XPos -= 2;

        break;

        // front pand down
        case 20:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = 376; camera.YPos = -844; camera.ZPos = -730;
                camera.XRot = -141; camera.YRot = 539; camera.ZRot = 0;
            }

            camera.XPos -= 5;

        break;

        // front pand down
        case 21:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = 1906; camera.YPos = 200; camera.ZPos = -770;
                camera.XRot = 6; camera.YRot = -344; camera.ZRot = 0;
            }

            camera.XPos -= 5;

        break;

        // front pand down
        case 22:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = -574; camera.YPos = 126; camera.ZPos = -1230;
                camera.XRot = -12; camera.YRot = -598; camera.ZRot = 0;
            }

            camera.YPos -= 5;
            camera.ZPos += 1;

        break;

        // front pand down
        case 23:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = 576; camera.YPos = -454; camera.ZPos = -1430;
                camera.XRot = -12; camera.YRot = -517; camera.ZRot = 0;
            }

            camera.XPos -= 10;

        break;

        // front pan up
        case 24:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 130;

                camera.XPos = 0; camera.YPos = -334; camera.ZPos = -650;
                camera.XRot = -81; camera.YRot = -513; camera.ZRot = 0;
            }

            camera.YPos -= 10;
            camera.XRot -= 1;
            camera.ZPos += 1;
            camera.YRot -= 1;

        break;

        // front pan up
        case 25:
            // Reset the camera
            if( cameraFrameCounter <= 0 )
            {
                cameraFrameCounter = 100;

                camera.XPos = 4; camera.YPos = -1297; camera.ZPos = -400;
                camera.XRot = -208; camera.YRot = -577; camera.ZRot = 0;
            }

            camera.YRot += 1;

        break;
    }

}   // SetCamera


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

        if( Settings.GetSizeToFit() && !ResLessThen640x480 )
            buffer.Draw( GetScreenDC() );
        else
            buffer.Draw( GetScreenDC(), RnderRect.left, RnderRect.top );

        #else
        // Now blit the buffer to the screen only if not in shareware mode
        buffer.Draw( GetScreenDC(), &RnderRect );
        #endif
    }
    else
    {
        buffer.Draw( GetScreenDC() );
    }

    // Show an error text if there is a problem playing music
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
    if( ZBuffer )
        delete [] ZBuffer;

    if( hOldPalette )
        SelectPalette( GetScreenDC(), hOldPalette, FALSE );
            
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
    int SelectedRes;
    BOOL ScreenResized = FALSE;

    // Set the size of the device mode structure
    dm.dmSize = sizeof( DEVMODE );

    // Check to see is the screen res that is set is less then 640x480
    if( Settings.GetRenderRes() < RES_640x480 )
    {
        SelectedRes = RES_640x480;
        ResLessThen640x480 = TRUE;
    }
    else
        SelectedRes = Settings.GetRenderRes();

    // search for the mode we are looking for
    while( EnumDisplaySettings( NULL, mode, &dm ) )
    {
        if( SystemBitDepth == dm.dmBitsPerPel && GetScreenResSizeID( dm.dmPelsWidth, dm.dmPelsHeight ) == SelectedRes )
        {
            // We found our device mode so change it
            if( ChangeDisplaySettings( &dm, 0 ) == DISP_CHANGE_SUCCESSFUL )
            {
                // Sit and sleep for a second and give the
                // system some time to change over.
                Sleep( 1700 );

                // Set the flag to true 
                ScreenResized = TRUE;

                // Set the flag that indicates the screen was resized so that the
                // screen saver will set the screen res back when it is done.
                SetScreenResize( TRUE );

                // Reset the screen width and height
                // This is mainly for properly drawing the shareware screens
                ScreenWidth = dm.dmPelsWidth;
                ScreenHeight = dm.dmPelsHeight;

                // Reset the render rect
                SetRect( &RnderRect, abs(ScreenWidth - RnderRect.right) / 2, 
                                     abs(ScreenHeight - RnderRect.bottom) / 2,
                                     RnderRect.right, RnderRect.bottom );
            }
            else
            {
                // Something went wrong, change it all back
                ChangeDisplaySettings( NULL, 0 );

                // Set flag to false because we can't do it
                Settings.SetSizeToFit( FALSE );
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