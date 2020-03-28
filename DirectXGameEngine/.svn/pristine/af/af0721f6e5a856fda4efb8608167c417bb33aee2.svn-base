
/************************************************************************
*    FILE NAME:       uihudcontrol.h
*
*    DESCRIPTION:     Class for user interface hud controls
************************************************************************/

#ifndef __ui_hud_control_h__
#define __ui_hud_control_h__

// Physical component dependency
#include <gui/uicontrol.h>

// Game lib dependencies
#include <common/dynamicoffset.h>

class CUIHudControl : public CUIControl
{
public:

    // Load the initial info from XML node
    virtual void LoadFromNode( const XMLNode & node );

    // Set the dynamic position
    virtual void SetDynamicPos();

    // Is the mouse point insprite
    virtual bool IsPointInSprite();

    // handle user select
    virtual bool HandleUserSelect( NUIMenu::EMenuNavigationState navState );

private:

    // Dynamic offset
    CDynamicOffset dynamicOffset;

};

#endif  // __ui_hud_control_h__


