/************************************************************************
*    FILE NAME:       collisionmesh2d.cpp
*
*    DESCRIPTION:     2D collision mesh class
************************************************************************/

// Physical component dependency
#include <2d/collisionmesh2d.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <2d/objectdata2d.h>
#include <xmlParser/xmlParser.h>
#include <utilities/genfunc.h>
#include <utilities/exceptionhandling.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. double to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it.
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCollisionMesh2D::CCollisionMesh2D()
                : radius(0),
                  radiusSquared(0)
{
}   // Constructer */


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCollisionMesh2D::~CCollisionMesh2D()
{
}   // Destructer */


/************************************************************************
*    desc:  Load the mesh from a size. This is so that we don't have to 
*			make collision meshes for simple objects that only require a 
*			box for a collision mesh.
*  
*    param: string & _filePath - path to file
************************************************************************/
void CCollisionMesh2D::GenerateMesh( const CSize<float> & size, const int genType )
{
    // Two faces and four vertices are used to make up a generated collision mesh
    CFace2D face1, face2;
    CCollisionVertex vert[4];

    // Convert the size to an int so we can check and see if they're even or odd
    int tmp_w = size.w;
    int tmp_h = size.h;

    // Additional offsets if we need them. These are used so that we correctly
    // line up the texels and pixels
    float additionalOffsetX = 0;
    float additionalOffsetY = 0;

    // Check if the width or height is even. If so, we offset by 0.5
    if( tmp_w % 2 == 0 )
        additionalOffsetX = 0.5f;

    if( tmp_h % 2 == 0 )
        additionalOffsetY = -0.5f;

    // Offsets to center the mesh
    float offsetX = size.w / 2;
    float offsetY = size.h / 2;

    // Calculate the vert normals
    float normal = sqrt(2.f) / 2;

    // Temporary variables
    CPoint tmpVert;
    CNormal tmpNorm;

    // Set up vertex 0
    tmpVert.x = -offsetX + additionalOffsetX;
    tmpVert.y = offsetY + additionalOffsetY;
    tmpNorm.x = -normal;
    tmpNorm.y = normal;
    vert[0].SetPos( tmpVert );
    vert[0].SetNormal( tmpNorm );

    // Set up vertex 1
    tmpVert.x = offsetX + additionalOffsetX;
    tmpVert.y = offsetY + additionalOffsetY;
    tmpNorm.x = normal;
    tmpNorm.y = normal;
    vert[1].SetPos( tmpVert );
    vert[1].SetNormal( tmpNorm );

    // Set up vertex 2
    tmpVert.x = offsetX + additionalOffsetX;
    tmpVert.y = -offsetY + additionalOffsetY;
    tmpNorm.x = normal;
    tmpNorm.y = -normal;
    vert[2].SetPos( tmpVert );
    vert[2].SetNormal( tmpNorm );

    // Set up vertex 3
    tmpVert.x = -offsetX + additionalOffsetX;
    tmpVert.y = -offsetY + additionalOffsetY;
    tmpNorm.x = -normal;
    tmpNorm.y = -normal;
    vert[3].SetPos( tmpVert );
    vert[3].SetNormal( tmpNorm );
    
    if( genType == ECGT_DEFAULT )
    {
        // Insert the vertices into the vertex vector
        for( int i = 0; i < 4; ++i )
            vertVector.push_back( vert[i] );

        // Create the two faces from the four vertices
        face1 = CFace2D( &vertVector[0], &vertVector[1], &vertVector[3] );
        face1.vertIndex[0] = 0;
        face1.vertIndex[1] = 1;
        face1.vertIndex[2] = 3;

        face2 = CFace2D( &vertVector[1], &vertVector[2], &vertVector[3] );
        face2.vertIndex[0] = 1;
        face2.vertIndex[1] = 2;
        face2.vertIndex[2] = 3;

        // Insert the faces into the face vector
        faceVector.push_back(face1);
        faceVector.push_back(face2);
    }
    else if( genType == ECGT_TOP_RIGHT )
    {
        // Insert the vertices into the vertex vector
        vertVector.push_back( vert[0] );
        vertVector.push_back( vert[1] );
        vertVector.push_back( vert[2] );

        // Create the two faces from the four vertices
        face1 = CFace2D( &vertVector[0], &vertVector[1], &vertVector[2] );
        face1.vertIndex[0] = 0;
        face1.vertIndex[1] = 1;
        face1.vertIndex[2] = 2;

        // Insert the faces into the face vector
        faceVector.push_back(face1);
    }
    else if( genType == ECGT_TOP_LEFT )
    {
        // Insert the vertices into the vertex vector
        vertVector.push_back( vert[0] );
        vertVector.push_back( vert[1] );
        vertVector.push_back( vert[3] );

        // Create the two faces from the four vertices
        face1 = CFace2D( &vertVector[0], &vertVector[1], &vertVector[2] );
        face1.vertIndex[0] = 0;
        face1.vertIndex[1] = 1;
        face1.vertIndex[2] = 2;

        // Insert the faces into the face vector
        faceVector.push_back(face1);
    }
    else if( genType == ECGT_BOTTOM_LEFT )
    {
        // Insert the vertices into the vertex vector
        vertVector.push_back( vert[0] );
        vertVector.push_back( vert[2] );
        vertVector.push_back( vert[3] );

        // Create the two faces from the four vertices
        face1 = CFace2D( &vertVector[0], &vertVector[1], &vertVector[2] );
        face1.vertIndex[0] = 0;
        face1.vertIndex[1] = 1;
        face1.vertIndex[2] = 2;

        // Insert the faces into the face vector
        faceVector.push_back(face1);
    }
    else if( genType == ECGT_BOTTOM_RIGHT )
    {
        // Insert the vertices into the vertex vector
        vertVector.push_back( vert[1] );
        vertVector.push_back( vert[2] );
        vertVector.push_back( vert[3] );

        // Create the two faces from the four vertices
        face1 = CFace2D( &vertVector[0], &vertVector[1], &vertVector[2] );
        face1.vertIndex[0] = 0;
        face1.vertIndex[1] = 1;
        face1.vertIndex[2] = 2;

        // Insert the faces into the face vector
        faceVector.push_back(face1);
    }

    // Calculate the radii
    //radius = pow(vertVector[0].GetVert().x, 2) + pow(vertVector[0].GetVert().y, 2);
    radiusSquared = vertVector[0].GetPos().GetLengthSquared();
    radius = sqrt(radiusSquared);

    // Find the unique edges in the mesh
    FindUniqueEdges();

}	// GenerateMesh */


