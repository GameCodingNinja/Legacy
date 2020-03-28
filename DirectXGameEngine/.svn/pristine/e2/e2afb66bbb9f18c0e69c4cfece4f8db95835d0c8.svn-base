
/************************************************************************
*    FILE NAME:       generator2d.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include <2d/generator2d.h>

// Standard lib dependencies

// Boost lib dependencies

// Game lib dependencies
#include <common/matrix.h>
#include <common/worldpoint.h>
#include <common/pointint.h>
#include <2d/sector2d.h>
#include <2d/objectdata2d.h>
#include <2d/spritegroup2d.h>
#include <2d/sector2d.h>
#include <managers/instancemeshmanager.h>


/************************************************************************
*    desc:  Constructer
************************************************************************/
CGenerator2D::CGenerator2D( CObjectData2D * pObjData,
                            int sectorSize,
                            uint sectDimensions,
                            uint countMin,
                            uint countMax,
                            uint depthMin,
                            uint depthMax,
                            uint rotateMin,
                            uint rotateMax,
                            uint scaleMin,
                            uint scaleMax,
                            float colorMinR,
                            float colorMaxR,
                            float colorMinG,
                            float colorMaxG,
                            float colorMinB,
                            float colorMaxB,
                            float colorMinA,
                            float colorMaxA,
                            int colorGridSize,
                            int hueShiftMin,
                            int hueShiftMax,
                            EFrameDisplay frmDisplay,
                            uint seed,
                            CPointInt & focus,
                            bool instance )
            : GetRandRot( generator, FloatDistribution( rotateMin, rotateMax ) ),
              GetRandFloatPos( generator, FloatDistribution( -CWorldValue::HALF_SECTOR_SIZE, CWorldValue::HALF_SECTOR_SIZE ) ),
              GetRandColorR( colorGenerator, FloatDistribution( colorMinR, colorMaxR ) ),
              GetRandColorG( colorGenerator, FloatDistribution( colorMinG, colorMaxG ) ),
              GetRandColorB( colorGenerator, FloatDistribution( colorMinB, colorMaxB ) ),
              GetRandColorA( generator, FloatDistribution( colorMinA, colorMaxA ) ),
              GetRandCount( generator, IntDistribution( countMin, countMax ) ),
              GetRandIntPos( generator, IntDistribution( -(sectorSize >> 1), (sectorSize >> 1) ) ),
              GetRandDepth( generator, IntDistribution( depthMin, depthMax ) ),
              GetRandScale( generator, FloatDistribution( scaleMin, scaleMax ) ),
              GetRandHueShift( generator, IntDistribution( hueShiftMin, hueShiftMax ) ),
              frameDisplay(frmDisplay),
              SECTOR_SIZE(sectorSize),
              SECTOR_DIMENSIONS(sectDimensions),
              SECTOR_COUNT(sectDimensions * sectDimensions),
              SEED(seed),
              COUNT_MIN(countMin),
              COUNT_MAX(countMax),
              DEPTH_MIN(depthMin),
              DEPTH_MAX(depthMax),
              ROTATE_MIN(rotateMin),
              ROTATE_MAX(rotateMax),
              SCALE_MIN(scaleMin),
              SCALE_MAX(scaleMax),
              COLOR_MIN_R(colorMinR),
              COLOR_MAX_R(colorMaxR),
              COLOR_MIN_G(colorMinG),
              COLOR_MAX_G(colorMaxG),
              COLOR_MIN_B(colorMinB),
              COLOR_MAX_B(colorMaxB),
              COLOR_MIN_A(colorMinA),
              COLOR_MAX_A(colorMaxA),
              COLOR_GRID_SIZE(colorGridSize * sectorSize),
              INSTANCE(instance)
{
    // Init the generator
    Init( pObjData, focus );

}   // constructor


/************************************************************************
*    desc:  Constructer
************************************************************************/
CGenerator2D::~CGenerator2D()
{
}   // constructor


/************************************************************************
*    desc:  Init the generator
************************************************************************/
void CGenerator2D::Init( CObjectData2D * pObjData, CPointInt & focus )
{
    spSectorVec.reserve( SECTOR_COUNT );

    CInstanceMesh2D * pInstMesh(NULL);
        
    if( INSTANCE )
        pInstMesh = CInstanceMeshManager::Instance().GetInstanceMeshPtr( pObjData->GetGroup() );

    for( int i = 0; i < SECTOR_COUNT; ++i )
    {
        // Create a sector
        spSectorVec.push_back( new CSector2D() );

        // Add it to the unused sector vector as well to force organization
        pUnusedSectorVec.push_back( &spSectorVec.back() );

        // Add the sprite group to the sector. The sector will own it and do clean up.
        for( int j = 0; j < COUNT_MAX; ++j )
        {
            CSpriteGroup2D * pSpriteGrp = new CSpriteGroup2D( pObjData, true );

            spSectorVec.back().AddSprite( pSpriteGrp );

            if( INSTANCE )
                pSpriteGrp->SetInstanceMesh( pInstMesh );
        }
    }

    // Do a force generation on init
    Generate( focus, CPointInt(), true );

}	// Init


