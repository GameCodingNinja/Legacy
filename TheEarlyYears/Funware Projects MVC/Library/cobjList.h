
/************************************************************************

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

#include <stdlib.h>          // Declares several commonly used routines such as atoi, random, abs, malloc etc. 

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

/************************************************************************
*    DESCRIPTION:         Constructor                                                             
************************************************************************/
template <class obj_type>
CObjLst<obj_type>::CObjLst()
{
    // Init class members
    count = 0;
    ppPointer = NULL;
    fAllocationError = false;
    freeData = true;

}   // Constructor


/************************************************************************
*    DESCRIPTION:  Destructor
************************************************************************/
template <class obj_type>
CObjLst<obj_type>::~CObjLst()
{
    Free();
                
}   // Destructor


/************************************************************************
*    DESCRIPTION: Allocates more memory and adds the pointer
*
*    FUNCTION PARAMETERS:
*    Input:    obj_type * obj    - Object to be added
*              var2type Var2Name - Description 
*
*    Output:   bool
************************************************************************/
template <class obj_type>
bool CObjLst<obj_type>::Add( obj_type * obj )
{
    pObj_type *ppTemp;
    bool result = false;

    // Return if there has been an allocation error
    if( fAllocationError )
    {
        goto HANDLE_ERROR;
    }

    // Allocate the buffer to hold the object pointers
    // Copy the pointer to a temp variable for coping the array
    // into the newly allocated array.
    ppTemp = ppPointer;
    ppPointer = new pObj_type[count+1];

    // Check that we have a pointer and that we have atleast one
    // pointer
    if( ppPointer != NULL )
    {
        if( count > 0 )
        {
            memcpy( ppPointer, ppTemp, sizeof(PVOID) * count );
            delete [] ppTemp;
        }

        // Copy the pointer to the newly allocated spot
        ppPointer[count] = obj;

        ++count;
    }
    else
    {
        fAllocationError = true;
        ppPointer = ppTemp;
        goto HANDLE_ERROR;
    }

    result = true;

    HANDLE_ERROR:;

    return result;

}   // Add


/************************************************************************
*    DESCRIPTION:  Returns a pointer to the pointer at
*                  the given index
*
*    FUNCTION PARAMETERS:
*    Input:    int Index         - Index of the requested object 
*
*    Output:   returntype - Description
************************************************************************/
template <class obj_type>
obj_type * CObjLst<obj_type>::Get( int index )
{
    obj_type * result = NULL;

    // Check that we are not out of bounds before handing out the pointer
    if( index > -1 && index < count )
    {
        result = ppPointer[index];
    }

    return result;

}   // Get


/************************************************************************
*    DESCRIPTION:           Sets the pointer at a given index
*
*    FUNCTION PARAMETERS:
*    Input:    int Index         - Index of the requested object
*              obj_type * t - Description 
*
*    Output:   returntype - Description
************************************************************************/
template <class obj_type>
void CObjLst<obj_type>::Set( int index, obj_type * t )
{
    // Check that we are not out of bounds before setting the pointer
    if( index > -1 && index < count )
    {
        ppPointer[index] = t;
    }
}   // set


/************************************************************************
*    DESCRIPTION:  Free up all allocated resources.
************************************************************************/
template <class obj_type>
void CObjLst<obj_type>::Free()
{
    if( ppPointer != NULL && count != 0 )
    {
        // Delete all the allocated pointers
        for( int i = 0; i < count && freeData; ++i )
        {
            delete Get( i );
        }

        // Delete the allocated pointer array
        delete [] ppPointer;

        ppPointer = NULL;
        count = 0;
    }
                
}   // Free


/************************************************************************
*    DESCRIPTION:  Mix up all the pointer into a random order
************************************************************************/
template <class obj_type>
void CObjLst<obj_type>::MixUp()
{
    // Reset the random number generator
    #ifndef MSVC_COMPILER
    randomize();
    #endif

    // It's no use if we don't have more than 2 items
    if( count > 2 )
    {
        pObj_type ppTemp;
        int mixItUp;

        for( int i = 0; i < count; ++i )
        {
            mixItUp = rand() % count;

            if( i != mixItUp )
            {
                ppTemp = ppPointer[i];
                ppPointer[i] = ppPointer[mixItUp];
                ppPointer[mixItUp] = ppTemp;   
            }
        }
    }

}   // MixUp


/************************************************************************
*    DESCRIPTION:  Delete pointer to the object at the given index
*
*    FUNCTION PARAMETERS:
*    Input:    int index         - Index of the requested pointer
************************************************************************/
template <class obj_type>
void CObjLst<obj_type>::Delete( int index )
{
    // Check that we are not out of bounds or allocation errors
    if( index > -1 && index < count && !fAllocationError )
    {
        // Only delete if we have item to delete
        if( count-1 > 0 )
        {
            // Delete the index in question
            if( freeData == true )
            {
                delete Get( index );
            }

            // Allocate the buffer to hold the object pointers
            // Copy the pointer to a temp variable for coping the array
            // into the newly allocated array.
            pObj_type *ppTemp = ppPointer;
            ppPointer = new pObj_type[count-1];

            // Check that we have a pointer
            if( ppPointer != NULL )
            {
                // We are deleting the first index
                if( index == 0 )
                {
                    memcpy( ppPointer, &ppTemp[index+1], sizeof(PVOID) * (count-1) );
                }
                // We are deleting the last index
                else if( index == count-1 )
                {
                    memcpy( ppPointer, ppTemp, sizeof(PVOID) * (count-1) );
                }
                else
                {
                    // Copy the front half of the array
                    memcpy( ppPointer, ppTemp, sizeof(PVOID) * index );

                    // Copy the back half of the array
                    memcpy( &ppPointer[index], &ppTemp[index+1], sizeof(PVOID) * (count-index-1) );
                }

                // Delete the old pointer array
                delete [] ppTemp;

                --count;
            }
            else
            {
                fAllocationError = true;
                ppPointer = ppTemp;
            }
        }
        else
        {
            // Just free the obj if we are deleting our last pointer
            Free();
        }
    }

}   // Delete


/************************************************************************
*    DESCRIPTION:  Moves an index to a new position
*
*    FUNCTION PARAMETERS:
*    Input:    int Index         - Index of the bject to be moved
*              int NewIndex     - New index position the current index
*                                  is to be moved 
************************************************************************/
template <class obj_type>
void CObjLst<obj_type>::MoveToNewPos( int currentIndex, int newIndex )
{
    obj_type * pTemp;

    // Do a bunch of tests to make sure it's save to shift stuff around
    if( (currentIndex != newIndex) && 
        (currentIndex > -1) &&
        (currentIndex < count) &&
        (newIndex > -1) &&
        (newIndex < count) )
    {
        pTemp = ppPointer[currentIndex];

        if( currentIndex < newIndex )
            memmove( &ppPointer[currentIndex], &ppPointer[currentIndex+1], sizeof(PVOID)*(newIndex-currentIndex) );

        else if( currentIndex > newIndex )
            memmove( &ppPointer[newIndex+1], &ppPointer[newIndex], sizeof(PVOID)*(currentIndex-newIndex) );

        ppPointer[ newIndex ] = pTemp;
    }

}   // MoveToNewPos 

#endif  /* __COBJECT_LIST_H__ */