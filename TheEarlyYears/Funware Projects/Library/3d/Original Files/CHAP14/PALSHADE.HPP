//
// File name: PalShade.HPP
//
// Description: The header file for a palette shading class
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef PALSHADEHPP
#define PALSHADEHPP

#include <StdIO.H>
#include <Windows.H>

const SHADE_COUNT = 32, COLOR_COUNT = 256;

class ShadeDat {
protected:

public:
  char Shade [ SHADE_COUNT * COLOR_COUNT ];
  void GenTable ( double, double, double, RGBQUAD * );
  int LoadTable ( char *FileName );
  int SaveTable ( char *FileName );
};

extern ShadeDat TextShade;

int GetColor ( int Red, int Green, int Blue, RGBQUAD *Pal );

#endif
