
/************************************************************************
*    FILE NAME:       stargenerator.h
*
*    DESCRIPTION:     Class template
************************************************************************/

#ifndef __star_generator_h__
#define __star_generator_h__

// Physical component dependency
#include "generator.h"

// Game lib dependencies
#include <common/pointint.h>
#include <common/defs.h>


class CStarGenerator : public CGenerator
{
public:

    // Constructor
    CStarGenerator();

    // Destructor
    virtual ~CStarGenerator();

    // Initialize the world generator
    virtual void Init( CPointInt & focus, uint wSeed );

    // Handle the world generation for space
    virtual void HandleGeneration( const CPointInt & focus, const CPointInt & newFocus, bool forceGenerate = false );

    // Clear the generator
    void Clear();

private:

    // Generate the stars
    void Generate( const CPointInt & pos, const CPointInt & newFocus, bool forceGenerate = false );
    void GenerateStars( std::vector<CSector2D *>::iterator & sectorIter );
    void GenerateBigStars( std::vector<CSector2D *>::iterator & sectorIter );

    // Find the sectors out of our focus range and moves them to the unused sector vector
    virtual void OrganizeSectors( const CPointInt & point );

private:

    // The center of the background
    CPointInt center;

    // Boost random number generators
    RandIntGen GetRandStarPos;
    RandFloatGen GetRandStarScale;
    RandIntGen GetRandStarCount;
    RandIntGen GetRandBigStarCount;
    RandFloatGen GetRandBigStarScale;

};

#endif  // __star_generator_h__


