
/************************************************************************
*    FILE NAME:       worldgenerator.cpp
*
*    DESCRIPTION:     Singleton to handle world generation
************************************************************************/

// Physical component dependency
#include "worldgenerator.h"

// Standard lib dependencies
#include <stdlib.h>
#include <algorithm>

// Game lib dependencies
#include <2d/stage2d.h>
#include <2d/sector2d.h>
#include <2d/objectdatalist2d.h>
#include <2d/actorsprite2d.h>
#include <2d/spritegroup2d.h>
#include <managers/actormanager.h>
#include <misc/settings.h>
#include <utilities/genfunc.h>
#include <common/defs.h>

// Game dependencies
#include "../game/savefile.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)
// disable warning about the use of the "this" pointer in the constructor list
#pragma warning(disable : 4355)


/************************************************************************
*    desc:  Constructer
************************************************************************/
CWorldGenerator::CWorldGenerator()
{
}   // constructor


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CWorldGenerator::~CWorldGenerator()
{
}   // destructer

/************************************************************************
*    desc:  Intialize the world generator
************************************************************************/
void CWorldGenerator::Init()
{
    // Get the position of the player as the new focus position
    focus = CActorManager::Instance().GetActor2D( "player_ship" )->GetPos().GetIntComponent();

    CGenerator * pTmpGen;
    
    pTmpGen = new CStarGenerator();
    generator.push_back( pTmpGen );

    pTmpGen = new CPlanetGenerator();
    generator.push_back( pTmpGen );

    pTmpGen = new CDustGenerator();
    generator.push_back( pTmpGen );

    pTmpGen = new CDebrisGenerator();
    generator.push_back( pTmpGen );

    for( size_t i = 0; i < generator.size(); ++i )
        generator[i].Init( focus, CSaveFile::Instance().GetWorldSeed() );

}	// Init */


/************************************************************************
*    desc:  Transform the world generator's components
************************************************************************/
void CWorldGenerator::Transform()
{
    for( size_t i = 0; i < generator.size(); ++i )
        generator[i].Transform();

}	// Transform */


/************************************************************************
*    desc:  Transform the world generator's visual components
*
*	 param: const CMatrix & _scaledMatrix  - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point      - passed in relative position
************************************************************************/
void CWorldGenerator::Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                                 const CWorldPoint & point )
{
    for( size_t i = 0; i < generator.size(); ++i )
        generator[i].Transform( _scaledMatrix, _unscaledMatrix, point );
    
}	// Transform */


/************************************************************************
*    desc:  Update the sectors in the world
************************************************************************/
void CWorldGenerator::Update()
{
    for( size_t i = 0; i < generator.size(); ++i )
        generator[i].Update();

}	// Update */


/************************************************************************
*    desc:  Render the sectors in the world
*
*	 param:	const CMatrix & matrix    - passed in relative rotation matrix
*			const CWorldPoint & point - passed in relative position
***********************************************************************/
void CWorldGenerator::Render()
{
    for( size_t i = 0; i < generator.size(); ++i )
        generator[i].Render();

}	// Render */


/************************************************************************
*    desc:  Handle the world generation for space
*
*	 param: bool forceGenerate - forces the generators to create their
*								 sectors and sprites as opposed to waiting
*								 for a difference in focus
************************************************************************/
void CWorldGenerator::HandleSpaceGeneration( bool forceGenerate )
{
    // Get the position of the player as the new focus position
    CPointInt newFocus = CActorManager::Instance().GetActor2D( "player_ship" )->GetPos().GetIntComponent();
    
    // If the current focus doesn't match the last one, we remove some sprites and generate new ones
    if( focus != newFocus || forceGenerate )
    {
        for( size_t i = 0; i < generator.size(); ++i )
        {
            generator[i].HandleGeneration( focus, newFocus, forceGenerate );
            generator[i].UpdateSpriteVector();
        }

        focus = newFocus;
    }

}   // HandleSpaceGeneration */


/************************************************************************
*    desc:  Clear the contents of the generator
************************************************************************/
void CWorldGenerator::Clear()
{
    for( size_t i = 0; i < generator.size(); ++i )
        generator[i].Clear();

    generator.clear();

}	// Clear */