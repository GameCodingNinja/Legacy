
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
//#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
                             // Note: we are not using the #define WIN32_LEAN_AND_MEAN because we need some multimedia functions
                             // WIN32_LEAN_AND_MEAN leaves out

#include "mintools.h"        // A misc group of useful functions
#include "ssAnim.h"          // Header file for this *.cpp file.
#include <stdlib.h>          // Declares several commonly used routines such as atoi, random, abs, malloc etc.
#include <algorithm>


/************************************************************************
*    FUNCTION NAME:         Constructor                                                             
************************************************************************/
CAnimateSaver::CAnimateSaver( PSSaverModeData ssData ) : CScreenSaver( ssData )
{
    // Init screen saver specific functions



    // Init non-specific class members
    animCounter = 0;
    saverTimer = 0;
    musicTimer = 0;
    songCount = 0;
    hOldPalette = NULL;
    errorNumber = 0;
    previewCounter = 0;

}   // Constructor

/************************************************************************
*    DESCRIPTION:           Get the user control panel settings
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   bool - true or false depending on if successful
************************************************************************/
bool CAnimateSaver::LoadControlPanelPrefs()
{
    // Get the user's settings
    // Load all the ini file settings
    settings.GetPrivateString();
    
    return true;

}   // LoadControlPanelPrefs


/************************************************************************
*    DESCRIPTION:           init buffers, load graphics etc.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   bool - true or false depending on if successful
************************************************************************/
bool CAnimateSaver::LoadGraphics()
{
    bool Result = false, Load;

    // Init the classes using the screens DC. That way it doesn't have to make one
    buffer.InitActiveDC( GetScreenDC() );

    // Load the bubbles. Load the 8 bit GIF graphic from resource.
    /*if( SystemBitDepth > BIT_COUNT_8 && Mode() == ssSaver )
    {
        // Load the RGB GIF file
        // This RGB image was saved as an indexed 8 bit image so that it could be GIF compressed.
        // It was never really an 8 bit image, we just made it look that way
        if( !ArtTmp.LoadFromBuffer( gif.LoadFromResource( "bubblesRGB", "GIF", Instance() ) ) ) 
            goto HANDLE_ERROR;

        // Make the RGB buffer for the stars and planets
        if( !bubbleArt.CreateStandardBuffer( ArtTmp.width(), ArtTmp.height() / 3 ) ) 
            goto HANDLE_ERROR;

        // Check to see that it's not empty
        if( !ArtTmp.IsEmpty() )
        {
            // Copy over the bitmap info data
            memcpy( &fh, ArtTmp.dib(), sizeof(TBitmapinfo) );

            // Now we are going to change some information so that it appears to be a 24 bit image again
            // Change the information needed to do the conversion
            fh.bmiHeader.biHeight /= 3;
            fh.bmiHeader.biBitCount = 24;
            fh.bmiHeader.biSizeImage = 0;
            fh.bmiHeader.biClrImportant = 0;
            fh.bmiHeader.biClrUsed = 0;

            // Blit the image to the buffer.
            StretchDIBits( bubbleArt.Handle(), 0, 0, ArtTmp.width(), fh.bmiHeader.biHeight,
                           0, 0, ArtTmp.width(), fh.bmiHeader.biHeight, ArtTmp.bits(), 
                           (BITMAPINFO*)&fh, DIB_RGB_COLORS, SRCCOPY );
        }
        else
            goto HANDLE_ERROR;

    }
    else  // 256 color verion of the artwork
    {
        if( !bubbleArt.LoadFromBuffer( gif.LoadFromResource( "bubble256", "GIF", Instance() ) ) ) 
            goto HANDLE_ERROR;

        if( !bubbleArt.CreateBitmap( DIB_SECTION_BMP, BIT_COUNT_16 ) )
            goto HANDLE_ERROR;
    }*/

    // Make the buffer. We always need the buffer
    if( systemBitDepth > BIT_COUNT_8 && Mode() == ssSaver )
        Load = buffer.CreateStandardBuffer( desktopWidth, desktopHeight );
    else
        Load = buffer.CreateDIBSectionBuffer( desktopWidth, desktopHeight, BIT_COUNT_16 );

    if( !Load ) 
        goto HANDLE_ERROR;

    // For now, just fill with black
    buffer.ColorRect( &desktopRect, BLACK_BRUSH );

    // Load the palette if we are in 8 bit or less mode
    // Init the screens palette
    if( systemBitDepth <= 8 )
    {
        // Saver the old palette to select back into the dc. If this doesn't happen, the newly created
        // palette will never get deleted
        //hOldPalette = SelectPalette( GetScreenDC(), pStars->GetPalette(), false );
        //RealizePalette( GetScreenDC() );
    }

    Result = true;

    HANDLE_ERROR:;

    return Result;

}   // LoadGraphics


