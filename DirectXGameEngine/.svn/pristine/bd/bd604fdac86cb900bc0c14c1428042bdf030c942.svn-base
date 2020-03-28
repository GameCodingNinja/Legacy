/************************************************************************
*    FILE NAME:       collisionsprite2d.cpp
*
*    DESCRIPTION:     2D collision sprite class
************************************************************************/

// Physical component dependency
#include <2d/collisionsprite2d.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <managers/meshmanager.h>
#include <utilities/genfunc.h>
#include <utilities/statcounter.h>
#include <utilities/exceptionhandling.h>
#include <utilities/mathfunc.h>
#include <misc/settings.h>
#include <common/collisionbody.h>
#include <2d/objectdata2d.h>


/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCollisionSprite2D::CCollisionSprite2D( CObjectData2D * pObjData )
                  : pObjectData(pObjData),
                    pMesh(NULL)
{
    Init();

}   // Constructer */

CCollisionSprite2D::CCollisionSprite2D()
                  : pObjectData(NULL),
                    pMesh(NULL)
{
}   // Constructer */


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCollisionSprite2D::~CCollisionSprite2D()
{
}   // Destructer */


/************************************************************************
*    desc:  Init the sprite
************************************************************************/
void CCollisionSprite2D::Init( const std::string groupName, const std::string & meshPath )
{
    pMesh = CMeshMgr::Instance().GetCollisionMesh2D( groupName, meshPath );
    SetRadius( pMesh->GetRadius() );
    CopyMesh();

}   // Init */


/************************************************************************
*    desc:  Init the spritea
************************************************************************/
void CCollisionSprite2D::Init( const std::string groupName, const CSize<float> & size, int genType )
{
    pMesh = CMeshMgr::Instance().GetCollisionMesh2D( groupName, size, genType );
    SetRadius( pMesh->GetRadius() );
    CopyMesh();

}   // Init */


/************************************************************************
*    desc:  Init the sprite
************************************************************************/
void CCollisionSprite2D::Init()
{
    pMesh = CMeshMgr::Instance().GetCollisionMesh2D( pObjectData );
    SetRadius( pMesh->GetRadius() );

    // Set the body information
    colBody.SetMass( GetCollisionData().GetDensity() );
    colBody.SetInertia( GetCollisionData().GetDensity() );
    colBody.SetRestitution( GetCollisionData().GetRestitution() );
    colBody.Init( GetRadius() );
    
    CopyMesh();

}   // Init */


/************************************************************************
*    desc:  Copy the verts and faces of the mesh to the sprite
************************************************************************/
void CCollisionSprite2D::CopyMesh()
{
    // Create a copy of the vert pointers
    for( size_t i = 0; i < pMesh->GetVertVector().size(); ++i )
        transVertVector.push_back( pMesh->GetVert(i) );

    for( size_t i = 0; i < pMesh->GetFaceVector().size(); ++i )
    {
        CFace2D tmpFace;

        // Copy the data from the mesh's faces
        tmpFace.center = pMesh->GetFace(i).center;
        tmpFace.radius = pMesh->GetFace(i).radius;
        tmpFace.sqrtRadius = pMesh->GetFace(i).sqrtRadius;

        // Apply the correct vert pointers to the verts in the face
        for( uint j = 0; j < 3; ++j )
        {
            tmpFace.vertIndex[j] = pMesh->GetFace(i).vertIndex[j];
            tmpFace.edge[j].pVert[0] = &transVertVector[pMesh->GetFace(i).vertIndex[j]];
            tmpFace.edge[j].normal = pMesh->GetFace(i).edge[j].normal;
            tmpFace.edge[j].unique = pMesh->GetFace(i).edge[j].unique;
        }

        tmpFace.ConnectEdges();

        // Add the copied face to the translated face vector
        transFaceVector.push_back( tmpFace );
    }

    pOuterEdgeVector.reserve( transFaceVector.size() * 3 );

    // Get the unique edges in the collision sprite
    for( size_t i = 0; i < transFaceVector.size(); ++i )
    {
        for( size_t j = 0; j < 3; ++j )
        {
            if( transFaceVector[i].edge[j].unique )
                pOuterEdgeVector.push_back( &transFaceVector[i].edge[j] );
        }
    }

}	// CopyMesh */


