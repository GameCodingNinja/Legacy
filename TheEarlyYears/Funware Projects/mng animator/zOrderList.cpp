
/************************************************************************
*
*    PRODUCT:         zOrder list
*
*    FILE NAME:       zOrderList.cpp
*
*    DESCRIPTION:     Object for holding lists of indexes and z orders
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

#include "zOrderList.h"      // Header file for this *.cpp file.
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

CzOrderList::CzOrderList()
{
    // Set the format ID for this object so that we can tell
    // when we are trying to load a file that is not ours.
    ObjFileHeader.ObjForamtID = 0x001DFD91;

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

CzOrderList::~CzOrderList()
{
                
}   // Destructor


/************************************************************************
*    FUNCTION NAME:         Add                                                             
*
*    DESCRIPTION:           Allocates more memory and copies the int.
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

BOOL CzOrderList::Add( int value )
{
    PSZOrder temp;

    // Return if there has been an allocation error
    if( fAllocationError )
        return FALSE;

    // Allocate the string to the size we need
    temp = new SZOrder;

    // Check the allocation and copy the new string
    if( temp )
    {
        // Init the allocated structure
        temp->index = value;

        // Add the string to the object list
        AddObj( (LPVOID)temp );
    }
    else
        fAllocationError = TRUE;

    return (fAllocationError == FALSE);

}   // Add


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

void CzOrderList::Free()
{
    // Free everything
    FreeObj();

}   // Free


/************************************************************************
*    FUNCTION NAME:         SortByZorder                                                             
*
*    DESCRIPTION:           Sort all the items in the list by the Z Order value
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

void CzOrderList::SortByZorder()
{
    WORD offset, j, x=0;
    BOOL inOrder;
    LPVOID lpvoid;

    // Make sure we have something to sort
    if( Count() > 1 )
    {
        inOrder = FALSE;
        offset = Count();

        while( x < offset && inOrder == FALSE )
        {
            ++x;
            inOrder = TRUE;
            for( j = 0; j < offset - x; ++j )
            {
                if( PSZOrder( GetObj(j) )->zOrder > PSZOrder( GetObj(j+1) )->zOrder  )
                {
                    inOrder = FALSE;
                    lpvoid = GetObj(j);
                    SetObj( GetObj(j+1), j );
                    SetObj( lpvoid, j+1 );
                }
            }
        }
    }

}   // SortByZorder