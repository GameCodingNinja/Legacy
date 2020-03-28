
/************************************************************************
*
*    PRODUCT:         Int List
*
*    FILE NAME:       TIntList.cpp
*
*    DESCRIPTION:     Object for holding lists of ints. Inherited from
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

#include "TIntList.h"        // Header file for this *.cpp file.
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

TIntList::TIntList()
{

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

TIntList::~TIntList()
{
    //FreeInt();
                
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

void TIntList::Add( int Value )
{
    int *TempInt;

    // Return if there has been an allocation error
    if( fAllocationError )
        return;

    // Allocate the int
    TempInt = new int;

    // Check the allocation
    if( TempInt )
    {
        *TempInt = Value;

        // Add the string to the object list
        AddObj( (LPVOID)TempInt );
    }
    else
        fAllocationError = TRUE;

}   // Add


/************************************************************************
*    FUNCTION NAME:         GetInt                                                             
*
*    DESCRIPTION:           Returns a pointer to the string at the index provided
*
*    FUNCTION PARAMETERS:
*    Input:    int Index         - Index of the requested string
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

int TIntList::GetInt( int Index )
{
    if( Index < count )
        return *(int *)GetObj(Index);
    else
        return 0;

}   // GetString


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

void TIntList::Free()
{
    //FreeInt();

    // Free the pointer array
    FreeObj();

}   // Free



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

void TIntList::Sort()
{
    int offset, j, x=0;
    BOOL inOrder;
    int *ppIntTemp;

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
                if( *(int *)GetObj(j) > *(int *)GetObj(j+1)  )
                {
                    inOrder = FALSE;
                    ppIntTemp = (int *)GetObj(j);
                    SetObj( GetObj(j+1), j );
                    SetObj( (int *)ppIntTemp, j+1 );
                }
            }
        }
    }

}   // Sort



/************************************************************************
*    FUNCTION NAME:         Delete                                                             
*
*    DESCRIPTION:           Delete the int
*
*    FUNCTION PARAMETERS:
*    Input:    int Index         - Index of the requested string
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

/*void TIntList::Delete( int Index )
{
    // Delete the allocated string
    delete (int *)GetObj(Index);
    
    // Delete the object pointer
    TObjList::Delete( Index );

}   // Delete */



/************************************************************************
*    FUNCTION NAME:         FreeInt                                                             
*
*    DESCRIPTION:           Free up this classes memory
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

/*void TIntList::FreeInt()
{
    WORD i;

    // Delete all the allocated strings
    for( i = 0; i < Count(); ++i )
        delete (int *)GetObj(i);
            
}   // FreeStr  */