/************************************************************************
*
*    PRODUCT:         Screen Saver Thread
*
*    FILE NAME:       SSAnim.cpp
*
*    DESCRIPTION:     Thread that runs the screen saver
*
*    IMPLEMENTATION:  This thread is screen saver specific. Here is where
*                     you initilize your screen saver, run it and then
*                     free all allocated memory and resources. 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "gdefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
                             // Note: we are not using the #define WIN32_LEAN_AND_MEAN because we need some multimedia functions
                             // WIN32_LEAN_AND_MEAN leaves out

#include "reels.h"           // Header file for this *.cpp file.
#include <stdlib.h>          // Declares several commonly used routines such as atoi, random, abs, malloc etc.


/************************************************************************
*    FUNCTION NAME:         Constructor                                                             
************************************************************************/
CReel::CReel()
{
    stopPos = 0;

}   // Constructor


/************************************************************************
*    FUNCTION NAME:         Destructor                                                             
************************************************************************/
CReel::~CReel()
{

            
}   // Destructor


/************************************************************************
*
*    DESCRIPTION:           Clip the destination height.
*
*    Input:    TRect &Dest      - Destination rect
*
*    Output:   bool True = allow blit, false = don't blit
************************************************************************/
bool CReel::ClipHeight( RECT &Dest )
{
    // Make sure we have something to blit and if the rects are in the buffer region.
    // If the rect is completely off the buffer, get out because there is nothing to do.
    if( Dest.top >= blitMaxH || Dest.top + Dest.bottom < blitMaxY )
    {
        return false;
    }

    // See if the rect is within the buffer region. If it is all there,
    // we have nothing to do here so get out now.
    if( Dest.top >= blitMaxY && Dest.top + Dest.bottom <= blitMaxH )
    {
        Dest.top = 0;
        return true;
    }
    
    // Clip the Y and reset the height of the rect
    if( Dest.top < blitMaxY )
    {
        // reset the Y of the rect
        Dest.top = blitMaxY - Dest.top;
                
        // Reset the height of the rect
        // This is the same as Height - Y without the abs()
        Dest.bottom = Dest.bottom - Dest.top;

        return true;
    }
        
    // Clip the height of the destination rect
    if( Dest.top + Dest.bottom > blitMaxH )
    {
        Dest.bottom = blitMaxH - Dest.top;
        Dest.top = 0;
    }  

    return true;

}   // ClipHeight


/************************************************************************
*    DESCRIPTION:           Set the symbol position 
*
*    Output:   bool - True or false depending on if successful
************************************************************************/
void CReel::SetReelPos()
{
    int i;

    // Set the symbol index
    symb[0].index = stopPos - 2;

    if( symb[0].index < 0 )
        symb[0].index = pPhyReel->count + symb[0].index;

    // Set the symbol index
    symb[1].index = stopPos - 1;

    if( symb[1].index < 0 )
        symb[1].index = pPhyReel->count + symb[1].index;

    // Set the symbol index
    symb[2].index = stopPos;

    // Set the symbol index
    symb[3].index = (stopPos + 1) % pPhyReel->count;

    // Set the symbol index
    symb[4].index = (stopPos + 2) % pPhyReel->count;

    for( i = 0; i < MAX_SYMBOL_CYCLE; ++i )
        symb[i].vector = 0;

}   // SetReelPos

/************************************************************************
*    DESCRIPTION:           init the symbols
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
************************************************************************/
void CReel::Init( CBitmap * pSymb, int x, int y )
{
    int i;

    pSymbol = pSymb;

    blitMaxY = y;
    blitMaxH = (SYMB_H * 3) + blitMaxY;
    offsetX = x;
    offsetY = blitMaxY + SYMB_H;

    for( i = 0; i < MAX_SYMBOL_CYCLE; ++i )
    {
        symb[i].index = i;
        symb[i].defPos = -(SYMB_H * 2) + (i * SYMB_H);
        symb[i].vector = 0;
    }

}   // InitSymbols


/************************************************************************
*    DESCRIPTION:           Animate the symbols
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
************************************************************************/
void CReel::Animate( int vector )
{
    int i;
    RECT dst, src;

    for( i = 0; i < MAX_SYMBOL_CYCLE; ++i )
    {
        // Inc the vector
        symb[i].vector += vector;
        
        // Check if this is a forward movement or a backwards movement
        if( vector > 0 )
        {
            // movement finished, reset the symbol
            if( symb[i].vector >= SYMB_H )
            {
                symb[i].vector = symb[i].vector - SYMB_H;
                --symb[i].index;

                if( symb[i].index < 0 )
                    symb[i].index = pPhyReel->count-1;
            }
        }
        else
        {
            // movement finished, reset the symbol
            if( symb[i].vector <= -SYMB_H )
            {
                symb[i].vector = symb[i].vector + SYMB_H;
                ++symb[i].index;

                if( symb[i].index >= pPhyReel->count )
                    symb[i].index = 0;
            }
        }
        
        // Set the source and destination rects
        SetRect( &dst, offsetX, offsetY + symb[i].defPos + symb[i].vector, SYMB_W, SYMB_H );
        SetRect( &src, 0, offsetY + symb[i].defPos + symb[i].vector, SYMB_W, SYMB_H );

        // Clip the source rect if it can blit
        if( ClipHeight( src ) )
        {
            // Update the destination rect with the clipped info
            dst.bottom = src.bottom;
            dst.top += src.top;
            
            // Blit the symbol
            pSymbol[ pPhyReel->phyReel[ symb[i].index ] ].Draw( outputDC, &dst, &src );
        } 
    }

}   // Animate