/************************************************************************
*    FILE NAME:       gamestate.h
*
*    DESCRIPTION:     high resolution timer class
************************************************************************/

// Standard lib dependencies
//#include <sstream>
//#include <string>

// Game lib dependencies
//#include "genfunc.h"

// Physical component dependency
#include "gamestate.h"

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CGameState::CGameState()
             : current_state(0)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CGameState::~CGameState()
{
}   // Destructer


/************************************************************************
*    desc:  Set the state of the game     
*
*	 param: int state - new state of the game
************************************************************************/
void CGameState::SetGameState( int state )
{
    current_state = state;

}	// SetGameState


/************************************************************************
*    desc:  Get the current state of the game     
*
*	 ret:	int current_state - current state of the game
************************************************************************/
int CGameState::GetGameState()
{
    return current_state;

}	// GetGameState


/************************************************************************
*    desc:  Get the instance of the game's state                                                            
************************************************************************/
CGameState & CGameState::Instance()
{
    static CGameState gameState;

    return gameState;
}