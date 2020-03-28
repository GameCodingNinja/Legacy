
/************************************************************************
*    FILE NAME:       generator2d.h
*
*    DESCRIPTION:     Class template
************************************************************************/

#ifndef __generator_2d_h__
#define __generator_2d_h__

// Standard lib dependencies

// Boost lib dependencies
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>

// Game lib dependencies
#include <common/defs.h>
#include <common/pointint.h>

// Forward declaration(s)
class CSector2D;
class CMatrix;
class CWorldPoint;
class CObjectData2D;
class CPointInt;

// Typedefs for changing the random generation
typedef boost::minstd_rand BaseRandGenType;
typedef boost::uniform_int<> IntDistribution;
typedef boost::uniform_real<float> FloatDistribution;
typedef boost::variate_generator<BaseRandGenType&, IntDistribution> RandIntGen;
typedef boost::variate_generator<BaseRandGenType&, FloatDistribution> RandFloatGen;

class CGenerator2D
{
public:

    enum EFrameDisplay
    {
        EFD_RANDOM,
        EFD_SEQUENTIALLY
    };

    // Constructor
    CGenerator2D( CObjectData2D * pObjData,
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
                  bool instance );
    ~CGenerator2D();

    // Update the sprites in the generator
    void Update();

    // Generate any sectors that become invalid
    void Generate( const CPointInt & focus, const CPointInt & newFocus, bool forceGenerate = false );

    // Transform the generator's components
    void Transform();
    void Transform( const CMatrix & _scaledMatrix,
                    const CMatrix & _unscaledMatrix, 
                    const CWorldPoint & point );

    // Render the sprites in the generator
    void Render();

private:

    // Init the generator
    void Init( CObjectData2D * pObjData, CPointInt & focus );

    // To forcefully generate our debris, we set up th generator as if the last 
    // focus was far enough away to regenerate all sectors around the new focus
    void InvalidateSectors();

    // Find the sectors out of our focus range and moves them to the unused sector vector
    void OrganizeSectors( const CPointInt & point );

    // Get the seed to a particular sector.
    uint GetSectorSeed( const CPointInt & point );

    // Generate the sector
    void GenerateSector( std::vector<CSector2D *>::iterator & sectorIter );

    // Get the position of the color sector
    CPointInt GetColorSectorPos( std::vector<CSector2D *>::iterator & sectorIter );

private:

    // Safe Sector vector
    boost::ptr_vector<CSector2D> spSectorVec;

    // Sector vectors of organization
    std::vector<CSector2D *> pUsedSectorVec;
    std::vector<CSector2D *> pUnusedSectorVec;
    std::vector<CSector2D *>::iterator sectorVecIter;

    // Used to generate a sector seed
    std::vector<uint> seedPieceVec;

    // Boost random number generators
    BaseRandGenType generator;
    RandFloatGen GetRandRot;
    RandFloatGen GetRandFloatPos;

    BaseRandGenType colorGenerator;
    RandFloatGen GetRandColorR;
    RandFloatGen GetRandColorG;
    RandFloatGen GetRandColorB;
    RandFloatGen GetRandColorA;

    RandIntGen GetRandCount;
    RandIntGen GetRandIntPos;
    RandIntGen GetRandDepth;
    RandFloatGen GetRandScale;

    RandIntGen GetRandHueShift;

    // How to display the frames
    EFrameDisplay frameDisplay;

    // The sector size
    const int SECTOR_SIZE;

    // The sector dimension
    const int SECTOR_DIMENSIONS;

    // The number of sectors used
    const int SECTOR_COUNT;

    // The seed for this generator
    const int SEED;

    // Count Min/Max
    const int COUNT_MIN;
    const int COUNT_MAX;

    // Depth Min/Max
    const int DEPTH_MIN;
    const int DEPTH_MAX;

    // Rotate Min/Max
    const int ROTATE_MIN;
    const int ROTATE_MAX;

    // Scale Min/Max
    const int SCALE_MIN;
    const int SCALE_MAX;

    // Color Min/Max
    const float COLOR_MIN_R;
    const float COLOR_MAX_R;
    const float COLOR_MIN_G;
    const float COLOR_MAX_G;
    const float COLOR_MIN_B;
    const float COLOR_MAX_B;
    const float COLOR_MIN_A;
    const float COLOR_MAX_A;
    const int COLOR_GRID_SIZE;

    // instance flag
    const bool INSTANCE; 

    // The center of the background
    CPointInt center;

};

#endif  // __generator_2d_h__


