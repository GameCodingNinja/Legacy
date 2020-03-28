/************************************************************************
*    FILE NAME:       stage2d.cpp
*
*    DESCRIPTION:    Class that holds an unordered map of all sector
*					 objects
************************************************************************/

// Physical component dependency
#include <2d/stage2d.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/deletefuncs.h>
#include <2d/spritegroup2d.h>
#include <2d/sector2d.h>
#include <managers/stagedatalist.h>
#include <common/camera.h>
#include <common/stagedata.h>
#include <common/sectordata.h>

const int SURROUNDING_SECTOR_COUNT = 9;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CStage2D::CStage2D()
{
    surroundingSectorModVector.reserve(SURROUNDING_SECTOR_COUNT);

    surroundingSectorModVector.push_back( CPointInt(0,1,0) );	// top
    surroundingSectorModVector.push_back( CPointInt(-1,1,0) );	// top left
    surroundingSectorModVector.push_back( CPointInt(1,1,0) );	// top right
    surroundingSectorModVector.push_back( CPointInt(0,0,0) );	// middle
    surroundingSectorModVector.push_back( CPointInt(-1,0,0) );	// left
    surroundingSectorModVector.push_back( CPointInt(1,0,0) );	// right
    surroundingSectorModVector.push_back( CPointInt(0,-1,0) );	// bottom
    surroundingSectorModVector.push_back( CPointInt(-1,-1,0) );	// bottom left
    surroundingSectorModVector.push_back( CPointInt(1,-1,0) );	// bottom righ

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CStage2D::~CStage2D()
{
    ClearStage();

}   // Destructer


/************************************************************************
*    desc:  Clear all the stage data so that data can be loaded
*
*	 param: bool clearSectorsOnly = false - only clear the sector data
*										    and not all of the stage data.
*											This is so sectors can be reused 
************************************************************************/
void CStage2D::ClearStage( bool clearSectorsOnly )
{
    // Clear the sectors
    for( std::map<CPointInt, CSector2D *>::iterator iter = pSectorMap.begin();
            iter != pSectorMap.end();
            ++iter)
    {
        iter->second->ClearSector();
    }

    // Delete the pointers
    if( !clearSectorsOnly )
        NDelFunc::DeleteMapPointers(pSectorMap);

}	// ClearStage


/************************************************************************
*    desc:  Loads the stage
*
*	 param:	string & name - name of the stage to load
************************************************************************/
void CStage2D::LoadStage( const std::string & name )
{
    // Clear the previous stage before loading another one
    ClearStage();
    
    // Get the stage data
    pStageData = CStageDataList::Instance().GetData( name );

    // Set the default camera position and rotation for this stage
    CCamera::Instance().Translate2D( pStageData->CameraPos() );
    CCamera::Instance().Rotate2D( pStageData->CameraRot() );

    // Load in the sectors
    for( unsigned int i = 0; i < pStageData->GetSectorCount(); ++i )
    {
        // New up a temporary sector
        CSector2D * pSector = new CSector2D();

        const CSectorData & sectorData = pStageData->GetSectorData(i);

        // Put the sector into the map
        pSectorMap.insert( std::make_pair(sectorData.GetPos(), pSector) );

        // Give the sector its position
        pSector->SetPosition( sectorData.GetPos() );

        // Load the sector
        pSector->LoadFromXML( sectorData.GetFile() );
    }

}	// LoadStage


/************************************************************************
*    desc:  Transform the stage's components
************************************************************************/
void CStage2D::Transform()
{
    for (sectorMapIter = pSectorMap.begin();
         sectorMapIter != pSectorMap.end();
         ++sectorMapIter)
    {
        sectorMapIter->second->Transform();
    }

}	// Transform


/************************************************************************
*    desc:  Transform the stage's components
*
*	 param: const CMatrix & _scaledMatrix  - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point      - passed in relative position
************************************************************************/
void CStage2D::Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                          const CWorldPoint & point )
{
    for (sectorMapIter = pSectorMap.begin();
         sectorMapIter != pSectorMap.end();
         ++sectorMapIter)
    {
        sectorMapIter->second->Transform( _scaledMatrix, _unscaledMatrix, point );
    }
    
}	// Transform


/************************************************************************
*    desc:  React the passed in sprites to the collision of their 
*			surrounding sectors
************************************************************************/
void CStage2D::ReactToCollision( CSpriteGroup2D * pGroup )
{

}	// ReactToCollision


