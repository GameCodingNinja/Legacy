
/************************************************************************
*
*    PRODUCT:         Screen Saver To Go
*
*    FILE NAME:       Loader.cpp
*
*    DESCRIPTION:     Code for loading the graphics from the back of the file 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "Loader.h"          // This projects header file
#include "gtools.h"          // A misc group of useful functions
#include "TStrList.h"        // Header file for the string list class.
#include "DecodGif.h"        // GIF decoder which can load GIFs from file or resource
#include "TJPEG.h"           // JPEG Library


/************************************************************************
*    FUNCTION NAME:         Loader_OpenEXEandReadHeader                                                             
*
*    DESCRIPTION:           Open this applications exe file and read in
*                           the header information. 
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

BOOL Loader_OpenEXEandReadHeader( PTFileHeader pFileHeader, PTProjectPrefs pProjPrefsData, PTPageList pPageList, char *pPath )
{
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long bytesReadWrite;
    HANDLE hFile;
    BOOL FileReadWriteResult;
    BOOL Result = FALSE;
    int i;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    // Open the file on the hard drive
    hFile = CreateFile( pPath, GENERIC_READ, FILE_SHARE_READ,
                        &FileSecurity, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hFile == INVALID_HANDLE_VALUE )
    {
        PostMsg( "Error", "Can't open application file (%s) on disk to run program.", pPath );
        goto FREE_MEMORY;
    }

    // Set the file pointer to the end of the file and then back it up to read the file header
    SetFilePointer( hFile, -sizeof(TFileHeader), 0, FILE_END );

    // Read the file header which is at the end of the file
    FileReadWriteResult = ReadFile( hFile, pFileHeader, sizeof(TFileHeader), &bytesReadWrite, NULL );
    if( !FileReadWriteResult || bytesReadWrite != sizeof(TFileHeader) )
    {
        PostMsg( "Error", "Error reading in application file header. (%s)", pPath );
        goto FREE_MEMORY;
    }

    // Check that we have a proper formatID. This is just an exact number and if that
    // number is not the same, we are not looking at the data we should be.
    if( pFileHeader->FormatID != FORMAT_ID )
    {
        PostMsg( "Error", "Format ID does not match. Application format out of sync. (%s)", pPath );
        goto FREE_MEMORY;
    }

    // Set the file pointer to read in the applications preferences
    // This is where we use the data offset that is saved with the header
    SetFilePointer( hFile, pFileHeader->ProjectDataOffset, 0, FILE_BEGIN );

    // Read contents of preferences data into structure
    FileReadWriteResult = ReadFile( hFile, pProjPrefsData, sizeof(TProjectPrefs), &bytesReadWrite, NULL );
    if( !FileReadWriteResult || bytesReadWrite != sizeof(TProjectPrefs) )
    {
        PostMsg( "Error", "Error reading in application preferences. (%s)", pPath );
        goto FREE_MEMORY;
    }

    // Check the preferences ID for vaild number
    if( pProjPrefsData->PrefsCheckID != PERFS_CHECK_ID )
    {
        PostMsg( "Error", "Preference ID does not match. Preference data out of sync." );
        goto FREE_MEMORY;
    }

    // Make sure we have some pages just to be safe
    if( !pProjPrefsData->Total_Num_Of_Pages )
    {
        PostMsg( "Error", "Screen saver has no pages." );
        goto FREE_MEMORY;
    }

    // Only read in the page data if it hasen't been read in before
    if( pPageList->Count() == 0 )
    {
        // Jump the file pointer to the offset the page data is starting at
        SetFilePointer( hFile, pProjPrefsData->PageDataOffset, 0, FILE_BEGIN );

        // Open the pages that have been saved
        for( i = 0; i < pProjPrefsData->Total_Num_Of_Pages; ++i )
        {
            // Add a blank page
			if( !pPageList->Add() )
	        {
	            PostMsg( "Error", "Error adding page." );
	            goto FREE_MEMORY;
	        }

            // Read contents of file into allocated memory
            if( !ReadFile( hFile, pPageList->GetObj( i ), pPageList->GetObjSize( i ), &bytesReadWrite, NULL ) )
            {
                PostMsg( "Error", "Error saving page data to disk." );
                goto FREE_MEMORY;
            }

            // Check the preferences ID for vaild number
            if( pPageList->GetPageCheckID( i ) != PAGE_CHECK_ID )
            {
                PostMsg( "Error", "Page ID does not match. Page data out of sync." );
                goto FREE_MEMORY;
            }
        }
    }

    Result = TRUE;


FREE_MEMORY:;

    // Close the file handle
    if( hFile != INVALID_HANDLE_VALUE )
        CloseHandle( hFile );

    return Result;

}   // Loader_OpenEXEandReadHeader


/************************************************************************
*    FUNCTION NAME:         Loader_LoadMainFile                                                             
*
*    DESCRIPTION:           Load the bitmap
*
*    FUNCTION PARAMETERS:
*    Input:    PTGBase pBase - Pointer to the base graphic object
*              WORD PageToLoad - The page to load 
*
*    Output:   int - The datatype of the loaded file
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL Loader_LoadFile( HWND hwnd, PTBitmap pBitmap, DWORD DataOffset, char *pPath )
{
    BOOL Result = FALSE;
    TLoadBuffer lb;
    TGif gif;
    TJPEG jpeg;

    // If there was an error, keep them from going to any other pages
    // Also, get the data type so we know what to do with the buffer    
    if( Loader_LoadFileDataFromEXEfile( DataOffset, &lb, pPath ) )
    {
        // Load the file
        // Give the pointer over to the dib class. It will delete it when it is done
        switch( lb.Type )
        {
            case FILE_TYPE_BMP:
                Result = pBitmap->LoadFromBuffer( lb.pData, lb.Size );
                // Normally we don't delete the uncompressed buffer because it ends up being owned by the
                // DIB class and get freed there when done but because there was an error, we free everything.
                if( !Result ) delete [] lb.pData;
            break;

            case FILE_TYPE_JPG:
                Result = pBitmap->LoadFromBuffer( jpeg.LoadFromBuffer( lb.pData, lb.Size ) );
            break;

            case FILE_TYPE_GIF:
                Result = pBitmap->LoadFromBuffer( gif.LoadFromBuffer( lb.pData, lb.Size ) );
            break;
        }

        if( !Result )
            PostMsg( "Error", "Error loading bitmap." );
    }

    return Result;

}   // Loader_LoadMainFile


/************************************************************************
*    FUNCTION NAME:         Loader_LoadFileDataFromEXEfile                                                             
*
*    DESCRIPTION:           Loads the imgage data from the exe 
*
*    FUNCTION PARAMETERS:
*    Input:    DWORD Offset - Offset to the compressed data
*              WORD *DataType - Pointer to a word to save the data type 
*
*    Output:   BYTE * - Pointer to filled data buffer
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    h rosenorn    12/10/00      Removed LZW Compression
************************************************************************/

