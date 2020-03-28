
/************************************************************************
*    FILE NAME:       uihudcontrol.cpp
*
*    DESCRIPTION:     Class for user interface hud controls
************************************************************************/

// Physical component dependency
#include <gui/uihudcontrol.h>

// Game lib dependencies
#include <utilities/xmlparsehelper.h>
#include <misc/settings.h>

typedef CUIControl CParent;

/************************************************************************
*    desc:  Load the control info from XML node
*  
*    param: node - XML node
************************************************************************/
void CUIHudControl::LoadFromNode( const XMLNode & node )
{
    CParent::LoadFromNode( node );

    // Load the dynamic offset
    dynamicOffset = NParseHelper::LoadDynamicOffset( node );

    // Set the dynamic position
    SetDynamicPos();

}	// LoadFromNode


/************************************************************************
*    desc:  Set the dynamic position
************************************************************************/
void CUIHudControl::SetDynamicPos()
{
    if( dynamicOffset.IsDynamicOffset() )
    {
        CPoint pos = GetPos();
        CSize<float> offset = (CSettings::Instance().GetDefaultSizeHalf() - (GetSize() * .5f));

        // Strip out any fractional component for correct rendering
        offset.Round();

        if( dynamicOffset.IsBitSet( CDynamicOffset::EDO_LEFT ) )
            pos.x = -(offset.w - dynamicOffset.GetX());

        else if( dynamicOffset.IsBitSet( CDynamicOffset::EDO_RIGHT ) )
            pos.x = offset.w - dynamicOffset.GetX();

        else if( dynamicOffset.IsBitSet( CDynamicOffset::EDO_HORZ_CENTER ) )
            pos.x = dynamicOffset.GetX();

        if( dynamicOffset.IsBitSet( CDynamicOffset::EDO_TOP ) )
            pos.y = offset.h - dynamicOffset.GetY();
            
        else if( dynamicOffset.IsBitSet( CDynamicOffset::EDO_BOTTOM ) )
            pos.y = -(offset.h - dynamicOffset.GetY());

        else if( dynamicOffset.IsBitSet( CDynamicOffset::EDO_VERT_CENTER ) )
            pos.x = dynamicOffset.GetY();

        SetPos( pos );
    }

}	// SetDynamicPos


/************************************************************************
*    desc:  Is the mouse point insprite
*
*	 ret:	bool - point was in sprite
************************************************************************/
bool CUIHudControl::IsPointInSprite()
{
    // progress bars don't respond to collision

    return false;

}	// IsPointInSprite


/************************************************************************
*    desc:  handle user select
*
*	parm: navState - navigation state
************************************************************************/
bool CUIHudControl::HandleUserSelect( NUIMenu::EMenuNavigationState navState )
{
    // progress bars don't respond to selection

    return false;

}	// HandleUserSelect





