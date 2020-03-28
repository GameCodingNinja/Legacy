/************************************************************************
*    FILE NAME:       uiprogressbar.h
*
*    DESCRIPTION:     Class for user interface progress bar
************************************************************************/

#ifndef __ui_progress_bar_h__
#define __ui_progress_bar_h__

// Physical component dependency
#include <gui/uihudcontrol.h>

// Windows lib dependencies
#include <atlbase.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Game lib dependencies
#include <2d/spritegroup2d.h>
#include <common/size.h>
#include <common/progressbar.h>

class CUIProgressBar : public CUIHudControl
{
public:
    
    // Constructor
    CUIProgressBar();

    // Load the initial info from XML node
    virtual void LoadFromNode( const XMLNode & node );

    // Load the control specific info from XML node
    virtual void LoadControlFromNode( const XMLNode & controlNode );

    // Render the control
    virtual void Render();

    // Transform the control
    void Transform();
    void Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                    const CWorldPoint & point );

    // Set/Get max value
    void SetMaxValue( float value );
    float GetMaxValue();

    // Set and get current value
    void SetCurrentValue( float value, uint index = 0 );
    float GetCurrentValue( uint index );

    // Increment current value
    void IncCurrentValue( float value, uint index = 0 );

    // Set the progress bar value
    void Set( const CProgressBar & progressBar, uint index = 0 );

    // Update the control
    virtual void Update();

private:

    // Update the text display of the progress bar
    void UpdateTextDisplay();

    // Render the progress bar
    void RenderProgressBar();

private:

    enum EPJustify
    {
        EPJ_LEFT,
        EPJ_CENTER,
        EPJ_RIGHT
    };

    enum EArtOrentation
    {
        EAO_HORZ,
        EPO_VERT
    };

    enum EDisplayFormat
    {
        EDF_NONE,
        EDF_CURRENT_ONLY,
        EDF_CURRENT_MAX,
        EDF_MAX_FORMAT
    };

    // Stencil size
    CSize<float> maxStencilSize;
    std::vector<float> curStencilSizeVec;

    // Progress bar sprite group vector
    boost::ptr_vector<CSpriteGroup2D> spProgSpriteGroupVec;

    // Stencil Mask sprite
    boost::ptr_vector<CSpriteGroup2D> spStencilMaskSpriteVec;

    // Stencil position
    std::vector<CPoint> stencilMaskPosVec;

    // Max value of progress bar
    float curMaxValue;

    // last Max value of progress bar
    float lastMaxValue;

    // Current value vector
    std::vector<float> curValueVec;

    // last value vector
    std::vector<float> lastValueVec;

    // Hightest current value
    float highestCurValue;

    // progress bar display
    EPJustify justify;

    // art orentation
    EArtOrentation artOrentation;

    // Percentage display format
    EDisplayFormat displayFormat;
};

#endif  // __ui_progress_bar_h__