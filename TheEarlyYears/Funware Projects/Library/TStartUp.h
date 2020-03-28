/************************************************************************
*
*    PRODUCT:         Art 4 Sale
*
*    FILE NAME:       TStartUpScrn.h
*
*    DESCRIPTION:     Start up screen
*
*    IMPLEMENTATION:
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __STARTUP_SCREEN_H__
#define __STARTUP_SCREEN_H__

#include "dib.h"             // Dib and Bitmap class for displaying and animating bitmaps


// Function for creating the startup screen window
HWND DisplayStartUpScreen( HINSTANCE hInst, HWND hwnd, char *pResName, char *pResType, BYTE *buffer = NULL );
// Message handler for the dialog box
LRESULT CALLBACK StartUpDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );


class TStartUpDlg
{
protected:
    // Dib Class
    TDIB StartUpDib;
    // Parent window
    HWND hParentWnd;

public:
    // Constructor
    TStartUpDlg();
    // Destructor
    ~TStartUpDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Standard Windows OnPaint message handler
    void OnPaint( HWND hwnd );
    // Create the dialog box
    HWND CreateDialogWnd( HINSTANCE hInst, HWND hwnd, char *pResName, char *pResType, BYTE *buffer );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TStartUpDlg *PTStartUpDlg;

/*
DLG_STARTUP_SCRN DIALOG 0, 0, 240, 120
STYLE WS_POPUP | WS_VISIBLE
CAPTION ""
FONT 8, "MS Sans Serif"
{
}
*/

#endif