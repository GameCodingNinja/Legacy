//
// File name: ScrnBuff.h
//
// Description: The CreateDIBSection object.
//
// Author: John De Goes
//
// Project:
//
// Target:
//

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

#include "ScrnBuff.h"

struct TLogpalette
{
    WORD          palVersion; 
    WORD          palNumEntries; 
    PALETTEENTRY  palPalEntry[256];        
};
typedef TLogpalette *PTLogpalette;

struct TBitmapinfo
{
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD          bmiColors[256];
};
typedef TBitmapinfo *PTBitmapinfo;


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

TScreenBuffer::TScreenBuffer()
{
    // Init Class Members
    hVBuffer = NULL;
    Bits = NULL;
    hPalette = NULL;
    hVBuffer = NULL;
    OffsetX = OffsetY = VBWidth = VBHeight = 0;
    hCurrentPen = NULL;
    hVBufDC = NULL;

}   // Constructer



/************************************************************************
*    FUNCTION NAME:         InitVBuffer()                                                             
*
*    DESCRIPTION:           Allocate the video buffer and palette if needed
*
*    FUNCTION PARAMETERS:
*    Input:    WORD Width   - Width of video buffer
*              WORD Height  - Height of video buffer
*              HWND hwnd    - Handle to active window
*              RGBQUAD *Pal - Pointer to init palette
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

BOOL TScreenBuffer::InitVBuffer( int Width, int Height, HWND hwnd, RGBQUAD *Pal, HDC hdc )
{
    int N; Pal;
    BOOL Result = FALSE;
    TLogpalette PalInfo;
    TBitmapinfo BMInfo;

    VBWidth = Width;
    VBHeight = Height;
    ActiveWindow = hwnd;

    Free();

    // Get the DC to the active window and make sure we have a valid DC
	if( ActiveWindow )
	{
	    ScreenDC = GetDC ( ActiveWindow );
	    if( !ScreenDC )
	        goto HANDLE_ERROR;
	}
	else if( hdc )
		ScreenDC = hdc;

    // Zero out the TBitmapinfo structure
    memset( &BMInfo, 0, sizeof( TBitmapinfo ) );

    // Init the TBitmapinfo structure for creating the DIB section
    BMInfo.bmiHeader.biSize          = sizeof ( BITMAPINFOHEADER );
    BMInfo.bmiHeader.biWidth         = Width;
    BMInfo.bmiHeader.biHeight        = -abs( Height ); // Top-down bitmap
    BMInfo.bmiHeader.biPlanes        = 1;
    BMInfo.bmiHeader.biBitCount      = VBUFFER_BIT_COUNT;
    // Its faster if the bits per pixel match the screen
    //BMInfo.bmiHeader.biBitCount      = GetDeviceCaps( ScreenDC, BITSPIXEL );
    BMInfo.bmiHeader.biCompression   = BI_RGB;
    BMInfo.bmiHeader.biClrUsed       = 256;
    BMInfo.bmiHeader.biClrImportant  = 256;

    // Make the palette if we need one
    if( Pal != NULL )
    {
        // Copy over the palette to the TBitmapinfo
        for( N = 0; N < 256; ++N )
            BMInfo.bmiColors[ N ] = Pal[ N ];

        // If the system is in 8 bit or less, create the palette
        if( GetDeviceCaps( ScreenDC, BITSPIXEL ) <= VBUFFER_BIT_COUNT )
        {
            PalInfo.palVersion = 0x300;
            PalInfo.palNumEntries = 256;

            for ( N = 0; N < 256; ++N )
            {
                PalInfo.palPalEntry[ N ].peRed   = Pal[ N ].rgbRed;
                PalInfo.palPalEntry[ N ].peGreen = Pal[ N ].rgbGreen;
                PalInfo.palPalEntry[ N ].peBlue  = Pal[ N ].rgbBlue;
                PalInfo.palPalEntry[ N ].peFlags = PC_NOCOLLAPSE;
            }

            // Create the palette and make sure we have a valid handle
            hPalette = CreatePalette ( ( tagLOGPALETTE * ) &PalInfo );
            if( !hPalette )
                goto HANDLE_ERROR;
        }
    }

    // Create the DIB Section that we use for the video buffer
    hVBuffer = CreateDIBSection( ScreenDC, ( BITMAPINFO * ) &BMInfo, DIB_RGB_COLORS, 
                               ( VOID * * ) &Bits, NULL, NULL );
    if( !hVBuffer )
        goto HANDLE_ERROR;

    // Create the compatable DC
    hVBufDC = CreateCompatibleDC ( ScreenDC );
    if( !hVBufDC )
        goto HANDLE_ERROR;

    // Select the video buffer into the compatible DC
    hOldBmp = SelectBitmap ( hVBufDC, hVBuffer );
    if( !hOldBmp )
        goto HANDLE_ERROR;

    // If we made it this far, everything should be ok.
    Result = TRUE;

    HANDLE_ERROR:;

    return Result;

}   // InitBuffer



/************************************************************************
*    FUNCTION NAME:         Show()                                                             
*
*    DESCRIPTION:           Blit the video buffer to the screen
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

void TScreenBuffer::Show()
{      
    BitBlt( ScreenDC, OffsetX, OffsetY, VBWidth, VBHeight, hVBufDC, 0, 0, SRCCOPY );

}   // Show


/************************************************************************
*    FUNCTION NAME:         Colors()                                                             
*
*    DESCRIPTION:           Realize the palette
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

void TScreenBuffer::Colors()
{
    if( hPalette )
    {
        SelectPalette( ScreenDC, hPalette, FALSE );
        RealizePalette( ScreenDC );
    }
}  // Colors


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TScreenBuffer::~TScreenBuffer() 
{ 
    Free();
}   // Destructor



/************************************************************************
*    FUNCTION NAME:         Free()                                                             
*
*    DESCRIPTION:           Realize the palette
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

void TScreenBuffer::Free()
{
    // Delete the palette handle:
    if( hPalette )
    {
        DeleteObject( hPalette );
        hPalette = NULL;
    }

    // Delete the pen
    FreePen();

    // Delete the video buffer
    if( hVBuffer )
    {
        SelectBitmap( hVBufDC, hOldBmp );
        DeleteDC( hVBufDC );
        DeleteObject( hVBuffer );

		if( ActiveWindow )
        	ReleaseDC( ActiveWindow, ScreenDC );

        hVBuffer = NULL;
    }

}  // Colors


/************************************************************************
*    FUNCTION NAME:         CreateNewPen()                                                             
*
*    DESCRIPTION:           Create a color pen
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

void TScreenBuffer::CreateNewPen( int Style, int PixelWidth, COLORREF color )
{
    FreePen();

    // Create the pen
    hCurrentPen = CreatePen( Style, PixelWidth, color );

    // Select the pen if we have one
    if( hCurrentPen )
        hOldPen = SelectPen( hVBufDC, hCurrentPen );

}   // CreateNewPen



/************************************************************************
*    FUNCTION NAME:         FreePen()                                                             
*
*    DESCRIPTION:           Create a color pen
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

void TScreenBuffer::FreePen()
{
    // Delete the pen
    if( hCurrentPen )
    {
        SelectPen( hVBufDC, hOldPen );
        DeletePen( hCurrentPen );
        hCurrentPen = NULL;
    }
}