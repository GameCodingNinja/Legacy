
/************************************************************************
*
*    PRODUCT:         JPEG Coder/Decoder
*
*    FILE NAME:       JPEG.cpp
*
*    DESCRIPTION:     JPEG Object for bitmaps
*
*    IMPLEMENTATION:  Codes and decodes JPEG
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include "windows.h"         // Windows header file for creating windows programs. This is a must have.
#include "TJPEG.h"           // Header file for this *.cpp file.
#include "mintools.h"        // A misc group of useful functions
#include <stdio.h>           // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include "jpeglib.h"         // Include file for users of JPEG library.
#include <setjmp.h>          // Used for the optional error recovery mechanism


#define DEFAULT_METERS_PER_INCH  2834
#define BITMAP_TYPE 0x4d42
#define DEFAULT_FILE_NAME  "hfunwarejpeg.tmp"

//#include "DebugTools.h"

/*
 * ERROR HANDLING:
 *
 * The JPEG library's standard error handler (jerror.c) is divided into
 * several "methods" which you can override individually.  This lets you
 * adjust the behavior without duplicating a lot of code, which you might
 * have to update with each future release.
 *
 * Our example here shows how to override the "error_exit" method so that
 * control is returned to the library's caller when a fatal error occurs,
 * rather than calling exit() as the standard error_exit method does.
 *
 * We use C's setjmp/longjmp facility to return control.  This means that the
 * routine which calls the JPEG library must first execute a setjmp() call to
 * establish the return point.  We want the replacement error_exit to do a
 * longjmp().  But we need to make the setjmp buffer accessible to the
 * error_exit routine.  To do this, we make a private extension of the
 * standard JPEG error handler object.  (If we were using C++, we'd say we
 * were making a subclass of the regular error handler.)
 *
 * Here's the extended error handler struct:
 */

struct my_error_mgr {
  struct jpeg_error_mgr pub;    /* "public" fields */

