
/************************************************************************
*
*    PRODUCT:         Tools
*
*    FILE NAME:       gTools.cpp
*
*    DESCRIPTION:     General Tools
*
*    IMPLEMENTATION:  This module holds misc funstions for working with
*                     windows programs in general.
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size
#include "gTools.h"          // Header file for this *.cpp file.

#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf

// This quiets the compiler warnings about a bunch of functions that Microsoft has
// decided to deprecate, even though the C++ standardization committee has not done so.
#pragma warning(disable : 4996)

/************************************************************************
*    FUNCTION NAME:         AskQuestion                                                             
*
*    DESCRIPTION:           Translates into a windows messagebox with
*                           sprintf capabilities. Makes debugging easier.
*
*    FUNCTION PARAMETERS:
*    Input:    char *Title - Title of the windows message box
*              char *fmt   - Used with vprintf to get the argument list
*                            for wvsprintf.
*
*    Output:   returntype - None
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int AskQuestion( UINT QuectionType, char *Title, char *fmt, ... )
{
    va_list argptr;
    char StrBuffer[1024];

    va_start( argptr, fmt );
    vprintf( fmt, argptr );
    wvsprintf( StrBuffer, fmt, argptr );
    va_end( argptr );
    return MessageBox( GetActiveWindow(), StrBuffer, Title, QuectionType );

} // AskQuestion


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
    HFONT ControlFont = GetStockFont( DEFAULT_GUI_FONT );
    SetWindowFont( hwnd, ControlFont, FALSE );

}   // ApplyWin95Font


/************************************************************************
*    FUNCTION NAME:         FileExist                                                             
*
*    DESCRIPTION:           A simplified use of SearchPath function
*
*    FUNCTION PARAMETERS:
*    Input:    char *SearchFilePath - A file and path is stripped out to
*                                     do the search.
*              char *SearchPathDir  - A second file path to use as an
*                                     optional directory to do a search.
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

bool FileExist( char *SearchFilePath, char *SearchPathDir )
{
    char SearchDir[MAX_PATH+1];
    char SearchFile[MAX_PATH+1];

    // Check to see if we have a valid path. If we have an empty string,
    // return true because there is nothing to find.
    if( !SearchFilePath[0] )
        return true;

    // Strip out the path to do a search
    // If this param is being used, strip out the directory from this one
    if( SearchPathDir )
        CropOutPath( SearchPathDir, SearchDir );
    else
        CropOutPath( SearchFilePath, SearchDir );

    // Strip out the file name to do a search
    // This also works if the file name is not part of a path
    CropFileNameFromPath( SearchFilePath, SearchFile );

    if( SearchPath( SearchDir, SearchFile, NULL, NULL, NULL, NULL ) > 0 )
		return true;
	else
		return false;

}   // FileExist


/************************************************************************
*    FUNCTION NAME:         DirectoryExist                                                             
*
*    DESCRIPTION:           See if a directory is there
*
*    FUNCTION PARAMETERS:
*    Input:    char *SearchFilePath - A directory path.
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

bool DirectoryExist( char *SearchPathDir )
{
    bool Result = FALSE;
    WIN32_FIND_DATA FindData;

    // This windows function can be used to find directorys
    if( FindFirstFile( SearchPathDir, &FindData ) != INVALID_HANDLE_VALUE )
    {
        // See if the directory attribute is set
        if( (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            Result = TRUE;
    }

    return Result;

}   // DirectoryExist


/************************************************************************
*    FUNCTION NAME:         AddFileToOtherPath                                                             
*
*    DESCRIPTION:           Takes a file path combo from one string and
*                           a file path combo from another string, pulls
*                           out the file name from one string and the path
*                           from the other string and combines the two.
*
*    FUNCTION PARAMETERS:
*    Input:    char *UseFileFromPath - Strip the file from this path
*              char *UseThisPath     - Strip the path from this file
*               char *Dest           - Add the two here.
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

void AddFileToOtherPath( char *UseFileFromPath, char *UseThisPath, char *Dest )
{
    char File[MAX_PATH+1];
    char NewPath[MAX_PATH+1];

    // Crop out the path from one string and add in the file from the other
    CropOutPath( UseThisPath, NewPath );
    CropFileNameFromPath( UseFileFromPath, File );

    // Copy over the path
    strcpy( Dest, NewPath );

    // If we don't have any \\ then add them
    if( NewPath[ strlen(NewPath)-1 ] != '\\' )
        strcat( Dest, "\\" );

    // Copy over the file name
    strcat( Dest, File );
        
}   // AddFileToOtherPath


/************************************************************************
*    FUNCTION NAME:         CopyFileToDest                                                             
*
*    DESCRIPTION:           Copy a file to an output file handle. This
*                           function handles it in chuncks just incase
*                           it is a big file
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileToCopy - Path to file to copy
*              HANDLE hDest     - File handle to recieve data
*              unsigned char pBuffer     - Pointer to data buffer
*              unsigned int BufferSize = Size of data buffer
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

bool CopyFileToDest( char *FileToCopy, HANDLE hDest, unsigned char *pBuffer, unsigned int BufferSize, int Action, unsigned char *Key )
{
    unsigned long bytesReadWrite;
    SECURITY_ATTRIBUTES FileSecurity;
    HANDLE hSourceFile;
    bool Result = FALSE;
    unsigned int FileSize;
    unsigned int FileCopySize;
    unsigned int DataWrittenSize = 0;
    BOOL FileReadWriteResult;
    unsigned int i, j=0;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    // Open the file on the hard drive
    hSourceFile = CreateFile( FileToCopy, GENERIC_READ, FILE_SHARE_READ,
                               &FileSecurity, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hSourceFile == INVALID_HANDLE_VALUE )
    {
        PostMsg( "Error", "Can't open project file. (%s)", FileToCopy );
        goto FREE_MEMORY;
    }

    // Get the file size of the file that is being added to the project
    FileSize = GetFileSize( hSourceFile, NULL );

    if( !FileSize )
    {
        PostMsg( "Error", "File has no size. (%s)", FileToCopy );
        goto FREE_MEMORY;
    }       

    // Setup the amount of memory will be using to copy over the file
    if( FileSize > BufferSize )
        FileCopySize = BufferSize;
    else
        FileCopySize = FileSize;

    // If the file is bigger than the buffer, then keep writting
    // chunks until the file is finished
    do
    {
        // Write the TPage structure to the file. We are not writing the file path to the file
        FileReadWriteResult = ReadFile( hSourceFile, pBuffer, FileCopySize, &bytesReadWrite, NULL );
        if( !FileReadWriteResult || bytesReadWrite != FileCopySize )
        {
            PostMsg( "Error", "Error reading file project data. (%s)", FileToCopy );
            goto FREE_MEMORY;
        }

        // Encrypt the data if need be
        if( Action == ENCRYPT_DATA )
        {
            for( i = 0; i < FileCopySize; ++i )
            {
                if( !Key[j] )
                    j = 0;

                pBuffer[i] -= Key[j];
                ++j;
            }
        }

        // Write the TPage structure to the file. We are not writing the file path to the file
        FileReadWriteResult = WriteFile( hDest, pBuffer, FileCopySize, &bytesReadWrite, NULL );
        if( !FileReadWriteResult || bytesReadWrite != FileCopySize )
        {
            PostMsg( "Error", "Error building file project data. (%s)", FileToCopy );
            goto FREE_MEMORY;
        }

        // Update the data writen size to the amount written
        DataWrittenSize += FileCopySize;

        if( DataWrittenSize < FileSize && DataWrittenSize + FileCopySize > FileSize )
            FileCopySize = FileSize - DataWrittenSize;
    }
    while( DataWrittenSize < FileSize );


    Result = TRUE;

FREE_MEMORY:;

    // Close the file handle
    if( hSourceFile )
        CloseHandle( hSourceFile );

    return Result;

}   // CopyFileToDest


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

unsigned int CheckSumFile( char *FileName )
{
    unsigned int CheckSum;
    bool Result = FALSE;
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
        PostMsg( "Error", "Can't open file (%s) on disk to do checksum.", FileName );
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

unsigned int CheckSumFile( HANDLE hFile )
{
    bool Result = FALSE;
    unsigned int CheckSum = 0;
    unsigned int FileSize;
    unsigned int FileReadSize;
    unsigned int DataReadSize = 0;
    unsigned int BufferSize;
    unsigned int QuadSize;
    unsigned char *pBuffer=FALSE;
    unsigned int *pQuadBuffer;
    unsigned long bytesRead;
    unsigned int i;
    BOOL FileReadWriteResult;

    // Get the file size of the file that is being checksumed
    FileSize = GetFileSize( hFile, NULL );

    // The file has to be quad aligned for this to work so check that it is
    if( FileSize % sizeof( unsigned int ) != 0 )
        goto FREE_MEMORY; 

    BufferSize = 1000000; // 1 meg
    pBuffer = new unsigned char[BufferSize];
    if( !pBuffer )
    {
        PostMsg( "Error", "Memory allocation error during checksum." );
        goto FREE_MEMORY;
    }

    // Convert the byte pointer to a quad pointer
    pQuadBuffer = (unsigned int *)pBuffer;
       
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
            PostMsg( "Error", "Error reading file for checksum." );
            goto FREE_MEMORY;
        }

        // Add up all the quads in the array
        QuadSize = FileReadSize / sizeof(unsigned int);
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

FREE_MEMORY:;

    // Close the file handle
    if( pBuffer )
        delete [] pBuffer;

    if( !Result )
        CheckSum = CHECK_SUM_ERROR;

    return CheckSum;

}   // CheckSumFile


/************************************************************************
*    FUNCTION NAME:         LockItUp                                                             
*
*    DESCRIPTION:           Encrypt & unecrypt the text block by adding the key.
*
*    FUNCTION PARAMETERS:
*    Input:    unsigned char *Key - NULL terminated byte array
*              unsigned char *pData - Data to encrypt
*              int DataSize - Data size
*              bool Action - Encypt or unencrypt
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void LockItUp( unsigned char *Key, unsigned char *pData, int DataSize, int Action )
{
    int i, j=0;

    if( Action == ENCRYPT_DATA )
    {
        for( i = 0; i < DataSize; ++i )
        {
            if( !Key[j] )
                j = 0;

            pData[i] -= Key[j];
            ++j;        
        }
    }
    else if( Action == UNENCRYPT_DATA )
    {
        for( i = 0; i < DataSize; ++i )
        {
            if( !Key[j] )
                j = 0;

            pData[i] += Key[j];
            ++j;        
        }
    }

}   // LockItUp


/************************************************************************
*    FUNCTION NAME:         GetFileName                                                             
*
*    DESCRIPTION:           General purpose get file name box
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool GetFileName( HWND hwnd, HINSTANCE hInst, char *FileName, char *Title, char *DefPath, 
                  char *FileMask, char *DefExtension, int DialogType )
{
    OPENFILENAME op;

    bool Result = FALSE;

    // Be sure to memset all windows structures because they
    // have the ability to grow for one version to the next.
    memset( &op, 0, sizeof(OPENFILENAME) );

    // Must terminate or it wont work
    op.lStructSize = sizeof(OPENFILENAME); 
    op.hwndOwner = hwnd; 
    op.hInstance = hInst; 
    op.lpstrFilter = FileMask; 
    op.nFilterIndex = 1; 
    op.lpstrFile = FileName;
    op.nMaxFile = sizeof(char)*(MAX_PATH+1);  
    op.lpstrInitialDir = DefPath;
    op.lpstrDefExt = DefExtension;
    op.lpstrTitle = Title;
    op.Flags = OFN_HIDEREADONLY|OFN_NONETWORKBUTTON|OFN_PATHMUSTEXIST|OFN_EXPLORER|OFN_OVERWRITEPROMPT; 

    if( DialogType == OPEN_FILENAME_DIALOG_BOX )
    {
        if( GetOpenFileName( &op ) )
            Result = TRUE;
    }
    if( DialogType == SAVE_FILENAME_DIALOG_BOX )
    {
        if( GetSaveFileName( &op ) )
            Result = TRUE;
    }

    return Result;

}   // GetFileName


/************************************************************************
*    FUNCTION NAME:         IsMenuChecked                                                             
*
*    DESCRIPTION:           Check to see if the menu is checked
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              int MenuID      - Menu Id 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool IsMenuChecked( HWND hwnd, int MenuID )
{
    HMENU hMenu;
    UINT uState;

    hMenu = GetMenu( hwnd );
    uState = GetMenuState( hMenu, MenuID, MF_BYCOMMAND );

    if( (uState & MFS_CHECKED) == 0 )
		return false;
	else
		return true;

}   // IsMenuChecked


/************************************************************************
*    FUNCTION NAME:         CenterRectInRect                                                             
*
*    DESCRIPTION:           Center one rect inside of another
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

void CenterRectInRect( LPRECT DestRect, int SourceW, int SourceH, int DestW, int DestH )
{
    SetRect( DestRect, (DestW - SourceW) / 2,
                  (DestH - SourceH) / 2,
                  ((DestW - SourceW) / 2) + SourceW,
                  ((DestH - SourceH) / 2) + SourceH );

}   // CenterRectInRect


/************************************************************************
*    FUNCTION NAME:         Edit_DisplayValue                                                             
*
*    DESCRIPTION:           Display an int in an edit control.
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

void Edit_DisplayValue( HWND hCtrl, int Value )
{
    char StrTmp[20];

    // Convert the int to a string
    sprintf( StrTmp, "%d", Value );

    // Have the control display the amount
    Edit_SetText( hCtrl, StrTmp );

}   // Edit_DisplayValue


/************************************************************************
*    FUNCTION NAME:         GetSystemBitDepth                                                             
*
*    DESCRIPTION:           Get the bit depth of the system from a windows handle
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

int GetSystemBitDepth( HWND hwnd )
{
    int SystemBitDepth;
    HDC hDC;

    // Get the screen DC
    hDC = GetDC( hwnd );

    // Get the bit depth of the system, be it 8, 16, 24, 32
    SystemBitDepth = GetDeviceCaps( hDC, BITSPIXEL );

    // Now release the DC
    ReleaseDC( hwnd, hDC );

    return SystemBitDepth;

}   // GetSystemBitDepth


/************************************************************************
*    FUNCTION NAME:         Edit_DisplayValue                                                             
*
*    DESCRIPTION:           Get value from an edit control.
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

int Edit_GetValue( HWND hCtrl )
{
    char StrTmp[20];
    int Result = 0;

    // Make sure there is text to get
    if( Edit_GetTextLength( hCtrl ) )
    {
        // Get the text from the control
        Edit_GetText( hCtrl, StrTmp, sizeof( StrTmp ) );

        Result = atoi( StrTmp );
    }

    return Result;

}   // Edit_DisplayValue


/************************************************************************
*    FUNCTION NAME:         FlipFlopIntSign                                                             
*
*    DESCRIPTION:           FlipFlop the int sign.
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

int FlipFlopSign( int Value )
{
    int Result( Value );
    
    if( Result )
    {
        //Result = 0x80000000 ^ Result;
        if( Result > 0 )
            Result = -Value;
        else
            Result = abs(Value);
    }

    return Result;  
}   // FlipFlopIntSign


/************************************************************************
*    FUNCTION NAME:         FindChar                                                             
*
*    DESCRIPTION:           Finds a character in a text string and
*                           returns it's position
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

int FindChar( char *SearchStr, char SearchChar, int StartIndex )
{
    int Index = StartIndex;
    bool Result = FALSE;

    while( SearchStr[ Index ] )
    {
        if( SearchStr[ Index ] == SearchChar )
        {
            Result = TRUE;
            break;
        }

        ++Index;
    }

    if( Result )
        return Index;
    else
        return -1;

}   // FindChar


/************************************************************************
*    FUNCTION NAME:         GetTheFileTime                                                             
*
*    DESCRIPTION:           Get the time of the file.
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

FILETIME GetTheFileTime( char *PathToFile )
{
    HANDLE hFile;
    SECURITY_ATTRIBUTES FileSecurity;
    FILETIME fileTime;

    fileTime.dwLowDateTime = 0;
    fileTime.dwHighDateTime = 0;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    hFile = CreateFile( PathToFile, GENERIC_READ,
                         FILE_SHARE_READ, &FileSecurity, OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL, 0 );

    if( hFile == INVALID_HANDLE_VALUE )
        return fileTime;

    GetFileTime( hFile, NULL, NULL, &fileTime );

    if( hFile )
        CloseHandle( hFile );

    return fileTime;

}	// GetTheFileTime


/************************************************************************
*    FUNCTION NAME:         SelectPath                                                             
*
*    DESCRIPTION:           Displays a window so that the path can be 
*                           selected and is stored in a string.
*                           
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
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool SelectPath( HWND hwnd, char *path, char *wndCaption, unsigned int flags )
{
    BROWSEINFO bi;
    LPITEMIDLIST pidlBrowse;
	bool result = FALSE;

	/* Available flags
	#define BIF_RETURNONLYFSDIRS   0x0001  // For finding a folder to start document searching
	#define BIF_DONTGOBELOWDOMAIN  0x0002  // For starting the Find Computer
	#define BIF_STATUSTEXT         0x0004
	#define BIF_RETURNFSANCESTORS  0x0008

	#define BIF_BROWSEFORCOMPUTER  0x1000  // Browsing for Computers.
	#define BIF_BROWSEFORPRINTER   0x2000  // Browsing for Printers	  */

    bi.hwndOwner = hwnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = path;  // Only returns the selected item name, not the full path
    bi.lpszTitle = wndCaption;
    bi.ulFlags = flags;
    bi.lpfn = NULL;
    bi.lParam = 0;
    bi.iImage = 0;
    
    // Allow the user to browse for the item
	pidlBrowse = SHBrowseForFolder( &bi );
    if( pidlBrowse != NULL )
    {
        // Use the returned item list to get the path
        SHGetPathFromIDList( pidlBrowse, path );

		result = TRUE;
    }

	return result;

}   // SelectPath


