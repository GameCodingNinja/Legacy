/************************************************************************
*    FILE NAME:       uisubcontrol.cpp
*
*    DESCRIPTION:     Class for user interface controls with sub-controls
************************************************************************/

// Physical component dependency
#include <gui/uisubcontrol.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <gui/uicontrolfactory.h>
#include <utilities/exceptionhandling.h>
#include <2d/spritegroup2d.h>
#include <controller/gamecontroller.h>

// Required namespace(s)
using namespace std;

typedef CUIControl CParent;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CUISubControl::CUISubControl()
             : activeSubCtrlIndex(NUIControl::NO_ACTIVE_CONTROL),
               baseReactsToSelect(true),
               respondsToAllStates(false),
               baseControlCollision(false),
               baseControlAsContainer(false)
{
    SetType( NUIControl::ECT_SUB_CONTROL );

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CUISubControl::~CUISubControl()
{
}   // Destructer


/************************************************************************
*    desc:  Load the control specific info from XML node
*  
*    param: node - XML node
************************************************************************/
void CUISubControl::LoadControlFromNode( const XMLNode & controlNode )
{
    // Have the parent load it's stuff
    CParent::LoadControlFromNode( controlNode );

    // Check for collision
    for( uint i = 0; i < spSpriteGroupVec.size() && !baseControlCollision; ++i )
    {
        if( spSpriteGroupVec[i].IsCollisionActive() )
            baseControlCollision = true;
    }

    // Get the sub-control settings
    XMLNode baseRespondsSelectNode = controlNode.getChildNode( "subControlSettings" );
    if( !baseRespondsSelectNode.isEmpty() )
    {
        // Does the base class respond to select. The default is true.
        if( baseRespondsSelectNode.isAttributeSet( "baseControlRespondsToSelect" ) )
            if( string( baseRespondsSelectNode.getAttribute( "baseControlRespondsToSelect" ) ) == "false" )
                baseReactsToSelect = false;

        // Do the sub-controls respond to all states. The default is false.
        if( baseRespondsSelectNode.isAttributeSet( "respondsToAllStates" ) )
            if( string( baseRespondsSelectNode.getAttribute( "respondsToAllStates" ) ) == "true" )
                respondsToAllStates = true;

        // Do the sub-controls respond gamepad/keyboard navigation.
        if( baseRespondsSelectNode.isAttributeSet( "baseControlAsContainer" ) )
            if( string( baseRespondsSelectNode.getAttribute( "baseControlAsContainer" ) ) == "true" )
            {
                baseControlAsContainer = true;

                // Don't want the base control to react to select when used as a container
                baseReactsToSelect = false;
            }
    }

    // Get the menu controls node
    XMLNode controlListNode = controlNode.getChildNode( "subControlList" );
    if( !controlListNode.isEmpty() )
    {
        for( int i = 0; i < controlListNode.nChildNode( "control" ); ++i )
        {
            XMLNode controlNode = controlListNode.getChildNode( "control", i );

            // The pointer is placed within a vector for all controls
            spSubControlVec.push_back( NUIControlFactory::Create( controlNode ) );
        }
    }

}	// LoadControlFromNode


/************************************************************************
*    desc:  Test for user mouse selection
*
*	parm: navState - navigation state
************************************************************************/
bool CUISubControl::CheckForUserMouseSelection( NUIMenu::EMenuNavigationState navState, NDevice::EActionPress actionPress )
{
    bool result = CParent::CheckForUserMouseSelection( navState, actionPress );
    
    if( !result && (activeSubCtrlIndex != NUIControl::NO_ACTIVE_CONTROL) )
        result = spSubControlVec[activeSubCtrlIndex].CheckForUserMouseSelection( navState, actionPress );

    return result;

}	// CheckForUserMouseSelection


/************************************************************************
*    desc:  Get the user mouse action press state
*
*	parm: EActionPress - action press state
************************************************************************/
NDevice::EActionPress CUISubControl::GetUserMouseActionPressState( NUIMenu::EMenuNavigationState navState )
{
    NDevice::EActionPress result = CParent::GetUserMouseActionPressState( navState );
    
    if( (result == NDevice::EAP_IDLE) && (activeSubCtrlIndex != NUIControl::NO_ACTIVE_CONTROL) )
        result = spSubControlVec[activeSubCtrlIndex].GetUserMouseActionPressState( navState );

    return result;

}	// GetUserMouseActionPressState


/************************************************************************
*    desc:  Set the transition state of this control
*  
*    param: ETransitionState value - transition state
************************************************************************/
void CUISubControl::SetTransitionState( NUIControl::ETransitionState value )
{
    // Call the parent
    CParent::SetTransitionState( value );

    for( size_t i = 0; i < spSubControlVec.size(); ++i )
        spSubControlVec[i].SetTransitionState( value );

}	// SetTransitionState


/************************************************************************
*    desc:  Set the state of this control
*  
*    param: EControlState value - control state
************************************************************************/
void CUISubControl::SetState( NUIControl::EControlState value )
{
    // Call the parent
    // if you don't want the parent responding to selection be sure not
    // to include the <selection selectAction="Menu Select" in the xml
    if( baseReactsToSelect || (value != NUIControl::ECS_SELECTED) )
        CParent::SetState( value );

    // coordinate these states with the internal controls 
    if( value < NUIControl::ECS_ACTIVE || respondsToAllStates )
    {
        for( size_t i = 0; i < spSubControlVec.size(); ++i )
            if( spSubControlVec[i].GetState() > NUIControl::ECS_DISABLED )
                spSubControlVec[i].SetState( value );
    }
    // Only the active control should respond to these states
    else
    {
        if( (activeSubCtrlIndex != NUIControl::NO_ACTIVE_CONTROL)  )
            spSubControlVec[activeSubCtrlIndex].SetState( value );
    }

}	// SetState


/************************************************************************
*    desc:  Set the state to the control and sub controls
*  
*    param: EControlState value - control state
************************************************************************/
void CUISubControl::SetAllStates( NUIControl::EControlState value )
{
    // Call the parent
    CParent::SetState( value );

    // Set all the sub-states
    SetSubStates( value );

}	// SetAllStates


/************************************************************************
*    desc:  Set the sub controls
*  
*    param: EControlState value - control state
************************************************************************/
void CUISubControl::SetSubStates( NUIControl::EControlState value )
{
    // Set all the sub-states
    for( size_t i = 0; i < spSubControlVec.size(); ++i )
        spSubControlVec[i].SetState( value );

}	// SetSubStates


/************************************************************************
*    desc:  Get the state of this control
*  
*    ret: EControlState - control state
************************************************************************/
NUIControl::EControlState CUISubControl::GetState()
{
    NUIControl::EControlState parentState = CParent::GetState();
    NUIControl::EControlState childState = NUIControl::ECS_NULL;

    if( activeSubCtrlIndex != NUIControl::NO_ACTIVE_CONTROL )
        childState = spSubControlVec[activeSubCtrlIndex].GetState();

    if( parentState > childState )
        return parentState;

    return childState;

}	// GetState


/************************************************************************
*    desc:  Get the last state of this control
*  
*    ret: EControlState - control state
************************************************************************/
NUIControl::EControlState CUISubControl::GetLastState()
{
    NUIControl::EControlState parentState = CParent::GetLastState();
    NUIControl::EControlState childState = NUIControl::ECS_NULL;

    if( activeSubCtrlIndex != NUIControl::NO_ACTIVE_CONTROL )
        childState = spSubControlVec[activeSubCtrlIndex].GetLastState();

    if( parentState > childState )
        return parentState;

    return childState;

}	// GetLastState


/************************************************************************
*    desc:  Reset the internal sub-controls to inactive if its not disabled
************************************************************************/
void CUISubControl::ResetSubControls()
{
    // Reset the internal controls
    for( size_t i = 0; i < spSubControlVec.size(); ++i )
        spSubControlVec[i].Reset();

}	// ResetSubControls


/************************************************************************
*    desc:  Get the control's action type
*  
*    ret: EControlActionType - control action type
************************************************************************/
NUIControl::EControlActionType CUISubControl::GetActionType()
{
    NUIControl::EControlActionType result = CParent::GetActionType();

    if( activeSubCtrlIndex != NUIControl::NO_ACTIVE_CONTROL )
        result = spSubControlVec[activeSubCtrlIndex].GetActionType();

    return result;

}	// GetActionType


/************************************************************************
*    desc:  Get the execution action
*  
*    ret:	string & - name of action
************************************************************************/
const string & CUISubControl::GetExecutionAction()
{
    if( activeSubCtrlIndex != NUIControl::NO_ACTIVE_CONTROL )
        return spSubControlVec[activeSubCtrlIndex].GetExecutionAction();
    else
        return CParent::GetExecutionAction();

}	// GetExecutionAction


/************************************************************************
*    desc:  Reset the control to inactive if its not disabled
************************************************************************/
void CUISubControl::Reset()
{
    CParent::Reset();

    // Reset the internal controls
    ResetSubControls();

}	// ResetControls


/************************************************************************
*    desc:  Act in accordance to user input
************************************************************************/
void CUISubControl::HandleUserNavigation( NUIMenu::EMenuNavigationState navState,
                                          const string & nextAction,
                                          const string & previousAction )
{
    // Update if the menu is active
    if( (GetState() == NUIControl::ECS_ACTIVE) && IsVisible() )
    {
        // Reset the control to inactive if its not disabled
        ResetSubControls();

        // Check and see if the mouse is inside any control
        if( (navState == NUIMenu::EMNS_MOUSE) && (!baseControlCollision || IsPointInSprite()) )
        {
            // In mouse nav state, deactivate the control
            activeSubCtrlIndex = NUIControl::NO_ACTIVE_CONTROL;

            // Change the state of the button if the mouse point is within the rect
            for( size_t i = 0; i < spSubControlVec.size(); ++i )
            {
                // Check and make sure the control isn't disabled or a label and visible, we've found the new active control
                // Check if the mouse pointer is within this control
                if( spSubControlVec[i].IsSelectable( navState ) && spSubControlVec[i].IsPointInSprite() )
                {
                    // Set the index of this control to the active control
                    activeSubCtrlIndex = static_cast<int>(i);

                    // Set this control active
                    spSubControlVec[activeSubCtrlIndex].SetState( NUIControl::ECS_ACTIVE );

                    // Have the control handle user navigation
                    spSubControlVec[activeSubCtrlIndex].HandleUserNavigation( navState, nextAction, previousAction );

                    // Resync the internal active control's animation with the base control
                    if( !spSubControlVec[activeSubCtrlIndex].IsStateChangeFinal() )
                    {
                        // Let the parent handle playing the "active" sound
                        // These conditions will fail if the parent collision box is even just 1 pixel greater the the sub control
                        if( GetPlayNavigationSnd() && (CParent::GetLastState() == NUIControl::ECS_INACTIVE) )
                        {
                            // Do nothing
                        }
                        // Let the sub-control pay the sound
                        else if( spSubControlVec[activeSubCtrlIndex].GetLastState() == NUIControl::ECS_INACTIVE )
                            spSubControlVec[activeSubCtrlIndex].PlayActiveSound();

                        ForceStateUpdate( navState );
                    }

                    break;
                }
            }
        }
        else if( baseControlAsContainer && (navState == NUIMenu::EMNS_GAMEPAD_KEYBAORD) )
        {
            // Check and see if we should move the focus to the next control
            if( CGameController::Instance().WasAction( nextAction, NDevice::EAP_DOWN ) )
            {
                for( size_t i = 0; i < spSubControlVec.size(); ++i )
                {
                    ++activeSubCtrlIndex;
                
                    // If the active control is equal to the number of controls, we
                    // loop around to the top of the control list
                    if( activeSubCtrlIndex == spSubControlVec.size() )
                        activeSubCtrlIndex = 0;

                    // Once we find a button that's not disabled or a label and visible, we've found the new active control
                    if( spSubControlVec[activeSubCtrlIndex].IsSelectable( navState ) )
                    {
                        spSubControlVec[activeSubCtrlIndex].PlayActiveSound();
                        break;
                    }
                }
            }
            // Check and see if we should move the focus to the previous control
            else if( CGameController::Instance().WasAction( previousAction, NDevice::EAP_DOWN ) )
            {
                for( size_t i = 0; i < spSubControlVec.size(); ++i )
                {
                    activeSubCtrlIndex--;

                    // If the active control is less than zero, we
                    // loop around to the back of the control list
                    if( activeSubCtrlIndex < 0 )
                        activeSubCtrlIndex = static_cast<int>(spSubControlVec.size() - 1);

                    // Once we find a button that's not disabled or a label and visible, we've found the new active control
                    if( spSubControlVec[activeSubCtrlIndex].IsSelectable( navState ) )
                    {
                        spSubControlVec[activeSubCtrlIndex].PlayActiveSound();
                        break;
                    }
                }
            }

            // if we only have one selectable control via keyboard/gamepad keep it active when in this mode
            if( activeSubCtrlIndex == NUIControl::NO_ACTIVE_CONTROL )
            {
                int selectableControlCount(0);
                int lastSelectableControl;

                for( size_t i = 0; i < spSubControlVec.size(); ++i )
                {
                    if( spSubControlVec[i].IsSelectable( navState ) )
                    {
                        ++selectableControlCount;
                        lastSelectableControl = static_cast<int>(i);
                    }

                    if( selectableControlCount == 1 )
                        activeSubCtrlIndex = lastSelectableControl;
                }
            }

            // Set the active control to active
            if( activeSubCtrlIndex != NUIControl::NO_ACTIVE_CONTROL )
            {
                spSubControlVec[activeSubCtrlIndex].SetState( NUIControl::ECS_ACTIVE );
                spSubControlVec[activeSubCtrlIndex].HandleUserNavigation( navState, nextAction, previousAction );

                // Resync the base control animation with the sub control
                if( !spSubControlVec[activeSubCtrlIndex].IsStateChangeFinal() )
                    ForceStateUpdate( navState );
            }
        }
    }

}	// HandleUserNavigation


/************************************************************************
*    desc:  handle user select
*
*	parm: navState - navigation state
************************************************************************/
bool CUISubControl::HandleUserSelect( NUIMenu::EMenuNavigationState navState )
{
    bool result(false);

    if( baseReactsToSelect )
        result = CParent::HandleUserSelect( navState );

    if( !result && (activeSubCtrlIndex != NUIControl::NO_ACTIVE_CONTROL) )
        result = spSubControlVec[activeSubCtrlIndex].HandleUserSelect( navState );

    return result;

}	// HandleUserSelect


/************************************************************************
*    desc:  Respond to user select
************************************************************************/
void CUISubControl::RespondToUserSelect()
{
    ForceStateUpdate();

    // resync any animations by forcing a state update
    for( size_t i = 0; i < spSubControlVec.size(); ++i )
    {
        spSubControlVec[i].RespondToUserSelect();
        spSubControlVec[i].ForceStateUpdate();
    }

}	// RespondToUserSelect


/************************************************************************
*    desc:  Sets the inactive control with the lowest index in the control
*			vector to active
************************************************************************/
void CUISubControl::SetToFirstInactiveControl()
{
    if( baseControlAsContainer )
    {
        ResetSubControls();

        activeSubCtrlIndex = NUIControl::NO_ACTIVE_CONTROL;

        for( size_t i = 0; i < spSubControlVec.size(); ++i )
        {
            if( !((spSubControlVec[i].GetState() == NUIControl::ECS_DISABLED) ||
                  (spSubControlVec[i].GetType() == NUIControl::ECT_LABEL) ||
                  spSubControlVec[i].IsMouseOnlyCtrl() ||
                  !spSubControlVec[i].IsVisible() ) )
            {
                activeSubCtrlIndex = static_cast<int>(i);

                // If the control is a container for other controls,
                // have it set the first control to active
                spSubControlVec[i].SetToFirstInactiveControl();
                break;
            }
        }
    }

}	// SetToFirstInactiveControl


/************************************************************************
*    desc:  Execute the action
************************************************************************/
void CUISubControl::ExecuteAction()
{
    // pass onto all controls
    for( size_t i = 0; i < spSubControlVec.size(); ++i )
        spSubControlVec[i].ExecuteAction();

    // Call the parent
    if( baseReactsToSelect )
        CParent::ExecuteAction();

}	// ExecuteAction


/************************************************************************
*    desc:  Update the button
************************************************************************/
void CUISubControl::Update()
{
    // Call the parent
    CParent::Update();

    // Update all controls
    for( size_t i = 0; i < spSubControlVec.size(); ++i )
        spSubControlVec[i].Update();

}	// Update


/************************************************************************
*    desc:  Transform the control
************************************************************************/
void CUISubControl::Transform()
{
    if( GetParameters().IsSet( CObject::TRANSFORM ) )
    {
        CBitmask tmpTransParam = GetParameters();
        CParent::Transform();

        // Tell the children of this control that they need to transform
        for( size_t i = 0; i < spSubControlVec.size(); ++i )
            spSubControlVec[i].SetTransformParameters( tmpTransParam, true );
    }

    // Transform the parent class's components
    CParent::Transform();

    for( size_t i = 0; i < spSubControlVec.size(); ++i )
        spSubControlVec[i].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

}	// Transform


/************************************************************************
*    desc:  Transform the control
*
*	 param: const CMatrix & _scaledMatrix  - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point      - passed in relative position
************************************************************************/
void CUISubControl::Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                               const CWorldPoint & point )
{
    if( GetParameters().IsSet( CObject::TRANSFORM ) )
    {
        CBitmask tmpTransParam = GetParameters();
        CParent::Transform( _scaledMatrix, _unscaledMatrix, point );

        // Tell the children of this control that they need to transform
        for( size_t i = 0; i < spSubControlVec.size(); ++i )
            spSubControlVec[i].SetTransformParameters( tmpTransParam, true );
    }

    // Transform the parent class's components
    CParent::Transform( _scaledMatrix, _unscaledMatrix, point );

    // Transform the control's visual components
    for( size_t i = 0; i < spSubControlVec.size(); ++i )
        spSubControlVec[i].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );
    
}	// Transform


