/************************************************************************
*
*    PRODUCT:         Template Dialog
*
*    FILE NAME:       DlgTemplate.h
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


#ifndef __DLG_TEMPLATE_H__
#define __DLG_TEMPLATE_H__


// Function for creating the Registration Number dialog box
BOOL DisplayTemplateDlg( HINSTANCE hInst, HWND hwnd );
// Message handler for the Info text setup dialog box
LRESULT CALLBACK TemplateDlg_SetupDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
// Standard Windows OnCommand message handler
void TemplateDlg_OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
// Initilization function
BOOL TemplateDlg_OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
// Standard Windows OnCommand message handler
void TemplateDlg_OnDestroy( HWND hwnd );

#endif  // __REGNUMDLG_H__