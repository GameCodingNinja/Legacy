
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
#include <gui/uimenutree.h>
#include <common/pointint.h>

// Required namespace(s)
using namespace std;

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
        stateMessage.groupUnload.push_back("(space)");

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
}


/***************************************************************************
*    decs:  Act upon what the user is doing
****************************************************************************/
void CRunState::HandleGameInput()
{
    CCommonState::HandleGameInput();

    if( !CUIMenuTree::Instance().IsVisible() )
    {
        CActorManager::Instance().HandleGameInput2D();

        CGeneratorMgr::Instance().HandleGeneration( CActorManager::Instance().GetActor2D( "player_ship" )->GetPos().GetIntComponent() );
    }
    
}	// HandleGameInput


/***************************************************************************
*    desc:  Check for collision and react to it
****************************************************************************/
void CRunState::ReactToCollision()
{
    CCommonState::ReactToCollision();

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
        CActorManager::Instance().Update2D();
        CGeneratorMgr::Instance().Update();
    }

    CHudManager::Instance().Update();

    // Display the player's sector position at the top of the window
    CStatCounter::Instance().SetPlayerPos( pPlayer->GetPos().GetIntComponent() );

}	// Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CRunState::Transform()
{
    CCommonState::Transform();

    CActorManager::Instance().Transform();
    CGeneratorMgr::Instance().Transform();

    CHudManager::Instance().Transform();

}	// Transform */


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CRunState::PreRender()
{
    CCommonState::PreRender();

    // Set the camera position to the player's position and translate back a sector and a half
    CWorldCamera::Instance().SetPos( pPlayer->GetPos() + CPoint(0,0,-1280) );
    //CWorldCamera::Instance().IncPos( CWorldPoint(CPointInt(0,0,-1),CPoint(0,0,-512) ) );

    // Render the generation
    CGeneratorMgr::Instance().Render();

    // Render the actors
    CActorManager::Instance().Render2D();

    // Render the hud
    CHudManager::Instance().Render();

    // Render everything in the instance mesh
    CInstanceMeshManager::Instance().Render("(space)");
    CInstanceMeshManager::Instance().Render("(actors)");

    // Clear the render multi map
    CInstanceMeshManager::Instance().ClearRender();

}	// PreRender






