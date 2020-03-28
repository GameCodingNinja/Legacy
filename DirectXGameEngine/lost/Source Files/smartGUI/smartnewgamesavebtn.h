
/************************************************************************
*    FILE NAME:       smartnewgamesavebtn.h
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

#ifndef __smart_new_game_save_btn_h__
#define __smart_new_game_save_btn_h__

// Physical component dependency
#include <gui/ismartguibase.h>

class CSmartNewGameSaveBtn : public CSmartGuiControl
{
public:

    // Constructor
    explicit CSmartNewGameSaveBtn( CUIControl * _pUIControl );

    // Called when the control is executed
    void Execute();

};

#endif  // __smart_new_game_save_btn_h__


