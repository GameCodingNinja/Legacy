
/************************************************************************
*    FILE NAME:       gamewnd.cpp
*
*    DESCRIPTION:     game window
************************************************************************/

// Game OS dependencies
                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

// Physical component dependency
#include <system/gamewnd.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <system/xdevice.h>
#include <misc/settings.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/statcounter.h>
#include <managers/shader.h>
#include <managers/physicsworldmanager.h>
#include <utilities/exceptionhandling.h>
#include <common/xvert2d.h>

// Required namespace(s)
using namespace std;

// Disable the warning we get from converting a BOOL to a bool
#pragma warning(disable : 4800)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CGameWnd::CGameWnd()
        : hInst(NULL),
          hWnd(NULL),
          gameRunning(false),
          gameLoadComplete(false),
          enablePostProcess(true),
          pPostProcEffectData(NULL),
          statsDisplayTimer(500),
          preRenderPause(false)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CGameWnd::~CGameWnd()
{
    DestroyWindow(hWnd);

}   // Destructer


/***************************************************************************
*   desc:  Create the game Window
*
*   param:  HWND hwnd   - Handle of the window receiving the message
*   param:  winCallbackFuncPtrType pWinCallback - Windows call back function
*
*   ret:  bool - true on success or false on fail
 ****************************************************************************/
void CGameWnd::Create( HINSTANCE hInstance, winCallbackFuncPtrType pWinCallback )
{
    // Save the program instance
    hInst = hInstance;

    // Register the parent window with Windows
    RegisterWnd( hInstance, pWinCallback );

    // Create the game window
    Create( hInstance );

}   // Create


/***************************************************************************
*   desc:  All windows created with CreateWindow must be registered first.
*          This does not include windows controls like buttons, listboxex 
*          and such because they register themselves.
*
*   param:  HINSTANCE hInst - This progams instance
*   param:  winCallbackFuncPtrType pWinCallback - Windows call back function
*
*   ret:  bool - true on success or false on fail
****************************************************************************/
void CGameWnd::RegisterWnd( HINSTANCE hInstance,
                            winCallbackFuncPtrType pWinCallback )
{
    WNDCLASSEX w;

    w.cbSize = sizeof( WNDCLASSEX );
    w.style = NULL;
    w.lpfnWndProc = pWinCallback;
    w.cbClsExtra = 0;
    w.cbWndExtra = 0;
    w.hInstance = hInstance;
    w.hIcon = LoadIcon( hInstance, "AppIcon" );
    w.hIconSm = NULL;
    w.hCursor = LoadCursor( NULL, IDC_ARROW );
    w.hbrBackground = NULL;
    w.lpszMenuName = NULL;
    w.lpszClassName = CSettings::Instance().GetGameWndClassName().c_str();

    if( RegisterClassEx(&w) == 0 )
        throw NExcept::CCriticalException("Register Class Window", "There was an error registering the class window.");

}   // Starter_RegisterWnd 


/***************************************************************************
*   desc:  Create the parent window
*
*   param:  HINSTANCE hInst - This progams instance
*
*   ret:  bool - true on success or false on fail
****************************************************************************/
void CGameWnd::Create( HINSTANCE hInstance )
{
    // Init the high resolution performance timer
    CHighResTimer::Instance().InitPerformanceTimer();

    // Create the game window
    CreateGameWindow( hInstance );

    // Calculate the center point of the mouse
    CalcMouseCenterPoint();

    // Create the directX device
    CXDevice::Instance().CreateXDevice( hWnd );

    // Show The Window
    ShowWindow( hWnd, SW_SHOW );

    // Display a black screen
    CXDevice::Instance().ClearBuffers();
    CXDevice::Instance().GetXDevice()->BeginScene();
    CXDevice::Instance().GetXDevice()->EndScene();
    CXDevice::Instance().GetXDevice()->Present( NULL, NULL, NULL, NULL );

    // Load game related objects
    LoadGameObjects();

}   // CreateWnd


