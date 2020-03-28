
/************************************************************************
*
*    PRODUCT:         True color 3D Engine texture library
*
*    FILE NAME:       CTextMap.h
*
*    DESCRIPTION:     Creating and loading a texture library
*
*    Copyright(c) John De Goes
*    All Rights Reserved
*
************************************************************************/

#ifndef TEXTURE_MAP_H
#define TEXTURE_MAP_H

#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf

#define MAX_TEXTURE_SIZE   256
#define BITMAP_TYPE        0x4d42

enum EBT_FLIP_FLOP
{
	EBT_KEEP_BOTTOM_TO_SAME=0,
	EBT_BOTTOM_TOP_FLIP,
};

///////////////////////////////////
// Define our own bitmap structors
// so that we don't have to use
// windows.h
///////////////////////////////////

struct BitmapInfoHeader
{ 
   unsigned int   biSize; 
   int            biWidth; 
   int            biHeight; 
   unsigned short biPlanes; 
   unsigned short biBitCount; 
   unsigned int   biCompression; 
   unsigned int   biSizeImage; 
   int            biXPelsPerMeter; 
   int            biYPelsPerMeter; 
   unsigned int   biClrUsed; 
   unsigned int   biClrImportant; 
};

struct BitmapFileHeader
{ 
    unsigned short bfType; 
    unsigned int   bfSize; 
    unsigned short bfReserved1; 
    unsigned short bfReserved2; 
    unsigned int   bfOffBits; 
};

// Our texture library header
struct BMPHeader
{
    int width;
    int height;
};
typedef BMPHeader *PBMPHeader;


////////////////////////////////////////////////
// Class for loading bitmaps be it a bitmap file
// or the bit data in the texture library.
////////////////////////////////////////////////

class CTextureMap 
{
public:
    // Pointer to bitmap data
    unsigned int *pImage32;
    unsigned short int *pImage16;
    // Size of bitmap
    int width, height;
    // Shift value depending on the width
    int multiShift;

    // Constructor
    CTextureMap();
    // Destructor
    ~CTextureMap();

    // Load the bitmap texture file. For building
    // the texture library file.
    bool LoadFromFile( char *FileName, EBT_FLIP_FLOP flipFlop );
    // Save native bitmap texture format file.
    // For building the texture library file.
    bool SaveBT_32( FILE *hFile );
    bool SaveBT_16( FILE *hFile );
    // Load native bitmap texture from the library file
    bool LoadBT_32( FILE *hFile );
    bool LoadBT_16( FILE *hFile );
    // Return pointer to bitmap bits
    unsigned int *GetTexturePtr32(){ return pImage32; }
    unsigned short int *GetTexturePtr16(){ return pImage16; }
    // Return the size of the buffer
    int GetBufferSize(){ return width * height; }
    // Free all allocations
    void Free();
    // Load native bitmap texture format file
    unsigned char *LoadFromPointer_32( unsigned char *pData );
	unsigned char *LoadFromPointer_16( unsigned char *pData );
    // Allocate the texture space
    bool AllocateTextureSpace( int w, int h );
    // Setup the shift value dependant on the texture width
    bool SetMultiShift( int aWidth );
    // Create a 16 bit texture from a 32 bit texture
    bool Create16BitTextFrom32();
};
typedef CTextureMap *PCTextureMap;

#endif  // TEXTURE_MAP_H
