/************************************************************************
*    FILE NAME:       gamestate.h
*
*    DESCRIPTION:     Game state singleton
************************************************************************/

#ifndef __gamestate_h__
#define __gamestate_h__

// Game OS dependencies
//#include <windows.h>

class CGameState
{
public:

    // get the instance
    static CGameState & Instance();

    // Get the state of the game
    int GetGameState();

    // Set the state of the game
    void SetGameState( int state );

    // States the game can be in
    enum{E_STATE_ENVIRONMENT, E_STATE_BATTLE, E_STATE_MENU, E_STATE_CUTSCENE};

private:

    // Constructor
    CGameState();

    // Destructor
    ~CGameState();

    // Current state of the game
    int current_state;
};

#endif  // __gamestate_h__