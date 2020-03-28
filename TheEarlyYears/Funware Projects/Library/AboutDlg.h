/************************************************************************
*
*    PRODUCT:         Template Dialog
*
*    FILE NAME:       DlgTemplate.h
*
*    DESCRIPTION:     Template Dialog
*
*    IMPLEMENTATION:  Template Dialog
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __DLG_ABOUT_H__
#define __DLG_ABOUT_H__

#include "dib.h"             // Dib and Bitmap class for displaying and animating bitmaps

#define ABOUT_DIB_OFFSET   10
#define ABOUT_DEFAULT_WIDTH  170


// Function for creating the Registration Number dialog box
BOOL DisplayAboutDlg( HINSTANCE hInst, HWND hwnd, char *pCaption, char *pAbout, char *pResName, char *pResType, BYTE *buffer );
// Message handler for the Info text setup dialog box
LRESULT CALLBACK AboutDlg_DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );


class TAboutDlg
{
protected:
    // Text to be displayed
    char *pDlgCaption, *pTxtAboutMsg;
    // Dib Class
    TDIB AboutDib;
    // Parent window
    HWND hParentWnd;

public:
    // Constructor
    TAboutDlg();
    // Destructor
    ~TAboutDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Standard Windows OnPaint message handler
    void OnPaint( HWND hwnd );
    // Create the dialog box
    BOOL CreateDialogWnd( HINSTANCE hInst, HWND hwnd, char *pCaption, char *pAbout, char *pResName, char *pResType, BYTE *buffer );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TAboutDlg *PTAboutDlg;

#endif  // __DLG_ABOUT_H__


/*

RESOURCE FOR ABOUT DIALOG

DLG_ABOUT DIALOG 0, 0, 240, 191
EXSTYLE WS_EX_DLGMODALFRAME
STYLE DS_MODALFRAME | DS_3DLOOK | DS_CENTER | WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU
CAPTION ""
FONT 8, "MS Sans Serif"
{
 CONTROL "OK", BTN_OK, "BUTTON", BS_DEFPUSHBUTTON | BS_CENTER | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 100, 168, 42, 14
 CONTROL "", TXT_ABOUT, "static", SS_CENTER | WS_CHILD | WS_VISIBLE, 8, 140, 224, 24, 0
 CONTROL "Goto Web Site...", BTN_GOTO_WEB_SITE, "button", BS_PUSHBUTTON | BS_CENTER | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 148, 168, 68, 14, 0
}

*/