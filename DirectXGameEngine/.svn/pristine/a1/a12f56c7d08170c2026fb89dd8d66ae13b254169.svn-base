/************************************************************************
*    FILE NAME:       uimenu.cpp
*
*    DESCRIPTION:     Class for DirectX user interface menu
************************************************************************/

// Physical component dependency
#include <gui/uimenu.h>

// Boost lib dependencies
#include <boost/format.hpp>
#include <boost/foreach.hpp>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/collisionfunc2d.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/genfunc.h>
#include <gui/uicheckbox.h>
#include <gui/uibuttonlist.h>
#include <gui/uilabel.h>
#include <gui/uiscrollbox.h>
#include <gui/controlevent.h>
#include <gui/ismartguibase.h>
#include <gui/uicontrolfactory.h>
#include <controller/gamecontroller.h>
#include <2d/spritegroup2d.h>
#include <2d/objectvisualdata2d.h>
#include <xact/xact.h>
#include <common/spritedata.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CUIMenu::CUIMenu()
          : state(NUIMenu::EMS_HIDDEN),
            lastState(NUIMenu::EMS_HIDDEN),
            focusGroupIndex(0)
{
    SetVisible(false);

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CUIMenu::~CUIMenu()
{
    //  ** NOTE: pControlHelperMap does not own it's pointers. Don't try to free it here **

}   // Destructer


/************************************************************************
*    desc:  Load the menu info from XML node
*  
*    param: node - XML node
************************************************************************/
void CUIMenu::LoadFromNode( XMLNode & node )
{
    name = node.getAttribute( "name" );
    string filePath = node.getAttribute( "file" );

    // Load the menu specific and control data from XML file
    LoadFromXML( filePath );

    // Set the transform data
    SetTransData( NParseHelper::LoadTransformData( node ) );

    // Load the dynamic offset
    dynamicOffset = NParseHelper::LoadDynamicOffset( node );

    // Position the menu based on the dynamic offset
    SetDynamicPos();

}	// LoadFromNode


/************************************************************************
*    desc:  Set the dynamic position
************************************************************************/
void CUIMenu::SetDynamicPos()
{
    if( dynamicOffset.IsDynamicOffset() )
    {
        CPoint dynPos = GetPos();
        CSize<float> offset = CSettings::Instance().GetDefaultSizeHalf() - (GetSize() * .5f);

        // Strip out any fractional component for correct rendering
        offset.Round();

        if( dynamicOffset.IsBitSet( CDynamicOffset::EDO_LEFT ) )
            dynPos.x = -(offset.w - dynamicOffset.GetX());

        else if( dynamicOffset.IsBitSet( CDynamicOffset::EDO_RIGHT ) )
            dynPos.x = offset.w - dynamicOffset.GetX();

        else if( dynamicOffset.IsBitSet( CDynamicOffset::EDO_HORZ_CENTER ) )
            dynPos.x = dynamicOffset.GetX();

        if( dynamicOffset.IsBitSet( CDynamicOffset::EDO_TOP ) )
            dynPos.y = offset.h - dynamicOffset.GetY();
            
        else if( dynamicOffset.IsBitSet( CDynamicOffset::EDO_BOTTOM ) )
            dynPos.y = -(offset.h - dynamicOffset.GetY());

        else if( dynamicOffset.IsBitSet( CDynamicOffset::EDO_VERT_CENTER ) )
            dynPos.x = dynamicOffset.GetY();

        SetPos( dynPos );
    }

}	// SetDynamicPos


/************************************************************************
*    desc:  Load the menu's parameters from an XML node
************************************************************************/
void CUIMenu::LoadMenuParamtersFromNode( XMLNode & node )
{
    //	MENU NAVIGATION
    const XMLNode menuNavNode = node.getChildNode( "menuNavigation" );
    if( !menuNavNode.isEmpty() )
    {
        nextAction = menuNavNode.getAttribute( "nextAction" );
        previousAction = menuNavNode.getAttribute( "previousAction" );
    }

    //	MENU COLLISION
    const XMLNode menuCollisionNode = node.getChildNode( "menuCollision" );
    if( !menuCollisionNode.isEmpty() )
    {
        if( menuCollisionNode.isAttributeSet("generateFromStaticSpriteIndex") )
        {
            const uint staticSpriteIndex = atoi( menuCollisionNode.getAttribute( "generateFromStaticSpriteIndex" ) );

            if( staticSpriteIndex < spStaticVector.size() )
                size = spStaticVector[staticSpriteIndex].GetVisualData().GetSize();
        }
        else
        {
            size.w = atof( menuCollisionNode.getAttribute( "width" ) );
            size.h = atof( menuCollisionNode.getAttribute( "height" ) );
        }

        menuCollisionSprite.SetParent( this );
        menuCollisionSprite.Init("gui", size);
    }

}	// LoadMenuParamtersFromNode


/************************************************************************
*    desc:  Load the menu info from file
*  
*    param: std::string filePath - path to file
*
*	 ret:	bool - menu was loaded or not loaded successfully
************************************************************************/
void CUIMenu::LoadFromXML( const string & filePath )
{
    // Open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "menu" );

    // Get the object type string
    const XMLNode listGroupNode = mainNode.getChildNode( "listGroup" );
    objGroupStr = listGroupNode.getAttribute( "name" );

    // Get the static sprite groups
    XMLNode staticSpritesNode = mainNode.getChildNode( "staticSprites" );
    if( !staticSpritesNode.isEmpty() )
    {
        for( int i = 0; i < staticSpritesNode.nChildNode(); ++i )
        {
            XMLNode spriteNode = staticSpritesNode.getChildNode( "sprite", i );

            // Load in the control
            LoadStaticSpriteFromNode( spriteNode );
        }
    }
    
    // Load the menu parameters
    LoadMenuParamtersFromNode( mainNode );

    // Get the menu controls
    XMLNode menuControlsNode = mainNode.getChildNode( "menuControls" );
    if( !menuControlsNode.isEmpty() )
    {
        for( int i = 0; i < menuControlsNode.nChildNode(); ++i )
        {
            // Get the menu controls
            XMLNode focusGroupNode = menuControlsNode.getChildNode( "focusGroup", i );
            if( !focusGroupNode.isEmpty() )
            {
                // Allocate the vector to hold the controls
                spControlVecVec.push_back( new boost::ptr_vector<CUIControl> );

                // Push back the active control counter
                activeControlVec.push_back( NUIControl::NO_ACTIVE_CONTROL );

                for( int j = 0; j < focusGroupNode.nChildNode(); ++j )
                {
                    XMLNode controlNode = focusGroupNode.getChildNode( "control", j );

                    // Load in the control
                    LoadControlFromNode( controlNode );
                }
            }
        }
    }

}	// LoadFromXML


/************************************************************************
*    desc:  Load a control from an XML node
************************************************************************/
void CUIMenu::LoadControlFromNode( XMLNode & node )
{
    // New up the control with its respected control type
    CUIControl * pControl = NUIControlFactory::Create( node );

    // The pointer is placed within a vector for all controls
    spControlVecVec.back().push_back( pControl );

    // Add the pointer to the map but map does not own the pointer
    if( !pControl->GetName().empty() )
        pControlHelperMap.insert( make_pair( pControl->GetName(), pControl ) );

    else if( !pControl->GetExecutionAction().empty() )
        pControlHelperMap.insert( make_pair( pControl->GetExecutionAction(), pControl ) );

}	// LoadControlFromNode


/************************************************************************
*    desc:  Load a static sprite from an XML node
************************************************************************/
void CUIMenu::LoadStaticSpriteFromNode( XMLNode & spriteNode )
{
    // Get the type of object
    string objType = spriteNode.getAttribute( "objectType" );

    // The pointer is placed within a vector for all the static sprite groups
    spStaticVector.push_back( 
        new CSpriteGroup2D( CObjectDataList2D::Instance().GetData( objGroupStr, objType ) ) );

    // Load the transform data
    spStaticVector.back().SetTransData( NParseHelper::LoadTransformData( spriteNode ) );

}	// LoadStaticSpriteFromNode


/************************************************************************
*    desc:  Reset controls to their default state
************************************************************************/
void CUIMenu::RevertToDefault()
{
    for( size_t i = 0; i < spControlVecVec.size(); ++i )
    {
        activeControlVec[i] = NUIControl::NO_ACTIVE_CONTROL;

        for( size_t j = 0; j < spControlVecVec[i].size(); ++j )
            spControlVecVec[i][j].RevertToDefaultState();
    }

}	// RevertToDefault


/************************************************************************
*    desc:  Transform the menu
************************************************************************/
void CUIMenu::Transform()
{
    if( menuCollisionSprite.IsActive() )
        menuCollisionSprite.Transform();

    if( GetParameters().IsSet( CObject::TRANSFORM ) )
    {
        CBitmask tmpTransParam = GetParameters();
        CObject::Transform();

        for( size_t i = 0; i < spStaticVector.size(); ++i )
            spStaticVector[i].SetTransformParameters( tmpTransParam, true );

        for( size_t i = 0; i < spControlVecVec.size(); ++i )
            for( size_t j = 0; j < spControlVecVec[i].size(); ++j )
                spControlVecVec[i][j].SetTransformParameters( tmpTransParam, true );
    }

    // Transform the menu's components
    for( size_t i = 0; i < spStaticVector.size(); ++i )
        spStaticVector[i].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

    for( size_t i = 0; i < spControlVecVec.size(); ++i )
        for( uint j = 0; j < spControlVecVec[i].size(); ++j )
            spControlVecVec[i][j].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

}	// Transform


/************************************************************************
*    desc:  Transform the menu
*
*	 param: const CMatrix & _scaledMatrix  - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point      - passed in relative position
************************************************************************/
void CUIMenu::Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                         const CWorldPoint & point )
{
    if( menuCollisionSprite.IsActive() )
        menuCollisionSprite.Transform();

    if( GetParameters().IsSet( CObject::TRANSFORM ) )
    {
        CBitmask tmpTransParam = GetParameters();
        CObject::Transform( _scaledMatrix, _unscaledMatrix, point );

        for( size_t i = 0; i < spStaticVector.size(); ++i )
            spStaticVector[i].SetTransformParameters( tmpTransParam, true );

        for( size_t i = 0; i < spControlVecVec.size(); ++i )
            for( size_t j = 0; j < spControlVecVec[i].size(); ++j )
            spControlVecVec[i][i].SetTransformParameters( tmpTransParam, true );
    }

    // Transform the menu's components
    for( size_t i = 0; i < spStaticVector.size(); ++i )
        spStaticVector[i].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

    for( size_t i = 0; i < spControlVecVec.size(); ++i )
        for( size_t j = 0; j < spControlVecVec[i].size(); ++j )
            spControlVecVec[i][j].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );
    
}	// Transform


