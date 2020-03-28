/************************************************************************
*
*    FILE NAME:       DoNothingDlg.h
*
*    DESCRIPTION:     Template Dialog
*
************************************************************************/


#ifndef __DLG_DONOTHING_H__
#define __DLG_DONOTHING_H__


// Function for creating the Registration Number dialog box
bool Display_DoNothingDlg( HINSTANCE hInstance, HWND hwnd );
// Message handler for the Info text setup dialog box
LRESULT CALLBACK DoNothingDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );


class CDoNothingDlg
{
protected:
    // Program instance
    HINSTANCE hInst;
    // Parent window
    HWND hParentWnd;

public:
    // Constructor
    CDoNothingDlg();
    // Destructor
    ~CDoNothingDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    bool OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    bool CreateDialogWnd( HINSTANCE hInstance, HWND hwnd );
};

#endif  // __DLG_DONOTHING_H__
