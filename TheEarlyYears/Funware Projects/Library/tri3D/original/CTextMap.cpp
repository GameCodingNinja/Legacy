
/************************************************************************
*
*    PRODUCT:         True color 3D Engine texture library
*
*    FILE NAME:       CTextMap.cpp
*
*    DESCRIPTION:     Creating and loading a texture library
*
*    Copyright(c) John De Goes & Howard Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

#include "CTextMap.h"        // This objects header file
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <string.h>          // string functions


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
    pImage32 = NULL;
    pImage16 = NULL;
    width = 0;
    height = 0;
    multiShift = 0;
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
*    DESCRIPTION:           Free allocated data
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
    if( pImage32 )
    {
        delete [] pImage32;
        pImage32 = NULL;
    }

    if( pImage16 )
    {
        delete [] pImage16;
        pImage16 = NULL;
    }

}   // Free


/************************************************************************
*    FUNCTION NAME:         LoadFromFile()                                                             
*
*    DESCRIPTION:           Load the bitmap texture file. This function is
*                           called when building the texture library file.
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

bool CTextureMap::LoadFromFile( char *fileName )
{
    int y, posY, i;
    unsigned int color;
    FILE *hFile;
    bool result = false;
    unsigned char *tmpData = NULL;
    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;

    // Free all allocations
    Free();

    // Try to load the bitmap file
    hFile = fopen ( fileName, "rb" );    
    if( hFile == NULL )
        goto HANDLE_ERROR;

    // Load the file header data
    if( fread ( &fileHeader, sizeof( fileHeader ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Check that this a true BMP file?
    if( fileHeader.bfType != BITMAP_TYPE )
        goto HANDLE_ERROR;

    // Load the information header data
    if( fread ( &infoHeader, sizeof( infoHeader ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // We don't support anyhting less then 24 bit bmp files.
    if( infoHeader.biBitCount < 24 )
        goto HANDLE_ERROR;

    // Image widths that are 4, 8, 16, 32, 64, 128, 256 
    // are automatically quad aligned. No need to check for it.
    // This works for 24 and 32 bit images
    width  = infoHeader.biWidth;
    height = abs( infoHeader.biHeight );

    // Do a sanity check. Height need be less then the max texture size
    if( height > MAX_TEXTURE_SIZE )
        goto HANDLE_ERROR;

    // Get the multiplier shift. Will return false if the with doesn't 
    // fall into range. For this function, the shift is not going to be used
    // but it provides a way to check the width of the image.
    if( !SetMultiShift( infoHeader.biWidth ) )
        goto HANDLE_ERROR;

    // Allocate memory for image
    pImage32 = new unsigned int [ width * height ];
    
    // Check all allocations
    if( pImage32 == NULL )
        goto HANDLE_ERROR;

    // If this is 24 bit color that we need to convert, allocate a tempoary array
    // to use as a way to read in the color to do the conversions
    if( infoHeader.biBitCount == 24 )
    {
        // Allocate memory for temporary scan line
        tmpData = new unsigned char [ width * 3 ];
        
        // Check all allocations
        if( tmpData == NULL )
            goto HANDLE_ERROR;
    }

    // If 24 bit color, read in and convert
    if( infoHeader.biBitCount == 24 )
    {
        // Load the bitmap
        for( y = 0; y < height; ++y )
        {
            // Read a line into the temporary scan line
            if( fread( tmpData, width * 3, 1, hFile ) != 1 )
                goto HANDLE_ERROR;
            
            // A bitmap can be saved top to bottom or bottom to top.
            // If this is a top to bottom image, the height is a negative number
            /*if( infoHeader.biHeight < 0 )
                posY = y * width;
            else
                posY = ( ( height - y ) * width ) - width;*/

            // The UV calculations take the bottom to top bitmap into consideration.
            // We doen't have to convert it ourselves.
            posY = y * width;

            // Conver the RGB scan line to a 32 bit color DWORD
            for( i = 0; i < width; ++i )
            {
                // Get the blue color and save it
                color =  tmpData[ (i * 3) + 0 ];

                // Get the green color, "OR" and shift it
                color |= tmpData[ (i * 3) + 1 ] << 8;

                // Get the red color, "OR" and shift it
                color |= tmpData[ (i * 3) + 2 ] << 16;

                // Save the new color
                pImage32[ posY + i ] = color;
            }
        }
    }
    else  // 32 bit bitmap
    {
        // Load the bitmap
        for( y = 0; y < height; ++y )
        {
            // A bitmap can be saved top to bottom or bottom to top.
            // If this is a top to bottom image, the height is a negative number
            /*if( infoHeader.biHeight < 0 )
                posY = y * width;
            else
                posY = ( ( height - y ) * width ) - width;*/

            // The UV calculations take the bottom to top bitmap into consideration.
            // We doen't have to convert it ourselves.
            posY = y * width;

            // Just a simple copy over
            if( fread( &pImage32[ posY ], width, 1, hFile ) != 1 )
                goto HANDLE_ERROR;
        }
    }

    // If we made it this far, we are OK
    result = true;

