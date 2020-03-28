
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
#include "Metafile.h"        // Class for loading and displaying Windows metafiles and enhanced metafiles



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
    // Init non-specific class members
    AnimCounter = 0;
    SaverTimer = 0;
    MusicTimer = 0;
    SongCount = 0;
    hOldPalette = NULL;
    Error_Number = 0;
    PreviewCounter = 0;

    // Init screen saver specific functions
    NumFishAnim = 0;
    FishPickTimer = 0;
    BubbleCount = 0;
    FishCount = 0;
    BackFishCount = 0;
    pBubble = NULL;
    backFish = NULL;
    fish = NULL;

}   // Constructor


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

    // Init the fish data
    if( !InitFishData() )
        goto HANDLE_ERROR;

    // Init the fish data
    if( Settings.GetBubblesOnOff() )
        if( !InitBubbleData() )
            goto HANDLE_ERROR;

    // Init the fish data
    if( Settings.GetBackFishOnOff() )
        if( !InitBackFishData() )
            goto HANDLE_ERROR;

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
        // Allocate the media player
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

    // Get the fish count
    FishCount = (Settings.GetFishCount()+1) * MIN_REQUIRED_FISH;
    
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
    TBitmapinfo fh;
    TBitmap FishArtTmp;

    // Init the classes using the screens DC. That way it doesn't have to make one
    buffer.InitActiveDC( GetScreenDC() );
    backBuffer.InitActiveDC( GetScreenDC() );
    FishArt.InitActiveDC( GetScreenDC() );

    // Load the fish. Load the GIF graphic from resource.
    if( SystemBitDepth > BIT_COUNT_8 && Mode() == ssSaver )
    {
        // Load the RGB GIF file
        // This RGB image was saved as an indexed 8 bit image so that it could be GIF compressed.
        // It was never really an 8 bit image, we just made it look that way
        if( !FishArtTmp.LoadFromBuffer( gif.LoadFromResource( "fishRGB", "GIF", Instance() ) ) ) 
            goto FREE_MEMORY;

        // Make the RGB buffer for the stars and planets
        if( !FishArt.CreateStandardBuffer( FishArtTmp.width(), FishArtTmp.height() / 3 ) )
        {
            strcpy( errorStr, "Can't allocate fish art buffer. Your system resources may be low so try restarting your computer." ); 
            goto FREE_MEMORY;
        }

        // Check to see that it's not empty
        if( !FishArtTmp.IsEmpty() )
        {
            // Copy over the bitmap info data
            memcpy( &fh, FishArtTmp.dib(), sizeof(TBitmapinfo) );

            // Now we are going to change some information so that it appears to be a 24 bit image again
            // Change the information needed to do the conversion
            fh.bmiHeader.biHeight /= 3;
            fh.bmiHeader.biBitCount = 24;
            fh.bmiHeader.biSizeImage = 0;
            fh.bmiHeader.biClrImportant = 0;
            fh.bmiHeader.biClrUsed = 0;

            // Blit the image to the buffer.
            StretchDIBits( FishArt.Handle(), 0, 0, FishArtTmp.width(), fh.bmiHeader.biHeight,
                           0, 0, FishArtTmp.width(), fh.bmiHeader.biHeight, FishArtTmp.bits(), 
                           (BITMAPINFO*)&fh, DIB_RGB_COLORS, SRCCOPY );
        }
        else
            goto FREE_MEMORY;

    }
    else  // 256 color verion of the artwork in computer is in 256 color mode or if this is the preview
    {     // Use DIBSection for preview mode because XP doesn't lag so bad when doing a stretchdraw
        if( !FishArt.LoadFromBuffer( gif.LoadFromResource( "fish256", "GIF", Instance() ) ) ) 
            goto FREE_MEMORY;

        if( !FishArt.CreateBitmap( DIB_SECTION_BMP, BIT_COUNT_16 ) )
        {
            strcpy( errorStr, "Can't allocate fish art buffer. Your system resources may be low so try restarting your computer." );
            goto FREE_MEMORY;
        }
    }

    // Make the buffer. We always need the buffer
    if( SystemBitDepth > BIT_COUNT_8 && Mode() == ssSaver )
    {
        if( !buffer.CreateStandardBuffer( DesktopWidth, DesktopHeight ) )
        {
            strcpy( errorStr, "Can't allocate work buffer. Your system resources may be low so try restarting your computer." );
            goto FREE_MEMORY;
        }
    }
    else
    {
        if( !buffer.CreateDIBSectionBuffer( DesktopWidth, DesktopHeight, BIT_COUNT_16 ) )
        {
            strcpy( errorStr, "Can't allocate work buffer. Your system resources may be low so try restarting your computer." );
            goto FREE_MEMORY;
        }
    }

    // Jump out if the screen saver has been killed soon
    if( !SaverActive() )
        return TRUE;

    // Make the back buffer
    if( SystemBitDepth > BIT_COUNT_8 && Mode() == ssSaver )
    {
        if( !backBuffer.CreateStandardBuffer( DesktopWidth, DesktopHeight ) )
        {
            strcpy( errorStr, "Can't allocate background buffer. Your system resources may be low so try restarting your computer." );
            goto FREE_MEMORY;
        }
    }
    else
    {
        if( !backBuffer.CreateDIBSectionBuffer( DesktopWidth, DesktopHeight, BIT_COUNT_16 ) )
        {
            strcpy( errorStr, "Can't allocate background buffer. Your system resources may be low so try restarting your computer." );
            goto FREE_MEMORY;
        }
    }

    // Build the background
    if( !BuildBackground() )
    {
        strcpy( errorStr, "Error building background buffer. Your system resources may be low so try restarting your computer." );
        goto FREE_MEMORY;
    }

    // Load the palette if we are in 8 bit or less mode
    if( SystemBitDepth <= 8 )
    {
        // Saver the old palette to select back into the dc. If this doesn't happen, the newly created
        // palette will never get deleted
        hOldPalette = SelectPalette( GetScreenDC(), FishArt.GetPalette(), FALSE );
        RealizePalette( GetScreenDC() );

        // Delete the allocated bit data because we don't need it any more
        FishArt.DeleteBitmapBits();
    }

    Result = TRUE;

    FREE_MEMORY:;

    return Result;
}   // LoadGraphics


