
/************************************************************************
*
*    PRODUCT:         DIB Manipulation
*
*    FILE NAME:       DIB.cpp
*
*    DESCRIPTION:     DIB Object for bitmaps
*
*    IMPLEMENTATION:  Load and blits bitmaps
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __DIB_H__
#define __DIB_H__

#include "TGBase.h"        // The Graphics Base Object

#define BITMAP_TYPE 0x4d42
#define BITMAP_BYTE_ALIGNMENT 4

#define ALLOC_MEM_FOR_BITMAP  0
#define USE_RESOURCE_POINTER  1

#define CONVERT_TO_PIXELS  39.36
#define DEFAULT_METERS_PER_INCH  2834
#define BIT_16_TRANS_COLOR     0x83E0

#define BIT_COUNT_1     1
#define BIT_COUNT_4     4
#define BIT_COUNT_8     8
#define BIT_COUNT_16    16
#define BIT_COUNT_24    24
#define BIT_COUNT_32    32

#define SUPRESS_ERROR_MSG  FALSE

enum
{
    DONT_CREATE_BUFFER=0,
    DIB_SECTION_BMP,
    STANDARD_DIB,
};

enum
{
    NO_TYPE=0,
    BOTTOM_UP_BMP,
    TOP_DOWN_BMP,
};

struct TLogpalette
{
    WORD          palVersion; 
    WORD          palNumEntries; 
    PALETTEENTRY  palPalEntry[256];
        
};

typedef TLogpalette *PTLogpalette;

struct TBitmapinfo
{
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD          bmiColors[256];

};

typedef TBitmapinfo *PTBitmapinfo;


enum
{
    RENDER_COPY=0,
    RENDER_XOR,
    RENDER_AND,
    RENDER_OR,
};


/************************************************************************
*                         TDIB Object
************************************************************************/

class TDIB : public TGBase
{
protected:

    // pointer to BITMAPINFO structur
    BITMAPINFO* Fdib;
    // Pointer to NEW allocated memory
    BYTE *pBitData;
    // Handle to Global allocated memory
    HANDLE hLoad;
    // Handle to palette
    HPALETTE hPalette;
    // Type of blit
    DWORD FRasterOp;
    // Pass a DC to use for creating compatible DC's
    HDC hDeviceDC;
    // How the pointer is used when loaded from resource
    int PointerType;
    // The size of the buffer
    int BufferSize;
	// Transparent flag
	int fTrans;

    // Rect clipping functions
    BOOL ClipRects( TDIB *pBitmap, RECT &Dest, RECT &Source );
    BOOL ClipRects( RECT &Dest, RECT &Source, int dWidth, int dHeight );

public:
    // Constructor
    TDIB();
    // Destructor
    ~TDIB(){ Free(); };
    // Get Handle to a block of memory that is a DIB
    // Removed because it is no longer used. Replaced by LoadFromBuffer
    //virtual BOOL LoadFromHandle( HANDLE handle, BOOL BitmapType = DIB_SECTION_BMP, int BitCount = BIT_COUNT_8 );

