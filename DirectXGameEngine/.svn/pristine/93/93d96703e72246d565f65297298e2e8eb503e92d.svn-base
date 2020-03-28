
/************************************************************************
*    FILE NAME:       gamewnd.h
*
*    DESCRIPTION:     game window
************************************************************************/

#ifndef __gamewnd_h__
#define __gamewnd_h__

// DirectX lib dependencies
#include <d3dx9.h>

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <common/winapidefs.h>
#include <common/size.h>
#include <utilities/timer.h>

// Forward declaration(s)
class CEffectData;

class CGameWnd
{
public:

    // Initilization function
    bool OnCreate( HWND hwnd, CREATESTRUCT* lpCreateStruct );

    // Standard Windows OnClose message handler.
    void OnClose( HWND hwnd );

    // Standard Windows OnSize message handler.
    void OnSize(HWND hwnd, UINT state, int cx, int cy);

    // window Frame pain message
    LRESULT OnNcPaint(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

    // On activate app message handler
    void OnActivateApp(HWND hwnd, BOOL fActivate, DWORD dwThreadId);

    // Game loaded windows message
    void OnLoadComplete(){ gameLoadComplete = true; }

    // Game loading message
    void OnLoading(){ gameLoadComplete = false; }

    // Register and create the parent window
    void Create( HINSTANCE hInstance, winCallbackFuncPtrType pWinCallback );

    // Reset the window size based on user settings
    void ResetWindowSize();

    // Get the handle to the game window
    HWND GetHandle();

    // Main game loop. This function is called from the window message loop
    void GameLoop();

    // Is the game running?
    bool IsGameRunning();

    // Start the game
    void StartGame();

    // Stop the game
    void StopGame();

    // Load game resources here
    virtual void LoadGameObjects();

    // Delete the allocated game objects
    virtual void DeleteGameObjects();

    // Handle the state change
    virtual void DoStateChange() = 0;

    // Act upon what the user is doing
    virtual void GetUserInput() = 0;

    // Check for collision and react to it
    virtual void ReactToCollision() = 0;

    // Update animations, Move sprites, Check for collision
    virtual void Update() = 0;

    // Transform game objects
    virtual void Transform() = 0;

    // Render to the shadow buffer
    virtual void RenderShadowBuffer() = 0;

    // Render of game content
    virtual void PreRender() = 0;

    // Post Render of game content
    virtual void PostRender() = 0;

    // Handle the device reset
    virtual void HandleDeviceReset( const CSize<float> & res, bool vSync, bool windowed ) = 0;

    // Exception Clean-up function
    void ExceptionCleanUp();

    // Does the game window have the focus?
    bool HasFocus() const;

    // Get/Set pre render pause
    bool IsPreRenderPause() const;
    void SetPreRenderPause( bool value = true );

    // Set the caption for the window
    void SetWindowCaption( const std::string & caption );

protected:

    // Constructor
    CGameWnd();

    // Destructor
    virtual ~CGameWnd();

    // All windows created with CreateWindow must be registered first.
    // This does not include windows controls like buttons, listboxex and such
    // because they register themselves
    void RegisterWnd( HINSTANCE hInstance, winCallbackFuncPtrType pWinCallback );

    // Create the parent window
    void Create( HINSTANCE hInstance );

    // Create the window
    void CreateGameWindow( const HINSTANCE hInst );

    // Center the mouse pointer to the game window
    void CalcMouseCenterPoint();

    // Render the post process
    virtual void RenderPostProcess();

    // Enable/disable post processing
    void EnablePostProcessing( bool value = true );

    // See if we are to do any post process
    virtual void PostProcessDecision() = 0;

    // Handle the effects for the post process
    virtual void HandlePostProcessEffects() = 0;

    // Post processing strings
    void SetPostProcStrings( CEffectData * _pEffectData,
                             const std::string & _postProcTechIdStr,
                             const std::string & _postProcTextureIdStr );
    void SetPostProcStrings( CEffectData * _pEffectData,
                             const std::string & _postProcTechIdStr );
    void SetPostProcStrings( const std::string & _postProcEffectFileIdStr,
                             const std::string & _postProcTechIdStr,
                             const std::string & _postProcTextureIdStr );
    void SetPostProcStrings( const std::string & _postProcEffectFileIdStr,
                             const std::string & _postProcTechIdStr );
    void SetPostProcStrings( const std::string & _postProcTechIdStr );

    // Get the post process tech ID string
    std::string & GetPostProcessTechIdStr(){ return postProcTechIdStr; }

    // Get the post process effect data
    CEffectData * GetPostProcEffectDataPtr();

    // Center point to judge position to
    POINT mouseCenterPt;

    // Do the rendering
    virtual void Render();

private:

    // Application instance
    HINSTANCE hInst;

    // handle to window
    HWND hWnd;

    // flag to indicate the game is running
    bool gameRunning;

    // Flag to indicate the load is complete
    bool gameLoadComplete;

    // Enable post process
    bool enablePostProcess;

    // post process strings
    std::string postProcTechIdStr;
    std::string postProcTextureIdStr;

    // Pointer to the effects data
    // NOTE: This class does not own this pointer.
    CEffectData * pPostProcEffectData;

    // Flag to indicate if the game window has focus
    bool hasFocus;

    // Stats display timer
    CTimer statsDisplayTimer;

    // Pre-render pause flag
    bool preRenderPause;
};

#endif  // __gamewnd_h__
