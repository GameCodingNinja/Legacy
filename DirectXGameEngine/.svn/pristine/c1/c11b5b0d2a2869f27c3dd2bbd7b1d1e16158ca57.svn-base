/************************************************************************
*    FILE NAME:       uicontrol.cpp
*
*    DESCRIPTION:     base class for DirectX user interface controls
************************************************************************/

// Physical component dependency
#include <gui/uicontrol.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <2d/spritegroup2d.h>
#include <2d/objectvisualdata2d.h>
#include <2d/collisionsprite2d.h>
#include <utilities/collisionfunc2d.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/genfunc.h>
#include <utilities/exceptionhandling.h>
#include <controller/gamecontroller.h>
#include <common/defs.h>
#include <common/fontgroup.h>
#include <gui/ismartguibase.h>
#include <gui/controlevent.h>
#include <xact/xact.h>

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CUIControl::CUIControl()
          : state(NUIControl::ECS_NULL),
            lastState(NUIControl::ECS_NULL),
            transitionState(NUIControl::ETS_NULL),
            lastTransitionState(NUIControl::ETS_NULL),
            defaultState(NUIControl::ECS_NULL),
            actionType(NUIControl::ECAT_ACTION),
            forceStateUpdate(false),
            playNavigationSnd(true),
            conflicted(false),
            mouseActionPress(NDevice::EAP_UP),
            dispatchActionEvent(false),
            mouseOnlyCtrl(false)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CUIControl::~CUIControl()
{
}   // Destructer


/************************************************************************
*    desc:  Load the control info from XML node
*  
*    param: node - XML node
************************************************************************/
void CUIControl::LoadFromNode( const XMLNode & node )
{
    std::string controlFilePath;

    // Get the file path node to the control specific xml code
    XMLNode filePathNode = node.getChildNode( "filePath" );

    // Load the control specific xml file
    if( !filePathNode.isEmpty() )
    {
        // Get the control's file path
        controlFilePath = filePathNode.getAttribute( "file" );

        // Open and parse the control's XML file:
        XMLNode controlNode = XMLNode::openFileHelper( controlFilePath.c_str(), "UIControl" );

        // Load xml specific control code
        LoadControlFromNode( controlNode );
    }

    // Set the controls name
    if( node.isAttributeSet( "name" ) )
        SetName( std::string( node.getAttribute( "name" ) ) );

    // Set the default state of the control
    if( node.isAttributeSet( "defaultState" ) )
        SetDefaultState( std::string( node.getAttribute( "defaultState" ) ) );

    // See if this control dispatches action events
    if( node.isAttributeSet( "dispatchActionEvent" ) )
        dispatchActionEvent = (std::string( node.getAttribute( "dispatchActionEvent" ) ) == "true");

    // Set if this control should play navigation sounds
    if( node.isAttributeSet( "playNavSnds" ) )
        playNavigationSnd = !( std::string( node.getAttribute( "playNavSnds" ) ) == "false" );

    // Is this a mouse only control
    if( node.isAttributeSet( "mouseOnlyCtrl" ) )
        mouseOnlyCtrl = ( std::string( node.getAttribute( "mouseOnlyCtrl" ) ) == "true" );

    // Get the selection string for this control
    XMLNode selectionNode = node.getChildNode( "selection" );

    // Load the control specific xml file
    if( !selectionNode.isEmpty() )
    {
        if( selectionNode.isAttributeSet( "selectAction" ) )
            selectAction = selectionNode.getAttribute( "selectAction" );

        if( selectionNode.isAttributeSet( "mouseActionPress" ) )
            if( std::string(selectionNode.getAttribute( "mouseActionPress" )) == "down" )
                mouseActionPress = NDevice::EAP_DOWN;
    }

    // Set the transform data
    SetTransData( NParseHelper::LoadTransformData( node ) );
    SetDefaultPos( GetPos() );

    // Setup the action
    XMLNode actionNode = node.getChildNode( "action" );

    if( !actionNode.isEmpty() )
    {
        // Set the action type
        if( actionNode.isAttributeSet( "actionType" ) )
            SetActionType( actionNode.getAttribute( "actionType" ) );

        // Set the execution action.
        if( actionNode.isAttributeSet( "executionAction" ) )
            SetExecutionAction( std::string(actionNode.getAttribute( "executionAction" )) );
    }

    // Init to the default state
    RevertToDefaultState();

    // Check for strings
    XMLNode stringLstNode = node.getChildNode( "fontStringLst" );
    if( !stringLstNode.isEmpty() )
    {
        if( spFontGroupVec.size() == 0 )
            throw NExcept::CCriticalException("Font String Load Error!",
                boost::str( boost::format("Control strings defined but font not defined (%s).\n\n%s\nLine: %s") % controlFilePath.c_str() % __FUNCTION__ % __LINE__ ));

        // See if we have a list of strings
        for( int i = 0; i < stringLstNode.nChildNode(); ++i )
        {
            uint fontIndex(0);
            XMLNode stringNode = stringLstNode.getChildNode( "string", i );

            if( stringNode.isAttributeSet( "fontIndex" ) )
                fontIndex = atoi(stringNode.getAttribute( "fontIndex" ));

            if( fontIndex >= spFontGroupVec.size() )
                throw NExcept::CCriticalException("Font String Index Error!",
                    boost::str( boost::format("Font index out of range for defined string (%s).\n\n%s\nLine: %s") % controlFilePath.c_str() % __FUNCTION__ % __LINE__ ));

            spFontGroupVec[fontIndex].fontStringVec.push_back( stringNode.getAttribute( "text" ) );
        }

        // Set each font group to it's first string if one exists
        for( size_t i = 0; i < spFontGroupVec.size(); ++i )
            if( !spFontGroupVec[i].fontStringVec.empty() )
                spFontGroupVec[i].fontSprite.SetFontString( spFontGroupVec[i].fontStringVec.front() );
    }

    // Set the controls based on their default state
    // This needs to be done before the transition in to set the correct color
    SetControlCommand();
    UpdateSprites();

}	// LoadFromNode