/************************************************************************
*    desc:  Generate the mesh using object data. This is so that we don't 
*			have to make collision meshes for simple objects that only 
*			require a box or triangle for a collision mesh.
*  
*    param: const CObjectData2D * pData - data used to create the mesh
************************************************************************/
void CCollisionMesh2D::GenerateMesh( const CObjectData2D * pData )
{
    // Two faces and four vertices are used to make up a generated collision mesh
    CFace2D face1, face2;
    CCollisionVertex vert[4];

    // Set the size of the collision mesh
    CSize<float> tmpSize;
    if( !pData->GetCollisionData().GetSize().IsEmpty() )
        tmpSize = pData->GetCollisionData().GetSize();
    else
        tmpSize = pData->GetVisualData().GetSize();

    // Additional offsets if we need them. These are used so that we correctly
    // line up the texels and pixels
    float additionalOffsetX = 0;
    float additionalOffsetY = 0;

    // Check if the width or height is even. If so, we offset by 0.5
    if( ((int)tmpSize.w & 1) == 0 )
        additionalOffsetX = 0.5f;

    if( ((int)tmpSize.h & 1) == 0 )
        additionalOffsetY = -0.5f;

    // Offsets to center the mesh
    float offsetX = tmpSize.w / 2.f;
    float offsetY = tmpSize.h / 2.f;

    // Calculate the vert normals
    float normal = sqrt(2.f) / 2.f;

    // Temporary variables
    CPoint tmpVert;
    CNormal tmpNorm;

    // Bottom and left mod have their signs flipped so that a positive mod always means
    // expansion of the side, and a negative mod always means a contraction of the side
    float topMod = pData->GetCollisionData().GetTopMod();
    float bottomMod = -pData->GetCollisionData().GetBottomMod();
    float leftMod = -pData->GetCollisionData().GetLeftMod();
    float rightMod = pData->GetCollisionData().GetRightMod();

    // Set up vertex 0
    tmpVert.x = -offsetX + additionalOffsetX + leftMod;
    tmpVert.y = offsetY + additionalOffsetY + topMod;
    tmpNorm.x = -normal;
    tmpNorm.y = normal;
    vert[0].SetPos( tmpVert );
    vert[0].SetNormal( tmpNorm );

    // Set up vertex 1
    tmpVert.x = offsetX + additionalOffsetX + rightMod;
    tmpVert.y = offsetY + additionalOffsetY + topMod;
    tmpNorm.x = normal;
    tmpNorm.y = normal;
    vert[1].SetPos( tmpVert );
    vert[1].SetNormal( tmpNorm );

    // Set up vertex 2
    tmpVert.x = offsetX + additionalOffsetX + rightMod;
    tmpVert.y = -offsetY + additionalOffsetY + bottomMod;
    tmpNorm.x = normal;
    tmpNorm.y = -normal;
    vert[2].SetPos( tmpVert );
    vert[2].SetNormal( tmpNorm );

    // Set up vertex 3
    tmpVert.x = -offsetX + additionalOffsetX + leftMod;
    tmpVert.y = -offsetY + additionalOffsetY + bottomMod;
    tmpNorm.x = -normal;
    tmpNorm.y = -normal;
    vert[3].SetPos( tmpVert );
    vert[3].SetNormal( tmpNorm );
    
    //if( pData->GetCollisionData().GetGenType() == ECGT_DEFAULT )
    switch( pData->GetCollisionData().GetGenType() )
    {
        case ECGT_DEFAULT:
        {
            // Insert the vertices into the vertex vector
            for( int i = 0; i < 4; ++i )
                vertVector.push_back( vert[i] );

            // Create the two faces from the four vertices
            face1 = CFace2D( &vertVector[0], &vertVector[1], &vertVector[3] );
            face1.vertIndex[0] = 0;
            face1.vertIndex[1] = 1;
            face1.vertIndex[2] = 3;

            face2 = CFace2D( &vertVector[1], &vertVector[2], &vertVector[3] );
            face2.vertIndex[0] = 1;
            face2.vertIndex[1] = 2;
            face2.vertIndex[2] = 3;

            // Insert the faces into the face vector
            faceVector.push_back(face1);
            faceVector.push_back(face2);
            break;
            // Calculate the radii
            //radius = pow(face1.edge[0].pVert[0]->GetVert().x, 2) + pow(face1.edge[0].pVert[0]->GetVert().y, 2);
            //radiusSqrt = sqrt(radius);
        }
        //else if( pData->GetCollisionData().GetGenType() == ECGT_TOP_RIGHT )
        case ECGT_TOP_RIGHT:
        {
            // Insert the vertices into the vertex vector
            vertVector.push_back( vert[0] );
            vertVector.push_back( vert[1] );
            vertVector.push_back( vert[2] );

            // Create the two faces from the four vertices
            face1 = CFace2D( &vertVector[0], &vertVector[1], &vertVector[2] );
            face1.vertIndex[0] = 0;
            face1.vertIndex[1] = 1;
            face1.vertIndex[2] = 2;

            // Insert the faces into the face vector
            faceVector.push_back(face1);
            break;
        }
        //else if( pData->GetCollisionData().GetGenType() == ECGT_TOP_LEFT )
        case ECGT_TOP_LEFT:
        {
            // Insert the vertices into the vertex vector
            vertVector.push_back( vert[0] );
            vertVector.push_back( vert[1] );
            vertVector.push_back( vert[3] );

            // Create the two faces from the four vertices
            face1 = CFace2D( &vertVector[0], &vertVector[1], &vertVector[2] );
            face1.vertIndex[0] = 0;
            face1.vertIndex[1] = 1;
            face1.vertIndex[2] = 2;

            // Insert the faces into the face vector
            faceVector.push_back(face1);
            break;
        }
        //else if( pData->GetCollisionData().GetGenType() == ECGT_BOTTOM_LEFT )
        case ECGT_BOTTOM_LEFT:
        {
            // Insert the vertices into the vertex vector
            vertVector.push_back( vert[0] );
            vertVector.push_back( vert[2] );
            vertVector.push_back( vert[3] );

            // Create the two faces from the four vertices
            face1 = CFace2D( &vertVector[0], &vertVector[1], &vertVector[2] );
            face1.vertIndex[0] = 0;
            face1.vertIndex[1] = 1;
            face1.vertIndex[2] = 2;

            // Insert the faces into the face vector
            faceVector.push_back(face1);
            break;
        }
        //else if( pData->GetCollisionData().GetGenType() == ECGT_BOTTOM_RIGHT )
        case ECGT_BOTTOM_RIGHT:
        {
            // Insert the vertices into the vertex vector
            vertVector.push_back( vert[1] );
            vertVector.push_back( vert[2] );
            vertVector.push_back( vert[3] );

            // Create the two faces from the four vertices
            face1 = CFace2D( &vertVector[0], &vertVector[1], &vertVector[2] );
            face1.vertIndex[0] = 0;
            face1.vertIndex[1] = 1;
            face1.vertIndex[2] = 2;

            // Insert the faces into the face vector
            faceVector.push_back(face1);
            break;
        }
    }

    // Calculate the radii
    radiusSquared = vertVector[0].GetPos().GetLengthSquared();
    radius = sqrt(radiusSquared);

    // Find the unique edges in the mesh
    FindUniqueEdges();

}	// GenerateMesh */


