
/************************************************************************
*    FILE NAME:       classtemplate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "runstate.h"

// Boost lib dependencies
#include <boost/bind.hpp>

// Game lib dependencies
#include <controller/gamecontroller.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/statcounter.h>
#include <managers/actormanager.h>
#include <managers/hudmanager.h>
#include <managers/instancemeshmanager.h>
#include <managers/actormanager.h>
#include <managers/generatormanager.h>
#include <managers/physicsworldmanager.h>
#include <gui/uimenutree.h>
#include <common/pointint.h>

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CRunState::CRunState()
         : pPlayer( CActorManager::Instance().GetActor2D( "player_ship" ) ),
           actionEventSubscriber( boost::bind(&CRunState::HandleControlActionEvent, this, _1) ),
           changeState(false)
{
    // Set the game state for this object
    gameState = EGS_RUN;

    // Turn on the subscriber
    actionEventSubscriber.Subscribe();

    // Set the orthographic camera so that the actors can be seen
    CWorldCamera::Instance().SetPos( CPoint(0,0,-50) );

}	// Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CRunState::~CRunState()
{
    actionEventSubscriber.Unsubscribe();

}   // Destructer


/***************************************************************************
*    decs:  Handle the control action event
****************************************************************************/
void CRunState::HandleControlActionEvent( CUIControl * pControl )
{
    if( pControl->GetActionType() == NUIControl::ECAT_BACK_TO_MAIN_MENU )
    {
        stateMessage.Clear();
        stateMessage.nextState = EGS_TITLE_SCREEN;
        stateMessage.lastState = EGS_RUN;
        
        // Unload the object data
        stateMessage.groupUnload.push_back("(actors)");
        stateMessage.stopPhysicsWorld.push_back("game");
        //stateMessage.groupUnload.push_back("(space)");

        // Load the object data
        stateMessage.groupLoad.push_back("(title_screen)");

        nextState = EGS_GAME_LOAD;

        changeState = true;
    }

}	// HandleControlActionEvent


/***************************************************************************
*    desc:  Check for collision and react to it
****************************************************************************/
bool CRunState::DoStateChange()
{
    return changeState && (!CUIMenuTree::Instance().IsVisible());

}	// DoStateChange


/***************************************************************************
*    decs:  Act upon what the user is doing
****************************************************************************/
void CRunState::HandleGameInput()
{
    CCommonState::HandleGameInput();

    if( !CUIMenuTree::Instance().IsVisible() )
        CActorManager::Instance().HandleGameInput2D();
    
}	// HandleGameInput


/***************************************************************************
*    desc:  Check for collision and react to it
****************************************************************************/
void CRunState::ReactToCollision()
{
    CCommonState::ReactToCollision();

    // Update the physics world's timer 
    CPhysicsWorldManager::Instance().GetWorld( "game" )->UpdateStepTimer();

    if( !CUIMenuTree::Instance().IsVisible() )
        CActorManager::Instance().ReactToCollision2D();

}	// ReactToCollision


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CRunState::Update()
{
    CCommonState::Update();

    if( !CUIMenuTree::Instance().IsVisible() )
    {
        CPhysicsWorldManager::Instance().GetWorld("game")->SetActive(true);
        CActorManager::Instance().Update2D();
        CHudManager::Instance().Update();
    }
    else
        CPhysicsWorldManager::Instance().GetWorld("game")->SetActive(false);

    // Display the player's sector position at the top of the window
    CStatCounter::Instance().SetPlayerPos( pPlayer->GetPos().GetIntComponent() );

}	// Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CRunState::Transform()
{
    CCommonState::Transform();

    // Transform the managers
    CActorManager::Instance().Transform();
    CHudManager::Instance().Transform();

    // Perform physics simulation, if possible 
    CPhysicsWorldManager::Instance().GetWorld( "game" )->Step();

}	// Transform


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CRunState::PreRender()
{
    CCommonState::PreRender();

    CWorldCamera::Instance().SetPos( CPoint(0,0,-50) );

    // Render the actors
    CActorManager::Instance().Render2D();

    // Render the hud
    CHudManager::Instance().Render();

    // Render everything in the instance mesh
    CInstanceMeshManager::Instance().Render("(actors)");

    // Clear the render multi map
    CInstanceMeshManager::Instance().ClearRender();

}	// PreRender






