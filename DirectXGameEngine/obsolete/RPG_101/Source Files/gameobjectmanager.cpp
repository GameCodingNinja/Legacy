/************************************************************************
*    FILE NAME:       gameobjectmanager.cpp
*
*    DESCRIPTION:     Game specific object manager class
************************************************************************/

// Physical component dependency
#include "gameobjectmanager.h"

// Game dependencies
#include "3d\\stage.h"
#include "3d\\actor.h"
#include "3d\\animatedspritegroup.h"
#include "3d\\spritegroup.h"
#include "3d\\xwindow.h"
#include "3d\\shader.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer
************************************************************************/
CGameObjectManager::CGameObjectManager()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer
************************************************************************/
CGameObjectManager::~CGameObjectManager()
{
}   // Destructer


/************************************************************************
*    desc:  Updates the manager
************************************************************************/
void CGameObjectManager::Update()
{
    charManager.Update();

}	// Update


/************************************************************************
*    desc:  Changes the characters when a hotspot warp has been touched
************************************************************************/
void CGameObjectManager::ChangeCharacters()
{
    charManager.SwapCharacters( pActors, pStage->GetStageName() );

}	// ChangeCharacters


/************************************************************************
*    desc:  Load the character lists
*
*	 param: string & filePath - path to the xml file
************************************************************************/
void CGameObjectManager::LoadCharacterLists( string & filePath )
{
    charManager.LoadCharacterGroupLists( filePath );

}	// LoadCharacterLists


/************************************************************************
*    desc:  Renders all objects
*
*	 param:	CMatrix & matrix
************************************************************************/
void CGameObjectManager::Render( CMatrix & matrix )
{
    CShader::Instance().GetActiveShader()->SetTechnique( "environmentShading" );

    pActors->UpdateAnimation( matrix, pStage->GetLightList() );
    pStage->UpdateAnimation( matrix );
    
    RenderShadow( matrix );

    pStage->Render( matrix );
    
    CShader::Instance().GetActiveShader()->SetTechnique( "celShading" );

    pActors->Render( matrix );

    CShader::Instance().GetActiveShader()->SetTechnique( "environmentShading" );

    for(unsigned int i = 0; i < pStage->warpSpotVector.size(); i++)
    {
        pStage->warpSpotVector[i]->RenderHotSpot( matrix );
    }

    CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_STENCILENABLE, false );
    
}	//Render