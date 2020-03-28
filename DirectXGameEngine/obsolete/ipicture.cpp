
/************************************************************************
*    FILE NAME:       ipicture.cpp
*
*    DESCRIPTION:     Class that loads BMP, GIF, JPG, WMF, EMF, ICO
*                     using the Windows OLE IPicture COM interface.
*
*                     Being able to load GIF and JPEG is what really
*                     makes IPicture usefull.
*
*    IMPLEMENTATION:  Load and blits BMP, GIF, JPG, WMF, EMF, ICO
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

// Physical component dependency
#include "utilities\\ipicture.h"

// Game lib dependencies
#include "utilities\\genfunc.h"
#include "utilities\\smarthandles.h"

// Required namespace(s)
using namespace std;

// Turn off fopen deprecation warning
#pragma warning(disable : 4996)

/************************************************************************
*    desc:  Constructer
************************************************************************/
CIPicture::CIPicture()
         : hHandle(NULL)
{
}   // constructor


/************************************************************************
*    desc:  Reset the data
************************************************************************/
void CIPicture::Reset()
{
    hHandle = NULL;
    size.Reset();
    sizeHi.Reset();

}  // FreePicture


/***************************************************************************
*   desc:  Load Picture obj from file
*
*   param:  FileName - Path to file
*
*   ret:  bool - true on success or false on fail
 ****************************************************************************/
bool CIPicture::LoadFromFile( string & filePath )
{
    // Reset the data
    Reset();

    // Open the file
    scoped_filehandle_ptr<FILE> pFile( fopen( filePath.c_str(), "rb" ) );
    if( pFile.isNull() )
    {
        NGenFunc::PostMsg( "Load File Error", "Error Loading file (%s).", filePath.c_str() );
        return false;
    }

    // Get the file size of the picture so that we can allocate enough memory for it
    fseek(pFile.get(), 0L, SEEK_END);
    unsigned int dataSizeInbytes = ftell(pFile.get());
    fseek(pFile.get(), 0L, SEEK_SET);
    if( dataSizeInbytes == 0 || dataSizeInbytes == 0xFFFFFFFF )
    {
        NGenFunc::PostMsg( "Load File Error", "File has no size (%s).", filePath.c_str() );
        return false;
    }

    // alloc memory based on file size
    scoped_global_handle<HGLOBAL> hGlobal( GlobalAlloc( GMEM_MOVEABLE, dataSizeInbytes ) );
    if( hGlobal.isNull() )
    {
        NGenFunc::PostMsg( "Load File Error", "Unable to allocate global memory (%s).", filePath.c_str() );
        return false;
    }

    // See if we can get the global pointer
    if( hGlobal.GetGlobalPtr() == NULL )
    {
        NGenFunc::PostMsg( "Load File Error", "Unable to lock down global memory (%s).", filePath.c_str() );
        return false;
    }

    // Read contents of file into allocated memory
    unsigned long bytesRead = fread( hGlobal.GetGlobalPtr(), 1, dataSizeInbytes, pFile.get() );

    // Make sure the file read in correctly
    if( bytesRead != dataSizeInbytes )
    {
        NGenFunc::PostMsg( "Load File Error", "Error reading data from file (%s).", filePath.c_str() );
        return false;
    }

    // Now try to load the file from the HGLOBAL
    if( !Load( hGlobal.get(), filePath ) )
        return false;

    return true;

}	// LoadFromFile


/***************************************************************************
*   desc:  Load Picture obj from resource
*
*   param:  name  - name of resource
*   param:  type  - type of resource
*   param:  hInst - instance of program
*
*   ret:  bool - true on success or false on fail
 ****************************************************************************/
