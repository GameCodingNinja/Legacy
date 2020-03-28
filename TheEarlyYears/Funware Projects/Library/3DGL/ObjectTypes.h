/************************************************************************
*
*    FILE NAME:       ObjectTypes.h
*
*    DESCRIPTION:     Defines the type of collisions to check for
*
*    IMPLEMENTATION:  Header file for the project files 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __object_types_H__
#define __object_types_H__

enum EOT_OBJECT_TYPE
{
	EOT_NO_TYPE=0,
	EOT_MAP_ROOM,
	EOT_MAP_DOORWAY,
	EOT_MAX_TYPES
};

struct SObjectType
{
	char typeStr[EOT_MAX_TYPES][50];

	SObjectType()
	{
		strcpy( typeStr[EOT_NO_TYPE], "no_type" );
		strcpy( typeStr[EOT_MAP_ROOM], "map_room" );
		strcpy( typeStr[EOT_MAP_DOORWAY], "map_doorway" );
	}
};


#endif // __object_types_H__