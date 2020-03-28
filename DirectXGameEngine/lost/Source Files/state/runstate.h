
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
#include <boost/ptr_container/ptr_vector.hpp>

// Game lib dependencies
#include <utilities/timer.h>
#include <gui/controlevent.h>

// Forward declaration(s)
class CActorSprite2D;
class CSpriteGroup2D;

class CRunState : public CCommonState
{
public:

    // Constructor
    CRunState();

    // Destructor
    virtual ~CRunState();

    // Check for collision and react to it
    void ReactToCollision();

    // Update objects that require them
    void Update();

    // Transform the game objects
    void Transform();

    // 2D/3D Render of game content
    void PreRender();

    // Is the state done
    bool DoStateChange();

protected:

    // Act upon what the user is doing
    void HandleGameInput();

    // Handle the control action event
    void HandleControlActionEvent( CUIControl * pControl );

private:

    // Actor sprite of the player
    CActorSprite2D * pPlayer;

    // A test of the instance mesh
    //CInstanceMesh2D instanceMeshTest;
    boost::ptr_vector<CSpriteGroup2D> pGroupTestVector;

    // Needed publish/subscriber call backs
    CSubscriber<CControlActionEvent, CControlActionEvent::CallbackType> actionEventSubscriber;

    // Flag to indicate it's time for a state change
    bool changeState;

};

#endif  // __run_up_state_h__