HANDLE_ERROR:;

    // close the file if it has been opened
    if( hFile )
        fclose( hFile );
        
    if( tmpData )
        delete [] tmpData;   

    return result;

}   // LoadFromFile


/************************************************************************
*    FUNCTION NAME:         AllocateTextureSpace()                                                             
*
*    DESCRIPTION:           Allocate the texture space. This function is
*                           used if you need to dynamically allocate a
*                           texture library on the fly.
*
*                           One example of needing to do this is if you
*                           need to texture map a big file to a complex
*                           polygon and don't want to precut it all up.
*                           Or another situation is if a user of your
*                           program wants to use there bitmaps as a texture. 
*
*    FUNCTION PARAMETERS:
*    Input:    int w - with of bitmap
*              int h - height of bitmap
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

bool CTextureMap::AllocateTextureSpace( int w, int h )
{
    // init the class variables
    width = w;
    height = h;

    // Do a sanity check. Height need be less then the max texture size
    if( height > MAX_TEXTURE_SIZE )
        return false;

    // Get the multiplier shift. Will return false if the with doesn't 
    // fall into range.
    if( !SetMultiShift( w ) )
        return false;

    // Allocate memory for image
    pImage32 = new unsigned int [ width * height ];
    
    // Check all allocations
    if( pImage32 == NULL )
        return false;

    return true;

}   // AllocateTextureSpace


/************************************************************************
*    FUNCTION NAME:         SaveBT_32()                                                             
*
*    DESCRIPTION:           Save 32 bit bitmap texture format file. This
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

bool CTextureMap::SaveBT_32( FILE *hFile )
{
    // Write the image width:
    if( fwrite( &width, sizeof width, 1, hFile ) != 1 )
        return false;

    // Write the image height:
    if( fwrite( &height, sizeof height, 1, hFile ) != 1 )
        return false;

    // Write the image:
    if( fwrite( pImage32, (width * height) * sizeof( unsigned int ), 1, hFile ) != 1 )
        return false;

    return true;

}   // SaveBT_32


/************************************************************************
*    FUNCTION NAME:         SaveBT_16()                                                             
*
*    DESCRIPTION:           Save 32 bit bitmap texture format file. This
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

bool CTextureMap::SaveBT_16( FILE *hFile )
{
    // Write the image width:
    if( fwrite( &width, sizeof width, 1, hFile ) != 1 )
        return false;

    // Write the image height:
    if( fwrite( &height, sizeof height, 1, hFile ) != 1 )
        return false;

    // Write the image:
    if( fwrite( pImage16, (width * height) * sizeof( unsigned short int ), 1, hFile ) != 1 )
        return false;

    return true;

}   // SaveBT_16


/************************************************************************
*    FUNCTION NAME:         LoadBT_32()                                                             
*
*    DESCRIPTION:           Load native bitmap texture from the library file
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

bool CTextureMap::LoadBT_32( FILE *hFile )
{
    // Free the image data
    Free();

    // Read the image width:
    if( fread( &width, sizeof( width ), 1, hFile ) != 1 )
        return false;

    // Read the image height:
    if( fread( &height, sizeof( height ), 1, hFile ) != 1 )
        return false;

    // Do a sanity check. Height needs to be within range
    if( height < 1 || height > MAX_TEXTURE_SIZE )
        return false;

    // Just to be safe, check the return value.
    if( !SetMultiShift( width ) )
        return false;

    // Allocate memory for image
    pImage32 = new unsigned int [ width * height ];
    
    // Check all allocations
    if( pImage32 == NULL )
        return false;

    // Read the image:
    if( fread( pImage32, (width * height) * sizeof( unsigned int ), 1, hFile ) != 1 )
        return false;

    return true;

}   // LoadBT_32


/************************************************************************
*    FUNCTION NAME:         LoadBT_16()                                                             
*
*    DESCRIPTION:           Load native bitmap texture from the library file
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

bool CTextureMap::LoadBT_16( FILE *hFile )
{
    // Free the image data
    Free();

    // Read the image width:
    if( fread( &width, sizeof( width ), 1, hFile ) != 1 )
        return false;

    // Read the image height:
    if( fread( &height, sizeof( height ), 1, hFile ) != 1 )
        return false;

    // Do a sanity check. Height needs to be within range
    if( height < 1 || height > MAX_TEXTURE_SIZE )
        return false;

    // Just to be save, check the return value.
    if( !SetMultiShift( width ) )
        return false;

    // Allocate memory for image
    pImage16 = new unsigned short int [ width * height ];
    
    // Check all allocations
    if( pImage16 == NULL )
        return false;

    // Read the image:
    if( fread( pImage16, (width * height) * sizeof( unsigned short int ), 1, hFile ) != 1 )
        return false;

    return true;

}   // LoadBT_16


/************************************************************************
*    FUNCTION NAME:         LoadFromPointer_32()                                                             
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

unsigned char *CTextureMap::LoadFromPointer_32( unsigned char *pData )
{
    PBMPHeader pBMPHeader;

    // Free the image data
    Free();

    // Get the pointer to the data
    pBMPHeader = PBMPHeader( pData );

    // Read the image width and height
    width = pBMPHeader->width;
    height = pBMPHeader->height;

    // Do a sanity check. Height needs to be within range
    if( height < 1 || height > MAX_TEXTURE_SIZE )
        return false;

    // Just to be save, check the return value.
    if( !SetMultiShift( width ) )
        return false;

    // Allocate memory for image
    pImage32 = new unsigned int [ width * height ];
    
    // Check all allocations
    if( pImage32 == NULL )
        return NULL;

    // inc the pointer
    pData += sizeof( BMPHeader );

    // Read the image:
    memcpy( pImage32, pData, (width * height) * sizeof( unsigned int ) );

    // inc the pointer
    pData += (width * height) * sizeof( unsigned int );

    return pData;

}   // LoadFromPointer_32


/************************************************************************
*    FUNCTION NAME:         LoadFromPointer_16()                                                             
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

unsigned char *CTextureMap::LoadFromPointer_16( unsigned char *pData )
{
    PBMPHeader pBMPHeader;

    // Free the image data
    Free();

    // Get the pointer to the data
    pBMPHeader = PBMPHeader( pData );

    // Read the image width and height
    width = pBMPHeader->width;
    height = pBMPHeader->height;

    // Do a sanity check. Height needs to be within range
    if( height < 1 || height > MAX_TEXTURE_SIZE )
        return false;

    // Just to be save, check the return value.
    if( !SetMultiShift( width ) )
        return false;

    // Allocate memory for image
    pImage16 = new unsigned short int [ width * height ];
    
    // Check all allocations
    if( pImage16 == NULL )
        return NULL;

    // inc the pointer
    pData += sizeof( BMPHeader );

    // Read the image:
    memcpy( pImage16, pData, (width * height) * sizeof( unsigned short int ) );

    // inc the pointer
    pData += (width * height) * sizeof( unsigned short int );

    return pData;

}   // LoadFromPointer_16


/************************************************************************
*    FUNCTION NAME:         SetMultiShift()                                                             
*
*    DESCRIPTION:           Setup the shift value dependant on the texture
*                           width.
*
*    FUNCTION PARAMETERS:
*    Input:    int aWidth - width of bitmap
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CTextureMap::SetMultiShift( int aWidth )
{
    bool result = true;
    
    switch( aWidth )
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

    // Return an error if the width doesn't fall into range
    if( multiShift == 0 )
        result = false;

    return result;

}   // SetMultiShift


/************************************************************************
*    FUNCTION NAME:         Create16BitTextFrom32()                                                             
*
*    DESCRIPTION:           Create a 16 bit texture from a 32 bit texture
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

bool CTextureMap::Create16BitTextFrom32()
{
    bool result = false;
    int length, i, color32, r32, b32, g32;

    // We have to have a 32 bit texture to start with
    if( pImage32 == NULL )
        goto HANDLE_ERROR;

    length = width * height;

    // Allocate memory for image
    pImage16 = new unsigned short int [ width * height ];

    // Check all allocations
    if( pImage16 == NULL )
        goto HANDLE_ERROR;

    // Convert to 16 bit
    for( i = 0; i < length; ++i )
    {
        // Get the color
        color32 = pImage32[i];

        // Bust out the colors
        r32 = (color32 & 0xFF0000) >> 16;
        g32 = (color32 & 0xFF00) >> 8;
        b32 = color32 & 0xFF;

        // Combine it all into a 16 color
        pImage16[i] = ((r32 >> 3) << 10) | ((g32 >> 3) << 5) | (b32 >> 3);
    }

    // If we made it this far, we are OK
    result = true;

HANDLE_ERROR:;   

    return result;

}   // Create16BitTextFrom32