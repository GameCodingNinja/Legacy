
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


#ifndef __TTEXTBLOCK_H__
#define __TTEXTBLOCK_H__

#include "minTools.h"        // A misc group of useful functions
#include "TGBase.h"          // The Graphics Base Object


/************************************************************************
*                       TTextBlock Object
************************************************************************/

class TTextBlock : public TGBase
{
protected:
    // Pointer to NEW allocated memory
    char *pData;  // Pointer to character block
    DWORD count;  // Holds total number of characters
    HFONT hFont;  // Handle to created font
    HFONT hOldFont;  // Handle to old font
    HWND hWndFont;   // Handle of window font was made for

public:
    // Constructor
    TTextBlock();
    // Destructor
    ~TTextBlock(){ Free(); };
    // Load bitmap from file
    virtual BOOL LoadFromFile( char *FileName, BOOL showErrorMsg = TRUE );
    // Load bitmap from resource
    virtual BOOL LoadFromResource( HINSTANCE hInst, char *name, char *type );
    // Get a pointer to a memory block
    virtual BOOL LoadFromBuffer( BYTE *pBuffer, DWORD Size );
    // Is a bitmap loaded
    BOOL IsEmpty();
    // Gets the pointer to the allocated memory pointer
    char *GetPointerToData(){ return pData; };
    // Free memory and palette
    void Free();
    // Returns total number of charactes
    DWORD Count(){ return count; };
    // Free the text
    void FreeBlock();
    // Free the font
    void FreeFont();
    // Create the font
    BOOL MakeNewFont( HWND hwnd, char *Name, int Size, WORD Style = FONT_REGULAR, COLORREF mColor = DEFAULT_BLACK, int Angle = 0, LOGFONT *pLogFont = NULL );   
    // Get the font
    HFONT GetFont(){ return hFont; };
    // Set the font to a window
    //void SetFontToWindow( HWND hwnd, BOOL Redraw = FALSE );
    // Wrap the output text to the device
    DWORD WrapText( HWND hwnd, int LeftMargin, int TopMargin, int TextWidth, int Leading = 0 );
    // Wrap the output text to the device
    DWORD WrapText( HDC DC, int LeftMargin, int TopMargin, int TextWidth, int Leading = 0 );
    // Get the height in pixels of the block of text
    SIZE BlockHeightWidthInPixels( HWND hwnd, char *LineOfText, int Leading = 0 );
};

typedef TTextBlock *PTTextBlock;


#endif  /* __TTEXTBLOCK_H__ */

