
/************************************************************************
*    FILE NAME:       shadowjointanimationmesh.h
*
*    DESCRIPTION:     3D joint animation mesh class
************************************************************************/  

#ifndef __shadow_joint_animation_mesh_h__
#define __shadow_joint_animation_mesh_h__

// Standard lib dependencies
#include <map>
#include <string>

// Game lib dependencies
#include "jointmultilinklist.h"
#include "jointnode.h"

// Physical component dependency
#include "shadowmesh.h"

// Forward declaration(s)
class CPoint;
class CNormal;
class CJoint;
class CXVertBuff;
class CMeshAnim;

class CShadowJointAnimMesh : public CShadowMesh
{
public:

    CShadowJointAnimMesh();
    virtual ~CShadowJointAnimMesh();

    // Load the mesh from XML file
    virtual bool LoadFromRSS( std::string & _filePath );

    // Load the mesh and animtions from XML
    bool LoadMeshAndAnimFromXML( std::string & _filePath );

    // Load the mesh animation from XML file
    bool LoadAnimFromRSA( std::string & name, float fps, std::string & _filePath );

    // Set the animation
    CJointLinkLst<CJointNode *> * GetAnimation( std::string & name );

    // Calculate the surface normals
    virtual void CalcSurfaceNormal();

    // Load the mesh animation from XML file
    //bool LoadAnimFromXML( std::string & name, float speed, std::string & _filePath );

    // Load the mesh from XML file
    //virtual bool LoadFromXML( std::string & _filePath );

private:

    // Allocated list of joints
    CJoint * pJoint;

    // Joint list count - this is a sanity check that the same number of
    // joints are being used by the mesh and the animation
    unsigned int jointCount;

    // map of joint animation multu-link list
    std::map<std::string, CJointLinkLst<CJointNode *> *> jointLinkLst;
};

#endif  // __shadow_joint_animation_mesh_h__

