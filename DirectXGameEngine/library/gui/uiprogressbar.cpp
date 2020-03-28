/************************************************************************
*    FILE NAME:       uiprogressbar.cpp
*
*    DESCRIPTION:     Class for user interface progress bar
************************************************************************/

// Physical component dependency
#include <gui/uiprogressbar.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <system/xdevice.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/statcounter.h>
#include <common/xface2d.h>
#include <common/fontgroup.h>
#include <managers/shader.h>

typedef CUIHudControl CParent;

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CUIProgressBar::CUIProgressBar()
              : curMaxValue(0),
                lastMaxValue(0),
                highestCurValue(0),
                justify(EPJ_CENTER),
                artOrentation(EAO_HORZ),
                displayFormat(EDF_NONE)
{
    SetType( NUIControl::ECT_PROGRESS_BAR );

}   // Constructer


/************************************************************************
*    desc:  Load the control info from XML node
*  
*    param: node - XML node
************************************************************************/
void CUIProgressBar::LoadFromNode( const XMLNode & node )
{
    CParent::LoadFromNode( node );

    const XMLNode settingsNode = node.getChildNode( "settings" );

    if( !settingsNode.isEmpty() )
    {
        if( settingsNode.isAttributeSet( "maxValue" ) )
            curMaxValue = atof( settingsNode.getAttribute( "maxValue" ) );

        if( settingsNode.isAttributeSet( "justify" ) )
        {
            if( settingsNode.getAttribute( "justify" ) == string("left") )
                justify = EPJ_LEFT;

            else if( settingsNode.getAttribute( "justify" ) == string("right") )
                justify = EPJ_RIGHT;
        }
    }

    // Load any pre-defined current values
    const XMLNode curValueLstNode = node.getChildNode( "curValueLst" );
    if( !curValueLstNode.isEmpty() )
    {
        for( int i = 0; i < curValueLstNode.nChildNode(); ++i )
            curValueVec[i] = atof(curValueLstNode.getChildNode( "curValue", i ).getAttribute( "value" ));
    }

    // Find the number of %d's to determine the formating
    if( !IsFontStringGroupEmpty() )
    {
        int counter(0);

        // See if we specified a format string
        string formatStr = GetFontString();

        std::size_t index = formatStr.find("%d");

        while( index != std::string::npos )
        {
            ++counter;
            index = formatStr.find("%d",index+1);
        };

        if( (counter > 0) && (counter < EDF_MAX_FORMAT) )
            displayFormat = EDisplayFormat(counter);
    }

    // Set the progress bar label if there is one
    UpdateTextDisplay();

}	// LoadFromNode


