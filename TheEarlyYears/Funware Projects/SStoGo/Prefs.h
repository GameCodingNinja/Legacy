/************************************************************************
*
*    PRODUCT:         Art 4 Sale
*
*    FILE NAME:       Preferences.h
*
*    DESCRIPTION:     Preferences dialog box
*
*    IMPLEMENTATION:  Dialog box for users to fill in their project info
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __SS2GO_PREFS_H__
#define __SS2GO_PREFS_H__

#include "SSToGo.h"           // Parent windows header file

enum
{
	S_SAVER_NAME=0,
	INSTALLER_NAME,
	MAX_SUBCLASS_CTRLS,
};

// Function for creating the orderform dialog box
BOOL DisplayPrefsDialogBox( HINSTANCE hInst, HWND hwnd, PTProjectPrefs ProjPrefs, PTActiveProjectData ActProj );
// Message handler for the orderform dialog box
LRESULT CALLBACK PrefsDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
// Call back for subclassing any of the controls procedure
LRESULT CALLBACK SubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam );


class TPrefsDlg
{
protected:
    PTProjectPrefs pProjPrefsData;
    PTActiveProjectData pActProj;
    // Parent window
    HWND hParentWnd;
    // Subclassed window control handles
    HWND hSubCtrl[ MAX_SUBCLASS_CTRLS ];
    // Matching windows procedure for subclassed controls
    WNDPROC OldSubCtrlProc[ MAX_SUBCLASS_CTRLS ];

    // Save information recorded in the prefs dialog box
    void SaveProjectPrefs( HWND hwnd );

public:
    // Constructor
    TPrefsDlg();
    // Destructor
    ~TPrefsDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    BOOL CreateDialogWnd( HINSTANCE hInst, HWND hwnd, PTProjectPrefs ProjPrefs, PTActiveProjectData ActProj );
    // Called from the subclassed control
    LRESULT HandleSubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TPrefsDlg *PTPrefsDlg;


#endif  // __SS2GO_PREFS_H__