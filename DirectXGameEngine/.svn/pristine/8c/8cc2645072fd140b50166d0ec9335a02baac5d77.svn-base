
/************************************************************************
*    FILE NAME:       game.h
*
*    DESCRIPTION:     game class
************************************************************************/

// Game OS dependencies
                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

// Physical component dependency
#include "game.h"

// Standard lib dependencies
#include <iostream>
#include <sstream>
#include <string>

// Game lib dependencies
#include "3d\\xwindow.h"
#include "3d\\shader.h"
#include "3d\\highresolutiontimer.h"
#include "gamestate.h"
#include "3d\\collision.h"
#include "genfunc.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CGame::CGame()
     : buttonUpDown(0),
       buttonLeftRight(0),
       CAMERA_Z_OFFSET(0.0f)
{
    // Init class members

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CGame::~CGame()
{
}   // Destructer


/************************************************************************
*    desc:  Get the instance of the CGameWnd singleton class
*
*    ret:   CGame - reference to singleton class
************************************************************************/
CGame & CGame::Instance()
{
    static CGame game;

    return game;
}


/****************************************************************************
*    desc:  Load game resources here
*
*    ret:   bool - true on success or false on fail
****************************************************************************/
bool CGame::LoadGameObjects()
{
    RECT rect;

    // get the window rect
    GetWindowRect( CXWindow::Instance().GetWndHandle(), &rect );
    
    // Set the center point for the mouse for movement comparison
    mouseCenterPt.x = rect.left + ((rect.right - rect.left) / 2);
    mouseCenterPt.y = rect.top + ((rect.bottom - rect.top) / 2);

    // Hide the mouse
    ShowCursor( false );

    // Set the mouse position to the middle of the window
    SetCursorPos( mouseCenterPt.x, mouseCenterPt.y );

    if( !CGameController::Instance().Init( CXWindow::Instance().GetWndHandle() ) )
    {
        return false;
    }

    CGameController::Instance().AddToPollLst( KEYBOARD );
    CGameController::Instance().AddToPollLst( MOUSE );
    CGameController::Instance().AddToPollLst( JOYPAD );

    CGameController::Instance().BindActionToEvent( string("quit"), KEY_ESCAPE );
    CGameController::Instance().BindActionToEvent( string("quit"), MOUSE_BUTTON_LEFT );
    CGameController::Instance().BindActionToEvent( string("quit"), POV_DEGREE_0 );

    vector<string> pointMatVec;

    pointMatVec.push_back("game_art\\snowFlake.png");

    testps.Init(1, pointMatVec, D3DXCOLOR(0.5f,0.5f,0.5f,1.0f), 2.0f);
    testps.LockVertBuffer();
    testps.SetPointPos( CPoint(-20,0,0), 0 );
    //testps.TransPoint( CPoint(0,50,50), camera.GetFinalMatrix(), 0);
    testps.UnlockVertBuffer();

    if( !testManager.LoadStage( string("xml\\stages\\testStage.xml"), camera ) )
    {
        return false;
    }

    if( !testManager.LoadAnimatedSprites( string("xml\\animatedGameObjects.xml") ) )
    {
        return false;
    }

    testManager.LoadCharacterLists( string("xml\\characterLists\\characterGroupLists.xml") );

    // Setup the object manager
    testManager.SetPlayerCollisionStats( 250.0f, 16.0f, 10.0f );

    CShader::Instance().LoadFromFile( string("shaders\\lights.fx"), string("lights") );
    CShader::Instance().SetActiveShader( string("lights") );
    CShader::Instance().GetActiveShader()->SetTechnique( "environmentShading" );
    CShader::Instance().SetLighting( testManager.GetStage()->GetLightList() );
    CShader::Instance().EnableLighting( true );

    // ambient lighting ON
    CShader::Instance().SetAmbient( 0.1f, 0.1f, 0.1f );
    
    // Set the default control scheme
    pControls = new CTestControls();

    CGameState::Instance().SetGameState( CGameState::E_STATE_ENVIRONMENT );

    // Let it run
    StartGame();
  
    return true;

}   // LoadGameObjects


/****************************************************************************
*    decs:  Delete game resources here
****************************************************************************/
void CGame::DeleteGameObjects()
{
}


/****************************************************************************
*    decs:  Act upon what the user is doing
****************************************************************************/
void CGame::GetUserImput()
{
    // get the elapsed time
    float elapsedTime = CHighResTimer::Instance().GetElapsedTime();

    const float DIAGNAL_MOVEMENT(0.05f);

    // Reset the button states
    buttonUpDown = 0;
    buttonLeftRight = 0;

    // Poll the active devices
    CGameController::Instance().Poll();

    CDeviceMovement devMovement = CGameController::Instance().GetMovement();

    if( HIBYTE( GetAsyncKeyState( VK_ESCAPE ) ) )
    {
        if( IsGameRunning() )
        {
            StopGame();
        }

        PostMessage( CXWindow::Instance().GetWndHandle(), WM_DESTROY, 0, 0 );

        return;    
    }

    // See if we want to escape out
    if( CGameController::Instance().WasAction(string("quit")) )
    {
        if( IsGameRunning() )
        {
            StopGame();
        }

        PostMessage( CXWindow::Instance().GetWndHandle(), WM_DESTROY, 0, 0 );

        return;    
    }

    pControls->GetUserInput( camera );

}   // GetUserImput


/****************************************************************************
*    decs:  Update animations, Move sprites, Check for collision
****************************************************************************/
void CGame::Update()
{
    testManager.Update();
    

}	// Update


/************************************************************************
*    desc:  Check for collision and react to it.
************************************************************************/
void CGame::ReactToCollision()
{
    testManager.ReactToCollision(camera);
}	// ReactToCollision


/****************************************************************************
*    decs:  Render the objects to the screen
****************************************************************************/
void CGame::Render()
{
    // Calc the finialized camera position
    CPoint cameraTrans;
    cameraTrans.z = 40.0f;
    cameraTrans.y = -5.0f;
    camera.Finialize(cameraTrans);

    testManager.Render( camera.GetFinalMatrix() );	

    testps.Render( camera.GetFinalMatrix() );
    
}	// Render


/***************************************************************************
*    decs:  Recover from a device lost
****************************************************************************/
void CGame::HandleDeviceReset( int width, int height )
{
    CShader::Instance().Free();

    CXWindow::Instance().HandleDeviceReset( width, height );

    /* This will need to be done when menus are added to your game
    
    if( width > 0 && height > 0 )
    {
        gameMenu.Scale();
    }*/

    CShader::Instance().LoadFromFile( string("shaders\\lights.fx"), string("lights") );
    CShader::Instance().SetActiveShader( string("lights") );
    CShader::Instance().GetActiveShader()->SetTechnique( "environmentShading" );
    CShader::Instance().SetLighting( testManager.GetStage()->GetLightList() );
    CShader::Instance().EnableLighting( true );
    CShader::Instance().SetAmbient( 0.1f, 0.1f, 0.1f );
}