/************************************************************************
*    desc:  the OnToMenu handler
************************************************************************/
void CUIMenu::OnToMenu( NUIMenu::EMenuNavigationState navState )
{
    SetState( NUIMenu::EMS_TRANSITION );

    // Do any menu Smart Display inits
    SmartDisplay();

    // Do any control Smart Display inits
    for( size_t i = 0; i < spControlVecVec.size(); ++i )
        for( size_t j = 0; j < spControlVecVec[i].size(); ++j )
            spControlVecVec[i][j].SmartDisplay();

    if( navState == NUIMenu::EMNS_MOUSE )
        DeactivateControls(true);
    else
        SetToFirstInactiveControl();

}	// OnToMenu


/************************************************************************
*    desc:  Update the menu
************************************************************************/
void CUIMenu::Update()
{
    if( IsStateChange() )
    {
        FinalizeStateChange();

        if( state == NUIMenu::EMS_TRANSITION_OUT )
        {
            Transition( NUIControl::ETS_TRANSITION_OUT, string("transitionOut") );
        }
        else if( state == NUIMenu::EMS_TRANSITION_IN )
        {
            SetVisible();

            Transition( NUIControl::ETS_TRANSITION_IN, string("transitionIn") );
        }
        else if( state == NUIMenu::EMS_HIDDEN )
        {
            SetVisible(false);
        }
    }
    
    // Update all sprites
    for( size_t i = 0; i < spStaticVector.size(); ++i )
        spStaticVector[i].Update();

    // Update all controls
    for( size_t i = 0; i < spControlVecVec.size(); ++i )
        for( size_t j = 0; j < spControlVecVec[i].size(); ++j )
            spControlVecVec[i][j].Update();

}	// Update


