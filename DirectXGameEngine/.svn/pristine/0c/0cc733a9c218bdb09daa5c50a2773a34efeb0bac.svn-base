
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

        // Unload the object data
        stateMessage.groupUnload.push_back("(title_screen)");

        // Load the object data
        stateMessage.groupLoad.push_back("(actors)");
        stateMessage.groupLoad.push_back("(space)");

        // Create the actors
        stateMessage.actorDataLoad = "data/objects/2d/actor/actorDataList.lst";
        stateMessage.createActor.push_back("player_ship");
        stateMessage.createActor.push_back("green_enemy_ship");
        stateMessage.createActorVec.push_back("player_projectiles");

        // Create the Mega Texture
        stateMessage.createMegaTexture.push_back( boost::tuple<std::string, int>("(actors)",1024) );
        stateMessage.createMegaTexture.push_back( boost::tuple<std::string, int>("(space)",2048) );

        // Creat the instance
        stateMessage.createInstanceMesh.push_back( "(actors)" );
        stateMessage.createInstanceMesh.push_back( "(space)" );

        // generator load path
        stateMessage.generatorLoadPath = "data/objects/2d/generators/generator.lst";

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







