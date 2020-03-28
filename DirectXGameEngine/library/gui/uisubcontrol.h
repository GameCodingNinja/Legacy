/************************************************************************
*    FILE NAME:       uisubcontrol.h
*
*    DESCRIPTION:     Class for user interface controls with sub-controls
************************************************************************/

#ifndef __ui_sub_control_h__
#define __ui_sub_control_h__

// Physical component dependency
#include <gui/uicontrol.h>

// Boost lib dependencies
#include <boost/ptr_container/ptr_vector.hpp>

class CUISubControl : public CUIControl
{
public:
    
    CUISubControl();
    virtual ~CUISubControl();

    // Set the state of the transition
    virtual void SetTransitionState( NUIControl::ETransitionState value );

    // Get/Set the state of this control
    virtual void SetState( NUIControl::EControlState value );
    virtual NUIControl::EControlState GetState();

    // Check for user mouse selection
    virtual bool CheckForUserMouseSelection( NUIMenu::EMenuNavigationState navState, NDevice::EActionPress actionPress );

    // Get the user mouse action press state
    virtual NDevice::EActionPress GetUserMouseActionPressState( NUIMenu::EMenuNavigationState navState );

    // Set the sub controls
    void SetSubStates( NUIControl::EControlState value );

    // Set the state to the control and sub sontrols
    void SetAllStates( NUIControl::EControlState value );

    // Get the last state of this control
    virtual NUIControl::EControlState GetLastState();

    // Get the action type
    virtual NUIControl::EControlActionType GetActionType();

    // Get the execution action
    virtual const std::string & GetExecutionAction();

    // Check to see if the control is animating
    virtual bool IsAnimating();

    // Act in accordance to user input
    virtual void HandleUserNavigation( NUIMenu::EMenuNavigationState navState,
                                       const std::string & nextAction,
                                       const std::string & previousAction );

    // handle user select
    virtual bool HandleUserSelect( NUIMenu::EMenuNavigationState navState );

    // Sets the inactive control
    virtual void SetToFirstInactiveControl();

    // Respond to user select
    virtual void RespondToUserSelect();

    // Get the sub control
    virtual CUIControl * GetSubControl( uint index = 0 );

    // Get sub control count
    size_t GetSubControlCount();

    // Reset the control to inactive if its not disabled
    virtual void Reset();

    // Transform the control
    void Transform();
    void Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                    const CWorldPoint & point );

    // Update the control
    virtual void Update();

    // Render the control
    virtual void Render();

    // Reset the control's transition state
    virtual void ResetTransitionStates();

    // Deactivates all controls 
    virtual void DeactivateControls( bool toMemu );

    // Get the active sub-control index
    virtual int GetActiveSubControlIndex();
    virtual void SetActiveSubControlIndex( int value );

    // Play the active sound of the active control
    void PlaySubControlActiveSound();

    // Play the select sound of the active control
    void PlaySubControlSelectSound();

    // Execute the action
    virtual void ExecuteAction();

protected:

    // Load the control info from XML node
    virtual void LoadControlFromNode( const XMLNode & controlNode );

    // Reset the sub-controls to inactive if its not disabled
    virtual void ResetSubControls();

private:

    // Active sub-control Index in this control
    int activeSubCtrlIndex;

    // Vector list of sub-controls
    boost::ptr_vector<CUIControl> spSubControlVec;

    // Base class reacts to select
    bool baseReactsToSelect;

    // Sub-controls responds to all states
    bool respondsToAllStates;

    // flag that indicates base control colision detection
    bool baseControlCollision;

    // flag to indicate base control used as a container
    bool baseControlAsContainer;

};

#endif  // __ui_sub_control_h__