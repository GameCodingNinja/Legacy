
/************************************************************************
*    FILE NAME:       smartnewgamesavebtn.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Game OS dependencies
                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

// Physical component dependency
#include "smartnewgamesavebtn.h"

// Game lib dependencies
#include <gui/uimenutree.h>
#include <gui/uimenu.h>
#include <gui/uicontrol.h>

// Game dependencies
#include "../game/savefile.h"

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartNewGameSaveBtn::CSmartNewGameSaveBtn( CUIControl * _pUIControl )
                    : CSmartGuiControl( _pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Called when the control is executed
****************************************************************************/
void CSmartNewGameSaveBtn::Execute()
{
    if( pUIControl->GetName() == "yes_button" )
        CSaveFile::Instance().OverwriteSave( pUIControl->GetExecutionAction() );

    else
    {
        CSaveFile::Instance().NewSave();
 
        // If this is the first safe file created, enable these menu buttons
        CUIMenu * pMenu = CUIMenuTree::Instance().GetPtrToMenu( "title_screen_menu" );
        CUIControl * pCtrl = pMenu->GetPtrToControl("continue_btn");
        if( pCtrl->GetState() == NUIControl::ECS_DISABLED )
        {
            pCtrl->SetState( NUIControl::ECS_INACTIVE );
            pCtrl->Update();
        }

        pCtrl = pMenu->GetPtrToControl("load_game_menu");
        if( pCtrl->GetState() == NUIControl::ECS_DISABLED )
        {
            pCtrl->SetState( NUIControl::ECS_INACTIVE );
            pCtrl->Update();
        }
    }

}	// Execute







