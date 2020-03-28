
/************************************************************************
*
*    PRODUCT:         Printer Output
*
*    FILE NAME:       TPrinter.h
*
*    DESCRIPTION:     TPrinter for printing to the printer
*
*    IMPLEMENTATION:  
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __PRINTER_H__
#define __PRINTER_H__

#include "TBitmap.h"         // Dib and Bitmap class for displaying and animating bitmaps
#include "Metafile.h"        // Class for loading and displaying Windows metafiles and enhanced metafiles
#include <commdlg.h>         // Windows common dialog box include
#include "minTools.h"        // A misc group of useful functions

////////////////////////////
//    Printer Defines
////////////////////////////

#define PRINTER_NAME         0
#define PRINTER_DRIVER       1
#define PRINTER_PORT         2
#define PRINTER_INFO_COUNT   3


class TPrinter
{
protected:

    char PrinterInfo[3][100];
    HDC PrinterDC;
    int StartDocRCode;
    HFONT hFont, hOldFont;
    HPEN hPen, hOldPen;
    HBRUSH hBrush, hOldBrush;
    PRINTDLG pdialog;

public:
    // Constructor
    TPrinter();
    // Destructor
    ~TPrinter();
    HDC Handle(){ return PrinterDC; };
    // Start the printing process
    BOOL BeginDoc( LPCTSTR DocName );
    // End the printing process
    void StopDoc();
    // Starts the page
    void BeginPage();
    // Prints out the page
    void StopPage();
    // Stop the printing, user cancel
    BOOL AbortPrintJob(){ return (PrinterDC ? AbortDoc( PrinterDC ) : 0); };
    // Prints text to the printer
    void PrintText( int X, int Y, char *Text, ... );
    // Get the pixels per inch. Return 0 if there is no DC
    int GetpixelsPerInchX(){ return (PrinterDC ? GetDeviceCaps( PrinterDC, LOGPIXELSX ) : 0); };
    int GetpixelsPerInchY(){ return (PrinterDC ? GetDeviceCaps( PrinterDC, LOGPIXELSY ) : 0); };
    // Get the offset of where the page begins. Return 0 if there is no DC
    int GetPhysicalOffsetX(){ return (PrinterDC ? GetDeviceCaps( PrinterDC, PHYSICALOFFSETX ) : 0); };
    int GetPhysicalOffsetY(){ return (PrinterDC ? GetDeviceCaps( PrinterDC, PHYSICALOFFSETY ) : 0); };
    // Get the Aspect. Return 0 if there is no DC
    int GetAspectX(){ return (PrinterDC ? GetDeviceCaps( PrinterDC, ASPECTX ) : 0); };
    int GetAspectY(){ return (PrinterDC ? GetDeviceCaps( PrinterDC, ASPECTY ) : 0); };
    // get the width and height
    int width(){ return (PrinterDC ? GetDeviceCaps( PrinterDC, HORZRES ) : 0); };
    int height(){ return (PrinterDC ? GetDeviceCaps( PrinterDC, VERTRES ) : 0); };
    // Create a font for the printer to use and select it into the DC
    void SetFont( char *Name, int Size, WORD Style = FONT_REGULAR, COLORREF mColor = DEFAULT_BLACK, int Angle = 0, LOGFONT * pLogFont = NULL );
    // Create a pen for the screen and select it into the DC
    void SetPen( int PenStyle, int PenWidth, COLORREF PenColor );
    // Create a brush for the screen and select it into the DC
    void SetBrush( COLORREF BrushColor );
    // Prints a bitmap to the printer
    void PrintBitmap( PTDIB pdib, LPRECT rect );
    void PrintBitmap( PTBitmap pBitmap, LPRECT rect );
    // Prints a metafile to the printer
    void PrintMetafile( PTMetafile pMetafile, RECT *rect );
    // Gets the width of the text string
    int TextWidth( char *text );
    // Gets the width of the text string
    int TextHeight( char *text );
    // Formats a block of text to the printer
    DWORD WrapText( char *Text, int LeftMargin, int TopMargin, int TextWidth, int TextHeight = 0, int Leading = 0, int Justify = LEFT_JUSTIFY );
    // Draw a line to the DC
    void Line( int Left, int Top, int Right, int Bottom );
    // Draw a box to the DC
    void Fill( int Left, int Top, int Right, int Bottom );
    // General purpose display printer setup dialog
    LPPRINTDLG DisplayPrinterSetup();
    // Displays the printer dialog
    LPPRINTDLG TPrinter::DisplayPrintDlg( int CurrentPage, int NumPages );
    // Create a specific printer DC
    BOOL CreatePrinterDC();
    // Free all printer objects
    void Free();
};


typedef TPrinter *PTPrinter;


#endif  /* __PRINTER__ */