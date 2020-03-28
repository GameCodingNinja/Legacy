
/************************************************************************
*
*    FILE NAME:       CBitmap.cpp
*
*    DESCRIPTION:     CBitmap object for using windows GDI graphics
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
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include "minTools.h"        // A misc group of useful functions
#include "bitmap.h"

/************************************************************************
*
*                         CBitmap Object
*
************************************************************************/




/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
************************************************************************/
CBitmap::CBitmap()
{
    hOldBmp = NULL;
    hBitmap = NULL;
    bmpDC = NULL;
    Fwidth = 0;
    Fheight = 0;
    ppDIBSectBits = NULL;
    Terminator = 0;
    DibSectionBitDepth = 0;
    StretchBlitMode = HALFTONE;
}


/***********************************************************************                                                             
*    DESCRIPTION:           Free allocated memory and palette
************************************************************************/
void CBitmap::Free()
{
    if( bmpDC )
    {
        SelectObject( bmpDC, hOldBmp );
        DeleteDC( bmpDC );
        bmpDC = NULL;
    }

    if( hBitmap )
    {
        DeleteObject( hBitmap );
        hBitmap = NULL;
        ppDIBSectBits = NULL;
        Fwidth = 0;
        Fheight = 0;
    }

}   // Free()


/************************************************************************                                                             
*    DESCRIPTION: Creates a CreateCompatibleDC and a
*                 CreateDIBSection and selects the DIBSection
*                 into the bmpDC. The CreateDIBSection allows for
*                 direct access to the memory which makes it that
*                 much faster than your standard GDI bitmaps. This
*                 uses the DIB data that is currently loaded.
*
*    FUNCTION PARAMETERS:
*    Input:    EDibType BitmapType - DIB_SECTION_BMP or STANDARD_DIB
*              int BitCount - bit count
*              EDibTopBottom orientation - BOTTOM_UP_BMP or TOP_DOWN_BMP
*
*    Output:   bool - true on success or false on fail
************************************************************************/
bool CBitmap::CreateBitmap( EDibType BitmapType, int BitCount, EDibTopBottom orientation )
{
    RGBQUAD *pRGBQUAD = NULL;

    // Free all windows resources if any have been allocated
    Free();

    // Return if this bitmap hasn't been loaded
    if( IsEmpty() )
        return false;

    // Create the DIBSection
    if( BitmapType == DIB_SECTION_BMP )
    {
        // Make sure we have an image with a palette
        if( getDIBcolors() )
            pRGBQUAD = GetRGBQUAD();

        // Top down or bottom up bitmap
        if( orientation == BOTTOM_UP_BMP )
        {        
            if( !CreateDIBSectionBuffer( width(), height(), BitCount, pRGBQUAD ) )
            {
                Free();
                return false;
            }
        }
        else if( orientation == TOP_DOWN_BMP )
        {
            if( !CreateDIBSectionBuffer( width(), -height(), BitCount, pRGBQUAD ) )
            {
                Free();
                return false;
            }
        }
    }
    // Create the standard DIB
    else if( BitmapType == STANDARD_DIB )
    {
        if( !CreateStandardBuffer( width(), height() ) )
        {
            Free();
            return false;
        }
    }

    // Blit our new BMP to the DC
    StretchDIBits( bmpDC, 0, 0, width(), height(),
                   0, 0, width(), height(), bits(), dib(), DIB_RGB_COLORS, SRCCOPY );

    return true;
   
}   // CreateSectionFromDIB


