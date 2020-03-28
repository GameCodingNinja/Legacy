
/************************************************************************
*    FILE NAME:       runstate.h
*
*    DESCRIPTION:     General gameplay state of the game
************************************************************************/

#ifndef __run_up_state_h__
#define __run_up_state_h__

// Physical component dependency
#include "commonstate.h"

// Boost lib dependencies
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

// Game lib dependencies
#include <2d/actorsprite2d.h>
#include <2d/spritegroup2d.h>
#include <gui/controlevent.h>

class CRunState : public CCommonState
{
public:

    // Constructor
    CRunState();

    // Destructor
    virtual ~CRunState();

    // Update objects that require them
    void Update();

    // Transform game objects
    void Transform();

    // Do the rendering
    void PreRender();

    // Is the state done
    bool DoStateChange();

protected:

    // Act upon what the user is doing
    virtual void HandleGameInput();

    // Handle the player based on the input
    //void HandlePlayer( int input );

    // Handle the control action event
    void HandleControlActionEvent( CUIControl * pControl );

private:

    // Needed publish/subscriber call backs
    CSubscriber<CControlActionEvent, CControlActionEvent::CallbackType> actionEventSubscriber;

    // Flag to indicate it's time for a state change
    bool changeState;

};

#endif  // __run_up_state_h__