    // Get a pointer to a block of memory that is a dib
    virtual BOOL LoadFromBuffer( BYTE *pBuffer, DWORD Size = 0 );
    // Load bitmap from file
    virtual BOOL LoadFromFile( char *FileName, BOOL showErrorMsg = TRUE );
    // Load bitmap from resource
    virtual BOOL LoadFromResource( HINSTANCE hInst, char *name, char *type );
    // Get the width
    virtual int width() { return ( Fdib ? Fdib->bmiHeader.biWidth : 0 ); };
    // Get the height
    virtual int height() { return ( Fdib ? Fdib->bmiHeader.biHeight : 0 ); };
    // Get the palette
    HPALETTE GetPalette();
    // Convert this image to a mask
    void ConvertToMask();
    // Is a bitmap loaded
    virtual BOOL IsEmpty();
    // Gets the RGB Red value at a given index into the palette
    BYTE GetRGBred(BYTE I) { return (BYTE)( Fdib->bmiColors[I].rgbRed ); };
    // Gets the RGB Green value at a given index into the palette
    BYTE GetRGBgreen(BYTE I) { return (BYTE)( Fdib->bmiColors[I].rgbGreen ); };
    // Gets the RGB Blue value at a given index into the palette
    BYTE GetRGBblue(BYTE I) { return (BYTE)( Fdib->bmiColors[I].rgbBlue ); };
    // Sets the RGB Red value at a given index into the palette
    void SetRGBred(BYTE Index, BYTE I) { if( Fdib )Fdib->bmiColors[Index].rgbRed = I; };
    // Sets the RGB Green value at a given index into the palette
    void SetRGBgreen(BYTE Index, BYTE I) { if( Fdib )Fdib->bmiColors[Index].rgbGreen = I; };
    // Sets the RGB Blue value at a given index into the palette
    void SetRGBblue(BYTE Index, BYTE I) { if( Fdib )Fdib->bmiColors[Index].rgbBlue = I; };
    // Get pointer to bitmap bits
    BYTE* bits() { return ( Fdib ? ((BYTE *)Fdib + Get_biSize()): 0); };
    // Get pointer to BITMAPINFO structure
    BITMAPINFO* dib() { return Fdib; };
    // Do a simple full size blit to a device. For simplicity, not speed.
    void BlitToDC( HDC hDestDC, int X = 0, int Y = 0 );
    void BlitToDC( HWND hwnd, int X = 0, int Y = 0 );
    // Do a simple full stretch blit to a device from a rect in the source. For simplicity, not speed.
    void BlitToDC( HDC hDestDC, RECT *Dest, RECT *Source );
    void BlitToDC( HWND hwnd, RECT *Dest, RECT *Source );
    // Same as the first "BlitToDC" but the rect structure allows for stretching
    void BlitToDC( HDC hDestDC, RECT *Dest );
    void BlitToDC( HWND hwnd, RECT *Dest );
    // Get the pixels per meter
    int GetPixelsPerMeterX(){ return ( Fdib ? Fdib->bmiHeader.biXPelsPerMeter : DEFAULT_METERS_PER_INCH ); };
    int GetPixelsPerMeterY(){ return ( Fdib ? Fdib->bmiHeader.biYPelsPerMeter : DEFAULT_METERS_PER_INCH ); };
    // Get the pixels per inch.
    int GetPixelsPerInchX();
    int GetPixelsPerInchY();
    // Set the copymode for all blits
    void SetCopyMode( DWORD CopyMode ){ FRasterOp = CopyMode; };
    // Get the copy mode
    DWORD GetCopyMode(){ return FRasterOp; };
    // Use a DC from the outside for all the graphic initilizations
    void InitActiveDC( HDC DC ){ hDeviceDC = DC; };
    // Returns the number of colors in the bitmap's palette
    int getDIBcolors();
    // Deletes the allocated bitmap data
    void DeleteBitmapBits();
    // Deletes the palette
    void DeletePalete();
    // Gets the pointer to the allocated memory pointer
    BYTE *GetPointerToData(){ return pBitData; };
    // Free memory and palette
    void Free();
    // Get a pointer to a memory block and then this object no longer owns it
    BYTE *TakeDIBpointer();
    // Set how the resource is to be used. The options now are to use
    // the pointer for blitting or copy data into a buffer
    void SetResourcePointerType( int Type ){ PointerType = Type; };
    // Calculates to offset to the bitmap bits
    int Get_biSize() { return Fdib->bmiHeader.biSize + (getDIBcolors() * sizeof(RGBQUAD)); };
    // Returns the bit depth of this graphic
    int GetBitCount() { return Fdib->bmiHeader.biBitCount; };
    // Copy one bitmap to another
    void operator = ( TDIB &DibToCopy );
    // Convert image to a sprite
    void ConvertToSprite();
    // Get the size of the buffer
    int GetBufferSize(){ return BufferSize; };
    // Returns the bit depth of this graphic
    DWORD GetCompressionType() { return Fdib->bmiHeader.biCompression; };
    // Get the RGBQUAD data to this bitmap
    RGBQUAD *GetRGBQUAD(){ return Fdib->bmiColors; };
    // Save bitmat to file
    void SaveToFile( char *fileName );
    // Create a DIB bitmap. Currently supports 8 bit and above.
    BOOL CreateDib( int Width, int Height, int BitCount );
    // Software blit
    void Blit( TDIB *pBitmap, RECT &Dest, RECT &Source, BOOL clipRect = FALSE );
    // Software blit to a memory buffer
    void Blit( BYTE *pBitmap, RECT &Dest, RECT &Source, int dWidth, int dHeight, int bitmapType = TOP_DOWN_BMP, BOOL clipRect = FALSE );
    // Returns the bitmap type
    int GetBitmapType();
    // Get the scan line of the bitmap
    BYTE * GetScanLine( int line );
    // Copy one bitmap to another
    BOOL CopyDIB( TDIB *DibToCopy );
    // Is the loaded bitmap a transparent bitmap?
    BOOL IsBitmapTransparent();   
};

typedef TDIB *PTDIB;


#endif  /* __DIB_H__ */

