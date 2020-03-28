/************************************************************************
*    FILE NAME:       visualmesh3d.h
*
*    DESCRIPTION:     3D DirectX mesh class
************************************************************************/  

#ifndef __visualmesh_h__
#define __visualmesh_h__

// Physical component dependency
#include <3d/mesh3d.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Boost lib dependencies
#include <boost/scoped_array.hpp>
#include <boost/noncopyable.hpp>

// Forward declaration(s)
class CXVertBuff;

class CVisualMesh3D : public CMesh3D
{
public:

    CVisualMesh3D();
    virtual ~CVisualMesh3D();

    // Load the mesh from RSS file
    virtual void LoadFromRSS( const CObjectData3D * pObjData );

    // Render this mesh to the back buffer
    virtual void Render();

    // Get the face group count
    virtual unsigned int GetFaceGroupCount();

    // Get the face count
    virtual unsigned int GetFaceCount( unsigned int index = 0 );

protected:

    // DirectX Vertex buffer list
    boost::scoped_array<CXVertBuff> spXVertBuf;

    // number of face lists
    unsigned int faceGrpCount;

    // DirectX vertex format
    unsigned int vertexForamtMask;

    // DirectX vertex data size
    unsigned int vertexDataSize;

};

#endif  // __visualmesh_h__

