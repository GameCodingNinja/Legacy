
/************************************************************************
*
*    PRODUCT:         Metafile Manipulation
*
*    FILE NAME:       metafile.h
*
*    DESCRIPTION:     Metafile Object for Window Metafiles
*
*    IMPLEMENTATION:  Load and palys metafiles
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __METAFILE_H__
#define __METAFILE_H__


#define ALDUS_KEY  0x9AC6CDD7L


struct THRect
{
    short Top;
    short Left;
    short Bottom;
    short Right;
};

// This is the file header if this is an Aldus
// metafile. I don't use it but it's here just in
// case someone does.
// Under the 5.5 command line tools, make sure the #pragma pack(push, 1)
// is defined so that structures are not padded.
struct TAldusFileHeader
{
    DWORD key;    // 4
    WORD  hmf;        // 2
    THRect  bbox;     // 8
    WORD  inch;       // 2
    DWORD reserved;   // 4
    WORD  checksum;   // 2
    //char dummy[22];
};
//typedef TAldusFileHeader *PTAldusFileHeader;


class TMetafile
{
protected:
    // Free memory
    void Free();
    // Check the data to see what type of metafile it is and create the metafile
    BOOL ProcessMetafileData( BYTE *pMetaData, DWORD size, BOOL convertToEnhance );

    // Handle to the enhanced metafile
    HENHMETAFILE hEnMetaFile;
    // Handle to the windows metafile
    HMETAFILE hMetaFile;
    // File header Can't seem to get the proper width and height
    ENHMETAHEADER enMetafileHeader;
    METAHEADER metafileHeader;
    // Pass a DC to use for creating the metafile
    HDC hDeviceDC;
    // Mapping mode
    int mappingMode;

public:
    // Constructor
    TMetafile();
    // Destructor
    ~TMetafile(){ Free(); };
    // Load metafile from file
    BOOL LoadFromFile( char *FileName, BOOL convertToEnhance = TRUE );
    // Load metafile from resource
    BOOL LoadFromResource( HINSTANCE hInst, char *name, char *type, BOOL convertToEnhance = TRUE );
    // Handle to metafile
    HENHMETAFILE Handle(){ return hEnMetaFile; };
    // Is a metafile loaded
    BOOL IsEmpty();
    // Draw the metafile
    BOOL Draw( HDC hDC, RECT *rect );
    BOOL Draw( HWND hDest, RECT *rect );
    // Use a DC from the outside for all the graphic initilizations
    void InitActiveDC( HDC DC ){ hDeviceDC = DC; };
    // Set the Mapping Mode
    void SetMapMode( int Value ){ mappingMode = Value; };
    // Width and height
    int width(){ return enMetafileHeader.rclBounds.left+enMetafileHeader.rclBounds.right; };
    int height(){ return enMetafileHeader.rclBounds.top+enMetafileHeader.rclBounds.bottom; };
};

typedef TMetafile *PTMetafile;


#endif /* __METAFILE_H__ */
