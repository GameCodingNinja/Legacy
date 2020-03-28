
/************************************************************************
*
*    PRODUCT:         eBook
*
*    FILE NAME:       ebStruct.h
*
*    DESCRIPTION:     Structures used with eBook 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __EBOOK_STRUCT_H__
#define __EBOOK_STRUCT_H__

#include "GDefines.h"        // Header file with #defines
#include <commctrl.h>        // Header file for Windows 95 controls
#include "TGBase.h"          // The Graphics Base Object

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
    HWND hMidWnd;                       // Handle to middle window
    HWND hViewWnd;                      // Handle to view window
    HWND hTBStatic;                     // Static text that shows what page & total pages
    HWND hEditCurPage;                  // Edit field displaying the current page
    HWND hCaptionEdit;                  // List box to display caption information
    RECT ViewWndRect;                   // The view window rect
    SCROLLINFO sv;                      // Vertical scroll bar info
    SCROLLINFO sh;                      // Horizontal scroll bar info
    BOOL HScrollVisible;                // Horizontal scroll bar visible
    BOOL VScrollVisible;                // Vertical scroll bar visible
    FLOAT ZoomPercent;                  // The zoom position for enlarging the page
    BOOL RegistrationMode;              // Registration mode variable
    char RegistrationNo[MAX_REG_NUM+1]; // Hold the the tool or view registration no.
    WNDPROC OldEditProc;                // Used for subclassing edit procedure
    BOOL FlagBigImages;                 // If an image is being added that is way too big, tell the
                                        // user and set this flag so that they are only told once.
    char ToolTipStr[SMALL_ARRAY_200];   // Holds the tool tip string
    char SoundFileTmpPath[MAX_PATH];    // String that holds the path the where the temp file is
                                        // written and played. This temp file also has a random
                                        // number in it to keep another version of the view program
                                        // from over writing an active file.
    PTGBase pGBase;                     // Currently loaded file type
    RECT CaptionTxtRect;                // Caption window rect
    int CurrentPageSoundPlay;           // The current page a sound is playing
};

#ifdef EBOOK_TOOL
struct TActiveProjectData
{
    char FilePath[MAX_STR_PATH];  // Path to project file
    BOOL LiveProject;             // Does this project require saving
    BOOL OpenedFile;              // Is this an opened project
};
#endif


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
};

// User defined configuations for the output viewer exe program
struct TProjectPrefs
{
    DWORD PrefsCheckID;            // Special number to check to validate data
    char  Title[MAX_TITLE+1];      // Window Title
    char  ExeName[MAX_EXE_NAME+1]; // Name of exe file
    char  RegNo[MAX_REG_NUM+1];    // Registration number required for this software

    char  CopyrightInfo[MAX_COPYRIGHT_INFO+1]; // Copyright string
    char  NameOrCompany[MAX_INFO+1]; // Name and/or company name
    char  AddressLine1[MAX_INFO+1];  // Address of company
    char  AddressLine2[MAX_INFO+1];  // Address of company
    char  City[MAX_INFO+1];          // City State
    char  Zip[SMALL_ARRAY_50+1];     // Zip of company
    char  WebAddress[MAX_INFO+1];    // WebAddress
    char  EmailAddress[MAX_INFO+1];  // Email Address of company
    double RegFee;                   // Registration fee required for art
    double SalesTax;                 // Sales tax needed if sold in users state
    WORD  StateIndex;                // Combo box state index

    char  StartUpInfoCap[MAX_INFO+1];  // Users text for the startup info caption
    char  StartUpInfoTitle[MAX_INFO+1];// Users text for the startup info Title
    char  OFInfoCap[MAX_INFO+1];       // Users text for the Order Form info caption
    char  OFInfoTitle[MAX_INFO+1];     // Users text for the Order Form info Title

    COLORREF DefaultTxtColor;   // Holds the default text color of the text window
    COLORREF DefBckgrdTxtColor; // Holds the default background color;
    LOGFONT DefFont;            // Holds the default font information

    DWORD DriveSerialNo;        // The serial number from the hardrive to check if 
                                // running on the same computer if in shareware mode
    BOOL  DoChecksum;           // Flag indicating if the program should do a checksum
    BOOL  UseRegNum;            // Flag indicating if a registration number is needed
    WORD  AskForRegAfterPage;   // Flag indicating when to show registration notice
    DWORD PageDataOffset;       // Starting offset to the TPage data
    DWORD StartUpScrnBMPOffset; // Data offset to intro screen bitmap
    DWORD BackgroundBMPOffset;  // Data offset to background bitmap
    DWORD InfoWndTXTOffset;     // Data offset to text for startup info screen
    DWORD InfoOFWndTXTOffset;   // Data offset to text for order form screen
    BOOL  AllowFileExports;     // Flag indicating file exports allowed
    BOOL  AllowPrintPage;       // Flag indicating if printing of pages is allowed
    BOOL  PrintPageNumbers;     // Print page numbers in the footers
    BOOL  AllowVisa;            // Flag to allow for credit card transactions
    BOOL  AllowMasterCard;      // Flag to allow for credit card transactions
    BOOL  AllowDiscover;        // Flag to allow for credit card transactions
    BOOL  AllowAmerExpress;     // Flag to allow for credit card transactions
    BOOL  ProjRegMode;          // Flag indicating if the built exe was built with a
    #ifndef SHARE_THE_MEMORIES
    char  CreditWebAddress[MAX_INFO+1]; // Credit card web site
    #endif
                                // registered version of Art 4 Sale
    BYTE  ZoomIndex;            // The last recorded zoom index

    DWORD ViewProgramRandNo;    // When a new project is created, a random number is generated
                                // that is used to tack onto the end of the name of the
                                // registration item to help ensure each new project is
                                // different from the last.

    WORD  Total_Num_Of_Pages;   // Total number of pages in this file

    // The tool version needs to save the file paths for the project file
    // The view program doesn't need this information
    #ifdef EBOOK_TOOL
    char StartUpScreenPath[ MAX_STR_PATH ];   // Path to startup screen bitmap
    char BackgroundBMPPath[ MAX_STR_PATH ];   // Path to background bitmap
    #endif

    // Future data here

    #ifdef EBOOK_TOOL
    BYTE FutureData[ PRJPREF_FUTURE ];       // Future data
    #endif
};

typedef TProjectPrefs *PTProjectPrefs;


// This structure is never used to hold data but is used to get the
// difference in size of the same structure for the view program.
// keeps the math easy.
#ifdef EBOOK_TOOL
struct TProjectPrefsSmallSize
{
    char StartUpScreenPath[MAX_STR_PATH];   // Path to startup screen bitmap
    char BackgroundBMPPath[MAX_STR_PATH];   // Path to background bitmap
    BYTE FutureData[ PRJPREF_FUTURE ];
};
#endif


struct TClickRect
{
    WORD X;                     // X coordinate of region
    WORD Y;                     // Y coordinate of region
    WORD Width;                 // Width of region
    WORD Height;                // Height of region
    DWORD DataOffset;           // Data offset to click region file
};


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


#endif  // __EBOOK_STRUCT_H__