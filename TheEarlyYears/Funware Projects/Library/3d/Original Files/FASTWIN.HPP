//
// File name: FastWin.HPP
//
// Description: The CreateDIBSection object.
//
// Author: John De Goes
//
// Project:
//
// Target:
//

#ifndef FASTWINHPP
#define FASTWINHPP

#include <Math.H>
#include <Windows.H>
#include <WindowsX.H>

class FastWin {
protected:

 struct MyBITMAPINFO {
   BITMAPINFOHEADER bmiHeader;
   RGBQUAD          bmiColors [ 256 ];
 } *BMInfo;
 
 struct MyLOGPALETTE {
   WORD         palVersion;
   WORD         palNumEntries;
   PALETTEENTRY palPalEntry [ 256 ];
 } *PalInfo;

 HBITMAP    BMHan;     // A handle to the bitmap
 BYTE       *Bits;     // A pointer to variable to receive a pointer to the bitmap's bit values

public:
  FastWin () : BMInfo ( NULL ), Bits ( NULL ), PalInfo ( NULL ) { }
  ~FastWin () { delete BMInfo; delete PalInfo; DeleteObject ( BMHan ); }
  void FastWin::Init ( DWORD Width, DWORD Height, RGBQUAD *Pal = NULL );
  operator unsigned char * () { return Bits; }
  void Show ();
  void Colors ();
};

#endif
