//
// File name: Palette.HPP
//
// Description: A palette setting inline function.
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//
// Target: 32-bit OS
//

#ifndef PALETTEHPP
#define PALETTEHPP

#include <DOS.H>

void inline SetPalReg ( long Index, char Red, char Green, char Blue )
   {
   REGS Regs;
   Regs.w.ax  = 0x1010;
   Regs.x.ebx = Index;
   Regs.h.ch  = Red;
   Regs.h.cl  = Green;
   Regs.h.dh  = Blue;
   int386 ( 0x10, &Regs, &Regs );
   }

void inline setpalreg ( long Index, char Red, char Green, char Blue )
   {
   REGS Regs;
   Regs.w.ax  = 0x1010;
   Regs.x.ebx = Index;
   Regs.h.ch  = Red;
   Regs.h.cl  = Green;
   Regs.h.dh  = Blue;
   int386 ( 0x10, &Regs, &Regs );
   }

#endif