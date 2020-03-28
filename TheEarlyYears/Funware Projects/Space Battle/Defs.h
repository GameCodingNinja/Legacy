
/************************************************************************
*
*    PRODUCT:         Screen Saver
*
*    FILE NAME:       Defs.h
*
*    DESCRIPTION:     Global defines for the screen saver
*
*    IMPLEMENTATION:  Header file for the project files 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __Defines_H__
#define __Defines_H__


#define CHECK_PLAYBACK_DONE_INTERVAL  7000
#define MAX_PREVIEW_TIMER_COUNTS      2
#define MAX_ATTACK_SHIPS              35
#define MAX_DEFEND_SHIPS              35
#define MAX_EMERGE_SHIPS              20
#define HALF_ATTACK_SHIPS             12
#define SHIP_MOVE_SIDE_TO_SIDE        56
#define SHIP_MOVE_UP_DOWN             24
#define MAX_FIGHT_SHIPS               35
#define MAX_OVERVIEW_ANIM             19
#define MAX_FLYBY_VIEWS               5
#define OFFSET_FROM_CAMERA            300
#define OFFSET_FROM_CAMERA_X          500
#define MAX_EXPLODE_PER_SHIP          2
#define MAX_CRASH_OFFSET              100
#define MAX_MIL_RENDER_WAIT           40
#define MAX_RES_CHECK                 14
#define MAX_CLOSE_UP_VIEWS            6

enum
{
    ERASE = 0,
    MOTION,
};

enum
{
    PLAY_MUSIC=0,
    RENDER_BUFFER,
    SHAREWARE_NOTICE,
    BLIT_TO_SCREEN,
    NUM_BLIT_OPERATIONS,
};

enum
{
    RES_NO_RES_SELECTED=-1,
    RES_320x200=0,
	RES_512x384,
    RES_640x480,
    RES_800x600,
	RES_960x600,
	RES_1024x600,
	RES_1024x640,
    RES_1024x768,
	RES_1088x612,
    RES_1152x864,
    RES_1280x720,
	RES_1280x768,
	RES_1280x800,
    RES_1280x960,
    RES_1280x1024,
	RES_1360x768,
	RES_1440x900,
    RES_1600x900,
    RES_1600x1024,
    RES_1600x1200,
    RES_1680x1050,
    RES_1800x1440,
    RES_1920x1080,	
	RES_1920x1200,
	RES_2048x1536,
    RES_3200x2400,
    RES_4000x3000,
    RES_6400x4800,
    MAX_RESOLUTION,
};

enum
{
    NO_SKIP_FRAMES=0,
    SKIP_1_FRAME,
    SKIP_2_FRAMES,
    SKIP_3_FRAMES,
    MAX_FRAME_SKIP,
};

// Scene enums
enum
{
    MOTHER_SHIP_FRONT_APPROACH=0,
    MOTHER_SHIP_BELLEY_PAN,
    MOTHER_SHIP_SIDE_CAMERA_PAN,
    ATTACK_SHIP_FLY_BY,
    MOTHER_SHIP_TOP_CAMERA_PAN,
    MOTHER_SHIP_BACK_CAMERA_PAN,
    APPROACH_MOTHER_SHIP,
    SWARM_MOTHER_SHIP,
    DEFENDER_SHIP_EMERGE,
    ATTACK_SCENES_START,
    OVER_VIEW_OF_ATTACK = ATTACK_SCENES_START,
    OVER_VIEW_OF_ATTACK_2,
    ATTACK_FLY_TOP_BOTTOM,
    ATTACK_FLY_SIDE,
    CLOSEUP_FLY_OVER,
    MAX_ANIMATED_SCENES,
};

// Media player objects
enum
{
    BACKGROUND=0,
    ATTACK_BLAST,
    DEFEND_BLAST,
    EXPLOSION,
    MAX_MEDIA_PLAYERS,
};

enum
{
	ATTACK_SHIP=0,
	DEFEND_SHIP,
};


#endif  // __Defines_H__