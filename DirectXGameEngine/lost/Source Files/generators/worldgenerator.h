
/************************************************************************
*    FILE NAME:       worldgenerator.h
*
*    DESCRIPTION:     Singleton to handle world generation
************************************************************************/

#ifndef __world_generator_h__
#define __world_generator_h__

// Standard lib dependencies
#include <map>
#include <vector>
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>

// Game lib dependencies
#include <common/pointint.h>
#include <common/defs.h>
#include <3d/worldcamera.h>

// Game dependencies
#include "debrisgenerator.h"
#include "dustgenerator.h"
#include "stargenerator.h"
#include "planetgenerator.h"

// Forward declaration(s)
class CSector2D;
class CSpriteGroup2D;

class CWorldGenerator
{
public:

    // Get the instance of the singleton class
    static CWorldGenerator & Instance()
    {
        static CWorldGenerator worldGen;
        return worldGen;
    }

    // Initialize the world generator
    void Init();

    // Transform the world generator's visual components
    virtual void Transform();
    virtual void Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                            const CWorldPoint & point );

    // Update the sectors in the world
    void Update();

    // Render the sectors in the world
    void Render();

    // Handle the world generation for space
    void HandleSpaceGeneration( bool forceGenerate = false );

    // Clear the contents of the generator
    void Clear();

private:

    CWorldGenerator();
    ~CWorldGenerator();

    // The generators
    CStarGenerator starGen;
    CPlanetGenerator planetGen;
    CDustGenerator dustGen;
    CDebrisGenerator debrisGen;

    // Vector of generators
    boost::ptr_vector< CGenerator > generator;

    // The sector the player is in
    CPointInt focus;
};

#endif  // __world_generator_h__

