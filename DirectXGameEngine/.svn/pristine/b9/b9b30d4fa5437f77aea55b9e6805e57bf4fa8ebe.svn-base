/************************************************************************
*    FILE NAME:       edge.h
*
*    DESCRIPTION:     Edge class
************************************************************************/  

#ifndef __edge_h__
#define __edge_h__

// Game lib dependencies
#include <common/worldpoint.h>
#include <common/normal.h>

// Forward dependencies
class CCollisionVertex;

class CEdge
{
public:

    // Constructor
    CEdge();
    CEdge(CCollisionVertex * pVert0, CCollisionVertex * pVert1);

    // Calculate the edge normal
    void CalculateNormal();
                                                            
    // Get the edge center point
    CWorldPoint GetCenter() const;

    // Two vertices making up the edge
    CCollisionVertex * pVert[2];

    // Normal of the edge
    CNormal normal;

    // Whether or not the edge is unique
    bool unique;

};

#endif  // __edge_h__
