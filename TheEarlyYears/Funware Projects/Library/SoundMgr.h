
/************************************************************************
*
*    PRODUCT:         Sound Manager
*
*    FILE NAME:       SoundMgr.h
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


#ifndef __SOUNDMGR_H__
#define __SOUNDMGR_H__

#include "TWave.h"           // Header file for low level loading and fast playing of wav files
#include "TIntList.h"        // List object for storing lists of ints


class CSoundMgr
{
protected:
    // Number of loaded sound files
    int loadCount;
    // Number of allocated sound devices
    int devCount;
    // Wave load pointer
    PCWaveLoad pWavLoad;
    // Device Pointer
    PCWaveDevice pWavDev;
    // integer list classes
    TIntList intList;
    // Sound Cycle counter
    int cycleCount;

public:
    // Constructor
    CSoundMgr();
    // Destructor
    ~CSoundMgr();
    // Load a sound from the resource
    BOOL LoadFromResource( HINSTANCE hInst, char *name, int index );
    // Load a sound from the resource
    BOOL LoadFromFile( char *filePath, int index );
    // Play the sound file
    BOOL Play( int SoundID = 0, DWORD loopCount = 0 );
    // Play sounds randomly
    BOOL PlayRandom();
    // Play the sounds in order
    BOOL PlayInOrder();
    // Free the loaded wav files
    void FreeLoadedWav();
    // Free the Allocated devices
    void FreeAllocDev();
    // Free everything
    // Always free the devices first
    void Free(){ FreeAllocDev(); FreeLoadedWav(); };
    // Allocate the number of wave load objects
    BOOL AllocateWavLoad( int Count );
    // Allocate the number of wave device objects.
    BOOL AllocateWavDevices( int Count );
    // Apply the loaded file to an allocated device
    BOOL ApplyLoadToDevice( int loadIndex = 0, int devIndex = 0 );
    // Get error string
    char *GetErrorStr();
    // Did we have a sound error
    BOOL IsSndError();
    // Is the object playing a sound
    BOOL IsMjrPlaying();
    // See if a device if playing
    BOOL IsPlaying( int devIndex = 0 ){ return ( (devIndex < devCount) ? pWavDev[devIndex].IsPlaying() : FALSE ); };
    // Pause the device
    BOOL PauseDev( int devIndex = 0 ){ return ( (devIndex < devCount) ? pWavDev[devIndex].Pause() : FALSE ); };
    // Stop the device from playing
    BOOL Stop( int devIndex = 0 );
    // Pause the device
    BOOL RestartDev( int devIndex = 0 ){ return ( (devIndex < devCount) ? pWavDev[devIndex].Restart() : FALSE ); };
    // Get the index of the sound file loaded
    int GetSoundIndex( int devIndex = 0 ){ return ( (devIndex < devCount) ? pWavDev[devIndex].GetLoadIndex() : -1 ); };
    // Get the next device index to play
    int GetNextDevToPlay(){ return intList.GetInt(cycleCount); };
    // inc to the next sound device
    void IncDevCounter();
    // Check if the sound data has been loaded
    BOOL IsSndLoaded( int loadIndex = 0 ){ return ( (loadIndex < loadCount) ? pWavLoad[loadIndex].IsSndLoaded() : FALSE ); };
};
typedef CSoundMgr *PCSoundMgr;


#endif  /* __SOUNDMGR__ */