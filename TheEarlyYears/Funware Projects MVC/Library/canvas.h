
/************************************************************************
*
*    PRODUCT:         Screen Output Output
*
*    FILE NAME:       canvas.cpp
*
*    DESCRIPTION:     CCanvas for printing to the screen
*
*    IMPLEMENTATION:  
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __CANVAS_H__
#define __CANVAS_H__

#include "minTools.h"      // A misc group of useful functions


class CCanvas
{
protected:

    HDC hDeviceDC;
    HFONT hFont, hOldFont;
    HPEN hPen, hOldPen;
    HBRUSH hBrush, hOldBrush;
    int BackgroundMode;
    COLORREF BackgroundColor;
    COLORREF ForgroundColor;
    COLORREF TextColor;

public:
    // Constructor
    CCanvas();
    // Destructor
    ~CCanvas();
    HDC Handle(){ return hDeviceDC; };
    // Prints text to the printer
    void DrawText( int X, int Y, char *Text );
	// Draw formatted text
	void DrawFormatedText( int x, int y, char *text, ... );
    // Create a font for the printer and select it into the DC
    void SetFont( char *Name, int Size, WORD Style = FONT_REGULAR, COLORREF mColor = DEFAULT_BLACK, int Angle = 0, LOGFONT *lf = NULL );
    // Create a pen for the screen and select it into the DC
    void SetPen( int PenStyle, int PenWidth, COLORREF PenColor );
    // Create a brush for the screen and select it into the DC
    void SetBrush( COLORREF BrushColor );
    // Gets the width of the text string
    int TextWidth( char *text );
    // Gets the width of the text string
    int TextHeight( char *text );
    // Formats a block of text to the printer
    DWORD WrapText( char *Text, int LeftMargin, int TopMargin, int TextWidth, int BottomMargin = 0, int Leading = 0, int Justify = LEFT_JUSTIFY );
    // Draw a line to the DC
    void Line( int Left, int Top, int Right, int Bottom );
    // Draw a box to the DC
    void Fill( int Left, int Top, int Right, int Bottom );
    // Set the background color
    void SetBackgroundColor( COLORREF BkColor );
    // Set the background color
    void SetForgroundColor( COLORREF color );
    // Set the text color
    void SetTextColorToDC( COLORREF color );
    // Set the background mode
    void SetBackgroundMode( int BkMode );
    // Use a DC from the outside for all the graphic initilizations
    void InitActiveDC( HDC DC );
    // Free all allocations
    void Free();
};


typedef CCanvas *PCCanvas;


#endif  /* __PRINTER__ */