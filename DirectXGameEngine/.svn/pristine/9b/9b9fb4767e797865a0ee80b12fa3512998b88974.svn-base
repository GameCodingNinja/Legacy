
/************************************************************************
*    FILE NAME:       fontmanager.cpp
*
*    DESCRIPTION:     font manager class singleton
************************************************************************/

// Physical component dependency
#include <2d/fontmanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <system/xdevice.h>
#include <managers/shader.h>
#include <2d/font.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CFontMgr::CFontMgr()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CFontMgr::~CFontMgr()
{
}   // Destructer


/************************************************************************
*    desc:  Load the material from file path
*  
*    param: string & filePath - path to material file
* 
*    return: bool - false on fail
************************************************************************/
void CFontMgr::LoadFromXML( const string & filePath )
{
    // open this file and parse
    const XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "fontLst" );

    // Get the object type string
    const XMLNode listGroupNode = mainNode.getChildNode( "listGroup" );
    const string objGroupStr = listGroupNode.getAttribute( "name" );

    // Get the list of character info
    XMLNode fontLstNode = mainNode.getChildNode( "fonts" );

    for( int i = 0; i < fontLstNode.nChildNode(); ++i )
    {
        const XMLNode fontNode = fontLstNode.getChildNode( "font", i );

        // Get the name of the font
        const string name = fontNode.getAttribute( "name" );

        CFont * pFont = new CFont;

        // Add the fontr to our list
        spFontMap.insert( name, pFont );

        // Load the character info from file
        pFont->LoadFromXML( objGroupStr, string(fontNode.getAttribute( "file" )) );
    }

}	// LoadFromFile


/************************************************************************
*    desc:  Get the font 
************************************************************************/
CFont * CFontMgr::GetFont( const string & name )
{
    // See if this character is part of the map
    fontMapIter = spFontMap.find( name );

    if( fontMapIter != spFontMap.end() )
        return fontMapIter->second;

    throw NExcept::CCriticalException("Font Manager Error!",
                boost::str( boost::format("Font name can't be found (%s).\n\n%s\nLine: %s") % name.c_str() % __FUNCTION__ % __LINE__ ));

    return NULL;

}	// GetFont


/************************************************************************
*    desc:  Is the font in the map 
************************************************************************/
bool CFontMgr::IsFont( const string & name )
{
    // See if this character is part of the map
    fontMapIter = spFontMap.find( name );

    if( fontMapIter != spFontMap.end() )
        return true;

    return false;

}	// IsFont


/************************************************************************
*    desc:  Delete a specific mesh
*
*    NOTE: The name of this function is because of windows naming
*          conflict. Can't name it DeleteFont().       
*  
*    param: string & key
************************************************************************/
void CFontMgr::DeleteTheFont( const string & key )
{
    // See if this font has already been loaded
    fontMapIter = spFontMap.find( key );

    // If it's found, delete from the map
    if( fontMapIter != spFontMap.end() )
        spFontMap.erase( fontMapIter );

}	// DeleteTheFont
