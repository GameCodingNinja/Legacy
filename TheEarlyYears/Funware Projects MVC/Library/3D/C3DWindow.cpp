
/************************************************************************
*
*    FILE NAME:       C3DWindow.cpp
*
*    DESCRIPTION:     For creating and managing the 3d Window and device 
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "C3DWindow.h"       // Header file for this *.cpp file
#include "minTools.h"          // A misc group of useful functions
#include "gl/gl.h"           // standard OpenGL include
#include "gl/glu.h"          // OpenGL utilities
#include "CMesh.h"

// These are globals used by the 3D engine and must be setup
float gfMaxZDistance;
float gfMinZDistance;
float gfSquarePercentage;
float gfFrustrumYRatio;
int giRenderDevice;
float gfCameraScaleX;
float gfCameraScaleY;
float gfBufWidthDiv2;
float gfBufHeightDiv2;
int giBufWidth;
int giBufHeight;
unsigned int *gusiVBufer;
int *giZBuffer;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

#ifdef _16_BIT_SOFTWARE_RENDERING_
unsigned short int *gusiShadeTbl=NULL;
#endif

// DirectX may or maynot be turned on
#ifdef _USE_DIRECTX_9_
    LPDIRECT3DVERTEXBUFFER9 g_DirectX_VB = NULL;
    LPDIRECT3DDEVICE9 g_pDXDevice = NULL;
#endif
#ifdef _USE_DIRECTX_8_
    LPDIRECT3DVERTEXBUFFER8 g_DirectX_VB = NULL;
    LPDIRECT3DDEVICE8 g_pDXDevice = NULL;
#endif


/************************************************************************
*                         C3DWindow Object
************************************************************************/


/************************************************************************
*    DESCRIPTION:               Constructer                                                             
************************************************************************/
C3DWindow::C3DWindow()
{
    // Init class members
    width = 0;
    height = 0;
    viewAngle = 0;
    giRenderDevice = ERD_NO_RENDER_DEVICE;
    hDC = NULL;
    hOpenGL_RC = NULL;
    hInstance = NULL;
    hWnd = NULL;
    displayMode = EDM_NO_DISPLAY_MODE;
    giZBuffer = NULL;
    #if defined(_USE_DIRECTX_8_) || defined(_USE_DIRECTX_9_)
    pDXInstance = NULL;
    #endif
    fullScreenModeChange = false;
    manageWindowCleanup = true;
}


/************************************************************************
*    DESCRIPTION:               Destructer                                                             
************************************************************************/
C3DWindow::~C3DWindow()
{
    Free();    
}


/************************************************************************                                                            
*    DESCRIPTION:  Free all allocations
************************************************************************/
void C3DWindow::Free()
{
    // Free the buffer
    buffer.Free();

    // Free the z buffer
    if( giZBuffer )
    {
        delete [] giZBuffer;
        giZBuffer = NULL;
    }

    #ifdef _16_BIT_SOFTWARE_RENDERING_
    // Allocate the shaded lookup table
    if( gusiShadeTbl )
    {
        delete [] gusiShadeTbl;
        gusiShadeTbl = NULL;
    }
    #endif

    // Deselect Rendering Contex and delete
    if( hOpenGL_RC != NULL )
    {
        wglMakeCurrent( NULL, NULL );
        wglDeleteContext( hOpenGL_RC );
        hOpenGL_RC = NULL;
    }

    #if defined(_USE_DIRECTX_8_) || defined(_USE_DIRECTX_9_)
    if( g_DirectX_VB != NULL )
    {
        g_DirectX_VB->Release();
        g_DirectX_VB = NULL;
    }

    if( g_pDXDevice != NULL )
    {
        g_pDXDevice->Release();
        g_pDXDevice = NULL;
    }
    
    if( pDXInstance != NULL )
    {
        pDXInstance->Release();
        pDXInstance = NULL;
    }
    #endif

    // Do we manage the cleanup of the window?
    if( manageWindowCleanup == true )
    {
        // Release the screen DC. Make sure this is the last item to free
        if( hDC != NULL )
        {
            ReleaseDC( hWnd, hDC );
            hDC = NULL;
        }

        // Change the display mode back to it's original settings
        if( displayMode == EDM_FULL_SCREEN && fullScreenModeChange )
        {
            ChangeDisplaySettings( NULL, 0 );
        }
    }

}   // Free