/************************************************************************
*    FUNCTION NAME:        PointInsideRect                                                             
*
*    DESCRIPTION:          Is the X Y inside the rect?
*
*    FUNCTION PARAMETERS:
*    Input:    PTTouchRect pTouchRect - Array of rects
*              int X - X screen touch point
*              int Y - Y screen touch point
*
*    Output:   int - Index of rectangle, RECT_TOUCH_MISSED if not true
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL PointInsideRect( int X, int Y, LPRECT rect )
{
    BOOL Result = FALSE;

    if( (X >= rect->left) && (X <= rect->right) &&
        (Y >= rect->top) && (Y <= rect->bottom) )
    {
        // The point is inside the rect
        Result = TRUE;
    }
 
    return Result;
    
} // PointInsideRect


/************************************************************************
*    FUNCTION NAME:         memcpy_or
*
*    DESCRIPTION:           Fast or memory copy
*
*    FUNCTION PARAMETERS:
*    Input:    void *dest     - Destination memory
*              void *source   - Source Memory
*              int size       - buffer size in bytes
*              bool clipRect    - To clip or not to clip
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    4/02/02       Function Created
************************************************************************/

//void memcpy_or( void *dest, void *source, int sizeInBytes )
//{
        //memor( dest, source, sizeInBytes >> 2 );

        // Setup the memory pointers into registers
        //unsigned char *pWDest((unsigned char *)dest), *pWSource((unsigned char *)source);

        // Convert to unsigned int size
        //int sizeInQuad( sizeInBytes >> 2 );

        //while( --sizeInBytes )
            //pWDest[sizeInBytes] |= pWSource[sizeInBytes];

        //pWDest[sizeInBytes] |= pWSource[sizeInBytes];

