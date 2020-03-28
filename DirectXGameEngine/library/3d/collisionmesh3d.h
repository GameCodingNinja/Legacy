
/************************************************************************
*    FILE NAME:       collisionmesh3d.h
*
*    DESCRIPTION:     3D collision mesh class
************************************************************************/  

#ifndef __collision_mesh_3d_h__
#define __collision_mesh_3d_h__

// Physical component dependency
#include <3d/mesh3d.h>

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/scoped_array.hpp>
#include <boost/noncopyable.hpp>

// Forward declaration(s)
class CFace;

class CCollisionMesh3D : public CMesh3D
{
public:

    CCollisionMesh3D();
    virtual ~CCollisionMesh3D();

    // Load the mesh from XML file
    virtual void LoadFromRSS( const CObjectData3D * pObjData );

    // Transform the mesh data
    virtual void Transform( CFace * pTransFace, float scale,
                            CMatrix & matrix, CMatrix & unscaledMatrix );

    // Get the face count
    virtual unsigned int GetFaceCount( unsigned int index = 0 );

protected:

    // Face list
    boost::scoped_array<CFace> spFace;

    // Number of faces loaded
    int faceCount;
};

#endif  // __collision_mesh_h__

