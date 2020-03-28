
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
#include "SSConfig.h"        // Header file for the config dialog box.


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

TAnimateSaver::TAnimateSaver( void *ssData ) : TScreenSaver( ssData )
{
    // Init screen saver specific functions
    ZBuffer = NULL;
    ResLessThen640x480 = FALSE;
    Background = NULL;
    flagCounter = 0;
    rotateCounter = 0;
    pStar = NULL;
    StarCount = 0;
    CloudMaxLines = 0;
    CloudCounter = 0;

    // Init non-specific class members
    AnimCounter = 0;
    SaverTimer = 0;
    SaverSpeed = 0;
    ShareWareMode = FALSE;
    ShareWareTimer = 0;
    MusicTimer = 0;
    SongCount = 0;
    hOldPalette = NULL;
    Error_Number = 0;

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
    int RenderWidth, RenderHeight, ScreenResID, i;
    DWORD RenderTime;

    // Get the user's settings
    // Load all the ini file settings
    Settings.GetPrivateString();

	//Settings.SetRotateFlag( FALSE );

/*
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
*/

    // Get the current res ID for the screen size
    ScreenResID = GetScreenResSizeID( ScreenRect.right, ScreenRect.bottom );

    // Calculate the maximum render size
    if( Settings.GetCalcRenderRes() )
    {        
        for( i = 0; i < MAX_RES_CHECK; ++i )
        {
            RenderTime = GetRenderTime( ResCheck[i], Instance() );

            // Make sure we at least set the first resolution if there wasn't an error
            if( RenderTime > 0 && (RenderTime <= (MAX_MIL_RENDER_WAIT - BlitToScreen[i]) || i == 0) )
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

BOOL TAnimateSaver::LoadGraphics()
{
    BOOL Result = FALSE;
    TLoadRes LoadRes;
    TBitmap tmpBackbuffer, tmpBackArt;
    int CountLeft, CountTop, i, j, TmpCount;
    RECT SourceRect, DestRect;
    char ResName[20];
    WORD tempColor = 0;
    PSWordToByte RGB16 = PSWordToByte( &tempColor );

    // Init the classes using the screens DC. That way it doesn't have to make one
    tmpBackbuffer.InitActiveDC( GetScreenDC() );
    tmpBackArt.InitActiveDC( GetScreenDC() );
    buffer.InitActiveDC( GetScreenDC() );

    // Load the crew picture
    if( !tmpBackArt.LoadFromFile( "C:\\crew1.BMP" ) )
        goto HANDLE_ERROR;

    // Create the bitmap
    if( !tmpBackArt.CreateBitmap( STANDARD_DIB, SystemBitDepth, TOP_DOWN_BMP ) )
        goto HANDLE_ERROR;

    // Allocate the Z Buffer
    ZBuffer = new int[ RnderRect.right * RnderRect.bottom ];
    if( !ZBuffer )
        goto HANDLE_ERROR;

    // Zero out the ZBuffer
    memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );

    // One time initilization of the cos and sin tables
    // Makes no difference who calls it because the tables are static
    flagMatrix.InitCosSinTables();

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

        // Set a 16 bit color
        if( SystemBitDepth == 16 )
        {
            // Randonize a star color value
            RGB16->R = 15 + rand() % 13;
            RGB16->G = RGB16->R;
            RGB16->B = RGB16->R;

            // Set the pixel color for the star
            pStar[i].Color = tempColor;
        }
        else   // Set an 8 bit color
        {
            // Set the pixel color for the star
            pStar[i].Color = 10 + rand() % 30;
        }

    }

    //////////////////////////////////////////////////////
    //
    //            Load Polygon Objects
    //
    //////////////////////////////////////////////////////

    for( i = 0; i < MAX_FLAG_FRAMES; ++i )
    {
        // Generate the resource name
        sprintf( ResName, "flag%d", i );

        // Load the flag polygon object from resource
        if( !LoadTheResource( Instance(), ResName, "POLY", &LoadRes ) )
            goto HANDLE_ERROR;

        // Load the flag polygon object 
        if( !Flag[i].LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
            goto HANDLE_ERROR;
    }

    // Load the flag pole polygon object from resource
    if( !LoadTheResource( Instance(), "pole", "POLY", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the flag pole polygon object 
    if( !Pole.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;


    //////////////////////////////////////////////////////
    //
    //            Load Texture Library
    //
    //////////////////////////////////////////////////////

    // Load the flag texture library from resource
    if( !LoadTheResource( Instance(), "FlagBt", "BTXT", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the flag texture library 
    if( !FlagTxt.LoadFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;


    //////////////////////////////////////////////////////
    //
    //   Load the Shaded Table into the polygon Objects
    //
    //////////////////////////////////////////////////////

    // Load the shaded table from resource
    if( SystemBitDepth <= 8 )
    {
        if( !LoadTheResource( Instance(), "ShadedTable8bit", "STBL", &LoadRes ) )
            goto HANDLE_ERROR;

        // Load the 8 bit shaded table
        for( i = 0; i < MAX_FLAG_FRAMES; ++i ) 
            Flag[i].SetShadeLookUpTable( (BYTE *)LoadRes.pData );

        Pole.SetShadeLookUpTable( (BYTE *)LoadRes.pData );
    }
    else
    {
        if( !LoadTheResource( Instance(), "ShadedTable16bit", "STBL", &LoadRes ) )
            goto HANDLE_ERROR;

        // Load the 16 bit shaded table
        for( i = 0; i < MAX_FLAG_FRAMES; ++i ) 
            Flag[i].SetShadeLookUpTable( (WORD *)LoadRes.pData );

        Pole.SetShadeLookUpTable( (WORD *)LoadRes.pData );
    }    

    // Create the video buffer depending on the screen color depth
    // All texture libraries use the same 8 bit palette
    if( !buffer.CreateDIBSectionBuffer( RnderRect.right, -RnderRect.bottom, SystemBitDepth, FlagTxt.TMap[ 0 ].Palette ) )
    {
        strcpy( errorStr, "Can't allocate background buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Set the ZBuffer
    for( i = 0; i < MAX_FLAG_FRAMES; ++i )
        Flag[i].SetZBuffer( ZBuffer );

    Pole.SetZBuffer( ZBuffer );

    // Set the size of the rendering area
    for( i = 0; i < MAX_FLAG_FRAMES; ++i )
        Flag[i].SetRenderSize( RnderRect.right, RnderRect.bottom );

    Pole.SetRenderSize( RnderRect.right, RnderRect.bottom );

    // Set the maxium Z and Light distance
    for( i = 0; i < MAX_FLAG_FRAMES; ++i )
    {
        Flag[i].SetMaxZDistance( 400000 );
        Flag[i].SetMaxLightDistance( 900000 );
    }

    Pole.SetMaxZDistance( 400000 );
    Pole.SetMaxLightDistance( 900000 );

    // Set the camera scale
    for( i = 0; i < MAX_FLAG_FRAMES; ++i )
        Flag[i].SetCameraScale( CameraSize );

    Pole.SetCameraScale( CameraSize );

    // Set the video buffer
    for( i = 0; i < MAX_FLAG_FRAMES; ++i )
        Flag[i].SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );

    Pole.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );

    // Set the texture libraries
    for( i = 0; i < MAX_FLAG_FRAMES; ++i )
        Flag[i].SetTextures( &FlagTxt );

    Pole.SetTextures( &FlagTxt );


    //////////////////////////////////////////////////////
    //
    //   Load Texture Coordinates
    //
    //   Loading the coordinates inits all the polygons
    //   in the polygon object file which is why it's the
    //   last item to load.
    //
    //////////////////////////////////////////////////////

    // Load the flag texture coordinates from resource
    if( !LoadTheResource( Instance(), "FlagCrd", "CRD", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the flag texture coordinates
    for( i = 0; i < MAX_FLAG_FRAMES; ++i )
    { 
        Flag[i].LoadCrdFromPointer( (BYTE *)LoadRes.pData );

        // Set the light source
        Flag[i].SetLightSource( 271.0F, -279.0F, 100.0F );
    }

    // Init with the default texture
    Pole.InitDefText();

    // Set all the polygons to render as a solid color. This will help in increase speed.
    for( i = 0; i < Pole.GetTotalPolyCount(); ++i )
        Pole.GetPoly( i )->SetSolidColor( TRUE );

    // Set the light source
    Pole.SetLightSource( LIGHT_SOURCE_X, LIGHT_SOURCE_Y, LIGHT_SOURCE_Z );

    // Load the palette if we are in 8 bit or less mode
    // Init the screens palette
    if( SystemBitDepth <= 8 )
    {
        // Saver the old palette to select back into the dc. If this doesn't happen, the newly created
        // palette will never get deleted
        hOldPalette = SelectPalette( GetScreenDC(), buffer.GetPalette(), FALSE );
        RealizePalette( GetScreenDC() );
    }
/*
    if( SystemBitDepth <= 8 )
    {
        // Load the background graphic from resource
        if( !tmpBackArt.LoadFromResource( Instance(), "BlueStarBackground8", "BMP" ) )
            goto HANDLE_ERROR;
    }
    else
    {
        // Load the background graphic from resource
        if( !tmpBackArt.LoadFromResource( Instance(), "BlueStarBackground16", "BMP" ) )
            goto HANDLE_ERROR;
    }

    // Create the bitmap
    if( !tmpBackArt.CreateBitmap( DIB_SECTION_BMP, SystemBitDepth ) )
        goto HANDLE_ERROR;


    // Calculate the number of art heights that will fill the screen
    TmpCount = abs( RnderRect.bottom / tmpBackArt.height() );

    // Make sure it's not zero
    if( TmpCount == 0 )
        ++TmpCount;

    // Add one more
    ++TmpCount;

    // Make it twice the size so the scroll feature work
    TmpCount *= 2;

    // Calculate the scrolling height of the buffer
    CloudMaxLines = TmpCount * tmpBackArt.height();
*/

    // Create a tempoary buffer depending on the screen color depth
    if( !tmpBackbuffer.CreateDIBSectionBuffer( RnderRect.right, -RnderRect.bottom, SystemBitDepth, FlagTxt.TMap[ 0 ].Palette ) )
    {
        strcpy( errorStr, "Can't allocate temp background buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Allocate the background buffer
    Background = new BYTE[ (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) ];
    if( !Background )
        goto HANDLE_ERROR;

	RECT rect;

	SizeToFitRect( &rect, tmpBackArt.width(), tmpBackArt.height(), RnderRect.right, RnderRect.bottom );

	tmpBackArt.StretchDraw( &tmpBackbuffer, &rect );
/*
    // Reset the CloudMaxLines so that we don't over run the buffer
    CloudMaxLines -= tmpBackArt.height() * (TmpCount / 2);

    // Blit the art to the temporary buffer
        
    // Divide the width and the heights to get the number of tiles needed
    CountLeft = tmpBackbuffer.width() / tmpBackArt.width();
    CountTop  = tmpBackbuffer.height() / tmpBackArt.height();

    // There's a good chance this wont divide evenly so add one more if there is a remainder
    if( tmpBackbuffer.width() % tmpBackArt.width() )
        ++CountLeft;
    if( tmpBackbuffer.height() % tmpBackArt.height() )
        ++CountTop;

    // Init the source rect which is the the size of the art
    SetRect( &SourceRect, 0, 0, tmpBackArt.width(), tmpBackArt.height() );

    // Tile the art across the temp buffer
    for( j = 0; j < CountTop; ++j )
    {
        for( i = 0; i < CountLeft; ++i )
        {        
            SetRect( &DestRect, i * tmpBackArt.width(), j * tmpBackArt.height(), 
                                tmpBackArt.width(), tmpBackArt.height() );
            tmpBackArt.Draw( &tmpBackbuffer, &DestRect, &SourceRect );
        }
    }
*/
    // Copy the bitmap buffer to the byte buffer
    memcpy( Background, tmpBackbuffer.GetPtrToDIBsectionBits(), (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );


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

BOOL TAnimateSaver::InitSaverData()
{
    BOOL Result = FALSE, resetScnRes = FALSE;
    int resID;

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

    // Set the initial rotation and offset
    flagMatrix.Rotate( FLAG_ROTATE_X, 0, 0 );
    flagMatrix.Translate( 0, FLAG_OFFSET_Y, FLAG_OFFSET_Z );
    //flagMatrix.Translate( 0, 0, 250 ); // test size

    Result = TRUE;

    HANDLE_ERROR:;

    return Result;

}   // InitSaverData


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

void TAnimateSaver::SetScreenResolution()
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
                Sleep( 2000 );

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
    if( !MPlayer.GetPlayError() && MusicTimer < timeGetTime() )
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

void TAnimateSaver::DisplayMusicError()
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

void TAnimateSaver::Animate()
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

            // RESET THE SCREEN HERE
            FillRect( GetScreenDC(), &DesktopRect, BlackBrush );

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
    if( ZBuffer )
        delete [] ZBuffer;

    if( Background )
        delete [] Background;

    if( pStar )
        delete [] pStar;

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
}   // PauseSound


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

void TAnimateSaver::RenderBuffer()
{    
    int Offset, i;
    WORD *tmpBackground16, StarColor;
    BYTE *tmpBackground8;

    // Zero out the ZBuffer
    memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );

	memcpy( buffer.GetPtrToDIBsectionBits(), Background, (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Paint the buffer with stars
    if( SystemBitDepth == 16 )
    {
        // Type cast the buffer to a word
        tmpBackground16 = (WORD *)buffer.GetPtrToDIBsectionBits();

        // Clear out the video buffer with the cloud buffer
        //memcpy( tmpBackground16, (Background + ((RnderRect.right * CloudCounter) * (SystemBitDepth / 8))), 
                //(RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

		

        // Render the stars to the buffer
        for( i = 0; i < StarCount; ++i )
        {   
            // Set the pixel color for the star
            tmpBackground16[ (pStar[i].Y * RnderRect.right) + pStar[i].X ] |= pStar[i].Color;
        }
    }
    else
    {
        // Copy the pointer
        tmpBackground8 = (BYTE *)buffer.GetPtrToDIBsectionBits();

        // Clear out the video buffer with the cloud buffer
        //memcpy( tmpBackground8, (Background + ((RnderRect.right * CloudCounter) * (SystemBitDepth / 8))), 
                //(RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );



        // Render the stars to the buffer
        for( i = 0; i < StarCount; ++i )
        {   
            // Set the pixel color for the star
            tmpBackground8[ (pStar[i].Y * RnderRect.right) + pStar[i].X ] = pStar[i].Color;
        }
    }

    // Inc the cloud counter
    //CloudCounter = (CloudCounter + 1) % CloudMaxLines;

    // Do we twinkle the stars?
    if( Settings.GetTwinkleStars() )
    {
        // Twinkle the stars every 4 cycles
        if( flagCounter % 4 == 0 )
        {
            // Randomly pick a star to twinkel          
            star = rand() % StarCount;
        }
        
        // Twinkle the stars
        if( SystemBitDepth == 16 )
        {
            // Calculate the offset
            Offset = (pStar[star].Y * RnderRect.right) + pStar[star].X;

            // Get the color of the star
            StarColor = tmpBackground16[ Offset ];

            // Set the pixel color for the star to simulate a twinkle
            //tmpBackground16[ Offset ] = 0xFFFF;
            tmpBackground16[ Offset-1 ] = StarColor;
            tmpBackground16[ Offset+1 ] = StarColor;
            tmpBackground16[ Offset-RnderRect.right ] = StarColor;
            tmpBackground16[ Offset+RnderRect.right ] = StarColor;
        }
        else
        {
            // Calculate the offset
            Offset = (pStar[star].Y * RnderRect.right) + pStar[star].X;

            // Get the color of the star
            StarColor = tmpBackground8[ Offset ];

            // Set the pixel color for the star to simulate a twinkle
            tmpBackground8[ Offset ] = StarColor;
            tmpBackground8[ Offset-1 ] = StarColor;
            tmpBackground8[ Offset+1 ] = StarColor;
            tmpBackground8[ Offset-RnderRect.right ] = StarColor;
            tmpBackground8[ Offset+RnderRect.right ] = StarColor;
        }
    }
    
    // Paint the buffer
    if( SystemBitDepth <= 8 )
    {        
        // Force the flag object to only render all polygons on one side.
        // This eliminates seeing broken, non-plainer polygons
        if( (rotateCounter >= 0 && rotateCounter <= 250) || (rotateCounter >= 760 && rotateCounter <= 1024) )
            Flag[flagCounter].PaintVBuffer8To8( flagMatrix, POS_ZBUFFER, Flag[flagCounter].GetTotalPolyCount() >> 1, 0, TRUE );
        else if( rotateCounter >= 260 && rotateCounter <= 750 )
            Flag[flagCounter].PaintVBuffer8To8( flagMatrix, POS_ZBUFFER, 0, Flag[flagCounter].GetTotalPolyCount() >> 1, TRUE );
        else
            Flag[flagCounter].PaintVBuffer8To8( flagMatrix, POS_ZBUFFER );

        Pole.PaintVBuffer8To8( flagMatrix, POS_ZBUFFER );
    }
    else
    {
        // Force the flag object to only render all polygons on one side.
        // This eliminates seeing broken, non-plainer polygons
        if( (rotateCounter >= 0 && rotateCounter <= 250) || (rotateCounter >= 770 && rotateCounter <= 1024) )
            Flag[flagCounter].PaintVBuffer8To16( flagMatrix, POS_ZBUFFER, Flag[flagCounter].GetTotalPolyCount() >> 1, 0, TRUE );
        else if( rotateCounter >= 260 && rotateCounter <= 750 )
            Flag[flagCounter].PaintVBuffer8To16( flagMatrix, POS_ZBUFFER, 0, Flag[flagCounter].GetTotalPolyCount() >> 1, TRUE );
        else
            Flag[flagCounter].PaintVBuffer8To16( flagMatrix, POS_ZBUFFER );

        Pole.PaintVBuffer8To16( flagMatrix, POS_ZBUFFER );
    }

    // Do we rotate the flag?
    if( Settings.GetRotateFlag() )
    {
        flagMatrix.InitilizeMatrix();
        flagMatrix.Rotate( FLAG_ROTATE_X, rotateCounter, 0 );
        flagMatrix.Translate( 0, FLAG_OFFSET_Y, FLAG_OFFSET_Z );
        
        // Rotation counter
        rotateCounter = (rotateCounter + 1) % DEGREECOUNT;
    }

    // inc the flag animation counter
    flagCounter = (flagCounter + 1) % MAX_FLAG_FRAMES;      

}   // RenderBuffer
