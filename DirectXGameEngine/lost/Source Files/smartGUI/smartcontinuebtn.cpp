
/************************************************************************
*    FILE NAME:       smartcontinuebtn.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Game OS dependencies
                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

// Physical component dependency
#include "smartcontinuebtn.h"

// Game dependencies
#include "../game/savefile.h"

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartContinueBtn::CSmartContinueBtn( CUIControl * _pUIControl )
                 : CSmartGuiControl( _pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the control is executed
****************************************************************************/
void CSmartContinueBtn::Execute()
{
    // Get all the save file names
    CSaveFile::Instance().GetSaveFileNames();

    // Get the file name map to get the newest save file to load
    const std::multimap<std::time_t, std::string> & fileMap = CSaveFile::Instance().GetFileNameMap();
    std::multimap<std::time_t, std::string>::const_reverse_iterator it = fileMap.rbegin();

    CSaveFile::Instance().SetFileName( it->second );

    CSaveFile::Instance().Load();

}	// Execute