/************************************************************************
*    desc:  Load the control specific info from XML node
*  
*    param: controlNode - XML node
************************************************************************/
void CUIControl::LoadControlFromNode( const XMLNode & controlNode )
{
    assert( (spSpriteGroupVec.size() == 0) && "Memory Leak!" );

    // Get the object type string
    const XMLNode listGroupNode = controlNode.getChildNode( "listGroup" );
    objGroupStr = listGroupNode.getAttribute( "name" );

    // Get the list of object data associated with this button
    const XMLNode objDataLstNode = controlNode.getChildNode( "objectDataList" );

    if( !objDataLstNode.isEmpty() )
    {
        spSpriteGroupVec.reserve( objDataLstNode.nChildNode() );

        for( int i = 0; i < objDataLstNode.nChildNode(); ++i )
        {
            const XMLNode objDataNode = objDataLstNode.getChildNode( "objectDataType", i );

            // Set up the control's sprite group
            spSpriteGroupVec.push_back( CreateSpriteGroup( objDataNode ) );

            // Find the largest size width and height of the different visual sprites for this controls size
            if( spSpriteGroupVec.back().GetVisualData().GetSize().w > size.w )
                size.w = spSpriteGroupVec.back().GetVisualData().GetSize().w;

            if( spSpriteGroupVec.back().GetVisualData().GetSize().h > size.h )
                size.h = spSpriteGroupVec.back().GetVisualData().GetSize().h;
        }
    }

    // Get the sprite fonts assiciated with this control
    XMLNode fontLstNode = controlNode.getChildNode( "fontList" );

    if( !fontLstNode.isEmpty() )
    {
        spFontGroupVec.reserve( fontLstNode.nChildNode() );

        for( int i = 0; i < fontLstNode.nChildNode(); ++i )
        {
            const XMLNode fontNode = fontLstNode.getChildNode( "font", i );

            // Allocate the font sprite
            spFontGroupVec.push_back( new CFontGroup );

            // Add to the safe vector
            spFontGroupVec.back().fontSprite.LoadFromNode(fontNode, GetObjGroup());
        }
    }

}	// LoadControlFromNode


/************************************************************************
*    desc:  Create a sprite group for this control
*           NOTE: This function doesn't own the pointer.
*  
*    param: objDataNode - XML node
************************************************************************/
CSpriteGroup2D * CUIControl::CreateSpriteGroup( const XMLNode & objDataNode )
{
    const std::string objTypeStr = objDataNode.getAttribute( "type" );

    // Set up the control's sprite group
    CSpriteGroup2D * pSpriteGrp =  
        new CSpriteGroup2D( CObjectDataList2D::Instance().GetData( GetObjGroup(), objTypeStr ) );

    // Load the transform data
    pSpriteGrp->SetTransData( NParseHelper::LoadTransformData( objDataNode ) );

    return pSpriteGrp;

}	// CreateSpriteGroup


