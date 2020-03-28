
/************************************************************************
*    FILE NAME:       xdevice.h
*
*    DESCRIPTION:     For creating and managing a DirectX Device singlton 
************************************************************************/

#ifndef __x_window_h__
#define __x_window_h__

// Windows lib dependencies
#include <atlbase.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Boost lib dependencies
#include <boost/ptr_container/ptr_vector.hpp>

// Game lib dependencies
#include <misc/settings.h>
#include <common/defs.h>

/************************************************************************
*                         C3DWindow Object
************************************************************************/
class CXDevice
{
public:

    // Get the instance of the singleton class
    static CXDevice & Instance()
    {
        static CXDevice xwindow;
        return xwindow;
    }

    // Create the DirectX device
    void CreateXDevice( HWND hwnd );

    // Free all allocations
    void Free();

    // Get the instance of DirectX object
    LPDIRECT3D9 GetXInstance();

    // Get the pointer to DirectX device
    LPDIRECT3DDEVICE9 GetXDevice();

    // Get the maximum Z distance
    float GetMaxZDist();

    // Get the minimum Z distance
    float GetMinZDist();

    // Get the view angle
    float GetViewAngle();

    // Get the square percentage
    float GetSquarePercentage();

    // Get the frustrum Y ratio
    float GetFrustrumYRatio();

    // Check for two sided stencil capabilities
    bool IsTwoSidedStencil();

    // Check if we are running zPass or zFail shadow method
    bool IsUsingZPassShadowMethod();

    // Set if we are running zPass or zFail shadow method
    void SetUsingZPassShadowMethod( bool value );

    // Setup the stencil buffer for shadows.
    void SetStencilBufferForShadows();

    // Get the texture filtering
    int GetTextureFiltering( CSettings::ETextFilter textFilter );

    // Get the anisotropic filtering
    int GetAnisotropicFiltering( CSettings::ETextFilter textFilter );

    // Get the projection matrix
    D3DXMATRIX & GetProjectionMatrix( CSettings::EProjectionType type = CSettings::EPT_PERSPECTIVE );

    // Get the shadow map projection matrix
    D3DXMATRIX & GetShadowMapProjMatrix();

    // Get the width of the directX buffer
    float GetBufferWidth();

    // Get the height of the directX buffer
    float GetBufferHeight();

    // Get the back buffer format
    D3DFORMAT GetBufferFormat();

    // Get the window mode
    BOOL GetWindowed();

    // Handle the device reset
    void HandleDeviceReset( const CSize<float> & res, bool vSync, bool windowed );

    // Clear the buffers
    void ClearBuffers();

    // Clear the z-buffer only
    void ClearZBuffer();

    // Clear the stencil buffer only
    void ClearStencilBuffer();

    // Get the shadow map buffer surface
    LPDIRECT3DSURFACE9 GetShadowMapBufferSurface();

    // Get the shadow map buffer texture
    LPDIRECT3DTEXTURE9 GetShadowMapBufferTexture();

    // Get the display buffer surface
    LPDIRECT3DSURFACE9 GetDisplayBufferSurface();

    // Is the shadow buffer active?
    bool IsShadowMapBufferActive();

    // Set the shadow map buffer as the render target
    void SetShadowMapBufferAsRenderTarget();

    // Set the display surface as the render target
    void SetDisplaySurfaceAsRenderTarget();

    // Set the post process surface as the render target
    void SetPostProcessSurfaceAsRenderTarget();

    // Is the post process buffer active?
    bool IsPostProcessBufferActive();

    // Get the post process buffer texture
    LPDIRECT3DTEXTURE9 GetPostProcessBufferTexture();

    // Flip the prost process buffers
    void FlipPostProcBuffers();

    // Get the post process buffer at index
    LPDIRECT3DTEXTURE9 GetPostProcBufText( unsigned int index );

    // Get the post process vertex buffer
    LPDIRECT3DVERTEXBUFFER9 GetPostProcessVertexBuffer();

    // Set the post process buffer index
    void SetActivePostProcBufIndex( unsigned int index );

    // Enable Z Buffering
    void EnableZBuffering( bool value = true, bool overrideSettings = true );

    // Enable the culling
    void EnableCulling( bool enable = true );

    // Calculate the projection matrixes
    void CalcProjMatrix( const CSize<float> & res, const CSize<float> & defSize );

    // Get the available texture memory
    uint GetAvailableTextureMem();

    // Get used amount of memory used for video
    uint GetVideoMemUsed();

    // Enable/Disable rendering to the stencil buffer
    void EnableStencilRender( bool enable );

    // Get the stencil buffer ready to render through it
    void InitRenderThroughStencil();

    // Get the max texture width
    uint GetMaxTextureWidth();

    // Get the max texture height
    uint GetMaxTextureHeight();

    // Get the handle to the game window
    HWND GetWndHandle();

protected:

    // Display error information
    void DisplayError( HRESULT hr );

    // Buffers and render states that need to be 
    // reset/recreated after a device rest
    void ResetXDevice();

    // Release device created members
    void ReleaseDeviceCreatedMembers();

    // Get the post process vertex buffer
    void CreatePostProcessVertexBuffer();

private:

    // Constructor
    CXDevice();

    // Destructor
    ~CXDevice();

private:

    // DirectX smart instance
    CComPtr<IDirect3D9> spDXInstance;

    // DirectX smart device
    CComPtr<IDirect3DDevice9> spDXDevice;

    // Display surface LPDIRECT3DSURFACE9
    CComPtr<IDirect3DSurface9> spDisplaySurface;

    // Get the shadow map buffer surface
    CComPtr<IDirect3DTexture9> spShadowMapBufferTexture;

    // Surface of the shadow map buffer
    CComPtr<IDirect3DSurface9> spShadowMapBufferSurface;

    // Texture of the post process buffers
    std::vector< CComPtr<IDirect3DTexture9> > pPostProcTextVec;

    // Surface of the post process buffers
    std::vector< CComPtr<IDirect3DSurface9> > pPostProcSurfaceVec;

    // Post process vertex buffer
    CComPtr<IDirect3DVertexBuffer9> spPostProcessVertBuf;

    // DirectX capabilities
    D3DCAPS9 d3dCaps;

    // DirectX present Parameters
    D3DPRESENT_PARAMETERS dxpp;

    // The maximum z distance
    float maximumZDist;

    // The minimum z distance
    float minimumZDist;

    // The view angle
    float viewAngle;

    // Calculated square percentage of window view area
    float squarePercentage;

    // ratio of y frustrum
    float frustrumYRatio;

    // Stencil buffer shadow mode - true = zPass, false = zFail
    bool zPassStencilBufferMode;

    // Projection martix
    D3DXMATRIX projMatrix;
    D3DXMATRIX perspectiveMatrix;
    D3DXMATRIX orthographicMatrix;

    // buffer mask
    DWORD bufferClearMask;

    // buffer mask
    DWORD depthStencilBufferClearMask;

    // post process buffer index
    int postProcBufIndex;

    // Initial video memory on start up
    uint initialVideoMemory;

    // Amount of video memory left after buffers
    uint videoMemWithBuffers;

    // Device window handle
    HWND hWnd;
};

inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }

#endif // __x_window_h__