/************************************************************************
*    desc:  Load the mesh using the object data
*  
*    param: const CObjectData2D * pData - data to load the mesh
*    
*    return: bool - true on success or false on fail
************************************************************************/
void CCollisionMesh2D::LoadFromData( const CObjectData2D * pData )
{
    // If the generation type isn't null, we're generating the mesh
    if( pData->GetCollisionData().GetGenType() != ECGT_NULL )
        GenerateMesh( pData );

    // NOTE: CURRENTLY NOT IN USE. ONLY RETURNS FALSE
    else if( pData->GetCollisionData().GetFile() != "" )
        LoadFromXML( pData->GetCollisionData().GetFile() );

}	// LoadFromData */


/***************************************************************************
*   desc:  Find the unique edges in the mesh
 ****************************************************************************/
void CCollisionMesh2D::FindUniqueEdges()
{
    // If a non-unique edge has been found
    bool similarEdgeFound;

    vector<CEdge *> tmpEdgeVector;
    tmpEdgeVector.reserve( faceVector.size() * 3 );
    
    for( size_t i = 0; i < faceVector.size(); ++i )
    {
        for( int j = 0; j < 3; ++j )
        {
            similarEdgeFound = false;

            for( size_t k = 0; k < faceVector.size(); ++k )
            {
                if( k != i )
                {
                    for( uint l = 0; l < 3; ++l )
                    {
                        // Compare vertex pointers to determine if the edges are the same
                        if( faceVector[i].edge[j].pVert[0] == faceVector[k].edge[l].pVert[1] && 
                            faceVector[i].edge[j].pVert[1] == faceVector[k].edge[l].pVert[0] )
                        {
                            faceVector[i].edge[j].unique = false;
                            faceVector[k].edge[l].unique = false;

                            similarEdgeFound = true;
                            break;
                        }
                    }
                }

                if( similarEdgeFound )
                    break;
            }

            if( !similarEdgeFound )
                faceVector[i].edge[j].unique = true;
        }
    }
    
}	// FindUniqueEdges */


