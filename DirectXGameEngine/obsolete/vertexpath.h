/************************************************************************
*    FILE NAME:       vertexpath.h
*
*    DESCRIPTION:     Class to hold vertex path information for collision
************************************************************************/

#ifndef __vertex_path_h__
#define __vertex_path_h__

// Standard lib dependencies
#include <vector>
#include <set>

// Game lib dependencies
#include <common/worldpoint.h>
#include <common/normal.h>

// Forward declarations
class CCollisionSprite2D;
class CEdge;

class CVertexPath
{
public:

    CVertexPath();

    // Set and get the pre or post position of the vertex
    void SetPrePos( const CWorldPoint & pos );
    void SetPostPos( const CWorldPoint & pos );
    const CWorldPoint & GetPrePos() const;
    const CWorldPoint & GetPostPos() const;

    // Set and get the moving vert's normal
    void SetVertNormal( const CNormal & normal );
    const CNormal & GetVertNormal() const;

    // Increment the pre or post position of the vertex
    void IncPrePos( const CPoint & inc );
    void IncPostPos( const CPoint & inc );

    // Calculate the vert path vector
    void CalculatePathVector();

    // Get the vert path vector
    const CPoint & GetPathVector() const;

    // Add an edge to the edge list
    void AddEdge( CEdge * edge );

    // Get an edge in the edge list
    const CEdge * GetEdge( int i ) const;

    // Get the number of edges in the edge list
    unsigned int GetEdgeCount() const;

    // Clear the edge list
    void ClearEdgeList();

    // Get the offset
    const CPoint & GetOffsetVector() const;

    // Calculate the offset using the edge list
    void CalculateOffset();

protected:

    // The pre-transformation position of the vertex
    CWorldPoint prePos;

    // The post-transformation position of the vertex
    CWorldPoint postPos;

    // The edges that the vertex can possibly collide with
    std::vector<CEdge *> pEdgeList;

    // The offset to move the vertex outside of another sprite
    CPoint offsetVector;

    // Edge chosen as the intersected edge
    CPoint pathVector;

    // The normal of the moving vertex
    CNormal vnorm;

};

#endif  // __vertex_path_h__