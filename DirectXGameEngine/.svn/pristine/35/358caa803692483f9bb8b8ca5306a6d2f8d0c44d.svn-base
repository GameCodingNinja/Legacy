
/************************************************************************
*    FILE NAME:       classtemplate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "runstate.h"

// Game lib dependencies
#include <controller/gamecontroller.h>
#include <utilities/highresolutiontimer.h>
#include <managers/actormanager.h>
#include <gui/uimenutree.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CRunState::CRunState()
{
    //CUIMenuTree::Instance().Activate( string("pause_menu") );
    CUIMenuTree::Instance().Activate();

    // Initialize the world manager
    worldManager.Init();

    pPlayer = CActorManager::Instance().GetActor2D( string("player") );
    pPlayer->SetWorldPos( CWorldPoint( CPointInt(0,0,0), CPoint(0,0,0) ) );

}	// Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CRunState::~CRunState()
{
}   // Destructer


/***************************************************************************
*    desc:  Act upon what the user is doing
****************************************************************************/
void CRunState::GetUserInput()
{
    CCommonState::GetUserInput();

}	// GetUserInput


/***************************************************************************
*    desc:  Check for collision and react to it
****************************************************************************/
void CRunState::ReactToCollision()
{
    CActorManager::Instance().ReactToCollision2D();
    //if( CUIMenuTree::Instance().GetState() == NUIMenu::EMTS_HIDDEN )
        //worldManager.ReactToCollision( pPlayer );

}	// ReactToCollision


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CRunState::Update()
{
    CCommonState::Update();

    CActorManager::Instance().Update2D();

    // Update the 2D stage
    worldManager.Update( pPlayer->GetWorldPos().i_pos );

}	// Update


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CRunState::PreRender()
{
    CCommonState::PreRender();

    // Set the camera position to the player's position and translate back 10
    CWorldCamera::Instance().SetWorldPos( pPlayer->GetWorldPos() );
    CWorldCamera::Instance().IncWorldPos( CPoint(0,0,-20) );

    //CActorManager::Instance().Render2D();

    // Render the stage
    worldManager.Render();
    //pPlayer->Render( CWorldCamera::Instance() );

    CActorManager::Instance().Render2D( CWorldCamera::Instance() );

}	// PreRender


/***************************************************************************
*    decs:  Act upon what the user is doing
****************************************************************************/
void CRunState::HandleGameInput()
{
    CCommonState::HandleGameInput();

    CActorManager::Instance().HandleGameInput2D();

    // get the elapsed time
    /*float elapsedTime = CHighResTimer::Instance().GetElapsedTime();

    const float PLAYER_SPEED(0.01f);
    const float KEY_BOARD_SPEED = CSettings::Instance().GetJoyPadStickRange() * PLAYER_SPEED;

    if( CGameController::Instance().WasAction(string("Up")) == EAP_HOLD )
        spPlayerShip->IncPos( CPoint( 0, (KEY_BOARD_SPEED * elapsedTime), 0) );

    if( CGameController::Instance().WasAction(string("Down")) == EAP_HOLD )
        spPlayerShip->IncPos( CPoint( 0, -(KEY_BOARD_SPEED * elapsedTime), 0) );

    if( CGameController::Instance().WasAction(string("Left")) == EAP_HOLD )
        spPlayerShip->IncPos( CPoint( -(KEY_BOARD_SPEED * elapsedTime), 0.0f, 0.0f) );

    if( CGameController::Instance().WasAction(string("Right")) == EAP_HOLD )
        spPlayerShip->IncPos( CPoint( (KEY_BOARD_SPEED * elapsedTime), 0.0f, 0.0f) );*/

}	// HandleGameInput





