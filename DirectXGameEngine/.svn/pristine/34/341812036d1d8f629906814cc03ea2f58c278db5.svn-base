/************************************************************************
*    FILE NAME:       uicheckbox.cpp
*
*    DESCRIPTION:     Class for user interface check box buttons
************************************************************************/

// Physical component dependency
#include <gui/uicheckbox.h>

// Game lib dependencies
#include <2d/spritegroup2d.h>
#include <common/fontgroup.h>
#include <common/defs.h>

// Required namespace(s)
using namespace std;

typedef CUIControl CParent;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CUICheckBox::CUICheckBox()
           : toggleState(NUIControl::TOGGLE_STATE_OFF)
{
    SetType( NUIControl::ECT_CHECK_BOX );

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CUICheckBox::~CUICheckBox()
{
}   // Destructer


/************************************************************************
*    desc:  Set the Select state of this control
*  
*    param: EControlState value - control state
************************************************************************/
void CUICheckBox::SetToggleState( bool value )
{
    toggleState = value;

}	// SetSelectState


/************************************************************************
*    desc:  Get the Select state of this control
*  
*    ret: EControlState - control state
************************************************************************/
bool CUICheckBox::GetToggleState()
{
    return toggleState;

}	// GetSelectState


/************************************************************************
*    desc:  Update the button
************************************************************************/
void CUICheckBox::Update()
{
    // Toggle the state
    if( (GetState() == NUIControl::ECS_SELECTED) && (GetState() != GetLastState()) )
        toggleState = !toggleState;

    // Call the parent
    CParent::Update();

}	// Update


/************************************************************************
*    desc:  Render the control
************************************************************************/
void CUICheckBox::Render()
{
    if( IsVisible() )
    {
        size_t lastObject = spSpriteGroupVec.size() - 1;

        for( size_t i = 0; i < spSpriteGroupVec.size(); ++i )
            if( (i < lastObject) || (toggleState == NUIControl::TOGGLE_STATE_ON) )
                spSpriteGroupVec[i].Render();

        for( size_t i = 0; i < spFontGroupVec.size(); ++i )
            spFontGroupVec[i].fontSprite.Render();
    }

}	// Render
