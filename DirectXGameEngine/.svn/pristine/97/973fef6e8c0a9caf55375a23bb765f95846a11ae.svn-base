
/************************************************************************
*    FILE NAME:       titlescreenstate.h
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

#ifndef __title_screen_state_h__
#define __title_screen_state_h__

// Physical component dependency
#include "commonstate.h"

// Game lib dependencies
#include <2d/spritegroup2d.h>

// Standard lib dependencies
#include <vector>

class CTitleScreenState : public CCommonState
{
public:

    // Constructor
    explicit CTitleScreenState();

    // Destructor
    virtual ~CTitleScreenState();

    // Update objects that require them
    void Update();

    // Transform game objects
    void Transform();

    // 2D/3D Render of game content
    void PreRender();

    // Is the state done
    bool DoStateChange();

private:

    // title screen background
    CSpriteGroup2D background;
};

#endif  // __title_screen_state_h__


