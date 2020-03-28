/************************************************************************
*
*    FILE NAME:       CPlayer.h
*
*    DESCRIPTION:     Player class
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "3D/CAnimMeshLst.h"    // Animation specific mesh class
#include "3D/CPathNode.h"       // Node class
#include "3D/CMapMgr.h"

/************************************************************************
*
*    Class for handling the player
*
************************************************************************/

class CPlayer
{
protected:

	// Matrix with opposite x rotations
    CMatrix oppositeRotMatrix;
	// Direction the player is facing
	CPoint dir;
	// Position of the player in the map
	CPoint pos;
	// first person animations
	CAnimMeshLst<CAnimMesh> anim;
	// Map type and index of the area the
	// object is located
    CMapType mapType;
	// Get last known node the player is by
	CPathNode * playerNode;
	// player move speed
    float moveSpeed;
	// player strafe speed
	float strafeSpeed;
	// player gravity
	float gravity;
	// player radious
	float radius;
	// player distance form wall
	float distFromWall;
	// player distance from floor
	float distFromFloor;

public:

	CPlayer();
    virtual ~CPlayer();

	// Save the rotation in the opposite direction
	void SetOppositeRotate( float x, float y, float z );
	// Update the player
	void Update( CMatrix &m, CMapMgr<CMeshMap> &map );
	// Get the pos of the player
	CPoint &GetPos(){ return pos; };
	// Get the dir of the player
	CPoint &GetDir(){ return dir; };
	// get the map type
	CMapType &GetMapType(){ return mapType; };
	// Get the last known node the player is near
	CPathNode * GetNode(){ return playerNode; };
	// Update the player position and direction
	void UpdatePosDir( CMatrix &m );
	// Find the closest path nde to the player
	void FindClosestPathNode( CMapMgr<CMeshMap> &map );
	// Load players spawn point
	bool LoadSpawnPoint( const std::string & tagFilename,
	                     CMatrix &matrix,
	                     CMapMgr<CMeshMap> &map );
	
	///////////////
	// get members
	///////////////
	
	float GetMoveSpeed(){ return moveSpeed; };
	float GetStrafeSpeed(){ return strafeSpeed; };
	float GetRadius(){ return radius; };
	float GetGravity(){ return gravity; };
	float GetDistFromWall(){ return distFromWall; };
	float GetDistFromFloor(){ return distFromFloor; };	
};


#endif  // _PLAYER_H_