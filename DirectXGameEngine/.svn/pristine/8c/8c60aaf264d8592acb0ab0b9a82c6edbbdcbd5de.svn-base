/************************************************************************
*    FILE NAME:       collisionsprite2d.h
*
*    DESCRIPTION:     2D collision sprite class
************************************************************************/

#ifndef __collisionsprite_2d_h__
#define __collisionsprite_2d_h__

// Physical component dependency
#include <common/object.h>

// Standard lib dependencies
#include <vector>
#include <string>

// Game lib dependencies
#include <common/matrix.h>
#include <common/size.h>
#include <common/face2d.h>
#include <common/edge.h>
#include <common/defs.h>
#include <common/collisionvertex.h>
#include <common/collisionbody.h>
#include <2d/collisionmesh2d.h>

// Forward Declarations
class CObjectData2D;
class CCollisionMesh2D;
class CObjectCollisionData2D;

class CCollisionSprite2D : public CObject
{
public:

    CCollisionSprite2D();
    CCollisionSprite2D( CObjectData2D * pObjData );
    virtual ~CCollisionSprite2D();

    // Init the sprite
    void Init( const std::string groupName, const std::string & meshPath );
    void Init( const std::string groupName, const CSize<float> & size, int genType = ECGT_DEFAULT );	
    
    // Is the sprite active
    bool IsActive();

    // Transform the sprite
    virtual void Transform();
    virtual void Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                            const CWorldPoint & point );
    virtual void TransformToWorld();

    // Get a face or translated face
    const CFace2D & GetFace( uint index );
    CFace2D & GetTransFace( uint index );
    std::vector<CFace2D> & GetTransFaceVector();

    // Get a translated vert or the translated vert vector
    CCollisionVertex & GetTransVert( uint index );
    std::vector<CCollisionVertex> & GetTransVertVector();

    // Get the vector of outer edges
    CEdge * GetOuterEdge( uint idex );
    std::vector<CEdge *> & GetOuterEdgeVector();

    // Get the number of faces in the sprite
    size_t GetFaceCount();

    // Get the number of vertices in the sprite
    size_t GetVertCount();

    // Get the number of outer edges in the sprite
    size_t GetOuterEdgeCount();

    // Get the support point of the sprite in relation to the passed in normal
    CCollisionVertex * GetSupportVert( CPoint & normal );

    // Get the mesh
    CCollisionMesh2D * GetMesh();
    
    // Get the collision sprite's body
    CCollisionBody & GetBody();

    // Get the object data components
    const CObjectCollisionData2D & GetCollisionData() const;

private:

    // Init the sprite
    void Init();

    // Copy the verts and faces of the mesh to the sprite
    void CopyMesh();

private:

    // Object's data. 
    // NOTE: This pointer belongs to the CObjectDataList2D singleton
    CObjectData2D * pObjectData;

    // Mesh that the sprite uses
    CCollisionMesh2D * pMesh;

    // Translated vertices
    std::vector<CCollisionVertex> transVertVector;

    // Translated faces
    std::vector<CFace2D> transFaceVector;

    // Outer edges of the sprite
    std::vector<CEdge *> pOuterEdgeVector;

    // The position of the collision sprite in the last frame
    CWorldPoint lastPos;

    // The collision body of the sprite used for physics calculations
    CCollisionBody colBody;
    
};

#endif  // __collisionsprite_2d_h__
