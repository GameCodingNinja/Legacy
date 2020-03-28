
/************************************************************************
*
*    PRODUCT:         Sound Manager
*
*    FILE NAME:       SoundMgr.cpp
*
*    DESCRIPTION:     Loads and plays wave files. For loading resource files,
*                     this class expects the PCM version of the wave file 
*                     which is the wav format and data is stripped from a      
*                     standard PCM format wav file.     
*
*    IMPLEMENTATION:  
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
                             // Note: we are not using the #define WIN32_LEAN_AND_MEAN because we need sound features

#include "SoundMgr.h"        // Header file for this *.cpp file.
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.



/************************************************************************
*    FUNCTION NAME:              Constructor                                                            
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CSoundMgr::CSoundMgr()
{
    // init class variables
    loadCount = 0;
    devCount = 0;
    cycleCount = 0;
    pWavLoad = NULL;
    pWavDev = NULL;

}   // Constructor


/************************************************************************
*    FUNCTION NAME:              Destructor                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CSoundMgr::~CSoundMgr()
{
    Free();

}   // Destructor


/************************************************************************
*    FUNCTION NAME:         FreeLoadedWav                                                             
*
*    DESCRIPTION:           Free the loaded wav files.
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CSoundMgr::FreeLoadedWav()
{
    if( pWavLoad )
        delete [] pWavLoad;

    pWavLoad = NULL;
    loadCount = 0;

} // FreeLoadedWav


/************************************************************************
*    FUNCTION NAME:         FreeAllocDev                                                             
*
*    DESCRIPTION:           Free the allocated sound devices.
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CSoundMgr::FreeAllocDev()
{
    if( pWavDev )
        delete [] pWavDev;

    pWavDev = NULL;
    devCount = 0;
    cycleCount = 0;
    intList.Free();

} // FreeAllocDev


/************************************************************************
*    FUNCTION NAME:         AllocateWavLoad                                                             
*
*    DESCRIPTION:           Allocate the number of wave load objects.
*
*    FUNCTION PARAMETERS:
*    Input:    int Count - Number to allocate
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CSoundMgr::AllocateWavLoad( int Count )
{
    BOOL Result = FALSE;

    // Free any previous wav loades
    FreeLoadedWav();

    // Allocate the wave load
    if( Count )
    {
        pWavLoad = new CWaveLoad[ Count ];

        // Test all allocations
        if( pWavLoad )
        {
            loadCount = Count;
            Result = TRUE;
        }   
    }

    return Result;

}   // AllocateWavLoad


/************************************************************************
*    FUNCTION NAME:         AllocateWavDevices                                                             
*
*    DESCRIPTION:           Allocate the number of wave device objects.
*
*    FUNCTION PARAMETERS:
*    Input:    int Count - Number to allocate
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CSoundMgr::AllocateWavDevices( int Count )
{
    BOOL Result = FALSE;
    int i;

    // Free any previous wav devices
    FreeAllocDev();

    // Allocate the wave load
    if( Count )
    {
        pWavDev = new CWaveDevice[ Count ];

        // Test all allocations
        if( pWavDev )
        {
            devCount = Count;
            Result = TRUE;

            // Setup the int list class if we are using more then one
            if( Count > 1 )
            {
                for( i = 0; i < Count; ++i )
                    intList.Add( i );

                // Mixup all the indexes
                intList.MixUp();
            }
        }   
    }

    return Result;

}   // AllocateWavDevices


/************************************************************************
*    FUNCTION NAME:         Play()                                                             
*
*    DESCRIPTION:           Play the sound.
*
*    FUNCTION PARAMETERS:
*    Input:    int DevID   - Plays the sound of the given device ID
*              DWORD loopCount - Number of times to loop
*
*    Output:   BOOL - Returns TRUE if successful or FALSE otherwise
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CSoundMgr::Play( int DevID, DWORD loopCount )
{
    BOOL Result = FALSE;

    if( DevID < devCount && pWavDev[DevID].Play( loopCount ) == MMSYSERR_NOERROR )
        Result = TRUE;

    return Result;

}   // Play


/************************************************************************
*    FUNCTION NAME:         Stop()                                                             
*
*    DESCRIPTION:           Load bitmap from resource file.
*
*    FUNCTION PARAMETERS:
*    Input:    int DevID   - Plays the sound of the given device ID
*              DWORD loopCount - Number of times to loop
*
*    Output:   BOOL - Returns TRUE if successful or FALSE otherwise
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CSoundMgr::Stop( int DevID )
{
    BOOL Result = FALSE;

    if( DevID < devCount && pWavDev[DevID].Stop() == MMSYSERR_NOERROR )
        Result = TRUE;

    return Result;

}   // Stop


/************************************************************************
*    FUNCTION NAME:         PlayRandom                                                             
*
*    DESCRIPTION:           Once all sounds have been played, it is
*                           the mixed up again
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   BOOL - Returns TRUE if successful or FALSE otherwise
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CSoundMgr::PlayRandom()
{
    BOOL Result = FALSE;

    if( devCount )
    {
        if( pWavDev[intList.GetInt(cycleCount)].Play() == MMSYSERR_NOERROR )
            Result = TRUE;

        // Inc the counter
        cycleCount = (cycleCount + 1) % devCount;

        if( cycleCount == 0 )
            intList.MixUp();
    }
    
    return Result;

}   // PlaySoundsRandomly


/************************************************************************
*    FUNCTION NAME:         PlayInOrder                                                             
*
*    DESCRIPTION:           Play the sounds in order
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   BOOL - Returns TRUE if successful or FALSE otherwise
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CSoundMgr::PlayInOrder()
{
    BOOL Result = FALSE;

    if( devCount )
    {
        if( pWavDev[cycleCount].Play() == MMSYSERR_NOERROR )
            Result = TRUE;

        // Inc the counter
        cycleCount = (cycleCount + 1) % devCount;
    }
    
    return Result;

}   // PlaySoundsRandomly


/************************************************************************
*    FUNCTION NAME:         LoadFromResource                                                             
*
*    DESCRIPTION:           Load from Resource
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CSoundMgr::LoadFromResource( HINSTANCE hInst, char *name, int index )
{
    BOOL Result = FALSE;

    // Make sure we don't blow our index
    if( index < loadCount )
        Result = pWavLoad[index].LoadFromResource( hInst, name );

    return Result;

}   // LoadFromResource


/************************************************************************
*    FUNCTION NAME:         LoadFromFile                                                             
*
*    DESCRIPTION:           Load from file
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CSoundMgr::LoadFromFile( char *filePath, int index )
{
    BOOL Result = FALSE;

    // Make sure we don't blow past our index
    if( index < loadCount )
        Result = pWavLoad[index].LoadFromFile( filePath );

    return Result;

}   // LoadFromFile


/************************************************************************
*    FUNCTION NAME:         ApplyLoadToDevice                                                             
*
*    DESCRIPTION:           Apply the loaded file to an allocated device
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CSoundMgr::ApplyLoadToDevice( int loadIndex, int devIndex )
{
    BOOL Result = FALSE;

    // Make sure we don't blow past our index
    if( loadIndex < loadCount && devIndex < devCount )
    {
        // Set the index of the sound files being loaded.
        pWavDev[devIndex].SetLoadIndex( loadIndex );

        // Close the wave device if it is open
        if( pWavDev[devIndex].Close() != MMSYSERR_NOERROR )
            goto HANDLE_ERROR;

        // Set the header
        if( pWavDev[devIndex].SetWaveHeader( pWavLoad[loadIndex].GetHeader() ) != MMSYSERR_NOERROR )
            goto HANDLE_ERROR;

        // Open a wav device to this loaded wav format
        if( pWavDev[devIndex].Open( pWavLoad[loadIndex].GetWavFormat() ) != MMSYSERR_NOERROR )
            goto HANDLE_ERROR;

        // Prepare the header for playing
        if( pWavDev[devIndex].PrepareHeader() != MMSYSERR_NOERROR )
            goto HANDLE_ERROR;
    }

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // ApplyLoadToDevice


/************************************************************************
*    FUNCTION NAME:         GetErrorStr                                                             
*
*    DESCRIPTION:           Get the error string of the devise with a problem.
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

char *CSoundMgr::GetErrorStr()
{
    char *Result = NULL;
    int i;

    for( i = 0; i < devCount; ++i )
    {
        if( pWavDev[i].IsSndError() )
        {
            Result = pWavDev[i].GetErrorStr();
            break;
        }
    }

    return Result;

}   // GetErrorStr


/************************************************************************
*    FUNCTION NAME:         IsSndError                                                             
*
*    DESCRIPTION:           Did we get a sound error?
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CSoundMgr::IsSndError()
{
    BOOL Result = FALSE;
    int i;

    for( i = 0; i < devCount; ++i )
    {
        if( pWavDev[i].IsSndError() )
        {
            Result = TRUE;
            break;
        }
    }

    return Result;
    
}   // IsSndError


/************************************************************************
*    FUNCTION NAME:         IsMjrPlaying                                                             
*
*    DESCRIPTION:           Is one of the devices playing a sound
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CSoundMgr::IsMjrPlaying()
{
    int i;
    BOOL Result = FALSE;

    for( i = 0; i < devCount; ++i )
    {
        if( pWavDev[i].IsPlaying() )
        {
            Result = TRUE;
            break;
        }
    }

    return Result;

}   // IsMjrPlaying


/************************************************************************
*    FUNCTION NAME:         IncDevCounter                                                             
*
*    DESCRIPTION:           inc to the next sound device
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CSoundMgr::IncDevCounter()
{
    if( devCount )
    {
        // inc the counter
        cycleCount = (cycleCount + 1) % devCount;
        
        // Mix it all up
        if( cycleCount == 0 )
            intList.MixUp();
    }

}   // IncDevCounter