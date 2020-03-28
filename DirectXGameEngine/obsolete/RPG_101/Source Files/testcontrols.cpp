/************************************************************************
*    FILE NAME:       testcontrols.cpp
*
*    DESCRIPTION:     test controls class
************************************************************************/

// Physical component dependency
#include "testcontrols.h"

// Game lib dependencies
#include "3d\\highresolutiontimer.h"
#include "genfunc.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CTestControls::CTestControls()
{
    CGameController::Instance().AddToPollLst( KEYBOARD );
    CGameController::Instance().AddToPollLst( MOUSE );
    CGameController::Instance().AddToPollLst( JOYPAD );

    CGameController::Instance().BindActionToEvent( string("up"), KEY_W );
    CGameController::Instance().BindActionToEvent( string("left"), KEY_A );
    CGameController::Instance().BindActionToEvent( string("down"), KEY_S );
    CGameController::Instance().BindActionToEvent( string("right"), KEY_D );

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CTestControls::~CTestControls()
{
}   // Destructer


/************************************************************************
*    desc:  Gets the user's input                                                             
************************************************************************/
void CTestControls::GetUserInput( CCamera & camera )
{
    // get the elapsed time
    float elapsedTime = CHighResTimer::Instance().GetElapsedTime();
    float fps = CHighResTimer::Instance().GetFPS();

    const float MOUSE_MOVEMENT(0.0004f);
    const float GAMEPAD_MOVEMENT_A2(0.003f);
    const float DIRECTIONAL_MOVEMENT(0.08f);
    const float DIAGNAL_MOVEMENT(0.0353553f);

    // Reset the button states
    buttonUpDown = 0;
    buttonLeftRight = 0;

    // Poll the active devices
    //CGameController::Instance().Poll();

    CDeviceMovement devMovement = CGameController::Instance().GetMovement();

    if( CGameController::Instance().WasAction(string("up")) && CGameController::Instance().WasAction(string("left")) )
    {
        buttonUpDown = -1;
        buttonLeftRight = 1;

        camera.Translate( CPoint( (DIAGNAL_MOVEMENT * elapsedTime), 0.0f, -(DIAGNAL_MOVEMENT * elapsedTime)) );
    }

    else if( CGameController::Instance().WasAction(string("up")) && CGameController::Instance().WasAction(string("right")) )
    {
        buttonUpDown = -1;
        buttonLeftRight = -1;

        camera.Translate( CPoint( -(DIAGNAL_MOVEMENT * elapsedTime), 0.0f, -(DIAGNAL_MOVEMENT * elapsedTime)) );
    }

    else if( CGameController::Instance().WasAction(string("down")) && CGameController::Instance().WasAction(string("left")) )
    {
        buttonUpDown = 1;
        buttonLeftRight = 1;

        camera.Translate( CPoint( (DIAGNAL_MOVEMENT * elapsedTime), 0.0f, (DIAGNAL_MOVEMENT * elapsedTime)) );
    }

    else if( CGameController::Instance().WasAction(string("down")) && CGameController::Instance().WasAction(string("right")) )
    {
        buttonUpDown = 1;
        buttonLeftRight = -1;

        camera.Translate( CPoint( -(DIAGNAL_MOVEMENT * elapsedTime), 0.0f, (DIAGNAL_MOVEMENT * elapsedTime)) );
    }

    else if( CGameController::Instance().WasAction(string("up")) )
    {
        buttonUpDown = -1;

        camera.Translate( CPoint( 0.0f, 0.0f, -(DIRECTIONAL_MOVEMENT * elapsedTime)) );
    }

    else if( CGameController::Instance().WasAction(string("down")) )
    {
        buttonUpDown = 1;

        camera.Translate( CPoint( 0.0f, 0.0f, (DIRECTIONAL_MOVEMENT * elapsedTime)) );
    }

    else if( CGameController::Instance().WasAction(string("left")) )
    {
        buttonLeftRight = 1;

        camera.Translate( CPoint( (DIRECTIONAL_MOVEMENT * elapsedTime), 0.0f, 0.0f) );
    }

    else if( CGameController::Instance().WasAction(string("right")) )
    {
        buttonLeftRight = -1;

        camera.Translate( CPoint( -(DIRECTIONAL_MOVEMENT * elapsedTime), 0.0f, 0.0f) );
    }

    // PostDebugMsg( "%f", (float)devMovement.gamepad1Y );

    if( devMovement.gamepad1Y != 0 )
    {
        camera.Translate( CPoint( 0.0f, 0.0f, (((float)devMovement.gamepad1Y * 0.002f) * elapsedTime)) );
    }

    if( devMovement.gamepad1X != 0 )
    {
        camera.Translate( CPoint( -(((float)devMovement.gamepad1X * 0.002f) * elapsedTime), 0.0f, 0.0f) );
    }


    // Add gravity
    camera.Translate( CPoint( 0.0f, 0.05f * elapsedTime, 0.0f) );

    // Get the mouse X & Y position
    POINT mousePt;
    GetCursorPos( &mousePt );

    // Reset the cursor positions
    //SetCursorPos( mouseCenterPt.x, mouseCenterPt.y );

    // Do the Y rotation. We are using the difference between the center and the new offsets
    //camera.Rotate( CPoint( ((float)mouseCenterPt.y - (float)mousePt.y) * MOUSE_MOVEMENT, 
                   //((float)mouseCenterPt.x - (float)mousePt.x) * MOUSE_MOVEMENT, 0) );

    if( devMovement.mouseY != 0 || devMovement.mouseX != 0 )
    {
        float speed = fps * MOUSE_MOVEMENT * elapsedTime ;
        camera.Rotate( CPoint( -((float)devMovement.mouseY * speed), 
                               -((float)devMovement.mouseX * speed), 0) );
    }

    if( devMovement.gamepad2Y != 0 || devMovement.gamepad2X != 0 )
    {
        float speed = GAMEPAD_MOVEMENT_A2 * elapsedTime ;
        camera.Rotate( CPoint( -((float)devMovement.gamepad2Y * speed), 
                               -((float)devMovement.gamepad2X * speed), 0) );

        //PostDebugMsg( "%f", (float)devMovement.gamepad2Y );
    }

}	// GetUserInput