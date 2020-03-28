
/************************************************************************
*
*    PRODUCT:         Int List
*
*    FILE NAME:       TIntList.h
*
*    DESCRIPTION:     Object for holding lists of ints
*
*    IMPLEMENTATION:  
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __INT_LIST_H__
#define __INT_LIST_H__ 

#include "TObjList.h"        // Base class header file


class TIntList : public TObjList
{

public:
    // Common Functions

    // Constructor
    TIntList();
    // Destructor - Put all your cleanup code here
    ~TIntList();

    // Get the size of the data
    virtual int GetObjSize( int Index ){ return sizeof(int); };
    // Dummy function. Not used
    virtual BOOL Add(){ return FALSE; };

    // Add a int
    void Add( int Value );
    // Returns the value of the int at the given index
    int GetInt( int Index );
    // Frees the pointer from the object class
    void Free();
    // Sort all the items in the list
    void Sort();


    // Free up this classes memory
    //void FreeInt();
    // Delete string
    //virtual void Delete( int Index );
};

typedef TIntList *PTIntList;


#endif  /* __INT_LIST__ */