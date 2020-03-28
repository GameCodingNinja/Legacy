
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

int FloatUpArry[ FLOAT_COUNT ] = {  0, 0, 0, 0, 0, 0, 1, 0, 0, 0,     0, 0, 0, 1, 0, 0, 0, 0, 0, 1,     0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
                                    0, 0, 1, 0, 0, 0, 1, 0, 0, 0,     1, 0, 0, 0, 1, 0, 0, 0, 1, 0,     0, 0, 0, 0, 0, -1, 0, 0, 0, 0,
                                    0, 0, -1, 0, 0, 0, 0, 0, -1, 0,   0, 0, 0, -1, 0, 0, 0, -1, 0, 0,   0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 
                                    0, 0, 0, -1, 0, 0, 0, -1 };


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
    bubbleCount = 0;
    bubble = NULL;
    MinPopDelay = 0;


    // Init non-specific class members
    AnimCounter = 0;
    SaverTimer = 0;
    SaverSpeed = 0;
    ShareWareMode = false;
    ShareWareTimer = 0;
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

    // Set the speed
    SaverSpeed = (Settings.GetSpeed() + 1) * BUBLE_SPEED_MULTI;

    // Do we need to randomly pick a fload mode
    if( Settings.GetFloatMode() == RANDOM_MODE )
        Settings.SetFloatMode( rand() % RANDOM_MODE );

    // Set the pop delay
    MinPopDelay = (Settings.GetPopAmount() + 1) * MIN_POP_DELAY;

    #ifdef DEBUG
    ShareWareMode = FALSE;
    #endif

    // If we are not in the screen saver mode, disable the sound
    if( Mode() != ssSaver )
        Settings.SetSoundOn( FALSE );
    
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
    TGif gif;
    TBitmapinfo fh;
    TBitmap ArtTmp;

    // Init the classes using the screens DC. That way it doesn't have to make one
    buffer.InitActiveDC( GetScreenDC() );
    bubbleArt.InitActiveDC( GetScreenDC() );

    // Load the bubbles. Load the 8 bit GIF graphic from resource.
    if( SystemBitDepth > BIT_COUNT_8 && Mode() == ssSaver )
    {
        // Load the RGB GIF file
        // This RGB image was saved as an indexed 8 bit image so that it could be GIF compressed.
        // It was never really an 8 bit image, we just made it look that way
        if( !ArtTmp.LoadFromBuffer( gif.LoadFromResource( "bubblesRGB", "GIF", Instance() ) ) ) 
            goto HANDLE_ERROR;

        // Make the bubble buffer
        if( !bubbleArt.CreateStandardBuffer( ArtTmp.width(), ArtTmp.height() / 3 ) )
		{
			strcpy( errorStr, "Can't allocate bubble art buffer. Your system resources may be low so try restarting your computer." );
            goto HANDLE_ERROR;
		}

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
        if( !bubbleArt.LoadFromBuffer( gif.LoadFromResource( "bubbles256", "GIF", Instance() ) ) ) 
            goto HANDLE_ERROR;

        if( !bubbleArt.CreateBitmap( DIB_SECTION_BMP, BIT_COUNT_16 ) )
		{
			strcpy( errorStr, "Can't allocate bubble art buffer. Your system resources may be low so try restarting your computer." );
            goto HANDLE_ERROR;
		}
    }

    // Make the buffer. We always need the buffer
    if( SystemBitDepth > BIT_COUNT_8 && Mode() == ssSaver )
        Load = buffer.CreateStandardBuffer( DesktopWidth, DesktopHeight );
    else
        Load = buffer.CreateDIBSectionBuffer( DesktopWidth, DesktopHeight, BIT_COUNT_16 );

    if( !Load )
    {
    	strcpy( errorStr, "Can't allocate background buffer. Your system resources may be low so try restarting your computer." ); 
        goto HANDLE_ERROR;
	}

    // Fill with black
    buffer.ColorRect( &DesktopRect, BLACK_BRUSH );

    // Load the palette if we are in 8 bit or less mode
    // Init the screens palette
    if( SystemBitDepth <= 8 )
    {
        // Saver the old palette to select back into the dc. If this doesn't happen, the newly created
        // palette will never get deleted
        hOldPalette = SelectPalette( GetScreenDC(), bubbleArt.GetPalette(), FALSE );
        RealizePalette( GetScreenDC() );

        // Delete the allocated bit data because we don't need it any more
        bubbleArt.DeleteBitmapBits();
    }

    Result = TRUE;

    HANDLE_ERROR:;

    return Result;

}   // LoadGraphics


