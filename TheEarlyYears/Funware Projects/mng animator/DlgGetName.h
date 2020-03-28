/************************************************************************
*
*    PRODUCT:         Get Name Dialog box
*
*    FILE NAME:       DlgGetName.h
*
*    DESCRIPTION:     Dialog box for getting a name
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __DLG_GET_NAME_H__
#define __DLG_GET_NAME_H__

#include "TStrList.h"        // List object for storing lists of strings

// Function for creating the Registration Number dialog box
BOOL Display_GetNameDlg( HINSTANCE hInstance, HWND hwnd, char *fileName, char *filePath, PTStringList pStringList );
// Message handler for the Info text setup dialog box
LRESULT CALLBACK GetNameDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
// Call back for subclassing any of the controls procedure
LRESULT CALLBACK SubClassEdtProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam );


class CGetNameDlg
{
protected:
    // Program instance
    HINSTANCE hInst;
    // Parent window
    HWND hParentWnd;
    // Dialog Window
    HWND hDlgWnd;
    // File name to create
    char *fileName;
	// File path
	char *filePath;
    // Pointer to string list
    PTStringList pList;
    // Subclass the edit control
    HWND hSubEdtCtrl;
    // Matching windows procedure for subclassed control
    WNDPROC OldSubEdtCtrlProc;

    // Check the entered filename
    BOOL CheckFileName( HWND hwnd );

public:
    // Constructor
    CGetNameDlg();
    // Destructor
    ~CGetNameDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    BOOL CreateDialogWnd( HINSTANCE hInstance, HWND hwnd, char *fName, char *path, PTStringList pStrList );
    // Call back function for edit control
    LRESULT OnEdit_GetName( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam );

    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef CGetNameDlg *PCGetNameDlg;

#endif  // __DLG_GET_NAME_H__
