
/************************************************************************
*
*    PRODUCT:         Minimum Tools
*
*    FILE NAME:       minTools.cpp
*
*    DESCRIPTION:     General Tools
*
*    IMPLEMENTATION:  This module holds misc funstions for working with
*                     windows programs in general. This is the minimum
*                     amout of tools need for the library files.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "minTools.h"        // Header file for this *.cpp file.

#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf

// This quiets the compiler warnings about a bunch of functions that Microsoft has
// decided to deprecate, even though the C++ standardization committee has not done so.
#pragma warning(disable : 4996)


/************************************************************************
*    DESCRIPTION: If successful, returns a pointer to the resource 
*                 data. Fill out a TLoadRes structure
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst   - Program Instance
*              char *name        - Name of the resource
*              char *type        - Resource Type Id
*              PTLoadRes pLoadRes - Pointer to a PTLoadRes structure to fill out
*
*    Output:   bool        - Returns true or false
************************************************************************/
bool LoadTheResource( HINSTANCE hInst, char *name, char *type, PTLoadRes pLoadRes )
{
    // Find the resource
    pLoadRes->hRes = FindResource(hInst, name, type);
    if( pLoadRes->hRes == NULL )
    {
        PostMsg( "Error", "Can't find resource (%s).", name );
        return FALSE;
    }

    // Get the size of the resource
    pLoadRes->Size = SizeofResource( hInst, pLoadRes->hRes );
    if( pLoadRes->Size == 0 )
    {
        PostMsg( "Error", "Resource (%s) has no size.", name );
        return FALSE;
    }

    // Load the resource
    pLoadRes->Handle = LoadResource( hInst, pLoadRes->hRes );
    if( pLoadRes->Handle == NULL )
    {
        PostMsg( "Error", "Can't load resource (%s).", name );
        return FALSE;
    }

    // Get a pointer to the memory block
    pLoadRes->pData = (unsigned char *)LockResource( pLoadRes->Handle );
    if( pLoadRes->pData == NULL )
    {
        PostMsg( "Error", "Can't lock resource (%s).", name );
        return FALSE;
    }

    return TRUE;

}   // LoadTheResource


/************************************************************************

*    DESCRIPTION: Specific pieces of information may need to be
*                 checksummed to help detour tampering with the
*                 program. Someone could easily open the program,
*                 change some information, and sell it as their
*                 own. This function works with my checksum utility
*                 which will read in a file, quad align it and
*                 checksum it to zero. That way all your checksum
*                 all need to result in zero and not some number
*                 you need to maintain.
*
*    FUNCTION PARAMETERS:
*    Input:    void *pData      - Pointer to data to be check summed
*              int SizeInBytes  - Number in bytes of length of data
*
*    Output:   int - The screen saver mode
************************************************************************/
bool CheckSumData( void *pData, unsigned int SizeInBytes )
{
    unsigned int CheckSumResult = 0;
    unsigned int *pDataTmp = (unsigned int *)pData;
    bool Result = FALSE;
    int DwordSize, i;

    // Check to make sure the data is quad aligned
    if( SizeInBytes % sizeof( unsigned int ) == 0 )
    {
        // Translate the size in bytes to the number of DWORDs
        DwordSize = SizeInBytes / sizeof( unsigned int );
        
        // Cycle through all the data and add it up
        for( i = 0; i < DwordSize; ++i )
            CheckSumResult += pDataTmp[i];
        
        // Is the result zero? Then it is ok    
        if( CheckSumResult == 0 )
            Result = TRUE;  
    }

    return Result;

}   // CheckSumData


/************************************************************************
*    DESCRIPTION: Translates into a windows messagebox with
*                 sprintf capabilities. Makes debugging easier.
*
*    FUNCTION PARAMETERS:
*    Input:    char *Title - Title of the windows message box
*              char *fmt   - Used with vprintf to get the argument list
*                            for wvsprintf.
************************************************************************/
void PostMsg( char *Title, char *fmt, ... )
{
    va_list argptr;
    char StrBuffer[1024];

    va_start( argptr, fmt );
    vprintf( fmt, argptr );
    wvsprintf( StrBuffer, fmt, argptr );
    MessageBox( GetActiveWindow(), StrBuffer, Title, MB_ICONINFORMATION );
    va_end( argptr );

} // PostMsg


