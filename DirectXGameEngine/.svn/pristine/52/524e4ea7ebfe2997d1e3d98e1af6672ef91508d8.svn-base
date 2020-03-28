/************************************************************************
*    FILE NAME:       uicontrol.h
*
*    DESCRIPTION:     base class for DirectX user interface controls
************************************************************************/

#ifndef __ui_control_h__
#define __ui_control_h__

// Physical component dependency
#include <common/object.h>

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/scoped_ptr.hpp>

// Game lib dependencies
#include <xmlParser/xmlParser.h>
#include <common/size.h>
#include <common/matrix.h>
#include <common/defs.h>
#include <gui/uicontroldefs.h>
#include <gui/uimenudefs.h>
#include <controller/controllerdefs.h>

// Forward declaration(s)
class CSpriteGroup2D;
class CFontGroup;
class CSmartGuiControl;

class CUIControl : public CObject
{
public:

    CUIControl();
    virtual ~CUIControl();

    // Load the initial info from XML node
    virtual void LoadFromNode( const XMLNode & node );

    // Get/Set the control type
    NUIControl::EControlType GetType();
    void SetType( NUIControl::EControlType value );

    // Set or get the execution action
    void SetExecutionAction( const std::string & action );
    virtual const std::string & GetExecutionAction();

    // Set or get the state of this control
    virtual void SetState( NUIControl::EControlState value );
    virtual void SetState( const std::string & value );
    virtual NUIControl::EControlState GetState();

    // Is the state change final
    bool IsStateChangeFinal();

    // Get the last state of this control
    virtual NUIControl::EControlState GetLastState();
    void FinalizeStateChange();

    // Force a state update
    virtual void ForceStateUpdate( NUIMenu::EMenuNavigationState navState = NUIMenu::EMNS_NULL );

    // Set the state of the transition
    virtual void SetTransitionState( NUIControl::ETransitionState value );
    NUIControl::ETransitionState GetTransitionState();

    // Get the last transition state
    NUIControl::ETransitionState GetLastTransitionState();

    // Is the state change final
    bool IsTransitionStateChangeFinal();

    // Is the transition state active
    bool IsTransitionStateActive();

    // Make the state change final
    void FinalizeTransitionStateChange();

    // Set the default state of this control
    void SetDefaultState( NUIControl::EControlState value );
    void SetDefaultState( std::string & value );

    // Set or get the type of action
    void SetActionType( NUIControl::EControlActionType value );
    void SetActionType( const std::string & value );
    virtual NUIControl::EControlActionType GetActionType();

    // Set the control back to its default state or inactive state
    virtual void RevertToDefaultState();

    // Reset the control to inactive if its not disabled
    virtual void Reset();

    // Is the mouse in the sprite
    virtual bool IsPointInSprite();

    // Is this control selectable
    bool IsSelectable( NUIMenu::EMenuNavigationState navState );

    // Set the string to the string list vector
    void SetToFontStringList( const std::string & str, uint fontGrpIndex = 0 );

    // Clear the string list vector
    void ClearFontStringList( uint fontGrpIndex = 0 );

    // Use the string index to display
    void DisplayFontStringIndex( uint fontGrpIndex = 0, uint strIndex = 0 );

    // Display a specific string in the control
    void DisplayFontString( const std::string & displayStr, uint fontGrpIndex = 0 );

    // Clear out all the font group strings
    void ClearAllFontStringList();

    // Update the control
    virtual void Update();

