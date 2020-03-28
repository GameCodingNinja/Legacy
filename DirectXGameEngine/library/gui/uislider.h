/************************************************************************
*    FILE NAME:       uislider.h
*
*    DESCRIPTION:     Class for user interface slider
************************************************************************/

#ifndef __ui_slider_h__
#define __ui_slider_h__

// Physical component dependency
#include <gui/uisubcontrol.h>

// Game lib dependencies
#include <utilities/timer.h>

class CUISlider : public CUISubControl
{
public:

    enum EOrientation
    {
        EO_HORIZONTAL,
        EO_VERTICAL
    };
    
    CUISlider();
    virtual ~CUISlider();

    // Load the initial info from XML node
    virtual void LoadFromNode( const XMLNode & node );

    // handle user select
    virtual bool HandleUserSelect( NUIMenu::EMenuNavigationState navState );

    // Set the slider min value
    void SetMinValue( float value );

    // Set the slider max value
    void SetMaxValue( float value );

    // Set the slider position
    void SetSlider( float value = 0 );

    // Set the slider inc value
    void IncSlider( float value = 0 );

    // Update the slider
    void UpdateSlider();

    // Get the current value
    float GetValue()
    { return curValue; }

    // Act in accordance to user input
    virtual void HandleUserNavigation( NUIMenu::EMenuNavigationState navState,
                                       const std::string & nextAction,
                                       const std::string & previousAction );

    // Respond to user select
    virtual void RespondToUserSelect();

    // Get the state of this control
    virtual NUIControl::EControlState GetState();

    // Force a state update
    virtual void ForceStateUpdate( NUIMenu::EMenuNavigationState navState );

protected:

    // Load the control info from XML node
    virtual void LoadControlFromNode( const XMLNode & controlNode );

private:

    // Cap the slider value
    void CapSliderValue();

    // Set the position of the slider
    void SetSliderPos();

    // Inc/Set the slider based on mouse movement
    void IncSliderMovePos( float value );

private:

    // Slider travel distance in pixels
    float travelDistPixels;

    // Slider Orientation
    EOrientation orientation;

    // Min value
    float minValue;

    // Max value
    float maxValue;

    // Current value
    float curValue;

    // slider button hold flag
    bool sliderBtnHold;

    // Timer for when the player is holding down or up
    CTimer moveWaitTimer;
    CTimer moveTimer;

    // Control actions
    std::string decSelectAction;
    std::string incSelectAction;

    // Flag to indicate to display the value as an int
    bool displayValueAsInt;

};

#endif  // __ui_slider_h__