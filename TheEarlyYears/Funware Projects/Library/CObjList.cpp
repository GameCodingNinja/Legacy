
/************************************************************************
*
*    PRODUCT:         Object List
*
*    FILE NAME:       CObjList.cpp
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

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "CObjList.h"        // Header file for this *.cpp file.
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
CObjLst<obj_type>::~CObjLst()
{
    Free();
                
}   // Destructor


/************************************************************************
*    FUNCTION NAME:         Add                                                             
*
*    DESCRIPTION:           Allocates more memory and adds the pointer
*
*    FUNCTION PARAMETERS:
*    Input:    obj_type * obj    - Object to be added
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
*    FUNCTION NAME:         Get                                                             
*
*    DESCRIPTION:           Returns a pointer to the pointer at
*                           the given index
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
*    FUNCTION NAME:         Set                                                             
*
*    DESCRIPTION:           Sets the pointer at a given index
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
*    FUNCTION NAME:         MixUp                                                             
*
*    DESCRIPTION:           Mix up all the pointer into a random order
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
*    FUNCTION NAME:         Delete                                                             
*
*    DESCRIPTION:           Delete pointer to the object at the given index
*
*    FUNCTION PARAMETERS:
*    Input:    int index         - Index of the requested pointer
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
