/************************************************************************
*    FILE NAME:       dustgenerator.cpp
*
*    DESCRIPTION:     Generator class to generate the space dust
************************************************************************/           

// Physical dependency
#include "dustgenerator.h"

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <2d/spritegroup2d.h>
#include <2d/sector2d.h>
#include <common/uv.h>
#include <managers/instancemeshmanager.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)
// Disable warning about the use of the "this" pointer in the constructor list
#pragma warning(disable : 4355)

// The sprite count constants
const int DUST_COUNT = 10;
const int DUST_COUNT_MIN = 3;
const int DUST_COUNT_MAX = DUST_COUNT;

// The index starts and ends of particular sprites. This way we can easily
// get what we want out of a sector's vector of sprites
const int DUST_INDEX_START = 0;
const int DUST_INDEX_END = DUST_INDEX_START + DUST_COUNT;

// Sector constants
const int DUST_SECTOR_SIZE = 121;
const int DUST_SECTOR_DIMENSIONS = 3;
const int DUST_SECTOR_TOTAL = DUST_SECTOR_DIMENSIONS * DUST_SECTOR_DIMENSIONS;

// Depths of the dust
const int DUST_DEPTH_MIN = 30;
const int DUST_DEPTH_MAX = 90;

// The min and max scale of the dust
const int DUST_SCALE_MIN = 80;
const int DUST_SCALE_MAX = 140;

// The diameter of influence a color sector has. This must be an odd number
const int DUST_COLOR_DIAMETER = DUST_SECTOR_SIZE * 3;

// The min and max color of a dust cloud as well the min and max multiplicative mod
const float DUST_COLOR_MIN = 0.0f;
const float DUST_COLOR_MAX = 1.0f;
//const float DUST_COLOR_MOD_MIN = 0.8f;
//const float DUST_COLOR_MOD_MAX = 1.2f;


/************************************************************************
*    desc:  Constructer
************************************************************************/
CDustGenerator::CDustGenerator()
               : CGenerator(),
                 GetRandDustPos( generator, IntDistribution( -(DUST_SECTOR_SIZE >> 1), (DUST_SECTOR_SIZE >> 1) ) ),
                 GetRandDustDepth( generator, IntDistribution( DUST_DEPTH_MIN, DUST_DEPTH_MAX ) ),
                 GetRandDustScale( generator, FloatDistribution( DUST_SCALE_MIN, DUST_SCALE_MAX ) ),
                 GetRandDustCount( generator, IntDistribution( DUST_COUNT_MIN, DUST_COUNT_MAX ) ),
                 GetRandDustColor( colorGenerator, FloatDistribution( DUST_COLOR_MIN, DUST_COLOR_MAX ) ),
                 GetRandHueShift( generator, IntDistribution( -45, 45 ) )
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CDustGenerator::~CDustGenerator()
{
}	// Destructer


