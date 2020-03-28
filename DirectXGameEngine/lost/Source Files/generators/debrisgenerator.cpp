
/************************************************************************
*    FILE NAME:       debrisgenerator.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "debrisgenerator.h"

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <2d/spritegroup2d.h>
#include <2d/sector2d.h>
#include <2d/objectdatalist2d.h>
#include <managers/instancemeshmanager.h>
#include <system/xdevice.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)
// disable warning about the use of the "this" pointer in the constructor list
#pragma warning(disable : 4355)

// Sector constants
const int DEBRIS_SECTOR_DIMENSIONS = 3;
const int DEBRIS_SECTOR_TOTAL = DEBRIS_SECTOR_DIMENSIONS * DEBRIS_SECTOR_DIMENSIONS;
const int DEBRIS_SECTOR_SIZE = 3;

// Star constants
const int DEBRIS_SCALE_MIN = 0.5;
const int DEBRIS_SCALE_MAX = 2;
const int DEBRIS_DEPTH = 1;
const int DEBRIS_COUNT_RANGE = 6;
const int DEBRIS_COUNT_MIN = 6;
const int DEBRIS_COUNT_MAX = DEBRIS_COUNT_MIN + DEBRIS_COUNT_RANGE;

// The index starts and ends of particular sprites. This way we can easily
// get what we want out of a sector's vector of sprites
const int DEBRIS_INDEX_START = 0;
const int DEBRIS_INDEX_END = DEBRIS_INDEX_START + DEBRIS_COUNT_MAX - 1;


/************************************************************************
*    desc:  Constructer
************************************************************************/
CDebrisGenerator::CDebrisGenerator()
              : CGenerator(),
                GetRandDebrisPos( generator, IntDistribution( -(DEBRIS_SECTOR_SIZE >> 1), (DEBRIS_SECTOR_SIZE >> 1) ) ),
                GetRandDebrisScale( generator, FloatDistribution( DEBRIS_SCALE_MIN, DEBRIS_SCALE_MAX ) ),
                GetRandDebrisCount( generator, IntDistribution( DEBRIS_COUNT_MIN, DEBRIS_COUNT_MAX ) )
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CDebrisGenerator::~CDebrisGenerator()
{
}	// destructer


/************************************************************************
*    desc:  Intialize the world generator
*
*	 param:	CPointInt & focus - which sector the player currently is in
*			uint wSeed		  - seed for generation
************************************************************************/
void CDebrisGenerator::Init( CPointInt & focus, uint wSeed )
{
    CInstanceMesh2D * pInstMesh = CInstanceMeshManager::Instance().GetInstanceMeshPtr( "(space)" );
    CObjectData2D * pObjData = CObjectDataList2D::Instance().GetData( "(space)", "debris" );

    pUsedSectorVector.reserve( DEBRIS_SECTOR_TOTAL );

    for( int i = 0; i < DEBRIS_SECTOR_TOTAL; ++i )
    {
        // Create a sector
        CSector2D * pSector = new CSector2D();
        pUnusedSectorVector.push_back( pSector );
        spSectorVector.push_back( pSector );

        // Add the stars to the sector
        for( int j = 0; j < DEBRIS_COUNT_MAX; ++j )
        {
            CSpriteGroup2D * pSpriteGrp = new CSpriteGroup2D( pObjData, true );
            pSpriteGrp->SetInstanceMesh( pInstMesh );
            pSector->AddSprite( pSpriteGrp );
        }
    }

    // Call the base class's init function as well
    CGenerator::Init( focus, wSeed );

}	// Init */


/************************************************************************
*    desc:  Handle the generation
*
*	 param: CPointInt & newFocus - the new focus position
*			CPointInt & focus    - the old focus position
*			bool forceGenerate   - forcefully generate the entire space
*								   background area	 
************************************************************************/
void CDebrisGenerator::HandleGeneration( const CPointInt & focus, const CPointInt & newFocus, bool forceGenerate )
{
    // Generate, and if anything was generated then update the sprite vector 
    // with the newly generated sprites
    if( Generate( focus, newFocus, forceGenerate ) )
        UpdateSpriteVector();

}   // HandleGeneration */


