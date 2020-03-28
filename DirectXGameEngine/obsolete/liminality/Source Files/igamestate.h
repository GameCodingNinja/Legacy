
/************************************************************************
*    FILE NAME:       igamestate.h
*
*    DESCRIPTION:     gamestate interface Class
************************************************************************/

#ifndef __game_state_h__
#define __game_state_h__

enum EGameState
{
    EGS_NULL,
    EGS_STARTUP,
    EGS_RUN
};

class iGameState
{
public:

    // Constructor
    iGameState(){};

    // Destructor
    virtual ~iGameState(){};

    // Act upon what the user is doing
    virtual void GetUserInput(){};

    // Check for collision and react to it
    virtual void ReactToCollision(){};

    // Update objects that require them
    virtual void Update(){};

    // Do the rendering
    virtual void PreRender(){};
    virtual void PostRender(){};

    // Is the state done
    virtual bool DoStateChange(){ return false; };

    // Get the game state
    virtual EGameState GetState(){ return EGS_NULL; }

};

#endif  // __game_state_h__


