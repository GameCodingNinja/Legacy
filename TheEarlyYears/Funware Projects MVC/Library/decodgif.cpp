/************************************************************************
*
*    PRODUCT:         GIF Decoder
*
*    FILE NAME:       DecodGif.cpp
*
*    DESCRIPTION:     Gif drcoder
*
*    IMPLEMENTATION:  I didn't write this code. I found it as a set of C
*                     function calls and everything was static, the functions
*                     and all the data members. All I did was to move it into
*                     it's own object, replace all the library calls with API
*                     calls and clean it up a bit and fix a few problems. I
*                     like to make use of Windows API call instead of carring
*                     around libraries because the Windows API calls are free
*                     and libraries can bulk up the code fast.
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#define STRICT
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "DecodGif.h"
#include "minTools.h"



#define BITMAP_TYPE 0x4d42

//#include <stdio.h>
//#include <string.h>
//#include <malloc.h>


/* DECODE.C - An LZW decoder for GIF
 * Copyright (C) 1987, by Steven A. Bennett
 *
 * Permission is given by the author to freely redistribute and include
 * this code in any program as long as this credit is given where due.
 *
 * In accordance with the above, I want to credit Steve Wilhite who wrote
 * the code which this is heavily inspired by...
 *
 * GIF and 'Graphics Interchange Format' are trademarks (tm) of
 * Compuserve, Incorporated, an H&R Block Company.
 *
 * Release Notes: This file contains a decoder routine for GIF images
 * which is similar, structurally, to the original routine by Steve Wilhite.
 * It is, however, somewhat noticably faster in most cases.
 *
 == This routine was modified for use in FRACTINT in two ways.
 ==
 == 1) The original #includes were folded into the routine strictly to hold
 ==    down the number of files we were dealing with.
 ==
 == 2) The 'stack', 'suffix', 'prefix', and 'buf' arrays were changed from
 ==    static and 'malloc()'ed to external only so that the assembler
 ==    program could use the same array space for several independent
 ==    chunks of code.  Also, 'stack' was renamed to 'dstack' for TASM
 ==    compatibility.
 ==
 == 3) The 'out_line()' external function has been changed to reference
 ==    '*outln()' for flexibility (in particular, 3D transformations)
 ==
 == 4) A call to 'keypressed()' has been added after the 'outln()' calls
 ==    to check for the presenc of a key-press as a bail-out signal
 ==
 == (Bert Tyler and Timothy Wegner)
 */




/* short decoder(linewidth)
 *    short linewidth;          * Pixels per line of image *
 *
 * - This function decodes an LZW image, according to the method used
 * in the GIF spec.  Every *linewidth* "characters" (ie. pixels) decoded
 * will generate a call to out_line(), which is a user specific function
 * to display a line of pixels.  The function gets its codes from
 * get_next_code() which is responsible for reading blocks of data and
 * seperating them into the proper size codes.  Finally, get_byte() is
 * the global routine to read the next byte from the GIF file.
 *
 * It is generally a good idea to have linewidth correspond to the actual
 * width of a line (as specified in the Image header) to make your own
 * code a bit simpler, but it isn't absolutely necessary.
 *
 * Returns: 0 if successful, else negative.  (See ERRS.H)
 *
 */



CGif::CGif()
{
    pGifDataBuffer = NULL;
    GIFDataSize = 0;
    hFile = NULL;
    skipxdots = 0;
    skipydots = 0;
}


// Wrapper to help keep it simple and more readable
BYTE * CGif::LoadFromFile( char *name )
{
    return DecodeGif( name );
}

// Wrapper to help keep it simple and more readable
BYTE * CGif::LoadFromResource( char *name, char *type, HINSTANCE hInst )
{
    return DecodeGif( name, type, hInst );
}

// Wrapper to help keep it simple and more readable
BYTE * CGif::LoadFromBuffer( BYTE *CmpBuffer, int Size )
{
    size = Size;

    return DecodeGif( NULL, NULL, NULL, CmpBuffer );
}