/************************************************************************
*    DESCRIPTION: Create a buffer for blitting to.
*                 Creates a CreateCompatibleDC and a
*                 CreateDIBSection and selects the DIBSection
*                 into the bmpDC. The CreateDIBSection allows for
*                 direct access to the memory which makes it that
*                 much faster than your standard GDI bitmaps.
*
*    FUNCTION PARAMETERS:
*    Input:    int Width  - Width of the buffer
*              int Height - Height of the buffer
*              int BitCount - Sets the type of bitmap you plan to use.
*                             Default to 8 bit because it's faster than 24 bit
*			   RGBQUAD *pPalette - a palette if this is a 256 color bitmap
*
*    Output:   bool - true on success or false on fail
************************************************************************/
bool CBitmap::CreateDIBSectionBuffer( int Width, int Height, int BitCount, RGBQUAD *pPalette )
{
    HDC DC;
    HWND hwnd;
    bool Result = false;
    SBitmapinfo BmpInfo;
    SLogpalette pal;
    int i;

    // Free all resources if any have been allocated
    Free();

    // Set the rect
    rect.top = 0;
    rect.left = 0;
    rect.right = Width;
    rect.bottom = abs(Height);

    // record the bit count of this buffer
    DibSectionBitDepth = BitCount;

    // Try using a DC we already have first.
    if( hDeviceDC )
        DC = hDeviceDC;
    else
    {
        // Get the handle to the active window
        // Even if the GetActiveWindow function fails, it returns 0 which is
        // the desktop window and that will work too.
        hwnd = GetActiveWindow();
        DC = GetDC( hwnd );
    }

    // Save the width and height of this buffer
    Fwidth = Width;
    Fheight = abs(Height);

    // Create the compatible dc but check that we have a valid handle
    bmpDC = CreateCompatibleDC( DC );
    if( !bmpDC )
    {
        MessageBox( hwnd, "Can't create compatible DC", "Error", MB_ICONWARNING );
        Free();
        goto FREEMEMORY;
    }

    // Zero out the structure
    memset( &BmpInfo, 0, sizeof( SBitmapinfo ) );

    // At the end of each horizontal line of pixels can be a terminator.
    // The horizontal scan line has to be 4 byte alligned so if the bitmaps
    // horizontal scan line is not divisable by 4, it is padded with zeros
    // or other values. This terminator has to be stripped out.
    Terminator = BmpInfo.bmiHeader.biWidth % sizeof(DWORD);
    if( Terminator > 0 )
        Terminator = sizeof(DWORD) - Terminator;

    // Make an 8 bit buffer because it animates the fastest
    BmpInfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
    BmpInfo.bmiHeader.biWidth         = Width;
    BmpInfo.bmiHeader.biHeight        = Height;
    BmpInfo.bmiHeader.biPlanes        = 1;
    BmpInfo.bmiHeader.biCompression   = BI_RGB;
    BmpInfo.bmiHeader.biSizeImage     = Width * abs(Height);

	if( BitCount == BIT_COUNT_8 )
	{
	    BmpInfo.bmiHeader.biXPelsPerMeter = (int)(CONVERT_TO_PIXELS * Width); 
	    BmpInfo.bmiHeader.biYPelsPerMeter = (int)(CONVERT_TO_PIXELS * abs(Height));
	    BmpInfo.bmiHeader.biClrUsed       = 256; 
	    BmpInfo.bmiHeader.biClrImportant  = 256;
	}

    // Use the current screen bitcount if one is not specified
    if( BitCount )
        BmpInfo.bmiHeader.biBitCount      = (WORD)BitCount;
    else
        BmpInfo.bmiHeader.biBitCount      = (WORD)GetDeviceCaps( DC, BITSPIXEL );

    // Use the palette passed as a parameter
    if( pPalette )
    {
        // Copy over the palette to the CBitmapinfo
        for( i = 0; i < 256; ++i )
        {
            BmpInfo.bmiColors[ i ] = pPalette[ i ];
            pal.palPalEntry[i].peRed       = pPalette[ i ].rgbRed;
            pal.palPalEntry[i].peGreen     = pPalette[ i ].rgbGreen;
            pal.palPalEntry[i].peBlue      = pPalette[ i ].rgbBlue;
            pal.palPalEntry[i].peFlags     = 0;
        }
    }
    else
    {
        // Get the palette from the system
        if( GetSystemPaletteEntries( DC, 0, 255, pal.palPalEntry ) )
        {
            for( i = 0; i < 256; ++i )
            {
                BmpInfo.bmiColors[i].rgbRed      = pal.palPalEntry[i].peRed;  
                BmpInfo.bmiColors[i].rgbGreen    = pal.palPalEntry[i].peGreen;
                BmpInfo.bmiColors[i].rgbBlue     = pal.palPalEntry[i].peBlue;
                BmpInfo.bmiColors[i].rgbReserved = 0;
            }
        }
    }

    // Make the palette if we are in 8 bit color or less
    if( GetDeviceCaps( DC, BITSPIXEL ) <= BIT_COUNT_8 )
    {
        // make the palette
        pal.palVersion = 0x0300;
        pal.palNumEntries = 256;

        // Create the palette and make sure we have a valid handle
        hPalette = CreatePalette ( (tagLOGPALETTE *)&pal );
        if( !hPalette )
            goto FREEMEMORY;
    }

    // Create the dib section for fast blitting and save the pointer to the bits
    hBitmap = CreateDIBSection( bmpDC, (BITMAPINFO *)&BmpInfo, DIB_RGB_COLORS, (void **)&ppDIBSectBits, NULL, NULL );
    if( !hBitmap )
    {
        MessageBox( hwnd, "Can't create compatible bitmap", "Error", MB_ICONWARNING );
        Free();
        goto FREEMEMORY;
    }

    // Select our new bitmap into the DC and save the old bitmap. It's needed for when we
    // ned to delete the DC
    hOldBmp = (HBITMAP)SelectObject( bmpDC, hBitmap );
    if( !hOldBmp )
    {
        MessageBox( hwnd, "Can't select object into device context", "Error", MB_ICONWARNING );
        Free();
        goto FREEMEMORY;
    }

    Result = true;

    FREEMEMORY:;

    if( !hDeviceDC && DC )
        ReleaseDC( hwnd, DC );

    return Result;

}   // CreateDIBSectionBuffer