/************************************************************************
*    desc:  Make the state change final
************************************************************************/
void CUIMenu::FinalizeStateChange()
{
    lastState = state;

}	// GetLastState


/************************************************************************
*    desc:  Are we in the process of a state change
*  
*    ret: bool - true if state is to be changed
************************************************************************/
bool CUIMenu::IsStateChange()
{
    return (state != lastState);

}	// IsStateChange


/************************************************************************
*    desc:  Are we in the state of hidden?
*  
*    ret: bool - true if state is hidden
************************************************************************/
bool CUIMenu::IsHidden()
{
    return (state == NUIMenu::EMS_HIDDEN);

}	// IsHidden


/************************************************************************
*    desc:  Is the mouse point in sprite
*
*	 ret:	bool - point was in sprite
************************************************************************/
bool CUIMenu::IsPointInSprite()
{
    if( menuCollisionSprite.IsActive() )
        return menuCollisionSprite.IsPointInSprite( CGameController::Instance().GetAbsolutePosScaled() );

    return true;

}	// IsPointInSprite


/************************************************************************
*    desc:  Set each static sprite and control to their transition in 
*			command if they have one.
************************************************************************/
void CUIMenu::Transition( NUIControl::ETransitionState transState, string & transStr )
{
    for( size_t i = 0; i < spStaticVector.size(); ++i )
        spStaticVector[i].SetActiveCommand( transStr );

    for( size_t i = 0; i < spControlVecVec.size(); ++i )
        for( size_t j = 0; j < spControlVecVec[i].size(); ++j )
            spControlVecVec[i][j].SetTransitionState( transState );

}	// TransitionIn