/************************************************************************
*    desc:  Act in accordance to user input
************************************************************************/
void CUIControl::HandleUserNavigation( NUIMenu::EMenuNavigationState navState,
                                       const std::string & nextAction,
                                       const std::string & previousAction )
{
    // Does nothing

}	// HandleUserNavigation


/************************************************************************
*    desc:  handle user select
*
*	parm: navState - navigation state
************************************************************************/
bool CUIControl::HandleUserSelect( NUIMenu::EMenuNavigationState navState )
{
    if( CheckForUserSelection( navState ) )
    {
        UserSelectionConfirmed( navState );
        return true;
    }

    return false;

}	// HandleUserSelect


/************************************************************************
*    desc:  handle user select mouse selection
*
*	parm: navState - navigation state
************************************************************************/
bool CUIControl::HandleUserMouseSelection( NUIMenu::EMenuNavigationState navState, NDevice::EActionPress actionPress )
{
    if( CheckForUserMouseSelection( navState, actionPress ) )
    {
        UserSelectionConfirmed( navState );
        return true;
    }

    return false;

}	// HandleUserMouseSelection


/************************************************************************
*    desc:  Respond to user select
************************************************************************/
void CUIControl::RespondToUserSelect()
{
    // Does nothing

}	// HandleUserNavigation


/************************************************************************
*    desc:  Records the owner on the down action
*
*	parm: controlActionPress - check for down action
************************************************************************/
void CUIControl::SetActionOwner( NDevice::EActionPress controlActionPress )
{
    if( controlActionPress == NDevice::EAP_DOWN )
        CGameController::Instance().SetActionOwner(this);

}	// SetActionOwner


/************************************************************************
*    desc:  Records the owner on the down action
*
*	parm: controlActionPress - check for down action
************************************************************************/
bool CUIControl::IsActionOwner()
{
    return CGameController::Instance().IsActionOwner(this);

}	// IsActionOwner


/************************************************************************
*    desc:  Test for user selection
*
*	parm: navState - navigation state
************************************************************************/
bool CUIControl::CheckForUserSelection( NUIMenu::EMenuNavigationState navState )
{
    NDevice::EActionPress controlActionPress = CGameController::Instance().WasAction( selectAction );

    // Records the owner on the down action
    SetActionOwner( controlActionPress );

    // If the mouse was used last, make sure it's still within the control to be able to selected it
    return IsActionOwner() &&
           (((navState == NUIMenu::EMNS_MOUSE) && (controlActionPress == mouseActionPress) && IsPointInSprite()) ||
           ((navState == NUIMenu::EMNS_GAMEPAD_KEYBAORD) && (controlActionPress == NDevice::EAP_DOWN)));

}	// CheckForUserSelection


/************************************************************************
*    desc:  Test for user mouse selection
*
*	parm: navState - navigation state
************************************************************************/
bool CUIControl::CheckForUserMouseSelection( NUIMenu::EMenuNavigationState navState, NDevice::EActionPress actionPress )
{
    NDevice::EActionPress controlActionPress = CGameController::Instance().WasAction( selectAction );

    // Records the owner on the down action
    SetActionOwner( controlActionPress );

    // If the mouse was used last, make sure it's still within the control to be able to selected it
    return( IsActionOwner() &&
            (navState == NUIMenu::EMNS_MOUSE) &&
            (controlActionPress == actionPress) && 
             IsPointInSprite() );

}	// CheckForUserMouseSelection


/************************************************************************
*    desc:  Get the user mouse action press state
*
*	parm: EActionPress - action press state
************************************************************************/
NDevice::EActionPress CUIControl::GetUserMouseActionPressState( NUIMenu::EMenuNavigationState navState )
{
    NDevice::EActionPress result( NDevice::EAP_IDLE );

    // Return the mouse action result
    if( (navState == NUIMenu::EMNS_MOUSE) && IsPointInSprite() )
        result = CGameController::Instance().WasAction( selectAction );

    return result;

}	// GetUserMouseActionPressState


/************************************************************************
*    desc:  A selection has been confirmed
*
*	parm: navState - navigation state
************************************************************************/
void CUIControl::UserSelectionConfirmed( NUIMenu::EMenuNavigationState navState )
{
    PlaySelectSound();
    SetState( NUIControl::ECS_SELECTED );

}	// UserSelectionConfirmed


