/************************************************************************
*    FILE NAME:     spritepath2d.cpp
*
*    DESCRIPTION:   Class to hold the information of a moving collision
*					sprite
************************************************************************/

// Physical component dependency
#include <2d/spritepath2d.h>

// Game lib dependencies
#include <2d/spritegroup2d.h>
#include <utilities/collisionfunc2d.h>
#include <utilities/genfunc.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

// Some padding when offseting sprites
const float PADDING = 0.1f;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CSpritePath2D::CSpritePath2D()
{
}	// Constructor */


/************************************************************************
*    desc:  Use the passed in sprite to find any potential collisions
*			against its edges
************************************************************************/
void CSpritePath2D::FindPossibleEdgeCollisions()
{
    for( unsigned int sprite = 0; sprite < pPossibleSpritesList.size(); sprite++ )
    {
        for( unsigned int i = 0; i < vertPathList.size(); i++ )
        {
            for( unsigned int edge = 0; edge < pPossibleSpritesList[sprite]->GetOuterEdgeCount(); edge++ )
            {
                if( NCollisionFunc2D::PointPiercedEdge( vertPathList[i].GetPostPos(), *pPossibleSpritesList[sprite]->GetOuterEdge(edge) ) && 
                    NCollisionFunc2D::VectorsFacing( pPossibleSpritesList[sprite]->GetOuterEdge(edge)->normal, vertPathList[i].GetPathVector() ) )
                {
                    // Add the edge to the vertex path's edge list
                    vertPathList[i].AddEdge( pPossibleSpritesList[sprite]->GetOuterEdge(edge) );
                }
            }
        }
    }

}	// FindPossibleEdgeCollisions */


/************************************************************************
*    desc:  Use the passed in sprite to find any potential collisions
*			against its vertices
************************************************************************/
void CSpritePath2D::FindPossibleVertCollisions()
{
    for( unsigned int sprite = 0; sprite < pPossibleSpritesList.size(); sprite++ )
    {
        for( unsigned int vert = 0; vert < pPossibleSpritesList[sprite]->GetVertCount(); vert++ )
            pStaticVertList.push_back( &pPossibleSpritesList[sprite]->GetTransVert(vert) );
    }

}	// FindPossibleVertCollisions */


/************************************************************************
*    desc:  Create the vertex path list
************************************************************************/
void CSpritePath2D::CreateVertPathList()
{
    // Reserve the amount of space needed for the vertex path list
    vertPathList.reserve( pGroup->GetCollisionSprite().GetVertCount() * 2 );
    
    // Begin creating the vertex path list is the pre-transformed sprite
    for( unsigned int i = 0; i < pGroup->GetCollisionSprite().GetVertCount(); i++ )
    {
        CVertexPath tmpPath;
        tmpPath.SetPrePos( pGroup->GetCollisionSprite().GetTransVert( i ).GetPos() );
        vertPathList.push_back( tmpPath );
    }

    // Get the pre-transformation edge center points and pretend that they are vertices
    for( unsigned int i = 0; i < pGroup->GetCollisionSprite().GetOuterEdgeCount(); i++ )
    {
        CVertexPath tmpPath;
        tmpPath.SetPrePos( pGroup->GetCollisionSprite().GetOuterEdge( i )->GetCenter() );
        vertPathList.push_back( tmpPath );
    }

    // Transform the sprite
    pGroup->TransformCollisionToWorld();

    // Add the transformed vertices into the vert path list
    for( unsigned int i = 0; i < pGroup->GetCollisionSprite().GetVertCount(); i++ )
    {
        vertPathList[i].SetPostPos( pGroup->GetCollisionSprite().GetTransVert( i ).GetPos() );
        vertPathList[i].SetVertNormal( pGroup->GetCollisionSprite().GetTransVert( i ).GetNormal() );
    }

    // Get the post-transformation edge center points and pretend that they are vertices
    for( unsigned int i = 0; i < pGroup->GetCollisionSprite().GetOuterEdgeCount(); i++ )
    {
        vertPathList[i + pGroup->GetCollisionSprite().GetVertCount()].SetPostPos( pGroup->GetCollisionSprite().GetOuterEdge( i )->GetCenter() );
        vertPathList[i + pGroup->GetCollisionSprite().GetVertCount()].SetVertNormal( pGroup->GetCollisionSprite().GetOuterEdge( i )->normal );
    }

    // Calculate the distance traveled by the sprite's center
    //CWorldPoint tmp = vertPathList[0].GetPostPos() - vertPathList[0].GetPrePos();
    //tmp.ConvertToFloatPoint();
    pathVector = vertPathList[0].GetPostPos() - vertPathList[0].GetPrePos();

    // Calculate the path vector for each vertex
    for( unsigned int i = 0; i < vertPathList.size(); i++ )
        vertPathList[i].CalculatePathVector();

}	// CreateVertPathList */


