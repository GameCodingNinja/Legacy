/************************************************************************
*    FILE NAME:       uislider.cpp
*
*    DESCRIPTION:     Class for user interface slider
************************************************************************/

// Physical component dependency
#include <gui/uislider.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <controller/gamecontroller.h>
#include <misc/settings.h>

// Required namespace(s)
using namespace std;

typedef CUISubControl CParent;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CUISlider::CUISlider()
         : travelDistPixels(0),
           orientation(EO_HORIZONTAL),
           minValue(0),
           maxValue(0),
           curValue(0),
           displayValueAsInt(false)
{
    moveTimer.Set(200);
    moveTimer.Disable();
    moveWaitTimer.Set(400);
    moveWaitTimer.Disable();

    SetType( NUIControl::ECT_SLIDER );

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CUISlider::~CUISlider()
{
}   // Destructer


/************************************************************************
*    desc:  Load the control info from XML node
*  
*    param: node - XML node
************************************************************************/
void CUISlider::LoadFromNode( const XMLNode & node )
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

    // Get the slider specific settings
    XMLNode settingsNode = node.getChildNode( "settings" );
    if( !settingsNode.isEmpty() )
    {
        if( settingsNode.isAttributeSet( "orientation" ) )
            if( string(settingsNode.getAttribute( "orientation" )) == "VERT" )
                orientation = EO_VERTICAL;

        if( settingsNode.isAttributeSet( "minValue" ) )
            minValue = atof(settingsNode.getAttribute( "minValue" ));

        if( settingsNode.isAttributeSet( "maxValue" ) )
            maxValue = atof(settingsNode.getAttribute( "maxValue" ));

        if( settingsNode.isAttributeSet( "defValue" ) )
            SetSlider( atof(settingsNode.getAttribute( "defValue" )) );
        else
            SetSlider();

        if( settingsNode.isAttributeSet( "displayValueAsInt" ) )
            if( string(settingsNode.getAttribute( "displayValueAsInt" )) == "true" )
                displayValueAsInt = true;
    }

}	// LoadFromNode


/************************************************************************
*    desc:  Load the control specific info from XML node
*  
*    param: node - XML node
************************************************************************/
void CUISlider::LoadControlFromNode( const XMLNode & controlNode )
{
    // Have the parent load it's stuff
    CParent::LoadControlFromNode( controlNode );

    // Get the slider specific settings
    XMLNode settingsNode = controlNode.getChildNode( "settings" );
    if( !settingsNode.isEmpty() )
    {
        if( settingsNode.isAttributeSet( "maxTravelDistPixels" ) )
            travelDistPixels = atof(settingsNode.getAttribute( "maxTravelDistPixels" ));

        if( settingsNode.isAttributeSet( "moveDelay" ) )
        {
            moveTimer.Set( atof(settingsNode.getAttribute( "moveDelay" )) );
            moveTimer.Disable();
        }

        if( settingsNode.isAttributeSet( "moveWaitDelay" ) )
        {
            moveWaitTimer.Set( atof(settingsNode.getAttribute( "moveWaitDelay" )) );
            moveWaitTimer.Disable();
        }
    }

}	// LoadControlFromNode


/************************************************************************
*    desc:  Set the slider min value
*  
*    param: value
************************************************************************/
void CUISlider::SetMinValue( float value )
{
    minValue = value;

}	// SetMinValue


/************************************************************************
*    desc:  Set the slider max value
*  
*    param: value
************************************************************************/
void CUISlider::SetMaxValue( float value )
{
    maxValue = value;

}	// SetMaxValue


/************************************************************************
*    desc:  Set the slider inc value
*  
*    param: float sliderPos
************************************************************************/
void CUISlider::SetSlider( float value )
{
    curValue = value;

    // Update the slider
    UpdateSlider();

}	// SetSlider


/************************************************************************
*    desc:  Set the slider inc value
*  
*    param: float slider value
************************************************************************/
void CUISlider::IncSlider( float value )
{
    curValue += value;

    // Update the slider
    UpdateSlider();

}	// IncSlider


