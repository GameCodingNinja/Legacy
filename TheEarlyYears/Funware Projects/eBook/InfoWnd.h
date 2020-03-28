/************************************************************************
*
*    PRODUCT:         Art4Sale
*
*    FILE NAME:       InfoWndSetup.h
*
*    DESCRIPTION:     Info window setup dialog
*
*    IMPLEMENTATION:  Dialog box that helps the user setup their info windows
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __INFOWNDSETUP_H__
#define __INFOWNDSETUP_H__


// Function for creating the Info text setup dialog box
BOOL DisplayInfoTxtSetupDlg( HINSTANCE hInst, HWND hwnd, char *DlgCaption,
                             char *InfoWndCaption, char *InfoWndTitle, char *InfoWndTxtFilePath );
// Message handler for the Info text setup dialog box
LRESULT CALLBACK InfoTxtDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );


class TInfoSetupDlg
{
protected:
    char *pInfoWndTitle;
    char *pInfoWndCaption;
    char *pInfoWndTxtFilePath;
    char *pDlgCaption;
    // Parent window
    HWND hParentWnd;

    // Saves the text file to the hard drive
    void SaveTextFile( HWND hwnd );

public:
    // Constructor
    TInfoSetupDlg();
    // Destructor
    ~TInfoSetupDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Create the dialog box
    BOOL CreateDialogWnd( HINSTANCE hInst, HWND hwnd, char *DlgCaption,
                          char *InfoWndCaption, char *InfoWndTitle, char *InfoWndTxtFilePath );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TInfoSetupDlg *PTInfoSetupDlg;


#endif  // __INFOWNDSETUP_H__