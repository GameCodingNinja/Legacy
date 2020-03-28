
/************************************************************************
*
*    PRODUCT:         True color 3D Engine texture library
*
*    FILE NAME:       CTextLib.h
*
*    DESCRIPTION:     Creating and loading a texture library
*
*    Copyright(c) John De Goes
*    All Rights Reserved
*
************************************************************************/

#ifndef TEXT_LIB_H
#define TEXT_LIB_H

#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include "CTextMap.h"        // class for creating and loading a Bitmap texture library

class CTextureLib 
{
protected:
    // number of textures in this library
    int count;
	// Library bit size
	int bitSize;

public:
    // Allocated array of texture classes
    CTextureMap *pTMap;

    // Constructor
    CTextureLib();
    // Destructor
    ~CTextureLib();

    // Load native format of bitmap textures
    bool LoadFromFile( char *fileName );
    // Save native bitmap texture format file
    bool SaveToFile_32( char *fileName );
	bool SaveToFile_16( char *fileName );
    // Free the textures
    void Free();
    // Get the number of textures loaded with this group of polygons
    int GetTextureCount(){ return count; };
    // Add a texture file to the library
    bool AddTextureToLib( int index, char *fileName, EBT_FLIP_FLOP flipFlop );
    // Get the count
    int Count(){ return count; };
    // Allocate the texture library
    bool AllocateTextLibrary( int aCount );
    // Load native format of bitmap textures from a pointer
    bool LoadFromPointer( unsigned char *pBTdata );
    // Convert the 32 bit library to a 16 bit library
    bool Create16BitLibFrom32();
    // Get the bit size
    int GetBitSize(){ return bitSize; }; 
};

typedef CTextureLib *PCTextureLib;


#endif  // TEXT_LIB_H