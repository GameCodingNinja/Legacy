/************************************************************************
*    FILE NAME:       uiscrollbox.h
*
*    DESCRIPTION:     Class for user interface scroll boxes
************************************************************************/

#ifndef __ui_scroll_box_h__
#define __ui_scroll_box_h__

// Physical component dependency
#include <gui/uisubcontrol.h>

// Boost lib dependencies
#include <boost/ptr_container/ptr_vector.hpp>

// Game lib dependencies
#include <2d/visualsprite2d.h>
#include <utilities/timer.h>

class CUIScrollBox : public CUISubControl
{
public:
    
    // Constructor
    CUIScrollBox();

    // Load the control specific info from XML node
    virtual void LoadControlFromNode( const XMLNode & controlNode );

    // Transform the control
    void Transform();
    void Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                    const CWorldPoint & point );

    // Set the state of the transition
    virtual void SetTransitionState( NUIControl::ETransitionState value );

    // Get/Set the state of this control
    virtual void SetState( NUIControl::EControlState value );
    virtual NUIControl::EControlState GetState();

    // Get the last state of this control
    NUIControl::EControlState GetLastState();

    // Act in accordance to user input
    virtual void HandleUserNavigation( NUIMenu::EMenuNavigationState navState,
                                       const std::string & nextAction,
                                       const std::string & previousAction );

    // handle user select
    virtual bool HandleUserSelect( NUIMenu::EMenuNavigationState navState );

    // Update the control
    virtual void Update();

    // Render the control
    virtual void Render();

    // Reset the control to inactive if its not disabled
    virtual void Reset();

    // Reset the controls to inactive if its not disabled
    virtual void ResetControls();

    // Check to see if the control is animating
    virtual bool IsAnimating();

    // Do any smart executes
    virtual void ExecuteAction();

    // Designed to be over written for controls that hold other controls
    virtual void SetToFirstInactiveControl();

    // Get the action type
    virtual NUIControl::EControlActionType GetActionType();

    // Get the execution action
    virtual const std::string & GetExecutionAction();

    // Gets the active control
    virtual CUIControl * GetActiveControl();

    // Get the number of scroll controls
    size_t GetScrollCtrlCount();

    // Get the number of scroll controls
    CUIControl * GetScrollCtrl( const uint index );

    // Handle mouse wheel scrolling
    void HandleMouseWheelScroll();

    // Add the scroll control from node
    CUIControl * AddScrollControlFromNode( const XMLNode & controlNode );

    // Remove all scroll controls
    void RemoveAllScrollControls();

    // Remove scroll control by index
    void RemoveScrollControl( uint index );

    // Deactivates all controls 
    virtual void DeactivateControls( bool toMemu );

    // Reset the control's transition state
    virtual void ResetTransitionStates();

protected:

    // Correct Scroll position
    void CorrectScrollPos();

    // Handle the time based Scroll
    void HandleScrollingUpdate();

    // Set the scroll box control positions
    void SetScrollCtrlPos();

    // Init the start timers
    void InitStartTimer( bool initTimers );

    // Init the hold timers
    void InitHoldTimer( bool initTimers );

    // Init the keyboard/Gamepad scroll
    void InitKeyboardGamepadScroll( int scrollVector );

    // Scroll button press inc/dec
    void ScrollButtonIncDec( int scrollVector );

    // Was there a Page Up or Down
    bool WasPageUpDown( bool pageUp, bool pageDown );

    // Bounds check the first scroll control index
    void BoundsCheckFirstScrollCtrl();

    // Handle keyboard/Gamepad scrolling
    void HandleKeyboardGamepadScroll( NDevice::EActionPress prevActionPress,
                                      NDevice::EActionPress nextActionPress );

    // Handle mouse scrolling
    void HandleMouseScroll();

    // Set the start and end positions
    void SetStartEndPos();

    // Set the scroll box position via control index
    void SetFirstScrollCtrolPos( uint ctrlIndex = 0 );

private:

    enum
    {
        DEC_SCROLL,
        INC_SCROLL,
        SCROLL_BAR,
    };

    // Vector list of controlsin scroll box
    boost::ptr_vector<CUIControl> spScrollControlVec;

    // stencil mask sprite
    CVisualSprite2D stencilMaskSprite;

    // Height to cull
    float cullHeight;

    // height of control
    uint controlHeight;

    // Scroll move counter
    float scrollCurPos;

    // Max scroll amount
    float maxMoveAmount;

    // speed members
    float scrollSpeed;
    float pageSpeed;

    // Number of controls visible in scroll box
    int visibleCount;

    // Flag to indicate the scroll box is to default to
    // the first entry when being called from OnToMenu
    bool resetFromOnToMenu;

    // Visible start pos
    int visStartPos;
    int visEndPos;

    // Timer for when the player is holding down or up
    CTimer downWaitTimer;
    CTimer holdWaitTimer;

    // Flag to indicate the control is scrolling
    bool scrollUp;
    bool scrollDown;
    bool paging;

    // Scroll counter
    float scrollCounter;

    // Scroll distance
    float scrollDistance;

    // Active scroll control index in this control
    int activeScrollCtrl;

    // Last active scroll control index
    int lastActiveScrollCtrl;

    // index of first control in scroll box
    int firstScrollCtrlIndex;

    // Control actions
    std::string pageUpAction;
    std::string pageDownAction;

    // Initial scroll box control offset
    CPoint initialOffset;

};

#endif  // __ui_button_h__