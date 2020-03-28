/************************************************************************
*    FILE NAME:       stage.h
*
*    DESCRIPTION:     Stage class
************************************************************************/

#ifndef __stage_h__
#define __stage_h__

// Standard lib dependencies
#include <string>
#include <vector>
#include <algorithm>
#include <map>

// Game lib dependencies
#include "matrix.h"
#include "hotspotwarp.h"
#include "point.h"
#include "spritegroup.h"
#include "animatedspritegroup.h"
#include "lightlst.h"
#include "camera.h"
#include "highresolutiontimer.h"

class CStage
{
public:

    CStage();
    virtual ~CStage();

    static bool SortCSpriteGroup( CSpriteGroup * sg1, CSpriteGroup * sg2 );

    virtual bool LoadFromXML( std::string & filePath );
    CLightLst & GetLightList();

    // Get function for the stage name
    std::string & GetStageName();

    virtual void Reset();

    // Renders the stage objects
    virtual void Render( CMatrix & matrix );

    // Render to shadow map
    virtual void RenderShadowMap( CMatrix & matrix );
    
    // Reacts to collision against the player
    virtual void ReactToPlayerCollision( CCollisionCheck & cCheck, float radius, float fpad, float wpad );

    // Reacts to collision against other objects
    virtual void ReactToObjectCollision( CCollisionCheck & cCheck );

    // Allow objects suseptible to gravity to fall
    virtual void ReactToGravity( float speed, float elapsedTime );

    // Reacts to collision against hotspots
    bool ReactToHotSpotWarp( CCollisionCheck & cCheck, float radius, float fpad, float wpad );

    // Inc the animation timer
    void IncAnimationTime();

    // Returns the pointer to the group
    CGroup * GetSpriteGroup( std::string & groupName );
    
    // Vector containing objects with collision against the player
    std::vector<CGroup *> colPlayerVector;

    // Light setup for the given stage
    CLightLst lightList;

    //CHotSpotWarp * warpSpot;
    std::vector<CHotSpotWarp *> warpSpotVector;

protected:

    std::string stageName;

    CSpriteGroup * pSkyMesh;

    // Map of all object groups.
    std::map<std::string, CGroup *> allGroupsMap;

    // Vector containing all objects
    std::vector<CGroup *> allGroupsVector;

    // Vector containing objects with collision against other objects
    std::vector<CGroup *> colObjVector;
    // Vector containing objects with all collision objects
    std::vector<CGroup *> allColVector;
    // Vector containing objects that will not move
    std::vector<CGroup *> staticVector;
    // Vector containing objects that will not be/may not be static
    std::vector<CGroup *> nonStaticVector;
    // Vector containing objects that will have alpha shading
    std::vector<CGroup *> alphaVector;
    // Vector containing objects that will not have alpha shading and can simply be rendered in any order
    std::vector<CGroup *> nonAlphaVector;
    // Vector containing objects that are animated
    std::vector<CGroup *> animatedVector;
    // Vector containing objects that project shadows
    std::vector<CGroup *> shadowCastVector;

};
#endif  // __stage_h__