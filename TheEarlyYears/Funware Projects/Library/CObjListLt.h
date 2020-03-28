
/************************************************************************
*
*    PRODUCT:         Object List Lite
*
*    FILE NAME:       TObjListLt.h
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

#ifndef __OBJECTLIST_LITE_H__
#define __OBJECTLIST_LITE_H__ 

typedef void *PVOID;

class CObjList
{
protected:
    // Number of current objects
    int count;
    // For allocating an array of voided pointers
    void **ppPointer;
    // If it is set to TRUE, then all allocations stop
    bool fAllocationError;
	// Flag to indicate the data is not to be freed
	bool freeData;

public:
    // Common Functions

    // Constructor
    CObjList();
    // Destructor - Put all your cleanup code here
    ~CObjList();
    // Add a nother pointer
    bool AddObj( PVOID obj );
    // Delete pointer to the object at the given index
    void Delete( int index );
    // Return the number of objects
    int Count(){ return count; };
    // Free up all the memory
    void FreeObj();
    // Returns a pointer to the object at the given index.
    PVOID GetObj( int index );
    // Sets an object pointer to the object array
    void SetObj( int index, PVOID t ){ if( index > -1 && index < count )ppPointer[index] = t; };
    // Moves an index to a new position
    void MoveToNewPos( int CurrentIndex, int NewIndex );
	// Indicate we don't want to free the data
	void DontFreeData(){ freeData = false; };
};

typedef CObjList *PCObjList;


#endif  /* __OBJLIST__ */