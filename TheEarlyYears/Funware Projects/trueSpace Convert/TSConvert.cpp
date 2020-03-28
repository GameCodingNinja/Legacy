
/************************************************************************
*
*    PRODUCT:         true space file convert
*
*    FILE NAME:       TSConvert.cpp
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
#include "CStrList.h"        // Object file for holding lists of strings
#include "CTSParser.h"		 // Convert object
#include "Status.h"			 // Status object
#include "TSConvert.h"       // Header file for this *.cpp file.


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
    CTrueSpaceParser trueSpacePars;
    char filePath[MAX_PATH+1], fileNnme[MAX_PATH+1];
    CStringLst<char> stringLst;

    // Get the path from the command line
    ParsePathFromCmdLine( GetCommandLine(), filePath );

    // Crop out the path
    CropOutPath( NULL, filePath );

    // Get all the wav files in the current directory the program is run in
    stringLst.GetAllFileNamesInDir( filePath, "*.scn" );
	stringLst.GetAllFileNamesInDir( filePath, "*.cob" );

	// Prepare the status window if we have somehting to show
	if( stringLst.Count() )
	{
		// Create the status window
		DisplayStatusWnd( hInst, NULL, FALSE );

		// Set the status bar
		StatusWnd_SetStatusBar( 0, stringLst.Count(), 1 );

	    // Convert all the wav files
	    for( int i = 0; i < stringLst.Count(); ++i )
	    {
	        // Pull the file name from the path
	        CropFileNameFromPath( stringLst.Get( i ), fileNnme );

	        // Display what file is currently being converted
			StatusWnd_SetStatusTxt( "Converting File:  %s", fileNnme );

			// Move the progress bar
			StatusWnd_MoveStatusBar();

			// Give it a second to update
			Sleep(50);
	        
	        // Load from file
	        if( !trueSpacePars.LoadFromFile( stringLst.Get( i ) ) )
	        {
	            switch( trueSpacePars.GetErrorCode() )
	            {
	            	case ERROR_ALLOCATION_FAIL:
						PostMsg( "Error", "Error allocating memory to parse file (%s).", stringLst.Get( i ) );
					break;
	            	case ERROR_OBJ_FILE_HANDLE_FAIL:
						PostMsg( "Error", "Error opening OBJ file (%s).", stringLst.Get( i ) );
					break;
	            	case ERROR_PLY_FILE_HANDLE_FAIL:
						PostMsg( "Error", "Error creating PLY file (%s).", trueSpacePars.GetErrorFilePath() );
					break;
	            	case ERROR_H_FILE_HANDLE_FAIL:
						PostMsg( "Error", "Error creating H file (%s).", trueSpacePars.GetErrorFilePath() );
					break;
	            	case ERROR_MAP_FILE_HANDLE_FAIL:
						PostMsg( "Error", "Error creating MAP file (%s).", trueSpacePars.GetErrorFilePath() );
					break;
					case ERROR_TXT_LIB_FILE_HANDLE_FAIL:
						PostMsg( "Error", "Error creating LIB file (%s).", trueSpacePars.GetErrorFilePath() );
					break;
	            	case ERROR_DATA_OUT_OF_RANGE:
						PostMsg( "Error", "Out Of Range data error for file (%s).", stringLst.Get( i ) );
					break;
	            	case ERROR_EMPTY_GROUP:
						PostMsg( "Error", "Empty Group data error for file (%s).", stringLst.Get( i ) );
					break;
	            	case ERROR_MATCH_NOT_FOUND_VERTICES:
						PostMsg( "Error", "Match Not Found for vertex data error for file (%s).", stringLst.Get( i ) );
					break;
					case ERROR_MATCH_NOT_FOUND_TEXTURE:
						PostMsg( "Error", "Match Not Found for texture error for file (%s).", stringLst.Get( i ) );
					break;
	            	case ERROR_FILE_WRITE:
						PostMsg( "Error", "Write error for file (%s).", trueSpacePars.GetErrorFilePath() );
					break;
	            	case ERROR_TEXT_LIB_ALLOC:
						PostMsg( "Error", "Error allocating memory for texture library (%s).", stringLst.Get( i ) );
					break;
	            	case ERROR_LOAD_TEXTURE:
						PostMsg( "Error", "Error loading texture (%s). Texture file may be missing or may not conform to library specs (24/32 bit; width must be 4,8,16,32,64,128,256 only ).", trueSpacePars.GetErrorFilePath() );
					break;
					case ERROR_CONVERT_TEXTLIB_TO_16:
						PostMsg( "Error", "Error creating 16 texture library from the generated 32 library.", trueSpacePars.GetErrorFilePath() );
					break;
					case ERROR_CAL_NORMALS_FILE:
						PostMsg( "Error", "Error calculating the normals.", trueSpacePars.GetErrorFilePath() );
					break;
					case ERROR_SAVE_NORMALS_FILE:
						PostMsg( "Error", "Error saving the normals file.", trueSpacePars.GetErrorFilePath() );
					break;
					case ERROR_VERTEX_OUT_OF_RANGE:
						PostMsg( "Error", "Number of vertexes per polygon is out of range.", trueSpacePars.GetErrorFilePath() );
					break;
					case ERROR_OBJ_CENTER_AXIS_USED:
						PostMsg( "Error", "Object uses center axises." );
					break;

				}

	            break;
	        }
	    }

		// Kill off the status window if we made one
		StatusWnd_HideStatusWnd();
	}

    return 0;

}   // WinMain