// Multi-purpose reads from file or resource
// I forced the read from resource into the program. Normaly I
// would write two seperate functions but there is too many global
// variables use in this decode GIF class. I use a wrapper around this
// function to help keep it simple and readable
BYTE * CGif::DecodeGif( char *name, char *type, HINSTANCE hInst, BYTE *CmpBuffer )
{
    int            numcolors;
    int            ImageBitCount;
    unsigned char       buffer[16];
    int            width, finished, NewWidth;
    LPBITMAPINFOHEADER  lpbi;
    RGBQUAD            *pRgb;
    int                 status = 0;
    int                 i, planes;
    //OFSTRUCT            of;
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long       bytesRead;
    TLoadRes            LoadRes;
    BITMAPFILEHEADER *BMPfHead;
    BYTE             *dib = NULL;

    
    bad_code_count = 0;
    gbuf = 0;
    gbn = 0;
    height = 0;
    xdots = 0;
    ydots = 0;

    // init Structure
    code_mask[0] = 0;
    code_mask[1] = 0x0001;
    code_mask[2] = 0x0003;
    code_mask[3] = 0x0007;
    code_mask[4] = 0x000F;
    code_mask[5] = 0x001F;
    code_mask[6] = 0x003F;
    code_mask[7] = 0x007F;
    code_mask[8] = 0x00FF;
    code_mask[9] = 0x01FF;
    code_mask[10] = 0x03FF;
    code_mask[11] = 0x07FF;
    code_mask[12] = 0x0FFF;

    memset( dstack, 0, sizeof(unsigned char)*(MAX_CODES + 1) );
    memset( suffix, 0, sizeof(unsigned char)*(MAX_CODES + 1) );
    memset( prefix, 0, sizeof(unsigned short)*(MAX_CODES + 1) );

    navail_bytes = 0;
    nbits_left = 0;
    curr_size = 0;
    clear = 0;
    ending = 0;
    newcodes = 0;
    top_slot = 0;
    slot = 0;
    b1 = 0;
    colors = 0;
    win_xdots = 0;
    win_ydots = 0;
    pixels_per_byte = 0;
    pixels_per_bytem1 = 0;
    pixelshift_per_byte = 0;
    bytes_per_pixelline = 0;
    win_bitmapsize = 0;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    /* initialize the row count for write-lines */
    rowcount = 0;

    /* zero out the full write-line */
    memset( decoderline, 0, sizeof(BYTE)*2049 );

    // Set the flag that tells us if we are decoding 
    // from a file, resource or a buffer 
    if( name && !type && !hInst && !CmpBuffer )
        fFile_Resource = GIF_FILE_READ;
    else if( name && type && hInst && !CmpBuffer )
        fFile_Resource = GIF_RESOURCE_READ;
    else if( !name && !type && !hInst && CmpBuffer )
        fFile_Resource = GIF_BUFFER_READ;

    // If type is null, then we are loading from a file
    if( fFile_Resource == GIF_FILE_READ )
    {
        // Open the file on the hard drive
        hFile = CreateFile( name, GENERIC_READ, FILE_SHARE_READ,
                             &FileSecurity, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

        if( hFile == INVALID_HANDLE_VALUE )
        {
           MessageBox( 0, "Can't open bitmap file to display", "Error", MB_ICONWARNING );
           goto HANDLE_ERROR;
        }

        size = GetFileSize( hFile, NULL );
    }
    else if( fFile_Resource == GIF_RESOURCE_READ ) // We're loading from resource
    {
        // Load the file from the resource
        if( !LoadTheResource( hInst, name, type, &LoadRes ) )
            goto HANDLE_ERROR;

        size = LoadRes.Size;
        rData = (BYTE *)LoadRes.pData;

    }
    else if( fFile_Resource == GIF_BUFFER_READ ) // We're loading from a buffer
    {
        pGifDataBuffer = CmpBuffer;
    }

    // Save the buffer size
    GIFDataSize = size;

   /* Open the file -- changed to OpenFile by mh */
   /*hFile = OpenFile ((LPSTR)filename, (LPOFSTRUCT)&of, OF_READ);
    if (hFile == -1)
    {
        return FALSE;
    }*/

    // Do we need to alocate a buffer?
    if( fFile_Resource != GIF_BUFFER_READ )
        pGifDataBuffer = new BYTE[size+100];

    if( pGifDataBuffer == NULL )
    {
        goto HANDLE_ERROR;
    }

    if( fFile_Resource == GIF_FILE_READ )
    {
        ReadFile( hFile, pGifDataBuffer, size, &bytesRead, NULL );
        gbn = bytesRead;
    }
    else if( fFile_Resource == GIF_RESOURCE_READ )
    {
        memcpy( pGifDataBuffer, rData, size );
    }


    gbn = 0;
#if SEG_CBUF
    gbuf = 0;
#else
    gbuf = NULL;
#endif

   /* Get the screen description */
    for (i = 0 ; i < 13 ; ++i )
    {
      status = get_byte();
        buffer[i] = (unsigned char) status;
        if (status < 0)
        {
            goto HANDLE_ERROR;
        }
    }

    if (strncmp((char *)buffer, "GIF87a", 3) ||     /* use updated GIF specs */
         buffer[3] < '0' || buffer[3] > '9' || 
         buffer[4] < '0' || buffer[4] > '9' || 
         buffer[5] < 'A' || buffer[5] > 'z')
    {
        goto HANDLE_ERROR;
    }

    planes = (buffer[10] & 0xF) + 1;

    if ((buffer[10] & 0x80) == 0)   /* color map (better be!) */
    {
        goto HANDLE_ERROR;
    }

    // init the number of colors
    numcolors = 1 << planes;

    // init class member used in other functions
    colors = numcolors;

#if GET_BYTES
   // Was dacbox but it doesn't want to compile so I stuck in buffer for now
    if (numcolors * 3 == (int)get_bytes(numcolors * 3, dacbox)) // dacbox
    {
#if PAL_SHIFT
        for (i = 0 ; i < numcolors ; ++i)
        {
            for (int j = 0 ; j < 3 ; ++j)
            {
                dacbox[i][j] >>= 2;
            }
        }
#endif
    }
    else
    {
        goto HANDLE_ERROR;
    }
#else
    for (i = 0 ;  i < numcolors ;  ++i)
    {
        for (int j = 0 ;  j < 3 ;  ++j)
        {
            status = get_byte();
            if (status < 0)                
                goto HANDLE_ERROR;

            //dacbox[i][j] = (status & 0x00ff) >> 2;
         *(dacbox + ((i*j)+j)) = (status & 0x00ff) >> 2;
        }
    }
#endif

   /* Now display one or more GIF objects */
    finished = 0;
    while (!finished)
    {
        switch (get_byte())
        {
            case ';':               /* End of the GIF dataset */
                finished = 1;
                status = 0;
                break;

            case '!':               /* GIF Extension Block */
                get_byte();     /* read (and ignore) the ID */
                while ((i = get_byte()) > 0)    /* get the data length */
#if GET_BYTES
                    get_bytes(i, NULL);
#else
                    for ( int j = 0;  j < i;  ++j)
                        get_byte(); /* flush the data */
#endif
                break;

            case ',':
                /*
                 * Start of an image object. Read the image description.
                 */

#if GET_BYTES
                if (9 != get_bytes(9, buffer))
                {
                    status = -1;
                }
                else
                {
                    status = 0;
                }
#else
                for (i = 0;  i < 9;  ++i)
                {
                    buffer[i] = (unsigned char) status = get_byte();
                    if (status < 0)
                    {
                        status = -1;
                        break;
                    }
                }
#endif
                if (status < 0)
                {
                    finished = 1;
                    break;
                }

                width = buffer[4] | (buffer[5] << 8);
                height = buffer[6] | (buffer[7] << 8);

                // fill in DIB stuff
                xdots = width;
                ydots = height;

                if (numcolors > 16)
                    ImageBitCount = 8;
                else if (numcolors > 2 && numcolors <= 16)
                    ImageBitCount = 4;
                else
                    ImageBitCount = 1;

                win_xdots = (xdots + 3) & 0xFFFC;
                win_ydots = ydots;
                pixelshift_per_byte = 0;
                pixels_per_byte = 1;
                pixels_per_bytem1 = 0;

                if( ImageBitCount == 4 )
                {
                    win_xdots = (xdots + 7) & 0xFFF8;
                    pixelshift_per_byte = 1;
                    pixels_per_byte = 2;
                    pixels_per_bytem1 = 1;
                    win_andmask[0] = 0xF0;
                    win_notmask[0] = 0xF;
                    win_bitshift[0] = 4;
                    win_andmask[1] = 0xF;
                    win_notmask[1] = 0xF0;
                    win_bitshift[1] = 0;
                }
                if( ImageBitCount == 1 )
                {
                    win_xdots = (xdots + 31) & 0xFFE0;
                    pixelshift_per_byte = 3;
                    pixels_per_byte = 8;
                    pixels_per_bytem1 = 7;
                    win_andmask[0] = 0x80;
                    win_notmask[0] = 0x7F;
                    win_bitshift[0] = 7;
                    for (i = 1 ;  i < 8 ;  ++i)
                    {
                        win_andmask[i] = win_andmask[i - 1] >> 1;
                        win_notmask[i] = (win_notmask[i - 1] >> 1) + 0x80;
                        win_bitshift[i] = win_bitshift[i - 1] - 1;
                    }
                }

                bytes_per_pixelline = win_xdots >> pixelshift_per_byte;

                if( ImageBitCount == 8 )
                {
                    // Make sure the size is DWORD alligned. Windows likes it that way.
                    if( width % sizeof(DWORD) )
                        NewWidth = width + sizeof(DWORD) - (width % sizeof(DWORD));
                    else
                        NewWidth = width;
                }
                else
                {
                    NewWidth = width / pixels_per_byte;
                    // Make sure the size is DWORD alligned. Windows likes it that way.
                    if( NewWidth % sizeof(DWORD) )
                        NewWidth += sizeof(DWORD) - (NewWidth % sizeof(DWORD));
                }

                // Get the complete size of the bitmap
                BitmapSizeInBytes = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
                                   (numcolors * sizeof(RGBQUAD)) + (NewWidth  * height);

                // Allocate memory
                // Allocate a little extra just to be safe
                dib = new BYTE [BitmapSizeInBytes+50];
                if( !dib )
                    goto HANDLE_ERROR;

                BMPfHead = (BITMAPFILEHEADER *) dib;
                BMPfHead->bfType = BITMAP_TYPE;

                BMPfHead->bfSize = BitmapSizeInBytes;
                BMPfHead->bfReserved1 = 0;
                BMPfHead->bfReserved2 = 0;
                BMPfHead->bfOffBits = BitmapSizeInBytes - (NewWidth * height);

                lpbi = (BITMAPINFOHEADER *)(dib + sizeof(BITMAPFILEHEADER));
                lpbi->biSize = sizeof(BITMAPINFOHEADER);
                lpbi->biWidth = width;
                lpbi->biHeight = height;
                lpbi->biPlanes = 1; //nb: NOT equal to planes from GIF
                //lpbi->biBitCount = planes; // pixels_per_byte;
                // Always tell it to be an 8 bit bitmap. Some API graphic calls don't
                // like images that can be less then 8 bit
                lpbi->biBitCount = ImageBitCount;

                lpbi->biCompression = BI_RGB;
                lpbi->biSizeImage = 0;//(DWORD) bytes_per_pixelline * win_ydots;
                // Default to 2834 because the GIF compression doesn't save this information
                // and it translates to 72 pixels per inch
                lpbi->biXPelsPerMeter = DEFAULT_METERS_PER_INCH;
                lpbi->biYPelsPerMeter = DEFAULT_METERS_PER_INCH;
                lpbi->biClrUsed = numcolors;
                lpbi->biClrImportant = numcolors;

                win_bitmapsize = (((long) win_xdots * (long) win_ydots) >> pixelshift_per_byte) + 1;

                    /* fill in intensities for all palette entry colors */

                //pRgb = (RGBQUAD FAR *) ((LPSTR) lpbi + (unsigned int)lpbi->biSize);
                pRgb = (RGBQUAD*)( dib + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) );
                for (i = 0;  i < numcolors;  ++i)
                {
#if PAL_SHIFT
                    pRgb[i].rgbRed = ((BYTE) dacbox[i][0]) << 2;
                    pRgb[i].rgbGreen = ((BYTE) dacbox[i][1]) << 2;
                    pRgb[i].rgbBlue = ((BYTE) dacbox[i][2]) << 2;
#else
                    //pRgb[i].rgbRed = ((BYTE) dacbox[i][0]);
                    //pRgb[i].rgbGreen = ((BYTE) dacbox[i][1]);
                    //pRgb[i].rgbBlue = ((BYTE) dacbox[i][2]);
                    pRgb[i].rgbRed = ((BYTE) *(dacbox + (i*3))); //dacbox[i][0]);
                    pRgb[i].rgbGreen = ((BYTE) *(dacbox + ((i*3)+1))); //dacbox[i][1]);
                    pRgb[i].rgbBlue = ((BYTE) *(dacbox + ((i*3)+2)));//dacbox[i][2]);
#endif
                }

                // Set the pointer to the starting position for the bits
                //pixels = (dib + BitmapSizeInBytes - (NewWidth * height));
                pixels = (dib + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
                                   (numcolors * sizeof(RGBQUAD)));

                //this does the grunt work
                decoder(width);

                // test
                //WriteDataToFile( "C:\\temp\\test.bmp", dib, BitmapSizeInBytes, CREATE_ALWAYS );

                status = 0;
                finished = 1;
                break;

            default:
                status = -1;
                finished = 1;
                break;
        }
    }

HANDLE_ERROR:;

    free();

    return dib;

}   // DecodeGif


