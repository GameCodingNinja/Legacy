
/************************************************************************
*
*    PRODUCT:         Text Block Manipulation
*
*    FILE NAME:       TTextBlock.cpp
*
*    DESCRIPTION:     Object for text blocks
*
*    IMPLEMENTATION:  Load text blocks
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
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size
#include "TTextBlk.h"        // Header file for this *.cpp file.

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

TTextBlock::TTextBlock()
{
    // Null out all important variables
    pData = NULL;
    count = 0;
    hFont = NULL;
    hOldFont = NULL;
    hWndFont = NULL;
    ObjectType = TEXT_OBJECT;
    ObjectDataSizeInBytes = 0;
}



/************************************************************************
*    FUNCTION NAME:         LoadFromResource()                                                             
*
*    DESCRIPTION:           Load bitmap from resource file.
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst   - Program Instance
*              char *name        - Name of the resource
*              char *type        - Resource Type Id
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

BOOL TTextBlock::LoadFromResource( HINSTANCE hInst, char *name, char *type )
{
    TLoadRes LoadRes;
    BOOL Result = FALSE;

    FreeBlock(); // Free memory if need be

    if( !LoadTheResource( hInst, name, type, &LoadRes ) )
        return FALSE;

    // Allocate the memory to hold the text block
    pData = new char[ LoadRes.Size + 1 ];

    // Did the allocation work?
    if( pData )
    {
        // Copy the character string
        strcpy( pData, (char *)LoadRes.pData );

        // Set the result
        Result = TRUE;

        // Save the number of characters
        count = LoadRes.Size;

        // Save the size of the file for later use
        ObjectDataSizeInBytes = LoadRes.Size;

        // Terminate the text block
        pData[ LoadRes.Size ] = 0;
    }

    return Result;

} // LoadFromResource


/************************************************************************
*    FUNCTION NAME:         LoadFromFile()                                                             
*
*    DESCRIPTION:           Load bitmap from file
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName    - Path to file name
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

BOOL TTextBlock::LoadFromFile( char *FileName, BOOL showErrorMsg )
{ 
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long bytesRead;
    HANDLE hFile;
    DWORD size;
    BOOL ReadResult;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    FreeBlock(); // Free memory if need be

    // Open the file on the hard drive
    hFile = CreateFile( FileName, GENERIC_READ, FILE_SHARE_READ,
                        &FileSecurity, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hFile == INVALID_HANDLE_VALUE )
    {
        if( showErrorMsg )
        	PostMsg( "Error", "Can't open text file (%s) to display.", FileName );
        return FALSE;
    }

    // Get the file size of the text block so that we can allocate enough memory for it
    size = GetFileSize( hFile, NULL );

    // Save the size of the file for later use
    ObjectDataSizeInBytes = size;

    // Allocate memory for text block
    pData = new char[ size + 1 ];
    if( pData == NULL )
    {
        if( showErrorMsg )
        	PostMsg( "Error", "Unable to allocate memory for text file (%s).", FileName );
        CloseHandle( hFile );
        return FALSE;
    }

    // Read contents of file into allocated memory
    ReadResult = ReadFile( hFile, pData, size, &bytesRead, NULL );

    if( !ReadResult || bytesRead != size )
    {
        if( showErrorMsg )
        	PostMsg( "Error", "Unable to load text file (%s).", FileName );
        FreeBlock();
        CloseHandle( hFile );
        return FALSE;
    }

    // Terminate the text block
    pData[ size ] = 0;

    // Close the file handle
    CloseHandle( hFile );

    // Save the number of characters
    count = size;

    return TRUE;

} // LoadFromFile



/************************************************************************
*    FUNCTION NAME:         LoadFromBuffer                                                             
*
*    DESCRIPTION:           Get a pointer to a memory block. An easy way to
*                           give one block of memory to another
*
*    FUNCTION PARAMETERS:
*    Input:    BYTE *pBuffer - Handle to a memory block
*              DWORD Size    - Size of character block 
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

BOOL TTextBlock::LoadFromBuffer( BYTE *pBuffer, DWORD Size )
{
    BOOL Result = FALSE;

    FreeBlock(); // Free memory if need be

    // Copy pointer
    if( pBuffer )
    {
        // Copy over the pointer
        pData = (char *)pBuffer;

        // Save the size of this block
        count = Size;

        // Save the size of the file for later use
        ObjectDataSizeInBytes = Size;

        Result = TRUE;
    }

    return Result;

}   // LoadFromBuffer



/************************************************************************
*    FUNCTION NAME:         Free                                                             
*
*    DESCRIPTION:           Free allocated memory
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

void TTextBlock::Free()
{
    // Deletes the text block
    FreeBlock();

    // Deletes the font
    FreeFont();

}  // Free



/************************************************************************
*    FUNCTION NAME:         FreeBlock                                                             
*
*    DESCRIPTION:           Free the text
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
 
void TTextBlock::FreeBlock()
{
    if( pData )
    {
        delete [] pData;
        pData = NULL;
        count = 0;
    }
}   // FreeBlock



/************************************************************************
*    FUNCTION NAME:         FreeFont                                                             
*
*    DESCRIPTION:           Free the font
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
 
void TTextBlock::FreeFont()
{
    HDC hDC;

    if( hFont && hWndFont )
    {
        // Get the DC of the window the font is being applied to
        hDC = GetDC( hWndFont );

        // The old font needs to be selected into the windows
        // DC before the created font can be deleted
        SelectFont( hDC, hOldFont );

        // Release the DC because we are done with it
        ReleaseDC( hWndFont, hDC );

        // Delete the font
        DeleteFont( hFont );

        hFont = NULL;
        hWndFont = NULL;
    }

}   // FreeFont



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

BOOL TTextBlock::IsEmpty()
{
    BOOL Result = TRUE;

    if( pData )
        Result = FALSE;

    return Result;

}   // IsEmpty



/************************************************************************
*    FUNCTION NAME:         MakeNewFont                                                             
*
*    DESCRIPTION:           Creates the font
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd   - Handle to the window the font is being applied to
*              char *Name  - Font Name
*              int Size    - Size of the font
*              WORD Style  - Style of the font
*              COLORREF mColor - Color of the font
*              int Angle   - Angle of the font
*
*    Output:   HFONT - Handle to the font
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TTextBlock::MakeNewFont( HWND hwnd, char *Name, int Size, WORD Style, COLORREF mColor, int Angle, LOGFONT *pLogFont )
{
    HDC hDC;

    // First thing we need to do is free the 
    // font if there is one currently in use.
    FreeFont();

    // Get the DC of the window the font is being applied to
    hDC = GetDC( hwnd );

    // Save the window the font was made for
    hWndFont = hwnd;

    // Make the new font
    hFont = MakeFont( hDC, Name, Size, Style, mColor, Angle, pLogFont );
    
    // Release the DC because we are done with it
    ReleaseDC( hwnd, hDC );
    
    return ( hFont != NULL );
        
}   // MakeNewFont



/************************************************************************
*    FUNCTION NAME:         SetFontToWindow                                                             
*
*    DESCRIPTION:           Set the font to a window
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

/*void TTextBlock::SetFontToWindow()
{
    HDC hDC;

    if( hFont )
    {
        // Get the DC of the window the font is being applied to
        hDC = GetDC( hwnd );

        // Select the font to the DC
        hOldFont = SelectFont( hDC, hFont );

        SetWindowFont( hwnd, hFont, Redraw );
        
        // Release the DC because we are done with it
        ReleaseDC( hwnd, hDC );
    }

}   // SetFontToWindow */


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
*    Input:    HWND hwnd       - Handle to the window
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