/************************************************************************
*    DESCRIPTION: Writes a block of data out to a file.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   bool - Description
************************************************************************/
bool WriteDataToFile( char *FileName, unsigned char *pBuffer, unsigned int size, unsigned int Flags )
{
    unsigned long bytesWritten;
    SECURITY_ATTRIBUTES FileSec;
    HANDLE hFile;
    bool Result = FALSE;
    BOOL FileReadWriteResult;

    FileSec.nLength = sizeof( SECURITY_ATTRIBUTES );
    FileSec.lpSecurityDescriptor = NULL;
    FileSec.bInheritHandle = FALSE;

    // Create the file
    hFile = CreateFile( FileName, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ,
                         &FileSec, Flags, FILE_ATTRIBUTE_NORMAL, 0 );

    if( hFile == INVALID_HANDLE_VALUE )
        goto HANDLE_ERROR;

    // Set the file pointer to the end of the file because we want to add more data to the file
    if( Flags == OPEN_ALWAYS )
        SetFilePointer( hFile, 0, 0, FILE_END );

    // Write your string
    FileReadWriteResult = WriteFile( hFile, pBuffer, size, &bytesWritten, NULL );
    if( !FileReadWriteResult || bytesWritten != size )
        goto HANDLE_ERROR;

    Result = TRUE;

    HANDLE_ERROR:;

    if( hFile )
        CloseHandle( hFile );

    return Result;

}   // TextToFile


/************************************************************************
*    DESCRIPTION: Sizing and centering a window. If you send it
*                 a 0 width or 0 height, the width or height will
*                 be the size of the computers resolution.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd     - Handle of the window receiving the message
*              int Width     - Width of the window
*              int Height    - Height of the window     
*
*    Output:   long - Returns the error code if any
************************************************************************/
void CenterWindow( HWND hwnd, int Width, int Height, HWND hParentWnd, bool IsChildWnd )
{
    int X, Y, ScreenW, ScreenH, ParentW, ParentH;
    RECT rect;

    // Get the size the the display screen
    ScreenW = GetSystemMetrics( SM_CXSCREEN );
    ScreenH = GetSystemMetrics( SM_CYSCREEN );

    // Are we centering inside of a parent
    if( hParentWnd )
    {
        // Get the parent windows rect
        GetWindowRect( hParentWnd, &rect );

        // Get the width and height of the parent window
        ParentW = rect.right - rect.left;
        ParentH = rect.bottom - rect.top;

        X = (ParentW - Width) / 2;
        Y = (ParentH - Height) / 2;

        // If this is not a child window add in the parents XY's
        if( !IsChildWnd )
        {
            X += rect.left;
            Y += rect.top;

            // Make sure we are not hidden in the window
            if( X + Width > ScreenW )
                X = ScreenW - Width;
            if( Y + Height > ScreenH )
                Y = ScreenH - Height;
        }
        else
        {
            if( X < 0 ) X = 0;
            if( Y < 0 ) Y = 0;

            // Make sure we are not hidden in the window
            if( X + Width > ParentW )
                X = ParentW - Width;
            if( Y + Height > ParentH )
                Y = ParentH - Height;
        }
    }
    // Center across the whole screen
    else
    {
        if (Width == 0)
        {
            Width = ScreenW;
            X = 0;
        }
        else
            X = (ScreenW - Width) / 2;

        if (Height == 0)
        {
            Height = ScreenH;
            Y = 0;
        }
        else
            Y = (ScreenH - Height) / 2;
    }

    // Put a window at a given position and size
    MoveWindow( hwnd, X, Y, Width, Height, TRUE );

}   // CenterWindow


/************************************************************************
*    DESCRIPTION: Retreves the Windows directory and adds the
*                 backslash
*
*    FUNCTION PARAMETERS:
*    Input:    char *WinDir - Character buffer
*               bool AddBackSlash - Add the back slash 
************************************************************************/
void GetTheWindowsDirectory( char *WinDir, bool AddBackSlash )
{
    int Length;

    GetWindowsDirectory( WinDir, MAX_PATH * sizeof( char ) );

    // Check if there is a \ added to the temp path
    Length = strlen( WinDir );

    // if they didn't install as the root on their hard drive
    // add the back slash
    if( AddBackSlash && WinDir[Length-1] != BACK_SLASH )
        strcat( WinDir, "\\" );

}   // GetTheWindowsDirectory


/************************************************************************
*    DESCRIPTION: Retreves the Windows temp directory and adds the
*                 backslash
*
*    FUNCTION PARAMETERS:
*    Input:    char *WinDir - Character buffer
*               bool AddBackSlash - Add the back slash 
************************************************************************/
void GetTheWindowsTmpDir( char *WinTmpDir )
{
    int Length;

    if( GetTempPath( MAX_PATH * sizeof( char ), WinTmpDir ) )
    {
        // Check if there is a \ added to the temp path
        Length = strlen( WinTmpDir );

        if( WinTmpDir[Length-1] != BACK_SLASH )
            strcat( WinTmpDir, "\\" );
    }
    else
        // If we go here there was an error so lets give them the Windows directory
        GetTheWindowsDirectory( WinTmpDir, TRUE );

}   // GetTheWindowsTmpDir