#if GET_BYTES
unsigned int CGif::get_bytes(unsigned int count, BYTE *dest)
{
   unsigned int count2 = count;

    if (count <= 0)
    {
        return count;
    }

    while (count)
    {
        if (gbn == 0)
        {
            //gbn = _lread(hFile,pBuffer,CBUFSIZE);
            if( size > 0 )
            {
               gbn = size;
               size = 0;
            }
            else
            {
               gbn = 0;
            }
           
#if SEG_CBUF
            gbuf = NULL;
#else
            gbuf = pGifDataBuffer;
#endif

            if (!gbn)
                return -1;
        }

        if (gbn >= count)
        {
            if (dest != NULL)
            {
#if SEG_CBUF
                memcpy(dest, pGifDataBuffer + gbuf, count);
                //CopyMemory( dest, pBuffer + gbuf, count );
#else
                memcpy(dest, gbuf, count);
                //CopyMemory( dest, gbuf, count );
#endif
            }
            gbuf += count;
            gbn -= count;
            count = 0;
        }
        else
        {
            if (dest != NULL)
            {
#if SEG_CBUF
                memcpy(dest, pGifDataBuffer + gbuf, gbn);
                //CopyMemory( dest, pBuffer + gbuf, gbn );
#else
                memcpy(dest, gbuf, gbn);
                //CopyMemory( dest, gbuf, gbn );
#endif
            }
            count -= gbn;
            dest += gbn;
            gbn = 0;
        }
    }

    return count2;
}
#endif