/************************************************************************
*    desc:  Initialize the generator
*
*	 param:	CPointInt & focus - which sector the player currently is in
*			uint wSeed		  - seed for generation
************************************************************************/
void CDustGenerator::Init( CPointInt & focus, uint wSeed )
{
    CInstanceMesh2D * pInstMesh = CInstanceMeshManager::Instance().GetInstanceMeshPtr( "(space)" );
    CObjectData2D * pObjData = CObjectDataList2D::Instance().GetData( "(space)", "dust" );

    pUsedSectorVector.reserve( DUST_SECTOR_TOTAL );

    for( int i = 0; i < DUST_SECTOR_TOTAL; ++i )
    {
        // Create a sector
        CSector2D * pSector = new CSector2D();
        pUnusedSectorVector.push_back( pSector );
        spSectorVector.push_back( pSector );

        // Add the dust sprites to the sector
        for( int j = 0; j < DUST_COUNT_MAX; ++j )
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
void CDustGenerator::HandleGeneration( const CPointInt & focus, const CPointInt & newFocus, bool forceGenerate )
{
    // Generate, and if anything was generated then update the sprite vector 
    // with the newly generated sprites
    if( Generate( focus, newFocus, forceGenerate ) )
        UpdateSpriteVector();

}   // HandleGeneration */


/************************************************************************
*    desc:  Main generation function. Determines if anything needs to
*			generate
*
*	 param: CPointInt & newFocus - the new focus position
*			CPointInt & focus    - the old focus position
*			bool forceGenerate   - forcefully generate the stars
*
*	 ret:	bool - if anything was generated
************************************************************************/
bool CDustGenerator::Generate( const CPointInt & focus, const CPointInt & newFocus, bool forceGenerate )
{
    // To forcefully generate, we set up th generator as if the last focus was
    // far enough away to regenerate all sectors around the new focus
    if( forceGenerate )
    {
        // Change the old focus so we'll regenerate all sectors
        center = (center + DUST_SECTOR_DIMENSIONS) * DUST_SECTOR_SIZE;
        center.z = 0;

        // Move all the sectors to the unused sector vector
        pUnusedSectorVector.insert(pUnusedSectorVector.end(), pUsedSectorVector.begin(), pUsedSectorVector.end());
        pUsedSectorVector.clear();

        // Set the x value's starting point
        int x = center.x - ( DUST_SECTOR_DIMENSIONS >> 1 ) * DUST_SECTOR_SIZE;
        
        // Set the value we'll use to index into the unused sector vector
        int index = 0;

        // Set each sector's position to something outside our current focus
        for( int i = 0; i < DUST_SECTOR_DIMENSIONS; ++i )
        {
            // Set the y value's starting point
            int y = center.y - ( DUST_SECTOR_DIMENSIONS >> 1 ) * DUST_SECTOR_SIZE;

            for( int j = 0; j < DUST_SECTOR_DIMENSIONS; ++j )
            {
                // Set the new sector position
                pUnusedSectorVector[index]->SetPosition( CPointInt( x, y, DUST_DEPTH_MIN ) );

                y += DUST_SECTOR_SIZE; 
                ++index;
            }

            x += DUST_SECTOR_SIZE;
        }
    }

    // Find the amount of sectors we've moved
    CPointInt focusDiff = newFocus - center;

    // If the new focus is the same as the old one, no need to perform any changes
    if( abs(focusDiff.x) > (DUST_SECTOR_SIZE >> 1) || abs(focusDiff.y) > (DUST_SECTOR_SIZE >> 1) )
    {
        // Determine the new center
        CPointInt newCenter;
        int xMult, yMult;

        // Get the number generator sector multiples it takes to get to the player
        xMult = newFocus.x / DUST_SECTOR_SIZE;
        yMult = newFocus.y / DUST_SECTOR_SIZE;

        // Find the remainder of the divisions above
        int xMod = abs(newFocus.x) % DUST_SECTOR_SIZE;
        int yMod = abs(newFocus.y) % DUST_SECTOR_SIZE;

        // If the remainder is greater than half the sector size rounded down, we
        // add or subtract 1 to the multiple
        if( xMod > (DUST_SECTOR_SIZE >> 1) )
        {
            if( newFocus.x < 0 )
                xMult -= 1;
            else
                xMult += 1;
        }

        if( yMod > (DUST_SECTOR_SIZE >> 1) )
        {
            if( newFocus.y < 0 )
                yMult -= 1;
            else
                yMult += 1;
        }

        // Calculate the new center of the generator
        newCenter.x = xMult * DUST_SECTOR_SIZE;
        newCenter.y = yMult * DUST_SECTOR_SIZE;

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
            newSectorPos.z = DUST_DEPTH_MIN;
            (*sectorVecIter)->SetPosition( newSectorPos );

            // Set the seed for our background generation
            generator.seed( GetSectorSeed( newSectorPos ) );

            // Generate the elements of the background
            GenerateDust( sectorVecIter );

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
*    desc:  Generate the space dust
*
*	 param: vector<CSector2D *>::iterator & sectorIter - sector to generate
*														 the dust in
************************************************************************/
void CDustGenerator::GenerateDust( vector<CSector2D *>::iterator & sectorIter )
{
    // Get a seed to determine if we have debris to create
    int dustCount = GetRandDustCount();

    // Set the generator seed back to the sector we're in
    uint colorSectorSeed = GetSectorSeed( GetColorSectorPos( sectorIter ) );

    // Generate the dust
    for( int i = DUST_INDEX_START; i < DUST_INDEX_END; ++i )
    {
        // Get the sprite group to randomize 
        CSpriteGroup2D * pTmpGroup = (*sectorIter)->GetGroup(i);

        if( dustCount > 0 )
        {
            // Make the dust visible
            pTmpGroup->SetVisible( true );

            // Randomize the rotation
            pTmpGroup->SetRot( CPoint( 0, 0, GetRandRot() ) );
            
            // Randomize the position
            CWorldPoint tmpPos;
            tmpPos.x.i = GetRandDustPos() + (*sectorIter)->GetPosition().x;
            tmpPos.y.i = GetRandDustPos() + (*sectorIter)->GetPosition().y;
            tmpPos.z.i = GetRandDustDepth();
            tmpPos.x.f = GetRandFloatPos();
            tmpPos.y.f = GetRandFloatPos();
            tmpPos.z.f = GetRandFloatPos();
            pTmpGroup->SetPos( tmpPos );

            // Randomize the scale
            float tmpScale = GetRandDustScale();
            pTmpGroup->SetScale( CPoint( tmpScale, tmpScale, 1 ) );

            // Call to get a random frame number
            RandIntGen GetRandFrame( generator, IntDistribution( 0, pTmpGroup->GetFrameCount() - 1 ) );

            // Randomize the frame number
            pTmpGroup->SetCurrentFrame( GetRandFrame() );

            // Reset the color sector seed
            colorGenerator.seed( colorSectorSeed );

            // Now we generate the color
            CColor color;
            color.r = GetRandDustColor();
            color.g = GetRandDustColor();
            color.b = GetRandDustColor();
            color.a = 0.9f;

            float saturationShift = (float)DUST_DEPTH_MIN / (float)tmpPos.z.i;

            color = color.TransformHSV( GetRandHueShift(), saturationShift, 1 );

            pTmpGroup->SetColor( color );
        }
        else
            pTmpGroup->SetVisible( false );

        dustCount--;
    }

    // Generate the color of the dust. We do this here because otherwise 
    // we'd keep reseting the seed between the sector and color sector, 
    // thus making our generation far less random
    //GenerateDustColor( sectorIter );

}	// GenerateDust */


/************************************************************************
*    desc:  Get the position of the color sector
************************************************************************/
CPointInt CDustGenerator::GetColorSectorPos( std::vector<CSector2D *>::iterator & sectorIter )
{
    // Find which dust color area this sector belongs to
    int x1, x2, y1, y2;
    CPointInt colorSectorPos;

    // Find the four color sectors surrounding the sector we're looking at
    // (x1,y1) (x1,y2) (x2,y1) (x2,y2)
    x1 = ( (*sectorIter)->GetPosition().x / DUST_COLOR_DIAMETER ) * DUST_COLOR_DIAMETER;

    if( (*sectorIter)->GetPosition().x > 0 )
        x2 = x1 + DUST_COLOR_DIAMETER;
    else
        x2 = x1 - DUST_COLOR_DIAMETER;

    y1 = ( (*sectorIter)->GetPosition().y / DUST_COLOR_DIAMETER ) * DUST_COLOR_DIAMETER;
    
    if( (*sectorIter)->GetPosition().y > 0 )
        y2 = y1 + DUST_COLOR_DIAMETER;
    else
        y2 = y1 - DUST_COLOR_DIAMETER;

    // Figure out which of the values are closest to determine which color sector we're in
    int value1, value2;
    value1 = abs( x1 - (*sectorIter)->GetPosition().x );
    value2 = abs( x2 - (*sectorIter)->GetPosition().x );

    if( value1 < value2 )
        colorSectorPos.x = x1;
    else
        colorSectorPos.x = x2;

    value1 = abs( y1 - (*sectorIter)->GetPosition().y );
    value2 = abs( y2 - (*sectorIter)->GetPosition().y );

    if( value1 < value2 )
        colorSectorPos.y = y1;
    else
        colorSectorPos.y = y2;

    // We've found our color sector position so now we use it as a seed to generate the colors
    return colorSectorPos;

}	// GetColorSectorPos


/************************************************************************
*    desc:  Generate the space dust's color
*
*	 param: vector<CSector2D *>::iterator & sectorIter - sector to generate
*														 the dusts' color
************************************************************************/
void CDustGenerator::GenerateDustColor( vector<CSector2D *>::iterator & sectorIter )
{
    // Find which dust color area this sector belongs to
    int x1, x2, y1, y2;
    CPointInt colorSectorPos;

    // Find the four color sectors surrounding the sector we're looking at
    // (x1,y1) (x1,y2) (x2,y1) (x2,y2)
    x1 = ( (*sectorVecIter)->GetPosition().x / DUST_COLOR_DIAMETER ) * DUST_COLOR_DIAMETER;

    if( (*sectorVecIter)->GetPosition().x > 0 )
        x2 = x1 + DUST_COLOR_DIAMETER;
    else
        x2 = x1 - DUST_COLOR_DIAMETER;

    y1 = ( (*sectorVecIter)->GetPosition().y / DUST_COLOR_DIAMETER ) * DUST_COLOR_DIAMETER;
    
    if( (*sectorVecIter)->GetPosition().y > 0 )
        y2 = y1 + DUST_COLOR_DIAMETER;
    else
        y2 = y1 - DUST_COLOR_DIAMETER;

    // Figure out which of the values are closest to determine which color sector we're in
    int value1, value2;
    value1 = abs( x1 - (*sectorVecIter)->GetPosition().x );
    value2 = abs( x2 - (*sectorVecIter)->GetPosition().x );

    if( value1 < value2 )
        colorSectorPos.x = x1;
    else
        colorSectorPos.x = x2;

    value1 = abs( y1 - (*sectorVecIter)->GetPosition().y );
    value2 = abs( y2 - (*sectorVecIter)->GetPosition().y );

    if( value1 < value2 )
        colorSectorPos.y = y1;
    else
        colorSectorPos.y = y2;

    // We've found our color sector position so now we use it as a seed to generate the colors
    uint test = GetSectorSeed( colorSectorPos );
    generator.seed( test );

    // Now we generate the color
    CColor sectorColor;
    sectorColor.r = GetRandDustColor();
    sectorColor.g = GetRandDustColor();
    sectorColor.b = GetRandDustColor();
    sectorColor.a = 1;

    // Set the generator seed back to the sector we're in
    generator.seed( GetSectorSeed( (*sectorIter)->GetPosition() ) );

    // RGB mod values
    /*CColor tmpColor = sectorColor;

    // Calculate the final colors for the dust
    for( int i = DUST_INDEX_START; i < DUST_INDEX_END; ++i )
    {
        CSpriteGroup2D * pTmpSprite = (*sectorIter)->GetGroup(i);

        if( pTmpSprite->IsVisible() )
        {
            tmpColor.r *= GetRandDustColorMod();
            tmpColor.g *= GetRandDustColorMod();
            tmpColor.b *= GetRandDustColorMod();
            tmpColor.a = pTmpSprite->GetVisualData().GetColor().a;

            pTmpSprite->SetColor( tmpColor );
        }
    }*/

}	// GenerateDustColor */


/************************************************************************
*    desc:  Find the sectors out of our focus range and moves them to the
*			unused sector vector
*
*	 param: const CPointInt & point - focus point
************************************************************************/
void CDustGenerator::OrganizeSectors( const CPointInt & point )
{
    // The full variance equation is: 
    int variance = DUST_SECTOR_SIZE;
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
void CDustGenerator::Clear()
{
    CGenerator::Clear();

}	// Clear */