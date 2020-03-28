
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

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include "GDefines.h"        // Header file with #defines
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.

#include "dib.h"             // Header file for this *.cpp file.

#include "minTools.h"        // A misc group of useful functions



/************************************************************************
*
*                         TDIB Object
*
************************************************************************/



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

TDIB::TDIB()
{
    // Null out all important variables
    hLoad = NULL;
    hPalette = NULL;
    Fdib = NULL;
    pBitData = NULL;
    FRasterOp = SRCCOPY;
    hDeviceDC = NULL;
    PointerType = USE_RESOURCE_POINTER;
    ObjectType = BITMAP_OBJECT;
    ObjectDataSizeInBytes = 0;
    BufferSize = 0;
	fTrans = -1;
}


/************************************************************************
*    FUNCTION NAME:         getDIBcolors()                                                             
*
*    DESCRIPTION:           Returns the color bit depth
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

int TDIB::getDIBcolors()
{
    if (Fdib->bmiHeader.biClrUsed==0)
    {
        switch (Fdib->bmiHeader.biBitCount)
        {
            case BIT_COUNT_1: {return 2;}
            case BIT_COUNT_4: {return 16;}
            case BIT_COUNT_8: {return 256;}
            case BIT_COUNT_16: {return 0;} // no color table
            case BIT_COUNT_24: {return 0;} // no color table
            case BIT_COUNT_32: {return 0;} // no color table
        }
    }
    return (int)Fdib->bmiHeader.biClrUsed;

}   // getDIBcolors


/************************************************************************
*    FUNCTION NAME:         LoadFromResource()                                                             
*
*    DESCRIPTION:           Load bitmap from resource file.
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst   - Program Instance
*              char *name        - Name of the resource
*              char *type        - Resource Type Id
*              int PointerType   - Flag to hold resource pointer or
*                                  allocate memory for the bitmap
*              The last two parameters are needed for TBitmap and are not used here
*              BOOL BitmapType   - Determines if it is a standard BMP or 
*                                  a DIB Section
*              int BitCount - Sets the type of bitmap you plan to use.
*                             Default to 8 bit because it's faster than 24 bit
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TDIB::LoadFromResource( HINSTANCE hInst, char *name, char *type )
{
    TLoadRes LoadRes;
    BYTE *pBits;
    BOOL Result = FALSE;

    Free(); // Free memory if need be

    if( !LoadTheResource( hInst, name, type, &LoadRes ) )
        goto HANDLE_ERROR;

    pBits = (BYTE *)LoadRes.pData;

    // Save the size of the file for later use
    ObjectDataSizeInBytes = LoadRes.Size;

    // Check to see if we have the file header
    // True BMP resource files loose their headers when
    // compiled into a resource
    if( type != RT_BITMAP )
    {
        // Set pointer past file header
        pBits += sizeof( BITMAPFILEHEADER );
        LoadRes.Size -= sizeof( BITMAPFILEHEADER );

        // Is this a true BMP file?
        if( ((BITMAPFILEHEADER *)LoadRes.pData)->bfType != BITMAP_TYPE )          
        {
            PostMsg( "Error", "File format unknown for resource. (%s) is not a valid BMP.", name );
            goto HANDLE_ERROR;
        }
    }

    // We can choose to alloc our own memory to store the
    // bitmap or use the pointer Windows has allocated for
    // us. The only difference is that we can't write to
    // the Windows resource pointer. If your not planning
    // on changing the bitmap during program runtime then there
    // is not much need to allocate the memory.
    if( PointerType == ALLOC_MEM_FOR_BITMAP )
    {
        // Allocate memory for our DIB
        pBitData = new BYTE[ LoadRes.Size ];
        if( pBitData == NULL )
        {
            PostMsg( "Error", "Unable to allocate memory for resource (%s).", name );
            
            goto HANDLE_ERROR;
        }

        // Copy contents of resource into allocated memory
        memcpy( pBitData, pBits, LoadRes.Size );

        // Copy pointer
        Fdib = (BITMAPINFO *)pBitData;
    }
    else if( PointerType == USE_RESOURCE_POINTER )
    {
        Fdib = (BITMAPINFO *)pBits;
    }

    Result = TRUE;

HANDLE_ERROR:;

    if( !Result )
        ObjectDataSizeInBytes = 0;

    return Result;

} // LoadFromResource


/************************************************************************
*    FUNCTION NAME:         LoadFromFile()                                                             
*
*    DESCRIPTION:           Load bitmap from file
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName    - Path to file name
*              The last two parameters are needed for TBitmap and are not used here
*              BOOL BitmapType   - Determines if it is a standard BMP or 
*                                  a DIB Section
*              int BitCount - Sets the type of bitmap you plan to use.
*                             Default to 8 bit because it's faster than 24 bit
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TDIB::LoadFromFile( char *FileName, BOOL showErrorMsg )
{ 
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long bytesRead;
    BITMAPFILEHEADER *BMPfHead;
    HANDLE hBmpFile;
    DWORD size;
    BOOL ReadResult;
    BOOL Result = FALSE;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    Free(); // Free memory if need be

    // Open the file on the hard drive
    hBmpFile = CreateFile( FileName, GENERIC_READ, FILE_SHARE_READ,
                           &FileSecurity, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hBmpFile == INVALID_HANDLE_VALUE )
    {
        if( showErrorMsg )
            PostMsg( "Error", "Can't open bitmap file (%s) to display.", FileName );

        goto HANDLE_ERROR;
    }

    // Get the file size of the bitmap so that we can allocate enough memory for it
    size = GetFileSize( hBmpFile, NULL );

    // Save the size of the file for later use
    ObjectDataSizeInBytes = size;

    // Allocate memory for bitmap
    pBitData = new BYTE[ size ];
    if( pBitData == NULL )
    {
        if( showErrorMsg )
            PostMsg( "Error", "Unable to allocate memory for bitmap file (%s).", FileName );

        goto HANDLE_ERROR;
    }

    // Read contents of file into allocated memory
    ReadResult = ReadFile( hBmpFile, pBitData, size, &bytesRead, NULL );

    if( !ReadResult || bytesRead != size )
    {
        if( showErrorMsg )
            PostMsg( "Error", "Unable to load bitmap (%s).", FileName );

        goto HANDLE_ERROR;
    }

    BMPfHead = (BITMAPFILEHEADER *)pBitData;

    // Is this a true BMP file?
    if( BMPfHead->bfType != BITMAP_TYPE )          
    {
        if( showErrorMsg )
            PostMsg( "Error", "File format unknown, file (%s) is not a valid BMP.", FileName );

        goto HANDLE_ERROR;
    }

    // Check the file size
    if( BMPfHead->bfSize > GetFileSize( hBmpFile, NULL ) )
    {
        if( showErrorMsg )
            PostMsg( "Error", "File size too small, the bitmap file (%s) could be damaged.", FileName );

        goto HANDLE_ERROR;
    }

    // Copy pointer
    Fdib = (BITMAPINFO *)(pBitData + sizeof( BITMAPFILEHEADER ));

    Result = TRUE;

HANDLE_ERROR:;

    if( !Result )
    {
        ObjectDataSizeInBytes = 0;

        // Delete the allocated data
        DeleteBitmapBits();
    }

    if( hBmpFile )
        CloseHandle( hBmpFile );

    return Result;

} // LoadFromFile


/************************************************************************
*    FUNCTION NAME:         GetBitmapType()                                                             
*
*    DESCRIPTION:           Returns the bitmap type.
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

int TDIB::GetBitmapType()
{
    int result = NO_TYPE;

    // Make sure we have a valid pointer
    if( Fdib )
    {
        if( Fdib->bmiHeader.biHeight > 0 )
            result = BOTTOM_UP_BMP;

        else if( Fdib->bmiHeader.biHeight < 0 )
            result = TOP_DOWN_BMP;
    }

    return result;

}   // GetBitmapType


/************************************************************************
*    FUNCTION NAME:         deletePalete()                                                             
*
*    DESCRIPTION:           Deletes the palette
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TDIB::DeletePalete()
{
    if( hPalette != NULL )
    {
        DeleteObject( hPalette );
        hPalette = NULL;
    }
}   // DeletePalete


/************************************************************************
*    FUNCTION NAME:         DeleteBitmapBits()                                                             
*
*    DESCRIPTION:           Deletes the allocated bitmap data
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TDIB::DeleteBitmapBits()
{
    // We may have received a memory handle from an outside source
    // This must be in front because the pBitData pointer could
    // be set to the handle
    if( hLoad )
    {
        GlobalUnlock( hLoad );
        GlobalFree( hLoad );
        hLoad = NULL;
        Fdib = NULL;
        pBitData = NULL;
    }

    // We allocated this memory and now it has to be freed
    if( pBitData )
    {
        delete [] pBitData;
        pBitData = NULL;
        Fdib = NULL;
    }

}   // DeleteBitmapBits


/************************************************************************
*    FUNCTION NAME:         Palette()                                                             
*
*    DESCRIPTION:           Creates the palette
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

HPALETTE TDIB::GetPalette()
{
    // Check to see if the palette is NULL
    // before doing it all over again
    if( hPalette || IsEmpty() )
        return hPalette;

    int i, colors;
    PTLogpalette pal;

    colors = getDIBcolors();

    if( colors > 0 )
    {
        pal = new TLogpalette;

        if( pal )
        {
            // Set version number and color count
            pal->palVersion = 0x0300;
            pal->palNumEntries = (WORD)colors;

            // Set values into palette entries
            for( i = 0; i < colors; ++i )
            {
                pal->palPalEntry[i].peRed   = Fdib->bmiColors[i].rgbRed;
                pal->palPalEntry[i].peGreen = Fdib->bmiColors[i].rgbGreen;
                pal->palPalEntry[i].peBlue  = Fdib->bmiColors[i].rgbBlue;
                pal->palPalEntry[i].peFlags = 0;
            }

            // Create the palette
            hPalette = CreatePalette( (LPLOGPALETTE)pal );

            delete pal;
        }
    }

    return hPalette;

}   // GetPalette


/************************************************************************
*    FUNCTION NAME:         Free                                                             
*
*    DESCRIPTION:           Free allocated memory and palette
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TDIB::Free()
{
    DeleteBitmapBits();

    DeletePalete();

	// reset the transparence flag
	fTrans = -1;

}  // Free


/************************************************************************
*    FUNCTION NAME:         IsEmpty()                                                             
*
*    DESCRIPTION:           Do we have a graphic sitting in here?
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TDIB::IsEmpty()
{
    BOOL Result = TRUE;

    if( Fdib )
        Result = FALSE;

    return Result;

}   // IsEmpty


/************************************************************************
*    FUNCTION NAME:         ConvertToMask()                                                             
*
*    DESCRIPTION:           To convert this image to a mask, we are changing 
*                           the palette entries.  All we need to do is change 
*                           the first palette entry to RGB white and make all
*                           other palette entries RGB black. We are making
*                           the assumption that our transparent color is the
*                           first palete entry.
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TDIB::ConvertToMask()
{
    int i, PaletteSize;

    if( Fdib )
    {
        // How many entries are in our palette
        PaletteSize = getDIBcolors();

        for( i = 1; i < PaletteSize; ++i )
        {
            SetRGBred( (BYTE)i, 0 );
            SetRGBgreen( (BYTE)i, 0 );
            SetRGBblue( (BYTE)i, 0 );
        }

        // Set the first element in the palette to White
        SetRGBred( 0, 0xFF );
        SetRGBgreen( 0, 0xFF );
        SetRGBblue( 0, 0xFF );

        // Set the copy mode to paint
        FRasterOp = SRCAND;
    }

}   // ConvertToMask


/************************************************************************
*    FUNCTION NAME:         LoadFromHandle                                                             
*
*    DESCRIPTION:           Get a handle to a memory block. An easy way to
*                           give one bitmaps memory to another bitmap
*
*    FUNCTION PARAMETERS:
*    Input:    HANDLE handle - Handle to a memory block
*              BOOL BitmapType   - Determines if it is a standard BMP or 
*                                  a DIB Section
*              int BitCount - Sets the type of bitmap you plan to use.
*                             Default to 8 bit because it's faster than 24 bit
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
// Removed because it is no longer used. Replaced by LoadFromBuffer
/*BOOL TDIB::LoadFromHandle( HANDLE handle, BOOL BitmapType, int BitCount )
{
    BITMAPFILEHEADER *BMPfHead;

    Free(); // Free memory if need be

    // Check the handle to make sure it is valid
    if( handle > 0 )
        hLoad = handle;
    else
    {
        MessageBox( GetActiveWindow(), "Invalid memory handle", "Error", MB_ICONWARNING );
        return FALSE;
    }

    // Lock down our memory
    pBitData = (BYTE *)GlobalLock( handle );

    // Convert to a BITMAPFILEHEADER to check if it is a true BMP
    BMPfHead = (BITMAPFILEHEADER *)pBitData;

    if( BMPfHead->bfType == BITMAP_TYPE )
    {
        // Copy pointer
        Fdib = (BITMAPINFO *)(pBitData + sizeof( BITMAPFILEHEADER ));
    }
    else
    {
        MessageBox( GetActiveWindow(), "Can't lock memory handle", "Error", MB_ICONWARNING );
        hLoad = 0;
        return FALSE;
    }

    Fdib = (BITMAPINFO *)pBitData;  // Copy pointer

    return TRUE;

}  // LoadFromHandle */