int CGif::get_byte()
{
   //unsigned long bytesRead;

    if (gbn == 0)
    {
        //gbn = _lread(hFile,pBuffer,CBUFSIZE);
        //ReadFile( hFile, pBuffer, CBUFSIZE, &bytesRead, NULL );
        //gbn = bytesRead;

        if( size > 0 )
        {
           gbn = size;
           size = 0;
        }
        else
        {
           gbn = 0;
        }

#if SEG_CBUF
        gbuf = 0;
#else
        gbuf = NULL;
#endif
    }

    if (gbn)
    {
        --gbn;
#if SEG_CBUF
        return (pGifDataBuffer[gbuf++]) & 0x00ff;
#else
        return (*gbuf++) & 0x00ff;
#endif
    }
    else
        return (-1);
}


int CGif::decoder(int linewidth)
{
#if NEAR_STACK
    register BYTE   *sp;
    register BYTE   *bufptr;
    BYTE        *buf;
#else
    register BYTE   *sp;
    register BYTE   *bufptr;
    BYTE        *buf;
#endif
    register int  code, fc, oc, bufcnt;
    int           c, DecodeSize, ret;
    int           xskip, yskip;
#if ALLOC_STACK
    unsigned char           *dstack;          /* Stack for storing pixels */
    unsigned char           *suffix;          /* Suffix table */
    unsigned short           *prefix;          /* Prefix linked list */
#endif

   /* Initialize for decoding a new image...
    */
    if ((DecodeSize = get_byte()) < 0)
        return (DecodeSize);
    if (DecodeSize < 2 || 9 < DecodeSize)
        return (BAD_CODE_SIZE);
    init_exp(DecodeSize);
    xskip = yskip = 0;

#if ALLOC_STACK
    /*
    **  Allocate space for tables and stack
    */
    dstack = (unsigned char *)malloc((2 * sizeof(unsigned char) + sizeof(unsigned short)) * (MAX_CODES + 1));
    suffix = (unsigned char *)(((char *)dstack) + sizeof(unsigned char) * (MAX_CODES + 1));
    prefix = (unsigned short *)(((char *)suffix) + sizeof(unsigned char) * (MAX_CODES + 1));
#endif

   /* Initialize in case they forgot to put in a clear code.
    * (This shouldn't happen, but we'll try and decode it anyway...)
    */
    oc = fc = 0;

    buf = decoderline;

   /* Set up the stack pointer and decode buffer pointer
    */
    sp = dstack;
    bufptr = buf;
    bufcnt = linewidth;

   /* This is the main loop.  For each code we get we pass through the
    * linked list of prefix codes, pushing the corresponding "character" for
    * each code onto the stack.  When the list reaches a single "character"
    * we push that on the stack too, and then start unstacking each
    * character for output in the correct order.  Special handling is
    * included for the clear code, and the whole thing ends when we get
    * an ending code.
    */

    while ((c = get_next_code()) != ending)
    {

      /* If we had a file error, return without completing the decode
       */
        if (c < 0)
        {
#if ALLOC_STACK
            free(dstack);
#endif
            //return (0);
            return -1;
        }

      /* If the code is a clear code, reinitialize all necessary items.
       */
        if (c == clear)
        {
            curr_size = DecodeSize + 1;
            slot = newcodes;
            top_slot = 1 << curr_size;

     /* Continue reading codes until we get a non-clear code
      * (Another unlikely, but possible case...)
      */
            while ((c = get_next_code()) == clear)
                ;

     /* If we get an ending code immediately after a clear code
      * (Yet another unlikely case), then break out of the loop.
      */
            if (c == ending)
                break;

     /* Finally, if the code is beyond the range of already set codes,
      * (This one had better NOT happen...  I have no idea what will
      * result from this, but I doubt it will look good...) then set it
      * to color zero.
      */
            if (c >= slot)
                c = 0;

            oc = fc = c;

     /* And let us not forget to put the char into the buffer... */
            *sp++ = c;      /* let the common code outside the if else stuff it */
        }
        else 
        {

     /* In this case, it's not a clear code or an ending code, so
      * it must be a code code...  So we can now decode the code into
      * a stack of character codes. (Clear as mud, right?)
      */
            code = c;

     /* Here we go again with one of those off chances...  If, on the
      * off chance, the code we got is beyond the range of those already
      * set up (Another thing which had better NOT happen...) we trick
      * the decoder into thinking it actually got the last code read.
      * (Hmmn... I'm not sure why this works...  But it does...)
      */
            if (code >= slot)
            {
                if (code > slot)
                    ++bad_code_count;
                code = oc;
                *sp++ = fc;
            }

     /* Here we scan back along the linked list of prefixes, pushing
      * helpless characters (ie. suffixes) onto the stack as we do so.
      */
            while (code >= newcodes)
            {
                *sp++ = suffix[code];
                code = prefix[code];
            }

     /* Push the last character on the stack, and set up the new
      * prefix and suffix, and if the required slot number is greater
      * than that allowed by the current bit size, increase the bit
      * size.  (NOTE - If we are all full, we *don't* save the new
      * suffix and prefix...  I'm not certain if this is correct...
      * it might be more proper to overwrite the last code...
      */
            *sp++ = code;
            if (slot < top_slot)
            {
                suffix[slot] = fc = code;
                prefix[slot++] = oc;
                oc = c;
            }
            if (slot >= top_slot)
                if (curr_size < 12)
                {
                    top_slot <<= 1;
                    ++curr_size;
                }
        }

      /* Now that we've pushed the decoded string (in reverse order)
       * onto the stack, lets pop it off and put it into our decode
       * buffer...  And when the decode buffer is full, write another
       * line...
       */
        while (sp > dstack)
        {
            --sp;
            if (--xskip < 0)
            {
                xskip = skipxdots;
                *bufptr++ = *sp;
            }
            if (--bufcnt == 0)  /* finished an input row? */
            {
                if (--yskip < 0)
                {
//                  if ((ret = (*outln)(buf, bufptr - buf)) < 0)
//                  if ((ret = out_line(buf, bufptr - buf)) < 0)
                    if ((ret = put_line(rowcount++, 0, bufptr - buf, buf)) < 0)
                        return (ret);
                    yskip = skipydots;
                }
                bufptr = buf;
                bufcnt = linewidth;
                xskip = 0;
            }
        }
    }

#if ALLOC_STACK
    free(dstack);
#endif

   /* PB note that if last line is incomplete, we're not going to try
      to emit it;  original code did, but did so via out_line and therefore
      couldn't have worked well in all cases... */
    return (0);
}