/************************************************************************
*    desc:  Set the smart control pointer. This class owns the pointer
************************************************************************/
void CUIControl::SetSmartGui( CSmartGuiControl * pSmartGuiControl )
{
    spSmartGui.reset( pSmartGuiControl );

}	// SetSmartGui


/************************************************************************
*    desc:  Do any smart create
************************************************************************/
void CUIControl::SmartCreate()
{
    if( spSmartGui )
        spSmartGui->Create();

}	// SmartCreate


/************************************************************************
*    desc:  Execute the action
************************************************************************/
void CUIControl::ExecuteAction()
{
    if( spSmartGui )
        spSmartGui->Execute();

    // Dispatch a call back to let the game handle this control event
    if( dispatchActionEvent )
    {
        CControlActionEvent controlActionEvent;
        controlActionEvent.Dispatch( this );
    }

}	// ExecuteAction


/************************************************************************
*    desc:  Do any smart Display Init
************************************************************************/
void CUIControl::SmartDisplay()
{
    if( spSmartGui )
        spSmartGui->Display();

}	// SmartDisplay


/************************************************************************
*    desc:  Get/Set the type of control
*  
*    ret: EControlState - control type
************************************************************************/
NUIControl::EControlType CUIControl::GetType()
{
    return type;

}	// GetType

void CUIControl::SetType( NUIControl::EControlType value )
{
    type = value;

}	// SetType


/************************************************************************
*    desc:  Get/Set the execution action
*  
*    ret:	string & - name of action
************************************************************************/
const std::string & CUIControl::GetExecutionAction()
{
    return executionAction;

}	// GetExecutionAction

void CUIControl::SetExecutionAction( const std::string & action )
{
    executionAction = action;

}	// SetExecutionAction


/************************************************************************
*    desc:  Get/Set the state of this control
*  
*    ret: EControlState - control state
************************************************************************/
NUIControl::EControlState CUIControl::GetState()
{
    return state;

}	// GetState

void CUIControl::SetState( NUIControl::EControlState value )
{
    state = value;

    if( state == NUIControl::ECS_INACTIVE )
        state = value;

}	// SetState

void CUIControl::SetState( const std::string & value )
{
    if( value == "inactive" )
        state = NUIControl::ECS_INACTIVE;

    else if( value == "active" )
        state = NUIControl::ECS_ACTIVE;

    else if( value == "disabled" )
        state = NUIControl::ECS_DISABLED;

    else if( value == "selected" )
        state = NUIControl::ECS_SELECTED;

}	// SetState


/************************************************************************
*    desc:  Get the last state of this control
*  
*    ret: EControlState - control state
************************************************************************/
NUIControl::EControlState CUIControl::GetLastState()
{
    return lastState;

}	// GetLastState


/************************************************************************
*    desc:  Make the state change final
************************************************************************/
void CUIControl::FinalizeStateChange()
{
    lastState = state;
    forceStateUpdate = false;

}	// GetLastState


/************************************************************************
*    desc:  Is the state change final
*  
*    ret: bool - true if state has been finalized
************************************************************************/
bool CUIControl::IsStateChangeFinal()
{
    return ((state == lastState) && !forceStateUpdate);

}	// IsStateChangeFinal


/************************************************************************
*    desc:  Force a state update
************************************************************************/
void CUIControl::ForceStateUpdate( NUIMenu::EMenuNavigationState navState )
{
    forceStateUpdate = true;

}	// ForceStateUpdate


/************************************************************************
*    desc:  Get/Set the transition state of this control
*  
*    ret: ETransitionState - transition state
************************************************************************/
NUIControl::ETransitionState CUIControl::GetTransitionState()
{
    return transitionState;

}	// GetTransitionState

void CUIControl::SetTransitionState( NUIControl::ETransitionState value )
{
    transitionState = value;

}	// SetTransitionState


/************************************************************************
*    desc:  Get the last transition state
*  
*    ret: ETransitionState - transition state
************************************************************************/
NUIControl::ETransitionState CUIControl::GetLastTransitionState()
{
    return lastTransitionState;

}	// GetLastTransitionState


/************************************************************************
*    desc:  Is the state change final
*  
*    ret: bool - true if state has been finalized
************************************************************************/
bool CUIControl::IsTransitionStateChangeFinal()
{
    return (transitionState == lastTransitionState);

}	// IsTransitionStateChangeFinal


