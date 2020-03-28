/************************************************************************
*    FILE NAME:       uiamountbutton.h
*
*    DESCRIPTION:     Class for user interface buttons
************************************************************************/

#ifndef __ui_amount_button_h__
#define __ui_amount_button_h__

// Physical component dependency
#include <gui/uisubcontrol.h>

// Boost lib dependencies
#include <boost/ptr_container/ptr_vector.hpp>

// Game lib dependencies
#include <common/defs.h>

class CUIAmountButton : public CUISubControl
{
public:

    // Increment/Decement constants
    enum
    {
        BTN_TMP = NUIControl::BTN_INCREMENT,
        BTN_DECREMENT_BY_VALUE,
        BTN_INCREMENT_BY_VALUE,
        BTN_CUSTOM_SELECT
    };
    
    CUIAmountButton();
    virtual ~CUIAmountButton();

    // Load the initial info from XML node
    virtual void LoadFromNode( const XMLNode & node );

    // Act in accordance to user input
    virtual void HandleUserNavigation( NUIMenu::EMenuNavigationState navState,
                                       const std::string & nextAction,
                                       const std::string & previousAction );

    // Inc/Dec the button display
    virtual void SmallIncDisplay();
    virtual void SmallDecDisplay();
    virtual void LargeIncDisplay();
    virtual void LargeDecDisplay();

    // Execute the action
    virtual void ExecuteAction();

    // handle user select
    virtual bool HandleUserSelect( NUIMenu::EMenuNavigationState navState );

    // Get the state of this control
    virtual NUIControl::EControlState GetState();

    // Set the slider min value
    void SetMinValue( float value );
    float GetMinValue() const;

    // Set the slider max value
    void SetMaxValue( float value );
    float GetMaxValue() const;

    // Update the amount displayed
    void UpdateDisplayValue();

    // Get the current value
    void SetValue( float value );
    float GetValue() const;

protected:

    // Was there a left/right/up/down or button EMNS_GAMEPAD_KEYBAORD select
    bool WasSelect( bool decSmallAction, bool incSmallAction, bool decLargeAction, bool incLargeAction, bool customAction );

    // Test for user selection
    virtual bool CheckForUserSelection( NUIMenu::EMenuNavigationState navState );

private:

    // Control selection actions
    std::string decSelectSmallAction;
    std::string incSelectSmallAction;
    std::string decSelectLargeAction;
    std::string incSelectLargeAction;
    std::string customSelectAction;

    // Min value
    float minValue;

    // Max value
    float maxValue;

    // Current value
    float curValue;

    // Increase variables
    float smallIncrease;
    float largeIncrease;

    // Flag to indicate to display the value as an int
    bool displayValueAsInt;

};

#endif  // __ui_amount_button_h__