/************************************************************************
*     desc: Handle user menu navigation
*
*	 param: navState - navigation state
************************************************************************/
void CUIMenu::HandleUserNavigation( NUIMenu::EMenuNavigationState navState )
{
    // Update if the menu is active
    if( (state == NUIMenu::EMS_ACTIVE) && IsVisible() )
    {
        // Reset the controls
        Reset();

        // Check for gamepad/keyboard navigation
        if( navState == NUIMenu::EMNS_GAMEPAD_KEYBAORD )
        {
            // Check and see if we should move the focus to the next control
            if( CGameController::Instance().WasAction( nextAction, NDevice::EAP_DOWN ) )
            {
                for( size_t i = 0; i < spControlVecVec[focusGroupIndex].size(); ++i )
                {
                    ++activeControlVec[focusGroupIndex];
                
                    // If the active control is equal to the number of controls, we
                    // loop around to the top of the control list
                    if( activeControlVec[focusGroupIndex] == spControlVecVec[focusGroupIndex].size() )
                        activeControlVec[focusGroupIndex] = 0;

                    // Once we find a button that's not disabled or a label and visible, we've found the new active control
                    if( spControlVecVec[focusGroupIndex][activeControlVec[focusGroupIndex]].IsSelectable( navState ) )
                    {
                        spControlVecVec[focusGroupIndex][activeControlVec[focusGroupIndex]].PlayActiveSound();
                        break;
                    }
                }
            }
            // Check and see if we should move the focus to the previous control
            else if( CGameController::Instance().WasAction( previousAction, NDevice::EAP_DOWN ) )
            {
                for( size_t i = 0; i < spControlVecVec[focusGroupIndex].size(); ++i )
                {
                    --activeControlVec[focusGroupIndex];

                    // If the active control is less than zero, we
                    // loop around to the back of the control list
                    if( activeControlVec[focusGroupIndex] < 0 )
                        activeControlVec[focusGroupIndex] = static_cast<int>(spControlVecVec[focusGroupIndex].size() - 1);

                    // Once we find a button that's not disabled or a label and visible, we've found the new active control
                    if( spControlVecVec[focusGroupIndex][activeControlVec[focusGroupIndex]].IsSelectable( navState ) )
                    {
                        spControlVecVec[focusGroupIndex][activeControlVec[focusGroupIndex]].PlayActiveSound();
                        break;
                    }
                }
            }

            // if we only have one selectable control via keyboard/gamepad keep it active when in this mode
            if( activeControlVec[focusGroupIndex] == NUIControl::NO_ACTIVE_CONTROL )
            {
                int selectableControlCount(0);
                int lastSelectableControl;

                for( size_t i = 0; i < spControlVecVec[focusGroupIndex].size(); ++i )
                {
                    if( spControlVecVec[focusGroupIndex][i].IsSelectable( navState ) )
                    {
                        ++selectableControlCount;
                        lastSelectableControl = static_cast<int>(i);
                    }

                    if( selectableControlCount == 1 )
                    {
                        activeControlVec[focusGroupIndex] = lastSelectableControl;
                        break;
                    }
                }
            }

            // Set the active control to active
            else
            {
                spControlVecVec[focusGroupIndex][activeControlVec[focusGroupIndex]].SetState( NUIControl::ECS_ACTIVE );
                spControlVecVec[focusGroupIndex][activeControlVec[focusGroupIndex]].HandleUserNavigation( navState, nextAction, previousAction );
            }
        }

        // Check and see if the mouse is inside any control
        else if( navState == NUIMenu::EMNS_MOUSE )
        {
            // In mouse nav state, reset the control index
            for( size_t i = 0; i < activeControlVec.size(); ++i )
                activeControlVec[i] = NUIControl::NO_ACTIVE_CONTROL;

            // Is the mouse pointer within the collision rect of the menu
            if( IsPointInSprite() )
            {
                bool controlFound(false);

                // Change the state of the button if the mouse point is within the rect
                for( size_t i = 0; i < spControlVecVec.size() && !controlFound; ++i )
                {
                    for( size_t j = 0; j < spControlVecVec[i].size(); ++j )
                    {
                        // Check and make sure the control isn't disabled or a label and visible, we've found the new active control
                        // Check if the mouse pointer is within this control
                        if( spControlVecVec[i][j].IsSelectable( navState ) && spControlVecVec[i][j].IsPointInSprite() )
                        {
                            // Because each control's state is reset at the top of the function, we must use the
                            // control's last state to determine if we want to play the highlight sound
                            if( spControlVecVec[i][j].GetLastState() == NUIControl::ECS_INACTIVE )
                                spControlVecVec[i][j].PlayActiveSound();

                            spControlVecVec[i][j].SetState( NUIControl::ECS_ACTIVE );

                            spControlVecVec[i][j].HandleUserNavigation( navState, nextAction, previousAction );

                            // Set the index of this control to the active control
                            activeControlVec[i] = static_cast<int>(j);

                            // Set the focus group index
                            focusGroupIndex = static_cast<int>(i);

                            controlFound = true;

                            break;
                        }
                    }
                }
            }
        }

        // Do any smart User Input
        SmartUserInput();
    }

}	// HandleUserNavigation


