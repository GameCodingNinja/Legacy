
/************************************************************************
*    FILE NAME:       classtemplate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "startupstate.h"

// Standard lib dependencies

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <managers/actordatalist.h>
#include <managers/meshmanager.h>
#include <managers/actormanager.h>
#include <managers/stagedatalist.h>
#include <managers/texturemanager.h>
#include <managers/shader.h>
#include <3d/objectdatalist.h>
#include <3d/stage.h>
#include <2d/objectdatalist2d.h>
#include <2d/fontmanager.h>
#include <2d/stage2d.h>
#include <2d/vertexbuffermanager2d.h>
#include <scripting/scriptmanager.h>
#include <controller/gamecontroller.h>
#include <xact/xact.h>
#include <system/xwindow.h>
#include <gui/uimenutree.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/criticalsection.h>
#include <common/texture.h>

// Game dependencies
#include "matuvlistmanager.h"

// Required namespace(s)
using namespace std;

// disable warning about the use of the "this" pointer in the constructor list
#pragma warning(disable : 4355)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CStartUpState::CStartUpState()
{
    // Force the initialzation of the critical state
    CCriticalSection::Instance();

    // Load the 2d shader
    CShader::Instance().LoadFromXML( string("data/shaders/startup_shader_effects.cfg") );

    // Load up the object data list
    CObjectDataList2D::Instance().LoadFromXML( string("data/objects/2d/object data list/startupDataList.lst") );

    // Load the script to animate the waffles startup
    CScriptMgr::Instance().LoadFromXML( string("data/objects/2d/startupScriptList.lst") );

    // Init the "Powered by Waffles" logo and tag it as unique so the mesh can be deleted
    poweredByWaffles.InitGroup( CObjectDataList2D::Instance().GetData( string("(startup)"), string("waffles") ) );
    poweredByWaffles.SetPos( CPoint(0,0,20) );
    poweredByWaffles.Transform();

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
    // Delete the scritp loaded for the startup
    CScriptMgr::Instance().DeleteScript( string("waffle_startup") );

    // Free all the assets associated with this graphic
    poweredByWaffles.FreeAssets();

}   // Destructer


/***************************************************************************
*    desc:  Load from thread durring the startup screen
****************************************************************************/
DWORD CStartUpState::Load()
{
    // Load XACT
    CXAct::Instance().Init( string("data\\sounds\\gameTemplate.xgs") );
    CXAct::Instance().LoadWaveBank( string("data\\sounds\\sound_effects.xwb") );
    CXAct::Instance().LoadSoundBank( string("data\\sounds\\sound_effects.xsb") );
    CXAct::Instance().LoadWaveBank( string("data\\sounds\\music.xwb") );
    CXAct::Instance().LoadSoundBank( string("data\\sounds\\music.xsb") );
    CXAct::Instance().BuildSoundCueMap();

    CCriticalSection::Instance().Lock();
    CXAct::Instance().LoadFinished();
    CCriticalSection::Instance().Unlock();

    // Load up the object data list
    CActorDataList::Instance().LoadFromXML( string("data\\objects\\2d\\actor\\actorDataList.lst") );
    CObjectDataList2D::Instance().LoadFromXML( string("data\\objects\\2d\\object data list\\objectDataList.lst") );
    CObjectDataList2D::Instance().LoadFromXML( string("data\\objects\\2d\\object data list\\menuDataList.lst") );
    CScriptMgr::Instance().LoadFromXML( string("data\\objects\\2d\\scriptList.lst") );
    CMatUVListManager::Instance().LoadFromXML( string("data\\materials\\textures\\layer_mat.cfg") );

    // Load in any fonts
    CFontMgr::Instance().LoadFromXML(string("data\\materials\\fonts\\fonts.cfg"));

    // Load up stage data list
    CStageDataList::Instance().LoadFromXML( string("data\\objects\\2d\\stageDataList.lst") );

    // Init the game controllers
    CGameController::Instance().Init( CXWindow::Instance().GetWndHandle() );

    // add devices to the controller list
    CGameController::Instance().AddToPollLst( KEYBOARD );
    CGameController::Instance().AddToPollLst( MOUSE );
    CGameController::Instance().AddToPollLst( JOYPAD );

    // load in the default controller mappings
    CGameController::Instance().LoadActionFromXML( string("data\\settings\\controllerMapping.cfg") );

    // Load the stages
    CStage2D::Instance().LoadStage( string("test") );

    // load the menu tree
    CUIMenuTree::Instance().LoadFromXML( string("data\\objects\\2d\\menus\\mainTree.menu") );

    // Create the needed actors
    CActorManager::Instance().CreateActor2D( string("player") );

    // Load shaders
    CShader::Instance().LoadFromXML( string("data\\shaders\\shader_effects.cfg") );

    // Enumerate through all the shaders to set their default values
    CCriticalSection::Instance().Lock();
    CShader::Instance().EnumerateShaderInit();
    CCriticalSection::Instance().Unlock();

    return thread::THREAD_EXIT_CODE;

}	// Load


/***************************************************************************
*    desc:  Act upon what the user is doing
****************************************************************************/
void CStartUpState::GetUserInput()
{

}	// GetUserInput


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CStartUpState::Update()
{
    poweredByWaffles.Update();

}	// Update


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
    // If done loading, start fading out
    if( (loadThread.Started() && !loadThread.Running()) && 
        loadDelayTimer.Expired() &&
        (poweredByWaffles.GetActiveCommandName() == "fadeIn") &&
        !poweredByWaffles.IsAnimating() )
    {
        poweredByWaffles.SetActiveCommand( string("fadeOut") );
    }

    // If fade out is complete, do a state change
    if( (poweredByWaffles.GetActiveCommandName() == "fadeOut") &&
        !poweredByWaffles.IsAnimating() )
    {
        // Tell the game window loading is complete
        PostMessage( CXWindow::Instance().GetWndHandle(), WM_LOAD_COMPLETE, 0, 0 );

        return true;
    }

    return false;

}	// DoStateChange





