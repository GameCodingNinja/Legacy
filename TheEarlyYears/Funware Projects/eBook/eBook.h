
/************************************************************************
*
*    PRODUCT:         eBook
*
*    FILE NAME:       eBook.h
*
*    DESCRIPTION:     The begining
*
*    IMPLEMENTATION: How it's done 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __EBOOK_H__
#define __EBOOK_H__

#include "TBitmap.h"         // Dib and Bitmap class for displaying and animating bitmaps
#include "ebStruct.h"        // All the structures
#include "Settings.h"        // User settings for this screen saver
#include "PageList.h"        // Dynamic Allocation of pages
#include "TTextBlk.h"        // Object for loading in text files
#include "MediaPly.h"        // Header file for media player
#include "Printer.h"         // Printer object.
#ifdef EBOOK_TOOL
#include <shellapi.h>        // Used for HDROP define
#endif

#define VIEW_WND_OFFSET_X     100
#define VIEW_WND_OFFSET_Y     30

// Here's the file format of the project file
//   TProjectPrefs
//   TPage (array)


// Here's the file format of the executable
//   EXE Application
//   TProjectPrefs
//   TPage (array)
//   Image, music Data
//   TFileHeader


// Standard Windows message handler
LRESULT CALLBACK ParentWndProc( HWND, UINT, WPARAM, LPARAM );
// Standard Windows message handler
LRESULT CALLBACK WndMidProc( HWND, UINT, WPARAM, LPARAM );
// Call back for the edit procedure
LRESULT CALLBACK EdtSubclass_LoadPage( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
// Standard Windows message handler
LRESULT CALLBACK ViewWndProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );

#if defined( EBOOK_TOOL ) || !defined( SHARE_THE_MEMORIES )
// Show the orderform
void ShowOrderFormDlg();
#endif


class TEbookWnd
{
protected:
    #ifdef EBOOK_TOOL
    TActiveProjectData ActProj;
    #else
    TBitmap TileBackDIB;
    #endif

    TApplicationData AppData;
    TProjectPrefs ProjPrefsData;
    TFileHeader FileHeader;
    TSettings Settings;
    TPageList PageList;
    TBitmap dib;
    TPrinter Printer;
    TTextBlock TextBlock;
    TMediaPlayer mediaPlayer;
    BOOL ViewWndActive;
    HBRUSH BackGrndBrush;

    // Check to see if the program needs to be saved
    BOOL CheckToSave( HWND hwnd );
    // All windows created with CreateWindow must be registered first
    BOOL RegisterWnd( HINSTANCE hInst );
    // Create the window
    HWND CreateWnd( HINSTANCE hInst, int nCmdShow );
    // Turns on or off the scroll bars
    void SetScrollBars( HWND hwnd );
    // Show the orderform
    void FlipPage( HWND hwnd, int Direction = PAGE_FLIP_RIGHT, int SpecificPage = FALSE );
    // Enable or disable menu items
    void EnableMenuItems( HWND hwnd, BOOL Enable );
    // Displays the current page and the total number of pages
    void UpdateControls( HWND hwnd, int CurrentPage, int TotalPages );
    // Enlarge or reduce the page
    void EnlargeReducePage( HWND hwnd, int Direction );
    // Shows the startup screen be it for the tool or the viewer program
    void ShowStartUpScreen( HWND hwnd );
    // Shows the helpful hint be it for the tool or the viewer program
    void ShowHelpFulHint( HWND hwnd );
    // Init specific TSetting class members that is used for the Order Form
    void SetupOrderFormInfo();
    // Show the about dialog box
    void ShowAboutDlg( HWND hwnd );
    // Print a page to the printer
    BOOL Print( HWND hwnd );
    // Load the page bitmap
    int LoadMainFile( HWND hwnd, WORD PageToLoad, PTGBase pBase );
    // Save the secret ini file settings
    void SaveSecretINIsettings();
    // Set the window caption
    void SetWindowCaption( HWND hwnd );
    // Set Flip Left and Flip Right buttons
    void SetFlipButtons();
    // Loads and plays the sound file
    void PlaySoundFile( HWND hwnd, WORD PageToLoad );
    // Display the print dialog box
    void mnuPrint( HWND hwnd );
    // Display the print setup dialog box
    void mnuPrintSetup( HWND hwnd );
    // Display the order form dialog box
    void mnuOrderForm( HWND hwnd );
    // Handles the POST_MSG_AFTER_WM_SHOW message
    void HandlePostMeg( HWND hwnd );
    // Create the tool bar
    BOOL CreateToolBar( HWND hwnd );

    // Create the view window
    BOOL ViewWnd_CreateViewWnd( HINSTANCE hInst, HWND hParent );
    // Resizes the view window
    void ViewWnd_ResetWnd( HWND hParent );
    // Sets the font for the text window
    void ViewWnd_SetTextFont( HWND hwnd );
    // Sets the text windows background color
    void ViewWnd_SetBackGrndColor( HWND hwnd );
    // Reset all the font and color settings to windows default
    void ViewWnd_ResetFontDefaults( HWND hwnd );

    #if defined( EBOOK_TOOL ) || !defined( SHARE_THE_MEMORIES )
    // Show the registration dialog the get the users registration number
    BOOL ShowRegDlg( HWND hwnd );
    #endif

    // These function calls are only for the tool version
    #ifdef EBOOK_TOOL
    // New message handler
    void mnuNew( HWND hwnd );
    // Open message handler
    void mnuOpen( HWND hwnd );
    // Save message handler
    BOOL mnuSave( HWND hwnd );
    // Save AS message handler
    BOOL mnuSaveAs( HWND hwnd );
    // Close message handler
    void mnuClose( HWND hwnd );
    // Save project data to the hard drive
    BOOL SaveToFile( HWND hwnd, char *FileName );
    // Open project data that was saved to the hard drive
    BOOL OpenFile( HWND hwnd );
    // Adds an new page with an image
    BOOL mnuAddPage( HWND hwnd );
    // Builds the final executible
    BOOL mnuBuild_EXE( HWND hwnd );
    // Compress all the image files
    BOOL CheckAllFilesAreThere( HWND hwnd );
    // Write the exe from the resource to the project directory
    HANDLE WriteEXEtoHDfromRes( char *FilePath, DWORD *FileSize );
    // Save the page info to the new EXE file
    DWORD SavePageDataToFile( HANDLE hFile, DWORD StartingOffset );
    // Save the project info to the new EXE file
    DWORD SaveProjectDataToFile( HANDLE hFile, DWORD StartingOffset );
    // Save the project file to the exe file
    BOOL SavePageFilesToEXE( HANDLE hFile );
    // Init application structures
    void InitProject( HWND hwnd );
    // Save the project files to the exe file
    BOOL SaveProjectFilesToEXE( HANDLE hFile );
    // Deletes the current page
    void DeleteCurPage( HWND hwnd );
    // Moves the current page
    void MovePage( HWND hwnd );
    // Loads the help file
    void DisplayHelpFile( HWND hwnd, UINT Command, DWORD Index );
    // Takes the file path and sets up the page list object
    BOOL AddMainFileToPageList( char *FileName );
    // Resets all the projects to act as unregistered
    void ResetProjects( HWND hwnd );
    // Builds and Runs the project
    void RunProject( HWND hwnd );
    // Returns true if it is an RTF file
    BOOL IsFileAnRTF( char *FileName );
    // Enable or disable menu items specific to having pages loaded
    void EnableSpecificMenuItems( HWND hwnd, BOOL Enable );
    // Help the user find missing project files
    BOOL FindMissingProjectFile( HWND hwnd, char *MissingFile, char *StartSearchPath, char *FileMask );
    // Add sound file to page
    void AddSound( HWND hwnd );
    // Delete the current page
    void DeletePage( HWND hwnd );
    // Remove sound file from the current page
    void RemoveSound( HWND hwnd );
    // Displays the new page or the last page of a group
    BOOL DisplayLoadedPage( HWND hwnd );
    // Find the project executable
    void FindProject( HWND hwnd );

    #ifndef SHARE_THE_MEMORIES
    // Setup the text for the info windows
    void SetupInfoWnd( HWND hwnd, char *pDlgCaption, char *WndCaption, char *WndTitle, char *FileName );
    #endif

    #else
    // Open the exe file and read in the header
    BOOL OpenEXEandReadHeader();
    // Checks to see if this view program should be allowed to run
    BOOL AllowViewProgramToRun();
    // Check sum the application
    BOOL CheckSumApp();
    // Loads the tiled background
    void LoadTiledBackground();
    // Checks to see if the customer has registered the view program
    BOOL IsViewProgramRegistered();
    // Saves the page data to a file
    void ExportFile( HWND hwnd, int CurrentPage, int PageType );
    #endif

public:
    // Constructor
    TEbookWnd();
    // Destructor
    ~TEbookWnd();

    // Create the dialog box
    HWND CreateParentWnd( HINSTANCE hInst, int nCmdShow );
    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Standard Windows OnCreate message handler
    BOOL OnCreate( HWND hwnd, LPCREATESTRUCT lpCreateStruct );
    // Make sure we want to end the application
    BOOL OnClose( HWND hwnd );
    // Standard Windows message handler
    void OnSize( HWND hwnd, UINT state, int cx, int cy );
    // Standard Windows message handler
    void OnMove( HWND hwnd, int x, int y );
    // Standard Windows WM_KEYDOWN message handler
    void OnKey( HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags );
    // Standard Windows OnMouseWheel message handler
    void OnMouseWheel( HWND hwnd, int zDelta );
    // OnNotify used for tool tips
    LRESULT OnNotify( HWND hwnd, int idFrom, NMHDR FAR * pnmhdr );
    // Standard Windows OnShowWindowe message handler
    void OnShowWindow( HWND hwnd, BOOL fShow, UINT status );
    // Standard Windows OnVScroll message handler
    void MidWnd_OnVScroll( HWND hwnd, HWND hwndCtl, UINT code, int pos );
    // Standard Windows OnHScroll message handler
    void MidWnd_OnHScroll( HWND hwnd, HWND hwndCtl, UINT code, int pos );
    // Standard Windows OnEraseBkgnd message handler
    BOOL ParentWnd_OnEraseBkgnd( HWND hwnd, HDC hdc );
    // Load the keyed in page the user wants to see
    void LoadKeyEntryPage();

    // Get the programs instance
    HINSTANCE GetInstance(){ return AppData.hInst; };
    // Get the parent window
    HWND GetParentWnd(){ return AppData.hParentWnd; };
    // Get pointer to settings object
    PTSettings GetPtrSettings(){ return &Settings; };
    // return the load page edit control subclassed old procedure
    WNDPROC GetLoadPageSubProc(){ return AppData.OldEditProc; };

    // Standard Windows OnPaint message handler
    void ViewWnd_OnPaint( HWND hwnd );
    // Standard Windows OnDestroy message handler
    void ViewWnd_OnDestroy( HWND hwnd );
    // Standard Windows OnCreate message handler
    BOOL ViewWnd_OnCreate( HWND hwnd, LPCREATESTRUCT lpCreateStruct );
    // Standard Windows OnEraseBkgnd message handler
    BOOL ViewWnd_OnEraseBkgnd( HWND hwnd, HDC hdc );
    #ifdef EBOOK_TOOL
    // Standard Windows WM_DROPFILES message handler
    void ViewWnd_OnDropFiles( HWND hwnd, HDROP hdrop );
    #else
    // Standard Windows OnPaint message handler
    void MidWnd_OnPaint( HWND hwnd );
    // Standard Windows OnEraseBkgnd message handler
    BOOL MidWnd_OnEraseBkgnd( HWND hwnd, HDC hdc );
    // Loads the image file from the exe
    BOOL LoadFileDataFromEXEfile( DWORD Offset, PTLoadBuffer pLoadBuffer );
    // Get a pointer to the project prefs data
    PTProjectPrefs GetPtrProjPrefs(){ return &ProjPrefsData; };
    #endif

    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TEbookWnd *PTEbookWnd;

#endif  // __EBOOK__