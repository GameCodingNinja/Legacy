
/************************************************************************
*
*    PRODUCT:         Screen Savers To Go
*
*    FILE NAME:       S2GStrct.h
*
*    DESCRIPTION:     Screen Savers To Go structure header file 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __SSTOGO_PROJECT_STRUCTS_H__
#define __SSTOGO_PROJECT_STRUCTS_H__

#include "GDefines.h"        // Header file with #defines
#include <commctrl.h>        // Header file for Windows 95 controls

// Here's the file format of the project file
//   TProjectPrefs
//   TPage (array)


// Here's the file format of the executable
//   EXE Application
//   TProjectPrefs
//   TPage (array)
//   Image, music Data
//   TFileHeader


////////////////////////////
//    Structures
////////////////////////////

struct TApplicationData
{
    HINSTANCE hInst;                    // Programs instance
    HWND hParentWnd;                    // Handle to parent window
    char Path[MAX_STR_PATH];            // Application exe path
    TBBUTTON ToolAry[NUM_TBBUTTONS];    // Structure array for tool bar
    HWND hToolBar;                      // Tool bar handle
    HWND hStatusBar;                    // Handle to the status bar
    HWND hViewWnd;                      // Handle to view window
    HWND hTBStatic;                     // Static text that shows what page & total pages
    HWND hCaptionEdit;                  // List box to display caption information
    RECT ViewWndRect;                   // The view window rect
    BOOL RegistrationMode;              // Registration mode variable
    char RegistrationNo[MAX_REG_NUM+1]; // Hold the the tool or view registration no.
    WNDPROC OldEditProc[MAX_EDT_CONTRL]; // Used for subclassing edit procedure
    WNDPROC OldEditTxtProc[MAX_EDT_TEXT]; // Used for subclassing the Top & Bot text edit procedure
    char ToolTipStr[MAX_PATH];            // Holds the tool tip string
    char SoundFileTmpPath[MAX_PATH+1];    // String that holds the path the where the temp file is
                                        // written and played. This temp file also has a random
                                        // number in it to keep another version of the view program
                                        // from over writing an active file.
    RECT CaptionTxtRect;                // Caption window rect
    HWND hEditMsg[MAX_EDT_TEXT];        // Handle to top and bottom message edit field
    HWND hChkSizeBMP;                   // Handle to size bitmat to fit
    HWND hEditContrls[MAX_EDT_CONTRL];  // Handle to delay edit field & Edit field displaying the current page
    HWND hBtnJust[MAX_JUST_BTN];        // Handle to Top Left, Center, Right text button
    HWND hFontDlg[MAX_FONT_DLG];        // Handle to Font and color dialogs
    HWND hStatic[MAX_STATIC_LABELS];    // Handle to the static labels
    HWND hStatusWnd;                    // Handle to status window
    int  ChildOffsetY;                  // Offset of children from the button bar
};

typedef TApplicationData *PTApplicationData;

struct TActiveProjectData
{
    char FilePath[MAX_STR_PATH];  // Path to project file
    BOOL LiveProject;             // Does this project require saving
    BOOL OpenedFile;              // Is this an opened project
};
typedef TActiveProjectData *PTActiveProjectData;


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
typedef TFileHeader *PTFileHeader;


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

    // Future data here

    #ifdef SS2GO_TOOL
    BYTE FutureData[ PRJPREF_FUTURE ];  // Future data
    #endif
};

typedef TProjectPrefs *PTProjectPrefs;


// This structure is never used to hold data but is used to get the
// difference in size of the same structure for the view program.
// keeps the math easy.
#ifdef SS2GO_TOOL
struct TProjectPrefsSmallSize
{
    BYTE FutureData[ PRJPREF_FUTURE ];  // Future data
};
#endif


// This structure is placed before all image, sound and video data blocks
struct TDataHeader
{
    WORD Type;                  // Data type.
    DWORD CompSize;             // Compressed size for BMP. Not used for other forms of data
    DWORD UncompSize;           // Uncompressed size used for all data types
    FILETIME FileTime;          // Date and time of the cource file
    BYTE FutureData[ DATE_HEADER_FUTURE ];  // Future data
};

struct TLoadBuffer
{
    WORD Type;                  // Data type.
    DWORD Size;                 // Size of buffer
    BYTE *pData;                // Data buffer pointer
};
typedef TLoadBuffer *PTLoadBuffer;


#endif   // __SSTOGO_PROJECT_STRUCTS_H__