/************************************************************************
*    DESCRIPTION: Creates a CreateCompatibleDC and a
*                 CreateCompatibleBitmap and selects the bitmap
*                 into the DC 
*
*    FUNCTION PARAMETERS:
*    Input:    int Width  - Width of the buffer
*              int Height - Height of the buffer
*
*    Output:   bool - true on success or false on fail
************************************************************************/
bool CBitmap::CreateStandardBuffer( int Width, int Height )
{
    HDC DC;
    HWND hwnd;
    bool Result = false;

    // Free all resources if any have been allocated
    Free();

    // Set the rect
    rect.top = 0;
    rect.left = 0;
    rect.right = Width;
    rect.bottom = Height;

    // Get the handle to the active window
    // Even if the GetActiveWindow function fails, it returns 0 which is
    // the desktop window and that will work too.
    if( hDeviceDC ) // Are we using a DC we already have?
        DC = hDeviceDC;
    else
    {
        hwnd = GetActiveWindow();
        DC = GetDC( hwnd );
    }

    // Record the width and height
    Fwidth = Width;
    Fheight = Height;

    // Create the compatible dc but check that we have a valid handle
    bmpDC = CreateCompatibleDC( DC );
    if( !bmpDC )
    {
        MessageBox( hwnd, "Can't create compatible DC", "Error", MB_ICONWARNING );
        Free();
        goto FREEMEMORY;
    }
  
    // Create the compatible bitmap to use as our buffer
    hBitmap = CreateCompatibleBitmap( DC, Width, Height );
    if( !hBitmap )
    {
        MessageBox( hwnd, "Can't create compatible bitmap", "Error", MB_ICONWARNING );
        Free();
        goto FREEMEMORY;
    }

    // Select our new bitmap into the DC and save the old bitmap. It's needed for when we
    // ned to delete the DC
    hOldBmp = (HBITMAP)SelectObject( bmpDC, hBitmap );
    if( !hOldBmp )
    {
        MessageBox( hwnd, "Can't select object into device context", "Error", MB_ICONWARNING );
        Free();
        goto FREEMEMORY;
    }

    Result = true;

    FREEMEMORY:;

    if( !hDeviceDC )
        ReleaseDC( hwnd, DC  );

    return Result;

}   // CreateStandardBuffer


/************************************************************************
*    DESCRIPTION: One of many flavors to blit to a DC. This 
*                 version of Draw is for convience and is not
*                 ment for full blown animation
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hDest - Handle to a device (window or printer) 
*                           where we get the DC
*              int X      - X coordinate to blit
*              int Y      - Y coordinate to blit
************************************************************************/
void CBitmap::Draw( HWND hDest, int X, int Y )
{
    HDC DestHDC;

    if( bmpDC && hBitmap )
    {
        // Get the DC from the supplied handle
        DestHDC = GetDC( hDest );
        // Blit it
        BitBlt( DestHDC, X, Y, width(), height(), bmpDC, 0, 0, FRasterOp );
        // Release the handle
        ReleaseDC( hDest, DestHDC  );
    }
}   // Draw


