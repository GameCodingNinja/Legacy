
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


// Blinker rects
const SStaticAnimRect BlinkRect[MAX_BLINK_NEIGHBORHOOD] = { { 745, 480, 146,  97,  31,  32},    // First house
                                                            { 894, 480,  66, 122, 177,  19},    // First Tree
                                                            { 963, 480, 160, 109, 253,  80},    // Second House
                                                            {1031,   0, 107, 190, 419,  19},    // Second Tree
                                                            { 745, 728, 161, 109, 542, 104},    // Third House
                                                            { 910, 728,  69, 110, 702,  24},    // Third Tree
                                                            { 983, 728,  34,  64, 788,  77} };  // Fourth House

const SStaticDest SmokeDest[MAX_SMOKE_NEIGHBORHOOD] = { { 77, 246},
                                                        {374, 214},
                                                        {666, 199},
                                                        {827, 216} };
                                                         
RECT SmokeArtRect[MAX_DIFFERENT_SNOW] = { {  0, 1083, 28, 60 },
                                          {747,  952, 24, 90},  //129
                                          {941,  952, 24, 83 },
                                          {941, 1043, 21, 38} }; 

// Snow art rects
const RECT SnowArtRect[MAX_DIFFERENT_SNOW] = { { 747, 679, 13, 12 },
                                               { 761, 679,  9,  8 },
                                               { 771, 679,  7,  6 },
                                               { 779, 679,  7,  6 },
                                               { 787, 679,  5,  4 },
                                               { 793, 679,  5,  4 },
                                               { 799, 679,  5,  4 } };


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
    pSnow = NULL;
    rowCount = 0;
    BlinkerCount = 0;
    pBlinker = NULL;
    snowCount = 0;
    PeoplePickTimer = 0;
    pSmoke = NULL;
    SmokeCount = 0;
    SmokeTimer = 0;
    SantaTimer = timeGetTime() + ((rand() % MAX_SANTA_START_DELAY)+MIN_SANTA_START_DELAY);

    // Init non-specific class members
    AnimCounter = 0;
    SaverTimer = 0;
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
    int screenSize;

    // Get the user's settings
    // Load all the ini file settings
    Settings.GetPrivateString();

    // get the total screen size
    screenSize = DesktopWidth * DesktopHeight;

    switch( Settings.GetSnowAmount() )
    {
        case 0:  snowCount = screenSize / 4000;  break;
        case 1:  snowCount = screenSize / 2000;  break;
        case 2:  snowCount = screenSize / 1000;  break;
        case 3:  snowCount = screenSize / 500;   break;
        case 4:  snowCount = screenSize / 250;   break;
        case 5:  snowCount = screenSize / 125;   break;
        default: snowCount = screenSize / 1000;  break;
    }

    // Kill the star if we are running the star and santa in 640 res
    // Not enough room for both
    if( Settings.GetStar() && Settings.GetSanta() && DesktopHeight < 600 )
        Settings.SetStar( FALSE );
    
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
    TBitmap snowArtTmp;
    TGif gif;           // GIF decoder which can load GIFs from file or resource
    TBitmapinfo fh;

    // Init the classes using the screens DC. That way it doesn't have to make one
    dirtyBuffer.InitActiveDC( GetScreenDC() );
    cleanBuffer.InitActiveDC( GetScreenDC() );
    snowArt.InitActiveDC( GetScreenDC() );

    // Load the art. Load the 8 bit GIF graphic from resource.
    if( SystemBitDepth > BIT_COUNT_8 && Mode() == ssSaver )
    {
        // Load the RGB GIF file
        // This RGB image was saved as an indexed 8 bit image so that it could be GIF compressed.
        // It was never really an 8 bit image, we just made it look that way
        if( !snowArtTmp.LoadFromBuffer( gif.LoadFromResource( "Art24bit", "GIF", Instance() ) ) ) 
            goto HANDLE_ERROR;

        // Make the RGB buffer for the stars and planets
        if( !snowArt.CreateStandardBuffer( snowArtTmp.width(), snowArtTmp.height() / 3 ) )
        { 
            strcpy( errorStr, "Can't allocate snow art buffer. Your system resources may be low so try restarting your computer." );
            goto HANDLE_ERROR;
        }

        // Check to see that it's not empty
        if( !snowArtTmp.IsEmpty() )
        {
            // Copy over the bitmap info data
            memcpy( &fh, snowArtTmp.dib(), sizeof(TBitmapinfo) );

            // Now we are going to change some information so that it appears to be a 24 bit image again
            // Change the information needed to do the conversion
            fh.bmiHeader.biHeight /= 3;
            fh.bmiHeader.biBitCount = 24;
            fh.bmiHeader.biSizeImage = 0;
            fh.bmiHeader.biClrImportant = 0;
            fh.bmiHeader.biClrUsed = 0;

            // Blit the image to the buffer.
            StretchDIBits( snowArt.Handle(), 0, 0, snowArtTmp.width(), fh.bmiHeader.biHeight,
                           0, 0, snowArtTmp.width(), fh.bmiHeader.biHeight, snowArtTmp.bits(), 
                           (BITMAPINFO*)&fh, DIB_RGB_COLORS, SRCCOPY );
        }
        else
            goto HANDLE_ERROR;

    }
    else  // 256 color verion of the artwork
    {
        if( !snowArt.LoadFromBuffer( gif.LoadFromResource( "Art8bit", "GIF", Instance() ) ) ) 
            goto HANDLE_ERROR;

        if( !snowArt.CreateBitmap( DIB_SECTION_BMP, BIT_COUNT_16 ) )
        {
            strcpy( errorStr, "Can't allocate snow art buffer. Your system resources may be low so try restarting your computer." );
            goto HANDLE_ERROR;
        }
    }

    // Jump out if the screen saver has been killed soon
    if( !SaverActive() )
        return TRUE;

    // Make the buffer. We always need the buffer
    if( SystemBitDepth > BIT_COUNT_8 && Mode() == ssSaver )
    {
        // Create the clean buffer
        if( !cleanBuffer.CreateStandardBuffer( DesktopWidth, DesktopHeight ) )
        {
            strcpy( errorStr, "Can't allocate clean buffer. Your system resources may be low so try restarting your computer." );
            goto HANDLE_ERROR;
        }

        // Create the dirty buffer
        if( !dirtyBuffer.CreateStandardBuffer( DesktopWidth, DesktopHeight ) )
        {
            strcpy( errorStr, "Can't allocate dirty buffer. Your system resources may be low so try restarting your computer." );
            goto HANDLE_ERROR;
        }
    }
    else
    {
        // Create the clean buffer
        if( !cleanBuffer.CreateDIBSectionBuffer( DesktopWidth, DesktopHeight, BIT_COUNT_16 ) )
        {
            strcpy( errorStr, "Can't allocate clean buffer. Your system resources may be low so try restarting your computer." );
            goto HANDLE_ERROR;
        }

        // Create the dirty buffer
        if( !dirtyBuffer.CreateDIBSectionBuffer( DesktopWidth, DesktopHeight, BIT_COUNT_16 ) )
        {
            strcpy( errorStr, "Can't allocate dirty buffer. Your system resources may be low so try restarting your computer." );
            goto HANDLE_ERROR;
        }
    }

    // For now, just fill with black
    cleanBuffer.ColorRect( &DesktopRect, BLACK_BRUSH );

    // Setup the background
    BuildBackground();

    // Load the palette if we are in 8 bit or less mode
    // Init the screens palette
    if( SystemBitDepth <= 8 )
    {
        // Saver the old palette to select back into the dc. If this doesn't happen, the newly created
        // palette will never get deleted
        hOldPalette = SelectPalette( GetScreenDC(), snowArt.GetPalette(), FALSE );
        RealizePalette( GetScreenDC() );

        // Delete the allocated bit data because we don't need it any more
        snowArt.DeleteBitmapBits();
    }

    // Copy the clean buffer to the dirty buffer
    cleanBuffer.Draw( &dirtyBuffer );

    Result = TRUE;

    HANDLE_ERROR:;

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

