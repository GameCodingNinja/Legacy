
/************************************************************************
*
*    PRODUCT:         Printer Output
*
*    FILE NAME:       TPrinter.cpp
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

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include "Printer.h"         // Header file for this *.cpp file.


/************************************************************************
*    FUNCTION NAME:              Constructer
*
*    DESCRIPTION:           Get information on the default printer so that
*                           we can print to it.                                                            
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TPrinter::TPrinter()
{
    // init the print dialog structure
    memset( &pdialog, 0, sizeof(PRINTDLG) );
    pdialog.lStructSize = sizeof(PRINTDLG);
    pdialog.hwndOwner = GetActiveWindow();

    // Init class members
    // Termanate the strings
    PrinterInfo[PRINTER_NAME][0]   = 0;
    PrinterInfo[PRINTER_DRIVER][0] = 0;
    PrinterInfo[PRINTER_PORT][0]   = 0;

    PrinterDC = NULL;
    StartDocRCode = 0;
    hFont = NULL;
    hPen = NULL;
    hBrush = NULL;

}   // TPrinter



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

TPrinter::~TPrinter()
{
    Free();
}



    // Create a specific printer DC

/************************************************************************
*    FUNCTION NAME:         CreatePrinterDC                                                             
*
*    DESCRIPTION:           Create a specific printer DC. Use this for creating
*                           your own printer DC instead of using the hDC
*                           returned from the PrintDlg
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
*    h rosenorn    8/23/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TPrinter::CreatePrinterDC()
{
    char Printer[300];
    char *TmpPrinterInfo1, *TmpPrinterInfo2;
    DWORD size;
    int i;
    BOOL Result = FALSE;

    // Free all printer objects
    Free();

    // Get the printer info from the windows ini file
    size = GetProfileString( "windows", "device", "", Printer, sizeof(Printer) );

    // Parse the information into it's respected places. There should be three pieces
    // of info, Printer name, driver name and port
    if( size && Printer[0] != 0 )
    {
        TmpPrinterInfo1 = Printer;
        TmpPrinterInfo2 = Printer;

        // Parse out all three pieces of information
        for( i = 0; i < PRINTER_INFO_COUNT; ++i )
        {
            // Parse out the printer name
            // Zip along until we find our first comma
            while( *TmpPrinterInfo1 != 0 && *TmpPrinterInfo1 != ',' )
            {
                // If we get a NULL this early in the game, just get out
                if( *TmpPrinterInfo1 == 0 && i != PRINTER_PORT )
                {
                    PostMsg( "Error", "Default printer information incomplete. Do you have a default printer selected?" );
                    goto HANDLE_ERROR;
                }

                TmpPrinterInfo1++;
            }

            // Replace the comma with a NULL
            *TmpPrinterInfo1 = 0;

            // copy the string
            lstrcpy( PrinterInfo[i], TmpPrinterInfo2 );

            // Jump past our NULL and continue to the next piece of info
            TmpPrinterInfo1++;
            TmpPrinterInfo2 = TmpPrinterInfo1;
        }

        // Create the DC for the printer
        PrinterDC = CreateDC( NULL, PrinterInfo[PRINTER_NAME], NULL, NULL );

        // Do we have a DC
        if( PrinterDC == NULL )
        {
            PostMsg( "Error", "Error Creating Printer Device Context. Do you have a default printer selected?" );
            goto HANDLE_ERROR;
        }
    }
    else
    {
        PostMsg( "Error", "Unable to get default printer information. Do you have a default printer selected?" );
        goto HANDLE_ERROR;
    }

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // CreatePrinterDC


/************************************************************************
*    FUNCTION NAME:         Free                                                             
*
*    DESCRIPTION:           Free all printer objects
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
*    h rosenorn    8/23/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPrinter::Free()
{
    // Deleted the selected font
    if( PrinterDC && hFont )
    {
        SelectFont( PrinterDC, hOldFont );
        DeleteFont( hFont );
        hFont = NULL;
    }

    // Delete the selected pen
    if( PrinterDC && hPen )
    {
        SelectPen( PrinterDC, hOldPen );
        DeletePen( hPen );
        hPen = NULL;
    }

    // Delete the selected brush
    if( PrinterDC && hBrush )
    {
        SelectBrush( PrinterDC, hOldBrush );
        DeleteBrush( hBrush );
        hBrush = NULL;
    }

    // Delete the printer dc
    if( PrinterDC )
    {
        DeleteDC( PrinterDC );
        PrinterDC = NULL;
    }

}   // Free



/************************************************************************
*    FUNCTION NAME:         DisplayPrinterSetup                                                             
*
*    DESCRIPTION:           General purpose display printer setup dialog
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              HINSTANCE hInst - Program instance 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    8/23/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

LPPRINTDLG TPrinter::DisplayPrinterSetup()
{
    // init the structure to all zeros
    pdialog.Flags = PD_PRINTSETUP;

    if( PrintDlg( &pdialog ) )
        return &pdialog;
    else
        return NULL;

}   // DisplayPrinterSetup



/************************************************************************
*    FUNCTION NAME:         DisplayPrintDlg                                                             
*
*    DESCRIPTION:           Displays the printer dialog
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              HINSTANCE hInst - Program instance 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    8/23/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

LPPRINTDLG TPrinter::DisplayPrintDlg( int CurrentPage, int NumPages )
{
    // Free all printer objects
    Free();

    pdialog.Flags = PD_HIDEPRINTTOFILE|PD_PAGENUMS|PD_NOSELECTION|PD_RETURNDC;
    pdialog.nFromPage = CurrentPage;
    pdialog.nToPage = CurrentPage;
    pdialog.nMinPage = 1;
    pdialog.nMaxPage = NumPages;

    if( PrintDlg( &pdialog ) )
    {
        // If the PD_PAGENUMS flag is not set then they choose
        // to print all the pages so set the ToPage to the max value
        if( !(pdialog.Flags & PD_PAGENUMS) )
        {
            pdialog.nFromPage = pdialog.nMinPage;
            pdialog.nToPage = pdialog.nMaxPage;
        }
                
        PrinterDC = pdialog.hDC;
        return &pdialog;
    }
    else
        return NULL;

}   // DisplayPrintDlg



/************************************************************************
*    FUNCTION NAME:        BeginDoc()                                                             
*
*    DESCRIPTION:           Starts the printing cycle
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   BOOL - Is all ok to print
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TPrinter::BeginDoc( LPCTSTR DocName )
{
    DOCINFO di;
    BOOL Result = FALSE;

    if( PrinterDC )
    {
        di.cbSize = sizeof(DOCINFO);
        di.lpszDocName = DocName;
        di.lpszOutput = NULL;
        di.lpszDatatype = NULL;
        di.fwType = 0;

        // Start the doc and get the return code
        StartDocRCode = StartDoc( PrinterDC, &di );

        // Did we get the code? Can we print.
        if( StartDocRCode > 0 )
            Result = TRUE;
    }

    return Result;
}



/************************************************************************
*    FUNCTION NAME:         StopDoc()                                                             
*
*    DESCRIPTION:           Ends the printing cycle and deletes the DC
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

void TPrinter::StopDoc()
{
    if( PrinterDC && StartDocRCode > 0 )
    {
        EndDoc( PrinterDC );

        // Reset the startdoc return code
        StartDocRCode = 0;
    }
}



/************************************************************************
*    FUNCTION NAME:         BeginPage()                                                             
*
*    DESCRIPTION:           Starts the page
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

void TPrinter::BeginPage()
{
    if( PrinterDC && StartDocRCode > 0 )
        StartPage( PrinterDC );
}




/************************************************************************
*    FUNCTION NAME:         StopPage()                                                             
*
*    DESCRIPTION:           Stops the page and prints it out
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

void TPrinter::StopPage()
{
    if( PrinterDC && StartDocRCode > 0 )
        EndPage( PrinterDC );

}   // StopPage




/************************************************************************
*    FUNCTION NAME:         PrintText()                                                             
*
*    DESCRIPTION:           Stops the page and prints it out
*
*    FUNCTION PARAMETERS:
*    Input:    int X - X coordinate
*              int Y - Y coordinate
*              char Text   - Used with vprintf to get the argument list
*                            for wvsprintf.
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

void TPrinter::PrintText( int X, int Y, char *Text, ... )
{
    va_list argptr;
    char StrBuffer[1024];

    va_start( argptr, Text );
    vprintf( Text, argptr );
    wvsprintf( StrBuffer, Text, argptr );

    if( PrinterDC && StartDocRCode > 0 )
        TextOut( PrinterDC, X, Y, StrBuffer, lstrlen(StrBuffer) );

    va_end( argptr );

}   // PrintText




/************************************************************************
*    FUNCTION NAME:         SetFont()                                                             
*
*    DESCRIPTION:           Creates the font and selects it into the printer DC
*
*    FUNCTION PARAMETERS:
*    Input:    char *Name  - Font Name
*              int Size    - Size of the font
*              WORD Style  - Style of the font
*              COLORREF mColor - Color of the font
*              int Angle   - Angle of the font
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

void TPrinter::SetFont( char *Name, int Size, WORD Style, COLORREF mColor, int Angle, LOGFONT * pLogFont )
{
    LOGFONT TmpLogFont;

    // Check first to see that we have a printer DC
    if( PrinterDC )
    {
        // If we currently have a font loaded, select the old one back in
        // and delete our current one
        if( hFont )
        {
            SelectFont( PrinterDC, hOldFont );
            DeleteFont( hFont );
        }

        if( pLogFont )
        {
           // Copy the plogfont to the tempoary structure
            memcpy( &TmpLogFont, pLogFont, sizeof( LOGFONT ) );

            // The height needs to be changed as a result of the printer
            // because as of now it's all in screen units
            TmpLogFont.lfHeight = -MulDiv( TmpLogFont.lfHeight, GetpixelsPerInchY(), 72 );

            // Create the font
            hFont = MakeFont( PrinterDC, Name, Size, Style, mColor, Angle, &TmpLogFont );
        }
        else
            // Create the font
            hFont = MakeFont( PrinterDC, Name, Size, Style, mColor, Angle );

        if( hFont )
            hOldFont = SelectFont( PrinterDC, hFont );
    }
}   // SetFont



/************************************************************************
*    FUNCTION NAME:         SetPen                                                             
*
*    DESCRIPTION:           Creates a pen and selects it into the DC
*
*    FUNCTION PARAMETERS:
*    Input:    int PenStyle  - Pen style
*              int PenWidth  - Pen width
*              COLORREF PenColor  - pen color
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

void TPrinter::SetPen( int PenStyle, int PenWidth, COLORREF PenColor )
{
    // Check first to see that we have a printer DC
    if( PrinterDC )
    {
        // If we currently have a pen loaded, select the old one back in
        // and delete our current one
        if( hPen )
        {
            SelectPen( PrinterDC, hOldPen );
            DeletePen( hPen );
        }

        // Create the font
        hPen = CreatePen( PenStyle, PenWidth, PenColor );

        // Select the new font into the divice context
        if( hPen )
            hOldPen = SelectPen( PrinterDC, hPen );
    }

}   // SetPen



/************************************************************************
*    FUNCTION NAME:         SetBrush                                                             
*
*    DESCRIPTION:           Creates the font and selects it into the printer DC
*
*    FUNCTION PARAMETERS:
*    Input:    COLORREF BrushColor - brush color
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

void TPrinter::SetBrush( COLORREF BrushColor )
{
    // Check first to see that we have a printer DC
    if( PrinterDC )
    {
        // If we currently have a pen loaded, select the old one back in
        // and delete our current one
        if( hBrush )
        {
            SelectBrush( PrinterDC, hOldBrush );
            DeleteBrush( hBrush );
        }

        // Create the font
        hBrush = CreateSolidBrush( BrushColor );

        // Select the new font into the divice context
        if( hBrush )
            hOldBrush = SelectBrush( PrinterDC, hBrush );
    }

}   // SetBrush


/************************************************************************
*    FUNCTION NAME:         PrintBitmap()                                                             
*
*    DESCRIPTION:           Prints a bitmap using its ppi resolution
*
*    FUNCTION PARAMETERS:
*    Input:    PTDIB pdib - Pointer to a DIB
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

void TPrinter::PrintBitmap( PTDIB pdib, LPRECT rect )
{
    //double width, height, pixelsInchX, pixelsInchY, dibMultX, dibMultY, printerMultiX, printerMultiY;
    //int ResultX, ResultY;

    // Do we have a real bitmap, a DC, a StartDocRCode and can the printer support printing bitmaps
    if( !pdib->IsEmpty() && PrinterDC && StartDocRCode > 0 && (GetDeviceCaps( PrinterDC, RASTERCAPS )&(RC_BITBLT|RC_STRETCHDIB|RC_STRETCHBLT)) )
    {
        SetStretchBltMode( PrinterDC, HALFTONE );
        pdib->BlitToDC( PrinterDC, rect );
        SetBrushOrgEx( PrinterDC, 0, 0, NULL );

        // It won't hurt to do this and I'm not sure if it helps or not
        /*if( GetDeviceCaps( PrinterDC, BITSPIXEL ) <= 8 )
        {
            SelectPalette( PrinterDC, pdib->GetPalette(), FALSE ); 
            RealizePalette( PrinterDC );
        }

        // Convert all to doubles for floating point math
        width = pdib->width();
        height = pdib->height();
        pixelsInchX = pdib->GetPixelsPerInchX();
        pixelsInchY = pdib->GetPixelsPerInchY();
        printerMultiX = GetpixelsPerInchX();
        printerMultiY = GetpixelsPerInchY();

        // Get the multiplyers
        if( width > pixelsInchX )
            dibMultX = width / pixelsInchX;
        else
            dibMultX = pixelsInchX / width;

        if( height > pixelsInchY )
            dibMultY = height / pixelsInchY;
        else
            dibMultY = pixelsInchY / height;

        // Generate the result
        ResultX = dibMultX * printerMultiX;
        ResultY = dibMultY * printerMultiY;

        // API call that uses the BITMAPINFO structure and a pointer to the bitmaps
        // bits to blit the image to the window. This is not the fastest way to blit
        // images and wouldn't be my first choice for animation, but for this purpose,
        // it works great and is low overhead and no fuss. Even though this function 
        // is called StretchDIBits, we are not doing any stretching here but its safer than SetDIBitsToDevice.
        StretchDIBits( PrinterDC, X, Y, ResultX, ResultY,
                       0, 0, pdib->width(), pdib->height(), pdib->bits(), pdib->dib(), DIB_RGB_COLORS, SRCCOPY );*/
    }

}   // PrintBitmap



