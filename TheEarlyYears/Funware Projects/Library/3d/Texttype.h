//
// File name: TextType.h
//
// Description:
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef TEXTTYPEHPP
#define TEXTTYPEHPP

#include <StdIO.H>

#include "LoadText.h"

class ImageData 
{
public:
    LoadTexture *TMap;
    WORD TCount;

    // Constructor
    ImageData();
    // Destructor
    ~ImageData();
    // Load the text file of BMP names to convert to native format
    BOOL LoadINI( char *FileName );
    // Load native format of bitmap textures
    BOOL LoadBT( FILE *InFile );
    // Save native bitmap texture format file
    BOOL SaveBT( FILE *OutFile );
    // Loads native bitmap texture format file
    BOOL ReadTexture( char *FileName );
    // Save native bitmap texture format file
    BOOL WriteTexture( char *FileName );
    // Free the textures
    void Free();
    // Load the palette
    BOOL LoadPal( FILE *InFile );
    // Save the palette
    BOOL SavePal( FILE *OutFile );
    // Get the number of textures loaded with this group of polygons
    int GetTextureCount(){ return TCount; };
    // Build native texture library file
    BOOL BuildTextLibrary( int Index, char *FileName );
    // Get the count
    WORD Count(){ return TCount; };
    // Allocate the texture library
    BOOL AllocateTextLibrary( int Count );
    // Load native format of bitmap textures from a pointer
    BOOL LoadFromPointer( BYTE *pBTdata );
    // Get the bitcount of the library. All images  
	// have to be the same bitdepth in a library
    int GetBitCount(){ return (TCount ? TMap[0].BitCount : 0); }; 
};

typedef ImageData *PImageData;


#endif