/************************************************************************
*    DESCRIPTION: Gets the resource and saves it to the hard drive 
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst   - Program Instance
*              char *name        - Name of the resource
*              char *type        - Resource Type Id
*              char *FileName    - Path to where the file is to be saved
*
*    Output:   bool        - Returns true or false
************************************************************************/
bool SaveResourceToDisk( HINSTANCE hInst, char *name, char *type, char *FileName )
{
    bool Result = FALSE;
    TLoadRes LoadRes;
    unsigned long bytesWritten;
    SECURITY_ATTRIBUTES FileSec;
    HANDLE hFile;

    FileSec.nLength = sizeof( SECURITY_ATTRIBUTES );
    FileSec.lpSecurityDescriptor = NULL;
    FileSec.bInheritHandle = FALSE;

    // Get the resource and save it to the hard drive as a file
    if( LoadTheResource( hInst, name, type, &LoadRes ) )
    {
        // Create the file
        hFile = CreateFile( FileName, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ,
                             &FileSec, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );

        if( hFile != INVALID_HANDLE_VALUE )
        {
            // Write the midi file to the windows directory
            WriteFile( hFile, LoadRes.pData, LoadRes.Size, &bytesWritten, NULL );

            // Check to see that we wrote the whole file
            if( LoadRes.Size == bytesWritten )
                Result = TRUE;

            // Close the file handle
            CloseHandle( hFile );
        }
    }   

    return Result;

}   // SaveResourceToDisk


