/************************************************************************
*    FILE NAME:       uibuttonlist.cpp
*
*    DESCRIPTION:     Class for user interface buttons
************************************************************************/

// Physical component dependency
#include <gui/uibuttonlist.h>

// Game lib dependencies
#include <2d/spritegroup2d.h>
#include <2d/visualsprite2d.h>
#include <common/fontgroup.h>
#include <controller/gamecontroller.h>
#include <xact/xact.h>
#include <gui/uicontrolfactory.h>

typedef CUISubControl CParent;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CUIButtonList::CUIButtonList()
             : activeIndex(0)

{
    SetType( NUIControl::ECT_BUTTON_LIST );

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CUIButtonList::~CUIButtonList()
{
}   // Destructer


/************************************************************************
*    desc:  Load the control info from XML node
*  
*    param: node - XML node
************************************************************************/
void CUIButtonList::LoadFromNode( const XMLNode & node )
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

    // See if this is a image list
    XMLNode objDataNode = node.getChildNode( "objectDataType" );

    if( !objDataNode.isEmpty() )
    {
        scpImageLst.reset( CreateSpriteGroup( objDataNode ) );
        activeIndex = 0;
    }

}	// LoadFromNode


/************************************************************************
*    desc:  Set the string index
************************************************************************/
void CUIButtonList::SetActiveIndex( uint index )
{
    if( !spFontGroupVec.empty() && !scpImageLst && (index < spFontGroupVec[0].fontStringVec.size()) )
    {
        activeIndex = index;
        spFontGroupVec[0].fontSprite.SetFontString( spFontGroupVec[0].fontStringVec[activeIndex] );
    }
    else if( spFontGroupVec.empty() && scpImageLst && (index < static_cast<uint>(scpImageLst->GetVisualSprite()->GetFrameCount())) )
    {
        activeIndex = index;
        scpImageLst->GetVisualSprite()->SetCurrentFrame( activeIndex );
    }

}	// SetActiveIndex


/************************************************************************
*    desc:  Act in accordance to user input
************************************************************************/
void CUIButtonList::HandleUserNavigation( NUIMenu::EMenuNavigationState navState,
                                          const std::string & nextAction,
                                          const std::string & previousAction )
{
    CParent::HandleUserNavigation( navState, nextAction, previousAction );

    // Update if the control is active
    if( (GetState() == NUIControl::ECS_ACTIVE) && IsVisible() && (navState == NUIMenu::EMNS_GAMEPAD_KEYBAORD) )
        SetSubStates( NUIControl::ECS_ACTIVE );

}	// HandleUserNavigation


/************************************************************************
*    desc:  handle user select
*
*	parm: navState - navigation state
************************************************************************/
bool CUIButtonList::HandleUserSelect( NUIMenu::EMenuNavigationState navState )
{
    bool result(false);

    // Need to allow base control selection for the keyboard
    if( navState == NUIMenu::EMNS_GAMEPAD_KEYBAORD )
        result = CUIControl::HandleUserSelect( navState );
    else
        result = CParent::HandleUserSelect( navState );

    return result;

}	// HandleUserSelect


/************************************************************************
*    desc:  Get the state of this control
*  
*    ret: EControlState - control state
************************************************************************/
NUIControl::EControlState CUIButtonList::GetState()
{
    // Need to use base control state to properly resync active animations
    if( CGameController::Instance().GetLastDevicedUsed() == NDevice::MOUSE )
        return CParent::GetState();
    else
        return CUIControl::GetState();

}	// GetState


/************************************************************************
*    desc:  Test for user selection
*
*	parm: navState - navigation state
************************************************************************/
bool CUIButtonList::CheckForUserSelection( NUIMenu::EMenuNavigationState navState )
{
    // Respond to controls that that use left\right for selection
    if( (navState == NUIMenu::EMNS_GAMEPAD_KEYBAORD) &&
        WasDecIncSelect( CGameController::Instance().WasAction( decSelectAction, NDevice::EAP_DOWN ),
                         CGameController::Instance().WasAction( incSelectAction, NDevice::EAP_DOWN ) ) )
    {
        return true;
    }

    return false;

}	// CheckForUserSelection


/************************************************************************
*    desc:  Was there a Dec/Inc select
************************************************************************/
bool CUIButtonList::WasDecIncSelect( bool decAction, bool incAction )
{
    if( decAction )
        SetActiveSubControlIndex( NUIControl::BTN_DECREMENT );

    else if( incAction )
        SetActiveSubControlIndex( NUIControl::BTN_INCREMENT );

    return (decAction || incAction);

}	// WasDecIncSelect