  jmp_buf setjmp_buffer;    /* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}



/************************************************************************
*
*                         TJPEG Object
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

TJPEG::TJPEG()
{
    JPGDataSize = 0;
}   // Constructor




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

TJPEG::~TJPEG()
{

}   // Destructor



/************************************************************************
*    FUNCTION NAME:         LoadFromFile()                                                             
*
*    DESCRIPTION:           Load JPEG from file
*
*    FUNCTION PARAMETERS:
*    Input:    char *filename    - Path to file name
*
*    Output:   BYTE * - Returns a byte pointer of a decoded jpeg file
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/01/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BYTE * TJPEG::LoadFromFile( char *filename )
{
    // pointer to BITMAPINFO structure
    BITMAPINFOHEADER* Fdib;
    // pointer to BITMAPFILEHEADER structure
    BITMAPFILEHEADER* Fheader;
    // Pointer to NEW allocated memory
    BYTE *pBitData, *pData=NULL;
    int Size, i;
    PTRGBarray RGBary;
    //PTCYMKarray CYMKary;
    PTRGBquadArray RGBquadAry;

    BYTE *Result = NULL;

    // This struct contains the JPEG decompression parameters and pointers to
    // working space (which is allocated as needed by the JPEG library).

    struct jpeg_decompress_struct cinfo;
    // We use our private extension JPEG error handler.
    // Note that this struct must live as long as the main JPEG parameter
    // struct, to avoid dangling-pointer problems.

    struct my_error_mgr jerr;
    // More stuff
    FILE * infile;      // source file
    JSAMPARRAY buffer;      // Output row buffer
    int RowSize;        // physical row width in output buffer

    // In this example we want to open the input file before doing anything else,
    // so that the setjmp() error recovery below can assume the file is open.
    // VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
    // requires it in order to read binary files.

    // Get the file size
    JPGDataSize = GetTheFileSize( filename );

    if ((infile = fopen(filename, "rb")) == NULL)
    {
        MessageBox( GetActiveWindow(), "Can't open jpeg file to display", "Error", MB_ICONWARNING );
        return Result;
    }

    // Step 1: allocate and initialize JPEG decompression object

    // We set up the normal JPEG error routines, then override error_exit.
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    // Establish the setjmp return context for my_error_exit to use.
    if (setjmp(jerr.setjmp_buffer)) 
    {
        MessageBox( GetActiveWindow(), "Error setting JPEG jump buffer", "Error", MB_ICONWARNING );
        // If we get here, the JPEG code has signaled an error.
        // We need to clean up the JPEG object, close the input file, and return.
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return Result;
    }

    // Now we can initialize the JPEG decompression object.
    jpeg_create_decompress(&cinfo);

    // Step 2: specify data source (eg, a file)

    jpeg_stdio_src(&cinfo, infile);

    // Step 3: read file parameters with jpeg_read_header()

    (void) jpeg_read_header(&cinfo, TRUE);
    // We can ignore the return value from jpeg_read_header since
    //   (a) suspension is not possible with the stdio data source, and
    //   (b) we passed TRUE to reject a tables-only JPEG file as an error.
    // See libjpeg.doc for more info.

    // Step 4: set parameters for decompression

    // In this example, we don't need to change any of the defaults set by
    // jpeg_read_header(), so we do nothing here.

    // Step 5: Start decompressor

    (void) jpeg_start_decompress(&cinfo);
    // We can ignore the return value since suspension is not possible
    // with the stdio data source.

    // We may need to do some setup of our own at this point before reading
    // the data.  After jpeg_start_decompress() we have the correct scaled
    // output image dimensions available, as well as the output colormap
    // if we asked for color quantization.
    // In this example, we need to make an output work buffer of the right size.
 
    // JSAMPLEs per row in output buffer
    RowSize = cinfo.output_width * cinfo.output_components;

    // Make sure the size is DWORD alligned. Windows likes it that way.
    if( RowSize % sizeof(DWORD) )
        RowSize += sizeof(DWORD) - (RowSize % sizeof(DWORD));

    // Make a one-row-high sample array that will go away when done with image
    buffer = (*cinfo.mem->alloc_sarray)
        ((j_common_ptr) &cinfo, JPOOL_IMAGE, RowSize, 1);

    // Tell the user we don't support CYMK JPEGs
    // I let the JPEG code still do it's thing because it is happier that way.
    if( cinfo.output_components == 4 )
    {
        MessageBox( GetActiveWindow(), 
        "This program does not support CYMK JPEG files. CYMK JPEG files are much larger in size then RGB JPEG files. You'll need to convert your CYMK JPEG files to RGB JPEG files.", 
        "Error CYMK JPEG", MB_ICONWARNING );    
    }

    // allocate memory to hold the bitmap
    Size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (RowSize * cinfo.output_height);

    // Make room for the palette if this is an 8 bit image
    if( cinfo.output_components == 1 )
        Size += sizeof( RGBQUAD )*256;

    // Only allocate the buffer if it is not a CYMK JPEG
    if( cinfo.output_components != 4 )
        pData = new BYTE[ Size ];

    // Did we allocate memory?
    if( pData && buffer )
    {
        // Index past the point of the BITMAPINFOHEADER. This is where we will be
        // writing the RGB data
        if( cinfo.output_components == 1 )
            pBitData = pData + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + (sizeof( RGBQUAD )*256);
        else
            pBitData = pData + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);

        // Make the palette if this is a Gray Scale JPEG
        if( cinfo.output_components == 1 )
        {
            RGBquadAry = PTRGBquadArray( pData + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) );
            for( i = 0; i < 256; ++i )
            {
                RGBquadAry->color[i].rgbBlue = i;
                RGBquadAry->color[i].rgbGreen = i;
                RGBquadAry->color[i].rgbRed = i;
                RGBquadAry->color[i].rgbReserved = 0;
            }   
        }

        /* Step 6: while (scan lines remain to be read) */
        /*           jpeg_read_scanlines(...); */

