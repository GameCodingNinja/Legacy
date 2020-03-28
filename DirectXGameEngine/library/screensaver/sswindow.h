/************************************************************************
*    FILE NAME:   sswindow.h
*
*    DESCRIPTION: For creating and managing a screen saver Window singlton 
************************************************************************/

#ifndef __ss_window_h__
#define __ss_window_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include "2d\\size.h"
#include "misc\\defs.h"

class CSSWindow
{
public:

	enum EScreenSaverMode
	{
		ESSM_NULL=0,
		ESSM_CFG,
		ESSM_PREV,
		ESSM_SAVER
	};

	// Create the parent Window
	bool CreateParentWnd( HINSTANCE hInstance,
						  std::string & cmdParam,
						  winCallbackFuncPtrType pWinCallback,
						  timerCallbackFuncPtrType pTimerCallBack );

	// Standard Windows OnCreate message handler
	bool OnCreate( HWND hwnd, LPCREATESTRUCT lpCreateStruct );

	// Standard Windows OnMouseMove message handler
	void OnMouseMove( HWND hwnd, int x, int y, UINT keyFlags );

	// Standard Windows OnClose message handler
	void OnClose( HWND hwnd );

	// Standard Windows OnDestroy message handler
	void OnDestroy( HWND hwnd );

	// Handles mouse or button presses
	void OnButton( HWND hwnd );

	// Load screen saver resources here
	virtual bool LoadSaverObjects() = 0;

    // Unload the allocated screen saver objects
	virtual void UnloadSaverObjects() = 0;

	// Screen Saver execution loop
	virtual void SaverLoop() = 0;

	// Reset the number of mouse ticks to ignore
    void ResetMouseTicks()
	{ mouseTicks = MOUSE_TICKS_TO_IGNOR; }

	// Reset the number of mouse ticks to ignore
    void SetSaverRunning( bool value = true )
	{ saverRunning = value; }

	// Reset the number of mouse ticks to ignore
    bool IsSaverRunning()
	{ return saverRunning; }

protected:

	// See if a semaphore already exists
	bool DoExist( std::string & semaphoreName );

	// All windows created with CreateWindow must be registered first
	bool RegisterWnd( winCallbackFuncPtrType pWinCallback );

	// If we are running the preview or a full screen  
    // window, the window is created here.
	bool CreateWnd( HWND hPreviewWnd = NULL );

	// Show/Hide the windows cursor
	void CursorShow( bool value = true );

	// Create the full screen saver window
	bool CreateFullScreenSaver( winCallbackFuncPtrType pWinCallback,
								timerCallbackFuncPtrType pTimerCallBack );

	// Create the preview Window
	bool CreatePreviewWnd( std::string & cmdParam,
						   winCallbackFuncPtrType pWinCallback );

	// Create the config Window
	void CreateConfigWnd( std::string & cmdParam );

protected:

	// Constructor
    CSSWindow();

	// Creat and display the config dialog box
	virtual void DisplayConfigDialogBox( HINSTANCE hInst, HWND hConfigParentWnd ){}

	// Access functions to class data

	// Get the handle to the saver window
	HWND GetWndHandle()
	{ return hWnd; }

	// Get the screen saver's instance
	HINSTANCE GetAppInstance()
	{ return hInst; }

	// Get the render window size
	CSize<int> & GetRenderWindSize()
	{ return renderWndSize; }

	// Get the display window size
	CSize<int> & GetDisplayWindSize()
	{ return displayWndSize; }

	// Get the saver mode
	EScreenSaverMode GetSaverMode()
	{ return screenSaverMode; }

	// Set the window name
	void SetWindName( std::string & nameStr )
	{ windowName = nameStr; }

	// Set small window debug mode
	void SetDebugMode( bool value = true )
	{ debugMode = value; }

	// Set small window debug mode
	void SetFullScreenDebug( bool value = true )
	{ fullScreenDebugWnd = value; }

	// Set the small window debug scaler
	void SetDebugWndScaler( float value )
	{ debugWndScaler = value; }

private:

	// Time to close the screen saver
	void CloseScreenSaver();

private:

	// Size of the render window
	CSize<int> renderWndSize;

	// Size of the display window
	CSize<int> displayWndSize;

	// Screen saver mode
	EScreenSaverMode screenSaverMode;

	// handle to this window
    HWND hWnd;

	// handle to this apps instance
	HINSTANCE hInst;

	// Class Name
	const std::string className;

	// Window Name
	std::string windowName;

	// Flag for more then one monitor
	bool multipleMonitors;

	// Flag to run in debug mode
	bool debugMode;

	// Flag for running in full screen debug window
	bool fullScreenDebugWnd;

	// Debug Window scaler
	float debugWndScaler;

	// Number of mouse ticks to ingnor
	const int MOUSE_TICKS_TO_IGNOR;

	// Mouse previous X
    int mousePrevX;

    // Mouse previous X          
    int mousePrevY;

    // Mouse nudge counts          
    int mouseTicks;

    // Return code for the windows timer to reset the mouse clicks         
    int timerReturnCode;

	// Mouse reset timer ID
	const uint MOUSE_RESET_TIMER_ID;

	// Flag to indicate the screen saver is running
	bool saverRunning;

	// Minimum mouse move amount
	const int MINIMUM_MOUSE_MOVE;

};


#endif // __ss_window_h__
