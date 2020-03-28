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
#include "BaseCfg.h"		 // Base class for config dialog box
#include "Defs.h"            // Screen saver defs file


// Function for creating the config dialog box
BOOL DisplayConfigDialogBox( HINSTANCE hInst, HWND hwnd );
// Message handler for the config dialog box
LRESULT CALLBACK ConfigDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );


class TConfigDlg : public TBaseConfigDlg
{
protected:          
    // Dib class for displaying and animating bitmaps
    TDIB dib;
    // Counts the number of shareware music notices and stops sending them
    // after a specif amount
    int sharewareMsgCounter;        

    // Set the labels for the scroll bars
    void SetScrollBarLabels( HWND hwnd, HWND hScrollBar, int ScrollPos );
    // Go through all the controls and record the settings they hold.
    void RecordUserSettings(  HWND hwnd  );
    // Free all the allocations
    void FreeAllAlocations();
    // Get users the directory the user
    void GetSoundDirectory( HWND hwnd );

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
    // On HScroll message handler for the config dialog box
    void OnHScroll( HWND hwnd, HWND hwndCtl, UINT code, int pos );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TConfigDlg *PTConfigDlg;


// Returns the screen size id
int GetScreenResSizeID( int ScreenW, int ScreenH );
// Get the render time depending on the screen size
DWORD GetRenderTime( int ResID, HINSTANCE hInst );
// Get the width, height and camera from the res ID
BOOL GetWidthHeightCamera( int ResID, int &ScreenW, int &ScreenH, int &CameraSize );

#endif  /* __SSCONFIG__ */