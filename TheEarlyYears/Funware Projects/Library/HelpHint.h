/************************************************************************
*
*    PRODUCT:         Screen saver
*
*    FILE NAME:       SSHelpHint.h
*
*    DESCRIPTION:     Screen saver Helpful hint dialog box
*
*    IMPLEMENTATION:  Dialog box to inform users about screen saver specific
*                     information and settings
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __SSHELPHINT_H__
#define __SSHELPHINT_H__


#define DONT_SHOW_CHECKBOX   NULL
#define DO_CHECK_SUM         TRUE
#define DONT_DO_CHECK_SUM    FALSE

// Function for creating the helpful hint dialog box
void DisplayHelpHintDialogBox( HINSTANCE hInst, HWND hwnd, char *ResName, char *Type,
                               char *HelpTitle, char *HelpWndCaption, BOOL *ShowHelp = NULL,
                               BOOL DoCheckSum = DONT_DO_CHECK_SUM, char *Buffer = NULL,
                               DWORD BufSize = 0, BOOL ACenterToParent = FALSE );
// Message handler for the helpful hint dialog box
LRESULT CALLBACK HelpHintDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK HelpHintEditProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );


class THelpHintDlg
{
protected:
    char *pHelpTitle;
    char *pHelpWndCaption;
    char *pResName;
    char *pType;
    char *pBuffer;
    BOOL *pShowHelp;
    BOOL CheckSum;
    BOOL CenterToParent;
    WNDPROC OldEditProc;
    HFONT TitleFont;
    DWORD BufferSize;
    HWND hParentWnd;
    HINSTANCE hInst;

public:
    // Constructor
    THelpHintDlg( HINSTANCE hInstance, HWND hwnd );
    // Destructor
    ~THelpHintDlg();

    // Used to load the text resource and do a checksum if required
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // On Command message handler for the helpful hint dialog box
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    BOOL CreateDialogWnd( char *ResName, char *Type, char *HelpTitle, char *HelpWndCaption, 
                          BOOL *ShowHelp, BOOL DoCheckSum, char *Buffer,
                          DWORD BufSize, BOOL ACenterToParent );
    // Get the static window procedure
    WNDPROC GetEditProc(){ return OldEditProc; };
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef THelpHintDlg *PTHelpHintDlg;


#endif  /* __SSHELPHINT__ */


/*

helpful hint dialog

DLG_HELPFUL_HINT DIALOG 0, 0, 257, 206
EXSTYLE WS_EX_DLGMODALFRAME
STYLE DS_MODALFRAME | DS_3DLOOK | DS_CENTER | WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU
CAPTION ""
FONT 8, "MS Sans Serif"
{
 CONTROL "Done", BTN_DONE, "BUTTON", BS_DEFPUSHBUTTON | BS_CENTER | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 204, 184, 46, 15
 CONTROL "", EDT_HELPFUL_HINT, "edit", ES_LEFT | ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_TABSTOP, 8, 24, 241, 152, 0
 CONTROL "Don't Show Again", CHK_DONT_SHOW_AGAIN, "button", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 8, 185, 84, 12, 0
 CONTROL "", TXT_HLP_TITLE, "static", SS_CENTER | WS_CHILD | WS_VISIBLE, 8, 3, 240, 16, 0
}
 
*/