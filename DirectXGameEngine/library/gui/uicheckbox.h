/************************************************************************
*    FILE NAME:       uicheckbox.h
*
*    DESCRIPTION:     Class for user interface check box buttons
************************************************************************/

#ifndef __ui_check_box_h__
#define __ui_check_box_h__

// Physical component dependency
#include <gui/uicontrol.h>

class CUICheckBox : public CUIControl
{
public:
    
    CUICheckBox();
    virtual ~CUICheckBox();

    // Set or get the Select state of this control
    void SetToggleState( bool value );
    bool GetToggleState();

    // Update the button
    virtual void Update();

    // Render the button
    virtual void Render();
    
private:

    // Select state
    bool toggleState;

};

#endif  // __ui_check_box_h__