DWORD TTextBlock::WrapText( HWND hwnd, int LeftMargin, int TopMargin, int TextWidth, int Leading )
{
    DWORD Result = 0;
    HDC DC;

    DC = GetDC( hwnd );

    // Send formatted output to the printer
    if( pData && DC )    // Is this long enough for ya
        Result = WrapTextToDevice( DC, pData, LeftMargin, TopMargin, TextWidth, 0, Leading, SCREEN_OUTPUT );

    if( DC )
        ReleaseDC( hwnd, DC );

    return Result;

}   // WrapText




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
*    Input:    HDC DC          - Handle to the DC
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

DWORD TTextBlock::WrapText( HDC DC, int LeftMargin, int TopMargin, int TextWidth, int Leading )
{
    DWORD Result = 0;

    // Send formatted output to the printer
    if( pData && DC )    // Is this long enough for ya
        Result = WrapTextToDevice( DC, pData, LeftMargin, TopMargin, TextWidth, 0, Leading, SCREEN_OUTPUT, LEFT_RIGHT_JUSTIFY );

    return Result;

}   // WrapText



/************************************************************************
*    FUNCTION NAME:         BlockHeightInPixels                                                             
*
*    DESCRIPTION:           Get the height in pixels of the block of text 
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle to the window
*              int TopMargin   - Starting point
*              char LineOfText - A line of text used for calculating the width of the text
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

SIZE TTextBlock::BlockHeightWidthInPixels( HWND hwnd, char *LineOfText, int Leading )
{
    SIZE Result;
    HDC DC;

    DC = GetDC( hwnd );

    // Select the font into the dc so that we get a proper size
    if( hFont )
        SelectFont( DC, hFont );
    else
        SelectFont( DC, GetStockFont( DEFAULT_GUI_FONT ) );

    // Get the size of the line of text
    GetTextExtentPoint32( DC, LineOfText, strlen(LineOfText), &Result );

    // Send formatted output to the printer
    if( pData && DC )    // Is this long enough for ya
        Result.cy = WrapTextToDevice( DC, pData, 0, 0, Result.cx, 0, Leading, RETURN_HEIGHT_IN_PIXELS, LEFT_RIGHT_JUSTIFY );

    if( DC )
        ReleaseDC( hwnd, DC );

    return Result;

}   // BlockHeightInPixels