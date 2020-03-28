
/************************************************************************
*
*    PRODUCT:         Screen Saver To Go
*
*    FILE NAME:       BldBuf.h
*
*    DESCRIPTION:     Code for building and initilizing the buffer 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __BUILD_BUFFER_H__
#define __BUILD_BUFFER_H__

#include "TBitmap.h"         // Dib and Bitmap class for displaying and animating bitmaps
#include "PageList.h"        // Dynamic Allocation of pages

// Resizes the view window
void BldBuffer_ResetBuffer( PTBitmap pBuffer, PTBitmap pMainDib, PTBitmap pBackDib, int CurPage, PTPageList pPageList );
// Blit the main image to the buffer
void BldBuffer_BlitMainImage( PTBitmap pBuffer, PTBitmap pMainDib, int CurPage, PTPageList pPageList );
// Blit the back image to the buffer
void BldBuffer_BlitBackImage( PTBitmap pBuffer, PTBitmap pBackDib );
// Send text to the buffer
void BldBuffer_TextToBuffer( PTBitmap pBuffer, char *Text, LOGFONT *TextFont, COLORREF TextColor, int TxtJust, int CurPage, int TextRegion );


#endif  // __BUILD_BUFFER_H__