
/************************************************************************
*
*    PRODUCT:         Low Level wave file load and play
*
*    FILE NAME:       TWave.h
*
*    DESCRIPTION:     This class provides functions for reading and playing
*                     PCM wave files.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __WAVE_H__
#define __WAVE_H__

#include <MMSystem.H>           // Include file for Multimedia API's

#define TEST_WAV_OPEN_COUNT     3

/************************************************************************
*   CWaveDevice Class
*          
*   Low level wav playing. This provides the most control and the best
*   speed for playing wave files. This class is only designed for playing
*   wave files. Audio devices for playing or recording are different
*   and should be kept seperate.
*
************************************************************************/

class CWaveDevice
{
protected:
    // Handle of output audio device
    HWAVEOUT hWaveOut;
    // Pointer to loaded wave data to play
    WAVEHDR waveHeader;
    // Error string
    char errorStr[100];
    // Structure that holds the capabilities of the sound card.
    // This is static because the information is not specific to
    // any piticular wave devise.
    static WAVEOUTCAPS waveOutCaps;
    // ID identifing the current loaded sound
    int loadIndex;

public:
    CWaveDevice();
    ~CWaveDevice();

    // Send the format info of the loaded wave file
    // to see if the audio device can play the data
    MMRESULT Open( LPWAVEFORMATEX pWaveFormat );
    // Close the opened device
    MMRESULT Close();
    // Stop the device from playing. This resets the play
    // position back to the start.
    MMRESULT Stop();
    // Play the wav data to the device.
    MMRESULT Play( DWORD loopCount = 0 );
    // Pause the play back
    MMRESULT Pause();
    // Restart the play back after a pause
    MMRESULT Restart();
    // Prepare the wave data header to be written
    // to the audio device.
    MMRESULT PrepareHeader();
    // Unprepare the wave data header to be written
    // to the audio device.
    MMRESULT UnprepareHeader();
    // Set the wave header
    MMRESULT SetWaveHeader( LPWAVEHDR pHeader );
    // Get the error string
    char *GetErrorStr(){ return errorStr; };
    // Check if we have had an error
    BOOL IsSndError(){ return errorStr[0]; };
    // See if we are playing a sound
    BOOL IsPlaying();
    // Set the error string
    void SetErrorStr( MMRESULT mmResult );
    // Init the capabilities of the wave device
    void InitWaveDevCaps(){ waveOutGetDevCaps( WAVE_MAPPER, &waveOutCaps, sizeof(WAVEOUTCAPS) ); };
    // Get the capabilities of the wave device
    LPWAVEOUTCAPS GetWaveDevCaps(){ return &waveOutCaps; };
    // Is the device synchronous?
    BOOL IsSynchronous(){ return (waveOutCaps.dwSupport & WAVECAPS_SYNC); };
    // Set the volume of the wav device
    MMRESULT SetVolume( DWORD volume );
    // Set the volume of the wav device
    MMRESULT GetVolume( DWORD &volume );
    // Test to see if we can open multiple devices
    BOOL TestMultiDeviceOpen();
    // Set the load index
    void SetLoadIndex( int index ){ loadIndex = index; };
    // Get the load index
    int GetLoadIndex(){ return loadIndex; };
};

typedef CWaveDevice *PCWaveDevice;


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

class CWaveLoad
{
protected:
    // Wave format structure
    WAVEFORMATEX WaveFormat;
    // The wave header. Needed for playing the wave
    WAVEHDR waveHeader;   
    // Flag that indicates the data is resource data 
    // and should not be freed
    BOOL fResLoad; 

public:
    CWaveLoad();
    ~CWaveLoad();

    // Load a wave file
    BOOL LoadFromFile( char *FileName );
    // Load a sound from the resource
    BOOL LoadFromResource( HINSTANCE hInst, char *name );
    // Get the wave header
    LPWAVEHDR GetHeader(){ return &waveHeader; };
    // Get the wave format data
    LPWAVEFORMATEX GetWavFormat(){ return &WaveFormat; };
    // Check if we have a sound file loaded
    BOOL IsSndLoaded(){ return (BOOL)waveHeader.lpData; };

    // Get PCM format functions
    WORD GetFormat(){ return WaveFormat.wFormatTag; };
    WORD GetBlockAlign(){ return WaveFormat.nBlockAlign; };
    WORD GetChannelCount(){ return WaveFormat.nChannels; };
    WORD GetBitsPerSample(){ return WaveFormat.wBitsPerSample; };
    DWORD GetSamplesPerSecond(){ return WaveFormat.nSamplesPerSec; };
    DWORD GetAverageBytesPerSecond(){ return WaveFormat.nAvgBytesPerSec; };
    DWORD GetDataSize(){ return waveHeader.dwBufferLength; };

    // Free all data
    void Free();
};
typedef CWaveLoad *PCWaveLoad;

#endif  // __WAVE_H__
