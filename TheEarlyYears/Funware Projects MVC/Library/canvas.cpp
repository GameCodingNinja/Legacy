
/************************************************************************
*
*    PRODUCT:         Screen Output
*
*    FILE NAME:       CCanvas.cpp
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

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "canvas.h"         // Header file for this *.cpp file.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf



/************************************************************************
*    FUNCTION NAME:              Constructer
************************************************************************/
CCanvas::CCanvas()
{
    // Init Class members
    hFont = NULL;
    hPen = NULL;
    hBrush = NULL;
    hDeviceDC = NULL;
    BackgroundMode = OPAQUE;
    BackgroundColor = 0;
    ForgroundColor = RGB( 256, 256, 256 );
    TextColor = ForgroundColor;

}   // CCanvas


/************************************************************************
*    FUNCTION NAME:              Destructor                                                             
************************************************************************/
CCanvas::~CCanvas()
{
    Free();

}   // Destructor


/************************************************************************
*    DESCRIPTION:           Prints a line of text to the screen
*
*    FUNCTION PARAMETERS:
*    Input:    int X - X coordinate
*              int Y - Y coordinate
*              char Text - Text to be printed
************************************************************************/
void CCanvas::DrawText( int X, int Y, char *Text )
{
    if( hDeviceDC )
        TextOut( hDeviceDC, X, Y, Text, lstrlen(Text) );

}   // TextOut


/************************************************************************
*    DESCRIPTION:           Prints a line of text to the screen
*
*    FUNCTION PARAMETERS:
*    Input:    int X - X coordinate
*              int Y - Y coordinate
*              char Text - Text to be printed
************************************************************************/
void CCanvas::DrawFormatedText( int x, int y, char *text, ... )
{
    if( hDeviceDC )
	{
		va_list argptr;
    	char strBuffer[1024];

    	va_start( argptr, text );
    	vprintf( text, argptr );
    	wvsprintf( strBuffer, text, argptr );

        TextOut( hDeviceDC, x, y, strBuffer, strlen(strBuffer) );

		va_end( argptr );
	}

}   // DrawFormatedText


/************************************************************************
*    DESCRIPTION:           Creates a font and selects it into the DC
*
*    FUNCTION PARAMETERS:
*    Input:    char *Name  - Font Name
*              int Size    - Size of the font
*              WORD Style  - Style of the font
*              COLORREF mColor - Color of the font
*              int Angle   - Angle of the font
************************************************************************/
void CCanvas::SetFont( char *Name, int Size, WORD Style, COLORREF mColor, int Angle, LOGFONT *lf )
{
    // If we currently have a font loaded, select the old one back in
    // and delete our current one
    if( hDeviceDC )
    {
        if( hFont )
        {
            SelectFont( hDeviceDC, hOldFont );
            DeleteFont( hFont );
        }

        // Create the font
        hFont = MakeFont( hDeviceDC, Name, Size, Style, mColor, Angle, lf );

        // Select the new font into the divice context
        if( hFont )
            hOldFont = SelectFont( hDeviceDC, hFont );

        // record the font color
        TextColor = mColor;
    }

}   // SetFont


/************************************************************************
*    DESCRIPTION:           Creates a pen and selects it into the DC
*
*    FUNCTION PARAMETERS:
*    Input:    int PenStyle  - Pen style
*              int PenWidth  - Pen width
*              COLORREF PenColor  - pen color
************************************************************************/
void CCanvas::SetPen( int PenStyle, int PenWidth, COLORREF PenColor )
{
    // If we currently have a pen loaded, select the old one back in
    // and delete our current one
    if( hDeviceDC )
    {
        if( hPen )
        {
            SelectPen( hDeviceDC, hOldPen );
            DeletePen( hPen );
        }

        // Create the pen
        hPen = CreatePen( PenStyle, PenWidth, PenColor );

        // Select the new pen into the divice context
        if( hPen )
            hOldPen = SelectPen( hDeviceDC, hPen );
    }

}   // SetPen


/************************************************************************
*    DESCRIPTION:           Creates the font and selects it into the printer DC
*
*    FUNCTION PARAMETERS:
*    Input:    COLORREF BrushColor - brush color
************************************************************************/
void CCanvas::SetBrush( COLORREF BrushColor )
{
    // If we currently have a brush loaded, select the old one back in
    // and delete our current one
    if( hDeviceDC )
    {
        if( hBrush )
        {
            SelectBrush( hDeviceDC, hOldBrush );
            DeleteBrush( hBrush );
        }

        // Create the brush
        hBrush = CreateSolidBrush( BrushColor );

        // Select the new brush into the divice context
        if( hBrush )
            hOldBrush = SelectBrush( hDeviceDC, hBrush );
    }

}   // SetBrush


/************************************************************************
*    DESCRIPTION:           Gets the width of the text string using the
*                           currently selected font
*
*    FUNCTION PARAMETERS:
*    Input:    char *text - Text string to get width of
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
************************************************************************/
int CCanvas::TextWidth( char *text )
{
    SIZE size;
    int Result = 0;

    // Check to ensure the function didn't fail
    if( hDeviceDC )
        if( GetTextExtentPoint32( hDeviceDC, text, strlen(text), &size ) )
            Result = size.cx;

    return Result;

}   // TextWidth


/************************************************************************
*    DESCRIPTION:           Gets the height of the text string using the
*                           currently selected font
*
*    FUNCTION PARAMETERS:
*    Input:    char *text - Text string to get width of
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
************************************************************************/
int CCanvas::TextHeight( char *text )
{
    SIZE size;
    int Result = 0;

    // Check to ensure the function didn't fail
    if( hDeviceDC )
        if( GetTextExtentPoint32( hDeviceDC, text, strlen(text), &size ) )
            Result = size.cy;

    return Result;

}   // TextHeight


