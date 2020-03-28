
/************************************************************************
*    FILE NAME:       uitabcontrol.cpp
*
*    DESCRIPTION:     Class for user interface tab control
************************************************************************/

// Physical component dependency
#include <gui/uitabcontrol.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <gui/controlevent.h>
#include <gui/uicontrolfactory.h>
#include <controller/gamecontroller.h>
#include <utilities/exceptionhandling.h>

// Required namespace(s)
using namespace std;

typedef CUISubControl CParent;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CUITabControl::CUITabControl()
             : activeTabCtrl(0),
               keyboardGamePadTabSelection(false)
{
    SetType( NUIControl::ECT_TAB_CONTROL );

}   // constructor


/************************************************************************
*    desc:  Load the control info from XML node
*  
*    param: node - XML node
************************************************************************/
void CUITabControl::LoadFromNode( const XMLNode & node )
{
    CParent::LoadFromNode( node );

    // Get the selection string for this control
    XMLNode selectionNode = node.getChildNode( "selection" );

    // Load the control specific xml file
    if( !selectionNode.isEmpty() )
    {
        if( selectionNode.isAttributeSet( "decSelect" ) )
            decSelectAction = selectionNode.getAttribute( "decSelect" );

        if( selectionNode.isAttributeSet( "incSelect" ) )
            incSelectAction = selectionNode.getAttribute( "incSelect" );
    }

}	// LoadFromNode


/************************************************************************
*    desc:  Load the control specific info from XML node
*  
*    param: controlNode - XML node
************************************************************************/
void CUITabControl::LoadControlFromNode( const XMLNode & controlNode )
{
    CParent::LoadControlFromNode( controlNode );

    // Get the menu controls node
    const XMLNode tabControlsNode = controlNode.getChildNode( "tabControlList" );
    if( !tabControlsNode.isEmpty() )
    {
        // Get the number of controls in this scroll box
        const int tabControlCount = tabControlsNode.nChildNode( "control" );

        for( int i = 0; i < tabControlCount; ++i )
        {
            XMLNode controlNode = tabControlsNode.getChildNode( "control", i );

            // The pointer is placed within a vector for all controls
            spTabControlVec.push_back( NUIControlFactory::Create( controlNode ) );
        }
    }

    // Get the dec awwor button
    const XMLNode decArrowNode = controlNode.getChildNode( "decBumperBtn" );
    if( !decArrowNode.isEmpty() )
    {
        decArrowCtrl.reset( NUIControlFactory::Create( decArrowNode.getChildNode( "control" ) ) );
    }

    // Get the inc awwor button
    const XMLNode incArrowNode = controlNode.getChildNode( "incBumperBtn" );
    if( !incArrowNode.isEmpty() )
    {
        incArrowCtrl.reset( NUIControlFactory::Create( incArrowNode.getChildNode( "control" ) ) );
    }

    if( GetSubControlCount() != spTabControlVec.size() )
        throw NExcept::CCriticalException("Tab Count Mismatch",
            boost::str( boost::format("No. of tabs vs tab groups don't match (%d,%d).\n\n%s\nLine: %s") % GetSubControlCount() % spTabControlVec.size() % __FUNCTION__ % __LINE__ ));

}	// LoadControlFromNode


/************************************************************************
*    desc:  Transform the control
************************************************************************/
void CUITabControl::Transform()
{
    if( GetParameters().IsSet( CObject::TRANSFORM ) )
    {
        CBitmask tmpTransParam = GetParameters();
        CParent::Transform();

        // Tell the children of this control that they need to transform
        for( size_t i = 0; i < spTabControlVec.size(); ++i )
            spTabControlVec[i].SetTransformParameters( tmpTransParam, true );

        if( decArrowCtrl != NULL )
            decArrowCtrl->SetTransformParameters( tmpTransParam, true );

        if( incArrowCtrl != NULL )
            incArrowCtrl->SetTransformParameters( tmpTransParam, true );
    }

    // Transform the parent class's components
    CParent::Transform();

    // Transform the control's components
    for( size_t i = 0; i < spTabControlVec.size(); ++i )
        spTabControlVec[i].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

    if( decArrowCtrl != NULL )
        decArrowCtrl->Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

    if( incArrowCtrl != NULL )
        incArrowCtrl->Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

}	// Transform