void CGif::free()
{
    if( pGifDataBuffer )
    {
        delete [] pGifDataBuffer;
        pGifDataBuffer = NULL;
    }

    if( hFile )
    {
        CloseHandle( hFile );
        hFile = NULL;
    }

}   // free()



#if GET_BLOCK
int CGif::get_block(void)
{
    //unsigned long bytesRead;
    register int    count;
    register    BYTE    *dest = byte_buff;

    count = get_byte();
    if (count < 0)
    {
        return count;
    }
    navail_bytes = count;

    while (count)
    {
        if (gbn == 0)
        {
            //gbn = _lread(hFile,pBuffer,CBUFSIZE);
            //ReadFile( hFile, pBuffer, CBUFSIZE, &bytesRead, NULL );
            //gbn = bytesRead;

            if( size > 0 )
            {
               gbn = size;
               size = 0;
            }
            else
            {
               gbn = 0;
            }

#if SEG_CBUF
            gbuf = NULL;
#else
            gbuf = pGifDataBuffer;
#endif

            if (!gbn)
                return -1;
        }

        if (gbn >= (unsigned int)count)
        {
#if SEG_CBUF
            memcpy(dest, pGifDataBuffer + gbuf, count);
            //CopyMemory( dest, pBuffer + gbuf, count );
#else
            memcpy(dest, gbuf, count);
            //CopyMemory( dest, gbuf, count );
#endif
            gbuf += count;
            gbn -= count;
            count = 0;
        }
        else
        {
#if SEG_CBUF
            memcpy(dest, pGifDataBuffer + gbuf, gbn);
            //CopyMemory( dest, pBuffer + gbuf, gbn );
#else
            memcpy(dest, gbuf, gbn);
            //CopyMemory( dest, gbuf, gbn );
#endif
            count -= gbn;
            dest += gbn;
            gbn = 0;
        }
    }

    return navail_bytes;
}
#endif