/************************************************************************
*    FUNCTION NAME:         LoadFromBuffer                                                             
*
*    DESCRIPTION:           Get a pointer to a memory block. An easy way to
*                           give one bitmaps memory to another bitmap
*
*    FUNCTION PARAMETERS:
*    Input:    BYTE *pBuffer - Handle to a memory block
*              The last two parameters are needed for TBitmap and are not used here
*              BOOL BitmapType   - Determines if it is a standard BMP or 
*                                  a DIB Section
*              int BitCount - Sets the type of bitmap you plan to use.
*                             Default to 8 bit because it's faster than 24 bit
*
*    Output:   BOOL - TRUE if successful, FALSE on error
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TDIB::LoadFromBuffer( BYTE *pBuffer, DWORD Size )
{
    BOOL Result = FALSE;
    BITMAPFILEHEADER *BMPfHead;

    Free(); // Free memory if need be

    // Copy pointer
    if( pBuffer )
    {
        // Record the buffer size
        BufferSize = Size;

        // Cast the pointer as a bitmap file header
        BMPfHead = (BITMAPFILEHEADER *)pBuffer;

        // Save the pointer so this data can be deleted
        pBitData = pBuffer;

        if( BMPfHead->bfType == BITMAP_TYPE )
        {
            // Copy pointer
            Fdib = (BITMAPINFO *)(pBitData + sizeof( BITMAPFILEHEADER ));

            // Save the size of the file for later use
            ObjectDataSizeInBytes = Size;

            Result = TRUE;
        }
        else
        {
            MessageBox( GetActiveWindow(), "Image not of type bitmap", "Error", MB_ICONWARNING );
            Free(); // Free memory if need be
        }
    }

    return Result;

}   // LoadFromBuffer


/************************************************************************
*    FUNCTION NAME:         BlitToDC                                                             
*
*    DESCRIPTION:           A quick down and dirty way to blit to a DC
*
*    FUNCTION PARAMETERS:
*    Input:    HDC hDestDC - Handle to a device context
*              int X - X coordinate
*              int Y - Y coordinate
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TDIB::BlitToDC( HDC hDestDC, int X, int Y )
{
    HPALETTE hOldPalette = NULL;

    // Check to see that it's not empty
    if( !IsEmpty() )
    {
        // You have to realize the palette each time you get a WM_PAINT message
        // or the bitmap will be drawn without it's colors. This does not apply
        // if the computer is in a higher color depth than 256.
        if( GetDeviceCaps( hDestDC, BITSPIXEL ) <= 8 )
        {
            // Save the old palette so that we can select it back in before
            // deleting the palette we created. DeleteObject doesn't seem to
            // work if your trying to delete a palette that is still in the DC
            hOldPalette = SelectPalette( hDestDC, GetPalette(), FALSE );                 
            RealizePalette( hDestDC );
        }
 
        // API call that uses the BITMAPINFO structure and a pointer to the bitmaps
        // bits to blit the image to the window. This is not the fastest way to blit
        // images and wouldn't be my first choice for animation, but for this purpose,
        // it works fine. Even though this function is called StretchDIBits, we are not
        // doing any stretching here.
        StretchDIBits( hDestDC, X, Y, width(), height(),
                       0, 0, width(), height(), bits(), dib(), DIB_RGB_COLORS, FRasterOp );

        // Reselect the old palette back into the DC so that the palette can be deleted
        if( hOldPalette )
        {
            SelectPalette( hDestDC, hOldPalette, FALSE );                
            //RealizePalette( hDestDC );  dont need to realise the old palette
        }
    }

}   // BlitToDC


/************************************************************************
*    FUNCTION NAME:         BlitToDC                                                             
*
*    DESCRIPTION:           A quick down and dirty way to blit to a DC
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd - Handle to a device context
*              int X - X coordinate
*              int Y - Y coordinate
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TDIB::BlitToDC( HWND hwnd, int X, int Y )
{
    HDC DestHDC;

    // Get the DC from the supplied handle
    DestHDC = GetDC( hwnd );

    // Blit it
    BlitToDC( DestHDC, X, Y );

    // Release the handle
    ReleaseDC( hwnd, DestHDC  );

}   // BlitToDC


/************************************************************************
*    FUNCTION NAME:         BlitToDC                                                             
*
*    DESCRIPTION:           Blit the bits to a TBitmap
*
*    FUNCTION PARAMETERS:
*    Input:    HDC hDestDC - Handle to a device context
*              RECT *Dest - Blit to a destination rect to the supplied DC
*              RECT *Source - Specify a rect to blit from. At this point,
*              the bitmap is a bottom to top DIB so it is upside down. Just
*              so you know.
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TDIB::BlitToDC( HDC hDestDC, RECT *Dest, RECT *Source )
{
    HPALETTE hOldPalette = NULL;

    // Check to see that it's not empty
    if( !IsEmpty() )
    {
        // You have to realize the palette each time you get a WM_PAINT message
        // or the bitmap will be drawn without it's colors. This does not apply
        // if the computer is in a higher color depth than 256.
        if( GetDeviceCaps( hDestDC, BITSPIXEL ) <= 8 )
        {
            // Save the old palette so that we can select it back in before
            // deleting the palette we created. DeleteObject doesn't seem to
            // work if your trying to delete a palette that is still in the DC
            hOldPalette = SelectPalette( hDestDC, GetPalette(), FALSE );                 
            RealizePalette( hDestDC );
        }

        // API call that uses the BITMAPINFO structure and a pointer to the bitmaps
        // bits to blit the image to the window. This is not the fastest way to blit
        // images and wouldn't be my first choice for animation, but for this purpose,
        // it works fine.
        StretchDIBits( hDestDC, Dest->left, Dest->top, Dest->right, Dest->bottom,
                       Source->left, Source->top, Source->right, Source->bottom, 
                       bits(), dib(), DIB_RGB_COLORS, FRasterOp );

        // Reselect the old palette back into the DC so that the palette can be deleted
        if( hOldPalette )
        {
            SelectPalette( hDestDC, hOldPalette, FALSE );                
            //RealizePalette( hDestDC );    dont need to realise the old palette
        }
    }
}   // BlitToDC


/************************************************************************
*    FUNCTION NAME:         BlitToDC                                                             
*
*    DESCRIPTION:           A quick down and dirty way to blit to a DC
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd - Handle to a device context
*              int X - X coordinate
*              int Y - Y coordinate
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TDIB::BlitToDC( HWND hwnd, RECT *Dest, RECT *Source )
{
    HDC DestHDC;

    // Get the DC from the supplied handle
    DestHDC = GetDC( hwnd );

    // Blit it
    BlitToDC( DestHDC, Dest, Source );

    // Release the handle
    ReleaseDC( hwnd, DestHDC  );

}   // BlitToDC


/************************************************************************
*    FUNCTION NAME:         BlitBits                                                             
*
*    DESCRIPTION:           Blit the bits to a TBitmap
*
*    FUNCTION PARAMETERS:
*    Input:    HDC hDestDC - Handle to a device context
*              RECT *Dest - Blit to a destination rect to the supplied DC
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TDIB::BlitToDC( HDC hDestDC, RECT *Dest )
{
    HPALETTE hOldPalette = NULL;

    // Check to see that it's not empty
    if( !IsEmpty() )
    {
        // You have to realize the palette each time you get a WM_PAINT message
        // or the bitmap will be drawn without it's colors. This does not apply
        // if the computer is in a higher color depth than 256.
        if( GetDeviceCaps( hDestDC, BITSPIXEL ) <= 8 )
        {
            // Save the old palette so that we can select it back in before
            // deleting the palette we created. DeleteObject doesn't seem to
            // work if your trying to delete a palette that is still in the DC
            hOldPalette = SelectPalette( hDestDC, GetPalette(), FALSE );
            RealizePalette( hDestDC );
        }

        // API call that uses the BITMAPINFO structure and a pointer to the bitmaps
        // bits to blit the image to the window. This is not the fastest way to blit
        // images and wouldn't be my first choice for animation, but for this purpose,
        // it works fine.
        StretchDIBits( hDestDC, Dest->left, Dest->top, Dest->right, Dest->bottom,
                       0, 0, width(), height(), 
                       bits(), dib(), DIB_RGB_COLORS, FRasterOp );

        // Reselect the old palette back into the DC so that the palette can be deleted
        if( hOldPalette )
        {
            SelectPalette( hDestDC, hOldPalette, FALSE );
            //RealizePalette( hDestDC );  dont need to realise the old palette
        }
    }
}   // BlitToDC


/************************************************************************
*    FUNCTION NAME:         BlitToDC                                                             
*
*    DESCRIPTION:           A quick down and dirty way to blit to a DC
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd - Handle to a device context
*              int X - X coordinate
*              int Y - Y coordinate
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TDIB::BlitToDC( HWND hwnd, RECT *Dest )
{
    HDC DestHDC;

    // Get the DC from the supplied handle
    DestHDC = GetDC( hwnd );

    // Blit it
    BlitToDC( DestHDC, Dest );

    // Release the handle
    ReleaseDC( hwnd, DestHDC  );

}   // BlitToDC


/************************************************************************
*    FUNCTION NAME:         GetPixelsPerInchX                                                             
*
*    DESCRIPTION:           Get the pixels per inch of this bitmap
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int TDIB::GetPixelsPerInchX()
{ 
    double temp1, temp2, Result = 72;

    // Check that we have a valid pointer and the value is more than zero
    if( Fdib && Fdib->bmiHeader.biXPelsPerMeter )
    {
        // Convert these values to floating point variables
        temp2 = CONVERT_TO_PIXELS;
        temp1 = Fdib->bmiHeader.biXPelsPerMeter;
        Result = temp1 / temp2;
    }

    return (int)Result;

}   // GetPixelsPerInchX


/************************************************************************
*    FUNCTION NAME:         GetPixelsPerInchY                                                             
*
*    DESCRIPTION:           Get the pixels per inch of this bitmap
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int TDIB::GetPixelsPerInchY()
{ 
    double temp1, temp2, Result = 72;

    // Check that we have a valid pointer and the value is more than zero
    if( Fdib && Fdib->bmiHeader.biYPelsPerMeter )
    {
        // Convert these values to floating point variables
        temp2 = CONVERT_TO_PIXELS;
        temp1 = Fdib->bmiHeader.biYPelsPerMeter;
        Result = temp1 / temp2;
    }

    return (int)Result;

}   // GetPixelsPerInchY


/************************************************************************
*    FUNCTION NAME:         TakeDIBpointer                                                             
*
*    DESCRIPTION:           Get a pointer to a memory block. An easy way to
*                           give one bitmaps memory to another bitmap. But
*                           this function then no longer lays clame to the
*                           data.
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BYTE *TDIB::TakeDIBpointer()
{
    BYTE *Result = pBitData;

    // Null out all important variables
    hLoad = NULL;
    Fdib = NULL;
    pBitData = NULL;

    Free(); // Free memory if need be

    return Result;

}   // TakeDIBpointer


/************************************************************************
*    FUNCTION NAME:         ConvertToSprite()                                                             
*
*    DESCRIPTION:           To convert this image to a sprite, we are changing 
*                           the palette entries.  All we need to do is change 
*                           the first palette entry to RGB black. We are making
*                           the assumption that our transparent color is the
*                           first palete entry.
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TDIB::ConvertToSprite()
{
    if( Fdib )
    {
        // Set the first element in the palette to black
        SetRGBred( 0, 0 );
        SetRGBgreen( 0, 0 );
        SetRGBblue( 0, 0 );

        // Set the copy mode to paint
        FRasterOp = SRCPAINT;
    }

}   // ConvertToSprite


/************************************************************************
*    FUNCTION NAME:         operator                                                             
*
*    DESCRIPTION:           Copy one bitmap to another.
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TDIB::operator = ( TDIB &DibToCopy )
{
    Free(); // Free memory if need be

    // Save the size of the file for later use
    ObjectDataSizeInBytes = DibToCopy.GetObjectSizeInBytes();

    // Make sure we have something to copy
    if( ObjectDataSizeInBytes && DibToCopy.dib() )
    {
        // Allocate memory for bitmap
        pBitData = new BYTE[ ObjectDataSizeInBytes ];
        if( pBitData == NULL )
        {
            PostMsg( "Error", "Unable to allocate memory to copy bitmap." );
            ObjectDataSizeInBytes = 0;
            return;
        }

        // Copy contents of resource into allocated memory
        memcpy( pBitData, (BYTE *)DibToCopy.dib(), ObjectDataSizeInBytes );

        // Typecast pointer
        Fdib = (BITMAPINFO *)pBitData;
    }

}   // operator =


/************************************************************************
*    FUNCTION NAME:         SaveToFile                                                             
*
*    DESCRIPTION:           Save data to a file.
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TDIB::SaveToFile( char * fileName )
{
        // Make sure we have data
    if( !IsEmpty() )
        WriteDataToFile( fileName, pBitData, ObjectDataSizeInBytes, CREATE_ALWAYS );

}   // SaveToFile


/************************************************************************
*    FUNCTION NAME:         CreateDib                                                             
*
*    DESCRIPTION:           Create a DIB bitmap. Currently supports 8 bit
*                           and above.
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TDIB::CreateDib( int Width, int Height, int BitCount )
{
    BOOL Result = FALSE;
    int RowSize, headerSize, i , j;
    BITMAPFILEHEADER * Fheader;

    Free(); // Free memory if need be

    // Make sure the bitmap has a size
    if( Width < 1 || abs( Height ) == 0 )
    {
        PostMsg( "Error", "DIB has no size." );
        goto HANDLE_ERROR;
    }

    // Add in the size of the file and info headers
    headerSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );

    // Add in the size of the palette if this is an 8 bit image
    if( BitCount == BIT_COUNT_8 )
        headerSize += (sizeof( RGBQUAD ) * 256);

    // Compute the size of the width taking into account the bit count
    RowSize = Width * ( BitCount / 8 );

    // Make sure the size is DWORD alligned. Windows likes it that way.
    if( RowSize % sizeof( DWORD ) )
        RowSize += sizeof( DWORD ) - ( RowSize % sizeof( DWORD ) );

    // Add the bitmap surface area to the overall size
    ObjectDataSizeInBytes = headerSize + ( RowSize * abs( Height ) );

    // Allocate memory for bitmap
    if( (pBitData = new BYTE[ ObjectDataSizeInBytes ]) == NULL )
    {
        PostMsg( "Error", "Unable to allocate memory for dib." );
        goto HANDLE_ERROR;
    }

    // Clear out the data
    if( BitCount == BIT_COUNT_8 || BitCount == BIT_COUNT_32 )
    {
        memset( pBitData, 0, ObjectDataSizeInBytes );
    }
    else if( BitCount == BIT_COUNT_16 )
    {
        memset( pBitData, 0, ObjectDataSizeInBytes );
    }
    else if( BitCount == BIT_COUNT_24 )
    {
        for( i = 0; i < abs( Height ); ++i )
        {
            for( j = 0; j < Width; ++j )
            {
                pBitData[ headerSize + (i * RowSize) + (j * 3) ]     = 0xFF;
                pBitData[ headerSize + (i * RowSize) + (j * 3) + 1 ] = 0x00;
                pBitData[ headerSize + (i * RowSize) + (j * 3) + 2 ] = 0x00;
            }
        }
    }

    // Setup the bitmap file structure
    Fheader = (BITMAPFILEHEADER *)pBitData;
    Fheader->bfType = BITMAP_TYPE; 
    Fheader->bfSize = ObjectDataSizeInBytes; 
    Fheader->bfReserved1 = 0; 
    Fheader->bfReserved2 = 0;
    Fheader->bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );

    // Add in the palette if this is an 8-bit image
    if( BitCount == BIT_COUNT_8 ) 
        Fheader->bfOffBits += (sizeof( RGBQUAD ) * 256);

    // Setup the bitmap info structure
    Fdib = (BITMAPINFO *)(pBitData + sizeof( BITMAPFILEHEADER ));

    Fdib->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    Fdib->bmiHeader.biWidth = Width;
    Fdib->bmiHeader.biHeight = Height;
    Fdib->bmiHeader.biPlanes = 1;
    Fdib->bmiHeader.biBitCount = (WORD)BitCount;
    Fdib->bmiHeader.biCompression = BI_RGB;
    Fdib->bmiHeader.biSizeImage = 0;
    Fdib->bmiHeader.biXPelsPerMeter = DEFAULT_METERS_PER_INCH;
    Fdib->bmiHeader.biYPelsPerMeter = DEFAULT_METERS_PER_INCH;
    Fdib->bmiHeader.biClrUsed = 0;
    Fdib->bmiHeader.biClrImportant = 0;

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // CreateDib


/************************************************************************
*    FUNCTION NAME:         ClipRects
*
*    DESCRIPTION:           Clip the destination and source rects.
*
*    FUNCTION PARAMETERS:
*    Input:    TDib *pBitmap    - source bitmap
*              TRect &Dest      - Destination rect
*              TRect &Source    - Source rect
*
*    Output:   BOOL True = allow blit, False = don't blit
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    4/02/02       Function Created
************************************************************************/

