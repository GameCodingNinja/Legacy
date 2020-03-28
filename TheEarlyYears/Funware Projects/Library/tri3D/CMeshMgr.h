
/************************************************************************
*
*    PRODUCT:         3D Library program
*
*    FILE NAME:       CMeshMgr.h
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

#ifndef _CMESH_MGR_H_
#define _CMESH_MGR_H_

class CMeshTri;
class CMatrix;

#define MAX_CELLS        200

enum EMeshType
{
	EMT_ROOM=0,
	EMT_HALL,
};


/************************************************************************
*
*    Class for taking the mesh positions and detemining which meshes
*    should be processed to the screen
*
************************************************************************/

class CMeshMgr
{
protected:

    CMeshTri *pCell_Cull[MAX_CELLS];
    CMeshTri *pCell_Sort[MAX_CELLS];
    CMeshTri *pCell_PtCollide[MAX_CELLS];
    CMeshTri *pCell_Final[MAX_CELLS];
    unsigned int totalCount;
    unsigned int sortCount;
    unsigned int finalCount;
	unsigned int gameLoopCounter;

public:

    CMeshMgr();
    ~CMeshMgr(){ Free(); }

    // Init the class with the mesh objects
    void SetCell( CMeshTri *pCellObj );
    // sort meshes to determine which ones should be seen.
    void Sort();
    // Free the allocations
    void Free();
    // Get the final count
    unsigned int GetCount(){ return finalCount; };
    // Get the mesh index
    unsigned int GetIndex( unsigned int index );
    // Cull the objects vis a bounding sphere
    void CullBoundingSpheres( CMatrix &m );
    // Render the cells we can see
    void RenderCells( CMatrix &m, int &polyCount, int &objCount );
};


#endif  // _CMESH_MGR_H_