/************************************************************************
*    desc:  Is the transition state active
*  
*    ret: bool - true if state is active
************************************************************************/
bool CUIControl::IsTransitionStateActive()
{
    return (transitionState != NUIControl::ETS_NULL);

}	// IsTransitionStateActive


/************************************************************************
*    desc:  Make the state change final
************************************************************************/
void CUIControl::FinalizeTransitionStateChange()
{
    lastTransitionState = transitionState;

}	// FinalizeTransitionStateChange


/************************************************************************
*    desc:  Get/Set the control's action type
*  
*    ret: EControlActionType - control action type
************************************************************************/
NUIControl::EControlActionType CUIControl::GetActionType()
{
    return actionType;

}	// GetActionType

void CUIControl::SetActionType( NUIControl::EControlActionType value )
{
    actionType = value;

}	// SetActionType

void CUIControl::SetActionType( const std::string & value )
{
    if( value == "action" )
        actionType = NUIControl::ECAT_ACTION;

    else if( value == "to_menu" )
        actionType = NUIControl::ECAT_TO_MENU;

    else if( value == "return" )
        actionType = NUIControl::ECAT_RETURN;

    else if( value == "close" )
        actionType = NUIControl::ECAT_CLOSE;

    else if( value == "deactivate" )
        actionType = NUIControl::ECAT_DEACTIVATE;

    else if( value == "change_focus" )
        actionType = NUIControl::ECAT_CHANGE_FOCUS;

    else if( value == "back_to_main_menu" )
        actionType = NUIControl::ECAT_BACK_TO_MAIN_MENU;

}	// SetActionType


/************************************************************************
*    desc:  Set the default state of this control
*  
*    param: EControlState value - control state
************************************************************************/
void CUIControl::SetDefaultState( NUIControl::EControlState value )
{
    defaultState = value;

}	// SetState


/************************************************************************
*    desc:  Get the smart gui pointer
*  
*    ret: CSmartGuiControl - 
************************************************************************/
CSmartGuiControl * CUIControl::GetSmartGuiPtr()
{
    return spSmartGui.get();

}	// GetActionType


/************************************************************************
*    desc:  Set the default state of this control
*  
*    param: string & value - control state
************************************************************************/
void CUIControl::SetDefaultState( std::string & value )
{
    if( value == "inactive" )
        defaultState = NUIControl::ECS_INACTIVE;

    else if( value == "active" )
        defaultState = NUIControl::ECS_ACTIVE;

    else if( value == "disabled" )
        defaultState = NUIControl::ECS_DISABLED;

    else if( value == "selected" )
        defaultState = NUIControl::ECS_SELECTED;

}	// SetDefaultState


/************************************************************************
*    desc:  Reset the control to inactive if its not disabled
************************************************************************/
void CUIControl::Reset()
{
    if( state != NUIControl::ECS_DISABLED )
        SetState( NUIControl::ECS_INACTIVE );

}	// Reset


/************************************************************************
*    desc:  Set the control to their default behavior
************************************************************************/
void CUIControl::RevertToDefaultState()
{
    SetState( defaultState );

}	// RevertToDefaultState


/************************************************************************
*    desc:  Is the mouse in the sprite
*
*	 ret:	bool - mouse was in sprite
************************************************************************/
bool CUIControl::IsPointInSprite()
{
    bool result(false);

    // The mouse's position relative to the control's parent
    CPoint mousePos = CGameController::Instance().GetAbsolutePosScaled() - GetTransPos();

    for( size_t i = 0; i < spSpriteGroupVec.size(); ++i )
    {
        // If there isn't any collision sprite, don't check if the mouse is in it
        if( !spSpriteGroupVec[i].IsCollisionActive() )
            continue;

        if( spSpriteGroupVec[i].IsPointInSprite( mousePos ) )
        {
            CSpriteGroup2D * pTest = &spSpriteGroupVec[i];
            result = true;
            break;
        }
    }

    return result;

}	// IsPointInSprite


/************************************************************************
*    desc:  Is this control selectable by mouse
*
*	 ret:	bool - control can be selected
************************************************************************/
bool CUIControl::IsSelectable( NUIMenu::EMenuNavigationState navState )
{
    if( navState == NUIMenu::EMNS_MOUSE )
    {
        return !((state == NUIControl::ECS_DISABLED) ||
                 (type == NUIControl::ECT_LABEL) ||
                 !IsVisible() );
    }
    else if( navState == NUIMenu::EMNS_GAMEPAD_KEYBAORD )
    {
        return !((state == NUIControl::ECS_DISABLED) ||
                 (type == NUIControl::ECT_LABEL) ||
                 mouseOnlyCtrl ||
                 !IsVisible() );
    }

    return false;

}	// IsSelectable