BOOL TDIB::ClipRects( TDIB *pBitmap, RECT &Dest, RECT &Source )
{
    // See if the rect is within the buffer region. If it is all there,
    // we have nothing to do here so get out now.
    if( Dest.left > -1 && Dest.left + Dest.right <= pBitmap->width() &&
        Dest.top > -1 && Dest.top + Dest.bottom <= pBitmap->height() )
    {
        return TRUE;
    }

    // Make sure we have something to blit and if the rects are in the buffer region.
    // If the rect is completely off the buffer, get out because there is nothing to do.
    if( Dest.left >= pBitmap->width() || Dest.top >= pBitmap->height() ||
        Dest.left + Dest.right < -1 || Dest.top + Dest.bottom < -1 )
    {
        return FALSE;
    }

    // Make sure we are blitting from within our source rect. Just to be safe.
    if( Source.left < 1 || Source.top < 1 || Source.right < 1 || Source.bottom < 1 ||
        Source.left + Source.right > width() || Source.top + Source.bottom > height() )
    {
        return FALSE;
    }

    // Clip the X and reset the width of the rect
    if( Dest.left < 0 )
    {               
        // Reset the width of the rect
        // This is the same as Width - X without the abs()
        Dest.right = Dest.right + Dest.left;

        // Reset the X of the rect
        Dest.left = 0;

        // Reset the source X. It could be an offset into the source.
        Source.left += Source.right - Dest.right;

        // Set the source width
        Source.right = Dest.right;
    }
    
    // Clip the Y and reset the height of the rect
    if( Dest.top < 0 )
    {        
        // Reset the height of the rect
        // This is the same as Height - Y without the abs()
        Dest.bottom = Dest.bottom + Dest.top;

        // reset the Y of the rect
        Dest.top = 0;

        // Reset the source Y. It could be an offset into the source.
        Source.top += Source.bottom - Dest.bottom;

        // Set the source height
        Source.bottom = Dest.bottom;
    }
    
    // Clip the width of the destination rect
    if( Dest.left + Dest.right > pBitmap->width() )
        Source.right = Dest.right = pBitmap->width() - Dest.left;
        
    // Clip the height of the destination rect
    if( Dest.top + Dest.bottom > pBitmap->height() )
        Source.bottom = Dest.bottom = pBitmap->height() - Dest.top;  

    return TRUE;

}   // ClipRects


