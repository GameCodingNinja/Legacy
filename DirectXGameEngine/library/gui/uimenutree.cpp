/************************************************************************
*    FILE NAME:       uimenutree.cpp
*
*    DESCRIPTION:     Class to manage the menus
************************************************************************/

// Physical component dependency
#include <gui/uimenutree.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <controller/gamecontroller.h>
#include <xmlParser/xmlParser.h>
#include <gui/controlevent.h>
#include <gui/uimenu.h>
#include <gui/uicontrol.h>
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <xact/xact.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CUIMenuTree::CUIMenuTree()
           : state(NUIMenu::EMTS_HIDDEN),
             navState(NUIMenu::EMNS_GAMEPAD_KEYBAORD),
             disableAllExceptEsc(false)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CUIMenuTree::~CUIMenuTree()
{
}   // Destructer


/************************************************************************
*    desc:  Load the sprite info from file
*  
*    param: std::string filePath - path to file
************************************************************************/
void CUIMenuTree::LoadFromXML( const string & filePath )
{
    // this open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "menuManager" );

    XMLNode treeNode = mainNode.getChildNode( "menus" );

    rootMenu = treeNode.getAttribute( "root" );
    returnAction = treeNode.getAttribute( "returnAction" );
    toggleAction = treeNode.getAttribute( "toggleAction" );
    escapeAction = treeNode.getAttribute( "escapeAction" );

    for( int i = 0; i < treeNode.nChildNode(); ++i )
    {
        //////////////////////////////////////////////
        // MENU NAME AND FILE PATH 
        //////////////////////////////////////////////
        XMLNode menuNode = treeNode.getChildNode( "menu", i );

        // Get the name of the menu
        string name = menuNode.getAttribute( "name" );

        // Insert the menu into the menu map
        CUIMenu * pMenu = new CUIMenu();
        spMenuMap.insert( name, pMenu );

        // Have the menu load it's share
        pMenu->LoadFromNode( menuNode );

        // Dispatch a call back to let the game handle smart gui inits
        CSmartGuiCreateEvent smartGuiCreateEventEvent;
        smartGuiCreateEventEvent.Dispatch( pMenu );
    }

}	// LoadFromXML


/************************************************************************
*    desc:  Reset the dynamic positions of menus
************************************************************************/
void CUIMenuTree::ResetDynamicOffset()
{
    for( menuMapIter = spMenuMap.begin(); 
         menuMapIter != spMenuMap.end();
         ++menuMapIter )
    {
        menuMapIter->second->SetDynamicPos();
    }

}	// ResetDynamicOffset


/************************************************************************
*    desc:  Go to the last menu
************************************************************************/
void CUIMenuTree::Return()
{
    if( !pathVector.empty() )
    {
        // Get an iterator of the active menu
        boost::ptr_map<std::string, CUIMenu>::iterator prevMenuIter = spMenuMap.find( pathVector.back() );

        // Set the menu being exited to have no active control
        prevMenuIter->second->DeactivateControls();

        // Hide the menu being exited
        prevMenuIter->second->SetState( NUIMenu::EMS_HIDDEN );

        pathVector.pop_back();

        if( !pathVector.empty() )
        {
            boost::ptr_map<std::string, CUIMenu>::iterator activeMenuIter = spMenuMap.find( pathVector.back() );

            // Deactivae if using a mouse
            if( navState == NUIMenu::EMNS_MOUSE )
                activeMenuIter->second->DeactivateControls();

            // If there's an active control on the active menu, set it back to active
            else
                activeMenuIter->second->ReactivateControl();

            activeMenuIter->second->SetState( NUIMenu::EMS_TRANSITION );
        }
        else
        {
            Deactivate();
            state = NUIMenu::EMTS_HIDDEN;
        }
    }

}	// Return


