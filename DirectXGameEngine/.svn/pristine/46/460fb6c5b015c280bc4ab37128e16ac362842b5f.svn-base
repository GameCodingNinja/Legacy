
/************************************************************************
*    FILE NAME:       face2d.h
*
*    DESCRIPTION:     2D face class
************************************************************************/  

#ifndef __face_2d_h__
#define __face_2d_h__

// Game lib dependencies
#include <common/edge.h>
#include <common/collisionvertex.h>

class CFace2D
{
public:

    // Constructors
    CFace2D():radius(0),sqrtRadius(0){}

    CFace2D( CCollisionVertex * v0, CCollisionVertex * v1, CCollisionVertex * v2 ):radius(0),sqrtRadius(0)
    {
        edge[0] = CEdge(v0, v1);
        edge[1] = CEdge(v1, v2);
        edge[2] = CEdge(v2, v0);

        CalculateCenter();
        CalculateRadii();
    }


    /************************************************************************                                                             
    *    desc:  Get a specific vertex
    *
    *	 param: int i - index into the edge array
    *
    *	 ret:	CPoint & - returned vertex
    ************************************************************************/
    CCollisionVertex * GetVert( int i )
    {
        return edge[i].pVert[0];
    }


    /************************************************************************                                                             
    *    desc:  Cacluate the center of the face
    ************************************************************************/
    void CalculateCenter()
    {
        // Get the positions of the vertices relative to the first one. That way
        // we lose no precision
        CPoint tmp[2];
        tmp[0] = edge[1].pVert[0]->GetPos() - edge[0].pVert[0]->GetPos();
        tmp[1] = edge[2].pVert[0]->GetPos() - edge[0].pVert[0]->GetPos();

        CPoint offset = (tmp[0] + tmp[1]) / 3;
        center = edge[0].pVert[0]->GetPos() + offset;
         
    }	// CalculateCenter */


    /************************************************************************                                                             
    *    desc:  Cacluate the radii of the face
    ************************************************************************/
    void CalculateRadii()
    {
        float tmpRadius;
        CPoint tmpDist;

        for( int i = 0; i < 3; ++i )
        {
            tmpRadius = ( center - edge[i].pVert[0]->GetPos() ).GetLengthSquared();

            if( tmpRadius > radius )
                radius = tmpRadius;
        }

        sqrtRadius = sqrt(radius);

    }	// CalculateRadii */


    /************************************************************************                                                             
    *    desc:  Sets the second vertex of each edge equal to its corresponding
    *			first vertex of a different edge. Call this function after
    *			performing a matrix transformation on the first vertex of
    *			each point. 
    ************************************************************************/
    void ConnectEdges()
    {
        edge[0].pVert[1] = edge[1].pVert[0];
        edge[1].pVert[1] = edge[2].pVert[0];
        edge[2].pVert[1] = edge[0].pVert[0];
    
    }	// ConnectEdges */

    // Vertex indexes
    int vertIndex[3];

    // Edges
    CEdge edge[3];

    // Center point
    CWorldPoint center;

    // Radii
    float radius, sqrtRadius;
};

#endif  // __face_2d_h__

