
/************************************************************************
*
*    FILE NAME:       ipicture.h
*
*    DESCRIPTION:     Class that loads BMP, GIF, JPG, WMF, EMF, ICO
*                     using the Windows IPicture COM interface.
*
*                     Being able to load GIF and JPEG is what really
*                     makes IPicture usefull.
*
*    IMPLEMENTATION:  Load and blits BMP, GIF, JPG, WMF, EMF, ICO
*
************************************************************************/

#ifndef __i_picture_h__
#define __i_picture_h__

// number of hi-metric units (.01mm) per inch
#define HIMETRICINCH  2540    

// Windows lib dependencies
#include <olectl.h>

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include "utilities\\smartpointers.h"
#include "common\\size.h"
#include "common\\rect.h"

class CIPicture
{
public:
    // Constructor
    CIPicture();

    // Load Picture obj from file
	bool LoadFromFile( std::string & filePath );

    // Load from resource
    bool LoadFromResource( std::string & name, std::string & type, HINSTANCE hInst );

    // Fill an array with bitmap bits
    bool GetBitmapBits( unsigned char *pData, int width, int height, int bitCount );

    // Get the width
    int GetWidth()
	{ return size.w; };

    // Get the height
    int GetHeight()
	{ return size.h; };

    // Is a image loaded?
    bool IsEmpty()
	{ return pIPict.isNull(); }

    // Do a simple full size blit to a device.
    void Render( HDC hDestDC, CRect<int> & destRect );
	void Render( HDC hDestDC, CRect<int> & destRect, CRect<int> & metafileRect );
	void Render( HDC hDestDC, int x = 0, int y = 0 );

    // Get the handle of the bitmap
    HANDLE GetHandle()
	{ return hHandle; }

protected:

    // Pointer to OLE picture object
    LPPICTURE pPict;

private:

	// Calculate the width and height of image
	void CalcImageSize();

	// Get the standard window handles
	void InitHandle();

	// Reset the data
	void Reset();

	// Load Picture obj from supplied stream
	bool Load( HGLOBAL hGlobal, std::string & errorInfo );

private:

	// Safe pointer to IPicture comm object
	scoped_comm_object_ptr<IPicture2> pIPict;

	// Size in pixels
	CSize<int> size;

	// Size in himetric
	CSize<int> sizeHi;

	// Handle to picture
    HANDLE hHandle;

};

#endif  // __i_picture_h__


