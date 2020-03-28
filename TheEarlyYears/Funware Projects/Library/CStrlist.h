
/************************************************************************
*
*    PRODUCT:         String List
*
*    FILE NAME:       CStrList.h
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

#ifndef __CSTRING_LIST_H__
#define __CSTRING_LIST_H__ 

#include "CObjList.h"        // Base class header file

template <class obj_type>
class CStringLst : public CObjLst<obj_type>
{

public:

    // Get's all the file names in a given directory and file extension
    void GetAllFileNamesInDir( char *dir, char *ext );
    // Get's all the folder names in a given directory
    void GetAllFoldersInDir( char *Dir, bool exploreSubDir = false );
    // Sort all the items in the list
    void Sort();
    // Sort all the items in the list by file name
    void SortByFileName();
};


/////////////////////////////////////////
//  Must include this here or you will
//  get a bunch of "Unresolved External"
//  errors. Templates are a pain to code
/////////////////////////////////////////
#include "CStrList.cpp"

#endif  /* __STRINGLIST__ */