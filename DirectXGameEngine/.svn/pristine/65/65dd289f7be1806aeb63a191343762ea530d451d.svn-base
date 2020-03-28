
/************************************************************************
*    FILE NAME:       gameloadstate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "gameloadstate.h"

// Game lib dependencies
#include <utilities/highresolutiontimer.h>
#include <2d/objectvisualdata2d.h>
#include <managers/actormanager.h>
#include <managers/megatexturemanager.h>
#include <managers/instancemeshmanager.h>
#include <managers/hudmanager.h>
#include <scripting/scriptmanager.h>
#include <xact/xact.h>
#include <system/xdevice.h>
#include <common/defs.h>
#include <misc/settings.h>

// Game dependencies
#include "../generators/worldgenerator.h"
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

    CPoint tmpPos;
    tmpPos.x =  ( CSettings::Instance().GetDefaultSize().w / 2 ) - ( loadingImage.GetVisualData().GetSize().w * loadingImage.GetScale().x );
    tmpPos.y = -( CSettings::Instance().GetDefaultSize().h / 2 ) + ( loadingImage.GetVisualData().GetSize().h * loadingImage.GetScale().y );
    tmpPos.z = 20;

    loadingImage.SetPos( tmpPos );

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

        // Free the instance mesh data
        CInstanceMeshManager::Instance().Clear();

        // Free the hud
        CHudManager::Instance().Clear();

        // Free the world generator
        CWorldGenerator::Instance().Clear();

        // Clear all the Mega Textures
        CMegaTextureManager::Instance().Clear();

        // Unload any data groups
        for( size_t i = 0; i < stateMessage.groupUnload.size(); ++i )
            CObjectDataList2D::Instance().FreeDataGroup( stateMessage.groupUnload[i] );


        // Load any data groups
        for( size_t i = 0; i < stateMessage.groupLoad.size(); ++i )
            CObjectDataList2D::Instance().LoadDataGroup( stateMessage.groupLoad[i] );

        // Create any actors
        for( size_t i = 0; i < stateMessage.createActor.size(); ++i )
            CActorManager::Instance().CreateActor2D( stateMessage.createActor[i] );

        // Create any actor vectors
        for( size_t i = 0; i < stateMessage.createActorVec.size(); ++i )
            CActorManager::Instance().CreateActorVec2D( stateMessage.createActorVec[i] );

        // Create any mega textures
        for( size_t i = 0; i < stateMessage.createMegaTexture.size(); ++i )
            CMegaTextureManager::Instance().CreateMegaTexture( 
                stateMessage.createMegaTexture[i].get<0>(),
                stateMessage.createMegaTexture[i].get<1>() );

        // Create any instance Meshes
        for( size_t i = 0; i < stateMessage.createInstanceMesh.size(); ++i )
            CInstanceMeshManager::Instance().CreateInstanceMesh( 
                stateMessage.createInstanceMesh[i] );

        if( !stateMessage.hudLoad.empty() )
            CHudManager::Instance().LoadHud( stateMessage.hudLoad );

        if( !stateMessage.createMegaTexture.empty() )
            CWorldGenerator::Instance().Init();


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
*    desc:  Transform the game objects
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
    loadingImage.Render();

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





