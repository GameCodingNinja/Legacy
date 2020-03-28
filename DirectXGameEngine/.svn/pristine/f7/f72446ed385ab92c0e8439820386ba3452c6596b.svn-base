
/************************************************************************
*    FILE NAME:       craftitem.h
*
*    DESCRIPTION:     Class for holding craft item
************************************************************************/

#ifndef __craft_item_h__
#define __craft_item_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <common/defs.h>

// Game dependencies local
#include "craftrecipe.h"

class CCraftItem
{
public:

    CCraftItem():stackSize(0)
    {}

    // Load the data from XML node
    void LoadFromNode( const XMLNode & node )
    {
        stackSize = atoi( node.getAttribute( "stackSize" ) );
        iconIndex = atoi( node.getAttribute( "iconIndex" ) );
        info = node.getChildNode( "info" ).getText();

        const XMLNode recipeNode = node.getChildNode( "recipe" );

        if( !recipeNode.isEmpty() )
            recipe.LoadFromNode( recipeNode );
    }

    // Get functions
    const CCraftRecipe & GetRecipe() const
    { return recipe; }

    const uint GetStackSize() const
    { return stackSize; }

    const uint GetIconIndex() const
    { return iconIndex; }

    const std::string & GetInfo() const
    { return info; }

private:

    // Recipe ingredient and their amounts
    // required to make this craft item
    CCraftRecipe recipe;

    // Number of craft items in a stack
    uint stackSize;

    // index to the icon art
    uint iconIndex;

    // Info on this item
    std::string info;
};

#endif  // __craft_item_h__