/*
        // Setup the memory pointers into registers
        unsigned int register *pWDest((unsigned int *)dest), *pWSource((unsigned int *)source);

        // Convert to unsigned int size
        //int sizeInQuad( sizeInBytes >> 2 );

        while( --sizeInBytes )
            pWDest[sizeInBytes] |= pWSource[sizeInBytes];

        pWDest[sizeInBytes] |= pWSource[sizeInBytes];
*/
/*
    if( sizeInBytes > 4 )
    {
        //memor( dest, source, sizeInBytes >> 2 );

        // Setup the memory pointers into registers
        unsigned int register *pWDest((unsigned int *)dest), *pWSource((unsigned int *)source);

        // Convert to unsigned int size
        int sizeInQuad( sizeInBytes >> 2 );

        while( --sizeInQuad )
            pWDest[sizeInQuad] |= pWSource[sizeInQuad];

        pWDest[sizeInQuad] |= pWSource[sizeInQuad];
        
        sizeInBytes &= 3;
        if( !sizeInBytes )
        {
            dest[sizeInBytes] |= source[sizeInBytes];
        }
    }
    else
    {
        while( --sizeInBytes )
            dest[sizeInBytes] |= source[sizeInBytes];

        dest[sizeInBytes] |= source[sizeInBytes];
    }
*/  
//} // memcpy_or


