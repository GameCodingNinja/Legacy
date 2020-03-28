
/************************************************************************
*    FILE NAME:       gameloadstate.h
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

#ifndef __game_load_state_h__
#define __game_load_state_h__

// Physical component dependency
#include "igamestate.h"

// Game lib dependencies
#include <2d/spritegroup2d.h>
#include <utilities/timer.h>
#include <utilities/thread.h>

class CGameLoadState : public iGameState
{
public:

    // Constructor
    explicit CGameLoadState( const CStateMessage & stateMsg );

    // Destructor
    virtual ~CGameLoadState();

    // Load during the loading screen
    DWORD Load();

    // Update objects that require them
    void Update();

    // Transform game objects
    void Transform();

    // Do the rendering
    void PreRender();

    // Is the state done
    bool DoStateChange();

    // Get the next state to load
    EGameState GetNextState();

private:

    // The image animating while things are loading
    CSpriteGroup2D loadingImage;

    // Load thread
    thread::CThread<CGameLoadState> loadThread;

    // Exception info
    std::string exceptionTitleStr;
    std::string exceptionInfo;

};

#endif  // __game_load_state_h__