/************************************************************************
*    desc:  To forcefully generate our debris, we set up th generator 
*			as if the last focus was far enough away to regenerate all
*           sectors around the new focus.
************************************************************************/
void CGenerator2D::InvalidateSectors()
{
    // Change the old focus so we'll regenerate all sectors
    center = (center + SECTOR_DIMENSIONS) * SECTOR_SIZE;
    center.z = 0;

    // Move all the sectors to the unused sector vector
    pUnusedSectorVec.insert(pUnusedSectorVec.end(), pUsedSectorVec.begin(), pUsedSectorVec.end());
    pUsedSectorVec.clear();

    // Set the x value's starting point
    int x = center.x - ( SECTOR_DIMENSIONS >> 1 ) * SECTOR_SIZE;
        
    // Set the value we'll use to index into the unused sector vector
    int index = 0;

    // Set each sector's position to something outside our current focus
    for( int i = 0; i < SECTOR_DIMENSIONS; ++i )
    {
        // Set the y value's starting point
        int y = center.y - ( SECTOR_DIMENSIONS >> 1 ) * SECTOR_SIZE;

        for( int j = 0; j < SECTOR_DIMENSIONS; ++j )
        {
            // Set the new sector position
            pUnusedSectorVec[index]->SetPosition( CPointInt( x, y, DEPTH_MIN ) );

            y += SECTOR_SIZE; 
            ++index;
        }

        x += SECTOR_SIZE;
    }

}	// InvalidateSectors


/************************************************************************
*    desc:  Generate any sectors that become invalid
*
*	 param: CPointInt & newFocus - the new focus position
*			CPointInt & focus    - the old focus position
*			bool forceGenerate   - forcefully generate all the sectors
************************************************************************/
void CGenerator2D::Generate( const CPointInt & focus, const CPointInt & newFocus, bool forceGenerate )
{
    // To forcefully generate our debris, we set up th generator as if the last focus was
    // far enough away to regenerate all sectors around the new focus
    if( forceGenerate )
        InvalidateSectors();

    // Find the amount of sectors we've moved
    CPointInt focusDiff = newFocus - center;

    // If the new focus is the same as the old one, no need to perform any changes
    if( abs(focusDiff.x) > (SECTOR_SIZE >> 1) || abs(focusDiff.y) > (SECTOR_SIZE) >> 1 )
    {
        // Determine the new center
        CPointInt newCenter;
        int xMult, yMult;

        // Get the number generator sector multiples it takes to get to the player
        xMult = newFocus.x / SECTOR_SIZE;
        yMult = newFocus.y / SECTOR_SIZE;

        // Find the remainder of the divisions above
        int xMod = abs(newFocus.x) % SECTOR_SIZE;
        int yMod = abs(newFocus.y) % SECTOR_SIZE;

        // If the remainder is greater than half the sector size rounded down, we
        // add or subtract 1 to the multiple
        if( xMod > (SECTOR_SIZE >> 1) )
        {
            if( newFocus.x < 0 )
                xMult -= 1;
            else
                xMult += 1;
        }

        if( yMod > (SECTOR_SIZE >> 1) )
        {
            if( newFocus.y < 0 )
                yMult -= 1;
            else
                yMult += 1;
        }

        // Calculate the new center of the generator
        newCenter.x = xMult * SECTOR_SIZE;
        newCenter.y = yMult * SECTOR_SIZE;

        // Organize the sectors by ones inside the new focus range and ones outside
        // of the new focus range
        OrganizeSectors( newCenter );

        // Go through the sectors and figure out which ones we're not using anymore
        sectorVecIter = pUnusedSectorVec.begin();
        while( sectorVecIter != pUnusedSectorVec.end() )
        {
            // Calculate the sector's new location
            CPointInt newSectorPos = (*sectorVecIter)->GetPosition() - center;
            newSectorPos = newCenter + ( newSectorPos * -1 );
            newSectorPos.z = DEPTH_MIN;
            (*sectorVecIter)->SetPosition( newSectorPos );

            // Set the seed for our background generation
            uint sectorSeed = GetSectorSeed( newSectorPos );
            generator.seed( sectorSeed );
            colorGenerator.seed( sectorSeed );

            // Generate the elements of the background
            GenerateSector( sectorVecIter );

            // Move the sector from the unused vector to the used one
            pUsedSectorVec.push_back( (*sectorVecIter) );
            sectorVecIter = pUnusedSectorVec.erase( sectorVecIter );
        }

        // Set the new center
        center = newCenter;
    }

}	// Generate


