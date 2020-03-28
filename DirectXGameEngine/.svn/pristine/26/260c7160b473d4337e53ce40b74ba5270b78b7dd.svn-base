/************************************************************************
*    FILE NAME:       testcontrols.h
*
*    DESCRIPTION:     test controls class
************************************************************************/  

#ifndef __testcontrols_h__
#define __testcontrols_h__

// Physical component dependency
#include "controls.h"
#include "controller\\gamecontroller.h"

// Game lib dependencies
#include "3d\\camera.h"

// Standard lib dependencies
#include <string>


class CTestControls : public CControls
{
public:

    CTestControls();
    virtual ~CTestControls();

    virtual void GetUserInput( CCamera & camera );

private:

    // flags that indicates button movement
    int buttonUpDown;
    int buttonLeftRight;

    // Center point to judge position to
    POINT mouseCenterPt;

};

#endif  // __testcontrols_h__