/************************************************************************
*    desc:  React the passed in sprite's vertex paths to the static edges
*
*	 param:	CSpriteGroup2D * pGroup - sprite to react to collision
************************************************************************/
void CSpritePath2D::ReactToStaticEdges()
{
    if( vertPathList.size() > 0 )
    {
        float magTest = 0;

        // The amount to offset the passed in sprite by
        CPoint offsetVector;
            
        // Vector to check against to find the smallest distance between an intersection
        // point and the starting position of the path
        CPoint tmpOffsetVector;
        
        for( unsigned int path = 0; path < vertPathList.size(); path++ )
        {
            vertPathList[path].CalculateOffset();

            tmpOffsetVector = vertPathList[path].GetOffsetVector();

            float tmpMag = tmpOffsetVector.x * tmpOffsetVector.x + tmpOffsetVector.y * tmpOffsetVector.y;

            // Keep track of the largest offset
            if( tmpMag > magTest )
            {
                offsetVector = tmpOffsetVector;
                magTest = tmpMag;
            }
        }
        
        // Offset and transform the sprite copy
        pGroup->IncPos( offsetVector );
        pGroup->TransformCollisionToWorld();
    }

}	// ReactToStaticEdges */


/************************************************************************
*    desc:  React the moving sprite to the vertices of the static sprites
*
*	 param:	CSpritePath2D & sPath - sprite to react to collision
************************************************************************/
void CSpritePath2D::ReactToStaticVerts()
{
    if( pStaticVertList.size() > 0 )
    {
        // Check each vert in the static vert list for collision
        for( unsigned int vert = 0; vert < pStaticVertList.size(); vert++ )
        {
            // If the vertex is no long inside of the sprite, we skip it's offset calculation
            // NOTE: If possible, I don't want to use this as a test but I'm leaving it in here just in case
            //if( NCollisionFunc2D::PointInSprite( pStaticVertList[vert]->GetVert(), pGroup->GetCollisionSprite() ) )
            {
                // Calculate the offset
                CPoint tmpOffsetVector = pStaticVertList[vert]->CalculateOffsetViaPath( this );
                
                // Offset and transform the sprite
                pGroup->IncPos( tmpOffsetVector );
                pGroup->TransformCollisionToWorld();
            }
        }
    }

}	// ReactToStaticVerts */


/************************************************************************
*    desc:  React the sprite's verts and edges to the static verts and edges
************************************************************************/
void CSpritePath2D::ReactToAll()
{
    // Boolean to check and see if we have our second vector set
    bool vector2Set = false;
    
    // Make a list of points to hold all of the offset possibilities
    vector<CPoint> offsetVectorList;
    offsetVectorList.reserve( vertPathList.size() + pStaticVertList.size() );

    float offsetLimit = 2 * (abs(pathVector.x) + abs(pathVector.y));

    // Add the vert path list offset vectors to the offset vector list
    for( unsigned int i = 0; i < vertPathList.size(); i++ )
    {
        if( vertPathList[i].GetEdgeCount() > 0 )
        {
            CPoint tmp = vertPathList[i].GetOffsetVector();

            float offsetTest = abs(tmp.x) + abs(tmp.y);

            if( ( offsetTest < offsetLimit ) && ( abs(tmp.x) > 0 || abs(tmp.y) > 0 ) )
                offsetVectorList.push_back( tmp );
        }
    }

    // Add the static vert list offset vectors to the offset vector list
    for( unsigned int i = 0; i < pStaticVertList.size(); i++ )
    {
        // TO DO: OPTIMIZE THIS //////////////////////////////////////////////////////////////////////////
        CPoint tmp = pStaticVertList[i]->CalculateOffsetViaPath( this );
        CPoint tmp2 = pStaticVertList[i]->CalculateOffsetViaNormal( pGroup );
        //////////////////////////////////////////////////////////////////////////////////////////////////

        float offsetTest = abs(tmp.x) + abs(tmp.y);

        if( ( offsetTest < offsetLimit ) && ( abs(tmp.x) > 0 || abs(tmp.y) > 0 ) )
            offsetVectorList.push_back( tmp );

        offsetTest = abs(tmp2.x) + abs(tmp2.y);

        if( ( offsetTest < offsetLimit ) && ( abs(tmp2.x) > 0 || abs(tmp2.y) > 0 ) )
            offsetVectorList.push_back( tmp2 );
    }

    // Temporary vectors
    CPoint tmpVector1, tmpVector2, offsetVector1, offsetVector2;

    // Compare each vector to find two that are compatible 
    for( unsigned int i = 0; i < offsetVectorList.size(); i++ )
    {
        tmpVector1 = offsetVectorList[i];

        for( unsigned int j = 0; j < offsetVectorList.size(); j++ )
        {
            if( i != j )
            {
                tmpVector2 = offsetVectorList[j];

                if( NCollisionFunc2D::VectorsCompatibleForColAll( tmpVector1, tmpVector2 ) )
                {
                    offsetVector1 = tmpVector1;
                    offsetVector2 = tmpVector2;

                    break;
                }
            }
        }
    }

    /* TEST CODE ***********************************************************************

    if( offsetVector1.x == 0 && offsetVector1.y == 0 && offsetVector2.x == 0 && offsetVector2.y == 0 )
    //if( offsetVector1.y == 0 && offsetVector2.y == 0 )
    //if( abs(offsetVector1.y) > 50 || abs(offsetVector2.y) > 50 || abs(offsetVector1.x) > 50 || abs(offsetVector2.x) > 50 )
        int asdf = 0;

    //if( offsetVector1.x == 0 && offsetVector2.x == 0 )
        //int asdf = 0;

    NGenFunc::PostDebugMsg( "V1(%f, %f) V2(%f, %f)", offsetVector1.x, offsetVector1.y, offsetVector2.x, offsetVector2.y );

    // TEST CODE //////////////////////////////////////////////////////////////////////*/

    // Variables used to calculate the final offset
    CPoint p1, p2, p1Vector, p2Vector, intersectPoint, finalOffsetVector;

    // Find the value of point 2 using the two offset vectors
    p2 = offsetVector1 + (offsetVector2 * -1);

    // Rotate the offset vectors counter-clockwise by 90 degrees
    p1Vector.x = -offsetVector1.y;
    p1Vector.y = offsetVector1.x;

    p2Vector.x = -offsetVector2.y;
    p2Vector.y = offsetVector2.x;

    // Find the intersection point of these two vectors from their corresponding points
    NCollisionFunc2D::EdgeIntersectionPoint( intersectPoint, p1, p1 + p1Vector, p2, p2 + p2Vector );

    // Calculate the final offsetVector
    finalOffsetVector = offsetVector1 - intersectPoint;

    // Offset and transform the sprite
    pGroup->IncPos( finalOffsetVector );
    pGroup->TransformCollisionToWorld();

}	// ReactToAll */


