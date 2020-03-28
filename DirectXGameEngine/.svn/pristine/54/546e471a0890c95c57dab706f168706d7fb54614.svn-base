/************************************************************************
*    FILE NAME:       uimenu.h
*
*    DESCRIPTION:     Class for DirectX user interface menu
************************************************************************/

#ifndef __ui_menu_h__
#define __ui_menu_h__

// Physical component dependency
#include <common/object.h>

// Standard lib dependencies
#include <string>
#include <map>

// Boost lib dependencies
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/scoped_ptr.hpp>

// Game lib dependencies
#include <common/defs.h>
#include <common/dynamicoffset.h>
#include <xmlParser/xmlParser.h>
#include <gui/uimenudefs.h>
#include <gui/uicontroldefs.h>
#include <2d/collisionsprite2d.h>

// Forward declaration(s)
class CSpriteGroup2D;
class CUIControl;
class CSmartGuiMenu;

class CUIMenu : public CObject
{
public:

    CUIMenu();
    virtual ~CUIMenu();

    // Load the menu info from XML node
    void LoadFromNode( XMLNode & node );

    // Reset controls to their default state
    void RevertToDefault();

    // Reset the controls to inactive if they're not disabled
    void Reset();

    // Reset the control and sprite's transition state
    void ResetTransitionStates();

    // Transform the menu
    void Transform();
    void Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                    const CWorldPoint & point );

    // the OnToMenu handler
    void OnToMenu( NUIMenu::EMenuNavigationState navState );

    // Do the render
    void Render();

    // Update the menu
    virtual void Update();

    // Handle user menu navigation
    void HandleUserNavigation( NUIMenu::EMenuNavigationState navState );

    // Handle user select
    bool HandleUserSelect( NUIMenu::EMenuNavigationState navState );

    // Respond to user select
    NUIMenu::EMenuTreeState RespondToUserSelect();

    // Gets the active control
    CUIControl * GetActiveControl();

    // Deactivates all controls 
    void DeactivateControls( bool toMemu = false );

    // Reactivates the active menu control
    void ReactivateControl();

    // Get the execution action
    std::string GetExecutionAction();

    // Sets the inactive control with the lowest index in the control
    // vector to active
    void SetToFirstInactiveControl();

    // Set or get the state of the menu
    void SetState( NUIMenu::EMenuState value );
    NUIMenu::EMenuState GetState();

    // Are we in the process of a state change
    bool IsStateChange();

    // Make the state change final
    void FinalizeStateChange();

    // Are we in the state of hidden?
    bool IsHidden();

    // Set the state of the active control
    void SetActiveControlState( NUIControl::EControlState value );

    // Is something in the menu animating
    bool IsAnimating();

    // Get the pointer to the control in question
    CUIControl * GetPtrToControl( const std::string & nameStr );

    // Get the name of the menu
    const std::string & GetName() const;

    // Get Menu size
    const CSize<float> & GetSize() const;

    // Get the string for next and previous action
    std::string & GetNextAction(){ return nextAction; }
    std::string & GetPreviousAction(){ return previousAction; }

    // Set the smart menu pointer. This class owns the pointer
    void SetSmartGui( CSmartGuiMenu * pSmartGuiMenu );

    // Do any smart create
    void SmartCreate();

    // Do any smart Display
    void SmartDisplay();

    // Do any smart User Input
    void SmartUserInput();

    // Get the smart gui pointer
    CSmartGuiMenu * GetSmartGuiPtr(){ return spSmartGui.get(); }

    // Is the mouse point in sprite
    bool IsPointInSprite();

    // Set the dynamic position
    void SetDynamicPos();

    // Get the focus group index value
    int GetFocusGroupIndex();

    // Decrement the focus group index
    void DecFocusGroup();

private:

    // Load the menu info from XML node
    void LoadFromXML( const std::string & filePath );

    // Load the menu's XML parameters
    void LoadMenuParamtersFromNode( XMLNode & node );
    void LoadControlFromNode( XMLNode & controlNode );
    void LoadStaticSpriteFromNode( XMLNode & spriteNode );

    // Set each static sprite and control to their transition in command if they have one.
    void Transition( NUIControl::ETransitionState transState, std::string & transStr );

private:

    // Vector list of controls
    boost::ptr_vector< boost::ptr_vector<CUIControl> > spControlVecVec;

    // Additional group sprites that don't do anything
    boost::ptr_vector<CSpriteGroup2D> spStaticVector;

    // Map container of controls for easy name access
    // NOTE: controlMap does not own it's pointers.
    std::map<std::string, CUIControl *> pControlHelperMap;

    // Menu collision sprite
    CCollisionSprite2D menuCollisionSprite;

    // Menu navigation actions
    std::string nextAction;
    std::string previousAction;

    // Active control index vector
    std::vector<int> activeControlVec;

    // The state and last state of the menu
    NUIMenu::EMenuState state;
    NUIMenu::EMenuState lastState;

    // This menu's name
    std::string name;

    // This menu's size
    CSize<float> size;

    // Object group name
    std::string objGroupStr;

    // Base control action class scoped pointer
    boost::scoped_ptr<CSmartGuiMenu> spSmartGui;

    // Dynamic offset
    CDynamicOffset dynamicOffset;

    // focus group index
    int focusGroupIndex;

};

#endif  // __ui_menu_h__