/************************************************************************
*    FILE NAME:       characterlist.cpp
*
*    DESCRIPTION:     Loads in the list of character group information
************************************************************************/

// Physical component dependency
#include "characterlist.h"

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
CCharacterList::CCharacterList()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCharacterList::~CCharacterList()
{
    DeleteVectorPointers(list);

}   // Destructer


/************************************************************************
*    desc:  Load the character list xml file
*
*    param: String filePath - path to xml file
************************************************************************/
void CCharacterList::LoadFromXML( string & filePath )
{
    // Character children enum
    enum{ECC_POSITION, ECC_PRETRANSROT};

    // X, Y, and Z attributes enum
    enum{E_X, E_Y, E_Z};

    // this open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "characterGroups" );

    for( int i = 0; i < mainNode.nChildNode(); i++ )
    {
        CCharacterGroup * pTmpGroup = new CCharacterGroup();
        XMLNode groupNode = mainNode.getChildNode(i);

        for( int j = 0; j < groupNode.nChildNode(); j++ )
        {
            CCharacterLoadInfo * pCharacter = new CCharacterLoadInfo();

            XMLNode characterNode = groupNode.getChildNode(j);
            string name = characterNode.getAttribute("name");

            /***********************************************************
            *	positionNode
            *	Has the default position of the characters
            ***********************************************************/
            XMLNode positionNode = characterNode.getChildNode( ECC_POSITION );
            CPoint pos;
            pos.x = atof( positionNode.getAttributeValue( E_X ) );
            pos.y = atof( positionNode.getAttributeValue( E_Y ) );
            pos.z = atof( positionNode.getAttributeValue( E_Z ) );

            /***********************************************************
            *	preTransRotNode
            *	Has the default pre-translation rotation of the character
            ***********************************************************/
            XMLNode preTransRotNode = characterNode.getChildNode( ECC_PRETRANSROT );
            CPoint preTransRot;
            preTransRot.x = atof( preTransRotNode.getAttributeValue( E_X ) );
            preTransRot.y = atof( preTransRotNode.getAttributeValue( E_Y ) );
            preTransRot.z = atof( preTransRotNode.getAttributeValue( E_Z ) );

            pCharacter->SetName( name );
            pCharacter->SetPos( pos );
            pCharacter->SetPreTransRot ( preTransRot );

            pTmpGroup->AddCharacter( pCharacter );
        }

        list.push_back( pTmpGroup );
    }
}


/************************************************************************
*    desc:  Returns the list of character groups
*
*    param: vector<CCharacterGroup *> & list - list of groups
************************************************************************/
vector<CCharacterGroup *> & CCharacterList::GetCharacterGroupList()
{
    return list;
}