
/************************************************************************
*
*    PRODUCT:         MNG Class
*
*    FILE NAME:       mng.cpp
*
*    DESCRIPTION:     Object for reading MNG files
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

#include "mng.h"             // Header file for this *.cpp file.


/************************************************************************
*
*      Prototypes for static callback functions - the LibMng Callbacks
*
************************************************************************/

// Memory allocation callback function
static mng_ptr __stdcall Memalloc( mng_uint32 iLen );
// Memory free callback function
static void __stdcall Memfree( mng_ptr iPtr, mng_size_t iLen );
// Open file stream callback function
static mng_bool __stdcall Openstream( mng_handle hHandle );
// Close file stream callback function
static mng_bool __stdcall Closestream( mng_handle hHandle );
// Data reading callback function
static mng_bool __stdcall Readdata( mng_handle hHandle, mng_ptr pBuf,
                                    mng_uint32 iBuflen, mng_uint32 *pRead );
// Returns the width and height of the image
static mng_bool __stdcall ProcessHeader( mng_handle hHandle, mng_uint32 iWidth, 
                                         mng_uint32 iHeight );
// Render a line of the image
static mng_ptr __stdcall GetCanvasLine( mng_handle hHandle, mng_uint32 iLinenr );
// Reads the background line in
static mng_ptr __stdcall GetBackgroundLine( mng_handle hHandle, mng_uint32 iLinenr );
// This function is called when the image is done rendering
static mng_bool __stdcall ImageRefresh( mng_handle hHandle, mng_uint32 iX, 
                                        mng_uint32 iY, mng_uint32 iWidth, 
                                        mng_uint32 iHeight );
// This function is called when the image is done rendering
static mng_uint32 __stdcall GetTickCount( mng_handle hHandle );
// This function is called when the image is done rendering
static mng_bool __stdcall SetTimer( mng_handle hHandle, mng_uint32 iMsecs );


