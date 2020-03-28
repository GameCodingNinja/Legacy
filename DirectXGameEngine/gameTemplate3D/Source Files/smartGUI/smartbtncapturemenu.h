
/************************************************************************
*    FILE NAME:       smartbtncapturemenu.h
*
*    DESCRIPTION:     Class CSmartBtnCaptureMenu
************************************************************************/

#ifndef __smart_button_capture_menu_h__
#define __smart_button_capture_menu_h__

// Physical component dependency
#include <gui/ismartguibase.h>

class CSmartBtnCaptureMenu : public CSmartGuiMenu
{
public:

    // Constructor
    explicit CSmartBtnCaptureMenu( CUIMenu * _pUIMenu );

    // Called when the gui is to be displayed
    void Display();

    // Called during the handle user imput
    void HandleUserInput();

private:

    // Needed to simulate key up for key binding
    int lastButtonID;

    // Boolean to determine when a key is ready to be bound
    bool keyBindReady;

};

#endif  // __smart_button_capture_menu_h__


