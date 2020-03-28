
/************************************************************************
*
*    PRODUCT:         Screen Savers To Go
*
*    FILE NAME:       SSToGo.h
*
*    DESCRIPTION:     Screen Savers To Go main header file 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __SSTOGO_H__
#define __SSTOGO_H__

#include "GDefines.h"        // Header file with #defines
#include "gTools.h"          // A misc group of useful functions
#include "TBitmap.h"         // Dib and Bitmap class for displaying and animating bitmaps
#include <commctrl.h>        // Header file for Windows 95 controls
#include "TGBase.h"          // The Graphics Base Object
#include "Printer.h"         // Printer object.
#include "Settings.h"        // User settings for this screen saver
#include "PageList.h"        // Dynamic Allocation of pages
#include "TJPEG.h"           // JPEG Library
#include "DecodGif.h"        // GIF decoder which can load GIFs from file or resource
#include "MediaPly.h"        // Header file for media player
#include "TStrList.h"        // Header file for the string list class.
#include "S2GStrct.h"		 // Screen Savers To Go structure header file


// Standard Windows message handler
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
// Call back for the edit procedure
LRESULT CALLBACK EdtPageTo_Delay_Proc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
// Call back for the edit procedure
LRESULT CALLBACK EdtTopBotCaption_Proc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
// Standard Windows message handler
LRESULT CALLBACK ViewWndProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );


class TSS2GO
{
protected:
    TActiveProjectData ActProj;
    TApplicationData AppData;
    TProjectPrefs ProjPrefsData;
    TFileHeader FileHeader;
    TSettings Settings;
    TPageList PageList;
    TBitmap MainDib, BackDib, buffer;
    TMediaPlayer mediaPlayer;
    TPrinter Printer;

    // Check to see if the program needs to be saved
    BOOL CheckToSave( HWND hwnd );
    // All windows created with CreateWindow must be registered first
    BOOL RegisterWnd( HINSTANCE hInst );
    // Create the window
    HWND CreateWnd( HINSTANCE hInst, int nCmdShow );
    // Show the orderform
    void FlipPage( HWND hwnd, int Direction = PAGE_FLIP_RIGHT, int SpecificPage = FALSE );
    // Enable or disable menu items
    void EnableControls( HWND hwnd, BOOL Enable );
    // Displays the current page and the total number of pages
    void UpdateControls( HWND hwnd, int CurrentPage, int TotalPages );
    // Load the keyed in page the user wants to see
    void LoadKeyEntryPage();
    // Shows the startup screen be it for the tool or the viewer program
    void ShowStartUpScreen( HWND hwnd );
    // Shows the helpful hint be it for the tool or the viewer program
    void ShowHelpFulHint( HWND hwnd );
    // Show the about dialog box
    void ShowAboutDlg( HWND hwnd );
    // Print a page to the printer
    BOOL Print( HWND hwnd );
    // Load the bitmap file
    int LoadImageFile( HWND hwnd, char *FileName, PTBitmap pBitmap );
    // Save the secret ini file settings
    void SaveSecretINIsettings();
    // Set the window caption
    void SetWindowCaption( HWND hwnd );
    // Set Flip Left and Flip Right buttons
    void SetFlipButtons();
    // Loads and plays the sound file
    void PlaySoundFile( HWND hwnd, WORD PageToLoad );
    // Handles the POST_MSG_AFTER_WM_SHOW message
    void HandlePostMeg( HWND hwnd );
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
    BOOL mnuAddPage( HWND hwnd, int PageType );
    // Adds an new background image to the page
    BOOL mnuAddBackImage( HWND hwnd );
    // Builds the final executible
    BOOL mnuBuild_EXE( HWND hwnd );
    // Compress all the image files
    BOOL CheckAllFilesAreThere( HWND hwnd );
    // Write the exe from the resource to the project directory
    HANDLE WriteEXEtoHDfromRes( char *ResID, char *FilePath, DWORD *FileSize = NULL );
    // Save the page info to the new EXE file
    DWORD SavePageDataToFile( HANDLE hFile, DWORD StartingOffset );
    // Save the project info to the new EXE file
    DWORD SaveProjectDataToFile( HANDLE hFile, DWORD StartingOffset );
    // Save the project file to the exe file
    BOOL SavePageFilesToEXE( HANDLE hFile );
    // Init application structures
    void InitProject( HWND hwnd );
    // Setup the text for the info windows
    void SetupInfoWnd( HWND hwnd, char *pDlgCaption, char *WndCaption, char *WndTitle, char *FileName );
    // Deletes the current page
    void DeleteCurPage( HWND hwnd );
    // Moves the current page
    void MovePage( HWND hwnd );
    // Loads the help file
    void DisplayHelpFile( HWND hwnd, UINT Command, DWORD Index );
    // Takes the file path and sets up the page list object
    BOOL AddImageFileToPageList( HWND hwnd, char *FileName, PTBitmap Image, int PageType );
    // Builds and Runs the project
    void RunProject( HWND hwnd, int RunType );
    // Enable or disable menu items specific to having pages loaded
    void EnableSpecificControls( HWND hwnd, BOOL Enable );
    // Help the user find missing project files
    BOOL FindMissingProjectFile( HWND hwnd, char *MissingFile, char *StartSearchPath, char *FileMask );
    // Add sound file to page
    void AddSound( HWND hwnd );
    // Delete the current page
    void DeletePage( HWND hwnd );
    // Remove sound file from the current page
    void RemoveSound( HWND hwnd );
    // Create the tool bar
    BOOL CreateToolBar( HWND hwnd );
    // Create the window controls
    BOOL CreateControls( HWND hwnd );
    // Create the window controls
    void ErrorDisableControls( HWND hwnd );
    // Set the color of the background or text
    void SetColor( HWND hwnd, int ColorDialog );
    // Set the font
    void SetTextFont( HWND hwnd, int FontDialogButton );
    // Write the screen saver to the back of the installer
    BOOL WriteScrToInstaller( HANDLE hScr, HANDLE hInst );
    // Delete the temporary screen saver in the temp folder
    void DeleteTmpSaver();
    // Create the view window
    BOOL ViewWnd_CreateViewWnd( HINSTANCE hInst, HWND hParent );
    // Force the window to repaint itself
    void ViewWnd_Repaint();
	// Finds the screen saver file by bringing up the directory it is in.
	void FindSaver( HWND hwnd );

public:
    // Constructor
    TSS2GO();
    // Destructor
    ~TSS2GO();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnCreate(HWND hwnd, CREATESTRUCT* lpCreateStruct);
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Create the dialog box
    HWND CreateParentWnd( HINSTANCE hInst, int nCmdShow );
    // OnNotify used for tool tips
    LRESULT OnNotify( HWND hwnd, int idFrom, NMHDR FAR * pnmhdr );
    // Make sure we want to end the application
    BOOL OnClose( HWND hwnd );
    // Standard Windows message handler
    void OnMove( HWND hwnd, int x, int y );
    // Standard Windows OnGetMinMaxInfo message handler
    void OnGetMinMaxInfo( HWND hwnd, LPMINMAXINFO lpMinMaxInfo );
    // Standard Windows OnShowWindowe message handler
    void OnShowWindow( HWND hwnd, BOOL fShow, UINT status );
    // Handle the Jump to page and delay settings subclassed controls procedure
    LRESULT HandlePage_DelayProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
    // Handle the top and bottom caption subclassed controls procedure
    LRESULT HandleTopBotCaption_Proc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
    // Set the thankyou note buffer
    void SetThankYouBuffer( char *Text ){ Settings.SetThankYouNoteBuffer( Text ); };
    // Get a pointer to the settings
    PTSettings GetPtrToSettings(){ return &Settings; };
    // Get the handle to the parent window
    HWND GetParentWnd(){ return AppData.hParentWnd; };
    // Get the handle to the program instance
    HINSTANCE GetAppInstance(){ return AppData.hInst; };
    // Standard Windows OnPaint message handler
    void ViewWnd_OnPaint( HWND hwnd );
    // Standard Windows OnDestroy message handler
    void ViewWnd_OnDestroy( HWND hwnd );
    // Standard Windows OnCreate message handler
    BOOL ViewWnd_OnCreate( HWND hwnd, LPCREATESTRUCT lpCreateStruct );
    // Standard Windows OnEraseBkgnd message handler
    BOOL ViewWnd_OnEraseBkgnd( HWND hwnd, HDC hdc );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TSS2GO *PTSS2GO;


#endif  // __SSTOGO_H__