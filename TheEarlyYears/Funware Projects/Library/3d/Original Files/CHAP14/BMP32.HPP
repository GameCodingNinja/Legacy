//
// File name: BMP32.CPP
//
// Description: A BMP image class implementation
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef BMP32HPP
#define BMP32HPP

#include <Math.H>
#include <StdIO.H>
#include <Windows.H>

class BMPImage {
protected:
  BITMAPFILEHEADER FileHeader;
  BITMAPINFOHEADER InfoHeader;
public:
  RGBQUAD          *Palette;
  BYTE             *Image;
  long Width, Height, ColorCount, BitCount;

  enum { OpenErr = 0, Success, MemErr, ReadErr, ResErr, CompErr };
  int Load ( char *FileName );
  int SaveBT ( FILE *OutFile );
  int LoadBT ( FILE *InFile );

  int SavePal ( FILE *OutFile );
  int LoadPal ( FILE *InFile );

  ~BMPImage ()
     {
     if ( Palette )
        delete [] Palette;
     if ( Image )
        delete [] Image;
     }
};

#endif
