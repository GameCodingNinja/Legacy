
/************************************************************************
*
*    PRODUCT:         Object List
*
*    FILE NAME:       CObjList.h
*
*    DESCRIPTION:     Base class for managing lists of pointers. I can't
*                     get the vector class to compile so I wrote my own.
*
*    IMPLEMENTATION:  I was never a fan of linked lists so this class
*                     manages a dynamic array of pointers  
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __COBJECT_LIST_H__
#define __COBJECT_LIST_H__ 


template <class obj_type>
class CObjLst
{
public:
    
    // Common Functions
    // Constructor
    CObjLst();
    // Destructor - Put all your cleanup code here
    ~CObjLst();
    // Add a nother pointer
    bool Add( obj_type * obj );
    // Delete pointer to the object at the given index
    void Delete( int index );
    // Return the number of objects
    int Count(){ return count; };
    // Free up all the memory
    void Free();
    // Mix up all the objects into a random order
    void MixUp();
    // Returns a pointer to the object at the given index.
    obj_type * Get( int index );
    // Sets an object pointer to the object array
    void Set( int index, obj_type * t );
    // Moves an index to a new position
    void MoveToNewPos( int currentIndex, int newIndex );
    // Indicate we don't want to free the data
    void DontFreeData(){ freeData = false; };

protected:
    // define a pointer type
    typedef obj_type * pObj_type;

    // For allocating an array of pointers
    pObj_type *ppPointer;
    // Number of current objects
    int count;
    // If it is set to TRUE, then all allocations stop
    bool fAllocationError;
    // Flag to indicate the data is not to be freed
    bool freeData;
};

/////////////////////////////////////////
//  Must include this here or you will
//  get a bunch of "Unresolved External"
//  errors. Templates are a pain to code
/////////////////////////////////////////
#include "CObjList.cpp" 

#endif  /* __COBJECT_LIST_H__ */