/************************************************************************
*    DESCRIPTION: One of many flavors to blit to a DC. This 
*                 version of Draw is for convience and is not
*                 ment for full blown animation
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hDest - Handle to a device (window or printer) 
*                           where we get the DC
*              POINT *Dest   - Point structure for the point on the screen
*              RECT *Source  - The rect of the art used for blitting
************************************************************************/
void CBitmap::Draw( HWND hDest, POINT *Dest, RECT *Source )
{
    HDC DestHDC;

    if( bmpDC && hBitmap )
    {
        // Get the DC from the supplied handle
        DestHDC = GetDC( hDest );
        // Blit it
        BitBlt( DestHDC, Dest->x, Dest->y, Source->right, Source->bottom, bmpDC, Source->left, Source->top, FRasterOp );
        // Release the handle
        ReleaseDC( hDest, DestHDC  );
    }
}   // Draw


/************************************************************************
*    DESCRIPTION: One of many flavors to blit to a DC. This 
*                 version of Draw is used for blitting to your
*                 windows DC
*
*    FUNCTION PARAMETERS:
*    Input:    HDC DestDC - DC of output device (window or printer)
*              int X      - X coordinate to blit
*              int Y      - Y coordinate to blit
************************************************************************/
void CBitmap::Draw( HDC DestDC, int X, int Y )
{  
    BitBlt( DestDC, X, Y, width(), height(), bmpDC, 0, 0, FRasterOp );
}


/************************************************************************
*    DESCRIPTION: One of many flavors to blit to a DC. This 
*                 version of Draw is used for blitting to your
*                 windows DC
*
*    FUNCTION PARAMETERS:
*    Input:    HDC DestDC - DC of output device (window or printer)
*              POINT *Dest   - Point structure for the point on the screen
*              RECT *Source  - The rect of the art used for blitting
************************************************************************/
void CBitmap::Draw( HDC DestDC, POINT *Dest, RECT *Source )
{  
    BitBlt( DestDC, Dest->x, Dest->y, Source->right, Source->bottom, bmpDC, Source->left, Source->top, FRasterOp );
}


/************************************************************************
*    DESCRIPTION: One of many flavors to blit to a DC. This 
*                 version of Draw is used for blitting from
*                 one CBitmap to another CBitmap
*
*    FUNCTION PARAMETERS:
*    Input:    CBitmap *pBitmap - Pointer to another CBitmap
*              int X            - X coordinate to blit
*              int Y            - Y coordinate to blit
************************************************************************/
void CBitmap::Draw( CBitmap *pBitmap, int X, int Y )
{  
    BitBlt( pBitmap->Handle(), X, Y, width(), height(), bmpDC, 0, 0, FRasterOp );
}


/************************************************************************
*    DESCRIPTION: One of many flavors to blit to a DC. This 
*                 version of Draw is used for blitting to your
*                 windows DC
*
*    FUNCTION PARAMETERS:
*    Input:    CBitmap *pBitmap - Pointer to another CBitmap
*              POINT *Dest   - Point structure for the point on the screen
*              RECT *Source  - The rect of the art used for blitting
************************************************************************/
void CBitmap::Draw( CBitmap *pBitmap, POINT *Dest, RECT *Source )
{
    BitBlt( pBitmap->Handle(), Dest->x, Dest->y, Source->right, Source->bottom, bmpDC, Source->left, Source->top, FRasterOp );
}


/************************************************************************
*    DESCRIPTION: One of many flavors to blit to a DC. This 
*                 version of Draw is used for blitting to your
*                 windows DC
*
*    FUNCTION PARAMETERS:
*    Input:    CBitmap *pBitmap - Pointer to another CBitmap
*              RECT *Source  - Same rect is used for the source and destination
************************************************************************/
void CBitmap::Draw( CBitmap *pBitmap, RECT *SourceDest )
{
    BitBlt( pBitmap->Handle(), SourceDest->left, SourceDest->top, SourceDest->right, 
            SourceDest->bottom, bmpDC, SourceDest->left, SourceDest->top, FRasterOp );
}


