//
// File name: PalShade.h
//
// Description: The header file for a palette shading class
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef PALSHADEHPP
#define PALSHADEHPP

#include <StdIO.H>
#include <Windows.H>

#define SHADE_COUNT  32
#define COLOR_COUNT_8_BIT  256
#define COLOR_COUNT_16_BIT  0x8000 // 32768
#define RGB_COUNT           3

// Make a structure that is 16 bits in size and cut
// it up into 3 5 bit chunks for 16 bit RGB color.
struct SWordToByte
{
    BYTE B : 5;
    BYTE G : 5;
    BYTE R : 5;
    BYTE NotUsed :1;
};
typedef SWordToByte *PSWordToByte;

void Generate16BitShadedTable( unsigned short int *shadeTbl );

class ShadeDat 
{
protected:
	int shadeCount;

    // Shaded table byte array for 256 color palettes 8 bit
    BYTE Shade8to8[ SHADE_COUNT * COLOR_COUNT_8_BIT ];
    // Shaded table word array for 8 bit images for a 16 bit color  table
    WORD Shade8to16[ SHADE_COUNT * COLOR_COUNT_8_BIT ];
    // Shaded table word array for 16 bit images for a 16 bit color table
    WORD Shade16to16[ SHADE_COUNT * COLOR_COUNT_16_BIT ];

public:
    // Constructor
    ShadeDat();
    // Destructor
    ~ShadeDat();

    // Generate a shaded table
    void GenTable( double R2, double G2, double B2, RGBQUAD *Pal );
    // Load an 8 bit shaded table from file
    BOOL LoadTable8to8bit( char *FileName );
    // Load a 16 bit shaded table from file
    BOOL LoadTable8to16bit( char *FileName );
    // Load a 16 bit RGB shaded table from file
    BOOL LoadTable16to16bit( char *FileName );
    // Save an 8 bit shaded table to file
    BOOL SaveTable8to8bit( char *FileName );
    // Save a 16 bit shaded table to file
    BOOL SaveTable8to16bit( char *FileName );
    // Save a 16 bit RGB shaded table to file
    BOOL SaveTable16to16bit( char *FileName );
    // Get the index of a color in a palette
    int GetColor( int Red, int Green, int Blue, RGBQUAD *Pal );
    // Load shaded table from pointer
    void LoadFromPointer8To8( BYTE *pData );
    // Load shaded table from pointer
    void LoadFromPointer8To16( WORD *pData );
    // Get pointer to 8 to 8 bit table
    BYTE *Get8to8BitShadedTable(){ return Shade8to8; };
    // Get pointer to 8 to 16 bit table
    WORD *Get8to16BitShadedTable(){ return Shade8to16; };
    // Get pointer to 16 to 16 bit table
    WORD *Get16to16BitShadedTable(){ return Shade16to16; };
    // Set the number of shades to use
	void SetShadeCount( int value ){ shadeCount = value; }; 
};

typedef ShadeDat *PShadeDat;

#endif