/***************************************************************************
*   desc:  Get the radius
*
*   ret:  float - value of radius
 ****************************************************************************/
float CCollisionMesh2D::GetRadius() const
{
    return radius;

}	// GetRadius */


/***************************************************************************
*   desc:  Get the radius squared
*
*   ret:  float - value of radius
 ****************************************************************************/
float CCollisionMesh2D::GetRadiusSquared() const
{
    return radiusSquared;

}	// GetRadiusSquared */


/****************************************************************************
*   desc:  Get a face in the mesh
*
*   ret:  CFace2D & - face
****************************************************************************/
const CFace2D & CCollisionMesh2D::GetFace( const size_t index )
{
    if( index >= faceVector.size() )
        throw NExcept::CCriticalException("Index out of range",
            boost::str( boost::format("Index out of range of vector size (%d,%d).\n\n%s\nLine: %s") % index % faceVector.size() % __FUNCTION__ % __LINE__ ));

    return faceVector[index];

}	// GetFaceVector */


/****************************************************************************
*   desc:  Get all of the mesh's faces
*
*   ret:  vector<CFace2D> & - vector of faces
****************************************************************************/
const vector<CFace2D> & CCollisionMesh2D::GetFaceVector()
{
    return faceVector;

}	// GetFaceVector */


/****************************************************************************
*   desc:  Get a vert in the mesh
*
*   ret:  CPoint & - vertex
****************************************************************************/
const CCollisionVertex & CCollisionMesh2D::GetVert( const size_t index )
{
    if( index >= vertVector.size() )
        throw NExcept::CCriticalException("Index out of range",
            boost::str( boost::format("Index out of range of vector size (%d,%d).\n\n%s\nLine: %s") % index % faceVector.size() % __FUNCTION__ % __LINE__ ));

    return vertVector[index];

}	// GetVert */


