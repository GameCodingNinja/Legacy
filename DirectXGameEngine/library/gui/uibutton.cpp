/************************************************************************
*    FILE NAME:       uibutton.cpp
*
*    DESCRIPTION:     Class for user interface buttons
************************************************************************/

// Physical component dependency
#include <gui/uibutton.h>

// Standard lib dependencies
#include <assert.h>

// Game lib dependencies
#include <2d/spritegroup2d.h>
#include <common/defs.h>
#include <common/fontgroup.h>
#include <utilities/xmlparsehelper.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CUIButton::CUIButton()
{
    SetType( NUIControl::ECT_BUTTON );

}   // Constructer