/************************************************************************
*    DESCRIPTION: Send text output to a device be it screen, printer
*                 or what not. This function uses the supplied
*                 DC to calculate the width and height of the 
*                 text due to the selected font currently in the DC.
*                 The LeftMargin, TopMargin, TextWidth values are
*                 in pixels amounts and that value depends on what
*                 device you are printing to. You can tell the 
*                 output to be a printer and it will reset the
*                 pages to the printer if it runs over the bottom
*                 margin 
*
*    FUNCTION PARAMETERS:
*    Input:    char *Text      - Pointer to the
*              int LeftMargin  - Size of the margin in pixels
*              int TopMargin   - Starting point
*              int TextWidth   - The width of the text to be printed
*              int TextHeight  - The height of the text to where a new page is started when reached
*              int Leading     - Additional ledding to add to each line
*              int OutPutType  - Do we need to be a little more specific by saying it's a printer
*              int Justify     - Set the output to left, right, center or left_right justification
*
*    Output:   unsigned int - Return the top margin where the next line of text can be drawn
************************************************************************/
unsigned int WrapTextToDevice( HDC DC, char *Text, int LeftMargin, int TopMargin, int TextWidth, int TextHeight, int Leading, int OutPutType, int Justify )
{
    unsigned int CharCount, TotalCount = 0;
    SIZE size;
    int StepIt, Top, SpaceCharCount=0, StrLength, TmpStrLength, TempLeftMargin;
    char CurrWord[210], LineOfText[1000], tmpLineOfText[1000];
    TEXTMETRIC tm;
    // Using the 160 character as a space character works most of the time
    //char TabAry[5]={ 160, 160, 160, 160, 0 };

    // Init text strings
    CurrWord[0] = 0;
    LineOfText[0] = 0;
    tmpLineOfText[0] = 0;

    // Setup step amount. This is what we'll increment each line to. Use GetTextMetrics
    // get information about the currently selected font.
    // If you need the leading to be a little more or less, use the Leading paramater
    // Right now it's using the leading information from the font
    if( GetTextMetrics( DC, &tm ) )
        StepIt = tm.tmHeight + tm.tmExternalLeading + Leading;
    else
        StepIt = 15; // Hard code a number just in case GetTextMetrics errors
    
    // Set the starting point
    Top = TopMargin;
    
    // Loop until we run out of letters
    while( Text[TotalCount] != 0 )
    {
        CharCount = 0;

        // Get the next word
        while( Text[TotalCount] > SPACE_CR && Text[TotalCount] != 0 )
        {
            CurrWord[CharCount] = Text[TotalCount];
            CharCount++; TotalCount++;

            // Since this word is so long, lets do a test because
            // this could be a a long array of random characters
            if( CharCount > 30 )
            {
                // Termanate the current word
                CurrWord[CharCount] = 0;

                // Make a temporary line of text to check it's length
                lstrcpy( tmpLineOfText, LineOfText );
                lstrcat( tmpLineOfText, CurrWord );

                // Get the temporary string length
                TmpStrLength = strlen( tmpLineOfText );

                // Get the size of the temporary line of text
                GetTextExtentPoint32( DC, tmpLineOfText, TmpStrLength, &size );

                // break out if this word is way too long
                if( size.cx > TextWidth || CharCount >= 200 )
                    break;
            }
        }

        // Termanate the current word
        CurrWord[CharCount] = 0;

        // Make a temporary line of text to check it's length
        lstrcpy( tmpLineOfText, LineOfText );
        lstrcat( tmpLineOfText, CurrWord );

        // Get the temporary string length
        TmpStrLength = strlen( tmpLineOfText );

        // Get the size of the temporary line of text
        GetTextExtentPoint32( DC, tmpLineOfText, TmpStrLength, &size );
    
        // Is the temporary line of text greater than the width we're looking for?
        // If so, then print to the device the real line of text.
        if( size.cx > TextWidth )
        {
            // Get the length of the string that is to be printed out
            StrLength = strlen( LineOfText );

            // Make sure we have a string to print out
            if( StrLength )
            {
                // Only output to the device if that is what we are supposed to do
                if( OutPutType != RETURN_HEIGHT_IN_PIXELS )
                {
                    // Remove the space characer at the end of the line
                    // and remove the space count from the SpaceCharCount 
                    if( StrLength && LineOfText[ StrLength-1 ] == SPACE_CR )
                    {
                        while( LineOfText[ StrLength-1 ] == SPACE_CR )
                        {
                            --StrLength;
                            --SpaceCharCount;
                        }
                        LineOfText[ StrLength ] = 0;
                    }

                    // Get the size of the text to be blited
                    GetTextExtentPoint32( DC, LineOfText, StrLength, &size );

                    // Set the justification of this line of text
                    if( SpaceCharCount && Justify == LEFT_RIGHT_JUSTIFY )
                        SetTextJustification( DC, TextWidth - size.cx, SpaceCharCount );

                    // Adjust the left margin amount to setup justification
                    if( Justify == LEFT_JUSTIFY || Justify == LEFT_RIGHT_JUSTIFY )
                        TempLeftMargin = LeftMargin;
                    else if( Justify == RIGHT_JUSTIFY )
                        TempLeftMargin = LeftMargin + (TextWidth - size.cx);
                    else if( Justify == CENTER_JUSTIFY )
                        TempLeftMargin = LeftMargin + ((TextWidth - size.cx) / 2);

                    // Print the text to the device
                    TextOut( DC, TempLeftMargin, Top, LineOfText, StrLength );

                    // Reset if we have spaces in the line of text
                    if( SpaceCharCount && Justify == LEFT_RIGHT_JUSTIFY )
                    {
                        // reset the text justification
                        SetTextJustification( DC, 0, SpaceCharCount );

                        // Zero out the space counter
                        SpaceCharCount = 0;
                    }
                }

                // Termanate the line of text to start a new line
                LineOfText[0] = 0;

                // Inc the top for the next line of text
                Top += StepIt;

                // If we are specificly printing this to a printer, we should check if we
                // need to print out this page and start another one
                if( OutPutType == PRINTER_OUTPUT )
                {
                    // Text[TotalCount] != 0   Don't reset a new page if this is the last piece of text
                    //if( (Top - TextHeight) >= GetDeviceCaps( DC, VERTRES ) && Text[TotalCount] != 0 )
                    if( Top >= (TextHeight + TopMargin) - StepIt && Text[TotalCount] != 0 )
                    {   
                        EndPage( DC );    // Print out this page
                        StartPage( DC );  // Start another page
                        Top = TopMargin;  // Reset the margin
                    }
                }
            }
        }

        // Add the current word to the line of text
        lstrcat( LineOfText, CurrWord );

        // Process whitespace and any control characters
        while( Text[TotalCount] <= SPACE_CR && Text[TotalCount] != 0 )
        {
            switch( Text[TotalCount] )
            {
                case SPACE_CR:
                    // Add a space to the line of text
                    lstrcat( LineOfText, " " );
                    ++SpaceCharCount;
                break;

                case TAB_CR:
                    lstrcat( LineOfText, "    " );
                    SpaceCharCount += 4;
                break;

                case LINE_FEED: // Ignore the linefeed character
                break;

                case RETURN_CR: // We hit a return, print the text

                    // Do we have text to print?
                    if( OutPutType != RETURN_HEIGHT_IN_PIXELS && LineOfText[0] )
                    {
                        // Get the length of the string that is to be printed out
                        StrLength = strlen(LineOfText);

                        // Get the size of the text to be blited
                        GetTextExtentPoint32( DC, LineOfText, StrLength, &size );

                        // Adjust the left margin amount to setup justification
                        if( Justify == LEFT_JUSTIFY || Justify == LEFT_RIGHT_JUSTIFY )
                            TempLeftMargin = LeftMargin;
                        else if( Justify == RIGHT_JUSTIFY )
                            TempLeftMargin = LeftMargin + (TextWidth - size.cx);
                        else if( Justify == CENTER_JUSTIFY )
                            TempLeftMargin = LeftMargin + ((TextWidth - size.cx) / 2);

                        // Print the text to the device
                        TextOut( DC, TempLeftMargin, Top, LineOfText, StrLength );
                    }

                    // Termanate the line of text to start a new line
                    LineOfText[0] = 0;

                    // Zero out the space counter
                    SpaceCharCount = 0;

                    // Inc the top for the next line of text
                    Top += StepIt;

                    // If we are specificly printing this to a printer, we should check if we
                    // need to print out this page and start another one
                    if( OutPutType == PRINTER_OUTPUT )
                    {
                        // Text[TotalCount] != 0   Don't reset a new page if this is the last piece of text
                        //if( (Top - TextHeight) >= GetDeviceCaps( DC, VERTRES ) && Text[TotalCount] != 0 )
                        if( Top >= (TextHeight + TopMargin) - StepIt && Text[TotalCount] != 0 )
                        {   
                            EndPage( DC );    // Print out this page
                            StartPage( DC );  // Start another page
                            Top = TopMargin;  // Reset the margin
                        }
                    }
                break;
            }
            TotalCount++;
        }
    }
    
    // We may have one last line of text to print out
    if( LineOfText[0] )
    {
        if( OutPutType != RETURN_HEIGHT_IN_PIXELS )
        {
            // Get the length of the string that is to be printed out
            StrLength = strlen(LineOfText);

            // Get the size of the text to be blited
            GetTextExtentPoint32( DC, LineOfText, StrLength, &size );

            // Adjust the left margin amount to setup justification
            if( Justify == LEFT_JUSTIFY || Justify == LEFT_RIGHT_JUSTIFY )
                TempLeftMargin = LeftMargin;
            else if( Justify == RIGHT_JUSTIFY )
                TempLeftMargin = LeftMargin + (TextWidth - size.cx);
            else if( Justify == CENTER_JUSTIFY )
                TempLeftMargin = LeftMargin + ((TextWidth - size.cx) / 2);

            // Print the text to the device
            TextOut( DC, TempLeftMargin, Top, LineOfText, StrLength );
        }

        // Inc the top for the next line of text
        Top += StepIt;
    } 

    // Return the top margin where the next line of text can be drawn
    return Top;

}   // WrapTextToDevice


