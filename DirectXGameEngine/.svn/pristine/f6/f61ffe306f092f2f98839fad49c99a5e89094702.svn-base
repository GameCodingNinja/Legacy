/************************************************************************
*    FILE NAME:       objectmanager.h
*
*    DESCRIPTION:     Stage class
************************************************************************/

#ifndef __objectmanager_h__
#define __objectmanager_h__

// Standard lib dependencies
#include <string>
#include <vector>
#include <algorithm>
#include <map>

// Game lib dependencies
#include "matrix.h"
#include "point.h"
#include "camera.h"
#include "collisioncheck.h"


// Forward declaration(s)
class CStage;
class CActor;
class CSpriteGroup;
class CAnimatedSpriteGroup;

class CObjectManager
{
public:

    CObjectManager();
    virtual ~CObjectManager();

    static bool SortCSpriteGroup( CSpriteGroup * sg1, CSpriteGroup * sg2 );

    // Renders the scene
    virtual void Render();

    // Renders to the shadow map buffer
    void RenderShadowMap(CMatrix & matrix);

    // Update the animations
    virtual void Update();

    // Updates the rendering order of the alpha objects (currently not being used)
    //virtual void UpdateAlpha( CMatrix & matrix );

    // Calculates object and player collisions
    virtual void ReactToCollision();

    // Renders the shadows
    //virtual void RenderShadow( CMatrix & matrix );

    // Sets the statistics of player collision
    void SetPlayerCollisionStats( float radius, float fpad, float wpad );

    // Loads a stage
    bool LoadStage( std::string & filePath );

    // Loads all animated sprites
    bool LoadAnimatedSprites( std::string & filePath );

    // Set and get functions for the stage
    void SetStage( CStage & _stage );
    CStage * GetStage();

    // Sets the animation of a specific animated sprite group in the map
    virtual void SetAnimation( std::string & groupName, std::string & animName );

    virtual void ChangeCharacters();


protected:

    // Statistics of player vs object collision
    float playerRadius;
    float playerFloorPad;
    float playerWallPad;	
    
    // All animated objects
    CActor * pActors;

    // All non-animated objects
    CStage * pStage;

    // Vector containing all objects
    //std::vector<CAnimatedSpriteGroup *> allAnimGroupsVector;

    // Vector containing all objects will an outline
    //std::vector<CAnimatedSpriteGroup *> animCellVector;
    // Vector containing objects with collision against the player
    //std::vector<CAnimatedSpriteGroup *> animColPlayerVector;
    // Vector containing objects with collision against other objects
    //std::vector<CAnimatedSpriteGroup *> animColObjVector;
    // Vector containing all objects that colide with something
    //std::vector<CAnimatedSpriteGroup *> allAnimColVector;
    // Vector containing objects that will have shadows
    //std::vector<CAnimatedSpriteGroup *> animShadowVector;

};
#endif  // __objectmanager_h__