
/************************************************************************
*
*    PRODUCT:         Object List
*
*    FILE NAME:       TObjList.cpp
*
*    DESCRIPTION:     Base class for managing lists of pointers. Inherit
*                     from this class to manage lists of specific data types.
*
*    IMPLEMENTATION:  
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "TObjList.h"        // Header file for this *.cpp file.
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

TObjList::TObjList()
{
    // Init class members
    count = 0;
    ppPointer = NULL;
    fAllocationError = FALSE;
    ObjCheckHeader = OBJECT_CHECK_HEADER;

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

TObjList::~TObjList()
{
    FreeObj();
                
}   // Destructor


/************************************************************************
*    FUNCTION NAME:         Add                                                             
*
*    DESCRIPTION:           Allocates more memory and copies the objects data.
*
*    FUNCTION PARAMETERS:
*    Input:    LPVOID t           - Object to be added
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

BOOL TObjList::AddObj( LPVOID Obj )
{
    LPVOID *ppTemp;
    BOOL Result = FALSE;

    // Return if there has been an allocation error
    if( fAllocationError )
        goto HANDLE_ERROR;

    // Allocate the buffer to hold the object pointers
    // Copy the pointer to a temp variable for coping the array
    // into the newly allocated array.
    ppTemp = ppPointer;
    ppPointer = new LPVOID[count+1];

    // Check that we have a pointer and that we have atleast one
    // pointer
    if( ppPointer )
    {
        if( count )
        {
            memcpy( ppPointer, ppTemp, sizeof(LPVOID)*count );
            delete [] ppTemp;
        }

        // Copy the pointer to the newly allocated spot
        ppPointer[count] = Obj;

        ++count;
    }
    else
    {
        fAllocationError = TRUE;
        ppPointer = ppTemp;
        goto HANDLE_ERROR;
    }

    Result = TRUE;

    HANDLE_ERROR:;

    return Result;

}   // Add


/************************************************************************
*    FUNCTION NAME:         GetObj                                                             
*
*    DESCRIPTION:           Returns a pointer to the object at the given index
*
*    FUNCTION PARAMETERS:
*    Input:    int Index         - Index of the requested object
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

LPVOID TObjList::GetObj( int Index )
{
    LPVOID Result = NULL;

    // Check that we are not out of bounds before handing out the pointer
    if( Index < count )
        Result = ppPointer[Index];

    return Result;

}   // GetObj


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

void TObjList::FreeObj()
{
    if( ppPointer && count )
    {
        // Delete all the allocated pointers
        for( int i = 0; i < count; ++i )
            delete [] GetObj( i );

        // Delete the allocated pointer array
        delete [] ppPointer;

        ppPointer = NULL;
        count = 0;
    }            
}   // Free


/************************************************************************
*    FUNCTION NAME:         MixUp                                                             
*
*    DESCRIPTION:           Mix up all the objects into a random order
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

void TObjList::MixUp()
{
    LPVOID ppTemp;
    int i, Switch;

    // Reset the random number generator
	#ifndef MSVC_COMPILER
    randomize();
    #endif

    // It's no use if we don't have more than 2 items
    if( count > 2 )
    {
        for( i = 0; i < count; ++i )
        {
            Switch = rand() % count;

            if( i != Switch )
            {
                ppTemp = ppPointer[i];
                ppPointer[i] = ppPointer[Switch];
                ppPointer[Switch] = ppTemp;   
            }
        }
    }

}   // MixUp


/************************************************************************
*    FUNCTION NAME:         Delete                                                             
*
*    DESCRIPTION:           Delete pointer to the object at the given index
*
*    FUNCTION PARAMETERS:
*    Input:    int Index         - Index of the requested object
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

void TObjList::Delete( int Index )
{
    LPVOID *ppTemp;
    int i, inc = 0;

    // Return if there has been an allocation error
    if( fAllocationError || !count )
        return;

    if( count-1 > 0 )
    {
        // Delete the allocated object
        delete [] GetObj( Index );

        // Allocate the buffer to hold the object pointers
        // Copy the pointer to a temp variable for coping the array
        // into the newly allocated array.
        ppTemp = ppPointer;
        ppPointer = new LPVOID[count-1];

        // Check that we have a pointer
        if( ppPointer )
        {
            // Copy over all the other valid pointers
            for( i = 0; i < count; ++i )
            {
                if( i != Index )
                {
                    ppPointer[inc] = ppTemp[i];
                    ++inc;
                }
            }

            // Delete the old pointer array
            delete [] ppTemp;

            --count;
        }
        else
        {
            fAllocationError = TRUE;
            ppPointer = ppTemp;
        }
    }
    else
        // Just free the obj if we are deleting our last pointer
        FreeObj();

}   // Delete


/************************************************************************
*    FUNCTION NAME:         MoveToNewPos                                                             
*
*    DESCRIPTION:           Moves an index to a new position
*
*    FUNCTION PARAMETERS:
*    Input:    int Index         - Index of the bject to be moved
*              int NewIndex     - New index position the current index
*                                  is to be moved 
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

void TObjList::MoveToNewPos( int CurrentIndex, int NewIndex )
{
    LPVOID pTemp;

    if( CurrentIndex < count )
    {
        pTemp = ppPointer[CurrentIndex];

        if( CurrentIndex < NewIndex )
            memmove( &ppPointer[CurrentIndex], &ppPointer[CurrentIndex+1], sizeof(LPVOID)*(NewIndex-CurrentIndex) );

        else if( CurrentIndex > NewIndex )
            memmove( &ppPointer[NewIndex+1], &ppPointer[NewIndex], sizeof(LPVOID)*(CurrentIndex-NewIndex) );

        ppPointer[ NewIndex ] = pTemp;
    }

}   // MoveToNewPos


/************************************************************************
*    FUNCTION NAME:         SaveToFile                                                             
*
*    DESCRIPTION:           Save the object data to a file
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TObjList::SaveToFile( char *FileName, DWORD Flags )
{
    SECURITY_ATTRIBUTES FileSec;
    HANDLE hFile;
    BOOL Result = FALSE;

    FileSec.nLength = sizeof( SECURITY_ATTRIBUTES );
    FileSec.lpSecurityDescriptor = NULL;
    FileSec.bInheritHandle = FALSE;

    // Create the file
    hFile = CreateFile( FileName, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ,
                        &FileSec, Flags, FILE_ATTRIBUTE_NORMAL, 0 );

    if( hFile == INVALID_HANDLE_VALUE )
        goto HANDLE_ERROR;

    // Set the file pointer to the end of the file because we want to add more data to the file
    if( Flags == OPEN_ALWAYS )
        SetFilePointer( hFile, 0, 0, FILE_END );

    // Wite all the data to the file
    Result = SaveToFile( hFile );

HANDLE_ERROR:;

    if( hFile )
        CloseHandle( hFile );

    return Result;

}   // SaveToFile


/************************************************************************
*    FUNCTION NAME:         SaveToFile                                                             
*
*    DESCRIPTION:           Save the object data to a file
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TObjList::SaveToFile( HANDLE hFile )
{
    unsigned long bytesWritten, ObjSize;
    BOOL Result = FALSE;
    BOOL FileWriteResult;
    int i;
    DWORD TmpObjCheckHeader;

    // Set the count value so we know how many object to load when reading in the file
    ObjFileHeader.ObjCount = count;

    // Write the file header
    FileWriteResult = WriteFile( hFile, &ObjFileHeader, sizeof( TObjFileHeader ), &bytesWritten, NULL );
    if( !FileWriteResult || bytesWritten != sizeof( TObjFileHeader ) )
        goto HANDLE_ERROR;

    // Wite all the data to the file
    for( i = 0; i < count; ++i )
    {
        // Get the data header incase it needed to change from object to object
        // This can be a set value to check for load errors or a data size for data
        // that is not consistant.
        TmpObjCheckHeader = GetObjDataHeader( i );

        // Write the object checker. This is how we make sure we are not reading something
        // we are not supposed to. So we write this value down before each of the object data is written.
        // We can also use this header to hold the size of object data that is not a consistant size.
        FileWriteResult = WriteFile( hFile, &TmpObjCheckHeader, sizeof( DWORD ), &bytesWritten, NULL );
        if( !FileWriteResult || bytesWritten != sizeof( DWORD ) )
            goto HANDLE_ERROR;

        // Get the size of this object's data
        ObjSize = GetObjSize( i );

        if( ObjSize )
        {
            // Write the data
            FileWriteResult = WriteFile( hFile, GetObj( i ), ObjSize, &bytesWritten, NULL );
            if( !FileWriteResult || bytesWritten != ObjSize )
                goto HANDLE_ERROR;
        }
        else
            goto HANDLE_ERROR;
    }

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // SaveToFile


/************************************************************************
*    FUNCTION NAME:         LoadFromFile                                                             
*
*    DESCRIPTION:           Load the object data from a file
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TObjList::LoadFromFile( char *FileName, DWORD Flags )
{
    SECURITY_ATTRIBUTES FileSec;
    HANDLE hFile;
    BOOL Result = FALSE;

    FileSec.nLength = sizeof( SECURITY_ATTRIBUTES );
    FileSec.lpSecurityDescriptor = NULL;
    FileSec.bInheritHandle = FALSE;

    // Free everything before loading a new file
    FreeObj();

    // Create the file
    hFile = CreateFile( FileName, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ,
                        &FileSec, Flags, FILE_ATTRIBUTE_NORMAL, 0 );

    if( hFile == INVALID_HANDLE_VALUE )
        goto HANDLE_ERROR;

    // Set the file pointer to the end of the file because we want to add more data to the file
    if( Flags == OPEN_ALWAYS )
        SetFilePointer( hFile, 0, 0, FILE_END );

    // Write all the data to the file
    Result = LoadFromFile( hFile );

HANDLE_ERROR:;

    if( hFile )
        CloseHandle( hFile );

    return Result;

}   // LoadFromFile 


/************************************************************************
*    FUNCTION NAME:         LoadFromFile                                                             
*
*    DESCRIPTION:           Load the object data from a file
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TObjList::LoadFromFile( HANDLE hFile )
{
    unsigned long bytesRead;
    BOOL Result = FALSE;
    BOOL FileReadResult;
    int i;
    TObjFileHeader TmpFileHeader;

    // Read the file header
    FileReadResult = ReadFile( hFile, &TmpFileHeader, sizeof( TObjFileHeader ), &bytesRead, NULL );
    if( !FileReadResult || bytesRead != sizeof( TObjFileHeader ) )
        goto HANDLE_ERROR;

    // Make sure this is the correct file and that there is something in it
    // The ObjFileHeader.ObjForamtID is set by the inhearted class
    if( TmpFileHeader.ObjForamtID != ObjFileHeader.ObjForamtID )
        goto HANDLE_ERROR;

    // Record the other file format items
    ObjFileHeader.Future1 = TmpFileHeader.Future1;
    ObjFileHeader.Future2 = TmpFileHeader.Future2;

    // Read all the data from the file
    for( i = 0; i < (int)TmpFileHeader.ObjCount; ++i )
    {
        // Before each chuck of data is a check value to ensure the data is correct
        // Read the file header
        FileReadResult = ReadFile( hFile, &ObjCheckHeader, sizeof( DWORD ), &bytesRead, NULL );
        if( !FileReadResult || bytesRead != sizeof( DWORD ) )
            goto HANDLE_ERROR;

        // Load the object data. This function is virtual so that reading in the 
        // object data can be customized for a given situation.
        if( !LoadObjectData( hFile, i ) )
            goto HANDLE_ERROR;
    }

    Result = TRUE;

HANDLE_ERROR:;

    if( !Result )
        fAllocationError = TRUE;

    return Result;

}   // LoadFromFile


/************************************************************************
*    FUNCTION NAME:         LoadObjectData                                                             
*
*    DESCRIPTION:           Load the object data from a file. This function
*                           is virtual so that reading in the object data can
*                           be customized for a given situation.
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TObjList::LoadObjectData( HANDLE hFile, int Index )
{
    BOOL Result = FALSE;
    BOOL FileReadResult;
    unsigned long bytesRead, ObjSize;

    // Get the size of this object's data
    ObjSize = GetObjSize( Index );

    if( ObjSize && (ObjCheckHeader == OBJECT_CHECK_HEADER) )
    {
        // Allocate a blank object
        if( !Add() )
            goto HANDLE_ERROR;

        // Read in the data
        FileReadResult = ReadFile( hFile, GetObj( Index ), ObjSize, &bytesRead, NULL );
        if( !FileReadResult || bytesRead != ObjSize )
            goto HANDLE_ERROR;
    }

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // LoadObjectData


/************************************************************************
*    FUNCTION NAME:         LoadFromPointer                                                             
*
*    DESCRIPTION:           Load the object data from a pointer
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
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

BOOL TObjList::LoadFromPointer( BYTE *pData )
{
    BOOL Result = FALSE;
    int i;
    PTObjFileHeader pFileHeader;

    // Init the file header pointer
    pFileHeader = PTObjFileHeader( pData );

    // Make sure this is the correct file and that there is something in it
    // The ObjFileHeader.ObjForamtID is set by the inhearted class
    if( pFileHeader->ObjForamtID != ObjFileHeader.ObjForamtID )
        goto HANDLE_ERROR;

    // Record the other file format items
    ObjFileHeader.Future1 = pFileHeader->Future1;
    ObjFileHeader.Future2 = pFileHeader->Future2;

    // Inc the data pointer
    pData += sizeof( TObjFileHeader );

    // Read all the data from the file
    for( i = 0; i < (int)pFileHeader->ObjCount; ++i )
    {
        // Before each chuck of data is a check value to ensure the data is correct
        ObjCheckHeader = *(DWORD *)pData;

        // Inc the data pointer
        pData += sizeof( DWORD );

        // Load the object data. This function is virtual so that reading in the 
        // object data can be customized for a given situation.
        pData = LoadObjectDataFromPointer( pData, i );
        if( !pData )
            goto HANDLE_ERROR;
    }

    Result = TRUE;

HANDLE_ERROR:;

    if( !Result )
        fAllocationError = TRUE;

    return Result;

}   // LoadFromPointer


/************************************************************************
*    FUNCTION NAME:         LoadObjectDataFromPointer                                                             
*
*    DESCRIPTION:           Load the object from a pointer. This function
*                           is virtual so that reading in the object data can
*                           be customized for a given situation.
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BYTE *TObjList::LoadObjectDataFromPointer( BYTE *pData, int Index )
{
    int ObjSize;

    // Get the size of this object's data
    ObjSize = GetObjSize( Index );

    // Do we have a file size and do
    if( ObjSize && (ObjCheckHeader == OBJECT_CHECK_HEADER) )
    {
        // Allocate a blank object
        if( !Add() )
        {
            pData = NULL;
            goto HANDLE_ERROR;
        }

        // Read in the data
        memcpy( GetObj( Index ), pData, ObjSize );

        // Inc the pointer
        pData += ObjSize;
    }
    else  // Null the pointer because we had a problem
        pData = NULL;

HANDLE_ERROR:;

    return pData;

}   // LoadObjectDataFromPointer