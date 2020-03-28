//
// File name: Point3D.CPP
//
// Description: The CPP file for the Point3D.h header file
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

// Global include files:
#include <Math.H>

// Local include files:
#include "Point3D.h"



/************************************************************************
*    FUNCTION NAME:         Read                                                    
*
*    DESCRIPTION:           
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

void Point3D::Read( FILE *File )
{
    fread ( &Lx, sizeof Lx, 1, File );
    fread ( &Ly, sizeof Ly, 1, File );
    fread ( &Lz, sizeof Lz, 1, File );

	Wx = Lx;
	Wy = Ly;
	Wz = Lz;

}   // Read


/************************************************************************
*    FUNCTION NAME:         LoadFromPointer                                                    
*
*    DESCRIPTION:           Load the 3D point from a pointer
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

unsigned char *Point3D::LoadFromPointer( unsigned char *pData )
{
    PPointLoad pPointLoad;

    pPointLoad = PPointLoad( pData );

    Wx = Lx = pPointLoad->Lx;
    Wy = Ly = pPointLoad->Ly;
    Wz = Lz = pPointLoad->Lz;

    // Inc Pointer
    pData += sizeof( PointLoad );

	return pData;

}   // LoadFromPointer


/************************************************************************
*    FUNCTION NAME:         Write                                                    
*
*    DESCRIPTION:           
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

void Point3D::Write( FILE *File )
{
    fwrite ( &Lx, sizeof Lx, 1, File );
    fwrite ( &Ly, sizeof Ly, 1, File );
    fwrite ( &Lz, sizeof Lz, 1, File );
}


/************************************************************************
*    FUNCTION NAME:         UniqueVert                                                    
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

int UniqueVert( Point3D &V, Point3D *List, int Range )
{
    // Loop through list of vertices:
    for ( int Count = 0; Count < Range; ++Count )
    {
        // If it's not unique, return false:
        if ( V == List [ Count ] )
            return 0;
    }

    // Return true (it's unique):
    return 1;
}


/************************************************************************
*    FUNCTION NAME:         UniqueVert                                                    
*
*    DESCRIPTION:           Function designed to search through List in an attempt
*                           to locate the index of a vertex that matches V
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

int GetVertIndex( Point3D &V, Point3D *List, int Range )
{
    // Loop through the list of vertices:
    for( int Count = 0; Count < Range; ++Count )
    {
        // If the vertex matches, return the index:
        if( V == List [ Count ] )
            return Count;
    }

    // Return zero as default - Note: this code should
    // never be reached.
    return 0;
}
