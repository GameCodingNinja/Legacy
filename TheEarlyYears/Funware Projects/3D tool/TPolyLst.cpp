
/************************************************************************
*
*    PRODUCT:         Polygon Object list
*
*    FILE NAME:       TPolyList.cpp
*
*    DESCRIPTION:     Object for holding lists of polygon objects and their
*                     animations.
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

#include "TPolyLst.h"        // Header file for this *.cpp file.
#include <stdlib.h>          // Declares several commonly used routines such as atoi, random, abs, malloc etc.
#include "gtools.h"          // Group of useful functions


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

TPolygonList::TPolygonList()
{
    // Set the format ID for this object so that we can tell
    // when we are trying to load a file that is not ours.
    ObjFileHeader.ObjForamtID = 0xFE421802;

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

TPolygonList::~TPolygonList()
{
    Free();
                    
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

BOOL TPolygonList::Add()
{
    PTPolygon pTemp;

    // Return if there has been an allocation error
    if( fAllocationError )
        return FALSE;

    // Allocate the object
    pTemp = new TPolygon;

    // Check the allocation and copy the new string
    if( pTemp )
    {
        // Add the object to the object list
        AddObj( (LPVOID)pTemp );
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

void TPolygonList::Free()
{
    PTPolygon pTemp;

    // Delete all the allocated class data
    for( int i = 0; i < count; ++i )
    {
        // Free the allocated story boards
        pTemp = (PTPolygon)GetObj( i );

        // Free all the object classes that allocate data
        pTemp->PolyObj.Free();
        pTemp->TextDat.Free();
        pTemp->PolyOffsetList.Free();
        pTemp->TextureLst.Free();
    }

    // Free the pointer array
    FreeObj();

}   // Free


/************************************************************************
*    FUNCTION NAME:         Delete                                                             
*
*    DESCRIPTION:           Since this class allocats anothe object, we
*                           free up that classes data because the destructor
*                           is not called.
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

void TPolygonList::Delete( int Index )
{
    PTPolygon pTemp;

    // Free the allocated story boards
    pTemp = (PTPolygon)GetObj( Index );

    // Free all the object classes that allocate data
    pTemp->PolyObj.Free();
    pTemp->TextDat.Free();
    pTemp->PolyOffsetList.Free();
    pTemp->TextureLst.Free();

    // Let the parent class handle the rest
    TObjList::Delete( Index );

}   // Delete