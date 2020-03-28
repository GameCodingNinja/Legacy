/************************************************************************
*    FILE NAME:       sector3d.h
*
*    DESCRIPTION:     Class that holds the positions, rotations and scale
*					  of the objects in a stage's sector
************************************************************************/

#ifndef __sector_3d_h__
#define __sector_3d_h__

// Standard lib dependencies
#include <vector>
#include <map>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Game lib dependencies
#include <common/matrix.h>
#include <common/pointint.h>

// Forward declaration(s)
class CSpriteGroup3D;
class CLight;

class CSector3D : public boost::noncopyable
{
public:

    CSector3D();
    ~CSector3D();

    // Load all stage object information from an xml
    void LoadFromXML( const std::string & filePath );

    // Renders the stage objects
    void Render( CMatrix & matrix );
    void RenderAlpha( CMatrix & matrix );

    // Render to shadow map
    void RenderShadowMap( CMatrix & matrix );
    
    /* NOTE - NOT USED. Collision needs revamping ******************************
    // Reacts to collision against the player
    void ReactToPlayerCollision( CCollisionCheck & cCheck, float radius, float fpad, float wpad );

    // Reacts to collision against other objects
    void ReactToObjectCollision( CCollisionCheck & cCheck );
    ***************************************************************************/

    // Allow objects suseptible to gravity to fall
    void ReactToGravity( float speed, float elapsedTime );

    // Inc the animation timer
    void Update();

    // Returns the light list
    std::map<std::string, CLight *> & GetLightMap();

    // Set and get functions for the sector position
    void SetPosition( const CPointInt & pos );
    CPointInt & GetPosition();

    // Clear all the data so new data can be loaded
    void ClearSector();

protected:

    /****************************************************************
    *	All groups in sector vectors are considered static for now
    ****************************************************************/

    // Map of all object groups.
    //std::map<std::string, CGroup *> allGroupsMap;

    CPointInt position;

    // Vector containing all objects
    std::vector<CSpriteGroup3D *> allGroupsVector;

    // Vector containing objects with collision against other objects
    std::vector<CSpriteGroup3D *> colObjVector;
    // Vector containing objects with collision against the player
    std::vector<CSpriteGroup3D *> colPlayerVector;
    // Vector containing objects that react to gravity
    std::vector<CSpriteGroup3D *> gravityVector;
    // Vector containing objects with all collision objects
    std::vector<CSpriteGroup3D *> allColVector;
    // Vector containing objects that will have alpha shading
    std::vector<CSpriteGroup3D *> alphaVector;
    // Vector containing objects that will not have alpha shading and can simply be rendered in any order
    std::vector<CSpriteGroup3D *> nonAlphaVector;
    // Vector containing objects that are animated
    std::vector<CSpriteGroup3D *> animatedVector;
    // Vector containing objects that project shadows
    std::vector<CSpriteGroup3D *> shadowCastVector;

    // Map of the lights in the sector
    std::map<std::string, CLight *> lightMap;

};

#endif  // __sector_3d_h__
