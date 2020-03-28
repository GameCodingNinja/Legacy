
/************************************************************************
*
*    PRODUCT:         Tools
*
*    FILE NAME:       gTools.h
*
*    DESCRIPTION:     General Tools
*
*    IMPLEMENTATION:  This module holds misc funstions for working with
*                     windows programs in general.
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __GTOOLS_H__
#define __GTOOLS_H__

#include "minTools.h"        // Minimum version of the general tools
#include "shlobj.h"          // Structs & #defines of the path selection window

// Asks a question
int AskQuestion( UINT QuectionType, char *Title, char *fmt, ... );
// Applies the windows 95 font to the handle
void ApplyWin95Font( HWND hwnd );
// A simplified use of SearchPath function
bool FileExist( char *SearchFilePath, char *SearchPathDir = NULL );
// See if a directory is there
bool DirectoryExist( char *SearchPathDir );
// Combines a file and a path from two seperate strings
void AddFileToOtherPath( char *UseFileFromPath, char *UseThisPath, char *Dest );
// Copy the file at a given path to a file handle
bool CopyFileToDest( char *FileToCopy, HANDLE hDest, unsigned char *pBuffer, unsigned int BufferSize, int Action=0, unsigned char *Key=NULL );
// Checksum the file
unsigned int CheckSumFile( HANDLE hFile );
unsigned int CheckSumFile( char *FileName );
// General purpose get file name box
bool GetFileName( HWND hwnd, HINSTANCE hInst, char *FileName, char *Title, char *DefPath, 
                  char *FileMask, char *DefExtension = NULL, int DialogType = OPEN_FILENAME_DIALOG_BOX );
// Encrypt & unecrypt the text block by adding the key
void LockItUp( unsigned char *Key, unsigned char *pData, int DataSize, int Action );
// Check to see if the menu is checked
bool IsMenuChecked( HWND hwnd, int MenuID );
// Center one rect inside of another
void CenterRectInRect( LPRECT DestRect, int SourceW, int SourceH, int DestW, int DestH );
// Display an int in an edit control
void Edit_DisplayValue( HWND hCtrl, int Value );
// Get value from an edit control
int Edit_GetValue( HWND hCtrl );
// Get the bit depth of the system from a windows handle
int GetSystemBitDepth( HWND hwnd );
// FlipFlop the int sign
int FlipFlopSign( int Value );
// Finds a character in a text string and returns it's position
int FindChar( char *SearchStr, char SearchChar, int StartIndex = 0 );
// Get the time of the file
FILETIME GetTheFileTime( char *PathToFile );
// Displays a window so that the path can be selected and is stored in a string.
bool SelectPath( HWND hwnd, char *pathToDir, char *wndCaption, unsigned int flags = BIF_RETURNONLYFSDIRS );
// Is the X Y inside the rect?
BOOL PointInsideRect( int X, int Y, LPRECT rect );
// Fast or memory copy
//void memcpy_or( void *dest, void *source, int size );
// Fast and memory copy
//void memcpy_and( void *dest, void *source, int size );

#endif  /* __SSTOOLS__ */