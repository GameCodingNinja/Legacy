
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

#define MAX_CYCLE_RENDER_WAIT         100
#define CHECK_PLAYBACK_DONE_INTERVAL  7000
#define MAX_PREVIEW_TIMER_COUNTS      2
#define MAX_BUFFERS                   3
#define MAX_BUF_INDEX_ARY             4
#define MAX_FIRE_ANIM                 9
#define MAX_TREE_LIGHT_ANIM           5
#define MAX_NUMBER_OF_SONGS           22
#define MAX_SANTA_EYES_ANIM           5
#define MAX_SANTA_EYE_FRAMES          8
#define MAX_SANTA_LEGS_ANIM           3
#define MAX_SANTA_LEG_FRAMES          4
#define MAX_CAT_SLEEP_ANIM            7
#define MAX_CAT_SLEEP_FRAMES          12
#define MAX_CAT_YAWN_ANIM             5
#define MAX_CAT_EYEOPEN_ANIM          4
#define MAX_CAT_EYEOPEN_FRAMES        20
#define MAX_CAT_YAWN_FRAMES           14
#define MAX_MOUSE_WALK_FRAMES         8
#define MAX_CAT_RANDOM_INDEXES        14
#define MAX_PRESENTS                  23
#define GRID_SIZE                     128
#define CLEAN_BUFF_UNITS_X            10L
#define CLEAN_BUFF_UNITS_Y            13L
#define CLEAN_BUFF_UNITS_W            5L
#define CLEAN_BUFF_UNITS_H            9L
#define BLIT_TREE_UNITS_X             21L
#define BLIT_TREE_UNITS_Y             4L
#define BLIT_TREE_UNITS_W             13L
#define BLIT_TREE_UNITS_H             22L

enum
{
    ERASE = 0,
    MOTION,
};

enum
{
	PLAY_MUSIC=0,
	ERASE_BUFFER,
	UPDATE_ANIM,
	BLIT_TO_BUFFER,
	BLIT_TO_SCREEN,
	NUM_BLIT_OPERATIONS,
};

enum
{
	STATE_CAT_SLEEP=0,
	STATE_CAT_YAWN,
	STATE_CAT_EYE_OPEN,
};


#endif  // __Defines_H__