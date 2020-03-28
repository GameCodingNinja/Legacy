/************************************************************************
*    FILE NAME:       stagecharactermap.cpp
*
*    DESCRIPTION:     Loads in the list of character group information
************************************************************************/

// Physical component dependency
#include "stagecharactermap.h"

// DirectX lib dependencies
//#include "d3dx9.h"

// Game lib dependencies
#include "xmlParser\\xmlParser.h"
//#include "xwindow.h"
#include "genfunc.h"
//#include "shader.h"
#include "deletefuncs.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CStageCharacterMap::CStageCharacterMap()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CStageCharacterMap::~CStageCharacterMap()
{
    DeleteMapPointers( environmentMap );
    DeleteMapPointers( specificBattleMap );
    DeleteMapPointers( randomBattleMap );

}   // Destructer


/************************************************************************
*    desc:  Load the character list xml file
*
*    param: String filePath - path to xml file
************************************************************************/
bool CStageCharacterMap::LoadFromXML( string & filePath )
{
    bool result = true;

    // this open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "stageCharacterMap" );

    for( int i = 0; i < mainNode.nChildNode(); i++ )
    {
        XMLNode stagesNode = mainNode.getChildNode(i);

        for( int j = 0; j < stagesNode.nChildNode(); j++ )
        {
            CCharacterList * pTmpList = new CCharacterList();

            XMLNode listNode = stagesNode.getChildNode(j);
            string name = listNode.getAttribute("name");
            string filePath = listNode.getAttribute("filePath");

            pTmpList->LoadFromXML( filePath );

            if ( i == E_ENVIRONMENT )
            {
                environmentMap.insert( make_pair( name, pTmpList ) );
            }
            else if ( i == E_RANDOM_BATTLE )
            {
                randomBattleMap.insert( make_pair( name, pTmpList ) );
            }
            else if ( i == E_SPECIFIC_BATTLE )
            {
                specificBattleMap.insert( make_pair( name, pTmpList ) );
            }
            
        }
    }

    return result;

}	// LoadFromXML


/************************************************************************
*    desc:  Load the character list xml file
*
*    param: String filePath - path to xml file
************************************************************************/
CCharacterList * CStageCharacterMap::GetEnvironmentMapElement( string & name )
{
    return environmentMap[name];
}