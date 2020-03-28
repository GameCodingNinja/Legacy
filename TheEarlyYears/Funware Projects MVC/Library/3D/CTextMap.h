
/************************************************************************
*
*    FILE NAME:       CTextMap.h
*
*    DESCRIPTION:     Creating and loading a texture from the library. Loads
*                     native uncompressed Windows 24 BMP and 24/32 bit
*                     TGA files. For compressed BMP, gif and JPEG files,
*                     the Windows IPicture is used.
*
************************************************************************/

#ifndef TEXTURE_MAP_H
#define TEXTURE_MAP_H

#include "types.h"
#include <stdio.h>      // Defines types and macros needed for the standard I/O. Needed for vprintf

#ifdef _USE_DIRECTX_9_
    #include "d3dx9.h"
#endif
#ifdef _USE_DIRECTX_8_
    #include "DX8/d3dx8.h"
#endif

#define MAX_TEXTURE_SIZE   512
#define MIN_TEXTURE_SIZE   4
#define BITMAP_TYPE        0x4d42

enum EBT_FLIP_FLOP
{
    EBT_KEEP_BOTTOM_TO_SAME=0,
    EBT_BOTTOM_TOP_FLIP,
};


/////////////////////////////////////
// Define our own bitmap structors
// so that we don't have to use
// windows.h
// Both file headers take up 54 bytes
/////////////////////////////////////

#pragma pack(1)
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


// TGA file header - 18 bytes
struct STGA_HEADER
{
    unsigned char identsize;        // size of ID field that follows 18 byte header (0 usually)
    unsigned char  colourmaptype;   // type of colour map 0=none, 1=has palette
    unsigned char  imagetype;       // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    short int colourmapstart;       // first colour map entry in palette
    short int colourmaplength;      // number of colours in palette
    unsigned char  colourmapbits;   // number of bits per palette entry 15,16,24,32

    short int xstart;               // image x origin
    short int ystart;               // image y origin
    short int width;                // image width in pixels
    short int height;               // image height in pixels
    unsigned char bits;             // image bits per pixel 8,16,24,32
    unsigned char descriptor;       // image descriptor bits (vh flip bits)
    
    // pixel data follows header    
};

// Map texture header
struct SMap_Header
{
    // Size of bitmap
    int width;
    int height;
    // Only set if BMP or TGA are loaded
    int bitCount;
    // Only set if GIF or JPEG are loaded
    unsigned int iPictureSizeInBytes;

    SMap_Header(){ Reset(); }

    void Reset()
    {
        width = 0;
        height = 0;
        bitCount = 0;
        iPictureSizeInBytes = 0;
    }
};
#pragma pack()

///////////////////////////////////////////////
//            Global functions
///////////////////////////////////////////////

void PostErrorMsg( char *Title, char *fmt, ... );


////////////////////////////////////////////////
// Class for loading bitmaps be it a bitmap file
// or the bit data in the texture library.
////////////////////////////////////////////////

class CTextureMap 
{
protected:
    // Map file header
    SMap_Header mapHeader;
    // Pointer to bits
    unsigned char *pBitData;
    // Shift value depending on the width
    int multiShift;
    // Pointer to directX texture
	#ifdef _USE_DIRECTX_9_
		LPDIRECT3DTEXTURE9 pDXTexture;
	#endif
	#ifdef _USE_DIRECTX_8_
		LPDIRECT3DTEXTURE8 pDXTexture;
	#endif
    // Generated texture id
    unsigned int openGLtextID;
	// does this texture requre alpha blending
	bool alphaBlend;

    // Takes the file path and returns the file type
    int GetFileType( char *fileName );
    // load BMP, GIF or JPG bitmap
    bool LoadFromFile_BMP( char *fileName, EBT_FLIP_FLOP flipFlop );
    // load TGA file
    bool LoadFromFile_TGA( char *fileName, EBT_FLIP_FLOP flipFlop );
    // Use I picture to load GIF and JPEG
    bool LoadFromFile_IPicture( char *fileName, EBT_FLIP_FLOP flipFlop );
    // Load IPicture data and convert
    bool LoadIPictureData( unsigned char *pData, bool deletePtr = false );
    // Load the texture into video memory
    void LoadTextIntoVideoMemory_OpenGL( bool autoMipMap, 
                                         int textFilter, int mipmapFilter );
    bool LoadTextIntoVideoMemory_DirectX( bool autoMipMap );
    // Is the width and height in range?
    bool CheckRange( int w, int h, char *fileName );
    // We need the data in true RGB order.
    void SwapRedandBlue();

public:

    // Constructor
    CTextureMap();
    // Destructor
    ~CTextureMap();

    // Free textures loaded into the video card.
    void Free();
    // Load 24-bit bitmap texture file.
    bool LoadFromFile( char *fileName, EBT_FLIP_FLOP flipFlop );
    // Save the data to the library
    bool SaveToLibraryFile( FILE *hFile );
    // Load native bitmap texture from the library file
    bool LoadFromLibraryFile_OpenGL( FILE *hFile, bool autoMipMap, int textFilter, int mipmapFilter );
    bool LoadFromLibraryFile_Software( FILE *hFile );
    bool LoadFromLibraryFile_DirectX( FILE *hFile, bool autoMipMap );
    // Load native bitmap texture library format from a pointer
    unsigned char *LoadFromPointer( unsigned char *pData, bool autoMipMap, 
                                    int textFilter, int mipmapFilter );
    // Flip the bitmap
    bool FlipBitmap( unsigned char *pBits, int size );

    int GetWidth(){ return mapHeader.width; };
    int GetHeight(){ return mapHeader.height; };
    int GetBitCount(){ return mapHeader.bitCount; };
    unsigned char *GetBits(){ return pBitData; };
    int GetMultiShift(){ return multiShift; };
    int GetOpenGLTxtID(){ return openGLtextID; };
    bool AlphaBlend(){ return alphaBlend; };    
	#ifdef _USE_DIRECTX_9_
		LPDIRECT3DTEXTURE9 GetDirectXTextPointer(){ return pDXTexture; };
	#endif
	#ifdef _USE_DIRECTX_8_
		LPDIRECT3DTEXTURE8 GetDirectXTextPointer(){ return pDXTexture; };
	#endif

};
typedef CTextureMap *PCTextureMap;

#endif  // TEXTURE_MAP_H
