/************************************************************************
*    FILE NAME:       uiscrollbox.cpp
*
*    DESCRIPTION:     Class for user interface scroll boxes
************************************************************************/

// Physical component dependency
#include <gui/uiscrollbox.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <gui/controlevent.h>
#include <gui/uicontrolfactory.h>
#include <gui/uislider.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/genfunc.h>
#include <controller/gamecontroller.h>
#include <xact/xact.h>
#include <2d/objectdatalist2d.h>
#include <2d/objectdata2d.h>
#include <system/xdevice.h>

// Required namespace(s)
using namespace std;

typedef CUISubControl CParent;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CUIScrollBox::CUIScrollBox()
            : cullHeight(0),
              controlHeight(0),
              scrollCurPos(0),
              maxMoveAmount(0),
              scrollSpeed(0.05),
              pageSpeed(0.05),
              visibleCount(0),
              resetFromOnToMenu(true),
              visStartPos(0),
              visEndPos(0),
              scrollUp(false),
              scrollDown(false),
              paging(false),
              scrollCounter(0),
              scrollDistance(0),
              activeScrollCtrl(NUIControl::NO_ACTIVE_CONTROL),
              lastActiveScrollCtrl(NUIControl::NO_ACTIVE_CONTROL),
              firstScrollCtrlIndex(0)
{
    SetType( NUIControl::ECT_SCROLL_BOX );

    downWaitTimer.Set(200);
    downWaitTimer.Disable();
    holdWaitTimer.Set(400);
    holdWaitTimer.Disable();

}   // Constructer


/************************************************************************
*    desc:  Load the control specific info from XML node
*  
*    param: const XMLNode & controlNode - XML node
************************************************************************/
void CUIScrollBox::LoadControlFromNode( const XMLNode & controlNode )
{
    CParent::LoadControlFromNode( controlNode );

    // Get the sub-control settings
    const XMLNode navigationNode = controlNode.getChildNode( "navigation" );
    if( !navigationNode.isEmpty() )
    {
        if( navigationNode.isAttributeSet( "pageUp" ) )
            pageUpAction = navigationNode.getAttribute( "pageUp" );

        if( navigationNode.isAttributeSet( "pageDown" ) )
            pageDownAction = navigationNode.getAttribute( "pageDown" );
    }

    // Get the scroll box settings node
    const XMLNode settingsNode = controlNode.getChildNode( "scrollBoxSettings" );
    if( !settingsNode.isEmpty() )
    {
        if( settingsNode.isAttributeSet( "downWaitTimerDelay" ) )
        {
            downWaitTimer.Set(atof( settingsNode.getAttribute( "downWaitTimerDelay" ) ));
            downWaitTimer.Disable();
        }

        if( settingsNode.isAttributeSet( "holdWaitTimerDelay" ) )
        {
            holdWaitTimer.Set(atof( settingsNode.getAttribute( "holdWaitTimerDelay" ) ));
            holdWaitTimer.Disable();
        }

        if( settingsNode.isAttributeSet( "scrollSpeed" ) )
            scrollSpeed = atof( settingsNode.getAttribute( "scrollSpeed" ) );

        if( settingsNode.isAttributeSet( "pageSpeed" ) )
            pageSpeed = atof( settingsNode.getAttribute( "pageSpeed" ) );

        // See if the scroll box should reset it's display upon entering OnToMenu
        if( settingsNode.isAttributeSet( "restFromOnToMenu" ) )
            resetFromOnToMenu = (string( settingsNode.getAttribute( "restFromOnToMenu" ) ) == "true");
    }

    // Get the menu controls node
    const XMLNode menuControlsNode = controlNode.getChildNode( "scrollBoxControlList" );
    if( !menuControlsNode.isEmpty() )
    {
        // Get the initial offset of the first control in the scroll box
        initialOffset = NParseHelper::LoadPosition( menuControlsNode );

        // Get the scroll boc info node
        const XMLNode controlInfoNode = menuControlsNode.getChildNode( "controlInfo" );
        controlHeight = atoi( controlInfoNode.getAttribute( "height" ) );
        visibleCount = atoi( controlInfoNode.getAttribute( "visibleInScrollBox" ) );

        // Get the number of controls in this scroll box
        const int scrollControlCount = menuControlsNode.nChildNode( "control" );

        for( int i = 0; i < scrollControlCount; ++i )
        {
            XMLNode controlNode = menuControlsNode.getChildNode( "control", i );

            // Add the scroll control from node
            AddScrollControlFromNode( controlNode );
        }
    }

    // Get the stencil mask node
    const XMLNode stencilMaskNode = controlNode.getChildNode( "stencilMask" );
    if( !stencilMaskNode.isEmpty() )
    {
        const string objType = stencilMaskNode.getAttribute( "type" );

        // Check for NULL and assert if so
        CObjectData2D * pObjectData2D = CObjectDataList2D::Instance().GetData( GetObjGroup(), objType );

        // Get the cull height
        cullHeight = (pObjectData2D->GetVisualData().GetSize().h + controlHeight) / 2;

        // Initialize the sprite
        stencilMaskSprite.Init( pObjectData2D->GetGroup(), pObjectData2D->GetName() );

        // Load the transform data
        stencilMaskSprite.SetTransData( NParseHelper::LoadTransformData( stencilMaskNode ) );
    }

}	// LoadControlFromNode


