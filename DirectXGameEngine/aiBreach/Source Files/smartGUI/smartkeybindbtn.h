
/************************************************************************
*    FILE NAME:       smartkeybindbtn.h
*
*    DESCRIPTION:     Class CSmartKeyBindBtn
************************************************************************/

#ifndef __smart_key_bind_btn_h__
#define __smart_key_bind_btn_h__

// Physical component dependency
#include <gui/ismartguibase.h>

// Game lib dependencies
#include <common/defs.h>

class CSmartKeyBindBtn : public CSmartGuiControl
{
public:

    // Constructor
    explicit CSmartKeyBindBtn( CUIControl * _pUIControl );

    // Called when the control is created
    void Create();

};

#endif  // __smart_key_bind_btn_h__


