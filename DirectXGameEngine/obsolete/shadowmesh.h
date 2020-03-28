
/************************************************************************
*    FILE NAME:       shadowmesh.h
*
*    DESCRIPTION:     3D shadow mesh class
************************************************************************/  

#ifndef __shadow_mesh_h__
#define __shadow_mesh_h__

// Standard lib dependencies
#include <string>

// Forward declaration(s)
class CFace;

// Physical component dependency
#include "mesh.h"

class CShadowMesh : public CMesh
{
public:

    CShadowMesh();
    virtual ~CShadowMesh();

    // Load the mesh from XML file
    virtual bool LoadFromXML( std::string & filePath );

    // Load the mesh from XML file
    virtual bool LoadFromRSS( std::string & filePath );

    // Transform the mesh data
    virtual void Transform( CFace * pTransFace,
                            CMatrix & matrix,
                            CMatrix & noScaleMatrix );

    // Get the face pointer
    CFace * GetFacePtr();

    // Get the face count
    virtual unsigned int GetFaceCount( unsigned int index = 0 );

protected:

    void Sort( CFace * pTransFace );

    // Face pointer
    CFace * pFace;

    // Number of faces loaded
    int faceCount;
};

#endif  // __shadow_mesh_h__