/************************************************************************
*    desc:  Set the string to the string list vector
************************************************************************/
void CUIControl::SetToFontStringList( const std::string & str, uint fontGrpIndex )
{
    if( fontGrpIndex < spFontGroupVec.size() )
        spFontGroupVec[fontGrpIndex].fontStringVec.push_back( str );

}	// SetToFontStringList


/************************************************************************
*    desc:  Use the string index to display
************************************************************************/
void CUIControl::DisplayFontStringIndex( uint fontGrpIndex, uint strIndex )
{
    if( (fontGrpIndex < spFontGroupVec.size()) && (strIndex < spFontGroupVec[fontGrpIndex].fontStringVec.size()) )
        spFontGroupVec[fontGrpIndex].fontSprite.SetFontString( spFontGroupVec[fontGrpIndex].fontStringVec[strIndex] );

}	// SetToFontStringList


/************************************************************************
*    desc:  Display a specific string in the control. 
************************************************************************/
void CUIControl::DisplayFontString( const std::string & displayStr, uint fontGrpIndex )
{
    if( fontGrpIndex < spFontGroupVec.size() )
        spFontGroupVec[fontGrpIndex].fontSprite.SetFontString( displayStr );

}	// SetToFontStringList


/************************************************************************
*    desc:  Clear the string list vector
************************************************************************/
void CUIControl::ClearFontStringList( uint fontGrpIndex )
{
    if( fontGrpIndex < spFontGroupVec.size() )
        spFontGroupVec[fontGrpIndex].fontStringVec.clear();

}	// SetToFontStringList


/************************************************************************
*    desc:  Get the string being displayed by the sprite font
************************************************************************/
std::string CUIControl::GetDisplayedString( uint fontGrpIndex )
{
    std::string result;

    if( fontGrpIndex < spFontGroupVec.size() )
        result = spFontGroupVec[fontGrpIndex].fontSprite.GetDisplayedString();

    return result;

}	// GetDisplayedString


/************************************************************************
*    desc:  Clear out all the font group strings
************************************************************************/
void CUIControl::ClearAllFontStringList()
{
    for( size_t i = 0; i < spFontGroupVec.size(); ++i )
        spFontGroupVec[i].fontStringVec.clear();

}	// ClearAllFontStringList


/************************************************************************
*    desc:  Transform the control
************************************************************************/
void CUIControl::Transform()
{
    if( GetParameters().IsSet( CObject::TRANSFORM ) )
    {
        CBitmask tmpTransParam = GetParameters();
        CObject::Transform();

        // Tell the children of this control that they need to transform
        for( size_t i = 0; i < spSpriteGroupVec.size(); ++i )
            spSpriteGroupVec[i].SetTransformParameters( tmpTransParam, true );

        for( size_t i = 0; i < spFontGroupVec.size(); ++i )
            spFontGroupVec[i].fontSprite.SetTransformParameters( tmpTransParam, true );
    }

    for( size_t i = 0; i < spSpriteGroupVec.size(); ++i )
        spSpriteGroupVec[i].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

    for( size_t i = 0; i < spFontGroupVec.size(); ++i )
        spFontGroupVec[i].fontSprite.Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

}	// Transform


/************************************************************************
*    desc:  Transform the control
*
*	 param: const CMatrix & _scaledMatrix  - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point      - passed in relative position
************************************************************************/
void CUIControl::Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                            const CWorldPoint & point )
{
    if( GetParameters().IsSet( CObject::TRANSFORM ) )
    {
        CBitmask tmpTransParam = GetParameters();
        CObject::Transform( _scaledMatrix, _unscaledMatrix, point );

        // Tell the children of this control that they need to transform
        for( size_t i = 0; i < spSpriteGroupVec.size(); ++i )
            spSpriteGroupVec[i].SetTransformParameters( tmpTransParam, true );

        for( size_t i = 0; i < spFontGroupVec.size(); ++i )
            spFontGroupVec[i].fontSprite.SetTransformParameters( tmpTransParam, true );
    }

    for( size_t i = 0; i < spSpriteGroupVec.size(); ++i )
        spSpriteGroupVec[i].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

    for( size_t i = 0; i < spFontGroupVec.size(); ++i )
        spFontGroupVec[i].fontSprite.Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );
    
}	// Transform