/************************************************************************                                                            
*    DESCRIPTION:  Create the parent window
*
*    Input:    int w        - Width of render window
*              int h        - Height of render window
*              HWND hWindow - Handle to an already cleaned up window
*              HDC hWinDC   - dc of created window
*              HINSTANCE hInst - instance of this program
*              EDM_DISPLAY_MODE disMode - Display Mode
*              bool windowCleanup - Flag to indicate if window cleanup
*                                   is managed by this class.
*                                   defaule is false.
************************************************************************/
void C3DWindow::Init3DWind( int w, int h, HWND hWindow, HDC hWinDC,
                            HINSTANCE hInst, EDM_DISPLAY_MODE disMode,
                            bool windowCleanup )
{
	// Set class members
    width = w;
    height = h;
	hWnd = hWindow;
	hDC = hWinDC;
    hInstance = hInst;
    displayMode = disMode;
	manageWindowCleanup = windowCleanup;
}


/************************************************************************                                                            
*    DESCRIPTION:  Create the parent window
*
*    Input:    int w      - Width of render window
*              int h      - Height of render window
*              HINSTANCE hInst -  instance of this program
*              EDM_DISPLAY_MODE disMode - Display Mode
*              char *windowName  - Registered name of the window
*              char *windowTitle - Info that show in the windows caption
*
*    Output:   returntype HWND - Handle to created window
************************************************************************/
HWND C3DWindow::Create3DWind( int w, int h, HINSTANCE hInst,
                              EDM_DISPLAY_MODE disMode, 
                              char *windowName, char *windowTitle )
{
    unsigned int dwExStyle;
    unsigned int dwStyle;
    RECT rect;
    
    // Set class members
    width = w;
    height = h;
    hInstance = hInst;
    displayMode = disMode;

    SetRect( &rect, 0, 0, width, height );

    if( displayMode == EDM_WINDOWED )
    {       
        // Setup the styles
        dwExStyle = WS_EX_APPWINDOW|WS_EX_WINDOWEDGE;
        dwStyle = WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX;

        // Calculate how big the window will be with the required client size           
        AdjustWindowRectEx( &rect, dwStyle, true, dwExStyle );

        // Center the window on the screen
        SetRect( &rect,
                 (GetSystemMetrics( SM_CXSCREEN ) - (rect.right-rect.left)) / 2,
                 (GetSystemMetrics( SM_CYSCREEN ) - (rect.bottom-rect.top)) / 2,
                 rect.right-rect.left,
                 rect.bottom-rect.top );            
    }
    else if( displayMode == EDM_FULL_SCREEN )
    {
        dwExStyle = WS_EX_TOPMOST;
        dwStyle = WS_POPUP | WS_VISIBLE;

        DEVMODE dm;
        DWORD mode = 0;
        bool modeFound = false;

        // search for the mode we are looking for
        while( !modeFound && EnumDisplaySettings( NULL, mode, &dm ) )
        {
            if( dm.dmBitsPerPel == 32 && dm.dmPelsWidth == width && dm.dmPelsHeight == height )
            {
                // We found our device mode so change it
                if( ChangeDisplaySettings( &dm, 0 ) == DISP_CHANGE_SUCCESSFUL )
                {
                    fullScreenModeChange = true;

                    // Sit and sleep for a second and give the
                    // system some time to change over.
                    Sleep( 700 );
                }
                else
                {
                    // Something went wrong, change it all back
                    ChangeDisplaySettings( NULL, 0 );
                }

                // Mode is found so get out of loop
                modeFound = true;
            }
        
            ++mode;
        }
    }

    // Create the window
    hWnd = CreateWindowEx( dwExStyle, windowName, windowTitle,
                           dwStyle, rect.left, rect.top, 
                           rect.right, rect.bottom,
                           NULL, NULL, hInstance, NULL );

    return hWnd;

}   // Create3DWind


/************************************************************************                                                             
*    DESCRIPTION:  Create the 3D device for rendering
*
*    Input:    int rendDevice  - Render device
*              float fova - field of view angle in degrees
*              float minZDist - Minimum Z Distance
*              float maxZDist - Maximum Z Distance 
************************************************************************/
bool C3DWindow::Create3DDevice( int rendDevice, float fova, 
                                float minZDist, float maxZDist )
{
    // Set class members
    giRenderDevice = rendDevice;
    viewAngle = fova;
    gfMinZDistance = minZDist;
    gfMaxZDistance = maxZDist;
    aspectRatio = (float)width / (float)height;
    gfSquarePercentage = (float)height / (float)width;
    gfFrustrumYRatio = gfSquarePercentage / aspectRatio;

    if( giRenderDevice == ERD_SOFTWARE )
    {
        return InitSoftwareDevice();
    }
    else if( giRenderDevice == ERD_DIRECTX )
    {
        return InitDirectXDevice();
    }
    else if( giRenderDevice == ERD_OPENGL )
    {
        return InitGLDevice();
    }

    return false;

}   // Create3DDevice