/* get_next_code()
 * - gets the next code from the GIF file.  Returns the code, or else
 * a negative number in case of file errors...
 */

int CGif::get_next_code(void)
{
    unsigned long   ret;

    if (nbits_left == 0)
    {
        if (navail_bytes <= 0)
        {
            /*
            **  Out of bytes in current block, so read next block
            */
            pbytes = byte_buff;
#if GET_BLOCK
            if ((navail_bytes = get_block()) < 0)
                return (navail_bytes);
#else
            if ((navail_bytes = get_byte()) < 0)
                return (navail_bytes);
            else if (navail_bytes)
            {
                for (i = 0 ; i < navail_bytes ; ++i)
                {
                    if ((x = get_byte()) < 0)
                        return (x);
                    byte_buff[i] = x;
                }
            }
#endif
        }
        b1 = *pbytes++;
        nbits_left = 8;
        --navail_bytes;
    }

    ret = b1 >> (8 - nbits_left);
    while (curr_size > nbits_left)
    {
        if (navail_bytes <= 0)
        {
            /*
            **  Out of bytes in current block, so read next block
            */
            pbytes = byte_buff;
#if GET_BLOCK
            if ((navail_bytes = get_block()) < 0)
                return (navail_bytes);
#else
            if ((navail_bytes = get_byte()) < 0)
                return (navail_bytes);
            else if (navail_bytes)
            {
                for (i = 0 ; i < navail_bytes ; ++i)
                {
                    if ((x = get_byte()) < 0)
                        return (x);
                    byte_buff[i] = x;
                }
            }
#endif
        }
        b1 = *pbytes++;
        ret |= b1 << nbits_left;
        nbits_left += 8;
        --navail_bytes;
    }
    nbits_left -= curr_size;
    ret &= code_mask[curr_size];
    return ((int) (ret));
}