/************************************************************************
*    FUNCTION NAME:         PrintBitmap()                                                             
*
*    DESCRIPTION:           Prints a bitmap using its ppi resolution
*
*    FUNCTION PARAMETERS:
*    Input:    PTDIB pdib - Pointer to a DIB
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

void TPrinter::PrintBitmap( PTBitmap pBitmap, LPRECT rect )
{
    // Do we have a real bitmap, a DC, a StartDocRCode and can the printer support printing bitmaps
    if( !pBitmap->IsEmpty() && PrinterDC && StartDocRCode > 0 && (GetDeviceCaps( PrinterDC, RASTERCAPS )&(RC_BITBLT|RC_STRETCHDIB|RC_STRETCHBLT)) )
    {
        pBitmap->StretchDraw( PrinterDC, rect );
    }

}   // PrintBitmap



/************************************************************************
*    FUNCTION NAME:         PrintMetafile                                                             
*
*    DESCRIPTION:           Prints a metafile to a printer
*
*    FUNCTION PARAMETERS:
*    Input:    PTDIB pdib - Pointer to a DIB
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

void TPrinter::PrintMetafile( PTMetafile pMetafile, RECT *rect )
{
    // Do we have a real metafile, a DC, a StartDocRCode
    if( !pMetafile->IsEmpty() && PrinterDC && StartDocRCode > 0 )
    {
        pMetafile->Draw( PrinterDC, rect );
    }

}   // PrintMetafile



/************************************************************************
*    FUNCTION NAME:         TextWidth()                                                             
*
*    DESCRIPTION:           Gets the width of the text string using the
*                           currently selected font
*
*    FUNCTION PARAMETERS:
*    Input:    char *text - Text string to get width of
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

int TPrinter::TextWidth( char *text )
{
    SIZE size;
    int Result = 0;

    // Check first to see that we have a printer DC
    if( PrinterDC )
    {
        // Check to ensure the function didn't fail
        if( GetTextExtentPoint32( PrinterDC, text, strlen(text), &size ) )
            Result = size.cx;
    }

    return Result;

}   // TextWidth



/************************************************************************
*    FUNCTION NAME:         TextHeight()                                                             
*
*    DESCRIPTION:           Gets the height of the text string using the
*                           currently selected font
*
*    FUNCTION PARAMETERS:
*    Input:    char *text - Text string to get width of
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

int TPrinter::TextHeight( char *text )
{
    SIZE size;
    int Result = 0;

    // Check first to see that we have a printer DC
    if( PrinterDC )
    {
        // Check to ensure the function didn't fail
        if( GetTextExtentPoint32( PrinterDC, text, strlen(text), &size ) )
            Result = size.cy;
    }

    return Result;

}   // TextHeight




/************************************************************************
*    FUNCTION NAME:          WrapText                                                             
*
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
*              int TextHeight  - The height of the text to where a new page is started when reached
*              int Leading     - Additional ledding to add to each line 
*
*    Output:   DWORD - Return the top margin where the next line of text can be drawn
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

DWORD TPrinter::WrapText( char *Text, int LeftMargin, int TopMargin, int TextWidth, int TextHeight, int Leading, int Justify )
{
    DWORD Result = 0;

    // Send formatted output to the printer
    if( PrinterDC && StartDocRCode > 0 )    // Is this long enough for ya
        Result = WrapTextToDevice( PrinterDC, Text, LeftMargin, TopMargin, TextWidth, TextHeight, Leading, PRINTER_OUTPUT, Justify );

    return Result;

}   // WrapText



/************************************************************************
*    FUNCTION NAME:         Line                                                             
*
*    DESCRIPTION:           Draw a line to the DC 
*
*    FUNCTION PARAMETERS:
*    Input:    int Left
*              int Top
*              int Right
*              int Bottom      - Line coordinates 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPrinter::Line( int Left, int Top, int Right, int Bottom )
{
    if( PrinterDC && hPen && StartDocRCode > 0 )
    {
        MoveToEx( PrinterDC, Left, Top, NULL );
        LineTo( PrinterDC, Right, Bottom );
    }

}   // Line


/************************************************************************
*    FUNCTION NAME:         Fill                                                             
*
*    DESCRIPTION:           Draw a box to the DC 
*
*    FUNCTION PARAMETERS:
*    Input:    int Left
*              int Top
*              int Right
*              int Bottom      - Line coordinates 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPrinter::Fill( int Left, int Top, int Right, int Bottom )
{
    if( PrinterDC && hBrush && StartDocRCode > 0 )
        Rectangle( PrinterDC, Left, Top, Right, Bottom );

}   // Fill