/************************************************************************
*    desc:  Update the sectors
************************************************************************/
void CStage2D::Update()
{
    for (sectorMapIter = pSectorMap.begin();
         sectorMapIter != pSectorMap.end();
         ++sectorMapIter)
    {
        sectorMapIter->second->Update();
    }

}	// Update


/************************************************************************
*    desc:  Update the sectors
*
*	 param:	CPointInt & focusSector - the sector, as well as its eight
*									  surrounding sectors that should be
*									  updated
************************************************************************/
void CStage2D::Update( const CPointInt & focusSector )
{
    for( int i = 0; i < SURROUNDING_SECTOR_COUNT; ++i )
    {
        CPointInt sectorIndex =  surroundingSectorModVector[i] + focusSector;

        sectorMapIter = pSectorMap.find( sectorIndex );

        if( sectorMapIter != pSectorMap.end() )
            sectorMapIter->second->Update();
    }

}	// Update


/************************************************************************
*    desc:  Renders the sectors
************************************************************************/
void CStage2D::Render()
{ 
    RenderAllSectors();

}	// Render


/************************************************************************
*    desc:  Renders all the stage sectors
************************************************************************/
void CStage2D::RenderAllSectors()
{ 
    for (sectorMapIter = pSectorMap.begin();
         sectorMapIter != pSectorMap.end();
         ++sectorMapIter)
    {
        sectorMapIter->second->Render();
    }

}	// RenderAllSectors


/************************************************************************
*    desc:  Add a sector to the stage
*
*	 param:	CSector2D * pSector - sector to add
************************************************************************/
void CStage2D::AddSector( CSector2D * pSector )
{
    // Check to see if the sector isn't already in the stage
    if( pSectorMap.find( pSector->GetPosition() ) == pSectorMap.end() )
        pSectorMap.insert( std::make_pair(pSector->GetPosition(), pSector) );

}	// AddSector


/************************************************************************
*    desc:  Get a sector from the stage
*
*	 param:	CPointInt & sectorPos - position of the sector
*
*	 ret:	CSector2D * - sector desired
************************************************************************/
CSector2D * CStage2D::GetSector( const CPointInt & sectorPos )
{
    sectorMapIter = pSectorMap.find( sectorPos );

    if( sectorMapIter != pSectorMap.end() )
        return sectorMapIter->second;

    return NULL;

}	// GetSector


/************************************************************************
*    desc:  Find a sector from the stage
*
*	 param:	CPointInt & sectorPos - position of the sector
*
*	 ret:	bool - whether or not the sector was found
************************************************************************/
bool CStage2D::FindSector( const CPointInt & sectorPos )
{
    sectorMapIter = pSectorMap.find( sectorPos );

    if( sectorMapIter != pSectorMap.end() )
        return true;

    return false;

}	// FindSector


/************************************************************************
*    desc:  Remove a sector from the stage
*
*	 param:	CPointInt & sectorPos - sector to remove
*			bool clearSectorOnly = false - only clear the sector data
************************************************************************/
void CStage2D::RemoveSector( const CPointInt & sectorPos, bool clearSectorOnly )
{
    // Check to see if the sector isn't already in the stage
    sectorMapIter = pSectorMap.find( sectorPos );

    if( sectorMapIter != pSectorMap.end() )
    {
        sectorMapIter->second->ClearSector();

        if( !clearSectorOnly )
            NDelFunc::Delete( sectorMapIter->second );

        pSectorMap.erase( sectorMapIter );
    }

}	// RemoveSector


/************************************************************************
*    desc:  Erase a sector from the stage's sector map
*
*	 param:	CPointInt & sectorPos - sector to erase
************************************************************************/
void CStage2D::EraseSectorFromMap( const CPointInt & sectorPos )
{
    // Check to see if the sector isn't already in the stage
    sectorMapIter = pSectorMap.find( sectorPos );

    if( sectorMapIter != pSectorMap.end() )
        pSectorMap.erase( sectorMapIter );

}	// EraseSectorFromMap


/************************************************************************
*    desc:  Get the size of the sector map
*
*	 ret:	unsigned int - number of sectors in the sector map
************************************************************************/
size_t CStage2D::GetSectorMapSize() const
{
    return pSectorMap.size();

}	// GetSectorMapSize



