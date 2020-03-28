
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
#define MAX_CYCLE_RENDER_WAIT         30

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


#endif  // __Defines_H__