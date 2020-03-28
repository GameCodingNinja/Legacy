
/************************************************************************
*    FILE NAME:       ismartguibase.h
*
*    DESCRIPTION:     iSmartGuiBase Class
************************************************************************/

#ifndef __smart_gui_base_h__
#define __smart_gui_base_h__

// Boost lib dependencies
#include <boost/shared_ptr.hpp>

// Standard lib dependencies
#include <vector>
#include <string>

// Forward declaration(s)
class CUIControl;
class CUIMenu;


/*************************************
*  Base smart GUI class
**************************************/
class iSmartGuiBase
{
public:

    // Called when the control is created
    virtual void Create(){}

    // Called when the gui is to be displayed
    virtual void Display(){}

};


/*************************************
*  Smart GUI Menu class
**************************************/
class CSmartGuiMenu : public iSmartGuiBase
{
public:

    // Constructor
    CSmartGuiMenu( CUIMenu * _pUIMenu );

    // Called during the handle user imput
    virtual void HandleUserInput(){}

protected:

    // Pointer to UI Menu class.
    // NOTE: We do not own this pointer. Don't try to free it!
    CUIMenu * pUIMenu;

};



/*************************************
*  Smart GUI Control
**************************************/
class CSmartGuiControl : public iSmartGuiBase
{
public:

    // Constructor
    CSmartGuiControl( CUIControl * _pUIControl );

    // Called when the control is executed
    virtual void Execute(){}

protected:

    // Pointer to UI Control class.
    // NOTE: We do not own this pointer. Don't try to free it!
    CUIControl * pUIControl;

};

#endif  // __smart_gui_base_h__


