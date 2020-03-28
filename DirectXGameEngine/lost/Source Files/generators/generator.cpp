/************************************************************************
*    FILE NAME:       generator.cpp
*
*    DESCRIPTION:     Generator base class
************************************************************************/           

// Physical dependency
#include "generator.h"

// Game lib dependencies
#include <2d/spritegroup2d.h>
#include <2d/sector2d.h>
#include <utilities/genfunc.h>
#include <utilities/sortfunc.h>
#include <common/worldvalue.h>


/************************************************************************
*    desc:  Constructer
************************************************************************/
CGenerator::CGenerator()
          : worldSeed(0),
            GetRandRot( generator, FloatDistribution( 0, 360 ) ),
            GetRandFloatPos( generator, FloatDistribution( -CWorldValue::HALF_SECTOR_SIZE, CWorldValue::HALF_SECTOR_SIZE ) ),
            GetRandPercent( generator, FloatDistribution( 0, 1 ) ),
            GetRandBool( generator, IntDistribution( 0, 1 ) )	
{
    seedPieceVector.reserve(8);

}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CGenerator::~CGenerator()
{
    Clear();

}	// destructer


/************************************************************************
*    desc:  Intialize the world generator
************************************************************************/
void CGenerator::Init( CPointInt & focus, uint wSeed )
{
    // Set the world seed
    worldSeed = wSeed;

    // Do a force generation
    HandleGeneration( focus, CPointInt(), true );

}	// Init */


/************************************************************************
*    desc:  Transform the generator's components
************************************************************************/
void CGenerator::Transform()
{
    for( size_t i = 0; i < pUsedSectorVector.size(); ++i )
        pUsedSectorVector[i]->Transform();

}	// Transform */


/************************************************************************
*    desc:  Transform the generator's visual components
*
*	 param: const CMatrix & _scaledMatrix  - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point      - passed in relative position
************************************************************************/
void CGenerator::Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                            const CWorldPoint & point )
{
    for( size_t i = 0; i < pUsedSectorVector.size(); ++i )
        pUsedSectorVector[i]->Transform( _scaledMatrix, _unscaledMatrix, point );
    
}	// Transform */


/************************************************************************
*    desc:  Update the sprites in the generator
************************************************************************/
void CGenerator::Update()
{
    for( size_t i = 0; i < pSpriteVector.size(); ++i )
        pSpriteVector[i]->Update();

}	// Update */


/************************************************************************
*    desc:  Update the sprite vector with the sprites in the sectors
*			and sort them by furthest z value
*
*	 NOTE:	This function should only be called when there has been a
*			change in the focus sector
************************************************************************/
void CGenerator::UpdateSpriteVector()
{
    pSpriteVector.clear();

    for( size_t i = 0; i < pUsedSectorVector.size(); ++i )
    {
        if( pUsedSectorVector[i]->IsVisible() )
            pSpriteVector.insert( pSpriteVector.begin(), 
                                  pUsedSectorVector[i]->GetGroupVector().begin(),
                                  pUsedSectorVector[i]->GetGroupVector().end() );
    }

    sort( pSpriteVector.begin(), pSpriteVector.end(), NSortFunc::SpriteGroup2DSort );

}	// UpdateSpriteVector */


/************************************************************************
*    desc:  Render the sprites in the generator
************************************************************************/
void CGenerator::Render()
{
    for( size_t i = 0; i < pSpriteVector.size(); ++i )
        pSpriteVector[i]->Render();

}	// Render */


/************************************************************************
*    desc:  Clear the contents of the generator
************************************************************************/
void CGenerator::Clear()
{
    pUsedSectorVector.clear();
    pUnusedSectorVector.clear();
    spSectorVector.clear();
    pSpriteVector.clear();
    seedPieceVector.clear();

}	// Clear */


/************************************************************************
*    desc:  Get the seed to a particular sector. The process invovles 
*			calculating 8 values which will be converted int an unsigned
*			4 bit int and put together to make a 32 bit int
*
*	 param: const CPointInt & point - sector position to get a seed for
************************************************************************/
uint CGenerator::GetSectorSeed( const CPointInt & point )
{
    uint seed = 0;
    uint value = 0;
    seedPieceVector.clear();

    // Begin calculating each piece of the seed and inserting the value into the seed piece vector
    value = ( point.x * 4016331601 + ( (3871808917 + worldSeed) ^ worldSeed ) ) ^ worldSeed;
    seedPieceVector.push_back( value & 65535 );
    value = ( point.x * 3316090507 + ( (4016331601 + worldSeed) ^ worldSeed ) ) ^ worldSeed;
    seedPieceVector.push_back( value & 65535 );
    value = ( point.y * 2421215411 + ( (3316090507 + worldSeed) ^ worldSeed ) ) ^ worldSeed;
    seedPieceVector.push_back( value & 65535 );
    value = ( point.y * 1565016097 + ( (2421215411 + worldSeed) ^ worldSeed ) ) ^ worldSeed;
    seedPieceVector.push_back( value & 65535 );
    value = ( point.z * 3345900949 + ( (1565016097 + worldSeed) ^ worldSeed ) ) ^ worldSeed;
    seedPieceVector.push_back( value & 65535 );
    value = ( point.z * 3871808917 + ( (3345900949 + worldSeed) ^ worldSeed ) ) ^ worldSeed;
    seedPieceVector.push_back( value & 65535 );
    value = ( ( point.x * 3871808917 + (1565016097 ^ worldSeed) ) + 
              ( point.y * 3345900949 + (3871808917 ^ worldSeed) ) +
              ( point.z * 1565016097 + (3345900949 ^ worldSeed) ) ) ^ worldSeed;
    seedPieceVector.push_back( value & 65535 );
    value = ( ( point.x * 2421215411 + (3316090507 ^ worldSeed) ) + 
              ( point.y * 3316090507 + (4016331601 ^ worldSeed) ) +
              ( point.z * 4016331601 + (2421215411 ^ worldSeed) ) ) ^ worldSeed;
    seedPieceVector.push_back( value & 65535 );

    // Sort the vector
    sort( seedPieceVector.begin(), seedPieceVector.end() );

    // Take each piece, mod it by 16, and put each piece together to make a 32 bit int
    for( int i = 0; i < 7; ++i )
    {
        seed += seedPieceVector[i] & 15;
        seed <<= 4;
    }

    // Add the last piece to the seed
    seed += seedPieceVector[7] & 15;

    return seed;

}	// GetSectorSeed */