/************************************************************************
*    desc:  Main generation function. Determines if anything needs to generate
*
*	 param: const CPointInt & newFocus - the new focus position
*			const CPointInt & focus    - the old focus position
*			bool forceGenerate         - forcefully generate dust at the focus
*
*	 ret:	bool - if anything was generated
************************************************************************/
bool CDebrisGenerator::Generate( const CPointInt & focus, const CPointInt & newFocus, bool forceGenerate )
{
    // To forcefully generate our debris, we set up th generator as if the last focus was
    // far enough away to regenerate all sectors around the new focus
    if( forceGenerate )
    {
        // Change the old focus so we'll regenerate all sectors
        center = (center + DEBRIS_SECTOR_DIMENSIONS) * DEBRIS_SECTOR_SIZE;
        center.z = 0;

        // Move all the sectors to the unused sector vector
        pUnusedSectorVector.insert(pUnusedSectorVector.end(), pUsedSectorVector.begin(), pUsedSectorVector.end());
        pUsedSectorVector.clear();

        // Set the x value's starting point
        int x = center.x - ( DEBRIS_SECTOR_DIMENSIONS >> 1 ) * DEBRIS_SECTOR_SIZE;
        
        // Set the value we'll use to index into the unused sector vector
        int index = 0;

        // Set each sector's position to something outside our current focus
        for( int i = 0; i < DEBRIS_SECTOR_DIMENSIONS; ++i )
        {
            // Set the y value's starting point
            int y = center.y - ( DEBRIS_SECTOR_DIMENSIONS >> 1 ) * DEBRIS_SECTOR_SIZE;

            for( int j = 0; j < DEBRIS_SECTOR_DIMENSIONS; ++j )
            {
                // Set the new sector position
                pUnusedSectorVector[index]->SetPosition( CPointInt( x, y, DEBRIS_DEPTH ) );

                y += DEBRIS_SECTOR_SIZE; 
                ++index;
            }

            x += DEBRIS_SECTOR_SIZE;
        }
    }

    // Find the amount of sectors we've moved
    CPointInt focusDiff = newFocus - center;

    // If the new focus is the same as the old one, no need to perform any changes
    if( abs(focusDiff.x) > (DEBRIS_SECTOR_SIZE >> 1) || abs(focusDiff.y) > (DEBRIS_SECTOR_SIZE >> 1) )
    {
        // Determine the new center
        CPointInt newCenter;
        int xMult, yMult;

        // Get the number generator sector multiples it takes to get to the player
        xMult = newFocus.x / DEBRIS_SECTOR_SIZE;
        yMult = newFocus.y / DEBRIS_SECTOR_SIZE;

        // Find the remainder of the divisions above
        int xMod = abs(newFocus.x) % DEBRIS_SECTOR_SIZE;
        int yMod = abs(newFocus.y) % DEBRIS_SECTOR_SIZE;

        // If the remainder is greater than half the sector size rounded down, we
        // add or subtract 1 to the multiple
        if( xMod > (DEBRIS_SECTOR_SIZE >> 1) )
        {
            if( newFocus.x < 0 )
                xMult -= 1;
            else
                xMult += 1;
        }

        if( yMod > (DEBRIS_SECTOR_SIZE >> 1) )
        {
            if( newFocus.y < 0 )
                yMult -= 1;
            else
                yMult += 1;
        }

        // Calculate the new center of the generator
        newCenter.x = xMult * DEBRIS_SECTOR_SIZE;
        newCenter.y = yMult * DEBRIS_SECTOR_SIZE;
    
        // Organize the sectors by ones inside the new focus range and ones outside
        // of the new focus range
        OrganizeSectors( newCenter );

        // Go through the sectors and figure out which ones we're not using anymore
        sectorVecIter = pUnusedSectorVector.begin();
        while( sectorVecIter != pUnusedSectorVector.end() )
        {
            // Calculate the sector's new location
            CPointInt newSectorPos = (*sectorVecIter)->GetPosition() - center;
            newSectorPos = newCenter + ( newSectorPos * -1 );
            newSectorPos.z = DEBRIS_DEPTH;
            (*sectorVecIter)->SetPosition( newSectorPos );

            // Set the seed for our background generation
            generator.seed( GetSectorSeed( newSectorPos ) );

            // Generate the elements of the stars
            GenerateDebris( sectorVecIter );

            // Move the sector from the unused vector to the used one
            pUsedSectorVector.push_back( (*sectorVecIter) );
            sectorVecIter = pUnusedSectorVector.erase( sectorVecIter );
        }

        // Set the new center
        center = newCenter;

        // If we're here, we have generated new objects
        return true;
    }

    // If we're here, nothing new was generated
    return false;

}	// Generate */


