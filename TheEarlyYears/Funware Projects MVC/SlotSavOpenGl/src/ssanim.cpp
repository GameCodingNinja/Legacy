
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
#include "gl/gl.h"           // standard OpenGL include
#include "gl/glu.h"          // OpenGL utilities

int SYMB_1    = 0; 
int SYMB_2    = 1; 
int SYMB_3    = 2; 
int SYMB_4    = 3; 
int SYMB_5    = 4;
int SYMB_6    = 5; 
int SYMB_7    = 6; 
int SYMB_8    = 7; 
int SYMB_9    = 8; 
int SYMB_10   = 9;
int SYMB_11   = 10;
int SYMB_12   = 11;
int SYMB_ANY  =  12;

SPhysicalReel reel1 = {
    {30},      // symbols on this reel                          
    {SYMB_12,  SYMB_7,    SYMB_8,    SYMB_6,  SYMB_5,   SYMB_7,   SYMB_4,   SYMB_8,    SYMB_3,   SYMB_7,
    SYMB_6,   SYMB_12,   SYMB_7,    SYMB_4,  SYMB_6,   SYMB_3,   SYMB_7,   SYMB_5,    SYMB_3,   SYMB_4,
    SYMB_5,   SYMB_7,    SYMB_8,    SYMB_3,  SYMB_6,   SYMB_5,   SYMB_8,   SYMB_2,    SYMB_7,   SYMB_6} };
                                           
SPhysicalReel reel2 = { 
    {29},  // symbols on this reel                          
    {SYMB_1,   SYMB_6,   SYMB_12,   SYMB_7,    SYMB_6,    SYMB_3,     SYMB_8,   SYMB_7,    SYMB_4,   SYMB_3,
    SYMB_6,   SYMB_4,   SYMB_1,    SYMB_5,    SYMB_4,    SYMB_12,    SYMB_6,   SYMB_7,    SYMB_2,   SYMB_6,
    SYMB_3,   SYMB_8,   SYMB_5,    SYMB_7,    SYMB_4,    SYMB_5,     SYMB_8,   SYMB_12,   SYMB_4} };
                                      
SPhysicalReel reel3 = { 
    {30},  // symbols on this reel                          
    {SYMB_1,   SYMB_8,   SYMB_7,   SYMB_5,   SYMB_4,    SYMB_7,    SYMB_12,    SYMB_3,   SYMB_6,   SYMB_7,
    SYMB_9,   SYMB_5,   SYMB_7,   SYMB_12,  SYMB_2,    SYMB_8,    SYMB_7,     SYMB_6,   SYMB_4,   SYMB_3,
    SYMB_5,   SYMB_6,   SYMB_2,   SYMB_4,   SYMB_7,    SYMB_6,    SYMB_9,     SYMB_5,   SYMB_7,   SYMB_6} };
                            
SPhysicalReel reel4 = {
    {31},  // symbols on this reel                          
    {SYMB_1,   SYMB_7,    SYMB_5,   SYMB_6,   SYMB_4,   SYMB_7,    SYMB_6,   SYMB_8,   SYMB_3,   SYMB_4,
    SYMB_5,   SYMB_10,   SYMB_6,   SYMB_5,   SYMB_8,   SYMB_12,   SYMB_6,   SYMB_7,   SYMB_3,   SYMB_4,
    SYMB_10,  SYMB_3,    SYMB_8,   SYMB_6,   SYMB_2,   SYMB_12,   SYMB_4,   SYMB_3,   SYMB_5,   SYMB_7,
    SYMB_12} };
                            
