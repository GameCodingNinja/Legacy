/************************************************************************
*    FILE NAME:       collisionmesh2d.h
*
*    DESCRIPTION:     Emulated 2D collision mesh class
************************************************************************/  

#ifndef __collisionmesh_2d_h__
#define __collisionmesh_2d_h__

// Standard lib dependencies
#include <string>
#include <vector>

// Game lib dependencies
#include <common/size.h>
#include <common/matrix.h>
#include <common/face2d.h>
#include <common/collisionvertex.h>
#include <common/worldpoint.h>
#include <common/defs.h>

// Forward declarations
class CObjectData2D;


class CCollisionMesh2D
{
public:

    CCollisionMesh2D();
    ~CCollisionMesh2D();

    // Load the mesh from XML file (NOT FINISHED)
    void LoadFromXML( const std::string & filePath ){}

    // Load the mesh using a size
    void GenerateMesh( const CSize<float> & size, const int genType );

    // Generate the mesh using object data
    void GenerateMesh( const CObjectData2D * pData );

    // Load the mesh using the object data
    void LoadFromData( const CObjectData2D * pData );

    // Get the radius
    float GetRadius() const;

    // Get the radius squared
    float GetRadiusSquared() const;

    // Transform the mesh and apply the transformation to the passed in vertices and faces
    void Transform( std::vector<CCollisionVertex> & transVert, std::vector<CFace2D> & transFace, float scale,
                    const CMatrix & scaledMatrix, const CMatrix & unscaledMatrix, const CWorldPoint & point );

    // Get a face or the vector of faces
    const CFace2D & GetFace(size_t index );
    const std::vector<CFace2D> & GetFaceVector();

    // Get a vert or the vector of verts
    const CCollisionVertex & GetVert( size_t index );
    const std::vector<CCollisionVertex> & GetVertVector();

protected:

    // Find the unique edges in the mesh
    void FindUniqueEdges();

    // Vector of faces in the mesh
    std::vector<CFace2D> faceVector;

    // Vector of vertices in the mesh
    std::vector<CCollisionVertex> vertVector;

    // Radii
    float radius;
    float radiusSquared;
};

#endif  // __collisionmesh_2d_h__

