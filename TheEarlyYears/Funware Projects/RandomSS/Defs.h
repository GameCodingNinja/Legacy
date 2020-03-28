
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
#define MAX_CYCLE_RENDER_WAIT         30000
#define PREV_TXT_MAX                  3

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
	SHAREWARE_NOTICE,
	BLIT_TO_SCREEN,
	NUM_BLIT_OPERATIONS,
};

enum
{
	AFTER_1_MIN=0,
	AFTER_2_MIN,
	AFTER_3_MIN,
	AFTER_5_MIN,
	AFTER_10_MIN,
	AFTER_15_MIN,
	AFTER_20_MIN,
	AFTER_25_MIN,
	AFTER_30_MIN,
	AFTER_45_MIN,
	AFTER_1_HOUR,
	AFTER_2_HOURS,
	AFTER_3_HOURS,
	AFTER_4_HOURS,
	AFTER_5_HOURS,
	NEVER,	
};

enum
{
   BUTTON_CLICK=0,
   MOUSE_CLICK,
   CLOSE_APP,	
};


#endif  // __Defines_H__