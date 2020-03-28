/************************************************************************
*    FILE NAME:       vertexpath.cpp
*
*    DESCRIPTION:     Class to hold vertex path information for collision
************************************************************************/

// Physical component dependency
#include <2d/vertexpath.h>

// Game lib dependencies
#include <common/edge.h>
#include <2d/collisionsprite2d.h>
#include <utilities/collisionfunc2d.h>
#include <utilities/genfunc.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

const float PADDING = 0.1f;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CVertexPath::CVertexPath()
{
}	// Constructor */


/************************************************************************
*    desc:  Set the pre position of the vertex   
*
*	 param:	CWorldPoint & pos - position to set to
************************************************************************/
void CVertexPath::SetPrePos( const CWorldPoint & pos )
{
    prePos = pos;

}	// SetPrePos */


/************************************************************************
*    desc:  Increment the pre position of the vertex   
*
*	 param:	CPoint & pos - position to add
************************************************************************/
void CVertexPath::IncPrePos( const CPoint & inc )
{
    prePos += inc;

}	// IncPrePos */


/************************************************************************
*    desc:  Get the pre position of the vertex   
*
*	 ret:	CWorldPoint & - pre position
************************************************************************/
const CWorldPoint & CVertexPath::GetPrePos() const
{
    return prePos;

}	// GetPrePos */


/************************************************************************
*    desc:  Set the post position of the vertex   
*
*	 param:	CWorldPoint & pos - position to set to
************************************************************************/
void CVertexPath::SetPostPos( const CWorldPoint & pos )
{
    postPos = pos;

}	// SetPostPos */


/************************************************************************
*    desc:  Increment the post position of the vertex   
*
*	 param:	CPoint & pos - position to add
************************************************************************/
void CVertexPath::IncPostPos( const CPoint & inc )
{
    postPos += inc;

}	// IncPostPos */


/************************************************************************
*    desc:  Get the post position of the vertex   
*
*	 ret:	CWorldPoint & - post position
************************************************************************/
const CWorldPoint & CVertexPath::GetPostPos() const
{
    return postPos;

}	// GetPostPos */


/************************************************************************
*    desc:  Set the moving vert's normal
*
*	 param:	CNormal & normal - vertex normal to set to
************************************************************************/
void CVertexPath::SetVertNormal( const CNormal & normal )
{
    vnorm = normal;

}	// SetVertNormal */


/************************************************************************
*    desc:  Get the moving vert's normal
*
*	 ret:	CNormal & - vertex normal
************************************************************************/
const CNormal & CVertexPath::GetVertNormal() const
{
    return vnorm;

}	// GetVertNormal */



/************************************************************************
*    desc:  Calculate the vert path vector
************************************************************************/
void CVertexPath::CalculatePathVector()
{
    //CWorldPoint tmp = postPos - prePos;
    //tmp.ConvertToFloatPoint();

    pathVector = postPos - prePos;

}	// CalculatePathVector */


/************************************************************************
*    desc:  Get the vert path vector
*
*	 ret:	CPoint & - path vector
************************************************************************/
const CPoint & CVertexPath::GetPathVector() const
{
    return pathVector;

}	// GetPathVector */


/************************************************************************
*    desc:  Add an edge to the edge list   
*
*	 param:	CEdge * edge - edge to add
************************************************************************/
void CVertexPath::AddEdge( CEdge * edge )
{
    pEdgeList.push_back( edge );

}	// AddEdge */


/************************************************************************
*    desc:  Get an edge in the edge list   
*
*	 param:	int i - index into the edge list
*
*	 ret:	CEdge * - edge to get
************************************************************************/
const CEdge * CVertexPath::GetEdge( int i ) const
{
    return pEdgeList[i];

}	// GetEdge */


/************************************************************************
*    desc:  Get the number of edges in the edge list
*
*	 ret:	unsigned int - number of edges in edge count
************************************************************************/
unsigned int CVertexPath::GetEdgeCount() const
{
    return pEdgeList.size();

}	// GetEdgeCount */


/************************************************************************
*    desc:  Clear the edge list
************************************************************************/
void CVertexPath::ClearEdgeList()
{
    pEdgeList.clear();

}	// ClearEdgeList */


/************************************************************************
*    desc:  Get the offset
*
*	 ret:	CPoint - offset vector
************************************************************************/
const CPoint & CVertexPath::GetOffsetVector() const
{
    return offsetVector;

}	// GetOffsetVector */


/************************************************************************
*    desc:  Calculate the offset using the edge list
************************************************************************/
void CVertexPath::CalculateOffset()
{
    if( GetEdgeCount() > 0 )
    {
        // The most likely edge to offset against
        CEdge * pIntersectEdge = NULL;

        // Whether or not we've found the edge using the path vector
        bool edgeFound = false;

        // Whether or not our moving vert is inside of the static vert volume we make
        bool inVertVolume = false;

        CalculatePathVector();

        // Sqaured distance between the point of intersection and the initial position of the moving vertex
        float distance = 0;

        for( unsigned int i = 0; i < GetEdgeCount(); i++ )
        {
            if( NCollisionFunc2D::VectorsFacing( vnorm, pEdgeList[i]->normal ) )
            {
                // Check to see if we're piercing the edge
                if( NCollisionFunc2D::PathPiercedEdge( *this, *pEdgeList[i], pathVector ) )
                {
                    // Find the intersection point between the edge and the path of the vert
                    CPoint tmpPoint;
                    NCollisionFunc2D::EdgeIntersectionPoint( tmpPoint, prePos, postPos,
                                                             pEdgeList[i]->pVert[0]->GetPos(),
                                                             pEdgeList[i]->pVert[1]->GetPos() );

                    // Calculate the squared distance
                    float tmpDistance = tmpPoint.x * tmpPoint.x + tmpPoint.y * tmpPoint.y;

                    if( tmpDistance < distance || !(distance > 0) )
                    {
                        distance = tmpDistance;
                        pIntersectEdge = pEdgeList[i];
                        edgeFound = true;
                    }
                }
            }
        }
        
        // We don't want to try and offset out of an edge that doesn't exist
        if( pIntersectEdge != NULL )
        {
            float tmpOffsetMag = NCollisionFunc2D::PointPiercedEdgeOffset( postPos, *pIntersectEdge ) + PADDING;
            offsetVector.x = ( pIntersectEdge->normal.x * tmpOffsetMag );
            offsetVector.y = ( pIntersectEdge->normal.y * tmpOffsetMag );
        }
    }

}	// CalculateOffset */