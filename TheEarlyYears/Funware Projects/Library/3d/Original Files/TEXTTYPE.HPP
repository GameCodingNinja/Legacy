//
// File name: TextType.HPP
//
// Description:
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef TEXTTYPEHPP
#define TEXTTYPEHPP

#include <StdIO.H>

#include "BMP32.HPP"

class ImageData {
protected:

public:
  ImageData () { TCount = 0; }

  BMPImage *TMap;
  DWORD     TCount;

  int LoadINI ( char *FileName );
  int LoadBT ( FILE *InFile );
  int SaveBT ( FILE *OutFile );

  int LoadPal ( FILE *InFile )
    {
    return TMap [ 0 ].LoadPal ( InFile );
    }
  int SavePal ( FILE *OutFile )
    {
    return TMap [ 0 ].SavePal ( OutFile );
    }
  ~ImageData () { if ( TMap ) delete [] TMap; }
};

extern ImageData TextDat;

#endif