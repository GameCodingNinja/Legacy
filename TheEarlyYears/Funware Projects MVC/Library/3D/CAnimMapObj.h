/************************************************************************
*
*    PRODUCT:         Animated map Object
*
*    FILE NAME:       CAnimMapObj.h
*
*    DESCRIPTION:     Animated object that is map aware and move about
*                     the map. 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef _CANIM_MAP_H_
#define _CANIM_MAP_H_

#include "CAnimMeshLst.h"    // Animation specific mesh class


/************************************************************************
*
*    Class for handling lists of meshes
*
************************************************************************/

template <class mesh_type>
class CAnimMapObj : public CAnimMeshLst<mesh_type>
{
protected:

	// Map type and index of the area the
	// object is located
    CMapType mapType;
	// floor offset
	float floorOffset;
	// Direction the agent is facing
	CPoint dir;
	// Position of the agent in the map
	CPoint pos;
	

public:

	CAnimMapObj();
    virtual ~CAnimMapObj();

    // Init some data members
	void Init( float rad, float offset );
	// Get the map type
	CMapType &GetMapType(){return mapType; };
	// Get the pos of the bot
	CPoint &GetPos(){ return pos; };
	// Get the dir of the bot
	CPoint &GetDir(){ return dir; };
	
};


/************************************************************************
*    FUNCTION NAME:  Constructer                                                             
************************************************************************/
template <class mesh_type>
CAnimMapObj<mesh_type>::CAnimMapObj()
{
	floorOffset = 0.0f;
}


/************************************************************************
*    FUNCTION NAME:  Destructer                                                             
************************************************************************/
template <class mesh_type>
CAnimMapObj<mesh_type>::~CAnimMapObj()
{
}


/************************************************************************                                                             
*    DESCRIPTION: Set the map that's currently being used
*
*    float radt - radius of object for world view
*    float offset - object offset from floor
************************************************************************/
template <class mesh_type>
void CAnimMapObj<mesh_type>::Init( float rad, float offset )
{
    radius = rad;
	floorOffset = offset;

}   // Init

#endif  // _CANIM_MAP_H_
