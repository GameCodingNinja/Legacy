/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CBone.h
*
*    DESCRIPTION:     Bone animation class
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __CBONE_H__
#define __CBONE_H__


class CBoneOffsets
{
public:

	float t, x, y, z;

    CBoneOffsets():t(0),x(0),y(0),z(0)
	{}
};

class CBone
{
public:

	// Bone Index
	int boneIndex;
	// Bone Parent
	int boneParent;
	// List Count
	int transRotListCount;
	// Translation offsets
	CBoneOffsets trans;
	// Rotation offsets
	CBoneOffsets rot;

	// Translation list
	CBoneOffsets *pTransList;
	// Rotation List
	CBoneOffsets *pRotList;

	// Constructer
	CBone();
	// Destructer
	~CBone();

	// Free all allocations
	void Free();
	// Allocate the lists
	bool AllocateLists( int count );
};

class CBoneList
{
public:

	// bone count
	int boneCount;
	// Bone list
	CBone *pBone;

	// Constructer
	CBoneList();
	// Destructer
	~CBoneList();

	// Free all allocations
	void Free();
	// Allocate the bone lists
	bool AllocateBoneList( int count );
};

#endif // __CBONE_H__