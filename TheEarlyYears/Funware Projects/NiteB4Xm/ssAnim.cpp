
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
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf



// Globals
int santaEyesList[ MAX_SANTA_EYE_FRAMES ] = { 0, 1, 2, 3, 4, 3, 2, 1 };
int santaLegList[ MAX_SANTA_LEG_FRAMES ] = { 0, 1, 2, 1 };
int catSleepList[ MAX_CAT_SLEEP_FRAMES ] = { 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1 };
int catEyeOpenList[ MAX_CAT_EYEOPEN_FRAMES ] = { 0, 1, 1, 2, 2, 1, 1, 3, 3, 1, 1, 2, 2, 1, 1, 3, 3, 1, 1, 0 };
int catYawnList[ MAX_CAT_YAWN_FRAMES ] = { 0, 1, 2, 3, 4, 4, 3, 3, 4, 4, 3, 2, 1, 0 };
int catAnimSelList[ MAX_CAT_RANDOM_INDEXES ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0 };


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
    fireAnimCounter = 0;
    treeAnimCounter = 0;
    treeTimer = 0;
    santaEyesTimer = 0;
    santaFeetTimer = 0;
    santaEyesCounter = 0;
    santaFeetCounter = 0;
    catCounter = 0;
    catSleepTimer = 0;
    mouseWalkTimer = 0;
    mouseWalkCounter = 0;
    MoveX = 0;
    mouseDirection = 1;
    presentIndex = 0;
    catState = STATE_CAT_SLEEP;

    // Init non-specific class members
    AnimCounter = 0;
    SaverTimer = 0;
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
    char songName[20];
    int i;

    // Allocate the media player for playing music
    if( SaverActive() && Mode() == ssSaver && (Settings.GetThemeMusicOn() || Settings.GetLoadYourOwn()) )
    {
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

    // Force a smaller window for preview mode
    if( Mode() == ssPreview )
    {
        DesktopWidth = 320;
        DesktopHeight = 240;
        
        ScreenWidth = DesktopWidth;
        ScreenHeight = DesktopHeight;
        
        SetRect( &ScreenRect, 0, 0, ScreenWidth, ScreenHeight );
        SetRect( &DesktopRect, 0, 0, DesktopWidth, DesktopHeight );     
    }
    
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
    int i;
    TMetafile metafile;

    // Init the classes using the screens DC. That way it doesn't have to make one
    dirtyBuffer.InitActiveDC( GetScreenDC() );
    cleanBuffer.InitActiveDC( GetScreenDC() );
    metafile.InitActiveDC( GetScreenDC() );

    if( Mode() == ssSaver )
    {
        // Create the clean buffer
        if( !cleanBuffer.CreateStandardBuffer( ScreenWidth, ScreenHeight ) )
        {
            strcpy( errorStr, "Can't allocate clean buffer. Your system resources may be low so try restarting your computer." );
            goto HANDLE_ERROR;
        }

        // Create the dirty buffer
        if( !dirtyBuffer.CreateStandardBuffer( ScreenWidth, ScreenHeight ) )
        {
            strcpy( errorStr, "Can't allocate dirty buffer. Your system resources may be low so try restarting your computer." );
            goto HANDLE_ERROR;
        }
    }
    else  // Use DIBSection for preview mode because XP doesn't lag so bad when doing a stretchdraw
    {
        // Create the clean buffer
        if( !cleanBuffer.CreateDIBSectionBuffer( ScreenWidth, ScreenHeight, BIT_COUNT_16 ) )
        {
            strcpy( errorStr, "Can't allocate clean buffer. Your system resources may be low so try restarting your computer." );
            goto HANDLE_ERROR;
        }

        // Create the dirty buffer
        if( !dirtyBuffer.CreateDIBSectionBuffer( ScreenWidth, ScreenHeight, BIT_COUNT_16 ) )
        {
            strcpy( errorStr, "Can't allocate dirty buffer. Your system resources may be low so try restarting your computer." );
            goto HANDLE_ERROR;
        }
    }

    // Paint the buffer black incase we dont have complete coverage
    FillRect( cleanBuffer.Handle(), &ScreenRect, BlackBrush );

    // Setup an error string in case we have a meta file load error
    strcpy( errorStr, "Error loading metafile data. Your system resources may be low so try restarting your computer." );

    // Load the fireplace background metafile
    if( !metafile.LoadFromResource( Instance(), "back", "WMF" ) )
        goto HANDLE_ERROR;

    // Render the background to the clean buffer
    if( !metafile.Draw( cleanBuffer.Handle(), &ScreenRect ) )
    {
        strcpy( errorStr, "Error rendering background metafile. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Jump out if the screen saver has been killed soon
    // The above call takes some time now would be a good time to jump out
    // if the screen saver was killed early.
    if( !SaverActive() )
        return TRUE;

    // Load the fire animation metafiles
    if( !LoadMetaFileAnim( metaFire, "Fire%d", MAX_FIRE_ANIM ) )
        goto HANDLE_ERROR;

    // Are we animating the cat?
    if( Settings.GetAnimateCat() )
    {
        // Load the cat sleep metafiles
        if( !LoadMetaFileAnim( catSleep, "CatSleep%d", MAX_CAT_SLEEP_ANIM ) )
            goto HANDLE_ERROR;

        // Load the cat yawn metafiles
        if( !LoadMetaFileAnim( catYawn, "CatYawn%d", MAX_CAT_YAWN_ANIM ) )
            goto HANDLE_ERROR;

        // Load the cat eye open metafiles
        if( !LoadMetaFileAnim( catEyeOpen, "CatEyeOpen%d", MAX_CAT_EYEOPEN_ANIM ) )
            goto HANDLE_ERROR;
    }

    // Are we animating the santa?
    if( Settings.GetAnimateSanta() )
    {
        // Load the santa clock body
        if( !metafile.LoadFromResource( Instance(), "SantaBody", "WMF" ) )
            goto HANDLE_ERROR;

        // Render the santa clock body to the clean buffer
        if( !metafile.Draw( cleanBuffer.Handle(), &ScreenRect ) )
        {
            strcpy( errorStr, "Error rendering santa clock metafile. Your system resources may be low so try restarting your computer." );
            goto HANDLE_ERROR;
        }

        // Load the Santa eyes metafiles
        if( !LoadMetaFileAnim( santaEyes, "SantaEyes%d", MAX_SANTA_EYES_ANIM ) )
            goto HANDLE_ERROR;

        // Load the Santa feet metafiles
        if( !LoadMetaFileAnim( santaLegs, "SantaFeet%d", MAX_SANTA_LEGS_ANIM ) )
            goto HANDLE_ERROR;
    }

    // Are we animating the tree?
    if( Settings.GetAnimateTree() )
    {
        // Load the fire tree light metafiles
        if( !LoadMetaFileAnim( metaTreeLights, "Tree%d", MAX_TREE_LIGHT_ANIM ) )
            goto HANDLE_ERROR;
    }

    // Are we animating the mouse?
    if( Settings.GetAnimateMouse() )
    {
        // Load the mouse walk metafiles
        if( !LoadMetaFileAnim( mouseWalk, "MouseWalk%d", MAX_MOUSE_WALK_FRAMES ) )
            goto HANDLE_ERROR;

        // Load the present metafiles
        if( !LoadMetaFileAnim( presents, "Present%d", MAX_PRESENTS ) )
            goto HANDLE_ERROR;

        // Setup the move rect
        SetRect( &moveRect, 0, 0, (ScreenWidth * 2) + (ScreenWidth / 2), ScreenHeight + (ScreenHeight / 2) );

        // calculate the width and height of the grid
        gridWidth = ScreenWidth / GRID_SIZE;
        gridHeight = ScreenHeight / GRID_SIZE;

        // Add the indexes to the int list
        for( i = 0; i < MAX_PRESENTS; ++i )
            presList.Add( i );

        // Now mix up all the ints
        presList.MixUp();
    }

    Result = TRUE;

    HANDLE_ERROR:;

    return Result;
}   // LoadGraphics


/************************************************************************
*    FUNCTION NAME:         LoadMetaFileAnim                                                             
*
*    DESCRIPTION:           Load the metafile animation into the metafile array
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

BOOL CAnimateSaver::LoadMetaFileAnim( PTMetafile pMetaFile, char *ResName, int noOfAnim )
{
    BOOL Result = TRUE;
    int i;
    char metaResName[20];

    for( i = 0; i < noOfAnim; ++i )
    {
        // Generate the resource name
        sprintf( metaResName, ResName, i );

        // Init the metafile class with the active DC. We already have it so it doen't need to be made again
        pMetaFile[i].InitActiveDC( GetScreenDC() );

        // Load the metafile
        if( !pMetaFile[i].LoadFromResource( Instance(), metaResName, "WMF" ) )
        {
            Result = FALSE;
            break;
        }
    }

    return Result;

}   // LoadMetaFileAnim


/************************************************************************
*    FUNCTION NAME:         DisplayBufferOnStartUp                                                             
*
*    DESCRIPTION:           First time display of the buffer to start
*                           the screen saver. For this screen saver, we
*                           are just using it to start the music playing
*                           if we are not in shareware mode.
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
        dirtyBuffer.Draw( GetScreenDC(), GetPasswordWindowRect() );

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

        // Do we play the included music?
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

                MPlayer.Play( StringList.GetString( SongCount ) );
                
                // Inc to the next song
                SongCount = (SongCount + 1) % StringList.Count();
            }
            // Player could have been paused by the monitor going asleep
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
    // Skip this part if we are in preview mode to help the preview mode animate faster
    //if( Mode() == ssPreview && PreviewCounter != 1 )
    //    return;

    // Clean the big buffers with the small fire buffers
    cleanBuffer.Draw( &dirtyBuffer );

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
    DWORD CurTimer = timeGetTime();

    // Inc the fire animation frame counter if it is time
    //if( fireTimer < CurTimer )
    //{
        fireAnimCounter = ( fireAnimCounter + 1 ) % MAX_FIRE_ANIM;

        //fireTimer = CurTimer + 100;
    //}

    // Inc the tree animation frame counter if it is time
    if( Settings.GetAnimateTree() && treeTimer < CurTimer )
    {
        treeAnimCounter = rand() % MAX_TREE_LIGHT_ANIM;

        treeTimer = CurTimer + 500;
    }

    if( Settings.GetAnimateSanta() )
    {
        // Inc the santa eyes animation frame counter if it is time
        if( santaEyesTimer < CurTimer )
        {
            santaEyesCounter = ( santaEyesCounter + 1 ) % MAX_SANTA_EYE_FRAMES;

            santaEyesTimer = CurTimer + 200;
        }

        // Inc the santa leg animation frame counter if it is time
        if( santaFeetTimer < CurTimer )
        {
            santaFeetCounter = ( santaFeetCounter + 1 ) % MAX_SANTA_LEG_FRAMES;

            santaFeetTimer = CurTimer + 300;
        }
    }

    // Inc the cat sleep animation frame counter if it is time
    if( Settings.GetAnimateCat() && catSleepTimer < CurTimer )
    {
        if( catState == STATE_CAT_SLEEP )
            catCounter = ( catCounter + 1 ) % MAX_CAT_SLEEP_FRAMES;         

        else if( catState == STATE_CAT_YAWN )
            catCounter = ( catCounter + 1 ) % MAX_CAT_YAWN_FRAMES;

        else if( catState == STATE_CAT_EYE_OPEN )
            catCounter = ( catCounter + 1 ) % MAX_CAT_EYEOPEN_FRAMES;

        // Select the next cat animation
        if( catCounter == 0 )
            catState = catAnimSelList[ rand() % MAX_CAT_RANDOM_INDEXES ];

        catSleepTimer = CurTimer + 200;
    }

    // Inc the mouse walk animation frame counter if it is time
    if( Settings.GetAnimateMouse() && catState == STATE_CAT_SLEEP )//mouseWalkTimer < CurTimer )
    {
        // Animation Counter
        mouseWalkCounter = ( mouseWalkCounter + 1 ) % MAX_MOUSE_WALK_FRAMES;

        // movement counter
        MoveX = ( MoveX + 1 ) % (GRID_SIZE + (GRID_SIZE / 2));

        // Pick the next present to animate
        if( MoveX == 0 )
        {
            // Inc to the next present
            presentIndex = ( presentIndex + 1 ) % MAX_PRESENTS;

            // Change the direction of the mouse
            mouseDirection  = ( mouseDirection + 1 ) % 2;
        }

        // Have the mouse walk from the left or the right
        if( mouseDirection )
        {
            moveRect.left = -(MoveX * gridWidth);
            moveRect.right = (ScreenWidth * 2) + (ScreenWidth / 2) + moveRect.left;
            //moveRect.top = -(moveY[ MoveX ] * gridWidth);
        }
        else
        {
            moveRect.right = (MoveX * gridWidth) - (ScreenWidth + (ScreenWidth / 2));
            moveRect.left = (ScreenWidth * 2) + (ScreenWidth / 2) + moveRect.right;
            //moveRect.top = -(moveY[ MoveX ] * gridWidth);
        }

        moveRect.top = -gridWidth;
        moveRect.bottom = (ScreenHeight + (ScreenHeight / 2)) + moveRect.top;

        //mouseWalkTimer = CurTimer + 100;
    }

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
    // Skip this part if we are in preview mode to help the preview mode animate faster
    //if( Mode() == ssPreview && PreviewCounter != 0 )
    //    return;

    // Animate the fire
    metaFire[ fireAnimCounter ].Draw( dirtyBuffer.Handle(), &ScreenRect );

    // Animate the Tree
    if( Settings.GetAnimateTree() )
        metaTreeLights[ treeAnimCounter ].Draw( dirtyBuffer.Handle(), &ScreenRect );

    // Animate the santa
    if( Settings.GetAnimateSanta() )
    {
        santaEyes[ santaEyesList[ santaEyesCounter ] ].Draw( dirtyBuffer.Handle(), &ScreenRect );
        santaLegs[ santaLegList[ santaFeetCounter ] ].Draw( dirtyBuffer.Handle(), &ScreenRect );
    }

    // Animate the mouse
    if( Settings.GetAnimateMouse() )
    {
        presents[ presList.GetInt( presentIndex ) ].Draw( dirtyBuffer.Handle(), &moveRect );
        mouseWalk[ mouseWalkCounter ].Draw( dirtyBuffer.Handle(), &moveRect );
    }

    // Animate the cat
    if( Settings.GetAnimateCat() )
    {
        if( catState == STATE_CAT_SLEEP )
            catSleep[ catSleepList[ catCounter ] ].Draw( dirtyBuffer.Handle(), &ScreenRect );          

        else if( catState == STATE_CAT_YAWN )
            catYawn[ catYawnList[ catCounter ] ].Draw( dirtyBuffer.Handle(), &ScreenRect );

        else if( catState == STATE_CAT_EYE_OPEN )
            catEyeOpen[ catEyeOpenList[ catCounter ] ].Draw( dirtyBuffer.Handle(), &ScreenRect );
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
    if( Mode() == ssSaver )
    {
        #if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )

            // Blit the flame animation
            dirtyBuffer.Draw( GetScreenDC() );

        #else
            dirtyBuffer.StretchDraw( GetScreenDC(), &WindowRect );
        #endif
    }
    else
    {
        dirtyBuffer.StretchDraw( GetScreenDC(), &WindowRect );
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