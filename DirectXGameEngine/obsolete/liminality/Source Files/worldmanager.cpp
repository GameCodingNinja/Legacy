
/************************************************************************
*    FILE NAME:       worldmanager.cpp
*
*    DESCRIPTION:     Class to manage the world generation
************************************************************************/

// Physical component dependency
#include "worldmanager.h"

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include "2d\\sector2d.h"
#include "2d\\spritegroup2d.h"
#include "2d\\objectdatalist2d.h"
#include "3d\\worldcamera.h"
#include "misc\\settings.h"
#include "utilities\\genfunc.h"
#include "2d\\stage2d.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)
// disable warning about the use of the "this" pointer in the constructor list
#pragma warning(disable : 4355)

// The maximum number of sectors to generate and handle at once
const int MAX_SECTOR_DIM_COUNT = 3;

const int TERRAIN_PIECE_DIM = 64;


/************************************************************************
*    desc:  Constructer
************************************************************************/
CWorldManager::CWorldManager()
             : terrainDimCount(0)
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CWorldManager::~CWorldManager()
{
}	// destructer


/************************************************************************
*    desc:  Initialize the world manager                                                             
************************************************************************/
void CWorldManager::Init()
{
    // Calculate how many pieces of terrain will fill up a sector along one dimension.
    // NOTE: This value should never change while the game is running
    terrainDimCount = 2 * CSettings::Instance().GetSectorSize() / TERRAIN_PIECE_DIM;

    CPointInt sectorIndex;

    // Load the stage
    //CStage2D::Instance().LoadStage( string("test") );

    /*
    // Loop through the y-axis of the sectors
    for( int y = 1; y >= -1; y-- )
    {
        sectorIndex.y = y;

        // Loop through the x-axis of the sectors
        for( int x = -1; x <= 1; x++ )
        {
            sectorIndex.x = x;

            CSector2D * pSector = new CSector2D();
            pSector->SetPosition( sectorIndex );

            // Generate the sprites for the sector passed in
            GenerateSector( pSector );

            CStage2D::Instance().AddSector( pSector );
        }
    }*/

}	// Init */


/************************************************************************
*    desc:  Update the world
*
*	 param:	CPointInt & pos - sector position to focus the update on
************************************************************************/
void CWorldManager::Update( CPointInt & pos )
{
    // Generate new sectors if we need to
    ManageGeneration(pos);

    //CStage2D::Instance().Update( focusSectorIndex );
    CStage2D::Instance().Update();

}	// Update */


/************************************************************************
*    desc:  React the passed in sprite group to the stage's collision
*
*	 param:	CSpriteGroup2D * pGroup - group to react to the world's collision
************************************************************************/
void CWorldManager::ReactToCollision( CSpriteGroup2D * pGroup )
{
    CStage2D::Instance().ReactToCollision( pGroup );

}	// ReactToCollision */


/************************************************************************
*    desc:  Render the world                                                             
************************************************************************/
void CWorldManager::Render()
{
    CStage2D::Instance().RenderAllSectors( CWorldCamera::Instance() );

}	// Render */


/************************************************************************
*    desc:  Check to see if our sector position has changed so that we
*			can generate new sectors
*
*	 param:	CPointInt & pos - sector position to check against
************************************************************************/
void CWorldManager::ManageGeneration( CPointInt & pos )
{
    // Calculate how many sectors we've traveled since before
    int focusDiffX = abs(pos.x - focusSectorIndex.x);
    int focusDiffY = abs(pos.y - focusSectorIndex.y);

    // If we've changed sectors, something will need to be loaded and unloaded
    if( focusDiffX > 0 || focusDiffY > 0 )
    {
        CPointInt newSectorIndex, oldSectorIndex;

        // Loop through the y-axis of the sectors
        for( int y = 1; y >= -1; y-- )
        {
            newSectorIndex.y = pos.y + y;

            // Loop through the x-axis of the sectors
            for( int x = -1; x <= 1; x++ )
            {
                newSectorIndex.x = pos.x + x;

                // Check to see if a sector we want to be able to see already exists in the stage.
                if( !CStage2D::Instance().FindSector( newSectorIndex ) )
                {
                    // If not, we take the equal but opposite equivalent sector in relation to the
                    // previous focus
                    oldSectorIndex.x = focusSectorIndex.x - x;
                    oldSectorIndex.y = focusSectorIndex.y - y;

                    CSector2D * pSector = CStage2D::Instance().GetSector( oldSectorIndex );

                    if( pSector != NULL )
                    {
                        // We remove the sector from the stage's sector map so that we can recreate it
                        // with a new index
                        //CStage2D::Instance().RemoveSector( oldSectorIndex, true );
                        CStage2D::Instance().EraseSectorFromMap( oldSectorIndex );

                        // Give the sector its new position
                        pSector->SetPosition( newSectorIndex );

                        // Generate the sprites for the sector passed in
                        GenerateSector( pSector );

                        // Add the sector back into the stage with its new sprites and position
                        CStage2D::Instance().AddSector( pSector );
                    }
                }
            }
        }

        // Set the passed in position as the new focus sector
        focusSectorIndex = pos;
    }

}	// CheckForGeneration */


/************************************************************************
*    desc:  Generate the sprites that will make up the sector
*
*	 param:	CSector2D * pSector - sector to generate sprites for
************************************************************************/
void CWorldManager::GenerateSector( CSector2D * pSector )
{
    // The position to set the created sprites to
    float start = CSettings::Instance().GetSectorSize() - (TERRAIN_PIECE_DIM >> 1);
    CPoint pos;
    pos.y = 0;// start;
    pos.x = 0;//-start;

    //for( int i = 0; i < terrainDimCount; i++ )
    for( int i = 0; i < 1; i++ )
    {
        //for( int j = 0; j < terrainDimCount; j++ )
        for( int j = 0; j < 1; j++ )
        {
            // Even though the sprite is newed up here, once we give it to the sector, the sector
            // will be responsible for cleaning up the sprite
            CSpriteGroup2D * pTmpGroup = pSector->GetGroup(0);

            // Initialize the sprite
            //if( !pTmpGroup->InitGroup( *CObjectDataList2D::Instance().GetData( string("grass_dirt") ) ) )
                //break;

            // Give the sprite its position
            pTmpGroup->SetWorldPos( CWorldPoint(pSector->GetPosition(), pos) );
            
            // Add the sprite to the sector
            //pSector->AddSprite( pTmpGroup );

            // Increment along the x axis
            //pos.x += TERRAIN_PIECE_DIM;
        }

        // Decrement along the y axis
        //pos.y -= TERRAIN_PIECE_DIM;

        //pos.x = -start;
    }

}	// GenerateSector */


