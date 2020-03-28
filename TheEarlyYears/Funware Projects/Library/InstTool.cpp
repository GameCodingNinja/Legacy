
/************************************************************************
*
*    PRODUCT:         Installer Tools
*
*    FILE NAME:       InstallTools.cpp
*
*    DESCRIPTION:     Screen saver base
*
*    IMPLEMENTATION:  Designed to be used as a screen saver base. New
*                     projects would use this like a library and not
*                     alter the code unless adding a new feature that
*                     all the screen savers would use or making fixes.
*
*                     All the tools needed to create a screen saver.
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include "InstTool.h"
#ifdef DEBUG
#include <stdio.h>
#endif


/************************************************************************
*    FUNCTION NAME:         CenterWindow()                                                             
*
*    DESCRIPTION:           Sizing and centering a window. If you send it
*                           a 0 width or 0 height, the width or height will
*                           be the size of the computers resolution.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd     - Handle of the window receiving the message
*              int Width     - Width of the window
*              int Height    - Height of the window     
*
*    Output:   long - Returns the error code if any
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
void CenterWindow( HWND hwnd, int Width, int Height )
{
    int X, Y, ScreenW, ScreenH;

    // Get the size the the display screen
    ScreenW = GetSystemMetrics( SM_CXSCREEN );
    ScreenH = GetSystemMetrics( SM_CYSCREEN );

    if (Width == 0)
    {
        Width = ScreenW;
        X = 0;
    }
    else
    {
        X = (ScreenW - Width) / 2;
    }

    if (Height == 0)
    {
        Height = ScreenH;
        Y = 0;
    }
    else
    {
        Y = (ScreenH - Height) / 2;
    }

    // Put a window at a given position and size
    MoveWindow( hwnd, X, Y, Width, Height, TRUE );

}   // CenterWindow



/************************************************************************
*    FUNCTION NAME:         loadFromResource()                                                             
*
*    DESCRIPTION:           Sizing and centering a window. If you send it
*                           a 0 width or 0 height, the width or height will
*                           be the size of the computers resolution.
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - Instance of this program
*              DWORD &size     - Inits to size of resource
*              char *name      - Name of resource
*              char *type      - Type of resource       
*
*    Output:   HANDLE - Returns handle of resource
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

HANDLE loadFromResource( HINSTANCE hInst, DWORD &size, char *name, char *type )
{
    HANDLE handle = NULL;
    HRSRC hResource = NULL;

    // Find the resource
    hResource = FindResource( hInst, name, type );
    if( hResource == NULL )
        return NULL;

    // Get the size of the resource
    size = SizeofResource( hInst, hResource );

    // Load the resource
    handle = LoadResource( hInst, hResource );
    if( handle == NULL )
        return NULL;

    return handle;
}



/************************************************************************
*    FUNCTION NAME:         ShowMessageBox()                                                             
*
*    DESCRIPTION:           Sets up a message box that displays text from
*                           a resource strings. This can be a standard
*                           message box or one that askes questions and
*                           returns a responce
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - Instance of this program
*              HWND hwnd       - Handle of the window receiving the message 
*              int resMessage  - Resource ID number for message
*              int resTitle    - Resource ID number for title message
*              UINT uType      - Type of message box        
*
*    Output:   int - Returns choices the user made if it was a question box
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int ShowMessageBox( HINSTANCE hInst, HWND hwnd, int resMessage, int resTitle, UINT uType )
{
    char strMessage[1000], strTitle[200];

    LoadString( hInst, resMessage, strMessage, sizeof(strMessage) );
    LoadString( hInst, resTitle, strTitle, sizeof(strTitle) );

    return MessageBox( hwnd, strMessage, strTitle, uType );
}



/************************************************************************
*    FUNCTION NAME:         ApplyWin95Font()                                                             
*
*    DESCRIPTION:           Use the standard Windows 95 thin font
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd     - Handle of the window receiving the message       
*
*    Output:   Nothing
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void ApplyWin95Font( HWND hwnd )
{
    HFONT ControlFont;
    ControlFont = GetStockFont( DEFAULT_GUI_FONT );
    SetWindowFont( hwnd, ControlFont, FALSE );
    //SendMessage( hwnd, WM_SETFONT, WPARAM(ControlFont), 0 );

}   // ApplyWin95Font



/************************************************************************
*    FUNCTION NAME:         sizeButtonWidthHeight()                                                             
*
*    DESCRIPTION:           Calculates button width & hight because of text
*                           size. This can be very handy if you are loading
*                           strings for your buttons from the resource and
*                           these strings can be any size. Also, it is not
*                           nessary to delete stock objects.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd     - Handle of the window receiving the message       
*
*    Output:   Nothing
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void sizeButtonWidthHeight( HWND hwnd, control &T )
{
    SIZE S, *pS;
    HDC DC;
    HFONT ControlFont, OldFont;

    pS = &S;

    // get the device context
    DC = GetDC( hwnd );
    ControlFont = GetStockFont( DEFAULT_GUI_FONT );
    OldFont = SelectFont( DC, ControlFont );
    GetTextExtentPoint32( DC, T.caption, strlen(T.caption), pS );
    SelectFont( DC, OldFont );

    ReleaseDC( hwnd, DC );

    // Add them up
    T.w = S.cx + BUTTON_W; T.h = S.cy + BUTTON_H;

    // See if they are at least the minimum height & width
    if (T.w < DEF_BUTTON_W)
        T.w = DEF_BUTTON_W;

    if (T.h < DEF_BUTTON_H)
        T.h = DEF_BUTTON_H;


} // TextWidthHeight


/************************************************************************
*    FUNCTION NAME:         CheckSumFile                                                             
*
*    DESCRIPTION:           Checksums a file and expects it to be quad aligned
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

DWORD CheckSumFile( char *FileName )
{
    DWORD CheckSum;
    BOOL Result = FALSE;
    SECURITY_ATTRIBUTES FileSecurity;
    HANDLE hFile;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    // Open this file on the hard drive
    hFile = CreateFile( FileName, GENERIC_READ, FILE_SHARE_READ,
                        &FileSecurity, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hFile == INVALID_HANDLE_VALUE )
    {
        MessageBox( GetActiveWindow(), "Can't open file on disk to do checksum", "Error", MB_OK );
        goto HANDLE_ERROR;
    }

    // Checksum the file
    CheckSum = CheckSumFile( hFile );

    Result = TRUE;

HANDLE_ERROR:;

    // Close the file handle
    if( hFile != INVALID_HANDLE_VALUE )
        CloseHandle( hFile );

    if( !Result )
        CheckSum = CHECK_SUM_ERROR;

    return CheckSum;

}   // CheckSumFile


/************************************************************************
*    FUNCTION NAME:         CheckSumFile                                                             
*
*    DESCRIPTION:           Checksums a file and expects it to be quad aligned
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

DWORD CheckSumFile( HANDLE hFile )
{
    BOOL Result = FALSE;
    DWORD CheckSum = 0;
    DWORD FileSize;
    DWORD FileReadSize;
    DWORD DataReadSize = 0;
    DWORD BufferSize;
    DWORD QuadSize;
    BYTE *pBuffer=FALSE;
    DWORD *pQuadBuffer;
    unsigned long bytesRead;
    DWORD i;
    BOOL FileReadWriteResult;

    // Get the file size of the file that is being checksumed
    FileSize = GetFileSize( hFile, NULL );

    // The file has to be quad aligned for this to work so check that it is
    if( FileSize % sizeof( DWORD ) != 0 )
        goto HANDLE_ERROR; 

    BufferSize = 1000000; // 1 meg
    pBuffer = new BYTE[BufferSize];
    if( !pBuffer )
    {
        MessageBox( GetActiveWindow(), "Memory allocation error during checksum.", "Error", MB_OK );
        goto HANDLE_ERROR;
    }

    // Convert the byte pointer to a quad pointer
    pQuadBuffer = (DWORD *)pBuffer;
       
    // Setup the amount of memory will be using to copy over the file
    if( FileSize > BufferSize )
        FileReadSize = BufferSize;
    else
        FileReadSize = FileSize;

    // Set the file pointer to the begining of the file
    SetFilePointer( hFile, 0, 0, FILE_BEGIN );

    // If the file is bigger than the buffer, then keep writting
    // chuncks until the file is finished
    do
    {
        // Read the amount specified into the buffer
        FileReadWriteResult = ReadFile( hFile, pBuffer, FileReadSize, &bytesRead, NULL );
        if( !FileReadWriteResult || bytesRead != FileReadSize )
        {
            MessageBox( GetActiveWindow(), "Error reading file for checksum.", "Error", MB_OK );
            goto HANDLE_ERROR;
        }

        // Add up all the quads in the array
        QuadSize = FileReadSize / sizeof(DWORD);
        for( i = 0; i < QuadSize; ++i )
            CheckSum += pQuadBuffer[i];
            
        // Update the data read size to the amount read
        DataReadSize += FileReadSize;

        // Update how much we need to read in for that last piece of memory
        // that is less then the file read size otherwise keep reading in the
        // same chunk size.
        if( DataReadSize < FileSize && DataReadSize + FileReadSize > FileSize )
            FileReadSize = FileSize - DataReadSize;
    }
    while( DataReadSize < FileSize );

    // We finished so the result it true
    Result = TRUE;

HANDLE_ERROR:;

    // Close the file handle
    if( pBuffer )
        delete [] pBuffer;

    if( !Result )
        CheckSum = CHECK_SUM_ERROR;

    return CheckSum;

}   // CheckSumFile



/************************************************************************
*    FUNCTION NAME:         IsEnoughDiskSpace                                                             
*
*    DESCRIPTION:           Is there enough room on the hard drive for this file?
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  3/06/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL IsEnoughDiskSpace( char *FileName, DWORD FileSize )
{
    DWORD SectorsPerCluster, BytesPerSector, NumberOfFreeClusters;
    DWORD TotalNumberOfClusters, FreeSpace;
    char Drive[MAX_PATH+1];
    BOOL Result = TRUE;

    // Copy over the file name and terminate after the first three char
    strcpy( Drive, FileName );
    Drive[3] = 0;

    // Check to make sure there is enough room on the hard drive for this program
    if( GetDiskFreeSpace( Drive, &SectorsPerCluster, &BytesPerSector, 
                          &NumberOfFreeClusters, &TotalNumberOfClusters ) )
    {
        // Calculate free space
        FreeSpace = BytesPerSector * SectorsPerCluster * NumberOfFreeClusters;

        // If they don't have enough disk space then kill the install. File size + 50k
        if( FreeSpace <= FileSize + (1024 * 50) )
            Result = FALSE;
    }
    else
        Result = FALSE;

    return Result;

}   // IsEnoughDiskSpace



/************************************************************************
*    FUNCTION NAME:         CropOutPath                                                             
*
*    DESCRIPTION:           Strip the path from a file name path string.
*                           Also works if the string is just the path 
*                           without a file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  3/06/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CropOutPath( char *SourceFilePath, char *DestPath )
{
    int i, Length;
    BOOL ExtensionCheck = FALSE;

    strcpy( DestPath, SourceFilePath );

    // Get the string length
    Length = strlen( SourceFilePath );

    for( i = Length; i > 0; --i )
    {
        // Set the flag if we found the dot extension
        if( SourceFilePath[i] == '.' )
            ExtensionCheck = TRUE;

        if( SourceFilePath[i] == '\\' )
            break;
    }

    // Keep the '\\' if this is the root
    if( i == 2 )
        ++i;

    // Ternamate only if there was an extension
    if( ExtensionCheck )
        DestPath[i] = 0;

}   // CropOutPath


/************************************************************************
*    FUNCTION NAME:         ParsePathFromCmdLine                                                             
*
*    DESCRIPTION:           Parses out the *.exe path from the command line.
*
*    FUNCTION PARAMETERS:
*    Input:    char *CommandLine  - The command line
*              char *Buffer       - The buffer to place the result in           
*
*    Output:   DWORD
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void ParsePathFromCmdLine( char *CommandLine, char *Buffer )
{
    int length, i;

    // Jump past any junk in the front of the command line
    while( (*CommandLine == '\"' || *CommandLine == ' ') && *CommandLine != 0 )
            CommandLine++;

    // Copy it to the buffer
    strcpy( Buffer, CommandLine );

    // Get the length if the string
    length = strlen( Buffer );

    // Work your way backwards to terminate the string by finding the "."
    for( i = length-1; i > 0; --i )
    {
        if( Buffer[i] == '.' )
        {
            Buffer[i+4] = 0;
            break;
        }   
    }
}   // ParsePathFromCmdLine



//--------------------------------------------------------------------------
// For debug work
#ifdef DEBUG
void PrintText(HWND hwnd, int X, int Y, char* S)
{
    HDC PaintDC = GetDC(hwnd);
    TextOut(PaintDC, X, Y, S, strlen(S));
    ReleaseDC(hwnd, PaintDC);
} // PrintText


//--------------------------------------------------------------------------

void PrintText(HWND hwnd, int X, int Y, int I)
{
    char S[20];
    HDC PaintDC = GetDC(hwnd);
    itoa(I, S, 10);

    // Could also use wsprintf-see p.129 Win API Super Bible
    TextOut(PaintDC, X, Y, S, strlen(S));
    ReleaseDC(hwnd, PaintDC);
} // PrintText
#endif



/************************************************************************
*                STRIP DOWN VERSION OF THE TDIB Object
*                THIS VERSION IS JUST FOR THE INSTALLER
************************************************************************/



