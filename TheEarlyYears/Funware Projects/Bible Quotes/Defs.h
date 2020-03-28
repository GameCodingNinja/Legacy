
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
#define MAX_MIL_RENDER_WAIT           60
#define MAX_CYCLE_RENDER_WAIT         50
#define MAX_RES_CHECK                 9

#define MAX_SANTA_DANCES              4
#define MAX_SANTA_FRAMES              11

#define CAP_ASCII_START				  65
#define UNDER_ASCII_START             97
#define NUM_ASCII_START				  48
#define MAX_MISC_CHAR                 11
#define MAX_TEXT_LINE                 400.0//400.0//440.0f
#define CHAR_SPACE                    15.0f
#define CHAR_LEADING                  0.0
#define LINE_HEIGHT                   50.0
#define ARIAL_Y_OFFSET                35.0
#define MAX_NUMBER_OF_SONGS           34

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
	FORMAT_TEXT=0,
	ANIM_BACK,
	ROTATE_OFF,
};

enum
{
	STR_QUOTE_COUNT=300,
	STR_QUOTE_START,
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
#endif  // __Defines_H__