/************************************************************************
*    desc:  Inc the slider based on mouse movement
*  
*    param: float slider value
************************************************************************/
void CUISlider::IncSliderMovePos( float value )
{
    curValue += value * (maxValue / travelDistPixels);

    // Update the slider
    UpdateSlider();

    // Set the slider control as selected
    GetSubControl()->SetState(NUIControl::ECS_SELECTED);

}	// IncSliderMovePos


/************************************************************************
*    desc:  Update the slider
************************************************************************/
void CUISlider::UpdateSlider()
{
    // Cap current value to it's range
    CapSliderValue();

    // Set the position of the slider
    SetSliderPos();

    // Set the slider label if there is one
    if( !IsFontStringGroupEmpty() )
    {
        // See if we specified a format string
        string formatStr = GetFontString();

        // Format for display
        string valueStr;

        if( displayValueAsInt )
            valueStr = boost::str( boost::format(formatStr) % (int)curValue );
        else
            valueStr = boost::str( boost::format(formatStr) % curValue );

        DisplayFontString(valueStr);
    }

}	// UpdateSlider


/************************************************************************
*    desc:  Cap the slider value
************************************************************************/
void CUISlider::CapSliderValue()
{
    // Cap current value to range
    if( curValue < minValue )
        curValue = minValue;

    else if( curValue > maxValue )
        curValue = maxValue;

}	// CapSliderValue


/************************************************************************
*    desc:  Set the position of the slider
************************************************************************/
void CUISlider::SetSliderPos()
{
    float startPos = -(travelDistPixels / 2);
    float pixelsPerVaue = travelDistPixels / maxValue;
    float pos = startPos + (pixelsPerVaue * curValue);

    if( orientation == EO_HORIZONTAL )
        GetSubControl()->SetPos( GetSubControl()->GetDefaultPos() + CPoint(pos,0,0) );
    else
        GetSubControl()->SetPos( GetSubControl()->GetDefaultPos() + CPoint(0,-pos,0) );

}	// SetSliderPos


/************************************************************************
*    desc:  Get the state of this control
*  
*    ret: EControlState - control state
************************************************************************/
NUIControl::EControlState CUISlider::GetState()
{
    return CUIControl::GetState();

}	// GetState

/************************************************************************
*    desc:  Force a state update
************************************************************************/
void CUISlider::ForceStateUpdate( NUIMenu::EMenuNavigationState navState )
{
    NDevice::EActionPress actionPress = GetSubControl()->GetUserMouseActionPressState( navState );

    if( !((actionPress > NDevice::EAP_IDLE) && (actionPress < NDevice::EAP_UP)) )
        CParent::ForceStateUpdate();

}	// ForceStateUpdate


/************************************************************************
*    desc:  Act in accordance to user input
************************************************************************/
void CUISlider::HandleUserNavigation( NUIMenu::EMenuNavigationState navState,
                                      const string & nextAction,
                                      const string & previousAction )
{
    if( (navState == NUIMenu::EMNS_MOUSE) &&
        (GetActiveSubControlIndex() != NUIControl::NO_ACTIVE_CONTROL) )
    {
        NDevice::EActionPress actionPress = GetSubControl()->GetUserMouseActionPressState( navState );

        if( !((actionPress > NDevice::EAP_IDLE) && (actionPress < NDevice::EAP_UP)) )
            CParent::HandleUserNavigation( navState, nextAction, previousAction );
    }
    else
        CParent::HandleUserNavigation( navState, nextAction, previousAction );

}	// HandleUserNavigation