/************************************************************************
*    FUNCTION NAME:         BuildBackground                                                             
*
*    DESCRIPTION:           Build the background
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

BOOL CAnimateSaver::BuildBackground()
{
    int Row, i, XOffset;
    TMetafile metafile;
    RECT sourceSRCAND, sourceSRCPAINT, source;
    POINT point;
    PTRectArry pProps;
    TLoadRes LoadRes;
    BOOL Result = FALSE;

    // Build the back buffer
    if( SystemBitDepth > BIT_COUNT_8 )
    {
        if( metafile.LoadFromResource( Instance(), "gradback", "WMF" ) )
            metafile.Draw( backBuffer.Handle(), &DesktopRect );
        else
            BackSolidColor();   
    }
    else
        BackSolidColor();

    // See if there is some text before trying to output text to the screen
    if( strlen( Settings.GetPtrBusinessName() ) )
    {
        // Give the canvas class the active DC
        bufferCanvas.InitActiveDC( backBuffer.Handle() );

        // Create the font
        bufferCanvas.SetFont( NULL, 0, 0, Settings.GetTxtColor(), 0, Settings.GetLogFont() );

        // Set the background to transparent
        bufferCanvas.SetBackgroundMode( TRANSPARENT );

        // Output the text to the buffer
        bufferCanvas.WrapText( Settings.GetPtrBusinessName(), 0, DesktopHeight / 6, ScreenWidth, 
                               DesktopHeight - (DesktopHeight / 6), 0, CENTER_JUSTIFY );

        // We now need to give it the buffer DC because that is where we need
        // to blit from here on. To do that we also need to create the font again
        bufferCanvas.InitActiveDC( buffer.Handle() );
    }

    // Load the office props art offsets from the resource
    if( !LoadTheResource( Instance(), "OfficePropsData", RT_RCDATA, &LoadRes ) )
        goto HANDLE_ERROR;

    // Get a pointer to the office props data
    pProps = (PTRectArry)LoadRes.pData;

    // Get the  offset
    XOffset = DesktopWidth / pProps->count;

    // Blit only the props that are behind the sand
    for( i = 0; i < pProps->count; ++i )
    {
        if( i % 2 )
        {
            point.x = (abs(XOffset - pProps->Art[i].Right) / 2) + (XOffset * i);
            point.y = DesktopHeight- pProps->Art[i].Bottom - SAND_BAR_LOW_H;
            
            SetRect( &source, pProps->Art[i].Left, pProps->Art[i].Top + pProps->Art[i].Bottom, 
                              pProps->Art[i].Right, pProps->Art[i].Bottom );
            FishArt.SetCopyMode( SRCAND );
            FishArt.Draw( &backBuffer, &point, &source );
            FishArt.SetCopyMode( SRCPAINT );
            source.top = pProps->Art[i].Top;
            FishArt.Draw( &backBuffer, &point, &source );
        }
    }

    // Calculate the number of rows
    if( backBuffer.width() % SAND_BAR_W )
        Row = (backBuffer.width() / SAND_BAR_W) + 1;
    else
        Row = backBuffer.width() / SAND_BAR_W;

    // Set the source rect
    SetRect( &sourceSRCAND, SAND_BAR_X, SAND_BAR_Y+SAND_BAR_H, SAND_BAR_W, SAND_BAR_H );
    SetRect( &sourceSRCPAINT, SAND_BAR_X, SAND_BAR_Y, SAND_BAR_W, SAND_BAR_H );

    // Set the Y offset
    point.y = DesktopHeight - SAND_BAR_H;

    // Blit the sand bar across the bottom
    for( i = 0; i < Row; ++i )
    {
        point.x = i * SAND_BAR_W;
        FishArt.SetCopyMode( SRCAND );
        FishArt.Draw( &backBuffer, &point, &sourceSRCAND );
        FishArt.SetCopyMode( SRCPAINT );
        FishArt.Draw( &backBuffer, &point, &sourceSRCPAINT );
    }

    // Blit only the props that are on top of the sand
    for( i = 0; i < pProps->count; ++i )
    {
        if( i % 2 == 0 )
        {
            point.x = (abs(XOffset - pProps->Art[i].Right) / 2) + (XOffset * i);
            point.y = DesktopHeight- pProps->Art[i].Bottom - (SAND_BAR_H / 2);
            
            SetRect( &source, pProps->Art[i].Left, pProps->Art[i].Top + pProps->Art[i].Bottom, 
                              pProps->Art[i].Right, pProps->Art[i].Bottom );
            FishArt.SetCopyMode( SRCAND );
            FishArt.Draw( &backBuffer, &point, &source );
            FishArt.SetCopyMode( SRCPAINT );
            source.top = pProps->Art[i].Top;
            FishArt.Draw( &backBuffer, &point, &source );
        }
    }

    // Load the fish props art offsets from the resource
    if( !LoadTheResource( Instance(), "FishPropsData", RT_RCDATA, &LoadRes ) )
        goto HANDLE_ERROR;

    // Get a pointer to the fish props data
    pProps = (PTRectArry)LoadRes.pData;

    // Blit the props randomly across the bottom
    for( i = 0; i < pProps->count; ++i )
    {
        point.x = rand() % (DesktopWidth - pProps->Art[i].Right);
        point.y = DesktopHeight - SAND_BAR_H - (pProps->Art[i].Bottom / 2);
        SetRect( &source, pProps->Art[i].Left, pProps->Art[i].Top + pProps->Art[i].Bottom, 
                          pProps->Art[i].Right, pProps->Art[i].Bottom );
        FishArt.SetCopyMode( SRCAND );
        FishArt.Draw( &backBuffer, &point, &source );
        FishArt.SetCopyMode( SRCPAINT );
        source.top = pProps->Art[i].Top;
        FishArt.Draw( &backBuffer, &point, &source );
    }

    // Copy the back buffer to the main buffer
    backBuffer.Draw( &buffer );

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // BuildBackground


/************************************************************************
*    FUNCTION NAME:         BackSolidColor                                                             
*
*    DESCRIPTION:           Setup the background with a solid color.
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

void CAnimateSaver::BackSolidColor()
{
    int Row, Col, i, j;
    RECT BrushRect;
    POINT point;

    // Calculate the number of rows and colums
    if( backBuffer.width() % BACK_COLOR_SIZE )
        Row = (backBuffer.width() / BACK_COLOR_SIZE) + 1;
    else
        Row = backBuffer.width() / BACK_COLOR_SIZE;

    if( backBuffer.height() % BACK_COLOR_SIZE )
        Col = (backBuffer.height() / BACK_COLOR_SIZE) + 1;
    else
        Col = backBuffer.height() / BACK_COLOR_SIZE;

    // Set the rect to blit the solid color background
    SetRect( &BrushRect, BACK_COLOR_X, BACK_COLOR_Y, BACK_COLOR_SIZE, BACK_COLOR_SIZE );

    // Tile the bitmat to the buffer
    for( j = 0; j < Col; ++j )
    {
        for( i = 0; i < Row; ++i )
        {
            // Init the point
            point.x = BACK_COLOR_SIZE * i;
            point.y = BACK_COLOR_SIZE * j;

            // Blit the back color
            FishArt.Draw( &backBuffer, &point, &BrushRect );
        }
    }

}   // BackSolidColor


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

        BlitToTheBuffer();

        // Now blit the buffer to the screen only if not in shareware mode
        buffer.Draw( GetScreenDC(), &DesktopRect );       
    }
    #endif

}   // DisplayBufferOnStartUp


/************************************************************************
*    FUNCTION NAME:         AnimateSaver                                                             
*
*    DESCRIPTION:           Animate the screen saver. Main screen saver
*                           loop function.
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
            // Time out the screen saver
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

        // Blit to screen
        case BLIT_TO_SCREEN:   BlitFromBufferToScreen();  break;
    }

    // Inc the animation counter
    AnimCounter = ( AnimCounter + 1 ) % NUM_BLIT_OPERATIONS;

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
    int i;

    // Skip this part if we are in preview mode to help the preview mode animate faster
    if( Mode() == ssPreview && PreviewCounter != 1 )
        return;

    // Erase the back fishfish
    for( i = 0; i < BackFishCount; ++i )
        backBuffer.Draw( &buffer, &backFish[i].Dest );

    // Erase the fish
    for( i = 0; i < pFishDataAry->count; ++i )
        if( fish[i].Active )
            backBuffer.Draw( &buffer, &fish[i].Dest );

    // Erase the bubbles
    for( i = 0; i < BubbleCount; ++i )
        backBuffer.Draw( &buffer, &pBubble[i].Dest );

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
    // Update the fish
    UpdateFishPositions();

    // Update the bubbles
    if( pBubble )
        UpdateBubblePositions();

    // Update the background fish
    if( backFish )
        UpdateBackFishPositions();

    // Is it time to send the boss fish in?
    InitBossFish();

    // If the boss is running move him along
    UpdateBossFishPos();

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
    int i, HalfBubbles;
    DWORD TmpTimer;
    RECT rect, source;

    // Skip this part if we are in preview mode to help the preview mode animate faster
    if( Mode() == ssPreview && PreviewCounter != 0 )
        return;

    // init the temp timer
    TmpTimer = timeGetTime();

    // Blit the background fish
    for( i = 0; i < BackFishCount; ++i )
    {
        if( backFish[i].AnimTimer < TmpTimer )
        {
            // inc the fish's animation frame
            backFish[i].FrameCounter = ( backFish[i].FrameCounter + 1 ) % backFish[i].NumFrames;

            // Reset the animation timer
            backFish[i].AnimTimer = TmpTimer + backFish[i].AnimDelay;
        }

        FishArt.SetCopyMode( SRCAND );
        rect = backFish[i].Source;
        rect.top += rect.bottom;
        source = backFish[i].Source;
        source.left += source.right * backFish[i].FrameCounter;
        rect.left += rect.right * backFish[i].FrameCounter;
        FishArt.Draw( &buffer, &backFish[i].Dest, &rect );
        FishArt.SetCopyMode( SRCPAINT );
        FishArt.Draw( &buffer, &backFish[i].Dest, &source );        
    }

    // Preset the scan mode
    FishArt.SetCopyMode( SRCPAINT );

    // Get half of the bubbles
    HalfBubbles = BubbleCount / 2;

    // Blit the first half of the bubblesbubbles
    for( i = 0; i < HalfBubbles; ++i )      
        FishArt.Draw( &buffer, &pBubble[i].Dest, &pBubble[i].Art );

    // Blit the text down if there is some
    // If we have a handle, we have text
    // We want to confine the text to the first monitor is this is a duel monitor
    if( bufferCanvas.Handle() )
        bufferCanvas.WrapText( Settings.GetPtrBusinessName(), 0, DesktopHeight / 6, ScreenWidth, 
                           DesktopHeight - (DesktopHeight / 6), 0, CENTER_JUSTIFY );

    // Blit the fish
    for( i = 0; i < pFishDataAry->count; ++i )
    {
        if( fish[i].Active )
        {
            if( fish[i].AnimTimer < TmpTimer )
            {
                // inc the fish's animation frame
                fish[i].FrameCounter = ( fish[i].FrameCounter + 1 ) % fish[i].NumFrames;

                // Reset the animation timer
                fish[i].AnimTimer = TmpTimer + fish[i].AnimDelay;
            }

            FishArt.SetCopyMode( SRCAND );
            rect = fish[i].Source;
            rect.top += rect.bottom;
            source = fish[i].Source;
            source.left += source.right * fish[i].FrameCounter;
            rect.left += rect.right * fish[i].FrameCounter;
            FishArt.Draw( &buffer, &fish[i].Dest, &rect );
            FishArt.SetCopyMode( SRCPAINT );
            FishArt.Draw( &buffer, &fish[i].Dest, &source );
        }
    }

    // Blit the last half of the bubbles
    for( i = HalfBubbles; i < BubbleCount; ++i )
        FishArt.Draw( &buffer, &pBubble[i].Dest, &pBubble[i].Art );

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
    int i;

    // Use a local variable for speed in these tight loops
    HDC ScreenDC = GetScreenDC();

    if( Mode() == ssSaver )
    {
        #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )

        // repaint the whole screen if we have too
        // This screen saver needs to repaint the whole screen in the even
        // the monitor wakes up from a sleep
        if( RepaintScreen() )
        {
            buffer.Draw( ScreenDC );
        }
        else
        {
            // Paint fish to the screen
            for( i = 0; i < pFishDataAry->count; ++i )
                if( fish[i].Active )
                    buffer.Draw( ScreenDC, &fish[i].Dest );

            // Paint bubbles to the screen
            for( i = 0; i < BubbleCount; ++i )
                buffer.Draw( ScreenDC, &pBubble[i].Dest );

            // Paint background fish to the screen
            for( i = 0; i < BackFishCount; ++i )
                buffer.Draw( ScreenDC, &backFish[i].Dest );
        }

        #else
            buffer.StretchDraw( ScreenDC, &WindowRect );
        #endif
    }
    else
    {
        // update the screen every X number of intervals so that it doesn't hog the CPU cycles
        if( PreviewCounter == 0 )
            buffer.StretchDraw( ScreenDC, &WindowRect );

        PreviewCounter = ( PreviewCounter + 1 ) % MAX_PREVIEW_TIMER_COUNTS;
    }

    // Show an error screen if there is a problem playing music
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
    if( hOldPalette )
        SelectPalette( GetScreenDC(), hOldPalette, FALSE );

    if( pBubble )
        delete [] pBubble;

    if( backFish )
        delete [] backFish;

    if( fish )
        delete [] fish;
            
}   // Destructor


/************************************************************************
*    FUNCTION NAME:         UpdateFishPositions                                                             
*
*    DESCRIPTION:           Update all the fish positions that are active
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

void CAnimateSaver::UpdateFishPositions()
{
    int i;

    for( i = 0; i < pFishDataAry->count-1; ++i ) // exclude boss fish
    {
        if( fish[i].Active )
        {
            switch( fish[i].Movement )
            {
                case MOVE_LEFT:
                    fish[i].Dest.left -= fish[i].PixelSpeed;

                    // Fish is done. Stop the animation
                    if( fish[i].Dest.left <= -fish[i].Dest.right )
                    {
                        fish[i].Active = FALSE;
                        --NumFishAnim;
                    }
                break;

                case MOVE_RIGHT:
                    fish[i].Dest.left += fish[i].PixelSpeed;

                    // Fish is done. Stop the animation
                    if( fish[i].Dest.left >= DesktopWidth )
                    {
                        fish[i].Active = FALSE;
                        --NumFishAnim;
                    }
                break;
            }

            // Is it time to inc the float offsets
            // This way each fish will float differenly because the Y offsets
            // are also inc due to a count.
            if( !fish[i].BottomOnly && fish[i].FloatIncCounter == 0 )
            {
                // Update the Y position
                fish[i].Dest.top += fish[i].YFloat[ fish[i].FloatCounter ];

                // inc the float counter
                fish[i].FloatCounter = ( fish[i].FloatCounter + 1 ) % MAX_FLOAT_MOVEMENTS;
            }

            // inc the counter
            fish[i].FloatIncCounter = ( fish[i].FloatIncCounter + 1 ) % fish[i].FloatIncCount;
        }
    }

    // See if we'll have some other fish animating
    PickNextFishToAnimate();

}   // UpdateFishPositions


/************************************************************************
*    FUNCTION NAME:         UpdateBossFishPos                                                             
*
*    DESCRIPTION:           Update the positions of the boss fish
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

void CAnimateSaver::UpdateBossFishPos()
{
    int i;

    if( fish[ BOSS_FISH ].Active )
    {
        if( fish[ BOSS_FISH ].Movement == MOVE_LEFT )
        {
            fish[ BOSS_FISH ].Dest.left -= fish[ BOSS_FISH ].PixelSpeed;

            // Fish is done. Stop the animation
            if( fish[ BOSS_FISH ].Dest.left <= -fish[ BOSS_FISH ].Dest.right )
            {
                fish[ BOSS_FISH ].Active = FALSE;
            }
        }
        else
        {
            fish[ BOSS_FISH ].Dest.left += fish[ BOSS_FISH ].PixelSpeed;

            // Fish is done. Stop the animation
            if( fish[ BOSS_FISH ].Dest.left >= DesktopWidth )
            {
                fish[ BOSS_FISH ].Active = FALSE;
            }
        }

        // Is it time to inc the float offsets
        // This way each fish will float differenly because the Y offsets
        // are also inc due to a count.
        if( fish[ BOSS_FISH ].FloatIncCounter == 0 )
        {
            // Update the Y position
            fish[ BOSS_FISH ].Dest.top += fish[ BOSS_FISH ].YFloat[ fish[ BOSS_FISH ].FloatCounter ];

            // inc the float counter
            fish[ BOSS_FISH ].FloatCounter = ( fish[ BOSS_FISH ].FloatCounter + 1 ) % MAX_FLOAT_MOVEMENTS;
        }

        // inc the counter
        fish[ BOSS_FISH ].FloatIncCounter = ( fish[ BOSS_FISH ].FloatIncCounter + 1 ) % fish[ BOSS_FISH ].FloatIncCount;

        // Reset all the other fish if the boss fish is done
        if( fish[ BOSS_FISH ].Active == FALSE )
        {
            // Set everyones swim speed to the max
            for( i = 0; i < pFishDataAry->count-1; ++i )
            {
                // Get the original pixel speed
                fish[i].PixelSpeed = pFishDataAry->FishData[i].PixelSpeed;

                // Get the original animation delay
                fish[i].AnimDelay = pFishDataAry->FishData[i].AnimDelay;

                // Get the original float inc counter
                fish[i].FloatIncCount = pFishDataAry->FishData[i].FloatIncCount;
            }

            // Set the timer for the boss to show up
            BossFishTimer = timeGetTime() + ( rand() % BOSS_FISH_PICK_TIME_MAX ) + BOSS_FISH_PICK_TIME_MIN;
        }
    }

}   // UpdateBossFishPos


/************************************************************************
*    FUNCTION NAME:         UpdateBackFishPositions                                                             
*
*    DESCRIPTION:           Update all the background fish positions
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

void CAnimateSaver::UpdateBackFishPositions()
{
    int i;
    BOOL RestartFish;

    for( i = 0; i < BackFishCount; ++i )
    {
        // Rest the flag
        RestartFish = FALSE;

        // Update the source rect
        if( backFish[ i ].Movement == MOVE_LEFT )
        {
            backFish[i].Dest.left -= backFish[i].PixelSpeed;

            // The fish has reached the side, restart the animation
            if( backFish[i].Dest.left <= -backFish[i].Dest.right )
                RestartFish = TRUE;
        }
        else
        {
            backFish[i].Dest.left += backFish[i].PixelSpeed;

            // The fish has reached the side, restart the animation
            if( backFish[i].Dest.left >= DesktopWidth )
                RestartFish = TRUE;
        }

        // Do we restart the fish because it has reached the other side?
        if( !RestartFish )
        {
            // Is it time to inc the float offsets
            // This way each fish will float differenly because the Y offsets
            // are also inc due to a count.
            if( backFish[i].FloatIncCounter == 0 )
            {
                // Update the Y position
                backFish[i].Dest.top += backFish[i].YFloat[ backFish[i].FloatCounter ];

                // inc the float counter
                backFish[i].FloatCounter = ( backFish[i].FloatCounter + 1 ) % MAX_FLOAT_MOVEMENTS;
            }

            // inc the counter
            backFish[i].FloatIncCounter = ( backFish[i].FloatIncCounter + 1 ) % backFish[i].FloatIncCount;
        }
        else
        {
            // Set if it will be comming from the left, right.
            backFish[ i ].Movement = rand() % MAX_NUM_OF_MOVEMENTS;

            // Reset the source
            backFish[i].Source = backFish[i].Art;
            
            // Update the source rect
            if( backFish[ i ].Movement == MOVE_LEFT )
            {
                // The reverse art is to the left or under the art
                backFish[i].Source.left = backFish[i].Art.left + (backFish[i].Art.right * backFish[i].NumFrames);
                backFish[ i ].Dest.left = DesktopWidth;
            }
            else
            {
                backFish[ i ].Source.left = backFish[ i ].Art.left;
                backFish[ i ].Source.top = backFish[ i ].Art.top;
                backFish[ i ].Dest.left = -backFish[ i ].Dest.right;
            }

            // Set the height of the fish
            backFish[ i ].Dest.top  = (rand() % ( DesktopHeight - backFish[ i ].Art.bottom - BOTTOM_PROP_OFFSET - (MAX_UP_DOWN_OFFSET*2)))+MAX_UP_DOWN_OFFSET;

            // If the fish is on the bottom half of the screen use the darker fish
            if( backFish[i].Dest.top > DesktopHeight / 2 )
                backFish[i].Source.top += DARK_FISH_OFFSET;

            // Set the pixel speed
            // Reset the animation timer
            backFish[ i ].AnimTimer = timeGetTime() + backFish[ i ].AnimDelay;
        }
    }
}   // UpdateBackFishPositions


/************************************************************************
*    FUNCTION NAME:         UpdateBubblePositions                                                             
*
*    DESCRIPTION:           Update all the bubble positions
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

void CAnimateSaver::UpdateBubblePositions()
{
    int i;

    for( i = 0; i < BubbleCount; ++i )
    {
        // Inc the offset
        pBubble[i].Dest.top -= pBubble[i].PixelSpeed;

        // Counter that inc depending on how fast the bubble is moving
        if( pBubble[i].FloatIncCount == 0 )
        {
            // inc the float motion
            pBubble[i].Dest.left += pBubble[i].YFloat[ pBubble[i].FloatCounter ];

            // inc the float counter
            pBubble[i].FloatCounter = ( pBubble[i].FloatCounter + 1 ) % MAX_BUB_FLOAT_MOVEMENTS;
        }

        pBubble[i].FloatIncCount = ( pBubble[i].FloatIncCount + 1 ) % pBubble[i].FloatSpeed;

        // The bubble has reached the top, restart the animation
        if( pBubble[i].Dest.top <= -pBubble[i].Dest.bottom )
        {
            // Set the Y offset
            pBubble[i].Dest.top = DesktopHeight - SAND_BAR_H; // pBubble[i].Art.bottom 

            // Set the X offset
            pBubble[i].Dest.left  = (rand() % ( DesktopWidth - pBubble[i].Art.right - (MAX_LEFT_RIGHT_OFFSET*2)))+MAX_LEFT_RIGHT_OFFSET;
        }
    }

}   // UpdateBubblePositions


/************************************************************************
*    FUNCTION NAME:         PickNextFishToAnimate                                                             
*
*    DESCRIPTION:           Picks the next fish to animate
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

void CAnimateSaver::PickNextFishToAnimate()
{
    // Randomly pick a fish
    int pick = rand() % (pFishDataAry->count-1); // Don't pick the boss fish
    int i;

    if( !fish[ BOSS_FISH ].Active && !fish[ pick ].Active && (NumFishAnim < FishCount) && (FishPickTimer < timeGetTime()) )
    {
        // Set if it will be comming from the left, right.
        fish[ pick ].Movement = rand() % MAX_NUM_OF_MOVEMENTS;
        
        // Update the source rect
        if( fish[ pick ].Movement == MOVE_LEFT )
        {
            // The reverse art is to the left or under the art
            fish[ pick ].Source.left = fish[ pick ].Art.left + 
                                         (fish[ pick ].Art.right * fish[ pick ].NumFrames);

            fish[ pick ].Dest.left = DesktopWidth;
        }
        else
        {
            fish[ pick ].Source.left = fish[ pick ].Art.left;
            fish[ pick ].Source.top = fish[ pick ].Art.top;
            fish[ pick ].Dest.left = -fish[ pick ].Dest.right;
        }

        // Set the height of the fish
        if( fish[ pick ].BottomOnly )
            fish[ pick ].Dest.top  = DesktopHeight - fish[ pick ].Art.bottom - ((rand() % MAX_BOTTOM_EDGE)+MIN_BOTTOM_EDGE);
        else
            fish[ pick ].Dest.top  = (rand() % ( DesktopHeight - fish[ pick ].Art.bottom - SAND_BAR_H - (MAX_UP_DOWN_OFFSET*2)))+MAX_UP_DOWN_OFFSET;

        // Let's make sure this fish will not be sittng on top of 
        // another fish for the duration of it's journey
        for( i = 0; i < pFishDataAry->count-1; ++i ) // exclude boss fish
        {
            if( fish[i].Active &&                                   // If this fish is active, check it
                fish[ pick ].Movement == fish[i].Movement &&        // See if they are moving in the same direction
                fish[ pick ].PixelSpeed == fish[i].PixelSpeed &&    // See if they are moving the same speed
                abs( fish[ pick ].Dest.top - fish[i].Dest.top ) < fish[ pick ].Art.right * 2 && // Are we far enough away from the fish
                abs( fish[ pick ].Dest.left - fish[i].Dest.left ) < fish[ pick ].Art.right * 2 )
            {
                // This fish is too close to another fish so break out
                // and better luck next time
                return;
            }
        }

        // Set the active state
        fish[ pick ].Active = TRUE;
        // Set the pixel speed
        // Reset the animation timer
        fish[ pick ].AnimTimer = timeGetTime() + fish[ pick ].AnimDelay;

        // Reset the timer for picking another ship
        FishPickTimer = timeGetTime() + ( rand() % FISH_PICK_TIME_MAX ) + FISH_PICK_TIME_MIN;

        ++NumFishAnim;          
    }
    // If there are the max ships and the timer has run out, reset the timer.
    else if( NumFishAnim == FishCount && FishPickTimer < timeGetTime() )
        FishPickTimer = timeGetTime() + ( rand() % FISH_PICK_TIME_MAX ) + FISH_PICK_TIME_MIN;

}   // PickNextFishToAnimate


/************************************************************************
*    FUNCTION NAME:         InitBossFish                                                             
*
*    DESCRIPTION:           Is it time to bring the boss back in?
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

void CAnimateSaver::InitBossFish()
{
    int i;

    if( !fish[ BOSS_FISH ].Active && (BossFishTimer < timeGetTime()) )
    {
        // Set if it will be comming from the left, right.
        fish[ BOSS_FISH ].Movement = rand() % MAX_NUM_OF_MOVEMENTS;

        // Update the source rect
        if( fish[ BOSS_FISH ].Movement == MOVE_LEFT )
        {
            // The reverse art is to the left or under the art
            fish[ BOSS_FISH ].Source.left = fish[ BOSS_FISH ].Art.left + 
                                         (fish[ BOSS_FISH ].Art.right * fish[ BOSS_FISH ].NumFrames);

            fish[ BOSS_FISH ].Dest.left = DesktopWidth;
        }
        else
        {
            fish[ BOSS_FISH ].Source.left = fish[ BOSS_FISH ].Art.left;
            fish[ BOSS_FISH ].Source.top = fish[ BOSS_FISH ].Art.top;
            fish[ BOSS_FISH ].Dest.left = -fish[ BOSS_FISH ].Dest.right;
        }

        // Set the height of the fish
        fish[ BOSS_FISH ].Dest.top  = (rand() % ( DesktopHeight - fish[ BOSS_FISH ].Art.bottom - (MAX_UP_DOWN_OFFSET*2)))+MAX_UP_DOWN_OFFSET;

        // Set the active state
        fish[ BOSS_FISH ].Active = TRUE;
        // Set the pixel speed
        // Reset the animation timer
        fish[ BOSS_FISH ].AnimTimer = timeGetTime() + fish[ BOSS_FISH ].AnimDelay;
        
        // Set everyones swim speed to the max
        for( i = 0; i < pFishDataAry->count-1; ++i )
        {
            // Set the max speed for all the fish
            fish[i].PixelSpeed = MAX_FISH_PIXEL_SPEED;

            // Get the animation delay
            fish[i].AnimDelay = MIN_ANIMATION_DELAY;

            // Copy over the float inc counter
            fish[i].FloatIncCount = MIN_FLOAT_COUNTER;

            // Change the direction of the fish to run away from the boss if he's not too close
            if( fish[i].Movement != fish[ BOSS_FISH ].Movement && 
                abs( fish[i].Dest.left - fish[ BOSS_FISH ].Dest.left ) > DesktopWidth / 3 )
                fish[i].Movement = fish[ BOSS_FISH ].Movement;

            // If the fish is right by the boss when he starts, have them turn around and go the other way
            if( fish[i].Movement == fish[ BOSS_FISH ].Movement && 
                abs( fish[i].Dest.left - fish[ BOSS_FISH ].Dest.left ) < DesktopWidth / 3 )
                fish[i].Movement = (fish[ BOSS_FISH ].Movement + 1) % MAX_NUM_OF_MOVEMENTS;
            
            // Update the source rect
            if( fish[i].Movement == MOVE_LEFT )
            {
                // The reverse art is to the left or under the art
                fish[i].Source.left = fish[i].Art.left + 
                                        (fish[i].Art.right * fish[i].NumFrames);
            }
            else
            {
                fish[i].Source.left = fish[i].Art.left;
                fish[i].Source.top = fish[i].Art.top;
            } 
        }          
    }

}   // InitBossFish


/************************************************************************
*    FUNCTION NAME:         InitFishData                                                             
*
*    DESCRIPTION:           Init the fish data
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

BOOL CAnimateSaver::InitFishData()
{
    int i, j, initFishCount;
    TLoadRes LoadRes;
    BOOL Result = FALSE;

    // Load the fish art offsets from the resource
    if( !LoadTheResource( Instance(), "FishData", RT_RCDATA, &LoadRes ) )
        goto HANDLE_ERROR;

    // Get a pointer to the fish data
    pFishDataAry = (PTFishDataArry)LoadRes.pData;

    // Load the float data from the resource
    if( !LoadTheResource( Instance(), "FloatData", RT_RCDATA, &LoadRes ) )
        goto HANDLE_ERROR;

    // Save this pointer for the background fish to use
    fishFloatData = (short *)LoadRes.pData;

    // Allocate the fish data
    fish = new TFish[ pFishDataAry->count ];

    if( !fish )
        goto HANDLE_ERROR;

    // init the structure to all zeros
    memset( fish, 0, sizeof(TFish)*pFishDataAry->count );

    // Set all the fish
    for( i = 0; i < pFishDataAry->count; ++i )
    {
        // Copy over the art offsets. The resource data is a short
        fish[i].Art.left   = pFishDataAry->FishData[i].Art.Left;
        fish[i].Art.top    = pFishDataAry->FishData[i].Art.Top;
        fish[i].Art.right  = pFishDataAry->FishData[i].Art.Right;
        fish[i].Art.bottom = pFishDataAry->FishData[i].Art.Bottom;

        // Copy the art settings to the sourec and dest
        fish[i].Source = fish[i].Art;
        fish[i].Dest = fish[i].Art;

        // Get the frame count
        fish[i].NumFrames = pFishDataAry->FishData[i].FrameCount;

        // Get the fish is on the bottom only
        fish[i].BottomOnly = pFishDataAry->FishData[i].BottomOnly;

        // Get the pixel speed
        fish[i].PixelSpeed = pFishDataAry->FishData[i].PixelSpeed;

        // Get the animation delay
        fish[i].AnimDelay = pFishDataAry->FishData[i].AnimDelay;

        // Copy over the float inc counter
        fish[i].FloatIncCount = pFishDataAry->FishData[i].FloatIncCount;

        // Randomise the starting movements
        fish[i].Movement = rand() %  MAX_NUM_OF_MOVEMENTS;

        // Get a pointer to the float offsets
        fish[i].YFloat = (short *)LoadRes.pData;

        // Randomize the fload counter
        fish[i].FloatCounter = rand() % MAX_FLOAT_MOVEMENTS;
    }

    // Randomly place how ever many fish are to run on the screen. The trick here
    // is to pick a number in the index of all the fish and then load from
    // that number how ever many fish that are to run. Pick a random number is a 
    // range to choose from but keep us from blowing past our array
    if( FishCount < pFishDataAry->count )
    {
        i = rand() % ((pFishDataAry->count-1) - FishCount);
        initFishCount = FishCount;
    }
    else
    {
        i = 0;
        initFishCount = pFishDataAry->count - 1;
    }

    // Put the fish on the screen
    for( j = 0; j < initFishCount; ++j )
    {
        fish[i+j].Active = TRUE;
        fish[i+j].Dest.left = rand() % ( DesktopWidth - fish[i+j].Art.right );

        if( fish[i+j].BottomOnly )
            fish[i+j].Dest.top  = DesktopHeight - fish[i+j].Art.bottom - ((rand() % MAX_BOTTOM_EDGE)+MIN_BOTTOM_EDGE);
        else
            fish[i+j].Dest.top  = (rand() % ( DesktopHeight - fish[i+j].Art.bottom - SAND_BAR_H - (MAX_UP_DOWN_OFFSET*2)))+MAX_UP_DOWN_OFFSET;
        fish[i+j].AnimTimer = timeGetTime() + fish[i+j].AnimDelay;
        
        // Update the source rect so they can move left
        if( fish[i+j].Movement == MOVE_LEFT )
        {
            // The reverse art is to the left
            fish[i+j].Source.left = fish[i+j].Art.left + 
                                         (fish[i+j].Art.right * fish[i+j].NumFrames);
        }

        ++NumFishAnim;
    }

    // Set the timer for picking the ships
    FishPickTimer = timeGetTime() + ( rand() % FISH_PICK_TIME_MAX ) + FISH_PICK_TIME_MIN;

    // Set the timer for the boss to show up
    BossFishTimer = timeGetTime() + ( rand() % BOSS_FISH_PICK_TIME_MAX ) + BOSS_FISH_PICK_TIME_MIN;

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // InitFishData


/************************************************************************
*    FUNCTION NAME:         InitBackFishData                                                             
*
*    DESCRIPTION:           Init the background fish data
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

BOOL CAnimateSaver::InitBackFishData()
{
    int i, count;
    PTFishDataArry pBackFishDataAry;
    TLoadRes LoadRes;
    BOOL Result = FALSE;

    // Load the fish art offsets from the resource
    if( !LoadTheResource( Instance(), "BackFishData", RT_RCDATA, &LoadRes ) )
        goto HANDLE_ERROR;

    // Get a pointer to the fish data
    pBackFishDataAry = (PTFishDataArry)LoadRes.pData;

    // Get the number of background fish
    BackFishCount = (DesktopWidth / 128) * pBackFishDataAry->count;

    // Allocate the back fish array
    backFish = new TFish[ BackFishCount ];

    if( !backFish )
        goto HANDLE_ERROR;

    // init the structure to all zeros
    memset( backFish, 0, sizeof(TFish)*BackFishCount );

    for( i = 0; i < BackFishCount; ++i )
    {
        // Update the count
        count = i % pBackFishDataAry->count;

        // Copy over the art offsets. The resource data is a short
        backFish[i].Art.left   = pBackFishDataAry->FishData[ count ].Art.Left;
        backFish[i].Art.top    = pBackFishDataAry->FishData[ count ].Art.Top;
        backFish[i].Art.right  = pBackFishDataAry->FishData[ count ].Art.Right;
        backFish[i].Art.bottom = pBackFishDataAry->FishData[ count ].Art.Bottom;

        // Init the frame counter
        backFish[i].FrameCounter = 0;

        // Copy the art settings to the sourec and dest
        backFish[i].Source = backFish[i].Art;
        backFish[i].Dest = backFish[i].Art;

        // Get the frame count
        backFish[i].NumFrames = pBackFishDataAry->FishData[ count ].FrameCount;

        // Get the fish is on the bottom only
        backFish[i].BottomOnly = pBackFishDataAry->FishData[ count ].BottomOnly;

        // Get the pixel speed
        backFish[i].PixelSpeed = pBackFishDataAry->FishData[ count ].PixelSpeed;

        // Get the animation delay
        backFish[i].AnimDelay = pBackFishDataAry->FishData[ count ].AnimDelay;

        // Copy over the float inc counter
        backFish[i].FloatIncCount = pBackFishDataAry->FishData[ count ].FloatIncCount;

        // Randomise the starting movements
        backFish[i].Movement = rand() % MAX_NUM_OF_MOVEMENTS;

        // Get a pointer to the float offsets
        backFish[i].YFloat = fishFloatData;

        // Randomize the fload counter
        backFish[i].FloatCounter = rand() % MAX_FLOAT_MOVEMENTS;

        // Randomize the X position
        backFish[i].Dest.left = rand() % ( DesktopWidth - backFish[i].Art.right );

        // Randomize the Y position
        backFish[i].Dest.top  = (rand() % ( DesktopHeight - backFish[i].Art.bottom - BOTTOM_PROP_OFFSET - (MAX_UP_DOWN_OFFSET*2)))+MAX_UP_DOWN_OFFSET;
        backFish[i].AnimTimer = timeGetTime() + backFish[i].AnimDelay;

        // If the fish is on the bottom half of the screen use the darker fish
        if( backFish[i].Dest.top > DesktopHeight / 2 )
            backFish[i].Source.top += DARK_FISH_OFFSET;

        // Update the source rect so they can move left
        if( backFish[i].Movement == MOVE_LEFT )
            // The reverse art is to the left
            backFish[i].Source.left = backFish[i].Art.left + (backFish[i].Art.right * backFish[i].NumFrames);
    }

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // InitBackFishData


/************************************************************************
*    FUNCTION NAME:         InitBubbleData                                                             
*
*    DESCRIPTION:           Init the bubble data
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

BOOL CAnimateSaver::InitBubbleData()
{
    int i, count;
    PTRectArry pBubbleDataAry;
    TLoadRes LoadRes;
    BOOL Result = FALSE;

    // Load the bubble art offsets from the resource
    if( !LoadTheResource( Instance(), "BubbleData", RT_RCDATA, &LoadRes ) )
        goto HANDLE_ERROR;

    // Get a pointer to the fish data
    pBubbleDataAry = (PTRectArry)LoadRes.pData;

    // How many bubbles do we need?
    BubbleCount = (DesktopWidth / 128) * pBubbleDataAry->count;

    // Allocate the bubble array
    pBubble = new TBubble[ BubbleCount ];

    if( !pBubble )
        goto HANDLE_ERROR;

    // init the structure to all zeros
    memset( pBubble, 0, sizeof(TBubble)*BubbleCount );

    // Load the float data from the resource
    if( !LoadTheResource( Instance(), "BubFloatData", RT_RCDATA, &LoadRes ) )
        goto HANDLE_ERROR;

    for( i = 0; i < BubbleCount; ++i )
    {
        // Inc the counter
        count = i % pBubbleDataAry->count;

        // Copy over the art offsets. The resource data is a short
        pBubble[i].Art.left   = pBubbleDataAry->Art[count].Left;
        pBubble[i].Art.top    = pBubbleDataAry->Art[count].Top;
        pBubble[i].Art.right  = pBubbleDataAry->Art[count].Right;
        pBubble[i].Art.bottom = pBubbleDataAry->Art[count].Bottom;

        // Copy the art settings to the sourec and dest
        pBubble[i].Dest = pBubble[i].Art;

        // Get the pixel speed
        pBubble[i].PixelSpeed = (rand() % 3) + 1;

        // Randomize the fload counter
        pBubble[i].FloatCounter = rand() % MAX_BUB_FLOAT_MOVEMENTS;

        // Get a pointer to the float offsets
        pBubble[i].YFloat = (short *)LoadRes.pData;

        // Set the Y offset
        pBubble[i].Dest.top = rand() % ( DesktopHeight - SAND_BAR_H ); // pBubble[i].Art.bottom

        // Set the X offset
        pBubble[i].Dest.left  = (rand() % ( DesktopWidth - pBubble[i].Art.right - (MAX_LEFT_RIGHT_OFFSET*2)))+MAX_LEFT_RIGHT_OFFSET;

        // This just needs to be zero on start
        pBubble[i].FloatIncCount = 0;

        // The speed of the side to side float is set by how fast 
        // the bubble is floating upward
        if( pBubble[i].PixelSpeed == 3 )
            pBubble[i].FloatSpeed = 1;
        else if( pBubble[i].PixelSpeed == 1 )
            pBubble[i].FloatSpeed = 3;
        else
            pBubble[i].FloatSpeed = 2;
    }

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // InitBubbleData


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