/************************************************************************
*    desc:  Add the scroll control from node
*  
*    param: const XMLNode & controlNode - XML node
************************************************************************/
CUIControl * CUIScrollBox::AddScrollControlFromNode( const XMLNode & controlNode )
{
    // The pointer is placed within a vector for all controls
    spScrollControlVec.push_back( NUIControlFactory::Create( controlNode ) );

    // Set the position
    spScrollControlVec.back().SetPos( initialOffset - CPoint(0, controlHeight * (spScrollControlVec.size()-1), 0) );
    spScrollControlVec.back().SetDefaultPos( spScrollControlVec.back().GetPos() );

    // Calculate the maximum scroll amount in pixels
    if( spScrollControlVec.size() > static_cast<uint>(visibleCount) )
        maxMoveAmount = (static_cast<int>(spScrollControlVec.size()) - visibleCount) * controlHeight;

    // Init the slider
    NGenFunc::DynCast<CUISlider *>(GetSubControl(SCROLL_BAR))->SetMaxValue(maxMoveAmount);
    NGenFunc::DynCast<CUISlider *>(GetSubControl(SCROLL_BAR))->SetSlider();

    // Calc the start and end positions of what should
    // be viewable in the scroll box
    SetStartEndPos();

    return &spScrollControlVec.back();

}	// AddControlFromNode


/************************************************************************
*    desc:  Remove all scroll controls
************************************************************************/
void CUIScrollBox::RemoveAllScrollControls()
{
    // Free all the scroll controls
    spScrollControlVec.clear();

    maxMoveAmount = 0;
    activeScrollCtrl = NUIControl::NO_ACTIVE_CONTROL;
    lastActiveScrollCtrl = NUIControl::NO_ACTIVE_CONTROL;

    // reset the slider
    NGenFunc::DynCast<CUISlider *>(GetSubControl(SCROLL_BAR))->SetMaxValue(maxMoveAmount);
    NGenFunc::DynCast<CUISlider *>(GetSubControl(SCROLL_BAR))->SetSlider();

    // Calc the start and end positions of what should
    // be viewable in the scroll box
    SetStartEndPos();

}	// RemoveAllScrollControls


/************************************************************************
*    desc:  Remove scroll control by index
*  
*    param: uint index - index of the control to remove
************************************************************************/
void CUIScrollBox::RemoveScrollControl( uint index )
{
    // Free all the scroll controls
    if( index < spScrollControlVec.size() )
    {
        spScrollControlVec.erase( spScrollControlVec.begin() + index );

        maxMoveAmount = 0;
        activeScrollCtrl = NUIControl::NO_ACTIVE_CONTROL;
        lastActiveScrollCtrl = NUIControl::NO_ACTIVE_CONTROL;

        // Calculate the maximum scroll amount in pixels
        if( spScrollControlVec.size() > static_cast<uint>(visibleCount) )
            maxMoveAmount = (static_cast<int>(spScrollControlVec.size()) - visibleCount) * controlHeight;

        // reset the slider
        NGenFunc::DynCast<CUISlider *>(GetSubControl(SCROLL_BAR))->SetMaxValue(maxMoveAmount);
        NGenFunc::DynCast<CUISlider *>(GetSubControl(SCROLL_BAR))->SetSlider();

        // Calc the start and end positions of what should
        // be viewable in the scroll box
        SetStartEndPos();
    }

}	// RemoveScrollControl


/************************************************************************
*    desc:  Set the state of this control
*  
*    param: EControlState value - control state
************************************************************************/
void CUIScrollBox::SetState( NUIControl::EControlState value )
{
    // Call the parent
    CParent::SetState( value );

    if( (activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL) &&
        (spScrollControlVec[activeScrollCtrl].GetState() != NUIControl::ECS_DISABLED) )
    {
        spScrollControlVec[activeScrollCtrl].SetState( value );
    }

}	// SetState


/************************************************************************
*    desc:  Get the state of this control
*  
*    ret:	EControlState - control state
************************************************************************/
NUIControl::EControlState CUIScrollBox::GetState()
{
    NUIControl::EControlState result = CParent::GetState();

    if( (activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL) &&
        (spScrollControlVec[activeScrollCtrl].GetState() != NUIControl::ECS_DISABLED) )
    {
        result = spScrollControlVec[activeScrollCtrl].GetState();
    }

    return result;

}	// GetState


/************************************************************************
*    desc:  Get the last state of this control
*  
*    ret:	EControlState - control state
************************************************************************/
NUIControl::EControlState CUIScrollBox::GetLastState()
{
    NUIControl::EControlState result = CParent::GetLastState();

    if( activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL )
        result = spScrollControlVec[activeScrollCtrl].GetLastState();

    return result;

}	// GetLastState