/************************************************************************
*    desc:  Go to the child menu
************************************************************************/
void CUIMenuTree::ToMenu( const string & menuName )
{
    // Reset any action owners
    CGameController::Instance().SetActionOwner(NULL);

    menuMapIter = spMenuMap.find( menuName );

    // Find the desired menu
    if( menuMapIter != spMenuMap.end() )
    {
        if( !pathVector.empty() )
            spMenuMap[pathVector.back()].SetState( NUIMenu::EMS_HIDDEN );
    
        pathVector.push_back( menuName );

        menuMapIter->second->OnToMenu( navState );

        state = NUIMenu::EMTS_WAIT_TRANSITION_IN;
    }
    else
        throw NExcept::CCriticalException("UIMenu Error!",
                boost::str( boost::format("Trying to access a menu that doesn't exist (%s).\n\n%s\nLine: %s") % menuName.c_str() % __FUNCTION__ % __LINE__ ));

}	// ToMenu


/************************************************************************
*    desc:  Get the user's input
************************************************************************/
void CUIMenuTree::HandleUserInput()
{
    if( state != NUIMenu::EMTS_HIDDEN )
    {
        // Set the nav state
        EvaluateNavState();

        // Determine the user's input
        DetermineUserInput();

        // Respond the menu tree actions depending on its state
        RespondToUserInput();
    }

}	// HandleUserInput


/************************************************************************
*    desc:  Is the menu system visible
************************************************************************/
bool CUIMenuTree::IsVisible()
{
    return (state != NUIMenu::EMTS_HIDDEN);
}


