
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
#define MAX_PREVIEW_TIMER_COUNTS      5
#define SHIP_MOVE_SIDE_TO_SIDE        56
#define SHIP_MOVE_UP_DOWN             24
#define ROATE_MAX					  35
#define MAX_VIEWS					  3
#define MAX_RES_CHECK                 9
#define MAX_MIL_RENDER_WAIT           30
#define MAX_FLIP_FLOP                 6
#define MAX_COLOR_VALUE_RANGE         50
#define COLOR_CHANGE_TIMEOUT          20000
#define SIDE_CAMERA_TIME              2000
#define FRONT_BACK_CAMERA_TIME        10000

enum
{
    ERASE = 0,
    MOTION,
};

enum
{
	ROTATE_CLOCK=0,
	ROTATE_COUNTER,
	MAX_ROTATION_TYPES,
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
    RES_400x300,
    RES_512x384,
    RES_640x480,
    RES_800x600,
    RES_1024x768,
    RES_1152x864,
    RES_1280x720,
    RES_1280x960,
    RES_1280x1024,
    RES_1600x900,
    RES_1600x1024,
    RES_1600x1200,
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


#endif  // __Defines_H__