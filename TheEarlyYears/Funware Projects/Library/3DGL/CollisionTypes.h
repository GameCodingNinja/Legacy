/************************************************************************
*
*    FILE NAME:       CollisionTypes.h
*
*    DESCRIPTION:     Defines the type of collisions to check for
*
*    IMPLEMENTATION:  Header file for the project files 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __collision_types_H__
#define __collision_types_H__

enum ECT_COLLISION_TYPE
{
	ECT_NO_TYPE=0,
	ECT_MAP_FLOOR,
	ECT_MAP_WALL,
	ECT_MAX_TYPES
};

struct SCollisionType
{
	char typeStr[ECT_MAX_TYPES][50];

	SCollisionType()
	{
		strcpy( typeStr[ECT_NO_TYPE], "no_type" );
		strcpy( typeStr[ECT_MAP_FLOOR], "map_floor" );
		strcpy( typeStr[ECT_MAP_WALL], "map_wall" );
	}
};


#endif // __collision_types_H__