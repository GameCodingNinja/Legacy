/************************************************************************
*
*    PRODUCT:         ebook
*
*    FILE NAME:       Preferences.h
*
*    DESCRIPTION:     Preferences dialog box
*
*    IMPLEMENTATION:  Dialog box for users to fill in their project info
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __PREFS_DLG_H__
#define __PREFS_DLG_H__

#include "ebStruct.h"    // All ebook structure definitions

// Function for creating the orderform dialog box
BOOL DisplayPrefsDialogBox( HINSTANCE hInst, HWND hwnd, PTProjectPrefs pProjPrefs, char *pArtPath );
// Message handler for the orderform dialog box
LRESULT CALLBACK PrefsDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
// Sunclassed edit procedures
#ifndef SHARE_THE_MEMORIES
LRESULT CALLBACK EdtSubClass_Fee( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK EdtSubClass_Taxes( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
#endif


class TPrefsDlg
{
protected:
    // Program instance
    HINSTANCE hInst;
    // All the prefs info
    PTProjectPrefs pProjPrefsData;
    // Path to the art files
    char *ArtPath;
    // Subclassed old procedures
	#ifndef SHARE_THE_MEMORIES
    WNDPROC OldEditProcFee, OldEditProcTaxes;
	#endif
    // Parent window
    HWND hParentWnd;

    // Save information recorded in the prefs dialog box
    void SaveProjectPrefs( HWND hwnd );
    // Get's the file path or clears out the path array
    void GetFilePath( HWND hwnd, int DlgItem, char *FileName, char *Title, char *FileMask );

public:
    // Constructor
    TPrefsDlg();
    // Destructor
    ~TPrefsDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    BOOL CreateDialogWnd( HINSTANCE hInstance, HWND hwnd, PTProjectPrefs pProjPrefs, char *pArtPath );
	// Get the handle to the subclassed controls
	#ifndef SHARE_THE_MEMORIES
	WNDPROC GetEdtProc_Fee(){ return OldEditProcFee; };
	WNDPROC GetEdtProc_Taxes(){ return OldEditProcTaxes; };
	#endif

    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TPrefsDlg *PTPrefsDlg;


#endif  // __PREFS_DLG_H__