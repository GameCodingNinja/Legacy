/************************************************************************
*
*    PRODUCT:         Move Page Dialog
*
*    FILE NAME:       MovePage.h
*
*    DESCRIPTION:     Template Dialog
*
*    IMPLEMENTATION:  MovePage Dialog
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __DLG_MOVEPAGE_H__
#define __DLG_MOVEPAGE_H__


// Function for creating the Registration Number dialog box
int DisplayMovePageDlg( HINSTANCE hInst, HWND hwnd, HWND hCtrl, int MovePage, int NumPages );
// Message handler for the Info text setup dialog box
LRESULT CALLBACK MovePageDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );


class TMovePageDlg
{
protected:
    int PageToBeMoved, TotalNumPages;
    // Parent window
    HWND hParentWnd;
	// Handle to List control
	HWND hListControl;

public:
    // Constructor
    TMovePageDlg();
    // Destructor
    ~TMovePageDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    int CreateDialogWnd( HINSTANCE hInst, HWND hwnd, HWND hCtrl, int MovePage, int NumPages );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TMovePageDlg *PTMovePageDlg;


#endif  // __DLG_MOVEPAGE_H__