/************************************************************************
*   desc:  handle user select
*
*	parm: navState - navigation state
************************************************************************/
bool CUIMenu::HandleUserSelect( NUIMenu::EMenuNavigationState navState )
{
    // Only handle select if we meet this criteria
    if( (navState == NUIMenu::EMNS_GAMEPAD_KEYBAORD) || ((navState == NUIMenu::EMNS_MOUSE) && IsPointInSprite()) )
    {
        // Get the active control
        CUIControl * pControl = GetActiveControl();

        // Make sure the active control isn't null
        if( (pControl != NULL) && pControl->HandleUserSelect( navState ) )
            return true;
    }

    return false;

}	// HandleUserSelect


/************************************************************************
*    desc:  Respond to user select
*
*	return: navState - navigation state
************************************************************************/
NUIMenu::EMenuTreeState CUIMenu::RespondToUserSelect()
{
    NUIMenu::EMenuTreeState state(NUIMenu::EMTS_WAIT_SELECTED);

    // Get the active control
    CUIControl * pControl = GetActiveControl();

    if( pControl != NULL )
    {
        // If the menu is transitioning, the active control is selected, and
        // the active control is finished animating, we enter here.
        if( (pControl->GetLastState() == NUIControl::ECS_SELECTED) &&
            !pControl->IsAnimating() )
        {
            // Let the control respond to user selection if it needs to
            pControl->RespondToUserSelect();

            // If the control has a "return" action type, set the menu tree state
            // to transition out and return to the previous menu
            if( pControl->GetActionType() == NUIControl::ECAT_RETURN )
            {
                SetState( NUIMenu::EMS_TRANSITION );
                state = NUIMenu::EMTS_WAIT_TRANSITION_OUT_RETURN;
            }

            // If the control has a "close" action type, set the menu tree state
            // to transition out and close out of the menu system
            else if( pControl->GetActionType() == NUIControl::ECAT_CLOSE )
            {
                SetState( NUIMenu::EMS_TRANSITION );
                state = NUIMenu::EMTS_WAIT_TRANSITION_OUT_TOGGLE;
            }
            
            // If the control has a "to menu" action type, set the menu tree state
            // to transition out and go to a new menu
            else if( pControl->GetActionType() == NUIControl::ECAT_TO_MENU )
            {
                SetState( NUIMenu::EMS_TRANSITION );
                state = NUIMenu::EMTS_WAIT_TRANSITION_OUT_TO_MENU;
            }
            
            // If the control has an "action" action type, exectute the action and
            // set the menu's state to wait for a new input
            else if( pControl->GetActionType() == NUIControl::ECAT_ACTION )
            {
                state = NUIMenu::EMTS_WAIT_INPUT;
            }

            // Deactivate is a way of closing out of the menu system without doing a fade.
            else if( (pControl->GetActionType() == NUIControl::ECAT_DEACTIVATE) ||
                     (pControl->GetActionType() == NUIControl::ECAT_BACK_TO_MAIN_MENU) )
            {
                state = NUIMenu::EMTS_DEACTIVATE;
            }

            // If the control has an "action" action type, exectute the action and
            // set the menu's state to wait for a new input
            else if( pControl->GetActionType() == NUIControl::ECAT_CHANGE_FOCUS )
            {
                // Inc to the next control group if mouse not used
                if( CGameController::Instance().GetLastDevicedUsed() != NDevice::MOUSE )
                    focusGroupIndex = (focusGroupIndex + 1) % spControlVecVec.size();
                    
                state = NUIMenu::EMTS_WAIT_INPUT;
            }

            // Let the control handle the action
            pControl->ExecuteAction();
        }
    }

    return state;

}	// RespondToUserSelect