int CGif::init_exp( int Size )
{
    curr_size = Size + 1;
    top_slot = 1 << curr_size;
    clear = 1 << Size;
    ending = clear + 1;
    slot = newcodes = ending + 1;
    navail_bytes = nbits_left = 0;

    return (0);
}



void CGif::putcolor(int x, int y, int color)
{
    long i;

    i = win_ydots - 1 - y;
    i = (i * win_xdots) + x;

    if (x >= 0 && x < xdots && y >= 0 && y < ydots)
    {
        if (pixelshift_per_byte == 0)
        {
            pixels[i] = color % colors;
        }
        else
        {
            unsigned int j;

            j = i & (unsigned int)pixels_per_bytem1;
            i = i >> pixelshift_per_byte;
            pixels[i] = (pixels[i] & win_notmask[j]) + 
                 (((unsigned char) (color % colors)) << win_bitshift[j]);
        }
    }
}

int CGif::put_line(int rownum, int leftpt, int rightpt, unsigned char *localvalues)
{
    int i, len;
    long    startloc;

    len = rightpt - leftpt;
    if (rightpt >= xdots)
    {
        len = xdots - leftpt;
    }
    startloc = win_ydots - 1 - rownum;
    startloc = (startloc * win_xdots) + leftpt;

    if (rownum < 0 || rownum >= ydots || leftpt < 0)
    {
        return (0);
    }

    if (pixelshift_per_byte == 0)
    {
        /*
        **  Done this way because _fmemcpy() does not handle HUGE pointers
        **  that can wrap over a selector/segment boundry.
        */
        BYTE    *ptr = pixels + startloc;

        if (HIWORD(ptr) != HIWORD(ptr + len))
        {
            char    *src = (char *)localvalues;

            while (len--)
                *ptr++ = *src++;
        }
        else
        {
            //_fmemcpy(ptr, localvalues, len);
            memcpy( ptr, localvalues, len );
        }
    }
    else
    {
        unsigned int    j;
        long                k;

        for (i = 0 ;  i <= len ;  ++i)
        {
            k = startloc + i;
            j = k & (unsigned int)pixels_per_bytem1;
            k = k >> pixelshift_per_byte;
            pixels[k] = (pixels[k] & win_notmask[j]) + 
                 (((unsigned char) (localvalues[i] % colors)) << win_bitshift[j]);
        }
    }
    putcolor(leftpt, rownum, localvalues[0]);

    return (1);
}

int CGif::out_line(unsigned char *localvalues, int numberofdots)
{
    return (put_line(rowcount++, 0, numberofdots, localvalues));
}