/************************************************************************
*    desc:  Update the control
************************************************************************/
void CUIControl::Update()
{
    // If the transition state is active, do transition related stuff
    if( IsTransitionStateActive() )
    {
        if( !IsTransitionStateChangeFinal() )
        {
            // Make the state change final
            FinalizeTransitionStateChange();

            // Force a state update after a transition
            ForceStateUpdate();

            std::string transCommand("transitionIn");

            if( GetTransitionState() == NUIControl::ETS_TRANSITION_OUT )
                transCommand = "transitionOut";

            // Set the active command string
            SetActiveCommand( transCommand );
        }
    }
    else
    {
        if( !IsStateChangeFinal() )
        {
            // Make the state change final
            FinalizeStateChange();

            // Set the controls command based on it's current state
            SetControlCommand();
        }
    }

    // Update the group
    UpdateSprites();

}	// Update


/************************************************************************
*    desc:  Update the sprites associated with this control
************************************************************************/
void CUIControl::UpdateSprites()
{
    for( size_t i = 0; i < spSpriteGroupVec.size(); ++i )
        spSpriteGroupVec[i].Update();

    for( size_t i = 0; i < spFontGroupVec.size(); ++i )
        spFontGroupVec[i].fontSprite.Update();

}	// UpdateSprites


/************************************************************************
*    desc:  Render the button
************************************************************************/
void CUIControl::Render()
{
    if( IsVisible() )
    {
        for( size_t i = 0; i < spSpriteGroupVec.size(); ++i )
            spSpriteGroupVec[i].Render();

        for( size_t i = 0; i < spFontGroupVec.size(); ++i )
            spFontGroupVec[i].fontSprite.Render();
    }

}	// Render


/************************************************************************
*    desc:  Check to see if the control is animating.
*           NOTE: We only care about non-looping animations
************************************************************************/
bool CUIControl::IsAnimating()
{
    for( uint i = 0; i < spSpriteGroupVec.size(); ++i )
        if( spSpriteGroupVec[i].IsAnimating() && !spSpriteGroupVec[i].IsEndModeLoop() )
            return true;

    for( uint i = 0; i < spFontGroupVec.size(); ++i )
        if(spFontGroupVec[i].fontSprite.IsAnimating() && !spFontGroupVec[i].fontSprite.IsEndModeLoop())
            return true;

    return false;

}	// IsAnimating


/************************************************************************
*    desc:  Set the controls command based on it's current state
************************************************************************/
void CUIControl::SetControlCommand()
{
    std::string stateAnimNameStr("disabled");

    if( GetState() == NUIControl::ECS_INACTIVE )
        stateAnimNameStr = "inactive";

    else if( GetState() == NUIControl::ECS_ACTIVE )
        stateAnimNameStr = "active";

    else if( GetState() == NUIControl::ECS_SELECTED )
        stateAnimNameStr = "selected";

    // Append "conflicted" to the animation name to
    // visually show the control as conflicted
    if( conflicted )
        stateAnimNameStr += "_conflicted";

    // Set the active command string
    SetActiveCommand( stateAnimNameStr );

}	// SetControlState


/************************************************************************
*    desc:  Get the font string
************************************************************************/
const std::string & CUIControl::GetFontString( uint fontIndex, uint stringIndex )
{
    if( (fontIndex >= spFontGroupVec.size()) || (stringIndex >= spFontGroupVec[fontIndex].fontStringVec.size()) )
        throw NExcept::CCriticalException("Font String Index out of Range!",
            boost::str( boost::format("Font or string index out of range (%d, %d).\n\n%s\nLine: %s") % fontIndex % stringIndex % __FUNCTION__ % __LINE__ ));

    return spFontGroupVec[fontIndex].fontStringVec[stringIndex];

}	// GetFontString


/************************************************************************
*    desc:  Play the control "active" sound
************************************************************************/
void CUIControl::PlayActiveSound()
{
    if( playNavigationSnd )
        CXAct::Instance().Play( "menu_highlight" );

}	// PlayActiveSound


/************************************************************************
*    desc:  Play the control "select" sound
************************************************************************/
void CUIControl::PlaySelectSound()
{
    if( playNavigationSnd )
        CXAct::Instance().Play( "menu_select" );

}	// PlayActiveSound