/************************************************************************
*    FUNCTION NAME:         LoadSound                                                             
*
*    DESCRIPTION:           Loads the screen saver sound effects. Since
*							the pop	is just one sound and there is no overlapping
*							sounds, we will only use one device for the most
*							compatability for other systems.
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

    // Load the sound resources if we are to play sound
    if( Settings.GetSoundOn() )
    {
        // Setup the sound manger
        if( !soundMgr.AllocateWavLoad( MAX_POP_SOUNDS ) )
            goto HANDLE_ERROR;

        if( !soundMgr.AllocateWavDevices( 1 ) )
            goto HANDLE_ERROR;

        // Load the pop sounds from resource
        for( i = 0; i < MAX_POP_SOUNDS; ++i )
        {
            wsprintf( tmpStr, "pop%d", i );

            if( !soundMgr.LoadFromResource( Instance(), tmpStr, i ) )
                goto HANDLE_ERROR;
        }

        // Force a sound load. If there is any errors to be seen, it will help 
        // to cause the error to show up sooner.
        if( !soundMgr.ApplyLoadToDevice( 0, 0 ) )
            goto HANDLE_ERROR;
    }

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
    BOOL Result = FALSE;
    TLoadRes LoadRes;
    int i;

    // Get the number of bubbles
    bubbleCount = (Settings.GetBubbleCount() + 1) * BUBLE_COUNT_MULTI;

    // Allocate the number of bubble we are to manage
    bubble = new TBubble[ bubbleCount ];

    // Load the bubble art offsets from the resource
    if( !LoadTheResource( Instance(), "bubbleRects", RT_RCDATA, &LoadRes ) )
        goto HANDLE_ERROR;

    // Get a pointer to the bubble rect data
    pBubbleArt = (PRECTS)LoadRes.pData;

    // Load the pop art offsets from the resource
    if( !LoadTheResource( Instance(), "bubblePopRects", RT_RCDATA, &LoadRes ) )
        goto HANDLE_ERROR;

    // Get a pointer to the bubble rect data
    pPopArt = (PRECTS)LoadRes.pData;

    // init the bubble array
    memset( bubble, 0, sizeof( TBubble ) * bubbleCount );

    for( i = 0; i < bubbleCount; ++i )
    {
        // Set the bubble
        SetBubble( i );
        
        // Reset the starting Y Offset so that the bubble starts some where on the screen
        bubble[i].Dest.top = rand() % (DesktopHeight-bubble[i].Dest.bottom);
    }

    // Set the pop timer
    popTimer = timeGetTime() + ((rand() % MAX_POP_DELAY) + MinPopDelay);

    // Pick a bubble to pop
    bubble[ rand() % bubbleCount ].ToPop = TRUE;


    Result = TRUE;

    HANDLE_ERROR:;

    return Result;

}   // InitSaverData


/************************************************************************
*    FUNCTION NAME:         SetBubble                                                             
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

void CAnimateSaver::SetBubble( int Index )
{
    int j, popOffset, SpeedX, SpeedY;

    // Do not reset the bubble if it is to pop.
    // Let the bubble finish it's cycle
    if( bubble[Index].ArtIndex == BUB )
    {
        // Randomize a bubble to use
        j = rand() % MAX_BUBBLE_ART;

        // Get the pop art index
        popOffset = j % MAX_POP_ART;

        // Set the artwork rect
        SetRect( &bubble[ Index ].Art[BUB], pBubbleArt[j].X, pBubbleArt[j].Y,
                                            pBubbleArt[j].W, pBubbleArt[j].H );

        // Set the pop rect
        SetRect( &bubble[ Index ].Art[POP], pPopArt[popOffset].X, pPopArt[popOffset].Y,
                                            pPopArt[popOffset].W, pPopArt[popOffset].H );

        // Set mode specific settings
        if( Settings.GetFloatMode() == BOUNCE_MODE )
        {
            // Set the starting destination offset
            SetRect( &bubble[ Index ].Dest, rand() % (DesktopWidth-pBubbleArt[j].W),
                                            -pBubbleArt[j].H, pBubbleArt[j].W, pBubbleArt[j].H );

            // Set the speed
            bubble[ Index ].PixelSpeedX = (rand() % MAX_PIXEL_SPEED) + 1;
            bubble[ Index ].PixelSpeedY = (rand() % MAX_PIXEL_SPEED) + 1;

            // Reset the Y direction so that the bubble goes in a different direction on the screen
            if( rand() % 2 )
                bubble[ Index ].PixelSpeedY = -bubble[ Index ].PixelSpeedY;

            // Set the direction
            if( rand() % 2 )
                bubble[ Index ].PixelSpeedX = -bubble[ Index ].PixelSpeedX;
        }
        else if( Settings.GetFloatMode() == FLOAT_MODE )
        {
            // Set the starting destination offset
            SetRect( &bubble[ Index ].Dest, rand() % (DesktopWidth-pBubbleArt[j].W),
                                      -pBubbleArt[j].H, pBubbleArt[j].W, pBubbleArt[j].H );

            // Set the speed
            bubble[ Index ].PixelSpeedX = (rand() % MAX_PIXEL_SPEED_FLOAT) + (-MAX_PIXEL_SPEED);
            bubble[ Index ].PixelSpeedY = (rand() % MAX_PIXEL_SPEED) + 1;
        }
        else if( Settings.GetFloatMode() == CHAMPAGNE_MODE )
        {
            // Set the starting destination offset
            SetRect( &bubble[ Index ].Dest, rand() % (DesktopWidth-pBubbleArt[j].W),
                                      DesktopHeight, pBubbleArt[j].W, pBubbleArt[j].H );

            // Set the speed
            bubble[ Index ].PixelSpeedX = 1;
            bubble[ Index ].PixelSpeedY = -((rand() % MAX_PIXEL_SPEED) + 1);

            // Set the floatup counter
            bubble[ Index ].FloatUpCounter = rand() % FLOAT_COUNT;
        }

        // Get the absolute value of the speeds
        SpeedX = abs( bubble[ Index ].PixelSpeedX );
        SpeedY = abs( bubble[ Index ].PixelSpeedY );

        // Set the trim X amount
        if( SpeedX == 1 )
            bubble[ Index ].PixelTrimX = 2;
        else if( SpeedX == 2 )
            bubble[ Index ].PixelTrimX = 1;
        else
            bubble[ Index ].PixelTrimX = 0;

        // Set the trim Y amount
        if( SpeedY == 1 )
            bubble[ Index ].PixelTrimY = 2;
        else if( SpeedY == 2 )
            bubble[ Index ].PixelTrimY = 1;
        else
            bubble[ Index ].PixelTrimY = 0;

        // Do not reset the bubble if it is to pop.
        // Let the bubble finish it's cycle
        if( !bubble[ Index ].ToPop )
        {
            // Init the pop count
            bubble[ Index ].PopCount = 0;

            // Set the art index
            bubble[ Index ].ArtIndex = BUB;

            // Init the pop erase
            bubble[ Index ].PopErase = FALSE;

            // Set the pop flag to false
            bubble[ Index ].ToPop = FALSE;
        }
    }

}   // SetBubble


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
        if( !ShareWareMode )
        {
            // Init the buffer
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
                SaverTimer = timeGetTime() + SaverSpeed;
            }
            else if( AnimCounter )
            {
                Animate();
            }
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
    if( soundMgr.IsSndError() )
    {
        // Set the background mode to transparent for the text
        canvas.SetBackgroundMode( TRANSPARENT );

        // Draw the text
        canvas.DrawText( 5, 5, soundMgr.GetErrorStr() );

        // Set the background mode back to opaque
        canvas.SetBackgroundMode( OPAQUE );
    }

}   // DisplaySoundError


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
    RECT dest, art;

    // Skip this part if we are in preview mode to help the preview mode animate faster
    if( Mode() == ssPreview && PreviewCounter == 0 )
        return;

    // Set the mode to black for erasing the bubbles
    bubbleArt.SetCopyMode( BLACKNESS );

    // Erase the bubbles
    for( i = 0; i < bubbleCount; ++i )
    {
        // Trim the rects to be more effecient
        dest.left = bubble[i].Dest.left + MAX_PIXEL_SPEED;
        dest.top = bubble[i].Dest.top + MAX_PIXEL_SPEED;
        dest.right = bubble[i].Dest.right - MAX_PIXEL_SPEED_DOUBLED;
        dest.bottom = bubble[i].Dest.bottom - MAX_PIXEL_SPEED_DOUBLED;

        art.left = bubble[i].Art[ bubble[i].ArtIndex ].left + MAX_PIXEL_SPEED;
        art.top = bubble[i].Art[ bubble[i].ArtIndex ].top + MAX_PIXEL_SPEED;
        art.right = dest.right;
        art.bottom = dest.bottom;
        
        bubbleArt.Draw( &buffer, &dest, &art );
    }

    // Set the mode to paint for transparent look
    bubbleArt.SetCopyMode( SRCPAINT );

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
    int i;
    DWORD tmpTime = timeGetTime();
    BOOL ResetBubble;

    for( i = 0; i < bubbleCount; ++i )
    {
        // Update the bubble to bounce
        if( Settings.GetFloatMode() == BOUNCE_MODE )
            ResetBubble = Bubble_Bounce( i );

        // Update the bubble to float
        else if( Settings.GetFloatMode() == FLOAT_MODE )
            ResetBubble = Bubble_Float( i );

        // Update the bubble to float Up
        else if( Settings.GetFloatMode() == CHAMPAGNE_MODE )
            ResetBubble = Bubble_Champagne( i );

        // Do we want to pop this bubble
        Bubble_Pop( i, tmpTime );

        // Does the bubble need to be reset?
        if( ResetBubble )
            SetBubble( i );           
    }

}   // UpdateAllAnimations


/************************************************************************
*    FUNCTION NAME:         Bubble_Bounce                                                             
*
*    DESCRIPTION:           Set the bubble to bounce
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

BOOL CAnimateSaver::Bubble_Bounce( int Index )
{
    if( bubble[Index].Dest.left <= 0 )
        bubble[Index].PixelSpeedX = abs(bubble[Index].PixelSpeedX);

    if( bubble[Index].Dest.top <= 0 )
        bubble[Index].PixelSpeedY = abs(bubble[Index].PixelSpeedY);
    
    if( bubble[Index].Dest.left + bubble[Index].Dest.right >= DesktopWidth )
        bubble[Index].PixelSpeedX = -bubble[Index].PixelSpeedX;
    
    if( bubble[Index].Dest.top + bubble[Index].Dest.bottom >= DesktopHeight )
        bubble[Index].PixelSpeedY = -bubble[Index].PixelSpeedY;

    bubble[Index].Dest.left += bubble[Index].PixelSpeedX;
    bubble[Index].Dest.top += bubble[Index].PixelSpeedY;

    return FALSE;

}   // Bubble_Bounce


/************************************************************************
*    FUNCTION NAME:         Bubble_Float                                                             
*
*    DESCRIPTION:           Set the bubble to float
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

BOOL CAnimateSaver::Bubble_Float( int Index )
{
    BOOL Result = FALSE;

    bubble[Index].Dest.left += bubble[Index].PixelSpeedX;
    bubble[Index].Dest.top += bubble[Index].PixelSpeedY;

    // See if the bubble went off the sides or the bottom of the screen
    if( bubble[Index].Dest.left <= -bubble[Index].Dest.right ||
        bubble[Index].Dest.left >= DesktopWidth ||
        bubble[Index].Dest.top >= DesktopHeight )
    {
        Result = TRUE;
    }

    return Result;

}   // Bubble_Float


/************************************************************************
*    FUNCTION NAME:         Bubble_Champagne                                                             
*
*    DESCRIPTION:           Set the bubble to float from the bottom up
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

BOOL CAnimateSaver::Bubble_Champagne( int Index )
{
    BOOL Result = FALSE;

    bubble[Index].Dest.left += FloatUpArry[ bubble[ Index ].FloatUpCounter ];
    bubble[Index].Dest.top += bubble[Index].PixelSpeedY;

    // Update the float counter
    bubble[ Index ].FloatUpCounter = (bubble[ Index ].FloatUpCounter + 1) % FLOAT_COUNT;

    // See if the bubble went off the top of the screen
    if( bubble[Index].Dest.top <= -bubble[Index].Dest.bottom )
    {
        Result = TRUE;
    }

    return Result;

}   // Bubble_Champagne


/************************************************************************
*    FUNCTION NAME:         Bubble_Pop                                                             
*
*    DESCRIPTION:           Set the bubble to pop
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

void CAnimateSaver::Bubble_Pop( int Index, DWORD tmpTimer )
{
    // Did the bubble pop
    if( Settings.GetBubblePopOn() && popTimer < tmpTimer && bubble[Index].ToPop )
    {           
        // Play the pop sound
        if( bubble[Index].ArtIndex == BUB && Settings.GetSoundOn() )
        {
            soundMgr.ApplyLoadToDevice( rand() % MAX_POP_SOUNDS, 0 );
            soundMgr.Play();
        }
        
        // Set the art index to the pop art
        bubble[Index].ArtIndex = POP;
        
        // Inc the pop count cycle
        if( !bubble[Index].PopErase )
            bubble[Index].PopCount = (bubble[Index].PopCount + 1) % POP_COUNT_CYCLES;

        // Once the counter cycles around and equals zero, reset the bubble
        if( bubble[Index].PopCount == 0 )
        {
            if( bubble[Index].PopErase  )
            {
                // Reset the pop flag
                bubble[Index].ToPop = FALSE;
                bubble[Index].ArtIndex = BUB;
                
                // Reset the bubble
                SetBubble( Index );

                // Reset the pop timer
                popTimer = timeGetTime() + ((rand() % MAX_POP_DELAY) + MinPopDelay);

                // Pick a bubble to pop
                bubble[ rand() % bubbleCount ].ToPop = TRUE;
            }
            else
                bubble[Index].PopErase = TRUE;
        }
    }

}   // Bubble_Pop


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
    int i;
    RECT dest, art;

    // Skip this part if we are in preview mode to help the preview mode animate faster
    if( Mode() == ssPreview && PreviewCounter == MAX_PREVIEW_TIMER_COUNTS-1 )
        return;

    // Draw the bubbles to the buffer
    for( i = 0; i < bubbleCount; ++i )
    {
        if( !bubble[i].PopErase )
        {
            // Trim the rects to be more effecient
            dest.left = bubble[i].Dest.left + MAX_PIXEL_SPEED;
            dest.top = bubble[i].Dest.top + MAX_PIXEL_SPEED;
            dest.right = bubble[i].Dest.right - MAX_PIXEL_SPEED_DOUBLED;
            dest.bottom = bubble[i].Dest.bottom - MAX_PIXEL_SPEED_DOUBLED;

            art.left = bubble[i].Art[ bubble[i].ArtIndex ].left + MAX_PIXEL_SPEED;
            art.top = bubble[i].Art[ bubble[i].ArtIndex ].top + MAX_PIXEL_SPEED;
            art.right = dest.right;
            art.bottom = dest.bottom;
            
            bubbleArt.Draw( &buffer, &dest, &art );
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
    int i;
    RECT dest;

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
            // Draw the bubbles to the buffer
            for( i = 0; i < bubbleCount; ++i )
            {
                // Trim the rects to be more effecient
                dest.left = bubble[i].Dest.left + bubble[i].PixelTrimX;
                dest.top = bubble[i].Dest.top + bubble[i].PixelTrimY;
                dest.right = bubble[i].Dest.right - (bubble[i].PixelTrimX << 1);
                dest.bottom = bubble[i].Dest.bottom - (bubble[i].PixelTrimY << 1);

                buffer.Draw( ScreenDC, &dest ); // &bubble[i].Dest
            }
        }

        #else
            buffer.StretchDraw( ScreenDC, &WindowRect );
        #endif

        // Let the user know there was an error
        DisplaySoundError();
    }
    else
    {
        // update the screen every X numer of intervals so that it doesn't hog the CPU cycles
        if( PreviewCounter == 0 )
            buffer.StretchDraw( ScreenDC, &WindowRect );

        // Inc the preview counter
        PreviewCounter = ( PreviewCounter + 1 ) % MAX_PREVIEW_TIMER_COUNTS;
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

            // Blit the whole buffer to the screen
            buffer.Draw( GetScreenDC(), &DesktopRect );

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

    if( bubble )
        delete [] bubble;
            
}   // Destructor