/************************************************************************
*    desc:  Get the control's action type
*  
*    ret:	EControlActionType - control action type
************************************************************************/
NUIControl::EControlActionType CUIScrollBox::GetActionType()
{
    NUIControl::EControlActionType result = CParent::GetActionType();

    if( activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL )
        result = spScrollControlVec[activeScrollCtrl].GetActionType();

    return result;

}	// GetActionType


/************************************************************************
*    desc:  Get the execution action
*  
*    ret:	const string & - name of action
************************************************************************/
const string & CUIScrollBox::GetExecutionAction()
{
    if( activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL )
        return spScrollControlVec[activeScrollCtrl].GetExecutionAction();
    else
        return CParent::GetExecutionAction();

}	// GetExecutionAction


/************************************************************************
*    desc:  Do any smart executes on the items in the list
************************************************************************/
void CUIScrollBox::ExecuteAction()
{
    if( (activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL) && 
        (spScrollControlVec[activeScrollCtrl].GetState() == NUIControl::ECS_SELECTED) )
    {
        spScrollControlVec[activeScrollCtrl].ExecuteAction();
    }

    // Call the parent
    CParent::ExecuteAction();

}	// ExecuteAction


/************************************************************************
*    desc:  Handle the scroll box navigation in accordance to user input
*
*	 param: EMenuNavigationState navState - what device we are using
*			const string & nextAction     - action to move to the next
*											item in the list
*			const string & previousAction - action to move to the previous
*											item in the list
************************************************************************/
void CUIScrollBox::HandleUserNavigation( NUIMenu::EMenuNavigationState navState,
                                         const string & nextAction,
                                         const string & previousAction )
{
    // Only allow if there are more controls then the scroll box can show at once
    if( spScrollControlVec.size() > static_cast<uint>(visibleCount) )
        CParent::HandleUserNavigation( navState, nextAction, previousAction );

    // Update if the control is active
    if( (GetState() == NUIControl::ECS_ACTIVE) && IsVisible() && !(scrollUp || scrollDown) )
    {
        // If there was mouse movement, we reset the active control to no active control
        if( (navState == NUIMenu::EMNS_MOUSE) && IsPointInSprite() )
            activeScrollCtrl = NUIControl::NO_ACTIVE_CONTROL;

        // Handle keyboard/Gamepad related scrolling
        if( navState == NUIMenu::EMNS_GAMEPAD_KEYBAORD )
        {
            if( !WasPageUpDown( CGameController::Instance().WasAction( pageUpAction, NDevice::EAP_DOWN ),
                                CGameController::Instance().WasAction( pageDownAction, NDevice::EAP_DOWN ) ) )
            {
                HandleKeyboardGamepadScroll( CGameController::Instance().WasAction( previousAction ),
                                             CGameController::Instance().WasAction( nextAction ) );
            }
        }
        // Handle mouse related scrolling
        else if( (navState == NUIMenu::EMNS_MOUSE) && IsPointInSprite() )
        {
            HandleMouseScroll();
        }

        // Set the control to active
        if( activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL )
        {
            if( spScrollControlVec[activeScrollCtrl].IsSelectable( navState ) )
            {
                spScrollControlVec[activeScrollCtrl].SetState( NUIControl::ECS_ACTIVE );

                // Resync the base control animation with the sub control
                if( !spScrollControlVec[activeScrollCtrl].IsStateChangeFinal() )
                    ForceStateUpdate( navState );
            }
            else
                activeScrollCtrl = NUIControl::NO_ACTIVE_CONTROL;
        }
    }

}	// HandleUserNavigation


/************************************************************************
*    desc:  Handle mouse wheel scrolling
************************************************************************/
void CUIScrollBox::HandleMouseWheelScroll()
{
    // Only allow if there are more controls then the scroll box can show at once
    if( spScrollControlVec.size() > static_cast<uint>(visibleCount) )
    {
        // Correct the scroll box after dragging with the mouse and switching to keyboard
        CorrectScrollPos();

        if( CGameController::Instance().GetMovement().mouseZ < 0 )
            scrollCurPos += controlHeight;
        else
            scrollCurPos -= controlHeight;

        // Do bounds checking
        if( scrollCurPos < 0 )
            scrollCurPos = 0;

        else if( scrollCurPos > maxMoveAmount )
            scrollCurPos = maxMoveAmount;

        // Reposition the scroll bar
        NGenFunc::DynCast<CUISlider *>(GetSubControl(SCROLL_BAR))->SetSlider(scrollCurPos);

        // Calc the start and end positions of what should
        // be viewable in the scroll box
        SetStartEndPos();

        // Set the scroll box control positions
        SetScrollCtrlPos();

        // Reset the first scroll index
        firstScrollCtrlIndex = scrollCurPos / controlHeight;
    }

}	// HandleMouseWheelScroll