/************************************************************************
*    FUNCTION NAME:         ClipRects
*
*    DESCRIPTION:           Clip the destination and source rects.
*
*    FUNCTION PARAMETERS:
*    Input:    TDib *pBitmap    - source bitmap
*              TRect &Dest      - Destination rect
*              TRect &Source    - Source rect
*
*    Output:   BOOL True = allow blit, False = don't blit
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    4/02/02       Function Created
************************************************************************/

BOOL TDIB::ClipRects( RECT &Dest, RECT &Source, int dWidth, int dHeight )
{
    // See if the rect is within the buffer region. If it is all there,
    // we have nothing to do here so get out now.
    if( Dest.left > -1 && Dest.left + Dest.right <= dWidth &&
        Dest.top > -1 && Dest.top + Dest.bottom <= dHeight )
    {
        return TRUE;
    }

    // Make sure we have something to blit and if the rects are in the buffer region.
    // If the rect is completely off the buffer, get out because there is nothing to do.
    if( Dest.left >= dWidth || Dest.top >= dHeight ||
        Dest.left + Dest.right < -1 || Dest.top + Dest.bottom < -1 )
    {
        return FALSE;
    }

    // Make sure we are blitting from within our source rect. Just to be safe.
    if( Source.left < 1 || Source.top < 1 || Source.right < 1 || Source.bottom < 1 ||
        Source.left + Source.right > width() || Source.top + Source.bottom > height() )
    {
        return FALSE;
    }

    // Clip the X and reset the width of the rect
    if( Dest.left < 0 )
    {               
        // Reset the width of the rect
        // This is the same as Width - X without the abs()
        Dest.right = Dest.right + Dest.left;

        // Reset the X of the rect
        Dest.left = 0;

        // Reset the source X. It could be an offset into the source.
        Source.left += Source.right - Dest.right;

        // Set the source width
        Source.right = Dest.right;
    }
    
    // Clip the Y and reset the height of the rect
    if( Dest.top < 0 )
    {        
        // Reset the height of the rect
        // This is the same as Height - Y without the abs()
        Dest.bottom = Dest.bottom + Dest.top;

        // reset the Y of the rect
        Dest.top = 0;

        // Reset the source Y. It could be an offset into the source.
        Source.top += Source.bottom - Dest.bottom;

        // Set the source height
        Source.bottom = Dest.bottom;
    }
    
    // Clip the width of the destination rect
    if( Dest.left + Dest.right > dWidth )
        Source.right = Dest.right = dWidth - Dest.left;
        
    // Clip the height of the destination rect
    if( Dest.top + Dest.bottom > dHeight )
        Source.bottom = Dest.bottom = dHeight - Dest.top;  

    return TRUE;

}   // ClipRects


