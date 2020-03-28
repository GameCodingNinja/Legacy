/************************************************************************
*
*    PRODUCT:         Registration Number
*
*    FILE NAME:       RegNumDlg.h
*
*    DESCRIPTION:     Registration Number Dialog Box
*
*    IMPLEMENTATION:  Dialog for retreving the registration number
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __REGNUMDLG_H__
#define __REGNUMDLG_H__


// Function for creating the Registration Number dialog box
BOOL DisplayRegNumDlg( HINSTANCE hInst, HWND hwnd, char *DlgCaption, char *ProgTitle, 
                       char *ProgCost, char *TxtInfo, char *RegNum, char *CreditCardURL, BOOL ACenterToParent = FALSE );
// Message handler for the Info text setup dialog box
LRESULT CALLBACK RegNumDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );


class TRegNumDlg
{
protected:
    char *pDlgCaption;
    char *pTxtInfo;
    char *pRegNum;
    char *pProgTitle;
    char *pProgCost;
    char *pCreditCardURL; 
    HFONT TitleFont;
    // Parent window
    HWND hParentWnd;
    // Center window flag
    BOOL CenterToParent;
    // Program instance
    HINSTANCE hInst;

public:
    // Constructor
    TRegNumDlg( HINSTANCE hInstance, HWND hwnd );
    // Destructor
    ~TRegNumDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    BOOL CreateDialogWnd( char *DlgCaption, char *ProgTitle, char *ProgCost, char *TxtInfo, char *RegNum, char *CreditCardURL, BOOL ACenterToParent );
    // Check the registration number they keyed in
    BOOL CheckRegNum( HWND hwnd );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TRegNumDlg *PTRegNumDlg;


#endif  // __REGNUMDLG_H__