/************************************************************************
*    desc:  Handle keyboard/Gamepad scrolling
*
*	 param:	EActionPress prevActionPress - if the previous action key was
*										   pressed, held, or neither
*			EActionPress nextActionPress - if the next action key was
*										   pressed, held, or neither
************************************************************************/
void CUIScrollBox::HandleKeyboardGamepadScroll( NDevice::EActionPress prevActionPress,
                                                NDevice::EActionPress nextActionPress )
{
    // Init the timers that control scrolling
    InitStartTimer( (nextActionPress == NDevice::EAP_DOWN) || (prevActionPress == NDevice::EAP_DOWN) );
    InitHoldTimer( ((nextActionPress == NDevice::EAP_HOLD) || (prevActionPress == NDevice::EAP_HOLD)) && holdWaitTimer.Expired() );

    // Correct the scroll box after dragging with the mouse and switching to keyboard
    CorrectScrollPos();

    // Check and see if we should move the focus to the next control
    // The downWaitTimer is for delaying the move to the next item in the scroll box without any scrolling
    if( (nextActionPress == NDevice::EAP_DOWN) || ((nextActionPress == NDevice::EAP_HOLD) && downWaitTimer.Expired(CTimer::RESTART_ON_EXPIRE)) )
        InitKeyboardGamepadScroll( 1 );

    else if( (prevActionPress == NDevice::EAP_DOWN) || ((prevActionPress == NDevice::EAP_HOLD) && downWaitTimer.Expired(CTimer::RESTART_ON_EXPIRE)))
        InitKeyboardGamepadScroll( -1 );

}	// HandleKeyboardGamepadScroll


/************************************************************************
*    desc:  Init the keyboard/Gamepad scroll
*
*	 param: int scrollVector - direction to scroll
************************************************************************/
void CUIScrollBox::InitKeyboardGamepadScroll( int scrollVector )
{
    bool controlInc(false);
    const bool SCROLL_DOWN((scrollVector > 0));
    const bool SCROLL_UP((scrollVector < 0));

    // Use the last active control as the starting point if no active control is available
    if( activeScrollCtrl == NUIControl::NO_ACTIVE_CONTROL )
        activeScrollCtrl = lastActiveScrollCtrl;

    // Set a temp variable to the active scroll control
    int tmpScrollCtrl = activeScrollCtrl;

    // If the active control is not within the active area, make it so that it will be the first one selected
    if( (tmpScrollCtrl < firstScrollCtrlIndex) || (tmpScrollCtrl > (firstScrollCtrlIndex + visibleCount)) )
        tmpScrollCtrl = firstScrollCtrlIndex + (-scrollVector);

    if( (SCROLL_UP && (tmpScrollCtrl > 0)) || 
        (SCROLL_DOWN && (tmpScrollCtrl < static_cast<int>(spScrollControlVec.size()-1))) )
    {
        // This loop doesn't go through all the controls. It only uses the
        // full range to find the next selectable control
        for( size_t i = 0; i < spScrollControlVec.size(); ++i )
        {
            tmpScrollCtrl += scrollVector;
                
            // Do not loop the items in the scroll box
            if( tmpScrollCtrl < 0 )
                tmpScrollCtrl = 0;
            else if( tmpScrollCtrl >= static_cast<int>(spScrollControlVec.size()) )
                tmpScrollCtrl = static_cast<int>(spScrollControlVec.size()-1);
            else
                controlInc = true;

            if( controlInc && ((tmpScrollCtrl < firstScrollCtrlIndex) || (tmpScrollCtrl >= (firstScrollCtrlIndex + visibleCount))) )
            {
                if( scrollVector > 0 )
                    scrollDown = true;
                else
                    scrollUp = true;

                scrollCounter = 0;
                scrollDistance += controlHeight;
            }

            // Once we find a button that's not disabled we've found the new active control
            if( spScrollControlVec[tmpScrollCtrl].IsSelectable( NUIMenu::EMNS_GAMEPAD_KEYBAORD ) ||
                (SCROLL_UP && (tmpScrollCtrl == 0)) ||
                (SCROLL_DOWN && (tmpScrollCtrl == spScrollControlVec.size()-1)) )
            {
                // Because each control's state is reset at the top of the function, we must use the
                // control's last state to determine if we want to play the highlight sound
                if( spScrollControlVec[tmpScrollCtrl].GetLastState() == NUIControl::ECS_INACTIVE )
                {
                    spScrollControlVec[tmpScrollCtrl].PlayActiveSound();

                    // Resync the internal active control's animation with the base control
                    ForceStateUpdate();
                }

                break;
            }
        }

        // Only reset the active scroll control if it is selectable
        if( spScrollControlVec[tmpScrollCtrl].IsSelectable( NUIMenu::EMNS_GAMEPAD_KEYBAORD ) )
            activeScrollCtrl = tmpScrollCtrl;
    }

}	// InitKeyboardGamepadScroll


