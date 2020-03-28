
/************************************************************************
*    FILE NAME:       uitabcontrol.h
*
*    DESCRIPTION:     Class for user interface tab control
************************************************************************/

#ifndef __ui_tab_control_h__
#define __ui_tab_control_h__

// Physical component dependency
#include <gui/uisubcontrol.h>

// Boost lib dependencies
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/scoped_ptr.hpp>

class CUITabControl : public CUISubControl
{
public:

    // Constructor
    CUITabControl();

    // Load the initial info from XML node
    virtual void LoadFromNode( const XMLNode & node );

    // Load the control specific info from XML node
    virtual void LoadControlFromNode( const XMLNode & controlNode );

    // Transform the control
    void Transform();
    void Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                    const CWorldPoint & point );

    // Get/Set the state of this control
    virtual void SetState( NUIControl::EControlState value );
    virtual NUIControl::EControlState GetState();

    // Get the last state of this control
    virtual NUIControl::EControlState GetLastState();

    // Update the control
    virtual void Update();

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

    // Execute the action
    virtual void ExecuteAction();

    // Render the control
    virtual void Render();

    // Reset the control's transition state
    virtual void ResetTransitionStates();

    // Set the state of the transition
    virtual void SetTransitionState( NUIControl::ETransitionState value );

    // Force a state update
    virtual void ForceStateUpdate( NUIMenu::EMenuNavigationState navState = NUIMenu::EMNS_NULL );

    // Get the type of action
    virtual NUIControl::EControlActionType GetActionType();

    // Respond to user select
    virtual void RespondToUserSelect();

protected:

    // Was there a left/right select
    bool WasDecIncSelect( bool decAction, bool incAction );

    // Test for user selection
    virtual bool CheckForUserSelection( NUIMenu::EMenuNavigationState navState );

private:

    // Vector list of tab controls
    boost::ptr_vector<CUIControl> spTabControlVec;

    // Control selection actions
    std::string decSelectAction;
    std::string incSelectAction;

    // Active tab control index in this control
    int activeTabCtrl;

    // Tab arrow buttons
    boost::scoped_ptr<CUIControl> decArrowCtrl;
    boost::scoped_ptr<CUIControl> incArrowCtrl;

    // flag indicating keyboard tab selection
    bool keyboardGamePadTabSelection;

};

#endif  // __ui_tab_control_h__


