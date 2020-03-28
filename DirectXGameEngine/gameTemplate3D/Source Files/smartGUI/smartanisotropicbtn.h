
/************************************************************************
*    FILE NAME:       smartanisotropicbtn.h
*
*    DESCRIPTION:     Class CSmartAnisotropicBtn
************************************************************************/

#ifndef __smart_anisotropic_btn_h__
#define __smart_anisotropic_btn_h__

// Physical component dependency
#include "smartsettingsmenubtn.h"

class CSmartAnisotropicBtn : public CSmartSettingsMenuBtn
{
public:

    // Constructor
    explicit CSmartAnisotropicBtn( CUIControl * _pUIControl );

    // Called when the gui is to be displayed
    void Display();

    // Called when the control is to excute
    void Execute();

};

#endif  // __smart_anisotropic_btn_h__


