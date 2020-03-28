
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

// These are globals used by the 3D engine and must be setup
float gfCameraScaleX;
float gfCameraScaleY;
unsigned short int *gusiVBufer;
int giBufWidth, giBufHeight;
float gfBufWidthDiv2, gfBufHeightDiv2;
int *giZBuffer;
float gfMaxZDistance;
float gfMinZDistance;
unsigned short int *gusiShadeTbl = NULL;
float gfSquarePercentage;
float aspectRatio;
float gfFrustrumYRatio;


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
    ResLessThen640x480 = FALSE;
    renderState = FORMAT_TEXT;
    pBackground = NULL;
    CloudMaxLines = 0;
    CloudCounter = 0;

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
    int RenderWidth, RenderHeight, ScreenResID;

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

    // Make sure we don't save a render resolution higher then the screen size
    if( Settings.GetRenderRes() > ScreenResID )
        Settings.SetRenderRes( ScreenResID );

    // What size should the screen be rendered?
    if( Settings.GetUseScreenRes() )
    {
        GetWidthHeightCamera( ScreenResID, RenderWidth, RenderHeight, CameraSize );
        Settings.SetRenderRes( ScreenResID );
    }
    else
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
    CameraSize = 160;
    RnderRect = WindowRect;
    #endif

    if( Mode() == ssPreview )
    {
        RnderRect = WindowRect;
        CameraSize = 80;
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
    TBitmap ArtTmp;
    TLoadRes LoadRes;
    TBitmap tmpBackbuffer, tmpBackArt;
    char strLoad[50], upperCharStr[2], lowerCharStr[2];
    int CountLeft, CountTop, i, j, TmpCount, lightCount;
    RECT SourceRect, DestRect;
    TGif gif;
    EDyn_Light lightType;
    EShade_Type shadeType;
	DWORD color;
	WORD quoteColor;
	WORD refColor;

    // Represents these characters - !"'(),-.:;?
    int miscChar[MAX_MISC_CHAR] = { 33,34,39,40,41,44,45,46,58,59,63 };
    // Resource entries that reperesent the misc characters
    char miscStr[MAX_MISC_CHAR][20] = { "exclamation","quotes","apotrophe","parenthesis_L",
                                        "parenthesis_R","comma","dash","period","colon",
                                        "semi_colon","question_Mark" };

    // Init the classes using the screens DC. That way it doesn't have to make one
    buffer.InitActiveDC( GetScreenDC() );
    tmpBackbuffer.InitActiveDC( GetScreenDC() );
    tmpBackArt.InitActiveDC( GetScreenDC() );

    // Load the GIF file
    if( !tmpBackArt.LoadFromResource( Instance(), "clouds", "BMP" ) )
    { 
        strcpy( errorStr, "Can't load background sky GIF. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Create the bitmap
    if( !tmpBackArt.CreateBitmap( DIB_SECTION_BMP, BIT_COUNT_16 ) )
    {
        strcpy( errorStr, "Can't create temp art DIB bitmap. Your system resources may be low so try restarting your computer." );
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
    if( !tmpBackbuffer.CreateDIBSectionBuffer( RnderRect.right, CloudMaxLines, BIT_COUNT_16 ) )
    {
        strcpy( errorStr, "Can't allocate temp DIB background buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Allocate the background buffer
    pBackground = new unsigned short int[ RnderRect.right * CloudMaxLines ];
    if( !pBackground )
    {
        strcpy( errorStr, "Can't allocate scrolling background buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Reset the CloudMaxLines so that we don't over run the buffer
    CloudMaxLines -= tmpBackArt.height() * (TmpCount / 2);

    // set the starting point for the cloud scroll
    CloudCounter = CloudMaxLines - 1;

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

    // Copy the bitmap buffer to the byte buffer
    memcpy( pBackground, tmpBackbuffer.GetPtrToDIBsectionBits(), (tmpBackbuffer.width() * tmpBackbuffer.height()) * sizeof(short int) );

    // Allocate the Z Buffer
    ZBuffer = new int[ RnderRect.right * RnderRect.bottom ];
    if( !ZBuffer )
    {
        strcpy( errorStr, "Can't allocate Z buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Zero out the ZBuffer
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Allocate the shaded lookup table
    gusiShadeTbl = new unsigned short int[ SHADE_COUNT * COLOR_COUNT_16_BIT ];
    if( gusiShadeTbl == NULL )
    {
        strcpy( errorStr, "Can't allocate shaded lookup table. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Generate the 16 bit shaded table
    Generate16BitShadedTable( gusiShadeTbl );

    // Create the video buffer depending on the screen color depth
    if( !buffer.CreateDIBSectionBuffer( RnderRect.right, -RnderRect.bottom, BIT_COUNT_16 ) )
    {
        strcpy( errorStr, "Can't allocate background buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Set the global variables
    gusiVBufer = (unsigned short int *)buffer.GetPtrToDIBsectionBits();
    giZBuffer = ZBuffer;
    giBufWidth = RnderRect.right;
    giBufHeight = RnderRect.bottom;
    gfBufWidthDiv2 = RnderRect.right / 2;
    gfBufHeightDiv2 = RnderRect.bottom / 2;
    gfMaxZDistance = 500000;
    gfMinZDistance = 10;
	gfCameraScaleX = ((float)giBufWidth * 0.885);
    gfCameraScaleY = -gfCameraScaleX;
	gfSquarePercentage = (float)giBufHeight / (float)giBufWidth;
    aspectRatio = (float)giBufWidth / (float)giBufHeight;
    gfFrustrumYRatio = gfSquarePercentage / aspectRatio;

    if( Settings.GetShadeType() == EShade_FLAT )
    {
        // Set the light properties
        lightType = EDL_NO_DYNAMIC_LIGHTNING;
        shadeType = EShade_FLAT;
        lightCount = 1;
    }
    else
    {
        shadeType = EShade_GOURAUD;

        // Set the light properties
        if( Settings.GetShimmer() == TRUE )
        {
            lightType = EDL_WORLD_DYNAMIC_LIGHTNING;            
            lightCount = 2;
        }
        else
        {
            lightType = EDL_NO_DYNAMIC_LIGHTNING;
            lightCount = 1;
        }
    }

    // Make the quote light
    if( !quoteLightList.SetLightCount( lightCount ) )
    {
        strcpy( errorStr, "Can't allocate quote light list. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Create the light list
    quoteLightList.SetLightPoint( 0, 0, 300, -1000 );
    quoteLightList.SetLightRange( 0, 250, 50 );
    quoteLightList.SetLightType( 0, ELT_POINT, shadeType );
	quoteLightList.SetDynamicLightType( lightType );

    // Make the rotating lights if needed
    if( lightCount > 1 )
    {
        quoteLightList.SetLightPoint( 1, 0, 1500, -1000 );
        quoteLightList.SetLightRange( 1, 250, 20 );
        quoteLightList.SetLightType( 1, ELT_POINT, shadeType );
    }
    else
        quoteLightList.SetLightRange( 0, 250, 110 );

    // Make the ref light
    if( !refLightList.SetLightCount( 1 ) )
    {
        strcpy( errorStr, "Can't allocate ref light list. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Create the light list
    refLightList.SetLightPoint( 0, 0, 300, -1000 );
    refLightList.SetLightRange( 0, 250, 110 );
    refLightList.SetLightType( 0, ELT_POINT, shadeType );

    strcpy( upperCharStr, "A" );
    strcpy( lowerCharStr, "a" );

    color = Settings.GetQuoteColor();

    quoteColor = (((color & 0xFF0000) >> 19)) |
	             (((color & 0xFF00) >> 11) << 5) |
				 (((color & 0xFF) >> 3) << 10);

    color = Settings.GetRefColor();

    refColor = (((color & 0xFF0000) >> 19)) |
	           (((color & 0xFF00) >> 11) << 5) |
			   (((color & 0xFF) >> 3) << 10);
    
    for( i = 0; i < 26; ++i )
    {
        //////////////////////////////
        // load all the cap letters
        //////////////////////////////
        
        // Set the light list
        lettersPly[ CAP_ASCII_START + i ].SetLightList( quoteLightList );

        // Set the 16 bit color
		lettersPly[ CAP_ASCII_START + i ].SetRGBColor16( quoteColor );

        // Set the render specs
        lettersPly[ CAP_ASCII_START + i ].SetRenderSpecs( EZ_ZBUFFER_POS, ES_RGB_SHADING, ERT_SOLID_LIGHT_RENDER );

        // Create the string
        sprintf( strLoad, "%s_caps", upperCharStr );

        // Get the data from the resource
        if( !LoadTheResource( Instance(), strLoad, "POLY", &LoadRes ) )
            goto HANDLE_ERROR;

        // load the data into the polygon object
        if( !lettersPly[ CAP_ASCII_START + i ].LoadFromPointer( (BYTE *)LoadRes.pData ) )
            goto HANDLE_ERROR;


        // Set the light list
        arialPly[ CAP_ASCII_START + i ].SetLightList( refLightList );

        // Set the 16 bit color
		arialPly[ CAP_ASCII_START + i ].SetRGBColor16( refColor );

        // Set the render specs
        arialPly[ CAP_ASCII_START + i ].SetRenderSpecs( EZ_ZBUFFER_POS, ES_RGB_SHADING, ERT_SOLID_LIGHT_RENDER );

        // Create the string
        sprintf( strLoad, "%s_arial", upperCharStr );

        // Get the data from the resource
        if( !LoadTheResource( Instance(), strLoad, "POLY", &LoadRes ) )
            goto HANDLE_ERROR;

        // load the data into the polygon object
        if( !arialPly[ CAP_ASCII_START + i ].LoadFromPointer( (BYTE *)LoadRes.pData ) )
            goto HANDLE_ERROR;


        //////////////////////////////
        // load all the lower case letters
        //////////////////////////////

        // Set the light list
        lettersPly[ UNDER_ASCII_START + i ].SetLightList( quoteLightList );

        // Set the 16 bit color
		lettersPly[ UNDER_ASCII_START + i ].SetRGBColor16( quoteColor );

        // Set the render specs
        lettersPly[ UNDER_ASCII_START + i ].SetRenderSpecs( EZ_ZBUFFER_POS, ES_RGB_SHADING, ERT_SOLID_LIGHT_RENDER );

        // Create the string
        sprintf( strLoad, "%s_under", lowerCharStr );

        // Get the data from the resource
        if( !LoadTheResource( Instance(), strLoad, "POLY", &LoadRes ) )
            goto HANDLE_ERROR;

        // load the data into the polygon object
        if( !lettersPly[ UNDER_ASCII_START + i ].LoadFromPointer( (BYTE *)LoadRes.pData ) )
            goto HANDLE_ERROR;

        // Inc to the next character
        ++upperCharStr[0];
        ++lowerCharStr[0];
    }

    // Load the arial numbers
    for( i = 0; i < 10; ++i )
    {
        // Set the light list
        arialPly[ NUM_ASCII_START + i ].SetLightList( refLightList );

        // Set the 16 bit color
        arialPly[ NUM_ASCII_START + i ].SetRGBColor16( refColor );

        // Set the render specs
        arialPly[ NUM_ASCII_START + i ].SetRenderSpecs( EZ_ZBUFFER_POS, ES_RGB_SHADING, ERT_SOLID_LIGHT_RENDER );

        // Create the string
        sprintf( strLoad, "arial_%d", i );

        // Get the data from the resource
        if( !LoadTheResource( Instance(), strLoad, "POLY", &LoadRes ) )
            goto HANDLE_ERROR;

        // load the data into the polygon object
        if( !arialPly[ NUM_ASCII_START + i ].LoadFromPointer( (BYTE *)LoadRes.pData ) )
            goto HANDLE_ERROR;
    }
    
    // load all the misc characters
    for( i = 0; i < MAX_MISC_CHAR; ++i )
    {
        // Set the light list
        lettersPly[ miscChar[i] ].SetLightList( quoteLightList );

        // Set the 16 bit color
		lettersPly[ miscChar[i] ].SetRGBColor16( quoteColor );

        // Set the render specs
        lettersPly[ miscChar[i] ].SetRenderSpecs( EZ_ZBUFFER_POS, ES_RGB_SHADING, ERT_SOLID_LIGHT_RENDER );

        if( !LoadTheResource( Instance(), miscStr[i], "POLY", &LoadRes ) )
            goto HANDLE_ERROR;

        if( !lettersPly[ miscChar[i] ].LoadFromPointer( (BYTE *)LoadRes.pData ) )
            goto HANDLE_ERROR;
    }

    // Set the light list
    arialPly[ 45 ].SetLightList( refLightList );

    // Set the 16 bit color
    arialPly[ 45 ].SetRGBColor16( refColor );

    // Set the render specs
    arialPly[ 45 ].SetRenderSpecs( EZ_ZBUFFER_POS, ES_RGB_SHADING, ERT_SOLID_LIGHT_RENDER );

    if( !LoadTheResource( Instance(), "arial_dash", "POLY", &LoadRes ) )
        goto HANDLE_ERROR;

    if( !arialPly[ 45 ].LoadFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Set the light list
    arialPly[ 58 ].SetLightList( refLightList );

    // Set the 16 bit color
    arialPly[ 58 ].SetRGBColor16( refColor );

    // Set the render specs
    arialPly[ 58 ].SetRenderSpecs( EZ_ZBUFFER_POS, ES_RGB_SHADING, ERT_SOLID_LIGHT_RENDER );

    if( !LoadTheResource( Instance(), "arial_colon", "POLY", &LoadRes ) )
        goto HANDLE_ERROR;

    if( !arialPly[ 58 ].LoadFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

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
    char strTmp[50];

    // Get the number of message strings currently available
    if( !LoadString( Instance(), STR_QUOTE_COUNT, strTmp, sizeof( strTmp ) ) )
        goto HANDLE_ERROR;

    // Record the number of string quotes
    noOfQuotes = atoi( strTmp );

    #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )
    if( Mode() == ssSaver )
    {
        // Reset the display
        if( !Settings.GetUseScreenRes() && Settings.GetSizeToFit() && Settings.GetRenderRes() != GetScreenResSizeID( ScreenRect.right, ScreenRect.bottom ) )
            resetScnRes = TRUE;

        // Reset the screen res and color depth
        if( resetScnRes )
            SetScreenResolution();
    }
    #endif

    // Display a message if the computer is in 8-bit or less color
    if( SystemBitDepth < BIT_COUNT_16 )
        DisplayShareWareScreen( buffer.Handle(), buffer.width(), buffer.height(), NEED_HIGHER_COLOR_MODE, HOW_TO_CHANGE_HIGHER_COLOR, FALSE );

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
        case PLAY_MUSIC:       if( SystemBitDepth > BIT_COUNT_8 ) PlayMusic();               break;

        // Erasing the buffer
        case RENDER_BUFFER:    if( SystemBitDepth > BIT_COUNT_8 ) RenderBuffer();            break;

        // Do the shareware check
        case SHAREWARE_NOTICE: if( SystemBitDepth > BIT_COUNT_8 ) ShowShareWareScreen();     break;

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
int test = 53;
void CAnimateSaver::RenderBuffer()
{
    CMatrix matrix;
    int i, j, length;

    // Zero out the ZBuffer
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (pBackground + (RnderRect.right * CloudCounter) ), 
            (RnderRect.right * RnderRect.bottom) * sizeof(short int) );
    
    // Rotate the light
    if( Settings.GetShadeType() == EShade_GOURAUD && Settings.GetShimmer() == TRUE )
    {
        lightMatrix2.Rotate( 0, 0, 10 );
        lightMatrix2.Transform( quoteLightList.light[1].pos );
        quoteLightList.light[1].CalculateMag();
    } 

    // Dec the cloud counter
    CloudCounter -= 2;

    if( CloudCounter <= 0 )
        CloudCounter = CloudMaxLines - 1;

    // Get and format the text
    if( renderState == FORMAT_TEXT )
    {
        // Choose the rotation angle
        if( rand() % 2 )
            rotateAngle = true;
        else
            rotateAngle = false;
        
        // Get the number of message strings currently available
        //if( LoadString( Instance(), STR_QUOTE_START + test, loadQuoteStr, sizeof(loadQuoteStr) ) )
        //if( LoadString( Instance(), 381, loadQuoteStr, sizeof(loadQuoteStr) ) )
        if( LoadString( Instance(), STR_QUOTE_START + (rand() % noOfQuotes), loadQuoteStr, sizeof(loadQuoteStr) ) )
        {         
            WrapTextToBuffer( loadQuoteStr, textLines, MAX_TEXT_LINE, CHAR_LEADING );

            renderState = ANIM_BACK;
            zDist = 10.5;
            zVector = 6;
            zMaxDist = 410;
            slowDownPoint = 290;
            turn = 0.0;
            rotate = 0.0;
            endVector = 0.0;
            
            //test = (test + 1) % noOfQuotes;
            
            if( textLines.count == 3 )
            {                           
                WrapTextToBuffer( loadQuoteStr, textLines, MAX_TEXT_LINE+10, CHAR_LEADING );
                zMaxDist = 420;
                slowDownPoint = 305;            
            }
            else if( textLines.count == 4 )
            {                           
                WrapTextToBuffer( loadQuoteStr, textLines, MAX_TEXT_LINE+20, CHAR_LEADING );
                zMaxDist = 430;
                slowDownPoint = 315;            
            }
			else if( textLines.count == 5 )
            {                           
                WrapTextToBuffer( loadQuoteStr, textLines, MAX_TEXT_LINE+30, CHAR_LEADING );
                zMaxDist = 440;
                slowDownPoint = 325;            
            }
            else if( textLines.count == 6 )
            {                           
                WrapTextToBuffer( loadQuoteStr, textLines, MAX_TEXT_LINE+40, CHAR_LEADING );
                zMaxDist = 450;
                slowDownPoint = 335;            
            }
            else if( textLines.count == 7 )
            {                           
                WrapTextToBuffer( loadQuoteStr, textLines, MAX_TEXT_LINE+50, CHAR_LEADING );
                zMaxDist = 540;
                slowDownPoint = 425;            
            }
            else if( textLines.count == 8 )
            {                           
                WrapTextToBuffer( loadQuoteStr, textLines, MAX_TEXT_LINE+105, CHAR_LEADING );
                zMaxDist = 560;
                slowDownPoint = 440;
                            
            }
            else if( textLines.count == 9 || textLines.count == 10 )
            {                           
                WrapTextToBuffer( loadQuoteStr, textLines, MAX_TEXT_LINE+150, CHAR_LEADING );
                zMaxDist = 590;
                slowDownPoint = 460;
            }
            else if( textLines.count >= 11 && textLines.count <= 16 )
            {
                WrapTextToBuffer( loadQuoteStr, textLines, MAX_TEXT_LINE+250, CHAR_LEADING );
                zMaxDist = 630;
                slowDownPoint = 520;
                
                // Bump to the nex one if we have exceeded 9 lines
                if( textLines.count > 9 )
                {
                    WrapTextToBuffer( loadQuoteStr, textLines, MAX_TEXT_LINE+350, CHAR_LEADING );
                    zMaxDist = 685;
                    slowDownPoint = 580;
                }
                                                
            }
            else if( textLines.count >= 17 )
            {                           
                WrapTextToBuffer( loadQuoteStr, textLines, MAX_TEXT_LINE+350, CHAR_LEADING );
                zMaxDist = 685;
                slowDownPoint = 580;                                
            }
        }
        //else
            //test = 0;
    }

    if( renderState == ANIM_BACK )
    {   
        for( i = 0; i < textLines.count; ++i )
        {
            length = strlen( textLines.text[ i ] );

            xShift = -(textLines.length[i] / 2);
            yShift = ((textLines.count * LINE_HEIGHT) / 2) - ((i * LINE_HEIGHT) + 30); // 38

            for( j = 0; j < length; ++j )
            {
                if( textLines.text[i][j] != ' ' )
                {
                    matrix.InitilizeMatrix();
                    matrix.Translate( xShift, yShift, zDist+turn );

                    if( rotate != 0.0 )
                        matrix.Rotate( 0, rotate, 0 );

                    lettersPly[textLines.text[i][j]].Render( matrix );

                    xShift += lettersPly[textLines.text[i][j]].GetRadiusX()+CHAR_LEADING;
                }
                else
                    xShift += CHAR_SPACE;
            }
        }

            
        length = strlen( textLines.endStr );
        xShift = 0;
        yShift -= ARIAL_Y_OFFSET;

        for( i = 0; i < length; ++i )
        {
            if( textLines.endStr[i] != ' ' )
            {
                matrix.InitilizeMatrix();
                matrix.Translate( xShift, yShift, zDist+turn );

                if( rotate != 0.0 )
                    matrix.Rotate( 0, rotate, 0 );

                arialPly[ textLines.endStr[i] ].Render( matrix );

                xShift += arialPly[ textLines.endStr[i] ].GetRadiusX()+1.5;
            }
            else
                xShift += 8;
        }

        zDist += zVector;

        if( rotate > 100 || rotate < -100 )
        {
            renderState = FORMAT_TEXT;
        }
        else if( zDist > zMaxDist )
        {
            turn += endVector;

            if( rotateAngle )
                rotate += 1+endVector;
            else
                rotate -= 1+endVector;

            endVector += 0.1;
        }
        else if( zDist > slowDownPoint && zVector > 0.46 )
        {
            zVector -= 0.25;
        }           
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

    if( pBackground )
        delete [] pBackground;

    if( gusiShadeTbl )
        delete gusiShadeTbl;
            
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
*    FUNCTION NAME:         WrapTextToBuffer                                                             
*
*    DESCRIPTION:           Format text into a text array
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

void CAnimateSaver::WrapTextToBuffer( char *Text, CTextLines &lines, float TextWidth, float Leading )
{
    unsigned int CharCount, TotalCount = 0;
    int StrLength;
    char CurrWord[70], LineOfText[400], tmpLineOfText[400];
    float strSize;
    bool finished = false;

    // Init text strings
    CurrWord[0] = 0;
    LineOfText[0] = 0;
    tmpLineOfText[0] = 0;
    lines.count = 0;
    
    // Loop until we run out of letters
    while( Text[TotalCount] != 0 && !finished )
    {
        CharCount = 0;

        // Get the next word
        while( Text[TotalCount] > SPACE_CR && Text[TotalCount] < '|' && Text[TotalCount] != 0 )
        {
            CurrWord[CharCount++] = Text[TotalCount++];
        }

        // Termanate the current word
        CurrWord[CharCount] = 0;

        // Make a temporary line of text to check it's length
        lstrcpy( tmpLineOfText, LineOfText );
        lstrcat( tmpLineOfText, CurrWord );

        // Get the size of the temporary line of text
        strSize = GetTextLength( tmpLineOfText, lettersPly, Leading );
    
        // Is the temporary line of text greater than the width we're looking for?
        // If so, then print to the device the real line of text.
        if( strSize > TextWidth )
        {
            // Get the length of the string that is to be printed out
            StrLength = strlen( LineOfText );

            // Make sure we have a string to print out
            if( StrLength )
            {
                // Remove the space characer at the end of the line
                // and remove the space count from the SpaceCharCount 
                if( LineOfText[ StrLength-1 ] == SPACE_CR )
                {
                    while( LineOfText[ StrLength-1 ] == SPACE_CR )
                    {
                        --StrLength;
                    }
                    LineOfText[ StrLength ] = 0;
                }

                // Record the line of text
                strcpy( lines.text[ lines.count ], LineOfText );

                // Save the length of the scting
                lines.length[lines.count++] = GetTextLength( LineOfText, lettersPly, Leading );

                // Termanate the line of text to start a new line
                LineOfText[0] = 0;
            }
        }

        // Add the current word to the line of text
        lstrcat( LineOfText, CurrWord );

        // Process whitespace and any control characters
        while( (Text[TotalCount] == SPACE_CR || Text[TotalCount] == '|') && Text[TotalCount] != 0 && !finished )
        {
            switch( Text[TotalCount] )
            {
                case SPACE_CR:
                    // Add a space to the line of text
                    lstrcat( LineOfText, " " );
                break;

                case '|':
                    strcpy( lines.endStr, &Text[TotalCount+1] );
                    // Upper case the reference
                    CharUpper( lines.endStr );
                    finished = true;
                break;
            }

            TotalCount++;
        }
    }
    
    // We may have one last line of text to print out
    if( LineOfText[0] )
    {
        // Record the line of text
        strcpy( lines.text[ lines.count ], LineOfText );

        // Save the length of the scting
        lines.length[lines.count++] = GetTextLength( LineOfText, lettersPly, Leading );
    }

}   // WrapTextToBuffer


/************************************************************************
*    FUNCTION NAME:         GetTextLength                                                             
*
*    DESCRIPTION:           Return the length of the text string.
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

float CAnimateSaver::GetTextLength( char *text, CMeshTri *objText, float leading )
{
    float addItUp = 0.0;
    int length, i;

    length = strlen(text);

    for( i = 0; i < length; ++i )
    {
        if( i < length-1 )
        {
            if( text[i] != ' ' )
            {
                addItUp += objText[text[i]].GetRadiusX() + leading;
            }
            else
                addItUp += CHAR_SPACE;
        }
        else
        {
            if( text[i] != ' ' )
            {
                addItUp += objText[text[i]].GetRadiusX();
            }
            else
                addItUp += CHAR_SPACE;
        }
    }

    return addItUp;

}   // GetTextLength