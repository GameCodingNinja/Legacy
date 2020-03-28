/************************************************************************
*    FILE NAME:       state.cpp
*
*    DESCRIPTION:     Class for state machine handling
************************************************************************/

// Physical component dependency
#include <utilities/state.h>

// Standard lib dependencies
#include <assert.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  constructer                                                             
************************************************************************/
CState::CState()
      : pState(NULL),
        level(0)
{
}	// constructer


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CState::~CState()
{
}	// destructer


/************************************************************************
*    desc:  Initilize the state class                                                             
************************************************************************/
void CState::Initilize()
{
    assert(pState == NULL && "Persistant state pointer already initilized!");

    // Check that we have not over run the queue
    assert(perState.state.size() == 0 && "Queue used before being initilized!");

    // Add the first state to the queue
    perState.state.push_back(0);

    // Init the pointer to the persistant class
    pState = &perState;

}	// Initilize


/************************************************************************
*    desc:  Returns the current state value
*
*    ret: Returns int of current state
************************************************************************/
CState::operator int() const
{
    // Sanity checks
    assert(pState != NULL && "Persistant state pointer NOT initilized!");
  
    // Check that we have not over run the queue
    assert(level < static_cast<int>(pState->state.size()) && "Level exceeds queue size!");

    // Just return the current states value for this level.
    return pState->state[level];

}	// operator int


/************************************************************************
*    desc:  Set this state's current value.
*
*    ret: Returns int of current state
************************************************************************/
int CState::operator = (int newState)
{
    // Sanity checks
    assert(pState != NULL && "Persistant state pointer NOT initilized!");

    // Check that we have not over run the queue
    assert(level < static_cast<int>(pState->state.size()) && "Level exceeds queue size!");

    // Don't do anything if it's the same state.
    if( pState->state[level] == newState )
        return newState;

    // Change our new state value for this level.
    pState->state[level] = newState;

    // Destroy any access to substates
    pState->level = level;

    // Pop any substates
    if( level+1 < static_cast<int>(pState->state.size()) )
    {
        assert(pState->state.size() != 0 && "Possible run away sub state! Sub state queue 0 before pop!!");
        pState->state.pop_back();
    }

    // Return out new state value.
    return newState;

}	// operator int


/************************************************************************
*    desc:  Create a substate by pushing onto the stack a new state value of zero.
*
*    ret: Returns a new state
************************************************************************/
CState CState::NewSubState() const
{
    // Sanity checks
    assert(pState != NULL && "Persistant state pointer NOT initilized!");

    // See if we need to add a new state
    if( level == pState->level )
    {
        // Increment the current state ptr.
        ++pState->level;

        // Create a new substate.
        pState->state.push_back(0);

        // Guard against a run away sub state queue
        assert(pState->state.size() < 50 && "Possible run away sub state! Sub state queue of 50!!");
    
        // Check that the new level is the last index in the queue
        assert( pState->level+1 == pState->state.size() && "Level and queue size out of sync!!");
    }

    // Create the state object referencing this substate and return it.
    CState state;

    // Equals the current level plus one.
    state.level = level+1;

    // Copy the ptr to nv object.
    state.pState = pState;

    // Reutrn our new substate for use.
    return state;

}	// NewSubState