/************************************************************************
*    desc:  Was there a Page Up or Down
*
*	 param:	bool pageUp   - if there was a page up
*			bool pageDown - if there was a page down
************************************************************************/
bool CUIScrollBox::WasPageUpDown( bool pageUp, bool pageDown )
{
    // Only allow if there are more controls then the scroll box can show at once
    if( spScrollControlVec.size() > static_cast<uint>(visibleCount) )
    {
        if( pageUp || pageDown )
        {
            int tmpfirstScrollCtrlIndex = firstScrollCtrlIndex;

            int scrollVector = visibleCount;

            if( pageUp )
                scrollVector = -visibleCount;

            firstScrollCtrlIndex += scrollVector;

            // Do bounds checking
            BoundsCheckFirstScrollCtrl();

            lastActiveScrollCtrl = activeScrollCtrl;

            // Move the active control
            if( activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL )
                activeScrollCtrl += scrollVector;
            else
            {
                activeScrollCtrl = firstScrollCtrlIndex;

                if( pageUp )
                    activeScrollCtrl += visibleCount-1;
            }

            if( activeScrollCtrl < 0 )
                activeScrollCtrl = 0;

            else if( activeScrollCtrl >= static_cast<int>(spScrollControlVec.size()))
                activeScrollCtrl = static_cast<int>(spScrollControlVec.size()) - 1;

            // Be sure to find a selectable control
            for( int i = activeScrollCtrl; i < static_cast<int>(spScrollControlVec.size()); ++i )
            {
                if( spScrollControlVec[i].IsSelectable( NUIMenu::EMNS_GAMEPAD_KEYBAORD ) )
                {
                    activeScrollCtrl = i;
                    break;
                }
            }

            // Page scroll the contents of the scroll box
            if( tmpfirstScrollCtrlIndex != firstScrollCtrlIndex )
            {
                // Calc the scroll distance
                scrollDistance = abs(firstScrollCtrlIndex - tmpfirstScrollCtrlIndex) * controlHeight;

                scrollUp = pageUp;
                scrollDown = pageDown;
                paging = true;

                // Don't remember the last control when it has been scrolled out of view
                lastActiveScrollCtrl = NUIControl::NO_ACTIVE_CONTROL;

                return true;
            }
            // No scrolling nessary but active control has been reset
            else if( lastActiveScrollCtrl != activeScrollCtrl )
            {
                // Play the active sound
                spScrollControlVec[activeScrollCtrl].PlayActiveSound();

                // Resync the internal active control's animation with the base control
                ForceStateUpdate();
            }
        }
    }

    return false;

}	// WasPageUpDown


/************************************************************************
*    desc:  Handle mouse scrolling
************************************************************************/
void CUIScrollBox::HandleMouseScroll()
{
    // Handle mouse wheel scrolling
    if( CGameController::Instance().GetMovement().mouseZ != 0 )
    {
        HandleMouseWheelScroll();
    }

    // See which of the sub-controls are active
    else if( GetActiveSubControlIndex() == INC_SCROLL )
    {
        NDevice::EActionPress actionPress = GetSubControl(INC_SCROLL)->GetUserMouseActionPressState( NUIMenu::EMNS_MOUSE );

        // Init the timers that control scrolling
        InitStartTimer( actionPress == NDevice::EAP_DOWN );
        InitHoldTimer( (actionPress == NDevice::EAP_HOLD) && holdWaitTimer.Expired() );

        if( (actionPress == NDevice::EAP_DOWN ) || ((actionPress == NDevice::EAP_HOLD ) && downWaitTimer.Expired()) )
            ScrollButtonIncDec( 1 );
    }
    else if( GetActiveSubControlIndex() == DEC_SCROLL )
    {
        NDevice::EActionPress actionPress = GetSubControl(DEC_SCROLL)->GetUserMouseActionPressState( NUIMenu::EMNS_MOUSE );

        // Init the timers that control scrolling
        InitStartTimer( actionPress == NDevice::EAP_DOWN );
        InitHoldTimer( (actionPress == NDevice::EAP_HOLD) && holdWaitTimer.Expired() );

        if( (actionPress == NDevice::EAP_DOWN ) || ((actionPress == NDevice::EAP_HOLD ) && downWaitTimer.Expired()) )
            ScrollButtonIncDec( -1 );
    }
    else if( GetActiveSubControlIndex() == SCROLL_BAR )
    {
        NDevice::EActionPress actionPress = GetSubControl(SCROLL_BAR)->GetUserMouseActionPressState( NUIMenu::EMNS_MOUSE );

        if( actionPress > NDevice::EAP_IDLE )
        {
            scrollCurPos = NGenFunc::DynCast<CUISlider *>(GetSubControl(SCROLL_BAR))->GetValue();

            // Calc the start and end positions of what should
            // be viewable in the scroll box
            SetStartEndPos();

            // Set the scroll box control positions
            SetScrollCtrlPos();

            // Reset the first scroll index
            firstScrollCtrlIndex = scrollCurPos / controlHeight;
        }
    }
    else
    {
        // Add in one so that controls peaking through the top and bottom can be selected
        // And then do some bounds checking
        int maxVisibleRange = firstScrollCtrlIndex + visibleCount + 1;
        if( maxVisibleRange > static_cast<int>(spScrollControlVec.size()) )
            maxVisibleRange = static_cast<int>(spScrollControlVec.size());

        // Change the state of the button if the mouse point is within the rect
        // Only check the possible buttons visible in the scroll box
        for( int i = firstScrollCtrlIndex; i < maxVisibleRange; ++i )
        {
            // Check and make sure the control isn't disabled or a label and visible, we've found the new active control
            // Check if the mouse pointer is within this control
            if( spScrollControlVec[i].IsSelectable( NUIMenu::EMNS_MOUSE ) && spScrollControlVec[i].IsPointInSprite() )
            {
                // Because each control's state is reset at the top of the function, we must use the
                // control's last state to determine if we want to play the highlight sound
                if( spScrollControlVec[i].GetState() == NUIControl::ECS_INACTIVE )
                {
                    spScrollControlVec[i].PlayActiveSound();

                    // Resync the internal active control's animation with the base control
                    ForceStateUpdate();
                }

                // Set the index of this control to the active control
                lastActiveScrollCtrl = activeScrollCtrl = i;

                break;
            }
        }
    }
}	// HandleMouseScroll


