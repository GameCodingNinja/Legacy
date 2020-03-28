
/************************************************************************
*    FILE NAME:       ismartguibase.h
*
*    DESCRIPTION:     iSmartGuiBase Class
************************************************************************/

// Physical component dependency
#include <gui/ismartguibase.h>

// Game lib dependencies
#include <gui/uicontrol.h>
#include <gui/uimenu.h>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartGuiControl::CSmartGuiControl( CUIControl * _pUIControl )
                 : pUIControl(_pUIControl)
{
}   // constructor


/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartGuiMenu::CSmartGuiMenu( CUIMenu * _pUIMenu )
              : pUIMenu(_pUIMenu)
{
}   // constructor






