
/************************************************************************
*    FILE NAME:       smartresetkeybind.h
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

#ifndef __smart_reset_key_bind_btn_h__
#define __smart_reset_key_bind_btn_h__

// Physical component dependency
#include <gui/ismartguibase.h>

class CSmartResetKeyBindBtn : public CSmartGuiControl
{
public:

    // Constructor
    explicit CSmartResetKeyBindBtn( CUIControl * _pUIControl );

    // Called when the control is executed
    void Execute();

};

#endif  // __smart_exit_btn_h__

