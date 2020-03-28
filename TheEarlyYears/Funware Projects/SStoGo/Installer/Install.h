/************************************************************************
*
*    PRODUCT:         Program Installer
*
*    FILE NAME:       Install.cpp
*
*    DESCRIPTION:     Program for installing my programs
*
*    IMPLEMENTATION:  This version is currently setup to install screen
*                     savers but can be updated to install any application
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __INSTALL_H__
#define __INSTALL_H__

#include "GDefines.h"        // Header file with #defines
#include "InstTool.h"

// Defines
#define ID_MISC_ABOUT      1
#define ID_FILE_EXIT       2
#define ID_FILE_INSTALL    3
#define ID_FILE_UNINSTALL  4
#define ID_BTN_EXIT        5
#define ID_BTN_INSTALL     6
#define ID_BTN_UNINSTALL   7
#define SLEEP_TIME         3000
#define MNU_WEB_LINK           121

#define SHOW_MESSAGE       TRUE
#define DONT_SHOW_MESSAGE  FALSE

#define PARENT_WIDTH       384
#define PARENT_HEIGHT      217

#define WEBSITE "www.howiesfunware.com"


// string resource enumeration
enum
{
    ABOUT_WINDOW_CAPTION=0,
    ABOUT_WINDOW_INFO,
    PARENT_WINDOW_TITLE,
    UNINSTALL_BUTTON,
    INSTALL_BUTTON,
    EXIT_BUTTON,
    PROGRAM_INSTALLER,
    PROGRAM_UNINSTALLER,
    ALLREADY_INSTALLED,
    SUCCESSFUL_INSTALL,
    QUESTION_UNINSTALL,
    NO_PRODUCT_TO_UNSTALL,
    INSTALL_FINISHED,
    SUCCESSFUL_UNINSTALL,
    UNINSTALL_FINISHED,
    WINDOWS_NEEDS_RESTART,

    // File Install Strings
    COMPRESSED_FILE_NAME = 500,
    INSTALLED_FILE_NAME,
    PROGRAM_INI_FILE,
    PROGRAM_COMPRESSED,

    // Error Messages Here
    RESOURCE_ERROR_TITLE = 1000,
    HANDLE_ERROR,
    LOCK_ERROR,
    PARENT_BITMAP_ERROR,
    INSATLL_ERROR,
    UNCOMPRESSED_ERROR,
    NOT_INSTALLED,
    COMPRESSED_FILE_TRANSFER_ERROR,
    TEMPORARY_FILE_CREATE_ERROR,
    UNINSTALL_ERROR,
    UNINSTALL_ERROR_MESSAGE,
    HARD_DRIVE_ERROR,
    ERROR_GETTING_AVAILABLE_SPACE,
    NOT_ENOUGH_SPACE_ON_DRIVE,
};


// This structure needs to be quad aligned
// The TFileHeader is saved to the end of the viewer exe because
// it makes for a quick way to get the offset into the exe file
// where the file information is stored
struct TFileHeader
{
    DWORD FormatID;             // Header Identifyer
    char  VerNo[MAX_VER_NO+1];  // Program version number 12 bytes
    DWORD CheckSumResult;       // Checksum remander
    DWORD ProjectDataOffset;    // Offset into the file of the starting
                                // position of the project data
    DWORD FileSize;             // TotalFileSize
};

// User defined configuations for the output viewer exe program
struct TProjectPrefs
{
    DWORD PrefsCheckID;         // Special number to check to validate data
    char  Title[MAX_TITLE+1];      // Window Title
    char  ExeName[MAX_EXE_NAME+1]; // Name of exe file

    DWORD DriveSerialNo;        // The serial number from the hardrive to check if 
                                // running on the same computer if in shareware mode

    DWORD PageDataOffset;       // Starting offset to the TPage data

    BOOL  ProjRegMode;          // Flag indicating if the built exe was built with a
                                // registered version of SS to Go

    WORD  Total_Num_Of_Pages;   // Total number of pages in this file
};


// Prototypes
LRESULT CALLBACK WndProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK InstallingDlg( HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam );

class TInstallS2G
{
protected:
    HINSTANCE gInst;
    HWND hInstallDlg;
    TProjectPrefs ProjPrefsData;
    TFileHeader FileHeader;
    char WinDir[MAX_PATH+1];
    TDIB installDIB;
    char ExePath[MAX_PATH+1];
    char ScrPath[MAX_PATH+1];
    char ScrFileName[MAX_TITLE+7];

	// Register the window
    BOOL Register(HINSTANCE hInst);
	// Create the parent window
    BOOL Create(HINSTANCE hInst, int nCmdShow);
	// Read the file header
    BOOL OpenEXEandReadHeader( char *FileName );
	// Write the screen saver to the hard drive
    BOOL WriteSaverToHardDrive( HWND hwnd );
	// End this install program
    void QuitProgram( HWND hwnd );
	// Select screen saver as default screen saver
    void SelectAsDefault( char *scr );
	// Install the screen saver file
    void InstallProgram( HWND hwnd );
	// Uninstall the screen saver file
    void UninstallProgram( HWND hwnd );
	// Delete the program
    void deleteProgram( HWND hwnd, BOOL fShowMsg );
	// init the parent window
    void initParentWnd( HWND hwnd );
	// Startup init
    void initStartUp( HWND hwnd );
	// see if we need to change the screen saver delay
    void CheckChangeSaverDelay( char* DelayTime );

public:
    // Constructor
    TInstallS2G();
    // Destructor
    ~TInstallS2G();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnCreate(HWND hwnd, CREATESTRUCT* lpCreateStruct);
    // Standard Windows OnCommand message handler
    void OnDestroy( HWND hwnd );
    // Standard Windows OnPaint message handler
    void OnPaint( HWND hwnd );
    // Create the dialog box
    BOOL CreateParentWnd( HINSTANCE hInst, int nCmdShow );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef TInstallS2G *PTInstallS2G;


#endif  /* __INSTALL_H__ */
