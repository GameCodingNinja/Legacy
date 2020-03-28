
/************************************************************************
*    FILE NAME:       smartnewsavescrollbox.h
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

#ifndef __smart_new_load_scroll_box_h__
#define __smart_new_load_scroll_box_h__

// Physical component dependency
#include <gui/ismartguibase.h>

class CSmartNewLoadScrollBox : public CSmartGuiControl
{
public:

    // Constructor
    explicit CSmartNewLoadScrollBox( CUIControl * _pUIControl );

    // Called when the gui is to be displayed
    virtual void Display();

};

#endif  // __smart_new_save_scroll_box_h__


