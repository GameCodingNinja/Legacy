
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

int gFlareFrames[MAX_FLARE_FRAMES] = { 0, 0, 1, 1, 2, 2, 3, 3, 3, 2, 2, 1, 1 };

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
    Background = NULL;
    pStar = NULL;
    StarCount = 0;
    CloudMaxLines = 0;
    CloudCounter = 0;
    colorCounter = 0;
    ManySndDevices = FALSE;

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
        CameraSize = 120;
    }

    // Reset the bit depth because we only want to
    // allow 8 or 16 bit screen depths
    if( SystemBitDepth <= 8 && !(Settings.GetForce16BitColor() && Mode() == ssSaver) )
        SystemBitDepth = 8;
    else
        SystemBitDepth = 16;

    // Select the angle we want
    switch( Settings.GetBurstAngle() )
    {        
        case ANGLE_20_DEGREES:
            MaxX = 3400;
            MaxY = 1300;
            MaxZ = 4500;
            RandZ = 50;
            MaxLifeCount = 180;
            Xangle = 0.0f;
            Ystart = -3000;
        break;
        
        case ANGLE_50_DEGREES:            
            MaxX = 2200;
            MaxY = 2500;
            MaxZ = 2600;
            RandZ = 500;
            MaxLifeCount = 150;
            Xangle = 90.0f;
            Ystart = -1000;
        break;
        
        case ANGLE_90_DEGREES:
            MaxX = 2500;
            MaxY = 3500;
            MaxZ = 0;
            RandZ = 700;
            MaxLifeCount = 170;
            Xangle = 255.0f;
            Ystart = 0;
        break;
        
        default:
            MaxX = 3400;
            MaxY = 1300;
            MaxZ = 4500;
            RandZ = 50;
            MaxLifeCount = 180;
            Xangle = 0.0f;
            Ystart = -3000;
        break;  
    }
    
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
    TBitmap tmpBackbuffer, tmpBackArt, tmpLtBackArt;
    int CountLeft, CountTop, i, j, TmpCount;
    RECT SourceRect, DestRect;
    WORD tempColor = 0;
    PSWordToByte RGB16 = PSWordToByte( &tempColor );

    // Init the classes using the screens DC. That way it doesn't have to make one
    tmpBackbuffer.InitActiveDC( GetScreenDC() );
    tmpBackArt.InitActiveDC( GetScreenDC() );
    tmpLtBackArt.InitActiveDC( GetScreenDC() );
    buffer.InitActiveDC( GetScreenDC() );

    // Just to be on the safe side, double check that the number is within range
    // If not for some strange reason, set it to the default
    if( Settings.GetFireworkCount() < 1 || Settings.GetFireworkCount() > MAX_FIREWORKS )
        Settings.SetFireworkCount( 2 );

    // Allocate the fireworks class
    pcFirework = new CFirework[ Settings.GetFireworkCount() ];
    if( !pcFirework )
    {
        strcpy( errorStr, "Can't allocate firework class. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // One time initilization of the cos and sin tables
    // Makes no difference who calls it because the tables are static
    pcFirework[0].fwMatrix1[0].InitCosSinTables();

    // Setup the color list for all the different colors
    for( i = 0; i < MAX_COLORS; ++i )
        colors.Add( i );

    // Now mix them up
    colors.MixUp();

    // Init the fireworks class
    for( i = 0; i < Settings.GetFireworkCount(); ++i )
    {
        // Init a burst because we are starting somewhere in the burst
        // This can not be done before the sin and cos tables are initilized
        pcFirework[i].InitBurst( MaxX, MaxY, MaxZ, Ystart, RandZ, MaxLifeCount );

        // Pick the color
        pcFirework[i].SetColorOffset( colors.GetInt( colorCounter ) * MAX_FLARE_IMAGES );

        // Inc the color counter
        colorCounter = (colorCounter + 1) % colors.Count();

        // Mix it up when we hit zero
        if( colorCounter == 0 )
            colors.MixUp();

        // Randomly set the art index the particle will use
        for( j = 0; j < MAX_PARTICLE_BURST; ++j )
            pcFirework[i].cParticle.SetFrame( j, rand() % MAX_FLARE_FRAMES );

        // Set the particle life time counter
        pcFirework[i].LifeTimeBurstCounter = rand() % pcFirework[i].MaxLifeTime;
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
            pStar[i].Color = STAR_8_BIT_COLOR;
        }

    }

    //////////////////////////////////////////////////////
    //
    //            Load Polygon Objects
    //
    //////////////////////////////////////////////////////

    // Choose the bigger box for the far away view
    if( Settings.GetBurstAngle() == ANGLE_20_DEGREES )
    {
        // Load the polygon object from resource
        if( !LoadTheResource( Instance(), "boxBig", "POLY", &LoadRes ) )
            goto HANDLE_ERROR;
    }
    else
    {
        // Load the polygon object from resource
        if( !LoadTheResource( Instance(), "box", "POLY", &LoadRes ) )
            goto HANDLE_ERROR;
    }

    // Load the polygon object 
    if( !box.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
    {
        strcpy( errorStr, "Can't load polygon object. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }


    //////////////////////////////////////////////////////
    //
    //            Load Texture Library
    //
    //////////////////////////////////////////////////////

    if( SystemBitDepth == 8 )
    {
        // Load the texture library from resource
        if( !LoadTheResource( Instance(), "box8Bt", "BTXT", &LoadRes ) )
            goto HANDLE_ERROR;
    }
    else
    {
        // Load the texture library from resource
        if( !LoadTheResource( Instance(), "box16Bt", "BTXT", &LoadRes ) )
            goto HANDLE_ERROR;
    }

    // Load the texture library 
    if( !boxTxt.LoadFromPointer( (BYTE *)LoadRes.pData ) )
    {
        strcpy( errorStr, "Can't load texture library. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }


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
        box.SetShadeLookUpTable( (BYTE *)LoadRes.pData );
    }    

    // Create the video buffer depending on the screen color depth
    // All texture libraries use the same 8 bit palette
    if( !buffer.CreateDIBSectionBuffer( RnderRect.right, -RnderRect.bottom, SystemBitDepth, boxTxt.TMap[ 0 ].Palette ) )
    {
        strcpy( errorStr, "Can't allocate background buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Set the size of the rendering area
    box.SetRenderSize( RnderRect.right, RnderRect.bottom );

    // Set the maxium Z and Light distance
    box.SetMaxZDistance( 400000 );
    box.SetMaxLightDistance( 50000 );

    // Set the camera scale
    box.SetCameraScale( CameraSize );

    // Set the video buffer
    box.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );

    // Set the texture libraries
    box.SetTextures( &boxTxt );


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
    if( !LoadTheResource( Instance(), "boxCrd", "CRD", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the texture coordinates
    box.LoadCrdFromPointer( (BYTE *)LoadRes.pData );

    // Load the palette if we are in 8 bit or less mode
    // Init the screens palette
    if( SystemBitDepth == 8 )
    {
        // Saver the old palette to select back into the dc. If this doesn't happen, the newly created
        // palette will never get deleted
        hOldPalette = SelectPalette( GetScreenDC(), buffer.GetPalette(), FALSE );
        RealizePalette( GetScreenDC() );
    }

    if( SystemBitDepth == 8 )
    {
        // Load the background graphic from resource
        if( !tmpBackArt.LoadFromResource( Instance(), "CloudBackground8", "BMP" ) )
        {
            strcpy( errorStr, "Can't load 8-bit background artwork. Your system resources may be low so try restarting your computer." );
            goto HANDLE_ERROR;
        }
    }
    else
    {
        // Load the background graphic from resource
        if( !tmpBackArt.LoadFromResource( Instance(), "CloudBackground16", "BMP" ) )
        {
            strcpy( errorStr, "Can't load 16-bit background artwork. Your system resources may be low so try restarting your computer." );
            goto HANDLE_ERROR;
        }
    }

    // Create the bitmap
    if( !tmpBackArt.CreateBitmap( DIB_SECTION_BMP, SystemBitDepth ) )
    {
        strcpy( errorStr, "Can't create temporary background art buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Calculate the number of art heights that will fill the screen
    TmpCount = abs( RnderRect.bottom / tmpBackArt.height() );

    // Make sure it's not zero
    if( TmpCount == 0 )
        TmpCount = 1;

    // If the height of the render rect is greater and not divisible, add one.
	if( RnderRect.bottom > tmpBackArt.height() && ( RnderRect.bottom % tmpBackArt.height() ) )
		++TmpCount;

    // Make it twice the size so the scroll feature work
    TmpCount *= 2;

    // Calculate the scrolling height of the buffer
    CloudMaxLines = TmpCount * tmpBackArt.height();

    // Create a tempoary buffer depending on the screen color depth
    if( !tmpBackbuffer.CreateDIBSectionBuffer( RnderRect.right, CloudMaxLines, SystemBitDepth, boxTxt.TMap[ 0 ].Palette ) )
    {
        strcpy( errorStr, "Can't create temporary background buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Allocate the background buffer
    Background = new BYTE[ (RnderRect.right * CloudMaxLines) * (SystemBitDepth / 8) ];
    if( !Background )
    {
        strcpy( errorStr, "Can't allocate background byte array buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

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
    // The light and dark bitmap are both the same size
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

    // Copy the bitmap buffer to the byte buffer
    memcpy( Background, tmpBackbuffer.GetPtrToDIBsectionBits(), (tmpBackbuffer.width() * tmpBackbuffer.height()) * (SystemBitDepth / 8) );

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

BOOL CAnimateSaver::LoadSound()
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
            // Setup the explosion sounds
            if( !sndExplode.AllocateWavLoad( MAX_EXPLODE_SNDS ) )
                goto HANDLE_ERROR;

            if( !sndExplode.AllocateWavDevices( MAX_EXPLODE_SNDS ) )
                goto HANDLE_ERROR;

            // Setup the shoot sounds
            if( !sndShoot.AllocateWavLoad( MAX_SHOOT_SNDS ) )
                goto HANDLE_ERROR;

            if( !sndShoot.AllocateWavDevices( MAX_SHOOT_SNDS ) )
                goto HANDLE_ERROR;


            // Load the explode sounds from resource
            for( i = 0; i < MAX_EXPLODE_SNDS; ++i )
            {
                wsprintf( tmpStr, "explode%d", i );

                if( !sndExplode.LoadFromResource( Instance(), tmpStr, i ) )
                    goto HANDLE_ERROR;

                if( !sndExplode.ApplyLoadToDevice( i, i ) )
                    goto HANDLE_ERROR;
            }

            // Load the shoot sounds from resource
            for( i = 0; i < MAX_SHOOT_SNDS; ++i )
            {
                wsprintf( tmpStr, "shoot%d", i );

                if( !sndShoot.LoadFromResource( Instance(), tmpStr, i ) )
                    goto HANDLE_ERROR;

                if( !sndShoot.ApplyLoadToDevice( i, i ) )
                    goto HANDLE_ERROR;
            }

            // Setup the one looping sound
            if( !loopLoad.LoadFromResource( Instance(), "fireworks" ) )
                goto HANDLE_ERROR;

            // Set the header
            if( loopDev.SetWaveHeader( loopLoad.GetHeader() ) != MMSYSERR_NOERROR )
                goto HANDLE_ERROR;

            // Open a wav device to this loaded wav format
            if( loopDev.Open( loopLoad.GetWavFormat() ) != MMSYSERR_NOERROR )
                goto HANDLE_ERROR;

            // Prepare the header for playing
            if( loopDev.PrepareHeader() != MMSYSERR_NOERROR )
                goto HANDLE_ERROR;

            // Setup the croud sound
            if( !croudLoad.LoadFromResource( Instance(), "croud" ) )
                goto HANDLE_ERROR;

            // Set the header
            if( croudDev.SetWaveHeader( croudLoad.GetHeader() ) != MMSYSERR_NOERROR )
                goto HANDLE_ERROR;

            // Open a wav device to this loaded wav format
            if( croudDev.Open( croudLoad.GetWavFormat() ) != MMSYSERR_NOERROR )
                goto HANDLE_ERROR;

            // Prepare the header for playing
            if( croudDev.PrepareHeader() != MMSYSERR_NOERROR )
                goto HANDLE_ERROR;

            // Start playing the looping fireworks sound
            if( loopDev.Play( 0xFFFFFFFF ) != MMSYSERR_NOERROR )
                goto HANDLE_ERROR;
        }
        else  // We can't have multiple devices
        {
            // Setup all the sounds
            if( !sndSimple.AllocateWavLoad( MAX_EXPLODE_SNDS + MAX_SHOOT_SNDS ) )
                goto HANDLE_ERROR;

            // All we get is one that all the sounds will have to share
            if( !sndSimple.AllocateWavDevices( 1 ) )
                goto HANDLE_ERROR;

            // Load the explode sounds from resource
            for( i = 0; i < MAX_EXPLODE_SNDS; ++i )
            {
                wsprintf( tmpStr, "explode%d", i );

                if( !sndSimple.LoadFromResource( Instance(), tmpStr, i ) )
                    goto HANDLE_ERROR;
            }

            // Load the shoot sounds from resource
            for( i = 0; i < MAX_SHOOT_SNDS; ++i )
            {
                wsprintf( tmpStr, "shoot%d", i );

                if( !sndSimple.LoadFromResource( Instance(), tmpStr, i + MAX_EXPLODE_SNDS ) )
                    goto HANDLE_ERROR;
            }

            // Kill the playing of any music because we can't have more then one device
            Settings.SetThemeMusicOn( FALSE );
            Settings.SetLoadYourOwn( FALSE );

            // Force a sound load. If there is any errors to be seen, it will help 
            // to cause the error to show up sooner.
            if( !sndSimple.ApplyLoadToDevice( 0, 0 ) )
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

BOOL CAnimateSaver::InitSaverData()
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

    // init to use the buffer
    errorCanvas.InitActiveDC( buffer.Handle() );

    // Create the default font
    errorCanvas.SetFont( "Arial", 12, FONT_BOLD, RGB( 255, 0, 0 ) );

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
        {   /*
            // This will play the song that has already been loaded
            if( PlayerStatus == PLAYER_STOPPED || PlayerStatus == PLAYER_PAUSED )
                MPlayer.Play();

            // This will load the song and play it for the first time
            else if( PlayerStatus == PLAYER_CLOSED )
                MPlayer.Play( "ThemeSong", "MIDI" ); */
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

        // Rest the timer to check if the music is done playing
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
        errorCanvas.SetBackgroundMode( TRANSPARENT );

        // Draw the text
        errorCanvas.DrawText( 5, 25, MPlayer.GetErrorStr() );

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
        // Show an error screen if there is a problem playing music
        DisplayMusicError();
        DisplaySoundError();

        // Us the RepaintScreen function to indicate we have come back
        // from sleeping. Use this to restart the looping background sound.
        if( RepaintScreen() )
        {
            if( Settings.GetSoundOn() && ManySndDevices )
                loopDev.Restart();
        }
        
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
    // Do any cleanup before the buffer is freed 
    // because this canvas uses the buffer's HDC
    errorCanvas.Free();

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

    // Pause the looping fireworks background sound
    if( Settings.GetSoundOn() && ManySndDevices )
        loopDev.Pause();

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

void CAnimateSaver::RenderBuffer()
{
    int i, j, frameIndex, activeCount, colorOffset;
    WORD *tmpBackground16;
    BYTE *tmpBackground8;

    // Paint the buffer with stars
    if( SystemBitDepth == 16 )
    {
        // Type cast the buffer to a word
        tmpBackground16 = (WORD *)buffer.GetPtrToDIBsectionBits();

        // Clear out the video buffer with the cloud buffer
        memcpy( tmpBackground16, (Background + ((RnderRect.right * CloudCounter) * (SystemBitDepth / 8))),
                (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

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
        memcpy( tmpBackground8, (Background + ((RnderRect.right * CloudCounter) * (SystemBitDepth / 8))), 
                (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

        // Render the stars to the buffer
        for( i = 0; i < StarCount; ++i )
        {   
            // Set the pixel color for the star
            tmpBackground8[ (pStar[i].Y * RnderRect.right) + pStar[i].X ] = pStar[i].Color;
        }
    }

    // Inc the cloud counter to ove the clouds
    CloudCounter = (CloudCounter + 1) % CloudMaxLines;

    // Animate all the fire works
    for( i = 0; i < Settings.GetFireworkCount(); ++i )
    {
        // Init a new particle burst
        if( pcFirework[i].LifeTimeBurstCounter == 0 )
        {
            // Init the particle burst
            pcFirework[i].InitBurst( MaxX, MaxY, MaxZ, Ystart, RandZ, MaxLifeCount );

            // Pick the color
            pcFirework[i].SetColorOffset( colors.GetInt( colorCounter ) * MAX_FLARE_IMAGES );

            // Inc the color counter
            colorCounter = (colorCounter + 1) % colors.Count();

            // Mix it up when we hit zero
            if( colorCounter == 0 )
                colors.MixUp();

            // Update the particle life time counter
            pcFirework[i].LifeTimeBurstCounter = (pcFirework[i].LifeTimeBurstCounter + 1) % pcFirework[i].MaxLifeTime;

            // Play the shoot sound
            if( Settings.GetSoundOn() )
            {
                if( ManySndDevices )
                {
                    sndShoot.PlayRandom();
                }
                else  // We can't have multiple devices
                {
                    if( !sndSimple.IsPlaying()  )
                    {
                        sndSimple.ApplyLoadToDevice( MAX_EXPLODE_SNDS + (rand() % MAX_SHOOT_SNDS), 0 );
                        sndSimple.Play();
                    }
                }
            }
        }
        // Animates one particle to simulate the rocket being fired off
        else if( pcFirework[i].LifeTimeBurstCounter == 1 )
        {
            if( !pcFirework[i].UpdatePoint() )
            {
                // Inc the life time counter
                pcFirework[i].LifeTimeBurstCounter = (pcFirework[i].LifeTimeBurstCounter + 1) % pcFirework[i].MaxLifeTime;

                // Play the sound we we are supposed to
                if( Settings.GetSoundOn() )
                {
                    if( ManySndDevices )
                    {
                        sndExplode.PlayRandom();

                        // Randomly pick if we are to hear the croud
                        if( pcFirework[i].GetActiveCount() > 600 )
                        {
                            if( !croudDev.IsPlaying() )
                                croudDev.Play();
                        }
                    }
                    else  // We can't have multiple devices
                    {
                        sndSimple.ApplyLoadToDevice( rand() % MAX_EXPLODE_SNDS, 0 );
                        sndSimple.Play();
                    }
                }
            }

            // Reinit the matrix
            pcFirework[i].fwMatrix1[0].InitilizeMatrix();
            pcFirework[i].fwMatrix1[0].Rotate( -Xangle, 0, 0 );            

            // Set the new position
            pcFirework[i].fwMatrix1[0].Translate( pcFirework[i].sIPoint3D.X, pcFirework[i].sIPoint3D.Y, pcFirework[i].sIPoint3D.Z );
            pcFirework[i].fwMatrix1[0].Rotate( Xangle, 0, 0 );

            // Get the index to the frame art
            frameIndex = pcFirework[i].cParticle.GetFrame( 0 );
            
            // Inc the frame and save it
            pcFirework[i].cParticle.SetFrame( 0, (frameIndex + 1) % MAX_FLARE_FRAMES );

            // Randomly pick the texture handle
            box.SetTextHandle( 0, (WHITE_COLOR * MAX_FLARE_IMAGES) + gFlareFrames[ frameIndex ] );

            if( SystemBitDepth == 8 )
                box.PaintVBuffer8To8( pcFirework[i].fwMatrix1[0], NO_ZBUFFERING, 0, 1, TRUE );
            else
                box.PaintVBuffer16To16( pcFirework[i].fwMatrix1[0], NO_ZBUFFERING, 0, 1, TRUE, PAINT_OR );
        }
        // Controls the burst animation
        else
        {
            // Get the active count for this firework burst
            activeCount = pcFirework[i].GetActiveCount();
            
            // Have the particle class set all the matrixes and do the billboarding
            pcFirework[i].cParticle.SetMatrixArray( activeCount, pcFirework[i].fwMatrix1, 
                                                    Xangle, 0, 0, pcFirework[i].LifeTimeBurstCounter );

            // Inc the time frame
            pcFirework[i].LifeTimeBurstCounter = (pcFirework[i].LifeTimeBurstCounter + 1) % pcFirework[i].MaxLifeTime;

            // Get the color offset before entering the loop. This is the color of the burst.
            colorOffset = pcFirework[i].GetColorOffset();
            
            // Paint the buffer
            for( j = 0; j < activeCount; ++j )
            {
                // Get the index to the frame art
                frameIndex = pcFirework[i].cParticle.GetFrame( j );
                
                // Inc the frame and save it
                pcFirework[i].cParticle.SetFrame( j, (frameIndex + 1) % MAX_FLARE_FRAMES );

                // Randomly pick the texture handle
                box.SetTextHandle( 0, colorOffset + gFlareFrames[ frameIndex ] );

                if( SystemBitDepth == 8 )
                    box.PaintVBuffer8To8( pcFirework[i].fwMatrix1[j], NO_ZBUFFERING, 0, 1, TRUE );
                else
                    box.PaintVBuffer16To16( pcFirework[i].fwMatrix1[j], NO_ZBUFFERING, 0, 1, TRUE, PAINT_OR );                
            }
        }
    }
      
}   // RenderBuffer


/************************************************************************                                                             
*
*    DESCRIPTION:           Fireworks Class
*
************************************************************************/


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

CFirework::CFirework()
{
    // Allocate the particle array
    cParticle.AllocateArray( MAX_PARTICLE_BURST );

    activeCount = 0;

}   // Constructor


/************************************************************************
*    FUNCTION NAME:         InitBurst                                                             
*
*    DESCRIPTION:           Init the burst data
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

void CFirework::InitBurst( int MaxX, int MaxY, int MaxZ, int Yoffset, int RandZ, int LifeCount )
{
    int offset[2] = { -1,1 };
    int vel;
    float accelY = -0.8, accelXZ = 0.0;
    //float accelZtb[] = { 6.0, SGN_MATCH_VEL, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0 };
    //float accelXZtb[] = { 1.0, SGN_NO_CHANGE, 0.0 };
    //float accelYtb[] = { 4.0, SGN_NO_CHANGE, -0.8, -0.9, -1.0, -1.1 };

    // Randomise the life time of the burst to give some
    // bursts a little delay
    MaxLifeTime = LifeCount + (rand() % 30);

    // Randomize the point in space to explode
    X = (1 + rand() % MaxX) * offset[rand() % 2];
    Y = MaxY + (rand() % 1000);
    Z = MaxZ + (RandZ * offset[rand() % 2]);

    // Randomize a velocity
    // Every now and then pick a real big one
    if( rand() % 7 )
        vel = 10 + (rand() % 10);
    else
        vel = 20 + (rand() % 11);

    // Set the active number of particles
    activeCount = vel * 30; 

    // Pick the type of burst we want
    if( rand() % 5 )
    {
        // Produces a circular burst
        cParticle.InitBurst( activeCount,                   // Number of active particles to init
                             X, Y, Z,                       // Init the point of explosion
                             vel, 0,                        // set the max and min velocity
                             &accelXZ, &accelY, &accelXZ,   // Set the X, Y & Z acceleration
                             ACCEL_REF_PTR_VALUE,           // Indicate how we are using the acceleration pointers
                             VEL_MAX_ONLY, SGN_ALL_POS,     // Indicate how we are using the velocity pointer
                             BURST_CIRCULAR, CIR_ROT_X );   // Indicate any additional calculations to the vector
    }
    else
    {
        // Produces a random burst
        cParticle.InitBurst( activeCount,                      // Number of active particles to init
                             X, Y, Z,                          // Init the point of explosion
                             vel, vel/2,                       // set the max and min velocity
                             &accelXZ, &accelY, &accelXZ,      // Set the X, Y & Z acceleration
                             ACCEL_REF_PTR_VALUE,              // Indicate how we are using the acceleration pointers
                             VEL_RAND_PER_PLAIN, SGN_ALL_RAND, // Indicate how we are using the velocity pointer
                             BURST_CIRCULAR, CIR_ROT_X );      // Indicate any additional calculations to the vector
    }


    // Set the start and destination offsets
    sIPoint3D.X = (1 + rand() % MaxX) * offset[rand() % 2];                         
    sIPoint3D.Y = Yoffset;
    sIPoint3D.Z = Z;
    
    // Compute the slope                         
    SlopeX = X - sIPoint3D.X;                         
    SlopeY = Y - sIPoint3D.Y;

    // Calculate the animation speed depending on the 
    // distance the rocket has to travel
    if( abs(SlopeX) > abs(SlopeY) )
        AnimSpeed = abs(SlopeX) / 35;
    else
        AnimSpeed = abs(SlopeY) / 35;

    // Init the slope index
    SlopeIndex = 0;

    // Init the error flag
    error = 0;  

}   // InitBurst


/************************************************************************
*    FUNCTION NAME:         UpdateSpritePointAnim()                                                             
*
*    DESCRIPTION:           Plots the line and updates the offsets. The code
*                           below makes use of Bresenham's line algorithm.
*
*    FUNCTION PARAMETERS:
*    Input:    PSSpriteBehavior pSpriteBehavior - Pointer to the sprite's behavior
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    4/5/02        Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CFirework::UpdatePoint()
{
    int tmpSlopeX, tmpSlopeY, x_inc, y_inc, index;
    BOOL Result = TRUE;

    // Init the temporary local slope variables
    tmpSlopeX = SlopeX;
    tmpSlopeY = SlopeY;

    // Slow down the animation speed
    if( AnimSpeed > 10 )
        --AnimSpeed;

    // test the X direction of the slope
    if( tmpSlopeX >= 0 )
    {
        // line is moving right
        x_inc = 1;
    }
    else
    {
        // line is moving left
        x_inc = -1;

        // need an absolute value
        tmpSlopeX = -tmpSlopeX;
    }

    // test y direction of slope
    if( tmpSlopeY >= 0 )
    {
        // line is moving down
        y_inc = 1;
    }
    else
    {
        // line is moving up
        y_inc = -1;

        // need an absolute value
        tmpSlopeY = -tmpSlopeY;
    }

    // Based on which slope is greater we can plot the path
    if( tmpSlopeX > tmpSlopeY )
    {
        // Plot our way down the path
        for( index = 0; index < AnimSpeed; ++index )
        {
            // adjust the error factor
            error += tmpSlopeY;

            // test if error overflowed
            if( error > tmpSlopeX )
            {
                // Reset the error factor
                error -= tmpSlopeX;

                // Inc the Y movement
                sIPoint3D.Y += y_inc;
            }

            // See if we have reached the end of our path
            if( SlopeIndex >= tmpSlopeX )
            {
                // We have reached the end of our point
                Result = FALSE;

                break;
            }
            else
            {
                // Inc our slope index
                ++SlopeIndex;
            }

            // Inc the X movement
            sIPoint3D.X += x_inc;
        }
    }
    else
    {
        // Plot our way down the path
        for( index = 0; index < AnimSpeed; ++index )
        {
            // adjust the error factor
            error += tmpSlopeX;

            // test if error overflowed
            if( error > 0 )
            {
                // Reset the error factor
                error -= tmpSlopeY;

                // Inc the X movement
                sIPoint3D.X += x_inc;
            }

            // See if we have reached the end of our path
            if( SlopeIndex >= tmpSlopeY )
            {
                // We have reached the end of our point
                Result = FALSE;

                break;
            }
            else
            {
                // Inc our slope index
                ++SlopeIndex;
            }

            // Inc the Y movement
            sIPoint3D.Y += y_inc;
        }
    }

    return Result;

}   // UpdateSpritePointAnim


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

void CAnimateSaver::DisplaySoundError()
{
    if( ManySndDevices )
    {
        if( sndExplode.IsSndError() )
        {
            // Set the background mode to transparent for the text
            errorCanvas.SetBackgroundMode( TRANSPARENT );

            // Draw the text
            errorCanvas.DrawText( 5, 5, sndExplode.GetErrorStr() );

            // Set the background mode back to opaque
            errorCanvas.SetBackgroundMode( OPAQUE );
        }
        else if( sndShoot.IsSndError() )
        {
            // Set the background mode to transparent for the text
            errorCanvas.SetBackgroundMode( TRANSPARENT );

            // Draw the text
            errorCanvas.DrawText( 5, 5, sndShoot.GetErrorStr() );

            // Set the background mode back to opaque
            errorCanvas.SetBackgroundMode( OPAQUE );
        }
        else if( loopDev.IsSndError() )
        {
            // Set the background mode to transparent for the text
            errorCanvas.SetBackgroundMode( TRANSPARENT );

            // Draw the text
            errorCanvas.DrawText( 5, 5, loopDev.GetErrorStr() );

            // Set the background mode back to opaque
            errorCanvas.SetBackgroundMode( OPAQUE );
        }
        else if( croudDev.IsSndError() )
        {
            // Set the background mode to transparent for the text
            errorCanvas.SetBackgroundMode( TRANSPARENT );

            // Draw the text
            errorCanvas.DrawText( 5, 5, croudDev.GetErrorStr() );

            // Set the background mode back to opaque
            errorCanvas.SetBackgroundMode( OPAQUE );
        }
    }
    else  // We can't have multiple devices
    {       
        if( sndSimple.IsSndError() )
        {
            // Set the background mode to transparent for the text
            errorCanvas.SetBackgroundMode( TRANSPARENT );

            // Draw the text
            errorCanvas.DrawText( 5, 5, sndSimple.GetErrorStr() );

            // Set the background mode back to opaque
            errorCanvas.SetBackgroundMode( OPAQUE );
        }
    }

}   // DisplaySoundError