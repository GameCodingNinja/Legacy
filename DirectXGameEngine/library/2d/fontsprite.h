/************************************************************************
*    FILE NAME:       fontsprite.h
*
*    DESCRIPTION:     font sprite class
************************************************************************/

#ifndef __fontsprite_h__
#define __fontsprite_h__

// Physical component dependency
#include <common/object.h>
#include <scripting/scriptobject.h>

// Windows lib dependencies
#include <atlbase.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <vector>

// Game lib dependencies
#include <common/defs.h>
#include <2d/objectscriptdata2d.h>

// Forward Declaration(s)
class CFont;
struct XMLNode;

class CFontSprite : public CScriptObject
{
public:

    CFontSprite();
    virtual ~CFontSprite();

    // Load the font info from XML node
    void LoadFromNode( const XMLNode & node, const std::string & objGroupStr );

    // Set the display string
    void SetFontString( const std::string & str,
                        const std::string & fontNStr,
                        float _kerning = 0.f,
                        float _spaceCharKerning = 0.f,
                        float lineWrapWidth = -1.f,
                        float lineWrapHeightPadding = 0.f,
                        EHorzAlignment hAlign = EHA_HORZ_LEFT,
                        EVertAlignment vAlign = EVA_VERT_TOP );

    void SetFontString( const std::string & str );

    // Set the alignment
    void SetAlignment( EHorzAlignment hAlign, EVertAlignment vAlign );

    // Set the font name
    void SetFontName( const std::string & fontNStr );

    // Set the line wrap settings
    void SetLineWrap( float lineWrapWidth, float lineWrapHeightPadding );

    // Build the vertex buffer based on max number of chars needed
    void SetMaxCharacters( uint maxChar, bool forceRecreate = false );

    // Is the sprite active
    bool IsActive();

    // Update function
    virtual void Update();

    // Transform the font sprite
    void Transform();
    void Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                    const CWorldPoint & point );

    // Render this mesh to the back buffer
    void Render();

    // Get the displayed font string
    const std::string & GetDisplayedString();

private:

    // Init the sprite with the path to the mesh file
    void CreateVertexBuffer( uint maxChar, bool forceRecreate = false );

    // Count the number of spcae characters
    int CountSpaceChars( std::string & str );

    // Add up all the character widths
    std::vector<float> CalcLineWidthOffset( const std::string & str,
                                          float _kerning,
                                          float _spaceCharKerning,
                                          float lineWrapWidth,
                                          EHorzAlignment hAlign );

    // Add the line width to the vector based on horz alignment
    void AddLineWithToVec( std::vector<float> & lineWidthOffsetVec,
                           EHorzAlignment hAlign,
                           float width,
                           float firstCharOffset,
                           float lastCharOffset );

    // Update the shader prior to rendering
    void UpdateShader();

    // Render this mesh to the back buffer
    void RenderMesh();

    // Count up all the characters in this font string 
    // that does not exist in this font
    int CountNullCharacters( const std::string & str );

private:

    // Displayed font string
    std::string displayedStr;

    // last displayed font
    std::string lastFontNameStr;

    // Last font name - The name of the font last used
    std::string fontNameStr;

    // number of triangles to render char string
    int fontPrimativeCount;

    // Vertex buffer
    CComPtr<IDirect3DVertexBuffer9> spVertBuf;

    // The maximum number of characters
    // that can be rendered at one time
    uint maxCharCount;

    // The current number of characters
    // being rendered at this time
    uint currentCharCount;

    // Number of faces per character
    const int CHAR_FACE_COUNT;

    // DirectX vertex format
    uint vertexForamtMask;

    // DirectX vertex data size
    uint vertexDataSize;

    // Name of the shader technique and effect
    std::string effect, technique;

    // Name of the script 
    std::string scriptName;

    // The name of the default command
    std::string defaultCommand;

    // font pointer
    // NOTE: This class does not own this pointer
    CFont * pFont;

    // Alignment
    EHorzAlignment horzAlign;
    EVertAlignment vertAlign;

    // kerning value
    float kerning;

    // Space character kerning
    float spaceCharKerning;

    // Line wrap settings
    float wrapWidth;
    float wrapHeightPadding;

    // Key to the font map
    std::string fontMapKey;
};

#endif  // __fontsprite_h__