/************************************************************************
*    desc:  Transform the control
*
*	 param: const CMatrix & _scaledMatrix  - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point      - passed in relative position
************************************************************************/
void CUITabControl::Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                               const CWorldPoint & point )
{
    if( GetParameters().IsSet( CObject::TRANSFORM ) )
    {
        CBitmask tmpTransParam = GetParameters();
        CParent::Transform( _scaledMatrix, _unscaledMatrix, point );

        // Tell the children of this control that they need to transform
        for( size_t i = 0; i < spTabControlVec.size(); ++i )
            spTabControlVec[i].SetTransformParameters( tmpTransParam, true );

        if( decArrowCtrl != NULL )
            decArrowCtrl->SetTransformParameters( tmpTransParam, true );

        if( incArrowCtrl != NULL )
            incArrowCtrl->SetTransformParameters( tmpTransParam, true );
    }

    // Transform the parent class's components
    CParent::Transform( _scaledMatrix, _unscaledMatrix, point );

    // Transform the control's components
    for( size_t i = 0; i < spTabControlVec.size(); ++i )
        spTabControlVec[i].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

    if( decArrowCtrl != NULL )
        decArrowCtrl->Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

    if( incArrowCtrl != NULL )
        incArrowCtrl->Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );
    
}	// Transform


/************************************************************************
*    desc:  Set the state of this control
*  
*    param: EControlState value - control state
************************************************************************/
void CUITabControl::SetState( NUIControl::EControlState value )
{
    CParent::SetState( value );

    if( value < NUIControl::ECS_SELECTED )
        spTabControlVec[activeTabCtrl].SetState( value );

}	// SetState


/************************************************************************
*    desc:  Get the state of this control
*  
*    ret: EControlState - control state
************************************************************************/
NUIControl::EControlState CUITabControl::GetState()
{
    NUIControl::EControlState parentState = CParent::GetState();

    NUIControl::EControlState childState = spTabControlVec[activeTabCtrl].GetState();

    if( parentState > childState )
        return parentState;

    return childState;

}	// GetState


/************************************************************************
*    desc:  Get the last state of this control
*  
*    ret: EControlState - control state
************************************************************************/
NUIControl::EControlState CUITabControl::GetLastState()
{
    NUIControl::EControlState parentState = CParent::GetLastState();

    NUIControl::EControlState childState = spTabControlVec[activeTabCtrl].GetLastState();

    if( parentState > childState )
        return parentState;

    return childState;

}	// GetLastState


/************************************************************************
*    desc:  Update the button
************************************************************************/
void CUITabControl::Update()
{
    // Call the parent
    CParent::Update();

    // Update all controls
    spTabControlVec[activeTabCtrl].Update();

    if( decArrowCtrl != NULL )
        decArrowCtrl->Update();

    if( incArrowCtrl != NULL )
        incArrowCtrl->Update();
    
}	// Update


/************************************************************************
*    desc:  Check to see if the control is animating
*           NOTE: Only check the sub controls for selection animation
************************************************************************/
bool CUITabControl::IsAnimating()
{
    bool animating = CParent::IsAnimating();

    if( !animating )
        animating = spTabControlVec[activeTabCtrl].IsAnimating();

    return animating;

}	// IsAnimating


/************************************************************************
*    desc:  Act in accordance to user input
************************************************************************/
void CUITabControl::HandleUserNavigation( NUIMenu::EMenuNavigationState navState,
                                          const string & nextAction,
                                          const string & previousAction )
{
    CParent::HandleUserNavigation( navState, nextAction, previousAction );

    GetSubControl(activeTabCtrl)->SetState( NUIControl::ECS_ACTIVE );

    spTabControlVec[activeTabCtrl].HandleUserNavigation( navState, nextAction, previousAction );

    if( !spTabControlVec[activeTabCtrl].IsStateChangeFinal() )
        ForceStateUpdate( navState );

    // Reset the flag
    keyboardGamePadTabSelection = false;

}	// HandleUserNavigation


/************************************************************************
*    desc:  handle user select
*
*	parm: navState - navigation state
************************************************************************/
bool CUITabControl::HandleUserSelect( NUIMenu::EMenuNavigationState navState )
{
    bool result(false);

    // Need to allow base control selection for the keyboard
    if( navState == NUIMenu::EMNS_GAMEPAD_KEYBAORD )
        result = CUIControl::HandleUserSelect( navState );
    else
    {
        result = CParent::HandleUserSelect( navState );

        // If the user clicked on a tab button, deactivate all the controls
        if( result )
            spTabControlVec[GetActiveSubControlIndex()].DeactivateControls(false);
    }

    if( !result )
        result = spTabControlVec[activeTabCtrl].HandleUserSelect( navState );

    return result;

}	// HandleUserSelect


/************************************************************************
*    desc:  Test for user selection
*
*	parm: navState - navigation state
************************************************************************/
bool CUITabControl::CheckForUserSelection( NUIMenu::EMenuNavigationState navState )
{
    // Respond to controls that that use left\right for selection
    if( (navState == NUIMenu::EMNS_GAMEPAD_KEYBAORD) &&
        WasDecIncSelect( CGameController::Instance().WasAction( decSelectAction, NDevice::EAP_DOWN ),
                         CGameController::Instance().WasAction( incSelectAction, NDevice::EAP_DOWN ) ) )
    {
        keyboardGamePadTabSelection = true;
        return true;
    }

    return false;

}	// CheckForUserSelection


