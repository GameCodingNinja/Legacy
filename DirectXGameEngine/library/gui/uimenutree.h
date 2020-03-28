/************************************************************************
*    FILE NAME:       uimenutree.h
*
*    DESCRIPTION:     Class that hold a tree of menus
************************************************************************/

#ifndef __ui_menu_tree_h__
#define __ui_menu_tree_h__

// Standard lib dependencies
#include <string>
#include <vector>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Game lib dependencies
#include <common/matrix.h>
#include <common/worldpoint.h>
#include <gui/uimenudefs.h>

// Forward declaration(s)
class CUIMenu;

class CUIMenuTree
{
public:

    // Get the instance of the singleton class
    static CUIMenuTree & Instance()
    {
        static CUIMenuTree menuTree;
        return menuTree;
    }

    // Load the menu info from XML node
    void LoadFromXML( const std::string & filePath );

    // Reset the dynamic positions of menus
    void ResetDynamicOffset();

    // Navigate the menus
    void Return();
    void ToMenu( const std::string & menuName );

    // Is the menu system visible
    bool IsVisible();

    // Get the user's input
    void HandleUserInput();

    // Determine the user's input
    void DetermineUserInput();

    // Respond the menu tree actions depending on its state
    void RespondToUserInput();

    // Update the menu tree
    void Update();

    // Transform the menu tree's components
    virtual void Transform();
    virtual void Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                            const CWorldPoint & point );

    // Do the render
    void Render();

    // Activate a menu
    void Activate();
    void Activate( const std::string & menuName );

    // Is this menu active?
    bool IsActive( const std::string & menuName );

    // Have the menu return
    void SetMenuReturn( const std::string & menuName );

    // Deactivate the menus
    void Deactivate();

    // Transition between menus
    void TransitionMenus();

    // Get the state of the menu tree
    NUIMenu::EMenuTreeState GetState();

    // Get the pointer to the menu in question
    CUIMenu * GetPtrToMenu( const std::string & nameStr );

    // Get the pointer to the active menu
    CUIMenu * GetPtrToActiveMenu();

    // Set/Get the disable All Except Esc
    void SetDisableAllExceptEsc( bool value ){ disableAllExceptEsc = value; }
    bool GetDisableAllExceptEsc(){ return disableAllExceptEsc; }

    // Act as if the escape action was pressed
    void SimulateEscapeActionOutOfMenu();

    // Set the root menu
    void SetRootMenu( const std::string name ){ rootMenu = name; }

    // Evaluate the navigation state
    void EvaluateNavState();

    const std::string & GetReturnAction(){return returnAction;}
    const std::string & GetToggleAction(){return toggleAction;}
    const std::string & GetEscapeAction(){return escapeAction;}

private:

    CUIMenuTree();
    ~CUIMenuTree();

    // Set the state
    void SetState( NUIMenu::EMenuTreeState value );

private:

    // Name of the root menu
    std::string rootMenu;

    // Map of the menus
    boost::ptr_map<std::string, CUIMenu> spMenuMap;
    boost::ptr_map<std::string, CUIMenu>::iterator menuMapIter;

    // Vector of the path taken through the menu
    std::vector<std::string> pathVector;

    // Actions to progress forward and backward through the tree
    std::string returnAction;
    std::string toggleAction;
    std::string escapeAction;

    // Menu tree state
    NUIMenu::EMenuTreeState state;

    // Menu navigation state
    NUIMenu::EMenuNavigationState navState;

    // Flag to disable all menu buttons
    bool disableAllExceptEsc;
};

#endif  // __ui_menu_tree_h__