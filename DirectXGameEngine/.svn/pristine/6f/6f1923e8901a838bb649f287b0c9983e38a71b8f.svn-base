/************************************************************************
*    FILE NAME:       soundscriptdefs.h
*
*    DESCRIPTION:     Header file that contains the sound class scripts
************************************************************************/  

#ifndef __sound_script_defs_h__
#define __sound_script_defs_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <scripting/script.h>
#include <scripting/scriptdefs.h>
#include <scripting/scriptobject.h>
#include <xact/xact.h>

using namespace std;

// Script class used to play a sound in a script
class CScriptPlaySound : public CScript
{
public:

    CScriptPlaySound(){ type = EST_PLAY_SOUND; }
    CScriptPlaySound( CScript * pCopy )
    {
        soundCue = *pCopy->GetSound();
        type = EST_PLAY_SOUND;
    }
    CScriptPlaySound( CScript & copy )
    {
        soundCue = *copy.GetSound();
        type = EST_PLAY_SOUND;
    }

    // The sound we wish to play
    string soundCue;

    // Set and get functions for the sound cue
    virtual void SetSound(string & sound){soundCue = sound;}
    virtual string * GetSound(){return &soundCue;}

    // Update function
    bool Update( CScriptObject * pScriptObj )
    {
        if(!finished)
        {
            CXAct::Instance().Play( soundCue );
            finished = true;
        }

        return finished;
    }

};


// Script class used to stop a sound in a script
class CScriptStopSound : public CScript
{
public:

    CScriptStopSound(){ type = EST_STOP_SOUND; }
    CScriptStopSound( CScript * pCopy )
    {
        soundCue = *pCopy->GetSound();
        type = EST_STOP_SOUND;
    }
    CScriptStopSound( CScript & copy )
    {
        soundCue = *copy.GetSound();
        type = EST_STOP_SOUND;
    }

    // The sound we wish to stop
    string soundCue;

    // Set and get functions for the sound cue
    virtual void SetSound(string & sound){soundCue = sound;}
    virtual string * GetSound(){return &soundCue;}

    // Update function
    bool Update( CScriptObject * pScriptObj )
    {
        if(!finished)
        {
            CXAct::Instance().Stop( soundCue );
            finished = true;
        }

        return finished;
    }

};


// Script class used to pause a sound in a script
class CScriptPauseSound : public CScript
{
public:

    CScriptPauseSound(){ type = EST_PAUSE_SOUND; }
    CScriptPauseSound( CScript * pCopy )
    {
        soundCue = *pCopy->GetSound();
        type = EST_PAUSE_SOUND;
    }
    CScriptPauseSound( CScript & copy )
    {
        soundCue = *copy.GetSound();
        type = EST_PAUSE_SOUND;
    }

    // The sound we wish to pause
    string soundCue;

    // Set and get functions for the sound cue
    virtual void SetSound(string & sound){soundCue = sound;}
    virtual string * GetSound(){return &soundCue;}

    // Update function
    bool Update( CScriptObject * pScriptObj )
    {
        if(!finished)
        {
            CXAct::Instance().Pause( soundCue );
            finished = true;
        }

        return finished;
    }

};


// Script class used to unpause a sound in a script
class CScriptUnpauseSound : public CScript
{
public:

    CScriptUnpauseSound(){ type = EST_UNPAUSE_SOUND; }
    CScriptUnpauseSound( CScript * pCopy )
    {
        soundCue = *pCopy->GetSound();
        type = EST_UNPAUSE_SOUND;
    }
    CScriptUnpauseSound( CScript & copy )
    {
        soundCue = *copy.GetSound();
        type = EST_UNPAUSE_SOUND;
    }

    // The sound we wish to pause
    string soundCue;

    // Set and get functions for the sound cue
    virtual void SetSound(string & sound){soundCue = sound;}
    virtual string * GetSound(){return &soundCue;}

    // Update function
    bool Update( CScriptObject * pScriptObj )
    {
        if(!finished)
        {
            CXAct::Instance().Pause( soundCue, false );
            finished = true;
        }

        return finished;
    }

};


#endif  // __sound_script_defs_h__