/************************************************************************
*    desc:  Get the position of the color sector
************************************************************************/
CPointInt CGenerator2D::GetColorSectorPos( std::vector<CSector2D *>::iterator & sectorIter )
{
    // Find which dust color area this sector belongs to
    int x1, x2, y1, y2;
    CPointInt colorSectorPos;

    // Find the four color sectors surrounding the sector we're looking at
    // (x1,y1) (x1,y2) (x2,y1) (x2,y2)
    x1 = ( (*sectorIter)->GetPosition().x / COLOR_GRID_SIZE ) * COLOR_GRID_SIZE;

    if( (*sectorIter)->GetPosition().x > 0 )
        x2 = x1 + COLOR_GRID_SIZE;
    else
        x2 = x1 - COLOR_GRID_SIZE;

    y1 = ( (*sectorIter)->GetPosition().y / COLOR_GRID_SIZE ) * COLOR_GRID_SIZE;
    
    if( (*sectorIter)->GetPosition().y > 0 )
        y2 = y1 + COLOR_GRID_SIZE;
    else
        y2 = y1 - COLOR_GRID_SIZE;

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
*    desc:  Generate the sector
*
*	 param: vector<CSector2D *>::iterator & sectorIter - sector to generate
*														 the dust in
************************************************************************/
void CGenerator2D::GenerateSector( std::vector<CSector2D *>::iterator & sectorIter )
{
    uint colorSectorGridSeed(0);

    // Get the count to determine how many need to be generated
    int count = GetRandCount();

    // Use the color sector grid seed to generate the same color for the whole grid
    if( COLOR_GRID_SIZE > 0 )
        colorSectorGridSeed = GetSectorSeed( GetColorSectorPos( sectorIter ) );

    for( int i = 0; i < COUNT_MAX; ++i )
    {
        // Get the sprite group to randomize 
        CSpriteGroup2D * pSpriteGrp = (*sectorIter)->GetGroup( i );

        if( count > 0 )
        {
            // Make visible
            pSpriteGrp->SetVisible( true );

            // Randomize the rotation
            if( ROTATE_MIN != ROTATE_MAX )
                pSpriteGrp->SetRot( CPoint( 0, 0, GetRandRot() ) );
            else
                pSpriteGrp->SetRot( CPoint( 0, 0, ROTATE_MIN ) );
            
            // Randomize the position
            CWorldPoint pos;
            pos.x.i = GetRandIntPos() + (*sectorIter)->GetPosition().x;
            pos.y.i = GetRandIntPos() + (*sectorIter)->GetPosition().y;
            pos.z.i = GetRandDepth();
            pos.x.f = GetRandFloatPos();
            pos.y.f = GetRandFloatPos();
            pos.z.f = GetRandFloatPos();
            pSpriteGrp->SetPos( pos );

            // Randomise the new color and alpha
            CColor color = pSpriteGrp->GetColor();

            if( colorSectorGridSeed > 0 )
                colorGenerator.seed( colorSectorGridSeed );

            if( COLOR_MIN_R != COLOR_MAX_R )
                color.r = GetRandColorR();

            if( COLOR_MIN_G != COLOR_MAX_G )
                color.g = GetRandColorG();

            if( COLOR_MIN_B != COLOR_MAX_B )
                color.b = GetRandColorB();

            if( COLOR_MIN_A != COLOR_MAX_A )
                color.a = GetRandColorA();

            if( COLOR_GRID_SIZE > 0 )
            {
                float saturationShift = (float)DEPTH_MIN / (float)pos.z.i;

                color = color.TransformHSV( GetRandHueShift(), saturationShift, 1 );
            }

            pSpriteGrp->SetColor( color );

            // Randomize the scale
            if( SCALE_MIN != SCALE_MAX )
            {
                float scale = GetRandScale();
                pSpriteGrp->SetScale( CPoint( scale, scale, 1 ) );
            }
            else
                pSpriteGrp->SetScale( CPoint( SCALE_MIN, SCALE_MIN, 1 ) );

            // Set a random frame if that's how it needs to be displayed
            if( (pSpriteGrp->GetFrameCount() > 0) && (frameDisplay == EFD_RANDOM) )
            {
                RandIntGen GetRandFrame( generator, IntDistribution( 0, pSpriteGrp->GetFrameCount() - 1 ) );
                pSpriteGrp->SetCurrentFrame( GetRandFrame() );
            }
        }
        else
            pSpriteGrp->SetVisible( false );

        --count;
    }

}	// GenerateSector


/************************************************************************
*    desc:  Find the sectors out of our focus range and moves them to the
*			unused sector vector
*
*	 param: CPointInt & point - focus point
************************************************************************/
void CGenerator2D::OrganizeSectors( const CPointInt & point )
{
    // The full variance equation is: 
    int variance = SECTOR_SIZE;
    sectorVecIter = pUsedSectorVec.begin();

    // Go through the sectors and figure out which ones we're not using anymore
    while( sectorVecIter != pUsedSectorVec.end() )
    {
        // If we're not using them, we remove them
        if( ( (*sectorVecIter)->GetPosition().x > point.x + variance ) ||
            ( (*sectorVecIter)->GetPosition().x < point.x - variance ) ||
            ( (*sectorVecIter)->GetPosition().y > point.y + variance ) ||
            ( (*sectorVecIter)->GetPosition().y < point.y - variance ) )
        {
            pUnusedSectorVec.push_back( (*sectorVecIter) );
            sectorVecIter = pUsedSectorVec.erase( sectorVecIter );
        }
        else
            ++sectorVecIter;
    }

}	// OrganizeSectors


/************************************************************************
*    desc:  Transform the generator's components
************************************************************************/
void CGenerator2D::Transform()
{
    for( size_t i = 0; i < spSectorVec.size(); ++i )
        spSectorVec[i].Transform();

}	// Transform


/************************************************************************
*    desc:  Transform the generator's components
*
*	 param: const CMatrix & _scaledMatrix  - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point      - passed in relative position
************************************************************************/
void CGenerator2D::Transform( const CMatrix & _scaledMatrix,
                              const CMatrix & _unscaledMatrix, 
                              const CWorldPoint & point )
{
    for( size_t i = 0; i < spSectorVec.size(); ++i )
        spSectorVec[i].Transform( _scaledMatrix, _unscaledMatrix, point );
    
}	// Transform


/************************************************************************
*    desc:  Update the sprites in the generator
************************************************************************/
void CGenerator2D::Update()
{
    for( size_t i = 0; i < spSectorVec.size(); ++i )
        spSectorVec[i].Update();

}	// Update


/************************************************************************
*    desc:  Render the sprites in the generator
************************************************************************/
void CGenerator2D::Render()
{
    for( size_t i = 0; i < spSectorVec.size(); ++i )
    {
        if( frameDisplay == EFD_SEQUENTIALLY )
            spSectorVec[i].RenderAllFrames();
        else
            spSectorVec[i].Render();
    }

}	// Render


/************************************************************************
*    desc:  Get the seed to a particular sector. The process invovles 
*			calculating 8 values which will be converted int an unsigned
*			4 bit int and put together to make a 32 bit int
*
*	 param: const CPointInt & point - sector position to get a seed for
************************************************************************/
uint CGenerator2D::GetSectorSeed( const CPointInt & point )
{
    uint seed = 0;
    uint value = 0;
    seedPieceVec.clear();

    // Begin calculating each piece of the seed and inserting the value into the seed piece vector
    value = ( point.x * 4016331601 + ( (3871808917 + SEED) ^ SEED ) ) ^ SEED;
    seedPieceVec.push_back( value & 65535 );
    value = ( point.x * 3316090507 + ( (4016331601 + SEED) ^ SEED ) ) ^ SEED;
    seedPieceVec.push_back( value & 65535 );
    value = ( point.y * 2421215411 + ( (3316090507 + SEED) ^ SEED ) ) ^ SEED;
    seedPieceVec.push_back( value & 65535 );
    value = ( point.y * 1565016097 + ( (2421215411 + SEED) ^ SEED ) ) ^ SEED;
    seedPieceVec.push_back( value & 65535 );
    value = ( point.z * 3345900949 + ( (1565016097 + SEED) ^ SEED ) ) ^ SEED;
    seedPieceVec.push_back( value & 65535 );
    value = ( point.z * 3871808917 + ( (3345900949 + SEED) ^ SEED ) ) ^ SEED;
    seedPieceVec.push_back( value & 65535 );
    value = ( ( point.x * 3871808917 + (1565016097 ^ SEED) ) + 
              ( point.y * 3345900949 + (3871808917 ^ SEED) ) +
              ( point.z * 1565016097 + (3345900949 ^ SEED) ) ) ^ SEED;
    seedPieceVec.push_back( value & 65535 );
    value = ( ( point.x * 2421215411 + (3316090507 ^ SEED) ) + 
              ( point.y * 3316090507 + (4016331601 ^ SEED) ) +
              ( point.z * 4016331601 + (2421215411 ^ SEED) ) ) ^ SEED;
    seedPieceVec.push_back( value & 65535 );

    // Sort the vector
    std::sort( seedPieceVec.begin(), seedPieceVec.end() );

    // Take each piece, mod it by 16, and put each piece together to make a 32 bit int
    for( int i = 0; i < 7; ++i )
    {
        seed += seedPieceVec[i] & 15;
        seed <<= 4;
    }

    // Add the last piece to the seed
    seed += seedPieceVec[7] & 15;

    return seed;

}	// GetSectorSeed
