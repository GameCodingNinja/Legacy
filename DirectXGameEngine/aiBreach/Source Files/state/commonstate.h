
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
    virtual void GetUserInput( bool hasFocus );

    // Update objects that require them
    virtual void Update();

    // Transform the game objects
    virtual void Transform();

    // Do the rendering
    virtual void PreRender();
    virtual void PostRender();

protected:

    // Act upon what the user is doing
    virtual void HandleGameInput();

private:

    // Timer
    CTimer JoypadCheckTimer;

};

#endif  // __common_state_h__


