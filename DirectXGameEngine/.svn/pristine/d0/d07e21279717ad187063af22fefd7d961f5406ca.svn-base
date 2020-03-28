/************************************************************************
*    FILE NAME:       stage.h
*
*    DESCRIPTION:     Stage class
************************************************************************/

#ifndef __stage_3d_h__
#define __stage_3d_h__

// Standard lib dependencies
#include <string>
#include <map>

// Game lib dependencies
#include <3d/sector3D.h>
#include <3d/lightlst.h>

// Forward declaration(s)
class CStageData;

class CStage3D
{
public:

    // Get the instance of the singleton class
    static CStage3D & Instance()
    {
        static CStage3D stage;
        return stage;
    }

    // Load a stage
    void LoadStage( const std::string & name );

    // Renders the stage objects
    void Render( CMatrix & matrix );

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

    // Update the sectors
    void Update();

    // Returns the light list
    CLightLst & GetLightList();

    // Clear all the stage data so that data can be loaded
    void ClearStage();

    // Init the stage lights
    void InitStageLights();

private:

    CStage3D();
    ~CStage3D();

private:

    // Data of the stage
    CStageData * pStageData;

    // Hash map of sectors for later loading
    std::map<CPointInt, CSector3D *> pSectorMap;

    // List of lights in the stage
    CLightLst lightList;
};

#endif  // __stage_3d_h__