/************************************************************************
*    FILE NAME:       font.h
*
*    DESCRIPTION:     Font class
************************************************************************/  

#ifndef __font_h__
#define __font_h__

// DirectX lib dependencies
#include <d3dx9.h>

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_map.hpp>

// Game lib dependencies
#include <common/size.h>
#include <common/rect.h>

class CCharData
{
public:
    CCharData() : xAdvance(0) {}
    ~CCharData() {}

    // Character offsets
    CSize<float> offset;

    // Character rect
    CRect<float> rect;

    // Amount to advance
    float xAdvance;
};


class CFont : boost::noncopyable
{
public:

    CFont();
    ~CFont();

    // Load from XML file
    bool LoadFromXML( const std::string & group, const std::string & filePath );

    // Get the data for this character
    const CCharData * GetCharData( char id );

    // Get texture pointer
    LPDIRECT3DTEXTURE9 GetMaterialPtr(){ return pTexture; }

    // Get the line height
    float GetLineHeight() const { return lineHeight; }

    // Get the baseline offset
    float GetBaselineOffset() const { return baselineOffset; }

    // Get the horzontal padding
    float GetHorzPadding() const { return horzPadding; }

    // Get the vertical padding
    float GetVertPadding() const { return vertPadding; }

    // Get the texture size
    const CSize<float> & GetTextureSize() const { return size; }

private:

    // map list of character data
    boost::ptr_map<char, CCharData> spCharDataMap;
    boost::ptr_map<char, CCharData>::iterator charDataMapIter;

    // Line height
    float lineHeight;

    // base line offset
    float baselineOffset;

    // font texture size
    CSize<float> size;

    // Horizontal padding
    float horzPadding;

    // Vertival padding
    float vertPadding;

    // This font's texture
    // NOTE: This class doesn't own this pointer
    LPDIRECT3DTEXTURE9 pTexture;
};

#endif  // __font_h__
