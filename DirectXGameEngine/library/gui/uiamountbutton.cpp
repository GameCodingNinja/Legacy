/************************************************************************
*    FILE NAME:       uiamountbutton.cpp
*
*    DESCRIPTION:     Class for user interface buttons
************************************************************************/

// Physical component dependency
#include <gui/uiamountbutton.h>

// Boost lib dependencies
#include <boost/lexical_cast.hpp>

// Game lib dependencies
#include <2d/spritegroup2d.h>
#include <common/fontgroup.h>
#include <controller/gamecontroller.h>
#include <xact/xact.h>
#include <gui/uicontrolfactory.h>

// Required namespace(s)
using namespace std;

typedef CUISubControl CParent;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CUIAmountButton::CUIAmountButton()
               : minValue(1),
                 maxValue(1),
                 curValue(1),
                 smallIncrease(1),
                 largeIncrease(1),
                 displayValueAsInt(false)
{
    SetType( NUIControl::ECT_AMOUNT_BUTTON );

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CUIAmountButton::~CUIAmountButton()
{
}   // Destructer


/************************************************************************
*    desc:  Load the control info from XML node
*  
*    param: node - XML node
************************************************************************/
void CUIAmountButton::LoadFromNode( const XMLNode & node )
{
    CParent::LoadFromNode( node );

    // Get the selection string for this control
    XMLNode selectionNode = node.getChildNode( "selection" );

    // Load the control specific xml file
    if( !selectionNode.isEmpty() )
    {
        if( selectionNode.isAttributeSet( "customSelectAction" ) )
            customSelectAction = selectionNode.getAttribute( "customSelectAction" );

        if( selectionNode.isAttributeSet( "decSelectSmall" ) )
            decSelectSmallAction = selectionNode.getAttribute( "decSelectSmall" );

        if( selectionNode.isAttributeSet( "incSelectSmall" ) )
            incSelectSmallAction = selectionNode.getAttribute( "incSelectSmall" );

        if( selectionNode.isAttributeSet( "decSelectLarge" ) )
            decSelectLargeAction = selectionNode.getAttribute( "decSelectLarge" );

        if( selectionNode.isAttributeSet( "incSelectLarge" ) )
            incSelectLargeAction = selectionNode.getAttribute( "incSelectLArge" );
    }

    // Get the slider specific settings
    XMLNode settingsNode = node.getChildNode( "settings" );
    if( !settingsNode.isEmpty() )
    {
        if( settingsNode.isAttributeSet( "minValue" ) )
            minValue = atof(settingsNode.getAttribute( "minValue" ));

        if( settingsNode.isAttributeSet( "maxValue" ) )
            maxValue = atof(settingsNode.getAttribute( "maxValue" ));

        if( settingsNode.isAttributeSet( "defValue" ) )
            curValue = atof(settingsNode.getAttribute( "defValue" ));

        if( settingsNode.isAttributeSet( "smallIncrease" ) )
            smallIncrease = atof(settingsNode.getAttribute( "smallIncrease" ));

        if( settingsNode.isAttributeSet( "largeIncrease" ) )
            largeIncrease = atof(settingsNode.getAttribute( "largeIncrease" ));

        if( settingsNode.isAttributeSet( "displayValueAsInt" ) )
            if( string(settingsNode.getAttribute( "displayValueAsInt" )) == "true" )
                displayValueAsInt = true;
    }

    UpdateDisplayValue();

}	// LoadFromNode


/************************************************************************
*    desc:  Act in accordance to user input
************************************************************************/
void CUIAmountButton::HandleUserNavigation( NUIMenu::EMenuNavigationState navState,
                                          const string & nextAction,
                                          const string & previousAction )
{
    CParent::HandleUserNavigation( navState, nextAction, previousAction );

    // Update if the control is active
    if( (GetState() == NUIControl::ECS_ACTIVE) && IsVisible() && (navState == NUIMenu::EMNS_GAMEPAD_KEYBAORD) )
        SetSubStates( NUIControl::ECS_ACTIVE );

}	// HandleUserNavigation


/************************************************************************
*    desc:  handle user select
*
*	parm: navState - navigation state
************************************************************************/
bool CUIAmountButton::HandleUserSelect( NUIMenu::EMenuNavigationState navState )
{
    bool result(false);

    // Need to allow base control selection for the keyboard
    if( navState == NUIMenu::EMNS_GAMEPAD_KEYBAORD )
        result = CUIControl::HandleUserSelect( navState );
    else
        result = CParent::HandleUserSelect( navState );

    return result;

}	// HandleUserSelect


/************************************************************************
*    desc:  Get the state of this control
*  
*    ret: EControlState - control state
************************************************************************/
NUIControl::EControlState CUIAmountButton::GetState()
{
    // Need to use base control state to properly resync active animations
    if( CGameController::Instance().GetLastDevicedUsed() == NDevice::MOUSE )
        return CParent::GetState();
    else
        return CUIControl::GetState();

}	// GetState


/************************************************************************
*    desc:  Test for user selection
*
*	parm: navState - navigation state
************************************************************************/
bool CUIAmountButton::CheckForUserSelection( NUIMenu::EMenuNavigationState navState )
{
    // Respond to controls that that use left\right for selection
    if( (navState == NUIMenu::EMNS_GAMEPAD_KEYBAORD) &&
        WasSelect( CGameController::Instance().WasAction( decSelectSmallAction, NDevice::EAP_DOWN ),
                   CGameController::Instance().WasAction( incSelectSmallAction, NDevice::EAP_DOWN ),
                   CGameController::Instance().WasAction( incSelectLargeAction, NDevice::EAP_DOWN ),
                   CGameController::Instance().WasAction( decSelectLargeAction, NDevice::EAP_DOWN ),
                   CGameController::Instance().WasAction( customSelectAction,   NDevice::EAP_DOWN )) )
    {
        // Play the select sound of the active control
        PlaySubControlSelectSound();

        return true;
    }

    return false;

}	// CheckForUserSelection


/************************************************************************
*    desc:  Was there a left/right/up/down or button EMNS_GAMEPAD_KEYBAORD select
************************************************************************/
bool CUIAmountButton::WasSelect( bool decSmallAction, bool incSmallAction, bool decLargeAction, bool incLargeAction, bool customAction )
{
    if( decSmallAction )
        SetActiveSubControlIndex( NUIControl::BTN_DECREMENT );

    else if( incSmallAction )
        SetActiveSubControlIndex( NUIControl::BTN_INCREMENT );

    if( decLargeAction )
        SetActiveSubControlIndex( BTN_DECREMENT_BY_VALUE );

    else if( incLargeAction )
        SetActiveSubControlIndex( BTN_INCREMENT_BY_VALUE );

    else if( customAction )
        SetActiveSubControlIndex( BTN_CUSTOM_SELECT );

    return (decSmallAction || incSmallAction || decLargeAction || incLargeAction || customAction);

}	// WasSelect


/************************************************************************
*    desc:  Execute the action
************************************************************************/
void CUIAmountButton::ExecuteAction()
{
    // The button needs to change before the smart gui is called
    if( GetActiveSubControlIndex() == NUIControl::BTN_DECREMENT )
        SmallDecDisplay();

    else if( GetActiveSubControlIndex() == NUIControl::BTN_INCREMENT )
        SmallIncDisplay();

    if( GetActiveSubControlIndex() == BTN_DECREMENT_BY_VALUE )
        LargeDecDisplay();

    else if( GetActiveSubControlIndex() == BTN_INCREMENT_BY_VALUE )
        LargeIncDisplay();

    // Call the parent
    CParent::ExecuteAction();

}	// ExecuteAction


/************************************************************************
*    desc:  Inc the button display
************************************************************************/
void CUIAmountButton::SmallIncDisplay()
{
    if( (curValue + smallIncrease) <= maxValue )
        curValue += smallIncrease;
        
    // Wrap around the value
    else
        curValue = minValue;
    
    UpdateDisplayValue();

}	// SmallIncDisplay


/************************************************************************
*    desc:  Dec the button display
************************************************************************/
void CUIAmountButton::SmallDecDisplay()
{
    if( (curValue - smallIncrease) >= minValue )
        curValue -= smallIncrease;
        
    // Wrap around the value
    else
        curValue = maxValue;

    UpdateDisplayValue();

}	// SmallDecDisplay


/************************************************************************
*    desc:  Inc the button display
************************************************************************/
void CUIAmountButton::LargeIncDisplay()
{
    if( (curValue + largeIncrease) <= maxValue )
        curValue += largeIncrease;
        
    else if( curValue < maxValue )
        curValue = maxValue;

    // Wrap around the value
    else
        curValue = minValue;

    UpdateDisplayValue();

}	// LargeIncDisplay


/************************************************************************
*    desc:  Dec the button display
************************************************************************/
void CUIAmountButton::LargeDecDisplay()
{
    if( (curValue - largeIncrease) >= minValue )
        curValue -= largeIncrease;
        
    else if( curValue > minValue )
        curValue = minValue;

    // Wrap around the value
    else
        curValue = maxValue;

    UpdateDisplayValue();

}	// LargeDecDisplay


/************************************************************************
*    desc:  Update the amount displayed
************************************************************************/
void CUIAmountButton::UpdateDisplayValue()
{
    // Format for display
    string valueStr;

    if( displayValueAsInt )
        valueStr = boost::lexical_cast<string>( (int)curValue );
    else
        valueStr = boost::lexical_cast<string>( curValue );

    DisplayFontString(valueStr);

}	// UpdateSlider


/************************************************************************
*    desc:  Set the slider min value
*  
*    param: value
************************************************************************/
void CUIAmountButton::SetMinValue( float value )
{
    minValue = value;

}	// SetMinValue

float CUIAmountButton::GetMinValue() const
{
    return minValue;

}	// SetMinValue


/************************************************************************
*    desc:  Set the slider max value
*  
*    param: value
************************************************************************/
void CUIAmountButton::SetMaxValue( float value )
{
    maxValue = value;

}	// SetMaxValue

float CUIAmountButton::GetMaxValue() const
{
    return maxValue;

}	// SetMaxValue


/************************************************************************
*    desc:  Set/Get the current value
*  
*    param: value
************************************************************************/
void CUIAmountButton::SetValue( float value )
{
    curValue = value;

}	// SetValue

float CUIAmountButton::GetValue() const
{
    return curValue;

}	// GetValue