/************************************************************************
*    DESCRIPTION: Creates the font
*
*    FUNCTION PARAMETERS:
*    Input:    HDC DC      - Handle to the device context
*              char *Name  - Font Name
*              int Size    - Size of the font
*              unsigned int Style  - Style of the font
*              COLORREF mColor - Color of the font
*              int Angle   - Angle of the font
*
*    Output:   HFONT - Handle to the font
************************************************************************/
HFONT MakeFont( HDC DC, char *Name, int Size, unsigned int Style, COLORREF mColor, int Angle, LOGFONT *pLogFont )
{
    LOGFONT Font;

    // Set the color of the font
    SetTextColor( DC, mColor );

    if( !pLogFont )
    {
        // For the MM_TEXT mapping mode, you can use the following formula 
        // to specify a height for a font with a given point size
        // Because we are sending a negitive number, the font mapper transforms this
        // value into device units and matches its absolute value against the character 
        // height of the available fonts
        Font.lfHeight         = -MulDiv( Size, GetDeviceCaps( DC, LOGPIXELSY ), 72 );
        Font.lfWidth          = 0;                // 0 causes Windows to formulate the width
        Font.lfEscapement     = Angle;            // Font rotation made easy
        Font.lfOrientation    = Angle;            // Font rotation made easy
        // Set the defaults
        Font.lfWeight         = FW_REGULAR;
        Font.lfItalic         = 0;
        Font.lfUnderline      = 0;
        Font.lfStrikeOut      = 0;
        
        // Check to see if these bits are set
        if( (Style & FONT_BOLD) == FONT_BOLD )
            Font.lfWeight = FW_BOLD;
        if( (Style & FONT_ITALIC) == FONT_ITALIC )
            Font.lfItalic = 1;
        if( (Style & FONT_UNDERLINE) == FONT_UNDERLINE )
            Font.lfUnderline = 1;
        if( (Style & FONT_STRIKEOUT) == FONT_STRIKEOUT ) 
            Font.lfStrikeOut = 1;

        Font.lfCharSet        = ANSI_CHARSET;     // Works for all situations
        Font.lfOutPrecision   = OUT_STROKE_PRECIS;
        Font.lfClipPrecision  = CLIP_STROKE_PRECIS;
        Font.lfQuality        = DEFAULT_QUALITY;

        // Your supposed to supply a font family just in case the system doesn't have
        // the font requested. This way Windows can pick another font close to the missing font.
        // We'll at this point I don't know what the font family is until I select it into the
        // DC because then I can get that information. If the system doesn't have the font, I can't
        // tell it what family it would have belonged to. You tell me what I should do.
        Font.lfPitchAndFamily = FF_DONTCARE;

        lstrcpy( Font.lfFaceName, Name );
    }

    // Did we fill out our own logfont structure or used the supplied one
    return CreateFontIndirect( (pLogFont ? pLogFont : &Font) );

}   // MakeFont