BOOL Loader_LoadFileDataFromEXEfile( DWORD Offset, PTLoadBuffer pLoadBuffer, char *pPath )
{
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long bytesReadWrite;
    BOOL FileReadWriteResult;
    HANDLE hFile;
    BOOL Result = FALSE;
    TDataHeader DataHeader;
    BYTE *pUncBuffer;
    DWORD FileSize;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    // Init the load data buffer
    pLoadBuffer->Type = 0;
    pLoadBuffer->Size = 0;
    pLoadBuffer->pData = NULL;

    // Open the file on the hard drive
    hFile = CreateFile( pPath, GENERIC_READ, FILE_SHARE_READ,
                           &FileSecurity, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hFile == INVALID_HANDLE_VALUE )
    {
        PostMsg( "Error", "Can't open application file (%s) on disk.", pPath );
        goto FREE_MEMORY;
    }

    // Set the file pointer to the end of the file and then back it up to read the file header
    SetFilePointer( hFile, Offset, 0, FILE_BEGIN );

    // Read the data header that is at the beginning of all the data
    FileReadWriteResult = ReadFile( hFile, &DataHeader, sizeof(TDataHeader), &bytesReadWrite, NULL );
    if( !FileReadWriteResult || bytesReadWrite != sizeof(TDataHeader) )
    {
        PostMsg( "Error", "Error reading in file data header." );
        goto FREE_MEMORY;
    }

    // Varify the data header is what we are looking for
    if( !(DataHeader.Type > START_FILE_TYPE && DataHeader.Type < MAX_FILE_TYPES) )
    {
        PostMsg( "Error", "Error reading in file data header. Header type out of sync." );
        goto FREE_MEMORY;
    }

    pUncBuffer = new BYTE[DataHeader.UncompSize + 1];
    if( !pUncBuffer /*|| (DataHeader.CompSize ? !pCmpBuffer : FALSE)*/ )
    {
        PostMsg( "Error", "Memory allocation error while trying to load compressed file data." );

        // Normally we don't delete the uncompressed buffer because it ends up being owned by the
        // DIB class and get freed there when done but because there was an error, we free everything.
        delete [] pUncBuffer;
        goto FREE_MEMORY;
    }

    // Terminate the buffers
    pUncBuffer[ DataHeader.UncompSize ] = 0;

    FileSize = DataHeader.UncompSize;
    FileReadWriteResult = ReadFile( hFile, pUncBuffer, DataHeader.UncompSize, &bytesReadWrite, NULL );

    // The UncompSize and CompSize are the same for uncompressed files
    if( !FileReadWriteResult || bytesReadWrite != FileSize )
    {
        PostMsg( "Error", "Error reading in compressed data" );
        // Normally we don't delete the uncompressed buffer because it ends up being owned by the
        // DIB class and get freed there when done but because there was an error, we free everything.
        delete [] pUncBuffer;
        goto FREE_MEMORY;
    }

    // Close out the file now before we do anything else
    CloseHandle( hFile );
    hFile = NULL;

    // Save the pointer to the data
    Result = TRUE;

    // Fillout the load data structure so that we know what we loaded
    pLoadBuffer->Type = DataHeader.Type;
    pLoadBuffer->Size = DataHeader.UncompSize;
    pLoadBuffer->pData = pUncBuffer;


FREE_MEMORY:;

    // Close the file handle
    if( hFile != INVALID_HANDLE_VALUE )
        CloseHandle( hFile );

    return Result;

}   // Loader_LoadImageFromEXEfile
