
/************************************************************************
*    FILE NAME:       playershipai.cpp
*
*    DESCRIPTION:     Class player ship AI
************************************************************************/

// Physical component dependency
#include "playershipai.h"

// Game lib dependencies
#include <2d/actorsprite2d.h>
#include <utilities/highresolutiontimer.h>
#include <controller/gamecontroller.h>
#include <managers/actormanager.h>
#include <xact/xact.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)
// disable warning about the use of the "this" pointer in the constructor list
#pragma warning(disable : 4355)

/************************************************************************
*    desc:  Constructer
************************************************************************/
CPlayerShipAI::CPlayerShipAI( CActorSprite2D * _pActor )
             : iAIBase(_pActor)
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CPlayerShipAI::~CPlayerShipAI()
{
}	// destructer


/************************************************************************
*    desc:  Update                                                             
************************************************************************/
void CPlayerShipAI::HandleGameInput()
{
    /*
    // get the elapsed time
    float elapsedTime = CHighResTimer::Instance().GetElapsedTime();

    const float PLAYER_SPEED(0.01f);
    const float KEY_BOARD_SPEED = CSettings::Instance().GetJoyPadStickRange() * PLAYER_SPEED;
    bool playerMoved(false);
    
    // Player Movement
    if( CGameController::Instance().WasAction("Up") == EAP_HOLD )
    {
        pActor->IncPos( CPoint( 0, (KEY_BOARD_SPEED * elapsedTime), 0) );
        playerMoved = true;
    }

    if( CGameController::Instance().WasAction("Down") == EAP_HOLD )
    {
        pActor->IncPos( CPoint( 0, -(KEY_BOARD_SPEED * elapsedTime), 0) );
        playerMoved = true;
    }

    if( CGameController::Instance().WasAction("Left") == EAP_HOLD )
    {
        pActor->IncPos( CPoint( -(KEY_BOARD_SPEED * elapsedTime), 0.0f, 0.0f) );
        playerMoved = true;
    }

    if( CGameController::Instance().WasAction("Right") == EAP_HOLD )
    {
        pActor->IncPos( CPoint( (KEY_BOARD_SPEED * elapsedTime), 0.0f, 0.0f) );
        playerMoved = true;
    }
    */
    /*if( !playerMoved )
    {
        CDeviceMovement &devMovement = CGameController::Instance().GetMovement();

        if( devMovement.gamepad1Y != 0 || devMovement.gamepad1X != 0 )
        {
            const float speed = PLAYER_SPEED * elapsedTime;
            pActor->IncPos( CPoint( ((float)devMovement.gamepad1X * speed), -((float)devMovement.gamepad1Y * speed), 0) );
        }
    }*/

}	// HandleGameInput