/************************************************************************
*    desc:  Init the start timers
*
*	 param: bool initTimers - value
************************************************************************/
void CUIScrollBox::InitStartTimer( bool initTimers )
{
    if( initTimers )
    {
        holdWaitTimer.Reset();
        downWaitTimer.Disable();
    }

}	// InitStartTimer


/************************************************************************
*    desc:  Init the start timers
*
*	 param: bool initTimers - value
************************************************************************/
void CUIScrollBox::InitHoldTimer( bool initTimers )
{
    if( initTimers )
    {
        holdWaitTimer.Disable();
        downWaitTimer.Reset();
    }

}	// InitHoldTimer


/************************************************************************
*    desc:  Handle user select
*
*	 param: EMenuNavigationState navState - navigation state
*
*	 ret:	bool - if the user has made a selection
************************************************************************/
bool CUIScrollBox::HandleUserSelect( NUIMenu::EMenuNavigationState navState )
{
    bool result = CParent::HandleUserSelect( navState );

    if( !result && (activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL) )
        result = spScrollControlVec[activeScrollCtrl].HandleUserSelect( navState );
        
    return result;

}	// HandleUserSelect


/************************************************************************
*    desc:  Transform the control
************************************************************************/
void CUIScrollBox::Transform()
{
    if( GetParameters().IsSet( CObject::TRANSFORM ) )
    {
        CBitmask tmpTransParam = GetParameters();
        CParent::Transform();

        // Tell the children of this control that they need to transform
        for( int i = visStartPos; i < visEndPos; ++i )
            spScrollControlVec[i].SetTransformParameters( tmpTransParam, true );
    }

    // Transform the parent class's components
    CParent::Transform();

    // Transform the stencil mask
    stencilMaskSprite.Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

    for( int i = visStartPos; i < visEndPos; ++i )
        spScrollControlVec[i].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

}	// Transform


/************************************************************************
*    desc:  Transform the control
*
*	 param: const CMatrix & _scaledMatrix   - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point       - passed in relative position
************************************************************************/
void CUIScrollBox::Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                              const CWorldPoint & point )
{
    if( GetParameters().IsSet( CObject::TRANSFORM ) )
    {
        CBitmask tmpTransParam = GetParameters();
        CParent::Transform( _scaledMatrix, _unscaledMatrix, point );

        // Tell the children of this control that they need to transform
        for( int i = visStartPos; i < visEndPos; ++i )
            spScrollControlVec[i].SetTransformParameters( tmpTransParam, true );
    }

    // Transform the parent class's components
    CParent::Transform( _scaledMatrix, _unscaledMatrix, point );

    // Transform the stencil mask
    stencilMaskSprite.Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

    // Transform the control's visual components
    for( int i = visStartPos; i < visEndPos; ++i )
        spScrollControlVec[i].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );
    
}	// Transform


/************************************************************************
*    desc:  Set the transition state of this control
*  
*    param: ETransitionState value - transition state
************************************************************************/
void CUIScrollBox::SetTransitionState( NUIControl::ETransitionState value )
{
    // Call the parent
    CParent::SetTransitionState( value );

    for( int i = visStartPos; i < visEndPos; ++i )
        spScrollControlVec[i].SetTransitionState( value );

}	// SetTransitionState


/************************************************************************
*    desc:  Correct Scroll position
************************************************************************/
void CUIScrollBox::CorrectScrollPos()
{
    if( ((scrollCurPos - (int)scrollCurPos) > 0.0f) || (((int)scrollCurPos % controlHeight) != 0) )
    {
        // Recalucate the scroll position which will wipe the fractional component
        scrollCurPos = firstScrollCtrlIndex * controlHeight;

        NGenFunc::DynCast<CUISlider *>(GetSubControl(SCROLL_BAR))->SetSlider(scrollCurPos);

        // Calc the start and end positions of what should
        // be viewable in the scroll box
        SetStartEndPos();

        // Set the scroll box control positions
        SetScrollCtrlPos();

        // Force a reset on the active scroll indexes
        activeScrollCtrl = lastActiveScrollCtrl = NUIControl::NO_ACTIVE_CONTROL;
    }

}	// CorrectScrollPos


