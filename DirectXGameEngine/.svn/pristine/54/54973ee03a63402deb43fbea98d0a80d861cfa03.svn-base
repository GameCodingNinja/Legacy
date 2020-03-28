
/************************************************************************
*    FILE NAME:       generatormanager.h
*
*    DESCRIPTION:     Generator Manager
************************************************************************/

#ifndef __generator_manager_h__
#define __generator_manager_h__


// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_vector.hpp>

// Game lib dependencies
#include <3d/worldcamera.h>
#include <common/pointint.h>

// Forward declaration(s)
class CMatrix;
class CGenerator2D;

class CGeneratorMgr
{
public:

    // Get the instance of the singleton class
    static CGeneratorMgr & Instance()
    {
        static CGeneratorMgr generatorMgr;
        return generatorMgr;
    }

    // Create the generators
    void CreateGenerators( std::string & filePath, CPointInt & _focus );

    // Handle the generation
    void HandleGeneration( CPointInt & newFocus, bool forceGenerate = false );

    // Transform the world generator's components
    virtual void Transform();
    virtual void Transform( const CMatrix & _scaledMatrix, 
                            const CMatrix & _unscaledMatrix, 
                            const CWorldPoint & point );

    // Update the sectors in the world
    void Update();

    // Render the sectors in the world
    void Render();

    // Clear the contents of the generator
    void Clear();

private:

    // Constructor
    CGeneratorMgr();
    ~CGeneratorMgr();

private:

    // Vector of generators
    boost::ptr_vector< CGenerator2D > generator;

    // The sector the player is in
    CPointInt focus;

};

#endif  // __generator_manager_h__


