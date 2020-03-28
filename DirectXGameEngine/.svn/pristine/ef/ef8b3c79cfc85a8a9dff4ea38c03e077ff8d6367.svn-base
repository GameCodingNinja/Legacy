
/************************************************************************
*    FILE NAME:       smartkeybindscrollbox.cpp
*
*    DESCRIPTION:     Class CSmartKeyBindBtn
************************************************************************/

// Physical component dependency
#include "smartkeybindscrollbox.h"

// Game lib dependencies
#include <gui/uiscrollbox.h>
#include <controller/gamecontroller.h>
#include <utilities/genfunc.h>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartKeyBindScrollBox::CSmartKeyBindScrollBox( CUIControl * _pUIControl )
                      : CSmartGuiControl( _pUIControl )
{
    pScrollBoxCtrl = NGenFunc::DynCast<CUIScrollBox *>(_pUIControl);

}   // constructor


/***************************************************************************
*    decs:  Called when the control is created
****************************************************************************/
void CSmartKeyBindScrollBox::Create()
{
    FlagConflictedDeviceMappings();

}	// Create


/***************************************************************************
*    decs:  Go through all the controls and flag the conflicted mappings
****************************************************************************/
void CSmartKeyBindScrollBox::FlagConflictedDeviceMappings()
{
    // Go through and clear out any conflicted flags
    for( uint i = 0; i < pScrollBoxCtrl->GetScrollCtrlCount(); ++i )
    {
        CUIControl * pCtrl = pScrollBoxCtrl->GetScrollCtrl(i);

        for( uint j = 0; j < pCtrl->GetSubControlCount(); ++j )
        {
            if( pCtrl->GetSubControl(j)->GetConflictedFlag() )
            {
                pCtrl->GetSubControl(j)->SetConflictedFlag(false);
                pCtrl->GetSubControl(j)->ForceStateUpdate();
            }
        }
    }

    // Now go through and find and set any conflicted bindings
    for( uint i = 0; i < pScrollBoxCtrl->GetScrollCtrlCount(); ++i )
    {
        CUIControl * pCtrl1 = pScrollBoxCtrl->GetScrollCtrl(i);

        if( pCtrl1->GetState() != NUIControl::ECS_DISABLED )
        {
            for( uint dev = 0; dev < NDevice::MAX_UNIQUE_DEVICES; ++dev )
            {
                if( (pCtrl1->GetType() == NUIControl::ECT_SUB_CONTROL) &&
                    (pCtrl1->GetSubControl(dev)->GetState() != NUIControl::ECS_DISABLED) &&
                    !pCtrl1->GetSubControl(dev)->GetConflictedFlag() )
                {
                    bool found(false);
                    auto actionStr = pCtrl1->GetDisplayedString();
                    auto componetStrId1 = pCtrl1->GetSubControl(dev)->GetDisplayedString();
                    auto strPos = pCtrl1->GetDisplayedString().find("Menu");
                    bool menuItem1 = ((strPos != std::string::npos) && (strPos == 0));

                    for( uint j = i+1; j < pScrollBoxCtrl->GetScrollCtrlCount(); ++j )
                    {
                        CUIControl * pCtrl2 = pScrollBoxCtrl->GetScrollCtrl(j);

                        // No need to check disabled or already flagged controls
                        if( (pCtrl2->GetType() == NUIControl::ECT_SUB_CONTROL) &&
                            (pCtrl2->GetSubControl(dev)->GetState() != NUIControl::ECS_DISABLED) &&
                            !pCtrl2->GetSubControl(dev)->GetConflictedFlag() )
                        {
                            auto componetStrId2 = pCtrl2->GetSubControl(dev)->GetDisplayedString();
                            strPos = pCtrl2->GetDisplayedString().find("Menu");
                            bool menuItem2 = ((strPos != std::string::npos) && (strPos == 0));

                            // Only check "menu vs menu" and "game controls vs game controls"
                            if( ((menuItem1 == menuItem2) || (actionStr == "Menu Toggle:")) && 
                                (componetStrId1 == componetStrId2) && 
                                (componetStrId1 != NDevice::UNBOUND_KEYBINDING_ID) )
                            {
                                pCtrl2->GetSubControl(dev)->SetConflictedFlag(true);
                                pCtrl2->GetSubControl(dev)->ForceStateUpdate();
                                found = true;
                            }
                        }
                    }

                    if( found )
                    {
                        pCtrl1->GetSubControl(dev)->SetConflictedFlag(true);
                        pCtrl1->GetSubControl(dev)->ForceStateUpdate();
                    }
                }
            }
        }
    }

}	// FlagConflictedDeviceMappings
