/************************************************************************
*    FILE NAME:       font.cpp
*
*    DESCRIPTION:     font class
************************************************************************/

// Physical component dependency
#include <2d/font.h>

// Game lib dependencies
#include <xmlParser/xmlParser.h>
#include <managers/texturemanager.h>
#include <common/texture.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CFont::CFont()
     : lineHeight(0),
       baselineOffset(0),
       horzPadding(0),
       vertPadding(0),
       pTexture(NULL)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CFont::~CFont()
{
}   // Destructer


/************************************************************************
*    desc:  Load from XML file
*
*    param: String filePath - path to xml file
************************************************************************/
bool CFont::LoadFromXML( const string & group, const string & filePath )
{
    // load the texture - force power of two on this texture
    NText::CTextureFor2D * pTextureFor2D = CTextureMgr::Instance().LoadFor2D( group, filePath + ".png", D3DX_DEFAULT );

    // get the pointer to the texture
    // NOTE: This class doesn't own this pointer
    pTexture = pTextureFor2D->spTexture;

    // open this file and parse
    XMLNode mainNode = XMLNode::openFileHelper( (filePath + ".fnt").c_str(), "font" );

    // Get the padding
    string padding = mainNode.getChildNode( "info" ).getAttribute("padding");
    horzPadding = atof(padding.substr(6,1).c_str());
    vertPadding = atof(padding.substr(0,1).c_str());

    // Get the common font info
    XMLNode commonNode = mainNode.getChildNode( "common" );

    // Get the height and with of the texture
    size.w = atof(commonNode.getAttribute("scaleW"));
    size.h = atof(commonNode.getAttribute("scaleH"));

    // get the line height
    lineHeight = atof(commonNode.getAttribute("lineHeight"));

    // get the baseline offset
    baselineOffset = atof(commonNode.getAttribute("base"));

    // Get the list of character info
    XMLNode charLstNode = mainNode.getChildNode( "chars" );

    // Load in the individual character data
    for( int i = 0; i < charLstNode.nChildNode(); ++i )
    {
        XMLNode charNode = charLstNode.getChildNode( "char", i );

        // Get the character ID which is the ascii value of the character.
        char id = atoi(charNode.getAttribute( "id" ));

        // Add the character to our list
        CCharData * pCharData = new CCharData;
        spCharDataMap.insert( id, pCharData );

        // Get the offset of the character
        pCharData->offset.w = atof(charNode.getAttribute( "xoffset" ));
        pCharData->offset.h = atof(charNode.getAttribute( "yoffset" ));

        // Get the x advance of the character
        pCharData->xAdvance = atof(charNode.getAttribute( "xadvance" ));

        // Get the rect of the character
        pCharData->rect.x1 = atof(charNode.getAttribute( "x" ));
        pCharData->rect.y1 = atof(charNode.getAttribute( "y" ));
        pCharData->rect.x2 = atof(charNode.getAttribute( "width" ));
        pCharData->rect.y2 = atof(charNode.getAttribute( "height" ));
    }

    return true;

}	// LoadFromXML


/************************************************************************
*    desc:  Get the data for this character 
************************************************************************/
const CCharData * CFont::GetCharData( char id )
{
    // See if this character is part of the map
    charDataMapIter = spCharDataMap.find( id );

    if( charDataMapIter != spCharDataMap.end() )
        return charDataMapIter->second;

    return NULL;

}	// GetCharData
