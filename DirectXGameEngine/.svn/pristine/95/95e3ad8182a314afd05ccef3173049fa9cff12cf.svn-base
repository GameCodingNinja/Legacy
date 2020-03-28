
/************************************************************************
*    FILE NAME:       generator.h
*
*    DESCRIPTION:     Generator base class
************************************************************************/

#ifndef __generator_h__
#define __generator_h__

// Standard lib dependencies
#include <vector>

// Boost lib dependencies
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>

// Game lib dependencies
#include <common/pointint.h>
#include <common/defs.h>
#include <common/worldpoint.h>
#include <common/matrix.h>

// Forward declaration(s)
class CSector2D;
class CSpriteGroup2D;

// Typedefs for changing the random generation
typedef boost::minstd_rand BaseRandGenType;
typedef boost::uniform_int<> IntDistribution;
typedef boost::uniform_real<float> FloatDistribution;
typedef boost::variate_generator<BaseRandGenType&, IntDistribution> RandIntGen;
typedef boost::variate_generator<BaseRandGenType&, FloatDistribution> RandFloatGen;

class CGenerator
{
public:

    // Constructor
    CGenerator();

    // Destructor
    virtual ~CGenerator();

    // Initialize the generator
    virtual void Init( CPointInt & focus, uint wSeed );

    // Update the sprites in the generator
    virtual void Update();

    // Update the sprite vector with the sprites in the sectors and sort
    // them by furthest z value
    virtual void UpdateSpriteVector();

    // Transform the generator's components
    virtual void Transform();
    virtual void Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                            const CWorldPoint & point );

    // Render the sprites in the generator
    virtual void Render();

    // Handle the generation
    virtual void HandleGeneration( const CPointInt & focus, const CPointInt & newFocus, bool forceGenerate = false ) = 0;

    // Clear the contents of the generator
    virtual void Clear();

protected:

    // Get the seed to a particular sector 
    uint GetSectorSeed( const CPointInt & point );

    // Find the sectors out of our focus range and moves them to the unused sector vector
    virtual void OrganizeSectors( const CPointInt & point ) = 0;

protected:

    // A map of sector vectors
    std::vector<CSector2D *> pUsedSectorVector;
    std::vector<CSector2D *> pUnusedSectorVector;
    boost::ptr_vector<CSector2D> spSectorVector;
    std::vector<CSector2D *>::iterator sectorVecIter;

    // The vector of sprites to render and update. These sprites don't belong to the generator
    std::vector<CSpriteGroup2D *> pSpriteVector;

    // The seed for the world
    uint worldSeed;

    // Used to generate a sector seed
    std::vector<uint> seedPieceVector;

    // Boost random number generators
    BaseRandGenType generator;
    RandFloatGen GetRandRot;
    RandFloatGen GetRandFloatPos;
    RandFloatGen GetRandPercent;
    RandIntGen GetRandBool;
};

#endif  // __generator_h__