/************************************************************************
*    desc:  Execute the action
************************************************************************/
void CUIButtonList::ExecuteAction()
{
    // The button needs to change before the smart gui is called
    if( GetActiveSubControlIndex() == NUIControl::BTN_DECREMENT )
        DecDisplay();

    else if( GetActiveSubControlIndex() == NUIControl::BTN_INCREMENT )
        IncDisplay();

    // For this to work, need to bypass CUISubControl
    CUIControl::ExecuteAction();

}	// ExecuteAction


/************************************************************************
*    desc:  Inc the button display
************************************************************************/
void CUIButtonList::IncDisplay()
{
    if( !spFontGroupVec.empty() && !scpImageLst )
    {
        activeIndex = (activeIndex + 1) % spFontGroupVec[0].fontStringVec.size();

        spFontGroupVec[0].fontSprite.SetFontString( spFontGroupVec[0].fontStringVec[activeIndex] );
    }
    else if( scpImageLst )
    {
        activeIndex = (activeIndex + 1) % scpImageLst->GetVisualSprite()->GetFrameCount();

        scpImageLst->GetVisualSprite()->SetCurrentFrame( activeIndex );

        if( !spFontGroupVec.empty() )
            spFontGroupVec[0].fontSprite.SetFontString( spFontGroupVec[0].fontStringVec[activeIndex] );
    }

}	// IncDisplay


/************************************************************************
*    desc:  Dec the button display
************************************************************************/
void CUIButtonList::DecDisplay()
{
    if( !spFontGroupVec.empty() && !scpImageLst )
    {
        if( activeIndex > 0 )
            activeIndex = (activeIndex - 1) % spFontGroupVec[0].fontStringVec.size();
        else
            activeIndex = static_cast<int>(spFontGroupVec[0].fontStringVec.size()-1);

        spFontGroupVec[0].fontSprite.SetFontString( spFontGroupVec[0].fontStringVec[activeIndex] );
    }
    else if( scpImageLst )
    {
        if( activeIndex > 0 )
            activeIndex = (activeIndex - 1) % scpImageLst->GetVisualSprite()->GetFrameCount();
        else
            activeIndex = scpImageLst->GetVisualSprite()->GetFrameCount() - 1;

        scpImageLst->GetVisualSprite()->SetCurrentFrame( activeIndex );

        if( !spFontGroupVec.empty() )
            spFontGroupVec[0].fontSprite.SetFontString( spFontGroupVec[0].fontStringVec[activeIndex] );
    }

}	// DecDisplay


/************************************************************************
*    desc:  Update the button
************************************************************************/
void CUIButtonList::Update()
{
    // Call the parent
    CParent::Update();

    // Update all controls
    if( scpImageLst )
        scpImageLst->Update();

}	// Update


/************************************************************************
*    desc:  Transform the control
************************************************************************/
void CUIButtonList::Transform()
{
    if( scpImageLst && GetParameters().IsSet( CObject::TRANSFORM ) )
    {
        CBitmask tmpTransParam = GetParameters();
        CParent::Transform();

        // Tell the sprite group that they need to transform
        scpImageLst->SetTransformParameters( tmpTransParam, true );
    }

    // Transform the parent class's components
    CParent::Transform();

    if( scpImageLst )
        scpImageLst->Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

}	// Transform


/************************************************************************
*    desc:  Transform the control
*
*	 param: const CMatrix & _scaledMatrix  - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point      - passed in relative position
************************************************************************/
void CUIButtonList::Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                               const CWorldPoint & point )
{
    if( scpImageLst && GetParameters().IsSet( CObject::TRANSFORM ) )
    {
        CBitmask tmpTransParam = GetParameters();
        CParent::Transform( _scaledMatrix, _unscaledMatrix, point );

        // Tell the sprite group that they need to transform
        scpImageLst->SetTransformParameters( tmpTransParam, true );
    }

    // Transform the parent class's components
    CParent::Transform( _scaledMatrix, _unscaledMatrix, point );

    // Transform the control's visual components
    if( scpImageLst )
        scpImageLst->Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );
    
}	// Transform


/************************************************************************
*    desc:  Render
************************************************************************/
void CUIButtonList::Render()
{
    // Call the parent
    CParent::Render();

    if( scpImageLst && IsVisible() )
        scpImageLst->Render();

}	// Render


/************************************************************************
*    desc:  Set the active command string
************************************************************************/
void CUIButtonList::SetActiveCommand( const std::string & commandStr )
{
    // Call the parent
    CParent::SetActiveCommand( commandStr );

    if( scpImageLst )
        scpImageLst->SetActiveCommand( commandStr );

}	// SetActiveCommand