/************************************************************************
*    DESCRIPTION:           Init screen saver data
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   bool - true or false depending on if successful
************************************************************************/
bool CAnimateSaver::InitSaverData()
{
    bool Result = false;





    Result = true;

//    HANDLE_ERROR:;

    return Result;

}   // InitSaverData


/************************************************************************
*    DESCRIPTION:           Allocate the music player. If there is an error
*                           allocating the Windows Media player, we still
*                           allow the screen saver to run.
************************************************************************/
void CAnimateSaver::InitMusicPlayer()
{
    // Allocate the media player for playing music
    if( SaverActive() && Mode() == ssSaver && (settings.GetThemeMusicOn() || settings.GetLoadYourOwn()) )
    {
        // Init the media player
        if( mPlayer.Init( Handle(), Instance(), DONT_USE_ERROR_DLG_BOXES ) )
        {
            // if they are loading their own music, load up all the sound file paths
            if( settings.GetLoadYourOwn() )
            {
                GetAllFileNamesInDir( stringLst, settings.GetSoundDir(), "*.wav" );
				GetAllFileNamesInDir( stringLst, settings.GetSoundDir(), "*.mid" );
				GetAllFileNamesInDir( stringLst, settings.GetSoundDir(), "*.rmi" );
				GetAllFileNamesInDir( stringLst, settings.GetSoundDir(), "*.mp3" );

                // Make sure there has been music added to the sound directory
                if( stringLst.size() )
                {
                    if( !settings.GetRandomMusic() )
					{
                        sort( stringLst.begin(), stringLst.end() );
					}
                }
            }
        }
    }

}   // InitMusicPlayer


/************************************************************************
*    DESCRIPTION:           First time display of the buffer to start
*                           the screen saver
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

        // If this is a duel monitor senearo then we need to black out the screen before we start.
		// This is only needed if the screen saver is not designed to work with more then one monitor.
        if( IsDuelMonitor() )
            FillRect( GetScreenDC(), &windowRect, blackBrush );
        
        // Init the buffer
        BlitToTheBuffer();

        // Now blit the buffer to the screen only if not in shareware mode
        buffer.Draw( GetScreenDC(), &desktopRect );       
    }
    #endif

}   // DisplayBufferOnStartUp


/************************************************************************
*    DESCRIPTION:           Animate the screen saver. This function is called
*                           by the thread.
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
            if( animCounter == 0 && saverTimer < timeGetTime() )
            {
                Animate();
                saverTimer = timeGetTime() + MAX_CYCLE_RENDER_WAIT;
            }
            else if( animCounter )
                Animate();
		}
		// see if we need to pause the sound
		else if( GetSoundPause() )
		{
			// Pause the sound because the monitor is asleep
			PauseSound();

			// Set the pause state back to false so that we don't keep
			// trying to pause the sound over and over again.
			SetSoundPause( false );
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
*    DESCRIPTION:           Plays the music that comes with this screen
*                           saver or play the users music
************************************************************************/
void CAnimateSaver::PlayMusic()
{
    // Don't try to play any music if there has been any errors
    if( !settings.GetMusicOff() && !mPlayer.GetPlayError() && musicTimer < timeGetTime() )
    {
        // Get the status of the windows media player
        int PlayerStatus = mPlayer.GetStatus();

        // Do we play the included star song?
        if( settings.GetThemeMusicOn() )
        {
            // This will play the song that has already been loaded
            if( PlayerStatus == PLAYER_STOPPED || PlayerStatus == PLAYER_PAUSED )
                mPlayer.Play();

            // This will load the song and play it for the first time
            else if( PlayerStatus == PLAYER_CLOSED )
                mPlayer.Play( "ThemeSong", "MIDI" );
        }

        // Or do we play music from the users drive
        else if( settings.GetLoadYourOwn() )
        {
            if( PlayerStatus == PLAYER_STOPPED || PlayerStatus == PLAYER_CLOSED )
            {
                if( songCount == 0 )
                {
                    // If they choose to randomize the music, mix it up
                    if( settings.GetRandomMusic() )
					{
						random_shuffle( stringLst.begin(), stringLst.end() );
					}
                }

                // Play the song
                mPlayer.Play( const_cast<char *>(stringLst[ songCount ].c_str()) );
                
                // Inc to the next song
                songCount = (songCount + 1) % stringLst.size();
            }
            else if( PlayerStatus == PLAYER_PAUSED )
            	mPlayer.Play();   
        }

        // Reste the timer to check if the music is done playing
        musicTimer = timeGetTime() + CHECK_PLAYBACK_DONE_INTERVAL;
    }

}   // PlayMusic