    // Transform the control
    void Transform();
    void Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                    const CWorldPoint & point );

    // Update the sprites associated with this control
    virtual void UpdateSprites();

    // Render the control
    virtual void Render();

    // Check to see if the control is animating
    virtual bool IsAnimating();

    // Handle user navigation
    virtual void HandleUserNavigation( NUIMenu::EMenuNavigationState navState,
                                       const std::string & nextAction,
                                       const std::string & previousAction );

    // handle user select
    virtual bool HandleUserSelect( NUIMenu::EMenuNavigationState navState );

    // Respond to user select
    virtual void RespondToUserSelect();

    // Set the AI pointer
    void SetSmartGui( CSmartGuiControl * pSmartGuiControl );

    // Do any smart create
    void SmartCreate();

    // Execute the action
    virtual void ExecuteAction();

    // Do any smart Display
    void SmartDisplay();

    // Get the smart gui pointer
    CSmartGuiControl * GetSmartGuiPtr();

    // Get the font string
    const std::string & GetFontString( uint fontIndex = 0, uint stringIndex = 0 );

    // handle user select mouse selection
    bool HandleUserMouseSelection( NUIMenu::EMenuNavigationState navState, NDevice::EActionPress actionPress );

    // Check for user mouse selection
    virtual bool CheckForUserMouseSelection( NUIMenu::EMenuNavigationState navState, NDevice::EActionPress actionPress );

    // Get the user mouse action press state
    virtual NDevice::EActionPress GetUserMouseActionPressState( NUIMenu::EMenuNavigationState navState );

    // Set the state to the control and sub sontrols
    virtual void SetAllStates( NUIControl::EControlState value,
                               NUIControl::EControlState lessThenCurState ){};

    // Play the control "active" sound
    void PlayActiveSound();

    // Play the control "select" sound
    void PlaySelectSound();

    // Get the string being displayed by the sprite font
    std::string GetDisplayedString( uint fontGrpIndex = 0 );

    // Set/Get the conflicted flag
    void SetConflictedFlag( bool value );
    bool GetConflictedFlag() const;

    // Do we play navigation sounds?
    bool GetPlayNavigationSnd();

    // Get the object group name
    const std::string & GetObjGroup() const;

    // Set/Get string ID
    const std::string & GetName() const;
    void SetName( const std::string & nameStr );

    // Is the font group empty
    bool IsFontGroupEmpty() const;

    // Is the font string group empty
    bool IsFontStringGroupEmpty( uint fontIndex = 0 ) const;

    // Designed to be over written for controls that hold other controls
    virtual void SetToFirstInactiveControl();

    // Gets the active control
    virtual CUIControl * GetActiveControl();

    // Get the sub control
    virtual CUIControl * GetSubControl( uint index );

    // Get sub control count
    virtual size_t GetSubControlCount();

    // Deactivates all controls 
    virtual void DeactivateControls(bool toMemu);

    // Reset the control's transition state
    virtual void ResetTransitionStates();

    // Set the dynamic position
    virtual void SetDynamicPos();

    // Get Control size
    const CSize<float> & GetSize() const;

    // Get the active sub-control index
    virtual int GetActiveSubControlIndex();
    virtual void SetActiveSubControlIndex( int value );

    // Get the select action
    const std::string & GetSelectAction() const;

    // Is this a mouse only control?
    bool IsMouseOnlyCtrl() const;

protected:

    // Load the control specific info from XML node
    virtual void LoadControlFromNode( const XMLNode & node );

    // Set the controls command based on state
    virtual void SetControlCommand();

    // Test for user selection
    virtual bool CheckForUserSelection( NUIMenu::EMenuNavigationState navState );

    // A selection has been confirmed
    virtual void UserSelectionConfirmed( NUIMenu::EMenuNavigationState navState );

    // Records the owner on the down action
    void SetActionOwner( NDevice::EActionPress controlActionPress );
    bool IsActionOwner();

    // Create a sprite group
    CSpriteGroup2D * CreateSpriteGroup( const XMLNode & objDataNode );

    // Set the active command string
    virtual void SetActiveCommand( const std::string & commandStr );

protected:

    // sprite group vector
    boost::ptr_vector<CSpriteGroup2D> spSpriteGroupVec;

    // font sprite group
    boost::ptr_vector<CFontGroup> spFontGroupVec;

private:

    // Object group name
    std::string objGroupStr;

    // The type of control
    NUIControl::EControlType type;

    // Name of the action to perform under the correct circumstances
    std::string executionAction;

    // control's current state and previous state
    NUIControl::EControlState state;
    NUIControl::EControlState lastState;

    // The control's transition state and previous transition state
    NUIControl::ETransitionState transitionState;
    NUIControl::ETransitionState lastTransitionState;

    // control's default state
    NUIControl::EControlState defaultState;

    // How the control should respond when selected
    NUIControl::EControlActionType actionType;

    // String id
    std::string name;

    // Control selection actions
    std::string selectAction;

    // Base control action class scoped pointer
    boost::scoped_ptr<CSmartGuiControl> spSmartGui;

    // flag to indicate a state update is needed
    bool forceStateUpdate;

    // Flag to indicate if the control should play a sound
    bool playNavigationSnd;

    // Flag to indicate the control is conflicted
    bool conflicted;

    // Mouse action press type
    NDevice::EActionPress mouseActionPress;

    // Flag to indicate this control disbatches an action event
    bool dispatchActionEvent;

    // This menu's size
    CSize<float> size;

    // flag that indicates this control is a mouse only control
    bool mouseOnlyCtrl;

};

#endif  // __ui_control_h__
