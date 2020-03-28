
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
#include "planetSet.h"       // Planet settings INI file.


// Externed from SSConfig.cpp
extern int ResCheck[MAX_RES_CHECK];
extern int BlitToScreen[MAX_RES_CHECK];

// earth's Camera Z
float earthOffsetZ[MAX_Z_OFFSETS] = { 2000, 1600, 1400, 1200 };
float ringZOffset[MAX_Z_OFFSETS] = { -35, -50, -60, -70 };

LPSTR polyLoadStr[] = { "earthPly32_32","earthPly16_16" };
int polySlices[] = {64,128};   // number of segments width wise
int polyStacks[] = {32,64};    // number of segments height wise
                                                                                                                        

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
    ResLessThen640x480 = FALSE;
    rotationCounter = 0;
    ringCounterPingPong = -10.0f;
    ringPingPong = FALSE;
    earthAxis = (REDEFINE_DEGREECOUNT / 360) * EARTHS_AXIS_IN_DEGREES;
    cameraTimer = 0;
    Zindex = 0;
    earthOffsetY = 0;
    earthCameraX = 0;
    pStar = NULL;
    StarCount = 0;
    twinkleStar = 0;
    rotVector = 1;
    zoomMax = MAX_Z_OFFSETS;
	pShadedTbl = NULL;

    // Init non-specific class members
    ResLessThen640x480 = FALSE;
    AnimCounter = 0;
    SaverTimer = 0;
    SaverSpeed = 0;
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

    // Set up the blit rects
    #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )

    SetRect( &RnderRect, abs(ScreenWidth - RenderWidth) / 2, 
                         abs(ScreenHeight - RenderHeight) / 2,
                         RenderWidth, RenderHeight );
    #else
    CameraSize = 355;
    RnderRect = WindowRect;
    #endif

    if( Mode() == ssPreview )
    {
        RnderRect = WindowRect;
        CameraSize = 165;
    }

    // Reset the bit depth because we only want to
    // allow 8 or 16 bit screen depths
    if( SystemBitDepth <= 8 && !(Settings.GetForce16BitColor() && Mode() == ssSaver) )
        SystemBitDepth = 8;
    else
        SystemBitDepth = 16;

    // Get the path to a custom texture
    pFileTextureLoad = Settings.GetCustomTextureFileName();
    
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
    TLoadRes LoadRes;
    TDIB tmpLoad;
    int i, j, index, ply = HI_POLY_RES, textW, textH;
    RECT dest, source;
    WORD tempColor = 0;
    PSWordToByte RGB16 = PSWordToByte( &tempColor );
    WORD *tmpBackground16;

	// Do we need a shaded table?
	if( Settings.GetShadeType() > NO_SHADING )
	{
		// Allocate the shaded lookup table
	    pShadedTbl = new unsigned short int[ SHADE_COUNT * COLOR_COUNT_16_BIT ];
	    if( pShadedTbl == NULL )
	    {
	        strcpy( errorStr, "Can't allocate shaded lookup table. Your system resources may be low so try restarting your computer." );
	        goto HANDLE_ERROR;
	    }

		// Generate the 16 bit shaded table
    	Generate16BitShadedTable( pShadedTbl );
	}

    // Init the classes using the screens DC. That way it doesn't have to make one
    buffer.InitActiveDC( GetScreenDC() );

    // One time initilization of the cos and sin tables
    // Makes no difference who calls it because the tables are static
    earthMatrix.InitCosSinTables();

    // init the sun matrix
    sunMatrix.Rotate( 0, 0, -(earthAxis * 2) );

    // use the low res polygon for the preview or depending on the shading
    if( Mode() == ssPreview || Settings.GetPolyRes() == LOW_POLY_RES )
        ply = LOW_POLY_RES;

    //////////////////////////////////////////////////////
    //
    //            Load Polygon Objects
    //
    //////////////////////////////////////////////////////

    // Load the earth polygon object from resource
    if( !LoadTheResource( Instance(), polyLoadStr[ply], "POLY", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the earth polygon object 
    if( !earthPly.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the ring polygon object from resource
    if( !LoadTheResource( Instance(), "ringPly", "POLY", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the ring polygon object 
    if( !ringPly.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;


    //////////////////////////////////////////////////////
    //
    //            Load Texture Library
    //
    //  This screen saver uses more then one texture for the same 
    //  polygon earth object so we have to load the bitmap used for  
    //  the texture and build the texture library on startup.
    //
    //////////////////////////////////////////////////////

    // load your own texture
    if( !pFileTextureLoad[0] || !LoadTextureFromFile( tmpLoad ) )
    {
        // load the texture from resource.
        if( !tmpLoad.LoadFromResource( Instance(), "earthBMP", "BMP" ) )
            goto HANDLE_ERROR;
    }
    
    // Allocate the texture library
    if( !earthTxt.AllocateTextLibrary( earthPly.GetTotalPolyCount() ) )
        goto HANDLE_ERROR;

    // calculate the width and height of the texture segments
    textW = tmpLoad.width() / polySlices[ply];
    textH = tmpLoad.height() / polyStacks[ply];
    
    // allocate all the individual textures
    for( i = 0; i < earthPly.GetTotalPolyCount(); ++i )
    {
        if( !earthTxt.TMap[i].AllocateTextureSpace( textW, textH, BIT_COUNT_16, i ) )
            goto HANDLE_ERROR;

        // Take this chance rotate all the polygons twice
        earthPly.RotateTexture( i );
        earthPly.RotateTexture( i );
    }

    // set the destination rect
    SetRect( &dest, 0, 0, textW, textH );

    // Blit the texture into all the little texture pieces
    for( i = 0; i < polyStacks[ply]; ++i )
    {
        for( j = 0; j < polySlices[ply]; ++j )
        {
            // init the source rect
            SetRect( &source, j*textW, i*textH, textW, textH );

            // calculate the index into the texture library
            index = ((polySlices[ply] * polyStacks[ply])-1) - ((i * polySlices[ply]) + j);

            // copy a section of the bitmap to the texture library
            tmpLoad.Blit( earthTxt.TMap[index].Image, dest, source, textW, textH, TOP_DOWN_BMP );

            // Rotate the other half of the polygons to finish the rotation
            if( j >= (polySlices[ply] >> 2) && j < ((polySlices[ply] >> 2) * 3) )
                earthPly.RotateTexture( (i * polySlices[ply]) + j );
        }
    }

    // Check that we haven't already loaded a texture ring
    if( ringTxt.Count() == 0 )
    {
        // Load the ring texture library from resource
        if( !LoadTheResource( Instance(), "ringBt", "BTXT", &LoadRes ) )
            goto HANDLE_ERROR;

        // Load the ring texture library 
        if( !ringTxt.LoadFromPointer( (BYTE *)LoadRes.pData ) )
            goto HANDLE_ERROR;
    }


    //////////////////////////////////////////////////////
    //
    //   Load the Shaded Table into the polygon Objects
    //
    //////////////////////////////////////////////////////

    // Load the 16 bit shaded table for 16 bit images
    earthPly.SetShadeLookUpTable( pShadedTbl );

    // Create the video buffer depending on the screen color depth
    // All texture libraries use the same 8 bit palette
    if( !buffer.CreateDIBSectionBuffer( RnderRect.right, -RnderRect.bottom, BIT_COUNT_16, earthTxt.TMap[ 0 ].Palette ) )
    {
        strcpy( errorStr, "Can't allocate background buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Set the ZBuffer
    //earthPly.SetZBuffer( ZBuffer );

    // Set the size of the rendering area
    earthPly.SetRenderSize( RnderRect.right, RnderRect.bottom );
    ringPly.SetRenderSize( RnderRect.right, RnderRect.bottom );

    // Set the maxium Z and Light distance
    earthPly.SetMaxZDistance( 400000 );
    earthPly.SetMaxLightDistance( 900000 );
    ringPly.SetMaxZDistance( 400000 );
    ringPly.SetMaxLightDistance( 900000 );

    // Set the camera scale
    earthPly.SetCameraScale( CameraSize );
    ringPly.SetCameraScale( CameraSize );

    // Set the video buffer
    earthPly.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );
    ringPly.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );

    // Set the texture libraries
    earthPly.SetTextures( &earthTxt );
    ringPly.SetTextures( &ringTxt );

    //////////////////////////////////////////////////////
    //
    //   Load Texture Coordinates
    //
    //   Loading the coordinates inits all the polygons
    //   in the polygon object file which is why it's the
    //   last item to load.
    //
    //////////////////////////////////////////////////////

    // Init with the default texture because we don't have a
    // coordinate file
    earthPly.InitDefText();

    // Set the textures. 1 to 1 ratio
    for( i = 0; i < earthPly.GetTotalPolyCount(); ++i )
        earthPly.SetTextHandle( i, i );

    // Load the texture coordinates from resource
    if( !LoadTheResource( Instance(), "ringCrd", "CRD", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the texture coordinates 
    ringPly.LoadCrdFromPointer( (BYTE *)LoadRes.pData );

    // clearout the video buffer
    //memset( buffer.GetPtrToDIBsectionBits(), 0, (BIT_COUNT_16 / 8) * (RnderRect.right * RnderRect.bottom) );
    memsetQuad( buffer.GetPtrToDIBsectionBits(), 0, (RnderRect.right * RnderRect.bottom) >> 1 );

    // If this is startup shading, shade the polygons now
    if( Settings.GetShadeType() == START_UP_SHADING )
    {
        // Randomly rotate the sun
        sunMatrix.Rotate( 0, rand() % REDEFINE_DEGREECOUNT, 0 );

        // init the rotation of the light source
        earthPly.SetLightSource( sunMatrix.GetX(), sunMatrix.GetY(), sunMatrix.GetZ() );
    }

    // Calculate the number of needed stars
    StarCount = (RnderRect.right * RnderRect.bottom) / 1024;

    // Allocate the stars
    pStar = new SStar[ StarCount ];

    // We don't need to stop the screen saver if the star allocation failed
    if( !pStar )
        StarCount = 0;

    // Randomize the star positions
    for( i = 0; i < StarCount; ++i )
    {
        pStar[i].X = (rand() % (RnderRect.right-2)) + 1;
        pStar[i].Y = (rand() % (RnderRect.bottom-2)) + 1;

        // Randonize a star color value
        RGB16->R = 15 + rand() % 13;
        RGB16->G = RGB16->R;
        RGB16->B = RGB16->R;

        // Set the pixel color for the star
        pStar[i].Color = tempColor;
    }

    // Make sure the system is in 16 bit color or better before we render
    if( SystemBitDepth > 8 ) 
    {
        // Type cast the buffer to a word
        tmpBackground16 = (WORD *)buffer.GetPtrToDIBsectionBits();

        // Render the stars to the buffer
        for( i = 0; i < StarCount; ++i )
        {   
            // Set the pixel color for the star
            tmpBackground16[ (pStar[i].Y * RnderRect.right) + pStar[i].X ] = pStar[i].Color;
        }
    }

    Result = TRUE;

    HANDLE_ERROR:;

    return Result;

}   // LoadGraphics


/************************************************************************
*    FUNCTION NAME:         LoadTextureFromFile                                                             
*
*    DESCRIPTION:           Load texture from file
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

BOOL CAnimateSaver::LoadTextureFromFile( TDIB &dib )
{
    BOOL result = FALSE;
    TDIB tmpRing;
    RECT rect;
    CPlanetSet planetSettings;

    if( !FileExist( pFileTextureLoad ) )
        goto HANDLE_ERROR;

    if( Mode() == ssPreview )
    {
        // See if we can load the low res texture file for the preview
        RelpaceFileExtension( NULL, pFileTextureLoad, ".lbt" );

        // If the low res is mssing, load the high res
        if( !FileExist( pFileTextureLoad ) || !dib.LoadFromFile( pFileTextureLoad, SUPRESS_ERROR_MSG ) )
        {
            // replace with hi res file extension
            RelpaceFileExtension( NULL, pFileTextureLoad, ".pbt" );

            // load the texture from resource. If there's an error, jump out
            if( !dib.LoadFromFile( pFileTextureLoad ) )
                goto HANDLE_ERROR;
        }
    }
    else
    {
        // load the texture from file. If there's an error, jump out
        if( !dib.LoadFromFile( pFileTextureLoad, SUPRESS_ERROR_MSG ) )
            goto HANDLE_ERROR;
    }

    // Check the width of the loaded file
    if( (dib.width() % 32) != 0 || !(dib.width() >= 256) )
        goto HANDLE_ERROR;

    // Check the height of the loaded file
    if( (dib.height() % 32) != 0 || !(dib.height() >= 128) )
        goto HANDLE_ERROR;

    // Make sure this is a 16 bit file
    if( dib.GetBitCount() != BIT_COUNT_16 )
        goto HANDLE_ERROR;

    // See if we can load the ring texture file
    RelpaceFileExtension( NULL, pFileTextureLoad, ".rbt" );

    // Set the rect
    SetRect( &rect, 0, 0, RING_TEXT_W, RING_TEXT_H );

    // Try to load the ring texture from file
    if( FileExist( pFileTextureLoad ) )
    {
        if( tmpRing.LoadFromFile( pFileTextureLoad, SUPRESS_ERROR_MSG ) )
        {
            if( tmpRing.width() >= RING_TEXT_W && tmpRing.height() >= RING_TEXT_H && tmpRing.GetBitCount() == BIT_COUNT_16 )
            {
                if( ringTxt.AllocateTextLibrary( 1 ) && ringTxt.TMap[0].AllocateTextureSpace( RING_TEXT_W, RING_TEXT_H, BIT_COUNT_16, 1 ) )
                    // copy a section of the bitmap to the texture library
                    tmpRing.Blit( ringTxt.TMap[0].Image, rect, rect, RING_TEXT_W, RING_TEXT_H, TOP_DOWN_BMP );
            }
        }
    }

    // See if we can load an INI file
    RelpaceFileExtension( NULL, pFileTextureLoad, ".ini" );

    // Try to load the INI file
    if( FileExist( pFileTextureLoad ) )
    {
        // set the ini file to get the settings from
        planetSettings.SetIniFilePath( pFileTextureLoad );

        // Read in the settings
        planetSettings.GetPrivateString();

        // Reset the class variables
        rotVector = planetSettings.GetRotationVector();
        earthAxis = (REDEFINE_DEGREECOUNT / 360) * planetSettings.GetAxisAngle();
        zoomMax = planetSettings.GetZoomMax();
    }

    result = TRUE;

HANDLE_ERROR:;

    return result;

}   // LoadTextureFromFile


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

    #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )
    if( Mode() == ssSaver )
    {
        // Do we want to resize the screen or force 16 bit color
        if( (Settings.GetSizeToFit() && (Settings.GetRenderRes() != GetScreenResSizeID( ScreenRect.right, ScreenRect.bottom ))) ||
            Settings.GetForce16BitColor() && (GetDeviceCaps( GetScreenDC(), BITSPIXEL ) != 16) )
        {
            // Reset the screen res and color depth
            SetScreenResolution();
        }
    }
    #endif

    // Set the timer to the next change
    //cameraTimer = timeGetTime() + CAMERA_SWITCH_TIMEOUT;

    // Randomize the starting rotation
    rotationCounter = rand() % REDEFINE_DEGREECOUNT;

    // Display a message if the computer is in 8-bit or less color
    if( SystemBitDepth <= 8 )
        DisplayShareWareScreen( buffer.Handle(), buffer.width(), buffer.height(), NEED_HIGHER_COLOR_MODE, HOW_TO_CHANGE_COLOR_SET, FALSE );

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
            // This will play the song that has already been loaded
            if( PlayerStatus == PLAYER_STOPPED || PlayerStatus == PLAYER_PAUSED )
                MPlayer.Play();

            // This will load the song and play it for the first time
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
        case PLAY_MUSIC:       if( SystemBitDepth > 8 ) PlayMusic();   break;

        // Erasing the buffer
        case RENDER_BUFFER:    if( SystemBitDepth > 8 ) RenderBuffer(); break;

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
    int degree, newIndex, i, offset;
    WORD *tmpBackground16, StarColor;

    // Time to change the camers angle
    if( zoomMax != 0 && cameraTimer < timeGetTime() )
    {
        // Randomly select a degree
        // Randomly choose the lower or higher hemisphere
        //degree = (rand() % 120) - 90;
        degree = (rand() % 51) - 25;

        // Calculate the camera angle to the degree
        earthCameraX = (REDEFINE_DEGREECOUNT / 360) * degree;
        
        // Make sure we pick a new z offset
        do
        {
            newIndex = rand() % MAX_Z_OFFSETS;
        }
        while( newIndex == Zindex );

        // Check that we are within requested range
        // if the zoomMax has been set by outside init file
        if( newIndex > zoomMax )
            newIndex = zoomMax;

        // Record the new Z ofset
        Zindex = newIndex;

        // If the new Z offset has the camera closer, push the Y up or down
        // The closer we are, the more we can push the camera up or down
        if( Zindex == 1 )
            earthOffsetY = (rand() % 341) - 170;
        else if( Zindex == 2 )
            earthOffsetY = (rand() % 441) - 220;
        else if( Zindex == 3 )
            earthOffsetY = (rand() % 581) - 290;
        else
            earthOffsetY = 0;

        // clear out the video buffer
        //memset( buffer.GetPtrToDIBsectionBits(), 0, (BIT_COUNT_16 / 8) * (RnderRect.right * RnderRect.bottom) );
        memsetQuad( buffer.GetPtrToDIBsectionBits(), 0, (RnderRect.right * RnderRect.bottom) >> 1 );

        // reset the glow ring
        ringCounterPingPong = -10.0f;
        
        // Reset the timer
        cameraTimer = timeGetTime() + CAMERA_SWITCH_TIMEOUT;
        
        // Indicate a camera change
        if( Zindex < (MAX_Z_OFFSETS-1) && Mode() == ssSaver )
        {
            // Type cast the buffer to a word
            tmpBackground16 = (WORD *)buffer.GetPtrToDIBsectionBits();

            // Render the stars to the buffer
            for( i = 0; i < StarCount; ++i )
            {   
                // Set the pixel color for the star
                tmpBackground16[ (pStar[i].Y * RnderRect.right) + pStar[i].X ] = pStar[i].Color;
            }
        }            
    }

    // Only render the starts if we are not too close to the earth
    // Twinkle the stars every 4 cycles
    if( Zindex < (MAX_Z_OFFSETS-1) && rotationCounter % 8 == 0 && Mode() == ssSaver )
    {
        // Type cast the buffer to a word
        tmpBackground16 = (WORD *)buffer.GetPtrToDIBsectionBits();

        // Calculate the offset
        offset = (pStar[twinkleStar].Y * RnderRect.right) + pStar[twinkleStar].X;

        // Set the pixel color for the star to simulate a twinkle
        tmpBackground16[ offset-1 ] = 0;
        tmpBackground16[ offset+1 ] = 0;
        tmpBackground16[ offset-RnderRect.right ] = 0;
        tmpBackground16[ offset+RnderRect.right ] = 0;

        // Randomly pick a star to twinkel          
        twinkleStar = rand() % StarCount;
        
        // Twinkle the stars

        // Calculate the offset
        offset = (pStar[twinkleStar].Y * RnderRect.right) + pStar[twinkleStar].X;

        // Get the color of the star
        StarColor = tmpBackground16[ offset ];

        // Set the pixel color for the star to simulate a twinkle
        tmpBackground16[ offset-1 ] = StarColor;
        tmpBackground16[ offset+1 ] = StarColor;
        tmpBackground16[ offset-RnderRect.right ] = StarColor;
        tmpBackground16[ offset+RnderRect.right ] = StarColor;
    }

    // Setup the earth's rotation
    earthMatrix.InitilizeMatrix();
    earthMatrix.Rotate( 0, -rotationCounter, earthAxis );

    earthMatrix.Rotate( earthCameraX, 0, 0 );
    earthMatrix.Translate( 0, earthOffsetY, earthOffsetZ[ Zindex ] );

    // Inc the rotation counter
    rotationCounter += rotVector;

    // Render to the video buffer
    if( ringCounterPingPong >= -10.0f )
        ringPingPong = TRUE;
    else if( ringCounterPingPong <= -40.0f )
        ringPingPong = FALSE;

    if( ringPingPong )
        ringCounterPingPong -= 0.5f;

    else
        ringCounterPingPong += 0.5f;

    ringMatrix.InitilizeMatrix();
    ringMatrix.Translate( 0, earthOffsetY, ringCounterPingPong + earthOffsetZ[ Zindex ] + ringZOffset[ Zindex ] );

    // Render the ring
    ringPly.PaintVBuffer16To16( ringMatrix, NO_ZBUFFERING, 0, 0, FALSE, NO_SHADED_TABLE );

    if( Settings.GetShadeType() == NO_SHADING )
    {
        earthPly.PaintVBuffer16To16( earthMatrix, NO_ZBUFFERING, 0, 0, FALSE, NO_SHADED_TABLE );
    }
    else if( Settings.GetShadeType() == START_UP_SHADING )
    {
        earthPly.PaintVBuffer16To16( earthMatrix, NO_ZBUFFERING );
    }
    else
    {
        // Rotate the sun
        sunMatrix.Rotate( 0, 2, 0 );

        // init the rotation of the light source
        earthPly.SetLightSource( sunMatrix.GetX(), sunMatrix.GetY(), sunMatrix.GetZ(),  TRUE );

        earthPly.PaintVBuffer16To16( earthMatrix, NO_ZBUFFERING );
    }

}   // RenderBuffer


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
    if( pStar != NULL )
        delete [] pStar;

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


/************************************************************************
*    FUNCTION NAME:         FileExist                                                             
*
*    DESCRIPTION:           A simplified use of SearchPath function
*
*    FUNCTION PARAMETERS:
*    Input:    char *SearchFilePath - A file and path is stripped out to
*                                     do the search.
*              char *SearchPathDir  - A second file path to use as an
*                                     optional directory to do a search.
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  3/06/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL FileExist( char *SearchFilePath, char *SearchPathDir )
{
    char SearchDir[MAX_PATH+1];
    char SearchFile[MAX_PATH+1];

    // Check to see if we have a valid path. If we have an empty string,
    // return true because there is nothing to find.
    if( !SearchFilePath[0] )
        return TRUE;

    // Strip out the path to do a search
    // If this param is being used, strip out the directory from this one
    if( SearchPathDir )
        CropOutPath( SearchPathDir, SearchDir );
    else
        CropOutPath( SearchFilePath, SearchDir );

    // Strip out the file name to do a search
    // This also works if the file name is not part of a path
    CropFileNameFromPath( SearchFilePath, SearchFile );

    return BOOL(SearchPath( SearchDir, SearchFile, NULL, NULL, NULL, NULL ));

}   // FileExist


/************************************************************************
*    FUNCTION NAME:         CropFileExtenOffPath                                                             
*
*    DESCRIPTION:           Strip the file name from the file path
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
*                  3/06/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

/*void CropFileNameFromPath( char *SourceFilePath, char *DestFileName, BOOL KeepExtension )
{
    int i, Length;

    // Get the string length
    Length = strlen( SourceFilePath );

    for( i = Length; i > -1; --i )
    {
        if( SourceFilePath[i] == '\\' )
            break;
    }

    ++i;

    strcpy( DestFileName, (SourceFilePath + i) );

    // Remove the extension
    if( !KeepExtension )
    {
        // Get the string length
        Length = strlen( DestFileName );

        for( i = Length; i > -1; --i )
        {
            if( DestFileName[i] == '.' )
            {
                DestFileName[i] = 0;
                break;
            }
        }
    }

}*/   // CropFileNameFromPath