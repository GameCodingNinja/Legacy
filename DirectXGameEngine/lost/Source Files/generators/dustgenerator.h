
/************************************************************************
*    FILE NAME:       dustgenerator.h
*
*    DESCRIPTION:     Generator class to generate the space dust
************************************************************************/

#ifndef __dust_generator_h__
#define __dust_generator_h__

// Physical dependencies
#include "generator.h"

// Game lib dependencies
#include <common/pointint.h>
#include <common/defs.h>
#include <3d/worldcamera.h>

class CDustGenerator : public CGenerator
{
public:

    // Constructor
    CDustGenerator();

    // Destructor
    virtual ~CDustGenerator();

    // Initialize the generator
    virtual void Init( CPointInt & focus, uint wSeed );

    // Handle the generation
    virtual void HandleGeneration( const CPointInt & focus, const CPointInt & newFocus, bool forceGenerate = false );

    // Clear the generator
    void Clear();

private:

    // Main generation function. Determines if anything needs to generate
    bool Generate( const CPointInt & pos, const CPointInt & newFocus, bool forceGenerate = false );

    // Individual generation functions
    void GenerateDust( std::vector<CSector2D *>::iterator & sectorIter );
    void GenerateDustColor( std::vector<CSector2D *>::iterator & sectorIter );

    // Find the sectors out of our focus range and moves them to the unused sector vector
    virtual void OrganizeSectors( const CPointInt & point );

    // Get the position of the color sector
    CPointInt GetColorSectorPos( std::vector<CSector2D *>::iterator & sectorIter );

private:

    // The current center of the dust generator
    CPointInt center;

    // Generator just for the color
    BaseRandGenType colorGenerator;

    // Boost random number generators
    RandIntGen GetRandDustPos;
    RandIntGen GetRandDustDepth;
    RandFloatGen GetRandDustScale;
    RandIntGen GetRandDustCount;
    RandFloatGen GetRandDustColor;
    RandIntGen GetRandHueShift;

};

#endif  // __dust_generator_h__