/****************************************************************************
*   desc:  Get all of the mesh's verts
*
*   ret:  vector<CPoint> & - vector of vertices
****************************************************************************/
const vector<CCollisionVertex> & CCollisionMesh2D::GetVertVector()
{
    return vertVector;

}	// GetVertVector */


/*****************************************************************************
*   desc:	Transform the mesh and apply the transformation to the passed in 
*			vertices and faces
*
*   param:	vector<CCollisionVertex> & transVert - vector of vertices to be translated
*			vector<CFace2D> & transFace			 - vector of faces to be translated
*			float scale							 - value to scale the mesh
*			CMatrix & scaledMatrix				 - matrix including scale
*			CMatrix & unscaledMatrix			 - matrix not including scale
 ****************************************************************************/
void CCollisionMesh2D::Transform( vector<CCollisionVertex> & transVert, vector<CFace2D> & transFace, float scale,
                                  const CMatrix & scaledMatrix, const CMatrix & unscaledMatrix, const CWorldPoint & point )
{
    // Transform the vertex data
    for( size_t i = 0; i < vertVector.size(); ++i )
    {
        transVert[i].GetPos().Clear();

        CPoint dest;
        scaledMatrix.Transform3x3( dest, vertVector[i].GetPos() );
        transVert[i].SetPos( dest );
        transVert[i].IncPos( point );

        unscaledMatrix.Transform3x3( transVert[i].GetNormal(), vertVector[i].GetNormal() );
    }

    // Transform the face data
    for( size_t i = 0; i < transFace.size(); ++i )
    {
        // Transform the normals
        for( int j = 0; j < 3; ++j )
            unscaledMatrix.Transform3x3( transFace[i].edge[j].normal, faceVector[i].edge[j].normal );

        // Transform the face radius
        transFace[i].radius = faceVector[i].radius * scale;

        // Connect the edges in the face so we don't have to transform
        transFace[i].ConnectEdges();

        // Transform the center of the face
        transFace[i].CalculateCenter();
    }

}	// Transform */
