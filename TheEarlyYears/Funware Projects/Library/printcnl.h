/************************************************************************
*
*    PRODUCT:         PrintCancel Dialog
*
*    FILE NAME:       printcnl.h
*
*    DESCRIPTION:     PrintCancel Dialog
*
*    IMPLEMENTATION:  PrintCancel Dialog
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __DLG_PRINTCANCEL_H__
#define __DLG_PRINTCANCEL_H__


// Function for creating the dialog
HWND DisplayPrintCancelDlg( HINSTANCE hInst, HWND hwnd, char *StatusTxt, BOOL ACenterToParent = FALSE );
// Message handler for the Info text setup dialog box
LRESULT CALLBACK PrintCancelDlg_DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
// Printer Abort function
BOOL CALLBACK PrintCancelDlg_AbortFunc( HDC hdc, int error );
// Standard Windows OnCommand message handler
void PrintCancelDlg_UpdateStatus( char *Text, ... );
// Should the printing continue
BOOL PrintCancelDlg_GetPrintCancel();


class TPrintCnlDlg
{
protected:
    // String indicating the status of the print job
    char *PrintStatus;
    // Handle to the dialog
    HWND hPrintCnlDlg;
    // Flag to center the dialog window
    BOOL CenterToParent;
    // Handle to parent window
    HWND hParentWnd;

public:
    // Constructor
    TPrintCnlDlg();
    // Destructor
    ~TPrintCnlDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    HWND CreateDialogWnd( HINSTANCE hInst, HWND hwnd, char *StatusTxt, BOOL ACenterToParent );
    // Get the hangle to the dialog
    //HWND GetHandle(){ return hPrintCnlDlg; };
    // Get the value of the print cancel
    //BOOL GetPrintCancel(){ return PrintCancel; };
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TPrintCnlDlg *PTPrintCnlDlg;



#endif  // __PRINTCANCEL_H__