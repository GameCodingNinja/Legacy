
/************************************************************************
*    FILE NAME:       craftrecipe.h
*
*    DESCRIPTION:     Class for holding craft recipe
************************************************************************/

#ifndef __craft_recipe_h__
#define __craft_recipe_h__

// Standard lib dependencies
#include <vector>

// Game lib dependencies
#include <common/defs.h>

// Game dependencies local
#include "craftingredient.h"

class CCraftRecipe
{
public:

    CCraftRecipe():amount(0)
    {}

    // Load the data from XML node
    void LoadFromNode( const XMLNode & node )
    {
        amount = atoi( node.getAttribute( "amount" ) );

        for( int i = 0; i < node.nChildNode(); ++i )
        {
            ingredientVec.push_back( CCraftIngredient() );
            ingredientVec.back().LoadFromNode( node.getChildNode( "ingredient", i ) );
        }
    }

    // Get functions
    const std::vector<CCraftIngredient> & GetIngredientVec() const
    { return ingredientVec; }

    const uint GetAmount() const
    { return amount; }

private:

    // ingredients (and their amounts) required to build recipe
    std::vector<CCraftIngredient> ingredientVec;

    // Number of craft items recipe creates
    uint amount;
};


#endif  // __craft_recipe_h__


