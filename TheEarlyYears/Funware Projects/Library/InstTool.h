//-------------------------------------------------
// Program: Misc Functions Header File
// File: Howies.h
// Copyright (c) 1997 by Howard C Rosenorn
// Description: Collection of misc Functions
//-------------------------------------------------


#ifndef __INSTALLTOOLS_H__
#define __INSTALLTOOLS_H__


// Constants
#define BUTTON_W            20
#define BUTTON_H            10
#define DEF_BUTTON_W        57
#define DEF_BUTTON_H        25
#define CHECK_SUM_ERROR     0xFFFFFFFF

#define BITMAP_TYPE 0x424d
#define BITMAP_BYTE_ALIGNMENT 4

#define ALLOC_MEM_FOR_BITMAP  0
#define USE_RESOURCE_POINTER  1

// Structures
struct control
{
    char type[20];
    char caption[50];
    HWND hwnd; //handle
    int x;
    int y;
    int w;
    int h;
};

// Prototypes
void CenterWindow(HWND hwnd, int Width, int Height);
// Checksum the file
DWORD CheckSumFile( HANDLE hFile );
DWORD CheckSumFile( char *FileName );
void ParsePathFromCmdLine( char *CommandLine, char *Buffer );
void ApplyWin95Font(HWND hwnd);
void sizeButtonWidthHeight(HWND hwnd, control &T);
void CropOutPath( char *SourceFilePath, char *DestPath );
#ifdef DEBUG
void PrintText(HWND hwnd, int X, int Y, char* S);
void PrintText(HWND hwnd, int X, int Y, int I);
#endif
int ShowMessageBox(HINSTANCE hInst, HWND hwnd,  int resMessage, int resTitle, UINT uType);
HANDLE loadFromResource(HINSTANCE hInst, DWORD &size, char *name, char *type);
BOOL IsEnoughDiskSpace( char *FileName, DWORD FileSize );


/************************************************************************
*                STRIP DOWN VERSION OF THE TDIB Object
*                THIS VERSION IS JUST FOR THE INSTALLER
************************************************************************/


class TDIB
{
protected:
   // Calculates to offset to the bitmap bits
   int Get_biSize() { return Fdib->bmiHeader.biSize + (getDIBcolors() * sizeof(RGBQUAD)); };
   // Deletes the palette
   void deletePalete();
   // Returns the bit depth of this graphic
   int GetBitCount() { return Fdib->bmiHeader.biBitCount; };
   // Returns the number of colors in the bitmap's palette
   int getDIBcolors();
   // Free memory and palette
   void Free();

   // pointer to BITMAPINFO structur
   BITMAPINFO* Fdib;
   // Handle to allocated memory
   HANDLE hLoad;
   // Handle to bitmap file
   HANDLE hBmpFile;
   // Handle to palette
   HPALETTE hPalette;
   // Size in bytes of allocated memory
   DWORD size;

public:
   // Constructor
   TDIB();
   // Destructor
   ~TDIB();
   // Load bitmap from resource
   virtual BOOL loadFromResource(HINSTANCE hInst, char *name, char *type, int PointerType);
   // Get the width
   virtual int width() { return Fdib->bmiHeader.biWidth; };
   // Get the height
   virtual int height() { return Fdib->bmiHeader.biHeight; };
   // Get the palette
   HPALETTE GetPalette();
   // Is a bitmap loaded
   virtual BOOL IsEmpty();
   // Get pointer to bitmap bits
   BYTE* bits() { return (BYTE *)Fdib + Get_biSize(); };
   // Get pointer to BITMAPINFO structure
   BITMAPINFO* dib() { return Fdib; };   
};


#endif  /* __INSTALLTOOLS_H__ */