/************************************************************************
*    DESCRIPTION: One of many flavors to blit to a DC. This 
*                 version of Draw is used for blitting to your
*                 windows DC
*
*    FUNCTION PARAMETERS:
*    Input:    HDC DestDC - DC of output device (window or printer)
*              RECT *Source  - Same rect is used for the source and destination
************************************************************************/
void CBitmap::Draw( HDC DestDC, RECT *SourceDest )
{  
    BitBlt( DestDC, SourceDest->left, SourceDest->top, SourceDest->right, 
            SourceDest->bottom, bmpDC, SourceDest->left, SourceDest->top, FRasterOp );
}


/************************************************************************
*    DESCRIPTION: One of many flavors to blit to a DC. This 
*                 version of Draw is for convience and is not
*                 ment for full blown animation
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hDest - Handle to a device (window or printer) 
*                           where we get the DC
*              RECT *Source  - Same rect is used for the source and destination
************************************************************************/
void CBitmap::Draw( HWND hDest, RECT *SourceDest )
{
    HDC DestHDC;

    if( bmpDC && hBitmap )
    {
        // Get the DC from the supplied handle
        DestHDC = GetDC( hDest );
        // Blit it
        BitBlt( DestHDC, SourceDest->left, SourceDest->top, SourceDest->right, 
                SourceDest->bottom, bmpDC, SourceDest->left, SourceDest->top, FRasterOp );
        // Release the handle
        ReleaseDC( hDest, DestHDC  );
    }
}   // Draw


/************************************************************************
*    DESCRIPTION: One of many flavors to blit to a DC. This 
*                 version of Draw is used for blitting to your
*                 windows DC
*
*    FUNCTION PARAMETERS:
*    Input:    CBitmap *pBitmap - Pointer to another CBitmap
*              RECT *Dest   - Point structure for the point on the screen
*              RECT *Source  - The rect of the art used for blitting
************************************************************************/
void CBitmap::Draw( CBitmap *pBitmap, RECT *Dest, RECT *Source )
{
    BitBlt( pBitmap->Handle(), Dest->left, Dest->top, Dest->right, Dest->bottom, bmpDC, Source->left, Source->top, FRasterOp );
}


/************************************************************************
*    DESCRIPTION: One of many flavors to blit to a DC. This 
*                 version of Draw is used for blitting to your
*                 windows DC
*
*    FUNCTION PARAMETERS:
*    Input:    HDC DestDC - DC of output device (window or printer)
*              RECT *Dest   - RECT structure for the point on the screen
*              RECT *Source  - The rect of the art used for blitting
************************************************************************/
void CBitmap::Draw( HDC DestDC, RECT *Dest, RECT *Source )
{  
    BitBlt( DestDC, Dest->left, Dest->top, Dest->right, Dest->bottom, bmpDC, Source->left, Source->top, FRasterOp );
}


/************************************************************************
*    DESCRIPTION: One of many flavors to blit to a DC. This 
*                 version of Draw is for convience and is not
*                 ment for full blown animation
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hDest - Handle to a device (window or printer) 
*                           where we get the DC
*              RECT *Dest   - RECT structure for the point on the screen
*              RECT *Source  - The rect of the art used for blitting
************************************************************************/
void CBitmap::Draw( HWND hDest, RECT *Dest, RECT *Source )
{
    HDC DestHDC;

    if( bmpDC && hBitmap )
    {
        // Get the DC from the supplied handle
        DestHDC = GetDC( hDest );
        // Blit it
        BitBlt( DestHDC, Dest->left, Dest->top, Dest->right, Dest->bottom, bmpDC, Source->left, Source->top, FRasterOp );
        // Release the handle
        ReleaseDC( hDest, DestHDC  );
    }
}   // Draw


/************************************************************************
*    DESCRIPTION: One of many flavors to stretch blit to a DC.  
*                 This version of StretchDraw is for convience 
*                 and is not ment for full blown animation
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hDest   - Handle to a window where we get the DC
*              RECT r       - Rect structure containing the stretched coordinates
************************************************************************/
void CBitmap::StretchDraw( HWND hDest, RECT *r )
{
    HDC DestHDC;

    if( bmpDC && hBitmap )
    {
        // Get the DC from the supplied handle
        DestHDC = GetDC( hDest );

        // This ensures the niceest looking stretch
        SetStretchBltMode( DestHDC, StretchBlitMode );

        // Blit it
        StretchBlt( DestHDC, r->left, r->top, r->right, r->bottom, 
                    bmpDC, 0, 0, width(), height(), FRasterOp );

        // Resets every thing back after a stretch
        SetBrushOrgEx( DestHDC, 0, 0, NULL );

        // Release the handle
        ReleaseDC( hDest, DestHDC  );
    }
}   // StretchDraw


