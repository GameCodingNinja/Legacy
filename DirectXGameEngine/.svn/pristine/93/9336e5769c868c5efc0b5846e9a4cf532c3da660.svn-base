
/************************************************************************
*    FILE NAME:       startup.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "startupstate.h"

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <managers/actordatalist.h>
#include <managers/shader.h>
#include <managers/hudmanager.h>
#include <managers/vertexbuffermanager2d.h>
#include <2d/fontmanager.h>
#include <2d/objectdatalist2d.h>
#include <scripting/scriptmanager.h>
#include <controller/gamecontroller.h>
#include <xact/xact.h>
#include <system/xdevice.h>
#include <gui/uimenutree.h>
#include <gui/uimenu.h>
#include <gui/uicontrol.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/criticalsection.h>
#include <utilities/genfunc.h>
#include <misc/settings.h>

// Game dependencies
#include "../game/savefile.h"
#include "../game/craftitemlist.h"

// Required namespace(s)
using namespace std;

// disable warning about the use of the "this" pointer in the constructor list
#pragma warning(disable : 4355)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CStartUpState::CStartUpState()
{
    // Set the game states for this object
    gameState = EGS_STARTUP;
    nextState = EGS_TITLE_SCREEN;

    // Force the initialzation of the critical state
    CCriticalSection::Instance();

    // Load shader and it's default values
    CShader::Instance().LoadFromXML( "data/shaders/startup_shader_effects.cfg" );

    // Load the data list tables
    CObjectDataList2D::Instance().LoadListTable( "data/objects/2d/objectDataList/dataListTable.lst" );
    CScriptMgr::Instance().LoadListTable( "data/objects/2d/scripts/scriptListTable.lst" );

    // Load the startup data group
    CObjectDataList2D::Instance().LoadDataGroup("(startup)");

    // Init the "Powered by Waffles" logo and tag it as unique so the mesh can be deleted
    poweredByWaffles.Init( "(startup)", "waffles_logo" );
    poweredByWaffles.SetPos( CPoint(0,0,20) );

    // Setup a timer so that the "Powered by Waffles" logo displays for a minimum amount of time 
    loadDelayTimer.Set(3000);

    // Start the thread
    loadThread.Start( this, &CStartUpState::Load );

}	// Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CStartUpState::~CStartUpState()
{
    // Free all the assets associated with startup
    CObjectDataList2D::Instance().FreeDataGroup( "(startup)" );

}   // Destructer


/***************************************************************************
*    desc:  Load from thread during the startup screen
****************************************************************************/
DWORD CStartUpState::Load()
{
    try
    {
        // Load XAct
        CXAct::Instance().Init( "data/sounds/sounds.xgs" );
        CXAct::Instance().LoadWaveBank( "data/sounds/sound_effects.xwb" );
        CXAct::Instance().LoadSoundBank( "data/sounds/sound_effects.xsb" );
        CXAct::Instance().LoadWaveBank( "data/sounds/menu_sounds.xwb" );
        CXAct::Instance().LoadSoundBank( "data/sounds/menu_sounds.xsb" );
        CXAct::Instance().LoadWaveBank( "data/sounds/music.xwb" );
        CXAct::Instance().LoadSoundBank( "data/sounds/music.xsb" );
        CXAct::Instance().BuildSoundCueMap();

        // Load up the craft item list
        CCraftItemList::Instance().LoadFromXML( "data/game/craftItem.lst" );

        // Load up the actor data list
        CActorDataList::Instance().LoadFromXML( "data/objects/2d/actor/actorDataList.lst" );

        // Load all of the meshes and materials in these groups
        CObjectDataList2D::Instance().LoadDataGroup( "(menu)" );
        CObjectDataList2D::Instance().LoadDataGroup( "(title_screen)" );

        // Init the game controllers
        CGameController::Instance().Init( CXDevice::Instance().GetWndHandle() );

        // add devices to the controller list
        CGameController::Instance().AddToPollLst( NDevice::KEYBOARD );
        CGameController::Instance().AddToPollLst( NDevice::MOUSE );
        CGameController::Instance().AddToPollLst( NDevice::JOYPAD );

        // load in the default controller mappings
        CGameController::Instance().LoadActionFromXML( "data/settings/controllerMapping.cfg" );

        // Load in any fonts
        CFontMgr::Instance().LoadFromXML( "data/materials/fonts/font.lst" );

        // load the hud list table
        CHudManager::Instance().LoadListTable( "data/objects/2d/hud/hudListTable.lst" );

        // load the menu tree
        CUIMenuTree::Instance().LoadFromXML( "data/objects/2d/menus/mainTree.menu" );

        // Get all the save file names
        CSaveFile::Instance().GetSaveFileNames();

        // Enable the "Continue" and "Load Game" menu options if we have save files
        if( CSaveFile::Instance().GetfileSaveCount() > 0 )
        {
            CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu( "title_screen_menu" );

            CUIControl * pCtrl = pMenu->GetPtrToControl("continue_btn");
            pCtrl->SetState( NUIControl::ECS_INACTIVE );
            pCtrl->Update();

            pCtrl = pMenu->GetPtrToControl("load_game_menu");
            pCtrl->SetState( NUIControl::ECS_INACTIVE );
            pCtrl->Update();
        }

        // Load shaders
        CShader::Instance().LoadFromXML( "data/shaders/shader_effects.cfg" );

        // Enumerate through all the shaders to set their default values
        CCriticalSection::Instance().Lock();
        CShader::Instance().EnumerateShaderInit();
        CCriticalSection::Instance().Unlock();
    }
    catch( NExcept::CCriticalException & ex )
    {
        exceptionTitleStr = ex.GetErrorTitle();
        exceptionInfo = ex.GetErrorMsg();
    }
    catch( std::exception const & ex )
    {
        exceptionTitleStr = "Standard Exception";
        exceptionInfo = ex.what();
    }
    catch(...)
    {
        exceptionTitleStr = "Unknown Error";
        exceptionInfo = "Something bad happened and I'm not sure what it was.";
    }

    return thread::THREAD_EXIT_CODE;

}	// Load


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CStartUpState::Update()
{
    poweredByWaffles.Update();

}	// Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CStartUpState::Transform()
{
    poweredByWaffles.Transform();

}	// Transform */


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CStartUpState::PreRender()
{
    CCriticalSection::Instance().Lock();

    poweredByWaffles.Render();

    CCriticalSection::Instance().Unlock();

}	// Render2D


/***************************************************************************
*    desc:  Is the state done
****************************************************************************/
bool CStartUpState::DoStateChange()
{
    // Check for any exceptions and rethrow since it happened in the thread
    if( loadThread.Finished() && !exceptionTitleStr.empty() || !exceptionInfo.empty() )
        throw NExcept::CCriticalException( exceptionTitleStr, exceptionInfo );

    // If done loading, start fading out
    if( loadThread.Finished() && 
        loadDelayTimer.Expired() &&
        (poweredByWaffles.GetActiveCommandName() == "fadeIn") &&
        !poweredByWaffles.IsAnimating() )
    {
        poweredByWaffles.SetActiveCommand( "fadeOut" );
    }

    // If fade out is complete, do a state change
    if( (poweredByWaffles.GetActiveCommandName() == "fadeOut") &&
        !poweredByWaffles.IsAnimating() )
    {
        // Tell the game window loading is complete
        PostMessage( CXDevice::Instance().GetWndHandle(), WM_LOAD_COMPLETE, 0, 0 );

        return true;
    }

    return false;

}	// DoStateChange