/************************************************************************
*    DESCRIPTION: Fit source width/Height into Dest Width/Height
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hParent  - Handle of the parent windowe 
*
*    Output:   returntype - Description
************************************************************************/
bool SizeToFitRect( RECT *DestRect, int SourceW, int SourceH, int DestW, int DestH )
{
    double ScreenW, ScreenH, MultiW, MultiH, Multiplyer;
    double ImageW, ImageH;

    // Transfer values to floating point variables
    ImageW = SourceW;
    ImageH = SourceH;
    ScreenW = DestW;
    ScreenH = DestH;

    // The width or the height of any bitmap can't be zero. There is a problem
    if( !ImageW || !ImageH || !ScreenW || !ScreenH )
    {
        PostMsg( "Display Error", "Bitmap has no size" );
        return FALSE;
    }

    // Who is bigger. The image or the screen
    // Divide the largest number against the smaller number
    if( ImageW < ScreenW )
        MultiW = ScreenW / ImageW;
    else
        MultiW = ImageW / ScreenW;

    if( ImageH < ScreenH )
        MultiH = ScreenH / ImageH;
    else
        MultiH = ImageH / ScreenH;                       

    // If the image is smaller than the screen area
    // multiply the multiplyer. If it is bigger than the
    // printable area then divide.
    if( ImageW < ScreenW && ImageH < ScreenH )  // Enlarge to fit
    {
        // See if one side will enlarge past the Screen rect
        // Add 1 to compensate for floating point math to the Screen sizes
        if( ImageW * MultiW > ScreenW+1 || ImageH * MultiW > ScreenH+1 )
            Multiplyer = MultiH;
        else if( ImageW * MultiH > ScreenW+1 || ImageH * MultiH > ScreenH+1 )
            Multiplyer = MultiW;
        else
        {
            // Since none of the Screens extends past the Screen rect
            // take the larger multiplyer
            if( MultiW > MultiH )
                Multiplyer = MultiW;
            else
                Multiplyer = MultiH;
        }

        DestRect->right = (int)(ImageW * Multiplyer);
        DestRect->bottom = (int)(ImageH * Multiplyer);
    }
    else  // Reduce to fit
    {
        // See if one side will enlarge past the Screen rect
        // Add 1 to compensate for floating point math to the Screen sizes
        if( ImageW / MultiW > ScreenW+1 || ImageH / MultiW > ScreenH+1 )
            Multiplyer = MultiH;
        else if( ImageW / MultiH > ScreenW+1 || ImageH / MultiH > ScreenH+1 )
            Multiplyer = MultiW;
        else
        {
            // Since none of the Screens extends past the Screen rect
            // take the smaller multiplyer
            if( MultiW < MultiH )
                Multiplyer = MultiW;
            else
                Multiplyer = MultiH;
        }

        DestRect->right = (int)(ImageW / Multiplyer);
        DestRect->bottom = (int)(ImageH / Multiplyer);
    }

    // Center the image onto the page
    DestRect->left = (int)((ScreenW - DestRect->right) / 2);
    DestRect->top = (int)((ScreenH - DestRect->bottom) / 2);

    return TRUE;

}   // SizeToFitRect


/************************************************************************
*    DESCRIPTION:  This is one way to keep this program from running 
*                  other instances.
*
*    FUNCTION PARAMETERS:
*    Input:    char *SemaphoreName   - Handle to this programs window
*
*    Output:   bool        - Returns true or false
************************************************************************/
bool DoExist( char *SemaphoreName )
{
    bool Result = FALSE;
    HANDLE hSemaphore;

    // Create a semaphore
    hSemaphore = CreateSemaphore( NULL, 0, 1, SemaphoreName );

    // Now, check to see if the semaphore exists
    if( hSemaphore != NULL && GetLastError() == ERROR_ALREADY_EXISTS )
    {
        CloseHandle( hSemaphore );
        Result = TRUE;
    }

    return Result;

}   // DoExist


/************************************************************************
*    DESCRIPTION: The ShowCursor API function increments or
*                 decrements the internal display counter.
*                 The cursor is displayed only if the display
*                 count is greater than or equal to 0. So here,
*                 we are counting up how many times the cursor
*                 has to be hidden so that we can show it the
*                 same number of times when it needs to be shown.
*                 This way the screen saver will leave the internal
*                 display counter just as it found it.
*
*    FUNCTION PARAMETERS:
*    Input:    bool ShowHide  - True for show, false for hide 
************************************************************************/
void ShowHideCursor( bool ShowHide )
{
    if( ShowHide == HIDE_CURSOR )
    {
        // Return value has to be -1 or less to hide
        while( ShowCursor( HIDE_CURSOR ) > -1 )
        {}
    }
    else if( ShowHide == SHOW_CURSOR )
    {
        // Return value has to be 0 or greater to show
        while( ShowCursor( SHOW_CURSOR ) < 0 )
        {}
    }

}   // ShowHideCursor