/************************************************************************
*    desc:  Generate the simple stars
*
*	 param: vector<CSector2D *>::iterator & sectorIter - sector to generate
*														 the stars in
************************************************************************/
void CDebrisGenerator::GenerateDebris( vector<CSector2D *>::iterator & sectorIter )
{
    // Get a seed to determine if we have debris to create
    int debrisCount = GetRandDebrisCount();

    // Generate the dust
    for( int i = DEBRIS_INDEX_START; i <= DEBRIS_INDEX_END; ++i )
    {
        // Get the sprite group to randomize 
        CSpriteGroup2D * pTmpGroup = (*sectorIter)->GetGroup(i);

        if( debrisCount > 0 )
        {
            // Make the dust visible
            pTmpGroup->SetVisible( true );

            // Randomize the rotation
            pTmpGroup->SetRot( CPoint( 0, 0, GetRandRot() ) );
            
            // Randomize the position
            CWorldPoint tmpPos;
            tmpPos.x.i = GetRandDebrisPos() + (*sectorIter)->GetPosition().x;
            tmpPos.y.i = GetRandDebrisPos() + (*sectorIter)->GetPosition().y;
            tmpPos.z.i = DEBRIS_DEPTH;
            tmpPos.x.f = GetRandFloatPos();
            tmpPos.y.f = GetRandFloatPos();
            tmpPos.z.f = GetRandFloatPos();
            pTmpGroup->SetPos( tmpPos );

            // Randomize the scale
            //float tmpScale = GetRandDebrisScale();
            //pTmpGroup->SetScale( CPoint( tmpScale, tmpScale, 1 ) );

            // Call to get a random frame number
            RandIntGen GetRandFrame( generator, IntDistribution( 0, pTmpGroup->GetFrameCount() - 1 ) );

            // Randomize the frame number
            pTmpGroup->SetCurrentFrame( GetRandFrame() );
        }
        else
            pTmpGroup->SetVisible( false );

        debrisCount--;
    }

}	// GenerateDust */


/************************************************************************
*    desc:  Find the sectors out of our focus range and moves them to the
*			unused sector vector
*
*	 param: CPointInt & point - focus point
************************************************************************/
void CDebrisGenerator::OrganizeSectors( const CPointInt & point )
{
    // The full variance equation is: 
    int variance = DEBRIS_SECTOR_SIZE;
    vector<CSector2D *>::iterator sectorVecIter = pUsedSectorVector.begin();

    // Go through the sectors and figure out which ones we're not using anymore
    while( sectorVecIter != pUsedSectorVector.end() )
    {
        // If we're not using them, we remove them
        if( ( (*sectorVecIter)->GetPosition().x > point.x + variance ) ||
            ( (*sectorVecIter)->GetPosition().x < point.x - variance ) ||
            ( (*sectorVecIter)->GetPosition().y > point.y + variance ) ||
            ( (*sectorVecIter)->GetPosition().y < point.y - variance ) )
        {
            pUnusedSectorVector.push_back( (*sectorVecIter) );
            sectorVecIter = pUsedSectorVector.erase( sectorVecIter );
        }
        else
            ++sectorVecIter;
    }

}	// OrganizeSectors */


/************************************************************************
*    desc:  Clear the contents of the generator
************************************************************************/
void CDebrisGenerator::Clear()
{
    CGenerator::Clear();

}	// Clear */


