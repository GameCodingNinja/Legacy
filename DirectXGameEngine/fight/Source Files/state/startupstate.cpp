
/************************************************************************
*    FILE NAME:       startupstate.cpp
*
*    DESCRIPTION:     CStartUp Class State
************************************************************************/

// Physical component dependency
#include "startupstate.h"

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <managers/meshmanager.h>
#include <managers/stagedatalist.h>
#include <managers/texturemanager.h>
#include <managers/shader.h>
#include <managers/vertexbuffermanager2d.h>
#include <managers/hudmanager.h>
#include <2d/fontmanager.h>
#include <2d/objectdatalist2d.h>
#include <scripting/scriptmanager.h>
#include <controller/gamecontroller.h>
#include <xact/xact.h>
#include <system/xdevice.h>
#include <gui/uimenutree.h>
#include <gui/uimenu.h>
#include <gui/uicontrol.h>
#include <utilities/genfunc.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/criticalsection.h>

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

    // Load the data list table
    CObjectDataList2D::Instance().LoadListTable( "data/objects/2d/objectDataList/dataListTable.lst" );
    CScriptMgr::Instance().LoadListTable( "data/objects/2d/scripts/scriptListTable.lst" );

    // Load the startup data group
    CObjectDataList2D::Instance().LoadDataGroup("(startup)");

    // Init the "Powered by Waffles" logo and tag it as unique so the mesh can be deleted
    poweredByFuz.Init( "(startup)", "waffles" );
    poweredByFuz.SetPos( CPoint(0,0,20) );
    poweredByFuz.SetVisible(false);

    logo.Init( "(startup)", "logo" );
    logo.SetPos( CPoint(0,0,20) );

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
*    desc:  Load from thread durring the startup screen
****************************************************************************/
DWORD CStartUpState::Load()
{
    try
    {
        // Load XAct
        CXAct::Instance().Init( "data/sounds/gameTemplate.xgs" );
        CXAct::Instance().LoadWaveBank( "data/sounds/sound_effects.xwb" );
        CXAct::Instance().LoadSoundBank( "data/sounds/sound_effects.xsb" );
        CXAct::Instance().LoadWaveBank( "data/sounds/music.xwb" );
        CXAct::Instance().LoadSoundBank( "data/sounds/music.xsb" );
        CXAct::Instance().BuildSoundCueMap();

        // Load all of the meshes and materials in these groups
        CObjectDataList2D::Instance().LoadDataGroup( "(menu)" );
        CObjectDataList2D::Instance().LoadDataGroup( "(title_screen)" );

        // Load up stage data list
        CStageDataList::Instance().LoadFromXML( "data/objects/2d/stageDataList.lst" );

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
    poweredByFuz.Update();
    logo.Update();

}	// Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CStartUpState::Transform()
{
    logo.Transform();
    poweredByFuz.Transform();

}	// Transform


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CStartUpState::PreRender()
{
    CCriticalSection::Instance().Lock();

    logo.Render();
    poweredByFuz.Render();

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

    // Fade out the logo
    if( loadDelayTimer.Expired() && (logo.GetActiveCommandName() == "fadeIn") && !logo.IsAnimating() )
        logo.SetActiveCommand( "fadeOut" );

    // Fade in the powered by...
    else if( (poweredByFuz.GetActiveCommandName() == "") && (logo.GetActiveCommandName() == "fadeOut") && !logo.IsAnimating() )
    {
        poweredByFuz.SetActiveCommand( "fadeIn" );
        loadDelayTimer.Set(2000);
    }

    // If done loading, start fading out
    if( (loadThread.Started() && !loadThread.Running()) && 
        loadDelayTimer.Expired() &&
        (poweredByFuz.GetActiveCommandName() == "fadeIn") &&
        !poweredByFuz.IsAnimating() )
    {
        poweredByFuz.SetActiveCommand( "fadeOut" );
    }

    // If fade out is complete, do a state change
    if( (poweredByFuz.GetActiveCommandName() == "fadeOut") &&
        !poweredByFuz.IsAnimating() )
    {
        // Tell the game window loading is complete
        PostMessage( CXDevice::Instance().GetWndHandle(), WM_LOAD_COMPLETE, 0, 0 );

        return true;
    }

    return false;

}	// DoStateChange