        // Here we use the library's state variable cinfo.output_scanline as the
        // loop counter, so that we don't have to keep track ourselves.
        while( cinfo.output_scanline < cinfo.output_height )
        {
            // jpeg_read_scanlines expects an array of pointers to scanlines.
            // Here the array is only one element long, but you could ask for
            // more than one scanline at a time if that's more convenient.
            (void) jpeg_read_scanlines(&cinfo, buffer, 1);

            // RGB Conversion
            if( cinfo.output_components == 3 )
            {
                RGBary = PTRGBarray( buffer[0] );

                // For the image to look right, the red and the blue need to switch places.
                // Also, a DIB is bottom to top so we have to write from the bottom up.
                for( i = 0; i < cinfo.output_width; ++i )
                {
                    (*(pBitData + (i * cinfo.output_components) + ((cinfo.output_height - cinfo.output_scanline) * RowSize))) = RGBary->color[i].Blue;
                    (*(pBitData + (i * cinfo.output_components) + 1 + ((cinfo.output_height - cinfo.output_scanline) * RowSize))) = RGBary->color[i].Green;
                    (*(pBitData + (i * cinfo.output_components) + 2 + ((cinfo.output_height - cinfo.output_scanline) * RowSize))) = RGBary->color[i].Red;
                }
            }
            // Could not make work properly
            // CYMK Conversion
            /*else if( cinfo.output_components == 4 )
            {
                CYMKary = PTCYMKarray( buffer[0] );

                // For the image to look right, the red and the blue need to switch places.
                // Also, a DIB is bottom to top so we have to write from the bottom up.
                for( i = 0; i < cinfo.output_width; ++i )
                {
                    (*(pBitData + (i * cinfo.output_components) + ((cinfo.output_height - cinfo.output_scanline) * RowSize))) = CYMKary->color[i].Magenta;
                    (*(pBitData + (i * cinfo.output_components) + 1 + ((cinfo.output_height - cinfo.output_scanline) * RowSize))) = CYMKary->color[i].Yellow;
                    (*(pBitData + (i * cinfo.output_components) + 2 + ((cinfo.output_height - cinfo.output_scanline) * RowSize))) = CYMKary->color[i].Cyan;
                    (*(pBitData + (i * cinfo.output_components) + 3 + ((cinfo.output_height - cinfo.output_scanline) * RowSize))) = 10;//CYMKary->color[i].Black;
                }
            }*/
            // Index Color Conversion
            else if( cinfo.output_components == 1 )
                memcpy( (pBitData + ((cinfo.output_height - cinfo.output_scanline) * RowSize)), buffer[0], RowSize );
        }
        // Fill out the Windows bitmap file headers

        // Don't make a Windows bitmap if it is a CYMK JPEG
        if( cinfo.output_components != 4 )
        {
            // Setup the bitmap file structure
            Fheader = (BITMAPFILEHEADER *)pData;
            Fheader->bfType = BITMAP_TYPE; 
            Fheader->bfSize = Size; 
            Fheader->bfReserved1 = 0; 
            Fheader->bfReserved2 = 0;
            if( cinfo.output_components == 1 ) 
                Fheader->bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + (sizeof( RGBQUAD )*256);
            else
                Fheader->bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);

            // Setup the bitmap info structure
            Fdib = (BITMAPINFOHEADER *)(pData + sizeof(BITMAPFILEHEADER));
            Fdib->biSize = sizeof(BITMAPINFOHEADER);
            Fdib->biWidth = cinfo.output_width;
            Fdib->biHeight = cinfo.output_height;
            Fdib->biPlanes = 1;
            if( cinfo.output_components == 3 )
                Fdib->biBitCount = 24;
            //else if( cinfo.output_components == 4 )
            //  Fdib->biBitCount = 32;
            else if( cinfo.output_components == 1 )
                Fdib->biBitCount = 8;
            Fdib->biCompression = BI_RGB;
            Fdib->biSizeImage = 0;
            Fdib->biXPelsPerMeter = DEFAULT_METERS_PER_INCH;
            Fdib->biYPelsPerMeter = DEFAULT_METERS_PER_INCH;
            Fdib->biClrUsed = 0;
            Fdib->biClrImportant = 0;

