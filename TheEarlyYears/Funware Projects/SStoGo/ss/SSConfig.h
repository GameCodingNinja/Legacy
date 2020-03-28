/************************************************************************
*
*    PRODUCT:         Screen saver
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


#include "gtools.h"          // A misc group of useful functions
#include "TJPEG.h"           // JPEG Library
#include "TBitmap.h"         // Dib and Bitmap class for displaying and animating bitmaps
#include "DecodGif.h"        // GIF decoder which can load GIFs from file or resource
#include "Settings.h"        // User settings for this screen saver
#include "Printer.h"         // Printer object.
#include "MediaPly.h"        // Header file for media player
#include "S2GStrct.h"        // Screen Savers To Go structure header file
#include "PageList.h"        // Dynamic Allocation of pages


enum
{
    THANK_YOU_FOR_REG = 200,
    FIRST_TMP_NUM_ACT,
    SECOND_TMP_NUM_ACT,
    TMP_NUM_ONLY_ONCE,
    REG_NUM_ERROR_MSG,
};


// Function for creating the config dialog box
BOOL DisplayConfigDialogBox( HINSTANCE hInst, HWND hwnd );
// Message handler for the config dialog box
LRESULT CALLBACK ConfigDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
// Static control subclass
LRESULT CALLBACK ViewWnd_StaticProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );



class TConfigDlg
{
protected:
    // User settings for this screen saver
    TSettings Settings;          
    // Dib and Bitmap class for displaying and animating bitmaps
    TBitmap MainDib, BackDib, buffer, dib;
    // GIF decoder which can load GIFs from file or resource                    
    TGif gif;
    // JPEG decoder                    
    TJPEG jpeg;
    // Printer object
    TPrinter Printer;
    // Madia player
    TMediaPlayer mediaPlayer;
    // This program's instance
    HINSTANCE hInst;
    
    // Subclassed returned procedure    
    WNDPROC OldStaticWnd;

    // Project structors
    TActiveProjectData ActProj;
    TApplicationData AppData;
    TProjectPrefs ProjPrefsData;
    TFileHeader FileHeader;
    TPageList PageList;

    // Go through all the controls and record the settings they hold.
    void RecordUserSettings(  HWND hwnd  );
    // Updates the controls
    void UpdateControls( HWND hwnd, int CurrentPage, int TotalPages );
    // Flips the page left or right
    void FlipPage( HWND hwnd, int Direction );
    // Disable the controls on an error
    void ErrorDisableControls( HWND hwnd );
    // Play the sound file
    void PlaySoundFile( HWND hwnd, WORD PageToLoad );
    // Check or uncheck the menu
    void CheckUncheckMenu( HWND hwnd, int MenuID );
    // Print to the printer
    BOOL Print( HWND hwnd );
    // Force the view window to repaint
    void ViewWnd_Repaint();

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
    // Return the window procedure
    WNDPROC GetViewWndOldProc(){ return OldStaticWnd; };
    // Standard Windows OnPaint message handler
    void ViewWnd_OnPaint( HWND hwnd );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TConfigDlg *PTConfigDlg;

#endif  /* __SSCONFIG__ */