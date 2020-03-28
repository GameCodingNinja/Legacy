
/************************************************************************
*
*    PRODUCT:         3D Engine texture library
*
*    FILE NAME:       CTextMap.cpp
*
*    DESCRIPTION:     Creating and loading a texture from the library. Loads
*                     native uncompressed Windows 24 bit BMP and 24/32 bit
*                     TGA files. For compressed BMP, gif and JPEG files,
*                     the Windows IPicture is used.
*
*    Copyright(c) John De Goes & Howard Rosenorn
*    All Rights Reserved
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


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
CTextureMap::CTextureMap()
{
    textureID = 0;
    pBitData = NULL;
}


/************************************************************************
*    FUNCTION NAME:              Destructor                                                             
*
*    HISTORY:
*    Name          Date          Comment
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
*    FUNCTION NAME:         Free()                                                             
*
*    DESCRIPTION:           Free textures loaded into the video card.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CTextureMap::Free()
{
    if( textureID != 0 )
    {
        glDeleteTextures( 1, &textureID );
        textureID = 0;
    }

    if( pBitData != NULL )
    {
        delete [] pBitData;
        pBitData = NULL;
    }

    // Reset the header
    mapHeader.Reset();

}   // Free


/************************************************************************
*    FUNCTION NAME:         LoadFromFile()                                                             
*
*    DESCRIPTION:           Figure out what type of file we are dealing
*                           with and call the function to load it.
*
*    FUNCTION PARAMETERS:
*    Input:    char *fileName    - Path to load file
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
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
*    FUNCTION NAME:         LoadFromFile_IPicture()                                                             
*
*    DESCRIPTION:           Use Windows IPicture to load GIF and JPEG files. 
*                           This function is called when building the
*                           texture library. The whole file is sucked in
*                           and stored in the library
*
*    FUNCTION PARAMETERS:
*    Input:    char *fileName    - Path to load file
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
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
*    FUNCTION NAME:         LoadFromFile_BMP()                                                             
*
*    DESCRIPTION:           Load the BMP texture file. This function is 
*                           called when building the texture library.
*
*    FUNCTION PARAMETERS:
*    Input:    char *fileName    - Path to load file
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
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
*    FUNCTION NAME:         LoadFromFile_TGA()                                                             
*
*    DESCRIPTION:           Load the TGA texture file. TGA files can be
*                           32 bit but we'll also support 24 bit.
*
*    FUNCTION PARAMETERS:
*    Input:    char *fileName    - Path to load file
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
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
*    FUNCTION NAME:         FlipBitmap()                                                             
*
*    DESCRIPTION:           Flip the bitmap
*
*    FUNCTION PARAMETERS:
*    Input:    char *fileName    - Path to load file
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
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
*    FUNCTION NAME:         CheckRange()                                                             
*
*    DESCRIPTION:           Is the width and height in range?
*
*    FUNCTION PARAMETERS:
*    Input:    int w    - width of file
*              int h    - width of file
*              char *fileName    - Path to load file
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
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
*    FUNCTION NAME:         SaveToLibraryFile()                                                             
*
*    DESCRIPTION:           Saves bitmap data to library. This
*                           function is called when building the texture
*                           library file.
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile       - File pointer
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
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
*    FUNCTION NAME:         LoadFromLibraryFile()                                                             
*
*    DESCRIPTION:           Load native bitmap texture from the library file
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile       - File pointer
*              bool autMipMapping - Turn on auto mipmaping when loading texture
*              int textFilter - texture filter
*              int mipmapFilter - mipmapping filter
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CTextureMap::LoadFromLibraryFile( FILE *hFile, bool autoMipMap, int textFilter, int mipmapFilter )
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
    LoadTextIntoVideoMemory( pBitData, autoMipMap, textFilter, mipmapFilter );

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

}   // LoadFromLibraryFile


/************************************************************************
*    FUNCTION NAME:         LoadIPictureData()                                                             
*
*    DESCRIPTION:           Load IPicture data and convert
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
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
*    FUNCTION NAME:         LoadFromPointer()                                                             
*
*    DESCRIPTION:           Load native bitmap texture library format from
*                           a pointer. Mostly used for loading a texture
*                           library from resource.
*
*                           With the passed pointer, we load the data and
*                           inc the pointer and send it back. Similar to
*                           loading data via a file handle but we need to
*                           move the point position ourselves.
*
*    FUNCTION PARAMETERS:
*    Input:    unsigned char *pData - Pointer to texture library.
*              bool autMipMapping - Turn on auto mipmaping when loading texture
*              int textFilter - texture filter
*              int mipmapFilter - mipmapping filter
*
*    Output:   unsigned char * - Return the inc pointer
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

unsigned char *CTextureMap::LoadFromPointer( unsigned char *pData, bool autoMipMap, 
                                             int textFilter, int mipmapFilter )
{
    unsigned char *pTmpData;
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
            
        pTmpData = pBitData;
        size = mapHeader.iPictureSizeInBytes;    
    }
    // Uncompressed bit data in library
    else
    {
        pTmpData = pData;
        size = ((mapHeader.width * (mapHeader.bitCount / 8)) * mapHeader.height) * sizeof( unsigned char );
    }

    // Load the texture into video memory
    LoadTextIntoVideoMemory( pTmpData, autoMipMap, textFilter, mipmapFilter );

    // inc the pointer
    pData += size;

    return pData;

}   // LoadFromPointer


/************************************************************************
*    FUNCTION NAME:         SwapRedandGreen()                                                             
*
*    DESCRIPTION:           We need the data in true RGB order.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
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
*    FUNCTION NAME:         LoadTextIntoVideoMamory()                                                             
*
*    DESCRIPTION:           Load the texture into video memory.
*
*    FUNCTION PARAMETERS:
*    Input:    unsigned char *pData - Pointer to texture library.
*              bool autMipMapping - Turn on auto mipmaping when loading texture
*              int textFilter - texture filter
*              int mipmapFilter - mipmapping filter
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CTextureMap::LoadTextIntoVideoMemory( unsigned char *pData, bool autoMipMap, 
                                           int textFilter, int mipmapFilter )
{
    // Generate a unique id
    glGenTextures( 1, &textureID );

    // Bind the texture object with the ID
    glBindTexture( GL_TEXTURE_2D, textureID );

    // Do we want to auto mipmap the texture?
    if( autoMipMap )
    {
        // Set the filtering
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textFilter );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmapFilter );

        // load the texture that also generates the mipmaps
        if( mapHeader.bitCount == 24 )
        {
            gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, mapHeader.width, mapHeader.height, GL_RGB, GL_UNSIGNED_BYTE, pData );
        }
        else if( mapHeader.bitCount == 32 )
        {
            gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, mapHeader.width, mapHeader.height, GL_RGBA, GL_UNSIGNED_BYTE, pData );
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
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, mapHeader.width, mapHeader.height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData );
        }
        else if( mapHeader.bitCount == 32 )
        {
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mapHeader.width, mapHeader.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData );
        }
    }
}   // LoadTextIntoVideoMemory


/************************************************************************
*    FUNCTION NAME:         GetFileType()                                                             
*
*    DESCRIPTION:           Get the type of file being loaded
*
*    FUNCTION PARAMETERS:
*    Input:    char *fileName    - Path to load file
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
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
*    FUNCTION NAME:         PostErrorMsg                                                             
*
*    DESCRIPTION:           Translates into a windows messagebox with
*                           sprintf capabilities. Makes debugging easier.
*
*    FUNCTION PARAMETERS:
*    Input:    char *Title - Title of the windows message box
*              char *fmt   - Used with vprintf to get the argument list
*                            for wvsprintf.
*
*    Output:   returntype - None
*
*    HISTORY:
*    Name          Date          Comment
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