bool CIPicture::LoadFromResource( string & name, string & type, HINSTANCE hInst )
{
    // Reset the data
    Reset();

    // Find the resource
    HRSRC hRes = FindResource(hInst, name.c_str(), type.c_str());
    if( hRes == NULL )
    {
        NGenFunc::PostMsg( "Error", "Can't find resource (%s).", name.c_str() );
        return false;
    }

    // Get the size of the resource
    unsigned int dataSizeInbytes = SizeofResource( hInst, hRes );
    if( dataSizeInbytes == 0 || dataSizeInbytes == 0xFFFFFFFF )
    {
        NGenFunc::PostMsg( "Error", "Resource (%s) has no size.", name.c_str() );
        return false;
    }

    // Load the resource
    HANDLE handle = LoadResource( hInst, hRes );
    if( handle == NULL )
    {
        NGenFunc::PostMsg( "Error", "Can't load resource (%s).", name.c_str() );
        return false;
    }

    // Get a pointer to the memory block
    void * pDataPtr = (unsigned char *)LockResource( handle );
    if( pDataPtr == NULL )
    {
        NGenFunc::PostMsg( "Error", "Can't lock resource (%s).", name.c_str() );
        return false;
    }

    // alloc memory based on file size
    scoped_global_handle<HGLOBAL> hGlobal( GlobalAlloc( GMEM_MOVEABLE, dataSizeInbytes ) );
    if( hGlobal.isNull() )
    {
        NGenFunc::PostMsg( "Load File Error", "Unable to allocate global memory for resource (%s).", name.c_str() );
        return false;
    }

    // See if we can get the global pointer
    if( hGlobal.GetGlobalPtr() == NULL )
    {
        NGenFunc::PostMsg( "Load File Error", "Unable to lock down global memory for resource (%s).", name.c_str() );
        return false;
    }

    // Read contents of the resource into the allocated memory
    // We have to have a handle to an hGlobal to make the stream
    memcpy( hGlobal.GetGlobalPtr(), pDataPtr, dataSizeInbytes );

    // Now try to load the file from the HGLOBAL
    if( !Load( hGlobal.get(), name ) )
        return false;

    return true;

}	// LoadFromResource


/***************************************************************************
*   desc:  Load Picture obj from supplied HGLOBAL
*
*   param:  hGlobal - handel to HGLOBAL
*   param:  errorInfo - infor to indicate what errored
*
*   ret:  bool - true on success or false on fail
 ****************************************************************************/
bool CIPicture::Load( HGLOBAL hGlobal, string & errorInfo )
{
    // create IStream* from global memory
    // Indicating true automatically frees the global data when stream is released
    LPSTREAM plStream;
    HRESULT hResult = CreateStreamOnHGlobal( hGlobal, true, &plStream );
    scoped_comm_object_ptr<IStream> pStream( plStream );

    // Did we create the stream OK?
    if( FAILED( hResult ) || pStream.isNull() )
    {
        NGenFunc::PostMsg( "Load File Error", "Error creating global stream (%s).", errorInfo.c_str() );
        return false;
    }

    // Create IPicture from loaded image data
    LPPICTURE2 pTmpIPict;
    hResult = OleLoadPicture( pStream.get(), 0, false, IID_IPicture2, (LPVOID *)&pTmpIPict );
    pIPict.reset( pTmpIPict );

    // Did we load the picture ok?
    if( FAILED( hResult ) || pIPict.isNull() )
    {
        NGenFunc::PostMsg( "Load File Error", "Error loading stream durring OleLoadPicture (%s).", errorInfo.c_str() );
        return false;
    }

    // Calculate the width and height of image
    CalcImageSize();

    // Make sure the picture has a size
    if( size.w <= 0 || size.h <= 0 )
    {
        NGenFunc::PostMsg( "Load File Error", "Image has no size (%s).", errorInfo.c_str() );
        return false;
    }

    // Create a standard windows handle
    InitHandle();

    return true;

}	// Load


/***************************************************************************
*   desc: Calculate the width and height of image
 ****************************************************************************/
void CIPicture::CalcImageSize()
{
    if( !IsEmpty() )
    {
        long hmWidth;
        long hmHeight;

        // Get the width in himetric. Why, I don't know.
        pIPict->get_Width( &hmWidth );
        pIPict->get_Height( &hmHeight );

        // We need the DC of a window. Any window will do.
        HDC dc = GetDC( GetActiveWindow() );

        // Save the himetric size
        sizeHi.w = hmWidth;
        sizeHi.h = hmHeight;

        // convert himetric to pixels
        size.w = MulDiv( hmWidth, GetDeviceCaps( dc, LOGPIXELSX ), HIMETRICINCH );
        size.h = MulDiv( hmHeight, GetDeviceCaps( dc, LOGPIXELSY ), HIMETRICINCH );

        // Release the DC because we don't need it anymore
        ReleaseDC( GetActiveWindow(), dc );
    }

}   // CalcImageSize