/************************************************************************
*    FUNCTION NAME:         GetScanLine                                                             
*
*    DESCRIPTION:           Get the scan line of the bitmap.
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BYTE * TDIB::GetScanLine( int line )
{
    int RowSize;
    BYTE * scanLineOffset = NULL;

    // Make sure we have data
    if( !IsEmpty() )
    {
        // Make sure we are not trying to get a line that doesn't exsist
        if( line >= height() )
            line = height() - 1;

        // Compute the size of the width taking into account the bit count
        RowSize = width() * ( GetBitCount() / 8 );

        // Make sure the size is DWORD alligned. Windows likes it that way.
        if( RowSize % sizeof( DWORD ) )
            RowSize += sizeof( DWORD ) - ( RowSize % sizeof( DWORD ) );

        // Index from the starting point of the bits
        // Window DIB are bottom to top (height() - line)
        scanLineOffset = bits() + ( RowSize * ((height()-1) - line) );
    }

    return scanLineOffset;

}   // GetScanLine


/************************************************************************
*    FUNCTION NAME:         Blit
*
*    DESCRIPTION:           Software blit
*
*    FUNCTION PARAMETERS:
*    Input:    TDib *pBitmap    - source bitmap
*              TRect &Dest      - Destination rect
*              TRect &Source    - Source rect
*              BOOL clipRect    - To clip or not to clip
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    4/02/02       Function Created
************************************************************************/