/************************************************************************
*    DESCRIPTION: Strip the ".ext" of the end of a file path or
*                 file and replace with different extension
*
*    FUNCTION PARAMETERS:
*    Input:    char *filePath - file path
*              char *Ext - Extension string. 
************************************************************************/
void RelpaceFileExtension( char *SourceFileName, char *DestFileName, char *Ext )
{
    int i, Length;

    if( SourceFileName )
        strcpy( DestFileName, SourceFileName );

    // Get the string length
    Length = strlen( DestFileName );

    for( i = Length; i > 0; --i )
    {
        if( DestFileName[i] == '.' )
        {
            DestFileName[i] = 0;
            break;
        }
    }

    // Add on the new file extension
    strcat( DestFileName, Ext );

}   // RelpaceFileExtension


/************************************************************************
*    DESCRIPTION: Returns the file size of the given file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
************************************************************************/
unsigned int GetTheFileSize( char *FileName )
{
    HANDLE hFile;
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned int FileSize = 0;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    hFile = CreateFile( FileName, GENERIC_READ,
                     FILE_SHARE_READ, &FileSecurity, OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL, 0 );

    if( hFile == INVALID_HANDLE_VALUE )
        return FileSize;

    FileSize = GetFileSize( hFile, NULL );

    if( hFile )
        CloseHandle( hFile );

    return FileSize;

}   // GetTheFileSize


/************************************************************************
*    DESCRIPTION: Parses out the *.exe path from the command line.
*                 Sometimes there is junk in the command line
*
*    FUNCTION PARAMETERS:
*    Input:    char *CommandLine  - The command line
*              char *Buffer       - The buffer to place the result in           
************************************************************************/
void ParsePathFromCmdLine( char *CommandLine, char *Buffer )
{
    int length, i;

    // Jump past any junk in the front of the command line
    while( (*CommandLine == '\"' || *CommandLine == ' ') && *CommandLine != 0 )
            CommandLine++;

    // Copy it to the buffer
    strcpy( Buffer, CommandLine );

    // Get the length if the string
    length = strlen( Buffer );

    // Work your way backwards to terminate the string by finding the "."
    for( i = length-1; i > 0; --i )
    {
        if( Buffer[i] == '.' )
        {
            Buffer[i+4] = 0;
            break;
        }   
    }

}   // ParsePathFromCmdLine


/************************************************************************
*    DESCRIPTION: Strip the path from a file name path string.
*                 Also works if the string is just the path 
*                 without a file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
************************************************************************/
void CropOutPath( char *SourceFilePath, char *DestPath )
{
    int i, Length;
    bool ExtensionCheck = FALSE;

    if( SourceFilePath )
        strcpy( DestPath, SourceFilePath );

    // Get the string length
    Length = strlen( DestPath );

    for( i = Length; i > 0; --i )
    {
        // Set the flag if we found the dot extension
        if( DestPath[i] == '.' )
            ExtensionCheck = TRUE;

        if( DestPath[i] == '\\' )
            break;
    }

    // Keep the '\\' if this is the root
    if( i == 2 )
        ++i;

    // Ternamate only if there was an extension
    if( ExtensionCheck )
        DestPath[i] = 0;

}   // CropOutPath


/************************************************************************
*    DESCRIPTION: Takes the file path and returns the file type
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
************************************************************************/
unsigned int GetFileType( char *FileName )
{
    char FileExt[10];
    unsigned int Result = NO_FILE_TYPE;

    if( !FileName || !FileName[0] )
        return NO_FILE_TYPE;

    // Get the file extension
    CropExtensionFromPath( FileName, FileExt );

    // Upper case the file extension for compairison
    strupr( FileExt );

    // Which file format are we loading?
    if( strcmp( FileExt, "BMP" ) == 0 )
        Result = FILE_TYPE_BMP;

    else if( strcmp( FileExt, "TGA" ) == 0 )
        Result = FILE_TYPE_TGA;

    else if( strcmp( FileExt, "JPG" ) == 0 )
        Result = FILE_TYPE_JPG;

    else if( strcmp( FileExt, "JPEG" ) == 0 )
        Result = FILE_TYPE_JPG;

    else if( strcmp( FileExt, "GIF" ) == 0 )
        Result = FILE_TYPE_GIF;

    else if( strcmp( FileExt, "WMF" ) == 0 )
        Result = FILE_TYPE_META;

    else if( strcmp( FileExt, "EMF" ) == 0 )
        Result = FILE_TYPE_META;

    else if( strcmp( FileExt, "WAV" ) == 0 )
        Result = FILE_TYPE_WAV;

    else if( strcmp( FileExt, "MP3" ) == 0 )
        Result = FILE_TYPE_MP3;

    else if( strcmp( FileExt, "MID" ) == 0 )
        Result = FILE_TYPE_MID;

    else if( strcmp( FileExt, "RMI" ) == 0 )
        Result = FILE_TYPE_RMI;

    else if( strcmp( FileExt, "AVI" ) == 0 )
        Result = FILE_TYPE_AVI;

    else if( strcmp( FileExt, "TXT" ) == 0 )
        Result = FILE_TYPE_TXT;

    else if( strcmp( FileExt, "RTF" ) == 0 )
        Result = FILE_TYPE_RTF;

    else if( strcmp( FileExt, "DXF" ) == 0 )
        Result = FILE_TYPE_DXF;

    else if( strcmp( FileExt, "PLY" ) == 0 )
        Result = FILE_TYPE_PLY;

    return Result;

}   // GetFileType


