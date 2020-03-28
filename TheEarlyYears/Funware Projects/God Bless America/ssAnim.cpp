
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
    XBackCounter = 0;


    // Init non-specific class members
    AnimCounter = 0;
    SaverTimer = 0;
    SaverSpeed = 20;
    ShareWareMode = FALSE;
    ShareWareTimer = 0;
    MusicTimer = 0;
    SongCount = 0;
    hOldPalette = NULL;
    Error_Number = 0;
    PreviewCounter = 0;

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
    if( SaverActive() && Mode() == ssSaver && (Settings.GetThemeMusicOn() || Settings.GetLoadYourOwnOnOff()) )
    {
        if( MPlayer.Init( Handle(), Instance(), DONT_USE_ERROR_DLG_BOXES ) )
        {
            // if they are loading their own music, allocate the string list
            // and load up all the sound file paths
            if( Settings.GetLoadYourOwnOnOff() )
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
    // Get the user's settings
    // Load all the ini file settings
    Settings.GetPrivateString();
        
    return TRUE;

}   // LoadControlPanelPrefs


/************************************************************************
*    FUNCTION NAME:         LoadGraphics                                                             
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

BOOL CAnimateSaver::LoadGraphics()
{
    BOOL Result = FALSE;
    TGif gif;           // GIF decoder which can load GIFs from file or resource
    TBitmap LoadTmp;

    // Init the classes using the screens DC. That way it doesn't have to make one
    buffer.InitActiveDC( GetScreenDC() );
    BigFlag.InitActiveDC( GetScreenDC() );
    SmallFlag.InitActiveDC( GetScreenDC() );

    // Load the metafile
    if( !GBAText.LoadFromResource( Instance(), "GBAText", "WMF" ) )
        goto FREE_MEMORY;

    // load the big flag
    if( !LoadTmp.LoadFromBuffer( gif.LoadFromResource( "BigFlag", "GIF", Instance() ) ) ) 
        goto FREE_MEMORY;

    // load the small flag
    if( !SmallFlag.LoadFromBuffer( gif.LoadFromResource( "SmallFlag", "GIF", Instance() ) ) ) 
        goto FREE_MEMORY;

    // Make the buffer. We always need the buffer
    if( SystemBitDepth > BIT_COUNT_8 && Mode() == ssSaver )
    {
        if( !buffer.CreateStandardBuffer( ScreenWidth, ScreenHeight ) )
            goto FREE_MEMORY;

        if( !LoadTmp.CreateBitmap( STANDARD_DIB ) )
            goto FREE_MEMORY;

        if( !SmallFlag.CreateBitmap( STANDARD_DIB ) )
            goto FREE_MEMORY;
    }
    else  // Use DIBSection for preview mode because XP doesn't lag so bad when doing a stretchdraw
    {
        // reset the width and height for preview. Makes it easer to see.
        if( Mode() == ssPreview )
        {
            ScreenWidth = 640;
            ScreenHeight = 480;
            SetRect( &ScreenRect, 0, 0, 640, 480 );
        }

        if( !buffer.CreateDIBSectionBuffer( ScreenWidth, ScreenHeight, BIT_COUNT_16 ) )
            goto FREE_MEMORY;

        if( !LoadTmp.CreateBitmap( DIB_SECTION_BMP, BIT_COUNT_16 ) )
            goto FREE_MEMORY;

        if( !SmallFlag.CreateBitmap( DIB_SECTION_BMP, BIT_COUNT_16 ) )
            goto FREE_MEMORY;
    }

    // Calculate the size of the big flag image and Create the flag buffer size
    if( !SizeMainImageForBlit( &LoadTmp ) )
        goto FREE_MEMORY;

    // For now, just fill with black
    buffer.ColorRect( &ScreenRect, BLACK_BRUSH );

    // Load the palette if we are in 8 bit or less mode
    if( SystemBitDepth <= 8 )
    {
        // Saver the old palette to select back into the dc. If this doesn't happen, the newly created
        // palette will never get deleted
        hOldPalette = SelectPalette( GetScreenDC(), BigFlag.GetPalette(), FALSE );
        RealizePalette( GetScreenDC() );

        // Delete the allocated bit data because we don't need it any more
        BigFlag.DeleteBitmapBits();
    }

    // Delete the allocated bit data because we don't need it any more
    SmallFlag.DeleteBitmapBits();

    Result = TRUE;

    FREE_MEMORY:;

    return Result;

}   // LoadGraphics


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

        // If this is a duel monitor senearo then we need to black out the screen
        if( IsDuelMonitor() )
            FillRect( GetScreenDC(), &WindowRect, BlackBrush );

        // Blit the background
        BlitBackImage( &buffer, &SmallFlag );

        // Blit the flag
        BigFlag.Draw( buffer.Handle(), FlagRect.left, FlagRect.top );

        // Draw the text
        GBAText.Draw( buffer.Handle(), &ScreenRect );

        // Now blit the buffer to the screen
        #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )
            buffer.Draw( GetScreenDC() );
        #else
            buffer.StretchDraw( GetScreenDC(), &ScreenRect );
        #endif
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
                SaverTimer = timeGetTime() + SaverSpeed;
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
        // Update the flag region
        BigFlag.Draw( buffer.Handle(), FlagRect.left, FlagRect.top );

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
                MPlayer.Play( "ProudToBe", "MIDI" );
        }

        // Or do we play music from the users drive
        else if( Settings.GetLoadYourOwnOnOff() )
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
        case PLAY_MUSIC:           PlayMusic();               break;

        // Blit to buffer
        case BUILD_FLAG_BUFFER:    BlitToTheBuffer();         break;

        // Blit to screen
        case BLIT_FLAG_TO_SCREEN:  BlitFromBufferToScreen();  break;
    }

    // Inc the animation counter
    AnimCounter = ( AnimCounter + 1 ) % MAX_FLAG_BLIT_OPERATIONS; 

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

void CAnimateSaver::BlitToTheBuffer()
{
    if( Mode() == ssPreview && PreviewCounter == 0 )
    {
        BlitBackImage( &buffer, &SmallFlag );

        BigFlag.Draw( buffer.Handle(), FlagRect.left, FlagRect.top );

        GBAText.Draw( buffer.Handle(), &ScreenRect );
    }
    else
    {
        BlitBackImage( &buffer, &SmallFlag );

        GBAText.Draw( buffer.Handle(), &ScreenRect );
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
    RECT rect;

    if( Mode() == ssSaver )
    {
        // Blit all the sides around the flag
        #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )

        // Repaint the whole screen if we have too.
        // This screen saver needs to repaint the whole screen in the even
        // the monitor wakes up from a sleep
        if( RepaintScreen() )
        {
            BigFlag.Draw( buffer.Handle(), FlagRect.left, FlagRect.top );
            buffer.Draw( GetScreenDC() );
        }
        else
        {
            SetRect( &rect, 0, 0, ScreenWidth, FlagRect.top );
            buffer.Draw( GetScreenDC(), &rect );

            SetRect( &rect, 0, FlagRect.top, FlagRect.left, FlagRect.bottom );
            buffer.Draw( GetScreenDC(), &rect );

            SetRect( &rect, FlagRect.left + FlagRect.right, FlagRect.top, FlagRect.left, FlagRect.bottom );
            buffer.Draw( GetScreenDC(), &rect );

            SetRect( &rect, 0, FlagRect.top + FlagRect.bottom, ScreenWidth, ScreenHeight - (FlagRect.top + FlagRect.bottom) );
            buffer.Draw( GetScreenDC(), &rect );
        }

        #else
            BigFlag.Draw( buffer.Handle(), FlagRect.left, FlagRect.top );
            buffer.StretchDraw( GetScreenDC(), &WindowRect );
        #endif
    }
    else
    {
        // update the screen every 5 intervals so that it doesn't hog the CPU cycles
        if( PreviewCounter == 0 )
            buffer.StretchDraw( GetScreenDC(), &WindowRect );

        // Inc the preview counter
        PreviewCounter = ( PreviewCounter + 1 ) % MAX_PREVIEW_TIMER_COUNTS;
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

BOOL CAnimateSaver::ShowShareWareScreen()
{
    BOOL Result = TRUE;

    // If this is the shareware version, show the shareware screen when ready
    if( ShareWareTimer < timeGetTime() && Mode() == ssSaver )
    {
        // Display the shareware screen
        // Don't do the below if the screen savers been killed
        if( DisplayShareWareScreen() )
        {
            ShareWareTimer = timeGetTime() + SHAREWARE_MESSAGE_DELAY;
                
            // RESET THE SCREEN HERE



            // RESET THE SCREEN HERE

            // Blit the whole buffer to the screen
            buffer.Draw( GetScreenDC(), &ScreenRect );

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

CAnimateSaver::~CAnimateSaver()
{
    if( hOldPalette )
        SelectPalette( GetScreenDC(), hOldPalette, FALSE );
            
}   // Destructor


/************************************************************************
*    FUNCTION NAME:         SizeMainImageForBlit                                                             
*
*    DESCRIPTION:           Resize the big flag to a smaller size if need be.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hParent  - Handle of the parent windowe 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CAnimateSaver::SizeMainImageForBlit( PTBitmap pMainDib )
{
    BOOL SizeToFit = FALSE;
    int FrameBorder;
    BOOL Result = FALSE;
    RECT r;

    // Calculate the frame border
    FrameBorder = ScreenWidth / 20;

    // See if we need to size it to fit
    if( pMainDib->width() > (ScreenWidth-(FrameBorder*2)) ||
        pMainDib->height() > (ScreenHeight-(FrameBorder*2)) )
    {
        SizeToFit = TRUE;
    }

    // Size the main image to fit the buffer
    if( SizeToFit )
    {
        // Size the main image to fit into the buffer size
        if( SizeToFitRect( &FlagRect, pMainDib->width(), pMainDib->height(), 
                           ScreenWidth-(FrameBorder*2), ScreenHeight-(FrameBorder*2) ) )
        {
            // Add in the frame
            FlagRect.left += FrameBorder;
            FlagRect.top += FrameBorder;

            // Get just the size of the rect
            SetRect( &r, 0, 0, FlagRect.right, FlagRect.bottom );

            if( SystemBitDepth > BIT_COUNT_8 && Mode() == ssSaver )
            {
                // Create the flag buffer size
                if( !BigFlag.CreateStandardBuffer( FlagRect.right, FlagRect.bottom ) )
                    goto HANDLE_ERROR;
            }
            else
            {
                if( !BigFlag.CreateDIBSectionBuffer( FlagRect.right, FlagRect.bottom, BIT_COUNT_16 ) )
                    goto HANDLE_ERROR;
            }

            // Blit the image to the buffer
            pMainDib->StretchDraw( BigFlag.Handle(), &r );
        }
        else
            goto HANDLE_ERROR;
    }
    else  // Center the main image in the buffer
    {
        // Set the flag rect
        SetRect( &FlagRect, abs(ScreenWidth - pMainDib->width()) / 2,
                            abs(ScreenHeight - pMainDib->height()) / 2,
                            pMainDib->width(), pMainDib->height() );

        // Create the flag buffer size
        if( SystemBitDepth > BIT_COUNT_8 && Mode() == ssSaver )
        {
            // Create the flag buffer size
            if( !BigFlag.CreateStandardBuffer( FlagRect.right, FlagRect.bottom ) )
                goto HANDLE_ERROR;
        }
        else
        {
            if( !BigFlag.CreateDIBSectionBuffer( FlagRect.right, FlagRect.bottom, BIT_COUNT_16 ) )
                goto HANDLE_ERROR;
        }

        // Blit the image to the buffer
        pMainDib->Draw( BigFlag.Handle() );
    }

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // SizeMainImageForBlit


/************************************************************************
*    FUNCTION NAME:         BldBuffer_BlitBackImage                                                             
*
*    DESCRIPTION:           Blit the back image to the buffer
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hParent  - Handle of the parent windowe 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::BlitBackImage( PTBitmap pBuffer, PTBitmap pBackDib )
{
    int Row, Col, i, j;
    int X, Y;

    // Calculate the number of rows and colums
    if( pBuffer->width() % pBackDib->width() )
        Row = (pBuffer->width() / pBackDib->width()) + 2;
    else
        Row = pBuffer->width() / pBackDib->width() + 1;

    if( pBuffer->height() % pBackDib->height() )
        Col = (pBuffer->height() / pBackDib->height()) + 2;
    else
        Col = pBuffer->height() / pBackDib->height() + 1;

    // Tile the bitmat to the buffer
    for( j = 0; j < Col; ++j )
    {
        for( i = 0; i < Row; ++i )
        {           
            // Set the Y offset
            Y = pBackDib->height() * j;

            // Set the X offset
            if( j % 2 )
                X = ((pBackDib->width() * i) + XBackCounter) - pBackDib->width();
            else
                X = (pBackDib->width() * i) - XBackCounter;

            if( X < FlagRect.left || X + pBackDib->width() > FlagRect.right ||
                Y < FlagRect.top || Y + pBackDib->height() > FlagRect.bottom )
            {
                pBackDib->Draw( pBuffer->Handle(), X, Y );
            }
        }
    }

    // inc the counter
    XBackCounter = ( XBackCounter + 4 ) % pBackDib->width();

}   // BlitBackImage


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