void TDIB::Blit( TDIB *pBitmap, RECT &Dest, RECT &Source, BOOL clipRect )
{
    RECT tmpDest(Dest), tmpSource(Source);
    int destMemOffset, sourceMemOffset, sizeInBytes, i;
    int destWidth, sourceWidth;

    // Do we need to provide clipping?
    if( clipRect )
    {
        if( !ClipRects( pBitmap, tmpDest, tmpSource ) )
            return;
    }

    // Calculate the starting memory offsets
    // It is assumed they are all the same bit depths
    if( pBitmap->GetBitmapType() == BOTTOM_UP_BMP )
        destMemOffset = ((((pBitmap->height()-1)-tmpDest.top) * pBitmap->width()) + tmpDest.left) * (pBitmap->GetBitCount() >> 3);
    else
        destMemOffset = ((tmpDest.top * pBitmap->width()) + tmpDest.left) * (GetBitCount() >> 3);
        
    if( GetBitmapType() == BOTTOM_UP_BMP )
        sourceMemOffset = ((((height()-1)-tmpSource.top) * width()) + tmpSource.left) * (GetBitCount() >> 3);
    else
        sourceMemOffset = ((tmpSource.top * width()) + tmpSource.left) * (GetBitCount() >> 3);
    
    // The source and destination widths
    destWidth = pBitmap->width() * (pBitmap->GetBitCount() >> 3);
    sourceWidth = width() * (GetBitCount() >> 3);

    // The amount of bytes transfered
    sizeInBytes = tmpDest.right * (GetBitCount() >> 3);

    // Copy the memory
    if( pBitmap->GetCopyMode() == SRCCOPY )
    {
        if( pBitmap->GetBitmapType() == BOTTOM_UP_BMP && GetBitmapType() == BOTTOM_UP_BMP )
        {
            for( i = 0; i < tmpDest.bottom; ++i )
                memcpy( LPBYTE(pBitmap->bits()) + (destMemOffset - (i * destWidth)),
                        LPBYTE(bits()) + (sourceMemOffset - (i * sourceWidth)),                      
                        sizeInBytes );
        }
        else if( pBitmap->GetBitmapType() == TOP_DOWN_BMP && GetBitmapType() == TOP_DOWN_BMP )
        {
            for( i = 0; i < tmpDest.bottom; ++i )
                memcpy( LPBYTE(pBitmap->bits()) + (destMemOffset + (i * destWidth)),
                        LPBYTE(bits()) + (sourceMemOffset + (i * sourceWidth)),                      
                        sizeInBytes );
        }
        else if( pBitmap->GetBitmapType() == TOP_DOWN_BMP && GetBitmapType() == BOTTOM_UP_BMP )
        {
            for( i = 0; i < tmpDest.bottom; ++i )
                memcpy( LPBYTE(pBitmap->bits()) + (destMemOffset + (i * destWidth)),
                        LPBYTE(bits()) + (sourceMemOffset - (i * sourceWidth)),                      
                        sizeInBytes );
        }
        else if( pBitmap->GetBitmapType() == BOTTOM_UP_BMP && GetBitmapType() == TOP_DOWN_BMP )
        {
            for( i = 0; i < tmpDest.bottom; ++i )
                memcpy( LPBYTE(pBitmap->bits()) + (destMemOffset - (i * destWidth)),
                        LPBYTE(bits()) + (sourceMemOffset + (i * sourceWidth)),                      
                        sizeInBytes );
        }

    }
    /*
    else if( pBitmap->GetCopyMode() == SRCAND )
    {
        for( i = 0; i < tmpDest.bottom; ++i )
            memcpy_and( LPBYTE(bits()) + destMemOffset + (i * destWidth), 
                    LPBYTE(pBitmap->bits()) + sourceMemOffset + (i * sourceWidth), 
                    sizeInBytes );
    }
    else if( pBitmap->GetCopyMode() == SRCPAINT )
    {
        for( i = 0; i < tmpDest.bottom; ++i )
            memcpy_or( LPBYTE(bits()) + destMemOffset + (i * destWidth), 
                    LPBYTE(pBitmap->bits()) + sourceMemOffset + (i * sourceWidth), 
                    sizeInBytes );
    }
    else if( pBitmap->GetCopyMode() == BLACKNESS )
    {
        for( i = 0; i < tmpDest.bottom; ++i )
            memset( LPBYTE(bits()) + destMemOffset + (i * destWidth), 0, sizeInBytes );
    }
    else if( pBitmap->GetCopyMode() == WHITENESS )
    {
        for( i = 0; i < tmpDest.bottom; ++i )
            memset( LPBYTE(bits()) + destMemOffset + (i * destWidth), 0xFF, sizeInBytes );
    }
    */

}   // Blit