/************************************************************************
*    DESCRIPTION: Strip the file extension from the file path
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
************************************************************************/
void CropExtensionFromPath( char *SourceFilePath, char *DestFileName )
{
    int i, Length;

    // Get the string length
    Length = strlen( SourceFilePath );

    for( i = Length; i > -1; --i )
    {
        if( SourceFilePath[i] == '.' )
            break;
    }

    ++i;

    strcpy( DestFileName, (SourceFilePath + i) );

}   // CropExtensionFromPath


/************************************************************************
*    DESCRIPTION: Strip the file name from the file path
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
************************************************************************/
void CropFileNameFromPath( const char *SourceFilePath, char *DestFileName, BOOL KeepExtension )
{
    int i, Length;

    // Get the string length
    Length = strlen( SourceFilePath );

    for( i = Length; i > -1; --i )
    {
        if( SourceFilePath[i] == '\\' )
            break;
    }

    ++i;

    strcpy( DestFileName, (SourceFilePath + i) );

    // Remove the extension
    if( !KeepExtension )
    {
        // Get the string length
        Length = strlen( DestFileName );

        for( i = Length; i > -1; --i )
        {
            if( DestFileName[i] == '.' )
            {
                DestFileName[i] = 0;
                break;
            }
        }
    }

}   // CropFileNameFromPath


/************************************************************************
*    DESCRIPTION: Search a string and replace a character with
*                 a different character
*
*    FUNCTION PARAMETERS:
*    Input:    char *charString - string to search
*              char find - character to find
*              char replace - character to replace with found character 
************************************************************************/
void RelpaceChar( char *charString, char find, char replace )
{
    int i, length;

    // Get the string length
    length = strlen( charString );

    for( i = 0; i < length; ++i )
    {
        if( charString[i] == find )
            charString[i] = replace;
    }

}   // RelpaceChar


/************************************************************************
*    DESCRIPTION: Add file paths to a string vector
*
*    FUNCTION PARAMETERS:
*    Input:    vector<string> filePath - string vector
*              string dir - directory to search in
*              string ext - extension to look for 
************************************************************************/
void GetAllFileNamesInDir( vector<string> &filePath, string dir, string ext )
{
    WIN32_FIND_DATA FindData;
    string strBuffer;
    HANDLE hSearch;

    // Make sure none of the string params are empty or NULL
    if( (dir != "") && (ext != "") )
    {
        // Init the search string
		strBuffer = dir + "\\" + ext;

        // Find the first file
		//string myStr = strBuffer.str();
		hSearch = FindFirstFile( strBuffer.c_str(), &FindData );

        if( hSearch != INVALID_HANDLE_VALUE )
        {
            do
            {
                // Just be sure not to copy any directory names
                if( !(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
                {
                    // Add the file and it's path to the string list
					strBuffer = dir + "\\" + FindData.cFileName;

                    filePath.push_back( strBuffer );
                }
            }
            while( FindNextFile( hSearch, &FindData ) );

            FindClose( hSearch );
        }
    }

}   // GetAllFileNamesInDir


/************************************************************************
*    DESCRIPTION: Quad memset
*
*    FUNCTION PARAMETERS:
*    Input:    void *dest - array to set
*              unsigned int value - value to set with
*              unsigned int count - array size in unsigned ints
************************************************************************/
void memsetQuad( void *dest, unsigned int value, unsigned int count )
{
    _asm
    {
        mov edi, dest     // edi points to destination memory
        mov ecx, count    // number of 32-bit words to move
        mov eax, value    // 32-bit data
        rep stosd         // move data
    }

}   // memsetQuad


// Returns the width of the computer screen
int GetScreenWidth(){ return GetSystemMetrics( SM_CXSCREEN ); }
// Returns the Height of the computer screen
int GetScreenHeight(){ return GetSystemMetrics( SM_CYSCREEN ); }
// Returns the width of the computer desktop which could include duel monitors
int GetDesktopWidth(){ return GetSystemMetrics( SM_CXVIRTUALSCREEN ); }
// Returns the Height of the computer desktop which could include duel monitors
int GetDesktopHeight(){ return GetSystemMetrics( SM_CYVIRTUALSCREEN ); }