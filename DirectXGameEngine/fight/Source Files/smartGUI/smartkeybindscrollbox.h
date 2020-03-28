
/************************************************************************
*    FILE NAME:       smartkeybindscrollbox.h
*
*    DESCRIPTION:     Class CSmartKeyBindscrollbox
************************************************************************/

#ifndef __smart_key_bind_scroll_box_h__
#define __smart_key_bind_scroll_box_h__

// Physical component dependency
#include <gui/ismartguibase.h>

// Game lib dependencies
#include <common/defs.h>

// Forward declaration(s)
class CUIScrollBox;

class CSmartKeyBindScrollBox : public CSmartGuiControl
{
public:

    // Constructor
    explicit CSmartKeyBindScrollBox( CUIControl * _pUIControl );

    // Called when the control is created
    void Create();

    // Go through all the controls and flag the conflicted mappings
    void FlagConflictedDeviceMappings();

private:

    // Pointer to control type casted for easer handling
    CUIScrollBox * pScrollBoxCtrl;
};

#endif  // __smart_key_bind_btn_h__


