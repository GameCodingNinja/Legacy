
/************************************************************************
*
*    PRODUCT:         Screen Saver To Go
*
*    FILE NAME:       BldBuf.cpp
*
*    DESCRIPTION:     Code for building and initilizing the buffer 
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

#include "BldBuf.h"          // Header file for this *.cpp file.
#include "S2GStrct.h"        // Screen Savers To Go structure header file
#include "gTools.h"          // A misc group of useful functions


/************************************************************************
*    FUNCTION NAME:         BldBuffer_ResetBuffer                                                             
*
*    DESCRIPTION:           Resize the window to the parent
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hParent  - Handle of the parent windowe 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void BldBuffer_ResetBuffer( PTBitmap pBuffer, PTBitmap pMainDib, PTBitmap pBackDib, int CurPage, PTPageList pPageList )
{
    RECT BufferRect;
    int BackFileType;
    HBRUSH brush;

    // Setup the rect
    SetRect( &BufferRect, 0, 0, pBuffer->width(), pBuffer->height() );

	// Make sure we have some pages here
	if( pPageList->Count() )
	{
		// Just to be safe, make sure the current page doesn't exceed the total number of pages
		if( CurPage >= pPageList->Count() )
			CurPage = pPageList->Count()-1;

	    // Get the data type
	    BackFileType = pPageList->GetBackDataType( CurPage );

	    // Blit the background image to the buffer if there is an image to use
	    if( BackFileType && BackFileType != FILE_TYPE_COLOR )
	        BldBuffer_BlitBackImage( pBuffer, pBackDib );
	    else
	    {
	        // Use the selected color
	        if( BackFileType == FILE_TYPE_COLOR )
	        {
	            // Create a brush
	            brush = CreateSolidBrush( pPageList->GetBackColor( CurPage ) );

	            // Fill the rect with the color
	            pBuffer->ColorRect( &BufferRect, brush );

	            // Delete the brush
	            DeleteObject( brush );
	        }
	        else
	            // Clear out the buffer by coloring the buffer white
	            pBuffer->ColorRect( &BufferRect, WHITE_BRUSH );
	    }

	    // Blit the main image
	    BldBuffer_BlitMainImage( pBuffer, pMainDib, CurPage, pPageList );

	    // Does the top caption have text?
	    if( pPageList->IsTopCaptionText( CurPage ) )
	        BldBuffer_TextToBuffer( pBuffer, pPageList->GetPtrToTopCaption( CurPage ), 
	                              pPageList->GetPtrToTopLogfont( CurPage ),
	                              pPageList->GetTopFontColor( CurPage ),
	                              pPageList->GetTopTxtJust( CurPage ),
	                              CurPage, EDT_TEXT_TOP_MSG );

	    // Does the bottom caption have text?
	    if( pPageList->IsBotCaptionText( CurPage ) )
	        BldBuffer_TextToBuffer( pBuffer, pPageList->GetPtrToBotCaption( CurPage ), 
	                              pPageList->GetPtrToBotLogfont( CurPage ), 
	                              pPageList->GetBotFontColor( CurPage ), 
	                              pPageList->GetBotTxtJust( CurPage ),
	                              CurPage, EDT_TEXT_BOTTOM_MSG );
	}
	else
	    // Fill the rect with the color
	    pBuffer->ColorRect( &BufferRect, WHITE_BRUSH );

}   // BldBuffer_ResetBuffer


/************************************************************************
*    FUNCTION NAME:         BldBuffer_TextToBuffer                                                             
*
*    DESCRIPTION:           Output text to the buffer
*
*    FUNCTION PARAMETERS:
*    Input:    int TextRegion  - Top text or bottom text 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void BldBuffer_TextToBuffer( PTBitmap pBuffer, char *Text, LOGFONT *TextFont, COLORREF TextColor, int TxtJust, int CurPage, int TextRegion )
{
    HFONT hFont, hOldFont;
    SIZE Result;
    int TextYOffset;

    // Set the color of the font
    SetTextColor( pBuffer->Handle(), TextColor );

    // Make the font
    hFont = CreateFontIndirect( TextFont );

    // Select the new font into the buffer
    hOldFont = SelectFont( pBuffer->Handle(), hFont );

    // Get the font size of the line of text. Then we can set the margin
    // to the height and width of the text
    GetTextExtentPoint32( pBuffer->Handle(), "W", strlen("W"), &Result );

    // Set the background mode to transparent
    SetBkMode( pBuffer->Handle(), TRANSPARENT );

    // If this is the bottom text, we need to find the hight in pixels
    // the lines of text are.
    if( TextRegion == EDT_TEXT_BOTTOM_MSG )
    {
        TextYOffset = WrapTextToDevice( pBuffer->Handle(), Text, Result.cx, 0, 
                                        pBuffer->width()-(Result.cx*2), 0, 0, RETURN_HEIGHT_IN_PIXELS, TxtJust );

        // Make sure it's not taller than the screen
        if( TextYOffset < pBuffer->height() )
            TextYOffset = pBuffer->height() - TextYOffset - (Result.cy/4);
    }
    else
        TextYOffset = (Result.cy/4);    

    // Blit the text
    WrapTextToDevice( pBuffer->Handle(), Text, Result.cx, TextYOffset, 
                      pBuffer->width()-(Result.cx*2), 0, 0, SCREEN_OUTPUT, TxtJust );

    // Set the background mode back to OPAQUE
    SetBkMode( pBuffer->Handle(), OPAQUE );

    // The old font needs to be selected into the
    // DC before the created font can be deleted
    SelectFont( pBuffer->Handle(), hOldFont );

    // Delete the font
    DeleteFont( hFont );

}   // BldBuffer_TextToBuffer



/************************************************************************
*    FUNCTION NAME:         BldBuffer_BlitMainImage                                                             
*
*    DESCRIPTION:           Blit the main image to the buffer
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hParent  - Handle of the parent windowe 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void BldBuffer_BlitMainImage( PTBitmap pBuffer, PTBitmap pMainDib, int CurPage, PTPageList pPageList )
{
    RECT BufferRect;
    BOOL SizeToFit = FALSE;
    int FrameBorder;

    // Do we have a main image?
    if( !pBuffer->IsEmpty() )
    {
        // Calculate the frame border
        FrameBorder = pBuffer->width() / 20;

        // If the image is bigger than the screen, or the "Size to fit" check box
        // has been checked, size the bitmap to fit
        if( pMainDib->width() > (pBuffer->width()-(FrameBorder*2)) ||
            pMainDib->height() > (pBuffer->height()-(FrameBorder*2)) ||
            pPageList->GetSizeToFit( CurPage ) )
        {
            SizeToFit = TRUE;
        }

        // Size the main image to fit the buffer
        if( SizeToFit )
        {
            // Size the main image to fit into the buffer size
            if( SizeToFitRect( &BufferRect, pMainDib->width(), pMainDib->height(), 
                               pBuffer->width()-(FrameBorder*2), pBuffer->height()-(FrameBorder*2) ) )
            {
                // Add in the frame
                BufferRect.left += FrameBorder;
                BufferRect.top += FrameBorder;

		        // This ensures the niceest looking stretch
		        SetStretchBltMode( pBuffer->Handle(), HALFTONE );

                // Blit the main image to the buffer
                pMainDib->BlitToDC( pBuffer->Handle(), &BufferRect );

		        // Resets every thing back after a stretch
		        SetBrushOrgEx( pBuffer->Handle(), 0, 0, NULL );
            }
        }
        else  // Center the main image in the buffer
        {
            // Blit the main image to the buffer
            pMainDib->BlitToDC( pBuffer->Handle(), abs(pBuffer->width() - pMainDib->width()) / 2,
                                abs(pBuffer->height() - pMainDib->height()) / 2 );
        }
    }

}   // BldBuffer_BlitMainImage



/************************************************************************
*    FUNCTION NAME:         BldBuffer_BlitBackImage                                                             
*
*    DESCRIPTION:           Blit the back image to the buffer
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hParent  - Handle of the parent windowe 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void BldBuffer_BlitBackImage( PTBitmap pBuffer, PTBitmap pBackDib )
{
    int Row, Col, i, j;

    // Do we have a main image?
    if( !pBackDib->IsEmpty() )
    {
        // Calculate the number of rows and colums
        if( pBuffer->width() % pBackDib->width() )
            Row = (pBuffer->width() / pBackDib->width()) + 1;
        else
            Row = pBuffer->width() / pBackDib->width();

        if( pBuffer->height() % pBackDib->height() )
            Col = (pBuffer->height() / pBackDib->height()) + 1;
        else
            Col = pBuffer->height() / pBackDib->height();

        // Tile the bitmat to the buffer
        for( j = 0; j < Col; ++j )
        {
            for( i = 0; i < Row; ++i )
            {
                pBackDib->BlitToDC( pBuffer->Handle(), pBackDib->width() * i, pBackDib->height() * j );
            }
        }
    }

}   // BldBuffer_BlitMainImage