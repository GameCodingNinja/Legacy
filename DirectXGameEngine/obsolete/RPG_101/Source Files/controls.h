/************************************************************************
*    FILE NAME:       controls.h
*
*    DESCRIPTION:     controls class
************************************************************************/  

#ifndef __controls_h__
#define __controls_h__

// Game lib dependencies
#include "3d\\camera.h"

// Physical component dependency
//#include "3d\\gamewnd.h"

// Standard lib dependencies
#include <string>


class CControls //: public CGameWnd
{
public:

    CControls();
    virtual ~CControls();

    virtual void GetUserInput( CCamera & camera ) = 0;

};

#endif  // __controls_h__