SPhysicalReel reel5 = { 
    {29},  // symbols on this reel                          
    {SYMB_1,   SYMB_4,   SYMB_11,   SYMB_7,    SYMB_8,   SYMB_6,   SYMB_5,   SYMB_12,   SYMB_7,   SYMB_6,
    SYMB_5,   SYMB_7,   SYMB_6,    SYMB_2,    SYMB_7,   SYMB_1,   SYMB_5,   SYMB_11,   SYMB_6,   SYMB_5,
    SYMB_7,   SYMB_12,  SYMB_4,    SYMB_7,    SYMB_6,   SYMB_3,   SYMB_5,   SYMB_8,    SYMB_7} };

SPhysicalReel *physicalReels[] = { &reel1, &reel2, &reel3, &reel4, &reel5 };

int reelStopJog[MAX_JOG_COUNT] = {20,17,15,10,7,5,3,1,0,0,-1,-3,-5,-7,-10,-15,-17,-20, 0 };

/************************************************************************
*    FUNCTION NAME:         Constructor                                                             
************************************************************************/
CAnimateSaver::CAnimateSaver( PSSaverModeData ssData ) : CScreenSaver( ssData )
{
    jogCounter = 0;
    reelState = REEL_INIT_SPIN;
    spinTimer = 0;
    spinStopTimer = 0;
    jogReelIndex = 0;
    saverRenderSpeed = 30;
	hOpenGL_RC = NULL;

    // Init non-specific class members
    animCounter = 0;
    saverTimer = 0;
    musicTimer = 0;
    songCount = 0;
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
*    DESCRIPTION:           Create the OpenGL device
************************************************************************/
bool CAnimateSaver::CreateGLDevice()
{
	bool result = false;
    PIXELFORMATDESCRIPTOR pfd;
    int pixelFormat;

    // Clear the pixel format structure
    memset( &pfd, 0, sizeof(PIXELFORMATDESCRIPTOR) );

    // Set some of the members
    pfd.nSize       = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion    = 1;
    pfd.dwFlags     = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType  = PFD_TYPE_RGBA;
    pfd.cColorBits  = 32;
    pfd.cDepthBits  = 32;
    pfd.iLayerType  = PFD_MAIN_PLANE;
    
    // Choose the best matching pixel format
    pixelFormat = ChoosePixelFormat( GetScreenDC(), &pfd );

    // Can't be zero
    if( pixelFormat == 0 )
    {
        PostMsg( "Init Error", "Error choosing pixel format. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Fill in the blanks to completely describe the pixel
    if( DescribePixelFormat( GetScreenDC(), pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd ) == 0 )
    {
        PostMsg( "Init Error", "Error describing the pixel format. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }
    
    // Set the pixel format to the device context
    if( !SetPixelFormat( GetScreenDC(), pixelFormat, &pfd ) )
    {
        PostMsg( "Init Error", "Error setting pixel format. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    } 
    
    // Create the rendering contex
    hOpenGL_RC = wglCreateContext( GetScreenDC() );

    // Can't be NULL
    if( hOpenGL_RC == NULL )
    {
        PostMsg( "Init Error", "Error creating OpenGL Rendering Contex. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    if( !wglMakeCurrent( GetScreenDC(), hOpenGL_RC ) )
    {
        PostMsg( "Init Error", "Error making OpenGL Rendering Contex current. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Set the viewport
    glViewport( 0, 0, screenWidth, screenHeight );

	// Set projection matrix
    glMatrixMode( GL_PROJECTION );

	// load the identy matrix
    glLoadIdentity();

    // Define 2D viewport
    glOrtho( 0.0, (float)screenWidth - 1.0, (float)screenHeight - 1.0, 0.0, -1.0, 1.0 );

	// Set projection matrix
    glMatrixMode( GL_MODELVIEW );

	// load the identy matrix
    glLoadIdentity();

    result = true;

HANDLE_ERROR:;

    return result;

}	// CreateGLDevice


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
    bool result = false;
    int i;
    CGif gif;           // GIF decoder which can load GIFs from file or resource
    char tmpStr[50];

    // Init the classes using the screens DC. That way it doesn't have to make one
    //bkgnd.InitActiveDC( GetScreenDC() );
    //buffer.InitActiveDC( GetScreenDC() );

	if( !CreateGLDevice() )
		goto HANDLE_ERROR;

    if( !bkgnd.LoadFromResource( Instance(), "background", "DIB" ) ) 
            goto HANDLE_ERROR;

	// Convert the rgb
	bkgnd.SwapRedandBlue();

    //if( !bkgnd.CreateBitmap( STANDARD_DIB ) )
        //goto HANDLE_ERROR;

    //if( !buffer.CreateStandardBuffer( bkgnd.width(), bkgnd.height() ) )
        //goto HANDLE_ERROR;

    // Free the bits
    //bkgnd.DeleteBitmapBits();

    // load all the symbols
    for( i = 0; i < MAX_SYMBOLS; ++i )
    {
        wsprintf( tmpStr, "symbol_%d", i );

        // Init the classes using the screens DC.
        symbol[i].InitActiveDC( GetScreenDC() );

        if( !symbol[i].LoadFromResource( Instance(), tmpStr, "DIB" ) ) 
            goto HANDLE_ERROR;

		symbol[i].SwapRedandBlue();
    }

    result = true;

    HANDLE_ERROR:;

    return result;

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
    bool result = false;
    int i, reelOffsetX;

    // Init screen saver specific functions
    reelOffsetX = 180;
    spinStartVel = -1.5f;
    spinStartAcc = 0.15f;
    maxSpinSpeed = 40;
    timerInc = 0.6f;

    for( i = 0; i < REELS; ++i )
    {
        reel[i].InitSymbols( symbol );
        reel[i].InitPhysicalReel( physicalReels[i] );
        reel[i].SetDC( GetScreenDC() );
        reel[i].SetOffsets( i * reelOffsetX, 0 );
        reel[i].SetStopPos();
        reel[i].SetReelPos();   
    }

    result = true;

    HANDLE_ERROR:;

    return result;

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
        
        // Init the double buffer
        for( int i = 0; i < 2; ++i )
		{
	        // Clear the render buffer
	        glClear( GL_COLOR_BUFFER_BIT );

	        // Position the background
	        glRasterPos2i(0,bkgnd.height());
			
			// Draw the background
			glDrawPixels( bkgnd.width(), bkgnd.height(), GL_RGB, GL_UNSIGNED_BYTE, bkgnd.bits() );

			// Draw the reels
			for( int j = 0; j < REELS; ++j )
			{
	            reel[j].Animate( 0 );
			}

			// show what we just rendered
	    	SwapBuffers( GetScreenDC() );
		}       
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
                saverTimer = timeGetTime() + saverRenderSpeed;
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
        //buffer.Draw( GetScreenDC(), GetPasswordWindowRect() );

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
    if( reelState == REEL_INIT_SPIN )
    {
        for( int i = 0; i < REELS; ++i )
        {
            reelTime[i] = 0.0;
            reelStartTimer[i] = 0;
            reel[i].SetStopPos();
        }

        jogCounter = 0;
        jogReelIndex = 0;
		spinStopTimer = 0;
    }
    else if( reelState == REEL_START_SPIN && !reelStartTimer[0] )
    {
        for( int i = 0; i < REELS; ++i )
        {
            reelStartTimer[i] = timeGetTime() + (i * 70);
        }

        saverRenderSpeed = 5;
    }
    else if( reelState == REEL_SPINNING )
    {

    }
    else if( reelState == REEL_STOP )
    {

    }
    else if( reelState == REEL_EVALUATE )
    {
        saverRenderSpeed = 50;
    }

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
	float vol;

    if( Mode() == ssSaver )
    {
        #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )

        // Repaint the whole screen if we have too.
        // This screen saver needs to repaint the whole screen in the even
        // the monitor wakes up from a sleep
        //if( RepaintScreen() )
            //bkgnd.Draw( GetScreenDC() );

        if( reelState == REEL_INIT_SPIN )
        {
            // Set the state
            reelState = REEL_START_SPIN;
        }
		// This state uses physics to bounce start the reels spinning
        else if( reelState == REEL_START_SPIN )
        {
            for( int i = 0; i < REELS; ++i )
            {
                // Has the reel timed out and it is ready to spin
                if( reelStartTimer[i] < timeGetTime() )
                {
                    // Get the velocity
                    vol = (spinStartVel * reelTime[i]) + ((spinStartAcc * (reelTime[i] * reelTime[i])) / 2);

                    // Rest if the velocity is greater then the max speed
                    if( vol > maxSpinSpeed )
                        vol = maxSpinSpeed;

                    // Animate the reel
                    reel[i].Animate( vol );

                    // Inc the time	to speed up the reel to it's max speed
                    if( vol < maxSpinSpeed )
                        reelTime[i] += timerInc;

                    // See if we need to switch the state if all the reels are up to max speed.
                    if( vol == maxSpinSpeed && i == REEL5 )
                    {
                        // Set the state
                        reelState = REEL_SPINNING;

                        // Set the timer for how long all the reels will spin
                        spinTimer = timeGetTime() + SPIN_DURATION;
                    }
                }
            }
        }
        else if( reelState == REEL_SPINNING )
        {
            // Free spin for all the reels
            for( int i = 0; i < REELS; ++i )
                reel[i].Animate( maxSpinSpeed );

            // Are we done spinning all the reels?
			// Then reset the state
            if( spinTimer < timeGetTime() )                
                reelState = REEL_STOP;
        }
        else if( reelState == REEL_STOP )
        {       
            // Cycle through all the reels, stopping each one as they time out.
            for( int i = 0; i < REELS; ++i )
            {
                // Is it time to stop this reel?
                if( jogReelIndex == i && spinStopTimer < timeGetTime() )
                {                   
                    // First time we enter this function for each reel we need to stop,
                    // reset the stop positions so we are right where it shouls stop 
                    if( jogCounter == 0 )
                        reel[jogReelIndex].SetReelPos();

                    // Now we animate the reel stopping in a bounce sort of way
                    if( jogCounter < MAX_JOG_COUNT )
                    {
                        // Move the reel per the jog animation
                        reel[i].Animate( reelStopJog[ jogCounter ] );

                        // Inc the animation jog counter
                        ++jogCounter;
                    }
                    // The reel has come to a stop. Now reset to stop the next reel
                    else
                    {
                        // Set the timer to stop the next reel spininig
                        spinStopTimer = timeGetTime() + REEL_STOP_DELAY;

                        // Reset the jog for the next reel
                        jogCounter = 0;

                        // If we stopped all the reels, it's time to move on 
                        // to the next state.
                        if( jogReelIndex == REEL5 )
                            reelState = REEL_EVALUATE;
                        else
                            ++jogReelIndex;                        
                    }
                }
                // Keep moving the reels we haven't stopped yet
                else if( i >= jogReelIndex )
				{
                    reel[i].Animate( maxSpinSpeed );
				}
            }
        }
        else if( reelState == REEL_EVALUATE )
        {
            reelState = REEL_INIT_SPIN;
        }

        #else

        #endif
    }
    else
    {

    }

	glFlush();
	// show what we just rendered
	SwapBuffers( GetScreenDC() );

    // Show an error text if there is a problem playing music
    //DisplayMusicError();

}   // BlitFromBufferToScreen


/************************************************************************
*    DESCRIPTION:           Free up all allocated resources.
************************************************************************/
CAnimateSaver::~CAnimateSaver()
{
	// Deselect Rendering Contex and delete
    if( hOpenGL_RC != NULL )
    {
        wglMakeCurrent( NULL, NULL );
        wglDeleteContext( hOpenGL_RC );
        hOpenGL_RC = NULL;
    }
            
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