
/************************************************************************
*
*    PRODUCT:         Low Level wave file load and play
*
*    FILE NAME:       TWave.cpp
*
*    DESCRIPTION:     This class provides functions for reading and playing
*                     PCM wave files.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

#include "TWave.h"           // Header file for this *.cpp file.
#include "minTools.h"        // A misc group of useful functions
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.


// Static data
WAVEOUTCAPS CWaveDevice::waveOutCaps;

/************************************************************************
*   CWaveDevice Class
*          
*   Low level wav playing. This provides the most control and the best
*   speed for playing wave files.
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         Constructor 
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

CWaveDevice::CWaveDevice()
{
    hWaveOut = NULL;
    memset( &waveHeader, 0, sizeof(WAVEHDR) );
    errorStr[0] = 0;
    loadIndex = -1;

}   // Constructor


/************************************************************************
*    FUNCTION NAME:         Destructor 
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

CWaveDevice::~CWaveDevice()
{
    // Close the player
    Close();

}  // Destructor


/************************************************************************
*    FUNCTION NAME:         Open                                                             
*
*    DESCRIPTION:           Send the format info of the loaded wave file
*                           to see if the audio device can play the data.
*
*    FUNCTION PARAMETERS:
*    Input:     LPWAVEFORMATEX pWaveFormat - Pointer to wave format data
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

MMRESULT CWaveDevice::Open( LPWAVEFORMATEX pWaveFormat )
{
    MMRESULT mmResult = MMSYSERR_LASTERROR + 1;

    // Make sure we don't have any pending errors
    if( !errorStr[0] )
    {
        // Close any opened devices and check for errors
        if( (mmResult = Close()) != MMSYSERR_NOERROR )
            return mmResult;

        // Try to open the device
        mmResult = waveOutOpen( &hWaveOut, WAVE_MAPPER, pWaveFormat, 0, 0, CALLBACK_NULL );

        // Set the error string if we had an error
        SetErrorStr( mmResult );
    }

    return mmResult;

}   // Open


/************************************************************************
*    FUNCTION NAME:         Close                                                             
*
*    DESCRIPTION:           Close the device for playing, if it is open
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

MMRESULT CWaveDevice::Close()
{
    MMRESULT mmResult = MMSYSERR_NOERROR;

    // Make sure we have a handle before we try to close anything
    if( hWaveOut )
    {
        // unprepare the wave header
        UnprepareHeader();

        // Try to close the device
        mmResult = waveOutClose( hWaveOut );

        // Set the error string if we had an error
        SetErrorStr( mmResult );

        // Null the nandle
        hWaveOut = FALSE;
    }

    return mmResult;

}   // Close


/************************************************************************
*    FUNCTION NAME:         Stop                                                             
*
*    DESCRIPTION:           Stop the device from playing. This resets the 
*                           play position back to the start.
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

MMRESULT CWaveDevice::Stop()
{
    MMRESULT mmResult = MMSYSERR_NOERROR; 

    // Make sure we have a handle and the wave data is playing
    if( hWaveOut && !(waveHeader.dwFlags & WHDR_DONE) )
    {
        // Try to stop the device
        mmResult = waveOutReset( hWaveOut );

        // Set the error string if we had an error
        SetErrorStr( mmResult );
    }

    return mmResult;

}   // Stop


/************************************************************************
*    FUNCTION NAME:         Play                                                             
*
*    DESCRIPTION:           Play the wav data to the device.
*
*    FUNCTION PARAMETERS:
*    Input:    DWORD loopCount - Set the number of times you want the sound to loop
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

MMRESULT CWaveDevice::Play( DWORD loopCount )
{
    MMRESULT mmResult = MMSYSERR_NOERROR; 

    // Make sure we have a handle and the wave data is in 
    // the queue before we try to stop anything
    if( hWaveOut && !errorStr[0] )
    {
        // Close any opened devices and check for errors
        if( (mmResult = Stop()) != MMSYSERR_NOERROR )
            return mmResult;

        // If we are to loop, set the loop flags
        if( loopCount )
            waveHeader.dwFlags |= WHDR_BEGINLOOP | WHDR_ENDLOOP;

        // Set the loop count
        waveHeader.dwLoops = loopCount;

        // Try to write the data out to the wave device
        mmResult = waveOutWrite( hWaveOut, &waveHeader, sizeof(WAVEHDR) );

        // Set the error string if we had an error
        SetErrorStr( mmResult );
    }

    return mmResult;

}   // PLay


/************************************************************************
*    FUNCTION NAME:         PrepareHeader                                                             
*
*    DESCRIPTION:           Prepare the wave data header to be written
*                           to the audio device.
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

MMRESULT CWaveDevice::PrepareHeader()
{
    MMRESULT mmResult = MMSYSERR_NOERROR; 

    // Make sure we have a handle and the wave data is in 
    // the queue before we try to stop anything
    if( hWaveOut )
    {
        // If the header has already been prepared
        // we need to unprepare it
        UnprepareHeader();

        // Try to prepare the header for the device
        mmResult = waveOutPrepareHeader( hWaveOut, &waveHeader, sizeof(WAVEHDR) );

        // Preset the done bit so we can tell nothing is playing
        waveHeader.dwFlags |= WHDR_DONE;

        // Set the error string if we had an error
        SetErrorStr( mmResult );
    }

    return mmResult;

}   // PrepareHeader


/************************************************************************
*    FUNCTION NAME:         UnprepareHeader                                                             
*
*    DESCRIPTION:           Prepare the wave data header to be written
*                           to the audio device.
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

MMRESULT CWaveDevice::UnprepareHeader()
{
    MMRESULT mmResult = MMSYSERR_NOERROR; 

    // Make sure we have a handle and the wave data is in 
    // the queue before we try to stop anything
    if( hWaveOut && (waveHeader.dwFlags & WHDR_PREPARED) )
    {
        // Stop any playing devices
        Stop();

        // Try to unprepare the header for the device
        mmResult = waveOutUnprepareHeader( hWaveOut, &waveHeader, sizeof(WAVEHDR) );

        // Set the error string if we had an error
        SetErrorStr( mmResult );
    }

    return mmResult;

}   // UnprepareHeader


/************************************************************************
*    FUNCTION NAME:         SetWaveHeader                                                             
*
*    DESCRIPTION:           Set the wave header
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

MMRESULT CWaveDevice::SetWaveHeader( LPWAVEHDR pHeader )
{
    MMRESULT mmResult = MMSYSERR_NOERROR;

    // See if we have a header that's already loaded
    if( hWaveOut )
    {
        // If the header has already been prepared
        // we need to unprepare it
        if( (mmResult = UnprepareHeader()) != MMSYSERR_NOERROR )
            return mmResult;
    }

    // Set the wave header
    memcpy( &waveHeader, pHeader, sizeof(WAVEHDR) );

    return mmResult;

}   // SetWaveHeader


/************************************************************************
*    FUNCTION NAME:         IsPlaying                                                             
*
*    DESCRIPTION:           See if we are playing a sound.
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

BOOL CWaveDevice::IsPlaying()
{
    BOOL Result = FALSE;

    // See if we are playing a sound
    if( hWaveOut && !(waveHeader.dwFlags & WHDR_DONE) )
        Result = TRUE;

    return Result;

}   // IsPlaying


/************************************************************************
*    FUNCTION NAME:         Pause                                                             
*
*    DESCRIPTION:           Pause the play back
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

MMRESULT CWaveDevice::Pause()
{
    MMRESULT mmResult = MMSYSERR_NOERROR; 

    // Make sure we have a handle and the wave data is playing
    if( hWaveOut && !(waveHeader.dwFlags & WHDR_DONE) )
    {
        // Try to stop the device
        mmResult = waveOutPause( hWaveOut );

        // Set the error string if we had an error
        SetErrorStr( mmResult );
    }

    return mmResult;

}   // Pause


/************************************************************************
*    FUNCTION NAME:         Restart                                                             
*
*    DESCRIPTION:           Restart the play back after a pause
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

MMRESULT CWaveDevice::Restart()
{
    MMRESULT mmResult = MMSYSERR_NOERROR; 

    // Make sure we have a handle and the wave data is playing
    if( hWaveOut && !(waveHeader.dwFlags & WHDR_DONE) )
    {
        // Try to stop the device
        mmResult = waveOutRestart( hWaveOut );

        // Set the error string if we had an error
        SetErrorStr( mmResult );
    }

    return mmResult;

}   // Restart


/************************************************************************
*    FUNCTION NAME:         SetVolume                                                             
*
*    DESCRIPTION:           Set the volume of the wav device. This changes
*                           the volume for all the devices and not just
*                           the one the handle applies to.
*
*    FUNCTION PARAMETERS:
*    Input:    DWORD volume - New volume value
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

MMRESULT CWaveDevice::SetVolume( DWORD volume )
{
    MMRESULT mmResult = MMSYSERR_NOERROR; 

    // Make sure we have a handle and the wave data is in 
    // the queue before we try to stop anything
    if( hWaveOut && !errorStr[0] )
    {
        // Try to write the data out to the wave devices
        mmResult = waveOutSetVolume( hWaveOut, volume );

        // Set the error string if we had an error
        SetErrorStr( mmResult );
    }

    return mmResult;

}   // SetVolume


/************************************************************************
*    FUNCTION NAME:         GetVolume                                                             
*
*    DESCRIPTION:           Set the volume of the wav device.
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

MMRESULT CWaveDevice::GetVolume( DWORD &volume )
{
    MMRESULT mmResult = MMSYSERR_NOERROR; 

    // Make sure we have a handle and the wave data is in 
    // the queue before we try to stop anything
    if( hWaveOut && !errorStr[0] )
    {
        // Try to write the data out to the wave devices
        mmResult = waveOutGetVolume( hWaveOut, &volume );

        // Set the error string if we had an error
        SetErrorStr( mmResult );
    }

    return mmResult;

}   // GetVolume


/************************************************************************
*    FUNCTION NAME:         TestMultiDeviceOpen                                                             
*
*    DESCRIPTION:           Test to see if we can open multiple devices.
*                           Not all sound cards can do this.
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

BOOL CWaveDevice::TestMultiDeviceOpen()
{    
    HWAVEOUT hTestWaveOut[TEST_WAV_OPEN_COUNT];
    WAVEFORMATEX testWaveFormat;
    BOOL Result = TRUE;
    int i;

    // Init a pretend wave format to test if
    // we can open more then one device at a time.
    testWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
    testWaveFormat.nChannels = 2;
    testWaveFormat.nSamplesPerSec = 22050;
    testWaveFormat.nAvgBytesPerSec = 88200;
    testWaveFormat.nBlockAlign = 4;
    testWaveFormat.wBitsPerSample = 16;
    testWaveFormat.cbSize = 24932;

    // Null all the wave out handles
    for( i = 0; i < TEST_WAV_OPEN_COUNT; ++i )
        hTestWaveOut[i] = NULL;

    // Try to open 5 wave devices
    for( i = 0; i < TEST_WAV_OPEN_COUNT; ++i )
    {
        // Break out if we error.
        if( waveOutOpen( &hTestWaveOut[i], WAVE_MAPPER, &testWaveFormat, 0, 0, CALLBACK_NULL ) != MMSYSERR_NOERROR )
        {
            Result = FALSE;
            break;
        }
    }

    // Go through all the handles and close them
    for( i = 0; i < TEST_WAV_OPEN_COUNT; ++i )
    {
        if( hTestWaveOut[i] )
            waveOutClose( hTestWaveOut[i] );
        else
            break;
    }

    return Result;

}   // TestMultiDeviceOpen


/************************************************************************
*    FUNCTION NAME:         SetErrorStr                                                             
*
*    DESCRIPTION:           Sets the error string if there was an error.
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

void CWaveDevice::SetErrorStr( MMRESULT mmResult )
{
    // We only want to catch the first error
    if( !errorStr[0] && mmResult != MMSYSERR_NOERROR )
    {
        // Set the error strings if there was a problem
        if( mmResult == MMSYSERR_ERROR )
            strcpy( errorStr, "Unknown error in playing wave file" );

        else if( mmResult == MMSYSERR_BADDEVICEID )
            strcpy( errorStr, "Specified device identifier is out of range." );

        else if( mmResult == MMSYSERR_NOTENABLED )
            strcpy( errorStr, "Specified device driver failed enable." );

        else if( mmResult == MMSYSERR_ALLOCATED )
            strcpy( errorStr, "Error allocating sound resources. Try updating to the newest drivers for your sound device." );

        else if( mmResult == MMSYSERR_INVALHANDLE )
            strcpy( errorStr, "Specified device handle is invalid." );

        else if( mmResult == MMSYSERR_NODRIVER )
            strcpy( errorStr, "No device driver is present." );

        else if( mmResult == MMSYSERR_NOMEM )
            strcpy( errorStr, "Unable to allocate or lock memory." );

        else if( mmResult == MMSYSERR_NOTSUPPORTED )
            strcpy( errorStr, "Specified device is synchronous and does not support pausing." );

        else if( mmResult == MMSYSERR_HANDLEBUSY )
            strcpy( errorStr, "Specified device identifier is currently in use." );

        else if( mmResult == MMSYSERR_NODRIVERCB )
            strcpy( errorStr, "Driver does not call DriverCallback." );

        else if( mmResult == WAVERR_STILLPLAYING )
            strcpy( errorStr, "There are still buffers in the queue." );

        else if( mmResult == WAVERR_UNPREPARED )
            strcpy( errorStr, "The wave header data block hasn't been prepared for play back." );

        else if( mmResult == WAVERR_BADFORMAT )
            strcpy( errorStr, "Attempted to open with an unsupported waveform-audio format." );

        else if( mmResult == WAVERR_SYNC )
            strcpy( errorStr, "The device is synchronous but was opened without the WAVE_ALLOWSYNC flag." );

        else
            strcpy( errorStr, "Unknown error in using wave out device." );
    }

}   // SetErrorStr



/************************************************************************
*   CWaveLoad Class
*          
*   This class is for loading standard PCM (not compressed) RIFF wave
*   files. You can't use this class for loading wave files with
*   compressed data. The main reason you would want to use this class
*   is for loading waves to keep in buffered memory for fast sound play,
*   or for more control over the wave data for on-the-fly manipulation.
*   Also, dancing low level with the sound API provides more work but
*   more control over sound features.
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         Constructor 
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

CWaveLoad::CWaveLoad()
{
    fResLoad = FALSE;

    // zero out the wav format settings
    memset( &WaveFormat, 0, sizeof( WAVEFORMATEX ) );

    // zero out the wav header
    memset( &waveHeader, 0, sizeof( WAVEHDR ) );

}   // Constructor


/************************************************************************
*    FUNCTION NAME:         Destructor 
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

CWaveLoad::~CWaveLoad()
{
   Free();

}  // Destructor


/************************************************************************
*    FUNCTION NAME:         LoadFromFile                                                             
*
*    DESCRIPTION:           Load a wave file. It's amazing the hoops you
*                           have to jump through just to load one of 
*                           these files.
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName - File to load
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

BOOL CWaveLoad::LoadFromFile( char *FileName )
{
    MMCKINFO loadChunk;
    MMCKINFO ParentChunk;
    const FOURCC WaveFCC = mmioFOURCC ( 'W', 'A', 'V', 'E' );
    const int MinSize = sizeof( WaveFormat ) - sizeof( WaveFormat.cbSize );
    HMMIO hWaveFile;
    BOOL Result = FALSE;

    // Close the wav file if one was previously opened
    Free();

    // Attempt to open the specified RIFF file
    hWaveFile = mmioOpen ( FileName, NULL, MMIO_ALLOCBUF | MMIO_READ );

    // Check that we have a handle
    if( !hWaveFile )
        goto HANDLE_ERROR;

    // Zero out the parent chunk
    memset( &ParentChunk, 0, sizeof( ParentChunk ) );

    // Descend into the parent chunk
    if( mmioDescend( hWaveFile, &ParentChunk, NULL, 0 ) != MMSYSERR_NOERROR )
        goto HANDLE_ERROR;

    // Make sure the parent chunk is actually a RIFF chunk 
    // and that it contains wave data
    if( ( ParentChunk.ckid != FOURCC_RIFF ) || ( ParentChunk.fccType != WaveFCC ) )
        goto HANDLE_ERROR;
   
    // Zero out a chunk
    memset( &loadChunk, 0, sizeof( loadChunk ) );

    // Set the chunk identifier we are looking for
    loadChunk.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );

    // Descend into the 'fmt ' chunk, where the attributes 
    // of the wave data are stored
    if( mmioDescend( hWaveFile, &loadChunk, &ParentChunk, MMIO_FINDCHUNK ) != MMSYSERR_NOERROR )
        goto HANDLE_ERROR;

    // Make sure the format chunk contains sufficient information
    if( loadChunk.cksize < MinSize )
        goto HANDLE_ERROR;
   
    // Read the format chunk's data
    if( mmioRead( hWaveFile, (char *)&WaveFormat, sizeof( WAVEFORMATEX ) ) < MinSize )
        goto HANDLE_ERROR;

    // Seek to the beginning of the parent chunk to begin a search
    if( mmioSeek( hWaveFile, ParentChunk.dwDataOffset + sizeof( FOURCC ), SEEK_SET ) == -1 )
        goto HANDLE_ERROR;

    // Zero out a chunk
    memset( &loadChunk, 0, sizeof( loadChunk ) );

    // Set the chunk identifier we are looking for
    loadChunk.ckid = mmioFOURCC ( 'd', 'a', 't', 'a' );

    // Descend into the 'data' chunk
    if( mmioDescend( hWaveFile, &loadChunk, &ParentChunk, MMIO_FINDCHUNK ) != MMSYSERR_NOERROR )
        goto HANDLE_ERROR;

    // Make sure we have something greater then zero
    if( loadChunk.cksize == 0 )
        goto HANDLE_ERROR;

    // Allocate the buffer to hold the wave data
    waveHeader.lpData = new char[ loadChunk.cksize ];

    // Check all allocations
    if( !waveHeader.lpData )
        goto HANDLE_ERROR;

    // Read the wave data
    if( mmioRead( hWaveFile, waveHeader.lpData, loadChunk.cksize ) != loadChunk.cksize )
        goto HANDLE_ERROR;

    // Last but not least, fill in the relevant parts of the wave header
    waveHeader.dwBufferLength = loadChunk.cksize;
    // Set the done bit so we can tell the sound is not playing
    waveHeader.dwFlags = WHDR_DONE;

    Result = TRUE;

HANDLE_ERROR:;

    if( !Result )
        Free();

    if( hWaveFile )
        mmioClose( hWaveFile, 0 );

    return Result;

}   // LoadFromFile


/************************************************************************
*    FUNCTION NAME:         LoadFromResource                                                             
*
*    DESCRIPTION:           Load from a resource. This function expects
*                           PCM version of the wave file which is the wav
*                           format and data is stripped from a standard
*                           PCM format wav file.
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName - File to load
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

BOOL CWaveLoad::LoadFromResource( HINSTANCE hInst, char *name )
{
    BOOL Result = FALSE;
    TLoadRes LoadRes;

    // We always assume the resource type is a WAV
    if( LoadTheResource( hInst, name, "WAV", &LoadRes ) )
    {
        // Check that we have the data
        if( LoadRes.Size && LoadRes.pData )
        {
            // Copy over the wave format
            memcpy( &WaveFormat, LoadRes.pData, sizeof( WAVEFORMATEX ) );

            // Fill in the wav header
            // The wav data is right behind the format data
            waveHeader.lpData = ((char *)LoadRes.pData + sizeof( WAVEFORMATEX ) );
            waveHeader.dwBufferLength = LoadRes.Size - sizeof( WAVEFORMATEX );
            // Set the done bit so we can tell the sound is not playing
            waveHeader.dwFlags = WHDR_DONE;

            // Set the flag that indicates this was loaded from a
            // resource and does not need to be freed
            fResLoad = TRUE;

            Result = TRUE;
        }
    }

    return Result;

}   // LoadFromResource


/************************************************************************
*    FUNCTION NAME:         Free                                                             
*
*    DESCRIPTION:           Free all data
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

void CWaveLoad::Free()
{
    if( waveHeader.lpData )
    {
        // Only delete if this was not loaded from a resource
        if( !fResLoad )
            delete [] waveHeader.lpData;

        // zero out the wav format settings
        memset( &WaveFormat, 0, sizeof( WAVEFORMATEX ) );

        // zero out the wav header
        memset( &waveHeader, 0, sizeof( WAVEHDR ) );

        // Reset the flag
        fResLoad = FALSE;
    }   

}   // Free