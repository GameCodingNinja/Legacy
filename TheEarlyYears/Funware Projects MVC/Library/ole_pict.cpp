
/************************************************************************
*
*    FILE NAME:       OLE_Pict.cpp
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

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include "OLE_Pict.h"        // Header file for this *.cpp file.


/************************************************************************
*    DESCRIPTION:  Constructer                                                             
*
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
COLE_Pict::COLE_Pict()
{
    // init class members
    pPict = NULL;
    hHandle = NULL;
    hDC = NULL;
    dataSizeInbytes = 0;
    width = 0;
    height = 0;
    widthHi = 0;
    heightHi = 0;

}   // constructor


/************************************************************************
*    DESCRIPTION:  Free allocated memory and palette
************************************************************************/
void COLE_Pict::Free()
{
    FreePicture();

}  // Free


/************************************************************************
*    DESCRIPTION:  Free the picture
************************************************************************/
void COLE_Pict::FreePicture()
{
    if( pPict != NULL )
    {
        pPict->Release();
        pPict = NULL;
        hHandle = NULL;
        hDC = NULL;
        dataSizeInbytes = 0;
		width = 0;
		height = 0;
		widthHi = 0;
		heightHi = 0;
    }

}  // FreePicture


/************************************************************************
*    DESCRIPTION:  Load Picture obj from file
*
*    Input:    char *FileName    - Path to file name
*              The last two parameters are needed for TBitmap and are not used here
*              BOOL BitmapType   - Determines if it is a standard BMP or 
*                                  a DIB Section
*              int BitCount - Sets the type of bitmap you plan to use.
*                             Default to 8 bit because it's faster than 24 bit
*
*    Output:   bool - true on success - false on error
************************************************************************/
bool COLE_Pict::LoadFromFile( char *fileName )
{ 
    SECURITY_ATTRIBUTES fileSecurity;
    unsigned long bytesRead;
    HANDLE hFile;
    BOOL readResult;
    bool result = false;
    HRESULT hResult;
    LPSTREAM pStream;
    HGLOBAL hGlobal;
    LPVOID pGlobalData;

    fileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    fileSecurity.lpSecurityDescriptor=NULL;
    fileSecurity.bInheritHandle=FALSE;

    Free(); // Free memory if need be

    // Open the file on the hard drive
    hFile = CreateFile( fileName, GENERIC_READ, FILE_SHARE_READ,
                        &fileSecurity, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hFile == INVALID_HANDLE_VALUE )
    {
        goto HANDLE_ERROR;
    }

    // Get the file size of the picture so that we can allocate enough memory for it
    dataSizeInbytes = GetFileSize( hFile, NULL );
    if( dataSizeInbytes == 0 || dataSizeInbytes == 0xFFFFFFFF )
    {
        goto HANDLE_ERROR;
    }

    // alloc memory based on file size
    hGlobal = GlobalAlloc( GMEM_MOVEABLE, dataSizeInbytes );
    if( hGlobal == NULL )
    {
        goto HANDLE_ERROR;
    }

    // Lock down global memory to get the pointer
    pGlobalData = GlobalLock( hGlobal );
    if( pGlobalData == NULL )
    {
        goto HANDLE_ERROR;
    }

    // Read contents of file into allocated memory
    readResult = ReadFile( hFile, pGlobalData, dataSizeInbytes, &bytesRead, NULL );

    // Make sure the file read in correctly
    if( readResult == false || bytesRead != dataSizeInbytes )
    {
        goto HANDLE_ERROR;
    }

    // Unlock the memory handle
    GlobalUnlock( hGlobal );

    // create IStream* from global memory
    // Indicating true automatically frees the global data when
    // stream is released
    hResult = CreateStreamOnHGlobal( hGlobal, true, &pStream );

    // Did we create the stream OK?
    if( SUCCEEDED( hResult ) == false || pStream == NULL )
    {
        goto HANDLE_ERROR;
    }

    // Create IPicture from loaded image file
    hResult = ::OleLoadPicture( pStream, dataSizeInbytes, false, IID_IPicture, (LPVOID *)&pPict );

    // Did we load the picture ok?
    if( SUCCEEDED( hResult ) == false || pPict == NULL )
    {
        goto HANDLE_ERROR;
    }

    // Set the width and height properties
    width = GetWidth();
    height = GetHeight();
    widthHi = WidthInHimetric();
    heightHi = HeightInHimetric();

    // Make sure the picture has a size
    if( width <= 0 || height <= 0 )
    {
        goto HANDLE_ERROR;
    }

    // If we made it this, far the load was a success
    result = true;

HANDLE_ERROR:;

    // No longer needed
    if( hFile != NULL )
    {
        CloseHandle( hFile );
    }

    // No longer needed
    // hGlobal data also released by this call
    if( pStream != NULL )
    {
        pStream->Release();
    }

    // Was there a load error?
    if( result == false )
    {
        // Release everything if there was an error
        Free();
    }

    return result;

} // LoadFromFile