/************************************************************************
*    DESCRIPTION:  Use OpenGL to render the 3D
*
*    Output:   bool - true on success, false on error
************************************************************************/
bool C3DWindow::InitGLDevice()
{
    bool result = false;
    PIXELFORMATDESCRIPTOR pfd;
    int pixelFormat;

    // Get the device context to the parent window
    if( hDC == NULL )
    {
        hDC = GetDC( hWnd );
    }

    // Make sure we have a screen DC
    if( hDC == NULL )
    {
        PostMsg( "Init Error", "Can't get a DC from the window handle. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Clear the pixel format structure
    memset( &pfd, 0, sizeof(PIXELFORMATDESCRIPTOR) );

    // Set some of the members
    pfd.nSize       = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion    = 1;
    pfd.dwFlags     = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType  = PFD_TYPE_RGBA;
    pfd.cColorBits  = 32;
    pfd.cDepthBits  = 32;
    pfd.iLayerType  = PFD_MAIN_PLANE;
    
    // Choose the best matching pixel format
    pixelFormat = ChoosePixelFormat( hDC, &pfd );

    // Can't be zero
    if( pixelFormat == 0 )
    {
        PostMsg( "Init Error", "Error choosing pixel format. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Fill in the blanks to completely describe the pixel
    if( DescribePixelFormat( hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd ) == 0 )
    {
        PostMsg( "Init Error", "Error describing the pixel format. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }
    
    // Set the pixel format to the device context
    if( !SetPixelFormat( hDC, pixelFormat, &pfd ) )
    {
        PostMsg( "Init Error", "Error setting pixel format. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    } 
    
    // Create the rendering contex
    hOpenGL_RC = wglCreateContext( hDC );

    // Can't be NULL
    if( hOpenGL_RC == NULL )
    {
        PostMsg( "Init Error", "Error creating OpenGL Rendering Contex. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    if( !wglMakeCurrent( hDC, hOpenGL_RC ) )
    {
        PostMsg( "Init Error", "Error making OpenGL Rendering Contex current. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Set the viewport
    glViewport( 0, 0, width, height );

    // Set the 3D viewport
    SetOpenGL_3D_ViewPort();

    // Enable z buffering
    glEnable( GL_DEPTH_TEST );

    // Enable texturing
    glEnable( GL_TEXTURE_2D );

    // Clear the render buffer
    glClear( GL_COLOR_BUFFER_BIT );

	// Setup the blending math if alpha blending is enabled
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// Show the cleared buffer
    SwapBuffers( hDC );

    result = true;

HANDLE_ERROR:;

    return result;

}   // InitGLwindow


/************************************************************************
*    DESCRIPTION:  Use DirectX to render the 3D
*
*    Output:   bool - true on success, false on error
************************************************************************/
bool C3DWindow::InitDirectXDevice()
{
    bool result = false;
    #if defined(_USE_DIRECTX_8_) || defined(_USE_DIRECTX_9_)
    D3DPRESENT_PARAMETERS dxpp;
    D3DDISPLAYMODE dxdm;
    D3DXMATRIX dxMatrix;
    HRESULT hresult;

    // Create the DirectX 9 instance
    #ifdef _USE_DIRECTX_9_
    pDXInstance = Direct3DCreate9( D3D_SDK_VERSION );
    #endif
    #ifdef _USE_DIRECTX_8_
    pDXInstance = Direct3DCreate8( D3D_SDK_VERSION );
    #endif

    if( pDXInstance == NULL )
    {
        PostMsg( "Init Error", "Error creating an instance of DirectX9. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Get info about the video card
    pDXInstance->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &dxdm );

    // clear out the structure
    memset( &dxpp, 0, sizeof(dxpp) );

    // Fill out the remaining items of the structure
    if( displayMode == EDM_WINDOWED )
    {
        dxpp.Windowed = true;
    }
    else
    {
        dxpp.Windowed = false;
    }

    dxpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    dxpp.BackBufferFormat       = dxdm.Format;
    dxpp.EnableAutoDepthStencil = true;
    dxpp.BackBufferWidth        = width;
    dxpp.BackBufferHeight       = height;
    dxpp.AutoDepthStencilFormat = D3DFMT_D24X8;//D3DFMT_D16;
    #ifdef _USE_DIRECTX_9_
    dxpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
    #endif

    // Create the device
    hresult = pDXInstance->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                         D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                         &dxpp, &g_pDXDevice );

    if( hresult == D3DERR_DEVICELOST )
    {
        PostMsg( "Init Error", "Error creating DirectX9 device. Device has been lost." );
        goto HANDLE_ERROR;
    }
    else if( hresult == D3DERR_INVALIDCALL )
    {
        PostMsg( "Init Error", "Error creating DirectX9 device. Invalid paramter call." );
        goto HANDLE_ERROR;
    }
    else if( hresult == D3DERR_NOTAVAILABLE )
    {
        PostMsg( "Init Error", "Error creating DirectX9 device. Unsupported Queried Technique." );
        goto HANDLE_ERROR;
    }
    else if( hresult == D3DERR_OUTOFVIDEOMEMORY )
    {
        PostMsg( "Init Error", "Error creating DirectX9 device. Out of video memory." );
        goto HANDLE_ERROR;
    }
    else if( hresult != D3D_OK )
    {
        PostMsg( "Init Error", "Error creating DirectX9 device. Unknow error." );
        goto HANDLE_ERROR;
    }

    // Turn off D3D lighting, since we are providing our own vertex colors
    g_pDXDevice->SetRenderState(D3DRS_LIGHTING, false);

    // Turn off culling, so we see the front and back of the polygons
    g_pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    // Turn on the zbuffer
    g_pDXDevice->SetRenderState( D3DRS_ZENABLE, true );

    //g_pDXDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
    g_pDXDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );


	//alpha blending enabled
	//g_pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	//source blend factor		
	g_pDXDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	//destination blend factor
	g_pDXDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	//alpha from texture
	g_pDXDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);


    // Set the perspective
    D3DXMatrixPerspectiveFovLH( &dxMatrix, D3DXToRadian( viewAngle ), aspectRatio, gfMinZDistance, gfMaxZDistance );
    g_pDXDevice->SetTransform(D3DTS_PROJECTION, &dxMatrix);

    // Create the triangle vertex buffer.
    #ifdef _USE_DIRECTX_9_
    if( D3D_OK != g_pDXDevice->CreateVertexBuffer( MAX_VERTEX_BUFFER * sizeof(CDirectXVert),
                                                   D3DUSAGE_WRITEONLY, D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1,
                                                   D3DPOOL_DEFAULT, &g_DirectX_VB, NULL ) )
    #endif
    #ifdef _USE_DIRECTX_8_
    if( D3D_OK != g_pDXDevice->CreateVertexBuffer( MAX_VERTEX_BUFFER * sizeof(CDirectXVert),
                                                   D3DUSAGE_WRITEONLY, D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1,
                                                   D3DPOOL_DEFAULT, &g_DirectX_VB ) )
    #endif
    {
        PostMsg( "Init Error", "Error creating triangle vertex buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Clear the depth buffer
    g_pDXDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

	// Display the cleared screen
    g_pDXDevice->BeginScene();
	g_pDXDevice->EndScene();
    g_pDXDevice->Present( NULL, NULL, NULL, NULL );

    result = true;

HANDLE_ERROR:;

    #endif
    return result;
}   // InitDirectXDevice


/************************************************************************
*    DESCRIPTION:  Use Software to render the 3D
*
*    Output:   bool - true on success, false on error
************************************************************************/

bool C3DWindow::InitSoftwareDevice()
{
    bool result = false;
    int bitCount;

    // Do some inits
    giBufWidth = width;
    giBufHeight = height;
    gfBufWidthDiv2 = width / 2;
    gfBufHeightDiv2 = height / 2;
    gfCameraScaleX = -((float)width * 0.9);//885
    gfCameraScaleY = -gfCameraScaleX;

    // Get the device context to the parent window
    if( hDC == NULL )
    {
        hDC = GetDC( hWnd );
    }

    // Make sure we have a screen DC
    if( hDC == NULL )
    {
        PostMsg( "Init Error", "Can't get a DC from the window handle. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Allocate the Z Buffer
    giZBuffer = new int[ giBufWidth * giBufHeight ];
    if( giZBuffer == NULL )
    {
        PostMsg( "Init Error", "Can't allocate Z buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Zero out the ZBuffer
    memsetQuad( giZBuffer, 0, giBufWidth * giBufHeight );

    #ifdef _16_BIT_SOFTWARE_RENDERING_
    bitCount = BIT_COUNT_16;
    #else
    bitCount = BIT_COUNT_32;
    #endif

    // Create the video buffer
    if( !buffer.CreateDIBSectionBuffer( giBufWidth, -giBufHeight, bitCount ) )
    {
        PostMsg( "Init Error", "Can't allocate background buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    gusiVBufer = (unsigned int *)buffer.GetPtrToDIBsectionBits();

    #ifdef _16_BIT_SOFTWARE_RENDERING_
    // Allocate the shaded lookup table
    gusiShadeTbl = new unsigned short int[ SHADE_COUNT * COLOR_COUNT_16_BIT ];
    if( gusiShadeTbl == NULL )
    {
        PostMsg( "Init Error", "Can't allocate shaded lookup table. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Generate the 16 bit shaded table
    Generate16BitShadedTable( gusiShadeTbl );
    #endif

    result = true;

HANDLE_ERROR:;

    return result;

}   // InitSoftwareDevice


/************************************************************************
*    DESCRIPTION:  Set the OpenGL view port to 3D rendering
************************************************************************/
void C3DWindow::SetOpenGL_3D_ViewPort()
{
    // Set projection matrix
    glMatrixMode( GL_PROJECTION );

    // load the identy matrix
    glLoadIdentity();

    // calculation aspect ratio of window
    gluPerspective( viewAngle, (GLfloat)aspectRatio, gfMinZDistance, gfMaxZDistance );

    // Set projection matrix
    glMatrixMode( GL_MODELVIEW );

    // load the identy matrix
    glLoadIdentity();

}   // SetOpenGL3DViewPort


/************************************************************************
*    DESCRIPTION:  Set the OpenGL view port to 2D rendering
************************************************************************/
void C3DWindow::SetOpenGL_2D_ViewPort()
{
    // Set projection matrix
    glMatrixMode( GL_PROJECTION );

    // load the identy matrix
    glLoadIdentity();

    // calculation aspect ratio of window
    glOrtho( 0.0, (float)width - 1.0, (float)height - 1.0, 0.0, -1.0, 1.0 );

    // Set projection matrix
    glMatrixMode( GL_MODELVIEW );

    // load the identy matrix
    glLoadIdentity();

}   // SetOpenGL3DViewPort


/************************************************************************
*    DESCRIPTION:  Get the render device
************************************************************************/
int C3DWindow::GetRenderDevice()
{ 
    return giRenderDevice;

}   // GetRenderDevice


/************************************************************************
*    DESCRIPTION:  Get the render buffer
************************************************************************/
unsigned int * C3DWindow::GetBufferBits()
{
    return gusiVBufer;
}


/************************************************************************
*    DESCRIPTION:  Get the render buffer
************************************************************************/
int * C3DWindow::GetZBufferBits()
{
    return giZBuffer;
}


/************************************************************************
*    DESCRIPTION:  Get the DirectX device
************************************************************************/
#if defined(_USE_DIRECTX_8_) || defined(_USE_DIRECTX_9_)
#ifdef _USE_DIRECTX_9_
    LPDIRECT3DDEVICE9 C3DWindow::GetDirectXDevice()
#endif
#ifdef _USE_DIRECTX_8_
    LPDIRECT3DDEVICE8 C3DWindow::GetDirectXDevice()
#endif
{
    return g_pDXDevice;
}
#endif


/************************************************************************
*    DESCRIPTION:  Get the minimum distance
*
*    Output:   LPDIRECT3DDEVICE9 - DirectX device
************************************************************************/
float C3DWindow::GetMinDistance()
{
    return gfMinZDistance;
}


/************************************************************************
*    DESCRIPTION:  Get the max distance
*
*    Output:   LPDIRECT3DDEVICE9 - DirectX device
************************************************************************/
float C3DWindow::GetMaxDistance()
{
    return gfMaxZDistance;
}

/************************************************************************                                                             
*    DESCRIPTION:  Set the caption for the window
*
*    Input:    char * pCaption  - Render device
************************************************************************/
void C3DWindow::SetWindowCaption( char * pCaption )
{
	SetWindowText( hWnd, pCaption );

}	// SetWindowCaption