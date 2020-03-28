
/************************************************************************
*    FILE NAME:       smartresolutionbtn.cpp
*
*    DESCRIPTION:     Class CSmartResBtn
************************************************************************/

// Game OS dependencies
                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

// Physical component dependency
#include "smartresolutionbtn.h"

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <misc/settings.h>
#include <system/xdevice.h>
#include <gui/uibuttonlist.h>
#include <utilities/genfunc.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartResolutionBtn::CSmartResolutionBtn( CUIControl * _pUIControl )
                    : CSmartSettingsMenuBtn( _pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the control is created - Set the resolution strings
****************************************************************************/
void CSmartResolutionBtn::Create()
{
    DEVMODE dm;
    memset(&dm, 0, sizeof(DEVMODE));
    dm.dmSize = sizeof(DEVMODE);
    DWORD mode = 0;

    DEVMODE dmCur;
    memset(&dmCur, 0, sizeof(DEVMODE));
    dmCur.dmSize = sizeof(DEVMODE);

    HMONITOR hMonitor = MonitorFromWindow(CXDevice::Instance().GetWndHandle(), MONITOR_DEFAULTTOPRIMARY);
    MONITORINFOEX monInfo;
    memset(&monInfo, 0, sizeof(MONITORINFOEX));
    monInfo.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(hMonitor, &monInfo);

    // Get the current display size from the settings
    const CSize<float> & size = CSettings::Instance().GetSize();

    int counter(0);
    resIndex = -1;

    // Add all the display resolutions to the button list
    if( EnumDisplaySettings(monInfo.szDevice, ENUM_CURRENT_SETTINGS, &dmCur) )
    {
        // Use a set to store only unique resolution strings
        vector<string> resStrVec;

        // search for the mode we are looking for
        mode = 0;
        while( EnumDisplaySettings( NULL, mode++, &dm ) )
        {
            // We only care about 32-bit displays of the same frequency
            if( (dm.dmBitsPerPel == dmCur.dmBitsPerPel) &&
                (dm.dmDisplayFrequency == dmCur.dmDisplayFrequency) )
            {
                string resStr = boost::str( boost::format("%d x %d") % dm.dmPelsWidth % dm.dmPelsHeight );

                // Keep out the duplicates
                if( find(resStrVec.begin(), resStrVec.end(), resStr) == resStrVec.end() )
                {
                    resStrVec.push_back(resStr);
                    resVec.push_back( CSize<float>(dm.dmPelsWidth, dm.dmPelsHeight) );

                    if( size.w == dm.dmPelsWidth && size.h == dm.dmPelsHeight )
                        resIndex = counter;

                    ++counter;
                }
            }
        }

        // Set the unique resolution strings
        for( vector<string>::iterator it = resStrVec.begin(); it != resStrVec.end(); ++it )
            pUIControl->SetToFontStringList( (*it) );
    }

    if( resIndex > -1 )
        NGenFunc::DynCast<CUIButtonList *>(pUIControl)->SetActiveIndex( resIndex );

}	// Create


/***************************************************************************
*    decs:  Called when the gui is to be displayed
****************************************************************************/
void CSmartResolutionBtn::Display()
{
    // Check for the resolution button list
    if( resIndex > -1 )
        NGenFunc::DynCast<CUIButtonList *>(pUIControl)->SetActiveIndex( resIndex );

}	// Display


/***************************************************************************
*    decs:  Called when the control is executed - Enable/disable the apply btn
****************************************************************************/
void CSmartResolutionBtn::Execute()
{
    EnableDisableApplyBtn();

}	// Execute


/***************************************************************************
*    decs:  Set the resolution change
****************************************************************************/
void CSmartResolutionBtn::SetResolutionChange()
{
    resIndex = NGenFunc::DynCast<CUIButtonList *>(pUIControl)->GetActiveIndex();

    CSettings::Instance().SetChangeSize( resVec[resIndex] );

}	// SetResolutionChange


/***************************************************************************
*    decs:  Was the resolution changed
****************************************************************************/
bool CSmartResolutionBtn::WasResolutionChanged()
{
    CUIButtonList * pBtnLstCtrl = NGenFunc::DynCast<CUIButtonList *>(pUIControl);

    if( resIndex != pBtnLstCtrl->GetActiveIndex() )
        return true;

    return false;

}	// WasResolutionChanged