/************************************************************************
*    desc:  Check to see if the control is animating
************************************************************************/
bool CUISubControl::IsAnimating()
{
    bool animating(false);
    
    if( baseReactsToSelect || spSubControlVec.empty() )
        animating = CParent::IsAnimating();

    //if( !animating && (activeSubCtrlIndex != NUIControl::NO_ACTIVE_CONTROL) )
        //animating = spSubControlVec[activeSubCtrlIndex].IsAnimating();

    for( uint i = 0; i < spSubControlVec.size() && !animating; ++i )
        if( spSubControlVec[i].IsAnimating() )
            animating = true;

    return animating;

}	// IsAnimating


/************************************************************************
*    desc:  Render the sub control
************************************************************************/
void CUISubControl::Render()
{
    // Call the parent
    CParent::Render();

    if( IsVisible() )
    {
        for( size_t i = 0; i < spSubControlVec.size(); ++i )
            spSubControlVec[i].Render();
    }

}	// Render


/************************************************************************
*    desc:  Get/Set the active sub-control index
************************************************************************/
int CUISubControl::GetActiveSubControlIndex()
{
    return activeSubCtrlIndex;

}	// GetActiveSubControlIndex

void CUISubControl::SetActiveSubControlIndex( int value )
{
    activeSubCtrlIndex = value;

}	// SetActiveSubControlIndex


