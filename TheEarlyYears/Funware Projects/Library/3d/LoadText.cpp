//
// File name: LoadText.CPP
//
// Description: A BMP image class implementation
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

#include "LoadText.h"
#include "PalShade.h"

// Turn off warnings for deprecated calls
#pragma warning(disable : 4996)

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
LoadTexture::LoadTexture()
{
    Palette = NULL;
    Image = NULL;
    Width = 0;
    Height = 0;
    ColorCount = 0;
    BitCount = 0;
	PreMultShift = 0;
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
LoadTexture::~LoadTexture()
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

void LoadTexture::Free()
{
    FreePalette();

    FreeImage();

}   // Free


/************************************************************************
*    FUNCTION NAME:         FreePalette()                                                             
*
*    DESCRIPTION:           Free allocated pallete
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

void LoadTexture::FreePalette()
{
    if( Palette )
    {
        delete [] Palette;
        Palette = NULL;
    }

}   // FreePalette


/************************************************************************
*    FUNCTION NAME:         FreeImage()                                                             
*
*    DESCRIPTION:           Free allocated bit data
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

void LoadTexture::FreeImage()
{
    if( Image )
    {
        delete [] Image;
        Image = NULL;
    }

}   // FreeImage


/************************************************************************
*    FUNCTION NAME:         Load()                                                             
*
*    DESCRIPTION:           Load the bitmap texture file
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

bool LoadTexture::Load( char *FileName, int textureIndex )
{
    int ImageSize, ScanWidth, ScanWidth24;
    int ScanPadding, ScanPadding24, Y, YPos, i;
    FILE *InFile;
    bool Result = false;
    BITMAPFILEHEADER FileHeader;
    BITMAPINFOHEADER InfoHeader;
    unsigned char *tmpArray = NULL;
	WORD *ptrWORD, color;

    // Free all allocations
    Free();

    if( ( InFile = fopen ( FileName, "rb" ) ) == 0 )
        goto ERROR_JUMP;

    // Load the file header data:
    if( fread ( &FileHeader, sizeof FileHeader, 1, InFile ) < 1 )
        goto ERROR_JUMP;

    // Load the information header data:
    if( fread ( &InfoHeader, sizeof InfoHeader, 1, InFile ) < 1 )
        goto ERROR_JUMP;

    ColorCount = ( int )pow ( 2.0F, InfoHeader.biBitCount );
    BitCount   = InfoHeader.biBitCount;

    // If this is a 24 bit image, say it's a 16 bit image because we are going
    // to convert it to 16 bit color
    if( BitCount == 24 )
        BitCount = 16;

    // A palette is always assumed to be there even if
    // the image is 16 bit. Also, we only need a palette
    // on the first image because if there is 8 bit images here,
    // they are assumed to use the same palette.
    if( textureIndex == 0 )
    {
        if( BitCount == 8 )
        {
            if( ( Palette = new RGBQUAD[ ColorCount ] ) == 0 )
                goto ERROR_JUMP;

            if( fread ( Palette, sizeof( RGBQUAD ), ColorCount, InFile ) < (unsigned int)ColorCount )
                goto ERROR_JUMP;
        }
        else
        {
            // This is a 16 bit image so assume a dummy 256 color palette
            if( ( Palette = new RGBQUAD[ 256 ] ) == 0 )
                goto ERROR_JUMP;

            // If this is a 16 bit image, just clear it out. The palette is not used
            memset( Palette, 0, sizeof( RGBQUAD ) * 256 );
        }
    }
    else if( textureIndex && BitCount == 8 )
    {
        // Zoom past the palette
        fseek( InFile, sizeof( RGBQUAD ) * ColorCount, SEEK_CUR );
    }

    Width         = InfoHeader.biWidth;
    Height        = InfoHeader.biHeight;
    ImageSize     = Width * Height;
    ScanWidth     = ( ( Width * ( BitCount / 8 ) ) + 3 ) & ( ~3 );
    ScanPadding   = ScanWidth - ( Width * ( BitCount / 8 ) );
    ScanWidth24   = ( ( Width * ( InfoHeader.biBitCount / 8 ) ) + 3 ) & ( ~3 );
    ScanPadding24 = ScanWidth24 - ( Width * ( InfoHeader.biBitCount / 8 ) );

    if( ( Image = new unsigned char [ ImageSize * ( BitCount / 8 ) ] ) == NULL )
        goto ERROR_JUMP;

    // If this is 24 bit color that we need to convert, allocate a tempoary array
    // to use as a way to read in the color to do the conversions
    if( InfoHeader.biBitCount == 24 )
    {       
        if( ( tmpArray = new unsigned char [ ScanWidth24 ] ) == NULL )
            goto ERROR_JUMP;

        // Use a word pointer to index into the array
		ptrWORD = (WORD *)Image;
    }

    // Load the bitmap:
    for( Y = 0; Y < abs( Height ); ++Y )
    {
        // A bitmap can be saved top to bottom or bottom to top.
        // If this is a top to bottom image, the height is a negative number
        if( Height < 0 )
            YPos = Y * ScanWidth;
        else
            YPos = ( ( Height - Y ) * ScanWidth ) - ScanWidth;

        // If 24 bit color, read in and convert
        if( InfoHeader.biBitCount == 24 )
        {
            if( fread( tmpArray, ScanWidth24, 1, InFile ) < 1 )
                goto ERROR_JUMP;

            // Conver the RGB scan line to 16 bit color word
            for( i = 0; i < Width; ++i )
            {
                // Get the blue color, convert it and save it
				color = tmpArray[ (i * RGB_COUNT) ];
				ptrWORD[ (YPos >> 1) + i ] = (color >> 3);

				// Get the green color, convert it and "OR" it
				color = tmpArray[ (i * RGB_COUNT) + 1 ];
				// With a mask, zero out the first 3 bits then you only	have to shift
				// it 2 times. Same as ((color / 8) << 5) or ((color >> 3) << 5) but much faster
				ptrWORD[ (YPos >> 1) + i ] |= ((color & 0xFFF8) << 2);

				// Get the red color, convert it and "OR" it
				color = tmpArray[ (i * RGB_COUNT) + 2 ];
				// With a mask, zero out the first 3 bits then you only	have to shift
				// it 7 times. Same as ((color / 8) << 10) or ((color >> 3) << 10) but much faster
				ptrWORD[ (YPos >> 1) + i ] |= ((color & 0xFFF8) << 7);
            }

            // Skip the padding possibly located at the end of each
            // scan-line:
            if( ScanPadding24 )
                fseek( InFile, ScanPadding24, SEEK_CUR );
        }
        else
        {
            if( fread( &Image[ YPos ], ScanWidth, 1, InFile ) < 1 )
                goto ERROR_JUMP;

            // Skip the padding possibly located at the end of each
            // scan-line:
            if( ScanPadding )
                fseek( InFile, ScanPadding, SEEK_CUR );
        }
    }

    Height = abs( Height );

    // Do the PreMultShift now so it doesn't have to be done later
    switch( Width )
    {
        case ( 2 ):   PreMultShift = 1; break;
        case ( 4 ):   PreMultShift = 2; break;
        case ( 8 ):   PreMultShift = 3; break;
        case ( 16 ):  PreMultShift = 4; break;
        case ( 32 ):  PreMultShift = 5; break;
        case ( 64 ):  PreMultShift = 6; break;
        case ( 128 ): PreMultShift = 7; break;
        case ( 256 ): PreMultShift = 8; break;
		default:
			goto ERROR_JUMP;
    }

    // If we made it this far, we are OK
    Result = true;

    ERROR_JUMP:;

    // close the file if it has been opened
    if( InFile )
        fclose( InFile );
        
    if( tmpArray )
        delete [] tmpArray;   

    return Result;

}   // Load


/************************************************************************
*    FUNCTION NAME:         AllocateTextureSpace()                                                             
*
*    DESCRIPTION:           Allocate the texture space
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

bool LoadTexture::AllocateTextureSpace( int W, int H, int bit, int textureIndex )
{
	bool Result = false;

	// init the class variables
	BitCount = bit;
    Width = W;
    Height = H;
	ColorCount = 256; // Just assume a 256 color palette

    // Allocate the image space
    if( ( Image = new unsigned char [ (Width * Height) * ( BitCount / 8 ) ] ) == NULL )
        goto ERROR_JUMP;

    // A palette is always. Also, we only need a palette
    // on the first image because if there is 8 bit images here,
    // they are assumed to use the same palette.
    if( textureIndex == 0 )
    {
        if( ( Palette = new RGBQUAD[ ColorCount ] ) == 0 )
            goto ERROR_JUMP;
    }

    // Do the PreMultShift now so it doesn't have to be done later
    switch( Width )
    {
        case ( 2 ):   PreMultShift = 1; break;
        case ( 4 ):   PreMultShift = 2; break;
        case ( 8 ):   PreMultShift = 3; break;
        case ( 16 ):  PreMultShift = 4; break;
        case ( 32 ):  PreMultShift = 5; break;
        case ( 64 ):  PreMultShift = 6; break;
        case ( 128 ): PreMultShift = 7; break;
        case ( 256 ): PreMultShift = 8; break;
		default:
			goto ERROR_JUMP;
    }

    // If we made it this far, we are OK
    Result = true;

    ERROR_JUMP:;

    return Result;

}	// AllocateTextureSpace


/************************************************************************
*    FUNCTION NAME:         SaveBT()                                                             
*
*    DESCRIPTION:           Save native bitmap texture format file
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

bool LoadTexture::SaveBT( FILE *OutFile )
{
    // Write the image width:
    if( fwrite( &Width, sizeof Width, 1, OutFile ) < 1 )
        return false;

    // Write the image height:
    if( fwrite( &Height, sizeof Height, 1, OutFile ) < 1 )
        return false;

    // Write the image's bit count per pixel:
    if( fwrite( &BitCount, sizeof BitCount, 1, OutFile ) < 1 )
        return false;

    // Write the image's color count:
    if( fwrite( &ColorCount, sizeof ColorCount, 1, OutFile ) < 1 )
        return false;

    // Write the image:
    if( fwrite( Image, Width * Height * ( BitCount / 8 ), 1, OutFile ) < 1 )
        return false;

    return true;

}   // SaveBT


/************************************************************************
*    FUNCTION NAME:         LoadBT()                                                             
*
*    DESCRIPTION:           Load native bitmap texture format file
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

bool LoadTexture::LoadBT( FILE *InFile )
{
    // Free the image data
    FreeImage();

    // Read the image width:
    if( fread( &Width, sizeof Width, 1, InFile ) < 1 )
        return false;

    // Read the image height:
    if( fread( &Height, sizeof Height, 1, InFile ) < 1 )
        return false;

    // Read the image's bit count per pixel:
    if( fread( &BitCount, sizeof BitCount, 1, InFile ) < 1 )
        return false;

    // Read the image's color count:
    if( fread( &ColorCount, sizeof ColorCount, 1, InFile ) < 1 )
        return false;

    // Allocate memory for image:
    if( ( Image = new unsigned char [ (Width * Height) * ( BitCount / 8 ) ] ) == NULL )
        return false;

    // Read the image:
    if( fread( Image, (Width * Height) * ( BitCount / 8 ), 1, InFile ) < 1 )
        return false;

    // Do the PreMultShift now so it doesn't have to be done later
    switch( Width )
    {
        case ( 2 ):   PreMultShift = 1; break;
        case ( 4 ):   PreMultShift = 2; break;
        case ( 8 ):   PreMultShift = 3; break;
        case ( 16 ):  PreMultShift = 4; break;
        case ( 32 ):  PreMultShift = 5; break;
        case ( 64 ):  PreMultShift = 6; break;
        case ( 128 ): PreMultShift = 7; break;
        case ( 256 ): PreMultShift = 8; break;
		default:
			goto ERROR_JUMP;
    }

    return true;

	ERROR_JUMP:;

	return false;

}   // LoadBT


/************************************************************************
*    FUNCTION NAME:         LoadBT()                                                             
*
*    DESCRIPTION:           Load native bitmap texture format file
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

unsigned char *LoadTexture::LoadBTfromPtr( unsigned char *pBTdata )
{
    PBMPHeader pBMPHeader;

    // Free the image data
    FreeImage();

    // Get the pointer to the data
    pBMPHeader = PBMPHeader( pBTdata );

    // Read the image width:
    Width = pBMPHeader->Width;

    // Read the image height:
    Height = pBMPHeader->Height;

    // Read the image's bit count per pixel:
    BitCount = pBMPHeader->BitCount;

    // Read the image's color count:
    ColorCount = pBMPHeader->ColorCount;

    // Allocate memory for image:
    if( ( Image = new unsigned char [ Width * Height * ( BitCount / 8 ) ] ) == NULL )
        return false;

    // inc the pointer
    pBTdata += sizeof( BMPHeader );

    // Read the image:
    memcpy( Image, pBTdata, (Width * Height) * ( BitCount / 8 ) );

    // inc the pointer
    pBTdata += (Width * Height) * ( BitCount / 8 );

    // Do the PreMultShift now so it doesn't have to be done later
    switch( Width )
    {
        case ( 2 ):   PreMultShift = 1; break;
        case ( 4 ):   PreMultShift = 2; break;
        case ( 8 ):   PreMultShift = 3; break;
        case ( 16 ):  PreMultShift = 4; break;
        case ( 32 ):  PreMultShift = 5; break;
        case ( 64 ):  PreMultShift = 6; break;
        case ( 128 ): PreMultShift = 7; break;
        case ( 256 ): PreMultShift = 8; break;
    }

    return pBTdata;

}   // LoadBTfromPtr


/************************************************************************
*    FUNCTION NAME:         SavePal()                                                             
*
*    DESCRIPTION:           Save the texture file
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

bool LoadTexture::SavePal( FILE *OutFile )
{
    // Write the color count:
    if( fwrite ( &ColorCount, sizeof ColorCount, 1, OutFile ) < 1 )
        return false;

    // Write the bit count:
    if( fwrite ( &BitCount, sizeof BitCount, 1, OutFile ) < 1 )
        return false;

    // For 16 bit images, fake a 256 color palette
    if( BitCount == 16 )
    {
        // Write the palette:
        if( fwrite ( Palette, sizeof ( RGBQUAD ), 256, OutFile ) < 1 )
            return false;
    }
    else
    {
        // Write the palette:
        if( fwrite ( Palette, sizeof ( RGBQUAD ), ColorCount, OutFile ) < 1 )
            return false;
    }

    return true;

}   // SavePal


/************************************************************************
*    FUNCTION NAME:         LoadPal()                                                             
*
*    DESCRIPTION:           Load the palette
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

bool LoadTexture::LoadPal( FILE *InFile )
{
    // Free the palette
    FreePalette();

    // Load the color count:
    if( fread ( &ColorCount, sizeof ColorCount, 1, InFile ) < 1 )
        return false;

    // Load the bit count:
    if( fread ( &BitCount, sizeof BitCount, 1, InFile ) < 1 )
        return false;

    // For 16 bit images, fake a 256 color palette
    if( BitCount == 16 )
    {
        // Allocate memory for palette:
        if( ( Palette = new RGBQUAD [ 256 ] ) == 0 )
            return false;

        // Load the palette:
        if( fread ( Palette, sizeof ( RGBQUAD ), 256, InFile ) < 256 )
            return false;
    }
    else
    {
        // Allocate memory for palette:
        if( ( Palette = new RGBQUAD [ ColorCount ] ) == 0 )
            return false;

        // Load the palette:
        if( fread ( Palette, sizeof ( RGBQUAD ), ColorCount, InFile ) < (unsigned int)ColorCount )
            return false;
    }

    return true;

}   // LoadPal


/************************************************************************
*    FUNCTION NAME:         LoadPalfromPtr()                                                             
*
*    DESCRIPTION:           Load the palette from a pointer
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

unsigned char *LoadTexture::LoadPalfromPtr( unsigned char *pBTdata )
{
    PPaletteHeader pPaletteHeader;

    // Free the palette
    FreePalette();

    // Get the bitmap info
    pPaletteHeader = PPaletteHeader( pBTdata );

    // Load the color count:
    ColorCount = pPaletteHeader->ColorCount;

    // Load the bit count:
    BitCount = pPaletteHeader->BitCount;

    // inc the pointer
    pBTdata += sizeof( PaletteHeader );

    // For 16 bit images, fake a 256 color palette
    if( BitCount == 16 )
    {
        // Allocate memory for palette:
        if( ( Palette = new RGBQUAD [ 256 ] ) == 0 )
            return false;

        // Load the palette:
        memcpy( Palette, pBTdata, sizeof( RGBQUAD ) * 256 );

        // inc the pointer
        pBTdata += sizeof( RGBQUAD ) * 256;
    }
    else
    {
        // Allocate memory for palette:
        if( ( Palette = new RGBQUAD [ ColorCount ] ) == 0 )
            return false;

        // Load the palette:
        memcpy( Palette, pBTdata, sizeof( RGBQUAD ) * ColorCount );

        // inc the pointer
        pBTdata += sizeof( RGBQUAD ) * ColorCount;
    }

    return pBTdata;

}   // LoadPalfromPtr