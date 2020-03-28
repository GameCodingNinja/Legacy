/************************************************************************
*
*    PRODUCT:         Move Item Dialog
*
*    FILE NAME:       TMoveItem.h
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


#ifndef __DLG_MOVEITEM_H__
#define __DLG_MOVEITEM_H__


// Function for creating the Registration Number dialog box
int DisplayMoveItemDlg( HINSTANCE hInst, HWND hwnd, HWND hCtrl, int MoveItem, int NumItems );
// Message handler for the Info text setup dialog box
LRESULT CALLBACK MoveItemDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );


class TMoveItemDlg
{
protected:
    int ItemToBeMoved, TotalNumItems;
    // Parent window
    HWND hParentWnd;
	// Handle to List control
	HWND hListControl;

public:
    // Constructor
    TMoveItemDlg();
    // Destructor
    ~TMoveItemDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    int CreateDialogWnd( HINSTANCE hInst, HWND hwnd, HWND hCtrl, int MoveItem, int NumItems );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TMoveItemDlg *PTMoveItemDlg;


#endif  // __DLG_MOVEITEM_H__