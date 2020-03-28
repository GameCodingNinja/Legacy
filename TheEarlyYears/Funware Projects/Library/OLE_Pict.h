
/************************************************************************
*
*    PRODUCT:         OLE Ipicture Object
*
*    FILE NAME:       OLE_Pict.h
*
*    DESCRIPTION:     Class that loads BMP, GIF, JPG, WMF, EMF, ICO
*                     using the Windows OLE IPicture COM interface.
*
*                     Being able to load GIF and JPEG is what really
*                     makes IPicture usefull.
*
*    IMPLEMENTATION:  Load and blits BMP, GIF, JPG, WMF, EMF, ICO
*
************************************************************************/


#ifndef __OLE_PICT_H__
#define __OLE_PICT_H__

#include <olectl.h>

// number of hi-metric units (.01mm) per inch
#define HIMETRICINCH  2540    


/************************************************************************
*                         OLE_Pict Object
************************************************************************/

class COLE_Pict
{
protected:
    // Pointer to OLE picture object
    LPPICTURE pPict;
    // Width and height in pixels
    int width, height;
    // Width and height in himetric
    int widthHi, heightHi;
    // Handle to picture
    HANDLE hHandle;
    // Handle to DC
    HDC hDC;
    // data size in bytes
    unsigned long dataSizeInbytes;  

    // Get the width from the loaded image
    int GetWidth();
    // Get the width from the loaded image
    int GetHeight();
    // Width in himetric
    int WidthInHimetric();
    // Height in himetric
    int HeightInHimetric();

public:
    // Constructor
    COLE_Pict();
    // Destructor
    ~COLE_Pict(){ Free(); };
    // Free the memory
    void Free();
    // Free the picture
    void FreePicture();
    // Free the global memory
    void FreeGlobalMemory();

    // Load Picture obj from file
    bool LoadFromFile( char *fileName );
    // Load Picture obj from data pointer
    bool LoadFromPointer( LPVOID pDataPtr, unsigned int size );
    // Fill an array with bitmap bits
    bool GetBitmapBits( unsigned char *pData, int width, int height, int bitCount );
    // Get the width
    int Width(){ return width; };
    // Get the height
    int Height(){ return height; };
    // Is a bitmap loaded
    bool IsEmpty();
    // Do a simple full size blit to a device.
    void Render( HDC hDestDC, RECT *destRect, RECT *metafileRect = NULL );
    // Get the handle of the bitmap
    HANDLE GetHandle();
    // Get the DC of the picture. This only relates to bitmaps.
    HDC GetHDC();
    // Get the size in byte of memory we are holding
    unsigned int GetSizeInBytes(){ return dataSizeInbytes; };   
};


#endif  // __OLE_PICT_H__