/************************************************************************
*    desc:  Render the menu
************************************************************************/
void CUIMenu::Render()
{
    if( IsVisible() )
    {
        for( size_t i = 0; i < spStaticVector.size(); ++i )
            spStaticVector[i].Render();

        for( size_t i = 0; i < spControlVecVec.size(); ++i )
            for( size_t j = 0; j < spControlVecVec[i].size(); ++j )
                spControlVecVec[i][j].Render();
    }

}	// Render


/************************************************************************
*    desc:  Reset all controls 
************************************************************************/
void CUIMenu::Reset()
{
    for( size_t i = 0; i < spControlVecVec.size(); ++i )
        for( size_t j = 0; j < spControlVecVec[i].size(); ++j )
            spControlVecVec[i][j].Reset();

}	// Reset


/************************************************************************
*    desc:  Reset the controls' transition state 
************************************************************************/
void CUIMenu::ResetTransitionStates()
{
    for( size_t i = 0; i < spControlVecVec.size(); ++i )
        for( size_t j = 0; j < spControlVecVec[i].size(); ++j )
            spControlVecVec[i][j].ResetTransitionStates();

}	// ResetTransitionStates


/************************************************************************
*    desc:  Deactivates all controls 
************************************************************************/
void CUIMenu::DeactivateControls( bool toMemu )
{
    for( size_t i = 0; i < spControlVecVec.size(); ++i )
    {
        activeControlVec[i] = NUIControl::NO_ACTIVE_CONTROL;

        for( size_t j = 0; j < spControlVecVec[i].size(); ++j )
            spControlVecVec[i][j].DeactivateControls( toMemu );
    }

    // Init to the first control group
    focusGroupIndex = 0;

}	// DeactivateControls


