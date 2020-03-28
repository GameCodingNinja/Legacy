
/************************************************************************
*    FILE NAME:       runstate.h
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

#ifndef __run_up_state_h__
#define __run_up_state_h__

// Physical component dependency
#include "commonstate.h"

// Boost lib dependencies
#include <boost/shared_ptr.hpp>

// Game lib dependencies
#include <2d/actorsprite2d.h>

// Game dependencies
#include "worldmanager.h"

// Forward declaration(s)
class CActorSprite2D;

class CRunState : public CCommonState
{
public:

    // Constructor
    CRunState();

    // Destructor
    virtual ~CRunState();

    // Act upon what the user is doing
    void GetUserInput();

    // Check for collision and react to it
    void ReactToCollision();

    // Update objects that require them
    void Update();

    // Do the rendering
    void PreRender();

    // Get the game state
    EGameState GetState(){ return EGS_RUN; }

protected:

    // Act upon what the user is doing
    virtual void HandleGameInput();

private:

    CActorSprite2D * pPlayer;

    CWorldManager worldManager;

};

#endif  // __run_up_state_h__


