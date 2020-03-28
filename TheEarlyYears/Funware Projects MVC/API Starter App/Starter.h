
/************************************************************************

*    FILE NAME:       Starter.h
*
*    DESCRIPTION:     The begining
**
************************************************************************/

#ifndef __STARTER_H__
#define __STARTER_H__

// Standard Windows message handler
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );


class CStarterWnd
{
protected:
    HINSTANCE hInst;
    HWND hParentWnd;

    // All windows created with CreateWindow must be registered first.
	// This does not include windows controls like buttons, listboxex and such
	// because they register themselves
    bool RegisterWnd( HINSTANCE hInstance );
    // Create the parent window
    bool CreateWnd( HINSTANCE hInstance, int nCmdShow );

public:
    // Constructor
    CStarterWnd();
    // Destructor
    ~CStarterWnd();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    bool OnCreate( HWND hwnd, CREATESTRUCT* lpCreateStruct );
    // Standard Windows OnDestroy message handler
    void OnDestroy( HWND hwnd );
    // Register and create the parent window
    bool CreateParentWnd( HINSTANCE hInstance, int nCmdShow );
};

#endif  // __STARTER_H__
