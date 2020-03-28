
/************************************************************************
*    FILE NAME:       runstate.cpp
*
*    DESCRIPTION:     General gameplay state of the game
************************************************************************/

// Physical component dependency
#include "runstate.h"

// Boost lib dependencies
#include <boost/bind.hpp>

// Game lib dependencies
#include <controller/gamecontroller.h>
#include <utilities/highresolutiontimer.h>
#include <managers/actormanager.h>
#include <managers/hudmanager.h>
#include <common/defs.h>
#include <gui/uimenutree.h>
#include <3d/spritegroup3D.h>
#include <3d/visualsprite3d.h>
#include <common/camera.h>
#include <managers/physicsworldmanager.h>
#include <2d/collisionsprite2d.h>

// Required namespace(s)
using namespace std;

enum
{
    E_NO_INPUT = 0,
    E_LEFT = 1,
    E_RIGHT = 2,
    E_UP = 4,
    E_DOWN = 8,
    E_MAX = 15
};

CSpriteGroup3D testSprite;
float rotation = 0;
float rotDir = 1;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CRunState::CRunState()
         : actionEventSubscriber( boost::bind(&CRunState::HandleControlActionEvent, this, _1) ),
           changeState(false)
{
    // Set the game state for this object
    gameState = EGS_RUN;

    // Turn on the subscriber
    actionEventSubscriber.Subscribe();

    testSprite.Init( "(object)", "box" );
    testSprite.SetPos( 0, 100, 0 );
    testSprite.SetScale( 3 );

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

        stateMessage.group2DLoad.push_back("(title_screen)");

        stateMessage.group2DUnload.push_back("(object)");
        stateMessage.group2DUnload.push_back("(hud)");
        stateMessage.group3DUnload.push_back("(object)");

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
*    desc:  Update objects that require them
****************************************************************************/
void CRunState::Update()
{
    CCommonState::Update();

    CActorManager::Instance().Update2D();

    CHudManager::Instance().Update();

    const float ROT_SPEED = 0.02f;
    rotation += ROT_SPEED * CHighResTimer::Instance().GetElapsedTime() * rotDir;

    if( rotation > 20 )
    {
        rotDir = -1;
        rotation = 20;
    }
    else if( rotation < -20 )
    {
        rotDir = 1;
        rotation = -20;
    }

    testSprite.SetRot( 0, rotation, 0);

}	// Update


/***************************************************************************
*    desc:  Transform game objects
****************************************************************************/
void CRunState::Transform()
{
    CCommonState::Transform();

    CActorManager::Instance().Transform();

    CHudManager::Instance().Transform();

}	// Transform */


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CRunState::PreRender()
{
    CCommonState::PreRender();

    testSprite.GetVisualSprite().Render( CCamera::Instance().GetFinalMatrix() );

    CActorManager::Instance().Render2D();

    // Render the hud
    CHudManager::Instance().Render();

}	// PreRender


/***************************************************************************
*    decs:  Act upon what the user is doing
****************************************************************************/
void CRunState::HandleGameInput()
{
    CCommonState::HandleGameInput();

    if( CUIMenuTree::Instance().GetState() == NUIMenu::EMTS_HIDDEN )
    {
        CActorManager::Instance().HandleGameInput2D();

        //HandlePlayer( input );
    }
    
}	// HandleGameInput


/***************************************************************************
*    decs:  Handle the player based on the input
*
*	 param:	int input - input received
****************************************************************************
void CRunState::HandlePlayer( int input )
{
    // get the elapsed time
    float elapsedTime = CHighResTimer::Instance().GetElapsedTime();

    const float ACCELERATION = 0.0007f;
    const float DIAGNAL = 0.70710678118f;
    CPoint acceleration;

    float direction = 1;

    // If the input isn't a power of two, then we're moving diagnaly
    if( ( input & ( input - 1 ) ) != 0 )
        direction = DIAGNAL;

    // Calculate the acceleration vector
    if( input & E_UP )
        acceleration.y = ACCELERATION * direction;
    else if( input & E_DOWN )
        acceleration.y = -ACCELERATION * direction;

    if( input & E_LEFT )
        acceleration.x = -ACCELERATION * direction;
    else if( input & E_RIGHT )
        acceleration.x = ACCELERATION * direction;

    // Incorporate the acceleration into the velocity
    CPoint velocity = spColTestSprite[0].GetCollisionSprite()->GetBody().GetVelocity() + (acceleration * elapsedTime);
    spColTestSprite[0].GetCollisionSprite()->GetBody().SetVelocity( velocity );
    
}	// HandlePlayer */