/************************************************************************
*    FUNCTION NAME:         CopyDIB                                                             
*
*    DESCRIPTION:           Copy one bitmap to another.
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TDIB::CopyDIB( TDIB *DibToCopy )
{    
    BOOL Result = FALSE;
    
    Free(); // Free memory if need be

    // Save the size of the file for later use
    ObjectDataSizeInBytes = DibToCopy->GetObjectSizeInBytes();

    // Make sure we have something to copy
    if( ObjectDataSizeInBytes && DibToCopy->dib() )
    {
        // Allocate memory for bitmap
        pBitData = new BYTE[ ObjectDataSizeInBytes ];
        if( pBitData == NULL )
        {
            PostMsg( "Error", "Unable to allocate memory to copy bitmap." );
            ObjectDataSizeInBytes = 0;
            goto HANDLE_ERROR;
        }

        // Copy contents of resource into allocated memory
        memcpy( pBitData, DibToCopy->GetPointerToData(), ObjectDataSizeInBytes );

        // Copy pointer
        Fdib = (BITMAPINFO *)(pBitData + sizeof( BITMAPFILEHEADER ));
    }
	else
		goto HANDLE_ERROR;

	Result = TRUE;

HANDLE_ERROR:;

	return Result;

}   // CopyDIB


/************************************************************************
*    FUNCTION NAME:         IsBitmapTransparent                                                             
*
*    DESCRIPTION:           Is the loaded bitmap a transparent bitmap?
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TDIB::IsBitmapTransparent()
{ 
    BOOL Result = FALSE;
    BYTE *BYTEScanLine;
    WORD *WORDScanLine;
    int Terminator, i, j, imageWidth, imageHeight, Offset;

	// See if the transparent flag has been set. 
	// If it has, just return that value
	if( fTrans != -1 )
		return fTrans;

    // Check that we have a valid pointer and the value is more than zero
    if( !IsEmpty() )
    {
        // Use local varaibles for the width and height
        // for speed in the tight loops
        imageWidth = width();
        imageHeight = height();

        // see if we have a terminator
        Terminator = (width() * (GetBitCount() / 8)) % sizeof( DWORD );

        // Get a pointer to the bits
        if( GetBitCount() == 16 )
        {
            WORDScanLine = (WORD *)bits();
            Terminator /= 2;
        }
        else
        {
            BYTEScanLine = bits();

            if( Terminator > 0 )
                Terminator = sizeof( DWORD ) - Terminator;
        }

        // Take the ternimator into consideration when getting the width
        //if( GetBitCount() == 24 )
        //  ImageWidth = width() * 3;
        //else
        //  ImageWidth = width() + Terminator;

        if( GetBitCount() == 8 )
        {
            for( i = 0; i < imageHeight; ++i )
            {
                for( j = 0; j < imageWidth; ++j )
                {
                    if( *(BYTEScanLine + (( i * imageWidth)  + (i * Terminator) + j) ) == 0 )
                    {
                        Result = TRUE;
                        goto INFO_FOUND_JUMP;
                    }
                }
            }
        }
        else if( GetBitCount() == 16 )
        {
            for( i = 0; i < imageHeight; ++i )
            {
                for( j = 0; j < imageWidth; ++j )
                {
                    if( *(WORDScanLine + (( i * imageWidth) + (i * Terminator) + j) ) == BIT_16_TRANS_COLOR )
                    {
                        Result = TRUE;
                        goto INFO_FOUND_JUMP;
                    }
                }
            }
        }
        else if( GetBitCount() == 24 )
        {
            for( i = 0; i < imageHeight; ++i )
            {
                for( j = 0; j < imageWidth; ++j )
                {
                    // Calculate the offset
                    Offset = ( (i * imageWidth) + (i * Terminator) + (j * 3) );
                    
                    if( *(BYTEScanLine + Offset ) == 0x00 && 
                        *(BYTEScanLine + Offset + 1 ) == 0xFF && 
                        *(BYTEScanLine + Offset + 2 ) == 0x00 )
                    {
                        Result = TRUE;
                        goto INFO_FOUND_JUMP;
                    }
                }
            }
        }
    }

INFO_FOUND_JUMP:;

	// Save the result
	fTrans = Result;

    return Result;

}   // IsBitmapTransparent


/************************************************************************
*    FUNCTION NAME:         Blit
*
*    DESCRIPTION:           Software blit. This assumes a top down bitmap
*                           and that it's the same bit depth as the DIB
*
*    FUNCTION PARAMETERS:
*    Input:    TDib *pBitmap    - source bitmap
*              TRect &Dest      - Destination rect
*              TRect &Source    - Source rect
*              BOOL clipRect    - To clip or not to clip
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    4/02/02       Function Created
************************************************************************/

