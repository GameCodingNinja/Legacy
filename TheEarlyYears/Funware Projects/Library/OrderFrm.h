/************************************************************************
*
*    PRODUCT:         Oder form dialog
*
*    FILE NAME:       OrderFrm.h
*
*    DESCRIPTION:     Order Form dialog box
*
*    IMPLEMENTATION:  Dialog box for users to order software
*                     After it is setup once, it shouldn't have to reworked
*                     unless making changes
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/



#ifndef __ORDERFRM_H__
#define __ORDERFRM_H__

#include "Printer.h"         // Class for printing to the printer
#include "Settings.h"        // User settings for this program

// Function for creating the orderform dialog box
BOOL DisplayOrderFrmDialogBox( HINSTANCE hInst, HWND hwnd, PTSettings pSet, BOOL ACenterToParent = FALSE );
// Message handler for the orderform dialog box
LRESULT CALLBACK OrderFrmDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );


class TOrderFrmDlg
{
protected:
    // Parent Window handle
    HWND hParentWnd;
    // instance of the app
    HINSTANCE hInst;
    // Dib class for displaying bitmaps
    TDIB OrderDib;
    // Center window flag   
    BOOL CenterToParent;
    // Using the settings from the config window. Keeps these settings from being erased
    PTSettings pOrderSettings;

    // Save the information entered into the orderform
    void SaveUserSettings( HWND hwnd );
    // Print the orderform so they can send me a check
    void PrintOrderForm( HWND hwnd );
    // Check to see if we need to send this page to the printer and start another one
    int CheckPrinterTopMargin( int TopMargin, PTPrinter printer );
    // Calculate the total money owed
    void FormatTotalCost( HWND hwnd );
    // Display the no printer dialog box
    void ShowNoPrinterDlg( HWND hwnd );

public:
    // Constructor
    TOrderFrmDlg();
    // Destructor
    ~TOrderFrmDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Standard Windows OnPaint message handler
    void OnPaint( HWND hwnd );
    // I'm using this to post a message to show the helpful hint. I do this so that the
    // config dialog box shows up and then the helpful hint box. Otherwise, the helpful
    // hint box will show before the dialog box will.
    void OnShowWindow( HWND hwnd, BOOL fShow, UINT status );
    // Create the dialog box
    BOOL CreateDialogWnd( HINSTANCE hInstance, HWND hwnd, PTSettings pSet, BOOL ACenterToParent );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TOrderFrmDlg *PTOrderFrmDlg;


#endif  /* __ORDERFRM__ */