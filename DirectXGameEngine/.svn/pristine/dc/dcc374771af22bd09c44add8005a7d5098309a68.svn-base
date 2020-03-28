/************************************************************************
*    FILE NAME:       visualsprite2d.cpp
*
*    DESCRIPTION:     2D DirectX sprite class
************************************************************************/

// Physical component dependency
#include <2d/fontsprite.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <managers/meshmanager.h>
#include <managers/texturemanager.h>
#include <managers/shader.h>
#include <system/xdevice.h>
#include <misc/settings.h>
#include <scripting/command.h>
#include <scripting/commandscript.h>
#include <scripting/scriptmanager.h>
#include <utilities/statcounter.h>
#include <utilities/exceptionhandling.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/genfunc.h>
#include <utilities/xmlparsehelper.h>
#include <common/defs.h>
#include <common/xface2d.h>
#include <2d/font.h>
#include <2d/fontmanager.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CFontSprite::CFontSprite()
           : //materialColor(1.f,1.f,1.f,1.f),
             fontPrimativeCount(0),
             maxCharCount(0),
             currentCharCount(0),
             CHAR_FACE_COUNT(2),
             pFont(NULL),
             horzAlign(EHA_HORZ_LEFT),
             vertAlign(EVA_VERT_TOP),
             kerning(0.0f),
             spaceCharKerning(0.0f),
             wrapWidth(-1.0f),
             wrapHeightPadding(0.0f)
{
    vertexForamtMask = D3DFVF_XYZ|D3DFVF_TEX1;
    vertexDataSize = sizeof(CVertex2D);

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CFontSprite::~CFontSprite()
{
}   // Destructer


/************************************************************************
*    desc:  Load the font info from XML node
*  
*    param: node - XML node
*           objGroupStr - group this font belongs to
*
*	 ret:	bool - control was loaded or not loaded successfully
************************************************************************/
void CFontSprite::LoadFromNode( const XMLNode & node, const string & objGroupStr )
{
    // Get the must have font related items
    SetFontName( string(node.getAttribute( "fontName" )) );

    if( node.isAttributeSet( "maxChar" ) )
    {
        uint maxChar = atoi(node.getAttribute( "maxChar" ));

        SetMaxCharacters( maxChar );
    }

    if( node.isAttributeSet( "kerning" ) )
        kerning = atof(node.getAttribute( "kerning" ));

    if( node.isAttributeSet( "spaceCharKerning" ) )
        spaceCharKerning = atof(node.getAttribute( "spaceCharKerning" ));

    // Set the default word wrap and padding
    float wordWrap = -1.0f;
    float wordPad = 0.0f;

    if( node.isAttributeSet( "wrapWidth" ) )
        wordWrap = atof(node.getAttribute( "wrapWidth" ));

    if( node.isAttributeSet( "wrapPad" ) )
        wordPad = atof(node.getAttribute( "wrapPad" ));

    SetLineWrap( wordWrap, wordPad );


    ////////////////////////////////////////////////////////////
    //	VISUAL
    ////////////////////////////////////////////////////////////
    XMLNode lightingNode = node.getChildNode( "lighting" );

    // The lighting shader needs to be there
    if( !lightingNode.isEmpty() )
    {
        // Set the strings for shader lighting
        effect = lightingNode.getAttribute( "effect" );
        technique = lightingNode.getAttribute( "technique" );

        // Set the color and default color
        SetDefaultColor( NParseHelper::LoadColor(lightingNode) );
        ResetColor();
    }
    else
        throw NExcept::CCriticalException("XML Font Creation Error!",
            boost::str( boost::format("No lighting data specified for font.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

    // The lighting shader needs to be there
    XMLNode alignNode = node.getChildNode( "alignment" );
    if( !alignNode.isEmpty() )
    {
        // Set the default alignment
        EHorzAlignment horzAlign = NParseHelper::LoadHorzAlignment( alignNode, EHA_HORZ_CENTER);
        EVertAlignment vertAlign = NParseHelper::LoadVertAlignment( alignNode, EVA_VERT_CENTER);

        SetAlignment( horzAlign, vertAlign );
    }

    // Load the transform data
    SetTransData( NParseHelper::LoadTransformData( node ) );


    ////////////////////////////////////////////////////////////
    //	SCRIPTING
    ////////////////////////////////////////////////////////////
    XMLNode scriptNode = node.getChildNode( "script" );

    if( !scriptNode.isEmpty() )
    {
        if( scriptNode.isAttributeSet("name") )
            scriptName = scriptNode.getAttribute( "name" );

        if( scriptNode.isAttributeSet("command") )
            defaultCommand = scriptNode.getAttribute( "command" );

        // Set up the script
        if( !scriptName.empty() )
            SetCommandScript( CScriptMgr::Instance().GetCommandScript( 
                          objGroupStr, scriptName ) );

        // Start the default command
        if( !defaultCommand.empty() )
            SetActiveCommand( defaultCommand );
    }

}	// LoadFromNode


/************************************************************************
*    desc:  Create the vertex buffer
*  
*    param: maxChar - number of characters needed for vertex buffer
*                     NOTE: Space characer is ignored
*           forceRecreate - force a recreate of the vertex buffer
*    
*    return: bool - true on success or false on fail
************************************************************************/
void CFontSprite::CreateVertexBuffer( uint maxChar, bool forceRecreate )
{
    // Only create the vertex buffer if one doesn't exist or more 
    // characters are needed then the current buffer
    if( maxChar > 0 )
    {
        if( (spVertBuf == NULL) || (maxChar > maxCharCount) || forceRecreate )
        {
            // Clear out the data before loading
            spVertBuf.Release();

            // Create the vertex buffer that will be used for all fonts
            if( D3D_OK != CXDevice::Instance().GetXDevice()->CreateVertexBuffer( CHAR_FACE_COUNT * sizeof(CXFace2D) * maxChar,
                                                                                 D3DUSAGE_WRITEONLY,
                                                                                 0,
                                                                                 D3DPOOL_MANAGED,
                                                                                 &spVertBuf,
                                                                                 NULL ) )
            {
                throw NExcept::CCriticalException("Font Vertex Buffer Creation Error!", 
                    boost::str( boost::format("Error creating vertex buffer.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));
            }

            // Only set the max char count if the vertex buffer size has changed.
            maxCharCount = maxChar;
        }
    }

    currentCharCount = maxChar;

}   // CreateVertexBuffer


/************************************************************************
*    desc:  Build the vertex buffer based on max number of chars needed
************************************************************************/
void CFontSprite::SetMaxCharacters( uint maxChar, bool forceRecreate )
{
    CreateVertexBuffer( maxChar, forceRecreate );

}	// SetMaxCharacters


/************************************************************************
*    desc:  Set the display string
*
*    parm: str - character string to render
*          fontName - font to use
*          _kerning - distance between each character
*          _spaceCharKerning - special kerning just for the space character
*          lineWrapWidth - width of line to force wrap
*          lineWrapHeightPadding - add spacing to the lines
*          hAlign - horzontal alignment
*          vAlign - vertical alignment
*
*    NOTE: Line wrap feature only supported for horizontal left
************************************************************************/
void CFontSprite::SetFontString( const string & str,
                                 const string & fontName,
                                 float _kerning,
                                 float _spaceCharKerning,
                                 float lineWrapWidth,
                                 float lineWrapHeightPadding,
                                 EHorzAlignment hAlign,
                                 EVertAlignment vAlign )
{
    if( lastFontNameStr != fontName )
        pFont = CFontMgr::Instance().GetFont( fontName );

    fontMapKey = fontName;

    bool allowStringFont = (pFont != NULL) && !str.empty();

    // Set visible to false for these conditions
    if( !allowStringFont )
        SetVisible( false );

    if( allowStringFont && ((displayedStr != str) || (fontNameStr != fontName)) )
    {
        displayedStr = str;
        lastFontNameStr = fontName;

        // count up the number of space characters
        int spaceCharCount = NGenFunc::CountStrOccurrence( str, " " );

        // Count up the number of NULL characters - characters not in this font
        int nullCharCount = CountNullCharacters( str );

        // See if we need to make a vertex buffer
        CreateVertexBuffer( static_cast<uint>(str.size() - spaceCharCount - nullCharCount) );

        float xOffset = 0.f;
        float width = 0.f;
        float lineHeightOffset = 0.f;
        float lineHeightWrap = pFont->GetLineHeight() + pFont->GetVertPadding() + lineWrapHeightPadding;
        float initialHeightOffset = pFont->GetBaselineOffset() + pFont->GetVertPadding();
        float lineSpace = pFont->GetLineHeight() - pFont->GetBaselineOffset();

        uint strLoopCounter = 0;
        int lineCount = 0;

        // Make a face pointer
        CXFace2D * pFace;

        // Lock the vertex buffer for copying
        if( FAILED(spVertBuf->Lock( 0, 0, (void **)&pFace, 0 )) )
        {
            throw NExcept::CCriticalException("Vertex Buffer Lock Error!", 
                boost::str( boost::format("Error locking vertex buffer.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));
        }

        // Get the size of the texture
        CSize<float> textureSize = pFont->GetTextureSize();

        // Handle the horizontal alignment
        vector<float> lineWidthOffsetVec = CalcLineWidthOffset( str, _kerning, _spaceCharKerning, lineWrapWidth, hAlign );

        // Set the initial line offset
        xOffset = lineWidthOffsetVec[lineCount++];

        // Handle the vertical alighnmenrt
        if( vAlign == EVA_VERT_TOP )
            lineHeightOffset = -initialHeightOffset;

        if( vAlign == EVA_VERT_CENTER )
        {
            lineHeightOffset = -(initialHeightOffset - ((pFont->GetBaselineOffset()-lineSpace) / 2) - pFont->GetVertPadding());

            if( lineWidthOffsetVec.size() > 1 )
                lineHeightOffset = ((lineHeightWrap * lineWidthOffsetVec.size()) / 2) - pFont->GetBaselineOffset();
        }

        else if( vAlign == EVA_VERT_BOTTOM )
        {
            lineHeightOffset = -(initialHeightOffset - pFont->GetBaselineOffset() - pFont->GetVertPadding());

            if( lineWidthOffsetVec.size() > 1 )
                lineHeightOffset += (lineHeightWrap * (lineWidthOffsetVec.size()-1));
        }

        // Remove any fractional component of the line height offset
        lineHeightOffset = (int)lineHeightOffset;

        // Setup each character in the vertex buffer
        for( size_t i = 0; i < str.size(); ++i )
        {
            char id = str[i];

            // See if we can find the character
            const CCharData * pCharData = pFont->GetCharData(id);

            // Character might not exist in font
            if( pCharData != NULL )
            {
                // Ignore space characters
                if( id != ' ' )
                {
                    CRect<float> rect = pCharData->rect;

                    float yOffset = (pFont->GetLineHeight() - rect.y2 - pCharData->offset.h) + lineHeightOffset;

                    // Index into the faces. Two faces (quad) per character
                    int index = strLoopCounter * 2;

                    // Check if the width or height is even. If so, we offset 
                    // by 0.5 for proper orthographic rendering
                    float additionalOffsetX = 0;
                    if( (int)rect.x2 % 2 == 0 )
                        additionalOffsetX = 0.5f;

                    float additionalOffsetY = 0;
                    if( (int)rect.y2 % 2 == 0 )
                        additionalOffsetY = 0.5f;

                    // Calculate the first vertex of the first face
                    pFace[index].vert[0].vert.x = xOffset + pCharData->offset.w + additionalOffsetX;
                    pFace[index].vert[0].vert.y = yOffset + rect.y2 + additionalOffsetY;
                    pFace[index].vert[0].uv.u = rect.x1 / textureSize.w;
                    pFace[index].vert[0].uv.v = rect.y1 / textureSize.h;

                    // Calculate the second vertex of the first face
                    pFace[index].vert[1].vert.x = xOffset + rect.x2 + pCharData->offset.w + additionalOffsetX;
                    pFace[index].vert[1].vert.y = yOffset + rect.y2 + additionalOffsetY;
                    pFace[index].vert[1].uv.u = (rect.x1 + rect.x2) / textureSize.w;
                    pFace[index].vert[1].uv.v = rect.y1 / textureSize.h;

                    // Calculate the third vertex of the first face
                    pFace[index].vert[2].vert.x = xOffset + pCharData->offset.w + additionalOffsetX;
                    pFace[index].vert[2].vert.y = yOffset + additionalOffsetY;
                    pFace[index].vert[2].uv.u = rect.x1 / textureSize.w;
                    pFace[index].vert[2].uv.v = (rect.y1 + rect.y2) / textureSize.h;

                    // The first vertex of the second face is the same as the
                    // previously calculated vertex
                    pFace[index+1].vert[0] = pFace[index].vert[1];

                    // The third vertex of the second face is the same as the
                    // previously calculated vertex
                    pFace[index+1].vert[2] = pFace[index].vert[2];

                    // Calculate the second vertex of the second face
                    pFace[index+1].vert[1].vert.x = xOffset + rect.x2 + pCharData->offset.w;
                    pFace[index+1].vert[1].vert.y = yOffset;
                    pFace[index+1].vert[1].uv.u = (rect.x1 + rect.x2) / textureSize.w;
                    pFace[index+1].vert[1].uv.v = (rect.y1 + rect.y2) / textureSize.h;

                    ++strLoopCounter;
                }

                // Inc the font position
                float inc = pCharData->xAdvance + _kerning + pFont->GetHorzPadding();

                // Add in any additional spacing for the space character
                if( id == ' ' )
                    inc += _spaceCharKerning;

                width += inc;
                xOffset += inc;

                // Wrap to another line
                if( (id == ' ') && (lineWrapWidth > 0.f) )
                {
                    float nextWord = 0.f;

                    // Get the length of the next word to see if if should wrap
                    for( size_t j = i+1; j < str.size(); ++j )
                    {
                        id = str[j];

                        // See if we can find the character
                        pCharData = pFont->GetCharData(id);

                        // Character might not exist in font
                        if( pCharData != NULL )
                        {
                            // Break here when space is found
                            // Don't add the space to the size of the next word
                            if( id == ' ' )
                                break;

                            // Don't count the 
                            nextWord += pCharData->xAdvance + _kerning + pFont->GetHorzPadding();
                        }
                    }

                    if( width + nextWord >= lineWrapWidth )
                    {
                        xOffset = lineWidthOffsetVec[lineCount++];
                        width = 0.f;

                        lineHeightOffset += -lineHeightWrap;
                    }
                }
            }
            else
            {
                // Line wrap if '|' character was used
                if( id == '|' )
                {
                    xOffset = lineWidthOffsetVec[lineCount];
                    width = 0.f;

                    lineHeightOffset += -lineHeightWrap;
                    ++lineCount;
                }
            }
        }

        // Unlock the buffer so it can be used
        spVertBuf->Unlock();

        SetVisible( true );
    }

}	// SetFontString


/************************************************************************
*    desc:  Set the display string
*
*    parm: str - character string to render
************************************************************************/
void CFontSprite::SetFontString( const string & str )
{
    SetFontString( str, fontNameStr, kerning, spaceCharKerning, wrapWidth, wrapHeightPadding, horzAlign, vertAlign );
}


/************************************************************************
*    desc:  Add up all the character widths
************************************************************************/
vector<float> CFontSprite::CalcLineWidthOffset( const string & str,
                                              float _kerning,
                                              float _spaceCharKerning,
                                              float lineWrapWidth,
                                              EHorzAlignment hAlign )
{
    float firstCharOffset = 0;
    float lastCharOffset = 0;
    float spaceWidth = 0;
    float width = 0;
    int counter = 0;
    vector<float> lineWidthOffsetVec;

    for( size_t i = 0; i < str.size(); ++i )
    {
        char id = str[i];

        // See if we can find the character
        const CCharData * pCharData = pFont->GetCharData( id );

        // Character might not exist in font
        if( pCharData != NULL )
        {
            if(counter == 0)
                firstCharOffset = pCharData->offset.w;

            spaceWidth = pCharData->xAdvance + _kerning + pFont->GetHorzPadding();

            // Add in any additional spacing for the space character
            if( id == ' ' )
                spaceWidth += _spaceCharKerning;

            width += spaceWidth;

            if( id != ' ')
                lastCharOffset = pCharData->offset.w;

            ++counter;
        }
        else
        {
            // Line wrap if '|' character was used
            if( id == '|' )
            {
                // Add the line width to the vector based on horz alignment
                AddLineWithToVec( lineWidthOffsetVec, hAlign, width, firstCharOffset, lastCharOffset );

                counter = 0;
                width = 0;
            }
        }

        // Wrap to another line
        if( (id == ' ') && (lineWrapWidth > 0.f) )
        {
            float nextWord = 0.f;

            // Get the length of the next word to see if if should wrap
            for( size_t j = i+1; j < str.size(); ++j )
            {
                id = str[j];

                // See if we can find the character
                pCharData = pFont->GetCharData(id);

                // Character might not exist in font
                if( pCharData != NULL )
                {
                    // Break here when space is found
                    // Don't add the space to the size of the next word
                    if( id == ' ' )
                        break;

                    // Don't count the 
                    nextWord += pCharData->xAdvance + _kerning + pFont->GetHorzPadding();
                }
            }

            if( width + nextWord >= lineWrapWidth )
            {
                // Add the line width to the vector based on horz alignment
                AddLineWithToVec( lineWidthOffsetVec, hAlign, width-spaceWidth, firstCharOffset, lastCharOffset );

                counter = 0;
                width = 0;
            }
        }
    }

    // Add the line width to the vector based on horz alignment
    AddLineWithToVec( lineWidthOffsetVec, hAlign, width, firstCharOffset, lastCharOffset );

    return lineWidthOffsetVec;

}	// CalcLineWidthOffset


/************************************************************************
*    desc:  Add the line width to the vector based on horz alignment
************************************************************************/
void CFontSprite::AddLineWithToVec( vector<float> & lineWidthOffsetVec,
                                    const EHorzAlignment hAlign,
                                    float width,
                                    float firstCharOffset,
                                    float lastCharOffset )
{
    if( hAlign == EHA_HORZ_LEFT )
        lineWidthOffsetVec.push_back(-(firstCharOffset + pFont->GetHorzPadding()));

    else if( hAlign == EHA_HORZ_CENTER )
        lineWidthOffsetVec.push_back(-((width / 2) + pFont->GetHorzPadding()));

    else if( hAlign == EHA_HORZ_RIGHT )
        lineWidthOffsetVec.push_back(-(width - lastCharOffset - pFont->GetHorzPadding()));

    // Remove any fractional component of the line height offset
    lineWidthOffsetVec.back() = (int)lineWidthOffsetVec.back();

}	// AddLineWithToVec


/************************************************************************
*    desc:  Count up all the characters in this font string that does
*           not exist in this font
************************************************************************/
int CFontSprite::CountNullCharacters( const string & str )
{
    int result = 0;

    for( size_t i = 0; i < str.size(); ++i )
    {
        // See if we can find the character
        const CCharData * pCharData = pFont->GetCharData( str[i] );

        if( pCharData == NULL )
            ++result;
    }

    return result;

}	// CountNullCharacters


/************************************************************************
*    desc:  Update the shader prior to rendering
*
*    param: CMatrix & matrix - passed in matrix class
************************************************************************/
void CFontSprite::UpdateShader()
{
    // Inc our stat counter to keep track of what is going on.
    CStatCounter::Instance().IncDisplayCounter();

    // Copy it to the DirectX matrix
    D3DXMATRIX scalCameraMatrix( GetScaledMatrix()() );
    scalCameraMatrix._41 = GetTransPos().x;
    scalCameraMatrix._42 = GetTransPos().y;
    scalCameraMatrix._43 = GetTransPos().z;

    // Create a projection matrix to send to the shader
    D3DXMATRIX cameraViewProjectionMatrix = scalCameraMatrix *
        CXDevice::Instance().GetProjectionMatrix( CSettings::EPT_ORTHOGRAPHIC );

    // Set the type of shader being used
    CEffectData * pEffectData =
        CShader::Instance().SetEffectAndTechnique( effect, technique );

    // Copy the matrix to the shader
    CShader::Instance().SetEffectValue( pEffectData, "cameraViewProjMatrix", cameraViewProjectionMatrix );

    // Set the material color
    CShader::Instance().SetEffectValue( pEffectData, "materialColor", GetColor()() );

    // Set the texture
    CTextureMgr::Instance().SelectTexture( pFont->GetMaterialPtr() );

}	// UpdateShader


/************************************************************************
*    desc:  Is this sprite active
*  
*    return: bool - sprite scale
************************************************************************/
bool CFontSprite::IsActive()
{
    return ((spVertBuf != NULL) && (pFont != NULL) && !displayedStr.empty());

}   // IsActive


/****************************************************************************
*   desc:	Update the sprite group
****************************************************************************/
void CFontSprite::Update()
{
    CScriptObject::Update();

}	// Update


/************************************************************************
*    desc:  Transform the sprite
************************************************************************/
void CFontSprite::Transform()
{
    if( GetParameters().IsSet( CObject::TRANSFORM ) )
        CObject::Transform();

}	// Transform


/************************************************************************
*    desc:  Transform the sprite
*
*	 param: const CMatrix & _scaledMatrix  - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point      - passed in relative position
************************************************************************/
void CFontSprite::Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                             const CWorldPoint & point )
{
    if( GetParameters().IsSet( CObject::TRANSFORM ) )
        CObject::Transform( _scaledMatrix, _unscaledMatrix, point );

}	// Transform


/************************************************************************
*    desc:  Render the sprite
************************************************************************/
void CFontSprite::Render()
{
    if( IsVisible() && IsActive() )
    {
        UpdateShader();
        RenderMesh();
    }

}	// Render


/************************************************************************
*    desc:  Render this mesh to the back buffer 
************************************************************************/
void CFontSprite::RenderMesh()
{
    // The 0 specifies that ID3DXEffect::Begin and ID3DXEffect::End will 
    // save and restore all state modified by the effect.
    UINT uPasses;
    CShader::Instance().GetActiveShader()->Begin( &uPasses, 0 );

    CXDevice::Instance().GetXDevice()->SetStreamSource( 0, spVertBuf, 0, vertexDataSize );
    CXDevice::Instance().GetXDevice()->SetFVF( vertexForamtMask );

    for( UINT uPass = 0; uPass < uPasses; ++uPass )
    {
        CShader::Instance().GetActiveShader()->BeginPass( uPass );
        CXDevice::Instance().GetXDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, CHAR_FACE_COUNT * currentCharCount );
        CShader::Instance().GetActiveShader()->EndPass();
    }

    CShader::Instance().GetActiveShader()->End();

}	// RenderMesh


/************************************************************************
*    desc:  Set the alignment 
************************************************************************/
void CFontSprite::SetAlignment( EHorzAlignment hAlign, EVertAlignment vAlign )
{
    horzAlign = hAlign;
    vertAlign = vAlign;
}


/************************************************************************
*    desc:  Set the font name 
************************************************************************/
void CFontSprite::SetFontName( const std::string & fontNStr )
{
    fontNameStr = fontNStr;
}


/************************************************************************
*    desc:  Set the line wrap settings 
************************************************************************/
void CFontSprite::SetLineWrap( float lineWrapWidth, float lineWrapHeightPadding )
{
    wrapWidth = lineWrapWidth;
    wrapHeightPadding = lineWrapHeightPadding;
}


/************************************************************************
*    desc:  Get the displayed font string 
************************************************************************/
const std::string & CFontSprite::GetDisplayedString()
{
    return displayedStr;
}
