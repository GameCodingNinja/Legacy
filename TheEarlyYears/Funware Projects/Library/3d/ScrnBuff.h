//
// File name: FastWin.h
//
// Description: The CreateDIBSection object.
//
// Author: John De Goes
//
// Project:
//
// Target:
//

#ifndef SCREEN_BUFFER
#define SCREEN_BUFFER

#include <Windows.H>
#include <WindowsX.H>

#define VBUFFER_BIT_COUNT      8

class TScreenBuffer 
{
protected:
    // Handle to the screen DC
    HDC ScreenDC;
    // Handle to the active window
    HWND ActiveWindow;
    // Handle to video buffer DC
    HDC hVBufDC;
    // Handle to old bitmap
    HBITMAP hOldBmp;
    // Size of the video buffer
    int OffsetX, OffsetY, VBWidth, VBHeight;
    // Handle to the palette
    HPALETTE hPalette;
    // Handle to the video buffer bitmap
    HBITMAP hVBuffer;
    // Pointer to variable to receive a pointer to the bitmap's bit values     
    BYTE *Bits;
    // Pen
    HPEN hCurrentPen, hOldPen;

public:

    // Constructor
    TScreenBuffer();
    // Destructor
    ~TScreenBuffer();
    // Create the video buffer
    BOOL InitVBuffer( int Width, int Height, HWND hwnd, RGBQUAD *Pal = NULL, HDC hdc = NULL );
    // Get a pointer to the buffers bits
    BYTE *GetPtrToBits(){ return Bits; };
    // Blit the buffer to the screen
    void Show();
    // Realize the palette
    void Colors();
    // Get the buffers DC
    HDC GetVBufferDC(){ return hVBufDC; }
    // Free all allocated memory
    void Free();
    // Free the pen
    void FreePen();
    // Clear out the video buffer with one of the indexes of the palette entry
    void ClearVideoBuffer( BYTE Value ){ if( Bits ) memset( Bits, Value, sizeof(BYTE) * (VBWidth * VBHeight) ); }
    // Set the video buffer offsets
    void SetXYOffsets( int X, int Y ){ OffsetX = X; OffsetY = Y; }
    // Create a pen to use
    void CreateNewPen( int Style, int PixelWidth, COLORREF color );
    // Is the buffer ready?
    BOOL IsEmpty(){ return (hVBufDC == NULL); };
};

#endif
