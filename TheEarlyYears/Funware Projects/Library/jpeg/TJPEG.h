
/************************************************************************
*
*    PRODUCT:         JPEG Coder/Decoder
*
*    FILE NAME:       JPEG.h
*
*    DESCRIPTION:     JPEG Object for bitmaps
*
*    IMPLEMENTATION:  Codes and decodes JPEG
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __JPEG_H__
#define __JPEG_H__


/************************************************************************
*                         TJPEG Object
************************************************************************/

class TJPEG
{
protected:
    BYTE * WriteToFileAndLoad( BYTE *pData, DWORD Size );
    int JPGDataSize;

public:
    // Constructor
    TJPEG();
    // Destructor
    ~TJPEG();
    // Load jpeg from file
    BYTE * LoadFromFile( char *filename );
    // Load jpeg from buffer
    BYTE * LoadFromBuffer( BYTE *pData, DWORD Size );
    // Save the jpeg data to a temporary file
    BYTE * LoadFromResource( char *name, char *type, HINSTANCE hInst );
    int GetJPGDataSize(){ return JPGDataSize; };
};

typedef TJPEG *PTJPEG;


struct TRGB
{
    BYTE Red;
    BYTE Green;
    BYTE Blue;
};

struct TRGBarray
{
    TRGB color[];   
};
typedef TRGBarray *PTRGBarray;

/*
struct TCYMK
{
    BYTE Cyan;
    BYTE Yellow;
    BYTE Magenta;
    BYTE Black;
};

struct TCYMKarray
{
    TCYMK color[];  
};
typedef TCYMKarray *PTCYMKarray;
*/

struct TRGBquadArray
{
    RGBQUAD color[];
};
typedef TRGBquadArray *PTRGBquadArray;

#endif  /* __JPEG_H__ */
