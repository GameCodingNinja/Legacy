
/************************************************************************
*
*    PRODUCT:         String List
*
*    FILE NAME:       TStrList.h
*
*    DESCRIPTION:     Object for holding lists of strings
*
*    IMPLEMENTATION:  
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __STRINGLIST_H__
#define __STRINGLIST_H__ 

#include "CObjList.h"        // Base class header file

template <class obj_type>
class TStringList : public CObjLst<obj_type>
{
protected:

    // Get the size of the data (string) plus the terminator
    virtual int GetObjSize( int Index ){ return (strlen( GetString( Index ) ) + 1) * sizeof( char ); };
    // Get the object check data incase we want to change it.
    // For our use, this header is holding the size of the current string object
    // because each string can be a different size.
    virtual DWORD GetObjDataHeader( int Index ){ return (strlen( GetString( Index ) ) + 1) * sizeof( char ); };
    // Load the object data from a file. Since we are  loading strings, each string can be a different
    // length so what we did is use the object data header to hold the size of the string plus the terminator.
    virtual BOOL LoadObjectData( HANDLE hFile, int Index );

public:

    // Constructor
    TStringList();
    // Destructor - Put all your cleanup code here
    virtual ~TStringList();
    // Add a string
    virtual BOOL Add( LPSTR t );
    // Returns a pointer to a string at the given index
    LPSTR GetString( int Index );
    // Frees the pointer from the object class
    void Free();
    // Get's all the file names in a given directory and file extension
    void GetAllFileNamesInDir( char *Dir, char *Ext );
	// Get's all the folder names in a given directory
	void GetAllFoldersInDir( char *Dir, BOOL ExploreSubDir = FALSE );
    // Sort all the items in the list
    void Sort();
	// Sort all the items in the list by file name
	void SortByFileName();
    // Dummy function. Not used
    virtual BOOL Add(){ return FALSE; };
};


                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "TStrList.h"        // Header file for this *.cpp file.
#include <stdlib.h>          // Declares several commonly used routines such as atoi, random, abs, malloc etc.
#include "minTools.h"        // Minimum misc tools


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
template <class obj_type>
TStringList::TStringList()
{

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
template <class obj_type>
TStringList::~TStringList()
{
    //FreeStr();
                
}   // Destructor



/************************************************************************
*    FUNCTION NAME:         Add                                                             
*
*    DESCRIPTION:           Allocates more memory and copies the string.
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
template <class obj_type>
BOOL TStringList::Add( LPSTR t )
{
    LPSTR TempStr;

    // Return if there has been an allocation error
    if( fAllocationError )
        return FALSE;

    // Allocate the string to the size we need
    TempStr = new char[strlen(t)+1];

    // Check the allocation and copy the new string
    if( TempStr )
    {
        strcpy( TempStr, t );

        // Add the string to the object list
        AddObj( (LPVOID)TempStr );
    }
    else
        fAllocationError = TRUE;

    return (fAllocationError == FALSE);

}   // Add



/************************************************************************
*    FUNCTION NAME:         GetString                                                             
*
*    DESCRIPTION:           Returns a pointer to the string at the index provided
*
*    FUNCTION PARAMETERS:
*    Input:    int Index         - Index of the requested string
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
template <class obj_type>
LPSTR TStringList::GetString( int Index )
{
    return (LPSTR)GetObj(Index);

}   // GetString


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
template <class obj_type>
void TStringList::Free()
{
    //FreeStr();

    // Free the pointer array
    FreeObj();
}



/************************************************************************
*    FUNCTION NAME:         Sort                                                             
*
*    DESCRIPTION:           Sort all the items in the list
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
template <class obj_type>
void TStringList::Sort()
{
    int offset, j, x=0;
    BOOL inOrder;
    LPSTR ppStrTemp;

    if( Count() > 1 )
    {
        offset = Count();

        inOrder = FALSE;

        while( x < offset && inOrder == FALSE )
        {
            ++x;
            inOrder = TRUE;
            for( j = 0; j < offset - x; ++j )
            {
                if( strcmpi( (LPSTR)GetObj(j), (LPSTR)GetObj(j+1) ) > 0 ) // Non-case sensitive string compare
                {
                    inOrder = FALSE;
                    ppStrTemp = (LPSTR)GetObj(j);
                    SetObj( GetObj(j+1), j );
                    SetObj( (LPVOID)ppStrTemp, j+1 );
                }
            }
        }
    }

}   // Sort


/************************************************************************
*    FUNCTION NAME:         SortByFileName                                                             
*
*    DESCRIPTION:           Sort all the items in the list by file name
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
template <class obj_type>
void TStringList::SortByFileName()
{
    int offset, j, x=0, result;
    BOOL inOrder;
    LPSTR ppStrTemp;
    char fileName[2][MAX_PATH+1];

    if( Count() )
    {
        offset = Count();
        inOrder = FALSE;

        while( x < offset && inOrder == FALSE )
        {
            ++x;
            inOrder = TRUE;
            for( j = 0; j < offset - x; ++j )
            {
                // Parse out the file names
                CropFileNameFromPath( (LPSTR)GetObj(j), fileName[0] );
                CropFileNameFromPath( (LPSTR)GetObj(j+1), fileName[1] );
                
                // Non-case sensitive string compare
                result = strcmpi( fileName[0], fileName[1] );
                 
                if( result > 0 )
                {
                    inOrder = FALSE;
                    ppStrTemp = (LPSTR)GetObj(j);
                    SetObj( GetObj(j+1), j );
                    SetObj( (LPVOID)ppStrTemp, j+1 );
                }
                // If we have the same file names because they are 
                // in different directories compare the whole path
                else if( result == 0 )
                {
                    if( strcmpi( (LPSTR)GetObj(j), (LPSTR)GetObj(j+1) ) > 0 )
                    {
                        inOrder = FALSE;
                        ppStrTemp = (LPSTR)GetObj(j);
                        SetObj( GetObj(j+1), j );
                        SetObj( (LPVOID)ppStrTemp, j+1 );
                    }
                }
            }
        }
    }

}   // SortByFileName


/************************************************************************
*    FUNCTION NAME:         GetAllFileNamesInDir                                                             
*
*    DESCRIPTION:           Gets all the file names via a suplied 
*                           extension, in a give directory
*
*    FUNCTION PARAMETERS:
*    Input:    PTStringList pStrList - Pointer to a string list object
*                                      to fill up with strings
*              char *Dir - Directory to look in
*              char *Ext - Extension to wild cards to use like *.* or *.wav 
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
template <class obj_type>
void TStringList::GetAllFileNamesInDir( char *Dir, char *Ext )
{
    WIN32_FIND_DATA FindData;
    char StrBuffer[MAX_PATH+1];
    HANDLE hSearch;

    // Make sure none of the string params are empty or NULL
    if( Dir && Dir[0] && Ext && Ext[0] )
    {
        // Init the search string
        strcpy( StrBuffer, Dir );
        strcat( StrBuffer, "\\" );
        strcat( StrBuffer, Ext );

        // Find the first file
        hSearch = FindFirstFile( StrBuffer, &FindData );

        if( hSearch != INVALID_HANDLE_VALUE )
        {
            do
            {
                // Just be sure not to copy any directory names
                if( !(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
                {
                    // Add the file and it's path to the string list
                    strcpy( StrBuffer, Dir );
                    strcat( StrBuffer, "\\" );
                    strcat( StrBuffer, FindData.cFileName );
                    Add( StrBuffer );
                }
            }
            while( FindNextFile( hSearch, &FindData ) );

            FindClose( hSearch );
        }
    }

}   // GetAllFileNamesInDir


/************************************************************************
*    FUNCTION NAME:         GetAllFoldersInDir                                                             
*
*    DESCRIPTION:           Gets all the folder names 
*                           in a give directory
*
*    FUNCTION PARAMETERS:
*    Input:    char *Ext - Extension to wild cards to use like *.* or *.wav 
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
template <class obj_type>
void TStringList::GetAllFoldersInDir( char *Dir, BOOL ExploreSubDir )
{
    WIN32_FIND_DATA FindData;
	char StrBuffer[MAX_PATH+1];
    HANDLE hSearch;

    // Init the search string
    wsprintf( StrBuffer, "%s\\*.*", Dir );

    // Find the first file
    hSearch = FindFirstFile( StrBuffer, &FindData );

    if( hSearch != INVALID_HANDLE_VALUE )
    {
        do
        {
            // Just be sure not to copy any directory that are named . or ..
            if( (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
                FindData.cFileName[0] != '.' )
            {
                // Add the file and it's path to the string list
                strcpy( StrBuffer, Dir );
                strcat( StrBuffer, "\\" );
                strcat( StrBuffer, FindData.cFileName );
                Add( StrBuffer );

                // Have the function call itself for sub folders
                if( ExploreSubDir )
                    GetAllFoldersInDir( StrBuffer, ExploreSubDir ); 
            }
        }
        while( FindNextFile( hSearch, &FindData ) );

        FindClose( hSearch );
    }

}   // GetAllFileNamesInDir


/************************************************************************
*    FUNCTION NAME:         LoadObjectData                                                             
*
*    DESCRIPTION:           Load the object data from a file. Since we are
*                           loading strings, each string can be a different
*                           length so what we did is use the object data header to
*                           hold the size of the string plus the terminator.
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
template <class obj_type>
BOOL TStringList::LoadObjectData( HANDLE hFile, int Index )
{
    BOOL Result = FALSE;
    BOOL FileReadResult;
    unsigned long bytesRead;
    char *TmpString=NULL;

    // This has to be a size. the value of ObjCheckHeader is read in from file
    // in function TObjList::LoadFromFile( HANDLE hFile )
    if( ObjCheckHeader )
    {
        // Allocate space to hold the tempoary string
        // We used the ObjCheckHeader to hold the size of the string because each
        // string will be a different size. Normally the ObjCheckHeader is a safty check
        // that we are not reading ramdom data but for this case we need it for size.
        TmpString = new char[ ObjCheckHeader ];

        // Always check allocations
        if( !TmpString )
            goto HANDLE_ERROR;

        // Read in the data
        FileReadResult = ReadFile( hFile, TmpString, ObjCheckHeader, &bytesRead, NULL );
        if( !FileReadResult || bytesRead != ObjCheckHeader )
            goto HANDLE_ERROR;

        // Load the string
        if( !Add( TmpString ) )
            goto HANDLE_ERROR;

        Result = TRUE;
    }

HANDLE_ERROR:;

    if( TmpString )
        delete [] TmpString;

    return Result;

}   // LoadObjectData


#endif  /* __STRINGLIST__ */