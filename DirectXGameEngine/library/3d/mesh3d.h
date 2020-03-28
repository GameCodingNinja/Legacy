/************************************************************************
*    FILE NAME:       mesh3d.h
*
*    DESCRIPTION:     3D mesh class
************************************************************************/  

#ifndef __mesh_3d_h__
#define __mesh_3d_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <common/matrix.h>
#include <3d/collision.h>
#include <3d/jointmultilinklist.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Forward declaration(s)
class CFace;
class CJoint;
class CJointNode;
class CObjectData3D;

class CMesh3D : boost::noncopyable
{
public:

    CMesh3D();
    virtual ~CMesh3D();

    // Load the mesh from XML file
    virtual void LoadFromRSS( const CObjectData3D * pObjData ) = 0;

    // Load the mesh animation from XML file
    virtual void LoadAnimFromRSA( std::string & name, float speed, std::string & animFilePath );

    // Get the animation
    virtual CJointLinkLst<CJointNode *> * GetAnimation( std::string & name );

    // Update the animation
    virtual void CalcSurfaceNormal();

    // Transform the mesh data
    virtual void Transform( CFace * pTransFace, float scale,
                            CMatrix & matrix, CMatrix & unscaledMatrix );

    // Transform the mesh data
    virtual void Transform( CFace * pTransFace,
                            CMatrix & matrix, CMatrix & unscaledMatrix );

    // Get the face group count
    virtual unsigned int GetFaceGroupCount();

    // Get the face count
    virtual unsigned int GetFaceCount( unsigned int index = 0 );

    // Render this mesh to the back buffer
    virtual void Render();

    // Get the radius
    float GetRadius();

    // Get the radius
    float GetRadiusSqrt();

    // Get the joint array
    virtual CJoint * GetJoint( unsigned int index );

    // Get the joint count
    virtual unsigned int GetJointCount();

protected:

    // Square rooted and un-square rooted radius
    float radius;
    float radiusSqrt;

};

#endif  // __mesh_h__