/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TDIB::TDIB()
{
  // Null out all important variables
  hLoad = NULL;
  hBmpFile = NULL;
  hPalette = NULL;
  Fdib = NULL;
}


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TDIB::~TDIB()
{
  Free();
}



/************************************************************************
*    FUNCTION NAME:         getDIBcolors()                                                             
*
*    DESCRIPTION:           Returns the color bit depth
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int TDIB::getDIBcolors()
{
   if (Fdib->bmiHeader.biClrUsed==0)
   {
        switch (Fdib->bmiHeader.biBitCount)
      {
         case 1: {return 2;}
         case 4: {return 16;}
         case 8: {return 256;}
         case 24: {return 0;} // no color table
      }
   }
   return (int)Fdib->bmiHeader.biClrUsed;
}




/************************************************************************
*    FUNCTION NAME:         loadFromResource()                                                             
*
*    DESCRIPTION:           Load bitmap from resource file.
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst   - Program Instance
*              char *name        - Name of the resource
*              char *type        - Resource Type Id
*              int PointerType   - Flag to hold resource pointer or
*                                  allocate memory for the bitmap
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TDIB::loadFromResource(HINSTANCE hInst, char *name, char *type, int PointerType )
{
   HANDLE handle;
   HRSRC hResource;
   BYTE *rData, *bitData;
   DWORD size;

   Free(); // Free memory if need be

   // Find the resource
   hResource = FindResource(hInst, name, type);
   if( hResource == NULL )
      return FALSE;

   // Get the size of the resource
   size = SizeofResource(hInst, hResource);
   if( size == 0 )
      return FALSE;

   // Load the resource
   handle = LoadResource(hInst, hResource);
   if (handle == (HANDLE)NULL)
      return FALSE;

   // Get a pointer to the memory block
   rData = (BYTE *)LockResource(handle);
   if (rData == (BYTE *)NULL)
      return FALSE;

   // Check to see if we have the file header
   // True BMP resource files loose their headers when
   // compiled into a resource
   if( type != RT_BITMAP )
   {
      // Set pointer past file header
      rData += sizeof(BITMAPFILEHEADER);
      size -= sizeof(BITMAPFILEHEADER);
   }

   // We can choose to alloc our own memory to store the
   // bitmap or use the pointer Windows has allocated for
   // us. The only difference is that we can't write to
   // the Windows resource pointer. If your not planning
   // on changing the bitmap during program runtime then there
   // is not much need to allocate the memory.
   if( PointerType == ALLOC_MEM_FOR_BITMAP )
   {
      // Allocate memory for our DIB
      hLoad = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE, size);
      if (hLoad == (HANDLE)NULL)
         return FALSE;

      // Get a pointer to the memory block
      bitData = (BYTE *)GlobalLock(hLoad);
      if (bitData == (BYTE *)NULL)
         return FALSE;

      // Copy contents of resource into allocated memory
      CopyMemory( bitData, rData, size );
   }
   else if( PointerType == USE_RESOURCE_POINTER )
   {
        bitData = rData;
   }


   Fdib = (BITMAPINFO *)bitData;  // Copy pointer

   return TRUE;
} // loadFromResource


/************************************************************************
*    FUNCTION NAME:         deletePalete()                                                             
*
*    DESCRIPTION:           Deletes the palette
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TDIB::deletePalete()
{
    if( hPalette != NULL )
   {
      DeleteObject( hPalette );
      hPalette = NULL;
   }
}



/************************************************************************
*    FUNCTION NAME:         Palette()                                                             
*
*    DESCRIPTION:           Creates the palette
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

HPALETTE TDIB::GetPalette()
{
  // Check to see if the palette is NULL
  // before doing it all over again
  if( hPalette != NULL )
    return hPalette;

   int i, colors;
   LPLOGPALETTE pal;
   HANDLE handle;

   colors = getDIBcolors();

    if( colors > 0 )
   {
      handle = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE,
                (sizeof(LOGPALETTE) + colors) * sizeof(PALETTEENTRY));

      if (handle != (HANDLE)NULL)
      {
        pal = (LPLOGPALETTE)GlobalLock(handle); // do we have the memory

        if (pal != (LPLOGPALETTE)NULL)
        {
            // Set version number and color count
            pal->palVersion = 0x0300;
            pal->palNumEntries = (WORD)colors;

            // Set values into palette entries
            for( i = 0; i < colors; ++i )
            {
                pal->palPalEntry[i].peRed = Fdib->bmiColors[i].rgbRed;
                pal->palPalEntry[i].peGreen = Fdib->bmiColors[i].rgbGreen;
                pal->palPalEntry[i].peBlue = Fdib->bmiColors[i].rgbBlue;
                pal->palPalEntry[i].peFlags = 0;
            }

            // Create the palette
            hPalette = CreatePalette(pal);

            GlobalUnlock(handle);
        }
         GlobalFree(handle);
      }
   }
   return hPalette;
}



/************************************************************************
*    FUNCTION NAME:         Free                                                             
*
*    DESCRIPTION:           Free allocated memory and palette
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TDIB::Free()
{
   if( hLoad )
   {
     GlobalUnlock(hLoad);
     GlobalFree(hLoad);
     hLoad = NULL;
     Fdib = NULL;
   }

   if( hPalette )
   { 
     DeleteObject(hPalette);
     hPalette = NULL;
   }

}



/************************************************************************
*    FUNCTION NAME:         IsEmpty()                                                             
*
*    DESCRIPTION:           Do we have a graphic sitting in here?
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TDIB::IsEmpty()
{
  if( Fdib )
    return FALSE;
  else
    return TRUE;
}