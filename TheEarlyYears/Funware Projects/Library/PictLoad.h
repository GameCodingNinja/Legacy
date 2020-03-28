/************************************************************************
*
*    PRODUCT:         Screen saver
*
*    FILE NAME:       PictLoad.h
*
*    DESCRIPTION:     Load picture dialog box
*
*    IMPLEMENTATION:  This is not a full dialog box but an extension to the
*                     common file open dialog box.
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __PICTLOAD_H__
#define __PICTLOAD_H__

#include "TBitmap.h"         // Dib and Bitmap class for displaying and animating bitmaps
#include "TGBase.h"          // The Graphics Base Object

// Function for creating the media sound directory dialog box
BOOL PictLoad_GetPicture( HINSTANCE hInst, HWND hwnd, char *Caption, DWORD Flags, char *FileName, int fBufSize, char *FileMask, char *GetDir = NULL );
// Hook Message handler function for common dialog box mods
UINT APIENTRY LoadPictDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );
// Subclass call back function for static window
LRESULT CALLBACK PictLoadStaticProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );


class TPictLoadDlg
{
protected:
    // Preview Buffer
    TBitmap PrevBuffer;
    // Loaded Bitmap
    TDIB PreviewImage;
    // Subclassed static procedure
    WNDPROC OldStaticWnd;

public:
    // Constructor
    TPictLoadDlg();
    // Destructor
    ~TPictLoadDlg();

    // Used to load the text resource and do a checksum if required
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // On Command message handler for the helpful hint dialog box
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Standard Windows OnDestroy message handler.
    void OnDestroy( HWND hwnd );
    // For trapping messages sent to the dialog box
    LRESULT OnNotify( HWND hwnd, int idFrom, NMHDR FAR * pnmhdr );
    // Standard Windows OnPaint message handler
    void OnPaint( HWND hwnd );
    // Create the dialog box
    BOOL CreateDialogWnd( HINSTANCE hInst, HWND hwnd, char *Caption, DWORD Flags, char *FileName, int fBufSize, char *FileMask, char *GetDir );
    // Get the static window procedure
    WNDPROC GetStaticProc(){ return OldStaticWnd; };
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TPictLoadDlg *PTPictLoadDlg;


/*

RESOURCE FOR PICT LOAD DIALOG

DLG_LOAD_PICT DIALOG 0, -1, 235, 119
STYLE DS_3DLOOK | DS_CONTROL | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS
FONT 8, "MS Sans Serif"
{
 CONTROL "", STC_PICTLOAD_IMAGE_WND, "static", SS_LEFT | SS_SUNKEN | WS_CHILD | WS_VISIBLE | WS_BORDER, 52, 0, 172, 104, 0
 CONTROL "", STC_PICTLOAD_INFO, "static", SS_LEFT | WS_CHILD | WS_VISIBLE, 52, 107, 172, 8, 0
}
*/

#endif  /* __PICTLOAD__ */