/************************************************************************
*    desc:  Handle the time based Scroll
************************************************************************/
void CUIScrollBox::HandleScrollingUpdate()
{
    if( scrollUp || scrollDown )
    {
        float dist = CHighResTimer::Instance().GetElapsedTime() * scrollSpeed;

        if( paging )
            dist = CHighResTimer::Instance().GetElapsedTime() * pageSpeed;

        if( scrollDown )
            scrollCurPos += dist;
        else
            scrollCurPos -= dist;

        NGenFunc::DynCast<CUISlider *>(GetSubControl(SCROLL_BAR))->SetSlider(scrollCurPos);

        scrollCounter += dist;

        if( scrollCounter >= scrollDistance )
        {
            // Reset the first scroll index
            firstScrollCtrlIndex = scrollCurPos / controlHeight;

            // Scrolling up due to floating division will be off by one
            if( scrollUp && (scrollCurPos > 0) )
                ++firstScrollCtrlIndex;

            // Do bounds checking
            BoundsCheckFirstScrollCtrl();

            // Recalucate the scroll position which will wipe the fractional component
            scrollCurPos = firstScrollCtrlIndex * controlHeight;
            NGenFunc::DynCast<CUISlider *>(GetSubControl(SCROLL_BAR))->SetSlider(scrollCurPos);

            // Resync the internal active control's animation with the base control  when paging
            if( paging && (activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL) )
            {
                // Play the active sound
                spScrollControlVec[activeScrollCtrl].PlayActiveSound();

                spScrollControlVec[activeScrollCtrl].ForceStateUpdate();
                ForceStateUpdate();
            }

            scrollDown = false;
            scrollUp = false;
            paging = false;
            scrollCounter = 0;
            scrollDistance = 0;
        }

        // Calc the start and end positions of what should
        // be viewable in the scroll box
        SetStartEndPos();

        // Set the scroll box control positions
        SetScrollCtrlPos();
    }

}	// HandleScrollingUpdate


/************************************************************************
*    desc:  Scroll button press inc/dec
************************************************************************/
void CUIScrollBox::ScrollButtonIncDec( int scrollVector )
{
    const bool SCROLL_DOWN((scrollVector > 0));
    const bool SCROLL_UP((scrollVector < 0));

    if( !(scrollUp || scrollDown) && 
        ((SCROLL_DOWN && (scrollCurPos < maxMoveAmount)) ||
         (SCROLL_UP && (scrollCurPos > 0)) ) )
    {
        int adjustment = (int)scrollCurPos % controlHeight;

        if( SCROLL_DOWN )
        {
            scrollDown = true;
            if(adjustment > 0)
                adjustment = controlHeight - adjustment;
        }
        else
            scrollUp = true;

        scrollCounter = 0;
        scrollDistance = controlHeight;

        // Add 1 to resolve any floating point imprecision
        if( adjustment > 0 )
            scrollDistance = adjustment + 1;
    }

}	// ScrollButtonIncDec


/************************************************************************
*    desc:  Set the first control to be active
************************************************************************/
void CUIScrollBox::SetToFirstInactiveControl()
{
    CParent::SetToFirstInactiveControl();

    // reset the scroll box position on entry if desired or if no control is active
    if( resetFromOnToMenu || (activeScrollCtrl == NUIControl::NO_ACTIVE_CONTROL) )
    {
        SetFirstScrollCtrolPos();

        for( size_t i = 0; i < spScrollControlVec.size(); ++i )
        {
            if( !((spScrollControlVec[i].GetState() == NUIControl::ECS_DISABLED) ||
                  (spScrollControlVec[i].GetType() == NUIControl::ECT_LABEL) ||
                  spScrollControlVec[i].IsMouseOnlyCtrl() ||
                  !spScrollControlVec[i].IsVisible() ) )
            {
                activeScrollCtrl = static_cast<int>(i);
                break;
            }
        }
    }

}	// SetToFirstInactiveControl


/************************************************************************
*    desc:  Set the start and end positions
************************************************************************/
void CUIScrollBox::SetStartEndPos()
{
    // The visual positions are 1 less then the first and one more then the last
    // to handle the posibility of 1 extra control peaking through the top or bottom
    int pos = scrollCurPos / controlHeight;

    visStartPos = pos - 1;
    visEndPos = pos + visibleCount + 1;

    if( visStartPos < 0 )
        visStartPos = 0;

    if( visEndPos > static_cast<int>(spScrollControlVec.size()) )
        visEndPos = static_cast<int>(spScrollControlVec.size());

}	// SetFirstScrollCtrlIndex


/************************************************************************
*    desc:  Set the scroll box position via control index
*
*	 param:	uint ctrlIndex - index of the control
************************************************************************/
void CUIScrollBox::SetFirstScrollCtrolPos( uint ctrlIndex )
{
    // Only allow if there are more controls then the scroll box can show at once
    if( spScrollControlVec.size() > static_cast<uint>(visibleCount) )
    {
        // Set the scroll position
        scrollCurPos = ctrlIndex * controlHeight;

        // Do bounds checking
        if( scrollCurPos > maxMoveAmount )
            scrollCurPos = maxMoveAmount;

        // Reposition the scroll bar
        NGenFunc::DynCast<CUISlider *>(GetSubControl(SCROLL_BAR))->SetSlider(scrollCurPos);

        // Calc the start and end positions of what should
        // be viewable in the scroll box
        SetStartEndPos();

        // Set the scroll box control positions
        SetScrollCtrlPos();

        // Reset the first scroll index
        firstScrollCtrlIndex = scrollCurPos / controlHeight;
    }

}	// SetFirstScrollCtrolPos