/************************************************************************
*    DESCRIPTION:  Load Picture obj from data pointer. Unlike
*                  LoadFromFile where we keep a copy of the
*                  loaded memory, since we are already supplying
*                  a pointer of memory, there's no need to
*                  keep another block of memory hanging around.
*
*    Input:    LPVOID pDataPtr   - pointer to data buffer
*              unsigned int size - Side in bytes of data buffer
*
*    Output:   bool - true on success - false on error
************************************************************************/
bool COLE_Pict::LoadFromPointer( LPVOID pDataPtr, unsigned int size )
{
    bool result = false;
    HRESULT hResult;
    LPSTREAM pStream;
    HGLOBAL hGlobal;
    LPVOID pGlobalData;

    Free(); // Free memory if need be

    // alloc memory based on file size
    // We are doing all this so we can send the COM interface
    // a stream it requires for decoding the file.
    hGlobal = GlobalAlloc( GMEM_MOVEABLE, size );

    if( hGlobal == NULL )
    {
        goto HANDLE_ERROR;
    }

    // Lock down global memory to get the pointer
    pGlobalData = GlobalLock( hGlobal );

    if( pGlobalData == NULL )
    {
        goto HANDLE_ERROR;
    }

    // Read contents of the resource into the allocated memory
    // We have to have a handle to an hGlobal to make the stream
    memcpy( pGlobalData, pDataPtr, size );

    // Unlock the memory handle
    GlobalUnlock( hGlobal );

    // create IStream* from global memory
    // Indicating true automatically frees the global data when
    // stream is released
    hResult = CreateStreamOnHGlobal( hGlobal, true, &pStream );

    // Did we creat the stream OK?
    if( SUCCEEDED( hResult ) == false || pStream == NULL )
    {
        goto HANDLE_ERROR;
    }

    // Create IPicture from image file
    hResult = ::OleLoadPicture( pStream, size, false, IID_IPicture, (LPVOID *)&pPict );

    // Did we load the picture ok?
    if( SUCCEEDED( hResult ) == false || pPict == NULL )
    {
        goto HANDLE_ERROR;
    }

    // Set the width and height properties
    width = GetWidth();
    height = GetHeight();
    widthHi = WidthInHimetric();
    heightHi = HeightInHimetric();

    // No longer needed
    // hGlobal data also released by this call
    if( width <= 0 || height <= 0 )
    {
        goto HANDLE_ERROR;
    }

    // If we made it this far the load was a success
    result = true;

HANDLE_ERROR:;

   // No longer needed
   if( pStream != NULL )
   {
       pStream->Release();
   }

    return result;

} // LoadFromPointer


/************************************************************************
*    DESCRIPTION:  Do we have a graphic sitting in here?
*
*    Output:   bool - true if empty. No image loaded.
************************************************************************/
bool COLE_Pict::IsEmpty()
{
    bool result = true;

    if( pPict != NULL )
    {
        result = false;
    }

    return result;

}   // IsEmpty


/************************************************************************
*    DESCRIPTION:  Get the width of the image in pixels
*
*    Output:   int - width of image
************************************************************************/
int COLE_Pict::GetWidth()
{
    long hmWidth;
    int result;

    if( !IsEmpty() )
    {
        // Get the width in himetric. Why, I don't know.
        pPict->get_Width( &hmWidth );

        // We need the DC of a window. Any window will do.
        HDC dc = GetDC( GetActiveWindow() );

        // convert himetric to pixels
        result = MulDiv( hmWidth, GetDeviceCaps( dc, LOGPIXELSX ), HIMETRICINCH );

        // Release the DC because we don't need it anymore
        ReleaseDC( GetActiveWindow(), dc );
    }
    else
        result = 0;

    return result;

}   // GetWidth


