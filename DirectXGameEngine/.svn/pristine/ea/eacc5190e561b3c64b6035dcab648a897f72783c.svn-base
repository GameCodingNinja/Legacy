
/************************************************************************
*    FILE NAME:       smartresolutionbtn.h
*
*    DESCRIPTION:     Class CSmartResBtn
************************************************************************/

#ifndef __smart_resolution_btn_h__
#define __smart_resolution_btn_h__

// Physical component dependency
#include "smartsettingsmenubtn.h"

// Standard lib dependencies
#include <vector>

// Game lib dependencies
#include <common/size.h>

class CSmartResolutionBtn : public CSmartSettingsMenuBtn
{
public:

    // Constructor
    explicit CSmartResolutionBtn( CUIControl * _pUIControl );

    // Called when the control is created
    void Create();

    // Called when the gui is to be displayed
    void Display();

    // Called when the control is executed
    void Execute();

    // Set the resolution change
    void SetResolutionChange();

    // Was the resolution changed
    bool WasResolutionChanged();

private:

    // Size resolution vector
    std::vector<CSize<float>> resVec;

    // Resolution index
    int resIndex;

};

#endif  // __smart_resolution_btn_h__