/************************************************************************
*    desc:  Determine the user's input
************************************************************************/
void CUIMenuTree::DetermineUserInput()
{
    if( state == NUIMenu::EMTS_WAIT_INPUT )
    {
        // Should never be a NULL string
        if( pathVector.empty() || pathVector.back().empty() )
            throw NExcept::CCriticalException("UIMenu Error!",
                boost::str( boost::format("NULL string used to access a menu.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

        // Get the menu from the map
        CUIMenu & uiMenu = spMenuMap[pathVector.back()];

        // Make sure the menu is active
        if( uiMenu.GetState() == NUIMenu::EMS_ACTIVE )
        {
            // Allow the menu to handle user navigation.
            uiMenu.HandleUserNavigation( navState );

            // Handle any selection that may have happened
            if( !disableAllExceptEsc && uiMenu.HandleUserSelect( navState ) )
            {
                state = NUIMenu::EMTS_WAIT_SELECTED;
            }
            // If a return action was called, return to the previous menu or deactivate the menu
            else if( !disableAllExceptEsc && CGameController::Instance().WasAction( returnAction, NDevice::EAP_DOWN ) )
            {
                if( !pathVector.empty() && pathVector.back() != rootMenu )
                {
                    state = NUIMenu::EMTS_WAIT_TRANSITION_OUT_RETURN;
                    uiMenu.SetState( NUIMenu::EMS_TRANSITION );
                }
            }
            // If the toggle menu action was called, toggle the menues off
            else if( !disableAllExceptEsc && CGameController::Instance().WasAction( toggleAction, NDevice::EAP_DOWN ) )
            {
                if( !pathVector.empty() && pathVector[0] != rootMenu )
                {
                    state = NUIMenu::EMTS_WAIT_TRANSITION_OUT_TOGGLE;
                    uiMenu.SetState( NUIMenu::EMS_TRANSITION );
                }
            }
            // If the escape menu action was called, either move back a menu or deactivate the menus
            else if( CGameController::Instance().WasAction( escapeAction, NDevice::EAP_DOWN ) )
            {
                if( !pathVector.empty() && (pathVector.back() != rootMenu) )
                {
                    if( uiMenu.GetFocusGroupIndex() == 0 )
                    {
                        state = NUIMenu::EMTS_WAIT_TRANSITION_OUT_RETURN;
                        uiMenu.SetState( NUIMenu::EMS_TRANSITION );
                    }
                    else
                    {
                        uiMenu.DecFocusGroup();
                    }
                }
            }
        }
    }

}	// DetermineUserInput /*


/************************************************************************
*    desc:  Respond the menu tree actions depending on its state
************************************************************************/
void CUIMenuTree::RespondToUserInput()
{
    if( state == NUIMenu::EMTS_WAIT_SELECTED )
    {
        // Get an iterator of the active menu
        boost::ptr_map<std::string, CUIMenu>::iterator activeMenuIter = spMenuMap.find( pathVector.back() );

        // get the response state
        state = activeMenuIter->second->RespondToUserSelect();

        // Don't allow a menu toggle if the last menu is the root menu
        if( (state == NUIMenu::EMTS_WAIT_TRANSITION_OUT_TOGGLE) && !pathVector.empty() && pathVector.front() == rootMenu )
            state = NUIMenu::EMTS_WAIT_TRANSITION_OUT_RETURN;

        // Deactivate the whole menu system
        else if( state == NUIMenu::EMTS_DEACTIVATE )
            Deactivate();
    }
    // If the tree is waiting for a menu to transition out to another menu, we enter here
    else if( state == NUIMenu::EMTS_WAIT_TRANSITION_OUT_TO_MENU )
    {
        // Get an iterator of the active menu
        boost::ptr_map<std::string, CUIMenu>::iterator activeMenuIter = spMenuMap.find( pathVector.back() );

        // If the menu is in the transition state, we set it to the transition out state.
        // This is so we only go in here once.
        if( activeMenuIter->second->GetState() == NUIMenu::EMS_TRANSITION )
        {
            activeMenuIter->second->SetState( NUIMenu::EMS_TRANSITION_OUT );
        }
        // If the menu has finished its transition animation, we go in here
        else if( activeMenuIter->second->GetState() == NUIMenu::EMS_TRANSITION_OUT && !activeMenuIter->second->IsAnimating() )
        {
            activeMenuIter->second->ResetTransitionStates();

            string executionAction = activeMenuIter->second->GetExecutionAction();

            if( !executionAction.empty() )
            {
                ToMenu( executionAction );
                state = NUIMenu::EMTS_WAIT_TRANSITION_IN;
            }
            else
            {
                state = NUIMenu::EMTS_WAIT_TRANSITION_OUT_TOGGLE;
                activeMenuIter->second->SetState( NUIMenu::EMS_TRANSITION );
            }
        }
    }
    // If the tree is waiting to transition to a previous menu, we enter here
    else if( state == NUIMenu::EMTS_WAIT_TRANSITION_OUT_RETURN )
    {
        disableAllExceptEsc = false;

        // If there's more than one menu in the path vector, then that means we're
        // returning to a previous menu
        if( pathVector.size() > 1 )
        {
            // Get an iterator of the active menu
            boost::ptr_map<std::string, CUIMenu>::iterator activeMenuIter = spMenuMap.find( pathVector.back() );

            // If the menu is in the transition state, we set it to the transition out state.
            // This is so we only go in here once.
            if( activeMenuIter->second->GetState() == NUIMenu::EMS_TRANSITION )
                activeMenuIter->second->SetState( NUIMenu::EMS_TRANSITION_OUT );
        
            // If the menu has finished its transition animation, we go in here
            else if( activeMenuIter->second->GetState() == NUIMenu::EMS_TRANSITION_OUT && !activeMenuIter->second->IsAnimating() )
            {
                // Reset the controls' transition states
                activeMenuIter->second->ResetTransitionStates();

                // Return to the previous menu.
                // This function call does a pathVector pop_back
                Return();

                // Set the state to wait for the previous menu to transition in
                state = NUIMenu::EMTS_WAIT_TRANSITION_IN;
            }
        }
        // If there is only one menu in the path vector, switch to the transition out toggle
        // state to exit the menus
        else
            state = NUIMenu::EMTS_WAIT_TRANSITION_OUT_TOGGLE;

    }
    else if( state == NUIMenu::EMTS_WAIT_TRANSITION_OUT_TOGGLE )
    {
        // Get an iterator of the active menu
        boost::ptr_map<std::string, CUIMenu>::iterator activeMenuIter = spMenuMap.find( pathVector.back() );

        // If the menu is in the transition state, we set it to the transition out state.
        // This is so we only go in here once.
        if( activeMenuIter->second->GetState() == NUIMenu::EMS_TRANSITION )
            activeMenuIter->second->SetState( NUIMenu::EMS_TRANSITION_OUT );
        
        // If the menu has finished its transition animation, we go in here
        else if( activeMenuIter->second->GetState() == NUIMenu::EMS_TRANSITION_OUT && !activeMenuIter->second->IsAnimating() )
        {
            activeMenuIter->second->ResetTransitionStates();
            Deactivate();
        }
    }
    else if( state == NUIMenu::EMTS_WAIT_TRANSITION_IN )
    {
        // Get an iterator of the active menu
        boost::ptr_map<std::string, CUIMenu>::iterator activeMenuIter = spMenuMap.find( pathVector.back() );

        if( activeMenuIter->second->GetState() == NUIMenu::EMS_TRANSITION )
            activeMenuIter->second->SetState( NUIMenu::EMS_TRANSITION_IN );
        
        else if( activeMenuIter->second->GetState() == NUIMenu::EMS_TRANSITION_IN && !activeMenuIter->second->IsAnimating() )
        {
            activeMenuIter->second->ResetTransitionStates();

            activeMenuIter->second->SetState( NUIMenu::EMS_ACTIVE );
            state = NUIMenu::EMTS_WAIT_INPUT;
        }
    }

}	// RespondToUserInput


/************************************************************************
*    desc:  Act as if the escape action was pressed
************************************************************************/
void CUIMenuTree::SimulateEscapeActionOutOfMenu()
{
    if( !pathVector.empty() && (state == NUIMenu::EMTS_WAIT_INPUT) )
    {
        menuMapIter = spMenuMap.find( pathVector.back() );

        if( menuMapIter != spMenuMap.end() )
        {
            state = NUIMenu::EMTS_WAIT_TRANSITION_OUT_RETURN;
            menuMapIter->second->SetState( NUIMenu::EMS_TRANSITION );
        }
        else
        {
            throw NExcept::CCriticalException("UIMenu Error!",
                    boost::str( boost::format("Trying to set the state of a menu that doesn't exist (%s).\n\n%s\nLine: %s") 
                                % pathVector.back().c_str() % __FUNCTION__ % __LINE__ ));
        }
    }
}	// SimulateEscapeActionOutOfMenu


/************************************************************************
*    desc:  Update the menus
************************************************************************/
void CUIMenuTree::Update()
{
    for( menuMapIter = spMenuMap.begin(); 
         menuMapIter != spMenuMap.end();
         ++menuMapIter )
    {
        if( !menuMapIter->second->IsHidden() ||
            menuMapIter->second->IsStateChange() )
        {
            menuMapIter->second->Update();
        }
    }

}	// Update


/************************************************************************
*    desc:  Transform the menu tree's visual components
************************************************************************/
void CUIMenuTree::Transform()
{
    for( menuMapIter = spMenuMap.begin(); 
         menuMapIter != spMenuMap.end(); 
         ++menuMapIter )
    {
        menuMapIter->second->Transform();
    }
    
}	// Transform


/************************************************************************
*    desc:  Transform the menu tree's visual components
*
*	 param: const CMatrix & _scaledMatrix  - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point      - passed in relative position
************************************************************************/
void CUIMenuTree::Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                             const CWorldPoint & point )
{
    for( menuMapIter = spMenuMap.begin(); 
         menuMapIter != spMenuMap.end(); 
         ++menuMapIter )
    {
        menuMapIter->second->Transform( _scaledMatrix, _unscaledMatrix, point );
    }
    
}	// Transform


/************************************************************************
*    desc:  Render the menu
************************************************************************/
void CUIMenuTree::Render()
{
    // Render Tree
    if( state != NUIMenu::EMTS_HIDDEN )
        for( size_t i = 0; i < pathVector.size(); ++i )
        {
            menuMapIter = spMenuMap.find( pathVector[i] );

            if( menuMapIter != spMenuMap.end() )
                menuMapIter->second->Render();
            else
                throw NExcept::CCriticalException("UIMenu Error!",
                        boost::str( boost::format("Trying to render a menu that doesn't exist (%s).\n\n%s\nLine: %s") 
                                    % pathVector[i].c_str() % __FUNCTION__ % __LINE__ ));
        }

}	// Render


/************************************************************************
*    desc:  Activate a menu
************************************************************************/
void CUIMenuTree::Activate()
{
    Activate( rootMenu );

}	// Activate


/************************************************************************
*    desc:  Activate a menu
*  
*    param: string & name - menu to set active
************************************************************************/
void CUIMenuTree::Activate( const string & menuName )
{
    menuMapIter = spMenuMap.find( menuName );

    // Find the desired menu
    if( menuMapIter != spMenuMap.end() )
    {
        if( state == NUIMenu::EMTS_HIDDEN )
        {
            state = NUIMenu::EMTS_WAIT_TRANSITION_IN;

            menuMapIter->second->SetState( NUIMenu::EMS_TRANSITION );
            pathVector.push_back( menuName );
        }
    }

}	// Activate


/************************************************************************
*    desc:  Is this menu active?
*  
*    param: bool
************************************************************************/
bool CUIMenuTree::IsActive( const string & menuName )
{
    menuMapIter = spMenuMap.find( menuName );

    // Find the desired menu
    if( menuMapIter != spMenuMap.end() )
        if( menuMapIter->second->GetState() == NUIMenu::EMS_ACTIVE )
            return true;

    return false;

}	// IsActive


/************************************************************************
*    desc:  Have the menu return
*  
*    param: bool
************************************************************************/
void CUIMenuTree::SetMenuReturn( const string & menuName )
{
    menuMapIter = spMenuMap.find( menuName );

    // Find the desired menu
    if( menuMapIter != spMenuMap.end() )
    {
        if( menuMapIter->second->GetState() == NUIMenu::EMS_ACTIVE )
        {
            menuMapIter->second->SetState( NUIMenu::EMS_TRANSITION );
            state = NUIMenu::EMTS_WAIT_TRANSITION_OUT_RETURN;
        }
    }

}	// SetMenuReturn


/************************************************************************
*    desc:  Deactivate all menus
************************************************************************/
void CUIMenuTree::Deactivate()
{
    state = NUIMenu::EMTS_HIDDEN;

    // Hide all the menues in the tree
    for( menuMapIter = spMenuMap.begin(); 
         menuMapIter != spMenuMap.end();
         ++menuMapIter )
    {
        menuMapIter->second->DeactivateControls();
        menuMapIter->second->SetState( NUIMenu::EMS_HIDDEN );
    }

    // Clear the path vector
    pathVector.clear();

}	// Deactivate


/************************************************************************
*    desc:  Get the state of the menu tree
*  
*    ret: EMenuTreeState - state of the menu tree
************************************************************************/
NUIMenu::EMenuTreeState CUIMenuTree::GetState()
{
    return state;

}	// GetState


/************************************************************************
*    desc:  Get the pointer to the menu in question
*  
*    param: nameStr - name of menu to retrieve
************************************************************************/
CUIMenu * CUIMenuTree::GetPtrToMenu( const string & nameStr )
{
    // Get a pointer to the menu
    menuMapIter = spMenuMap.find( nameStr );

    // Pass back the pointer if found
    if( menuMapIter != spMenuMap.end() )
        return menuMapIter->second;

    throw NExcept::CCriticalException("UIMenu Error!",
        boost::str( boost::format("Menu being asked for is missing (%s).\n\n%s\nLine: %s") % nameStr.c_str() % __FUNCTION__ % __LINE__ ));

    return NULL;

}	// GetPtrToMenu


/************************************************************************
*    desc:  Get the pointer to the active menu
*  
*    param: nameStr - name of menu to retrieve
************************************************************************/
CUIMenu * CUIMenuTree::GetPtrToActiveMenu()
{
    if( !pathVector.empty() )
    {
        // Get a pointer to the menu
        menuMapIter = spMenuMap.find( pathVector.back() );

        // Pass back the pointer if found
        if( menuMapIter != spMenuMap.end() )
            return menuMapIter->second;
    }

    throw NExcept::CCriticalException("UIMenu Error!",
        boost::str( boost::format("Menu being asked for is missing (%s).\n\n%s\nLine: %s") % pathVector.back().c_str() % __FUNCTION__ % __LINE__ ));

    return NULL;

}	// GetPtrToMenu


/************************************************************************
*    desc:  Evaluate the navigation state
************************************************************************/
void CUIMenuTree::EvaluateNavState()
{
    if( !pathVector.empty() )
    {
        menuMapIter = spMenuMap.find( pathVector.back() );

        if( menuMapIter != spMenuMap.end() )
        {
            // Set the nav state
            if( CGameController::Instance().GetLastDevicedUsed() == NDevice::MOUSE )
                navState = NUIMenu::EMNS_MOUSE;
            else
                navState = NUIMenu::EMNS_GAMEPAD_KEYBAORD;
        }
    }

}	// EvaluateNavState