            // Set the result to the byte pointer
            Result = pData;
        }
    }
    else
        MessageBox( GetActiveWindow(), "Error allocating memory", "Error", MB_ICONWARNING );

    // Step 7: Finish decompression

    (void) jpeg_finish_decompress(&cinfo);
    // We can ignore the return value since suspension is not possible
    // with the stdio data source.

    // Step 8: Release JPEG decompression object

    // This is an important step since it will release a good deal of memory.
    jpeg_destroy_decompress(&cinfo);

    // After finish_decompress, we can close the input file.
    // Here we postpone it until after no more JPEG errors are possible,
    // so as to simplify the setjmp error logic above.  (Actually, I don't
    // think that jpeg_destroy can do an error exit, but why assume anything...)

    fclose(infile);

    // At this point you may want to check to see whether any corrupt-data
    // warnings occurred (test whether jerr.pub.num_warnings is nonzero).
    if( jerr.pub.num_warnings )
        MessageBox( GetActiveWindow(), "Error decoding JPEG file", "Error", MB_ICONWARNING );

  // And we're done!
    return Result;
}


/************************************************************************
*    FUNCTION NAME:         LoadFromBuffer()                                                             
*
*    DESCRIPTION:           Takes a pointer to a buffer and writes
*                           the data to a temporary file in the
*                           windows directory. Then the file
*                           is read in and docoded.
*
*    FUNCTION PARAMETERS:
*    Input:    BYTE *pData    - Buffered data
*              DWORD Size     - Size of buffer
*
*    Output:   BYTE * - Returns a byte pointer of a decoded jpeg file
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/01/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BYTE * TJPEG::WriteToFileAndLoad( BYTE *pData, DWORD Size )
{
    unsigned long bytesWritten;
    SECURITY_ATTRIBUTES FileSec;
    HANDLE hFile;
    char FilePath[MAX_PATH];
    BYTE *Result = NULL;

    FileSec.nLength = sizeof( SECURITY_ATTRIBUTES );
    FileSec.lpSecurityDescriptor = NULL;
    FileSec.bInheritHandle = FALSE;

    // Get's the windows directory and adds a back slash
    GetTheWindowsTmpDir( FilePath );

    // Add the default file name
    strcat( FilePath, DEFAULT_FILE_NAME );

    // Create the file
    hFile = CreateFile( FilePath, GENERIC_WRITE, FILE_SHARE_WRITE,
                         &FileSec, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );

    if( hFile != INVALID_HANDLE_VALUE )
    {
        // Write the jpeg data to the temporary file
        WriteFile( hFile, pData, sizeof(BYTE)*Size, &bytesWritten, NULL );

        // Close the file handle
        CloseHandle( hFile );

        // Read in the data and decode it. I would prefer to do this all
        // in memory and save a step but the jpeg code is to complicated
        // to convert it to decode from a buffer.
        Result = LoadFromFile( FilePath );

        // We are done with the temporary file so delete it
        DeleteFile( FilePath );
    }

    return Result;

}   // WriteToFileAndLoad



/************************************************************************
*    FUNCTION NAME:         LoadFromResource()                                                             
*
*    DESCRIPTION:           Load jpeg from resource file. Takes a pointer to
*                           a buffer and writes the data to a temporary
*                           file in the windows directory. Then the file
*                           is read in and docoded.
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst   - Program Instance
*              char *name        - Name of the resource
*              char *type        - Resource Type Id
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

BYTE * TJPEG::LoadFromResource( char *name, char *type, HINSTANCE hInst )
{
    TLoadRes LoadRes;

    // Load the file from the resource
    if( !LoadTheResource( hInst, name, type, &LoadRes ) )
        return NULL;

    // Get the file size
    JPGDataSize = LoadRes.Size;

    return WriteToFileAndLoad( (BYTE *)LoadRes.pData, LoadRes.Size );

}   // LoadFromResource



/************************************************************************
*    FUNCTION NAME:         LoadFromBuffer()                                                             
*
*    DESCRIPTION:           Load JPEG from a buffer. Takes a pointer to
*                           a buffer and writes the data to a temporary
*                           file in the windows directory. Then the file
*                           is read in and docoded.
*
*    FUNCTION PARAMETERS:
*    Input:    BYTE *pData    - Buffered data
*              DWORD Size     - Size of buffer
*
*    Output:   BYTE * - Returns a byte pointer of a decoded jpeg file
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/01/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BYTE * TJPEG::LoadFromBuffer( BYTE *pData, DWORD Size )
{
    BYTE *Result;

    // Get the file size
    JPGDataSize = Size;

    Result = WriteToFileAndLoad( pData, Size );

    delete [] pData;

    return Result;

}   // LoadFromResource