/************************************************************************
*    desc:  Set the scroll box control positions
************************************************************************/
void CUIScrollBox::SetScrollCtrlPos()
{
    for( int i = visStartPos; i < visEndPos; ++i )
        spScrollControlVec[i].SetPos( spScrollControlVec[i].GetDefaultPos() + CPoint(0, scrollCurPos, 0) );

}	// SetScrollCtrlPos


/************************************************************************
*    desc:  Bounds check the first scroll control index
************************************************************************/
void CUIScrollBox::BoundsCheckFirstScrollCtrl()
{
    // Do bounds checking just for sanity reasons
    if( firstScrollCtrlIndex < 0 )
        firstScrollCtrlIndex = 0;

    else if( (firstScrollCtrlIndex + visibleCount) > static_cast<int>(spScrollControlVec.size()) )
        firstScrollCtrlIndex = static_cast<int>(spScrollControlVec.size() - visibleCount);

}	// BoundsCheckFirstScrollCtrl


/************************************************************************
*    desc:  Get the active control
*  
*    ret:	CUIControl * - active control
************************************************************************/
CUIControl * CUIScrollBox::GetActiveControl()
{
    CUIControl * pControl(NULL);

    if( activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL )
        pControl = &spScrollControlVec[activeScrollCtrl];

    return pControl;

}	// GetActiveControl


/************************************************************************
*    desc:  Get the number of scroll controls
*  
*    ret:	size_t - number of controls
************************************************************************/
size_t CUIScrollBox::GetScrollCtrlCount()
{
    return spScrollControlVec.size();

}	// GetScrollCtrlCount


/************************************************************************
*    desc:  Get the a scroll control
*
*	 param:	const uint index - index of the control
*  
*    ret:	CUIControl * - returned control pointer
************************************************************************/
CUIControl * CUIScrollBox::GetScrollCtrl( const uint index )
{
    if( index < spScrollControlVec.size() )
        return &spScrollControlVec[index];

    return NULL;

}	// GetScrollCtrl


/************************************************************************
*    desc:  Update the button
************************************************************************/
void CUIScrollBox::Update()
{
    // Call the parent
    CParent::Update();

    // Handle the time based Scroll
    HandleScrollingUpdate();

    // Update all controls
    for( int i = visStartPos; i < visEndPos; ++i )
        spScrollControlVec[i].Update();

}	// Update


/************************************************************************
*    desc:  Check to see if the control is animating
*           
*	 NOTE:	Only check the sub controls for selection animation
************************************************************************/
bool CUIScrollBox::IsAnimating()
{
    bool animating = CParent::IsAnimating();

    if( !animating && (activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL) )
        animating = spScrollControlVec[activeScrollCtrl].IsAnimating();

    return animating;

}	// IsAnimating


/************************************************************************
*    desc:  Render the button
************************************************************************/
void CUIScrollBox::Render()
{
    // Call the parent
    CParent::Render();

    if( IsVisible() )
    {
        // Enable rendering to the stencil buffer
        CXDevice::Instance().EnableStencilRender( true );

        // Render to the stencil
        stencilMaskSprite.Render();

        // Get the stencil buffer ready to render through it
        CXDevice::Instance().InitRenderThroughStencil();

        for( int i = visStartPos; i < visEndPos; ++i )
        {
            float height = abs( spScrollControlVec[i].GetPos().y - stencilMaskSprite.GetPos().y );

            if( height < cullHeight )
                spScrollControlVec[i].Render();
        }

        // Disable rendering to the stencil buffer
        CXDevice::Instance().EnableStencilRender( false );
    }

}	// Render


/************************************************************************
*    desc:  Reset the controls to inactive if its not disabled
************************************************************************/
void CUIScrollBox::ResetControls()
{
    // Reset all controls
    for( int i = visStartPos; i < visEndPos; ++i )
        spScrollControlVec[i].Reset();

}	// ResetControls


/************************************************************************
*    desc:  Reset the control scrollbox
************************************************************************/
void CUIScrollBox::Reset()
{
    CParent::Reset();

    // Reset all controls
    ResetControls();

}	// ResetControls


/************************************************************************
*    desc:  Deactivates all controls 
*
*	 param:	bool toMemu - if we're deactivating because of entering the menu
************************************************************************/
void CUIScrollBox::DeactivateControls( bool toMemu )
{
    CParent::DeactivateControls( toMemu );

    if( resetFromOnToMenu && toMemu )
        SetFirstScrollCtrolPos();

    lastActiveScrollCtrl = activeScrollCtrl = NUIControl::NO_ACTIVE_CONTROL;

}	// DeactivateControls


/************************************************************************
*    desc:  Reset the transition states 
************************************************************************/
void CUIScrollBox::ResetTransitionStates()
{
    CParent::ResetTransitionStates();

    for( int i = visStartPos; i < visEndPos; ++i )
        spScrollControlVec[i].ResetTransitionStates();

}	// ResetTransitionStates


