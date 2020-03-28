
/************************************************************************
*    FILE NAME:       craftingredient.h
*
*    DESCRIPTION:     Class for holding craft ingredient
************************************************************************/

#ifndef __craft_ingredient_h__
#define __craft_ingredient_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <common/defs.h>
#include <xmlParser/xmlParser.h>

class CCraftIngredient
{
public:

    CCraftIngredient():amount(0)
    {}

    // Load the data from XML node
    void LoadFromNode( const XMLNode & node )
    {
        name = node.getAttribute( "name" );
        amount = atoi( node.getAttribute( "amount" ) );
    }

    // Get functions
    const std::string & GetName() const
    { return name; }

    const uint GetAmount() const
    { return amount; }

private:

    // Name of ingredient
    std::string name;

    // Number of this ingredient required for recipe
    uint amount;
};


#endif  // __craft_ingredient_h__