/************************************************************************
*    DESCRIPTION:           Send text output to a device be it screen, printer
*                           or what not. This function uses the supplied
*                           DC to calculate the width and height of the 
*                           text due to the selected font currently in the DC.
*                           The LeftMargin, TopMargin, TextWidth values are
*                           in pixels amounts and that value depends on what
*                           device you are printing to. 
*
*    FUNCTION PARAMETERS:
*    Input:    char *Text      - Pointer to the
*              int LeftMargin  - Size of the margin in pixels
*              int TopMargin   - Starting point
*              int TextWidth   - The width of the text to be printed
*              int BottomMargin- The bottom where the text will stop
*              int Leading     - Additional ledding to add to each line 
*
*    Output:   DWORD - Return the top margin where the next line of text can be drawn
************************************************************************/
DWORD CCanvas::WrapText( char *Text, int LeftMargin, int TopMargin, int TextWidth, int BottomMargin, int Leading, int Justify )
{
    DWORD Result = 0;

    // Send formatted output to the screen
    if( hDeviceDC )
        Result = WrapTextToDevice( hDeviceDC, Text, LeftMargin, TopMargin, TextWidth, BottomMargin, Leading, SCREEN_OUTPUT, Justify );

    return Result;

}   // WrapText


/************************************************************************
*    DESCRIPTION:           Draw a line to the DC 
*
*    FUNCTION PARAMETERS:
*    Input:    int Left        - Pointer to the
*              int Top         - Size of the margin in pixels
*              int Right       - Starting point
*              int Bottom      - The width of the text to be printed 
************************************************************************/
void CCanvas::Line( int Left, int Top, int Right, int Bottom )
{
    if( hDeviceDC && hPen )
    {
        MoveToEx( hDeviceDC, Left, Top, NULL );
        LineTo( hDeviceDC, Right, Bottom );
    }

}   // Line


/************************************************************************
*    DESCRIPTION:           Draw a box to the DC 
*
*    FUNCTION PARAMETERS:
*    Input:    int Left
*              int Top
*              int Right
*              int Bottom      - Line coordinates 
************************************************************************/
void CCanvas::Fill( int Left, int Top, int Right, int Bottom )
{
    if( hDeviceDC && hBrush )
        Rectangle( hDeviceDC, Left, Top, Right, Bottom );

}   // Fill


/************************************************************************
*    DESCRIPTION:           Use a DC from the outside for all the graphic
*                           initilizations. If we are switching from one
*                           DC to another, we need to pull out of the current
*                           one and reapply everything to the next one.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
************************************************************************/
void CCanvas::InitActiveDC( HDC DC )
{ 
    // if we currently have an active DC, we neeed to pull
    // out of that one and reapply everything to the new one
    if( hDeviceDC )
    {
        if( hFont )
        {
            // Pull the font out of the current DC
            SelectFont( hDeviceDC, hOldFont );

            // Apply the font to the new DC
            hOldFont = SelectFont( DC, hFont );
        }

        if( hPen )
        {
            // Pull the pen out of the current DC
            SelectPen( hDeviceDC, hOldPen );

            // Apply the font to the new DC
            hOldPen = SelectPen( DC, hPen );
        }

        // Delete the selected brush
        if( hBrush )
        {
            // Pull the bruch out of the current DC
            SelectBrush( hDeviceDC, hOldBrush );

            // Apply the brush to the new DC
            hOldBrush = SelectBrush( DC, hBrush );
        }
    }

    // init to the DC
    hDeviceDC = DC;

    // Set the defaults
    SetBackgroundColor( BackgroundColor );
    SetForgroundColor( ForgroundColor );
    SetBackgroundMode( BackgroundMode );
    SetTextColorToDC( TextColor );
     
}   // InitActiveDC


/************************************************************************
*    DESCRIPTION:           Set the background color
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
************************************************************************/
void CCanvas::SetBackgroundColor( COLORREF BkColor )
{ 
    if( hDeviceDC ) 
        SetBkColor( hDeviceDC, BkColor );

    BackgroundColor = BkColor;
     
}   // SetBackgroundColor


/************************************************************************
*    DESCRIPTION:     Set the forground color
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
************************************************************************/
void CCanvas::SetForgroundColor( COLORREF color )
{ 

    ForgroundColor = color;

}   // SetForgroundColor


/************************************************************************
*    DESCRIPTION:           Set the background mode
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
************************************************************************/
void CCanvas::SetBackgroundMode( int BkMode )
{
    if( hDeviceDC )
        SetBkMode( hDeviceDC, BkMode );

    BackgroundMode = BkMode;

}   // SetBackgroundMode


/************************************************************************
*    DESCRIPTION:           Set the text color
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
************************************************************************/
void CCanvas::SetTextColorToDC( COLORREF color )
{ 
    if( hDeviceDC )
        SetTextColor( hDeviceDC, color );

    TextColor = color;

}   // SetTextColorToDC


/************************************************************************
*    DESCRIPTION:           Free all allocations
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
************************************************************************/
void CCanvas::Free()
{
    // Deleted the selected font
    if( hFont )
    {
        SelectFont( hDeviceDC, hOldFont );
        DeleteFont( hFont );
        hFont = NULL;
    }

    // Delete the selected pen
    if( hPen )
    {
        SelectPen( hDeviceDC, hOldPen );
        DeletePen( hPen );
        hPen = NULL;
    }

    // Delete the selected brush
    if( hBrush )
    {
        SelectBrush( hDeviceDC, hOldBrush );
        DeleteBrush( hBrush );
        hBrush = NULL;
    }
}