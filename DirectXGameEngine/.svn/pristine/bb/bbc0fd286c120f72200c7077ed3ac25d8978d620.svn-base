
/************************************************************************
*    FILE NAME:       matuvlistmanager.cpp
*
*    DESCRIPTION:     Singleton class to manager the UV values of materials
************************************************************************/

// Physical component dependency
#include "matuvlistmanager.h"

// Standard lib dependencies

// Game lib dependencies

// Boost lib dependencies

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)
// disable warning about the use of the "this" pointer in the constructor list
#pragma warning(disable : 4355)


/************************************************************************
*    desc:  Constructer
************************************************************************/
CMatUVListManager::CMatUVListManager()
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CMatUVListManager::~CMatUVListManager()
{
}	// destructer


/************************************************************************
*    desc:  Get the instance of the CMatUVListManager singleton class                                                             
************************************************************************/
CMatUVListManager & CMatUVListManager::Instance()
{
    static CMatUVListManager uvListManager;

    return uvListManager;

}	// Instance */


/************************************************************************
*    desc:  Load the UV lists from an XML
*
*	 param:	string & filePath - file used to load in the UV lists
*
*	 ret:	bool - whether or not the file was successfully loaded
************************************************************************/
bool CMatUVListManager::LoadFromXML( string & filePath )
{
    // this open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "sectorLayerMaterials" );

    for( int i = 0; i < mainNode.nChildNode(); i++ )
    {
        CMatUVList tmpUVList;

        // Get the name of the material list
        XMLNode matNode = mainNode.getChildNode("material", i);
        string name = matNode.getAttribute("name");

        if( !tmpUVList.LoadFromNode( matNode ) )
            return false;

        // Add the material list to the map
        uvListMap.insert( make_pair( name, tmpUVList ) );
    }

    return true;

}	// LoadFromXML */