/***************************************************************************
*   desc:  Blit the graphic to the DC
*
*   param:  hDestDC - HDC we are blitting the graphic to a dc
*   param:  destRect - Desination rect. You can stretch blit if
*                      you want by setting larger widths & heights
 ****************************************************************************/
void CIPicture::Render( HDC hDestDC, CRect<int> & destRect )
{
    if( !IsEmpty() )
        pPict->Render( hDestDC, destRect.x1, destRect.y1, destRect.x2, destRect.y2, 0, 
                       sizeHi.h, sizeHi.w, -sizeHi.h, NULL );

}   // Render


/***************************************************************************
*   desc:  Blit the graphic to the DC
*
*   param:  hDestDC - HDC we are blitting the graphic to a dc
*   param:  destRect - Desination rect. You can stretch blit if
*                       you want by setting larger widths & heights
*   param:  metafileRect - Use if metafile was loaded.
 ****************************************************************************/
void CIPicture::Render( HDC hDestDC, CRect<int> & destRect, CRect<int> & metafileRect )
{
    if( !IsEmpty() )
    {
        RECT metRect;
        SetRect(&metRect, metafileRect.x1, metafileRect.y1, metafileRect.x2, metafileRect.y2);

        pPict->Render( hDestDC, destRect.x1, destRect.y1, destRect.x2, destRect.y2, 0, 
                       sizeHi.h, sizeHi.w, -sizeHi.h, &metRect );
    }

}   // Render


/***************************************************************************
*   desc:  Blit the graphic to the DC
*
*   param:  hDestDC - HDC we are blitting the graphic to a dc
*   param:  x/y - Desination point
 ****************************************************************************/
void CIPicture::Render( HDC hDestDC, int x, int y )
{
    if( !IsEmpty() )
        pPict->Render( hDestDC, x, y, size.w, size.h, 0, sizeHi.h, sizeHi.w, -sizeHi.h, NULL );

}   // Render


/***************************************************************************
*   desc:  Get the standard window handles
 ****************************************************************************/
void CIPicture::InitHandle()
{
    if( !pIPict.isNull() )
    {
        // Convert to a normal windows handle
        OLE_HANDLE handle;
        pIPict->get_Handle( &handle );
        *(&hHandle) = (HANDLE)handle;
    }

}   // InitHandle


/***************************************************************************
*   desc:  Fill an array with bitmap bits
*
*   param:  *pData - Pointer to buffer
*   param:  width - Width of image
*   param:  height - Height of imaget
*   param:  bitCount - Bit count of image
*
*   ret:   bool - true on success - false on error
 ****************************************************************************/
bool CIPicture::GetBitmapBits( unsigned char *pData, int width, int height, int bitCount )
{
    BITMAPINFOHEADER bi;
    bool result = true;
    HDC dc;
 
    // Fill out the bitmap info header
    bi.biSize = sizeof(BITMAPINFOHEADER); 
    bi.biWidth = width; 
    bi.biHeight = height; 
    bi.biPlanes = 1; 
    bi.biBitCount = (WORD)bitCount;
    bi.biCompression = BI_RGB; 
    bi.biSizeImage = 0; 
    bi.biXPelsPerMeter = (LONG)0;
    bi.biYPelsPerMeter = (LONG)0; 
    bi.biClrUsed = 0; 
    bi.biClrImportant = 0;
    
    // We need the DC of a window. Any window will do.
    dc = GetDC( GetActiveWindow() ); 
 
    if( GetDIBits( dc, (HBITMAP)GetHandle(), 0, height, pData, (BITMAPINFO *)&bi, DIB_RGB_COLORS ) != height )
        result = false;

    // Release the DC because we don't need it anymore
    ReleaseDC( GetActiveWindow(), dc );

    return result;

}   // GetBitmapBits



