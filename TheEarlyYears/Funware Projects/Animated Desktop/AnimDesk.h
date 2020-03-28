
/************************************************************************
*
*    PRODUCT:         Animated Desktop
*
*    FILE NAME:       AnimDesk.h
*
*    DESCRIPTION:     Program the runs small animations while the computer
*                     is on durring normal use 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __ANIM_DESK_H__
#define __ANIM_DESK_H__

#include "TBitmap.h"         // Dib and Bitmap class for displaying and animating bitmaps


// Standard Windows message handler
LRESULT CALLBACK WndProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK AnimatedWndProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
// Running thread for animation
unsigned long _stdcall ActiveThread( void *ptData );

// Active thread data
struct SThreadData
{
    BOOL active;            // Flag indicating that the screen saver is running
    DWORD threadID;         // Id to the screen saver thread
    HANDLE hThread;         // Handle the the thread
    DWORD animCycleTimeOut; // Number of miliseconds of each animation cycle
	DWORD animTimeOut;      // Number of seconds between each animation cycle
    BOOL pauseThread;       // Pause the thread
    TBitmap clean, dirty, art;	// Art and buffer
	RECT Source, Dest;
	int NumFrames;
	int FrameCounter;
	int Movement; 

    SThreadData()
    {
        active = TRUE;
        threadID = 0;
        hThread = NULL;
        animCycleTimeOut = 0;
		animTimeOut = 0;
        pauseThread = FALSE;
		Source.left = 0;
		Source.top = 0;
		Source.right = 126;
		Source.bottom = 134;
		Dest = Source;
		FrameCounter = 0;
		NumFrames = 4;
    }
};
typedef SThreadData *PSThreadData;


class CAnimDeskWnd
{
protected:
    // This programs Instance
    HINSTANCE hInst;
    // Handle to program window
    HWND hParentWnd;
    // Contains information that the system needs to process
    // taskbar notification area messages
    NOTIFYICONDATA notIconData;
    // Our popup menu
    HMENU hmenu, hSubMenu;
    // Cursor point struct
    // Keeps the icon notify message handler from have
    // to make this a zillion times
    POINT cursorPos;
    // Acyive thread data
    SThreadData tData;
	// Animateed Window
	HWND hAnimWnd;
	HDC hAnimDC;

    // All windows created with CreateWindow must be registered first.
    // This does not include windows controls like buttons, listboxex and such
    // because they register themselves
    BOOL RegisterWnd( HINSTANCE hInstance );
    // Create the parent window
    BOOL CreateWnd( HINSTANCE hInstance, int nCmdShow );
    // Setup the tray app
    BOOL SetUpTrayApp( HWND hwnd );
    // Kills the thread
    BOOL killTheThread( HWND hwnd );

public:
    // Constructor
    CAnimDeskWnd();
    // Destructor
    ~CAnimDeskWnd();

	BOOL SelfErase;
	int SelfEraceCounter;

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnCreate( HWND hwnd, CREATESTRUCT* lpCreateStruct );
    // Standard Windows OnDestroy message handler
    void OnDestroy( HWND hwnd );
    // Tray app icon notification message handler
    void OnIconNotic( HWND hwnd, WPARAM wParam, LPARAM lParam );
    // Register and create the parent window
    BOOL CreateParentWnd( HINSTANCE hInstance, int nCmdShow );
    // Windows timer to start animations
    void StartTimer( HWND hwnd );
    // Windows timer to run animations
    void RunAnimTimer( HWND hwnd );

	BOOL Animate( PSThreadData pData );
	void SetupAnimation( PSThreadData pData );
};

typedef CAnimDeskWnd *PCAnimDeskWnd;


#endif  // __ANIM_DESK_H__