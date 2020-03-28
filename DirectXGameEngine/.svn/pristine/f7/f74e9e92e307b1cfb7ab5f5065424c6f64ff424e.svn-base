/************************************************************************
*    FILE NAME:       uibuttonlist.h
*
*    DESCRIPTION:     Class for user interface buttons
************************************************************************/

#ifndef __ui_button_list_h__
#define __ui_button_list_h__

// Physical component dependency
#include <gui/uisubcontrol.h>

// Boost lib dependencies
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/scoped_ptr.hpp>

// Game lib dependencies
#include <common/defs.h>

// Forward declaration(s)
class CSpriteGroup2D;

class CUIButtonList : public CUISubControl
{
public:
    
    CUIButtonList();
    virtual ~CUIButtonList();

    // Load the initial info from XML node
    virtual void LoadFromNode( const XMLNode & node );

    // Act in accordance to user input
    virtual void HandleUserNavigation( NUIMenu::EMenuNavigationState navState,
                                       const std::string & nextAction,
                                       const std::string & previousAction );

    // Set the string index
    void SetActiveIndex( uint index );
    uint GetActiveIndex(){ return activeIndex; }

    // Inc/Dec the button display
    virtual void IncDisplay();
    virtual void DecDisplay();

    // Execute the action
    virtual void ExecuteAction();

    // handle user select
    virtual bool HandleUserSelect( NUIMenu::EMenuNavigationState navState );

    // Get the state of this control
    virtual NUIControl::EControlState GetState();

    // Transform the control
    void Transform();
    void Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                    const CWorldPoint & point );

    // Update the control
    virtual void Update();

    // Render the control
    virtual void Render();

protected:

    // Was there a left/right select
    bool WasDecIncSelect( bool decAction, bool incAction );

    // Test for user selection
    virtual bool CheckForUserSelection( NUIMenu::EMenuNavigationState navState );

    // Set the active command string
    virtual void SetActiveCommand( const std::string & commandStr );

private:

    // Control selection actions
    std::string decSelectAction;
    std::string incSelectAction;

    // Active index into the list
    int activeIndex;

    // Sprite group of images
    boost::scoped_ptr<CSpriteGroup2D> scpImageLst;

};

#endif  // __ui_button_list_h__