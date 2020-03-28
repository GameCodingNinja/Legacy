
/************************************************************************
*
*    PRODUCT:         OBJ file convert
*
*    FILE NAME:       objConvert.cpp
*
*    DESCRIPTION:     Converts all the obj files in a folder where the 
*                     program runs, to PLY, BT, MAP & H files.
*
*    IMPLEMENTATION:  Read it in an spit back out only what we need. 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "gTools.h"          // A misc group of useful functions
#include "Status.h"			 // Status object
#include "complzss.h"        // Header file for this *.cpp file.


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         WinMain                                                             
*
*    DESCRIPTION:           The entry point for all good windows programs
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HINSTANCE hPrevInstance - Of no value to 32 bit windows programs
*              LPSTR lpszCmdParam - The command line is sent by Windows. The command
*                                   originally has the path to this program but
*                                   Windows parses it out before it hands it over.
*                                   To get the command line with the path in it you
*                                   need to call GetCommandLine() API call.
*              int nCmdShow - Specifies how the window is to be shown.
*                             Example: SW_SHOWMINIMIZED, SW_SHOWMAXIMIZED   
*
*    Output:   int - Usually it's the last message wParam.
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInstance,
                            LPSTR lpszCmdParam, int nCmdShow )
{
    char fileName[MAX_PATH+1], file[MAX_PATH+1], fileTmp[MAX_PATH+1];

    // Create the status window
	DisplayStatusWnd( hInst, NULL, FALSE );

    // Set the status bar
	StatusWnd_SetStatusBar( 0, 1, 1 );

	// Get the file name from the path
	CropFileNameFromPath( lpszCmdParam, file );
	
	// Display what file is currently being converted
	StatusWnd_SetStatusTxt( "Compressing File: %s", file );

	// Replace the file extension
	RelpaceFileExtension( lpszCmdParam, fileName, ".lzw" );
	RelpaceFileExtension( lpszCmdParam, fileTmp, ".bac" );

	Sleep(20);

	//CompressFile( lpszCmdParam, fileName );

	StatusWnd_SetStatusTxt( "Deompressing File: %s", file );
	Sleep(20);

    //DecompressFile( fileName, fileTmp );
	DecompressFile( "C:\\Funwar~1\\fileco~1\\temp.lzw", "temp.bac" );

    // Move the progress bar
	StatusWnd_MoveStatusBar();

	// Give it a second to update
	Sleep(100);

    // Kill off the status window if we made one
	StatusWnd_HideStatusWnd();

    return 0;

}   // WinMain