/************************************************************************
*    DESCRIPTION: One of many flavors to stretch blit to a DC.  
*                 This version of StretchDraw is for convience 
*                 and is not ment for full blown animation
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hDest   - Handle to a window where we get the DC
*              RECT *Dest   - Rect structure containing the stretched coordinates
*              RECT *Source - Rect of the art used for blitting
************************************************************************/
void CBitmap::StretchDraw( HWND hDest, RECT *Dest, RECT *Source )
{
    HDC DestHDC;

    if( bmpDC && hBitmap )
    {
        // Get the DC from the supplied handle
        DestHDC = GetDC( hDest );

        // This ensures the niceest looking stretch
        SetStretchBltMode( DestHDC, StretchBlitMode );

        // Blit it
        StretchBlt( DestHDC, Dest->left, Dest->top, Dest->right, Dest->bottom, 
                    bmpDC, Source->left, Source->top, Source->right, Source->bottom, FRasterOp );

        // Resets every thing back after a stretch
        SetBrushOrgEx( DestHDC, 0, 0, NULL );

        // Release the handle
        ReleaseDC( hDest, DestHDC  );
    }
}   // StretchDraw


/************************************************************************
*    DESCRIPTION: One of many flavors to blit to a DC. This 
*                 version of Draw is used for blitting to your
*                 windows DC
*
*    FUNCTION PARAMETERS:
*    Input:    HDC DestDC - DC of output device
*              RECT r     - Rect structure containing the stretched coordinates
************************************************************************/
void CBitmap::StretchDraw( HDC DestDC, RECT *r )
{
    // This ensures the niceest looking stretch
    SetStretchBltMode( DestDC, StretchBlitMode );

    StretchBlt( DestDC, r->left, r->top, r->right, r->bottom,
                bmpDC, 0, 0, width(), height(), FRasterOp );

    // Resets every thing back after a stretch
    SetBrushOrgEx( DestDC, 0, 0, NULL );

}   // StretchDraw


/************************************************************************
*    DESCRIPTION: One of many flavors to blit to a DC. This 
*                 version of Draw is used for blitting to your
*                 windows DC
*
*    FUNCTION PARAMETERS:
*    Input:    HDC DestDC - DC of output device
*              RECT *Dest   - Rect structure containing the stretched coordinates
*              RECT *Source - Rect of the art used for blitting
************************************************************************/
void CBitmap::StretchDraw( HDC DestDC, RECT *Dest, RECT *Source )
{
    // This ensures the niceest looking stretch
    SetStretchBltMode( DestDC, StretchBlitMode );

    StretchBlt( DestDC, Dest->left, Dest->top, Dest->right, Dest->bottom,
                bmpDC, Source->left, Source->top, Source->right, Source->bottom, FRasterOp );

    // Resets every thing back after a stretch
    SetBrushOrgEx( DestDC, 0, 0, NULL );

}   // StretchDraw


/************************************************************************
*    DESCRIPTION: One of many flavors to blit to a DC. This 
*                 version of Draw is used for blitting from
*                 one CBitmap to another CBitmap
*
*    FUNCTION PARAMETERS:
*    Input:    HDC DestDC - DC of output device
*              RECT r     - Rect structure containing the stretched coordinates
************************************************************************/
void CBitmap::StretchDraw( CBitmap *pBitmap, RECT *r )
{
    // This ensures the niceest looking stretch
    SetStretchBltMode( pBitmap->Handle(), StretchBlitMode );

    StretchBlt( pBitmap->Handle(), r->left, r->top, r->right, r->bottom,
                bmpDC, 0, 0, width(), height(), FRasterOp );

    // Resets every thing back after a stretch
    SetBrushOrgEx( pBitmap->Handle(), 0, 0, NULL );

}   // StretchDraw


