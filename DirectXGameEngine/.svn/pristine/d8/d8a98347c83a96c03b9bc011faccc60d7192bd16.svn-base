
/************************************************************************
*    FILE NAME:       jointanimationmesh3d.h
*
*    DESCRIPTION:     3D joint animation mesh class
************************************************************************/  

#ifndef __joint_animation_mesh_3d_h__
#define __joint_animation_mesh_3d_h__

// Physical component dependency
#include <3d/visualmesh3d.h>

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/scoped_array.hpp>

// Forward declaration(s)
class CJoint;

class CJointAnimMesh3D : public CVisualMesh3D
{
public:

    CJointAnimMesh3D();
    virtual ~CJointAnimMesh3D();

    // Load the mesh from XML file
    virtual void LoadFromRSS( const CObjectData3D * pObjData );

    // Get the joint array
    virtual CJoint * GetJoint( unsigned int index );

    // Get the joint count
    virtual unsigned int GetJointCount();

protected:

    // Allocated list of joints
    boost::scoped_array<CJoint> pJoint;

    // Joint list count - this is a sanity check that the same number of
    // joints are being used by the mesh and the animation
    unsigned int jointCount;
};

#endif  // __joint_animation_mesh_h__

