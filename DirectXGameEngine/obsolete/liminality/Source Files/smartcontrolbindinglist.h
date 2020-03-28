
/************************************************************************
*    FILE NAME:       smartcontrolbindinglist.h
*
*    DESCRIPTION:     Class CSmartCtrlBindLstBtn
************************************************************************/

#ifndef __smart_control_binding_btn_lst_h__
#define __smart_control_binding_btn_lst_h__

// Physical component dependency
#include <gui/ismartguibase.h>

class CSmartCtrlBindLstBtn : public CSmartGuiControl
{
public:

    // Constructor
    explicit CSmartCtrlBindLstBtn( CUIControl * _pUIControl );

    // Called when the control is created
    void Create();

    // Called when the control is executed
    void Execute();

};

#endif  // __smart_control_binding_btn_lst_h__