/************************************************************************
*    desc:  Check to see if the moving sprite is colliding with anything
************************************************************************/
bool CSpritePath2D::Colliding()
{
    // Check to see if any vertices in the moving sprite are within the surrounding sprites
    for( unsigned int i = 0; i < pGroup->GetCollisionSprite().GetVertCount(); i++ )
    {
        for( unsigned int j = 0; j < pPossibleSpritesList.size(); j++ )
        {
            if( NCollisionFunc2D::PointInSpriteFast( pGroup->GetCollisionSprite().GetTransVert(i).GetPos(), *pPossibleSpritesList[j], true ) )
                return true;
        }
    }

    // Check to see if any vertices in the static sprites are within the moving sprite
    for( unsigned int i = 0; i < pStaticVertList.size(); i++ )
    {	
        //if( NCollisionFunc2D::PointInSpriteFast( pStaticVertList[i]->GetVert(), pGroup->GetCollisionSprite(), true ) )
        if( NCollisionFunc2D::PointInSpriteFast( pStaticVertList[i]->GetPos(), pGroup->GetCollisionSprite(), true ) )
            return true;
    }

    return false;

}	// Colliding */


/************************************************************************
*    desc:  Set the moving object's sprite                
*
*	 param:	CSpriteGroup2D * pSprite - sprite to set to
************************************************************************/
void CSpritePath2D::SetSprite( CSpriteGroup2D * pSprite )
{
    // Set the collision sprite to the passed in sprite
    pGroup = pSprite;

    // Get the position of the collision sprite
    //initialTransPos = pGroup->GetCollisionSprite().GetPos();
    initialTransPos = pGroup->GetCollisionSprite().GetPos();

    // Create the vertex path list
    CreateVertPathList();

}	// SetCollisionSprite */


/************************************************************************
*    desc:  Get the moving object's sprite                
*
*	 param:	CSpriteGroup2D * - moving sprite
************************************************************************/
CSpriteGroup2D * CSpritePath2D::GetSprite()
{
    return pGroup;

}	// GetCollisionSprite */


/************************************************************************
*    desc:  Get the moving sprite's radius 
************************************************************************/
float CSpritePath2D::GetRadiusSqrt()
{
    return radiusSqrt;

}	// GetRadiusSqrt */


/************************************************************************
*    desc:  Get the sprite path vector
*
*	 ret:	CPoint & - path vector
************************************************************************/
CPoint & CSpritePath2D::GetPathVector()
{
    return pathVector;

}	// GetPathVector */


/************************************************************************
*    desc:  Get the moving sprite's center point 
************************************************************************/
CPoint & CSpritePath2D::GetCenter()
{
    return center;

}	// GetCenter */


/************************************************************************
*    desc:  Add a sprite to the possible sprite list
*
*	 param:	vector<CCollisionSprite2D *> & pSpriteList - sprites to set
************************************************************************/
void CSpritePath2D::SetPossibleSpriteList( vector<CCollisionSprite2D *> & pSpriteList )
{
    pPossibleSpritesList = pSpriteList;

}	// SetPossibleSpriteList */


/************************************************************************
*    desc:  Transform the collision sprite using the initial translation
*			position 
************************************************************************/
void CSpritePath2D::ResetSpriteTransformation()
{
    pGroup->SetPos( initialTransPos );
    pGroup->TransformCollisionToWorld();

}	// ResetSpriteTransformation */