/************************************************************************
*    desc:  handle user select
*
*	parm: navState - navigation state
************************************************************************/
bool CUISlider::HandleUserSelect( NUIMenu::EMenuNavigationState navState )
{
    bool result(false);

    if( navState == NUIMenu::EMNS_MOUSE )
    {
        NDevice::EActionPress sliderCtrl(NDevice::EAP_IDLE);

        // Get the parent control's press action state
        NDevice::EActionPress parentCtrl = CUIControl::GetUserMouseActionPressState( navState );

        // Records the owner on the down action
        SetActionOwner( parentCtrl );

        if( IsActionOwner() )
        {
            if( GetActiveSubControlIndex() != NUIControl::NO_ACTIVE_CONTROL )
            {
                // Get the slider control's press action state
                sliderCtrl = GetSubControl()->GetUserMouseActionPressState( navState );

                if( sliderCtrl == NDevice::EAP_DOWN )
                {
                    // Set the slider control as selected
                    GetSubControl()->SetState(NUIControl::ECS_SELECTED);
                    PlaySelectSound();
                }
                else if( sliderCtrl == NDevice::EAP_UP )
                {
                    // Force an execute action becuase the slider button 
                    // is managed inside the slide class. This has to be
                    // done on the up state to register correctly on the first movement
                    ExecuteAction();
                }
            }

            if( (sliderCtrl == NDevice::EAP_HOLD) || (parentCtrl == NDevice::EAP_HOLD) )
            {
                CPoint dif = CGameController::Instance().GetAbsolutePosScaled() - GetSubControl()->GetTransPos();//GetSubControl()->GetCenter();

                if( orientation == EO_HORIZONTAL )
                    IncSliderMovePos( dif.GetX() );
                else
                    IncSliderMovePos( -dif.GetY() );
            }

            // Respond to clicking on the slider parent
            if( (GetActiveSubControlIndex() == NUIControl::NO_ACTIVE_CONTROL) && (parentCtrl == NDevice::EAP_DOWN) )
                result = CUIControl::HandleUserSelect( navState );
        }
    }
    else if( (navState == NUIMenu::EMNS_GAMEPAD_KEYBAORD) && !decSelectAction.empty() && !incSelectAction.empty() )
    {
        NDevice::EActionPress decAction = CGameController::Instance().WasAction( decSelectAction );
        NDevice::EActionPress IncAction = CGameController::Instance().WasAction( incSelectAction );

        bool excAction(false);

        // Arm the timer
        if( (decAction == NDevice::EAP_DOWN) || (IncAction == NDevice::EAP_DOWN) )
        {
            moveWaitTimer.Reset();
            moveTimer.Disable();

            excAction = true;
        }

        if( ((decAction == NDevice::EAP_HOLD) || (IncAction == NDevice::EAP_HOLD)) && moveWaitTimer.Expired() )
        {
            moveWaitTimer.Disable();
            moveTimer.Reset();
        }

        if( (IncAction == NDevice::EAP_DOWN) || ((IncAction == NDevice::EAP_HOLD) && moveTimer.Expired(CTimer::RESTART_ON_EXPIRE)) )
        {
            IncSlider(1);
        }
        else if( (decAction == NDevice::EAP_DOWN) || ((decAction == NDevice::EAP_HOLD) && moveTimer.Expired(CTimer::RESTART_ON_EXPIRE)) )
        {
            IncSlider(-1);
        }

        // Force an execute action becuase the slider button 
        // is managed inside the slide class. This has to be
        // done on the up state to register correctly on the first movement
        // This has to be done after the IncSlider
        if( excAction )
            ExecuteAction();
    }

    return result;

}	// HandleUserSelect


/************************************************************************
*    desc:  Respond to user select
************************************************************************/
void CUISlider::RespondToUserSelect()
{
    CParent::RespondToUserSelect();

    if( (GetActiveSubControlIndex() == NUIControl::NO_ACTIVE_CONTROL) && IsActionOwner() )
    {
        CPoint dif = CGameController::Instance().GetAbsolutePosScaled() - GetSubControl()->GetTransPos();//GetSubControl()->GetCenter();

        if( orientation == EO_HORIZONTAL )
            IncSliderMovePos( dif.GetX() );

        else
            IncSliderMovePos( -dif.GetY() );
    }

}	// RespondToUserSelect




