/************************************************************************
*    FILE NAME:       stage.cpp
*
*    DESCRIPTION:    Class that holds an unordered map of all sector
*					 objects
************************************************************************/

// Physical component dependency
#include <3d/stage3d.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Standard lib dependencies
#include <algorithm>
#include <functional>

// Game lib dependencies
#include <utilities/deletefuncs.h>
#include <managers/stagedatalist.h>
#include <managers/shader.h>
#include <common/camera.h>
#include <common/stagedata.h>
#include <common/sectordata.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CStage3D::CStage3D()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CStage3D::~CStage3D()
{
    ClearStage();

}   // Destructer


/************************************************************************
*    desc:  Clear all the stage data so that data can be loaded
************************************************************************/
void CStage3D::ClearStage()
{
    // Delete the pointers
    NDelFunc::DeleteMapPointers(pSectorMap);

    // Reset the light list
    lightList.Free();

}	// ClearStage


/************************************************************************
*    desc:  Loads the stage
*
*	 param:	string & name - name of the stage to load
************************************************************************/
void CStage3D::LoadStage( const string & name )
{
    // Clear the previous stage before loading another one
    ClearStage();

    // Get the stage data
    pStageData = CStageDataList::Instance().GetData( name );

    // Set the default camera position and rotation for this stage
    CCamera::Instance().Reset();
    CCamera::Instance().Translate( pStageData->CameraPos() );
    CCamera::Instance().Rotate( pStageData->CameraRot() );

    // Load in the sectors
    for( size_t i = 0; i < pStageData->GetSectorCount(); ++i )
    {
        // New up a temporary sector
        CSector3D * pSector = new CSector3D();

        const CSectorData & sectorData = pStageData->GetSectorData(i);

        // Put the sector into the map
        pSectorMap.insert( make_pair( sectorData.GetPos(), pSector ) );

        // Load the sector
        pSector->LoadFromXML( sectorData.GetFile() );

        // Give the sector its position
        pSector->SetPosition( sectorData.GetPos() );

        // Place the lights from the sectors into the stage's light list
        for (map<string, CLight *>::iterator iter = pSector->GetLightMap().begin();
             iter != pSector->GetLightMap().end();
             ++iter)
        {
            string name = iter->first;
            lightList.AddLight( name, iter->second );
        }
    }

}	// LoadStage


/************************************************************************
*    desc:  Init the stage lights
************************************************************************/
void CStage3D::InitStageLights()
{
    // Init the shader lights
    GetLightList().SetShaderLights( CShader::Instance().GetEffectData( pStageData->GetEffect() ) );
}


/************************************************************************
*    desc:  Renders the stage objects
*
*	 param:	CMatrix & matrix - passed in matrix class
************************************************************************/
void CStage3D::Render( CMatrix & matrix )
{
    // Render objects with alpha first
    for( map<CPointInt, CSector3D *>::iterator iter = pSectorMap.begin();
         iter != pSectorMap.end();
         ++iter)
    {
        iter->second->RenderAlpha( matrix );
    }

    // Render objects without alpha last
    for( map<CPointInt, CSector3D *>::iterator iter = pSectorMap.begin();
         iter != pSectorMap.end();
         ++iter )
    {
        iter->second->Render( matrix );
    }

}	// Render


/************************************************************************
*    desc:  Render to shadow map
*
*	 param:	CMatrix & matrix - passed in matrix class
************************************************************************/
void CStage3D::RenderShadowMap( CMatrix & matrix )
{
    for( map<CPointInt, CSector3D *>::iterator iter = pSectorMap.begin();
         iter != pSectorMap.end();
         ++iter)
    {
        iter->second->RenderShadowMap( matrix );
    }

}	// RenderShadow


/************************************************************************
*    desc:  Allow objects suseptible to gravity to fall
*
*	 param:	float speed - speed of gravity
************************************************************************/
void CStage3D::ReactToGravity( float speed, float elapsedTime )
{
    for( map<CPointInt, CSector3D *>::iterator iter = pSectorMap.begin();
         iter != pSectorMap.end();
         ++iter)
    {
        iter->second->ReactToGravity( speed, elapsedTime );
    }

}	// ReactToGravity


/************************************************************************
*    desc:  Update the sectors
************************************************************************/
void CStage3D::Update()
{
    for( map<CPointInt, CSector3D *>::iterator iter = pSectorMap.begin();
         iter != pSectorMap.end();
         ++iter)
    {
        iter->second->Update();
    }

}	// IncAnimationTime


/************************************************************************
*    desc:  Returns the light list
*
*	 ret:	CLightLst - light list
************************************************************************/
CLightLst & CStage3D::GetLightList()
{
    return lightList;

}	// GetLightList

