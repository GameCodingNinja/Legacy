
/************************************************************************
*
*    PRODUCT:         Starter
*
*    FILE NAME:       Starter.h
*
*    DESCRIPTION:     The begining
*
*    IMPLEMENTATION: How it's done 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __STARTER_H__
#define __STARTER_H__


// Standard Windows message handler
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
// Callback timer function
void CALLBACK TimerCallBack( HWND hwnd, UINT uTimerMsg, UINT uTimerID, DWORD dwTime );


// #defines
#define MNU_ABOUT       1
#define MNU_EXIT        2  // Also the same value when the little X box is clicked
#define MNU_START_SS    3
#define MNU_RESET_COUNTER    4


class TStarterWnd
{
protected:
    HINSTANCE hInst;
    HWND hParentWnd;
    // Return code for the windows timer
    int TimerReturnCode;
	// Screen saver name
	char ssName[MAX_PATH+1];
	// Windows directory
	char winDir[MAX_PATH+1];
	// interval counter
	int intervalCounter;
	// handle to static
	HWND hStatic;

    // All windows created with CreateWindow must be registered first.
	// This does not include windows controls like buttons, listboxex and such
	// because they register themselves
    BOOL RegisterWnd( HINSTANCE hInstance );
    // Create the parent window
    BOOL CreateWnd( HINSTANCE hInstance, int nCmdShow );
	// Edit_DisplayValue
	void Edit_DisplayValue( HWND hCtrl, int Value );
	// Center the window
	void CenterWindow( HWND hwnd, int Width, int Height, HWND hParentWnd = NULL, BOOL IsChildWnd = FALSE );

public:
    // Constructor
    TStarterWnd();
    // Destructor
    ~TStarterWnd();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnCreate( HWND hwnd, CREATESTRUCT* lpCreateStruct );
    // Standard Windows OnDestroy message handler
    void OnDestroy( HWND hwnd );
    // Register and create the parent window
    BOOL CreateParentWnd( HINSTANCE hInstance, int nCmdShow );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
    // Timer Callback function
    void TimerCallback( HWND hwnd );
};

typedef TStarterWnd *PTStarterWnd;


#endif  // __STARTER_H__