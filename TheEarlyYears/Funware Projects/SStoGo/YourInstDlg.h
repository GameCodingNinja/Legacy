/************************************************************************
*
*    PRODUCT:         Dialog box that tells them what file there
*                     installer is
*
*    FILE NAME:       YourInstDlg.cpp
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __DLG_YOUR_INSTALL_H__
#define __DLG_YOUR_INSTALL_H__


// Function for creating the Registration Number dialog box
BOOL Display_YourInstallerDlg( HINSTANCE hInstance, HWND hwnd );
// Message handler for the Info text setup dialog box
LRESULT CALLBACK YourInstallerDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );


class CYourInstallerDlg
{
protected:
    // Program instance
    HINSTANCE hInst;
    // Parent window
    HWND hParentWnd;
    // Dialog Window
    HWND hDlgWnd;

public:
    // Constructor
    CYourInstallerDlg();
    // Destructor
    ~CYourInstallerDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    BOOL CreateDialogWnd( HINSTANCE hInstance, HWND hwnd );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef CYourInstallerDlg *PCYourInstallerDlg;

#endif  // __DLG_YOUR_INSTALL_H__
