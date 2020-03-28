
/************************************************************************
*
*    PRODUCT:         3D Library program
*
*    FILE NAME:       CMeshMgr.cpp
*
*    DESCRIPTION:     Show's how simple dealing with Windows really is 
*
*    IMPLEMENTATION:  In 3D, speed is everything. This object is for managing
*                     all the different mesh classes
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

#include "CMeshTri.h"        // Triangle mesh class
#include "CMatrix.h"       // Matrix class
#include "CMeshMgr.h"        // Header file for this *.cpp file.


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    08/01/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CMeshMgr::CMeshMgr()
{
    // Init class members
    totalCount = 0;
    sortCount = 0;
    finalCount = 0;
	gameLoopCounter = 0;
}


/************************************************************************
*    FUNCTION NAME:         SetCell()                                                             
*
*    DESCRIPTION:           Init the class with the mesh objects
*
*    FUNCTION PARAMETERS:
*    Input:    CMeshTri *pCellCull - Cell/room object
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    08/01/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMeshMgr::SetCell( CMeshTri *pCellObj )
{   
    if( totalCount < MAX_CELLS )
    {
        pCell_Cull[totalCount] = pCellObj;
        ++totalCount;
    }
    
}   // SetCell


/************************************************************************
*    FUNCTION NAME:         CullBoundingSpheres()                                                             
*
*    DESCRIPTION:           Cull the objects vis a bounding sphere
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
*    H Rosenorn    08/01/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMeshMgr::CullBoundingSpheres( CMatrix &m )
{
    int collisionCount = 0;
    CPoint pt;

    // Reset the counters
    sortCount = 0;
    finalCount = 0;

    for( int i = 0; i < totalCount; ++i )
    {
        // Test if the object to see if it is within the view fustrum
        pCell_Cull[i]->CullMesh_BoundSphere( m );

        // If the object was not culled, save it to the sort array
        if( !pCell_Cull[i]->WasCulled() )
        {
            pCell_Sort[sortCount] = pCell_Cull[i];
            ++sortCount;
        }
    }

    // Sort in order the closest to the farthest distances
    Sort();

    // How many sorted POV bounding spheres do we collide with
    for( int i = 0; i < sortCount; ++i )
    {
        if( pCell_Sort[i]->Collide_BoundSphere( pt, 20 ) )
        {
            pCell_PtCollide[collisionCount] = pCell_Sort[i];
            ++collisionCount;

			pCell_Final[finalCount]	= pCell_Sort[i];
			++finalCount;
        }
    }

    if( collisionCount == 0 )
    {
        collisionCount = 1;
        pCell_PtCollide[0] = pCell_Sort[0];

		pCell_Final[finalCount]	= pCell_Sort[0];
		++finalCount;
    }

    // Assume we are in the closest object, see if the bounding spheres
    // collide with our cloest cell
    for( int i = 0; i < collisionCount; ++i )
    {
        for( int j = 0; j < sortCount; ++j )
        {
            if( pCell_PtCollide[i]->GetIndex() != pCell_Sort[j]->GetIndex() &&
			    pCell_PtCollide[i]->GetType() != pCell_Sort[j]->GetType() &&
            	pCell_PtCollide[i]->Collide_BoundSphere( pCell_Sort[j] ) )
            {
                bool alreadyThere = false;

                // Make sure it's not already in there
                for( int w = 0; w < finalCount && !alreadyThere; ++w )
                {
                    if( pCell_Final[w]->GetIndex() == pCell_Sort[j]->GetIndex() )
                    {
                        alreadyThere = true;
                    }
                }

                if( alreadyThere == false )
                {
                    pCell_Final[finalCount] = pCell_Sort[j];
                    ++finalCount;
                    
                    if( pCell_PtCollide[i]->GetType() == EMT_HALL )
					{
						break;
					}					
                }
            }
        }
    }

}   // CullBoundingSpheres


/************************************************************************
*    FUNCTION NAME:         RenderCells()                                                             
*
*    DESCRIPTION:           Render the cells we can see
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
*    H Rosenorn    08/01/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMeshMgr::RenderCells( CMatrix &m, int &polyCount, int &objCount )
{
	if( gameLoopCounter & 1 )
	{
	    // Render the mesh objects
	    for( int i = 0; i < finalCount; ++i )
	    {
	        polyCount += pCell_Final[ i ]->Render( m, 0.55, EZ_ZBUFFER_NEG );

	        ++objCount;
	    }
	}
	else
	{
	    // Render the mesh objects
	    for( int i = 0; i < finalCount; ++i )
	    {
	        polyCount += pCell_Final[ i ]->Render( m, 0.55, EZ_ZBUFFER_POS );

	        ++objCount;
	    }
	}

	// Inc the game loop counter
    ++gameLoopCounter;

}   // RenderCells


/************************************************************************
*    FUNCTION NAME:         Sort()                                                             
*
*    DESCRIPTION:           sort meshes to determine which ones should
*                           be seen.
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
*    H Rosenorn    08/01/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMeshMgr::Sort()
{
    int offset, j, x=0;
    bool inOrder;
    CMeshTri *temp;

    // Make sure we have something to sort
    if( sortCount > 1 )
    {
        inOrder = false;
        offset = sortCount;

        while( x < offset && inOrder == false )
        {
            ++x;
            inOrder = true;
            for( j = 0; j < offset - x; ++j )
            {
                if( (pCell_Sort[j]->GetDistance() - pCell_Sort[j]->GetRadius()) > 
                    (pCell_Sort[j+1]->GetDistance() - pCell_Sort[j+1]->GetRadius()) )
                {
                    inOrder = false;
                    temp = pCell_Sort[j];
                    pCell_Sort[j] = pCell_Sort[j+1];
                    pCell_Sort[j+1] = temp;
                }
            }
        }
    }

}   // Sort


/************************************************************************
*    FUNCTION NAME:         GetIndex()                                                             
*
*    DESCRIPTION:           Get the mesh index
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
*    H Rosenorn    08/01/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

unsigned int CMeshMgr::GetIndex( unsigned int index )
{
    if( index < finalCount )
    {
        return pCell_Final[index]->GetIndex(); 
    }

    return 0;
        
}   // GetIndex


/************************************************************************
*    FUNCTION NAME:         Free()                                                             
*
*    DESCRIPTION:           Free the allocations
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
*    H Rosenorn    08/01/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMeshMgr::Free()
{
    totalCount = 0;
    sortCount = 0;
    finalCount = 0;

}   // Free