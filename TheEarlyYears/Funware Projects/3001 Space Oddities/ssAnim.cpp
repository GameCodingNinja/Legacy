
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


// Init data for the stars
const RECT starArtRect[MAX_STAR_TYPES] = { {   0, 0, 21, 21 },
                                           {  21, 0, 17, 17 },
                                           {  38, 0, 15, 15 },
                                           {  53, 0, 15, 15 },
                                           {  68, 0, 11, 11 },
                                           {  79, 0, 11, 11 },
                                           {  68, 0, 11, 11 },
                                           {  79, 0, 11, 11 },
                                           {  68, 0, 11, 11 },
                                           {  79, 0, 11, 11 },
                                           {  90, 0,  9,  9 },
                                           {  99, 0,  9,  9 },
                                           { 108, 0,  9,  9 },
                                           { 117, 0,  9,  9 },
                                           { 126, 0,  9,  9 },
                                           { 135, 0,  9,  9 },
                                           { 144, 0,  9,  9 },
                                           { 153, 0,  9,  9 },
                                           { 162, 0,  9,  9 },
                                           {  90, 0,  9,  9 },
                                           {  99, 0,  9,  9 },
                                           { 108, 0,  9,  9 },
                                           { 117, 0,  9,  9 },
                                           { 126, 0,  9,  9 },
                                           { 135, 0,  9,  9 },
                                           { 144, 0,  9,  9 },
                                           { 153, 0,  9,  9 },
                                           { 162, 0,  9,  9 } };

// Init data for the planets
const RECT planetArtRect[NUM_PLANETS] = { {   0,  43,  35,  35 },
                                          {  36,  34,  45,  42 },
                                          {   0, 113,  33,  34 },
                                          {  34, 118,  46,  47 },
                                          {  82,  23,  63,  63 },
                                          { 146,  19,  76,  79 },
                                          { 223,   0, 123, 124 },
                                          { 347,   0, 132, 133 },
                                          { 480,   0, 110, 112 },
                                          { 591,   0,  65,  64 },
                                          { 590, 128,  59,  60 },
                                          {   0, 212,  95,  95 },
                                          {  96, 177, 108, 109 },
                                          { 205, 248,  87,  86 },
                                          { 293, 266,  93,  92 },
                                          { 387, 266,  71,  73 },
                                          { 479, 224,  99,  99 },
                                          { 579, 248,  75,  76 } };



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
    pStarData = NULL;
    StarCount = 0;
    NumPlanetsAnim = 0;
    NumShipsAnim = 0;
    PlanetPickTimer = 0;
    ShipPickTimer = 0;
    StarMovement = 0;

    // Init non-specific class members
    MusicTimer = 0;
    SongCount = 0;
    hOldPalette = NULL;
    Error_Number = 0;
    PreviewCounter = 0;
    AnimCounter = 0;
    ShareWareMode = false;
    ShareWareTimer = 0;
    SaverTimer = 0;

}   // Constructor


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

}   // LoadSpecificStartUpData


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
	int screenSize;

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

    #ifdef DEBUG
    ShareWareMode = FALSE;
    #endif

    // get the total screen size
    screenSize = DesktopWidth * DesktopHeight;

    switch( Settings.GetStarCount() )
    {
        case 0:  StarCount = screenSize / 4000;  break;
        case 1:  StarCount = screenSize / 2000;  break;
        case 2:  StarCount = screenSize / 1000;  break;
        case 3:  StarCount = screenSize / 500;   break;
        case 4:  StarCount = screenSize / 250;   break;
        case 5:  StarCount = screenSize / 125;   break;
        default: StarCount = screenSize / 1000;  break;
    }

    // Which direction are the stars moving?
    StarMovement = Settings.GetStarMovement();

    // If they selected the random option, we need to randomly
    // pick one
    if( StarMovement == MOVE_RANDOM )
        StarMovement = rand() %  NUM_OF_MOVEMENTS;
    
    return TRUE;

}   // LoadControlPanelPrefs