/************************************************************************
*    desc:  Was there a Dec/Inc select
************************************************************************/
bool CUITabControl::WasDecIncSelect( bool decAction, bool incAction )
{
    if( decAction || incAction )
    {
        if( decAction )
        {
            --activeTabCtrl;

            if( activeTabCtrl < 0 )
                activeTabCtrl = static_cast<int>(spTabControlVec.size() - 1);

            if( decArrowCtrl != NULL )
                decArrowCtrl->SetState( NUIControl::ECS_SELECTED );
        }
        else if( incAction )
        {
            activeTabCtrl = (activeTabCtrl + 1) % spTabControlVec.size();

            if( incArrowCtrl != NULL )
                incArrowCtrl->SetState( NUIControl::ECS_SELECTED );
        }
        
        SetActiveSubControlIndex( activeTabCtrl );
        SetToFirstInactiveControl();

        GetSubControl(activeTabCtrl)->PlaySelectSound();

        return true;
    }

    return false;

}	// WasDecIncSelect


/************************************************************************
*    desc:  Designed to be over written for controls that hold other controls
************************************************************************/
void CUITabControl::SetToFirstInactiveControl()
{
    spTabControlVec[activeTabCtrl].SetToFirstInactiveControl();

}	// SetToFirstInactiveControl


/************************************************************************
*    desc:  Execute the action
************************************************************************/
void CUITabControl::ExecuteAction()
{
    CParent::ExecuteAction();

    const int subActiveIndex = GetActiveSubControlIndex();

    if( (subActiveIndex != NUIControl::NO_ACTIVE_CONTROL) && (subActiveIndex != activeTabCtrl) )
        activeTabCtrl = subActiveIndex;

    if( decArrowCtrl != NULL )
        decArrowCtrl->SetState( NUIControl::ECS_INACTIVE );

    if( incArrowCtrl != NULL )
        incArrowCtrl->SetState( NUIControl::ECS_INACTIVE );

}	// ExecuteAction


/************************************************************************
*    desc:  Render the button
*
*    param: CMatrix & _matrix - passed in matrix class
************************************************************************/
void CUITabControl::Render()
{
    // Call the parent
    CParent::Render();

    spTabControlVec[activeTabCtrl].Render();

    if( decArrowCtrl != NULL )
        decArrowCtrl->Render();

    if( incArrowCtrl != NULL )
        incArrowCtrl->Render();

}	// Render


/************************************************************************
*    desc:  Set the transition state of this control
*  
*    param: ETransitionState value - transition state
************************************************************************/
void CUITabControl::SetTransitionState( NUIControl::ETransitionState value )
{
    // Call the parent
    CParent::SetTransitionState( value );

    spTabControlVec[activeTabCtrl].SetTransitionState( value );

    if( decArrowCtrl != NULL )
        decArrowCtrl->SetTransitionState( value );

    if( incArrowCtrl != NULL )
        incArrowCtrl->SetTransitionState( value );

}	// SetTransitionState


/************************************************************************
*    desc:  Reset the transition states 
************************************************************************/
void CUITabControl::ResetTransitionStates()
{
    CParent::ResetTransitionStates();

    spTabControlVec[activeTabCtrl].ResetTransitionStates();

    if( decArrowCtrl != NULL )
        decArrowCtrl->ResetTransitionStates();

    if( incArrowCtrl != NULL )
        incArrowCtrl->ResetTransitionStates();

}	// ResetTransitionStates


/************************************************************************
*    desc:  Force a state update
************************************************************************/
void CUITabControl::ForceStateUpdate( NUIMenu::EMenuNavigationState navState )
{
    CParent::ForceStateUpdate( navState );

    GetSubControl(activeTabCtrl)->ForceStateUpdate( navState );

}	// ForceStateUpdate


/************************************************************************
*    desc:  Get the control's action type
*  
*    ret: EControlActionType - control action type
************************************************************************/
NUIControl::EControlActionType CUITabControl::GetActionType()
{
    NUIControl::EControlActionType result = CParent::GetActionType();

    if( !keyboardGamePadTabSelection && (activeTabCtrl != NUIControl::NO_ACTIVE_CONTROL) && 
        (spTabControlVec[activeTabCtrl].GetActiveSubControlIndex() != NUIControl::NO_ACTIVE_CONTROL) )
    {
        result = spTabControlVec[activeTabCtrl].GetActionType();
    }

    return result;

}	// GetActionType


/************************************************************************
*    desc:  Respond to user select
************************************************************************/
void CUITabControl::RespondToUserSelect()
{
    CParent::RespondToUserSelect();

    if( activeTabCtrl != NUIControl::NO_ACTIVE_CONTROL )
        spTabControlVec[activeTabCtrl].RespondToUserSelect();

}	// RespondToUserSelect