/************************************************************************
*    FUNCTION NAME:         memcpy_and
*
*    DESCRIPTION:           Fast and memory copy
*
*    FUNCTION PARAMETERS:
*    Input:    void *dest     - Destination memory
*              void *source   - Source Memory
*              int size       - buffer size in bytes
*              bool clipRect    - To clip or not to clip
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    4/02/02       Function Created
************************************************************************/
/*
void memcpy_and( void *dest, void *source, int sizeInBytes )
{
    if( sizeInBytes > 4 )
    {
        memand( dest, source, sizeInBytes >> 2 );

        // Setup the memory pointers into registers
        unsigned int register *pWDest((unsigned int *)dest), *pWSource((unsigned int *)source);

        // Convert to unsigned int size
        int sizeInQuad( sizeInBytes >> 2 );

        while( --sizeInQuad )
            pWDest[sizeInQuad] &= pWSource[sizeInQuad];

        pWDest[sizeInQuad] &= pWSource[sizeInQuad];
        
        sizeInBytes &= 3;
        if( !sizeInBytes )
        {
            dest[sizeInBytes] |= source[sizeInBytes];
        }
    }
    else
    {
        while( --sizeInBytes )
            dest[sizeInBytes] &= source[sizeInBytes];

        dest[sizeInBytes] &= source[sizeInBytes];
    }

    
}   // memcpy_and
*/
