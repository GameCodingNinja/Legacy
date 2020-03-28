
/************************************************************************
*
*    PRODUCT:         Object List Lite
*
*    FILE NAME:       CObjListLt.cpp
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

#include "CObjListLt.h"        // Header file for this *.cpp file.
#include <string.h>			 // needed for memcopy


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

CObjList::CObjList()
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

CObjList::~CObjList()
{
    FreeObj();
                
}   // Destructor


/************************************************************************
*    FUNCTION NAME:         Add                                                             
*
*    DESCRIPTION:           Allocates more memory and copies the objects data.
*
*    FUNCTION PARAMETERS:
*    Input:    PVOID t           - Object to be added
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

bool CObjList::AddObj( PVOID obj )
{
    PVOID *ppTemp;
    bool result = false;

    // Return if there has been an allocation error
    if( fAllocationError )
        goto HANDLE_ERROR;

    // Allocate the buffer to hold the object pointers
    // Copy the pointer to a temp variable for coping the array
    // into the newly allocated array.
    ppTemp = ppPointer;
    ppPointer = new PVOID[count+1];

    // Check that we have a pointer and that we have atleast one
    // pointer
    if( ppPointer )
    {
        if( count )
        {
            memcpy( ppPointer, ppTemp, sizeof(PVOID)*count );
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

PVOID CObjList::GetObj( int index )
{
    PVOID result = NULL;

    // Check that we are not out of bounds before handing out the pointer
    if( index < count )
        result = ppPointer[index];

    return result;

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

void CObjList::FreeObj()
{
    if( ppPointer )
    {
        // Delete all the allocated pointers
        for( int i = 0; i < count && freeData; ++i )
            delete [] GetObj( i );

        // Delete the allocated pointer array
        delete [] ppPointer;

        ppPointer = NULL;
        count = 0;
    }            
}   // Free


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

void CObjList::Delete( int index )
{
    PVOID *ppTemp;
    int i, inc = 0;

    // Return if there has been an allocation error
    if( fAllocationError || !count )
        return;

    if( count-1 > 0 )
    {
        // Delete the allocated object
		if( freeData == true )
		{
        	delete [] GetObj( index );
		}

        // Allocate the buffer to hold the object pointers
        // Copy the pointer to a temp variable for coping the array
        // into the newly allocated array.
        ppTemp = ppPointer;
        ppPointer = new PVOID[count-1];

        // Check that we have a pointer
        if( ppPointer )
        {
            // Copy over all the other valid pointers
            for( i = 0; i < count; ++i )
            {
                if( i != index )
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
            fAllocationError = true;
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

void CObjList::MoveToNewPos( int currentIndex, int newIndex )
{
    PVOID pTemp;

    if( currentIndex < count )
    {
        pTemp = ppPointer[currentIndex];

        if( currentIndex < newIndex )
            memmove( &ppPointer[currentIndex], &ppPointer[currentIndex+1], sizeof(PVOID)*(newIndex-currentIndex) );

        else if( currentIndex > newIndex )
            memmove( &ppPointer[newIndex+1], &ppPointer[newIndex], sizeof(PVOID)*(currentIndex-newIndex) );

        ppPointer[ newIndex ] = pTemp;
    }

}   // MoveToNewPos