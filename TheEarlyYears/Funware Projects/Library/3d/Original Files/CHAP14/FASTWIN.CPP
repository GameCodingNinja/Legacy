//
// File name: FastWin.CPP
//
// Description: The CreateDIBSection object.
//
// Author: John De Goes
//
// Project:
//
// Target:
//

#include "FastWin.HPP"

void FastWin::Init ( DWORD Width, DWORD Height, RGBQUAD *Pal )
   {
   long N; Pal;
   HWND ActiveWindow = GetActiveWindow ();

   HDC ScreenDC = GetDC ( ActiveWindow );

   BMInfo  = new MyBITMAPINFO;
   PalInfo = new MyLOGPALETTE;
   
   HPALETTE PalHan;
   
   BMInfo->bmiHeader.biSize          = sizeof ( BITMAPINFOHEADER );
   BMInfo->bmiHeader.biWidth         = Width;
   BMInfo->bmiHeader.biHeight        = -abs ( Height ); // Top-down bitmap
   BMInfo->bmiHeader.biPlanes        = 1;
   BMInfo->bmiHeader.biBitCount      = 8;
   BMInfo->bmiHeader.biCompression   = BI_RGB;
   BMInfo->bmiHeader.biSizeImage     = NULL;
   BMInfo->bmiHeader.biXPelsPerMeter = NULL;
   BMInfo->bmiHeader.biYPelsPerMeter = NULL;
   BMInfo->bmiHeader.biClrUsed       = 256;
   BMInfo->bmiHeader.biClrImportant  = 256;

   if ( Pal != NULL )
      {
      for ( N = 0; N < 256; N++ )
          {
          BMInfo->bmiColors [ N ] = Pal [ N ];
          }
      PalInfo->palVersion = 0x300;
      PalInfo->palNumEntries = 256;
      for ( N = 0; N < 256; N++ )
          {
          PalInfo->palPalEntry [ N ].peRed   = Pal [ N ].rgbRed;
          PalInfo->palPalEntry [ N ].peGreen = Pal [ N ].rgbGreen;
          PalInfo->palPalEntry [ N ].peBlue  = Pal [ N ].rgbBlue;
          PalInfo->palPalEntry [ N ].peFlags = PC_NOCOLLAPSE;// | PC_RESERVED;
          }
      PalHan = CreatePalette ( ( tagLOGPALETTE * ) PalInfo );
      SelectPalette ( ScreenDC, PalHan, FALSE );
      RealizePalette ( ScreenDC );
      DeleteObject ( PalHan );
      }

   BMHan = CreateDIBSection ( ScreenDC, ( BITMAPINFO * ) BMInfo, DIB_RGB_COLORS, 
                              ( VOID * * ) &Bits, NULL, NULL );

   ReleaseDC ( ActiveWindow, ScreenDC );
   }

void FastWin::Show ()
   {
   HWND ActiveWindow; HDC ScreenDC;
   RECT Dest;
   
   if ( ( ActiveWindow = GetActiveWindow () ) == NULL )
      return;
   ScreenDC = GetDC ( ActiveWindow );
   GetClientRect ( ActiveWindow, &Dest );

   HDC Context = CreateCompatibleDC ( 0 );
   HBITMAP DefaultBitmap = SelectBitmap ( Context, BMHan );
   
   BitBlt ( ScreenDC,
                   0, // x-coordinate of upper-left corner of destination rectangle 
                   0, // y-coordinate of upper-left corner of destination rectangle 
          Dest.right - Dest.left, // width of destination rectangle 
         Dest.bottom - Dest.top, // height of destination rectangle 
               Context, // handle of source device context 
                   0, // x-coordinate of upper-left corner of source rectangle 
                   0, // y-coordinate of upper-left corner of source rectangle
              SRCCOPY // raster operation code 
          );
      
   SelectBitmap ( Context, DefaultBitmap );
   DeleteDC ( Context );
   DeleteObject ( DefaultBitmap );
   ReleaseDC ( ActiveWindow, ScreenDC );
   }

void FastWin::Colors ()
   {
   // Get the active window:
   HWND ActiveWindow = GetActiveWindow ();
   HDC ScreenDC = GetDC ( ActiveWindow );
   HPALETTE PalHan;

   // Create, select, and realize a palette:
   PalHan = CreatePalette ( ( tagLOGPALETTE * ) PalInfo );
   SelectPalette ( ScreenDC, PalHan, FALSE );
   RealizePalette ( ScreenDC );

   // Release the screen device context:
   ReleaseDC ( ActiveWindow, ScreenDC );

   // Delete the palette handle:
   DeleteObject ( PalHan );
   }