/************************************************************************
*    FILE NAME:       worldmanager.h
*
*    DESCRIPTION:     Class to manage the world generation
************************************************************************/

#ifndef __world_manager_h__
#define __world_manager_h__

// Standard lib dependencies
//#include <vector>

// Game lib dependencies
#include "2d\\spritegroup2d.h"
#include "common\\pointint.h"

// Forward declarations
class CSpriteGroup2D;
class CSector2D;

class CWorldManager
{
public:

    CWorldManager();
    ~CWorldManager();

    // Update the world
    void Update( CPointInt & pos );

    // React the passed in sprite group to the stage's collision
    void ReactToCollision( CSpriteGroup2D * pGroup );

    // Render the world
    void Render();

    // Check to see if our sector position has changed so that we
    // can generate new sectors
    void ManageGeneration( CPointInt & pos );

    // Generate the sprites that will make up the sector
    void GenerateSector( CSector2D * pSector );

    // Initialize the world manager
    void Init();

protected:

    // The sector index that the world manager focuses on
    CPointInt focusSectorIndex;

    // The number of terrain pieces it takes to fill up one dimension of a sector
    int terrainDimCount;

};

#endif  // __world_manager_h__