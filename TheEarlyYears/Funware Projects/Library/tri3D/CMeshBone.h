/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CMeshBone.h
*
*    DESCRIPTION:     Complex polygon object class
*
*    IMPLEMENTATION:  For these classes, we are using a class more so
*                     as a container and not so much for following good
*                     object design. In 3D, speed is everything. We need
*                     fast access to all data which is why everything
*                     is made public.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __CMESH_TRI_BONE_H__
#define __CMESH_TRI_BONE_H__

#include "CMeshTri.h"         // Mesh triangle class
#include "CBone.h"

class CMeshBone : public CMeshTri
{
protected:

	// Number of bone Animations
	int boneAnimCount;
	// Number of bones
	int boneCount;
	// Bone Animation List
	CBoneList *pBoneAnimList;

public:

    CMeshBone();
    ~CMeshBone();

    // Free all allocations
    virtual void Free();
    // Load the polygon object file
    virtual bool LoadFromFile( char *filePath, bool calNormals = true );
    // Load an object from data pointer
    virtual bool LoadFromPointer( unsigned char *pData, bool calNormals = true );
};


#endif // __CMESH_TRI_BONE_H__