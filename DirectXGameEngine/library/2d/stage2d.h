/************************************************************************
*    FILE NAME:       stage2d.h
*
*    DESCRIPTION:     2D stage class
************************************************************************/

#ifndef __stage_2d_h__
#define __stage_2d_h__

// Standard lib dependencies
#include <string>
#include <map>
#include <vector>

// Game lib dependencies
#include <3d/worldcamera.h>
#include <common/matrix.h>

// Forward declarations
class CStageData;
class CSpriteGroup2D;
class CSector2D;

class CStage2D
{
public:

    // Get the instance of the singleton class
    static CStage2D & Instance()
    {
        static CStage2D stage;
        return stage;
    }

    // Load a stage
    void LoadStage( const std::string & name );

    // Transform the stage's components
    virtual void Transform();
    virtual void Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                            const CWorldPoint & point );

    // React the passed in sprites to the collision of their surrounding sectors
    void ReactToCollision( CSpriteGroup2D * pGroup );

    // Update the sectors
    void Update();
    void Update( const CPointInt & focusSector );

    // Renders the stage sprites
    void Render();
    void RenderAllSectors();

    // Add a sector to the stage
    void AddSector( CSector2D * pSector );

    // Get a sector from the stage
    CSector2D * GetSector( const CPointInt & sectorPos );

    // Find a sector from the stage
    bool FindSector( const CPointInt & sectorPos );

    // Remove a sector from the stage
    void RemoveSector( const CPointInt & sectorPos, bool clearSectorOnly = false );

    // Erase a sector from the stage's sector map
    void EraseSectorFromMap( const CPointInt & sectorPos );

    // Get the size of the sector map
    size_t GetSectorMapSize() const;

    // Clear all the stage data so that data can be loaded
    void ClearStage( bool clearSectorsOnly = false );

private:

    CStage2D();
    ~CStage2D();

private:

    // Data of the stage
    CStageData * pStageData;

    // Hash map of sectors for later loading
    std::map<CPointInt, CSector2D *> pSectorMap;

    // Define the iterator
    std::map<CPointInt, CSector2D *>::iterator sectorMapIter;

    // A vector of point ints that are added to the position of any
    // moving object to examine the sector its in as well as the sectors
    // surrounding it
    std::vector<CPointInt> surroundingSectorModVector;
};

#endif  // __stage_2d_h__