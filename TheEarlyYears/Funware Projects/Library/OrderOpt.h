/************************************************************************
*
*    PRODUCT:         Ordering Options Dialog
*
*    FILE NAME:       OrderOpt.h
*
*    DESCRIPTION:     Simple dialog that asks if they want to order online 
*                     or by standard U.S. mail
*
*    IMPLEMENTATION:  Template Dialog
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __DLG_ORDER_OPTIONS_H__
#define __DLG_ORDER_OPTIONS_H__

enum
{
    NO_ORDER_OPTION_SELECTED=0,
    ONLINE_WEB_ORDER,
    STANDARD_US_MAIL_ORDER,
};

// Function for creating the Registration Number dialog box
int DisplayOrderOptionDlg( HINSTANCE hInst, HWND hwnd, BOOL ACenterToParent = FALSE );
// Message handler for the Info text setup dialog box
LRESULT CALLBACK OrderOption_DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );


class TOrderOptDlg
{
protected:
    // Center window flag
    BOOL CenterToParent;
    // Parent window
    HWND hParentWnd;

public:
    // Constructor
    TOrderOptDlg();
    // Destructor
    ~TOrderOptDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    int CreateDialogWnd( HINSTANCE hInst, HWND hwnd, BOOL ACenterToParent );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TOrderOptDlg *PTOrderOptDlg;


#endif  // __DLG_ORDER_OPTIONS_H__