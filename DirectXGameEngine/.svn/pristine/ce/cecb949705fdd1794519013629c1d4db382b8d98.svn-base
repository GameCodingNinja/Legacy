
/************************************************************************
*    FILE NAME:       fontmanager.h
*
*    DESCRIPTION:     font manager class singleton
************************************************************************/  

#ifndef __font_manager_h__
#define __font_manager_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Forward Declarations
class CFont;

class CFontMgr
{
public:

    // Get the instance of the singleton class
    static CFontMgr & Instance()
    {
        static CFontMgr fontMgr;
        return fontMgr;
    }

    // Load the material from file path
    void LoadFromXML( const std::string & filePath );

    // Get the font
    CFont * GetFont( const std::string & name );

    // Is the font in the map
    bool IsFont( const std::string & name );

    // Delete a specific mesh
    void DeleteTheFont( const std::string & key );

private:

    CFontMgr();
    ~CFontMgr();

    // map list of fonts
    boost::ptr_map<const std::string, CFont> spFontMap;
    boost::ptr_map<const std::string, CFont>::iterator fontMapIter;

};

#endif  // __font_manager_h__