/************************************************************************                                                            
*    desc:  Create the game window
*
*    param:    HINSTANCE hInst -  instance of this program
************************************************************************/
void CGameWnd::CreateGameWindow( const HINSTANCE hInst )
{
    RECT rect;

    // Set the rect
    SetRect( &rect, 0, 0, (int)CSettings::Instance().GetSize().w, (int)CSettings::Instance().GetSize().h );
    
    // Create the window
    hWnd = CreateWindowEx( WS_EX_TOPMOST,
                           CSettings::Instance().GetGameWndClassName().c_str(),
                           "",
                           CSettings::Instance().GetFullScreen() ? WS_POPUP : WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
                           rect.left,
                           rect.top,
                           rect.right,
                           rect.bottom,
                           NULL,
                           NULL,
                           hInst,
                           NULL );

    if( hWnd == NULL )
        throw NExcept::CCriticalException("Create Window Error",
            boost::str( boost::format("There was an error creating the game window.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

    if( !CSettings::Instance().GetFullScreen() )
    {
        // Adjust the rect so that the client size of the window is the exact size of the needed resolution
        AdjustWindowRectEx( &rect, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL, GetWindowExStyle(hWnd) );

        // Position the window in the center of the screen
        SetWindowPos( hWnd,
                      HWND_NOTOPMOST,
                      (GetSystemMetrics( SM_CXSCREEN ) - (rect.right-rect.left)) / 2,
                      (GetSystemMetrics( SM_CYSCREEN ) - (rect.bottom-rect.top)) / 2,
                      rect.right - rect.left,
                      rect.bottom - rect.top,
                      SWP_NOACTIVATE );
    }

}   // CreateGameWindow


/************************************************************************
*    desc:  Reset the window size
************************************************************************/
void CGameWnd::ResetWindowSize()
{
    RECT rect;

    // Set the rect
    SetRect( &rect, 0, 0, (int)CSettings::Instance().GetSizeChange().w, (int)CSettings::Instance().GetSizeChange().h );

    if( CSettings::Instance().WasResolutionChanged() || CSettings::Instance().WasScreenModeChanged() )
    {
        // Reset the windows styles on mode change
        if( CSettings::Instance().WasScreenModeChanged() )
        {
            DWORD style = GetWindowLong(hWnd, GWL_STYLE);

            // Going to full screen - remove the windows styles and add the popup
            if( CSettings::Instance().IsChangeToFullScreen() )
                SetWindowLong(hWnd, GWL_STYLE, (style & ~(WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX)) | WS_POPUP);

            // Going to windowed view - remove the popup and add the windows styles
            else if( CSettings::Instance().IsChangeToWindowView() )
                SetWindowLong(hWnd, GWL_STYLE, (style & ~WS_POPUP) | WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX);
        }

        if( CSettings::Instance().WasResolutionChanged() )
        {
            // Recalculate the default size and ratio
            CSettings::Instance().CalcRatio( CSettings::Instance().GetSizeChange() );

            // Recalculate the projection matrixes
            CXDevice::Instance().CalcProjMatrix( CSettings::Instance().GetSizeChange(),
                                                 CSettings::Instance().GetDefaultSize() );
        }

        if( !CSettings::Instance().GetFullScreenChange() )
        {
            // Adjust the rect so that the client size of the window is the exact size of the needed resolution
            AdjustWindowRectEx( &rect, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL, GetWindowExStyle(hWnd) );

            // Position the window in the center of the screen
            SetWindowPos( hWnd,
                          HWND_NOTOPMOST,
                          ((GetSystemMetrics( SM_CXSCREEN ) - (rect.right-rect.left)) / 2), 
                          ((GetSystemMetrics( SM_CYSCREEN ) - (rect.bottom-rect.top)) / 2),
                          rect.right - rect.left,
                          rect.bottom - rect.top,
                          SWP_SHOWWINDOW|SWP_FRAMECHANGED );
        }
    }

}	// ResetWindowSize


/***************************************************************************
*   desc:  Standard Windows OnClose message handler.
*          NOTE: The window is destroyed from xwindow destructor
*                Do not use the OnDestroy message handler
*
*   param:  HWND hwnd  - Handle of the window receiving the message
****************************************************************************/
void CGameWnd::OnClose( HWND hwnd )
{
    if( gameLoadComplete )
    {
        // Hide The Window
        ShowWindow( hwnd, SW_HIDE );

        StopGame();

        // Clean up some DirectX objects before DirectX is freed.
        DeleteGameObjects();

        // This causes the WinMain to fall out of it's while loop
        PostQuitMessage( 0 );
    }

}   // OnClose


/***************************************************************************
*  desc:   Standard Windows OnCreate message handler.
*
*  param:  HWND hwnd - Handle of the window receiving the message
*          LPCREATESTRUCT lpCreateStruct  - Structure used in window creation
****************************************************************************/
bool CGameWnd::OnCreate( HWND hwnd, LPCREATESTRUCT lpCreateStruct )
{
    return true;

}   // Starter_OnCreate


/***************************************************************************
*  desc:   Standard Windows OnSize message handler.
*
*  param:  HWND hwnd - Handle of the window receiving the message
*          UINT state - message state
*          int cx - new width
*          int cy - new height
****************************************************************************/
void CGameWnd::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    if( gameLoadComplete )
    {
        if( state == SIZE_MINIMIZED )
        {
            gameRunning = false;
        }
        else if( state == SIZE_RESTORED )
        {
            gameRunning = true;

            CalcMouseCenterPoint();
        }
    }

}	// OnSize


/***************************************************************************
*  desc:   // window Frame pain message
*
*   param:  HWND hwnd      - Handle of the window receiving the message
*           UINT Message   - Message being sent 
*           WPARAM wParam  - Additional information included with message
*           LPARAM lParam  - Additional information included with message
****************************************************************************/
LRESULT CGameWnd::OnNcPaint(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    if( CXDevice::Instance().GetWindowed() )
        return DefWindowProc( hwnd, Message, wParam, lParam );
    else
        return 0;

}	// OnNcPaint


/***************************************************************************
*   desc:  Standard Windows WM_ACTIVATEAPP message handler.
*
*   param:  hwnd  - Handle of the window receiving the message
*   param:  fActivate  - true/false if active
*   param:  dwThreadId  - thread id
****************************************************************************/
void CGameWnd::OnActivateApp(HWND hwnd, BOOL fActivate, DWORD dwThreadId)
{
    hasFocus = fActivate;

}	// OnActivateApp


/***************************************************************************
*    desc:  Load game resources here
*
*    ret:   bool - true on success or false on fail
****************************************************************************/
void CGameWnd::LoadGameObjects()
{
}	// LoadGameObjects


/***************************************************************************
*    decs:  Delete game resources here
****************************************************************************/
void CGameWnd::DeleteGameObjects()
{
    // Clear the physics worlds
    CPhysicsWorldManager::Instance().Clear();

}	// DeleteGameObjects


/***************************************************************************
*   desc:  Exception Clean-up function
*          NOTE: The window is destroyed from xwindow destructor
*                Do not use the OnDestroy message handler
****************************************************************************/
void CGameWnd::ExceptionCleanUp()
{
    StopGame();

    DeleteGameObjects();

}	// ExceptionCleanUp


/***************************************************************************
*   desc:  Center the mouse pointer to the game window
****************************************************************************/
void CGameWnd::CalcMouseCenterPoint()
{
    RECT rect;

    // get the window rect
    GetWindowRect( hWnd, &rect );
    
    // Set the center point for the mouse for movement comparison
    mouseCenterPt.x = rect.left + ((rect.right - rect.left) / 2);
    mouseCenterPt.y = rect.top + ((rect.bottom - rect.top) / 2);

}	// CalcMouseCenterPoint


/***************************************************************************
*   desc:  Main game loop. This function is called from the window message loop
****************************************************************************/
void CGameWnd::GameLoop()
{
    HRESULT hr;
    if( FAILED( hr = CXDevice::Instance().GetXDevice()->TestCooperativeLevel() ) )
    {
        if( hr == D3DERR_DEVICELOST )
        {
            return;
        }

        if( hr == D3DERR_DEVICENOTRESET )
        {
            HandleDeviceReset( CSettings::Instance().GetSize(),
                               CSettings::Instance().GetVSync(),
                               !CSettings::Instance().GetFullScreen() );
            return;
        }
    }

    // First thing we need to do is get our elapsed time
    CHighResTimer::Instance().CalcElapsedTime();

    // Handle the state change
    DoStateChange();

    // Get user input
    GetUserInput();

    if( gameRunning )
    {
        // Check for collision and react to it
        ReactToCollision();

        // Update animations, Move sprites, Check for collision
        Update();

        // See if we are to do any post process
        PostProcessDecision();

        // Transform game objects
        Transform();

        // Do the rendering
        Render();

        // Inc the cycle
        CStatCounter::Instance().IncCycle();

        // update the stats every 500 miliseconds
        if( statsDisplayTimer.Expired(CTimer::RESTART_ON_EXPIRE) )
        {
            // The call also resets the counters
            SetWindowCaption( CStatCounter::Instance().GetStatString() );
        }
    }

}   // GameLoop


/***************************************************************************
*   desc:  Do the rendering
****************************************************************************/
void CGameWnd::Render()
{
    if( !IsPreRenderPause() )
    {
        // Render to the shadow map buffer if one is present
        if( CXDevice::Instance().IsShadowMapBufferActive() )
        {
            // Set the shadow map buffer as the render target
            CXDevice::Instance().SetShadowMapBufferAsRenderTarget();

            // Clear the buffers that are in use (depth, target and stencil buffer)
            CXDevice::Instance().ClearBuffers();

            // Indicate rendering will begin
            if( SUCCEEDED( CXDevice::Instance().GetXDevice()->BeginScene() ) )
            {
                // Render to shadow map buffer
                RenderShadowBuffer();

                // Indicate rendering has stopped
                CXDevice::Instance().GetXDevice()->EndScene();
            }
        }

        // Set the render target
        if( CXDevice::Instance().IsPostProcessBufferActive() && enablePostProcess )
            CXDevice::Instance().SetPostProcessSurfaceAsRenderTarget();
        else
            CXDevice::Instance().SetDisplaySurfaceAsRenderTarget();

        // Clear the buffers that are in use (depth, target and stencil buffer)
        CXDevice::Instance().ClearBuffers();


        // Do the Pre post process rendering of game content
        {
            // Clear only the z-buffer
            CXDevice::Instance().ClearZBuffer();
        
            // Indicate rendering will begin
            if( SUCCEEDED( CXDevice::Instance().GetXDevice()->BeginScene() ) )
            {
                // Render all 3D visual objects
                PreRender();

                // Indicate rendering has stopped
                CXDevice::Instance().GetXDevice()->EndScene();
            }
        }
    }



    // Do the post process render if the buffer is in use
    if( CXDevice::Instance().IsPostProcessBufferActive() && enablePostProcess )
    {
        // Indicate rendering will begin
        if( SUCCEEDED( CXDevice::Instance().GetXDevice()->BeginScene() ) )
        {
            // Do the post process render
            RenderPostProcess();

            // Indicate rendering has stopped
            CXDevice::Instance().GetXDevice()->EndScene();
        }

        // If render pause was activated, need to reset the display surface as the 
        // render target because the last render was done on the post process buffer
        // and that still may be the render target
        CXDevice::Instance().SetDisplaySurfaceAsRenderTarget();
    }



    // Do the render of game content after the post process
    {
        // Clear only the z-buffer
        CXDevice::Instance().ClearZBuffer();

        // Indicate rendering will begin
        if( SUCCEEDED( CXDevice::Instance().GetXDevice()->BeginScene() ) )
        {
            // Render all 3D visual objects
            PostRender();

            // Indicate rendering has stopped
            CXDevice::Instance().GetXDevice()->EndScene();
        }
    }

    // show what we rendered
    CXDevice::Instance().GetXDevice()->Present( NULL, NULL, NULL, NULL );

}	// Render


/************************************************************************
*    desc:  Render the post process
************************************************************************/
void CGameWnd::RenderPostProcess()
{
    if( !IsPreRenderPause() )
    {
        // Set the shader effect and technique
        CShader::Instance().SetEffectAndTechnique( pPostProcEffectData, postProcTechIdStr );

        // Set the current post process buffer as the render target 
        CShader::Instance().SetEffectValue( pPostProcEffectData, postProcTextureIdStr, CXDevice::Instance().GetPostProcessBufferTexture() );

        // Do game specific post process effects
        HandlePostProcessEffects();

        // The 0 specifies that ID3DXEffect::Begin and ID3DXEffect::End will 
        // save and restore all state modified by the effect.
        UINT uPasses;
        CShader::Instance().GetActiveShader()->Begin( &uPasses, 0 );

        // Flip the post process buffers for multi pass rendering
        if( uPasses > 1 )
        {
            CXDevice::Instance().FlipPostProcBuffers();
            CXDevice::Instance().SetPostProcessSurfaceAsRenderTarget();
        }
        else // Set the display surface as the render target
            CXDevice::Instance().SetDisplaySurfaceAsRenderTarget();

        // Give the vertex buffer to DirectX
        CXDevice::Instance().GetXDevice()->SetStreamSource( 0, CXDevice::Instance().GetPostProcessVertexBuffer(), 0, sizeof( CXVert2D ) );
        CXDevice::Instance().GetXDevice()->SetFVF( D3DFVF_XYZRHW|D3DFVF_TEX1 );

        for( UINT uPass = 0; uPass < uPasses; ++uPass )
        {
            if( uPass > 0 )
            {
                // Set the current post process buffer as the texture to render to the target 
                CShader::Instance().SetEffectValue( postProcTextureIdStr, CXDevice::Instance().GetPostProcessBufferTexture() );

                if( (uPasses > 1) && (IsPreRenderPause() ||(uPass < uPasses-1)) )
                {
                    CXDevice::Instance().FlipPostProcBuffers();
                    CXDevice::Instance().SetPostProcessSurfaceAsRenderTarget();
                }
                else if( uPass == uPasses-1 )
                {
                    // Set the display surface as the render target
                    CXDevice::Instance().SetDisplaySurfaceAsRenderTarget();
                }
            }

            CShader::Instance().GetActiveShader()->BeginPass( uPass );
            CXDevice::Instance().GetXDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
            CShader::Instance().GetActiveShader()->EndPass();
        }
    
        CShader::Instance().GetActiveShader()->End();
    }
    else
    {
        // Set the display surface as the render target
        CXDevice::Instance().SetDisplaySurfaceAsRenderTarget();

        // Set up the shader before the rendering
        CShader::Instance().SetEffectAndTechnique( pPostProcEffectData, postProcTechIdStr );

        // Set the current post process buffer as the texture to render to the target 
        CShader::Instance().SetEffectValue( pPostProcEffectData, postProcTextureIdStr, CXDevice::Instance().GetPostProcessBufferTexture() );

        // The 0 specifies that ID3DXEffect::Begin and ID3DXEffect::End will 
        // save and restore all state modified by the effect.
        UINT uPasses;
        CShader::Instance().GetActiveShader()->Begin( &uPasses, 0 );

        // Give the vertex buffer to DirectX
        CXDevice::Instance().GetXDevice()->SetStreamSource( 0, CXDevice::Instance().GetPostProcessVertexBuffer(), 0, sizeof( CXVert2D ) );
        CXDevice::Instance().GetXDevice()->SetFVF( D3DFVF_XYZRHW|D3DFVF_TEX1 );

        for( UINT uPass = 0; uPass < uPasses; ++uPass )
        {
            CShader::Instance().GetActiveShader()->BeginPass( uPass );
            CXDevice::Instance().GetXDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
            CShader::Instance().GetActiveShader()->EndPass();
        }
    
        CShader::Instance().GetActiveShader()->End();
    }

}	// RenderPostProcess


/***************************************************************************
*   desc:  Start the game
****************************************************************************/
void CGameWnd::StartGame()
{
    gameRunning = true;

}   // StartGame


/***************************************************************************
*   desc:  Stop the game
****************************************************************************/
void CGameWnd::StopGame()
{
    gameRunning = false;

}   // StopGame


/***************************************************************************
*  desc:  Is the game running?
*
*  ret: bool - true or false if game is running
****************************************************************************/
bool CGameWnd::IsGameRunning()
{
    return gameRunning;

}   // IsGameRunning


/***************************************************************************
*  desc:  Enable/disable post processing
****************************************************************************/
void CGameWnd::EnablePostProcessing( bool value )
{
    enablePostProcess = value;

}   // EnablePostProcessing



/***************************************************************************
*   desc:  Does the game window have the focus?
****************************************************************************/
bool CGameWnd::HasFocus() const
{
    return hasFocus;

}   // HasFocus


/***************************************************************************
*  desc:  Post processing strings
*
*  parm: _postProcEffectFileIdStr - effect file ID
*        _postProcTechIdStr - technique ID
*        _postProcTextureIdStr - post process texture ID
****************************************************************************/
void CGameWnd::SetPostProcStrings( const string & _postProcEffectFileIdStr,
                                   const string & _postProcTechIdStr,
                                   const string & _postProcTextureIdStr )
{
    // NOTE: This class does not own this pointer.
    pPostProcEffectData = CShader::Instance().GetEffectData( _postProcEffectFileIdStr );

    postProcTechIdStr = _postProcTechIdStr;
    postProcTextureIdStr = _postProcTextureIdStr;

}   // SetPostProcStrings

void CGameWnd::SetPostProcStrings( CEffectData * _pEffectData,
                                   const string & _postProcTechIdStr,
                                   const string & _postProcTextureIdStr )
{
    // NOTE: This class does not own this pointer.
    pPostProcEffectData = _pEffectData;

    postProcTechIdStr = _postProcTechIdStr;
    postProcTextureIdStr = _postProcTextureIdStr;

}   // SetPostProcStrings

void CGameWnd::SetPostProcStrings( const string & _postProcEffectFileIdStr,
                                   const string & _postProcTechIdStr )
{
    // NOTE: This class does not own this pointer.
    pPostProcEffectData = CShader::Instance().GetEffectData( _postProcEffectFileIdStr );

    postProcTechIdStr = _postProcTechIdStr;

}   // SetPostProcStrings

void CGameWnd::SetPostProcStrings( CEffectData * _pEffectData,
                                   const string & _postProcTechIdStr )
{
    // NOTE: This class does not own this pointer.
    pPostProcEffectData = _pEffectData;

    postProcTechIdStr = _postProcTechIdStr;

}   // SetPostProcStrings

void CGameWnd::SetPostProcStrings( const string & _postProcTechIdStr )
{
    postProcTechIdStr = _postProcTechIdStr;

}   // SetPostProcStrings


/***************************************************************************
*  desc:  Get the effect data
****************************************************************************/
CEffectData * CGameWnd::GetPostProcEffectDataPtr()
{
    return pPostProcEffectData;
}


/***************************************************************************
*  desc:  Get/Set pre render pause
****************************************************************************/
bool CGameWnd::IsPreRenderPause() const
{
    return preRenderPause;
}

void CGameWnd::SetPreRenderPause( bool value )
{
    preRenderPause = value;
}


/************************************************************************
*    desc:  Get the handle to the game window
*
*    ret: HWND - Handle to game window
************************************************************************/
HWND CGameWnd::GetHandle()
{
    return hWnd;

}	// GetHandle


/************************************************************************                                                             
*    desc:  Set the caption for the window
*
*    param:  const char * pCaption  - Render device
************************************************************************/
void CGameWnd::SetWindowCaption( const std::string & caption )
{
    SetWindowText( hWnd, caption.c_str() );

}	// SetWindowCaption