void TDIB::Blit( BYTE *pBitmap, RECT &Dest, RECT &Source, int dWidth, int dHeight, int bitmapType, BOOL clipRect )
{
    RECT tmpDest(Dest), tmpSource(Source);
    int destMemOffset, sourceMemOffset, sizeInBytes, i;
    int destWidth, sourceWidth;

    // Do we need to provide clipping?
    if( clipRect )
    {
        if( !ClipRects( tmpDest, tmpSource, dWidth, dHeight ) )
            return;
    }

    // Calculate the starting memory offsets
    // It is assumed they are all the same bit depths
    if( bitmapType == BOTTOM_UP_BMP )
        destMemOffset = ((((dHeight-1)-tmpDest.top) * dWidth) + tmpDest.left) * (GetBitCount() >> 3);
    else
        destMemOffset = ((tmpDest.top * dWidth) + tmpDest.left) * (GetBitCount() >> 3);
        
    if( GetBitmapType() == BOTTOM_UP_BMP )
        sourceMemOffset = ((((height()-1)-tmpSource.top) * width()) + tmpSource.left) * (GetBitCount() >> 3);
    else
        sourceMemOffset = ((tmpSource.top * width()) + tmpSource.left) * (GetBitCount() >> 3);
    
    // The source and destination widths
    destWidth = dWidth * (GetBitCount() >> 3);
    sourceWidth = width() * (GetBitCount() >> 3);

    // The amount of bytes transfered
    sizeInBytes = tmpDest.right * (GetBitCount() >> 3);

    // Copy the memory
    if( GetCopyMode() == SRCCOPY )
    {
        if( bitmapType == BOTTOM_UP_BMP && GetBitmapType() == BOTTOM_UP_BMP )
        {
            for( i = 0; i < tmpDest.bottom; ++i )
                memcpy( LPBYTE(pBitmap) + (destMemOffset - (i * destWidth)),
                        LPBYTE(bits()) + (sourceMemOffset - (i * sourceWidth)),                      
                        sizeInBytes );
        }
        else if( bitmapType == TOP_DOWN_BMP && GetBitmapType() == TOP_DOWN_BMP )
        {
            for( i = 0; i < tmpDest.bottom; ++i )
                memcpy( LPBYTE(pBitmap) + (destMemOffset + (i * destWidth)),
                        LPBYTE(bits()) + (sourceMemOffset + (i * sourceWidth)),                      
                        sizeInBytes );
        }
        else if( bitmapType == TOP_DOWN_BMP && GetBitmapType() == BOTTOM_UP_BMP )
        {
            for( i = 0; i < tmpDest.bottom; ++i )
                memcpy( LPBYTE(pBitmap) + (destMemOffset + (i * destWidth)),
                        LPBYTE(bits()) + (sourceMemOffset - (i * sourceWidth)),                      
                        sizeInBytes );
        }
        else if( bitmapType == BOTTOM_UP_BMP && GetBitmapType() == TOP_DOWN_BMP )
        {
            for( i = 0; i < tmpDest.bottom; ++i )
                memcpy( LPBYTE(pBitmap) + (destMemOffset - (i * destWidth)),
                        LPBYTE(bits()) + (sourceMemOffset + (i * sourceWidth)),                      
                        sizeInBytes );
        }
    }
    /*
    else if( GetCopyMode() == SRCAND )
    {
        for( i = 0; i < tmpDest.bottom; ++i )
            memcpy_and( LPBYTE(bits()) + destMemOffset + (i * destWidth), 
                    LPBYTE(pBitmap->bits()) + sourceMemOffset + (i * sourceWidth), 
                    sizeInBytes );
    }
    else if( GetCopyMode() == SRCPAINT )
    {
        for( i = 0; i < tmpDest.bottom; ++i )
            memcpy_or( LPBYTE(bits()) + destMemOffset + (i * destWidth), 
                    LPBYTE(pBitmap->bits()) + sourceMemOffset + (i * sourceWidth), 
                    sizeInBytes );
    }
    else if( GetCopyMode() == BLACKNESS )
    {
        for( i = 0; i < tmpDest.bottom; ++i )
            memset( LPBYTE(bits()) + destMemOffset + (i * destWidth), 0, sizeInBytes );
    }
    else if( GetCopyMode() == WHITENESS )
    {
        for( i = 0; i < tmpDest.bottom; ++i )
            memset( LPBYTE(bits()) + destMemOffset + (i * destWidth), 0xFF, sizeInBytes );
    }
    */

}   // Blit