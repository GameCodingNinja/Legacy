
/************************************************************************
*    FILE NAME:       smartkeybindbtn.cpp
*
*    DESCRIPTION:     Class CSmartKeyBindBtn
************************************************************************/

// Physical component dependency
#include "smartkeybindbtn.h"

// Game lib dependencies
#include <gui/uicontrol.h>
#include <controller/gamecontroller.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartKeyBindBtn::CSmartKeyBindBtn( CUIControl * _pUIControl )
                 : CSmartGuiControl( _pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the control is created
****************************************************************************/
void CSmartKeyBindBtn::Create()
{
    string idStr;
    bool configurable;
    uint disableCounter(0);

    string actionNameStr = pUIControl->GetFontString();

    for( uint i = 0; i < NDevice::MAX_UNIQUE_DEVICES; ++i )
    {
        // clear out any conflicted flags
        if( pUIControl->GetSubControl(i)->GetConflictedFlag() )
        {
            pUIControl->GetSubControl(i)->SetConflictedFlag(false);
            pUIControl->GetSubControl(i)->ForceStateUpdate();
        }

        if( CGameController::Instance().GetDeviceActionStr(NDevice::EDeviceId(i), actionNameStr, idStr, configurable) )
        {
            pUIControl->GetSubControl(i)->DisplayFontString(idStr);

            if( !configurable )
            {
                pUIControl->GetSubControl(i)->SetState( NUIControl::ECS_DISABLED );
                ++disableCounter;
            }
        }
        else
        {
            pUIControl->GetSubControl(i)->SetState( NUIControl::ECS_DISABLED );
            pUIControl->GetSubControl(i)->DisplayFontString("NA");
            ++disableCounter;
        }
    }

    // If all 3 device types are disabled, disable the button
    if( disableCounter == NDevice::MAX_UNIQUE_DEVICES )
        pUIControl->SetState( NUIControl::ECS_DISABLED );

}	// Create








