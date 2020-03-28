
/************************************************************************
*
*    FILE NAME:       C3DWindow.h
*
*    DESCRIPTION:     For creating and managing the 3d Window and device 
*
************************************************************************/

#ifndef _C3D_WINDOW_H_
#define _C3D_WINDOW_H_

#include "types.h"
#include "bitmap.h"         // Dib and Bitmap class for displaying and animating bitmaps

// DirectX may or maynot be turned on
#ifdef _USE_DIRECTX_9_
    #include "d3dx9.h"
#endif
#ifdef _USE_DIRECTX_8_
    #include "d3dx8.h"
#endif


/************************************************************************
*                         C3DWindow Object
************************************************************************/

class C3DWindow
{
public:

    C3DWindow();
    virtual ~C3DWindow();

    // 3D Window has already been created so init object
    void Init3DWind( int w, int h, HWND hWindow, HDC hWinDC,
                     HINSTANCE hInst, EDM_DISPLAY_MODE disMode,
                     bool windowCleanup = false );

    // Create the window
    HWND Create3DWind( int w, int h, HINSTANCE hInst,
                       EDM_DISPLAY_MODE disMode, 
                       char *windowName, char *windowTitle );
    // Create the 3D device
    bool Create3DDevice( int rendDevice, float fova, 
                         float minZDist, float maxZDist );
    // Free all allocations
    void Free();

    // Set the type of view port
    void SetOpenGL_3D_ViewPort();
    void SetOpenGL_2D_ViewPort();

	// Set the caption for the window
	void SetWindowCaption( char * pCaption );

    // Access functions
    HWND GetWndHandle(){ return hWnd; };
    HDC GetDCHandle(){ return hDC; };
    int GetWidth(){ return width; };
    int GetHeight(){ return height; };
    HGLRC GetOpenGL_RC(){ return hOpenGL_RC; };
    int GetRenderDevice();
    EDM_DISPLAY_MODE GetDisplayMode(){ return displayMode; };
    unsigned int *GetBufferBits();
    int *GetZBufferBits();
    void DrawSoftDevice(){ buffer.Draw( hDC ); };
    HDC GetBufferDCHandle(){ return buffer.Handle(); };
	#ifdef _USE_DIRECTX_9_
		LPDIRECT3DDEVICE9 GetDirectXDevice();
	#endif
	#ifdef _USE_DIRECTX_8_
		LPDIRECT3DDEVICE8 GetDirectXDevice();
	#endif
    float GetMinDistance();
    float GetMaxDistance();
    float GetAspectRatio(){ return aspectRatio; };
    float GetViewAngle(){ return viewAngle; };

protected:

    // Render size
    int width;
    int height;
    // field of view angle
    float viewAngle;
    // Display Mode
    EDM_DISPLAY_MODE displayMode;
    // OpenGL Rendering Contex
    HGLRC hOpenGL_RC;
    // device contex
    HDC hDC;
    // program instance
    HINSTANCE hInstance;
    // handle to window
    HWND hWnd;
    // Image buffers
    CBitmap buffer;
    // DirectX instance
	#ifdef _USE_DIRECTX_9_
		LPDIRECT3D9 pDXInstance;
	#endif
	#ifdef _USE_DIRECTX_8_
		LPDIRECT3D8 pDXInstance;
	#endif    
    // Full screen mode change flag
    bool fullScreenModeChange;
    // aspect ratio
    float aspectRatio;
    // do we manage the window cleanup
    bool manageWindowCleanup;  

    // Use OpenGL to render the 3D
    bool InitGLDevice();
    // Use Software to render the 3D
    bool InitSoftwareDevice();
    // Use DirectX to render the 3D
    bool InitDirectXDevice();
};


#endif // _C3D_WINDOW_H_