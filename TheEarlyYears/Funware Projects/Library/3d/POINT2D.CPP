//
// File name: Poin2D.CPP
//
// Description: Support file for the Point2D.h header file
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

#include "Point2D.h"


/************************************************************************
*    FUNCTION NAME:         UniqueVert()                                                             
*
*    DESCRIPTION:           Function designed to search through List in an 
*                           attempt to determine uniqueness of vertex V
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
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int UniqueVert( Point2D &V, Point2D *List, int Range )
{
    // Loop through list of vertices:
    for( int Count = 0; Count < Range; ++Count )
    {
        // If it's not unique, return false:
        if( V == List[ Count ] )
            return 0;
    }

    // Return true (it's unique):
    return 1;
}



/************************************************************************
*    FUNCTION NAME:         UniqueVert()                                                             
*
*    DESCRIPTION:           Function designed to search through List in an 
*                           attempt to locate the index of a vertex that
*                           matches V
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
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int GetVertIndex( Point2D &V, Point2D *List, int Range )
{
    // Loop through the list of vertices:
    for( int Count = 0; Count < Range; ++Count )
    {
        // If the vertex matches, return the index:
        if( V == List[ Count ] )
            return Count;
    }

    // Return zero as default - Note: this code should
    // never be reached.
    return 0;
}