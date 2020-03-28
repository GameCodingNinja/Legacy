
/************************************************************************
*    FILE NAME:       xact.h
*
*    DESCRIPTION:     DirectX sound class
************************************************************************/

#ifndef __xact_h__
#define __xact_h__

// Physical component dependency
#include <xact3.h>
#include <xact3d3.h>

// Standard lib dependencies
#include <string>
#include <map>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Game lib dependencies
#include <xact/soundcue.h>
#include <xact/xactdefs.h>
#include <common/point.h>

class CXAct
{
public:

    // Get the instance of the singleton class
    static CXAct & Instance()
    {
        static CXAct xact;
        return xact;
    }

    // Init the xact audio system with the global settings file
    void Init( const std::string & filePath );

    // Load a wav bank
    void LoadWaveBank( const std::string & filePath );

    // Destroy a wav bank
    void DestroyWaveBank( const std::string & filePath );

    // Load a sound bank
    void LoadSoundBank( const std::string & filePath );

    // Build the sound Que map
    void BuildSoundCueMap();

    // Destroy a sound bank
    void DestroySoundBank( const std::string & filePath );

    // Allows XACT to do required periodic work. call often
    virtual void Update();

    // Play a sound
    void Play( const std::string & sndCueStr );

    // Stop a sound
    void Stop( const std::string & sndCueStr );

    // Prepare a sound
    void Prepare( const std::string & sndCueStr );

    // Check if a cue is busy
    bool IsBusy( const std::string & sndCueStr );

    // Check if a cue is stopped
    bool IsStopped( const std::string & sndCueStr );

    // Check if a cue is paused
    bool IsPaused( const std::string & sndCueStr );

    // Pause the play of a catagory
    void Pause( const std::string & catagoryStr, bool paused = true );

    // Set the volume level of a catagory
    void SetVolumeLevel( const std::string & catagoryStr, float level );

    // Position the sound cue based on the point
    void PositionCue( const CPoint & point, IXACT3Cue * pCue );

protected:

    // Get a sound cue
    CSoundCue * GetSoundCue( const std::string & sndCueStr );

private:

    // Constructor
    CXAct();

    // Destructor
    ~CXAct();

    // notification call back function -  needs to be static to be passed as function pointer
    static void WINAPI XACTNotificationCallback( const XACT_NOTIFICATION * pNotification );

    // Function for handling XACT notifications
    void HandleNotification( const XACT_NOTIFICATION * pNotification );

    // Display error information
    void DisplayError( HRESULT hr );

    // Init the class member variables for 3D sound
    void Init3DSound();

private:

    // XACT audio engine
    IXACT3Engine* pEngine;

    // 3D audio instance
    X3DAUDIO_HANDLE x3DInstance;

    // map for wave bank
    boost::ptr_map<const std::string, CWaveBank> spWaveBankMap;
    boost::ptr_map<const std::string, CWaveBank>::iterator wavBankIter;

    // map for sound bank
    boost::ptr_map<const std::string, CSoundBank> spSoundBankMap;
    boost::ptr_map<const std::string, CSoundBank>::iterator sndBankIter;

    // Global sound cue map
    std::map<const std::string, CSoundCue> soundCueMap;
    std::map<const std::string, CSoundCue>::iterator soundCueIter;

    // 3D sound data members
    X3DAUDIO_DSP_SETTINGS dspSettings;
    X3DAUDIO_LISTENER listener;
    X3DAUDIO_EMITTER emitter;
    FLOAT32 delayTimes[2];
    FLOAT32 matrixCoefficients[2 * 8];

};

#endif  // __xact_h__
