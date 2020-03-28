
/************************************************************************
*
*    PRODUCT:         Quick Screen Capture
*
*    FILE NAME:       QScrnCapture.cpp
*
*    DESCRIPTION:     Screen Capture Program
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __Q_SCREEN_CAPTURE_H__
#define __Q_SCREEN_CAPTURE_H__

#include "Settings.h"        // Ini project settings file
#include "gTools.h"          // A misc group of useful functions

// Standard Windows message handler
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
// Callback timer function
void CALLBACK CheckForKeyPressTimer( HWND hwnd, UINT uTimerMsg, UINT uTimerID, DWORD dwTime );
// Callback timer function
void CALLBACK CheckForRunningTimer( HWND hwnd, UINT uTimerMsg, UINT uTimerID, DWORD dwTime );
// Parse the command line
void ParseCmndLine( HINSTANCE hInst, char *CndLine );


class CQScrnCaptWnd
{
protected:
    // Program instance
    HINSTANCE hInst;
    // Handle to parent window
    HWND hParentWnd;
    // Return code for the key press timer         
    int keyPressTimerReturnCode;
	// Return code for the running timer         
    int runningTimerReturnCode;
    // Flag to keep timer from re-entering image functions
    bool clickActive;
    // The camera click resource
    TLoadRes clickSndRes;
	// file counter
	int fileCounter;

    // Create the parent window
    BOOL CreateWnd( HINSTANCE hInstance, int nCmdShow );
    // Standard Windows OnClose message handler
    BOOL QuitProgram( HWND hwnd );
	// Make the bitmap
	void MakeBitmap( HDC hScrDC, HDC hBmpDc, HBITMAP hBitmap, int Width, int Height, int bitsPerPixel );
	// Update the info string
	void UpdateInfo( HWND hwnd );
	// Simple help screen
	void MnuClk_Help( HWND hwnd );
	// Create a running timer
	void CreateRunningTimer( HWND hwnd );

public:
	// Class for saving settings
	TSettings Settings;

    // Constructor
    CQScrnCaptWnd();
    // Destructor
    ~CQScrnCaptWnd();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnDestroy message handler
    void OnDestroy( HWND hwnd );
    // Standard Windows OnNotify message handler
    LRESULT OnNotify( HWND hwnd, int idFrom, NMHDR FAR * pnmhdr );
    // Register and create the parent window
    BOOL CreateParentWnd( HINSTANCE hInstance, int nCmdShow );
    // Get a handle to the parent window
    HWND GetParentWnd(){ return hParentWnd; };
    // Check for key press
    void CheckForKeyPress();
	// running timer
	void RunningTimerScreenShot();
	// Grab the windows desktop
	void GrabScreen( BOOL OnlyActWnd = FALSE );

    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef CQScrnCaptWnd *PCQScrnCaptWnd;


#endif  // __Q_SCREEN_CAPTURE_H__