/************************************************************************
*    desc:  Set/Get the conflicted flag
************************************************************************/
void CUIControl::SetConflictedFlag( bool value )
{
    conflicted = value;

}	// SetConflictedFlag

bool CUIControl::GetConflictedFlag() const
{
    return conflicted;

}	// GetConflictedFlag


/************************************************************************
*    desc:  Get the object group name
************************************************************************/
const std::string & CUIControl::GetObjGroup() const
{
    return objGroupStr;

}	// GetObjGroup


/************************************************************************
*    desc:  Set/Get the control name
************************************************************************/
void CUIControl::SetName( const std::string & nameStr )
{
    name = nameStr;

}	// SetName

const std::string & CUIControl::GetName() const
{
    return name;

}	// GetName


/************************************************************************
*    desc:  Is the font group empty
************************************************************************/
bool CUIControl::IsFontGroupEmpty() const
{
    return (spFontGroupVec.size() == 0);

}	// IsFontGroupEmpty


/************************************************************************
*    desc:  Is the font string group empty
************************************************************************/
bool CUIControl::IsFontStringGroupEmpty( uint fontIndex ) const
{
    if( fontIndex < spFontGroupVec.size() )
        if( !spFontGroupVec[fontIndex].fontStringVec.empty() )
            return false;

    return true;

}	// IsFontGroupEmpty


/************************************************************************
*    desc: Do we play navigation sounds?
************************************************************************/
bool CUIControl::GetPlayNavigationSnd()
{
    return playNavigationSnd;

}	// GetPlayNavigationSnd


/************************************************************************
*    desc: Designed to be over written for controls that hold other controls
************************************************************************/
void CUIControl::SetToFirstInactiveControl()
{
    // Base class does nothing

}	// SetToFirstInactiveControl


/************************************************************************
*    desc: Gets the active control
************************************************************************/
CUIControl * CUIControl::GetActiveControl()
{
    // Base class does nothing
    return NULL;

}	// GetActiveControl


/************************************************************************
*    desc: Get the sub control
************************************************************************/
CUIControl * CUIControl::GetSubControl( uint index )
{
    // Base class does nothing
    return NULL;

}	// GetSubControl


/************************************************************************
*    desc: Get the sub control count
************************************************************************/
size_t CUIControl::GetSubControlCount()
{
    // Base class does nothing
    return 0;

}	// GetSubControlCount


/************************************************************************
*    desc: Set the dynamic position
************************************************************************/
void CUIControl::SetDynamicPos()
{
    // Base class does nothing

}	// SetDynamicPos


/************************************************************************
*    desc:  Deactivates all controls 
************************************************************************/
void CUIControl::DeactivateControls(bool toMemu)
{
    Reset();

}	// DeactivateControls


/************************************************************************
*    desc:  Reset the transition states 
************************************************************************/
void CUIControl::ResetTransitionStates()
{
    lastTransitionState = transitionState = NUIControl::ETS_NULL;

}	// ResetTransitionStates


/************************************************************************
*    desc:  Get control size
*  
*    ret: CSize
************************************************************************/
const CSize<float> & CUIControl::GetSize() const
{
    return size;

}	// GetSize


/************************************************************************
*    desc:  Get/Set the active sub-control index
************************************************************************/
int CUIControl::GetActiveSubControlIndex()
{
    // Base control only returns non-active control value
    return NUIControl::NO_ACTIVE_CONTROL;

}	// GetActiveSubControlIndex

void CUIControl::SetActiveSubControlIndex( int value )
{
    // base control does nothing

}	// SetActiveSubControlIndex


/************************************************************************
*    desc:  Get the select action
************************************************************************/
const std::string & CUIControl::GetSelectAction() const
{
    // Base control only returns non-active control value
    return selectAction;

}	// GetSelectAction


/************************************************************************
*    desc:  Is this a mouse only control?
************************************************************************/
bool CUIControl::IsMouseOnlyCtrl() const
{
    return mouseOnlyCtrl;

}	// IsMouseOnlyCtrl


/************************************************************************
*    desc:  Set the active command string
************************************************************************/
void CUIControl::SetActiveCommand( const std::string & commandStr )
{
    for( size_t i = 0; i < spSpriteGroupVec.size(); ++i )
        spSpriteGroupVec[i].SetActiveCommand( commandStr );

    for( size_t i = 0; i < spFontGroupVec.size(); ++i )
        spFontGroupVec[i].fontSprite.SetActiveCommand( commandStr );

}	// SetActiveCommand

