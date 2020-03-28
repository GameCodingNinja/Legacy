
/************************************************************************
*
*    PRODUCT:         Screen Saver Thread
*
*    FILE NAME:       reels.h
*
*    DESCRIPTION:     Reels class 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __CREELS_H__
#define __CREELS_H__

#include "dib.h"         // Dib and Bitmap class for displaying and animating bitmaps
#include "defs.h"            // Screen saver defs file


/***********************************************************************
*  Reel class
***********************************************************************/

class CReel
{
protected:

	// Symbol Movement
	SSymb symb[MAX_SYMBOL_CYCLE];
    // Image buffers
    CDib * pSymbol;
	// Height offsets
	int blitMaxY, blitMaxH, reelCenter;
	// DC
	HDC outputDC;
	// Offsets
	int offsetX, offsetY;
	// The stop position
	int stopPos;
	// The width and height of the symbols
	int symbW, symbH;
	// Physical reel pointer
	SPhysicalReel * pPhyReel;

	// Clip the destination height
	bool ClipHeight( RECT &Dest );

public:

	// Constructor
	CReel();
	// Destructor
	~CReel();

	// init the symbol pointer
	void InitSymbols( CDib * pSymb );
	// Set the DC
	void SetDC( HDC dc ){ outputDC = dc; };
	// Animate the symbols
	void Animate( int vector );
	// Set the offsets
	void SetOffsets( int x, int y ){ offsetX += x; offsetY += y; };
	// Set the stop position
	void SetStopPos(){ stopPos = rand() % pPhyReel->count; };
	// Set reel positions
	void SetReelPos();
	// Poll the reel until it stops on the selected symbol
	int GetStopStatus(){ return (symb[2].index == stopPos); };
	// init the reel resolution
	void InitResolution( int res );
	// init the physical reel
	void InitPhysicalReel( SPhysicalReel * pReel ){ pPhyReel = pReel; };
};


#endif  // __CREELS_H__
