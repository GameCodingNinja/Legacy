
/************************************************************************
*    FILE NAME:       stargenerator.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "stargenerator.h"

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <2d/spritegroup2d.h>
#include <2d/sector2d.h>
#include <managers/instancemeshmanager.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)
// disable warning about the use of the "this" pointer in the constructor list
#pragma warning(disable : 4355)

// Arbitrarily large number
const unsigned int GEN_INFINITY = -1;


// Sector constants
const int STAR_SECTOR_DIMENSIONS = 3;
const int STAR_SECTOR_TOTAL = STAR_SECTOR_DIMENSIONS * STAR_SECTOR_DIMENSIONS;
const int STAR_SECTOR_SIZE = 121;

// Star constants
const int STAR_SCALE_MIN = 160;
const int STAR_SCALE_MAX = 200;
const int STAR_DEPTH = 100;
const int STAR_COUNT_RANGE = 0;
const int STAR_COUNT_MIN = 35;
const int STAR_COUNT_MAX = STAR_COUNT_MIN + STAR_COUNT_RANGE;
const int BIG_STAR_COUNT_RANGE = 5;
const int BIG_STAR_COUNT_MIN = 1;
const int BIG_STAR_COUNT_MAX = BIG_STAR_COUNT_MIN + BIG_STAR_COUNT_RANGE;
const int BIG_STAR_SCALE_MIN = 61;
const int BIG_STAR_SCALE_MAX = 111;

// The index starts and ends of particular sprites. This way we can easily
// get what we want out of a sector's vector of sprites
const int STAR_INDEX_START = 0;
const int STAR_INDEX_END = STAR_INDEX_START + STAR_COUNT_MAX - 1;
const int BIG_STAR_INDEX_START = STAR_INDEX_END + 1;
const int BIG_STAR_INDEX_END = BIG_STAR_INDEX_START + BIG_STAR_COUNT_MAX - 1;


/************************************************************************
*    desc:  Constructer
************************************************************************/
CStarGenerator::CStarGenerator()
              : CGenerator(),
                GetRandStarPos( generator, IntDistribution( -(STAR_SECTOR_SIZE >> 1), (STAR_SECTOR_SIZE >> 1) ) ),
                GetRandStarScale( generator, FloatDistribution( STAR_SCALE_MIN, STAR_SCALE_MAX ) ),
                GetRandStarCount( generator, IntDistribution( STAR_COUNT_MIN, STAR_COUNT_MAX ) ),
                GetRandBigStarCount( generator, IntDistribution( BIG_STAR_COUNT_MIN, BIG_STAR_COUNT_MAX ) ),
                GetRandBigStarScale( generator, FloatDistribution( BIG_STAR_SCALE_MIN, BIG_STAR_SCALE_MAX ) )
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CStarGenerator::~CStarGenerator()
{
}	// destructer


/************************************************************************
*    desc:  Intialize the world generator
*
*	 param:	CPointInt & focus - which sector the player currently is in
*			uint wSeed		  - seed for generation
************************************************************************/
void CStarGenerator::Init( CPointInt & focus, uint wSeed )
{
    CInstanceMesh2D * pInstMesh = CInstanceMeshManager::Instance().GetInstanceMeshPtr( "(space)" );

    CObjectData2D * pSmallStarsObjData = CObjectDataList2D::Instance().GetData( "(space)", "stars" );
    CObjectData2D * pBigStarsObjData = CObjectDataList2D::Instance().GetData( "(space)", "big_star" );

    pUsedSectorVector.reserve( STAR_SECTOR_TOTAL );

    for( int i = 0; i < STAR_SECTOR_TOTAL; ++i )
    {
        // Create a sector
        CSector2D * pSector = new CSector2D();
        pUnusedSectorVector.push_back( pSector );
        spSectorVector.push_back( pSector );

        // Add the stars to the sector
        for( int j = 0; j < STAR_COUNT_MAX; ++j )
        {
            CSpriteGroup2D * pSpriteGrp = new CSpriteGroup2D( pSmallStarsObjData, true );
            pSpriteGrp->SetInstanceMesh( pInstMesh );
            pSector->AddSprite( pSpriteGrp );
        }

        // Add the stars to the sector
        for( int j = 0; j < BIG_STAR_COUNT_MAX; ++j )
        {
            CSpriteGroup2D * pSpriteGrp = new CSpriteGroup2D( pBigStarsObjData, true );
            pSpriteGrp->SetInstanceMesh( pInstMesh );
            pSector->AddSprite( pSpriteGrp );
        }
    }

    // Call the base class's init function as well
    CGenerator::Init( focus, wSeed );

}	// Init */


