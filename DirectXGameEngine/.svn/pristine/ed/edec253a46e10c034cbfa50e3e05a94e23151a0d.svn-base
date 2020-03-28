
/************************************************************************
*    FILE NAME:       game.h
*
*    DESCRIPTION:     game class
************************************************************************/

#ifndef __game_h__
#define __game_h__

// Game lib dependencies
#include "3d\\visualsprite.h"
#include "3d\\collisionsprite.h"
#include "3d\\lightlst.h"
#include "3d\\camera.h"
#include "3d\\shadowsprite.h"
#include "3d\\jointanimationsprite.h"
#include "3d\\shadowjointanimationsprite.h"
#include "controller\\gamecontroller.h"
#include "3d\\pointsprite.h"
#include "3d\\stage.h"

#include "gameobjectmanager.h"

#include "testcontrols.h"
#include "controls.h"

// Physical component dependency
#include "3d\\gamewnd.h"

class CGame : public CGameWnd
{
public:

    // Get the instance of the CGameWnd singleton class
    static CGame & Instance();

protected:

    // Constructor
    CGame();

    // Destructor
    virtual ~CGame();

    // Load game resources here
    virtual bool LoadGameObjects();

    // Delete the allocated game objects
    virtual void DeleteGameObjects();

    // Act upon what the user is doing
    virtual void GetUserImput();

    // Update animations, Move sprites, Check for collision
    virtual void Update();

    // Check for collision and react to it
    virtual void ReactToCollision();

    // Render the screen
    virtual void Render();

    // Handle the device reset
    virtual void HandleDeviceReset( int width, int height );

private:

    // flags that indicates button movement
    int buttonUpDown;
    int buttonLeftRight;

    float CAMERA_Z_OFFSET;

    CGameObjectManager testManager;

    CPointSprite testps;

    // Center point to judge position to
    POINT mouseCenterPt;

    CCamera camera;

    CControls * pControls;

    //CStageCharacterMap test;
};

#endif  // __game_h__