/************************************************************************
*    desc:  Reactivates the active menu control
************************************************************************/
void CUIMenu::ReactivateControl()
{
    // If there's an active control, set it back to active
    SetActiveControlState( NUIControl::ECS_ACTIVE );

}	// ReactivateControl


/************************************************************************
*    desc:  Sets the inactive control with the lowest index in the control
*			vector to active.
*           NOTE: Do Not set the controls state here
************************************************************************/
void CUIMenu::SetToFirstInactiveControl()
{
    Reset();

    for( size_t i = 0; i < spControlVecVec[focusGroupIndex].size(); ++i )
    {
        if( !((spControlVecVec[focusGroupIndex][i].GetState() == NUIControl::ECS_DISABLED) ||
              (spControlVecVec[focusGroupIndex][i].GetType() == NUIControl::ECT_LABEL) ||
              spControlVecVec[focusGroupIndex][i].IsMouseOnlyCtrl() ||
              !spControlVecVec[focusGroupIndex][i].IsVisible() ) )
        {
            activeControlVec[focusGroupIndex] = static_cast<int>(i);

            // If the control is a container for other controls,
            // have it set the first control to active
            spControlVecVec[focusGroupIndex][i].SetToFirstInactiveControl();

            break;
        }
    }

}	// SetToFirstInactiveControl


/************************************************************************
*    desc:  Get the active control
*  
*    ret:	CUIControl * - active control
************************************************************************/
CUIControl * CUIMenu::GetActiveControl()
{
    CUIControl * pControl(NULL);

    if( activeControlVec[focusGroupIndex] != NUIControl::NO_ACTIVE_CONTROL )
        pControl = &spControlVecVec[focusGroupIndex][activeControlVec[focusGroupIndex]];

    return pControl;

}	// GetActiveControl


/************************************************************************
*    desc:  Get the execution action
*  
*    ret:	CUIControl * - active control
************************************************************************/
string CUIMenu::GetExecutionAction()
{
    string result;

    CUIControl * pControl = GetActiveControl();

    if( pControl != NULL )
        result = pControl->GetExecutionAction();

    return result;

}	// GetExecutionAction


/************************************************************************
*    desc:  Set the menu state
*  
*    param: EMenuState value - menu state
************************************************************************/
void CUIMenu::SetState( NUIMenu::EMenuState value )
{
    state = value;

}	// SetState


