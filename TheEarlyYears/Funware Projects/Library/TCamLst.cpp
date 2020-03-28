
/************************************************************************
*
*    PRODUCT:         Camera list
*
*    FILE NAME:       TCamLst.cpp
*
*    DESCRIPTION:     Object for holding lists of camera movements.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "TCamLst.h"        // Header file for this *.cpp file.
#include <stdlib.h>          // Declares several commonly used routines such as atoi, random, abs, malloc etc.


/************************************************************************
*    FUNCTION NAME:         Constructor                                                             
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TCameraList::TCameraList()
{
    // Set the format ID for this object so that we can tell
    // when we are trying to load a file that is not ours.
    ObjFileHeader.ObjForamtID = 0x001DFD91;

}   // Constructor


/************************************************************************
*    FUNCTION NAME:         Destructor                                                             
*
*    DESCRIPTION:           Free up all allocated resources.
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

TCameraList::~TCameraList()
{
                
}   // Destructor


/************************************************************************
*    FUNCTION NAME:         Add                                                             
*
*    DESCRIPTION:           Allocates more memory and copies.
*
*    FUNCTION PARAMETERS:
*    Input:    char *t           - String to be added
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

BOOL TCameraList::Add()
{
    PTCameraOffset pTemp;

    // Return if there has been an allocation error
    if( fAllocationError )
        return FALSE;

    // Allocate the string to the size we need
    pTemp = new TCameraOffset;

    // Check the allocation and copy the new string
    if( pTemp )
    {
        // Add the string to the object list
        AddObj( (LPVOID)pTemp );
    }
    else
        fAllocationError = TRUE;

    return (fAllocationError == FALSE);

}   // Add


/************************************************************************
*    FUNCTION NAME:         Free                                                             
*
*    DESCRIPTION:           Free up all allocated resources.
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

void TCameraList::Free()
{
    // Free everything
    FreeObj();

}   // Free


/************************************************************************
*    FUNCTION NAME:         ExportSimpleList                                                             
*
*    DESCRIPTION:           Export data in the size of a byte
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

BOOL TCameraList::ExportSimpleList( char *FileName, int DataSize, BOOL compressType )
{
    SECURITY_ATTRIBUTES FileSec;
    unsigned long bytesWritten;
    BOOL FileWriteResult;
    HANDLE hFile;
    BOOL Result = FALSE;
    WORD ArrayCount, i;

    // Make sure we have something to save to disk
    if( count )
    {
        // Get the number of entries
        ArrayCount = count;
        
        FileSec.nLength = sizeof( SECURITY_ATTRIBUTES );
        FileSec.lpSecurityDescriptor = NULL;
        FileSec.bInheritHandle = FALSE;

        // Create the file
        hFile = CreateFile( FileName, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ,
                            &FileSec, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );

        if( hFile == INVALID_HANDLE_VALUE )
            goto HANDLE_ERROR;

        // Write the word amount of how many items in the list
        FileWriteResult = WriteFile( hFile, &ArrayCount, sizeof( ArrayCount ), &bytesWritten, NULL );
        if( !FileWriteResult || bytesWritten != sizeof( ArrayCount ) )
            goto HANDLE_ERROR;

        // Write the rest of the data to the file
        for( i = 0; i < ArrayCount; ++i )
        {
            if( DataSize == BYTE_SIZE )
            {
                if( !WriteByteList( i, hFile, compressType ) )
                    goto HANDLE_ERROR;
            }
            else if( DataSize == WORD_SIZE )
            {
                if( !WriteWordList( i, hFile, compressType ) )
                    goto HANDLE_ERROR;
            }
            else if( DataSize == INT_SIZE )
            {
                if( !WriteIntList( i, hFile, compressType ) )
                    goto HANDLE_ERROR;
            }
        }
    }

    Result = TRUE;

HANDLE_ERROR:;

    if( hFile )
        CloseHandle( hFile );

    return Result;
}   // ExportIntList


/************************************************************************
*    FUNCTION NAME:         WriteByteList                                                             
*
*    DESCRIPTION:           Export data in the size of a byte
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

BOOL TCameraList::WriteByteList( int index, HANDLE hFile, BOOL compressType )
{
    unsigned long bytesWritten;
    BOOL FileWriteResult;
    BOOL Result = FALSE;
    TCameraByte Camera;

    // Write out every entry
    if( compressType == UNCOMPRESSED )
    {
        Camera.XPos = GetXPos( index );
        Camera.YPos = GetYPos( index );
        Camera.ZPos = GetZPos( index );

        Camera.XRot = GetXRot( index );
        Camera.YRot = GetYRot( index );
        Camera.ZRot = GetZRot( index );

        // Write the data to the file
        FileWriteResult = WriteFile( hFile, &Camera, sizeof( Camera ), &bytesWritten, NULL );
        if( !FileWriteResult || bytesWritten != sizeof( Camera ) )
            goto HANDLE_ERROR;
    }
    else
    {
        if( index == 0 )
        {
            Camera.XPos = GetXPos( index );
            Camera.YPos = GetYPos( index );
            Camera.ZPos = GetZPos( index );

            Camera.XRot = GetXRot( index );
            Camera.YRot = GetYRot( index );
            Camera.ZRot = GetZRot( index );

            counter = 1;
        }
        else
        {
            // Is the next entry the same?
            if( Camera.XPos == GetXPos( index ) && Camera.YPos == GetYPos( index ) &&
                Camera.ZPos == GetZPos( index ) && Camera.XRot == GetXRot( index ) &&
                Camera.YRot == GetYRot( index ) && Camera.ZRot == GetZRot( index ) )
            {
                ++counter;

                // If this is the last entry, just writ it out
                if( index == count-1 )
                {
                    // Write the data to the file
                    FileWriteResult = WriteFile( hFile, &Camera, sizeof( Camera ), &bytesWritten, NULL );
                    if( !FileWriteResult || bytesWritten != sizeof( Camera ) )
                        goto HANDLE_ERROR;

                    // Write the counter to the file
                    FileWriteResult = WriteFile( hFile, &counter, sizeof( counter ), &bytesWritten, NULL );
                    if( !FileWriteResult || bytesWritten != sizeof( counter ) )
                        goto HANDLE_ERROR;
                }
            }
            else  // the next entry is not the same
            {
                // Write the data to the file
                FileWriteResult = WriteFile( hFile, &Camera, sizeof( Camera ), &bytesWritten, NULL );
                if( !FileWriteResult || bytesWritten != sizeof( Camera ) )
                    goto HANDLE_ERROR;

                // Write the counter to the file
                FileWriteResult = WriteFile( hFile, &counter, sizeof( counter ), &bytesWritten, NULL );
                if( !FileWriteResult || bytesWritten != sizeof( counter ) )
                    goto HANDLE_ERROR;

                // init to the next entry
                Camera.XPos = GetXPos( index );
                Camera.YPos = GetYPos( index );
                Camera.ZPos = GetZPos( index );

                Camera.XRot = GetXRot( index );
                Camera.YRot = GetYRot( index );
                Camera.ZRot = GetZRot( index );

                counter = 1;

                // If this is the last entry, just writ it out
                if( index == count-1 )
                {
                    // Write the data to the file
                    FileWriteResult = WriteFile( hFile, &Camera, sizeof( Camera ), &bytesWritten, NULL );
                    if( !FileWriteResult || bytesWritten != sizeof( Camera ) )
                        goto HANDLE_ERROR;

                    // Write the counter to the file
                    FileWriteResult = WriteFile( hFile, &counter, sizeof( counter ), &bytesWritten, NULL );
                    if( !FileWriteResult || bytesWritten != sizeof( counter ) )
                        goto HANDLE_ERROR;
                }
            }
        }
    }

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // WriteByteList


/************************************************************************
*    FUNCTION NAME:         WriteWordList                                                             
*
*    DESCRIPTION:           Export data in the size of a word
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

BOOL TCameraList::WriteWordList( int index, HANDLE hFile, BOOL compressType )
{
    unsigned long bytesWritten;
    BOOL FileWriteResult;
    BOOL Result = FALSE;
    TCameraWord Camera;

    // Write out every entry
    if( compressType == UNCOMPRESSED )
    {
        Camera.XPos = GetXPos( index );
        Camera.YPos = GetYPos( index );
        Camera.ZPos = GetZPos( index );

        Camera.XRot = GetXRot( index );
        Camera.YRot = GetYRot( index );
        Camera.ZRot = GetZRot( index );

        // Write the data to the file
        FileWriteResult = WriteFile( hFile, &Camera, sizeof( Camera ), &bytesWritten, NULL );
        if( !FileWriteResult || bytesWritten != sizeof( Camera ) )
            goto HANDLE_ERROR;
    }
    else
    {
        if( index == 0 )
        {
            Camera.XPos = GetXPos( index );
            Camera.YPos = GetYPos( index );
            Camera.ZPos = GetZPos( index );

            Camera.XRot = GetXRot( index );
            Camera.YRot = GetYRot( index );
            Camera.ZRot = GetZRot( index );

            counter = 1;
        }
        else
        {
            // Is the next entry the same?
            if( Camera.XPos == GetXPos( index ) && Camera.YPos == GetYPos( index ) &&
                Camera.ZPos == GetZPos( index ) && Camera.XRot == GetXRot( index ) &&
                Camera.YRot == GetYRot( index ) && Camera.ZRot == GetZRot( index ) )
            {
                ++counter;

                // If this is the last entry, just writ it out
                if( index == count-1 )
                {
                    // Write the data to the file
                    FileWriteResult = WriteFile( hFile, &Camera, sizeof( Camera ), &bytesWritten, NULL );
                    if( !FileWriteResult || bytesWritten != sizeof( Camera ) )
                        goto HANDLE_ERROR;

                    // Write the counter to the file
                    FileWriteResult = WriteFile( hFile, &counter, sizeof( counter ), &bytesWritten, NULL );
                    if( !FileWriteResult || bytesWritten != sizeof( counter ) )
                        goto HANDLE_ERROR;
                }
            }
            else  // the next entry is not the same
            {
                // Write the data to the file
                FileWriteResult = WriteFile( hFile, &Camera, sizeof( Camera ), &bytesWritten, NULL );
                if( !FileWriteResult || bytesWritten != sizeof( Camera ) )
                    goto HANDLE_ERROR;

                // Write the counter to the file
                FileWriteResult = WriteFile( hFile, &counter, sizeof( counter ), &bytesWritten, NULL );
                if( !FileWriteResult || bytesWritten != sizeof( counter ) )
                    goto HANDLE_ERROR;

                // init to the next entry
                Camera.XPos = GetXPos( index );
                Camera.YPos = GetYPos( index );
                Camera.ZPos = GetZPos( index );

                Camera.XRot = GetXRot( index );
                Camera.YRot = GetYRot( index );
                Camera.ZRot = GetZRot( index );

                counter = 1;

                // If this is the last entry, just writ it out
                if( index == count-1 )
                {
                    // Write the data to the file
                    FileWriteResult = WriteFile( hFile, &Camera, sizeof( Camera ), &bytesWritten, NULL );
                    if( !FileWriteResult || bytesWritten != sizeof( Camera ) )
                        goto HANDLE_ERROR;

                    // Write the counter to the file
                    FileWriteResult = WriteFile( hFile, &counter, sizeof( counter ), &bytesWritten, NULL );
                    if( !FileWriteResult || bytesWritten != sizeof( counter ) )
                        goto HANDLE_ERROR;
                }
            }
        }
    }

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // WriteWordList


/************************************************************************
*    FUNCTION NAME:         WriteIntList                                                             
*
*    DESCRIPTION:           Export data in the size of a int
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

BOOL TCameraList::WriteIntList( int index, HANDLE hFile, BOOL compressType )
{
    unsigned long bytesWritten;
    BOOL FileWriteResult;
    BOOL Result = FALSE;
    TCameraInt Camera;

    // Write out every entry
    if( compressType == UNCOMPRESSED )
    {
        Camera.XPos = GetXPos( index );
        Camera.YPos = GetYPos( index );
        Camera.ZPos = GetZPos( index );

        Camera.XRot = GetXRot( index );
        Camera.YRot = GetYRot( index );
        Camera.ZRot = GetZRot( index );

        // Write the data to the file
        FileWriteResult = WriteFile( hFile, &Camera, sizeof( Camera ), &bytesWritten, NULL );
        if( !FileWriteResult || bytesWritten != sizeof( Camera ) )
            goto HANDLE_ERROR;
    }
    else
    {
        if( index == 0 )
        {
            Camera.XPos = GetXPos( index );
            Camera.YPos = GetYPos( index );
            Camera.ZPos = GetZPos( index );

            Camera.XRot = GetXRot( index );
            Camera.YRot = GetYRot( index );
            Camera.ZRot = GetZRot( index );

            counter = 1;
        }
        else
        {
            // Is the next entry the same?
            if( Camera.XPos == GetXPos( index ) && Camera.YPos == GetYPos( index ) &&
                Camera.ZPos == GetZPos( index ) && Camera.XRot == GetXRot( index ) &&
                Camera.YRot == GetYRot( index ) && Camera.ZRot == GetZRot( index ) )
            {
                ++counter;

                // If this is the last entry, just writ it out
                if( index == count-1 )
                {
                    // Write the data to the file
                    FileWriteResult = WriteFile( hFile, &Camera, sizeof( Camera ), &bytesWritten, NULL );
                    if( !FileWriteResult || bytesWritten != sizeof( Camera ) )
                        goto HANDLE_ERROR;

                    // Write the counter to the file
                    FileWriteResult = WriteFile( hFile, &counter, sizeof( counter ), &bytesWritten, NULL );
                    if( !FileWriteResult || bytesWritten != sizeof( counter ) )
                        goto HANDLE_ERROR;
                }
            }
            else  // the next entry is not the same
            {
                // Write the data to the file
                FileWriteResult = WriteFile( hFile, &Camera, sizeof( Camera ), &bytesWritten, NULL );
                if( !FileWriteResult || bytesWritten != sizeof( Camera ) )
                    goto HANDLE_ERROR;

                // Write the counter to the file
                FileWriteResult = WriteFile( hFile, &counter, sizeof( counter ), &bytesWritten, NULL );
                if( !FileWriteResult || bytesWritten != sizeof( counter ) )
                    goto HANDLE_ERROR;

                // init to the next entry
                Camera.XPos = GetXPos( index );
                Camera.YPos = GetYPos( index );
                Camera.ZPos = GetZPos( index );

                Camera.XRot = GetXRot( index );
                Camera.YRot = GetYRot( index );
                Camera.ZRot = GetZRot( index );

                counter = 1;

                // If this is the last entry, just writ it out
                if( index == count-1 )
                {
                    // Write the data to the file
                    FileWriteResult = WriteFile( hFile, &Camera, sizeof( Camera ), &bytesWritten, NULL );
                    if( !FileWriteResult || bytesWritten != sizeof( Camera ) )
                        goto HANDLE_ERROR;

                    // Write the counter to the file
                    FileWriteResult = WriteFile( hFile, &counter, sizeof( counter ), &bytesWritten, NULL );
                    if( !FileWriteResult || bytesWritten != sizeof( counter ) )
                        goto HANDLE_ERROR;
                }
            }
        }
    }

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // WriteIntList