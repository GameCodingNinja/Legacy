
/************************************************************************
*    FILE NAME:       gameloadstate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "gameloadstate.h"

// Game lib dependencies
#include <utilities/highresolutiontimer.h>
#include <managers/actormanager.h>
#include <managers/hudmanager.h>
#include <scripting/scriptmanager.h>
#include <xact/xact.h>
#include <system/xdevice.h>
#include <common/defs.h>
#include <misc/settings.h>
#include <2d/stage2d.h>
#include <2d/objectdatalist2d.h>
#include <2d/objectvisualdata2d.h>
#include <2d/visualsprite2d.h>
#include <3d/stage3d.h>
#include <3d/objectdatalist3d.h>
#include <utilities/criticalsection.h>

// Game dependencies
#include "../game/savefile.h"

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CGameLoadState::CGameLoadState( const CStateMessage & stateMsg )
{
    // Set the game state for this object
    gameState = EGS_GAME_LOAD;

    // Copy over the message to react to
    stateMessage = stateMsg;

    // Tell the game window loading has started
    SendMessage( CXDevice::Instance().GetWndHandle(), WM_LOADING, 0, 0 );

    // Load all of the meshes and materials in these groups
    CObjectDataList2D::Instance().LoadDataGroup( "(load_screen)" );
    
    // Initialize the loading screen image
    loadingImage.Init( "(load_screen)", "loading_image" );
    loadingImage.SetScale( CPoint(2,2,1) );

    // Calculate the position to place the loading image
    CPoint tmpPos;
    CSize<float> imageSize = loadingImage.GetSize();
    tmpPos.x =  ( CSettings::Instance().GetDefaultSize().w / 2 ) - ( imageSize.w * 0.75f );
    tmpPos.y = -( CSettings::Instance().GetDefaultSize().h / 2 ) + ( imageSize.h * 0.75f );
    tmpPos.z = 20;

    loadingImage.SetPos( tmpPos );

    // Start the load thread
    loadThread.Start( this, &CGameLoadState::Load );

}	// Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CGameLoadState::~CGameLoadState()
{
    // Free all the assets associated with load screen
    CObjectDataList2D::Instance().FreeDataGroup( "(load_screen)" );

}   // Destructer


/***************************************************************************
*    desc:  Load from thread during the loading screen
****************************************************************************/
DWORD CGameLoadState::Load()
{
    try
    {
        // Free all the actor data
        CActorManager::Instance().ClearAll();

        // Free the hud
        CHudManager::Instance().Clear();

        // Free the stages
        CStage2D::Instance().ClearStage();
        CStage3D::Instance().ClearStage();

        // Unload any data groups
        for( size_t i = 0; i < stateMessage.group2DUnload.size(); ++i )
            CObjectDataList2D::Instance().FreeDataGroup( stateMessage.group2DUnload[i] );

        for( size_t i = 0; i < stateMessage.group3DUnload.size(); ++i )
            CObjectDataList3D::Instance().FreeDataGroup( stateMessage.group3DUnload[i] );

        // Load any data groups
        for( size_t i = 0; i < stateMessage.group2DLoad.size(); ++i )
            CObjectDataList2D::Instance().LoadDataGroup( stateMessage.group2DLoad[i] );

        for( size_t i = 0; i < stateMessage.group3DLoad.size(); ++i )
            CObjectDataList3D::Instance().LoadDataGroup( stateMessage.group3DLoad[i] );

        // Create any actors
        for( size_t i = 0; i < stateMessage.createActor.size(); ++i )
            CActorManager::Instance().CreateActor2D( stateMessage.createActor[i] );

        // Create any actor vectors
        for( size_t i = 0; i < stateMessage.createActorVec.size(); ++i )
            CActorManager::Instance().CreateActorVec2D( stateMessage.createActorVec[i] );

        // load the hud
        if( !stateMessage.hudLoad.empty() )
            CHudManager::Instance().LoadHud( stateMessage.hudLoad );

        // Load the 2d stage
        if( !stateMessage.stage2DLoad.empty() )
            CStage2D::Instance().LoadStage( stateMessage.stage2DLoad );

        // Load the 3d stage
        if( !stateMessage.stage3DLoad.empty() )
        {
            CStage3D::Instance().LoadStage( stateMessage.stage3DLoad );

            CCriticalSection::Instance().Lock();
            CStage3D::Instance().InitStageLights();
            CCriticalSection::Instance().Unlock();
        }

        // Save the game file
        if( (stateMessage.nextState == EGS_TITLE_SCREEN) &&
            (stateMessage.lastState == EGS_RUN))
        {
            CSaveFile::Instance().Save();
        }
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
void CGameLoadState::Update()
{
    if( (loadingImage.GetActiveCommandName() == "fadeIn") && !loadingImage.IsAnimating() )
        loadingImage.SetActiveCommand( "load" );

    loadingImage.Update();

}	// Update


/***************************************************************************
*    desc:  Transform game objects
****************************************************************************/
void CGameLoadState::Transform()
{
    loadingImage.Transform();

}	// Transform */


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CGameLoadState::PreRender()
{
    CCriticalSection::Instance().Lock();

    loadingImage.Render();

    CCriticalSection::Instance().Unlock();

}	// Render2D


/***************************************************************************
*    desc:  Is the state done
****************************************************************************/
bool CGameLoadState::DoStateChange()
{
    // Check for any exceptions and rethrow since it happened in the thread
    if( loadThread.Finished() && !exceptionTitleStr.empty() || !exceptionInfo.empty() )
        throw NExcept::CCriticalException( exceptionTitleStr, exceptionInfo );

    // If done loading, start fading out
    if( loadThread.Finished() &&
        (loadingImage.GetActiveCommandName() == "load") )
    {
        loadingImage.SetActiveCommand( "fadeOut" );
    }

    // If fade out is complete, do a state change
    if( (loadingImage.GetActiveCommandName() == "fadeOut") &&
        !loadingImage.IsAnimating() )
    {
        // Tell the game window loading is complete
        PostMessage( CXDevice::Instance().GetWndHandle(), WM_LOAD_COMPLETE, 0, 0 );

        return true;
    }

    return false;

}	// DoStateChange


/***************************************************************************
*    desc:  Get the next state to load
****************************************************************************/
EGameState CGameLoadState::GetNextState()
{
    return stateMessage.nextState;

}	// GetNextState





