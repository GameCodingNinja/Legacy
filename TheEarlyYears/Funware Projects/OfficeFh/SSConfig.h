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

#include "BaseCfg.h"         // Base class for config dialog box
#include "TBitmap.h"         // Dib and Bitmap class for displaying and animating bitmaps
#include "defs.h"            // Structure defines
#include "Metafile.h"        // Class for loading and displaying Windows metafiles and enhanced metafiles
#include "TCanvas.h"         // Header file for this *.cpp file.


enum
{
    EDT_BUS_NAME=0,
    STC_PREVIEW_WND,
    MAX_SUBCLASS_CTRLS,
};


// Function for creating the config dialog box
BOOL DisplayConfigDialogBox( HINSTANCE hInst, HWND hwnd );
// Message handler for the config dialog box
LRESULT CALLBACK ConfigDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
// Call back for subclassing any of the controls procedure
LRESULT CALLBACK SubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam );

class TConfigDlg : public TBaseConfigDlg
{
protected:          
    // Dib class for displaying and animating bitmaps
    TDIB dib;
    // The screens current rect
    RECT BufferRect;
    // The current system depth
    int SystemBitDepth;
    
    // Buffer to build image in
    TBitmap buffer;
    // Buffer to load artwork
    TBitmap FishArt;
    // Structure for art offsets
    PTFishDataArry pFish;
    PTFishDataArry pBackFish;
    PTRectArry bubble;
    PTRectArry pOfficeProps;
    PTRectArry pFishProps;
    TFish *fish;
    TFish *backFish;
    TBubble *pBubble;
    int *pFishPropXOffset, BackFishCount, BubbleCount;
    // Background meta file
    TMetafile metafile;
    // Subclassed window control handles
    HWND hSubCtrl[ MAX_SUBCLASS_CTRLS ];
    // Matching windows procedure for subclassed controls
    WNDPROC OldSubCtrlProc[ MAX_SUBCLASS_CTRLS ];
    // Flag that indicates blitting is ready
    BOOL BufferReadyToBlit;
    // Canvas object for blitting of text
    TCanvas canvas;
    
    // get the new font the user wants
    void SetTextFont( HWND hwnd );
    // Set the text color
    void SetTextColor( HWND hwnd );
    // Build the background
    void BuildBackground( HWND hwnd );
    // Load all the resources
    void LoadArtResources( HWND hwnd );
    // Subclassed edit control
    LRESULT OnEdit_BusinessName( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );        
    // Subclassed static control
    LRESULT OnPreviewWnd( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Setup the background with a solid color
    void BackSolidColor();
    // Force a repaint onto the preview window
    void RepaintPreviewWnd( HWND hwnd );

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
    // Get the instance
    HINSTANCE Instance(){ return hInst; };
    // Called from the subclassed control
    LRESULT HandleSubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam );

    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TConfigDlg *PTConfigDlg;


#endif  /* __SSCONFIG__ */