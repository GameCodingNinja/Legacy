/************************************************************************
*
*    FILE NAME:       types.h
*
*    DESCRIPTION:     Defines various types
*
************************************************************************/

#ifndef __types_H__
#define __types_H__

#include <string>
#include "directXBuildDef.h"

// This quiets the compiler warnings about a bunch of functions that Microsoft has
// decided to deprecate, even though the C++ standardization committee has not done so.
#pragma warning(disable : 4996)

#define TRI                     3
#define QUAD                    4
#define FORMAT_SPEC_STR_COUNT   20
#define MAX_CLIPPED_POINTS      6
#define SHADE_COUNT             32
#define COLOR_COUNT_16_BIT      0x8000
#define FIX_SHIFT_26_FLOAT      0x4000000
#define FIX_SHIFT_6_FLOAT       0x40
#define FIX_SHIFT_6_INT         6
#define FIX_SHIFT_26_INT        26
#define FIX_SHIFT_20_INT        20
#define FIX_SHIFT_23_INT        23
#define CEIL_FRACT             (FIX_SHIFT_20_INT-1)

#define DIVIDE_BY_16_SHIFT      4
#define RUN_LENGTH_16           16
#define SHIFT_15_MASK           0xFFFF8000  // Mask out the top 17 bits for a 32 bit shift int
#define SHIFT_16_MASK           0xFFFF0000  // Mask out the top 17 bits for a 32 bit shift int
#define SHIFT_23_MASK           0xFF800000  // Mask out the top 9 bits for a 32 bit shift int
#define FIX_SHIFT_16_FLOAT      0x10000
#define FIX_SHIFT_16_INT        16
#define PRECISION_Z_PLANE       -15
#define FIX_SHIFT_15_FLOAT      0x8000
#define FIX_SHIFT_15_INT        15

#define MAX_POLYGON_COUNT       600
#define MAX_VERTEX_BUFFER       1800  // = MAX_POLYGON_COUNT * 3

#define OBJECT_WITH_ALPHA_TXT   4       

enum EOT_OBJECT_TYPE
{
    EOT_NO_TYPE=0,		 // 0. Objects of no type that gets added to the mesh closest room
    EOT_MAP_ROOM,		 // 1. Object specified as a room
	EOT_MAP_HALLWAY,	 // 2. Object specified as a hallway
	EOT_MAP_STAIRWAY,	 // 3. Object specified as a stairway
    EOT_MAP_DOORWAY,	 // 4. Object specified as a doorway
	EOT_MAP_HALLDOORWAY, // 5. Object specified as a doorway
	EOT_MAP_OBJECT,		 //	Object that is associated with a room but is culled seperately from the room.
	EOT_MAP_OBJ_COL,	 // Object that has faces tagged for collision detection
	EOT_MAP_COLLISION,   // An object that is not drawn but is only used for collision detection.
	EOT_MAP_PLAYER_SPAWN_POINT, // player spawn point
	EOT_MAP_BOT_SPAWN_POINT, // spawn point
    EOT_MAX_TYPES
};

// File types
enum EN_BM_File_Type
{
    EN_BM_NO_FILE_TYPE=0,
    EN_BM_FILE_TYPE_BMP,
    EN_BM_FILE_TYPE_TGA,
    EN_BM_FILE_TYPE_JPG,
    EN_BM_FILE_TYPE_GIF,   
    EN_BM_FILE_TYPE_PNG,  // not supported
    EN_BM_FILE_TYPE_MNG,  // not supported
    MAX_EN_BM_FILE_TYPES
};

struct SObjectType
{
    std::string typeStr[EOT_MAX_TYPES];

    SObjectType()
    {
        typeStr[EOT_NO_TYPE] = "no_type";

		// Room types
        typeStr[EOT_MAP_ROOM] = "map_room";
		typeStr[EOT_MAP_HALLWAY] = "map_hallway";
		typeStr[EOT_MAP_STAIRWAY] = "map_stairway";

        // Doorway trpes
        typeStr[EOT_MAP_DOORWAY] = "map_doorway";
		typeStr[EOT_MAP_HALLDOORWAY] = "map_halldoorway";

        // Misc types
		typeStr[EOT_MAP_OBJECT] = "map_object";
		typeStr[EOT_MAP_OBJ_COL] = "map_object_collision";
		typeStr[EOT_MAP_COLLISION] = "map_collision";
		typeStr[EOT_MAP_PLAYER_SPAWN_POINT] = "player_spawn";
		typeStr[EOT_MAP_BOT_SPAWN_POINT] = "bot_spawn";
    }

	EOT_OBJECT_TYPE GetType( std::string &type )
	{
		EOT_OBJECT_TYPE result = EOT_NO_TYPE;

	    // find the type
	    for( int i = 0; i < EOT_MAX_TYPES; ++i )
	    {
	        if( typeStr[i] == type)
	        {
	            result = (EOT_OBJECT_TYPE)i;
	            break;
	        }
	    }

		return result;
	}
};

// Collision type
enum ECT_COLLISION_TYPE
{
    ECT_NO_COLLISION=-1,
	ECT_MAP_IGNOR,
    ECT_NO_TYPE,
    ECT_MAP_FLOOR,
    ECT_MAP_WALL,
    ECT_MAX_TYPES
};

struct SCollisionType
{
    std::string typeStr[ECT_MAX_TYPES];

    SCollisionType()
    {
        typeStr[ECT_MAP_IGNOR] = "map_ignor";
        typeStr[ECT_NO_TYPE] = "no_type";
        typeStr[ECT_MAP_FLOOR] = "map_floor";
        typeStr[ECT_MAP_WALL] = "map_wall";
    }

	ECT_COLLISION_TYPE GetType( std::string &type )
	{
		ECT_COLLISION_TYPE result = ECT_NO_COLLISION;

	    // find the type
	    for( int i = 0; i < ECT_MAX_TYPES; ++i )
	    {
	        if( typeStr[i] == type)
	        {
	            result = (ECT_COLLISION_TYPE)i;
	            break;
	        }
	    }

		return result;
	}
};

enum ERM_RENDER_DEVICE
{
    ERD_NO_RENDER_DEVICE=0,
    ERD_SOFTWARE,
    ERD_DIRECTX,
    ERD_OPENGL,
    ERD_MAX_RENDER_DEVICES  
};

enum EDM_DISPLAY_MODE
{
    EDM_NO_DISPLAY_MODE=0,
    EDM_WINDOWED,
    EDM_FULL_SCREEN,
    EDM_MAX_DISPLAY_MODE
};

class CMapType
{
public:

	// type of area currently stepping on
	EOT_OBJECT_TYPE activeType;
	// index of area we are currently steping on
    int activeIndex;

	CMapType()
	{
		activeType = EOT_NO_TYPE;
		activeIndex = 0;
	};
};

// simple class for holding the result of added up normals
// for creating vertex normals
class CNormAdd
{
public:

	long double x, y, z;

	CNormAdd():x(0.0),y(0.0),z(0.0)
	{}

    void Reset()
	{
		x = y = z = 0.0;
	}
};

#endif // __types_H__