/************************************************************************
*    DESCRIPTION: One of many flavors to blit to a DC. This 
*                 version of Draw is used for blitting from
*                 one CBitmap to another CBitmap
*
*    FUNCTION PARAMETERS:
*    Input:    HDC DestDC - DC of output device
*              RECT *Dest   - Rect structure containing the stretched coordinates
*              RECT *Source - Rect of the art used for blitting
************************************************************************/
void CBitmap::StretchDraw( CBitmap *pBitmap, RECT *Dest, RECT *Source )
{
    // This ensures the niceest looking stretch
    SetStretchBltMode( pBitmap->Handle(), StretchBlitMode );

    StretchBlt( pBitmap->Handle(), Dest->left, Dest->top, Dest->right, Dest->bottom,
                bmpDC, Source->left, Source->top, Source->right, Source->bottom, FRasterOp );

    // Resets every thing back after a stretch
    SetBrushOrgEx( pBitmap->Handle(), 0, 0, NULL );

}   // StretchDraw


/************************************************************************
*    DESCRIPTION: Fill the DC with a color 
*
*    FUNCTION PARAMETERS:
*    Input:    RECT r     - Rect structure containing the stretched coordinates
************************************************************************/
void CBitmap::ColorRect( LPRECT r, int BrushColor )
{
    if( bmpDC )
        FillRect( bmpDC, r, (HBRUSH)GetStockObject( BrushColor ) );

}   // ColorRect


/************************************************************************
*    DESCRIPTION: Fill the DC with a color. Pass it the handle to the brush.                            
*
*    FUNCTION PARAMETERS:
*    Input:    RECT r     - Rect structure containing the stretched coordinates
*              HBRUSH hBrush - handle to a brush
************************************************************************/
void CBitmap::ColorRect( LPRECT r, HBRUSH hBrush )
{
    if( bmpDC )
        FillRect( bmpDC, r, hBrush );

}  // ColorRect


/************************************************************************
*    DESCRIPTION: Fill the DC with a color. Creates and deletes
*                 the brush it makes. 
*
*    FUNCTION PARAMETERS:
*    Input:    RECT r     - Rect structure containing the stretched coordinates
*              HBRUSH hBrush - handle to a brush
************************************************************************/
void CBitmap::ColorRect( LPRECT r, COLORREF colorRef )
{
    HBRUSH hBrush;

    if( bmpDC )
    {
        // Create the brush
        hBrush = CreateSolidBrush( colorRef );

        // Make sure we have the brush
        if( hBrush )
        {
            // Fill the rect
            FillRect( bmpDC, r, hBrush );

            // Delete the brush
            DeleteBrush( hBrush );
        }
    }
}  // ColorRect


/************************************************************************
*    DESCRIPTION: Do we have a DC and a Windows bitmap sitting in here 
************************************************************************/
bool CBitmap::IsEmpty()
{
    bool Result;

    if( bmpDC && hBitmap )
        Result = false;
    else
        Result = CDib::IsEmpty();

    return Result;
}


/************************************************************************
*    DESCRIPTION: Reblit the image to the DC 
************************************************************************/
void CBitmap::ResetImage()
{
    if( bmpDC || hBitmap )
    { 
        StretchDIBits( bmpDC, 0, 0, width(), height(),
                       0, 0, width(), height(), bits(), dib(), DIB_RGB_COLORS, SRCCOPY );
    }
}   // ResetImage


/************************************************************************
*    DESCRIPTION: Returns the width. If the CBitmap class has
*                 one then it is used other wise it tries to
*                 use the TDIB class.
************************************************************************/
int CBitmap::width()
{ 
    int Result = 0;

    if( Fwidth )
        Result = Fwidth;
    else
        if( !IsEmpty() )
            Result = CDib::width();

    return Result;
     
}   // width()


/************************************************************************
*    DESCRIPTION: Returns the height. If the CBitmap class has
*                 one then it is used other wise it tries to
*                 use the TDIB class.
************************************************************************/
int CBitmap::height()
{ 
    int Result = 0;

    if( Fheight )
        Result = Fheight;
    else
        if( !IsEmpty() )
            Result = CDib::height();

    return Result;
     
}   // width()


