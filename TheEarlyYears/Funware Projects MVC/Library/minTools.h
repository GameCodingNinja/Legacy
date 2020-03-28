
/************************************************************************
*
*    PRODUCT:         Minimum Tools
*
*    FILE NAME:       minTools.h
*
*    DESCRIPTION:     General Tools
*
*    IMPLEMENTATION:  This module holds misc funstions for working with
*                     windows programs in general. This is the minimum
*                     amout of tools need for the library files.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __MINTOOLS_H__
#define __MINTOOLS_H__

#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <vector>
#include <string>

using namespace std;

// defines
#define RETURN_CR           0x0d
#define LINE_FEED           0x0a
#define NUM_0_KEY           0x30
#define NUM_9_KEY           0x39
#define SPACE_CR            0x20
#define TAB_CR              0x09
#define BACK_SLASH          0x5c
#define SHOW_CURSOR         TRUE
#define HIDE_CURSOR         FALSE
#define DEFAULT_BLACK       0x000000
#define CHECK_SUM_ERROR     0xFFFFFFFF
#define BACKSPACE_KEY       0x8
#define DELETE_KEY          0x2E
#define PERIOD_KEY          0xBE
#define LEFT_ARROW_KEY      0x25
#define RIGHT_ARROW_KEY     0x27
#define UP_ARROW_KEY        0x26
#define DOWN_ARROW_KEY      0x28

// Windows 98 GetSystemMetrics #defines
#define SM_XVIRTUALSCREEN       76
#define SM_YVIRTUALSCREEN       77
#define SM_CXVIRTUALSCREEN      78
#define SM_CYVIRTUALSCREEN      79
#define SM_CMONITORS            80


////////////////////////////
//    Font Defines
////////////////////////////

#define FONT_REGULAR         0L
#define FONT_BOLD            1L
#define FONT_ITALIC          2L
#define FONT_UNDERLINE       4L
#define FONT_STRIKEOUT       8L


////////////////////////////
//    ENUMS
////////////////////////////

enum
{
    LEFT_JUSTIFY=0,
    CENTER_JUSTIFY,
    RIGHT_JUSTIFY,
    LEFT_RIGHT_JUSTIFY,
};

enum
{
    PRINTER_OUTPUT=0,
    SCREEN_OUTPUT,
    RETURN_HEIGHT_IN_PIXELS,
};

enum
{
    NO_ENCRYPTION=0,
    ENCRYPT_DATA,
    UNENCRYPT_DATA,
};

enum
{
    OPEN_FILENAME_DIALOG_BOX=0,
    SAVE_FILENAME_DIALOG_BOX,
};

// File types
enum
{
    FILE_BK_LOAD_ERROR=-2,
    FILE_LOAD_ERROR=-1,
    START_FILE_TYPE=0,
    NO_FILE_TYPE=0,
    FILE_TYPE_BMP,
    FILE_TYPE_JPG,
    FILE_TYPE_GIF,
    FILE_TYPE_META,
    FILE_TYPE_TXT,
    FILE_TYPE_COLOR,
    FILE_TYPE_EBP,  // ebook page
    FILE_TYPE_RTF,
    FILE_TYPE_WAV,
    FILE_TYPE_MP3,
    FILE_TYPE_MID,
    FILE_TYPE_RMI,
    FILE_TYPE_AVI,
    FILE_TYPE_DXF,
    FILE_TYPE_PLY,
	FILE_TYPE_PNG,
    FILE_TYPE_LRF,
    FILE_TYPE_ILI,
	FILE_TYPE_TGA,
    MAX_FILE_TYPES,
};

////////////////////////////
//    Structures
////////////////////////////

struct TLoadRes
{
    HANDLE Handle;
    HRSRC hRes;
    unsigned int Size;
    void *pData;

    TLoadRes()
    {
        Handle = NULL;
        hRes = NULL;
        Size = 0;
        pData = NULL;   
    };
};

typedef TLoadRes *PTLoadRes;


// Loads a resourse and does all the checks and fills out a structure
bool LoadTheResource( HINSTANCE hInst, char *name, char *type, PTLoadRes pLoadRes );
// Checksums quad aligned data and if the checksum is greater than 0, returns false
bool CheckSumData( void *pData, unsigned int SizeInBytes );
// Shows a message box that works like sprintf
void PostMsg( char *Title, char *fmt, ... );
// Writes a block of data out to a file.
bool WriteDataToFile( char *FileName, unsigned char *pBuffer, unsigned int size, unsigned int Flags = OPEN_ALWAYS );
// Center a window
void CenterWindow( HWND hwnd, int Width, int Height, HWND hParentWnd = NULL, bool IsChildWnd = FALSE );
// get the windows directory and add the back slash
void GetTheWindowsDirectory( char *WinDir, bool AddBackSlash = TRUE );
// get the windows temp directory and add the back slash
void GetTheWindowsTmpDir( char *WinTmpDir );
// Gets the resource and saves it to the hard drive 
bool SaveResourceToDisk( HINSTANCE hInst, char *name, char *type, char *FileName );
// wrap text to the given device
unsigned int WrapTextToDevice( HDC DC, char *Text, int LeftMargin, int TopMargin, int TextWidth, int TextHeight, int Leading = 0, int OutPutType = SCREEN_OUTPUT, int Justify = LEFT_JUSTIFY );
// Make a font
HFONT MakeFont( HDC DC, char *Name, int Size, unsigned int Style = FONT_REGULAR, COLORREF mColor = DEFAULT_BLACK, int Angle = 0, LOGFONT *pLogFont = NULL );
// Fit source width/Height into Dest Width/Height
bool SizeToFitRect( RECT *DestRect, int SourceW, int SourceH, int DestW, int DestH );
// Use a semaphore to see if this program is already running
bool DoExist( char *SemaphoreName );
// Takes the file path and returns the file type
unsigned int GetFileType( char *FileName );
// Show and hide the cursor
void ShowHideCursor( bool ShowHide );
// Get the size of a file
unsigned int GetTheFileSize( char *FileName );
// Parses out the *.exe path from the command line.
// Sometimes there is junk in the command line
void ParsePathFromCmdLine( char *CommandLine, char *Buffer );
// Strip the path from a file name path string
void CropOutPath( char *SourceFilePath, char *DestPath );
// Strip the ".ext" of the end of a file path or
// file and replace with different extension
void RelpaceFileExtension( char *SourceFileName, char *DestFileName, char *Ext );
// Strip the file extension from the file path
void CropExtensionFromPath( char *SourceFilePath, char *DestFileName );
// Quad memset
void memsetQuad( void *dest, unsigned int value, unsigned int count );
// Returns the width of the computer screen
int GetScreenWidth();
// Returns the Height of the computer screen
int GetScreenHeight();
// Returns the width of the computer desktop which could include duel monitors
int GetDesktopWidth();
// Returns the Height of the computer desktop which could include duel monitors
int GetDesktopHeight();
// Strip the file name from the file path
void CropFileNameFromPath( const char *SourceFilePath, char *DestFileName, BOOL KeepExtension = TRUE );
// Search a string and replace a character with	a different character
void RelpaceChar( char *charString, char find, char replace );
// Add file paths to a string vector
void GetAllFileNamesInDir( vector<string> &filePath, string dir, string ext );

#endif  // __MINTOOLS_H__