/************************************************************************
*    DESCRIPTION:  Get the height of the image in pixels
*
*    Output:   int - height of image
************************************************************************/

int COLE_Pict::GetHeight()
{
    long hmHeight;
    int result;

    if( !IsEmpty() )
    {
        // Get the width in himetric. Why, I don't know.
        pPict->get_Height( &hmHeight );

        // We need the DC of a window. Any window will do.
        HDC dc = GetDC( GetActiveWindow() );

        // convert himetric to pixels
        result = MulDiv( hmHeight, GetDeviceCaps( dc, LOGPIXELSY ), HIMETRICINCH );

        // Release the DC because we don't need it anymore
        ReleaseDC( GetActiveWindow(), dc );
    }
    else
        result = 0;

    return result;

}   // GetHeight


/************************************************************************
*    DESCRIPTION:  Blit the graphic to the DC
*
*    Input:    HDC hDestDC - HDC we are blitting the graphic to a dc
*              RECT *destRect - Desination rect. You can stretch blit if
*                               you want by setting larger widths & heights
*              RECT *metafileRect - Use if metafile was loaded.
************************************************************************/
void COLE_Pict::Render( HDC hDestDC, RECT *destRect, RECT *metafileRect )
{
    // display picture using IPicture::Render
    if( !IsEmpty() )
    {
        pPict->Render( hDestDC, destRect->left, destRect->top, destRect->right, destRect->bottom, 0, 
                       heightHi, widthHi, -heightHi, metafileRect );
    }
}   // Render


/************************************************************************
*    DESCRIPTION:  Blit the graphic to the DC
*
*    Input:    HDC hDestDC - HDC we are blitting the graphic to a dc
*              RECT *destRect - Desination rect. You can stretch blit if
*                               you want by setting larger widths & heights
*              RECT *metafileRect - Use if metafile was loaded.
************************************************************************/
void COLE_Pict::Render( HDC hDestDC, int x, int y )
{
    // display picture using IPicture::Render
    if( !IsEmpty() )
    {
        pPict->Render( hDestDC, x, y, width, height, 0, heightHi, widthHi, -heightHi, NULL );
    }
}   // Render


/************************************************************************
*    DESCRIPTION:  Get the width of the image in Himetric
*
*    Output:   returntype - Description
************************************************************************/
int COLE_Pict::WidthInHimetric()
{
    long result(0);

    if( !IsEmpty() )
    {
        // Get the width in himetric. Why, I don't know.
        pPict->get_Width( &result );
    }

    return result;

}   // WidthInHimetric


/************************************************************************
*    DESCRIPTION:  Get the height of the image in Himetric
*
*    Output:   returntype - Description
************************************************************************/
int COLE_Pict::HeightInHimetric()
{
    long result(0);

    if( !IsEmpty() )
    {
        // Get the width in himetric. Why, I don't know.
        pPict->get_Height( &result );
    }

    return result;

}   // HeightInHimetric


/************************************************************************
*    DESCRIPTION:  Get the handle of the picture. If it's a bitmap
*                  then cast HANDLE HBITMAP.
*
*    Output:   HANDLE - Returns the OLE Picture handle
************************************************************************/
HANDLE COLE_Pict::GetHandle()
{
    OLE_HANDLE handle;

    if( !IsEmpty() && hHandle == NULL )
    {
        pPict->get_Handle( &handle );
        *(&hHandle) = (HANDLE)handle;
    }

    return hHandle;

}   // GetHandle


/************************************************************************
*    DESCRIPTION:  Get the DC of the picture. This only relates
*                  to bitmaps.
*
*    Output:   HDC - Returns the handle to the HDC
************************************************************************/
HDC COLE_Pict::GetHDC()
{
    if( !IsEmpty() && hDC == NULL )
    {
        pPict->get_CurDC( &hDC );
    }

    return hDC;

}   // GetHandle


/************************************************************************
*    DESCRIPTION:  Fill an array with bitmap bits
*
*    Input:    unsigned char *pData - Pointer to buffer
*              int width - Width of image
*              int height - Height of image
*              int bitCount - Bit count of image
*
*    Output:   bool - true on success - false on error
************************************************************************/
bool COLE_Pict::GetBitmapBits( unsigned char *pData, int width, int height, int bitCount )
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
    {
        result = false;
    }

    // Release the DC because we don't need it anymore
    ReleaseDC( GetActiveWindow(), dc );

    return result;

}   // GetBitmapBits