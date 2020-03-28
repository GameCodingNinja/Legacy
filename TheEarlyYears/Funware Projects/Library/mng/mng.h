
/************************************************************************
*
*    PRODUCT:         MNG Class
*
*    FILE NAME:       mng.h
*
*    DESCRIPTION:     Object for reading MNG files
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __MNG_CLASS_H__
#define __MNG_CLASS_H__

//#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
//#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include <io.h>
#include <fcntl.h>
#include "dib.h"             // Dib class for displaying and animating bitmaps
#include "TIntList.h"        // Int list class

// These MUST be defined before we include "Libmng.h
# define MNG_SUPPORT_READ
# define MNG_SUPPORT_WRITE
# define MNG_ACCESS_CHUNKS
# define MNG_STORE_CHUNKS
# define MNG_NO_CMS
# define MNG_USE_DLL
# define MNG_SUPPORT_DISPLAY
//# define MNG_SKIP_ZLIB          // we don't need the zlib definitions here
//# define MNG_SKIP_IJG6B         // we don't need the IJG definitions here
#include "libmng.h"

#define USE_BK_BUFFER  TRUE
#define NO_BK_BUFFER   FALSE

enum
{
	RT_NO_RENDER_TYPE=0,
	RT_MNG_TO_SINGLE_CANVAS,	// Have the MNG class render to the canvas. No contol over the frames
	RT_ALLOCATE_BITMAP_LIST,	// Allocate a bitmap for each animation frame.
};


class CMNG
{
protected:
    // input stream
    int hFile;
    // the libray handle
    mng_handle hMNG_lib;
    // filename of the input stream
    char *fileName;
    // Width and height of the image
    int width, height;
    // Handle to the parent window
    HWND hParentWnd;
    // Number of frames
    int count;
    // Bitmap
    PTDIB pdib;
	// Single canvas
	TDIB dibCanvas;
	// Background buffer
	TDIB dibBK;
    // Frame count index
    int frameIndex;
	// Frame and image coint
	int frameCount, imageCount;
	// Render type
	int renderType;
	// Use a background buffer
	BOOL useBKBuffer;
	// Frame delay list array
	TIntList frameDelay;

public:
    // Constructor
    CMNG();
    // Destructor
    ~CMNG();

    // Init the MNG class
    bool Initialize( HWND hParWnd = NULL, int renType = RT_MNG_TO_SINGLE_CANVAS, BOOL bkBuffer = false );
    // Get the filename
    char *GetFileName(){ return fileName; };

    // Open a file stream
    bool OpenFileStream();
    // Open a file stream
    void CloseFileStream();
    // Read the file stream
    bool ReadFileStream( mng_ptr pBuf, mng_uint32 iBuflen, mng_uint32 *pRead );
    // Set the width and height of the image
    void SetWidthHeight( int w, int h ){ width = w, height = h; };
    // Display an error message
    void DisplayError( char *message ){ MessageBox( hParentWnd, message, "error", MB_OK ); };
    // Load the MNG from file
    bool LoadFromFile( char *pfilename );
    // Get the scan line of the bitmap
    BYTE * GetCanvasScanLine( int line );
	// Get the scan line of the background
    BYTE * GetBackgroundScanLine( int line ){ return dibBK.GetScanLine( line ); };
    // Free all allocated bitmaps
    void FreeBitmaps();
    // Free the mng library
    void FreeMngLibary();
    // Free all allocated objects
    void Free();
    // Number of frames in this mng file
    int Count(){ return count; };
    // Get the pointer to the DIB array
    PTDIB GetDIBptr(){ return pdib; };
    // Get the pointer to a individual dib
    PTDIB GetDIB( int index = 0 );
	// Get the background
    PTDIB GetDIBbackCanvas(){ return &dibBK; };
    // Count up the number of frames in the MNG file
    void IncFrameCount(){ ++frameCount; };
    // Count up the number of images in the MNG file
    void IncImageCount(){ ++imageCount; };
	// Get the frame chunk for additional information
	void GetFrameChunk( mng_handle hHandle, mng_handle hChunk );
    // Is an MNG loaded?
    BOOL IsEmpty();
	// Get the width and height
	int Width(){ return width; };
	int Height(){ return height; };
};

typedef CMNG *PCMNG;


#endif  // __MNG_CLASS_H__