/************************************************************************
*    DESCRIPTION:           Let the user know there was an error
************************************************************************/
void CAnimateSaver::DisplayMusicError()
{
    if( mPlayer.GetPlayError() )
    {
        // Set the background mode to transparent for the text
        canvas.SetBackgroundMode( TRANSPARENT );

        // Draw the text
        canvas.DrawText( 0, 0, mPlayer.GetErrorStr() );

        // Set the background mode back to opaque
        canvas.SetBackgroundMode( OPAQUE );
    }

}   // DisplayMusicError


/************************************************************************
*    DESCRIPTION:           Animates the stars. This is designed to have it
*                           do one thing at a time as it come through the
*                           function.
************************************************************************/
void CAnimateSaver::Animate()
{
    switch( animCounter )
    {
        // Do we need to play music
        case PLAY_MUSIC:       PlayMusic();               break;

        // Erasing the buffer
        case ERASE_BUFFER:     EraseTheBuffer();          break;

        // Update all the animations
        case UPDATE_ANIM:      UpdateAllAnimations();     break;

        // Blit to buffer
        case BLIT_TO_BUFFER:   BlitToTheBuffer();         break;

        // Blit to screen
        case BLIT_TO_SCREEN:   BlitFromBufferToScreen();  break;
    }

    // Inc the animation counter
    animCounter = ( animCounter + 1 ) % NUM_BLIT_OPERATIONS;

}   // Animate


/************************************************************************
*    DESCRIPTION:           Erases the buffer in prep for the next 
*                           animation round
************************************************************************/
void CAnimateSaver::EraseTheBuffer()
{
    // Skip this part if we are in preview mode to help the preview mode animate faster
    if( Mode() == ssPreview && previewCounter == 0 )
        return;

}   // EraseTheBuffer


/************************************************************************
*    DESCRIPTION:           Updates all the animation positions
************************************************************************/
void CAnimateSaver::UpdateAllAnimations()
{

}   // UpdateAllAnimations


/************************************************************************
*    DESCRIPTION:           Blit all the animations to the buffer
************************************************************************/
void CAnimateSaver::BlitToTheBuffer()
{

    // Skip this part if we are in preview mode to help the preview mode animate faster
    if( Mode() == ssPreview && previewCounter == MAX_PREVIEW_TIMER_COUNTS-1 )
        return;


}   // BlitToTheBuffer


/************************************************************************
*    DESCRIPTION:           Blit to the buffer from the screen
************************************************************************/
void CAnimateSaver::BlitFromBufferToScreen()
{
    // Use a local variable for speed in these tight loops
    HDC ScreenDC = GetScreenDC();

    if( Mode() == ssSaver )
    {
        #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )

        // Repaint the whole screen if we have too.
        // This screen saver needs to repaint the whole screen in the even
        // the monitor wakes up from a sleep
        if( RepaintScreen() )
        {
            buffer.Draw( ScreenDC );
        }
        else
        {

		}

        #else
            buffer.StretchDraw( ScreenDC, &WindowRect );
        #endif
    }
    else
    {
        // update the screen every X numer of intervals so that it doesn't hog the CPU cycles
        if( previewCounter == 0 )
            buffer.StretchDraw( ScreenDC, &windowRect );

        // Inc the preview counter
        previewCounter = ( previewCounter + 1 ) % MAX_PREVIEW_TIMER_COUNTS;
    }

    // Show an error text if there is a problem playing music
    DisplayMusicError();

}   // BlitFromBufferToScreen


/************************************************************************
*    DESCRIPTION:           Free up all allocated resources.
************************************************************************/
CAnimateSaver::~CAnimateSaver()
{
    if( hOldPalette )
        SelectPalette( GetScreenDC(), hOldPalette, false );
            
}   // Destructor


/************************************************************************
*    DESCRIPTION:           Pause the sound
************************************************************************/
void CAnimateSaver::PauseSound()
{
    int PlayerStatus;

    // Don't try to play any music if there has been any errors
    if( !mPlayer.GetPlayError() )
    {
        PlayerStatus = mPlayer.GetStatus();

        if( PlayerStatus == PLAYER_PLAYING )
            mPlayer.Pause();
    }
}   // PauseSound