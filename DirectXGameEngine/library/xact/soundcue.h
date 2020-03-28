/************************************************************************
*    FILE NAME:       soundcue.h
*
*    DESCRIPTION:     Sound cue management class
************************************************************************/

#ifndef __sound_cue_h__
#define __sound_cue_h__

// DirectX lib dependencies
#include <xact3.h>

class CSoundCue
{
public:

    /************************************************************************                                                             
    *    desc:  constructor
    ************************************************************************/
    CSoundCue() : pSoundBank(NULL), cueIndex(XACTINDEX_INVALID), pCue(NULL)
    {
    }
 
    /************************************************************************                                                             
    *    desc:  Copy constructor
    ************************************************************************/
    CSoundCue( const CSoundCue & obj )
    {
        *this = obj;
    }

    /************************************************************************                                                             
    *    desc:  Play a sound
    ************************************************************************/
    void Play()
    {
        if( (pSoundBank != NULL) && (cueIndex != XACTINDEX_INVALID) )
        {
            pSoundBank->Play( cueIndex, 0, 0, &pCue );
        }
    }

    /************************************************************************                                                             
    *    desc:  stop a sound
    ************************************************************************/
    void Stop()
    {
        if( (pSoundBank != NULL) && (cueIndex != XACTINDEX_INVALID) )
        {
            pSoundBank->Stop( cueIndex, 0 );
        }
    }

    /************************************************************************                                                             
    *    desc:  Prepare a sound
    ************************************************************************/
    void Prepare()
    {
        if( (pSoundBank != NULL) && (cueIndex != XACTINDEX_INVALID) )
        {
            pSoundBank->Prepare( cueIndex, 0, 0, &pCue );
        }
    }

    /************************************************************************                                                             
    *    desc:  Check if a cue is busy
    *
    *    ret: bool - true if busy
    ************************************************************************/
    bool IsBusy()
    {
        DWORD cueState = GetState();
        
        return ((cueState > XACT_STATE_CREATED) &&
                (cueState < XACT_STATE_STOPPED));
    }

    /************************************************************************                                                             
    *    desc:  Check if a cue is stopped
    *
    *    ret: bool - true if stopped
    ************************************************************************/
    bool IsStopped()
    {
        DWORD cueState = GetState();
        
        return (cueState == XACT_STATE_STOPPED);
    }

    /************************************************************************                                                             
    *    desc:  Check if a cue is paused
    *
    *    ret: bool - true if paused
    ************************************************************************/
    bool IsPaused()
    {
        DWORD cueState = GetState();
        
        return (cueState == XACT_STATE_PAUSED);
    }

    /************************************************************************                                                             
    *    desc:  Get the state of the sound
    *
    *    ret: DWORD - state value of cue
    ************************************************************************/
    DWORD GetState()
    {
        DWORD cueState(0);

        if( pCue != NULL )
        {
            pCue->GetState( &cueState );
        }

        return cueState;
    }

    // Pointer to sound bank
    IXACT3SoundBank * pSoundBank;

    // Index of sound cue
    XACTINDEX cueIndex;

    // Cue instance pointer
    IXACT3Cue * pCue;
};


#endif  // __sound_cue_h__