/************************************************************************
*    DESCRIPTION: Scale one bitmap into another.
*
*    FUNCTION PARAMETERS:
*    Input:    CBitmap *pBitmap
************************************************************************/
void CBitmap::ScaleToBuffer16Bit( CBitmap *pBitmap )
{
    WORD register *Source, *Dest;
    float xIndex, yIndex=0, xStep, yStep;
    int i, j, w, h, SourceW, sourceOffset = 0;

    // Set the pointers
    Dest = (WORD *)GetPtrToDIBsectionBits();
    Source = (WORD *)pBitmap->GetPtrToDIBsectionBits();

    // Make sure we have a pointer
    if( !Dest || !Source )
        return;

    // Calculate the scaling factor
    xStep = (float)pBitmap->width() / (float)width();
    yStep = (float)pBitmap->height() / (float)height();

    // Get the width and height
    w = width();
    h = height();
    SourceW = pBitmap->width();

    // render the sized bitmap
    for( i = 0; i < h; ++i )
    {
        xIndex = 0;

        for( j = 0; j < w; ++j )
        {
            Dest[ (i * w) + j ] = Source[ sourceOffset + (int)xIndex ];

            xIndex += xStep;
        }

        yIndex += yStep;
        sourceOffset = SourceW * (int)yIndex;
    }

}   // ScaleBitmap


/************************************************************************
*    DESCRIPTION: Renders a 16 bit color line
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
************************************************************************/
void CBitmap::Line16bit( int X1, int Y1, int X2, int Y2, WORD aColor, int renderType )
{
    int xSlope, ySlope, xInc, yInc, error(0), index;
    int yOffset, xOffset, yMax(Fwidth * (Fheight-1)), xMax( Fwidth ); 

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register color(aColor), *pBuffer;

    // Init the video buffer pointer
    pBuffer = (WORD *)ppDIBSectBits;
    // Color depth specific data types
    ///////////////////////////////////////////

    // Make sure we have a buffer to render the line to
    if( pBuffer )
    {
        // Compute the slope                         
        xSlope = X2 - X1;                         
        ySlope = Y2 - Y1;

        // init the offsets
        xOffset = X1;
        yOffset = Y1 * xMax;

        // test the X direction of the slope
        if( xSlope >= 0 )
        {
            // line is moving right
            xInc = 1;
        }
        else
        {
            // line is moving left
            xInc = -1;

            // need an absolute value
            xSlope = -xSlope;
        }

        // test Y direction of slope
        if( ySlope >= 0 )
        {
            // line is moving down
            yInc = xMax;
        }
        else
        {           
            // line is moving up
            yInc = -xMax;

            // need an absolute value
            ySlope = -ySlope;
        }

        // Based on which slope is greater we can plot the path
        if( xSlope > ySlope )
        {           
            for( index = 0; index < xSlope; ++index )
            {                
                // Clip the line
                if( yOffset > -1 && yOffset < yMax &&
                    xOffset > -1 && xOffset < xMax )
                {
                    // Set the pixel color
                    if( renderType == RENDER_COPY )
                        pBuffer[ yOffset + xOffset ] = color;

                    else if( renderType == RENDER_XOR )
                        pBuffer[ yOffset + xOffset ] ^= color;

                    else if( renderType == RENDER_AND )
                        pBuffer[ yOffset + xOffset ] &= color;

                    else if( renderType == RENDER_OR )
                        pBuffer[ yOffset + xOffset ] |= color;
                }

                // adjust the error factor
                error += ySlope;

                // test if error overflowed
                if( error > xSlope )
                {
                    // Reset the error factor
                    error -= xSlope;

                    // Inc the Y movement
                    yOffset += yInc;
                }

                // Inc the X movement
                xOffset += xInc;
            }
        }
        else
        {
            for( index = 0; index < ySlope; ++index )
            {
                // Clip the line
                if( yOffset > -1 && yOffset < yMax &&
                    xOffset > -1 && xOffset < xMax )
                {
                    // Set the pixel color
                    if( renderType == RENDER_COPY )
                        pBuffer[ yOffset + xOffset ] = color;

                    else if( renderType == RENDER_XOR )
                        pBuffer[ yOffset + xOffset ] ^= color;

                    else if( renderType == RENDER_AND )
                        pBuffer[ yOffset + xOffset ] &= color;

                    else if( renderType == RENDER_OR )
                        pBuffer[ yOffset + xOffset ] |= color;
                }

                // adjust the error factor
                error += xSlope;

                // test if error overflowed
                if( error > 0 )
                {
                    // Reset the error factor
                    error -= ySlope;

                    // Inc the X movement
                    xOffset += xInc;
                }

                // Inc the Y movement
                yOffset += yInc;
            }
        }
    }           
}   // Line16bit