/************************************************************************
*
*    PRODUCT:         Pref dialog box
*
*    FILE NAME:       DlgPrefs.h
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __DLG_PREFS_H__
#define __DLG_PREFS_H__


#include "Settings.h"        // Ini project settings file


// Function for creating the Registration Number dialog box
BOOL Display_PrefsDlg( HINSTANCE hInstance, HWND hwnd, PTSettings pSet );
// Message handler for the Info text setup dialog box
LRESULT CALLBACK PrefsDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
// Call back for subclassing any of the controls procedure
LRESULT CALLBACK SubClassEdtProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam );


class CPrefsDlg
{
protected:
    // Program instance
    HINSTANCE hInst;
    // Parent window
    HWND hParentWnd;
    // Dialog Window
    HWND hDlgWnd;
    // Matching windows procedure for subclassed control
    WNDPROC OldSubEdtCtrlProc;
    // The program settings
    PTSettings pSettings;

    // Allows the user to select a directory
    void BtnClk_SelectDirectory( HWND hwnd );
	// Save the user selection
	void SaveUserSelections( HWND hwnd );

public:
    // Constructor
    CPrefsDlg();
    // Destructor
    ~CPrefsDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    BOOL CreateDialogWnd( HINSTANCE hInstance, HWND hwnd, PTSettings pSet );
    // Call back function for edit control
    LRESULT OnEdit_GetName( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam );

    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef CPrefsDlg *PCPrefsDlg;

#endif  // __DLG_PREFS_H__
