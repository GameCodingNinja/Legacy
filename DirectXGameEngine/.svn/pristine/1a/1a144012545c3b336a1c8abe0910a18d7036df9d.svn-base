
/************************************************************************
*    FILE NAME:       runstate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "runstate.h"

// Boost lib dependencies
#include <boost/bind.hpp>

// Game lib dependencies
#include <2d/objectdatalist2d.h>
#include <2d/stage2d.h>
#include <2d/actorsprite2d.h>
#include <2d/spritegroup2d.h>
#include <controller/gamecontroller.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/statcounter.h>
#include <utilities/genfunc.h>
#include <system/xdevice.h>
#include <managers/actormanager.h>
#include <managers/megatexturemanager.h>
#include <managers/instancemeshmanager.h>
#include <managers/hudmanager.h>
#include <gui/uimenutree.h>
#include <gui/uiprogressbar.h>
#include <xact/xact.h>

// Game dependencies
#include "../generators/worldgenerator.h"
#include "../game/savefile.h"

// Required namespace(s)
using namespace std;

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

    // Get a pointer to the player
    pPlayer = CActorManager::Instance().GetActor2D( "player_ship" );
    CInstanceMeshManager::Instance().InitInstanceSprite( "(actors)", pPlayer );

    // Set the player's position
    pPlayer->SetPos( CSaveFile::Instance().GetPlayerPos() );

    // Set the progress bars
    NGenFunc::DynCast<CUIProgressBar *>(CHudManager::Instance().GetHudControl("ship_progress_bar"))->Set( CSaveFile::Instance().GetShipHealth() );
    NGenFunc::DynCast<CUIProgressBar *>(CHudManager::Instance().GetHudControl("shield_progress_bar"))->Set( CSaveFile::Instance().GetShipShields() );
    NGenFunc::DynCast<CUIProgressBar *>(CHudManager::Instance().GetHudControl("fuel_progress_bar"))->Set( CSaveFile::Instance().GetShipFuel() );
    NGenFunc::DynCast<CUIProgressBar *>(CHudManager::Instance().GetHudControl("ammo_tier_1_progress_bar"))->Set( CSaveFile::Instance().GetShipAmmoTier1() );
    NGenFunc::DynCast<CUIProgressBar *>(CHudManager::Instance().GetHudControl("ammo_tier_2_progress_bar"))->Set( CSaveFile::Instance().GetShipAmmoTier2() );
    NGenFunc::DynCast<CUIProgressBar *>(CHudManager::Instance().GetHudControl("ammo_tier_3_progress_bar"))->Set( CSaveFile::Instance().GetShipAmmoTier3() );
    NGenFunc::DynCast<CUIProgressBar *>(CHudManager::Instance().GetHudControl("ammo_tier_4_progress_bar"))->Set( CSaveFile::Instance().GetShipAmmoTier4() );



    
    // Add sprites to the instance mesh
    //CSpriteGroup2D * pTmp;

    // Initialize three different sprites with an instance mesh
    //pTmp = new CSpriteGroup2D();
    //pTmp->InitGroup( CObjectDataList2D::Instance().GetData( "(space)", "enemy_ship" ), true );
    //pTmp->SetPos( CPointInt( 0, 0, 90 ) );
    //pTmp->SetScale( CPoint( 171, 171, 1 ) );
    //pTmp->GetAnimationData().SetCurrentFrame( 5 );
    //pGroupTestVector.push_back( pTmp );
    //CInstanceMeshManager::Instance().InitInstanceSprite( "actors", pTmp );

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

        stateMessage.groupLoad.push_back("(title_screen)");

        stateMessage.groupUnload.push_back("(space)");
        stateMessage.groupUnload.push_back("(actors)");
        stateMessage.groupUnload.push_back("(hud)");

        nextState = EGS_GAME_LOAD;

        changeState = true;
    }

}	// HandleControlActionEvent


/***************************************************************************
*    decs:  Act upon what the user is doing
****************************************************************************/
void CRunState::HandleGameInput()
{
    CCommonState::HandleGameInput();

    if( CUIMenuTree::Instance().GetState() == NUIMenu::EMTS_HIDDEN )
    {
        CActorManager::Instance().HandleGameInput2D();
        CWorldGenerator::Instance().HandleSpaceGeneration();
    }

}	// HandleGameInput */


/***************************************************************************
*    desc:  Check for collision and react to it
****************************************************************************/
bool CRunState::DoStateChange()
{
    return changeState && (!CUIMenuTree::Instance().IsVisible());

}	// DoStateChange */


/***************************************************************************
*    desc:  Check for collision and react to it
****************************************************************************/
void CRunState::ReactToCollision()
{
    CCommonState::ReactToCollision();

    if( !CUIMenuTree::Instance().IsVisible() )
        CActorManager::Instance().ReactToCollision2D();

}	// ReactToCollision */


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CRunState::Update()
{
    CCommonState::Update();

    if( CUIMenuTree::Instance().GetState() == NUIMenu::EMTS_HIDDEN )
    {
        CActorManager::Instance().Update2D();
        CWorldGenerator::Instance().Update();
    }

    CHudManager::Instance().Update();

    // Display the player's sector position at the top of the window
    CStatCounter::Instance().SetPlayerPos( pPlayer->GetPos().GetIntComponent() );

    // Save the player position
    CSaveFile::Instance().SetPlayerPos( pPlayer->GetPos() );

}	// Update */


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CRunState::Transform()
{
    CCommonState::Transform();

    CHudManager::Instance().Transform();

    CWorldGenerator::Instance().Transform();
    CActorManager::Instance().Transform();

}	// Transform */


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CRunState::PreRender()
{
    CCommonState::PreRender();

    // Set the camera position to the player's position and translate back a sector and a half
    CWorldCamera::Instance().SetPos( pPlayer->GetPos() );
    CWorldCamera::Instance().IncPos( CWorldPoint(CPointInt(0,0,-1),CPoint(0,0,-512) ) );

    // Render the world
    CWorldGenerator::Instance().Render();

    // Render the contents of the mega texture (for test purposes only)
    //CMegaTextureManager::Instance().Render( "(space)" );

    // All the groups in this vector are in the instance mesh. Even though the sprites'
    // render function doesn't actually render anything, it's still transforms the
    // sprite and places it into the instance mesh
    //for( size_t i = 0; i < pGroupTestVector.size(); ++i )
        //pGroupTestVector[i].Render();

    // Render the actors
    CActorManager::Instance().Render2D();

    // Render everything in the instance mesh
    CInstanceMeshManager::Instance().Render( "(space)" );
    CInstanceMeshManager::Instance().Render( "(actors)" );

    // Clear the render vectors
    CInstanceMeshManager::Instance().ClearRender();

    // Render the hud
    CHudManager::Instance().Render();

}	// PreRender */






