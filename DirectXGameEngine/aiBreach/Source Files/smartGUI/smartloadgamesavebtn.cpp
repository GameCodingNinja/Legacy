
/************************************************************************
*    FILE NAME:       smartloadgamesavebtn.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Game OS dependencies
                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

// Physical component dependency
#include "smartloadgamesavebtn.h"

// Game lib dependencies
#include <gui/uicontrol.h>

// Game dependencies
#include "../game/savefile.h"

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartLoadGameSaveBtn::CSmartLoadGameSaveBtn( CUIControl * _pUIControl )
                     : CSmartGuiControl( _pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the control is executed
****************************************************************************/
void CSmartLoadGameSaveBtn::Execute()
{
    CSaveFile::Instance().SetFileName( pUIControl->GetName() );

    CSaveFile::Instance().Load();

}	// Execute