/************************************************************************
*    desc:  Handle the world generation for stars
*
*	 param: CPointInt & newFocus - the new focus position
*			CPointInt & focus    - the old focus position
*			bool forceGenerate   - forcefully generate the entire space
*								   background area	 
************************************************************************/
void CStarGenerator::HandleGeneration( const CPointInt & focus, const CPointInt & newFocus, bool forceGenerate )
{
    // Generate the space background
    Generate( focus, newFocus, forceGenerate );

    // Update the sprite vector with the newly generated sprites
    UpdateSpriteVector();

}   // HandleSpaceGeneration */


/************************************************************************
*    desc:  Main generation function. Determines if anything needs to
*			generate
*
*	 param: CPointInt & newFocus - the new focus position
*			CPointInt & focus    - the old focus position
*			bool forceGenerate   - forcefully generate the stars
************************************************************************/
void CStarGenerator::Generate( const CPointInt & focus, const CPointInt & newFocus, bool forceGenerate )
{
    // To forcefully generate our debris, we set up th generator as if the last focus was
    // far enough away to regenerate all sectors around the new focus
    if( forceGenerate )
    {
        // Change the old focus so we'll regenerate all sectors
        center = (center + STAR_SECTOR_DIMENSIONS) * STAR_SECTOR_SIZE;
        center.z = 0;

        // Move all the sectors to the unused sector vector
        pUnusedSectorVector.insert(pUnusedSectorVector.end(), pUsedSectorVector.begin(), pUsedSectorVector.end());
        pUsedSectorVector.clear();

        // Set the x value's starting point
        int x = center.x - ( STAR_SECTOR_DIMENSIONS >> 1 ) * STAR_SECTOR_SIZE;
        
        // Set the value we'll use to index into the unused sector vector
        int index = 0;

        // Set each sector's position to something outside our current focus
        for( int i = 0; i < STAR_SECTOR_DIMENSIONS; ++i )
        {
            // Set the y value's starting point
            int y = center.y - ( STAR_SECTOR_DIMENSIONS >> 1 ) * STAR_SECTOR_SIZE;

            for( int j = 0; j < STAR_SECTOR_DIMENSIONS; ++j )
            {
                // Set the new sector position
                pUnusedSectorVector[index]->SetPosition( CPointInt( x, y, STAR_DEPTH ) );

                y += STAR_SECTOR_SIZE; 
                ++index;
            }

            x += STAR_SECTOR_SIZE;
        }
    }

    // Find the amount of sectors we've moved
    CPointInt focusDiff = newFocus - center;

    // If the new focus is the same as the old one, no need to perform any changes
    if( abs(focusDiff.x) > (STAR_SECTOR_SIZE >> 1) || abs(focusDiff.y) > (STAR_SECTOR_SIZE >> 1) )
    {
        // Determine the new center
        CPointInt newCenter;
        int xMult, yMult;

        // Get the number generator sector multiples it takes to get to the player
        xMult = newFocus.x / STAR_SECTOR_SIZE;
        yMult = newFocus.y / STAR_SECTOR_SIZE;

        // Find the remainder of the divisions above
        int xMod = abs(newFocus.x) % STAR_SECTOR_SIZE;
        int yMod = abs(newFocus.y) % STAR_SECTOR_SIZE;

        // If the remainder is greater than half the sector size rounded down, we
        // add or subtract 1 to the multiple
        if( xMod > (STAR_SECTOR_SIZE >> 1) )
        {
            if( newFocus.x < 0 )
                xMult -= 1;
            else
                xMult += 1;
        }

        if( yMod > (STAR_SECTOR_SIZE >> 1) )
        {
            if( newFocus.y < 0 )
                yMult -= 1;
            else
                yMult += 1;
        }

        // Calculate the new center of the generator
        newCenter.x = xMult * STAR_SECTOR_SIZE;
        newCenter.y = yMult * STAR_SECTOR_SIZE;

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
            newSectorPos.z = STAR_DEPTH;
            (*sectorVecIter)->SetPosition( newSectorPos );

            // Set the seed for our background generation
            generator.seed( GetSectorSeed( newSectorPos ) );

            // Generate the elements of the stars
            GenerateStars( sectorVecIter );
            GenerateBigStars( sectorVecIter );

            // Move the sector from the unused vector to the used one
            pUsedSectorVector.push_back( (*sectorVecIter) );
            sectorVecIter = pUnusedSectorVector.erase( sectorVecIter );
        }

        // Set the new center
        center = newCenter;
    }

}	// Generate */


