
/************************************************************************
*
*    PRODUCT:         String List
*
*    FILE NAME:       TStrList.cpp
*
*    DESCRIPTION:     Object for holding lists of strings. Inherited from
*                     the object list class
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
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "CStrList.h"        // Header file for this *.cpp file.
#include <stdlib.h>          // Declares several commonly used routines such as atoi, random, abs, malloc etc.
#include "minTools.h"        // Minimum misc tools



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
void CStringLst<obj_type>::Sort()
{
    int offset, j, x=0;
    bool inOrder;
    char *ppStrTemp;

    if( Count() > 1 )
    {
        offset = Count();

        inOrder = false;

        while( x < offset && inOrder == FALSE )
        {
            ++x;
            inOrder = TRUE;
            for( j = 0; j < offset - x; ++j )
            {
                if( strcmpi( Get(j), Get(j+1) ) > 0 ) // Non-case sensitive string compare
                {
                    inOrder = false;
                    ppStrTemp = Get(j);
                    Set( j, Get(j+1) );
                    Set( j+1, ppStrTemp );
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
void CStringLst<obj_type>::SortByFileName()
{
    int offset, j, x=0, result;
    bool inOrder;
    char *ppStrTemp;
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
                CropFileNameFromPath( Get(j), fileName[0] );
                CropFileNameFromPath( Get(j+1), fileName[1] );
                
                // Non-case sensitive string compare
                result = strcmpi( fileName[0], fileName[1] );
                 
                if( result > 0 )
                {
                    inOrder = false;
                    ppStrTemp = Get(j);
                    Set( j, Get(j+1) );
                    Set( j+1, ppStrTemp );
                }
                // If we have the same file names because they are 
                // in different directories compare the whole path
                else if( result == 0 )
                {
                    if( strcmpi( Get(j), Get(j+1) ) > 0 )
                    {
                        inOrder = false;
                        ppStrTemp = Get(j);
                        Set( j, Get(j+1) );
                        Set( j+1, ppStrTemp );
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
void CStringLst<obj_type>::GetAllFileNamesInDir( char *dir, char *ext )
{
    WIN32_FIND_DATA findData;
    char strBuffer[MAX_PATH*2], *pPath;
    HANDLE hSearch;

    // Make sure none of the string params are empty or NULL
    if( dir && dir[0] && ext && ext[0] )
    {
        // Init the search string
        strcpy( strBuffer, dir );
        strcat( strBuffer, "\\" );
        strcat( strBuffer, ext );

        // Find the first file
        hSearch = FindFirstFile( strBuffer, &findData );

        if( hSearch != INVALID_HANDLE_VALUE )
        {
            do
            {
                // Just be sure not to copy any directory names
                if( !(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
                {
                    // Add the file and it's path to the string list
                    strcpy( strBuffer, dir );
                    strcat( strBuffer, "\\" );
                    strcat( strBuffer, findData.cFileName );

                    pPath = new char[ strlen(strBuffer)+1 ];
                    strcpy( pPath, strBuffer );
                    Add( pPath );
                }
            }
            while( FindNextFile( hSearch, &findData ) );

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
void CStringLst<obj_type>::GetAllFoldersInDir( char *dir, bool exploreSubDir )
{
    WIN32_FIND_DATA findData;
    char strBuffer[MAX_PATH+1], *pPath;
    HANDLE hSearch;

    // Init the search string
    wsprintf( strBuffer, "%s\\*.*", dir );

    // Find the first file
    hSearch = FindFirstFile( strBuffer, &findData );

    if( hSearch != INVALID_HANDLE_VALUE )
    {
        do
        {
            // Just be sure not to copy any directory that are named . or ..
            if( (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                findData.cFileName[0] != '.' )
            {
                // Add the file and it's path to the string list
                strcpy( strBuffer, dir );
                strcat( strBuffer, "\\" );
                strcat( strBuffer, findData.cFileName );
                pPath = new char[ strlen(strBuffer)+1 ];
                strcpy( pPath, strBuffer );
                Add( pPath );

                // Have the function call itself for sub folders
                if( exploreSubDir )
                {
                    GetAllFoldersInDir( strBuffer, exploreSubDir );
                } 
            }
        }
        while( FindNextFile( hSearch, &findData ) );

        FindClose( hSearch );
    }

}   // GetAllFileNamesInDir