/************************************************************************
*    desc:  Load the control specific info from XML node
*  
*    param: controlNode - XML node
************************************************************************/
void CUIProgressBar::LoadControlFromNode( const XMLNode & controlNode )
{
    CParent::LoadControlFromNode( controlNode );

    // Get the art orentation node
    const XMLNode artOrentationNode = controlNode.getChildNode( "artOrentation" );
    if( !artOrentationNode.isEmpty() &&
        artOrentationNode.isAttributeSet( "vert" ) &&
        (string(artOrentationNode.getAttribute( "vert" )) == "true") )
    {
        artOrentation = EPO_VERT;
    }

    // Get the list of progress bars associated with this button
    const XMLNode progBarLstNode = controlNode.getChildNode( "progressBarList" );

    spSpriteGroupVec.reserve( progBarLstNode.nChildNode() );
    curValueVec.reserve( progBarLstNode.nChildNode() );
    lastValueVec.reserve( progBarLstNode.nChildNode() );
    curStencilSizeVec.reserve( progBarLstNode.nChildNode() );

    for( int i = 0; i < progBarLstNode.nChildNode(); ++i )
    {
        const XMLNode objDataNode = progBarLstNode.getChildNode( "progressBarType", i );

        const string objTypeStr = objDataNode.getAttribute( "type" );

        // Set up the control's sprite group
        spProgSpriteGroupVec.push_back( 
            new CSpriteGroup2D( CObjectDataList2D::Instance().GetData( GetObjGroup(), objTypeStr ) ) );

        // Load the transform data
        spProgSpriteGroupVec.back().SetTransData( NParseHelper::LoadTransformData( objDataNode ) );

        // Add an entry for each progress bar
        curValueVec.push_back(0);
        lastValueVec.push_back(0);
        curStencilSizeVec.push_back(0);
    }

    // Get the list of stencil masks
    const XMLNode stencilMaskLstNode = controlNode.getChildNode( "stencilMaskList" );

    // Load the stencil size
    maxStencilSize = NParseHelper::LoadSize( stencilMaskLstNode );

    spStencilMaskSpriteVec.reserve( stencilMaskLstNode.nChildNode() );
    stencilMaskPosVec.reserve( stencilMaskLstNode.nChildNode() );

    for( int i = 0; i < stencilMaskLstNode.nChildNode(); ++i )
    {
        const XMLNode objDataNode = stencilMaskLstNode.getChildNode( "stencilMaskType", i );

        const string objTypeStr = objDataNode.getAttribute( "type" );

        // Set up the control's sprite group
        spStencilMaskSpriteVec.push_back( 
            new CSpriteGroup2D( CObjectDataList2D::Instance().GetData( GetObjGroup(), objTypeStr ) ) );

        // Load the transform data
        spStencilMaskSpriteVec.back().SetTransData( NParseHelper::LoadTransformData( objDataNode ) );

        // Save the stencil position for reseting
        stencilMaskPosVec.push_back( spStencilMaskSpriteVec.back().GetPos() );
    }

    if( spProgSpriteGroupVec.size() != spStencilMaskSpriteVec.size() )
        throw NExcept::CCriticalException("Progress Bar Load Error!",
                    boost::str( boost::format("progress bar art and stencil mismatch.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

}	// LoadControlFromNode


/************************************************************************
*    desc:  Transform the control
************************************************************************/
void CUIProgressBar::Transform()
{
    if( GetParameters().IsSet( CObject::TRANSFORM ) )
    {
        CBitmask tmpTransParam = GetParameters();
        CParent::Transform();

        // Tell the children of this control that they need to transform
        for( size_t i = 0; i < spProgSpriteGroupVec.size(); ++i )
            spProgSpriteGroupVec[i].SetTransformParameters( tmpTransParam, true );

        // Tell the stencil mask it needs to transform
        for( size_t i = 0; i < spStencilMaskSpriteVec.size(); ++i )
            spStencilMaskSpriteVec[i].SetTransformParameters( tmpTransParam, true );
    }

    // Transform the parent class's components
    CParent::Transform();

    for( size_t i = 0; i < spProgSpriteGroupVec.size(); ++i )
        spProgSpriteGroupVec[i].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

    // Transform the stencil mask
    for( size_t i = 0; i < spStencilMaskSpriteVec.size(); ++i )
        spStencilMaskSpriteVec[i].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

}	// Transform


/************************************************************************
*    desc:  Transform the control
*
*	 param: const CMatrix & _scaledMatrix  - passed in relative scaled rotation matrix
*			const CMatrix & _noScaleMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point      - passed in relative position
************************************************************************/
void CUIProgressBar::Transform( const CMatrix & _scaledMatrix, const CMatrix & _noScaleMatrix, 
                                const CWorldPoint & point )
{
    if( GetParameters().IsSet( CObject::TRANSFORM ) )
    {
        CBitmask tmpTransParam = GetParameters();
        CParent::Transform( _scaledMatrix, _noScaleMatrix, point );

        // Tell the children of this control that they need to transform
        for( size_t i = 0; i < spProgSpriteGroupVec.size(); ++i )
            spProgSpriteGroupVec[i].SetTransformParameters( tmpTransParam, true );

        // Tell the stencil mask it needs to transform
        for( size_t i = 0; i < spStencilMaskSpriteVec.size(); ++i )
            spStencilMaskSpriteVec[i].SetTransformParameters( tmpTransParam, true );
    }

    // Transform the parent class's components
    CParent::Transform( _scaledMatrix, _noScaleMatrix, point );

    for( size_t i = 0; i < spProgSpriteGroupVec.size(); ++i )
        spProgSpriteGroupVec[i].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

    // Transform the stencil mask
    for( size_t i = 0; i < spStencilMaskSpriteVec.size(); ++i )
        spStencilMaskSpriteVec[i].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );
    
}	// Transform


/************************************************************************
*    desc:  Update the control
************************************************************************/
void CUIProgressBar::Update()
{
    CParent::Update();

    bool recalcDisplay(false);

    // Find the hightest value and display it
    highestCurValue = 0;
    for( size_t i = 0; i < curValueVec.size(); ++i )
    {
        if( curValueVec[i] > highestCurValue )
            highestCurValue = curValueVec[i];

        if( curValueVec[i] != lastValueVec[i] )
        {
            recalcDisplay = true;
            lastValueVec[i] = curValueVec[i];
        }
    }

    // See if the max value has been changed
    if( curMaxValue != lastMaxValue )
    {
        recalcDisplay = true;
        lastMaxValue = curMaxValue;
    }

    if( recalcDisplay )
    {
        // Set the progress bar label if there is one
        UpdateTextDisplay();

        if( artOrentation == EAO_HORZ )
        {
            for( size_t i = 0; i < curValueVec.size(); ++i )
            {
                // Calculate the new width of the stencil buffer and strip off any floating point values
                curStencilSizeVec[i] = (int)(maxStencilSize.w * (curValueVec[i] / curMaxValue));

                // Set the new scale
                spStencilMaskSpriteVec[i].SetScale( CPoint(curStencilSizeVec[i], 1, 1) );

                // Reposition the progress bar
                CPoint offsetCorrection;

                if( justify == EPJ_LEFT )
                    offsetCorrection.x -= (maxStencilSize.w - curStencilSizeVec[i]) / 2;

                else if( justify == EPJ_RIGHT )
                    offsetCorrection.x += (maxStencilSize.w - curStencilSizeVec[i]) / 2;

                // Reset the position
                spStencilMaskSpriteVec[i].SetPos( stencilMaskPosVec[i] + offsetCorrection );
            }
        }
        else
        {
            for( size_t i = 0; i < curValueVec.size(); ++i )
            {
                // Calculate the new width of the stencil buffer and strip off any floating point values
                curStencilSizeVec[i] = (int)(maxStencilSize.h * (curValueVec[i] / curMaxValue));

                // Set the new scale
                spStencilMaskSpriteVec[i].SetScale( CPoint(1, curStencilSizeVec[i], 1) );

                // Reposition the progress bar
                CPoint offsetCorrection;

                if( justify == EPJ_LEFT )
                    offsetCorrection.y -= (maxStencilSize.h - curStencilSizeVec[i]) / 2;

                else if( justify == EPJ_RIGHT )
                    offsetCorrection.y += (maxStencilSize.h - curStencilSizeVec[i]) / 2;

                // Reset the position
                spStencilMaskSpriteVec[i].SetPos( stencilMaskPosVec[i] + offsetCorrection );
            }
        }
    }

}	// Update


/************************************************************************
*    desc:  Update the text display of the progress bar
************************************************************************/
void CUIProgressBar::UpdateTextDisplay()
{
    // Set the label if there is one
    if( !IsFontStringGroupEmpty() )
    {
        // See if we specified a format string
        string formatStr = GetFontString();

        // Format for display
        string valueStr = formatStr;

        if( displayFormat == EDF_CURRENT_ONLY )     // "%d%%"
            valueStr = boost::str( boost::format(formatStr) % (int)highestCurValue );

        else if( displayFormat == EDF_CURRENT_MAX ) // "%d/%d"
            valueStr = boost::str( boost::format(formatStr) % (int)highestCurValue % (int)curMaxValue );

        DisplayFontString(valueStr);
    }

}	// UpdateTextDisplay


/************************************************************************
*    desc:  Render the sub control
************************************************************************/
void CUIProgressBar::Render()
{
    if( IsVisible() )
    {
        for( size_t i = 0; i < spSpriteGroupVec.size(); ++i )
            spSpriteGroupVec[i].Render();

        // Render the progress bar inbetween the sprite groups and font
        RenderProgressBar();

        for( size_t i = 0; i < spFontGroupVec.size(); ++i )
            spFontGroupVec[i].fontSprite.Render();
    }

}	// Render


/************************************************************************
*    desc:  Render the progress bar 
************************************************************************/
void CUIProgressBar::RenderProgressBar()
{
    if( highestCurValue > 0 )
    {
        for( size_t i = 0; i < curValueVec.size(); ++i )
        {
            // Enable rendering to the stencil buffer
            CXDevice::Instance().EnableStencilRender( true );

            // Render to the stencil
            spStencilMaskSpriteVec[i].Render();

            // Get the stencil buffer ready to render through it
            CXDevice::Instance().InitRenderThroughStencil();

            // Render through stencil buffer
            spProgSpriteGroupVec[i].Render();

            // If this is the hightest current value then jump out here
            if( highestCurValue == curValueVec[i] )
                break;
        }

        // Disable rendering to the stencil buffer
        CXDevice::Instance().EnableStencilRender( false );
    }
}	// RenderProgressBar


/************************************************************************
*    desc:  Set and get max value
************************************************************************/
void CUIProgressBar::SetMaxValue( float value )
{
    lastMaxValue = curMaxValue;
    curMaxValue = value;

}	// SetMaxValue

float CUIProgressBar::GetMaxValue()
{
    return curMaxValue;

}	// GetMaxValue


/************************************************************************
*    desc:  Set and get current value
************************************************************************/
void CUIProgressBar::SetCurrentValue( float value, uint index )
{
    if( value < curMaxValue )
        curValueVec[index] = value;
    else
        curValueVec[index] = curMaxValue;

}	// SetCurrentValue

float CUIProgressBar::GetCurrentValue( uint index )
{
    return curValueVec[index];

}	// GetCurrentValue


/************************************************************************
*    desc:  Increment current value
************************************************************************/
void CUIProgressBar::IncCurrentValue( float value, uint index )
{
    float tmpValue = curValueVec[index] + value;

    if( tmpValue < 0 )
        curValueVec[index] = 0;
    else if( tmpValue < curMaxValue )
        curValueVec[index] = tmpValue;
    else
        curValueVec[index] = curMaxValue;

}	// IncCurrentValue


/************************************************************************
*    desc:  Set the progress bar value
************************************************************************/
void CUIProgressBar::Set( const CProgressBar & progressBar, uint index )
{
    curValueVec[index] = progressBar.GetCurrentValue();
    curMaxValue = progressBar.GetMaxValue();

}	// Set




