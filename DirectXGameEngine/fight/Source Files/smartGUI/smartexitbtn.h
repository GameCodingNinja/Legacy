
/************************************************************************
*    FILE NAME:       smartexitbtn.h
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

#ifndef __smart_exit_btn_h__
#define __smart_exit_btn_h__

// Physical component dependency
#include <gui/ismartguibase.h>

class CSmartExitBtn : public CSmartGuiControl
{
public:

    // Constructor
    explicit CSmartExitBtn( CUIControl * _pUIControl );

    // Called when the control is executed
    void Execute();

};

#endif  // __smart_exit_btn_h__