/************************************************************************
*    desc:  Is this sprite active
*  
*    ret:	bool - sprite scale
************************************************************************/
bool CCollisionSprite2D::IsActive()
{
    return (pMesh != NULL);

}   // IsActive */


/************************************************************************
*    desc:  Transform the sprite
************************************************************************/
void CCollisionSprite2D::Transform()
{
    if( IsActive() )
    {
        if( GetParameters().IsSet( CObject::TRANSFORM ) )
        {
            CObject::Transform();

            // Inc our stat counter to keep track of what is going on.
            CStatCounter::Instance().IncCollisionCounter(1);

            CPoint tmpScale = GetScale();
            float tmpScaleValue = tmpScale.x;

            if( tmpScaleValue < tmpScale.y )
                tmpScaleValue = tmpScale.y;

            else if( tmpScaleValue < tmpScale.z )
                tmpScaleValue = tmpScale.z;

            // Transform the mesh
            pMesh->Transform( transVertVector, transFaceVector, tmpScaleValue, 
                              GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

            ResetTransformParameters();
        }
    }

}	// Transform */


/************************************************************************
*    desc:  Transform the sprite
*
*	 param: const CMatrix & _scaledMatrix  - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point      - passed in relative position
************************************************************************/
void CCollisionSprite2D::Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                                    const CWorldPoint & point )
{
    if( IsActive() )
    {
        if( GetParameters().IsSet( CObject::TRANSFORM ) )
        {
            // Transform the sprite's matrices
            CObject::Transform( _scaledMatrix, _unscaledMatrix, point );

            // Inc our stat counter to keep track of what is going on.
            CStatCounter::Instance().IncCollisionCounter(1);

            CPoint tmpScale = GetScale();
            float tmpScaleValue = tmpScale.x;

            if( tmpScaleValue < tmpScale.y )
                tmpScaleValue = tmpScale.y;

            else if( tmpScaleValue < tmpScale.z )
                tmpScaleValue = tmpScale.z;

            // Transform the mesh
            pMesh->Transform( transVertVector, transFaceVector, tmpScaleValue, 
                              GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

            ResetTransformParameters();
        }
    }

}	// Transform */


/************************************************************************
*    desc:  Transform the sprite
************************************************************************/
void CCollisionSprite2D::TransformToWorld()
{
    if( IsActive() )
    {
        if( GetParameters().IsSet( CObject::TRANSFORM ) )
        {
            CObject::TransformToLocal();

            // Inc our stat counter to keep track of what is going on.
            CStatCounter::Instance().IncCollisionCounter(1);

            CPoint tmpScale = GetScale();
            float tmpScaleValue = tmpScale.x;

            if( tmpScaleValue < tmpScale.y )
                tmpScaleValue = tmpScale.y;

            else if( tmpScaleValue < tmpScale.z )
                tmpScaleValue = tmpScale.z;

            // Transform the mesh
            pMesh->Transform( transVertVector, transFaceVector, tmpScaleValue, 
                              GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

            for( size_t i = 0; i < transVertVector.size(); ++i )
                transVertVector[i].IncPos( GetPos() );

            ResetTransformParameters();
        }
    }

}	// Transform */



/************************************************************************
*    desc:  Get a face in the sprite
*
*	 param: int index - index into the translated faces
*  
*    ret:	CFace2D & - face in the sprite
************************************************************************/
const CFace2D & CCollisionSprite2D::GetFace( uint index )
{
    return pMesh->GetFace(index);

}   // GetFace */


/************************************************************************
*    desc:  Get a translated face in the sprite
*
*	 param: int index - index into the translated faces
*  
*    ret:	CFace2D & - translated face in the sprite
************************************************************************/
CFace2D & CCollisionSprite2D::GetTransFace( uint index )
{
    if( index >= transFaceVector.size() )
        throw NExcept::CCriticalException("Index out of range",
            boost::str( boost::format("Index out of range of vector size (%d,%d).\n\n%s\nLine: %s") % index % transFaceVector.size() % __FUNCTION__ % __LINE__ ));

    return transFaceVector[index];

}   // GetTransFace */


/************************************************************************
*    desc:  Get the translated face vector
*  
*    ret:	vector<CFace2D> & - translated face vector
************************************************************************/
std::vector<CFace2D> & CCollisionSprite2D::GetTransFaceVector()
{
    return transFaceVector;

}   // GetTransFaceVector */


/************************************************************************
*    desc:  Get a translated vert in the sprite
*
*	 param: int index - index into the translated verts
*  
*    ret:	CPoint & - translated vert in the sprite
************************************************************************/
CCollisionVertex & CCollisionSprite2D::GetTransVert( uint index )
{
    if( index >= transVertVector.size() )
        throw NExcept::CCriticalException("Index out of range",
            boost::str( boost::format("Index out of range of vector size (%d,%d).\n\n%s\nLine: %s") % index % transVertVector.size() % __FUNCTION__ % __LINE__ ));

    return transVertVector[index];

}   // GetTransVert */


/************************************************************************
*    desc:  Get the vector of all edges surrounding the sprite
*  
*    ret:	vector<CEdge *> & - outer edge vector
************************************************************************/
std::vector<CEdge *> & CCollisionSprite2D::GetOuterEdgeVector()
{
    return pOuterEdgeVector;

}   // GetOuterEdgeVector */


/************************************************************************
*    desc:  Get an outer edge in the sprite
*
*	 param: int index - index into the outer edges
*  
*    ret:	CEdge * - outer edge in the sprite
************************************************************************/
CEdge * CCollisionSprite2D::GetOuterEdge( uint index )
{
    if( index >= pOuterEdgeVector.size() )
        throw NExcept::CCriticalException("Index out of range",
            boost::str( boost::format("Index out of range of vector size (%d,%d).\n\n%s\nLine: %s") % index % pOuterEdgeVector.size() % __FUNCTION__ % __LINE__ ));

    return pOuterEdgeVector[index];

}   // GetOuterEdge */


/************************************************************************
*    desc:  Get the translated vert vector
*  
*    ret:	vector<CPoint *> & - translated vert vector
************************************************************************/
std::vector<CCollisionVertex> & CCollisionSprite2D::GetTransVertVector()
{
    return transVertVector;

}   // GetTransVertVector */


/************************************************************************
*    desc:  Get the number of faces in the sprite
*  
*    ret:	int - number of faces
************************************************************************/
size_t CCollisionSprite2D::GetFaceCount()
{
    return transFaceVector.size();

}   // GetFaceCount */


/************************************************************************
*    desc:  Get the number of vertices in the sprite
*  
*    ret:	int - number of vertices
************************************************************************/
size_t CCollisionSprite2D::GetVertCount()
{
    return transVertVector.size();

}   // GetVertCount */


/************************************************************************
*    desc:  Get the number of outer edges in the sprite
*  
*    ret:	int - number of outer edges
************************************************************************/
size_t CCollisionSprite2D::GetOuterEdgeCount()
{
    return pOuterEdgeVector.size();

}   // GetVertCount */


/************************************************************************
*    desc:  Get the support point of the sprite in relation to the passed
*			in normal. It's the vertex furthest along the direction of the
*			passed in normal
*
*	 param:	CPoint & normal - normal to apply dot product with
*  
*    ret:	CCollisionVertex * - support vertex
************************************************************************/
CCollisionVertex * CCollisionSprite2D::GetSupportVert( CPoint & normal )
{
    CWorldValue bestProjection(-INT_MAX, 0.f);
    CCollisionVertex * pBestVert;
 
    for( size_t i = 0; i < transVertVector.size(); ++i )
    {
        CCollisionVertex * tmpVert = &transVertVector[i];
        CWorldValue projection = NMathFunc::DotProduct2D( tmpVert->GetPos(), normal );
 
        if( projection > bestProjection )
        {
            pBestVert = tmpVert;
            bestProjection = projection;
        }
    }
 
    return pBestVert;

}	// GetSupportVert */


/************************************************************************
*    desc:  Get the collision mesh
*  
*    ret:	CCollisionMesh2D * - collision sprite's mesh
************************************************************************/
CCollisionMesh2D * CCollisionSprite2D::GetMesh()
{
    return pMesh;

}	// GetMesh */


/************************************************************************
*    desc:  Get the collision sprite's body
*  
*    ret:	CCollisionBody * - collision sprite's body
************************************************************************/
CCollisionBody & CCollisionSprite2D::GetBody()
{
    return colBody;

}	// GetBody */


/************************************************************************
*    desc:  Get the collision data
*
*    ret:	const CObjectCollisionData2D & - collision data
************************************************************************/
const CObjectCollisionData2D & CCollisionSprite2D::GetCollisionData() const
{
    return pObjectData->GetCollisionData();

}	// GetCollisionData */