/************************************************************************
*    FUNCTION NAME:         LoadGraphics                                                             
*
*    DESCRIPTION:           Load the graphics. Using the standard compatable
*                           bitmap is very fast but in 256 color mode, can't
*                           apply the palette to make it work. But using a
*                           DibSection when in 8 bit mode is a lot slower
*                           but at least the palette get's realized.
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
    RECT SourceRect, DestRect;
    TDIB starsPlanetTemp;
    TBitmap cloudArt;
    TGif gif;           // GIF decoder which can load GIFs from file or resource
    TBitmapinfo fh;
    int CountLeft, CountTop, i, j;
    BOOL Result = FALSE, Load;
    // These are all the cloud resource names that we will randomly pick from.
    LPSTR CloudResources[NUM_CLOUDS] = { "RedClouds","BlueClouds","PurpleClouds",
                                         "OrangeClouds","BlueGreenClouds","RedBrownClouds" };

    // Load the space ship artwork if they want space ships
    if( Settings.GetSpaceShipsOnOff() )
    {
        // Init the classes using the screens DC. That way it doesn't have to make one
        ships.InitActiveDC( GetScreenDC() );

        // Load the ships GIF graphic from resource
        // Create the GDI bitmap
        if( !ships.LoadFromBuffer( gif.LoadFromResource( "Ships", "GIF", Instance() ) ) )
            goto HANDLE_ERROR;

        // Use one or the other depending on the color depth
        if( SystemBitDepth > BIT_COUNT_8 && Mode() == ssSaver )
            Load = ships.CreateBitmap( STANDARD_DIB );
        else
            Load = ships.CreateBitmap( DIB_SECTION_BMP, BIT_COUNT_16 );            
            
        if( !Load )
		{
            strcpy( errorStr, "Can't allocate buffer for space ships. Your system resources may be low so try restarting your computer." );
            goto HANDLE_ERROR;
        } 

        // Free up the bitmap data because we don't need it anymore
        ships.DeleteBitmapBits();
    }

    // Init the classes using the screens DC. That way it doesn't have to make one
    buffer.InitActiveDC( GetScreenDC() );

    // Make the buffer. We always need the buffer
    if( SystemBitDepth > BIT_COUNT_8 && Mode() == ssSaver )
	{
        if( !buffer.CreateStandardBuffer( DesktopWidth, DesktopHeight ) )
		{
			strcpy( errorStr, "Can't allocate background buffer. Your system resources may be low so try restarting your computer." );
			goto HANDLE_ERROR;
		}
	}
    else  // Use DIBSection for preview mode because XP doesn't lag so bad when doing a stretchdraw
	{
        if( !buffer.CreateDIBSectionBuffer( DesktopWidth, DesktopHeight, BIT_COUNT_16 ) )
		{
			strcpy( errorStr, "Can't allocate background buffer. Your system resources may be low so try restarting your computer." );
			goto HANDLE_ERROR;
		}
	}

    // Load the cloud art if it is selected and in a higher resolution then 8 bit
    // Cloud art is not supported on systems with 8 bit or less color
    if( SystemBitDepth > BIT_COUNT_8 )
    {
        if( Settings.GetCloudsOnOff() )
        {
            // Init the classes using the screens DC. That way it doesn't have to make one
            cloudBuffer.InitActiveDC( GetScreenDC() );
            cloudArt.InitActiveDC( GetScreenDC() );

            // Load the GIF graphic from resource. Randomly choose one out of the six.
            if( !cloudArt.LoadFromBuffer( gif.LoadFromResource( CloudResources[rand() %  NUM_CLOUDS], "GIF", Instance() ) ) ||
                !cloudArt.CreateBitmap( STANDARD_DIB ) )
            {
            	strcpy( errorStr, "Can't allocate cloud art buffer. Your system resources may be low so try restarting your computer." ); 
                goto HANDLE_ERROR;
			}

            // Make the background cloud buffer
			if( Mode() == ssSaver )
			{
	            if( !cloudBuffer.CreateStandardBuffer( DesktopWidth, DesktopHeight ) )
	            { 
	                strcpy( errorStr, "Can't allocate cloud buffer. Your system resources may be low so try restarting your computer." );
	                goto HANDLE_ERROR;
				}
			}
			else  // Use DIBSection for preview mode because XP doesn't lag so bad when doing a stretchdraw
			{
	            if( !cloudBuffer.CreateDIBSectionBuffer( DesktopWidth, DesktopHeight, BIT_COUNT_16 ) )
	            {
	            	strcpy( errorStr, "Can't allocate cloud buffer. Your system resources may be low so try restarting your computer." ); 
	                goto HANDLE_ERROR;
				}
			}

            // Blit the cloud art to the cloud buffer
                
            // Divide the width and the heights to get the number of tiles needed
            CountLeft = cloudBuffer.width() / cloudArt.width();
            CountTop  = cloudBuffer.height() / cloudArt.height();

            // There's a good chance this wont divide evenly so add one more if there is a remainder
            if( cloudBuffer.width() % cloudArt.width() )
                CountLeft++;
            if( cloudBuffer.height() % cloudArt.height() )
                CountTop++;

            // Init the source rect which is the the size of the art
            SetRect( &SourceRect, 0, 0, cloudArt.width(), cloudArt.height() );

            // Tile the clouds across the cloud buffer
            for( j = 0; j < CountTop; ++j )
            {
                for( i = 0; i < CountLeft; ++i )
                {        
                    SetRect( &DestRect, i * cloudArt.width(), j * cloudArt.height(), 
                                        cloudArt.width(), cloudArt.height() );
                    cloudArt.Draw( &cloudBuffer, &DestRect, &SourceRect );
                }
            }
        }

        // Load the planet art if it is selected and in a higher resolution then 8 bit
        // Planet art is not supported on systems with 8 bit or less color
        if( Settings.GetPlanetsOnOff() )
        {
            // Init the classes using the screens DC. That way it doesn't have to make one
            stars.InitActiveDC( GetScreenDC() );

            // Load the RGB GIF file
            // This RGB image was saved as an indexed 8 bit image so that it could be GIF compressed.
            // It was never really an 8 bit image, we just made it look that way
            if( !starsPlanetTemp.LoadFromBuffer( gif.LoadFromResource( "StarsPlanets24", "GIF", Instance() ) ) )
                goto HANDLE_ERROR;

            // Make the RGB buffer for the stars and planets
			if( Mode() == ssSaver )
			{
	            if( !stars.CreateStandardBuffer( starsPlanetTemp.width(), starsPlanetTemp.height() / 3 ) )
	            { 
	                strcpy( errorStr, "Can't allocate planet buffer. Your system resources may be low so try restarting your computer." );
	                goto HANDLE_ERROR;
				}
			}
			else   // Use DIBSection for preview mode because XP doesn't lag so bad when doing a stretchdraw
			{
	            if( !stars.CreateDIBSectionBuffer( starsPlanetTemp.width(), starsPlanetTemp.height() / 3, BIT_COUNT_16 ) )
	            {
	            	strcpy( errorStr, "Can't allocate planet buffer. Your system resources may be low so try restarting your computer." ); 
	                goto HANDLE_ERROR;
				}
			}

            // Check to see that it's not empty
            if( !starsPlanetTemp.IsEmpty() )
            {
                // Copy over the bitmap info data
                memcpy( &fh, starsPlanetTemp.dib(), sizeof(TBitmapinfo) );

                // Now we are going to change some information so that it appears to be a 24 bit image again
                // Change the information needed to do the conversion
                fh.bmiHeader.biHeight /= 3;
                fh.bmiHeader.biBitCount = 24;
                fh.bmiHeader.biSizeImage = 0;
                fh.bmiHeader.biClrImportant = 0;
                fh.bmiHeader.biClrUsed = 0;

                // Blit the image to the buffer.
                StretchDIBits( stars.Handle(), 0, 0, starsPlanetTemp.width(), fh.bmiHeader.biHeight,
                               0, 0, starsPlanetTemp.width(), fh.bmiHeader.biHeight, starsPlanetTemp.bits(), 
                               (BITMAPINFO*)&fh, DIB_RGB_COLORS, SRCCOPY );
            }
            else
                goto HANDLE_ERROR;
        }
    }

    // Load the palette if we are in 8 bit or less mode
    // The cloud and planet images are not suppored in the 8 bit mode
    if( SystemBitDepth <= BIT_COUNT_8 || !Settings.GetPlanetsOnOff() )
    {
        // Init the classes using the screens DC. That way it doesn't have to make one
        stars.InitActiveDC( GetScreenDC() );

        // Load the 8 bit GIF graphic from resource. This is a duplicate set of graphice just incase someone
        // wants to run the screen saver in 8 bit
        if( !stars.LoadFromBuffer( gif.LoadFromResource( "StarsOnly", "GIF", Instance() ) ) ) 
            goto HANDLE_ERROR;

        // Use one or the other depending on the color depth
        if( SystemBitDepth > BIT_COUNT_8 && Mode() == ssSaver )
		{
            Load = stars.CreateBitmap( STANDARD_DIB );
		}
        else  // Use DIBSection for preview mode because XP doesn't lag so bad when doing a stretchdraw
		{
            Load = stars.CreateBitmap( DIB_SECTION_BMP, BIT_COUNT_16 );
		}
             
        if( !Load )
        {
        	strcpy( errorStr, "Can't allocate star buffer. Your system resources may be low so try restarting your computer." ); 
            goto HANDLE_ERROR;
		}

        // Init the screens palette
        if( SystemBitDepth <= BIT_COUNT_8 )
        {
            // Saver the old palette to select back into the dc. If this doesn't happen, the newly created
            // palette will never get deleted
            hOldPalette = SelectPalette( GetScreenDC(), stars.GetPalette(), FALSE );
            RealizePalette( GetScreenDC() );
        }

        // Free up the bitmap data because we don't need it anymore
        stars.DeleteBitmapBits();
    }

    // If we are drawing clouds, paint the buffer and the screen with the clouds
    if( !cloudBuffer.IsEmpty() )
    {
        // Init the buffer with the cloud buffer
        cloudBuffer.Draw( &buffer, &DesktopRect );
    }
    else // If we are not using the the clouds, just paint the screen black
    {
        // Paint the screen black
        FillRect( GetScreenDC(), &DesktopRect, BlackBrush );

        // Make the buffer black
        buffer.ColorRect( &DesktopRect, BlackBrush );
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
    BOOL Result = FALSE;

    // setup the star data
    if( !InitStarData() )
        goto HANDLE_ERROR;

    // setup the planet data
    if( Settings.GetPlanetsOnOff() && SystemBitDepth > BIT_COUNT_8 )
        if( !InitPlanetData() )
            goto HANDLE_ERROR;

    if( Settings.GetSpaceShipsOnOff() )
        if( !InitShipData() )
            goto HANDLE_ERROR;

    Result = TRUE;

    HANDLE_ERROR:;

    return Result;

}   // InitSaverData


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
        
        if( !ShareWareMode )
        {
            BlitToTheBuffer();

            // Now blit the buffer to the screen only if not in shareware mode
            buffer.Draw( GetScreenDC(), &DesktopRect );
        }       
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
        case PLAY_MUSIC:       PlayMusic();               break;

        // Erasing the buffer
        case ERASE_BUFFER:     EraseTheBuffer();          break;

        // Update all the animations
        case UPDATE_ANIM:      UpdateAllAnimations();     break;

        // Blit to buffer
        case BLIT_TO_BUFFER:   BlitToTheBuffer();         break;

        // Do the shareware check
        case SHAREWARE_NOTICE: ShowShareWareScreen();     break;

        // Blit to screen
        case BLIT_TO_SCREEN:   BlitFromBufferToScreen();  break;
    }

    // Inc the counter
    AnimCounter = ( AnimCounter + 1 ) % MAX_BLIT_OPERATIONS;

}   // Animate


/************************************************************************
*    FUNCTION NAME:         EraseTheBuffer                                                             
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

void CAnimateSaver::EraseTheBuffer()
{
    RECT rect;
    int i;

    // Skip this part if we are in preview mode to help the preview mode animate faster
    if( Mode() == ssPreview && PreviewCounter != 1 )
        return;

    // If your not using the clouds to erase the buffer
    // then just do a black fill
    if( !cloudBuffer.IsEmpty() )
    {
        cloudBuffer.SetCopyMode( SRCCOPY );

        // Erase the stars
        for( i = 0; i < StarCount; ++i )
        {
            // Trim down the rect so that only the what needs to be blitted gets erased
            AddSubtractRect( &rect, &pStarData[i].Dest, STAR_TRIM );
            cloudBuffer.Draw( &buffer, &rect );
        }

        // Erase the planets
        if( Settings.GetPlanetsOnOff() && SystemBitDepth > BIT_COUNT_8 )
        {
            for( i = 0; i < NUM_PLANETS; ++i )
                if( PlanetData[i].Active )
                    cloudBuffer.Draw( &buffer, &PlanetData[i].Dest );
        }

        // Erase the ships
        if( Settings.GetSpaceShipsOnOff() )
        {
            for( i = 0; i < NUM_SHIPS; ++i )
                if( ShipData[i].Active )
                    cloudBuffer.Draw( &buffer, &ShipData[i].Dest );
        }
    }
    else
    {
        // Set the copy mode to blackness to erase the buffer
        buffer.SetCopyMode( BLACKNESS );

        // Erase the stars
        for( i = 0; i < StarCount; ++i )
        {
            // Trim down the rect so that only the what needs to be blitted gets erased
            AddSubtractRect( &rect, &pStarData[i].Dest, STAR_TRIM );
            buffer.Draw( &buffer, &rect );
        }

        // Erase the planets
        if( Settings.GetPlanetsOnOff() && SystemBitDepth > BIT_COUNT_8 )
        {
            for( i = 0; i < NUM_PLANETS; ++i )
                if( PlanetData[i].Active )
                    buffer.Draw( &buffer, &PlanetData[i].Dest );
        }

        // Erase the ships
        if( Settings.GetSpaceShipsOnOff() )
        {
            for( i = 0; i < NUM_SHIPS; ++i )
                if( ShipData[i].Active )
                    buffer.Draw( &buffer, &ShipData[i].Dest );
        }

        // Set it back to the copy mode because we dont want to only blit black boxes
        buffer.SetCopyMode( SRCCOPY );
    }

}   // EraseTheBuffer


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

void CAnimateSaver::UpdateAllAnimations()
{
    // Update all the star positions
    UpdateStarPositions();

    // Update all the planet positions that are active
    if( Settings.GetPlanetsOnOff() && SystemBitDepth > BIT_COUNT_8 )
        UpdatePlanetPositions();

    // Update all the ship positions that are active
    if( Settings.GetSpaceShipsOnOff() )
        UpdateShipPositions();

}   // UpdateAllAnimations


/************************************************************************
*    FUNCTION NAME:         UpdateStarPositions                                                             
*
*    DESCRIPTION:           Updates all the star positions
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

void CAnimateSaver::UpdateStarPositions()
{
    int i;

    // Update all the star positions
    for( i = 0; i < StarCount; ++i )
    {
        switch( StarMovement )
        {
            case MOVE_LEFT:
                pStarData[i].Dest.left -= pStarData[i].Speed;
                if( pStarData[i].Dest.left <= -pStarData[i].Dest.right )
                    ResetStarStructure( &pStarData[i] );
            break;

            case MOVE_RIGHT:
                pStarData[i].Dest.left += pStarData[i].Speed;
                if( pStarData[i].Dest.left >= DesktopWidth )
                    ResetStarStructure( &pStarData[i] );
            break;

            case MOVE_DOWN:
                pStarData[i].Dest.top += pStarData[i].Speed;
                if( pStarData[i].Dest.top >= DesktopHeight )
                    ResetStarStructure( &pStarData[i] );
            break;

            case MOVE_UP:
                pStarData[i].Dest.top -= pStarData[i].Speed;
                if( pStarData[i].Dest.top <= -pStarData[i].Dest.bottom )
                    ResetStarStructure( &pStarData[i] );
            break;
        }
    }

}   // UpdateStarPositions


/************************************************************************
*    FUNCTION NAME:         UpdatePlanetPositions                                                             
*
*    DESCRIPTION:           Update all the planet positions that are active
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

void CAnimateSaver::UpdatePlanetPositions()
{
    int i;
    DWORD PlanetTimer;

    for( i = 0; i < NUM_PLANETS; ++i )
    {
        PlanetTimer = timeGetTime();

        if( PlanetData[i].Active && PlanetData[i].SpeedTimer < PlanetTimer )
        {
            // Reset the timer
            PlanetData[i].SpeedTimer = PlanetTimer + PlanetData[i].SpeedDelay;

            switch( StarMovement )
            {
                case MOVE_LEFT:
                    PlanetData[i].Dest.left -= PLANET_PIXEL_SPEED;

                    // Planet is done. Stop the animation
                    if( PlanetData[i].Dest.left <= -PlanetData[i].Dest.right )
                    {
                        PlanetData[i].Active = FALSE;
                        --NumPlanetsAnim;
                    }
                break;

                case MOVE_RIGHT:
                    PlanetData[i].Dest.left += PLANET_PIXEL_SPEED;

                    // Planet is done. Stop the animation
                    if( PlanetData[i].Dest.left >= DesktopWidth )
                    {
                        PlanetData[i].Active = FALSE;
                        --NumPlanetsAnim;
                    }
                break;

                case MOVE_DOWN:
                    PlanetData[i].Dest.top += PLANET_PIXEL_SPEED;

                    // Planet is done. Stop the animation
                    if( PlanetData[i].Dest.top >= DesktopHeight )
                    {
                        PlanetData[i].Active = FALSE;
                        --NumPlanetsAnim;
                    }
                break;

                case MOVE_UP:
                    PlanetData[i].Dest.top -= PLANET_PIXEL_SPEED;

                    // Planet is done. Stop the animation
                    if( PlanetData[i].Dest.top <= -PlanetData[i].Dest.bottom )
                    {
                        PlanetData[i].Active = FALSE;
                        --NumPlanetsAnim;
                    }
                break;
            }
        }
    }

    // See if we'll have some other planets animating
    ResetPlanetStructure( &PlanetData[rand() %  NUM_PLANETS] );

}   // UpdatePlanetPositions


/************************************************************************
*    FUNCTION NAME:         UpdateShipPositions                                                             
*
*    DESCRIPTION:           Update all the ship positions that are active
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

void CAnimateSaver::UpdateShipPositions()
{
    int i;

    for( i = 0; i < NUM_SHIPS; ++i )
    {
        if( ShipData[i].Active )
        {
            switch( ShipData[i].Movement )
            {
                case MOVE_LEFT:
                    ShipData[i].Dest.left -= ShipData[i].PixelSpeed;

                    // Planet is done. Stop the animation
                    if( ShipData[i].Dest.left <= -ShipData[i].Dest.right )
                    {
                        ShipData[i].Active = FALSE;
                        --NumShipsAnim;
                    }
                break;

                case MOVE_RIGHT:
                    ShipData[i].Dest.left += ShipData[i].PixelSpeed;

                    // Planet is done. Stop the animation
                    if( ShipData[i].Dest.left >= DesktopWidth )
                    {
                        ShipData[i].Active = FALSE;
                        --NumShipsAnim;
                    }
                break;

                case MOVE_DOWN:
                    ShipData[i].Dest.top += ShipData[i].PixelSpeed;

                    // Planet is done. Stop the animation
                    if( ShipData[i].Dest.top >= DesktopHeight )
                    {
                        ShipData[i].Active = FALSE;
                        --NumShipsAnim;
                    }
                break;

                case MOVE_UP:
                    ShipData[i].Dest.top -= ShipData[i].PixelSpeed;

                    // Planet is done. Stop the animation
                    if( ShipData[i].Dest.top <= -ShipData[i].Dest.bottom )
                    {
                        ShipData[i].Active = FALSE;
                        --NumShipsAnim;
                    }
                break;
            }
        }
    }

    // See if we'll have some other ships animating
    ResetShipStructure( &ShipData[rand() %  NUM_SHIPS] );

}   // UpdateShipPositions


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

void CAnimateSaver::BlitToTheBuffer()
{
    RECT rect, Dest, Source;
    DWORD TmpTimer;
    int i;

    // Skip this part if we are in preview mode to help the preview mode animate faster
    if( Mode() == ssPreview && PreviewCounter != 0 )//MAX_PREVIEW_TIMER_COUNTS-1 )
        return;

    // Paint planets to bufffer before the stars
    if( Settings.GetPlanetsOnOff() && SystemBitDepth > BIT_COUNT_8 && !Settings.GetPlanetsInfrontOfStars() )
    {
        for( i = 0; i < NUM_PLANETS; ++i )
        {
            if( PlanetData[i].Active )
            {
                stars.SetCopyMode( SRCAND );
                rect = PlanetData[i].Source;
                rect.top += rect.bottom;
                stars.Draw( &buffer, &PlanetData[i].Dest, &rect );
                stars.SetCopyMode( SRCPAINT );
                stars.Draw( &buffer, &PlanetData[i].Dest, &PlanetData[i].Source );
            }
        }
    }

    stars.SetCopyMode( SRCPAINT );

    // init the temp timer
    TmpTimer = timeGetTime();

    // Paint the stars to the buffer
    for( i = 0; i < StarCount; ++i )
    {
        if( !pStarData[i].StarOnOff )
        {
            AddSubtractRect( &Dest, &pStarData[i].Dest, STAR_TRIM );
            AddSubtractRect( &Source, &pStarData[i].Source, STAR_TRIM );

            stars.Draw( &buffer, &Dest, &Source );

            if( pStarData[i].BlinkTimer < TmpTimer )
            {
                pStarData[i].BlinkTimer = TmpTimer + ( rand() % BRIGHT_STAR_MAX ) + BRIGHT_STAR_MIN;
                pStarData[i].StarOnOff = TRUE;
            }
        }
        else
        {
            rect = pStarData[i].Source;
            rect.top = rect.bottom;

            AddSubtractRect( &Dest, &pStarData[i].Dest, STAR_TRIM );
            AddSubtractRect( &Source, &rect, STAR_TRIM );

            stars.Draw( &buffer, &Dest, &Source );

            if( pStarData[i].BlinkTimer < TmpTimer )
            {
                pStarData[i].BlinkTimer = TmpTimer + ( rand() % BLINK_DELAY );
                pStarData[i].StarOnOff = FALSE;
            }
        }
    }

    // Paint planets to buffer after the stars
    if( Settings.GetPlanetsOnOff() && SystemBitDepth > BIT_COUNT_8 && Settings.GetPlanetsInfrontOfStars() )
    {
        for( i = 0; i < NUM_PLANETS; ++i )
        {
            if( PlanetData[i].Active )
            {
                stars.SetCopyMode( SRCAND );
                rect = PlanetData[i].Source;
                rect.top += rect.bottom;
                stars.Draw( &buffer, &PlanetData[i].Dest, &rect );
                stars.SetCopyMode( SRCPAINT );
                stars.Draw( &buffer, &PlanetData[i].Dest, &PlanetData[i].Source );
            }
        }
    }

    // Paint ships to buffer
    if( Settings.GetSpaceShipsOnOff() )
    {
        // init the temp timer
        TmpTimer = timeGetTime();

        for( i = 0; i < NUM_SHIPS; ++i )
        {
            if( ShipData[i].Active )
            {
                if( ShipData[i].AnimTimer < TmpTimer )
                {
                    // inc the ship's animation frame
                    ShipData[i].CurrentFrame = ( ShipData[i].CurrentFrame + 1 ) % ShipData[i].NumFrames;

                    // Reset the animation timer
                    ShipData[i].AnimTimer = TmpTimer + ShipData[i].AnimDelay;
                }

                ships.SetCopyMode( SRCAND );
                rect = ShipData[i].Source;
                rect.top += rect.bottom;
                Source = ShipData[i].Source;
                Source.left += Source.right * ShipData[i].CurrentFrame;
                rect.left += rect.right * ShipData[i].CurrentFrame;
                ships.Draw( &buffer, &ShipData[i].Dest, &rect );
                ships.SetCopyMode( SRCPAINT );
                ships.Draw( &buffer, &ShipData[i].Dest, &Source );
            }
        }
    }

}   // BlitToTheBuffer


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
    // Use a local variable for speed in these tight loops
    HDC ScreenDC = GetScreenDC();

    if( Mode() == ssSaver )
    {
        #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )
        RECT Dest;
        int i;

        // Repaint the whole screen if we have too.
        // This screen saver needs to repaint the whole screen in the even
        // the monitor wakes up from a sleep
        if( RepaintScreen() )
        {
            buffer.Draw( ScreenDC );
        }
        else
        {
            if( Settings.GetSpaceShipsOnOff() )
            {
                // Paint ships to the screen
                for( i = 0; i < NUM_SHIPS; ++i )
                    if( ShipData[i].Active )
                        buffer.Draw( ScreenDC, &ShipData[i].Dest );
            }

            if( Settings.GetPlanetsOnOff() && SystemBitDepth > BIT_COUNT_8 )
            {
                // Paint planets to screen
                for( i = 0; i < NUM_PLANETS; ++i )
                    if( PlanetData[i].Active )
                        buffer.Draw( ScreenDC, &PlanetData[i].Dest );
            }

            // Blit the stars, from the buffer, to the screen
            // We only need to blit what is needed to ensure the star cleans up 
            // after itself
            for( i = 0; i < StarCount; ++i )
            {
                AddSubtractRect( &Dest, &pStarData[i].Dest, STAR_TRIM, MOTION );
                buffer.Draw( ScreenDC, &Dest );
            }
        }

        #else
            buffer.StretchDraw( ScreenDC, &WindowRect );
        #endif
    }
    else
    {
        // update the screen every MAX intervals so that it doesn't hog the CPU cycles
        if( PreviewCounter == 0 )
		{
			buffer.StretchDraw( ScreenDC, &WindowRect );
		}

        PreviewCounter = ( PreviewCounter + 1 ) % MAX_PREVIEW_TIMER_COUNTS;
    }

    // Show an error screen if there is a problem playing music
    DisplayMusicError();

}   // BlitFromBufferToScreen


/************************************************************************
*    FUNCTION NAME:         Destructor                                                             
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

            // Since it has timed out for the shareware screen, all the star timers
            // will be ready to go at the same time, so reset all the star timers.
            for( int i = 0; i <  StarCount; ++i )
                pStarData[i].BlinkTimer = timeGetTime() + ( rand() % BLINK_DELAY );

            // Blit the whole buffer to the screen
            buffer.Draw( GetScreenDC(), &DesktopRect );
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
    if( hOldPalette )
        SelectPalette( GetScreenDC(), hOldPalette, FALSE );

    if( pStarData )
        delete [] pStarData;
            
}   // Destructor


/************************************************************************
*    FUNCTION NAME:         InitStarData                                                             
*
*    DESCRIPTION:           Dynamicly allocates the star data array and fills
*                           out all the structures.
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

BOOL CAnimateSaver::InitStarData()
{
    int i, j;
    DWORD tmpTimer = timeGetTime();

    // Allocate the star array
    pStarData = new TStar[StarCount];

    // Check all allocations
    if( pStarData == NULL ) 
        return FALSE;

    // Fill up the structure
    for( i = 0; i < StarCount; ++i )
    {
        // Inc j index into rect array
        j = i % MAX_STAR_TYPES;
        
        // Set the destination
        SetRect( &pStarData[i].Dest, rand() % (DesktopWidth-starArtRect[j].right), 
        							 rand() % (DesktopHeight-starArtRect[j].bottom), starArtRect[j].right, starArtRect[j].bottom );

        // Set the artwork rect
        SetRect( &pStarData[i].Source, starArtRect[j].left, starArtRect[j].top, starArtRect[j].right, starArtRect[j].bottom );

        // Set the timer
        pStarData[i].BlinkTimer = tmpTimer + rand() % ( BLINK_DELAY );

        // Set the blink state
        pStarData[i].StarOnOff = rand() % MAX_BLINK_STATES;

        // Set the pixel speed
        pStarData[i].Speed = ( rand() % STAR_SPEED ) + 1;
    }

    return TRUE;

}   // InitStarData


/************************************************************************
*    FUNCTION NAME:         InitPlanetData                                                             
*
*    DESCRIPTION:           Dynamicly allocates the planet data array and fills
*                           out all the structures.
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

BOOL CAnimateSaver::InitPlanetData()
{
    int i, j;
    DWORD tmpTimer = timeGetTime();

    // init the structure to all zeros
    memset( &PlanetData, 0, sizeof(TPlanet)*NUM_PLANETS );

    for( i = 0; i < NUM_PLANETS; ++i )
    {
        // Set the destination rects
        SetRect( &PlanetData[i].Dest, rand() % (DesktopWidth-planetArtRect[i].right), 
                                      rand() % (DesktopHeight-planetArtRect[i].bottom), 
                                      planetArtRect[i].right, planetArtRect[i].bottom );

        // Set the art rects
        SetRect( &PlanetData[i].Source, planetArtRect[i].left, planetArtRect[i].top, planetArtRect[i].right, planetArtRect[i].bottom );

        // Set the delay of the planet
        PlanetData[i].SpeedDelay = ( rand() % PLANET_SPEED_DELAY_MAX ) + PLANET_SPEED_DELAY_MIN;

        // Set the timer
        PlanetData[i].SpeedTimer = tmpTimer + PlanetData[i].SpeedDelay;
    } 

    // Randomly place how ever many planets are to run on the screen. The trick here
    // is to pick a number in the index of all the planets and then load from
    // that number how ever many planets that are to run. Pick a random number is a 
    // range to choose from but keep us from blowing past our array
    i = rand() % (NUM_PLANETS - Settings.GetNumPlanetsAtOneTime());

    // Pick a few random planets to start with.
    // the value GetNumPlanetsAtOneTime is holding is zero based
    for( j = 0; j < Settings.GetNumPlanetsAtOneTime()+1; ++j )
    {
        // Set this planet as active
        PlanetData[i+j].Active = TRUE;

        // inc the active planet count
        ++NumPlanetsAnim;
    }

    // When this timer runs out, it's time to check the number of planets currently on the screen
    PlanetPickTimer = timeGetTime() + ( rand() % PLANET_PICK_TIME_MAX ) + PLANET_PICK_TIME_MIN;

    return TRUE;

}   // InitPlanetData


/************************************************************************
*    FUNCTION NAME:         InitShipData                                                             
*
*    DESCRIPTION:           Dynamicly allocates the ship data array and fills
*                           out all the structures.
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
*                  4/17/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CAnimateSaver::InitShipData()
{
    int i, j;

    // init the structure to all zeros
    memset( &ShipData, 0, sizeof(TShip)*NUM_SHIPS );

    // To start out with, we are setting the source and destination rects
    // the same, but the destination X, Y's will be set later

    // first Ship
    SetRect( &ShipData[0].Dest, Ship1X, Ship1Y, Ship1W, Ship1H );
    SetRect( &ShipData[0].Art, Ship1X, Ship1Y, Ship1W, Ship1H );
    SetRect( &ShipData[0].Source, Ship1X, Ship1Y, Ship1W, Ship1H );
    ShipData[0].CanFlip = Ship1CanFlip;
    ShipData[0].FlipLeftDown = Ship1FlipLeftUp;
    ShipData[0].CanGoUpDown = Ship1CanUpDown;
    ShipData[0].CanGoLeftRight = Ship1CanLeftRight;
    ShipData[0].NumFrames = Ship1NumFrames;
    ShipData[0].PixelSpeed = Ship1PixelSpeed;
    ShipData[0].AnimDelay = Ship1AnimDelay;
    // The first ship can only go up fromthe bottom
    // so it is set here
    ShipData[0].Movement = MOVE_UP;

    // second Ship
    SetRect( &ShipData[1].Dest, Ship2X, Ship2Y, Ship2W, Ship2H );
    SetRect( &ShipData[1].Art, Ship2X, Ship2Y, Ship2W, Ship2H );
    SetRect( &ShipData[1].Source, Ship2X, Ship2Y, Ship2W, Ship2H );
    ShipData[1].CanFlip = Ship2CanFlip;
    ShipData[1].FlipLeftDown = Ship2FlipLeftUp;
    ShipData[1].CanGoUpDown = Ship2CanUpDown;
    ShipData[1].CanGoLeftRight = Ship2CanLeftRight;
    ShipData[1].NumFrames = Ship2NumFrames;
    ShipData[1].PixelSpeed = Ship2PixelSpeed;
    ShipData[1].AnimDelay = Ship2AnimDelay;

    // third Ship
    SetRect( &ShipData[2].Dest, Ship3X, Ship3Y, Ship3W, Ship3H );
    SetRect( &ShipData[2].Art, Ship3X, Ship3Y, Ship3W, Ship3H );
    SetRect( &ShipData[2].Source, Ship3X, Ship3Y, Ship3W, Ship3H );
    ShipData[2].CanFlip = Ship3CanFlip;
    ShipData[2].FlipLeftDown = Ship3FlipLeftUp;
    ShipData[2].CanGoUpDown = Ship3CanUpDown;
    ShipData[2].CanGoLeftRight = Ship3CanLeftRight;
    ShipData[2].NumFrames = Ship3NumFrames;
    ShipData[2].PixelSpeed = Ship3PixelSpeed;
    ShipData[2].AnimDelay = Ship3AnimDelay;

    // fourth Ship
    SetRect( &ShipData[3].Dest, Ship4X, Ship4Y, Ship4W, Ship4H );
    SetRect( &ShipData[3].Art, Ship4X, Ship4Y, Ship4W, Ship4H );
    SetRect( &ShipData[3].Source, Ship4X, Ship4Y, Ship4W, Ship4H );
    ShipData[3].CanFlip = Ship4CanFlip;
    ShipData[3].FlipLeftDown = Ship4FlipLeftUp;
    ShipData[3].CanGoUpDown = Ship4CanUpDown;
    ShipData[3].CanGoLeftRight = Ship4CanLeftRight;
    ShipData[3].NumFrames = Ship4NumFrames;
    ShipData[3].PixelSpeed = Ship4PixelSpeed;
    ShipData[3].AnimDelay = Ship4AnimDelay;

    // fifth Ship
    SetRect( &ShipData[4].Dest,   Ship5X, Ship5Y, Ship5W, Ship5H );
    SetRect( &ShipData[4].Art, Ship5X, Ship5Y, Ship5W, Ship5H );
    SetRect( &ShipData[4].Source, Ship5X, Ship5Y, Ship5W, Ship5H );
    ShipData[4].CanFlip = Ship5CanFlip;
    ShipData[4].FlipLeftDown = Ship5FlipLeftUp;
    ShipData[4].CanGoUpDown = Ship5CanUpDown;
    ShipData[4].CanGoLeftRight = Ship5CanLeftRight;
    ShipData[4].NumFrames = Ship5NumFrames;
    ShipData[4].PixelSpeed = Ship5PixelSpeed;
    ShipData[4].AnimDelay = Ship5AnimDelay;

    // sixth Ship
    SetRect( &ShipData[5].Dest,   Ship6X, Ship6Y, Ship6W, Ship6H );
    SetRect( &ShipData[5].Art, Ship6X, Ship6Y, Ship6W, Ship6H );
    SetRect( &ShipData[5].Source, Ship6X, Ship6Y, Ship6W, Ship6H );
    ShipData[5].CanFlip = Ship6CanFlip;
    ShipData[5].FlipLeftDown = Ship6FlipLeftUp;
    ShipData[5].CanGoUpDown = Ship6CanUpDown;
    ShipData[5].CanGoLeftRight = Ship6CanLeftRight;
    ShipData[5].NumFrames = Ship6NumFrames;
    ShipData[5].PixelSpeed = Ship6PixelSpeed;
    ShipData[5].AnimDelay = Ship6AnimDelay;

    // seventh Ship
    SetRect( &ShipData[6].Dest,   Ship7X, Ship7Y, Ship7W, Ship7H );
    SetRect( &ShipData[6].Art, Ship7X, Ship7Y, Ship7W, Ship7H );
    SetRect( &ShipData[6].Source, Ship7X, Ship7Y, Ship7W, Ship7H );
    ShipData[6].CanFlip = Ship7CanFlip;
    ShipData[6].FlipLeftDown = Ship7FlipLeftUp;
    ShipData[6].CanGoUpDown = Ship7CanUpDown;
    ShipData[6].CanGoLeftRight = Ship7CanLeftRight;
    ShipData[6].NumFrames = Ship7NumFrames;
    ShipData[6].PixelSpeed = Ship7PixelSpeed;
    ShipData[6].AnimDelay = Ship7AnimDelay;

    // eightth Ship
    SetRect( &ShipData[7].Dest,   Ship8X, Ship8Y, Ship8W, Ship8H );
    SetRect( &ShipData[7].Art, Ship8X, Ship8Y, Ship8W, Ship8H );
    SetRect( &ShipData[7].Source, Ship8X, Ship8Y, Ship8W, Ship8H );
    ShipData[7].CanFlip = Ship8CanFlip;
    ShipData[7].FlipLeftDown = Ship8FlipLeftUp;
    ShipData[7].CanGoUpDown = Ship8CanUpDown;
    ShipData[7].CanGoLeftRight = Ship8CanLeftRight;
    ShipData[7].NumFrames = Ship8NumFrames;
    ShipData[7].PixelSpeed = Ship8PixelSpeed;
    ShipData[7].AnimDelay = Ship8AnimDelay;

    // nineth Ship
    SetRect( &ShipData[8].Dest,   Ship9X, Ship9Y, Ship9W, Ship9H );
    SetRect( &ShipData[8].Art, Ship9X, Ship9Y, Ship9W, Ship9H );
    SetRect( &ShipData[8].Source, Ship9X, Ship9Y, Ship9W, Ship9H );
    ShipData[8].CanFlip = Ship9CanFlip;
    ShipData[8].FlipLeftDown = Ship9FlipLeftUp;
    ShipData[8].CanGoUpDown = Ship9CanUpDown;
    ShipData[8].CanGoLeftRight = Ship9CanLeftRight;
    ShipData[8].NumFrames = Ship9NumFrames;
    ShipData[8].PixelSpeed = Ship9PixelSpeed;
    ShipData[8].AnimDelay = Ship9AnimDelay;

    // tenth Ship
    SetRect( &ShipData[9].Dest,   Ship10X, Ship10Y, Ship10W, Ship10H );
    SetRect( &ShipData[9].Art, Ship10X, Ship10Y, Ship10W, Ship10H );
    SetRect( &ShipData[9].Source, Ship10X, Ship10Y, Ship10W, Ship10H );
    ShipData[9].CanFlip = Ship10CanFlip;
    ShipData[9].FlipLeftDown = Ship10FlipLeftUp;
    ShipData[9].CanGoUpDown = Ship10CanUpDown;
    ShipData[9].CanGoLeftRight = Ship10CanLeftRight;
    ShipData[9].NumFrames = Ship10NumFrames;
    ShipData[9].PixelSpeed = Ship10PixelSpeed;
    ShipData[9].AnimDelay = Ship10AnimDelay;

    // eleventh Ship
    SetRect( &ShipData[10].Dest,   Ship11X, Ship11Y, Ship11W, Ship11H );
    SetRect( &ShipData[10].Art, Ship11X, Ship11Y, Ship11W, Ship11H );
    SetRect( &ShipData[10].Source, Ship11X, Ship11Y, Ship11W, Ship11H );
    ShipData[10].CanFlip = Ship11CanFlip;
    ShipData[10].FlipLeftDown = Ship11FlipLeftUp;
    ShipData[10].CanGoUpDown = Ship11CanUpDown;
    ShipData[10].CanGoLeftRight = Ship11CanLeftRight;
    ShipData[10].NumFrames = Ship11NumFrames;
    ShipData[10].PixelSpeed = Ship11PixelSpeed;
    ShipData[10].AnimDelay = Ship11AnimDelay;

    // twelveth Ship
    SetRect( &ShipData[11].Dest,   Ship12X, Ship12Y, Ship12W, Ship12H );
    SetRect( &ShipData[11].Art, Ship12X, Ship12Y, Ship12W, Ship12H );
    SetRect( &ShipData[11].Source, Ship12X, Ship12Y, Ship12W, Ship12H );
    ShipData[11].CanFlip = Ship12CanFlip;
    ShipData[11].FlipLeftDown = Ship12FlipLeftUp;
    ShipData[11].CanGoUpDown = Ship12CanUpDown;
    ShipData[11].CanGoLeftRight = Ship12CanLeftRight;
    ShipData[11].NumFrames = Ship12NumFrames;
    ShipData[11].PixelSpeed = Ship12PixelSpeed;
    ShipData[11].AnimDelay = Ship12AnimDelay;

    // thirteen Ship
    SetRect( &ShipData[12].Dest,   Ship13X, Ship13Y, Ship13W, Ship13H );
    SetRect( &ShipData[12].Art, Ship13X, Ship13Y, Ship13W, Ship13H );
    SetRect( &ShipData[12].Source, Ship13X, Ship13Y, Ship13W, Ship13H );
    ShipData[12].CanFlip = Ship13CanFlip;
    ShipData[12].FlipLeftDown = Ship13FlipLeftUp;
    ShipData[12].CanGoUpDown = Ship13CanUpDown;
    ShipData[12].CanGoLeftRight = Ship13CanLeftRight;
    ShipData[12].NumFrames = Ship13NumFrames;
    ShipData[12].PixelSpeed = Ship13PixelSpeed;
    ShipData[12].AnimDelay = Ship13AnimDelay;

    // fourteenth Ship
    SetRect( &ShipData[13].Dest,   Ship14X, Ship14Y, Ship14W, Ship14H );
    SetRect( &ShipData[13].Art, Ship14X, Ship14Y, Ship14W, Ship14H );
    SetRect( &ShipData[13].Source, Ship14X, Ship14Y, Ship14W, Ship14H );
    ShipData[13].CanFlip = Ship14CanFlip;
    ShipData[13].FlipLeftDown = Ship14FlipLeftUp;
    ShipData[13].CanGoUpDown = Ship14CanUpDown;
    ShipData[13].CanGoLeftRight = Ship14CanLeftRight;
    ShipData[13].NumFrames = Ship14NumFrames;
    ShipData[13].PixelSpeed = Ship14PixelSpeed;
    ShipData[13].AnimDelay = Ship14AnimDelay;

    // fifteenth Ship
    SetRect( &ShipData[14].Dest,   Ship15X, Ship15Y, Ship15W, Ship15H );
    SetRect( &ShipData[14].Art, Ship15X, Ship15Y, Ship15W, Ship15H );
    SetRect( &ShipData[14].Source, Ship15X, Ship15Y, Ship15W, Ship15H );
    ShipData[14].CanFlip = Ship15CanFlip;
    ShipData[14].FlipLeftDown = Ship15FlipLeftUp;
    ShipData[14].CanGoUpDown = Ship15CanUpDown;
    ShipData[14].CanGoLeftRight = Ship15CanLeftRight;
    ShipData[14].NumFrames = Ship15NumFrames;
    ShipData[14].PixelSpeed = Ship15PixelSpeed;
    ShipData[14].AnimDelay = Ship15AnimDelay;

    // sixteenth Ship
    SetRect( &ShipData[15].Dest,   Ship16X, Ship16Y, Ship16W, Ship16H );
    SetRect( &ShipData[15].Art, Ship16X, Ship16Y, Ship16W, Ship16H );
    SetRect( &ShipData[15].Source, Ship16X, Ship16Y, Ship16W, Ship16H );
    ShipData[15].CanFlip = Ship16CanFlip;
    ShipData[15].FlipLeftDown = Ship16FlipLeftUp;
    ShipData[15].CanGoUpDown = Ship16CanUpDown;
    ShipData[15].CanGoLeftRight = Ship16CanLeftRight;
    ShipData[15].NumFrames = Ship16NumFrames;
    ShipData[15].PixelSpeed = Ship16PixelSpeed;
    ShipData[15].AnimDelay = Ship16AnimDelay;

    // seventeenth Ship
    SetRect( &ShipData[16].Dest,   Ship17X, Ship17Y, Ship17W, Ship17H );
    SetRect( &ShipData[16].Art, Ship17X, Ship17Y, Ship17W, Ship17H );
    SetRect( &ShipData[16].Source, Ship17X, Ship17Y, Ship17W, Ship17H );
    ShipData[16].CanFlip = Ship17CanFlip;
    ShipData[16].FlipLeftDown = Ship17FlipLeftUp;
    ShipData[16].CanGoUpDown = Ship17CanUpDown;
    ShipData[16].CanGoLeftRight = Ship17CanLeftRight;
    ShipData[16].NumFrames = Ship17NumFrames;
    ShipData[16].PixelSpeed = Ship17PixelSpeed;
    ShipData[16].AnimDelay = Ship17AnimDelay;

    // eightteenth Ship
    SetRect( &ShipData[17].Dest,   Ship18X, Ship18Y, Ship18W, Ship18H );
    SetRect( &ShipData[17].Art, Ship18X, Ship18Y, Ship18W, Ship18H );
    SetRect( &ShipData[17].Source, Ship18X, Ship18Y, Ship18W, Ship18H );
    ShipData[17].CanFlip = Ship18CanFlip;
    ShipData[17].FlipLeftDown = Ship18FlipLeftUp;
    ShipData[17].CanGoUpDown = Ship18CanUpDown;
    ShipData[17].CanGoLeftRight = Ship18CanLeftRight;
    ShipData[17].NumFrames = Ship18NumFrames;
    ShipData[17].PixelSpeed = Ship18PixelSpeed;
    ShipData[17].AnimDelay = Ship18AnimDelay;

    // nineteenth Ship
    SetRect( &ShipData[18].Dest,   Ship19X, Ship19Y, Ship19W, Ship19H );
    SetRect( &ShipData[18].Art, Ship19X, Ship19Y, Ship19W, Ship19H );
    SetRect( &ShipData[18].Source, Ship19X, Ship19Y, Ship19W, Ship19H );
    ShipData[18].CanFlip = Ship19CanFlip;
    ShipData[18].FlipLeftDown = Ship19FlipLeftUp;
    ShipData[18].CanGoUpDown = Ship19CanUpDown;
    ShipData[18].CanGoLeftRight = Ship19CanLeftRight;
    ShipData[18].NumFrames = Ship19NumFrames;
    ShipData[18].PixelSpeed = Ship19PixelSpeed;
    ShipData[18].AnimDelay = Ship19AnimDelay;


    // Randomly place how ever many ships are to run on the screen. The trick here
    // is to pick a number in the index of all the ships and then load from
    // that number how ever many ships that are to run. Pick a random number is a 
    // range to choose from but keep us from blowing past our array
    // GetNumShipsAtOneTime() is zero indexed
    i = rand() % (NUM_SHIPS - (Settings.GetNumShipsAtOneTime()+(MIN_NUM_OF_SHIPS-1)));

    // Pick a few random ships to start with
    for( j = 0; j < Settings.GetNumShipsAtOneTime()+MIN_NUM_OF_SHIPS; ++j )
    {
        ShipData[i+j].Active = TRUE;
        ShipData[i+j].Dest.left = rand() % ( DesktopWidth - ShipData[i+j].Dest.right );
        ShipData[i+j].Dest.top  = rand() % ( DesktopHeight - ShipData[i+j].Dest.bottom );
        ShipData[i+j].AnimTimer = timeGetTime() + ShipData[i+j].AnimDelay;
        
        // Set if it will be comming from the left, right, up or down
        // Some ships can go up down left and right where as some can only
        // left to right or up down
        if( ShipData[i+j].CanGoUpDown && ShipData[i+j].CanGoLeftRight )
            ShipData[i+j].Movement = rand() % NUM_OF_MOVEMENTS;
        else
        {
            if( ShipData[i+j].CanGoLeftRight )
                ShipData[i+j].Movement = rand() % (NUM_OF_MOVEMENTS-2);
            else if( ShipData[i+j].CanGoUpDown )
                ShipData[i+j].Movement = (rand() % (NUM_OF_MOVEMENTS-2)+2);
        }
        
        // Update the source rect
        if( ShipData[i+j].Movement == MOVE_LEFT && ShipData[i+j].CanFlip )
        {
            // The reverse art is to the left or under the art
            if( ShipData[i+j].FlipLeftDown == FLIP_LEFT )
                ShipData[i+j].Source.left = ShipData[i+j].Art.left + 
                                         (ShipData[i+j].Art.right * ShipData[i+j].NumFrames);
            else
                ShipData[i+j].Source.top = ShipData[i+j].Art.top + 
                                         (ShipData[i+j].Art.bottom * 2);
        }

        ++NumShipsAnim;
    }

    // Set the timer for picking the ships
    ShipPickTimer = timeGetTime() + ( rand() % SHIP_PICK_TIME_MAX ) + SHIP_PICK_TIME_MIN;

    return TRUE;

}   // InitShipData


/************************************************************************
*    FUNCTION NAME:         AddSubtractRect                                                             
*
*    DESCRIPTION:           Each image has a little bit of fat around it
*                           so that it cleans up after itself. Depending on
*                           the situation, that can be optomized for faster
*                           blitting by making the rect smaller where it can be.
*
*    FUNCTION PARAMETERS:
*    Input:    RECT *dest   - The rect to init
*              RECT *source - This rect is used to add and subtract the 
*              int Amount   - The amout to use to add and subtract
*              int Type     - Decides how much to strip off depending
*                             on the situation 
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

void CAnimateSaver::AddSubtractRect( RECT *dest, RECT *source, int Amount, int Type )
{
    switch( Type )
    {
        case ERASE:
            dest->left   = source->left   + Amount;
            dest->top    = source->top    + Amount;
            dest->right  = source->right  - (Amount * 2);
            dest->bottom = source->bottom - (Amount * 2);
        break;

        case MOTION:

            switch( StarMovement )
            {
                case MOVE_LEFT:
                    dest->left   = source->left + Amount;
                    dest->top    = source->top + Amount;
                    dest->right  = source->right - Amount;
                    dest->bottom = source->bottom - (Amount * 2);
                break;

                case MOVE_RIGHT:
                    dest->left   = source->left;
                    dest->top    = source->top + Amount;
                    dest->right  = source->right - Amount;
                    dest->bottom = source->bottom - (Amount * 2);
                break;

                case MOVE_DOWN:
                    dest->left   = source->left + Amount;
                    dest->top    = source->top;
                    dest->right  = source->right - (Amount * 2);
                    dest->bottom = source->bottom - Amount;
                break;

                case MOVE_UP:
                    dest->left   = source->left + Amount;
                    dest->top    = source->top + Amount;
                    dest->right  = source->right - (Amount * 2);
                    dest->bottom = source->bottom - Amount;
                break;
            }

        break;
    }

}   // AddSubtractRect


/************************************************************************
*    FUNCTION NAME:         ResetStarStructure                                                             
*
*    DESCRIPTION:           This resets the star structure to a new star
*
*    FUNCTION PARAMETERS:
*    Input:    PTStar pStar - Pointer to a star structure
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

void CAnimateSaver::ResetStarStructure( PTStar pStar )
{
    switch( StarMovement )
    {
        case MOVE_LEFT:
            pStar->Dest.left = DesktopWidth;
            pStar->Dest.top  = rand() % ( DesktopHeight - pStar->Dest.bottom );
            pStar->Speed = ( rand() % STAR_SPEED ) + 1;
        break;

        case MOVE_RIGHT:
            pStar->Dest.left = -pStar->Dest.right;
            pStar->Dest.top  = rand() % ( DesktopHeight - pStar->Dest.bottom );
            pStar->Speed = ( rand() % STAR_SPEED ) + 1;
        break;

        case MOVE_DOWN:
            pStar->Dest.top = -pStar->Dest.bottom;
            pStar->Dest.left = rand() % ( DesktopWidth - pStar->Dest.right );
            pStar->Speed = ( rand() % STAR_SPEED ) + 1;
        break;

        case MOVE_UP:     
            pStar->Dest.top = DesktopHeight;
            pStar->Dest.left = rand() % ( DesktopWidth - pStar->Dest.right );
            pStar->Speed = ( rand() % STAR_SPEED ) + 1;
        break;
    }

}   // ResetStarStructure


/************************************************************************
*    FUNCTION NAME:         ResetPlanetStructure                                                             
*
*    DESCRIPTION:           This resets the planet structure to a
*                           new planet
*
*    FUNCTION PARAMETERS:
*    Input:    PTPlanet pPlanet  - Pointer to a planet structure
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

void CAnimateSaver::ResetPlanetStructure( PTPlanet pPlanet )
{
    DWORD PlanetTimer( timeGetTime() );

    if( !pPlanet->Active && (NumPlanetsAnim < Settings.GetNumPlanetsAtOneTime()+1) && (PlanetPickTimer < PlanetTimer) )
    {
        // Init the planet depending on it's direction
        switch( StarMovement )
        {
            case MOVE_LEFT:
                pPlanet->Dest.left = DesktopWidth;
                pPlanet->Dest.top  = rand() % ( DesktopHeight - pPlanet->Dest.bottom );
            break;

            case MOVE_RIGHT:
                pPlanet->Dest.left = -pPlanet->Dest.right;
                pPlanet->Dest.top  = rand() % ( DesktopHeight - pPlanet->Dest.bottom );
            break;

            case MOVE_DOWN:
                pPlanet->Dest.left = rand() % ( DesktopWidth - pPlanet->Dest.right );
                pPlanet->Dest.top  = -pPlanet->Dest.bottom;
            break;

            case MOVE_UP:
                pPlanet->Dest.left = rand() % ( DesktopWidth - pPlanet->Dest.right );
                pPlanet->Dest.top  = DesktopHeight;
            break;
        }

        pPlanet->Active = TRUE;
        pPlanet->SpeedDelay = rand() % ( PLANET_SPEED_DELAY_MAX ) + PLANET_SPEED_DELAY_MIN;
        pPlanet->SpeedTimer = PlanetTimer + pPlanet->SpeedDelay;
        PlanetPickTimer = PlanetTimer + ( rand() % PLANET_PICK_TIME_MAX ) + PLANET_PICK_TIME_MIN;
        
        ++NumPlanetsAnim;          
    }
    // If there are max planets and the timer has run out, reset the timer.
    else if( NumPlanetsAnim == Settings.GetNumPlanetsAtOneTime()+1 && PlanetPickTimer < PlanetTimer )
        PlanetPickTimer = PlanetTimer + ( rand() % PLANET_PICK_TIME_MAX ) + PLANET_PICK_TIME_MIN;

}   // ResetPlanetStructure


/************************************************************************
*    FUNCTION NAME:         ResetShipStructure                                                             
*
*    DESCRIPTION:           This resets the ship structure to a
*                           new ship
*
*    FUNCTION PARAMETERS:
*    Input:    PTPlanet pPlanet  - Pointer to a ship structure
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

void CAnimateSaver::ResetShipStructure( PTShip pShip )
{
    if( !pShip->Active && (NumShipsAnim < Settings.GetNumShipsAtOneTime()+MIN_NUM_OF_SHIPS) && (ShipPickTimer < timeGetTime()) )
    {
        // Set if it will be comming from the left, right, up or down
        // Some ships can go up down left and right where as some can only
        // left to right or up down
        if( pShip->CanGoUpDown && pShip->CanGoLeftRight )
            pShip->Movement = rand() % NUM_OF_MOVEMENTS;
        else
        {
            if( pShip->CanGoLeftRight )
                pShip->Movement = rand() % (NUM_OF_MOVEMENTS-2);
            else if( pShip->CanGoUpDown )
                pShip->Movement = (rand() % (NUM_OF_MOVEMENTS-2)+2);
        }
        
        // Update the source rect
        if( pShip->Movement == MOVE_LEFT && pShip->CanFlip )
        {
            // The reverse art is to the left or under the art
            if( pShip->FlipLeftDown == FLIP_LEFT )
                pShip->Source.left = pShip->Art.left + 
                                         (pShip->Art.right * pShip->NumFrames);
            else
                pShip->Source.top = pShip->Art.top + 
                                         (pShip->Art.bottom * 2);
        }
        else
        {
            pShip->Source.left = pShip->Art.left;
            pShip->Source.top = pShip->Art.top;
        }

        // Init the ship depending on it's direction
        switch( pShip->Movement )
        {
            case MOVE_LEFT:
                pShip->Dest.left = DesktopWidth;
                pShip->Dest.top  = rand() % ( DesktopHeight - pShip->Dest.bottom );
            break;

            case MOVE_RIGHT:
                pShip->Dest.left = -pShip->Dest.right;
                pShip->Dest.top  = rand() % ( DesktopHeight - pShip->Dest.bottom );
            break;

            case MOVE_DOWN:
                pShip->Dest.left = rand() % ( DesktopWidth - pShip->Dest.right );
                pShip->Dest.top  = -pShip->Dest.bottom;
            break;

            case MOVE_UP:
                pShip->Dest.left = rand() % ( DesktopWidth - pShip->Dest.right );
                pShip->Dest.top  = DesktopHeight;
            break;
        }

        // Set the active state
        pShip->Active = TRUE;
        // Set the pixel speed
        // Reset the animation timer
        pShip->AnimTimer = timeGetTime() + pShip->AnimDelay;

        // Reset the timer for picking another ship
        ShipPickTimer = timeGetTime() + ( rand() % SHIP_PICK_TIME_MAX ) + SHIP_PICK_TIME_MIN;

        ++NumShipsAnim;          
    }
    // If there are the max ships and the timer has run out, reset the timer.
    else if( NumShipsAnim == Settings.GetNumShipsAtOneTime()+MIN_NUM_OF_SHIPS && ShipPickTimer < timeGetTime() )
        ShipPickTimer = timeGetTime() + ( rand() % SHIP_PICK_TIME_MAX ) + SHIP_PICK_TIME_MIN;

}   // ResetShipStructure


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