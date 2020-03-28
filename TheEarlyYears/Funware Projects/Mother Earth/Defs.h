
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
#define MAX_MIL_RENDER_WAIT           50
#define MAX_CYCLE_RENDER_WAIT         50
#define MAX_RES_CHECK                 9

#define MAX_SANTA_DANCES              4
#define MAX_SANTA_FRAMES              11
#define EARTHS_AXIS_IN_DEGREES        23.4f
#define MAX_Z_OFFSETS                 4
#define CAMERA_SWITCH_TIMEOUT		  20000

#define RING_TEXT_W                   32
#define RING_TEXT_H                   64


// Poly
enum
{
	LOW_POLY_RES=0,
	HI_POLY_RES,
	MAX_POLY_RES,
};

// Shading
enum
{
	NO_SHADING=0,
	START_UP_SHADING,
	RUNTIME_DYNAMIC_SHADING,
	MAX_SHADING,
};

enum
{
    PLAY_MUSIC=0,
    RENDER_BUFFER,
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

struct SStar
{
    WORD X;
    WORD Y;
	WORD Color;
};

#endif  // __Defines_H__