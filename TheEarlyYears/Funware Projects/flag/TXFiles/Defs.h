
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
#define MAX_RES_CHECK                 9
#define MAX_PREVIEW_TIMER_COUNTS      2
#define MAX_MIL_RENDER_WAIT           50
#define MAX_FLAG_FRAMES               20
#define FLAG_OFFSET_Z                 370
#define FLAG_ROTATE_X                 70

#define TIME_TEST_OFFSET_X			  -30
#define TIME_TEST_OFFSET_Y			  0
#define TIME_TEST_OFFSET_Z			  250

#define LIGHT_SOURCE_X				  271.0F
#define LIGHT_SOURCE_Y				  -279.0F
#define LIGHT_SOURCE_Z				  200.0F

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
    ATTACK_FLY_TOP,
    ATTACK_FLY_BOTTOM,
    ATTACK_FLY_SIDE,
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

struct SStar
{
    WORD X;
    WORD Y;
	WORD Color;
};

#endif  // __Defines_H__