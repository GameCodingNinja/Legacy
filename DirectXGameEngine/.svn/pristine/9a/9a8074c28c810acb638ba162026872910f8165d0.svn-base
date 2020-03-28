/************************************************************************
*    FILE NAME:       visualmesh2d.h
*
*    DESCRIPTION:     Emulated 2D DirectX mesh class
************************************************************************/  

#ifndef __visualmesh_2d_h__
#define __visualmesh_2d_h__

// DirectX lib dependencies
#include <d3dx9.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Boost lib dependencies
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

// Game lib dependencies
#include <xmlParser/xmlParser.h>
#include <common/size.h>
#include <common/xface2d.h>
#include <common/defs.h>

// Forward declaration(s)
namespace NText { class CTextureFor2D; }
class CObjectData2D;

class CVisualMesh2D : boost::noncopyable
{
public:

    CVisualMesh2D();
    ~CVisualMesh2D();

    // Load the mesh from the object data
    void LoadFromData( const CObjectData2D * pData, const std::string & meshTag = std::string("") );
    
    // Render this mesh to the back buffer
    void Render( int frame = 0 );

    // Set and get a face
    CXFace2D & GetFace( int index );

    // Get the radius
    float GetRadius() const;

    // Get the radius
    float GetRadiusSquared() const;

    // Get the size of the mesh
    const CSize<float> & GetSize() const;

    // Get the material
    LPDIRECT3DTEXTURE9 GetMaterial( int i );

    // Get the texture
    NText::CTextureFor2D * GetTexture( int i );

    // Get the number of materials in the mesh
    size_t GetMaterialCount() const;

    // Get the number of faces in the mesh
    size_t GetFaceCount() const;

    // Updates the vertex buffer by copying the face vector
    void UpdateVertBuf();

protected:

    // Create the mesh
    void CreateMesh( const CObjectData2D * pData );

    // Create a scaled frame mesh
    void CreateScaledFrameMesh( const XMLNode & node );

    // Create from mesh data
    void CreateFromMeshData( const CObjectData2D * pData, const XMLNode & node );

    // Create a scaled frame corner
    void CreateQuad( const CPoint & vert,
                     const CSize<float> & vSize,
                     const CUV & uv,
                     const CSize<float> & uvSize,
                     const CSize<float> & textSize );

protected:

    // Vertex buffer
    LPDIRECT3DVERTEXBUFFER9 pVertBuf;

    // Vector copy of the faces in the vertex buffer
    std::vector<CXFace2D> faceVector;

    // Vector of textures for each frame in the mesh
    // NOTE: This class does not own these pointers.
    std::vector<NText::CTextureFor2D *> pTextureVector;

    // DirectX vertex format
    uint vertexForamtMask;

    // DirectX vertex data size
    uint vertexDataSize;

    // Square rooted and un-square rooted radius
    float radius;
    float radiusSquared;

    // Size of the mesh
    CSize<float> size;

};

#endif  // __visualmesh_2d_h__

