/************************************************************************
*
*    PRODUCT:         Configeration dialog box
*
*    FILE NAME:       ConfigDlg.h
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __DLG_CONFIG_H__
#define __DLG_CONFIG_H__


// Function for creating the Registration Number dialog box
BOOL Display_ConfigDlg( HINSTANCE hInstance, HWND hwnd );
// Message handler for the Info text setup dialog box
LRESULT CALLBACK ConfigDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );


class CConfigDlg
{
protected:
    // Program instance
    HINSTANCE hInst;
    // Parent window
    HWND hParentWnd;

public:
    // Constructor
    CConfigDlg();
    // Destructor
    ~CConfigDlg();

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
    // Get the windows handle
    HWND GetWnd(){ return hParentWnd; };
};

typedef CConfigDlg *PCConfigDlg;

#endif  // __DLG_CONFIG_H__