/************************************************************************
*    desc:  Generate the simple stars
*
*	 param: vector<CSector2D *>::iterator & sectorIter - sector to generate
*														 the stars in
************************************************************************/
void CStarGenerator::GenerateStars( vector<CSector2D *>::iterator & sectorIter )
{
    // Determine how many star clumps to create
    uint starCount = GEN_INFINITY;

    if( STAR_COUNT_RANGE > 0 )
        starCount = GetRandStarCount();

    // Generate the dust
    for( int i = STAR_INDEX_START; i <= STAR_INDEX_END; ++i )
    {
        // Get the sprite group to randomize 
        CSpriteGroup2D * pTmpGroup = (*sectorIter)->GetGroup(i);

        if( starCount > 0 )
        {
            // Make the dust visible
            pTmpGroup->SetVisible( true );

            // Randomize the rotation
            pTmpGroup->SetRot( CPoint( 0, 0, GetRandRot() ) );
            
            // Randomize the position
            CWorldPoint tmpPos;
            tmpPos.x.i = GetRandStarPos() + (*sectorIter)->GetPosition().x;
            tmpPos.y.i = GetRandStarPos() + (*sectorIter)->GetPosition().y;
            tmpPos.z.i = STAR_DEPTH;
            tmpPos.x.f = GetRandFloatPos();
            tmpPos.y.f = GetRandFloatPos();
            pTmpGroup->SetPos( tmpPos );

            // Randomize the scale
            float tmpScale = GetRandStarScale();
            pTmpGroup->SetScale( CPoint( tmpScale, tmpScale, 1 ) );

            // Call to get a random frame number
            RandIntGen GetRandFrame( generator, IntDistribution( 0, pTmpGroup->GetFrameCount() - 1 ) );

            // Randomize the frame number
            pTmpGroup->SetCurrentFrame( GetRandFrame() );

            starCount--;
        }
        else
            pTmpGroup->SetVisible( false );

        
    }

}	// GenerateStars */


/************************************************************************
*    desc:  Generate the simple stars
*
*	 param: vector<CSector2D *>::iterator & sectorIter - sector to generate
*														 the stars in
************************************************************************/
void CStarGenerator::GenerateBigStars( vector<CSector2D *>::iterator & sectorIter )
{
    // Determine how many star clumps to create
    uint starCount = GEN_INFINITY;

    if( BIG_STAR_COUNT_RANGE > 0 )
        starCount = GetRandBigStarCount();

    // Generate the dust
    for( int i = BIG_STAR_INDEX_START; i <= BIG_STAR_INDEX_END; ++i )
    {
        // Get the sprite group to randomize 
        CSpriteGroup2D * pTmpGroup = (*sectorVecIter)->GetGroup(i);

        if( starCount > 0 )
        {
            // Make the dust visible
            pTmpGroup->SetVisible( true );

            // Randomize the rotation
            pTmpGroup->SetRot( CPoint( 0, 0, GetRandRot() ) );
            
            // Randomize the position
            CWorldPoint tmpPos;
            tmpPos.x.i = GetRandStarPos() + (*sectorVecIter)->GetPosition().x;
            tmpPos.y.i = GetRandStarPos() + (*sectorVecIter)->GetPosition().y;
            tmpPos.z.i = STAR_DEPTH;
            tmpPos.x.f = GetRandFloatPos();
            tmpPos.y.f = GetRandFloatPos();
            pTmpGroup->SetPos( tmpPos );

            // Randomize the color
            CColor tmpColor;
            tmpColor.r = GetRandPercent();
            tmpColor.g = GetRandPercent();
            tmpColor.b = GetRandPercent();
            pTmpGroup->SetColor( tmpColor );

            // Randomize the scale
            float tmpScale = GetRandBigStarScale();
            pTmpGroup->SetScale( CPoint( tmpScale, tmpScale, 1 ) );

            // Call to get a random frame number
            RandIntGen GetRandFrame( generator, IntDistribution( 0, pTmpGroup->GetFrameCount() - 1 ) );

            // Randomize the frame number
            pTmpGroup->SetCurrentFrame( GetRandFrame() );

            starCount--;
        }
        else
            pTmpGroup->SetVisible( false );
    }

}	// GenerateBigStars */


/************************************************************************
*    desc:  Find the sectors out of our focus range and moves them to the
*			unused sector vector
*
*	 param: CPointInt & point - focus point
************************************************************************/
void CStarGenerator::OrganizeSectors( const CPointInt & point )
{
    // The full variance equation is: 
    int variance = STAR_SECTOR_SIZE;
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
void CStarGenerator::Clear()
{
    CGenerator::Clear();

}	// Clear */