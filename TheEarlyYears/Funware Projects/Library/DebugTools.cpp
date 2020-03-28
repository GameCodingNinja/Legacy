
/************************************************************************
*
*    PRODUCT:         Debuging Functions
*
*    FILE NAME:       DebugTools.cpp
*
*    DESCRIPTION:     Screen Saver Tools
*
*    IMPLEMENTATION:  This module holds misc funstions for debugging
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "DebugTools.h"      // Header file for this *.cpp file.

#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf



/************************************************************************
*    FUNCTION NAME:         TextToFile                                                             
*
*    DESCRIPTION:           Used for debugging to send text output to a file.
*                           Be sure to pass 
*                           char ReturnCR[] = { RETURN_CR, LINE_FEED, 0 };
*                           as your last paramater so that you have returns
*                           after each line of text.
*
*    FUNCTION PARAMETERS:
*    Input:    char *Title - Title of the windows message box
*              char *fmt   - Used with vprintf to get the argument list
*                            for wvsprintf.
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void Debug_TextToFile( char *FilePath, char *S, ... )
{
    char StrBuffer[2048];
    va_list argptr;
    unsigned long bytesWritten;
    SECURITY_ATTRIBUTES FileSec;
    HANDLE hFile;

    FileSec.nLength = sizeof( SECURITY_ATTRIBUTES );
    FileSec.lpSecurityDescriptor = NULL;
    FileSec.bInheritHandle = FALSE;

    // Create the file
    hFile = CreateFile( FilePath, GENERIC_WRITE, FILE_SHARE_READ,
                         &FileSec, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );

    if( hFile == INVALID_HANDLE_VALUE ) goto HANDLE_ERROR;

    // Set the file pointer to the end of the file
    SetFilePointer( hFile, 0L, 0L, FILE_END );

    va_start( argptr, S );
    vprintf( S, argptr );
    wvsprintf( StrBuffer, S, argptr );

    // Write your string
    WriteFile( hFile, StrBuffer, lstrlen(StrBuffer), &bytesWritten, NULL );

    va_end( argptr );

    CloseHandle( hFile );

    return;


    HANDLE_ERROR:;

    if( hFile ) CloseHandle( hFile );

}   // Debug_TextToFile



/************************************************************************
*    FUNCTION NAME:         Debug_DataToFile                                                             
*
*    DESCRIPTION:           Used for debugging to send data output to a file.
*
*    FUNCTION PARAMETERS:
*    Input:    char *Title - Title of the windows message box
*              void *data  - Pointer to data
*              int Size    - Size of data in bytes
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void Debug_DataToFile( char *FilePath, void *data, int Size )
{
    unsigned long bytesWritten;
    SECURITY_ATTRIBUTES FileSec;
    HANDLE hFile;

    FileSec.nLength = sizeof( SECURITY_ATTRIBUTES );
    FileSec.lpSecurityDescriptor = NULL;
    FileSec.bInheritHandle = FALSE;

    // Create the file
    hFile = CreateFile( FilePath, GENERIC_WRITE, FILE_SHARE_READ,
                         &FileSec, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );

    if( hFile != INVALID_HANDLE_VALUE )
	{
	    // Set the file pointer to the end of the file
	    SetFilePointer( hFile, 0L, 0L, FILE_END );

	    // Write your string
	    WriteFile( hFile, data, sizeof(BYTE)*Size, &bytesWritten, NULL );

		// Close the file handle
	    CloseHandle( hFile );
	}

}	// Debug_DataToFile