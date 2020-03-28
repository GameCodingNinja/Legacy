/************************************************************************
*    FILE NAME:       sector2D.h
*
*    DESCRIPTION:     Class that holds the positions, rotations and scale
*					  of the objects in a stage's sector
************************************************************************/

#ifndef __sector_2d_h__
#define __sector_2d_h__

// Standard lib dependencies
#include <vector>
#include <string>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Game lib dependencies
#include <common/matrix.h>
#include <common/pointint.h>
#include <common/worldpoint.h>

// Forward declaration
class CSpriteGroup2D;

class CSector2D : boost::noncopyable
{
public:

    CSector2D();
    ~CSector2D();

    // Load all stage object information from an xml
    void LoadFromXML( const std::string & filePath );

    // Add a sprite to the sector
    void AddSprite( CSpriteGroup2D * pGroup );
    
    // Remove the sprite from the sector
    void RemoveSprite( CSpriteGroup2D * pGroup, bool deleteSprite = true );

    // Transform the sector's components
    virtual void Transform();
    virtual void Transform( const CMatrix & _scaledMatrix,
                            const CMatrix & _unscaledMatrix, 
                            const CWorldPoint & point );

    // Renders the sector's objects
    void Render();

    // Renders all the frames of the sector's sprites in one shot
    void RenderAllFrames();

    // Inc the animation timer
    void Update();

    // Set and get functions for the sector position
    void SetPosition( const CPointInt & pos );
    CPointInt & GetPosition();

    // Clear all the data so new data can be loaded
    void ClearSector();

    // Get all the sprite groups in the sector
    std::vector<CSpriteGroup2D *> & GetGroupVector();

    // Get a sprite groups in the sector
    CSpriteGroup2D * GetGroup( int i );

    // Get all the sprite groups with collision
    std::vector<CSpriteGroup2D *> & GetCollisionVector();

    // Visibility access functions
    void SetVisible( bool value );
    bool IsVisible();

protected:

    // Position of the sector
    CPointInt i_pos;

    // Vector containing all objects
    std::vector<CSpriteGroup2D *> pGroupVector;

    // Vector containing objects with all collision objects
    std::vector<CSpriteGroup2D *> pCollisionVector;

    // Whether or not the sector is visible
    bool visible;

};

#endif  // __sector_2d_h__
