//
// File name: Mouselib.CPP
//
// Description: Provides support for various mouse functions
//              under 32-bit protected mode
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#include "MouseLib.hpp"

void MousePtr::Display ( unsigned char *Buffer )
   {
   Update ();
   if ( ( CImage != NULL ) && ( Buffer != NULL ) )
      {
      int HWidth = Width >> 1;
      int HHeight = Height >> 1;
      int XC = ( X >> 1 ) + XCenter;
      int YC = Y + YCenter;
      int Left = XC - HWidth;
      int Right = XC + HWidth;
      int Top = YC - HHeight;
      int Bottom = YC + HHeight;
      ClipPoint ( Left, Top );
      ClipPoint ( Right, Bottom );
      unsigned char *CPtr = CImage;
      for ( int Yp = Top; Yp < Bottom; Yp++ )
          {
          unsigned char *LPtr = &Buffer [ Yp * 320 + Left ];
          for ( int Xp = Left; Xp < Right; Xp++ )
              {
              if ( *CPtr != 0 )
                 *LPtr = *CPtr;
              LPtr++; CPtr++;   
              }
          }
      }
   }

int ResetMouse ( void )
  {
  REGS InReg, OutReg;
  InReg.w.ax = 0x0001;
  int386 ( 0x33, &InReg, &OutReg );
  return ( OutReg.w.ax );
  }

void ShowMouse ( void )
   {
   
   REGS InReg, OutReg;
   InReg.w.ax = 0x0001;
   int386 ( 0x33, &InReg, &OutReg );
   }

void HideMouse ( void )
   {
   REGS InReg, OutReg;
   InReg.w.ax = 0x0002;
   int386 ( 0x33, &InReg, &OutReg );
   }

void SetMousePos ( int X, int Y )
   {
   REGS InReg, OutReg;
   InReg.w.ax = 0x0004;
   InReg.w.cx = (unsigned short) X;
   InReg.w.dx = (unsigned short) Y;
   int386 ( 0x33, &InReg, &OutReg );
   }

void GetMousePos ( int &X, int &Y )
   {
   REGS InReg, OutReg;
	InReg.w.ax = 0x0003;
	int386 ( 0x33, &InReg, &OutReg );
	X = OutReg.w.cx;
	Y = OutReg.w.dx;
   }

void GetButtonPos ( int &Lb, int &Rb )
   {
   REGS InReg, OutReg;
	InReg.w.ax = 0x0003;
	int386 ( 0x33, &InReg, &OutReg );
	Lb = ( OutReg.w.bx & 1 );
	Rb = ( OutReg.w.bx & 2 ) >> 1;
   }
