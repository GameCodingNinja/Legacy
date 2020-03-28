/************************************************************************
*
*    PRODUCT:         Status Bas window
*
*    FILE NAME:       status.h
*
*    DESCRIPTION:     Status Bar Dialog
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __StatusWnd_H__
#define __StatusWnd_H__


// Create the status window
HWND DisplayStatusWnd( HINSTANCE Instance, HWND hwnd, BOOL ACenterToParent = TRUE );
// Message handler for the dialog box
LRESULT CALLBACK StatusWnd_Proc( HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam );
// Set the status bars position
void StatusWnd_SetStatusBar( int StartPos, int MaxPos, int Inc );
// Move the status bar
void StatusWnd_MoveStatusBar();
// Destroy the status window
void StatusWnd_HideStatusWnd();
// Cycles the progress bar
void StatusWnd_BarDance();
// Set the window text
void StatusWnd_SetStatusTxt( char *T, ... );


class TStatusDlg
{
protected:
    // Handle to the dialog
    HWND hStatusWnd;
    // Parent window
    HWND hParentWnd;
    // Flag to center window
    BOOL CenterToParent;
    // Bar move inc
    int IncProgBar;

public:
    // Constructor
    TStatusDlg();
    // Destructor
    ~TStatusDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    HWND CreateDialogWnd( HINSTANCE Instance, HWND hwnd, BOOL ACenterToParent );
    // Get the hangle to the dialog
    HWND GetHandle(){ return hStatusWnd; };
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
    // Move the progress bar
    void MoveProgressBar();
    // Set the progress bar
    void SetProgressBar( int StartPos, int MaxPos, int Inc );
    // Set the bar to dance
    void MakeBarDance();
};

typedef TStatusDlg *PTStatusDlg;


#endif  /* __StatusWnd_H__ */



/*

DLG_STATUS_WND DIALOG 0, 0, 240, 50
EXSTYLE WS_EX_DLGMODALFRAME
STYLE DS_MODALFRAME | DS_3DLOOK | DS_CENTER | WS_POPUP | WS_VISIBLE | WS_BORDER
CAPTION ""
FONT 8, "MS Sans Serif"
{
 CONTROL "", PROG_BAR, "msctls_progress32", WS_CHILD | WS_VISIBLE | WS_BORDER, 8, 24, 224, 12, 0
 CONTROL "", STC_PROGBAR_TXT, "static", SS_LEFT | WS_CHILD | WS_VISIBLE, 8, 8, 224, 8, 0
}

*/