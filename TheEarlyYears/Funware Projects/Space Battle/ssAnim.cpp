
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


int shipMoveLR[SHIP_MOVE_SIDE_TO_SIDE] = { -3, -6, -9, -12, -15, -18, -21, -24, -26, -28, 
                                        -30, -31, -32, -33, -32, -31, -30, -28, -26, -24, -21, -18, -15,
                                        -12, -9, -6, -3, 0, 3, 6, 9, 12, 15,
                                         18, 21, 24, 26, 28, 30, 31, 32, 33, 32, 31, 30, 28, 
                                         26, 24, 21, 18, 15, 12, 9, 6, 3, 0 };

int shipMoveUD[SHIP_MOVE_UP_DOWN] = { -1, -2, -3, -4, -5, -6, -5, -4, -3, -2, -1, 0,
                                       1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, 0 };

// Externed from SSConfig.cpp
extern int ResCheck[MAX_RES_CHECK];
extern int ResCheckWide[MAX_RES_CHECK];
extern int BlitToScreen[MAX_RES_CHECK];

// Turn off warnings for deprecated calls
#pragma warning(disable : 4996)

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
    animSceneState = MOTHER_SHIP_FRONT_APPROACH;
    animSceneCounter = 0;
    animSceneMaxCount = 0;
    setUpScene = TRUE;
    starBackground = NULL;
    cloudBufferOffset = 0;
    ResLessThen640x480 = FALSE;
    AttackSceneCounter = 0;
    OverviewCounter = 0;
    Rotate = 0;
    PickFightExplode = -1;
    PickTopOrBottom = -1;

    // Init non-specific class members
    AnimCounter = 0;
    saverTimer = 0;
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

    // Calculate the mamimum render size
	DoResCheck( Settings, Instance() );

    // Make sure we don't save a render resolution higher then the screen size
    if( Settings.GetRenderRes() > ScreenResID )
        Settings.SetRenderRes( ScreenResID );

    // What size should the screen be rendered?
    GetWidthHeightCamera( Settings.GetRenderRes(), RenderWidth, RenderHeight, CameraSize );

	CameraSize = ((float)RenderWidth * 0.32);

    #ifdef DEBUG
    ShareWareMode = FALSE;
    #endif

    // Set up the blit rects
    #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )

    SetRect( &RnderRect, abs(ScreenWidth - RenderWidth) / 2, 
                         abs(ScreenHeight - RenderHeight) / 2,
                         RenderWidth, RenderHeight );
    #else
    CameraSize = ((float)WindowRect.right * 0.32);
    RnderRect = WindowRect;

	//CameraSize = 100;
	//SetRect( &RnderRect, 0,0,180,135 );
    #endif

    if( Mode() == ssPreview )
    {
        RnderRect = WindowRect;
        CameraSize = 60;
    }

    // Does the user want to skip the intro?
    if( Settings.GetSkipIntro() )
        animSceneState = ATTACK_SCENES_START;

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
    TLoadRes LoadRes;
    TBitmap tmpBackbuffer, tmpBackArt;
    int CountLeft, CountTop, badPlyCount;
    RECT SourceRect, DestRect;
    short int *badPlyLst;

    // Init the classes using the screens DC. That way it doesn't have to make one
    tmpBackbuffer.InitActiveDC( GetScreenDC() );
    tmpBackArt.InitActiveDC( GetScreenDC() );
    buffer.InitActiveDC( GetScreenDC() );

    // Allocate the Z Buffer
    ZBuffer = new int[ RnderRect.right * RnderRect.bottom ];
    if( !ZBuffer )
    {
        strcpy( errorStr, "Can't allocate Z buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // One time initilization of the cos and sin tables
    // Makes no difference who calls it because the tables are static
    motherMatrix.InitCosSinTables();

    //////////////////////////////////////////////////////
    //
    //            Load Polygon Objects
    //
    //////////////////////////////////////////////////////

    // Load the attack ship polygon object from resource
    if( !LoadTheResource( Instance(), "AttackShipPly", "POLY", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the attack ship polygon object 
    if( !AttackShip.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the mother ship polygon object from resource
    if( !LoadTheResource( Instance(), "MotherShipPly", "POLY", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the mother ship polygon object 
    if( !MotherShip.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the defender ship polygon object from resource
    if( !LoadTheResource( Instance(), "DefendShipPly", "POLY", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the defender ship polygon object 
    if( !DefendShip.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the defender ship polygon laser object from resource
    if( !LoadTheResource( Instance(), "DefendLaser", "POLY", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the defender ship laser polygon object 
    if( !DefendLaser.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the attack ship polygon laser object from resource
    if( !LoadTheResource( Instance(), "AttackLaser", "POLY", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the attack ship laser polygon object 
    if( !AttackLaser.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the defender ship polygon laser object from resource
    if( !LoadTheResource( Instance(), "DefendLaserOnly", "POLY", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the defender ship laser polygon object 
    if( !DefendLaserOnly.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the attack ship polygon laser object from resource
    if( !LoadTheResource( Instance(), "AttackLaserOnly", "POLY", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the attack ship laser polygon object 
    if( !AttackLaserOnly.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the explosion polygon object from resource
    if( !LoadTheResource( Instance(), "Explosion", "POLY", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the explosion polygon object 
    if( !Explode.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;


    //////////////////////////////////////////////////////
    //
    //            Load Texture Libraries
    //
    //////////////////////////////////////////////////////

    // Load the attack ship texture library from resource
    if( !LoadTheResource( Instance(), "AttackShipBt", "BTXT", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the attack ship texture library 
    if( !AttackShipTxt.LoadFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the mother ship texture library from resource
    if( !LoadTheResource( Instance(), "MotherShipBt", "BTXT", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the mother ship texture library 
    if( !MotherShipTxt.LoadFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the defender ship texture library from resource
    if( !LoadTheResource( Instance(), "DefendShipBt", "BTXT", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the defender ship texture library 
    if( !DefendShipTxt.LoadFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the defender ship laser texture library from resource
    if( !LoadTheResource( Instance(), "DefendLaserBt", "BTXT", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the defender ship laser texture library 
    if( !DefendLaserTxt.LoadFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;

    // Load the attack ship laser texture library from resource
    if( !LoadTheResource( Instance(), "AttackLaserBt", "BTXT", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the attack ship laser texture library 
    if( !AttackLaserTxt.LoadFromPointer( (BYTE *)LoadRes.pData ) )
        goto HANDLE_ERROR;


    // Load the explosion texture library from resource
    if( !LoadTheResource( Instance(), "ExplosionBt", "BTXT", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the explosion texture library 
    if( !ExplodeTxt.LoadFromPointer( (BYTE *)LoadRes.pData ) )
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
        AttackShip.SetShadeLookUpTable( (BYTE *)LoadRes.pData );
        MotherShip.SetShadeLookUpTable( (BYTE *)LoadRes.pData );
        DefendShip.SetShadeLookUpTable( (BYTE *)LoadRes.pData );
        DefendLaser.SetShadeLookUpTable( (BYTE *)LoadRes.pData );
        DefendLaserOnly.SetShadeLookUpTable( (BYTE *)LoadRes.pData );
        AttackLaser.SetShadeLookUpTable( (BYTE *)LoadRes.pData );
        AttackLaserOnly.SetShadeLookUpTable( (BYTE *)LoadRes.pData );
        Explode.SetShadeLookUpTable( (BYTE *)LoadRes.pData );
    }
    else
    {
        if( !LoadTheResource( Instance(), "ShadedTable16bit", "STBL", &LoadRes ) )
            goto HANDLE_ERROR;

        // Load the 16 bit shaded table 
        AttackShip.SetShadeLookUpTable( (WORD *)LoadRes.pData );
        MotherShip.SetShadeLookUpTable( (WORD *)LoadRes.pData );
        DefendShip.SetShadeLookUpTable( (WORD *)LoadRes.pData );
        DefendLaser.SetShadeLookUpTable( (WORD *)LoadRes.pData );
        DefendLaserOnly.SetShadeLookUpTable( (WORD *)LoadRes.pData );
        AttackLaser.SetShadeLookUpTable( (WORD *)LoadRes.pData );
        AttackLaserOnly.SetShadeLookUpTable( (WORD *)LoadRes.pData );
        Explode.SetShadeLookUpTable( (WORD *)LoadRes.pData );
    }    

    // Create the video buffer depending on the screen color depth
    // All texture libraries use the same 8 bit palette
    if( !buffer.CreateDIBSectionBuffer( RnderRect.right, -RnderRect.bottom, SystemBitDepth, AttackShipTxt.TMap[ 0 ].Palette ) )
    {
        strcpy( errorStr, "Can't create buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Set the ZBuffer
    MotherShip.SetZBuffer( ZBuffer );
    AttackShip.SetZBuffer( ZBuffer );
    
    DefendShip.SetZBuffer( ZBuffer );
    DefendLaser.SetZBuffer( ZBuffer );
    DefendLaserOnly.SetZBuffer( ZBuffer );
    AttackLaser.SetZBuffer( ZBuffer );
    AttackLaserOnly.SetZBuffer( ZBuffer );
    Explode.SetZBuffer( ZBuffer );

    // Set the size of the rendering area
    MotherShip.SetRenderSize( RnderRect.right, RnderRect.bottom );
    AttackShip.SetRenderSize( RnderRect.right, RnderRect.bottom );   
    DefendShip.SetRenderSize( RnderRect.right, RnderRect.bottom );
    DefendLaser.SetRenderSize( RnderRect.right, RnderRect.bottom );
    DefendLaserOnly.SetRenderSize( RnderRect.right, RnderRect.bottom );
    AttackLaser.SetRenderSize( RnderRect.right, RnderRect.bottom );
    AttackLaserOnly.SetRenderSize( RnderRect.right, RnderRect.bottom );
    Explode.SetRenderSize( RnderRect.right, RnderRect.bottom );

    // Set the maxium Z and Light distance
    MotherShip.SetMaxZDistance( 400000 );
    MotherShip.SetMaxLightDistance( 900000 );

    AttackShip.SetMaxZDistance( 400000 );
    AttackShip.SetMaxLightDistance( 900000 );
    DefendShip.SetMaxZDistance( 400000 );
    DefendShip.SetMaxLightDistance( 900000 );

    DefendLaser.SetMaxZDistance( 400000 );
    DefendLaser.SetMaxLightDistance( 900000 );
    DefendLaserOnly.SetMaxZDistance( 400000 );
    DefendLaserOnly.SetMaxLightDistance( 900000 );
    AttackLaser.SetMaxZDistance( 400000 );
    AttackLaser.SetMaxLightDistance( 900000 );
    AttackLaserOnly.SetMaxZDistance( 400000 );
    AttackLaserOnly.SetMaxLightDistance( 900000 );
    Explode.SetMaxZDistance( 400000 );
    Explode.SetMaxLightDistance( 900000 );

    // Set the camera scale
    AttackShip.SetCameraScale( CameraSize );
    MotherShip.SetCameraScale( CameraSize );
    DefendShip.SetCameraScale( CameraSize );
    DefendLaser.SetCameraScale( CameraSize );
    DefendLaserOnly.SetCameraScale( CameraSize );
    AttackLaser.SetCameraScale( CameraSize );
    AttackLaserOnly.SetCameraScale( CameraSize );
    Explode.SetCameraScale( CameraSize );

    // Set the video buffer
    AttackShip.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );
    MotherShip.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );
    DefendShip.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );
    DefendLaser.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );
    DefendLaserOnly.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );
    AttackLaser.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );
    AttackLaserOnly.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );
    Explode.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );

    // Set the texture libraries
    AttackShip.SetTextures( &AttackShipTxt );
    MotherShip.SetTextures( &MotherShipTxt );
    DefendShip.SetTextures( &DefendShipTxt );
    DefendLaser.SetTextures( &DefendLaserTxt );
    DefendLaserOnly.SetTextures( &DefendLaserTxt );
    AttackLaser.SetTextures( &AttackLaserTxt );
    AttackLaserOnly.SetTextures( &AttackLaserTxt );
    Explode.SetTextures( &ExplodeTxt );


    //////////////////////////////////////////////////////
    //
    //   Load Texture Coordinates
    //
    //   Loading the coordinates inits all the polygons
    //   in the polygon object file which is why it's the
    //   last item to load.
    //
    //////////////////////////////////////////////////////

    // Load the attack ship texture coordinates from resource
    if( !LoadTheResource( Instance(), "AttackShipCrd", "CRD", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the attack ship texture coordinates 
    AttackShip.LoadCrdFromPointer( (BYTE *)LoadRes.pData );

    // Load the mother ship texture coordinates from resource
    if( !LoadTheResource( Instance(), "MotherShipCrd", "CRD", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the mother ship texture coordinates 
    MotherShip.LoadCrdFromPointer( (BYTE *)LoadRes.pData );

    // Load the defender ship texture coordinates from resource
    if( !LoadTheResource( Instance(), "DefendShipCrd", "CRD", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the defender ship texture coordinates 
    DefendShip.LoadCrdFromPointer( (BYTE *)LoadRes.pData );

    // Load the defender ship laser texture coordinates from resource
    if( !LoadTheResource( Instance(), "DefendLaserCrd", "CRD", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the defender ship laser texture coordinates 
    DefendLaser.LoadCrdFromPointer( (BYTE *)LoadRes.pData );

    // Load the attack ship laser texture coordinates from resource
    if( !LoadTheResource( Instance(), "AttackLaserCrd", "CRD", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the attack ship laser texture coordinates 
    AttackLaser.LoadCrdFromPointer( (BYTE *)LoadRes.pData );

    // Load the defender ship laser texture coordinates from resource
    if( !LoadTheResource( Instance(), "DefendLaserOnlyCrd", "CRD", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the defender ship laser texture coordinates 
    DefendLaserOnly.LoadCrdFromPointer( (BYTE *)LoadRes.pData );

    // Load the attack ship laser texture coordinates from resource
    if( !LoadTheResource( Instance(), "AttackLaserOnlyCrd", "CRD", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the attack ship laser texture coordinates 
    AttackLaserOnly.LoadCrdFromPointer( (BYTE *)LoadRes.pData );

    // Load the explosion texture coordinates from resource
    if( !LoadTheResource( Instance(), "ExplosionCrd", "CRD", &LoadRes ) )
        goto HANDLE_ERROR;

    // Load the explosion texture coordinates 
    Explode.LoadCrdFromPointer( (BYTE *)LoadRes.pData );


    // Load the bad polygon mothership list
    if( !LoadTheResource( Instance(), "BadMotherPlyLst", "BIN", &LoadRes ) )
        goto HANDLE_ERROR;

    // These are short int's so type cast
    badPlyLst = (short int *)LoadRes.pData;

    // Calculate the size.
    badPlyCount = LoadRes.Size / 2;

    // Set the polys that need to be checked.
    // The mother ship has polygons that are too big and also may not be plainer.
    // This can cause the engine to try to render a pixel outside the textures bouds.
    // A list has been loaded with all the known bad polys so that they are the only ones
    // being checked. It faster then having to check all of them.
    // See IDENTIFY_BAD_POLYS #define for how this list was made
    for( int i = 0; i < badPlyCount; ++i )
    {
        // Check just to make sure
        if( badPlyLst[i] < (int)MotherShip.GetTotalPolyCount() )
            MotherShip.GetPoly( badPlyLst[i] )->SetCheckBounds( TRUE );
    }

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
    {
        strcpy( errorStr, "Can't create temp art back buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Create a tempoary buffer depending on the screen color depth
    if( !tmpBackbuffer.CreateDIBSectionBuffer( RnderRect.right, (RnderRect.bottom * 2), SystemBitDepth, AttackShipTxt.TMap[ 0 ].Palette ) )
    {
        strcpy( errorStr, "Can't create temp back buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Allocate the star background buffer
    starBackground = new BYTE[ (RnderRect.right * (RnderRect.bottom * 2)) * (SystemBitDepth / 8) ];
    if( !starBackground )
    {
        strcpy( errorStr, "Can't allocate byte buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

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
    for( int j = 0; j < CountTop; ++j )
    {
        for( int i = 0; i < CountLeft; ++i )
        {        
            SetRect( &DestRect, i * tmpBackArt.width(), j * tmpBackArt.height(), 
                                tmpBackArt.width(), tmpBackArt.height() );
            tmpBackArt.Draw( &tmpBackbuffer, &DestRect, &SourceRect );
        }
    }

    // Clear out the video buffer
    memcpy( starBackground, tmpBackbuffer.GetPtrToDIBsectionBits(), (RnderRect.right * (RnderRect.bottom * 2)) * (SystemBitDepth / 8) );

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
    BOOL resetScnRes = FALSE;

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

    // Setup the int list for the attack scenes
    for( int i = ATTACK_SCENES_START; i < MAX_ANIMATED_SCENES; ++i )
        AttackList.Add( i );

    // Setup the int list for the general fly over scenes
    for( int i = 0; i < MAX_OVERVIEW_ANIM; ++i )
        OverviewList.Add( i );

    // Now mix up all the ints
    AttackList.MixUp();
    OverviewList.MixUp();

    return true;

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
            if( AnimCounter == 0 && saverTimer < timeGetTime() )
            {
                Animate();

                saverTimer = timeGetTime() + MAX_MIL_RENDER_WAIT;
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

        // Do we play the included theme music?
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
//int counter = 0;
void CAnimateSaver::BlitFromBufferToScreen()
{
    if( Mode() == ssSaver )
    {
        #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )

        /*if( Settings.GetSizeToFit() && !ResLessThen640x480 )
            buffer.Draw( GetScreenDC() );
        else*/
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
	/*
	TDIB tmpDib;
	char tmpTxt[100];
	tmpDib.CreateDib( buffer.width(), -buffer.height(), 16 );
	memcpy( tmpDib.bits(), buffer.GetPtrToDIBsectionBits(), (buffer.width() * 2) * buffer.height() );

	if( counter < 10 )
		sprintf( tmpTxt, "C:\\files\\grab_00%u.bmp", counter++ );
	else if( counter < 100 )
		sprintf( tmpTxt, "C:\\files\\grab_0%u.bmp", counter++ );
	else if( counter < 1000 )
		sprintf( tmpTxt, "C:\\files\\grab_%u.bmp", counter++ );

	tmpDib.SaveToFile( tmpTxt );
	*/
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
    if( ZBuffer )
        delete [] ZBuffer;

    if( starBackground )
        delete [] starBackground;

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
    switch( animSceneState )
    {
        case MOTHER_SHIP_FRONT_APPROACH:
            Scene_FrontMotherShipApproach();
        break;
        
        case MOTHER_SHIP_BELLEY_PAN:
            Scene_UnderMotherShipPan();
        break;

        case MOTHER_SHIP_SIDE_CAMERA_PAN:
            Scene_SideMotherShipPan();
        break;

        case MOTHER_SHIP_TOP_CAMERA_PAN:
            Scene_TopMotherShipPan();
        break;

        case MOTHER_SHIP_BACK_CAMERA_PAN:
            Scene_BackMotherShipPan();
        break;

        case ATTACK_SHIP_FLY_BY:
            Scene_AttackShipFlyBy();
        break;

        case APPROACH_MOTHER_SHIP:
            Scene_ApproachMotherShip();
        break;

        case SWARM_MOTHER_SHIP:
            Scene_SwarmMotherShip();
        break;

        case DEFENDER_SHIP_EMERGE:
            Scene_DefenderShipEmerge();
        break;

        case OVER_VIEW_OF_ATTACK:
        case OVER_VIEW_OF_ATTACK_2:
            Scene_OverViewAttack();
        break;

        case ATTACK_FLY_TOP_BOTTOM:
            // Pict if we want to see the top or the bottom flight
            if( PickTopOrBottom == -1 )
                PickTopOrBottom = rand() % 2;

            // Did we pick if we are to fight or explode?
            if( PickFightExplode == -1 )
                PickFightExplode = rand() % 2;

            // Animate the top or the bottom
            if( PickTopOrBottom )
            {
                if( PickFightExplode )
                    Scene_AttackFlyTop();
                else
                    Scene_FlyTopExplode();
            }
            else
            {
                if( PickFightExplode )
                    Scene_AttackFlyBottom();
                else
                    Scene_FlyBottomExplode();
            }
        break;

        case ATTACK_FLY_SIDE:
            // Did we pick if we are to fight or explode?
            if( PickFightExplode == -1 )
                PickFightExplode = rand() % 2;

            if( PickFightExplode )
                Scene_AttackFlySide();
            else
                Scene_FlySideExplode();
        break;

        case CLOSEUP_FLY_OVER:
            Scene_CloseUpFlyOver();
        break;
    }

}   // RenderBuffer


/************************************************************************
*    FUNCTION NAME:         Scene_FrontMotherShipApproach                                                             
*
*    DESCRIPTION:           Front approach of the mother ship
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

void CAnimateSaver::Scene_FrontMotherShipApproach()
{
    Matrix3D FinalMatrix;

    // Do scene start initilizations
    if( setUpScene )
    {
        // Init the starting position to the matrix
        motherMatrix.InitilizeMatrix();

        // Have the approach face one way or the other
        if( rand() % 2 )
        {
            motherMatrix.Translate( -5200, 8300, -46000 );
            motherMatrix.Rotate( 83, -544, 0 );
        }
        else
        {
            motherMatrix.Translate( 5200, 8300, -46000 );
            motherMatrix.Rotate( 83, 544, 0 );
        }

        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 130;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Z axis move counter
        Z_Move = 0;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (starBackground + cloudBufferOffset), 
            (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Move the mother ship's Z access
    Z_Move += 150;

    // Move the mother ship's Z access
    FinalMatrix.Translate( 0, 0, Z_Move );
    FinalMatrix.MergeMatrix( motherMatrix.Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        MotherShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        MotherShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = MOTHER_SHIP_BELLEY_PAN;

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_FrontMotherShipApproach


/************************************************************************
*    FUNCTION NAME:         Scene_UnderMotherShipPan                                                             
*
*    DESCRIPTION:           Camera pan under the mother ship
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

void CAnimateSaver::Scene_UnderMotherShipPan()
{
    Matrix3D FinalMatrix;

    // Do scene start initilizations
    if( setUpScene )
    {
        // Init the starting position to the matrix
        motherMatrix.InitilizeMatrix();
        motherMatrix.Translate( 0, 4400, -10500 );
        motherMatrix.Rotate( 80, 0, 0 );

        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 200;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Z axis move counter
        Z_Move = 0;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (starBackground + cloudBufferOffset), 
            (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Move the mother ship's Z access
    Z_Move += 50;

    // Move the mother ship's Z access
    FinalMatrix.Translate( 0, 0, Z_Move );
    FinalMatrix.MergeMatrix( motherMatrix.Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        MotherShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        MotherShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = MOTHER_SHIP_SIDE_CAMERA_PAN;

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_UnderMotherShipPan


/************************************************************************
*    FUNCTION NAME:         Scene_SideMotherShipPan                                                             
*
*    DESCRIPTION:           Camera pan side the mother ship
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

void CAnimateSaver::Scene_SideMotherShipPan()
{
    Matrix3D FinalMatrix;

    // Do scene start initilizations
    if( setUpScene )
    {
        // Init the starting position to the matrix
        motherMatrix.InitilizeMatrix();

        // Pick to show one side or the other
        if( rand() % 2 )
        {
            motherMatrix.Translate( 8000, -600, 3800 );
            motherMatrix.Rotate( 0, -250, 0 );
        }
        else
        {
            motherMatrix.Translate( -8000, -600, 3800 );
            motherMatrix.Rotate( 0, 250, 0 );
        }

        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 150;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Z axis move counter
        Z_Move = 0;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (starBackground + cloudBufferOffset), 
            (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Move the mother ship's Z access
    Z_Move += 70;

    // Move the mother ship's Z access
    FinalMatrix.Translate( 0, 0, Z_Move );
    FinalMatrix.MergeMatrix( motherMatrix.Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        MotherShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        MotherShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = ATTACK_SHIP_FLY_BY;

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_UnderMotherShipPan


/************************************************************************
*    FUNCTION NAME:         Scene_TopMotherShipPan                                                             
*
*    DESCRIPTION:           Camera pan top of the mother ship
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

void CAnimateSaver::Scene_TopMotherShipPan()
{
    Matrix3D FinalMatrix;

    // Do scene start initilizations
    if( setUpScene )
    {
        // Init the starting position to the matrix
        motherMatrix.InitilizeMatrix();
        motherMatrix.Translate( -70, -7000, -20910 );
        motherMatrix.Rotate( -95, 512, 0 );

        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 130;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Z axis move counter
        Z_Move = 0;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (starBackground + cloudBufferOffset), 
            (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Move the mother ship's Z access
    Z_Move += 70;

    // Move the mother ship's Z access
    FinalMatrix.Translate( 0, 0, Z_Move );
    FinalMatrix.MergeMatrix( motherMatrix.Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        MotherShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        MotherShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = MOTHER_SHIP_BACK_CAMERA_PAN;

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_TopMotherShipPan


/************************************************************************
*    FUNCTION NAME:         Scene_BackMotherShipPan                                                             
*
*    DESCRIPTION:           Camera pan back of the mother ship
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

void CAnimateSaver::Scene_BackMotherShipPan()
{
    Matrix3D FinalMatrix;

    // Do scene start initilizations
    if( setUpScene )
    {
        // Init the starting position to the matrix
        motherMatrix.InitilizeMatrix();
        motherMatrix.Translate( 550, 2360, 26000 );
        motherMatrix.Rotate( 27, 0, 0 );

        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 80;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Z axis move counter
        Z_Move = 0;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (starBackground + cloudBufferOffset), 
            (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Move the mother ship's Z access
    Z_Move += 50;

    // Move the mother ship's Z access
    FinalMatrix.Translate( 0, 0, Z_Move );
    FinalMatrix.MergeMatrix( motherMatrix.Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        MotherShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        MotherShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = APPROACH_MOTHER_SHIP;

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_BackMotherShipPan


/************************************************************************
*    FUNCTION NAME:         Scene_AttackShipFlyBy                                                             
*
*    DESCRIPTION:           Atack ship fly by
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

void CAnimateSaver::Scene_AttackShipFlyBy()
{
    Matrix3D FinalMatrix;
    int i;
    float XOffset;

    // Do scene start initilizations
    if( setUpScene )
    {
        const int cameraYTran = -700;
        const int cameraZTran = -3000;
        const int cameraXRot  = -17;
        int cameraXTran;
        int cameraYRot;

        // Randomly pick to view from one side or the other
        X_Move = rand() % 2;
        
        if( X_Move )
        {
            cameraXTran = -1200;
            cameraYRot  = 337;
        }
        else
        {
            cameraXTran = 1200;
            cameraYRot  = -330;
        }

        // Init the starting position to the matrix
        for( i = 0; i < MAX_ATTACK_SHIPS; ++i )
        {
            attackMatrix[i].InitilizeMatrix();
            if( X_Move )
                attackMatrix[i].Translate( cameraXTran + rand() % 800, cameraYTran + rand() % 1000, cameraZTran + (i * -1300) + -(rand() % 200) );
            else
                attackMatrix[i].Translate( cameraXTran + -(rand() % 800), cameraYTran + rand() % 1000, cameraZTran + (i * -1300) + -(rand() % 200) );
            attackMatrix[i].Rotate( cameraXRot, cameraYRot, 0 );
        }

        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 170;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Z axis move counter
        Z_Move = 0;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (starBackground + cloudBufferOffset), 
            (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Move the attack ship's Z access
    Z_Move += 250;

    for( i = 0; i < MAX_ATTACK_SHIPS; ++i )
    {
        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the mother ship's Z access
        FinalMatrix.Translate( 0, 0, Z_Move );
        FinalMatrix.MergeMatrix( attackMatrix[i].Matrix );

        // Clip the ships that are not even seen yet.
        XOffset = FinalMatrix.GetX();

        if( X_Move )
        {
            if( !(XOffset > -8000.0F && XOffset < 2000.0F) )
                continue;
        }
        else
        {
            if( !(XOffset > -2000.0F && XOffset < 8000.0F) )
                continue;
        }

        // Paint the buffer
        if( SystemBitDepth == 8 )
            AttackShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            AttackShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
    }

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = MOTHER_SHIP_TOP_CAMERA_PAN;

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_BackMotherShipPan


/************************************************************************
*    FUNCTION NAME:         Scene_ApproachMotherShip                                                             
*
*    DESCRIPTION:           Mother ship can be see way off in the distance
*                           while the attacking ships approach
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

void CAnimateSaver::Scene_ApproachMotherShip()
{
    Matrix3D FinalMatrix;
    int i, X, Y, Z;
    float Offset;

    // Do scene start initilizations
    if( setUpScene )
    {
        // Init the starting position to the matrix
        motherMatrix.InitilizeMatrix();
        motherMatrix.Translate( -18400, -52900, 205000 ); // 205000
        motherMatrix.Rotate( 25, 26, 0 );

        // Init the starting position to the matrix
        for( i = 0; i < MAX_ATTACK_SHIPS; ++i )
        {
            attackMatrix[i].InitilizeMatrix();
            X = -1000 + rand() % 2000;
            Y = -500 + rand() % 1000;
            Z = (i * -1000) + -(rand() % 200);

            // If the ship is too much in the center, push it off to the side
            if( X > -OFFSET_FROM_CAMERA && X < OFFSET_FROM_CAMERA && Y > -OFFSET_FROM_CAMERA && Y < OFFSET_FROM_CAMERA )
            {
                if( X > Y )
                    Y = OFFSET_FROM_CAMERA;
                else
                    X = OFFSET_FROM_CAMERA;
            }

            attackMatrix[i].Translate( X, Y, 10000 + Z );
            attackMatrix[i].Rotate( 36, 18, 0 );
        }

        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 100;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Z axis move counter
        Z_Move = 0;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (starBackground + cloudBufferOffset), 
            (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Move the mother ship's Z access
    Z_Move += 200;

    for( i = 0; i < MAX_ATTACK_SHIPS; ++i )
    {
        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the mother ship's Z access
        FinalMatrix.Translate( 0, 0, Z_Move );
        FinalMatrix.MergeMatrix( attackMatrix[i].Matrix );

        // Clip the Z offset on the ship level
        Offset = FinalMatrix.GetZ();

        if( !(Offset > 0.0F && Offset < 30000.0F) )
            continue;

        // Paint the buffer
        if( SystemBitDepth == 8 )
            AttackShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            AttackShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
    }

    // Reinit the matrix
    FinalMatrix.InitilizeMatrix();

    // Move the mother ship's Z access
    FinalMatrix.Translate( 0, 0, Z_Move );
    FinalMatrix.MergeMatrix( motherMatrix.Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        MotherShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        MotherShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = SWARM_MOTHER_SHIP;

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_BackMotherShipPan


/************************************************************************
*    FUNCTION NAME:         Scene_SwarmMotherShip                                                             
*
*    DESCRIPTION:           The attacking ships swarm the mother ships.
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

void CAnimateSaver::Scene_SwarmMotherShip()
{
    Matrix3D FinalMatrix;
    int i, X, Y, Z;
    float XOffset;

    // Do scene start initilizations
    if( setUpScene )
    {
        // Init the starting position to the matrix
        motherMatrix.InitilizeMatrix();
        motherMatrix.Translate( -10600, -9800, -13520 );
        motherMatrix.Rotate( -120, 260, 0 );

        // Init the starting position to the matrix
        for( i = 0; i < MAX_ATTACK_SHIPS; ++i )
        {
            attackMatrix[i].InitilizeMatrix();

            // Randomize the X, Y, Z offsets
            if( rand() % 2 )
                X = 4500 + rand() % 4000;
            else
                // Place the ships in the back at a fixed X
                X = -3000;

            Y = 3000 + rand() % 6000;

            Z = (i * -2000) + -(rand() % 500);

            attackMatrix[i].Translate( -10600 + X, -9800 + Y, -40520 + Z );
            attackMatrix[i].Rotate( -120, 260, 0 );
        }
        
        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 180;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Z axis move counter
        Z_Move = 0;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (starBackground + cloudBufferOffset), 
            (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Move the ship's Z access
    Z_Move += 150;

    // Reinit the matrix
    FinalMatrix.InitilizeMatrix();

    // Move the mother ship's Z access
    FinalMatrix.Translate( 0, 0, Z_Move );
    FinalMatrix.MergeMatrix( motherMatrix.Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        MotherShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        MotherShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    for( i = 0; i < MAX_ATTACK_SHIPS; ++i )
    {
        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the mother ship's Z access
        FinalMatrix.Translate( 0, 0, Z_Move * 3 );
        FinalMatrix.MergeMatrix( attackMatrix[i].Matrix );

        XOffset = FinalMatrix.GetX();

        if( !(XOffset > -20000.0F && XOffset < 20000.0F) )
            continue;

        // Paint the buffer
        if( SystemBitDepth == 8 )
            AttackShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            AttackShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
    }

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = DEFENDER_SHIP_EMERGE;

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_SwarmMotherShip


/************************************************************************
*    FUNCTION NAME:         Scene_DefenderShipEmerge
*
*    DESCRIPTION:           The emerging of the defender ships.
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

void CAnimateSaver::Scene_DefenderShipEmerge()
{
    Matrix3D FinalMatrix;
    int bufOffset, i, Offset;
    float XOffset;

    // Do scene start initilizations
    if( setUpScene )
    {
        int X, Y, Z;
        const int cameraXTran = -4900, cameraYTran = 4040, cameraZTran = -10540;
        const int cameraXRot = 70, cameraYRot = -732;

        // Init the starting position to the matrix
        motherMatrix.InitilizeMatrix();
        motherMatrix.Translate( cameraXTran, cameraYTran, cameraZTran );
        motherMatrix.Rotate( cameraXRot, cameraYRot, 0 );

        // Init the starting position to the matrix
        for( i = 0; i < MAX_EMERGE_SHIPS; ++i )
        {
            defendMatrix[i].InitilizeMatrix();

            // Randomize the X, Y, Z offset
            X = 942 + rand() % 1350;
            //X = 1442 + rand() % 850;
            Y = -3158 + (i * 500) + rand() % 100;
            Z = 3500 + rand() % 1950;

            defendMatrix[i].Translate( cameraXTran + X, cameraYTran + Y, cameraZTran + Z );
            defendMatrix[i].Rotate( cameraXRot, cameraYRot, 0 );

            // Set the random move array
            MoveAry1[i] = -800 + -(rand() % 700);

            // zero out the srray
            X_MoveAry[i] = 0;
            Y_MoveAry[i] = 0;
            Z_MoveAry[i] = 0;
        }

        // Init the starting position to the matrix
        for( i = 0; i < HALF_ATTACK_SHIPS; ++i )
        {
            attackMatrix[i].InitilizeMatrix();

            // Randomize the X, Y, Z offset
            X = 4150 + rand() % 400;
            Y = -2800 + -(rand() % 1200);
            Z = 9900 + (i * -3200);

            attackMatrix[i].Translate( cameraXTran + X, cameraYTran + Y, cameraZTran + Z );
            attackMatrix[i].Rotate( cameraXRot, cameraYRot, 0 );
        }
        
        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 120;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Y, Z axis move counter
        Y_Move = 0;
        Z_Move = 0;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    // Calculate an offset because we don't need to clear the whole buffer
    bufOffset = (RnderRect.right * (RnderRect.bottom / 2)) * (SystemBitDepth / 8);

    // Clear out the video buffer with the cloud buffer
    memcpy( (BYTE *)buffer.GetPtrToDIBsectionBits() + bufOffset, 
            (starBackground + cloudBufferOffset), bufOffset );

    // Move the Z access
    Z_Move += 70;

    // Move the attack ships
    for( i = 0; i < HALF_ATTACK_SHIPS; ++i )
    {
        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the mother ship's Z access
        FinalMatrix.Translate( 0, 0, Z_Move * 4 );
        FinalMatrix.MergeMatrix( attackMatrix[i].Matrix );

        XOffset = FinalMatrix.GetX();

        if( !(XOffset > -3000.0F && XOffset < 3000.0F) )
            continue;

        // Paint the buffer
        if( SystemBitDepth == 8 )
            AttackShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            AttackShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
    }

    // Reinit the matrix
    FinalMatrix.InitilizeMatrix();

    // Move the mother ship's Z access
    FinalMatrix.Translate( 0, 0, Z_Move );
    FinalMatrix.MergeMatrix( motherMatrix.Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        MotherShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        MotherShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Move the defender ships
    for( i = 0; i < MAX_EMERGE_SHIPS; ++i )
    {
        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the mother ship's Z access
        FinalMatrix.Translate( X_MoveAry[i], Y_MoveAry[i], Z_Move + Z_MoveAry[i] );
        FinalMatrix.MergeMatrix( defendMatrix[i].Matrix );

        // Calculate an offset
        Offset = i * 500;

        // When the Y reaches a certian point, move the Z and decrease the Y desent
        if( Offset + Y_MoveAry[i] < MoveAry1[i]  )
        {
            Z_MoveAry[i] += 200;
            Y_MoveAry[i] += -30;
            X_MoveAry[i] += 100;
        }
        else
        {
            Y_MoveAry[i] += -90;
        }

        // Clip the whole ship if it is too far up to see.
        if( FinalMatrix.GetX() > 4000.0F || Offset + Y_MoveAry[i] > 0 )
            continue;

        // Paint the buffer
        if( SystemBitDepth == 8 )
            DefendShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            DefendShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
    }

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = AttackList.GetInt( AttackSceneCounter );

        // Inc to the next attack scene
        AttackSceneCounter = ( AttackSceneCounter + 1 ) % AttackList.Count();

        // Now mix up all the ints
        if( AttackSceneCounter == 0 )
            AttackList.MixUp();

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_DefenderShipEmerge


/************************************************************************
*    FUNCTION NAME:         Scene_AttackFlyTop                                                             
*
*    DESCRIPTION:           Camera top fly by
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

void CAnimateSaver::Scene_AttackFlyTop()
{
    Matrix3D FinalMatrix;

    // Do scene start initilizations
    if( setUpScene )
    {
        int Y, Index;
        int cameraXTran[MAX_FLYBY_VIEWS] = {0,350,-350,500,232};
        const int cameraYTran[MAX_FLYBY_VIEWS] = {-5698,-5789,-5789,-5301,-6200};
        int cameraZTran[MAX_FLYBY_VIEWS] = {14400,11750,20000,20000,11200};
        const int cameraXRot[MAX_FLYBY_VIEWS] = {-120,-174,-151,-59,-244};
        int cameraYRot[MAX_FLYBY_VIEWS] = {0,-129,116,600,510};
        int shipZOffset[MAX_FLYBY_VIEWS] = {500,-100,100,-2000,-800};

        // Pick the camers angle
        Index = rand() % MAX_FLYBY_VIEWS;

        // Choose the movement direction
        if( rand() % 2 )    // Face the nose of the mother ship
        {
            Movement = 1;

            if( rand() % 2 )  // Going to noze
            {
                MoveAmount = -600;
                Direction = 1;

                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( Index == 2 )
                    cameraZTran[Index] = 12000;
                else if( Index == 3 )
                    cameraZTran[Index] = 10000;
            }
            else     // Going away from noze
            {
                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( Index == 0 )
                    cameraZTran[Index] = 19000;
                else if( Index == 1 )
                    cameraZTran[Index] = 20000;
                else if( Index == 4 )
                    cameraZTran[Index] = 21000;

                Direction = -1;
                MoveAmount = 600;
                cameraZTran[Index] *= -1;
            }
        }
        else                // Face the back of the mother ship
        {
            Movement = -1;
            
            if( rand() % 2 )       // Going to back
            {
                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( Index == 0 )
                    cameraZTran[Index] = 27000;
                else if( Index == 1 )
                    cameraZTran[Index] = 24000;
                else if( Index == 2 )
                    cameraZTran[Index] = 27000;
                else if( Index == 4 )
                    cameraZTran[Index] = 21000;

                Direction = -1;
                MoveAmount = 600;
                cameraXTran[Index] *= -1;
                cameraZTran[Index] *= -1;
                cameraYRot[Index] += 512;
            }
            else      // Going away from back
            {
                Direction = 1;
                MoveAmount = -600;
                cameraYRot[Index] += 512;
                cameraXTran[Index] *= -1;

                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( Index == 0 || Index == 1 )
                    cameraZTran[Index] = 8000;
                else if( Index == 2 )
                    cameraZTran[Index] = 1000;
                else if( Index == 3 )
                    cameraZTran[Index] = 12500;
            }
        }

        // Init the starting position to the matrix
        motherMatrix.InitilizeMatrix();
        motherMatrix.Translate( cameraXTran[Index], cameraYTran[Index], cameraZTran[Index] );
        motherMatrix.Rotate( cameraXRot[Index], cameraYRot[Index], 0 );

        // Init the starting position to the matrix
        attackMatrix[0].InitilizeMatrix();
        defendMatrix[0].InitilizeMatrix();
        LaserCounter[0] = 0;

        // rotate the ship if we are going in the other direction
        if( Direction == -1 )
        {
            attackMatrix[0].Rotate( 0, 512, 0 );                
            defendMatrix[0].Rotate( 0, 512, 0 );
        }

        // Randomize the Y offset
        Y = 5075;

        // Pick which will be the aggressive ship
        if( rand() % 2 )
        {
            Z_MoveAry[0] = shipZOffset[Index] * Movement;
            Z_MoveAry[1] = (shipZOffset[Index] + 1310) * Movement;
        }
        else
        {
            Z_MoveAry[0] = (shipZOffset[Index] + 1310) * Movement;
            Z_MoveAry[1] = shipZOffset[Index] * Movement;
        }

        attackMatrix[0].Translate( cameraXTran[Index], cameraYTran[Index] + Y, Z_MoveAry[0] );
        attackMatrix[0].Rotate( cameraXRot[Index], cameraYRot[Index], 0 );

        defendMatrix[0].Translate( cameraXTran[Index], cameraYTran[Index] + Y, Z_MoveAry[1] );
        defendMatrix[0].Rotate( cameraXRot[Index], cameraYRot[Index], 0 );
        
        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 50;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Z axis move counter
        Z_Move = 0;
        Move_1 = 0;
        Move_2 = SHIP_MOVE_SIDE_TO_SIDE / 2;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (starBackground + cloudBufferOffset), 
            (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Move the mother ship's Z access
    Z_Move += MoveAmount;

    // Move the mother ship's Z access
    FinalMatrix.Translate( shipMoveLR[Move_1]*6, shipMoveLR[Move_1], 0 );
    FinalMatrix.Rotate( 0, 0, shipMoveLR[Move_1] );
    FinalMatrix.MergeMatrix( attackMatrix[0].Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        AttackShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        AttackShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Reinit the matrix
    FinalMatrix.InitilizeMatrix();

    // Move the mother ship's Z access
    FinalMatrix.Translate( shipMoveLR[Move_2]*5, shipMoveLR[Move_2], 0 );
    FinalMatrix.Rotate( 0, 0, shipMoveLR[Move_2] );
    FinalMatrix.MergeMatrix( defendMatrix[0].Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        DefendShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        DefendShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Do we show the laser blast
    if( LaserCounter[0] == 0 && (rand() % 3) )
    {
        ++LaserCounter[0];

        // Reinit the laser
        laserMatrix[0].InitilizeMatrix();

        // Who is the aggresser
        if( (Direction == 1 && Z_MoveAry[0] > Z_MoveAry[1]) ||
            (Direction == -1 && Z_MoveAry[0] < Z_MoveAry[1]) )
        {
            laserMatrix[0].Translate( shipMoveLR[Move_2]*5, shipMoveLR[Move_2], 0 );
            laserMatrix[0].Rotate( 0, 0, shipMoveLR[Move_2] );
            laserMatrix[0].MergeMatrix( defendMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                DefendLaser.PaintVBuffer8To8( laserMatrix[0], POS_ZBUFFER );
            else
                DefendLaser.PaintVBuffer8To16( laserMatrix[0], POS_ZBUFFER );
        }
        else
        {
            laserMatrix[0].Translate( shipMoveLR[Move_1]*6, shipMoveLR[Move_1], 0 );
            laserMatrix[0].Rotate( 0, 0, shipMoveLR[Move_1] );
            laserMatrix[0].MergeMatrix( attackMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                AttackLaser.PaintVBuffer8To8( laserMatrix[0], POS_ZBUFFER );
            else
                AttackLaser.PaintVBuffer8To16( laserMatrix[0], POS_ZBUFFER );
        }
    }
    // Draw the laser blast going off into the distance
    else if( LaserCounter[0] )
    {
        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the laser down the line
        FinalMatrix.Translate( 0, 0, LaserCounter[0] * 700 );
        FinalMatrix.MergeMatrix( laserMatrix[0].Matrix );

        // Who is the aggresser
        if( (Direction == 1 && Z_MoveAry[0] > Z_MoveAry[1]) ||
            (Direction == -1 && Z_MoveAry[0] < Z_MoveAry[1]) )
        {        
            // Paint the buffer
            if( SystemBitDepth == 8 )
                DefendLaserOnly.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
            else
                DefendLaserOnly.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
        }
        else
        {        
            // Paint the buffer
            if( SystemBitDepth == 8 )
                AttackLaserOnly.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
            else
                AttackLaserOnly.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
        }

        // Inc the laser counter
        LaserCounter[0] = (LaserCounter[0] + 1) % 5;
    }

    // inc the counters
    Move_1 = (Move_1 + 1) % SHIP_MOVE_SIDE_TO_SIDE;
    Move_2 = (Move_2 + 1) % SHIP_MOVE_SIDE_TO_SIDE;

    // Reinit the matrix
    FinalMatrix.InitilizeMatrix();

    // Move the mother ship's Z access
    FinalMatrix.Translate( 0, 0, Z_Move );
    FinalMatrix.MergeMatrix( motherMatrix.Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        MotherShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        MotherShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = AttackList.GetInt( AttackSceneCounter );

        // Inc to the next attack scene
        AttackSceneCounter = ( AttackSceneCounter + 1 ) % AttackList.Count();

        // Reset the flags that picks if we are to 
        // fight or explode on the top or bottom
        PickFightExplode = -1;
        PickTopOrBottom = -1;

        // Now mix up all the ints
        if( AttackSceneCounter == 0 )
            AttackList.MixUp();

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_AttackFlyTop


/************************************************************************
*    FUNCTION NAME:         Scene_AttackFlyBottom                                                             
*
*    DESCRIPTION:           Camera bottom fly by
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

void CAnimateSaver::Scene_AttackFlyBottom()
{
    Matrix3D FinalMatrix;

    // Do scene start initilizations
    if( setUpScene )
    {
        int Y, Index;
        int cameraXTran[MAX_FLYBY_VIEWS] = {0,350,-350,500,232};
        const int cameraYTran[MAX_FLYBY_VIEWS] = {5698,5889,5789,5301,6200};
        int cameraZTran[MAX_FLYBY_VIEWS] = {18000,18000,16000,19000,11200};
        const int cameraXRot[MAX_FLYBY_VIEWS] = {120,174,151,59,244};
        int cameraYRot[MAX_FLYBY_VIEWS] = {0,-129,116,600,510};
        int shipZOffset[MAX_FLYBY_VIEWS] = {500,-100,100,-2000,-800};

        // Pick the camers angle
        Index = rand() % MAX_FLYBY_VIEWS;

        // Choose the movement direction
        if( rand() % 2 )    // Face the nose of the mother ship
        {
            Movement = 1;

            if( rand() % 2 )  // Going to noze
            {
                MoveAmount = -600;
                Direction = 1;

                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( Index == 3 )
                    cameraZTran[Index] = 15000;
            }
            else     // Going away from noze
            {
                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( Index == 3 )
                    cameraZTran[Index] = 20000;

                Direction = -1;
                MoveAmount = 600;
                cameraZTran[Index] *= -1;
            }
        }
        else                // Face the back of the mother ship
        {
            Movement = -1;
            
            if( rand() % 2 )       // Going to back
            {
                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( Index == 1 || Index == 3 )
                    cameraZTran[Index] = 15000;

                Direction = -1;
                MoveAmount = 600;
                cameraXTran[Index] *= -1;
                cameraZTran[Index] *= -1;
                cameraYRot[Index] += 512;
            }
            else      // Going away from back
            {
                Direction = 1;
                MoveAmount = -600;
                cameraYRot[Index] += 512;
                cameraXTran[Index] *= -1;
            }
        }

        // Init the starting position to the matrix
        motherMatrix.InitilizeMatrix();
        motherMatrix.Translate( cameraXTran[Index], cameraYTran[Index], cameraZTran[Index] );
        motherMatrix.Rotate( cameraXRot[Index], cameraYRot[Index], 0 );

        // Init the starting position to the matrix
        attackMatrix[0].InitilizeMatrix();
        defendMatrix[0].InitilizeMatrix();
        LaserCounter[0] = 0;

        // rotate the ship if we are going in the other direction
        if( Direction == -1 )
        {
            attackMatrix[0].Rotate( 0, 512, 0 );                
            defendMatrix[0].Rotate( 0, 512, 0 );
        }

        // Randomize the Y offset
        Y = -5075;

        // Pick which will be the aggressive ship
        if( rand() % 2 )
        {
            Z_MoveAry[0] = shipZOffset[Index] * Movement;
            Z_MoveAry[1] = (shipZOffset[Index] + 1310) * Movement;
        }
        else
        {
            Z_MoveAry[0] = (shipZOffset[Index] + 1310) * Movement;
            Z_MoveAry[1] = shipZOffset[Index] * Movement;
        }

        attackMatrix[0].Translate( cameraXTran[Index], cameraYTran[Index] + Y, Z_MoveAry[0] );
        attackMatrix[0].Rotate( cameraXRot[Index], cameraYRot[Index], 0 );

        defendMatrix[0].Translate( cameraXTran[Index], cameraYTran[Index] + Y, Z_MoveAry[1] );
        defendMatrix[0].Rotate( cameraXRot[Index], cameraYRot[Index], 0 );
        
        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 50;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Z axis move counter
        Z_Move = 0;
        Move_1 = 0;
        Move_2 = SHIP_MOVE_SIDE_TO_SIDE / 2;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (starBackground + cloudBufferOffset), 
            (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Move the mother ship's Z access
    Z_Move += MoveAmount;

    // Move the mother ship's Z access
    FinalMatrix.Translate( shipMoveLR[Move_1]*6, shipMoveLR[Move_1], 0 );
    FinalMatrix.Rotate( 0, 0, shipMoveLR[Move_1] );
    FinalMatrix.MergeMatrix( attackMatrix[0].Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        AttackShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        AttackShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Reinit the matrix
    FinalMatrix.InitilizeMatrix();

    // Move the mother ship's Z access
    FinalMatrix.Translate( shipMoveLR[Move_2]*5, shipMoveLR[Move_2], 0 );
    FinalMatrix.Rotate( 0, 0, shipMoveLR[Move_2] );
    FinalMatrix.MergeMatrix( defendMatrix[0].Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        DefendShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        DefendShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Do we show the laser blast
    if( LaserCounter[0] == 0 && (rand() % 3) )
    {
        ++LaserCounter[0];

        // Reinit the laser
        laserMatrix[0].InitilizeMatrix();

        // Who is the aggresser
        if( (Direction == 1 && Z_MoveAry[0] > Z_MoveAry[1]) ||
            (Direction == -1 && Z_MoveAry[0] < Z_MoveAry[1]) )
        {
            laserMatrix[0].Translate( shipMoveLR[Move_2]*5, shipMoveLR[Move_2], 0 );
            laserMatrix[0].Rotate( 0, 0, shipMoveLR[Move_2] );
            laserMatrix[0].MergeMatrix( defendMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                DefendLaser.PaintVBuffer8To8( laserMatrix[0], POS_ZBUFFER );
            else
                DefendLaser.PaintVBuffer8To16( laserMatrix[0], POS_ZBUFFER );
        }
        else
        {
            laserMatrix[0].Translate( shipMoveLR[Move_1]*6, shipMoveLR[Move_1], 0 );
            laserMatrix[0].Rotate( 0, 0, shipMoveLR[Move_1] );
            laserMatrix[0].MergeMatrix( attackMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                AttackLaser.PaintVBuffer8To8( laserMatrix[0], POS_ZBUFFER );
            else
                AttackLaser.PaintVBuffer8To16( laserMatrix[0], POS_ZBUFFER );
        }
    }
    // Draw the laser blast going off into the distance
    else if( LaserCounter[0] )
    {
        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the laser down the line
        FinalMatrix.Translate( 0, 0, LaserCounter[0] * 700 );
        FinalMatrix.MergeMatrix( laserMatrix[0].Matrix );

        // Who is the aggresser
        if( (Direction == 1 && Z_MoveAry[0] > Z_MoveAry[1]) ||
            (Direction == -1 && Z_MoveAry[0] < Z_MoveAry[1]) )
        {        
            // Paint the buffer
            if( SystemBitDepth == 8 )
                DefendLaserOnly.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
            else
                DefendLaserOnly.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
        }
        else
        {        
            // Paint the buffer
            if( SystemBitDepth == 8 )
                AttackLaserOnly.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
            else
                AttackLaserOnly.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
        }

        // Inc the laser counter
        LaserCounter[0] = (LaserCounter[0] + 1) % 5;
    }

    // inc the counters
    Move_1 = (Move_1 + 1) % SHIP_MOVE_SIDE_TO_SIDE;
    Move_2 = (Move_2 + 1) % SHIP_MOVE_SIDE_TO_SIDE;

    // Reinit the matrix
    FinalMatrix.InitilizeMatrix();

    // Move the mother ship's Z access
    FinalMatrix.Translate( 0, 0, Z_Move );
    FinalMatrix.MergeMatrix( motherMatrix.Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        MotherShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        MotherShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = AttackList.GetInt( AttackSceneCounter );

        // Inc to the next attack scene
        AttackSceneCounter = ( AttackSceneCounter + 1 ) % AttackList.Count();

        // Reset the flags that picks if we are to 
        // fight or explode on the top or bottom
        PickFightExplode = -1;
        PickTopOrBottom = -1;

        // Now mix up all the ints
        if( AttackSceneCounter == 0 )
            AttackList.MixUp();

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_AttackFlyBottom


/************************************************************************
*    FUNCTION NAME:         Scene_AttackFlySide                                                             
*
*    DESCRIPTION:           Camera left side fly by
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

void CAnimateSaver::Scene_AttackFlySide()
{
    Matrix3D FinalMatrix;

    // Do scene start initilizations
    if( setUpScene )
    {
        int Y, X, Index;
        int cameraXTran[MAX_FLYBY_VIEWS] = {-6460,   -6460,  -6310,   -6000,   -6300};
        int cameraYTran[MAX_FLYBY_VIEWS] = {980,     980,    1200,    1220,  150 };
        int cameraZTran[MAX_FLYBY_VIEWS] = {16000,   16000,  16000,   16000, 16000};
        const int cameraXRot[MAX_FLYBY_VIEWS] = {50, 50,     100,     153,    -50};
        int cameraYRot[MAX_FLYBY_VIEWS] = {256,      340,    212,     325,   255};
        int shipZOffset[MAX_FLYBY_VIEWS] = {-600,   -1200,   -150,    -900,  -600};

        // Pick the camera angle
        Index = rand() % MAX_FLYBY_VIEWS;

        // Choose the movement direction
        if( rand() % 2 )    // Left side of ship
        {
            Movement = 1;

            if( rand() % 2 )
            {
                MoveAmount = -600;
                Direction = 1;
            }
            else
            {
                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( Index == 0 )
                    cameraZTran[Index] = 24000;
                else if( Index == 1 )
                    cameraZTran[Index] = 27000;
                else if( Index == 2 )
                    cameraZTran[Index] = 23000;
                else if( Index == 3 || Index == 4 )
                    cameraZTran[Index] = 24000;

                Direction = -1;
                MoveAmount = 600;
                cameraZTran[Index] *= -1;
            }

            // Set the Y offset
            X = 5752;
        }
        else                // Right side of ship
        {
            Movement = -1;
            
            if( rand() % 2 )
            {
                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( Index == 0 )
                    cameraZTran[Index] = 24000;
                else if( Index == 1 )
                    cameraZTran[Index] = 22000;
                else if( Index == 2 )
                    cameraZTran[Index] = 26000;
                else if( Index == 3 || Index == 4 )
                    cameraZTran[Index] = 23000;

                Direction = -1;
                MoveAmount = 600;
                cameraXTran[Index] *= -1;
                cameraZTran[Index] *= -1;
                cameraYRot[Index] += 512;
            }
            else
            {
                Direction = 1;
                MoveAmount = -600;
                cameraYRot[Index] += 512;
                cameraXTran[Index] *= -1;
            }

            // Set the Y offset
            X = -5752;
        }

        // Set the Y offset
        Y = -671;

        // Init the starting position to the matrix
        motherMatrix.InitilizeMatrix();
        motherMatrix.Translate( cameraXTran[Index], cameraYTran[Index], cameraZTran[Index] );
        motherMatrix.Rotate( cameraXRot[Index], cameraYRot[Index], 0 );

        // Init the starting position to the matrix
        attackMatrix[0].InitilizeMatrix();
        defendMatrix[0].InitilizeMatrix();
        LaserCounter[0] = 0;

        // rotate the ship if we are going in the other direction
        if( Direction == -1 )
        {
            attackMatrix[0].Rotate( 0, 512, 0 );                
            defendMatrix[0].Rotate( 0, 512, 0 );
        }

        // Pick which will be the aggressive ship
        if( rand() % 2 )
        {
            Z_MoveAry[0] = shipZOffset[Index] * Movement;
            Z_MoveAry[1] = (shipZOffset[Index] + 1210) * Movement;
        }
        else
        {
            Z_MoveAry[0] = (shipZOffset[Index] + 1210) * Movement;
            Z_MoveAry[1] = shipZOffset[Index] * Movement;
        }

        attackMatrix[0].Translate( cameraXTran[Index] + X, cameraYTran[Index] + Y, Z_MoveAry[0] );
        attackMatrix[0].Rotate( cameraXRot[Index], cameraYRot[Index], 0 );

        defendMatrix[0].Translate( cameraXTran[Index] + X, cameraYTran[Index] + Y, Z_MoveAry[1] );
        defendMatrix[0].Rotate( cameraXRot[Index], cameraYRot[Index], 0 );
        
        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 60;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Z axis move counter
        Z_Move = 0;
        Move_1 = 0;
        Move_2 = SHIP_MOVE_SIDE_TO_SIDE / 2;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (starBackground + cloudBufferOffset), 
            (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Move the mother ship's Z access
    Z_Move += MoveAmount;

    // Move the mother ship's Z access
    FinalMatrix.Translate( shipMoveLR[Move_1]*5, shipMoveLR[Move_1], 0 );
    FinalMatrix.Rotate( 0, 0, shipMoveLR[Move_1] );
    FinalMatrix.MergeMatrix( attackMatrix[0].Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        AttackShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        AttackShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Reinit the matrix
    FinalMatrix.InitilizeMatrix();

    // Move the mother ship's Z access
    FinalMatrix.Translate( shipMoveLR[Move_2]*4, shipMoveLR[Move_2], 0 );
    FinalMatrix.Rotate( 0, 0, shipMoveLR[Move_2] );
    FinalMatrix.MergeMatrix( defendMatrix[0].Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        DefendShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        DefendShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Do we show the laser blast
    if( LaserCounter[0] == 0 && (rand() % 3) )
    {
        ++LaserCounter[0];

        // Reinit the laser
        laserMatrix[0].InitilizeMatrix();

        // Who is the aggresser
        if( (Direction == 1 && Z_MoveAry[0] > Z_MoveAry[1]) ||
            (Direction == -1 && Z_MoveAry[0] < Z_MoveAry[1]) )
        {
            laserMatrix[0].Translate( shipMoveLR[Move_2]*4, shipMoveLR[Move_2], 0 );
            laserMatrix[0].Rotate( 0, 0, shipMoveLR[Move_2] );
            laserMatrix[0].MergeMatrix( defendMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                DefendLaser.PaintVBuffer8To8( laserMatrix[0], POS_ZBUFFER );
            else
                DefendLaser.PaintVBuffer8To16( laserMatrix[0], POS_ZBUFFER );
        }
        else
        {
            laserMatrix[0].Translate( shipMoveLR[Move_1]*5, shipMoveLR[Move_1], 0 );
            laserMatrix[0].Rotate( 0, 0, shipMoveLR[Move_1] );
            laserMatrix[0].MergeMatrix( attackMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                AttackLaser.PaintVBuffer8To8( laserMatrix[0], POS_ZBUFFER );
            else
                AttackLaser.PaintVBuffer8To16( laserMatrix[0], POS_ZBUFFER );
        }
    }
    // Draw the laser blast going off into the distance
    else if( LaserCounter[0] )
    {
        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the laser down the line
        FinalMatrix.Translate( 0, 0, LaserCounter[0] * 700 );
        FinalMatrix.MergeMatrix( laserMatrix[0].Matrix );

        // Who is the aggresser
        if( (Direction == 1 && Z_MoveAry[0] > Z_MoveAry[1]) ||
            (Direction == -1 && Z_MoveAry[0] < Z_MoveAry[1]) )
        {        
            // Paint the buffer
            if( SystemBitDepth == 8 )
                DefendLaserOnly.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
            else
                DefendLaserOnly.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
        }
        else
        {        
            // Paint the buffer
            if( SystemBitDepth == 8 )
                AttackLaserOnly.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
            else
                AttackLaserOnly.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
        }

        // Inc the laser counter
        LaserCounter[0] = (LaserCounter[0] + 1) % 5;
    }

    // inc the counters
    Move_1 = (Move_1 + 1) % SHIP_MOVE_SIDE_TO_SIDE;
    Move_2 = (Move_2 + 1) % SHIP_MOVE_SIDE_TO_SIDE;

    // Reinit the matrix
    FinalMatrix.InitilizeMatrix();

    // Move the mother ship's Z access
    FinalMatrix.Translate( 0, 0, Z_Move );
    FinalMatrix.MergeMatrix( motherMatrix.Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        MotherShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        MotherShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = AttackList.GetInt( AttackSceneCounter );

        // Inc to the next attack scene
        AttackSceneCounter = ( AttackSceneCounter + 1 ) % AttackList.Count();

        // Reset the flags that picks if we are to 
        // fight or explode on the top or bottom
        PickFightExplode = -1;
        PickTopOrBottom = -1;

        // Now mix up all the ints
        if( AttackSceneCounter == 0 )
            AttackList.MixUp();

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_AttackFlySide


/************************************************************************
*    FUNCTION NAME:         Scene_OverViewAttack                                                             
*
*    DESCRIPTION:           Over view shot of the battle.
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

void CAnimateSaver::Scene_OverViewAttack()
{
    Matrix3D FinalMatrix;
    int i, MoveAmount = 150;
    float Offset;

    // Do scene start initilizations
    if( setUpScene )
    {
        const int cameraXTran[MAX_OVERVIEW_ANIM] = {4997,-133,-5527,-5307,2073,9083,-57,0,40,-11000,11000,8210,-8210,-2400,-650,-650,-150,2330,-2347};
        const int cameraYTran[MAX_OVERVIEW_ANIM] = {-4434,-6424,-4034,6654,10804,7000,-9500,-6200,6520,-960,-960,5040,5040,10050,-11000,-11000,-11000,10170,10804};
        const int cameraZTran[MAX_OVERVIEW_ANIM] = {-28530,-20230,-19730,-30430,-15430,-13730,-17930,-16730,-13000,-28430,-27430,-35230,-35230,-27520,-19020,-19020,-12870,-23020,-16430};
        const int cameraXRot[MAX_OVERVIEW_ANIM] = {-49,-40,-38,74,201,113,-202,-45,120,-10,-10,91,91,181,-228,-228,-228,189,201};
        const int cameraYRot[MAX_OVERVIEW_ANIM] = {554,522,470,446,-106,-121,120,0,0,-753,-261,-393,393,382,376,-376,-173,-347,164};

        const int TopBottomLeftRight[2] = {1,-1};
        int shipDirection[MAX_FIGHT_SHIPS], differenceX, differenceY;

        // init to the next overview scene
        AnimIndex = OverviewList.GetInt( OverviewCounter );

        // Inc to the next overview scene
        OverviewCounter = ( OverviewCounter + 1 ) % OverviewList.Count();

        // Now mix up all the ints
        if( OverviewCounter == 0 )
            OverviewList.MixUp();

        // Init the matrix for the mother ship
        motherMatrix.InitilizeMatrix();
        motherMatrix.Translate( cameraXTran[AnimIndex], cameraYTran[AnimIndex], cameraZTran[AnimIndex] );
        motherMatrix.Rotate( cameraXRot[AnimIndex], cameraYRot[AnimIndex], 0 );

        // Init the starting position to the matrix
        for( i = 0; i < MAX_FIGHT_SHIPS; ++i )
        {
            // Randomly pick the aggrssive ship
            MoveAry3[i] = rand() % 2;

            // randomly pick the ship positions
            // This the the generalized random positions
            X_MoveAry[i] = (3500 + (rand() % 3000)) * TopBottomLeftRight[rand() % 2];
            Y_MoveAry[i] = (4000 + (rand() % 3000)) * TopBottomLeftRight[rand() % 2];
            Z_MoveAry[i] = (1 + (i * 3500)) * TopBottomLeftRight[rand() % 2];

            ////////////////////////////////////////////////////////////////////////////
            // No need having any ships animationg that we can't see
            // So the ones we can't see, push them into view.
            // Also, depending on the view, we can push more of them close to us to see
            // Below are specific conditions depending on the camera position
            ////////////////////////////////////////////////////////////////////////////
            switch( AnimIndex )
            {
                case 0:
                    // can't have +X and a -Y
                    if( X_MoveAry[i] > 0 && Y_MoveAry[i] < 0 )
                    {
                        // Throw the ship on the side we are viewing
                        X_MoveAry[i] *= -1;
                        Y_MoveAry[i] = abs(Y_MoveAry[i]);

                        // Set some of the X to go down the center
                        if( rand() % 3 == 0 )
                        {
                            X_MoveAry[i] = (rand() % 300) * TopBottomLeftRight[rand() % 2];

                            //Make sure the Y isn't so low that it goes into the mother ship
                            if( Y_MoveAry[i] < 5200 )
                                Y_MoveAry[i] = 5200;
                        }
                    }
                break;

                case 1: case 7:
                    // can't have -Y
                    // Fix one of the two offsets so that the ships are seen
                    if( Y_MoveAry[i] < 0 )
                    {
                        Y_MoveAry[i] = abs(Y_MoveAry[i]);

                        // Set some of the X to go down the center
                        if( rand() % 2 )
                        {
                            X_MoveAry[i] = (rand() % 300) * TopBottomLeftRight[rand() % 2];

                            //Make sure the Y isn't so low that it goes into the mother ship
                            if( Y_MoveAry[i] < 5200 )
                                Y_MoveAry[i] = 5200 + (rand() % 2000);

                            // Make sure the ships don't run right into the camera
                            differenceY = abs(Y_MoveAry[i] - abs(cameraYTran[AnimIndex]));

                            // Push the Y out of the way of the camera
                            if( differenceY < OFFSET_FROM_CAMERA )
                            {
                                if( rand() % 2 )
                                    // Push up if on top of the mother ship
                                    Y_MoveAry[i] = abs(cameraYTran[AnimIndex]) + OFFSET_FROM_CAMERA;
                                else
                                    // Push down if under the mother ship
                                    Y_MoveAry[i] = abs(cameraYTran[AnimIndex]) - OFFSET_FROM_CAMERA;
                            }
                        }
                    }
                break;

                case 2:
                    // can't have +X and a -Y
                    if( X_MoveAry[i] < 0 && Y_MoveAry[i] < 0 )
                    {
                        // Throw the ship on the side we are viewing
                        Y_MoveAry[i] = 3000 + (rand() % 1000);
                        X_MoveAry[i] = 4000 + (rand() % 3000);

                        // Set some of the X to go down the center
                        if( rand() % 3 == 0 )
                        {
                            X_MoveAry[i] = (rand() % 300) * TopBottomLeftRight[rand() % 2];

                            //Make sure the Y isn't so low that it goes into the mother ship
                            if( Y_MoveAry[i] < 5200 )
                                Y_MoveAry[i] = 5200;
                        }
                    }
                    // The Y needs to be a little higher so we can see the ships
                    else if( X_MoveAry[i] < 0 && Y_MoveAry[i] > 0 && Y_MoveAry[i] < 5200 )
                    {
                        Y_MoveAry[i] = 5000 + (rand() % 5000);
                    }
                    
                break;

                case 3:
                    // can't have -X and a +Y
                    if( X_MoveAry[i] < 0 && Y_MoveAry[i] > 0 )
                    {
                        // Throw the ship on the side we are viewing
                        Y_MoveAry[i] *= -1;
                        X_MoveAry[i] = abs(X_MoveAry[i]);
                    }
                    // If the Y is too low, push the X to the middle of the bottom of the ship
                    else if( Y_MoveAry[i] < -6000 )
                        X_MoveAry[i] = (rand() % 1500) * TopBottomLeftRight[rand() % 2];
                break;

                case 4: 
                    // can't have +Y
                    // Since it is off, reset the X and Y
                    if( Y_MoveAry[i] > 0 )
                    {
                        Y_MoveAry[i] = -(7000 + (rand() % 3000));

                        X_MoveAry[i] = -(rand() % 2500);
                    }
                break;

                case 5:
                    // can't have +X and a +Y
                    if( X_MoveAry[i] > 0 && Y_MoveAry[i] > 0 )
                    {
                        // Throw the ship on the side we are viewing
                        // and push it out a little further
                        X_MoveAry[i] = (X_MoveAry[i] * -1) + -2000;
                        Y_MoveAry[i] *= -1;
                    }
                break;

                case 6:
                    // can't have -Y
                    // Since it is off, reset the X and Y
                    if( Y_MoveAry[i] < 0 )
                    {
                        Y_MoveAry[i] = (6000 + (rand() % 3000));

                        if( i < 30 )
                            X_MoveAry[i] = (rand() % 1000) * TopBottomLeftRight[rand() % 2];
                        else
                            X_MoveAry[i] = (rand() % 300) * TopBottomLeftRight[rand() % 2];
                    }
                break;

                case 8:
                    // can't have +Y
                    // Since it is off, reset the X and Y
                    if( Y_MoveAry[i] > 0 )
                    {
                        // make all Y negative
                        Y_MoveAry[i] *= -1;

                        // Randomize all new positions
                        X_MoveAry[i] = (rand() % 1500) * TopBottomLeftRight[rand() % 2];

                        // Make sure the ships don't run right into the camera
                        differenceY = abs(abs(Y_MoveAry[i]) - cameraYTran[AnimIndex]);

                        // Push the Y out of the way of the camera
                        if( differenceY < OFFSET_FROM_CAMERA && abs(X_MoveAry[i]) < OFFSET_FROM_CAMERA_X )
                        {
                            if( rand() % 2 )
                                // Push up if on top of the mother ship
                                Y_MoveAry[i] = -(cameraYTran[AnimIndex] + OFFSET_FROM_CAMERA);
                            else
                                // Push down if under the mother ship
                                Y_MoveAry[i] = -(cameraYTran[AnimIndex] - OFFSET_FROM_CAMERA);
                        }
                    }
                break;

                case 9:
                    // can't have too many -X      
                    // Since it is off, reset the X and Y
                    // Throw the ship on the side we are viewing
                    if( X_MoveAry[i] < 0 )
                    {
                        // Push it out in front of use so wecan see it better
                        if( rand() % 2 )
                        {
                            Y_MoveAry[i] = (rand() % 1500);
                            // Push the X out so we can see it more
                            X_MoveAry[i] = 6000 + (rand() % 4000);

                            // Make sure the Y doesn't run into the protruding side thing
                            if( X_MoveAry[i] < 7850 )
                                Y_MoveAry[i] = 1350 + rand() % 2000;
                        }
                        else
                           X_MoveAry[i] = abs(X_MoveAry[i]);
                    }
                    // The Y might be too high or low to see so center it on the top or bottom of the ship
                    else if( abs(Y_MoveAry[i]) > 5000 )
                    {
                        if( Y_MoveAry[i] > 0 )
                            X_MoveAry[i] = (rand() % 500) * TopBottomLeftRight[rand() % 2];
                        else
                            X_MoveAry[i] = (rand() % 1500) * TopBottomLeftRight[rand() % 2];
                    } 
                break;

                case 10:
                    // can't have too many +X
                    // Since it is off, reset the X and Y
                    // Throw the ship on the side we are viewing
                    if( X_MoveAry[i] > 0 )
                    {
                        // Push it out in front of use so wecan see it better
                        if( rand() % 2 )
                        {
                            Y_MoveAry[i] = (rand() % 1500);
                            // Push the X out so we can see it more
                            X_MoveAry[i] = -(6000 + (rand() % 4000));

                            // Make sure the Y doesn't run into the protruding side thing
                            if( X_MoveAry[i] > -7850 )
                                Y_MoveAry[i] = 1350 + rand() % 2000;
                        }
                        else
                           X_MoveAry[i] = -X_MoveAry[i];
                    }
                    else if( abs(Y_MoveAry[i]) > 5000 )
                    {
                        if( Y_MoveAry[i] > 0 )
                            X_MoveAry[i] = (rand() % 500) * TopBottomLeftRight[rand() % 2];
                        else
                            X_MoveAry[i] = (rand() % 1500) * TopBottomLeftRight[rand() % 2];
                    }
                break;

                case 11:
                    // can't have +X and a +Y
                    if( X_MoveAry[i] > 0 && Y_MoveAry[i] > 0 )
                    {
                        // Push the X & Y out so we can see it more
                        X_MoveAry[i] = -(4000 + (rand() % 2500));
                        Y_MoveAry[i] = -(3000 + (rand() % 2000));
                    }
                    // If the Y is too low, push the X to the middle of the bottom of the ship
                    else if( Y_MoveAry[i] < -5000 )
                        X_MoveAry[i] = (rand() % 1500) * TopBottomLeftRight[rand() % 2];
                break;

                case 12:
                    // can't have -X and a +Y
                    if( X_MoveAry[i] < 0 && Y_MoveAry[i] > 0 )
                    {
                        // Push the X & Y out so we can see it more
                        X_MoveAry[i] = 4500 + (rand() % 2000);
                        Y_MoveAry[i] = -(3000 + (rand() % 2000));
                    }
                    // If the Y is too low, push the X to the middle of the bottom of the ship
                    else if( Y_MoveAry[i] < -5000 )
                        X_MoveAry[i] = (rand() % 1500) * TopBottomLeftRight[rand() % 2];
                break;

                case 13:
                    // can't have +Y
                    // Since it is off, reset the X and Y
                    if( Y_MoveAry[i] > 0 )
                    {
                        Y_MoveAry[i] = -(6000 + (rand() % 3500));

                        X_MoveAry[i] = (rand() % 3000);
                    }
                break;

                case 14: case 15: case 16:
                    // can't have -Y
                    // Fix one of the two offsets so that the ships are seen
                    if( Y_MoveAry[i] < 0 )
                    {
                        Y_MoveAry[i] = (5000 + (rand() % 5000));

                        // Set the X to go down the center
                        X_MoveAry[i] = (rand() % 300) * TopBottomLeftRight[rand() % 2];
                    }
                break;

                case 17:
                    // can't have +Y
                    // Since it is off, reset the X and Y
                    if( Y_MoveAry[i] > 0 )
                    {
                        Y_MoveAry[i] = -(6000 + (rand() % 3500));

                        X_MoveAry[i] = -(rand() % 3000);
                    }
                break;

                case 18: 
                    // can't have +Y
                    // Since it is off, reset the X and Y
                    if( Y_MoveAry[i] > 0 )
                    {
                        Y_MoveAry[i] = -(7000 + (rand() % 3000));

                        X_MoveAry[i] = 1000 + (rand() % 2000);
                    }
                break;
            }

            // Make sure the ships don't run right into the camera
            // This causes polygons to stop drawing for what ever reason
            if( (Y_MoveAry[i] > 0 && cameraYTran[AnimIndex] < 0) ||
                (Y_MoveAry[i] < 0 && cameraYTran[AnimIndex] > 0) )
            {
                if( (X_MoveAry[i] > 0 && cameraXTran[AnimIndex] < 0) ||
                    (X_MoveAry[i] < 0 && cameraXTran[AnimIndex] > 0) )
                {
                    // Make sure the ships don't run right into the camera
                    differenceX = abs(abs(X_MoveAry[i]) - abs(cameraXTran[AnimIndex]));
                    differenceY = abs(abs(Y_MoveAry[i]) - abs(cameraYTran[AnimIndex]));

                    // Push the Y out of the way of the camera
                    if( differenceX < OFFSET_FROM_CAMERA && differenceY < OFFSET_FROM_CAMERA )
                    {
                        if( Y_MoveAry[i] > 0 )
                            // Push up if on top of the mother ship
                            Y_MoveAry[i] = abs(cameraYTran[AnimIndex]) + OFFSET_FROM_CAMERA;
                        else
                            // Push down if under the mother ship
                            Y_MoveAry[i] = -(abs(cameraYTran[AnimIndex]) + OFFSET_FROM_CAMERA);
                    }
                }
            }

            // Set the way the ship moves
            if( Z_MoveAry[i] > 0 )
                shipDirection[i] = -1;
            else
                shipDirection[i] = 1;

            // Init the matrixes
            attackMatrix[i].InitilizeMatrix();
            defendMatrix[i].InitilizeMatrix();

            // rotate the ship if we are going in the other direction
            if( shipDirection[i] == -1 )
            {
                attackMatrix[i].Rotate( 0, 512, 0 );                
                defendMatrix[i].Rotate( 0, 512, 0 );
            }

            // Set which ship will be the attacking ship
            if( MoveAry3[i] )
            {
                attackMatrix[i].Translate( cameraXTran[AnimIndex] + X_MoveAry[i], cameraYTran[AnimIndex] + Y_MoveAry[i], cameraZTran[AnimIndex] + Z_MoveAry[i] + (2000 * shipDirection[i]) );
                defendMatrix[i].Translate( cameraXTran[AnimIndex] + X_MoveAry[i], cameraYTran[AnimIndex] + Y_MoveAry[i], cameraZTran[AnimIndex] + Z_MoveAry[i] );
            }
            else
            {
                attackMatrix[i].Translate( cameraXTran[AnimIndex] + X_MoveAry[i], cameraYTran[AnimIndex] + Y_MoveAry[i], cameraZTran[AnimIndex] + Z_MoveAry[i] );
                defendMatrix[i].Translate( cameraXTran[AnimIndex] + X_MoveAry[i], cameraYTran[AnimIndex] + Y_MoveAry[i], cameraZTran[AnimIndex] + Z_MoveAry[i] + (2000 * shipDirection[i]) );
            }

            attackMatrix[i].Rotate( cameraXRot[AnimIndex], cameraYRot[AnimIndex], 0 );              
            defendMatrix[i].Rotate( cameraXRot[AnimIndex], cameraYRot[AnimIndex], 0 );


            // Randomly pick the sway position
            MoveAry1[i] = rand() % SHIP_MOVE_SIDE_TO_SIDE;
            MoveAry2[i] = rand() % SHIP_MOVE_SIDE_TO_SIDE;

            LaserCounter[i] = 0;
        }
        
        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 200;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Z axis move counter
        Z_Move = 0;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (starBackground + cloudBufferOffset), 
            (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Move the ship's Z access
    Z_Move += MoveAmount;

    // Move the ships
    for( i = 0; i < MAX_FIGHT_SHIPS; ++i )
    {
        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Sway the attack attack ship as it is moved
        FinalMatrix.Translate( shipMoveLR[MoveAry1[i]]*6, shipMoveLR[MoveAry1[i]], (Z_Move * 3) );
        FinalMatrix.Rotate( 0, 0, shipMoveLR[MoveAry1[i]] );

        FinalMatrix.MergeMatrix( attackMatrix[i].Matrix );

        ///////////////////////////////////////////////////////////////////////////////
        // For many of the camera angles we need to do some X or Y clipping of whole ships.
        // This helps speed the whole scene. Dont process the ships we don't even see
        ///////////////////////////////////////////////////////////////////////////////
        switch( AnimIndex )
        {
            case 4: case 18:
                Offset = FinalMatrix.GetY();

                if( !(Offset > -12000.0F && Offset < 7000.0F) )
                    continue;
            break;

            case 6:
                Offset = FinalMatrix.GetY();

                if( !(Offset > -5000.0F && Offset < 10000.0F) )
                    continue;
            break;

            case 9: case 10:
 
                Offset = FinalMatrix.GetX();

                if( !(Offset > -20000.0F && Offset < 20000.0F) )
                    continue;
            break;

            case 13: case 17:
                Offset = FinalMatrix.GetY();

                if( !(Offset > -15000.0F && Offset < 15000.0F) )
                    continue;
            break;

            case 14: case 15: case 16:
                Offset = FinalMatrix.GetY();

                if( !(Offset > -10000.0F && Offset < 10000.0F) )
                    continue;

                Offset = FinalMatrix.GetX();

                if( !(Offset > -10000.0F && Offset < 10000.0F) )
                    continue;
            break;
        }

        // Clip the Z offset on the ship level
        Offset = FinalMatrix.GetZ();

        if( !(Offset > -2000.0F && Offset < 35000.0F) )
            continue;

        // Paint the buffer
        if( SystemBitDepth == 8 )
            AttackShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            AttackShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Sway the defender attack ship as it is moved
        FinalMatrix.Translate( shipMoveLR[MoveAry2[i]]*5, shipMoveLR[MoveAry2[i]], (Z_Move * 3) );
        FinalMatrix.Rotate( 0, 0, shipMoveLR[MoveAry1[i]] );
        FinalMatrix.MergeMatrix( defendMatrix[i].Matrix );

        // Paint the buffer
        if( SystemBitDepth == 8 )
            DefendShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            DefendShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

        // Do we show the laser blast
        if( LaserCounter[i] == 0 && (rand() % 3) )
        {
            ++LaserCounter[i];

            // Reinit the laser
            laserMatrix[i].InitilizeMatrix();

            // Choose which laser is firing
            if( MoveAry3[i] )
            {
                laserMatrix[i].Translate( shipMoveLR[MoveAry2[i]]*5, shipMoveLR[MoveAry2[i]], (Z_Move * 3) );
                laserMatrix[i].Rotate( 0, 0, shipMoveLR[MoveAry2[i]] );
                laserMatrix[i].MergeMatrix( defendMatrix[i].Matrix );

                // Paint the buffer
                if( SystemBitDepth == 8 )
                    DefendLaser.PaintVBuffer8To8( laserMatrix[i], POS_ZBUFFER );
                else
                    DefendLaser.PaintVBuffer8To16( laserMatrix[i], POS_ZBUFFER );
            }
            else
            {

                laserMatrix[i].Translate( shipMoveLR[MoveAry1[i]]*6, shipMoveLR[MoveAry1[i]], (Z_Move * 3) );
                laserMatrix[i].Rotate( 0, 0, shipMoveLR[MoveAry1[i]] );
                laserMatrix[i].MergeMatrix( attackMatrix[i].Matrix );

                // Paint the buffer
                if( SystemBitDepth == 8 )
                    AttackLaser.PaintVBuffer8To8( laserMatrix[i], POS_ZBUFFER );
                else
                    AttackLaser.PaintVBuffer8To16( laserMatrix[i], POS_ZBUFFER );
            }
        }
        // Draw the laser blast going off into the distance
        else if( LaserCounter[i] )
        {
            // Reinit the matrix
            FinalMatrix.InitilizeMatrix();

            // Move the laser down the line
            FinalMatrix.Translate( 0, 0, ((LaserCounter[i] * 700) + (MoveAmount * 3)) );
            FinalMatrix.MergeMatrix( laserMatrix[i].Matrix );

            // Choose which laser is firing
            if( MoveAry3[i] )
            {               
                // Paint the buffer
                if( SystemBitDepth == 8 )
                    DefendLaserOnly.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
                else
                    DefendLaserOnly.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
            }
            else
            {               
                // Paint the buffer
                if( SystemBitDepth == 8 )
                    AttackLaserOnly.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
                else
                    AttackLaserOnly.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
            }

            // Inc the laser counter
            LaserCounter[i] = (LaserCounter[i] + 1) % 5;
        }

        // inc the counters
        MoveAry1[i] = (MoveAry1[i] + 1) % SHIP_MOVE_SIDE_TO_SIDE;
        MoveAry2[i] = (MoveAry2[i] + 1) % SHIP_MOVE_SIDE_TO_SIDE;
    }

    // Reinit the matrix
    FinalMatrix.InitilizeMatrix();

    // Move the mother ship's Z access
    FinalMatrix.Translate( 0, 0, Z_Move );
    FinalMatrix.MergeMatrix( motherMatrix.Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        MotherShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        MotherShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = AttackList.GetInt( AttackSceneCounter );

        // Inc to the next attack scene
        AttackSceneCounter = ( AttackSceneCounter + 1 ) % AttackList.Count();

        // Reset the flags that picks if we are to 
        // fight or explode on the top or bottom
        PickFightExplode = -1;
        PickTopOrBottom = -1;

        // Now mix up all the ints
        if( AttackSceneCounter == 0 )
            AttackList.MixUp();

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_OverViewAttack


/************************************************************************
*    FUNCTION NAME:         Scene_FlySideExplode                                                             
*
*    DESCRIPTION:           Camera left side fly by explode one of the fighters
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

void CAnimateSaver::Scene_FlySideExplode()
{
    Matrix3D FinalMatrix, ExplodeMatrix[MAX_EXPLODE_PER_SHIP];
    int i;

    // Do scene start initilizations
    if( setUpScene )
    {
        int Y, X;
        int cameraXTran[MAX_FLYBY_VIEWS] = {-6480,   -6470,  -6330,   -6000,   -6250};
        int cameraYTran[MAX_FLYBY_VIEWS] = {1080,     1080,    1200,    1220,  150 };
        int cameraZTran[MAX_FLYBY_VIEWS] = {16000,   16000,  16000,   16000, 16000};
        const int cameraXRot[MAX_FLYBY_VIEWS] = {50, 50,     100,     153,    -90};
        int cameraYRot[MAX_FLYBY_VIEWS] = {256,      340,    212,     325,   255};
        int shipZOffset[MAX_FLYBY_VIEWS] = {-600,   -1200,   -260,    -900,  -600};
        const int TopBottomLeftRight[2] = {1,-1};

        // Pick the camera angle
        AnimIndex = rand() % MAX_FLYBY_VIEWS;

        // Choose the movement direction
        if( rand() % 2 )    // Left side of ship
        {
            Movement = 1;

            if( rand() % 2 )
            {
                MoveAmount = -600;
                Direction = 1;
            }
            else
            {
                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( AnimIndex == 0 )
                    cameraZTran[AnimIndex] = 24000;
                else if( AnimIndex == 1 )
                    cameraZTran[AnimIndex] = 27000;
                else if( AnimIndex == 2 )
                    cameraZTran[AnimIndex] = 23000;
                else if( AnimIndex == 3 || AnimIndex == 4 )
                    cameraZTran[AnimIndex] = 24000;

                Direction = -1;
                MoveAmount = 600;
                cameraZTran[AnimIndex] *= -1;
            }

            // Set the Y offset
            X = 5752;
        }
        else                // Right side of ship
        {
            Movement = -1;
            
            if( rand() % 2 )
            {
                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( AnimIndex == 0 )
                    cameraZTran[AnimIndex] = 24000;
                else if( AnimIndex == 1 )
                    cameraZTran[AnimIndex] = 22000;
                else if( AnimIndex == 2 )
                    cameraZTran[AnimIndex] = 26000;
                else if( AnimIndex == 3 || AnimIndex == 4 )
                    cameraZTran[AnimIndex] = 23000;

                Direction = -1;
                MoveAmount = 600;
                cameraXTran[AnimIndex] *= -1;
                cameraZTran[AnimIndex] *= -1;
                cameraYRot[AnimIndex] += 512;
            }
            else
            {
                Direction = 1;
                MoveAmount = -600;
                cameraYRot[AnimIndex] += 512;
                cameraXTran[AnimIndex] *= -1;
            }

            // Set the Y offset
            X = -5752;
        }

        // Set the Y offset
        Y = -671;

        // Init the starting position to the matrix
        motherMatrix.InitilizeMatrix();
        motherMatrix.Translate( cameraXTran[AnimIndex], cameraYTran[AnimIndex], cameraZTran[AnimIndex] );
        motherMatrix.Rotate( cameraXRot[AnimIndex], cameraYRot[AnimIndex], 0 );

        // Init the starting position to the matrix
        attackMatrix[0].InitilizeMatrix();
        defendMatrix[0].InitilizeMatrix();
        LaserCounter[0] = 0;

        // rotate the ship if we are going in the other direction
        if( Direction == -1 )
        {
            attackMatrix[0].Rotate( 0, 512, 0 );                
            defendMatrix[0].Rotate( 0, 512, 0 );
        }

        // Pick which will be the aggressive ship
        if( rand() % 2 )
        {
            Z_MoveAry[0] = shipZOffset[AnimIndex] * Movement;
            Z_MoveAry[1] = (shipZOffset[AnimIndex] + 1210) * Movement;
        }
        else
        {
            Z_MoveAry[0] = (shipZOffset[AnimIndex] + 1210) * Movement;
            Z_MoveAry[1] = shipZOffset[AnimIndex] * Movement;
        }

        // Randomize the explosion offsets
        for( i = 0; i < MAX_EXPLODE_PER_SHIP; ++i )
        {
            ExplodeY[i] = (rand() % 51) * TopBottomLeftRight[rand() % 2];
            ExplodeZ[i] = (rand() % 151) * TopBottomLeftRight[rand() % 2];
        }

        // Pick the rotation direction
        RotDir = TopBottomLeftRight[rand() % 2];

        attackMatrix[0].Translate( cameraXTran[AnimIndex] + X, cameraYTran[AnimIndex] + Y, Z_MoveAry[0] );
        attackMatrix[0].Rotate( cameraXRot[AnimIndex], cameraYRot[AnimIndex], 0 );

        defendMatrix[0].Translate( cameraXTran[AnimIndex] + X, cameraYTran[AnimIndex] + Y, Z_MoveAry[1] );
        defendMatrix[0].Rotate( cameraXRot[AnimIndex], cameraYRot[AnimIndex], 0 );
        
        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 60;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Z axis move counter
        Z_Move = 0;
        Move_1 = 0;
        Rotate = 0;
        Move_3 = 0;
        Move_2 = SHIP_MOVE_SIDE_TO_SIDE / 2;

        // Set the Y rotation offset for the crash
        CrashOffset = rand() % MAX_CRASH_OFFSET;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (starBackground + cloudBufferOffset), 
            (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Move the mother ship's Z access
    Z_Move += MoveAmount;


    // Who is the aggresser
    if( (Direction == 1 && Z_MoveAry[0] > Z_MoveAry[1]) ||
        (Direction == -1 && Z_MoveAry[0] < Z_MoveAry[1]) )
    {
        // Move the attacking ship
        FinalMatrix.Rotate( 0, CrashOffset, Rotate );
        FinalMatrix.Translate( 0, Move_3, 0 );
        FinalMatrix.MergeMatrix( attackMatrix[0].Matrix );

        // Paint the buffer
        if( SystemBitDepth == 8 )
            AttackShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            AttackShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

        for( i = 0; i < MAX_EXPLODE_PER_SHIP; ++i )
        {
            // Setup the explode matrix
            ExplodeMatrix[i].Rotate( 0, CrashOffset, Rotate );
            ExplodeMatrix[i].Translate( 0, Move_3 + ExplodeY[i], ExplodeZ[i] );
            ExplodeMatrix[i].MergeMatrix( attackMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                Explode.PaintVBuffer8To8( ExplodeMatrix[i], POS_ZBUFFER );
            else
                Explode.PaintVBuffer8To16( ExplodeMatrix[i], POS_ZBUFFER );
        }

        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the mother ship's Z access
        FinalMatrix.Translate( shipMoveLR[Move_2]*4, shipMoveLR[Move_2], 0 );
        FinalMatrix.Rotate( 0, 0, shipMoveLR[Move_2] );
        FinalMatrix.MergeMatrix( defendMatrix[0].Matrix );


        // Paint the buffer
        if( SystemBitDepth == 8 )
            DefendShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            DefendShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
    }
    else
    {
        // Move the mother ship's Z access
        FinalMatrix.Translate( shipMoveLR[Move_1]*5, shipMoveLR[Move_1], 0 );
        FinalMatrix.Rotate( 0, 0, shipMoveLR[Move_1] );
        FinalMatrix.MergeMatrix( attackMatrix[0].Matrix );

        // Paint the buffer
        if( SystemBitDepth == 8 )
            AttackShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            AttackShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the mother ship's Z access
        FinalMatrix.Rotate( 0, CrashOffset, Rotate );
        FinalMatrix.Translate( 0, Move_3, 0 );
        FinalMatrix.MergeMatrix( defendMatrix[0].Matrix );

        // Paint the buffer
        if( SystemBitDepth == 8 )
            DefendShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            DefendShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

        for( i = 0; i < MAX_EXPLODE_PER_SHIP; ++i )
        {
            // Setup the explode matrix
            ExplodeMatrix[i].Rotate( 0, CrashOffset, Rotate );
            ExplodeMatrix[i].Translate( 0, Move_3 + ExplodeY[i], ExplodeZ[i] );
            ExplodeMatrix[i].MergeMatrix( defendMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                Explode.PaintVBuffer8To8( ExplodeMatrix[i], POS_ZBUFFER );
            else
                Explode.PaintVBuffer8To16( ExplodeMatrix[i], POS_ZBUFFER );
        }
    }

    Rotate += (50  * RotDir);

    if( AnimIndex == 2 || AnimIndex == 3 )
        Move_3 -= 10;
    else
        Move_3 -= 15;

    // Do we show the laser blast
    if( LaserCounter[0] == 0 && (rand() % 3) )
    {
        ++LaserCounter[0];

        // Reinit the laser
        laserMatrix[0].InitilizeMatrix();

        // Who is the aggresser
        if( (Direction == 1 && Z_MoveAry[0] > Z_MoveAry[1]) ||
            (Direction == -1 && Z_MoveAry[0] < Z_MoveAry[1]) )
        {
            laserMatrix[0].Translate( shipMoveLR[Move_2]*4, shipMoveLR[Move_2], 0 );
            laserMatrix[0].Rotate( 0, 0, shipMoveLR[Move_2] );
            laserMatrix[0].MergeMatrix( defendMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                DefendLaser.PaintVBuffer8To8( laserMatrix[0], POS_ZBUFFER );
            else
                DefendLaser.PaintVBuffer8To16( laserMatrix[0], POS_ZBUFFER );
        }
        else
        {
            laserMatrix[0].Translate( shipMoveLR[Move_1]*5, shipMoveLR[Move_1], 0 );
            laserMatrix[0].Rotate( 0, 0, shipMoveLR[Move_1] );
            laserMatrix[0].MergeMatrix( attackMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                AttackLaser.PaintVBuffer8To8( laserMatrix[0], POS_ZBUFFER );
            else
                AttackLaser.PaintVBuffer8To16( laserMatrix[0], POS_ZBUFFER );
        }
    }
    // Draw the laser blast going off into the distance
    else if( LaserCounter[0] )
    {
        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the laser down the line
        FinalMatrix.Translate( 0, 0, LaserCounter[0] * 700 );
        FinalMatrix.MergeMatrix( laserMatrix[0].Matrix );

        // Who is the aggresser
        if( (Direction == 1 && Z_MoveAry[0] > Z_MoveAry[1]) ||
            (Direction == -1 && Z_MoveAry[0] < Z_MoveAry[1]) )
        {        
            // Paint the buffer
            if( SystemBitDepth == 8 )
                DefendLaserOnly.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
            else
                DefendLaserOnly.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
        }
        else
        {        
            // Paint the buffer
            if( SystemBitDepth == 8 )
                AttackLaserOnly.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
            else
                AttackLaserOnly.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
        }

        // Inc the laser counter
        LaserCounter[0] = (LaserCounter[0] + 1) % 5;
    }

    // inc the counters
    Move_1 = (Move_1 + 1) % SHIP_MOVE_SIDE_TO_SIDE;
    Move_2 = (Move_2 + 1) % SHIP_MOVE_SIDE_TO_SIDE;

    // Reinit the matrix
    FinalMatrix.InitilizeMatrix();

    // Move the mother ship's Z access
    FinalMatrix.Translate( 0, 0, Z_Move );
    FinalMatrix.MergeMatrix( motherMatrix.Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        MotherShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        MotherShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = AttackList.GetInt( AttackSceneCounter );

        // Inc to the next attack scene
        AttackSceneCounter = ( AttackSceneCounter + 1 ) % AttackList.Count();

        // Reset the flags that picks if we are to 
        // fight or explode on the top or bottom
        PickFightExplode = -1;
        PickTopOrBottom = -1;

        // Now mix up all the ints
        if( AttackSceneCounter == 0 )
            AttackList.MixUp();

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_FlySideExplode


/************************************************************************
*    FUNCTION NAME:         Scene_FlyTopExplode                                                             
*
*    DESCRIPTION:           Camera top fly by explode one of the fighters
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

void CAnimateSaver::Scene_FlyTopExplode()
{
    Matrix3D FinalMatrix, ExplodeMatrix[MAX_EXPLODE_PER_SHIP];
    int i;

    // Do scene start initilizations
    if( setUpScene )
    {
        int Y, Index;
        int cameraXTran[MAX_FLYBY_VIEWS] = {0,350,-350,500,232};
        const int cameraYTran[MAX_FLYBY_VIEWS] = {-5698,-5789,-5789,-5301,-6200};
        int cameraZTran[MAX_FLYBY_VIEWS] = {14400,11750,20000,20000,11200};
        const int cameraXRot[MAX_FLYBY_VIEWS] = {-120,-174,-151,-59,-244};
        int cameraYRot[MAX_FLYBY_VIEWS] = {0,-129,116,600,510};
        int shipZOffset[MAX_FLYBY_VIEWS] = {500,-100,100,-2000,-800};
        const int TopBottomLeftRight[2] = {1,-1};

        // Pick the camers angle
        Index = rand() % MAX_FLYBY_VIEWS;

        // Choose the movement direction
        if( rand() % 2 )    // Face the nose of the mother ship
        {
            Movement = 1;

            if( rand() % 2 )  // Going to noze
            {
                MoveAmount = -600;
                Direction = 1;

                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( Index == 2 )
                    cameraZTran[Index] = 12000;
                else if( Index == 3 )
                    cameraZTran[Index] = 10000;
            }
            else     // Going away from noze
            {
                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( Index == 0 )
                    cameraZTran[Index] = 19000;
                else if( Index == 1 )
                    cameraZTran[Index] = 20000;
                else if( Index == 4 )
                    cameraZTran[Index] = 21000;

                Direction = -1;
                MoveAmount = 600;
                cameraZTran[Index] *= -1;
            }
        }
        else                // Face the back of the mother ship
        {
            Movement = -1;
            
            if( rand() % 2 )       // Going to back
            {
                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( Index == 0 )
                    cameraZTran[Index] = 27000;
                else if( Index == 1 )
                    cameraZTran[Index] = 24000;
                else if( Index == 2 )
                    cameraZTran[Index] = 27000;
                else if( Index == 4 )
                    cameraZTran[Index] = 21000;

                Direction = -1;
                MoveAmount = 600;
                cameraXTran[Index] *= -1;
                cameraZTran[Index] *= -1;
                cameraYRot[Index] += 512;
            }
            else      // Going away from back
            {
                Direction = 1;
                MoveAmount = -600;
                cameraYRot[Index] += 512;
                cameraXTran[Index] *= -1;

                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( Index == 0 || Index == 1 )
                    cameraZTran[Index] = 8000;
                else if( Index == 2 )
                    cameraZTran[Index] = 1000;
                else if( Index == 3 )
                    cameraZTran[Index] = 12500;
            }
        }

        // Init the starting position to the matrix
        motherMatrix.InitilizeMatrix();
        motherMatrix.Translate( cameraXTran[Index], cameraYTran[Index], cameraZTran[Index] );
        motherMatrix.Rotate( cameraXRot[Index], cameraYRot[Index], 0 );

        // Init the starting position to the matrix
        attackMatrix[0].InitilizeMatrix();
        defendMatrix[0].InitilizeMatrix();
        LaserCounter[0] = 0;

        // rotate the ship if we are going in the other direction
        if( Direction == -1 )
        {
            attackMatrix[0].Rotate( 0, 512, 0 );                
            defendMatrix[0].Rotate( 0, 512, 0 );
        }

        // Randomize the Y offset
        Y = 5075;

        // Pick which will be the aggressive ship
        if( rand() % 2 )
        {
            Z_MoveAry[0] = shipZOffset[Index] * Movement;
            Z_MoveAry[1] = (shipZOffset[Index] + 1310) * Movement;
        }
        else
        {
            Z_MoveAry[0] = (shipZOffset[Index] + 1310) * Movement;
            Z_MoveAry[1] = shipZOffset[Index] * Movement;
        }

        // Randomize the explosion offsets
        for( i = 0; i < MAX_EXPLODE_PER_SHIP; ++i )
        {
            ExplodeY[i] = (rand() % 51) * TopBottomLeftRight[rand() % 2];
            ExplodeZ[i] = (rand() % 151) * TopBottomLeftRight[rand() % 2];
        }

        // Pick the rotation direction
        RotDir = TopBottomLeftRight[rand() % 2];

        attackMatrix[0].Translate( cameraXTran[Index], cameraYTran[Index] + Y, Z_MoveAry[0] );
        attackMatrix[0].Rotate( cameraXRot[Index], cameraYRot[Index], 0 );

        defendMatrix[0].Translate( cameraXTran[Index], cameraYTran[Index] + Y, Z_MoveAry[1] );
        defendMatrix[0].Rotate( cameraXRot[Index], cameraYRot[Index], 0 );
        
        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 50;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Z axis move counter
        Z_Move = 0;
        Move_1 = 0;
        Rotate = 0;
        Move_3 = 0;
        Move_2 = SHIP_MOVE_SIDE_TO_SIDE / 2;

        // Set the Y rotation offset for the crash
        CrashOffset = rand() % MAX_CRASH_OFFSET;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (starBackground + cloudBufferOffset), 
            (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Move the mother ship's Z access
    Z_Move += MoveAmount;

    // Who is the aggresser
    if( (Direction == 1 && Z_MoveAry[0] > Z_MoveAry[1]) ||
        (Direction == -1 && Z_MoveAry[0] < Z_MoveAry[1]) )
    {
        // Move the attacking ship
        FinalMatrix.Rotate( 0, CrashOffset, Rotate );
        FinalMatrix.Translate( 0, Move_3, 0 );
        FinalMatrix.MergeMatrix( attackMatrix[0].Matrix );

        // Paint the buffer
        if( SystemBitDepth == 8 )
            AttackShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            AttackShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

        for( i = 0; i < MAX_EXPLODE_PER_SHIP; ++i )
        {
            // Setup the explode matrix
            ExplodeMatrix[i].Rotate( 0, CrashOffset, Rotate );
            ExplodeMatrix[i].Translate( 0, Move_3 + ExplodeY[i], ExplodeZ[i] );
            ExplodeMatrix[i].MergeMatrix( attackMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                Explode.PaintVBuffer8To8( ExplodeMatrix[i], POS_ZBUFFER );
            else
                Explode.PaintVBuffer8To16( ExplodeMatrix[i], POS_ZBUFFER );
        }

        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the mother ship's Z access
        FinalMatrix.Translate( shipMoveLR[Move_2]*4, shipMoveLR[Move_2], 0 );
        FinalMatrix.Rotate( 0, 0, shipMoveLR[Move_2] );
        FinalMatrix.MergeMatrix( defendMatrix[0].Matrix );


        // Paint the buffer
        if( SystemBitDepth == 8 )
            DefendShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            DefendShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
    }
    else
    {
        // Move the mother ship's Z access
        FinalMatrix.Translate( shipMoveLR[Move_1]*5, shipMoveLR[Move_1], 0 );
        FinalMatrix.Rotate( 0, 0, shipMoveLR[Move_1] );
        FinalMatrix.MergeMatrix( attackMatrix[0].Matrix );

        // Paint the buffer
        if( SystemBitDepth == 8 )
            AttackShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            AttackShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the mother ship's Z access
        FinalMatrix.Rotate( 0, CrashOffset, Rotate );
        FinalMatrix.Translate( 0, Move_3, 0 );
        FinalMatrix.MergeMatrix( defendMatrix[0].Matrix );

        // Paint the buffer
        if( SystemBitDepth == 8 )
            DefendShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            DefendShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

        for( i = 0; i < MAX_EXPLODE_PER_SHIP; ++i )
        {
            // Setup the explode matrix
            ExplodeMatrix[i].Rotate( 0, CrashOffset, Rotate );
            ExplodeMatrix[i].Translate( 0, Move_3 + ExplodeY[i], ExplodeZ[i] );
            ExplodeMatrix[i].MergeMatrix( defendMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                Explode.PaintVBuffer8To8( ExplodeMatrix[i], POS_ZBUFFER );
            else
                Explode.PaintVBuffer8To16( ExplodeMatrix[i], POS_ZBUFFER );
        }
    }

    Rotate += (50 * RotDir);

    Move_3 += 10;

    // Do we show the laser blast
    if( LaserCounter[0] == 0 && (rand() % 3) )
    {
        ++LaserCounter[0];

        // Reinit the laser
        laserMatrix[0].InitilizeMatrix();

        // Who is the aggresser
        if( (Direction == 1 && Z_MoveAry[0] > Z_MoveAry[1]) ||
            (Direction == -1 && Z_MoveAry[0] < Z_MoveAry[1]) )
        {
            laserMatrix[0].Translate( shipMoveLR[Move_2]*5, shipMoveLR[Move_2], 0 );
            laserMatrix[0].Rotate( 0, 0, shipMoveLR[Move_2] );
            laserMatrix[0].MergeMatrix( defendMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                DefendLaser.PaintVBuffer8To8( laserMatrix[0], POS_ZBUFFER );
            else
                DefendLaser.PaintVBuffer8To16( laserMatrix[0], POS_ZBUFFER );
        }
        else
        {
            laserMatrix[0].Translate( shipMoveLR[Move_1]*6, shipMoveLR[Move_1], 0 );
            laserMatrix[0].Rotate( 0, 0, shipMoveLR[Move_1] );
            laserMatrix[0].MergeMatrix( attackMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                AttackLaser.PaintVBuffer8To8( laserMatrix[0], POS_ZBUFFER );
            else
                AttackLaser.PaintVBuffer8To16( laserMatrix[0], POS_ZBUFFER );
        }
    }
    // Draw the laser blast going off into the distance
    else if( LaserCounter[0] )
    {
        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the laser down the line
        FinalMatrix.Translate( 0, 0, LaserCounter[0] * 700 );
        FinalMatrix.MergeMatrix( laserMatrix[0].Matrix );

        // Who is the aggresser
        if( (Direction == 1 && Z_MoveAry[0] > Z_MoveAry[1]) ||
            (Direction == -1 && Z_MoveAry[0] < Z_MoveAry[1]) )
        {        
            // Paint the buffer
            if( SystemBitDepth == 8 )
                DefendLaserOnly.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
            else
                DefendLaserOnly.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
        }
        else
        {        
            // Paint the buffer
            if( SystemBitDepth == 8 )
                AttackLaserOnly.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
            else
                AttackLaserOnly.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
        }

        // Inc the laser counter
        LaserCounter[0] = (LaserCounter[0] + 1) % 5;
    }

    // inc the counters
    Move_1 = (Move_1 + 1) % SHIP_MOVE_SIDE_TO_SIDE;
    Move_2 = (Move_2 + 1) % SHIP_MOVE_SIDE_TO_SIDE;

    // Reinit the matrix
    FinalMatrix.InitilizeMatrix();

    // Move the mother ship's Z access
    FinalMatrix.Translate( 0, 0, Z_Move );
    FinalMatrix.MergeMatrix( motherMatrix.Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        MotherShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        MotherShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = AttackList.GetInt( AttackSceneCounter );

        // Inc to the next attack scene
        AttackSceneCounter = ( AttackSceneCounter + 1 ) % AttackList.Count();

        // Reset the flags that picks if we are to 
        // fight or explode on the top or bottom
        PickFightExplode = -1;
        PickTopOrBottom = -1;

        // Now mix up all the ints
        if( AttackSceneCounter == 0 )
            AttackList.MixUp();

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_FlyTopExplode


/************************************************************************
*    FUNCTION NAME:         Scene_FlyBottomExplode                                                             
*
*    DESCRIPTION:           Camera bottom fly by explode one of the fighters
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

void CAnimateSaver::Scene_FlyBottomExplode()
{
    Matrix3D FinalMatrix, ExplodeMatrix[MAX_EXPLODE_PER_SHIP];
    int i;

    // Do scene start initilizations
    if( setUpScene )
    {
        int Y, Index;
        int cameraXTran[MAX_FLYBY_VIEWS] = {0,350,-350,500,232};
        const int cameraYTran[MAX_FLYBY_VIEWS] = {5698,5889,5789,5301,6200};
        int cameraZTran[MAX_FLYBY_VIEWS] = {18000,18000,16000,19000,11200};
        const int cameraXRot[MAX_FLYBY_VIEWS] = {120,174,151,59,244};
        int cameraYRot[MAX_FLYBY_VIEWS] = {0,-129,116,600,510};
        int shipZOffset[MAX_FLYBY_VIEWS] = {500,-100,100,-1600,-800};
        const int TopBottomLeftRight[2] = {1,-1};

        // Pick the camers angle
        Index = rand() % MAX_FLYBY_VIEWS;

        // Choose the movement direction
        if( rand() % 2 )    // Face the nose of the mother ship
        {
            Movement = 1;

            if( rand() % 2 )  // Going to noze
            {
                MoveAmount = -600;
                Direction = 1;

                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( Index == 3 )
                    cameraZTran[Index] = 15000;
            }
            else     // Going away from noze
            {
                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( Index == 3 )
                    cameraZTran[Index] = 20000;

                Direction = -1;
                MoveAmount = 600;
                cameraZTran[Index] *= -1;
            }
        }
        else                // Face the back of the mother ship
        {
            Movement = -1;
            
            if( rand() % 2 )       // Going to back
            {
                // Adjust the Z offset depending on the index
                // This is to avoid seeing glitches in the polygons
                if( Index == 1 || Index == 3 )
                    cameraZTran[Index] = 15000;

                Direction = -1;
                MoveAmount = 600;
                cameraXTran[Index] *= -1;
                cameraZTran[Index] *= -1;
                cameraYRot[Index] += 512;
            }
            else      // Going away from back
            {
                Direction = 1;
                MoveAmount = -600;
                cameraYRot[Index] += 512;
                cameraXTran[Index] *= -1;
            }
        }

        // Init the starting position to the matrix
        motherMatrix.InitilizeMatrix();
        motherMatrix.Translate( cameraXTran[Index], cameraYTran[Index], cameraZTran[Index] );
        motherMatrix.Rotate( cameraXRot[Index], cameraYRot[Index], 0 );

        // Init the starting position to the matrix
        attackMatrix[0].InitilizeMatrix();
        defendMatrix[0].InitilizeMatrix();
        LaserCounter[0] = 0;

        // rotate the ship if we are going in the other direction
        if( Direction == -1 )
        {
            attackMatrix[0].Rotate( 0, 512, 0 );                
            defendMatrix[0].Rotate( 0, 512, 0 );
        }

        // Randomize the Y offset
        Y = -5075;

        // Pick which will be the aggressive ship
        if( rand() % 2 )
        {
            Z_MoveAry[0] = shipZOffset[Index] * Movement;
            Z_MoveAry[1] = (shipZOffset[Index] + 1310) * Movement;
        }
        else
        {
            Z_MoveAry[0] = (shipZOffset[Index] + 1310) * Movement;
            Z_MoveAry[1] = shipZOffset[Index] * Movement;
        }

        // Randomize the explosion offsets
        for( i = 0; i < MAX_EXPLODE_PER_SHIP; ++i )
        {
            ExplodeY[i] = (rand() % 51) * TopBottomLeftRight[rand() % 2];
            ExplodeZ[i] = (rand() % 151) * TopBottomLeftRight[rand() % 2];
        }

        // Pick the rotation direction
        RotDir = TopBottomLeftRight[rand() % 2];

        attackMatrix[0].Translate( cameraXTran[Index], cameraYTran[Index] + Y, Z_MoveAry[0] );
        attackMatrix[0].Rotate( cameraXRot[Index], cameraYRot[Index], 0 );

        defendMatrix[0].Translate( cameraXTran[Index], cameraYTran[Index] + Y, Z_MoveAry[1] );
        defendMatrix[0].Rotate( cameraXRot[Index], cameraYRot[Index], 0 );
        
        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 50;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Z axis move counter
        Z_Move = 0;
        Move_1 = 0;
        Rotate = 0;
        Move_3 = 0;
        Move_2 = SHIP_MOVE_SIDE_TO_SIDE / 2;

        // Set the Y rotation offset for the crash
        CrashOffset = rand() % MAX_CRASH_OFFSET;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (starBackground + cloudBufferOffset), 
            (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Move the mother ship's Z access
    Z_Move += MoveAmount;

    // Who is the aggresser
    if( (Direction == 1 && Z_MoveAry[0] > Z_MoveAry[1]) ||
        (Direction == -1 && Z_MoveAry[0] < Z_MoveAry[1]) )
    {
        // Move the attacking ship
        FinalMatrix.Rotate( 0, CrashOffset, Rotate );
        FinalMatrix.Translate( 0, Move_3, 0 );
        FinalMatrix.MergeMatrix( attackMatrix[0].Matrix );

        // Paint the buffer
        if( SystemBitDepth == 8 )
            AttackShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            AttackShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

        for( i = 0; i < MAX_EXPLODE_PER_SHIP; ++i )
        {
            // Setup the explode matrix
            ExplodeMatrix[i].Rotate( 0, CrashOffset, Rotate );
            ExplodeMatrix[i].Translate( 0, Move_3 + ExplodeY[i], ExplodeZ[i] );
            ExplodeMatrix[i].MergeMatrix( attackMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                Explode.PaintVBuffer8To8( ExplodeMatrix[i], POS_ZBUFFER );
            else
                Explode.PaintVBuffer8To16( ExplodeMatrix[i], POS_ZBUFFER );
        }

        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the mother ship's Z access
        FinalMatrix.Translate( shipMoveLR[Move_2]*4, shipMoveLR[Move_2], 0 );
        FinalMatrix.Rotate( 0, 0, shipMoveLR[Move_2] );
        FinalMatrix.MergeMatrix( defendMatrix[0].Matrix );


        // Paint the buffer
        if( SystemBitDepth == 8 )
            DefendShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            DefendShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
    }
    else
    {
        // Move the mother ship's Z access
        FinalMatrix.Translate( shipMoveLR[Move_1]*5, shipMoveLR[Move_1], 0 );
        FinalMatrix.Rotate( 0, 0, shipMoveLR[Move_1] );
        FinalMatrix.MergeMatrix( attackMatrix[0].Matrix );

        // Paint the buffer
        if( SystemBitDepth == 8 )
            AttackShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            AttackShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the mother ship's Z access
        FinalMatrix.Rotate( 0, CrashOffset, Rotate );
        FinalMatrix.Translate( 0, Move_3, 0 );
        FinalMatrix.MergeMatrix( defendMatrix[0].Matrix );

        // Paint the buffer
        if( SystemBitDepth == 8 )
            DefendShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            DefendShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

        for( i = 0; i < MAX_EXPLODE_PER_SHIP; ++i )
        {
            // Setup the explode matrix
            ExplodeMatrix[i].Rotate( 0, CrashOffset, Rotate );
            ExplodeMatrix[i].Translate( 0, Move_3 + ExplodeY[i], ExplodeZ[i] );
            ExplodeMatrix[i].MergeMatrix( defendMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                Explode.PaintVBuffer8To8( ExplodeMatrix[i], POS_ZBUFFER );
            else
                Explode.PaintVBuffer8To16( ExplodeMatrix[i], POS_ZBUFFER );
        }
    }

    Rotate += (50 * RotDir);
    Move_3 -= 10;

    // Do we show the laser blast
    if( LaserCounter[0] == 0 && (rand() % 3) )
    {
        ++LaserCounter[0];

        // Reinit the laser
        laserMatrix[0].InitilizeMatrix();

        // Who is the aggresser
        if( (Direction == 1 && Z_MoveAry[0] > Z_MoveAry[1]) ||
            (Direction == -1 && Z_MoveAry[0] < Z_MoveAry[1]) )
        {
            laserMatrix[0].Translate( shipMoveLR[Move_2]*5, shipMoveLR[Move_2], 0 );
            laserMatrix[0].Rotate( 0, 0, shipMoveLR[Move_2] );
            laserMatrix[0].MergeMatrix( defendMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                DefendLaser.PaintVBuffer8To8( laserMatrix[0], POS_ZBUFFER );
            else
                DefendLaser.PaintVBuffer8To16( laserMatrix[0], POS_ZBUFFER );
        }
        else
        {
            laserMatrix[0].Translate( shipMoveLR[Move_1]*6, shipMoveLR[Move_1], 0 );
            laserMatrix[0].Rotate( 0, 0, shipMoveLR[Move_1] );
            laserMatrix[0].MergeMatrix( attackMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                AttackLaser.PaintVBuffer8To8( laserMatrix[0], POS_ZBUFFER );
            else
                AttackLaser.PaintVBuffer8To16( laserMatrix[0], POS_ZBUFFER );
        }
    }
    // Draw the laser blast going off into the distance
    else if( LaserCounter[0] )
    {
        // Reinit the matrix
        FinalMatrix.InitilizeMatrix();

        // Move the laser down the line
        FinalMatrix.Translate( 0, 0, LaserCounter[0] * 700 );
        FinalMatrix.MergeMatrix( laserMatrix[0].Matrix );

        // Who is the aggresser
        if( (Direction == 1 && Z_MoveAry[0] > Z_MoveAry[1]) ||
            (Direction == -1 && Z_MoveAry[0] < Z_MoveAry[1]) )
        {        
            // Paint the buffer
            if( SystemBitDepth == 8 )
                DefendLaserOnly.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
            else
                DefendLaserOnly.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
        }
        else
        {        
            // Paint the buffer
            if( SystemBitDepth == 8 )
                AttackLaserOnly.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
            else
                AttackLaserOnly.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );
        }

        // Inc the laser counter
        LaserCounter[0] = (LaserCounter[0] + 1) % 5;
    }

    // inc the counters
    Move_1 = (Move_1 + 1) % SHIP_MOVE_SIDE_TO_SIDE;
    Move_2 = (Move_2 + 1) % SHIP_MOVE_SIDE_TO_SIDE;

    // Reinit the matrix
    FinalMatrix.InitilizeMatrix();

    // Move the mother ship's Z access
    FinalMatrix.Translate( 0, 0, Z_Move );
    FinalMatrix.MergeMatrix( motherMatrix.Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        MotherShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        MotherShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = AttackList.GetInt( AttackSceneCounter );

        // Inc to the next attack scene
        AttackSceneCounter = ( AttackSceneCounter + 1 ) % AttackList.Count();

        // Reset the flags that picks if we are to 
        // fight or explode on the top or bottom
        PickFightExplode = -1;
        PickTopOrBottom = -1;

        // Now mix up all the ints
        if( AttackSceneCounter == 0 )
            AttackList.MixUp();

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_FlyBottomExplode


/************************************************************************
*    FUNCTION NAME:         Scene_CloseUpFlyOver                                                             
*
*    DESCRIPTION:           Camera close up on ship
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

void CAnimateSaver::Scene_CloseUpFlyOver()
{
    Matrix3D FinalMatrix, ExplodeMatrix[MAX_EXPLODE_PER_SHIP];
    int i;

    // Do scene start initilizations
    if( setUpScene )
    {
        int Index;
        int cameraXTran[MAX_CLOSE_UP_VIEWS] = {-326, 417, 250, -250, 250, -250};
        int cameraYTran[MAX_CLOSE_UP_VIEWS] = {-5851,5923,-5373,-5373,6230,6230};
        //int cameraYTran[MAX_CLOSE_UP_VIEWS] = {-4851,4723,-5173,-5173,4730,4730};
        int cameraZTran[MAX_CLOSE_UP_VIEWS] = {14400,14400,14400,14400,14400,14400};
        int cameraXRot[MAX_CLOSE_UP_VIEWS]  = {-205,181,-181,-181,181,181};
        int cameraYRot[MAX_CLOSE_UP_VIEWS]  = {27,-42,-183,183,-183,183};
        
        int shipXOffset[MAX_CLOSE_UP_VIEWS] = {311,-313,0,0,0,0};
        int shipYOffset[MAX_CLOSE_UP_VIEWS] = {5448,-5559,4951,4951,-5850,-5850};
        //int shipYOffset[MAX_CLOSE_UP_VIEWS] = {4528,-4419,4851,4851,-4450,-4450};
        int shipZOffset[MAX_CLOSE_UP_VIEWS] = {0,100,0,0,0,0};

        int cameraYAdd[MAX_CLOSE_UP_VIEWS] = {130,70,50,50,50,50};

        const int TopBottomLeftRight[2] = {1,-1};

        // Pick the camers angle
        Index = rand() % MAX_CLOSE_UP_VIEWS;

        // Pick if we want the ship to shoot or explade
        fExplode = rand() % 2;

        // Randomly pick a ship and it's laser
        if( !(rand() % 2) )
        {
            pShip = &AttackShip;
            pLaser = &AttackLaser;
        }
        else
        {
            pShip = &DefendShip;
            pLaser = &DefendLaser;
        }

        // Init the explode stuff if we are to explode
        if( fExplode )
        {
            // Randomize the explosion offsets
            for( i = 0; i < MAX_EXPLODE_PER_SHIP; ++i )
            {
                ExplodeY[i] = (rand() % 51) * TopBottomLeftRight[rand() % 2];
                ExplodeZ[i] = (rand() % 151) * TopBottomLeftRight[rand() % 2];
            }

            // Pick the rotation direction
            RotDir = TopBottomLeftRight[rand() % 2];

            // Set the Y rotation offset for the crash
            CrashOffset = rand() % MAX_CRASH_OFFSET;

            // Are we on the top or bottom
            if( cameraYTran[Index] > 0 )
            {
                Y_Move = -1;

                // Add more to the cameras Y offset because we need room
                // for the spinning out of control ship.
                cameraYTran[Index] += cameraYAdd[Index];
            }
            else
            {
                Y_Move = 1;

                // Add more to the cameras Y offset because we need room
                // for the spinning out of control ship.
                cameraYTran[Index] -= cameraYAdd[Index];
            }
        }

        // Choose the movement direction
        if( rand() % 2 )    // Face the nose of the mother ship
        {
            Movement = 1;

            if( rand() % 2 )  // Going to noze
            {
                MoveAmount = -600;
                Direction = 1;
                shipZOffset[Index] += 100;
            }
            else     // Going away from noze
            {
                cameraZTran[Index] = 20000;

                Direction = -1;
                MoveAmount = 600;
                cameraZTran[Index] *= -1;
                shipZOffset[Index] += 200;
            }
        }
        else                // Face the back of the mother ship
        {
            Movement = -1;
            
            if( rand() % 2 )       // Going to back
            {
                cameraZTran[Index] = 27000;

                Direction = -1;
                MoveAmount = 600;
                cameraXTran[Index] *= -1;
                cameraZTran[Index] *= -1;
                cameraYRot[Index] += 512;
                shipXOffset[Index] *= -1;
                shipZOffset[Index] += 100;
            }
            else      // Going away from back
            {
                Direction = 1;
                MoveAmount = -600;
                cameraYRot[Index] += 512;
                cameraXTran[Index] *= -1;
                shipXOffset[Index] *= -1;
                shipZOffset[Index] += 200;

                cameraZTran[Index] = 11000;
            }
        }

        // Init the starting position to the matrix
        motherMatrix.InitilizeMatrix();
        motherMatrix.Translate( cameraXTran[Index], cameraYTran[Index], cameraZTran[Index] );
        motherMatrix.Rotate( cameraXRot[Index], cameraYRot[Index], 0 );

        // Init the starting position to the matrix
        attackMatrix[0].InitilizeMatrix();
        LaserCounter[0] = 0;

        // rotate the ship if we are going in the other direction
        if( Direction == -1 )
            attackMatrix[0].Rotate( 0, 512, 0 );

        Z_MoveAry[0] = shipZOffset[Index] * Movement;

        attackMatrix[0].Translate( cameraXTran[Index] + shipXOffset[Index], cameraYTran[Index] + shipYOffset[Index], Z_MoveAry[0] );
        attackMatrix[0].Rotate( cameraXRot[Index], cameraYRot[Index], 0 );
        
        // Init the max scene counter to the number of times
        // we will go into this function
        animSceneMaxCount = 60;

        // Init the counter to zero to start
        animSceneCounter = 0;

        // Set the Z axis move counter
        Z_Move = 0;
        Move_1 = 0;
        Rotate = 0;
        Move_3 = 0;

        // Set the cloud buffer offset
        cloudBufferOffset = (RnderRect.right * (rand() % RnderRect.bottom)) * (SystemBitDepth / 8);

        // Setup is done
        setUpScene = FALSE;
    }

    // Zero out the ZBuffer
    //memset( ZBuffer, 0, sizeof(int)*(RnderRect.right * RnderRect.bottom) );
    memsetQuad( ZBuffer, 0, RnderRect.right * RnderRect.bottom );

    // Clear out the video buffer with the cloud buffer
    memcpy( buffer.GetPtrToDIBsectionBits(), (starBackground + cloudBufferOffset), 
            (RnderRect.right * RnderRect.bottom) * (SystemBitDepth / 8) );

    // Move the mother ship's Z access
    Z_Move += MoveAmount;

    // Do we shoot or explode
    if( fExplode )
    {
        // Move the attacking ship
        FinalMatrix.Rotate( 0, CrashOffset, Rotate );
        FinalMatrix.Translate( 0, Move_3, 0 );
        FinalMatrix.MergeMatrix( attackMatrix[0].Matrix );

        // Paint the buffer
        if( SystemBitDepth == 8 )
            pShip->PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            pShip->PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

        for( i = 0; i < MAX_EXPLODE_PER_SHIP; ++i )
        {
            // Setup the explode matrix
            ExplodeMatrix[i].Rotate( 0, CrashOffset, Rotate );
            ExplodeMatrix[i].Translate( 0, Move_3 + ExplodeY[i], ExplodeZ[i] );
            ExplodeMatrix[i].MergeMatrix( attackMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                Explode.PaintVBuffer8To8( ExplodeMatrix[i], POS_ZBUFFER );
            else
                Explode.PaintVBuffer8To16( ExplodeMatrix[i], POS_ZBUFFER );
        }

        Rotate += (50 * RotDir);
        Move_3 += 3 * Y_Move;
    }
    else
    {
        // Move the mother ship's Z access
        FinalMatrix.Translate( shipMoveLR[Move_1]*2, 0, 0 );
        FinalMatrix.Rotate( 0, 0, shipMoveLR[Move_1] );
        FinalMatrix.MergeMatrix( attackMatrix[0].Matrix );

        // Paint the buffer
        if( SystemBitDepth == 8 )
            pShip->PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
        else
            pShip->PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

        // Do we show the laser blast
        if( LaserCounter[0] == 0 && (rand() % 2) )
        {
            ++LaserCounter[0];

            // Reinit the laser
            laserMatrix[0].InitilizeMatrix();

            // Who is the aggresser
            laserMatrix[0].Translate( shipMoveLR[Move_1]*2, 0, 0 );
            laserMatrix[0].Rotate( 0, 0, shipMoveLR[Move_1] );
            laserMatrix[0].MergeMatrix( attackMatrix[0].Matrix );

            // Paint the buffer
            if( SystemBitDepth == 8 )
                pLaser->PaintVBuffer8To8( laserMatrix[0], POS_ZBUFFER );
            else
                pLaser->PaintVBuffer8To16( laserMatrix[0], POS_ZBUFFER );
        }
        // Draw the laser blast going off into the distance
        else if( LaserCounter[0] )
        {
            // Inc the laser counter
            LaserCounter[0] = (LaserCounter[0] + 1) % 5;
        }

        // inc the counters
        Move_1 = (Move_1 + 1) % SHIP_MOVE_SIDE_TO_SIDE;
    }

    // Reinit the matrix
    FinalMatrix.InitilizeMatrix();

    // Move the mother ship's Z access
    FinalMatrix.Translate( 0, 0, Z_Move );
    FinalMatrix.MergeMatrix( motherMatrix.Matrix );

    // Paint the buffer
    if( SystemBitDepth == 8 )
        MotherShip.PaintVBuffer8To8( FinalMatrix, POS_ZBUFFER );
    else
        MotherShip.PaintVBuffer8To16( FinalMatrix, POS_ZBUFFER );

    // Check to see if we are done with this scene
    if( animSceneCounter >= animSceneMaxCount )
    {
        // init to the next scene
        animSceneState = AttackList.GetInt( AttackSceneCounter );

        // Inc to the next attack scene
        AttackSceneCounter = ( AttackSceneCounter + 1 ) % AttackList.Count();

        // Reset the flags that picks if we are to 
        // fight or explode on the top or bottom
        PickFightExplode = -1;
        PickTopOrBottom = -1;

        // Now mix up all the ints
        if( AttackSceneCounter == 0 )
            AttackList.MixUp();

        // Set the flag to setup the scene
        setUpScene = TRUE;
    }

    ++animSceneCounter;

}   // Scene_CloseUpFlyOver