/************************************************************************
*    desc:  Get the sub control
*  
*    ret:	CUIControl &
************************************************************************/
CUIControl * CUISubControl::GetSubControl( uint index )
{
    if( index >= spSubControlVec.size() )
        throw NExcept::CCriticalException("Index out of range",
            boost::str( boost::format("Index out of range of vector size (%d,%d).\n\n%s\nLine: %s") % index % spSubControlVec.size() % __FUNCTION__ % __LINE__ ));

    return &spSubControlVec[index];

}	// GetExecutionAction


/************************************************************************
*    desc:  Get sub control count
************************************************************************/
size_t CUISubControl::GetSubControlCount()
{
    return spSubControlVec.size();

}	// GetSubControlCount


/************************************************************************
*    desc:  Reset the transition states 
************************************************************************/
void CUISubControl::ResetTransitionStates()
{
    CParent::ResetTransitionStates();

    for( size_t i = 0; i < spSubControlVec.size(); ++i )
        spSubControlVec[i].ResetTransitionStates();

}	// ResetTransitionStates


/************************************************************************
*    desc:  Deactivates all controls 
************************************************************************/
void CUISubControl::DeactivateControls( bool toMemu )
{
    CParent::DeactivateControls( toMemu );

    for( size_t i = 0; i < spSubControlVec.size(); ++i )
        spSubControlVec[i].DeactivateControls( toMemu );

}	// DeactivateControls


/************************************************************************
*    desc:  Play the active sound of the active control 
************************************************************************/
void CUISubControl::PlaySubControlActiveSound()
{
    if( activeSubCtrlIndex != NUIControl::NO_ACTIVE_CONTROL )
        spSubControlVec[activeSubCtrlIndex].PlayActiveSound();

}	// PlaySubControlActiveSound


/************************************************************************
*    desc:  Play the select sound of the active control 
************************************************************************/
void CUISubControl::PlaySubControlSelectSound()
{
    if( activeSubCtrlIndex != NUIControl::NO_ACTIVE_CONTROL )
        spSubControlVec[activeSubCtrlIndex].PlaySelectSound();

}	// PlaySubControlSelectSound