/************************************************************************
*    desc:  Get the menu state
*  
*    ret: EMenuType - menu state
************************************************************************/
NUIMenu::EMenuState CUIMenu::GetState()
{
    return state;

}	// GetState


/************************************************************************
*    desc:  Set the active control state
*  
*    param: EControlState value - control state
************************************************************************/
void CUIMenu::SetActiveControlState( NUIControl::EControlState value )
{
    if( activeControlVec[focusGroupIndex] != NUIControl::NO_ACTIVE_CONTROL )
        spControlVecVec[focusGroupIndex][activeControlVec[focusGroupIndex]].SetState( value );

}	// SetActiveControlState


/************************************************************************
*    desc:  Is something in the menu animating
*  
*    ret: bool - animating or not
************************************************************************/
bool CUIMenu::IsAnimating()
{
    if( state != NUIMenu::EMS_HIDDEN )
    {
        for( size_t i = 0; i < spStaticVector.size(); ++i )
        {
            if( spStaticVector[i].IsAnimating() && !spStaticVector[i].IsEndModeLoop() )
                return true;
        }

        for( size_t i = 0; i < spControlVecVec.size(); ++i )
        {
            for( size_t j = 0; j < spControlVecVec[i].size(); ++j )
            {
                if( spControlVecVec[i][j].IsAnimating() )
                    return true;
            }
        }
    }

    return false;

}	// IsAnimating


/************************************************************************
*    desc:  Get the pointer to the control in question
*  
*    param: nameStr - name of control to retrieve
************************************************************************/
CUIControl * CUIMenu::GetPtrToControl( const string & nameStr )
{
    // See if this material has already been loaded
    map<string, CUIControl *>::iterator iter = pControlHelperMap.find( nameStr );

    // Check that we have not over run the queue
    if(iter == pControlHelperMap.end() )
        throw NExcept::CCriticalException("UIMenu Control Error!",
                    boost::str( boost::format("Control being asked for is missing!(%s)\n\n%s\nLine: %s") % nameStr.c_str() % __FUNCTION__ % __LINE__ ));

    // Pass back the pointer if found
    return iter->second;

}	// GetPtrToControl


/************************************************************************
*    desc:  Get the name of the menu
*  
*    ret: string
************************************************************************/
const std::string & CUIMenu::GetName() const
{
    return name;

}	// GetName


/************************************************************************
*    desc:  Get Menu size
*  
*    ret: CSize
************************************************************************/
const CSize<float> & CUIMenu::GetSize() const
{
    return size;

}	// GetSize


/************************************************************************
*    desc:  Set the smart menu pointer. This class owns the pointer
************************************************************************/
void CUIMenu::SetSmartGui( CSmartGuiMenu * pSmartGuiMenu )
{
    spSmartGui.reset( pSmartGuiMenu );

}	// SetSmartGui


/************************************************************************
*    desc:  Do any smart create
************************************************************************/
void CUIMenu::SmartCreate()
{
    if( spSmartGui.get() != NULL )
        spSmartGui->Create();

}	// SmartCreate


/************************************************************************
*    desc:  Do any smart Display Init
************************************************************************/
void CUIMenu::SmartDisplay()
{
    if( spSmartGui.get() != NULL )
        spSmartGui->Display();

}	// SmartDisplay


/************************************************************************
*    desc:  Do any smart Display Init
************************************************************************/
void CUIMenu::SmartUserInput()
{
    if( spSmartGui.get() != NULL )
        spSmartGui->HandleUserInput();

}	// SmartUserInput


/************************************************************************
*    desc:  Get the focus group index value
************************************************************************/
int CUIMenu::GetFocusGroupIndex()
{
    return focusGroupIndex;

}	// GetFocusGroupIndex


/************************************************************************
*    desc:  Decrement the focus group index
************************************************************************/
void CUIMenu::DecFocusGroup()
{
    --focusGroupIndex;

}	// DecFocusGroup
