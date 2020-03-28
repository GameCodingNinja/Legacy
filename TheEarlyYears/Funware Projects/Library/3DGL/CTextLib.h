
/************************************************************************
*
*    PRODUCT:         3D Engine texture library
*
*    FILE NAME:       CTextLib.h
*
*    DESCRIPTION:     Creating and loading a texture library
*
*    Copyright(c) John De Goes
*    All Rights Reserved
*
************************************************************************/

#ifndef GL_TEXT_LIB_H
#define GL_TEXT_LIB_H

#include "CTextMap.h"      // class for creating and loading a Bitmap texture library

class CTextureLib 
{
protected:
    // number of textures in this library
    int count;
    // Auto generate mipmapping
    bool autoMipmap;
    // Texture Filter value
    int textureFilter;
    // Mipmap texture value
    int mipmapFilter;

public:
    
    // Allocated array of texture classes
    CTextureMap *pTMap;

    // Constructor
    CTextureLib();
    // Destructor
    ~CTextureLib();
    // Free the textures
    void Free();

    // Load native format of bitmap textures
    bool LoadFromFile( char *fileName );
    // Save native bitmap texture format file
    bool SaveToFile( char *fileName );
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
    // Set the texture filter
    void SetTextureFilter( int value ){ textureFilter = value; };
    // Set the mipmapping filter
    void SetMipMappingFilter( int value ){ mipmapFilter = value; };
    // Turn on or off mipmapping
    void SetMipMapping( bool value ){ autoMipmap = value; };
    // Set active texture
    void SetActiveTexture( int index ); 
};

typedef CTextureLib *PCTextureLib;


#endif  // GL_TEXT_LIB_H