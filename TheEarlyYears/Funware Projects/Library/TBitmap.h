
/************************************************************************
*
*    PRODUCT:         Bitmap Manipulation
*
*    FILE NAME:       TBitmap.h
*
*    DESCRIPTION:     TBitmap object for using windows GDI graphics
*
*    IMPLEMENTATION:  Load and blits bitmaps
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __TBITMAP_H__
#define __TBITMAP_H__

#include "dib.h"             // Dib class for displaying bitmaps


/************************************************************************
*                         TBitmap Object
************************************************************************/

class TBitmap : public TDIB
{
protected:
    // Handles to the HBITMAP
    HBITMAP hOldBmp, hBitmap;
    // Handle to the Compatible DC
    HDC bmpDC;
    // Width and height of HBITMAP
    int Fwidth, Fheight;
    // Pointer to HBITMAP bits
    void *ppDIBSectBits;
    // The Terminator for this bitmap
    int Terminator;
    // Stretch Blit Mode
    int StretchBlitMode;
    // This bitmaps rect
    RECT rect;
    // Dib Section Bit Depth
    int DibSectionBitDepth;

    // Rect clipping function
    //BOOL ClipRects( TBitmap *pBitmap, RECT &Dest, RECT &Source );

public:

    // Constructor
    TBitmap();
    // Destructor
    ~TBitmap(){ Free(); };

    // Creates a standard windows bitmap or a DIBsection bitmap
    BOOL CreateBitmap( int BitmapType, int BitCount = 0, int orientation = BOTTOM_UP_BMP );
    // Create the compatible DC and DIB section buffer
    BOOL CreateDIBSectionBuffer( int Width, int Height, int BitCount = 0, RGBQUAD *pPalette = NULL );
    // Create a standard DIB buffer
    BOOL CreateStandardBuffer( int Width, int Height );
    // Return the DC
    HDC Handle(){ return bmpDC; };
    // Return the Width of the HBITMAP or the DIB
    virtual int width();
    // Return the Height of the HBITMAP or the DIB
    virtual int height();
    // Draw to window handle
    void Draw( HWND hDest, int X = 0, int Y = 0 );
    // Draw to DC
    void Draw( HDC DestDC, int X = 0, int Y = 0 );
    // Draw to TBitmap
    void Draw( TBitmap *pBitmap, int X = 0, int Y = 0 );
    // Draw to TBitmap
    void Draw( TBitmap *pBitmap, POINT *Dest, RECT *Source );
    // Draw a portion of the bitmap to the screen
    void Draw( HDC DestDC, POINT *Dest, RECT *Source );
    // Draw a portion of the bitmap to the screen
    void Draw( HWND hDest, POINT *Dest, RECT *Source );
    // Draw to window handle
    void Draw( HWND hDest, RECT *SourceDest );
    // Draw to DC
    void Draw( HDC DestDC, RECT *SourceDest );
    // Draw to TBitmap
    void Draw( TBitmap *pBitmap, RECT *SourceDest );
    // Draw to TBitmap
    void Draw( TBitmap *pBitmap, RECT *Dest, RECT *Source );
    // Draw a portion of the bitmap to the screen
    void Draw( HDC DestDC, RECT *Dest, RECT *Source );
    // Draw a portion of the bitmap to the screen
    void Draw( HWND hDest, RECT *Dest, RECT *Source );
    // StretchDraw to window handle
    void StretchDraw( HWND hDest, RECT *r );
    // StretchDraw to window handle
    void StretchDraw( HWND hDest, RECT *Dest, RECT *Source );
    // StretchDraw to DC
    void StretchDraw( HDC DestDC, RECT *r );
    // StretchDraw to DC
    void StretchDraw( HDC DestDC, RECT *Dest, RECT *Source );
    // StretchDraw to another TBitmap
    void StretchDraw( TBitmap *pBitmap, RECT *r );
    // StretchDraw to another TBitmap
    void StretchDraw( TBitmap *pBitmap, RECT *Dest, RECT *Source );
    // Fill a rectangle in the DC with a color. Only uses stock brushes
    void ColorRect( LPRECT r, int BrushColor );
    // Fill a rectangle in the DC with a color. Pass it any brush you want
    void ColorRect( LPRECT r, HBRUSH hBrush );
    // Fill a rectangle in the DC with a color. Creates and deletes the brush it makes
    void ColorRect( LPRECT r, COLORREF colorRef );
    // Reblit the image to it's own DC
    void ResetImage();
    // Is the HBITMAP empty?
    virtual BOOL IsEmpty();
    // Returns the pointer to the DIB section bits
    void *GetPtrToDIBsectionBits(){ return ppDIBSectBits; };
    // Get the dib section Terminator
    int GetDIBsectionTerminator(){ return Terminator; };
    // Free handle to compatible bitmap and DC
    void Free();
    // Get the rect of this bitmap
    LPRECT GetRect(){ return &rect; };
    // Get the Dib Section Bit Depth
    int GetDibSectionBitDepth(){ return DibSectionBitDepth; };
    // Set the Stretch Blit Mode
    void SetStretchBlitMode( int Value ){ StretchBlitMode = Value; };
    // Scale one bitmap into another.
    void ScaleToBuffer16Bit( TBitmap *pBitmap );
    // Renders a 16 bit color line
    void Line16bit( int X1, int Y1, int X2, int Y2, WORD aColor, int renderType = RENDER_COPY );
    // Software blit
    //void Blit( TBitmap *pBitmap, RECT &Dest, RECT &Source, BOOL clipRect = TRUE );
};

typedef TBitmap *PTBitmap;


#endif  /* __TBITMAP_H__ */

