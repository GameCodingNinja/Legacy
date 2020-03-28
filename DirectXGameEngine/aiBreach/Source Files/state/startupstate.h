
/************************************************************************
*    FILE NAME:       startupstate.h
*
*    DESCRIPTION:     CStartUp Class State
************************************************************************/

#ifndef __start_up_state_h__
#define __start_up_state_h__

// Standard lib dependencies
#include "igamestate.h"

// Game lib dependencies
#include <2d/spritegroup2d.h>
#include <utilities/timer.h>
#include <utilities/thread.h>

class CStartUpState : public iGameState
{
public:

    // Constructor
    CStartUpState();

    // Destructor
    virtual ~CStartUpState();

    // Load during the startup screen
    DWORD Load();

    // Update objects that require them
    void Update();

    // Transform the game objects
    void Transform();

    // 2D/3D Render of game content
    void PreRender();

    // Is the state done
    bool DoStateChange();

private:

    // Powered by waffles logo
    CSpriteGroup2D poweredByWaffles;

    // Setup a timer so that the "Powered by Waffles" logo displays for a minimum amount of time
    CTimer loadDelayTimer;

    // Load thread
    thread::CThread<CStartUpState> loadThread;

    // Exception info
    std::string exceptionTitleStr;
    std::string exceptionInfo;

};

#endif  // __start_up_state_h__


