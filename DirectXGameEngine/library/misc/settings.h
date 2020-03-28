/************************************************************************
*    FILE NAME:       settings.h
*
*    DESCRIPTION:     game settings class
************************************************************************/

#ifndef __settings_h__
#define __settings_h__

// Standard lib dependencies
#include <vector>
#include <string>
#include <Windows.h>

// Game lib dependencies
#include "common/size.h"
#include <xmlParser/xmlParser.h>

class CSettings
{
public:

    enum EProjectionType
    {
        EPT_NULL,
        EPT_PERSPECTIVE,
        EPT_ORTHOGRAPHIC,
        EPT_PROJ_MAX
    };

    enum ETextFilter
    {
        ETF_POINT,
        ETF_LINEAR,
        ETF_ANISOTROPIC_2X = 2,
        ETF_ANISOTROPIC_4X = 4,
        ETF_ANISOTROPIC_8X = 8,
        ETF_ANISOTROPIC_16X = 16,
        ETF_TEXT_FILTER_MAX
    };

    class CPostProcBuff
    {
    public:
        float scale;
        std::string format;
    };

    // Get the instance of the singleton class
    static CSettings & Instance()
    {
        static CSettings settings;
        return settings;
    }

    // Append the game folder name to the path
    void AppendToFolderPath( const std::string & folderName );

    // Get the folder path
    const std::string & GetFolderPath() const;

    // Clear the folder path
    void ClearFolderPath();

    // Load settings data from xml file
    void LoadFromXML( const std::string & _filePath );

    // Save the settings file
    void SaveSettings();

    // Get the class name
    const std::string & GetGameWndClassName() const;

    // Get game window size
    const CSize<float> & GetSize() const;
    const CSize<float> & GetSizeHalf() const;
    const CSize<float> & GetSizeChange() const;
    const CSize<float> & GetDefaultSize() const;
    const CSize<float> & GetDefaultSizeHalf() const;

    // Set the change size
    void SetChangeSize( const CSize<float> & _size );

    // Get ratios
    const CSize<float> & GetScreenRatio() const;

    // Get ratios
    float GetDeviceRatio() const;

    // Get the projection type
    EProjectionType GetProjectionType() const;

    // Get the view angle
    float GetViewAngle() const;

    // Get the minimum z distance
    float GetMinZdist() const;

    // Get the maximum z distance
    float GetMaxZdist() const;

    // Get the orthographic screen projection
    float GetOrthoScreenPercentage() const;

    // Do we create the depth stencil buffer
    bool GetCreateDepthStencilBuffer() const;

    // Do we clear the stencil buffer
    bool GetClearStencilBuffer() const;

    // Is the depth buffer enabled by default
    bool GetEnableDepthBuffer() const;

    // Do we clear the target buffer
    bool GetClearTargetBuffer() const;

    // Calculate the 2D ratio for translations
    void CalculateTransRatio2D();

    // Do we create a shadow buffer
    bool GetCreateShadowMapBuffer() const;

    // Get the height and width of the shadow buffer
    const CSize<int> & GetShadowMapBufferSize() const;

    // Do we create a post process buffer
    bool GetCreatePostProcBuf() const;

    // Get the number of post process buffers
    size_t GetPostProcBufCount() const;

    // Get the post process buffer info
    const CPostProcBuff & GetPostProcBufInfo(int index) const;

    // Do we want tripple buffering?
    bool GetTripleBuffering() const;

    // Get the texture filtering
    ETextFilter GetTextFiltering() const;

    // Get the texture filtering change
    CSettings::ETextFilter GetTextFilteringChange() const;

    ////// Members that can be reset ///////

    // Is the window full screen
    bool GetFullScreen() const;

    // Is the full screen window change
    bool GetFullScreenChange() const;

    // Set the window to full screen
    void SetFullScreenChange( bool value );

    // Do we want vSync?
    bool GetVSync() const;
    bool GetVSyncChange() const;
    void SetVSyncChange( bool value );

    // Get/Set the filter index
    int GetTextFilterIndex() const;
    void SetTextFilterByIndex( int index );
    std::string GetTextFilterString() const;

    // Get/Set the joypad stick range
    int GetJoyPadStickRange() const;
    void SetJoyPadStickRange( int value );

    // Get/Set the joypad stick dead zone
    int GetJoyPadStickDeadZone() const;
    void SetJoyPadStickDeadZone( int value );

    // Was the resolution changed
    bool WasResolutionChanged() const;

    // Are we changing to full screen?
    bool IsChangeToFullScreen() const;

    // Are we changing to window view?
    bool IsChangeToWindowView() const;

    // Was the full screen changed
    bool WasScreenModeChanged() const;

    // Was the VSync changed
    bool WasVSyncChanged() const;

    // Was the text filtering changed
    bool WasTextFilteringChanged() const;

    // Calculate the ratio
    void CalcRatio( const CSize<float> & res );

    // Get the display settings
    const DEVMODE & GetDisplaySettings() const;

private:

    // Constructor
    CSettings();

    // Destructor
    ~CSettings();

    // file path string
    std::string filePath;

    // xml node
    XMLNode mainNode;

    // with and height of game window
    CSize<float> size;
    CSize<float> size_half;
    CSize<float> sizeChange;
    CSize<float> default_size;
    CSize<float> default_size_half;

    // Height and width screen ratio for render objects
    CSize<float> screenRatio;

    // Ratio for devices (mice) to correctly calculate movement
    // between the default height and actual height
    float deviceRatio;

    // Single ratio applied to all 2D translations
    float transRatio2D;

    // class name for the window class
    std::string className;

    // Full screen flag loaded from file
    bool fullScreen;

    // Full screen flag change
    bool fullScreenChange;

    // The projection type
    EProjectionType projType;

    // view angle
    float viewAngle;

    // minimum Z distance
    float minZdist;

    // maximum Z distance
    float maxZdist;

    // Orthographic screen percentage
    float orthoScrnPercentage;

    // Do we create the depth stencil buffer
    bool createDepthStencilBuffer;

    // Do we clear the stencil buffer
    bool clearStencilBuffer;

    // Do we enable the depth buffer
    bool enableDepthBuffer;

    // Do we clear the target buffer
    bool clearTargetBuffer;

    // Do we create the shadow buffer
    bool createShadowMapBuffer;

    // Shadow buffer width and height
    CSize<int> shadowMapBufferSize;

    // Post process buffer info vector
    std::vector<CPostProcBuff> postProcBuffVec;

    // Triple buffering flag
    bool tripleBuffering;

    // VSync flag
    bool vSync;
    bool vSyncChange;

    // Texture filtering
    ETextFilter textFiltering;
    ETextFilter textFilteringChange;

    // Render groups
    bool pre3D;
    bool pre2D;
    bool post3D;
    bool post2D;

    // Joypad settings
    int joypadStickRange;
    int joypadStickDeadZone;

    // User folder path
    std::string userFolderPath;

    // Struct to hold display settings
    DEVMODE dm;

};

#endif  // __settings_h__
