
/************************************************************************
*    FILE NAME:       gameloadstate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "gameloadstate.h"

// Game lib dependencies
#include <utilities/highresolutiontimer.h>
#include <managers/actordatalist.h>
#include <managers/actormanager.h>
#include <managers/hudmanager.h>
#include <managers/instancemeshmanager.h>
#include <managers/megatexturemanager.h>
#include <managers/generatormanager.h>
#include <managers/physicsworldmanager.h>
#include <scripting/scriptmanager.h>
#include <xact/xact.h>
#include <system/xdevice.h>
#include <common/defs.h>
#include <common/pointint.h>
#include <common/worldpoint.h>
#include <misc/settings.h>
#include <2d/stage2d.h>
#include <2d/actorsprite2d.h>
#include <2d/objectvisualdata2d.h>

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
        // Free the stages
        CStage2D::Instance().ClearStage();

        // Free all the actor data
        CActorManager::Instance().ClearAll();

        // Clear out the actor data
        CActorDataList::Instance().Clear();

        // Free the instance mesh data
        CInstanceMeshManager::Instance().Clear();

        // Free the hud
        CHudManager::Instance().Clear();

        // Clear all the Mega Textures
        CMegaTextureManager::Instance().Clear();

        // Unload any data groups
        for( size_t i = 0; i < stateMessage.groupUnload.size(); ++i )
            CObjectDataList2D::Instance().FreeDataGroup( stateMessage.groupUnload[i] );

        // Load any data groups
        for( size_t i = 0; i < stateMessage.groupLoad.size(); ++i )
            CObjectDataList2D::Instance().LoadDataGroup( stateMessage.groupLoad[i] );

        // Start any physics worlds
        for( size_t i = 0; i < stateMessage.startPhysicsWorld.size(); ++i )
            CPhysicsWorldManager::Instance().GetWorld( stateMessage.startPhysicsWorld[i] )->SetActive(true);

        // Stop any physics worlds
        for( size_t i = 0; i < stateMessage.stopPhysicsWorld.size(); ++i )
            CPhysicsWorldManager::Instance().GetWorld( stateMessage.stopPhysicsWorld[i] )->SetActive(false);

        // Create any mega textures
        for( size_t i = 0; i < stateMessage.createMegaTexture.size(); ++i )
            CMegaTextureManager::Instance().CreateMegaTexture( 
                stateMessage.createMegaTexture[i].get<0>(),
                stateMessage.createMegaTexture[i].get<1>() );

        // Create any instance Meshes
        for( size_t i = 0; i < stateMessage.createInstanceMesh.size(); ++i )
            CInstanceMeshManager::Instance().CreateInstanceMesh( 
                stateMessage.createInstanceMesh[i] );

        // Load up the actor data list
        if( !stateMessage.actorDataLoad.empty() )
            CActorDataList::Instance().LoadFromXML( stateMessage.actorDataLoad );

        // Create any actors
        for( size_t i = 0; i < stateMessage.createActor.size(); ++i )
            CActorManager::Instance().CreateActor2D( stateMessage.createActor[i] );

        // Create any actor vectors
        for( size_t i = 0; i < stateMessage.createActorVec.size(); ++i )
            CActorManager::Instance().CreateActorVec2D( stateMessage.createActorVec[i] );

        // Load the 2d stage
        if( !stateMessage.stage2DLoad.empty() )
            CStage2D::Instance().LoadStage( stateMessage.stage2DLoad );
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

}	// Transform


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





