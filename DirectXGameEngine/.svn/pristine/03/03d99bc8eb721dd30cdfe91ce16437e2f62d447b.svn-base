
/************************************************************************
*    FILE NAME:       commonstate.h
*
*    DESCRIPTION:     CCommonState Class State
************************************************************************/

#ifndef __common_state_h__
#define __common_state_h__

// Physical component dependency
#include "igamestate.h"

// Game lib dependencies
#include <utilities/timer.h>

class CCommonState : public iGameState
{
public:

    // Constructor
    CCommonState();

    // Destructor
    virtual ~CCommonState();

    // Act upon what the user is doing
    virtual void GetUserInput();

    // Update objects that require them
    virtual void Update();

    // Do the rendering
    virtual void PostRender();

protected:

    // Act upon what the user is doing
    virtual void HandleGameInput();

private:

    // Timers
    CTimer JoypadCheckTimer;

};

#endif  // __common_state_h__