/************************************************************************
*    FUNCTION NAME:         Memalloc()                                                             
*
*    DESCRIPTION:           Memory allocation callback function
*
*    FUNCTION PARAMETERS:
*    Input:    mng_size_t iLen - Size-in-bytes of memory needed
*                                to be allacated
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  7/12/02       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

static mng_ptr __stdcall Memalloc( mng_uint32 iLen )
{
    // get memory from the heap
    mng_ptr pResult = malloc( iLen );             

    // Clear out the memory
    if( pResult )
        memset( pResult, 0, iLen );

    return pResult;

}   // Memalloc


/************************************************************************
*    FUNCTION NAME:         Memfree()                                                             
*
*    DESCRIPTION:           Memory free callback function
*
*    FUNCTION PARAMETERS:
*    Input:    mng_handle hHandle - Handle that identifies this 
*                                   object's instance
*              mng_size_t iLen    - Size-in-bytes of allacated memory
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  7/12/02       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

static void __stdcall Memfree( mng_ptr iPtr, mng_size_t iLen )
{
    // free the memory
    if( iPtr )
        free( iPtr );

}   // Memfree


/************************************************************************
*    FUNCTION NAME:         Openstream()                                                             
*
*    DESCRIPTION:           Open file stream callback function
*
*    FUNCTION PARAMETERS:
*    Input:    mng_handle hHandle - Handle that identifies this 
*                                   object's instance
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  7/12/02       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

static mng_bool __stdcall Openstream( mng_handle hHandle )
{
    mng_bool Result = MNG_FALSE;

    // Get the pointer to the CMNG class object
    // so that we can access the class members
    PCMNG pMNG = PCMNG( mng_get_userdata( hHandle ) );

    // Open a new file stream
    if( pMNG->OpenFileStream() )
        Result = MNG_TRUE;
        
    return Result;

}   // Openstream


/************************************************************************
*    FUNCTION NAME:         Closestream()                                                             
*
*    DESCRIPTION:           Close file stream callback function
*
*    FUNCTION PARAMETERS:
*    Input:    mng_handle hHandle - Handle that identifies this 
*                                   object's instance
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  7/12/02       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

static mng_bool __stdcall Closestream( mng_handle hHandle )
{    
    // Get the pointer to the CMNG class object
    // so that we can access the class members
    PCMNG pMNG = PCMNG( mng_get_userdata( hHandle ) );

    // Close the file stream
    pMNG->CloseFileStream();

    return MNG_TRUE;

}   // Closestream


/************************************************************************
*    FUNCTION NAME:         Readdata()                                                             
*
*    DESCRIPTION:           Data reading callback function
*
*    FUNCTION PARAMETERS:
*    Input:    mng_handle hHandle - Handle that identifies this 
*                                   object's instance
*              mng_ptr pBuf       - Pointer to the allocated buffer
*              mng_uint32 iBuflen - Size-in-bytes of how much to read in
*              mng_uint32 *pRead  - Amount of data read in to be reported
*                                   back to the MNG library
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

static mng_bool __stdcall Readdata( mng_handle hHandle, mng_ptr pBuf,
                                    mng_uint32 iBuflen, mng_uint32 *pRead )
{
    mng_bool Result = MNG_FALSE;

    // Get the pointer to the CMNG class object
    // so that we can access the class members
    PCMNG pMNG = PCMNG( mng_get_userdata( hHandle ) );

    // Read in a chunk of data to the allocated memory block
    if( pMNG->ReadFileStream( pBuf, iBuflen, pRead ) )
        Result = MNG_TRUE;

    return Result;

}   // Readdata


/************************************************************************
*    FUNCTION NAME:         ProcessHeader()                                                             
*
*    DESCRIPTION:           The name of this call back is a little missleading.
*                           All we are really doing is getting the width and
*                           height of the loaded MNG. We are also taking this
*                           oppertunity to set how we want the RGB to be layed out.
*
*    FUNCTION PARAMETERS:
*    Input:    mng_handle hHandle - Handle that identifies this 
*                                   object's instance
*              mng_uint32 iWidth  - Image width
*              mng_uint32 iHeight - Image height
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

static mng_bool __stdcall ProcessHeader( mng_handle hHandle,
                                         mng_uint32 iWidth, mng_uint32 iHeight )
{
    // Get the pointer to the CMNG class object
    // so that we can access the class members
    PCMNG pMNG = PCMNG( mng_get_userdata( hHandle ) );

    // Save the width and height of the loaded image
    pMNG->SetWidthHeight( iWidth, iHeight );
        
    return MNG_TRUE;

}   // ProcessHeader


/************************************************************************
*    FUNCTION NAME:         GetCanvasLine()                                                             
*
*    DESCRIPTION:           Render one of the scan lines of the bitmap
*
*    FUNCTION PARAMETERS:
*    Input:    mng_handle hHandle - Handle that identifies this 
*                                   object's instance
*              mng_uint32 iLinenr - Render line index
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

static mng_ptr __stdcall GetCanvasLine( mng_handle hHandle, mng_uint32 iLinenr )
{
    // Get the pointer to the CMNG class object
    // so that we can access the class members
    PCMNG pMNG = PCMNG( mng_get_userdata( hHandle ) );

    // Save the width and height of the loaded image
    return pMNG->GetCanvasScanLine( iLinenr );

}   // GetCanvasLine


/************************************************************************
*    FUNCTION NAME:         GetBackgroundLine()                                                             
*
*    DESCRIPTION:           Reads one of the background lines from a buffer
*
*    FUNCTION PARAMETERS:
*    Input:    mng_handle hHandle - Handle that identifies this 
*                                   object's instance
*              mng_uint32 iLinenr - Render line index
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

static mng_ptr __stdcall GetBackgroundLine( mng_handle hHandle, mng_uint32 iLinenr )
{
    // Get the pointer to the CMNG class object
    // so that we can access the class members
    PCMNG pMNG = PCMNG( mng_get_userdata( hHandle ) );

    // Save the width and height of the loaded image
    return pMNG->GetBackgroundScanLine( iLinenr );

}   // GetCanvasLine


/************************************************************************
*    FUNCTION NAME:         ImageRefresh()                                                             
*
*    DESCRIPTION:           This function is called when the image is done
*                           rendering. For our needs, we don't use it but
*                           it is needed to make the libmng work.
*
*    FUNCTION PARAMETERS:
*    Input:    mng_handle hHandle - Handle that identifies this 
*                                   object's instance
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

static mng_bool __stdcall ImageRefresh( mng_handle hHandle, mng_uint32 iX, 
                                        mng_uint32 iY, mng_uint32 iWidth, 
                                        mng_uint32 iHeight )
{

    return MNG_TRUE;

}   // ImageRefresh


/************************************************************************
*    FUNCTION NAME:         SetTimer()                                                             
*
*    DESCRIPTION:           This function is called when the image is done
*                           rendering. For our needs, we don't use it but
*                           it is needed to make the libmng work.
*
*    FUNCTION PARAMETERS:
*    Input:    mng_handle hHandle - Handle that identifies this 
*                                   object's instance
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

static mng_bool __stdcall SetTimer( mng_handle hHandle, mng_uint32 iMsecs )
{
    return MNG_TRUE;

}   // SetTimer


/************************************************************************
*    FUNCTION NAME:         GetTickCount()                                                             
*
*    DESCRIPTION:           This function is called when the image is done
*                           rendering. For our needs, we don't use it but
*                           it is needed to make the libmng work.
*
*    FUNCTION PARAMETERS:
*    Input:    mng_handle hHandle - Handle that identifies this 
*                                   object's instance
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

static mng_uint32 __stdcall GetTickCount( mng_handle hHandle )
{
    // Call the windows function
    return timeGetTime();

}   // GetTickCount


/************************************************************************
*    FUNCTION NAME:         IterateChunks()                                                             
*
*    DESCRIPTION:           Iterate through all the chunks counting up
*                           all the FRAM chunks to see how many frames
*                           are in the MNG
*
*    FUNCTION PARAMETERS:
*    Input:    mng_handle hHandle - Handle that identifies this 
*                                   object's instance
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

static mng_bool __stdcall IterateChunks( mng_handle hHandle, mng_handle hChunk,
                                         mng_chunkid iChunktype, mng_uint32 iChunkseq )
{
    // Get the pointer to the CMNG class object
    // so that we can access the class members
    PCMNG pMNG = PCMNG( mng_get_userdata( hHandle ) );

    // Count up the number of frame chunks there are.
    if( iChunktype == MNG_UINT_FRAM )
        pMNG->GetFrameChunk( hHandle, hChunk );

    // Count up the number of image chunks there are.
    else if( iChunktype == MNG_UINT_IHDR )
        pMNG->IncImageCount();

    return MNG_TRUE;

}   // IterateChunks


/************************************************************************
*
*                             MNG Object
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CMNG::CMNG()
{
    // Init class members
    hFile = -1;
    hMNG_lib = NULL;
    count = 0;
    pdib = NULL;
    frameIndex = 0;
    frameCount = 0;
    imageCount = 0;
    renderType = RT_NO_RENDER_TYPE;
    useBKBuffer = false;

}   // Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CMNG::~CMNG()
{ 
    // Free everything
    Free();

}   // Destructer


/************************************************************************
*    FUNCTION NAME:         Initialize()                                                             
*
*    DESCRIPTION:           Init the MNG class
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMNG::Initialize( HWND hParWnd, int renType, BOOL bkBuffer )
{
    BOOL Result = FALSE;

    // Init some class members
    hParentWnd = hParWnd;
    renderType = renType;
    useBKBuffer = bkBuffer;

    // now initialize the library
    hMNG_lib = mng_initialize( mng_ptr(this), Memalloc, Memfree, NULL );

    if( hMNG_lib == NULL )
    {
        // Display an eror message
        DisplayError( "libmng library initializiation error." );

        goto HANDLE_ERROR;
    }

    // no need to store chunk-info! Chunk info is needed if
    // you want to change the MNG data, reset animations and so on.
    if( mng_set_storechunks( hMNG_lib, MNG_TRUE ) != MNG_NOERROR )
        DisplayError( "libmng library reported an error setting store the chunks option." );

    // use suspension-buffer
    // I have no idea what this means
    if( mng_set_suspensionmode( hMNG_lib, MNG_FALSE ) != MNG_NOERROR )
        DisplayError( "libmng library reported an error setting the suspension mode option." );

    // Tell the libmng library the RGB order we need. Windows uses RGB in a different order.
    // So what we need is the RGB to be in the order of BGR. The red and blue positions are swapped.
    if( mng_set_canvasstyle( hMNG_lib, MNG_CANVAS_BGR8 ) != MNG_NOERROR )
        DisplayError( "libmng library reported an error setting the RGB order." );

    // Set the background color to out default transparent color
    if( useBKBuffer == NO_BK_BUFFER )
    {
        if( mng_set_bgcolor( hMNG_lib, 0x0000, 0xFFFF, 0x0000 ) != MNG_NOERROR )
            DisplayError( "libmng reported an error setting the background color!" );
    }
    else
    {
		// Tell the libmng library the RGB order we need. Windows uses RGB in a different order.
	    // So what we need is the RGB to be in the order of BGR. The red and blue positions are swapped.
	    if( mng_set_bkgdstyle( hMNG_lib, MNG_CANVAS_BGR8 ) != MNG_NOERROR )
	        DisplayError( "libmng library reported an error setting the RGB order." );

        // We are going to use a background canvas
        if( mng_setcb_getbkgdline( hMNG_lib, GetBackgroundLine ) != MNG_NOERROR )
        {
            // Display an eror message
            DisplayError( "libmng reported an error setting GetBackgroundLine callback function!" );

            goto HANDLE_ERROR;
        }
    }

    // set all the callbacks
    if( (mng_setcb_openstream    (hMNG_lib, Openstream   )     != MNG_NOERROR) ||
        (mng_setcb_closestream   (hMNG_lib, Closestream  )     != MNG_NOERROR) ||
        (mng_setcb_readdata      (hMNG_lib, Readdata     )     != MNG_NOERROR) ||
        (mng_setcb_processheader (hMNG_lib, ProcessHeader)     != MNG_NOERROR) ||
        (mng_setcb_getcanvasline (hMNG_lib, GetCanvasLine)     != MNG_NOERROR) ||
        (mng_setcb_refresh       (hMNG_lib, ImageRefresh )     != MNG_NOERROR) ||
        (mng_setcb_gettickcount  (hMNG_lib, GetTickCount )     != MNG_NOERROR) ||
        (mng_setcb_settimer      (hMNG_lib, SetTimer     )     != MNG_NOERROR))
    {
        // Display an eror message
        DisplayError( "libmng reported an error setting a callback function!" );

        goto HANDLE_ERROR;
    }

    // Setup some specific call backs

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // Initialize


/************************************************************************
*    FUNCTION NAME:         OpenFileStream()                                                             
*
*    DESCRIPTION:           Open the file stream
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMNG::OpenFileStream()
{
    bool Result = FALSE;

    // Check to see if we need to close the file stream
    CloseFileStream();

    // Open a new file stream
    if( ( hFile = open( fileName, O_RDONLY | O_BINARY ) ) != -1 )
        Result = TRUE;

    return Result;

}   // OpenFileStream


/************************************************************************
*    FUNCTION NAME:         CloseFileStream()                                                             
*
*    DESCRIPTION:           Close the file stream
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMNG::CloseFileStream()
{
    // Check to see if we have a file stream
    if( hFile > -1 )
    {
        // Close the file stream
        close( hFile );

        // Null the handle
        hFile = -1;
    }
}   // CloseFileStream


/************************************************************************
*    FUNCTION NAME:         ReadFileStream()                                                             
*
*    DESCRIPTION:           Read the file stream
*
*    FUNCTION PARAMETERS:
*    Input:    mng_ptr pBuf       - Pointer to the allocated buffer
*              mng_uint32 iBuflen - Size-in-bytes of how much to read in
*              mng_uint32 *pRead  - Amount of data read in to be reported
*                                   back to the MNG library
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMNG::ReadFileStream( mng_ptr pBuf, mng_uint32 iBuflen, mng_uint32 *pRead )
{
    bool Result = TRUE;
    int bytesRead;

    // Check for the end of file
    if( eof( hFile ) )
    {
        // Indicate the amount of data read to the libmng class
        *pRead = 0;
    }
    else
    {
        // Read in a chunk of data to the allocated memory block
        bytesRead = read( hFile, pBuf, iBuflen );

        // An error has occured if the bytesRead is a negative number
        if( bytesRead > -1 )
        {
            // Indicate the amount of data read to the libmng class
            *pRead = bytesRead;
        }
        else
        {
            // Indicate the amount of data read to the libmng class
            *pRead = 0;

            Result = FALSE;
        }
    }

    return Result;

}   // ReadFileStream


/************************************************************************
*    FUNCTION NAME:         LoadFromFile()                                                             
*
*    DESCRIPTION:           Load the MNG from file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMNG::LoadFromFile( char *filepath )
{
    bool Result = FALSE;
    mng_retcode retCode;
    int i, j;

    // Free all bitmap allocations
    FreeBitmaps();

    // get the path to the file
    fileName = filepath;

    // Read in the MNG file
    retCode = mng_read( hMNG_lib );

    // Check for any errors
    if( (retCode != MNG_NOERROR) && (retCode != MNG_NEEDTIMERWAIT) )
    {
        // Let them know we had an error
        DisplayError( "libmng reported an error reading the input file!" );

        goto HANDLE_ERROR;
    }

    // run through the chunk list to get the number of frames in use
    if( mng_iterate_chunks( hMNG_lib, 0, IterateChunks ) != MNG_NOERROR )
    {
        // Let them know we had an error
        DisplayError( "libmng reported an error iterating through the chunk list!" );

        goto HANDLE_ERROR;
    }

    // The MNG file has a frame and an image chunk. Use the largest one of the two
    if( frameCount > imageCount )
        count = frameCount;
    else
        count = imageCount;

    // Make sure we have some frames
    if( count )
    {
		// Create the background buffer
		if( useBKBuffer == USE_BK_BUFFER )
		{
			if( !dibBK.CreateDib( width, height, BIT_COUNT_24 ) )
            {
                // Let them know we had an error
                DisplayError( "Error allocating bitmap canvas." );

                goto HANDLE_ERROR;
            }
		}

        // Allocate one DIB to render to
        if( renderType == RT_MNG_TO_SINGLE_CANVAS )
        {
            if( !dibCanvas.CreateDib( width, height, BIT_COUNT_24 ) )
            {
                // Let them know we had an error
                DisplayError( "Error allocating bitmap canvas." );

                goto HANDLE_ERROR;
            }
        }
        // Allocate a DIB for each frame
        else if( renderType == RT_ALLOCATE_BITMAP_LIST )
        {
            // Allocate the bitmap array
            pdib = new TDIB[ count ];
            if( pdib == NULL )
            {
                // Let them know we had an error
                DisplayError( "Error allocating bitmap array." );

                goto HANDLE_ERROR;
            }

            // Create a DIB bitmap in the size and bit depth needed
            for( i = 0; i < count; ++i )
            {
                if( !pdib[i].CreateDib( width, height, BIT_COUNT_24 ) )
                {
                    // Let them know we had an error
                    DisplayError( "Error allocating bitmaps for holding mng frames." );

                    goto HANDLE_ERROR;
                }
            }

            // Render each frame to it's allocated bitmap
            for( i = 0; i < count; ++i )
            {
                // mng_display has to be called first, then mng_display_resume
                // for all the other frames.
                if( i == 0 )
                {
                    // Do a display
                    retCode = mng_display( hMNG_lib );

                    if( (retCode != MNG_NOERROR) && (retCode != MNG_NEEDTIMERWAIT) )
                    {
                        // Let them know we had an error
                        DisplayError( "libmng reported an error displaying a frame!" );

                        goto HANDLE_ERROR;
                    }
                }
                else
                {                
                    // Do a display
                    retCode = mng_display_resume( hMNG_lib );

                    if( (retCode != MNG_NOERROR) && (retCode != MNG_NEEDTIMERWAIT) )
                    {
                        // Let them know we had an error
                        DisplayError( "libmng reported an error displaying a frame!" );

                        goto HANDLE_ERROR;
                    }
                }

                // Inc the frame index so that the next bitmap is rendered
                ++frameIndex;
            }
        }

        Result = TRUE;

    }   // if( count )


HANDLE_ERROR:;

    // Reset and free the memory at the end
    if( mng_reset( hMNG_lib ) != MNG_NOERROR )
    {
        // Let them know we had an error
        DisplayError( "libmng reported an error during reset!" );
    }

    // Reset the count to zero because we had an error
    if( !Result )
        count = 0;

    return Result;

}   // LoadFromFile


/************************************************************************
*    FUNCTION NAME:         GetCanvasScanLine()                                                             
*
*    DESCRIPTION:           Get the scann line to the canvas
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BYTE * CMNG::GetCanvasScanLine( int line )
{
    // Single canvas render
    if( renderType == RT_MNG_TO_SINGLE_CANVAS )
    {
        return dibCanvas.GetScanLine( line );
    }
    // Render to specific frame
    else if( renderType == RT_ALLOCATE_BITMAP_LIST )
    {
        return pdib[ frameIndex ].GetScanLine( line );
    }
    
}   // GetCanvasScanLine


/************************************************************************
*    FUNCTION NAME:         Free()                                                             
*
*    DESCRIPTION:           Free all allocated objects
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMNG::Free()
{
    // Free the MNG library
    FreeMngLibary();

    // Free the allocated bitmaps
    FreeBitmaps();

}   // Free


/************************************************************************
*    FUNCTION NAME:         FreeBitmaps()                                                             
*
*    DESCRIPTION:           Free all allocated bitmaps
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMNG::FreeBitmaps()
{
    if( pdib != NULL )
    {
        delete [] pdib;
        pdib = NULL;
    }

	dibCanvas.Free();
	dibBK.Free();

	// Clear out the frames
	frameDelay.Free();

    frameIndex = 0;
    count = 0;
    frameCount = 0;
    imageCount = 0;

}   // FreeBitmaps


/************************************************************************
*    FUNCTION NAME:         FreeMngLibary()                                                             
*
*    DESCRIPTION:           Free the mng library
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMNG::FreeMngLibary()
{
    if( hMNG_lib )
    {
        // In the event we loaded an MNG file, release all the allocations
        if( mng_cleanup( &hMNG_lib ) != MNG_NOERROR )
            DisplayError( "libmng cleanup error" );

        // NULL the pointer
        hMNG_lib = NULL;    
    }
}   // FreeMngLibary


/************************************************************************
*    FUNCTION NAME:         IsEmpty()                                                             
*
*    DESCRIPTION:           Is an MNG loaded?
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CMNG::IsEmpty()
{
    BOOL Result = TRUE;

    if( pdib )
        Result = FALSE;

    return Result;

}   // IsEmpty


/************************************************************************
*    FUNCTION NAME:         GetFrameChunk()                                                             
*
*    DESCRIPTION:           We need to get the frame chunk to get frame
*                           specific info?
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMNG::GetFrameChunk( mng_handle hHandle, mng_handle hChunk )
{
    mng_bool    bEmpty;
    mng_uint8   iMode;
    mng_uint32  iNamesize;
    mng_pchar   zName;
    mng_uint8   iChangedelay;
    mng_uint8   iChangetimeout;
    mng_uint8   iChangeclipping;
    mng_uint8   iChangesyncid;
    mng_uint32  iDelay;
    mng_uint32  iTimeout;
    mng_uint8   iBoundarytype;
    mng_int32   iBoundaryl;
    mng_int32   iBoundaryr;
    mng_int32   iBoundaryt;
    mng_int32   iBoundaryb;
    mng_uint32  iCount;
    mng_uint32p pSyncids;

    // Get the chunk
    mng_getchunk_fram( hHandle, hChunk, &bEmpty, &iMode, &iNamesize, &zName,
                       &iChangedelay, &iChangetimeout, &iChangeclipping, &iChangesyncid,
                       &iDelay, &iTimeout, &iBoundarytype, &iBoundaryl, &iBoundaryr,&iBoundaryt, 
                       &iBoundaryb, &iCount, &pSyncids );

    // Add a frame delay value
    frameDelay.Add( (int)iDelay );

    // Inc the frame count
    ++frameCount;

}   // GetFrameChunk


/************************************************************************
*    FUNCTION NAME:         GetDIB()                                                             
*
*    DESCRIPTION:           Get the dib for blitting.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

PTDIB CMNG::GetDIB( int index )
{
	// Single canvas render
    if( renderType == RT_MNG_TO_SINGLE_CANVAS )
    {
        return &dibCanvas;
    }
    // Render to specific frame
    else if( renderType == RT_ALLOCATE_BITMAP_LIST )
    {
        return &pdib[ index ];
    }
}	// GetDIB