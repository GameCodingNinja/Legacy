
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

#include "TObjList.h"        // Base class header file


class TStringList : public TObjList
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
    ~TStringList();
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

typedef TStringList *PTStringList;


#endif  /* __STRINGLIST__ */