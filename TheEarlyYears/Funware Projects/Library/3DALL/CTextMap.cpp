
/************************************************************************
*
*    FILE NAME:       CTextMap.cpp
*
*    DESCRIPTION:     Creating and loading a texture from the library. Loads
*                     native uncompressed Windows 24 bit BMP and 24/32 bit
*                     TGA files. For compressed BMP, gif and JPEG files,
*                     the Windows IPicture is used.
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.

#include "gl/gl.h"           // standard OpenGL include
#include "gl/glu.h"          // OpenGL utilities

#include "CTextMap.h"        // This objects header file
#include "OLE_Pict.h"        // Windows OLE IPicture; Used for loading GIF and JPEG
#include "types.h"

#ifdef _USE_DIRECTX_9_
    extern LPDIRECT3DDEVICE9 g_pDXDevice;
#endif
#ifdef _USE_DIRECTX_8_
    extern LPDIRECT3DDEVICE8 g_pDXDevice;
#endif


/************************************************************************
*    DESCRIPTION:  Constructer                                                             
*
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
CTextureMap::CTextureMap()
{
    openGLtextID = 0;
    pBitData = NULL;
    alphaBlend = false;

    #if defined(_USE_DIRECTX_8_) || defined(_USE_DIRECTX_9_)
    pDXTexture = NULL;
    #endif
}


/************************************************************************
*    DESCRIPTION:  Destructor                                                             
*
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
CTextureMap::~CTextureMap()
{
    Free();
}


/************************************************************************
*    DESCRIPTION:  Free textures loaded into the video card.
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void CTextureMap::Free()
{
    if( openGLtextID != 0 )
    {
        glDeleteTextures( 1, &openGLtextID );
        openGLtextID = 0;
    }

    if( pBitData != NULL )
    {
        delete [] pBitData;
        pBitData = NULL;
    }

    #if defined(_USE_DIRECTX_8_) || defined(_USE_DIRECTX_9_)
    if( pDXTexture != NULL )
    {
        pDXTexture->Release();
        pDXTexture = NULL;
    }
    #endif

    // Reset the header
    mapHeader.Reset();

}   // Free


/************************************************************************
*    DESCRIPTION:   Figure out what type of file we are dealing
*                   with and call the function to load it.
*
*    Input:    char *fileName    - Path to load file
*              EBT_FLIP_FLOP flipFlop - Convert bitmap to top done
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CTextureMap::LoadFromFile( char *fileName, EBT_FLIP_FLOP flipFlop )
{
    bool result = false;

    ///////////////////////////////////////////////////////////
    // Below are the supported file formats the library can use
    ///////////////////////////////////////////////////////////

    switch( GetFileType( fileName ) )
    {
        // Only 24 bit files
        case EN_BM_FILE_TYPE_BMP:
            result = LoadFromFile_BMP( fileName, flipFlop );
        break;
        
        // Only used for 24 & 32 images - RGB + alpha
        case EN_BM_FILE_TYPE_TGA:
            result = LoadFromFile_TGA( fileName, flipFlop );
        break;

        case EN_BM_FILE_TYPE_JPG:
        case EN_BM_FILE_TYPE_GIF:
            result = LoadFromFile_IPicture( fileName, flipFlop );
        break;
        
        default:
            PostErrorMsg( "Error loading texture File", "Not a supported file format. (%s)", fileName );
        break;  
    }

    return result;
    
}   // LoadFromFile


/************************************************************************
*    DESCRIPTION:   Use Windows IPicture to load GIF and JPEG files. 
*                   This function is called when building the
*                   texture library. The whole file is sucked in
*                   and stored in the library
*
*    Input:    char *fileName    - Path to load file
*              EBT_FLIP_FLOP flipFlop - Convert bitmap to top done
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CTextureMap::LoadFromFile_IPicture( char *fileName, EBT_FLIP_FLOP flipFlop )
{
    COLE_Pict iPicture;
    bool result = false;
    FILE *hFile = NULL;

    // Free all allocations
    Free();

    // Load the file to see that it can be loaded without error
    if( !iPicture.LoadFromFile( fileName ) )
    {
        PostErrorMsg( "Error loading Texture File", "Error opening file. (%s)", fileName );
        goto HANDLE_ERROR;
    }

    // get the width and height of the image
    mapHeader.width = iPicture.Width();
    mapHeader.height = iPicture.Height();

    // The files Windows IPicture handles is always converted to 24 bit
    mapHeader.bitCount = 24;

    // Is the width and height in range?
    if( !CheckRange( mapHeader.width, mapHeader.height, fileName ) )
    {
        goto HANDLE_ERROR;
    }

    // Get the size of the image file. This variable is only set if we are to load 
    // files via Windows iPicture. Do not set it any where else.
    mapHeader.iPictureSizeInBytes = iPicture.GetSizeInBytes();

    // Allocate memory for the whole image file
    pBitData = new unsigned char [ mapHeader.iPictureSizeInBytes ];
    
    // Check all allocations
    if( pBitData == NULL )
    {
        PostErrorMsg( "Error loading Texture File", "Could not allocate memory to hold file data. (%s)", fileName );
        goto HANDLE_ERROR;
    }

    // Try to load the bitmap file
    hFile = fopen ( fileName, "rb" );    
    if( hFile == NULL )
    {
        PostErrorMsg( "Error loading texture File", "Error opening file. (%s)", fileName );
        goto HANDLE_ERROR;
    }

    // Load the file data. The whole file is being saved into the library
    if( fread( pBitData, mapHeader.iPictureSizeInBytes, 1, hFile ) != 1 )
    {
        PostErrorMsg( "Error loading texture File", "Can't load file header. (%s)", fileName );
        goto HANDLE_ERROR;
    }

    // If we made it this far, we are OK
    result = true;

HANDLE_ERROR:;

    // close the file if it has been opened
    if( hFile )
        fclose( hFile );   

    return result;

}   // LoadFromFile_IPicture


/************************************************************************
*    DESCRIPTION:   Load the BMP texture file. This function is 
*                   called when building the texture library.
*
*    Input:    char *fileName    - Path to load file
*              EBT_FLIP_FLOP flipFlop - Convert bitmap to top done
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CTextureMap::LoadFromFile_BMP( char *fileName, EBT_FLIP_FLOP flipFlop )
{
    FILE *hFile;
    bool result = false;
    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;
    int size, lineSize, bytesPerPixel;

    // Free all allocations
    Free();

    // Try to load the bitmap file
    hFile = fopen ( fileName, "rb" );    
    if( hFile == NULL )
    {
        PostErrorMsg( "Error loading Windows BMP File", "Error opening file. (%s)", fileName );
        goto HANDLE_ERROR;
    }

    // Load the file header data
    if( fread ( &fileHeader, sizeof( fileHeader ), 1, hFile ) != 1 )
    {
        PostErrorMsg( "Error loading Windows BMP File", "Can't load file header. (%s)", fileName );
        goto HANDLE_ERROR;
    }

    // Check that this a true BMP file?
    if( fileHeader.bfType != BITMAP_TYPE )
    {
        PostErrorMsg( "Error loading Windows BMP File", "Not a true Windows BMP file. (%s)", fileName );
        goto HANDLE_ERROR;
    }

    // Load the information header data
    if( fread ( &infoHeader, sizeof( infoHeader ), 1, hFile ) != 1 )
    {
        PostErrorMsg( "Error loading Windows BMP File", "Error loading info header. (%s)", fileName );
        goto HANDLE_ERROR;
    }

    // get the width and height of the image
    mapHeader.width = infoHeader.biWidth;
    mapHeader.height = abs(infoHeader.biHeight);

    // Get the bit count
    mapHeader.bitCount = infoHeader.biBitCount;

    // Is the width and height in range?
    if( !CheckRange( mapHeader.width, mapHeader.height, fileName ) )
    {
        goto HANDLE_ERROR;
    }

    // Load the file ourselves if it is 24 bit and uncompressed. No need to use Windows IPicture for this.
    if( infoHeader.biBitCount == 24 && infoHeader.biCompression == BI_RGB )
    {
        // Get the size of the data
        bytesPerPixel = mapHeader.bitCount / 8;
        lineSize = (mapHeader.width * bytesPerPixel) * sizeof( unsigned char );
        size = lineSize * mapHeader.height;

        // Allocate memory for the whole image
        pBitData = new unsigned char [ size ];
        
        // Check all allocations
        if( pBitData == NULL )
        {
            PostErrorMsg( "Error loading Windows BMP File", "Could not allocate memory to hold file data. (%s)", fileName );
            goto HANDLE_ERROR;
        }

        // Read the file into the buffer
        if( fread( pBitData, size, 1, hFile ) != 1 )
        {
            PostErrorMsg( "Error loading Windows BMP File", "Error reading file into buffer. (%s)", fileName );
            goto HANDLE_ERROR;
        }

        if( flipFlop == EBT_BOTTOM_TOP_FLIP )
        {
            if( !FlipBitmap( pBitData, size ) )
            {
                PostErrorMsg( "Error flipping bitmap. (%s)", fileName );
                goto HANDLE_ERROR;
            }
        }

        // We need the data in true RGB order
        SwapRedandBlue();
    }
    // The file is not 24 bit and/or it is a compressed BMP file. 
    // In any case, load it using Windows IPicture
    else
    {
        if( !LoadFromFile_IPicture( fileName, flipFlop ) )
            goto HANDLE_ERROR;
    }

    // If we made it this far, we are OK
    result = true;

HANDLE_ERROR:;

    // close the file if it has been opened
    if( hFile )
        fclose( hFile );   

    return result;

}   // LoadFromFile_BMP


/************************************************************************
*    DESCRIPTION:  Load the TGA texture file. TGA files can be
*                  32 bit but we'll also support 24 bit.
*
*    Input:    char *fileName    - Path to load file
*              EBT_FLIP_FLOP flipFlop - Convert bitmap to top done
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CTextureMap::LoadFromFile_TGA( char *fileName, EBT_FLIP_FLOP flipFlop )
{
    FILE *hFile;
    bool result = false;
    STGA_HEADER fileHeader;
    int size, lineSize, bytesPerPixel;

    // Free all allocations
    Free();

    // Try to load the bitmap file
    hFile = fopen ( fileName, "rb" );    
    if( hFile == NULL )
    {
        PostErrorMsg( "Error loading TGA File", "Error opening file. (%s)", fileName );
        goto HANDLE_ERROR;
    }

    // Load the file header data
    if( fread ( &fileHeader, sizeof( fileHeader ), 1, hFile ) != 1 )
    {
        PostErrorMsg( "Error loading TGA File", "Can't load file header. (%s)", fileName );
        goto HANDLE_ERROR;
    }

    // get the width and height of the image
    mapHeader.width = fileHeader.width;
    mapHeader.height = fileHeader.height;

    // Get the bit count
    mapHeader.bitCount = fileHeader.bits;

    // Is the width and height in range?
    if( !CheckRange( mapHeader.width, mapHeader.height, fileName ) )
    {
        goto HANDLE_ERROR;
    }

    // Check the bit count
    if( mapHeader.bitCount < 24 )
    {
        PostErrorMsg( "Error loading TGA File", "The file (%s) is %d bits wide. Only 24 & 32 bit files are supported.", mapHeader.bitCount, fileName );
        goto HANDLE_ERROR;
    }

    // Only uncompressed RGB files are supported
    if( fileHeader.imagetype != 2 )
    {
        PostErrorMsg( "Error loading TGA File", "Only uncompressed RGB files are supported.", fileName );
        goto HANDLE_ERROR;
    }

    // Get the size of the data
    bytesPerPixel = mapHeader.bitCount / 8;
    lineSize = (mapHeader.width * bytesPerPixel) * sizeof( unsigned char );
    size = lineSize * mapHeader.height;

    // Allocate memory for the whole image
    pBitData = new unsigned char [ size ];
    
    // Check all allocations
    if( pBitData == NULL )
    {
        PostErrorMsg( "Error loading TGA File", "Could not allocate memory to hold file data. (%s)", fileName );
        goto HANDLE_ERROR;
    }

    // Read the file into the buffer
    if( fread( pBitData, size, 1, hFile ) != 1 )
    {
        PostErrorMsg( "Error loading Windows BMP File", "Error reading file into buffer. (%s)", fileName );
        goto HANDLE_ERROR;
    }

    // We need the data in true RGB order
    SwapRedandBlue();

    // If we made it this far, we are OK
    result = true;

HANDLE_ERROR:;

    // close the file if it has been opened
    if( hFile )
        fclose( hFile );   

    return result;

}   // LoadFromFile_TGA


/************************************************************************
*    DESCRIPTION:  Flip the bitmap
*
*    Input:    unsigned char *pBits  - Pointer to the bits
*              int size - Buffer size
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CTextureMap::FlipBitmap( unsigned char *pBits, int size )
{    
    int posY, tmpPosY, bytesPerPixel, scanLineSizeInBytes;

    // Allocate memory for the whole image
    unsigned char *tempBits = new unsigned char [ size ];

    if( tempBits == NULL )
        return false;

    // copy the data to the tempory buffer
    memcpy( tempBits, pBits, size );

    // Get the size of the data
    bytesPerPixel = mapHeader.bitCount / 8;
    scanLineSizeInBytes = (mapHeader.width * bytesPerPixel) * sizeof( unsigned char );

    // Rewrite the scan line to swap the red and blue colors
    for( int y = 0; y < mapHeader.height; ++y )
    {
        // Calculate the Y offset
        posY = y * scanLineSizeInBytes;
        tmpPosY = ( ( mapHeader.height - y ) * scanLineSizeInBytes ) - scanLineSizeInBytes;
                
        // Copy the scan line
        for( int i = 0; i < scanLineSizeInBytes; ++i )
        {            
            // Calculate the X offset
            pBits[posY + i] = tempBits[tmpPosY + i];
        }
    }

    return true;

}   // FlipBitmap


/************************************************************************
*    DESCRIPTION:  Is the width and height in range?
*
*    FUNCTION PARAMETERS:
*    Input:    int w    - width of file
*              int h    - width of file
*              char *fileName    - Path to load file
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CTextureMap::CheckRange( int w, int h, char *fileName )
{
    // check to make sure we are in range
    if( !((w == 4)  || (w == 8) || (w == 16) || (w == 32) || 
        (w == 64) || (w == 128) || (w == 256) || (w == 512)) )
    {
        PostErrorMsg( "Error loading Texture File", "File width not within range(4,8,16,32,64,128,256,512). (%s)", fileName );
        return false;
    }

    // check to make sure we are in range
    if( !((h == 4)  || (h == 8) || (h == 16) || (h == 32) || 
        (h == 64) || (h == 128) || (h == 256) || (h == 512)) )
    {
        PostErrorMsg( "Error loading Texture File", "File height not within range(4,8,16,32,64,128,256,512). (%s)", fileName );
        return false;
    }

    return true;

}   // CheckRange


/************************************************************************
*    DESCRIPTION:  Saves bitmap data to library. This
*                  function is called when building the texture
*                  library file.
*
*    Input:    FILE *hFile       - File pointer
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CTextureMap::SaveToLibraryFile( FILE *hFile )
{
    // Write the map header:
    if( fwrite( &mapHeader, sizeof( mapHeader ), 1, hFile ) != 1 )
        return false;

    if( mapHeader.iPictureSizeInBytes == 0 )
    {
        int size = ((mapHeader.width * (mapHeader.bitCount / 8)) * mapHeader.height) * sizeof( unsigned char );

        // Write the bit data:
        if( fwrite( pBitData, size, 1, hFile ) != 1 )
            return false;
    }
    else
    {
        // Write the bit data:
        if( fwrite( pBitData, mapHeader.iPictureSizeInBytes, 1, hFile ) != 1 )
            return false;
    }

    return true;

}   // SaveToLibraryFile


/************************************************************************
*    DESCRIPTION:  Load native bitmap texture from the library file
*
*    Input:    FILE *hFile       - File pointer
*              bool autMipMapping - Turn on auto mipmaping when loading texture
*              int textFilter - texture filter
*              int mipmapFilter - mipmapping filter
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CTextureMap::LoadFromLibraryFile_OpenGL( FILE *hFile, bool autoMipMap, int textFilter, int mipmapFilter )
{
    unsigned int size;
    bool result = false;

    // Free the image data
    Free();

    // Read the map header:
    if( fread( &mapHeader, sizeof( mapHeader ), 1, hFile ) != 1 )
        return false;

    // Do a sanity check. This insures we are not reading random data.
    if( mapHeader.bitCount < 24 || mapHeader.bitCount > 32 ||
        mapHeader.width < MIN_TEXTURE_SIZE  || mapHeader.width > MAX_TEXTURE_SIZE ||
        mapHeader.height < MIN_TEXTURE_SIZE || mapHeader.height > MAX_TEXTURE_SIZE )
    {
        goto HANDLE_ERROR;
    }

    // Set the alpha blend flag
    alphaBlend = (mapHeader.bitCount == 32);

    // Get the size of the data
    // Uncompressed 24/32 bit data
    if( mapHeader.iPictureSizeInBytes == 0 )
    {
        size = ((mapHeader.width * (mapHeader.bitCount / 8)) * mapHeader.height) * sizeof( unsigned char );
    }
    // Compressed BMP, gif or jpeg or uncompressed 8 bit or less data.
    // using IPicture to handle it
    else
    {
        size = mapHeader.iPictureSizeInBytes;
    }

    // Allocate memory for image
    pBitData = new unsigned char [ size ];
    
    // Check all allocations
    if( pBitData == NULL )
        goto HANDLE_ERROR;

    // Read the bit data from library file:
    if( fread( pBitData, size, 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // decode the data using Windows IPicture
    if( mapHeader.iPictureSizeInBytes > 0 )
    {
        if( !LoadIPictureData( pBitData, true ) )
            goto HANDLE_ERROR;    
    }

    // Load the texture into video memory
    LoadTextIntoVideoMemory_OpenGL( autoMipMap, textFilter, mipmapFilter );

    // If we made it this far, we are OK
    result = true;

HANDLE_ERROR:;

    // Free the image data. We are done with it.
    if( pBitData != NULL )
    {
        delete [] pBitData;
        pBitData = NULL;
    }

    return result;

}   // LoadFromLibraryFile_OpenGL


/************************************************************************
*    DESCRIPTION:  Load native bitmap texture from the library file
*
*    Input:    FILE *hFile       - File pointer
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CTextureMap::LoadFromLibraryFile_DirectX( FILE *hFile )
{
    bool result = false;

    #if defined(_USE_DIRECTX_8_) || defined(_USE_DIRECTX_9_)

    unsigned int size;

    int byteShift, length, i;
    unsigned char *pTmp;
    unsigned int *pImage;

    // Free the image data
    Free();

    // Read the map header:
    if( fread( &mapHeader, sizeof( mapHeader ), 1, hFile ) != 1 )
        return false;

    // Do a sanity check. This insures we are not reading random data.
    if( mapHeader.bitCount < 24 || mapHeader.bitCount > 32 ||
        mapHeader.width < MIN_TEXTURE_SIZE  || mapHeader.width > MAX_TEXTURE_SIZE ||
        mapHeader.height < MIN_TEXTURE_SIZE || mapHeader.height > MAX_TEXTURE_SIZE )
    {
        goto HANDLE_ERROR;
    }

    // Set the alpha blend flag
    alphaBlend = (mapHeader.bitCount == 32);

    // Get the size of the data
    // Uncompressed 24/32 bit data
    if( mapHeader.iPictureSizeInBytes == 0 )
    {
        size = ((mapHeader.width * (mapHeader.bitCount / 8)) * mapHeader.height) * sizeof( unsigned char );
    }
    // Compressed BMP, gif or jpeg or uncompressed 8 bit or less data.
    // using IPicture to handle it
    else
    {
        size = mapHeader.iPictureSizeInBytes;
    }

    // Allocate memory for image
    pBitData = new unsigned char [ size ];
    
    // Check all allocations
    if( pBitData == NULL )
        goto HANDLE_ERROR;

    // Read the bit data from library file:
    if( fread( pBitData, size, 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // decode the data using Windows IPicture
    if( mapHeader.iPictureSizeInBytes > 0 )
    {
        if( !LoadIPictureData( pBitData, true ) )
            goto HANDLE_ERROR;    
    }

    // We need to switch the red and green channels
    // Convert to 32 bit if it is 24 bit
    if( mapHeader.bitCount == 24 )
    {
        // Calculate the length
        length = mapHeader.width * mapHeader.height;
        byteShift = mapHeader.bitCount / 8;

        // Allocate memory for image
        pImage = new unsigned int [ length ];
        pTmp = (unsigned char *)pImage;

        // Convert to 32 bit
        for( i = 0; i < length; ++i )
        {
            pTmp[(i<<2)] = pBitData[(i*byteShift)+2];
            pTmp[(i<<2)+1] = pBitData[(i*byteShift)+1];
            pTmp[(i<<2)+2] = pBitData[(i*byteShift)];
            // Set to be opaque
            pTmp[(i<<2)+3] = 255;            
        }

        // Delete the main buffer and use the new buffer
        delete [] pBitData;
        pBitData = (unsigned char *)pImage;
    }
    else if( mapHeader.bitCount == 32 )
    {
        // Calculate the length
        length = mapHeader.width * mapHeader.height;
        byteShift = mapHeader.bitCount / 8;

        // Allocate memory for image
        pImage = new unsigned int [ length ];
        pTmp = (unsigned char *)pImage;

        // Convert to 32 bit
        for( i = 0; i < length; ++i )
        {
            pTmp[(i<<2)] = pBitData[(i*byteShift)+2];
            pTmp[(i<<2)+1] = pBitData[(i*byteShift)+1];
            pTmp[(i<<2)+2] = pBitData[(i*byteShift)];
            pTmp[(i<<2)+3] = pBitData[(i*byteShift)+3];            
        }

        // Delete the main buffer and use the new buffer
        delete [] pBitData;
        pBitData = (unsigned char *)pImage;
    }

    // Load the texture into video memory
    if( LoadTextIntoVideoMemory_DirectX() == false )
        goto HANDLE_ERROR;

    // If we made it this far, we are OK
    result = true;

HANDLE_ERROR:;

    // Free the image data. We are done with it.
    if( pBitData != NULL )
    {
        delete [] pBitData;
        pBitData = NULL;
    }

    #endif

    return result;

}   // LoadFromLibraryFile_DirectX


/************************************************************************
*    DESCRIPTION:  Load native bitmap texture from the library file
*
*    Input:    FILE *hFile       - File pointer
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CTextureMap::LoadFromLibraryFile_Software( FILE *hFile )
{
    unsigned int size;
    bool result = false;    
    int byteShift, length, i;
    
    #ifndef _16_BIT_SOFTWARE_RENDERING_
    unsigned int *pImage = NULL;
    unsigned char *pTmp;
    #else
    unsigned short int *pImage = NULL;
    #endif

    // Free the image data
    Free();

    // Read the map header:
    if( fread( &mapHeader, sizeof( mapHeader ), 1, hFile ) != 1 )
        return false;

    // Do a sanity check. This insures we are not reading random data.
    if( mapHeader.bitCount < 24 || mapHeader.bitCount > 32 ||
        mapHeader.width < MIN_TEXTURE_SIZE  || mapHeader.width > MAX_TEXTURE_SIZE ||
        mapHeader.height < MIN_TEXTURE_SIZE || mapHeader.height > MAX_TEXTURE_SIZE )
    {
        goto HANDLE_ERROR;
    }

    // Set the alpha blend flag
    alphaBlend = (mapHeader.bitCount == 32);

    byteShift = mapHeader.bitCount / 8;

    // Set the multi shift value
    switch( mapHeader.width )
    {
        case ( 4 ):   multiShift = 2; break;
        case ( 8 ):   multiShift = 3; break;
        case ( 16 ):  multiShift = 4; break;
        case ( 32 ):  multiShift = 5; break;
        case ( 64 ):  multiShift = 6; break;
        case ( 128 ): multiShift = 7; break;
        case ( 256 ): multiShift = 8; break;
        default:
            multiShift = 0; break;
    }

    // Get the size of the data
    // Uncompressed 24/32 bit data
    if( mapHeader.iPictureSizeInBytes == 0 )
    {
        size = ((mapHeader.width * (mapHeader.bitCount / 8)) * mapHeader.height) * sizeof( unsigned char );
    }
    // Compressed BMP, gif or jpeg or uncompressed 8 bit or less data.
    // using IPicture to handle it
    else
    {
        size = mapHeader.iPictureSizeInBytes;
    }

    // Allocate memory for image
    pBitData = new unsigned char [ size ];
    
    // Check all allocations
    if( pBitData == NULL )
        goto HANDLE_ERROR;

    // Read the bit data from library file:
    if( fread( pBitData, size, 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // decode the data using Windows IPicture
    if( mapHeader.iPictureSizeInBytes > 0 )
    {
        if( !LoadIPictureData( pBitData, true ) )
            goto HANDLE_ERROR;    
    }

    // Calculate the length
    length = mapHeader.width * mapHeader.height;

    // Allocate memory for image
    #ifndef _16_BIT_SOFTWARE_RENDERING_
    pImage = new unsigned int [ length ];
    pTmp = (unsigned char *)pImage;
    #else
    pImage = new unsigned short int [ length ];
    #endif

    if( mapHeader.bitCount == 24 )
    {
        for( i = 0; i < length; ++i )
        {
            // Convert to software 32 bit
            #ifndef _16_BIT_SOFTWARE_RENDERING_
            pTmp[(i<<2)] = pBitData[(i*byteShift)+2];
            pTmp[(i<<2)+1] = pBitData[(i*byteShift)+1];
            pTmp[(i<<2)+2] = pBitData[(i*byteShift)];
            pTmp[(i<<2)+3] = 0; // not using the alpha channel here
            #else
            // Convert to 16 bit
            pImage[i] = ((pBitData[(i*byteShift)] & 0xF8) << 7) | 
                        ((pBitData[(i*byteShift)+1] & 0xF8) << 2) | 
                        (pBitData[(i*byteShift)+2] >> 3);

            #endif
        }
    }
    else
    {
        // If we loaded a texture with alpha data, apply the alpha value
		// to the texture now so we don't have to do it later. Then store
		// the alpha value as a fixed point number to use later on the
		// destination values the texture will be applied to.
        for( i = 0; i < length; ++i )
        {
            // Already 32 bit color but need to re-arrange color
            #ifndef _16_BIT_SOFTWARE_RENDERING_

            // get the alpha value but convert it to a percentage
            float alpha = (float)pBitData[(i*byteShift)+3] / 255.0f;

            // Multiply the alpha by the rgb values
            pTmp[(i<<2)] = alpha * (float)pBitData[(i*byteShift)+2];
            pTmp[(i<<2)+1] = alpha * (float)pBitData[(i*byteShift)+1];
            pTmp[(i<<2)+2] = alpha * (float)pBitData[(i*byteShift)];

            // store the fix point alpha value into the last 8 bits
            pTmp[(i<<2)+3] = (1 - alpha) * FIX_SHIFT_6_FLOAT;
            #else
            // Convert to 16 bit
            pImage[i] = ((pBitData[(i*byteShift)] & 0xF8) << 7) | 
                        ((pBitData[(i*byteShift)+1] & 0xF8) << 2) | 
                        (pBitData[(i*byteShift)+2] >> 3);

            #endif
        }
    }

    // Delete the main buffer and use the new buffer
    delete [] pBitData;
    pBitData = (unsigned char *)pImage;

    // If we made it this far, we are OK
    result = true;

HANDLE_ERROR:;

    // Free the image data. We are done with it.
    if( !result )
    {
        if( pBitData != NULL )
        {
            delete [] pBitData;
            pBitData = NULL;
        }

        if( pImage != NULL )
        {
            delete [] pImage;
        }
    }

    return result;

}   // LoadFromLibraryFile


/************************************************************************
*    DESCRIPTION:  Load IPicture data and convert
*
*    Input:    unsigned char *pData - Pointer to buffer
*              bool deletePtr - Flag to indicate if memory is to be freed
*
*    Output:   bool - true on success, false on error
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CTextureMap::LoadIPictureData( unsigned char *pData, bool deletePtr )
{
    COLE_Pict iPicture;

    // Load the IPicture pointer data
    if( !iPicture.LoadFromPointer( pData, mapHeader.iPictureSizeInBytes ) )
    {
        return false;
    }

    // Delete the data pointer and make a new one
    if( deletePtr == true )
    {
        delete [] pData;
    }

    // Allocate memory for image
    pBitData = new unsigned char [ ((mapHeader.width * 3) * mapHeader.height) * sizeof(unsigned char) ];
    
    // Check all allocations
    if( pBitData == NULL )
    {
        return false;
    }

    // Have IPicture generate the bits to our buffer
    if( !iPicture.GetBitmapBits( pBitData, mapHeader.width, mapHeader.height, mapHeader.bitCount ) )
    {
        return false;
    }

    // We need the data in true RGB order
    SwapRedandBlue();

    return true;

}   // LoadIPictureData


/************************************************************************
*    DESCRIPTION:  Load native bitmap texture library format from
*                  a pointer. Mostly used for loading a texture
*                  library from resource.
*
*                  With the passed pointer, we load the data and
*                  inc the pointer and send it back. Similar to
*                  loading data via a file handle but we need to
*                  move the point position ourselves.
*
*    Input:    unsigned char *pData - Pointer to texture library.
*              bool autMipMapping - Turn on auto mipmaping when loading texture
*              int textFilter - texture filter
*              int mipmapFilter - mipmapping filter
*
*    Output:   unsigned char * - Return the inc pointer
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
unsigned char *CTextureMap::LoadFromPointer( unsigned char *pData, bool autoMipMap, 
                                             int textFilter, int mipmapFilter )
{
    unsigned int size;
    
    // Free the image data
    Free();

    // Get the pointer to the data
    SMap_Header *pHeader = (SMap_Header *)pData;

    // Set the class data
    mapHeader.bitCount = pHeader->bitCount;
    mapHeader.width = pHeader->width;
    mapHeader.height = pHeader->height;
    mapHeader.iPictureSizeInBytes = pHeader->iPictureSizeInBytes;

    // Do a sanity check. This insures we are not reading random data.
    if( mapHeader.bitCount < 24 || mapHeader.bitCount > 32 ||
        mapHeader.width < MIN_TEXTURE_SIZE  || mapHeader.width > MAX_TEXTURE_SIZE ||
        mapHeader.height < MIN_TEXTURE_SIZE || mapHeader.height > MAX_TEXTURE_SIZE )
    {
        return false;
    }

    // inc the pointer
    pData += sizeof( SMap_Header );

    // decode the data using Windows IPicture
    if( mapHeader.iPictureSizeInBytes > 0 )
    {
        if( !LoadIPictureData( pData, false ) )
            return false;
            
        size = mapHeader.iPictureSizeInBytes;    
    }
    // Uncompressed bit data in library
    else
    {
        size = ((mapHeader.width * (mapHeader.bitCount / 8)) * mapHeader.height) * sizeof( unsigned char );
    }

    // Load the texture into video memory
    LoadTextIntoVideoMemory_OpenGL( autoMipMap, textFilter, mipmapFilter );

    // inc the pointer
    pData += size;

    return pData;

}   // LoadFromPointer


/************************************************************************
*    DESCRIPTION:  We need the data in true RGB order.
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CTextureMap::SwapRedandBlue()
{
    int posY, posX, bytesPerPixel, scanLineSizeInBytes;
    unsigned char temp;

    // Get the size of the data
    bytesPerPixel = mapHeader.bitCount / 8;
    scanLineSizeInBytes = (mapHeader.width * bytesPerPixel) * sizeof( unsigned char );

    // Rewrite the scan line to swap the red and blue colors
    for( int y = 0; y < mapHeader.height; ++y )
    {
        // Calculate the Y offset
        posY = y * scanLineSizeInBytes;
                
        for( int i = 0; i < mapHeader.width; ++i )
        {            
            // Calculate the X offset
            posX = posY + (i * bytesPerPixel);

            // Swap red and blue
            temp = pBitData[ posX + 0 ];
            pBitData[ posX + 0 ] = pBitData[ posX + 2 ];
            pBitData[ posX + 2 ] = temp;
        }
    }
}   // SwapRedandGreen


/************************************************************************
*    DESCRIPTION:  Load the texture into video memory.
*
*    Input:    bool autMipMapping - Turn on auto mipmaping when loading texture
*              int textFilter - texture filter
*              int mipmapFilter - mipmapping filter
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void CTextureMap::LoadTextIntoVideoMemory_OpenGL( bool autoMipMap, 
                                                  int textFilter, int mipmapFilter )
{
    // Generate a unique id
    glGenTextures( 1, &openGLtextID );

    // Bind the texture object with the ID
    glBindTexture( GL_TEXTURE_2D, openGLtextID );

    // Do we want to auto mipmap the texture?
    if( autoMipMap )
    {
        // Set the filtering
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textFilter );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmapFilter );

        // load the texture that also generates the mipmaps
        if( mapHeader.bitCount == 24 )
        {
            gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, mapHeader.width, mapHeader.height, GL_RGB, GL_UNSIGNED_BYTE, pBitData );
        }
        else if( mapHeader.bitCount == 32 )
        {
            gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, mapHeader.width, mapHeader.height, GL_RGBA, GL_UNSIGNED_BYTE, pBitData );
        }
    }
    else
    {
        // Set the filtering
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textFilter );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textFilter );

        // load the texture
        if( mapHeader.bitCount == 24 )
        {
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, mapHeader.width, mapHeader.height, 0, GL_RGB, GL_UNSIGNED_BYTE, pBitData );
        }
        else if( mapHeader.bitCount == 32 )
        {
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mapHeader.width, mapHeader.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBitData );
        }
    }
}   // LoadTextIntoVideoMemory_OpenGL


/************************************************************************
*    DESCRIPTION:  Load the texture into video memory.
*
*    Input:    unsigned char *pData - Pointer to texture library.
*              bool autMipMapping - Turn on auto mipmaping when loading texture
*              int textFilter - texture filter
*              int mipmapFilter - mipmapping filter
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
bool CTextureMap::LoadTextIntoVideoMemory_DirectX()
{
    #if defined(_USE_DIRECTX_8_) || defined(_USE_DIRECTX_9_)

    D3DLOCKED_RECT lr;
    
    // Create the texture surface
    if( D3D_OK != D3DXCreateTexture( g_pDXDevice, mapHeader.width, mapHeader.height, 1, 0, 
                                     D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pDXTexture ) )
        return false;

    // Lock it
    if( D3D_OK != pDXTexture->LockRect( 0, &lr, NULL, 0 ) )
        return false;

    // Copy it to memory
    memcpy( lr.pBits, pBitData, mapHeader.width * 4 * mapHeader.height );

    // Unlock it
    if( D3D_OK != pDXTexture->UnlockRect( 0 ) )
        return false;

    #endif

    return true;

}   // LoadTextIntoVideoMemory_DirectX


/************************************************************************
*    DESCRIPTION:  Get the type of file being loaded
*
*    Input:    char *fileName    - Path to load file
*
*    Output:   int - Value indicating file type
*
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
int CTextureMap::GetFileType( char *fileName )
{
    char fileExt[10];
    int Result;

    if( !fileName || !fileName[0] )
        return EN_BM_NO_FILE_TYPE;

    ////////////////////////////
    // Get the file extension
    ////////////////////////////

    // Search for the "." for the extension
    for( int i = strlen( fileName ); i > -1; --i )
    {
        if( fileName[i] == '.' )
        {
            // copy over the file extension
            strcpy( fileExt, (fileName + i + 1) );

            // Upper case the file extension for compairison
            strupr( fileExt );

            break;
        }
    }
    
    // Which file format are we loading?
    if( strcmp( fileExt, "BMP" ) == 0 )
        Result = EN_BM_FILE_TYPE_BMP;

    else if( strcmp( fileExt, "JPG" ) == 0 )
        Result = EN_BM_FILE_TYPE_JPG;

    else if( strcmp( fileExt, "JPEG" ) == 0 )
        Result = EN_BM_FILE_TYPE_JPG;

    else if( strcmp( fileExt, "GIF" ) == 0 )
        Result = EN_BM_FILE_TYPE_GIF;

    else if( strcmp( fileExt, "TGA" ) == 0 )
        Result = EN_BM_FILE_TYPE_TGA;

    // not supported
    else if( strcmp( fileExt, "PNG" ) == 0 )
        Result = EN_BM_FILE_TYPE_PNG;

    // not supported
    else if( strcmp( fileExt, "MNG" ) == 0 )
        Result = EN_BM_FILE_TYPE_MNG;

    else
        Result = EN_BM_NO_FILE_TYPE;

    return Result;

}   // GetFileType


/************************************************************************
*    DESCRIPTION:  Translates into a windows messagebox with
*                  sprintf capabilities. Makes debugging easier.
*
*    Input:    char *Title - Title of the windows message box
*              char *fmt   - Used with vprintf to get the argument list
*                            for wvsprintf.
*
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void PostErrorMsg( char *Title, char *fmt, ... )
{
    va_list argptr;
    char StrBuffer[1024];

    va_start( argptr, fmt );
    vprintf( fmt, argptr );
    wvsprintf( StrBuffer, fmt, argptr );
    MessageBox( GetActiveWindow(), StrBuffer, Title, MB_ICONINFORMATION );
    va_end( argptr );

} // PostErrorMsg


