
/************************************************************************
*    FILE NAME:       craftitemlist.cpp
*
*    DESCRIPTION:     Class for loading craft data
************************************************************************/

// Physical component dependency
#include "craftitemlist.h"

// Game lib dependencies
#include <xmlParser/xmlParser.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Load all object information from an xml
*
*	 param:	string & filePath - file path of the object data list XML
************************************************************************/
void CCraftItemList::LoadFromXML( const string & filePath )
{
    // open and parse the XML file:
    const XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "craftItemList" );

    for( int i = 0; i < mainNode.nChildNode(); ++i )
    {
        const XMLNode craftItemNode = mainNode.getChildNode( "craftItem", i );

        // Place the object in the map
        CCraftItem * pCraftItem = new CCraftItem();

        // Add the craft item to the map
        spCraftItemMap.insert( craftItemNode.getAttribute( "name" ), pCraftItem );

        // Load from xml node
        pCraftItem->LoadFromNode( craftItemNode );
    }

}	// LoadFromXML







