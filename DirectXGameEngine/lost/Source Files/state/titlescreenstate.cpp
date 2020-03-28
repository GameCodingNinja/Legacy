
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

        stateMessage.groupUnload.push_back("(title_screen)");

        stateMessage.groupLoad.push_back("(hud)");
        stateMessage.groupLoad.push_back("(space)");
        stateMessage.groupLoad.push_back("(actors)");

        stateMessage.createActor.push_back("player_ship");
        stateMessage.createActorVec.push_back("player_projectiles");

        stateMessage.createMegaTexture.push_back( boost::tuple<std::string, int>("(space)",4096) );
        stateMessage.createMegaTexture.push_back( boost::tuple<std::string, int>("(actors)",1024) );

        stateMessage.createInstanceMesh.push_back( "(actors)" );
        stateMessage.createInstanceMesh.push_back( "(space)" );

        stateMessage.hudLoad = "space";

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
*    desc:  Transform the game objects
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







