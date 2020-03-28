
/************************************************************************
*    FILE NAME:       titlescreenstate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "titlescreenstate.h"

// Game lib dependencies
#include <controller/gamecontroller.h>
#include <utilities/highresolutiontimer.h>
#include <managers/actormanager.h>
#include <gui/uimenutree.h>
#include <2d/iaibase.h>
#include <xact/xact.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CTitleScreenState::CTitleScreenState()
{
    // Set the game states for this object
    gameState = EGS_TITLE_SCREEN;
    nextState = EGS_GAME_LOAD;

    CUIMenuTree::Instance().Activate( "title_screen_menu" );

    // Init the background sprite
    background.Init( "(title_screen)", "background" );
    background.SetPos( CPoint(0,0,50) );

}	// Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CTitleScreenState::~CTitleScreenState()
{
}   // Destructer


/***************************************************************************
*    desc:  Is the state done
****************************************************************************/
bool CTitleScreenState::DoStateChange()
{
    if( CUIMenuTree::Instance().GetState() == NUIMenu::EMTS_HIDDEN )
    {
        stateMessage.Clear();
        stateMessage.nextState = EGS_RUN;
        stateMessage.lastState = EGS_TITLE_SCREEN;

        stateMessage.group2DUnload.push_back("(title_screen)");

        stateMessage.group2DLoad.push_back("(object)");
        stateMessage.group2DLoad.push_back("(hud)");

        stateMessage.group3DLoad.push_back("(object)");

        stateMessage.createActor.push_back("player_ship");

        stateMessage.hudLoad = "objects";

        stateMessage.stage2DLoad = "test2D";
        stateMessage.stage3DLoad = "test3D";

        return true;
    }

    return false;

}	// DoStateChange */


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CTitleScreenState::Update()
{
    CCommonState::Update();

}	// Update


/***************************************************************************
*    desc:  Transform game objects
****************************************************************************/
void CTitleScreenState::Transform()
{
    CCommonState::Transform();

    background.Transform();

}	// Transform */


/***************************************************************************
*    desc:  2D/3D Render of game content
****************************************************************************/
void CTitleScreenState::PreRender()
{
    CCommonState::PreRender();

    background.Render();

}	// Render2D







