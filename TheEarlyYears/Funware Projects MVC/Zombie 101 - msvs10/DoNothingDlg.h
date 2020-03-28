/************************************************************************
*
*    PRODUCT:         Template Dialog box
*
*    FILE NAME:       DoNothingDlg.h
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


#ifndef __DLG_DONOTHING_H__
#define __DLG_DONOTHING_H__

#include "settings.h"

// Function for creating the Registration Number dialog box
BOOL Display_DoNothingDlg( HINSTANCE hInstance, HWND hwnd );
// Message handler for the Info text setup dialog box
LRESULT CALLBACK DoNothingDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );


class CDoNothingDlg
{
protected:
    // Program instance
    HINSTANCE hInst;
    // Parent window
    HWND hParentWnd;
	// settings
	CSettings settings;

public:
    // Constructor
    CDoNothingDlg();
    // Destructor
    ~CDoNothingDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    BOOL CreateDialogWnd( HINSTANCE hInstance, HWND hwnd );
};

typedef CDoNothingDlg *PCDoNothingDlg;

#endif  // __DLG_DONOTHING_H__
