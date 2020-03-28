/************************************************************************
*
*    PRODUCT:         Screen saver config dialog box
*
*    FILE NAME:       SSConfig.h
*
*    DESCRIPTION:     Screen saver config dialog box
*
*    IMPLEMENTATION:  Dialog box for users to configure the screen saver
*                     Every project will need to have it's own different
*                     dialog box. This *.cpp and *.h are project specific
*                     and a new one for each screen saver project is required.
*
*    Copyright(c) 2000 by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __SSCONFIG_H__
#define __SSCONFIG_H__

#include "dib.h"             // Dib and Bitmap class for displaying and animating bitmaps
#include "Defs.h"            // Screen saver defs file
#include "Settings.h"        // User settings for this screen saver
#include "TStrList.h"        // Header file for the string list class.


// Function for creating the config dialog box
BOOL DisplayConfigDialogBox( HINSTANCE hInst, HWND hwnd );
// Message handler for the config dialog box
LRESULT CALLBACK ConfigDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );


class TConfigDlg
{
protected:          
    // Dib class for displaying and animating bitmaps
    TDIB dib;
    // Handle to the parent window
    HWND hParentWnd;
    // This program's instance
    HINSTANCE hInst;
    // User settings for this screen saver
    TSettings Settings;
    // Available saver list
    TStringList availSaverList, selSaverList;
    // List box handles
    HWND hAvailSavers, hToRunSavers;
    HWND focusWnd;        

    // Go through all the controls and record the settings they hold.
    void RecordUserSettings(  HWND hwnd  );
    // Free all the allocations
    void FreeAllAlocations();
    // Add saver to active list
    void btn_AddSaverToList( HWND hwnd );
    // Remove saver from the list
    void btn_RemoveSaverFromList( HWND hwnd );
	// run the selected screen saver
	void btn_Run( HWND hwnd, int runType );

public:
    // Constructor
    TConfigDlg();
    // Destructor
    ~TConfigDlg();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Standard Windows OnPaint message handler
    void OnPaint( HWND hwnd );
    // Create the dialog box
    int CreateDialogWnd( HINSTANCE hInstance, HWND hwnd );
    // I'm using this to post a message to show the helpful hint. I do this so that the
    // config dialog box shows up and then the helpful hint box. Otherwise, the helpful
    // hint box will show before the dialog box will.
    void OnShowWindow( HWND hwnd, BOOL fShow, UINT status );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TConfigDlg *PTConfigDlg;


#endif  /* __SSCONFIG__ */