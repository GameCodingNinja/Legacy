/************************************************************************
*
*    PRODUCT:         Art4Sale
*
*    FILE NAME:       CaptionWnd.h
*
*    DESCRIPTION:     Caption window setup dialog
*
*    IMPLEMENTATION:  Dialog box that helps the users add captions to the images
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __CAPTIONWND_H__
#define __CAPTIONWND_H__

// Function for creating the Info text setup dialog box
BOOL DisplayCaptionDlg( HINSTANCE hInst, HWND hwnd, char *pCaption );
// Message handler for the Info text setup dialog box
LRESULT CALLBACK CaptionDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );


class TCaptionDlg
{
protected:
    // Pointer to the string that will be saved
    char *pDlgCaption;
    // Parent window
    HWND hParentWnd;

public:
    // Constructor
    TCaptionDlg();
    // Destructor
    ~TCaptionDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    BOOL CreateDialogWnd( HINSTANCE hInst, HWND hwnd, char *pCaption );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TCaptionDlg *PTCaptionDlg;

#endif  // __CAPTIONWND_H__