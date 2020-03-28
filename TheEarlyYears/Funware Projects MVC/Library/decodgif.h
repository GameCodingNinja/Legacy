/************************************************************************
*
*    PRODUCT:         GIF Decoder
*
*    FILE NAME:       DecodGif.h
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

#ifndef __DECODEGIF_H__
#define __DECODEGIF_H__

#define SEG_CBUF    01
#define FASTCALLS   01
#define GET_BYTES   01
#define PAL_SHIFT   0
#define GET_BLOCK   01
#define NEAR_STACK  01
#define ALLOC_STACK 0
#define MAX_CODES   4095

enum
{
    GIF_FILE_READ=0,
    GIF_RESOURCE_READ,
    GIF_BUFFER_READ,
};

/* Various error codes used by decoder
 * and my own routines...   It's okay
 * for you to define whatever you want,
 * as long as it's negative...  It will be
 * returned intact up the various subroutine
 * levels...
 */
#define OUT_OF_MEMORY -10
#define BAD_CODE_SIZE -20
#define READ_ERROR -1
#define WRITE_ERROR -2
#define OPEN_ERROR -3
#define CREATE_ERROR -4

#define CBUFSIZE    (32u * 1024u)       // (16u * 1024u)

#define DEFAULT_METERS_PER_INCH  2834

//typedef unsigned short DCWORD;
//typedef unsigned char UTINY;
//typedef unsigned long ULONG;
//typedef int INT;


class CGif
{
protected:

    int decoder(int linewidth);
    unsigned int get_bytes( unsigned int count, BYTE *dest );
    int get_byte();
    void free();
    int get_block(void);
    int get_next_code(void);
    int init_exp(int size);
    void putcolor(int x, int y, int color);
    int put_line(int rownum, int leftpt, int rightpt, unsigned char *localvalues);
    int out_line(unsigned char *localvalues, int numberofdots);
    BYTE * DecodeGif( char *name, char *type = NULL, HINSTANCE hInst = 0, BYTE *CmpBuffer = NULL );

    int    rowcount;        /* row counter for screen */
    int    height;
    BYTE   dacbox[256*3];

    BYTE   decoderline[2049];       /* write-line routines use this */
    BYTE   win_andmask[8];
    BYTE   win_notmask[8];
    BYTE   win_bitshift[8];

    int    xdots, ydots, colors;
    int    win_xdots, win_ydots;
    BYTE   *pixels;                     /* the device-independent bitmap pixels */
    int    pixels_per_byte;        /* pixels/byte in the pixmap */
    long   pixels_per_bytem1;       /* pixels / byte - 1 (for ANDing) */
    int    pixelshift_per_byte;    /* 0, 1, 2, or 3 */
    int    bytes_per_pixelline;    /* pixels/line / pixels/byte */
    long   win_bitmapsize;          /* bitmap size, in bytes */
    int    bad_code_count;      /* needed by decoder module */
    BYTE   *pGifDataBuffer;
    HANDLE hFile;
    int    gbuf;

    //HANDLE hcbuf;
    unsigned int    gbn;

    /* whups, here are more globals, added by PB: */
    int skipxdots; /* 0 to get every dot, 1 for every 2nd, 2 every 3rd, ... */
    int skipydots; /* ditto for rows */

    /* was Static variables */
    int curr_size;              /* The current code size */
    int clear;              /* Value for a clear code */
    int ending;             /* Value for a ending code */
    int newcodes;           /* First available code */
    int top_slot;           /* Highest code for current size */
    int slot;           /* Last read code */

    /* The following static variables are used
     * for seperating out codes
     */
    int navail_bytes;       /* # bytes left in block */
    int nbits_left;         /* # bits left in current byte */
    unsigned char b1;             /* Current byte */
    unsigned char byte_buff[257];         /* Current block, reuse shared mem */
    unsigned char *pbytes;            /* Pointer to next byte in block */

    LONG code_mask[13];

    unsigned char dstack[MAX_CODES + 1];          /* Stack for storing pixels */
    unsigned char suffix[MAX_CODES + 1];          /* Suffix table */
    unsigned short prefix[MAX_CODES + 1];          /* Prefix linked list */

    BYTE  *rData;
    int   size, GIFDataSize, BitmapSizeInBytes;
    BOOL  fFile_Resource;

public:
    CGif();
    ~CGif(){};
    BYTE * LoadFromFile( char *name );
    BYTE * LoadFromResource( char *name, char *type = NULL, HINSTANCE hInst = 0 );
    BYTE * LoadFromBuffer( BYTE *CmpBuffer, int Size );
    int GetGifDataSize(){ return GIFDataSize; };
    int GetBMPsize(){ return BitmapSizeInBytes; };    
};


typedef CGif *PCGif;


#endif  // __DECODEGIF_H__