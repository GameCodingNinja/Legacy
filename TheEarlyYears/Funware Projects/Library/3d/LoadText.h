//
// File name: BMP32.CPP
//
// Description: A BMP image class implementation
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef LOAD_TEXTURE_H
#define LOAD_TEXTURE_H

#include <Math.H>
#include <StdIO.H>
#include <Windows.H>

struct PaletteHeader
{
	int ColorCount;
	int BitCount;
};
typedef PaletteHeader *PPaletteHeader;

struct BMPHeader
{
	int Width;
	int Height;
	int BitCount;
	int ColorCount;
};
typedef BMPHeader *PBMPHeader;

class LoadTexture 
{
protected:

public:
    RGBQUAD *Palette;
    unsigned char *Image;
    int Width, Height, ColorCount, BitCount, PreMultShift;

    // Constructor
    LoadTexture();
    // Destructor
    ~LoadTexture();
    // Load the bitmap texture file
    bool Load( char *FileName, int textureIndex );
    // Save native bitmap texture format file
    bool SaveBT( FILE *OutFile );
    // Load native bitmap texture format file
    bool LoadBT( FILE *InFile );
    // Save Palette
    bool SavePal( FILE *OutFile );
    // Load Palette
    bool LoadPal( FILE *InFile );
    // Return pointer to bitmap bits
    unsigned char *GetTexturePtr(){ return Image; }
    // Return the size of the buffer
    int GetBufferSize(){ return (Width * Height) * ( BitCount / 8 ); }
    // Free all allocations
    void Free();
    // Free the palette
    void FreePalette();
    // Free the bit data
    void FreeImage();
	// Load the palette from a pointer
	unsigned char *LoadPalfromPtr( unsigned char *pBTdata );
	// Load native bitmap texture format file
	unsigned char *LoadBTfromPtr( unsigned char *pBTdata );
	// Allocate the texture space
	bool AllocateTextureSpace( int W, int H, int bit, int textureIndex );
};
typedef LoadTexture *PLoadTexture;

#endif  // LOAD_TEXTURE_H
