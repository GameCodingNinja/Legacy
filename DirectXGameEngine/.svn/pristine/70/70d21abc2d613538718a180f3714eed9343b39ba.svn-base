/************************************************************************
*    FILE NAME:       objectmanager.cpp
*
*    DESCRIPTION:     Base Object Manager Class
************************************************************************/

// Physical component dependency
#include "objectmanager.h"

// DirectX lib dependencies
#include "d3dx9.h"

// Game lib dependencies
#include "xmlParser\\xmlParser.h"
#include "xwindow.h"
#include "misc\\genfunc.h"
#include "misc\\shader.h"
#include "misc\\deletefuncs.h"
#include "statcounter.h"
#include "actor.h"
#include "stage.h"
#include "spritegroup.h"
#include "camera.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CObjectManager::CObjectManager()
       :playerRadius(0),
        playerFloorPad(0),
        playerWallPad(0)
        //cameraOffset(-40)
{
    pActors = new CActor();
    pStage = new CStage();

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CObjectManager::~CObjectManager()
{
    Delete( pActors );
    Delete( pStage );

}   // Destructer


/************************************************************************
*    desc:  Takes in a file path to load in animated sprites
*
*	 param:	string & filepath - path to the animated sprites
************************************************************************/
bool CObjectManager::LoadAnimatedSprites( string & filePath )
{
    return pActors->LoadFromXML( filePath );

}	// LoadAnimatedSprites


/************************************************************************
*    desc:  Takes in a file path to load in animated sprites
*
*	 param:	string & filepath - path to the stage xml
************************************************************************/
bool CObjectManager::LoadStage( string & filePath )
{
    return pStage->LoadFromXML( filePath );

}	// LoadStage


/************************************************************************
*    desc:  Sprite group sort function
*
*    param: two sprite groups
************************************************************************/
bool CObjectManager::SortCSpriteGroup(CSpriteGroup * sg1, CSpriteGroup * sg2)
{
    return (sg1->GetVisualSprite().GetCenter().z > sg2->GetVisualSprite().GetCenter().z);

}	// SortCSpriteGroup


/************************************************************************
*    desc:  Gets the collision parameters for collision against player
*
*    param: float radius - radius of object, 
*			float fpad - padding of floor, 
*			float wpad - padding of wall
************************************************************************/
void CObjectManager::SetPlayerCollisionStats( float radius, float fpad, float wpad )
{
    playerRadius = radius;
    playerFloorPad = fpad;
    playerWallPad = wpad;

}	// SetPlayerCollisionStats


/************************************************************************
*    desc:  Sorts all objects with alpha shading in order from furthest
*	        to closest
*
*	 param:	CMatrix & matrix
************************************************************************
void CObjectManager::UpdateAlpha(CMatrix & matrix)
{	
    for(unsigned int i = 0; i < pStage->alphaVector.size(); i++)
    {
        pStage->alphaVector[i]->GetVisualSprite().Transform( matrix );
    }

    sort( pStage->alphaVector.begin(), pStage->alphaVector.end(), SortCSpriteGroup );	

}	// UpdateAlpha


/************************************************************************
*    desc:  Manages what objects colide with what
*
*	 param:	CCamera & camera
************************************************************************/
void CObjectManager::ReactToCollision()
{
    CCollisionCheck cCheck;
    CCollisionCheck cameraCheck;

    // get the elapsed time
    float elapsedTime = CHighResTimer::Instance().GetElapsedTime();

    // Causes the objects to react to gravity
    pStage->ReactToGravity( -0.05, elapsedTime );
    pActors->ReactToGravity( -0.05f, elapsedTime );

    // Apply all stage collisions against the player
    pStage->ReactToPlayerCollision( cCheck, playerRadius, playerFloorPad, playerWallPad );

    if( pStage->ReactToHotSpotWarp( cCheck, playerRadius, playerFloorPad, playerWallPad ) )
    {
        //ChangeCharacters();
    }

    // Apply all animated sprite collisions against the player
    pActors->ReactToPlayerCollision( CCamera::Instance().GetMoveMatrix(), cCheck, playerRadius, playerFloorPad, playerWallPad );

    CCamera::Instance().Translate( cCheck.GetFinalViewOffset() );

    //Apply all collisions against other objects
    pStage->ReactToObjectCollision( cCheck );

    //Apply all animated sprite collisions against other objects
    pActors->ReactToStageCollision( pStage->colPlayerVector, cCheck );

}	//ReactToCollision


/************************************************************************
*    desc:  Renders all objects
*
*	 param:	CMatrix & matrix
************************************************************************/
void CObjectManager::Render()
{
    pStage->Render( CCamera::Instance().GetFinalMatrix() );

    pActors->Render( CCamera::Instance().GetFinalMatrix() );

    for(unsigned int i = 0; i < pStage->warpSpotVector.size(); i++)
    {
        pStage->warpSpotVector[i]->RenderHotSpot( CCamera::Instance().GetFinalMatrix() );
    }
    
}	//Render


/************************************************************************
*    desc:  Update the objects
************************************************************************/
void CObjectManager::Update()
{
    pActors->IncAnimationTime();
    pStage->IncAnimationTime();

}	// Update


/************************************************************************
*    desc:  Renders to the shadow map buffer
*
*	 param:	CMatrix & matrix
************************************************************************/
void CObjectManager::RenderShadowMap(CMatrix & matrix)
{
    pStage->RenderShadowMap(matrix);
}


/************************************************************************
*    desc:  Renders the shadows of all objects currently visible
*
*	 param:	CMatrix & matrix
************************************************************************/
/*void CObjectManager::RenderShadow(CMatrix & matrix)
{
    if( pStage->lightList.IsShadowCast() )
    {
        CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_CCW_STENCILPASS,  D3DSTENCILOP_KEEP );

        // Disable all lighting
        CShader::Instance().SetEffectValue( string("enableLights"), false );

        // ambient lighting ON
        //CShader::Instance().SetAmbient( 0.1f, 0.1f, 0.1f );

        // Ignor our counters because these sprite will get rendered agian
        CStatCounter::Instance().IgnorCounters(true);

        // Renders all the objects in the stage which can have shadows casted onto them
        pStage->RenderShadowable( matrix );

        // Renders all the animated sprites which can have shadows casted onto them
        pActors->RenderShadowable( matrix );

        // Allow the counters to continue
        CStatCounter::Instance().IgnorCounters(false);

        // turn off the z-buffer
        CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_ZWRITEENABLE, false );

        // turn ON stencil buffer
        CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_STENCILENABLE, true );

        // Setup for shadow blending
        CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
        CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

        CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_ALWAYS );

        if( !CXWindow::Instance().IsUsingZPassShadowMethod() )
        {
            CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR );
        }
    
        // Builds and renders the shadow sprites in the stage
        pStage->RenderShadow( matrix );

        // Renders the animated shadow sprites
        pActors->RenderShadow( matrix );

        CShader::Instance().SetEffectValue( string("enableLights"), true );

        // turn ON the z-buffer
        CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_ZWRITEENABLE, true );

        // Setup to blend the rest of the scene
        CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

        CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_GREATER );

        CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP );

        // Clears the Z buffer
        CXWindow::Instance().GetXDevice()->Clear( 0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
    }

}	//RenderShadow */


/************************************************************************
*    desc:  Returns the stage object
*
*	 ret:	CStage stage
************************************************************************/
CStage * CObjectManager::GetStage()
{
    return pStage;

}	// GetStage


/************************************************************************
*    desc:  Set an animation
*
*	 param:	string & groupName - Name of sprite group
*           string & animName - name of animation
************************************************************************/
void CObjectManager::SetAnimation( string & groupName, string & animName )
{
    CGroup * pSpriteGrp = pActors->allGroupsMap[groupName];

    if( pSpriteGrp != NULL )
    {
        pSpriteGrp->SetAnimation( animName );
    }
    
}	// SetAnimation


void CObjectManager::ChangeCharacters()
{

}