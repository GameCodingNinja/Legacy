
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

#include "gtools.h"          // A misc group of useful functions
#include "ssAnim.h"          // Header file for this *.cpp file.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include "Loader.h"          // Code for loading the graphics from the back of the file
  


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
    MusicTimer = 0;
    hOldPalette = NULL;
    Error_Number = 0;
    BlitCounter = 0;
    ShareWareTimer = 0;
    MainTimer = 0;
    SoundFileCount = 0;
    hOldPalette = NULL;
    TransPt = NULL;
    TransCounter = 0;
    TransTimer = 0;
    TimerTypeCounter = 0;
    PreviewTimer = 0;
    BackFileCount = 0;

}   // Constructor


/************************************************************************
*    FUNCTION NAME:         InitSaverData                                                             
*
*    DESCRIPTION:           Initilize all the screen saver specific code
*                           here.
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

BOOL CAnimateSaver::InitSaverData()
{
    BOOL Result = FALSE;
    int i, width, height;

    // First this to do is memset the AppData
    memset( &AppData, 0, sizeof( TApplicationData ) );

    // zero out all the structures
    memset( &ProjPrefsData, 0, sizeof(TProjectPrefs) );
    memset( &FileHeader, 0, sizeof(TFileHeader) );
    memset( &AppData.ViewWndRect, 0, sizeof(RECT) );

    //Create the path to the temp sound file
    //GetTheWindowsTmpDir( TmpString );
    sprintf( AppData.SoundFileTmpPath, DEFAULT_MUSIC_FILE_NAME, "C:\\", rand() % 0xFFFFFFF );

    Error_Number = 21; // Set the error number to help tell when the error happened

    // Get the command line and parse it to be sure
    // there is no junk with it
    ParsePathFromCmdLine( GetCommandLine(), AppData.Path );

    // We are in debug mode and are loading data from the default file name.
    // This causes the program to open the other file on the hard drive
    // instead of itself which allows us to step through it in the debugger
    #ifdef VIEW_DEBUG

    char FilePath[MAX_PATH];

    // We are asuming this file has bee generated by the tool
    CropOutPath( AppData.Path, FilePath );
    strcat( FilePath, "\\" );
    strcat( FilePath, PRO_EXAMPLE_EXE_NAME );
    strcpy( AppData.Path, FilePath );

    #endif

    // Reads in the file header, the preferences and all the TPage data
    if( !Loader_OpenEXEandReadHeader( &FileHeader, &ProjPrefsData, &PageList, AppData.Path ) )
        goto INITILIZE_ERROR;

    Error_Number = 22; // Set the error number to help tell when the error happened

    // Setup the transition
    if( Settings.GetTransOnOff() )
    {
        // Allocate space for the transition and make sure this isn't some
        // wierd rez that isn't divisible by SMALLEST_TRANS_SIZE
        width = ScreenWidth / SMALLEST_TRANS_SIZE;
        if( (ScreenWidth % SMALLEST_TRANS_SIZE) != 0 )
            ++width;

        height = ScreenHeight / SMALLEST_TRANS_SIZE;
        if( (ScreenHeight % SMALLEST_TRANS_SIZE) != 0 )
            ++height;

        // allocate
        TransPt = new POINT[ width * height ];

        // Make sure it allocated
        if( !TransPt ) goto INITILIZE_ERROR;
    }

    // Go through all the pages and see if we have any music files
    for( i = 0; i < PageList.Count(); ++i )
    {
        // Did we find a sound file?
        if( PageList.GetSoundDataType( i ) )
            // Add the sound offset to the sound list
            SoundList.Add( PageList.GetSoundDataOffset( i ) );      
    }

    // Do they want the backs in random order?
    if( Settings.GetBackRandOrder() )
    {
        for( i = 0; i < PageList.Count(); ++i )
        {
            // Did we find a background?
            if( PageList.GetBackDataType( i ) > NO_FILE_TYPE && 
                PageList.GetBackDataType( i ) < FILE_TYPE_META )
                // Add the background offset to the back list
                BackList.Add( PageList.GetBackDataOffset( i ) );      
        }
    }
    
    // See if they choose to size all images to fit
    if( Settings.GetSizeImageToFit() )
    {
        for( i = 0; i < PageList.Count(); ++i )
            PageList.SetSizeToFit( i, TRUE );
    }
    
    // Record the bit depth of the screen
    ScreenBitDepth = GetDeviceCaps( GetScreenDC(), BITSPIXEL );
    
    Error_Number = 55; // Set the error number to help tell when the error happened
    
    #ifdef DEBUG
    //ProjPrefsData.ProjRegMode = FULL_REG_VERSION;
    #endif   

    Result = TRUE;

INITILIZE_ERROR:;

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
    // Allocate the media player for playing music if there is a need to
    if( SoundList.Count() && SaverActive() && Mode() == ssSaver && Settings.GetSoundOnOff() )
    {
        MPlayer.Init( Handle(), Instance(), DONT_USE_ERROR_DLG_BOXES );
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

        // If this is a duel monitor senearo then we need to black out the screen before we start.
        // This is only needed if the screen saver is not designed to work with more then one monitor.
        if( IsDuelMonitor() )
            FillRect( GetScreenDC(), &WindowRect, BlackBrush );       
    }
    #endif

}   // DisplayBufferOnStartUp


/************************************************************************
*    FUNCTION NAME:         SetupTransition                                                             
*
*    DESCRIPTION:           Setup the transition 
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
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimateSaver::SetupTransition()
{
    int i, j, Width, Height, Offset;
    POINT tmpPoint;

    PtWidth = 1 << ((rand() % 4)+2);
    PtHeight = 1 << ((rand() % 4)+2);

    // get the width and the height
    Width = ScreenWidth / PtWidth;
    Height = ScreenHeight / PtHeight;

    // Some of the newer resolution are kind of funcky
    // and aren't divisible by all values so do a check here
    // just to be safe
    if( (ScreenWidth % PtWidth) != 0 )
        ++Width;

    if( (ScreenHeight % PtHeight) != 0 )
        ++Height;

    // get the transition size
    TransArraySize = Width * Height;

    // Setup the timed out intervals
    if( TransArraySize > 1000 )
    {
        TransTimerInterval = TransArraySize / 1000;
        TimerType = TIMER_TYPE_GROUP;
    }
    else
    {
        TransTimerInterval = 1000 / TransArraySize;
        TimerType = TIMER_TYPE_DELAY;

        if( TransTimerInterval > 0 )
            TransTimerInterval -= 1;
    }

    // init the points
    for( j = 0; j < Height; ++j )
    {
        for( i = 0; i < Width; ++i )
        {
            Offset = (j*Width)+i;
            TransPt[ Offset ].x = i * PtWidth;
            TransPt[ Offset ].y = j * PtHeight;
        }
    }
    
    // Now pick a transition
    switch( rand() % MAX_TRANSITIONS )
    {
        case RANDOM_TRANS:
            for( i = 0; i < TransArraySize; ++i )
            {
                Offset = rand() % TransArraySize;
                
                if( Offset != i )
                {
                    tmpPoint = TransPt[ Offset ];
                    TransPt[ Offset ] = TransPt[ i ];
                    TransPt[ i ] = tmpPoint;
                }   
            }
        break;

        case VERT_LINES_TRANS:
            for( i = 0; i < TransArraySize/2; ++i )
            {               
                if( i % 2 )
                {
                    tmpPoint = TransPt[ i ];
                    TransPt[ i ] = TransPt[ (TransArraySize-1)-i ];
                    TransPt[ (TransArraySize-1)-i ] = tmpPoint;
                }   
            }
        break;

        case HORZ_LINES_TRANS:
            for( j = 0; j < Height/2; ++j )
            {
                if( j % 2 )
                {
                    for( i = 0; i < Width; ++i )
                    {
                        tmpPoint = TransPt[ (j*Width)+i ];
                        TransPt[ (j*Width)+i ] = TransPt[ (TransArraySize-1)-((j*Width)+i) ];
                        TransPt[ (TransArraySize-1)-((j*Width)+i) ] = tmpPoint;
                    }
                }
            }
        break;

        case DIAG_LINES_TRANS:
            for( i = 0; i < TransArraySize/2; ++i )
            {               
                if( i % 3 )
                {
                    tmpPoint = TransPt[ i ];
                    TransPt[ i ] = TransPt[ (TransArraySize-1)-i ];
                    TransPt[ (TransArraySize-1)-i ] = tmpPoint;
                }   
            }
        break;
    }        

}   // SetupTransition


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
    BOOL Result = FALSE, Load;

    // Copy over the screen DC
    buffer.InitActiveDC( GetScreenDC() );

    // Create the buffer for blitting to the little preview window
    // CreateDIBSectionBuffer works best for 8 bit mode, and for transfering big blocks
    if( SystemBitDepth <= 8 || Mode() == ssPreview || Settings.GetTransOnOff() == FALSE )  // 24 bit seems to be faster in 16 & 14 bit. 32 bit no difference
        Load = buffer.CreateDIBSectionBuffer( ScreenWidth, ScreenHeight, (SystemBitDepth > 8 ? BIT_COUNT_24 : SystemBitDepth) );
    else
        Load = buffer.CreateStandardBuffer( ScreenWidth, ScreenHeight );        

    if( !Load )
	{
		strcpy( errorStr, "Can't allocate background buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
	}

    // Color the buffer white
    buffer.ColorRect( &ScreenRect, WHITE_BRUSH );

    Result = TRUE;

    HANDLE_ERROR:;

    return Result;

}   // LoadGraphics


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
            // Animate the screen saver
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
        buffer.Draw( GetScreenDC() );

        // Resets the rect back to zero.
        ResetPasswordRect();
    }

}   // AnimateSaver


/************************************************************************
*    FUNCTION NAME:         PlayMusic                                                             
*
*    DESCRIPTION:           Plays the music that comes with this screen
*                           saver
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
    TLoadBuffer lb;

    // Don't try to play any music if there has been any errors
    if( SoundList.Count() && !MPlayer.GetPlayError() && MusicTimer < timeGetTime() )
    {
        // Get the status of the media player
        PlayerStatus = MPlayer.GetStatus();

        // Has the player stopped playing?
        if( PlayerStatus != PLAYER_PLAYING && PlayerStatus != PLAYER_PAUSED )
        {
            // If the player is not closed, close it
            if( PlayerStatus != PLAYER_CLOSED )
                MPlayer.Close();

            // If they choose to randomize the music, mix it up
            if( Settings.GetPlayMusicRandOrder() && SoundFileCount == 0 )
                SoundList.MixUp();

            // Load the data into the buffer
            if( Loader_LoadFileDataFromEXEfile( SoundList.GetInt( SoundFileCount ), &lb, AppData.Path ) )
            {
                // Attach the correct file extension to the file name
                switch( lb.Type )
                {
                    case FILE_TYPE_MP3:
                        RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".mp3" );
                    break;

                    case FILE_TYPE_WAV:
                        RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".wav" );
                    break;

                    case FILE_TYPE_MID:
                        RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".mid" );
                    break;

                    case FILE_TYPE_RMI:
                        RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".rmi" );
                    break;                  
                }

                // Write the music data to a temp file
                if( WriteDataToFile( AppData.SoundFileTmpPath, lb.pData, lb.Size, CREATE_ALWAYS ) )
                    // Play the music
                    MPlayer.Play( AppData.SoundFileTmpPath );

                // Delete the buffer because we are done with it
                delete [] lb.pData;
            }
            else
                // We had an error so init the error string
                strcpy( MPlayer.GetErrorStr(), "Error Loading music data" );

            // inc the sound counter
            SoundFileCount = ( SoundFileCount + 1 ) % SoundList.Count();
        }
        else if( PlayerStatus == PLAYER_PAUSED )
            MPlayer.Play();

        // Reset the timer to check if the music is done playing
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
    if( SoundList.Count() && MPlayer.GetPlayError() )
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
        // Play the music
        case PLAY_MUSIC:
        	PlayMusic();
			AnimCounter = ( AnimCounter + 1 ) % MAX_BLIT_OPERATIONS;
        break;
        
        // Load the graphics for this page
        case LOAD_GRAPHICS:
        	LoadPageGraphics();
			AnimCounter = ( AnimCounter + 1 ) % MAX_BLIT_OPERATIONS;
        break;

        // Blit to buffer
        case BLIT_TO_BUFFER:
        	BlitToTheBuffer();
			AnimCounter = ( AnimCounter + 1 ) % MAX_BLIT_OPERATIONS;
        break;

        // Reset the transition
        case RESET_TRANSITION:
        	if( TransPt )
        		SetupTransition();

			AnimCounter = ( AnimCounter + 1 ) % MAX_BLIT_OPERATIONS;
        break;

        // Blit to screen
        case BLIT_TO_SCREEN:
        	BlitFromBufferToScreen();
        break;
    }
}   // Animate


/************************************************************************
*    FUNCTION NAME:         LoadPageGraphics                                                             
*
*    DESCRIPTION:           Load the graphics for the current page
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

BOOL CAnimateSaver::LoadPageGraphics()
{
    BOOL Result = FALSE;
    int CurPage = PageList.GetCurrentPage();
    DWORD BackDataOffset;

    // Do we randomize the screens?
    if( Settings.GetScreensRandomOrder() && PageList.GetCurrentPage() == 0 )
        PageList.MixUp();

    // If we are in 256 color mode, reselect the old palette back
    // into the DC so that the created palette can be deleted
    if( hOldPalette )
    {
        SelectPalette( GetScreenDC(), hOldPalette, FALSE );
        hOldPalette = NULL;
    }

    // Load the first page
    if( !Loader_LoadFile( Handle(), &MainDib, PageList.GetMainDataOffset( CurPage ), AppData.Path ) )
        goto LOAD_ERROR;

    // The background can be a color so just return TRUE if it is
    if( PageList.GetBackDataType( CurPage ) > NO_FILE_TYPE && 
        PageList.GetBackDataType( CurPage ) < FILE_TYPE_META )
    {
        // Do we want to mix up the backgrounds?
        if( Settings.GetBackRandOrder() && BackList.Count() > 1 )
        {
            // Mix up all the backgrounds
            if( BackFileCount == 0 )
                BackList.MixUp();

            // Get the background offset
            BackDataOffset = BackList.GetInt( BackFileCount );

            // inc the sound counter
            BackFileCount = ( BackFileCount + 1 ) % BackList.Count();
        }
        else
            BackDataOffset = PageList.GetBackDataOffset( CurPage );

        // Load the first page
        if( !Loader_LoadFile( Handle(), &BackDib, BackDataOffset, AppData.Path ) )
            goto LOAD_ERROR;
    }

    Result = TRUE;

LOAD_ERROR:;

    if( !Result )
        ScreenSaverErrorAbortSaver();

    return Result;

}   // LoadPageGraphics


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
    // Update the buffer
    BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, PageList.GetCurrentPage(), &PageList );

    // Let's take this time to set the palette if we are in 256 color mode or less
    if( ScreenBitDepth <= 8 )
    {
        // Save the old palette so that we can select it back in before
        // deleting the palette we created. DeleteObject doesn't seem to
        // work if your trying to delete a palette that is still in the DC
        // We only use the main dib's pallete
        hOldPalette = SelectPalette( GetScreenDC(), MainDib.GetPalette(), FALSE );                 
        RealizePalette( GetScreenDC() );
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
    BOOL TransFinished = TRUE;

    switch( BlitCounter )
    {
        case BLIT_BUFFER_TO_SCREEN:
            if( Mode() == ssSaver )
            {
             	#if !defined( DEBUG ) || defined( FULL_SCREEN_DEBUG )
             	    if( Settings.GetTransOnOff() )
             	        TransFinished = TransToNextScreen();
             	    else
             	        buffer.Draw( GetScreenDC() );
             	#else 
             	    buffer.StretchDraw( GetScreenDC(), &WindowRect );
             	#endif
            }
            else
                buffer.StretchDraw( GetScreenDC(), &WindowRect );

            // Inc the counters and set the timer if the  transition is finished
            if( TransFinished )
            {
                BlitCounter = ( BlitCounter + 1 ) % MAX_BUFFER_OPERATIONS;
                MainTimer = PageList.GetDelayInSec( PageList.GetCurrentPage() ) *
                            1000 + timeGetTime();
            }
        break;

        case TIME_OUT_SCREEN:

	        // Repaint the whole screen if we have too.
	        // This screen saver needs to repaint the whole screen in the even
	        // the monitor wakes up from a sleep
	        if( RepaintScreen() )
				buffer.Draw( GetScreenDC() );
        
            if( MainTimer < timeGetTime() )
            {
                BlitCounter = ( BlitCounter + 1 ) % MAX_BUFFER_OPERATIONS;
            }

            // Update the preview window regularly
            if( Mode() == ssPreview )
            {
                if( PreviewTimer < timeGetTime() )
                {
                    buffer.StretchDraw( GetScreenDC(), &WindowRect );
                    PreviewTimer = timeGetTime() + PREVIEW_WND_INTERVAL;
                }
            }
        break;

        case RESTART_OPERATION:
            PageList.NextPage();
            AnimCounter = ( AnimCounter + 1 ) % MAX_BLIT_OPERATIONS;
            BlitCounter = ( BlitCounter + 1 ) % MAX_BUFFER_OPERATIONS;
        break;
    }       

    // Show an error screen if there is a problem playing music
    DisplayMusicError();

}   // BlitFromBufferToScreen


/************************************************************************
*    FUNCTION NAME:         TransToNextScreen                                                             
*
*    DESCRIPTION:           Do the transition to the next screen
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - BOOL, TRUE indicates transition finished
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CAnimateSaver::TransToNextScreen()
{
    RECT r;

    if( TransTimer < timeGetTime() )
    {
        // Set the rect
        SetRect( &r, TransPt[ TransCounter ].x, TransPt[ TransCounter ].y, PtWidth, PtHeight );

        // Blit the piece to the screen
        buffer.Draw( GetScreenDC(), &r );

        // inc the counter
        TransCounter = ( TransCounter + 1 ) % TransArraySize;

        if( TimerType == TIMER_TYPE_DELAY )
            TransTimer = timeGetTime() + TransTimerInterval;
        else
        {
            TimerTypeCounter = ( TimerTypeCounter + 1 ) % TransTimerInterval;

            if( TimerTypeCounter == 0 )
                TransTimer = timeGetTime();
        }
    }

    // Zero means the trans is finished
    if( TransCounter )
        return FALSE;
    else
        return TRUE;

}   // TransToNextScreen


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
    if( ProjPrefsData.ProjRegMode == SHAREWARE_VERSION && ShareWareTimer < timeGetTime() && Mode() == ssSaver )
    {
        // Display the shareware screen
        // Don't do the below if the screen savers been killed
        if( DisplayShareWareScreen() )
        {
            ShareWareTimer = timeGetTime() + SHAREWARE_MESSAGE_DELAY;
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
    // Reselect the old palette back into the DC so that the created palette can be deleted
    if( hOldPalette )
    {
        SelectPalette( GetScreenDC(), hOldPalette, FALSE );
        hOldPalette = NULL;
    }

	// Close the player so that temporary files can be deleted
	MPlayer.Close();

    // delete any temp files
    // Copy the path to add the extension
    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".mp3" );
    DeleteFile( AppData.SoundFileTmpPath );

    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".wav" );
    DeleteFile( AppData.SoundFileTmpPath );

    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".mid" );
    DeleteFile( AppData.SoundFileTmpPath );

    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".rmi" );
    DeleteFile( AppData.SoundFileTmpPath );

    // Free the allocated point array
    if( TransPt )
        delete [] TransPt;
            
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
    if( SoundList.Count() && !MPlayer.GetPlayError() )
    {
        PlayerStatus = MPlayer.GetStatus();

        if( PlayerStatus == PLAYER_PLAYING )
            MPlayer.Pause();
    }
}   // PauseSound