
/************************************************************************
*
*    PRODUCT:         Lists of Art 4 Sale List
*
*    FILE NAME:       TPageList.cpp
*
*    DESCRIPTION:     Object for holding lists of Art 4 Sale pages.
*                     Inherited from the object list class
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

#include "PageList.h"        // Header file for this *.cpp file.
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

TPageList::TPageList()
{
    // Init class members
    CurrentPage = 0;

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

TPageList::~TPageList()
{
    //FreePages();
                
}   // Destructor



/************************************************************************
*    FUNCTION NAME:         Add                                                             
*
*    DESCRIPTION:           Allocates more memory and copies the page.
*
*    FUNCTION PARAMETERS:
*    Input:    char *t           - Page to be added
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

BOOL TPageList::Add()
{
    PTPage TempPage;

    // Return if there has been an allocation error
    if( fAllocationError )
        return FALSE;

    // Allocate the page to the size we need
    TempPage = new TPage;

    // Check the allocation and copy the new page
    if( TempPage )
    {
		// Zero out the page
		memset( TempPage, 0, sizeof(TPage) );

        // Set the page check ID to check if data is valid
        TempPage->PageCheckID = PAGE_CHECK_ID;

        // Add the page to the object list
        AddObj( (LPVOID)TempPage );
    }
    else
        fAllocationError = TRUE;

    return (fAllocationError == FALSE);

}   // Add



/************************************************************************
*    FUNCTION NAME:         GetPage                                                             
*
*    DESCRIPTION:           Returns a pointer to the page at the index provided
*
*    FUNCTION PARAMETERS:
*    Input:    int Index         - Index of the requested page
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

PTPage TPageList::GetPage( WORD Index )
{
    if( Index < Count() )
        return (PTPage)GetObj( Index );
    else
        return (PTPage)GetObj( Count()-1 );

}   // GetPage


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

void TPageList::Free()
{
    //FreePages();
    CurrentPage = 0;

    // Free the pointer array
    FreeObj();
}


/************************************************************************
*    FUNCTION NAME:         Delete                                                             
*
*    DESCRIPTION:           Delete the page
*
*    FUNCTION PARAMETERS:
*    Input:    int Index         - Index of the requested page
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

/*#ifdef SS2GO_TOOL
void TPageList::Delete( int Index )
{
    if( Count() )
    {
        // Delete the allocated page
        delete (PTPage)GetObj(Index);
        
        // Delete the object pointer
        TObjList::Delete( Index );
    }
}
#endif */


/************************************************************************
*    FUNCTION NAME:         Free                                                             
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

/*void TPageList::FreePages()
{
    WORD i;

    // Delete all the allocated pagess
    for( i = 0; i < Count(); ++i )
        delete (PTPage)GetObj(i);

    CurrentPage = 0;
            
}   // FreeStr   */