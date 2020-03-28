
/************************************************************************
*    FILE NAME:       debrisgenerator.h
*
*    DESCRIPTION:     Class template
************************************************************************/

#ifndef __debris_generator_h__
#define __debris_generator_h__

// Physical component dependency
#include "generator.h"

// Game lib dependencies
#include <common/pointint.h>
#include <common/defs.h>
#include <2d/visualsprite2d.h>

class CDebrisGenerator : public CGenerator
{
public:

    // Constructor
    CDebrisGenerator();

    // Destructor
    virtual ~CDebrisGenerator();

    // Initialize the world generator
    virtual void Init( CPointInt & focus, uint wSeed );

    // Handle the world generation for space
    virtual void HandleGeneration( const CPointInt & focus, const CPointInt & newFocus, bool forceGenerate = false );

    // Clear the generator
    void Clear();

private:

    // Generate the stars
    bool Generate( const CPointInt & pos, const CPointInt & newFocus, bool forceGenerate = false );
    void GenerateDebris( std::vector<CSector2D *>::iterator & sectorIter );

    // Find the sectors out of our focus range and moves them to the unused sector vector
    virtual void OrganizeSectors( const CPointInt & point );

private:

    // The center of the background
    CPointInt center;

    // Boost random number generators
    RandIntGen GetRandDebrisPos;
    RandFloatGen GetRandDebrisScale;
    RandIntGen GetRandDebrisCount;

};

#endif  // __debris_generator_h__

