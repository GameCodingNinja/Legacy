
/************************************************************************
*    FILE NAME:       meshmanager.h
*
*    DESCRIPTION:     Mesh manager class
************************************************************************/

#ifndef __mesh_manager_h__
#define __mesh_manager_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Game lib dependencies
#include <common/size.h>

// Forward declaration(s)
class CVisualMesh3D;
class CCollisionMesh3D;
class CJointAnimMesh3D;
class CVisualMesh2D;
//class CCollisionMesh2D;
class CMesh3D;
class CObjectData2D;
class CObjectData3D;

class CMeshMgr
{
public:

    // Get the instance of the singleton class
    static CMeshMgr & Instance()
    {
        static CMeshMgr meshMgr;
        return meshMgr;
    }

    // Load the mesh info from XML file
    CMesh3D * GetVisualMesh3D( const CObjectData3D * pObjData );
    CMesh3D * GetJointAnimMesh3D( const CObjectData3D * pObjData );
    CMesh3D * GetCollisionMesh3D( const CObjectData3D * pObjData );
    CVisualMesh2D * GetVisualMesh2D( const CObjectData2D * pObjData, const std::string & meshUniqueTag = std::string("") );
    //CCollisionMesh2D * GetCollisionMesh2D( const std::string groupName, const std::string & meshPath );
    //CCollisionMesh2D * GetCollisionMesh2D( const std::string groupName, const CSize<float> & size, int genType );
    //CCollisionMesh2D * GetCollisionMesh2D( const CObjectData2D * pObjData );

    // Delete functions
    void DeleteVisualMeshGroup3D( const std::string & group );
    void DeleteJointMeshGroup3D( const std::string & group );
    void DeleteCollisionMeshGroup3D( const std::string & group );
    void DeleteVisualMeshGroup2D( const std::string & group );
    //void DeleteCollisionMeshGroup2D( const std::string & group );

    // Dump out informatrion
    void Dump();

private:

    // Constructor
    CMeshMgr();

    // Destructor
    ~CMeshMgr();

    // map list of loaded meshes
    boost::ptr_map<const std::string, boost::ptr_map<const std::string, CVisualMesh3D>> spVisualMesh3DMapMap;
    boost::ptr_map<const std::string, boost::ptr_map<const std::string, CJointAnimMesh3D>> spJointMesh3DMapMap;
    boost::ptr_map<const std::string, boost::ptr_map<const std::string, CCollisionMesh3D>> spCollisionMesh3DMapMap;

    boost::ptr_map<const std::string, boost::ptr_map<const std::string, CVisualMesh2D>> spVisualMesh2DMapMap;
    //boost::ptr_map<const std::string, boost::ptr_map<const std::string, CCollisionMesh2D>> spCollisionMesh2DMapMap;

    // List of map iterators
    boost::ptr_map<const std::string, CVisualMesh3D>::iterator visualMesh3DMapIter;
    boost::ptr_map<const std::string, CCollisionMesh3D>::iterator collisionMesh3DMapIter;

    boost::ptr_map<const std::string, CJointAnimMesh3D>::iterator jointMesh3DMapIter;
    boost::ptr_map<const std::string, CVisualMesh2D>::iterator visualMesh2DMapIter;
    //boost::ptr_map<const std::string, CCollisionMesh2D>::iterator collisionMesh2DMapIter;

    boost::ptr_map<const std::string, boost::ptr_map<const std::string, CVisualMesh3D>>::iterator visualMesh3DMapMapIter;
    boost::ptr_map<const std::string, boost::ptr_map<const std::string, CJointAnimMesh3D>>::iterator jointMesh3DMapMapIter;
    boost::ptr_map<const std::string, boost::ptr_map<const std::string, CCollisionMesh3D>>::iterator collisionMesh3DMapMapIter;

    boost::ptr_map<const std::string, boost::ptr_map<const std::string, CVisualMesh2D>>::iterator visualMesh2DMapMapIter;
    //boost::ptr_map<const std::string, boost::ptr_map<const std::string, CCollisionMesh2D>>::iterator collisionMesh2DMapMapIter;

};

#endif  // __mesh_manager_h__