void CAnimateSaver::BuildBackground()
{
    int i;
    RECT source;
    POINT point;

    // Calculate the number of rows
    if( cleanBuffer.width() % NEIGHBORHOOD_W )
        rowCount = (cleanBuffer.width() / NEIGHBORHOOD_W) + 1;
    else
        rowCount = cleanBuffer.width() / NEIGHBORHOOD_W;

    // Set the source rect
    SetRect( &source, 0, 0, NEIGHBORHOOD_W, NEIGHBORHOOD_H );

    // Set the Y offset
    point.y = DesktopHeight - NEIGHBORHOOD_H;

    // Blit the neighborhood across the bottom
    for( i = 0; i < rowCount; ++i )
    {
        point.x = i * NEIGHBORHOOD_W;
        snowArt.Draw( &cleanBuffer, &point, &source );
    }

}   // BuildBackground


/************************************************************************
*    FUNCTION NAME:         SetupSmoke                                                             
*
*    DESCRIPTION:           Setup the smoke
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

void CAnimateSaver::SetupSmoke()
{
    int i, j, Switch;
    RECT tmpRect;

    // Count up the number of smoking chimneys
    for( i = 0; i < rowCount; ++i )
    {
        for( j = 0; j < MAX_SMOKE_NEIGHBORHOOD; ++j )
        {
            // If a blinker will be seen, the add one
            if( (NEIGHBORHOOD_W * i) + SmokeDest[j].destX < DesktopWidth )
                ++SmokeCount;
        }
    }

    // Allocate the smoke array if one is needed
    if( SmokeCount )
        pSmoke = new SSmoke[ SmokeCount ];

    // Test all allocations
    if( !pSmoke )
    {
        SmokeCount = 0;
        return; 
    }

    // Mix up the smoke animations
    for( i = 0; i < MAX_SMOKE_NEIGHBORHOOD; ++i )
    {
        Switch = rand() % MAX_SMOKE_NEIGHBORHOOD;

        if( i != Switch )
        {
            tmpRect = SmokeArtRect[i];
            SmokeArtRect[i] = SmokeArtRect[Switch];
            SmokeArtRect[Switch] = tmpRect;   
        }
    }

    for( i = 0; i < SmokeCount; ++i )
    {
        j = i % MAX_SMOKE_NEIGHBORHOOD;
        
        // Set the buffer offset
        SetRect( &pSmoke[i].Dest, (SmokeDest[j].destX + ((i / MAX_SMOKE_NEIGHBORHOOD) * NEIGHBORHOOD_W)) - (SmokeArtRect[j].right / 2), 
                 (DesktopHeight - SmokeDest[j].destY) - SmokeArtRect[j].bottom, 
                 SmokeArtRect[j].right, SmokeArtRect[j].bottom );

        // Setup the source which is the art
        pSmoke[i].Source = SmokeArtRect[j];
        
        // Randomize a position in the counter
        pSmoke[i].FrameCounter = rand() % MAX_SMOKE_FRAMES;           
    }

}   // SetupSmoke


/************************************************************************
*    FUNCTION NAME:         SetupBlinkers                                                             
*
*    DESCRIPTION:           Setup the blinking houses, trees and star
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

void CAnimateSaver::SetupBlinkers()
{
    int i, j, StarCount=0;
    DWORD tmpTimer = timeGetTime();

    // Do we want Christmas lights?
    if( Settings.GetXMasLights() )
    {
        // Count up the number of blinkers
        for( i = 0; i < rowCount; ++i )
        {
            for( j = 0; j < MAX_BLINK_NEIGHBORHOOD; ++j )
            {
                // If a blinker will be seen, the add one
                if( (NEIGHBORHOOD_W * i) + BlinkRect[j].destX < DesktopWidth )
                    ++BlinkerCount;
            }
        }
    }

    // Do we want a star?
    if( Settings.GetStar() )
        ++StarCount;

    // Allocate the blinker array if one is needed
    if( BlinkerCount || StarCount )
        pBlinker = new TBlinker[ BlinkerCount + StarCount ];

    // Test all allocations
    if( !pBlinker )
    {
        BlinkerCount = 0;
        return; 
    }

    for( i = 0; i < BlinkerCount; ++i )
    {
        j = i % MAX_BLINK_NEIGHBORHOOD;
        
        // Set the buffer offset
        SetRect( &pBlinker[i].DestRect, BlinkRect[j].destX + ((i / MAX_BLINK_NEIGHBORHOOD) * NEIGHBORHOOD_W), 
                 BlinkRect[j].destY + (DesktopHeight-NEIGHBORHOOD_H), BlinkRect[j].width, BlinkRect[j].height );

        // Set the art buffer offset for the on blink
        SetRect( &pBlinker[i].ArtBlinkOn, BlinkRect[j].artX, 
                 BlinkRect[j].artY, BlinkRect[j].width, BlinkRect[j].height );

        // Set the art buffer offset for the off blink
        SetRect( &pBlinker[i].ArtBlinkOff, BlinkRect[j].artX, 
                 BlinkRect[j].artY + BlinkRect[j].height, BlinkRect[j].width, BlinkRect[j].height );

        // Set the blink timerout On amount
        pBlinker[i].TimerIntervalOn = MIN_BLINK_TIMEOUT_ON + (rand() % RANDOM_BLINK_AMOUNT_ON);

        // Set the blink timerout Off amount
        pBlinker[i].TimerIntervalOff = MIN_BLINK_TIMEOUT_OFF + (rand() % RANDOM_BLINK_AMOUNT_OFF);
                 
        // Decide if this blinker is on or off
        if( rand() % 2 )
        {
            // Ste the state of the blink
            pBlinker[i].BlinkState = BLINKER_ON;

            // Set the changed state to on
            pBlinker[i].ChangeState = TRUE;

            // Set the blink timer
            pBlinker[i].BlinkTimer = pBlinker[i].TimerIntervalOn + tmpTimer;
        }
        else
        {
            // Ste the state of the blink
            pBlinker[i].BlinkState = BLINKER_OFF;

            // Set the changed state to on
            pBlinker[i].ChangeState = FALSE;

            // Set the blink timer
            pBlinker[i].BlinkTimer = pBlinker[i].TimerIntervalOff + tmpTimer;
        }   
    }

    // Do we have a star?
    if( StarCount )
    {
        // Set the buffer offset
        SetRect( &pBlinker[i].DestRect, STAR_DESTX, STAR_DESTY, STAR_ARTW, STAR_ARTH );

        // Set the art buffer offset for the on blink
        SetRect( &pBlinker[i].ArtBlinkOn, STAR_ARTX, STAR_ARTY, STAR_ARTW, STAR_ARTH );

        // Set the art buffer offset for the off blink
        SetRect( &pBlinker[i].ArtBlinkOff, STAR_ARTX, STAR_ARTY+STAR_ARTH, STAR_ARTW, STAR_ARTH );

        // Set the blink timerout On amount
        pBlinker[i].TimerIntervalOn = MIN_STAR_BLINK_TIMEOUT_ON + (rand() % RANDOM_STAR_BLINK_AMOUNT_ON);

        // Set the blink timerout Off amount
        pBlinker[i].TimerIntervalOff = MIN_STAR_BLINK_TIMEOUT_OFF + (rand() % RANDOM_STAR_BLINK_AMOUNT_OFF);
                 
        // Ste the state of the blink
        pBlinker[i].BlinkState = BLINKER_OFF;

        // Set the changed state to on
        pBlinker[i].ChangeState = TRUE;

        // Set the blink timer
        pBlinker[i].BlinkTimer = pBlinker[i].TimerIntervalOff + tmpTimer;
    }

    // Update the blinker count to include the star
    BlinkerCount += StarCount;

}   // SetupBlinkers


/************************************************************************
*    FUNCTION NAME:         SetupSnow                                                             
*
*    DESCRIPTION:           Setup the snow
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

BOOL CAnimateSaver::SetupSnow()
{
    int i, j;
    DWORD tmpTimer = timeGetTime();

    // Allocate the snow
    pSnow = new TSnow[ snowCount ];

    // Check all allocations
    if( !pSnow )
        return FALSE;

    // Init the snow
    for( i = 0; i < snowCount; ++i )
    {
        j = i % MAX_DIFFERENT_SNOW;
        
        // Set the random destination rect
        SetRect( &pSnow[i].DestRect, rand() % (DesktopWidth-SnowArtRect[j].right), 
                                     rand() % (DesktopHeight-SnowArtRect[j].bottom),
                                     SnowArtRect[j].right, SnowArtRect[j].bottom );

        // Set the location of the On Art
        SetRect( &pSnow[i].ArtBlinkOn, SnowArtRect[j].left, SnowArtRect[j].top, SnowArtRect[j].right, SnowArtRect[j].bottom );

        // Set the location of the Off Art
        SetRect( &pSnow[i].ArtBlinkOff, SnowArtRect[j].left, SnowArtRect[j].top+SnowArtRect[j].bottom, SnowArtRect[j].right, SnowArtRect[j].bottom );

        // Set the pixel X speed
        pSnow[i].PixelSpeedX = -1 + (rand() % MAX_PIXEL_SPEEDSX);

        // Set the pixel Y speed
        pSnow[i].PixelSpeedY = (rand() % MAX_PIXEL_SPEEDSY) + 1;

        // Set the timer interval
        pSnow[i].TimerInterval = MIN_SNOW_BLINK + (rand() % RANDOM_SNOW_BLINK);

        // Set the timer
        pSnow[i].BlinkTimer = pSnow[i].TimerInterval + tmpTimer;

        // set the blink state
        pSnow[i].BlinkState = rand() % MAX_BLINKER_STATES;
    }

    return TRUE;

}   // SetupSnow


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

    // Setup the snow
    if( !SetupSnow() )
        goto HANDLE_ERROR;

    // Setup the blinking houses, trees and star
    SetupBlinkers();

    // Init the people data
    SetupPeople();

    // Setup the smoke
    if( Settings.GetSmoke() )
        SetupSmoke();

    // Setup Santa
    SetupSanta();

    Result = TRUE;

    HANDLE_ERROR:;

    return Result;

}   // InitSaverData


/************************************************************************
*    FUNCTION NAME:         SetupSanta                                                             
*
*    DESCRIPTION:           Setup the santa
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

void CAnimateSaver::SetupSanta()
{
    santa.Art.left   = 0;
    santa.Art.top    = 480;
    santa.Art.right  = 371;
    santa.Art.bottom = 86;

    santa.Dest = santa.Art;
    santa.Source = santa.Art;
    santa.Active = FALSE;
    santa.FrameCounter = 0;
    

}   // SetupSanta


/************************************************************************
*    FUNCTION NAME:         SetupPeople                                                             
*
*    DESCRIPTION:           Setup the people
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

void CAnimateSaver::SetupPeople()
{
    int i;

    // clear out the structor arrays
    memset( &people, 0, sizeof(SPeople) * MAX_PEOPLE_ANIM );

    // Set up all the art info
    people[0].Art.left   = 0;
    people[0].Art.top    = 381;
    people[0].Art.right  = 37;
    people[0].Art.bottom = 46;
    people[0].NumFrames  = 9;
    people[0].PixelSpeed = 1;
    people[0].AnimDelay  = 90;
    people[0].WalkDelay  = 30;

    people[1].Art.left   = 668;
    people[1].Art.top    = 381;
    people[1].Art.right  = 26;
    people[1].Art.bottom = 49;
    people[1].NumFrames  = 9;
    people[1].PixelSpeed = 1;
    people[1].AnimDelay  = 90;
    people[1].WalkDelay  = 40;

    people[2].Art.left   = 0;
    people[2].Art.top    = 275;
    people[2].Art.right  = 31;
    people[2].Art.bottom = 43;
    people[2].NumFrames  = 8;
    people[2].PixelSpeed = 1;
    people[2].AnimDelay  = 90;
    people[2].WalkDelay  = 60;

    people[3].Art.left   = 498;
    people[3].Art.top    = 275;
    people[3].Art.right  = 28;
    people[3].Art.bottom = 47;
    people[3].NumFrames  = 9;
    people[3].PixelSpeed = 1;
    people[3].AnimDelay  = 90;
    people[3].WalkDelay  = 50;

    people[4].Art.left   = 225;
    people[4].Art.top    = 1083;
    people[4].Art.right  = 30;
    people[4].Art.bottom = 43;
    people[4].NumFrames  = 9;
    people[4].PixelSpeed = 1;
    people[4].AnimDelay  = 90;
    people[4].WalkDelay  = 35;

    for( i = 0; i < MAX_PEOPLE_ANIM; ++i )
    {
        people[i].Dest = people[i].Art;
        people[i].Source = people[i].Art;
    }

} // SetupPeople


/************************************************************************
*    FUNCTION NAME:         UpdatePeoplePositions                                                             
*
*    DESCRIPTION:           Update all the people positions that are active
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

void CAnimateSaver::UpdatePeoplePositions( DWORD tmpTimer )
{
    int i;

    for( i = 0; i < MAX_PEOPLE_ANIM; ++i )
    {
        if( people[i].Active )
        {
            switch( people[i].Movement )
            {
                case MOVE_LEFT:
                    if( people[i].WalkTimer < tmpTimer )
                    {
                        people[i].Dest.left -= people[i].PixelSpeed;

                        // Reset the animation timer
                        people[i].WalkTimer = tmpTimer + people[i].WalkDelay;

                        // Person is done. Stop the animation
                        if( people[i].Dest.left <= -people[i].Dest.right )
                            people[i].Active = FALSE;
                    }
                break;

                case MOVE_RIGHT:
                    if( people[i].WalkTimer < tmpTimer )
                    {
                        people[i].Dest.left += people[i].PixelSpeed;

                        // Reset the animation timer
                        people[i].WalkTimer = tmpTimer + people[i].WalkDelay;

                        // Person is done. Stop the animation
                        if( people[i].Dest.left >= DesktopWidth )
                            people[i].Active = FALSE;
                    }
                break;
            }
        }
    }

    // See if we'll have some other people animating
    PickNextPersonToAnimate( tmpTimer );

}   // UpdatePeoplePositions


/************************************************************************
*    FUNCTION NAME:         PickNextPersonToAnimate                                                             
*
*    DESCRIPTION:           Picks the next person to animate
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

void CAnimateSaver::PickNextPersonToAnimate( DWORD tmpTimer )
{
    // Randomly pick a person
    int pick = rand() % MAX_PEOPLE_ANIM;

    if( !people[ pick ].Active && (PeoplePickTimer < tmpTimer) )
    {
        // Set if it will be comming from the left, right.
        people[ pick ].Movement = rand() % MAX_NUM_OF_MOVEMENTS;
        
        // Update the source rect
        if( people[ pick ].Movement == MOVE_LEFT )
        {
            // The reverse art is to the left or under the art
            people[ pick ].Source.left = people[ pick ].Art.left + 
                                         (people[ pick ].Art.right * people[ pick ].NumFrames);

            people[ pick ].Dest.left = DesktopWidth;
        }
        else
        {
            people[ pick ].Source.left = people[ pick ].Art.left;
            people[ pick ].Source.top = people[ pick ].Art.top;
            people[ pick ].Dest.left = -people[ pick ].Dest.right;
        }

        // Set the height of the person
        people[ pick ].Dest.top  = DesktopHeight - people[ pick ].Art.bottom - ((rand() % MAX_BOTTOM_EDGE)+MIN_BOTTOM_EDGE);

        // Set the active state
        people[ pick ].Active = TRUE;

        // Set the pixel speed
        // Reset the animation timer
        people[ pick ].AnimTimer = tmpTimer + people[ pick ].AnimDelay;

        // Sort all the people
        Sort();

        // Reset the timer.
        PeoplePickTimer = tmpTimer + ( rand() % PEOPLE_PICK_TIME_MAX ) + PEOPLE_PICK_TIME_MIN;          
    }
    else if( PeoplePickTimer < tmpTimer )
        PeoplePickTimer = tmpTimer + ( rand() % PEOPLE_PICK_TIME_MAX ) + PEOPLE_PICK_TIME_MIN;

}   // PickNextPersonToAnimate


/************************************************************************
*    FUNCTION NAME:         Sort                                                             
*
*    DESCRIPTION:           Sort all the people by height pos in the screen
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

void CAnimateSaver::Sort()
{
    int offset, j, x=0;
    BOOL inOrder = FALSE;
    SPeople Temp;

    offset = MAX_PEOPLE_ANIM;

    while( x < offset && inOrder == FALSE )
    {
        ++x;
        inOrder = TRUE;
        for( j = 0; j < offset - x; ++j )
        {
            if( (people[j].Dest.top + people[j].Dest.bottom) > (people[j+1].Dest.top + people[j+1].Dest.bottom) )
            {
                inOrder = FALSE;
                Temp = people[j];
                people[j] = people[j+1];
                people[j+1] = Temp;
            }
        }
    }

}   // Sort


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
        
        // Init the buffer
            BlitToTheBuffer();

        // Now blit the buffer to the screen only if not in shareware mode
        dirtyBuffer.Draw( GetScreenDC(), &DesktopRect );       
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
            if( AnimCounter == 0 && (SaverTimer < timeGetTime()) )
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
    int i;

    // Skip this part if we are in preview mode to help the preview mode animate faster
    if( Mode() == ssPreview && PreviewCounter == 0 )
        return;

    // Erase the snow
    for( i = 0; i < snowCount; ++i )
        cleanBuffer.Draw( &dirtyBuffer, &pSnow[i].DestRect );

    // Erase the people
    for( i = 0; i < MAX_PEOPLE_ANIM; ++i )
        if( people[i].Active )
            cleanBuffer.Draw( &dirtyBuffer, &people[i].Dest );

}   // EraseTheBuffer


/************************************************************************
*    FUNCTION NAME:         UpdateSanta                                                             
*
*    DESCRIPTION:           Updates Santa
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

void CAnimateSaver::UpdateSanta( DWORD tmpTimer )
{
    if( !santa.Active && (SantaTimer < tmpTimer) )
    {
        // Set if it will be comming from the left, right.
        santa.Movement = rand() % MAX_NUM_OF_MOVEMENTS;
        
        // Update the source rect
        if( santa.Movement == MOVE_LEFT )
        {
            // The reverse art is to the left or under the art
            santa.Source.left = santa.Art.left + santa.Art.right;

            santa.Dest.left = DesktopWidth;
        }
        else
        {
            santa.Source.left = santa.Art.left;
            santa.Source.top = santa.Art.top;
            santa.Dest.left = -santa.Dest.right;
        }

        // Set the height o santa
        if( DesktopHeight >= 768 )
            santa.Dest.top = ((DesktopHeight - NEIGHBORHOOD_H) / 2) + santa.Art.bottom;
        else
            santa.Dest.top = ((DesktopHeight - NEIGHBORHOOD_H) / 2) - (santa.Art.bottom / 2);

        // Set the active state
        santa.Active = TRUE;

        // Reset the animation timer
        santa.AnimTimer = timeGetTime() + SANTA_ANIM_DELAY; 
    }

    if( santa.Active )
    {
        switch( santa.Movement )
        {
            case MOVE_LEFT:

                // Move Santa
                santa.Dest.left -= SANTA_PIXEL_SPEED;

                // Santa is done. Stop the animation
                if( santa.Dest.left <= -santa.Dest.right )
                {
                    santa.Active = FALSE;
                    SantaTimer = timeGetTime() + ((rand() % MAX_SANTA_START_DELAY)+MIN_SANTA_START_DELAY);
                }
            break;

            case MOVE_RIGHT:

                // Move Santa
                santa.Dest.left += SANTA_PIXEL_SPEED;

                // Santa is done. Stop the animation
                if( santa.Dest.left >= DesktopWidth )
                {
                    santa.Active = FALSE;
                    SantaTimer = timeGetTime() + ((rand() % MAX_SANTA_START_DELAY)+MIN_SANTA_START_DELAY);
                }
            break;
        }
    }

}   // UpdateSanta


/************************************************************************
*    FUNCTION NAME:         UpdateBlinkers                                                             
*
*    DESCRIPTION:           Updates all the blinkers
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

void CAnimateSaver::UpdateBlinkers( DWORD tmpTimer )
{
    int i;

    // Is it time to update the blinkers?
    for( i = 0; i < BlinkerCount; ++i )
    {
        if( pBlinker[i].BlinkTimer < tmpTimer )
        {
            // Reset the blinker state
            pBlinker[i].BlinkState = (pBlinker[i].BlinkState + 1) % MAX_BLINKER_STATES;

            if( pBlinker[i].BlinkState == BLINKER_OFF )
                // Reset the timer
                pBlinker[i].BlinkTimer = pBlinker[i].TimerIntervalOff + tmpTimer;

            else if( pBlinker[i].BlinkState == BLINKER_ON )
                // Reset the timer
                pBlinker[i].BlinkTimer = pBlinker[i].TimerIntervalOn + tmpTimer;

            // Set the flag that indicates a change has been made
            pBlinker[i].ChangeState = TRUE;
        }
    }
}   // UpdateBlinkers


/************************************************************************
*    FUNCTION NAME:         UpdateSnow                                                             
*
*    DESCRIPTION:           Updates all the snow
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

void CAnimateSaver::UpdateSnow( DWORD tmpTimer )
{
    int i;

    for( i = 0; i < snowCount; ++i )
    {
        // Have we timed out? Reset the blink state
        if( pSnow[i].BlinkTimer < tmpTimer )
        {
            // Set the timer
            pSnow[i].BlinkTimer = pSnow[i].TimerInterval + tmpTimer;

            // set the blink state
            pSnow[i].BlinkState = (pSnow[i].BlinkState + 1) % MAX_BLINKER_STATES;
        }

        // Do we need to reset the snow?
        if( pSnow[i].DestRect.left <= -pSnow[i].DestRect.right || // did we go off the left side?
            pSnow[i].DestRect.left >= DesktopWidth ||             // did we go off the right side?
            pSnow[i].DestRect.top >= DesktopHeight )              // did we vanish below the bottom?
        {
            // Reset the snow top
            pSnow[i].DestRect.top = -pSnow[i].DestRect.bottom;

            // Randomize the snows position
            pSnow[i].DestRect.left = rand() % (DesktopWidth-pSnow[i].DestRect.right);

            // Set the pixel X speed
            pSnow[i].PixelSpeedX = -1 + (rand() % MAX_PIXEL_SPEEDSX);

            // Set the pixel Y speed
            pSnow[i].PixelSpeedY = (rand() % MAX_PIXEL_SPEEDSY) + 1;
        }
        else
        {
            // Inc the X
            pSnow[i].DestRect.left += pSnow[i].PixelSpeedX;

            // Inc the Y
            pSnow[i].DestRect.top += pSnow[i].PixelSpeedY;
        }
    }
}   // UpdateSnow


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
    DWORD tmpTimer = timeGetTime();

    // Is it time to update the blinkers?
    UpdateBlinkers( tmpTimer );

    // Update the snow animation
    UpdateSnow( tmpTimer );

    // Update all the people
    if( Settings.GetPeople() )
        UpdatePeoplePositions( tmpTimer );

    // Update Santa
    if( Settings.GetSanta() )
        UpdateSanta( tmpTimer );

}   // UpdateAllAnimations


/************************************************************************
*    FUNCTION NAME:         BlitBlinkersToTheBuffer                                                             
*
*    DESCRIPTION:           Blit the blinkers to the buffer
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

void CAnimateSaver::BlitBlinkersToTheBuffer()
{
    int i;

    // Update the blitters
    for( i = 0; i < BlinkerCount; ++i )
    {
        // Has a change been made to a blinker?
        if( pBlinker[i].ChangeState )
        {
            if( pBlinker[i].BlinkState == BLINKER_OFF )
            {
                snowArt.Draw( &cleanBuffer, &pBlinker[i].DestRect, &pBlinker[i].ArtBlinkOff );
                snowArt.Draw( &dirtyBuffer, &pBlinker[i].DestRect, &pBlinker[i].ArtBlinkOff );
            }

            else if( pBlinker[i].BlinkState == BLINKER_ON )
            {
                snowArt.Draw( &cleanBuffer, &pBlinker[i].DestRect, &pBlinker[i].ArtBlinkOn );
                snowArt.Draw( &dirtyBuffer, &pBlinker[i].DestRect, &pBlinker[i].ArtBlinkOn );
            }
        }

        #if defined( DEBUG ) && !defined( FULL_SCREEN_DEBUG )
        pBlinker[i].ChangeState = FALSE;
        #endif
    }

}   // BlitBlinkersToTheBuffer


/************************************************************************
*    FUNCTION NAME:         BlitSmokeToTheBuffer                                                             
*
*    DESCRIPTION:           Blit the smoke to the buffer
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

void CAnimateSaver::BlitSmokeToTheBuffer()
{
    int i;
    RECT source;
    BOOL incFrame = FALSE;

    // is it time to inc the frame
    if( SmokeTimer < timeGetTime() )
    {
        // Reset the animation timer
        SmokeTimer = timeGetTime() + SMOKE_ANIM_DELAY;

        incFrame = TRUE;
    }

    // Blit the smoke
    for( i = 0; i < SmokeCount; ++i )
    {
        if( incFrame )
            // inc the animation frame
            pSmoke[i].FrameCounter = ( pSmoke[i].FrameCounter + 1 ) % MAX_SMOKE_FRAMES;

        source = pSmoke[i].Source;
        source.left += source.right * pSmoke[i].FrameCounter;
        snowArt.Draw( &dirtyBuffer, &pSmoke[i].Dest, &source );
    }

}   // BlitSmokeToTheBuffer


/************************************************************************
*    FUNCTION NAME:         BlitPeopleToTheBuffer                                                             
*
*    DESCRIPTION:           Blit the people to the buffer
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

void CAnimateSaver::BlitPeopleToTheBuffer()
{
    int i;
    RECT rect, source;
    DWORD tmpTimer;

    // Init the timer
    tmpTimer = timeGetTime();

    // Blit the people
    for( i = 0; i < MAX_PEOPLE_ANIM; ++i )
    {
        if( people[i].Active )
        {
            if( people[i].AnimTimer < tmpTimer )
            {
                // inc the animation frame
                people[i].FrameCounter = ( people[i].FrameCounter + 1 ) % people[i].NumFrames;

                // Reset the animation timer
                people[i].AnimTimer = tmpTimer + people[i].AnimDelay;
            }

            snowArt.SetCopyMode( SRCAND );
            rect = people[i].Source;
            rect.top += rect.bottom;
            source = people[i].Source;
            source.left += source.right * people[i].FrameCounter;
            rect.left += rect.right * people[i].FrameCounter;
            snowArt.Draw( &dirtyBuffer, &people[i].Dest, &rect );
            snowArt.SetCopyMode( SRCPAINT );
            snowArt.Draw( &dirtyBuffer, &people[i].Dest, &source );
        }
    }

}   // BlitPeopleToTheBuffer


/************************************************************************
*    FUNCTION NAME:         BlitSantaToTheBuffer
*
*    DESCRIPTION:           Blit the santa to the buffer
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

void CAnimateSaver::BlitSantaToTheBuffer()
{
    RECT source;

    if( santa.Active )
    {
        if( santa.AnimTimer < timeGetTime() )
        {
            // inc the animation frame
            santa.FrameCounter = ( santa.FrameCounter + 1 ) % MAX_SANTA_FRAMES;

               // Cause santa to go upward a little
            if( DesktopHeight > 600 )
                   santa.Dest.top -= 1;

            // Reset the animation timer
            santa.AnimTimer = timeGetTime() + SANTA_ANIM_DELAY;
        }

        source = santa.Source;
        source.top += source.bottom * santa.FrameCounter;
        snowArt.Draw( &dirtyBuffer, &santa.Dest, &source );
    }

}   // BlitSantaToTheBuffer


/************************************************************************
*    FUNCTION NAME:         BlitSnowToTheBuffer                                                             
*
*    DESCRIPTION:           Blit the snow to the buffer
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

void CAnimateSaver::BlitSnowToTheBuffer()
{
    int i;

    // Reset the copy mode
    snowArt.SetCopyMode( SRCPAINT );

    // Blit the snow
    for( i = 0; i < snowCount; ++i )
    {
        if( pSnow[i].BlinkState == BLINKER_OFF )
            snowArt.Draw( &dirtyBuffer, &pSnow[i].DestRect, &pSnow[i].ArtBlinkOff );

        else if( pSnow[i].BlinkState == BLINKER_ON )
            snowArt.Draw( &dirtyBuffer, &pSnow[i].DestRect, &pSnow[i].ArtBlinkOn );
    }

    // Reset the copy mode
    snowArt.SetCopyMode( SRCCOPY );

}   // BlitSnowToTheBuffer


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
    if( Mode() == ssPreview && PreviewCounter == MAX_PREVIEW_TIMER_COUNTS-1 )
        return;

    // Blit the blinkers
    BlitBlinkersToTheBuffer();

    // Blit smoke to the buffer
    if( Settings.GetSmoke() )
        BlitSmokeToTheBuffer();

    // Blit santa to the buffer
    if( Settings.GetSanta() )
        BlitSantaToTheBuffer();

    // Blit the people
    if( Settings.GetPeople() )
        BlitPeopleToTheBuffer();

    // Blit the snow
    BlitSnowToTheBuffer();

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

        // Repaint the whole screen if we have too.
        // This screen saver needs to repaint the whole screen in the even
        // the monitor wakes up from a sleep
        if( RepaintScreen() )
        {
            dirtyBuffer.Draw( GetScreenDC() );
        }
        else
        {
            // Paint people to the screen
            if( Settings.GetPeople() )
                BlitPeopleToScreen();

            // blit the snow
            BlitSnowToScreen();

            // Paint changed blinkers to the screen
            BlitChangedBlinkersToScreen();

            // Blit the smaoke to the screen
            if( Settings.GetSmoke() )
                BlitSmokeToScreen();

            // Blit santa to the screen
            if( Settings.GetSanta() )
                BlitSantaToScreen();
        }

        #else
            dirtyBuffer.StretchDraw( GetScreenDC(), &WindowRect );
        #endif
    }
    else
    {
        // update the screen every X numer of intervals so that it doesn't hog the CPU cycles
        if( PreviewCounter == 0 )
            dirtyBuffer.StretchDraw( GetScreenDC(), &WindowRect );

        // Inc the preview counter
        PreviewCounter = ( PreviewCounter + 1 ) % MAX_PREVIEW_TIMER_COUNTS;
    }

    // Show an error text if there is a problem playing music
    DisplayMusicError();

}   // BlitFromBufferToScreen


/************************************************************************
*    FUNCTION NAME:         BlitPeopleToScreen                                                             
*
*    DESCRIPTION:           Blit the people to the screen
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

void CAnimateSaver::BlitPeopleToScreen()
{
    // Use a local variable for speed in these tight loops
    HDC ScreenDC = GetScreenDC();
    int i;

    // Paint people to the screen
    for( i = 0; i < MAX_PEOPLE_ANIM; ++i )
        if( people[i].Active )
            dirtyBuffer.Draw( ScreenDC, &people[i].Dest );

}   // BlitPeopleToScreen


/************************************************************************
*    FUNCTION NAME:         BlitSmokeToScreen                                                             
*
*    DESCRIPTION:           Blit the smoke to the screen
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

void CAnimateSaver::BlitSmokeToScreen()
{
    // Use a local variable for speed in these tight loops
    HDC ScreenDC = GetScreenDC();
    int i;

    // Paint people to the screen
    for( i = 0; i < SmokeCount; ++i )
        dirtyBuffer.Draw( ScreenDC, &pSmoke[i].Dest );

}   // BlitSmokeToScreen


/************************************************************************
*    FUNCTION NAME:         BlitSantaToScreen                                                             
*
*    DESCRIPTION:           Blit the Santa to the screen
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

void CAnimateSaver::BlitSantaToScreen()
{
    // Paint Santa to the screen
    if( santa.Active )
        dirtyBuffer.Draw( GetScreenDC(), &santa.Dest );

}   // BlitSantaToScreen


/************************************************************************
*    FUNCTION NAME:         BlitSnowToScreen                                                             
*
*    DESCRIPTION:           Blit the snow to the screen
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

void CAnimateSaver::BlitSnowToScreen()
{
    // Use a local variable for speed in these tight loops
    HDC ScreenDC = GetScreenDC();
    int i;

    // blit the snow
    for( i = 0; i < snowCount; ++i )
        dirtyBuffer.Draw( ScreenDC, &pSnow[i].DestRect );

}   // BlitSnowToScreen


/************************************************************************
*    FUNCTION NAME:         BlitChangedBlinkersToScreen                                                             
*
*    DESCRIPTION:           Blit the changed blinkers to the screen
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

void CAnimateSaver::BlitChangedBlinkersToScreen()
{
    // Use a local variable for speed in these tight loops
    HDC ScreenDC = GetScreenDC();
    int i;

    // Paint changed blinkers to the screen
    for( i = 0; i < BlinkerCount; ++i )
    {
        // Has a change been made to a blinker?
        if( pBlinker[i].ChangeState )
        {            
            dirtyBuffer.Draw( ScreenDC, &pBlinker[i].DestRect );

            pBlinker[i].ChangeState = FALSE;
        }
    }

}   // BlitChangedBlinkersToScreen


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

    if( pSnow )
        delete [] pSnow;

    if( pSmoke )
        delete [] pSmoke;

    if( pBlinker )
        delete [] pBlinker;
            
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