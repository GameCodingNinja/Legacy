
/************************************************************************
*    FILE NAME:       igamestate.h
*
*    DESCRIPTION:     gamestate interface Class
************************************************************************/

#ifndef __i_game_state_h__
#define __i_game_state_h__

// STL lib dependencies
#include <vector>
#include <string>

// Boost lib dependencies
#include <boost/noncopyable.hpp>
#include <boost/tuple/tuple.hpp>

enum EGameState
{
    EGS_NULL,
    EGS_STARTUP,
    EGS_TITLE_SCREEN,
    EGS_GAME_LOAD,
    EGS_RUN
};


class CStateMessage
{
public:

    CStateMessage() : nextState(EGS_NULL), lastState(EGS_NULL)
    {}

    std::vector<std::string> groupLoad;
    std::vector<std::string> groupUnload;

    std::vector<std::string> createActor;
    std::vector<std::string> createActorVec;

    std::vector<std::string> startPhysicsWorld;
    std::vector<std::string> stopPhysicsWorld;

    std::vector< boost::tuple<std::string, int> > createMegaTexture;

    std::vector<std::string> createInstanceMesh;

    std::string stage2DLoad;

    std::string hudLoad;

    std::string actorDataLoad;

    std::string generatorLoadPath;

    EGameState nextState;
    EGameState lastState;

    void Clear()
    {
        groupLoad.clear();
        groupUnload.clear();
        startPhysicsWorld.clear();
        stopPhysicsWorld.clear();
        createActor.clear();
        createActorVec.clear();
        createMegaTexture.clear();
        createInstanceMesh.clear();
        stage2DLoad.clear();
        hudLoad.clear();
        actorDataLoad.clear();
        generatorLoadPath.clear();
        
        nextState = EGS_NULL;
        lastState = EGS_NULL;
    }
};


class iGameState : public boost::noncopyable
{
public:

    // Constructor
    iGameState():gameState(EGS_NULL),nextState(EGS_NULL){};

    // Constructor
    iGameState( const CStateMessage & stateMsg ){};

    // Destructor
    virtual ~iGameState(){};

    // Act upon what the user is doing
    virtual void GetUserInput( bool hasFocus ){};

    // Check for collision and react to it
    virtual void ReactToCollision(){};

    // Update objects that require them
    virtual void Update(){};

    // Transform the game objects
    virtual void Transform(){};

    // 2D/3D Render of game content
    virtual void PreRender(){};
    virtual void PostRender(){};

    // Is the state done
    virtual bool DoStateChange()
    { return false; };

    // Get the currect game state
    EGameState GetState()
    { return gameState; }

    // Get the next game state
    virtual EGameState GetNextState()
    { return nextState; }

    // Get the message
    const CStateMessage & GetMessage()
    { return stateMessage; }

protected:

    // Message to send to next state
    CStateMessage stateMessage;

    // This objects current game state value
    EGameState gameState;

    // This objects next game state value
    EGameState nextState;

};

#endif  // __i_game_state_h__


