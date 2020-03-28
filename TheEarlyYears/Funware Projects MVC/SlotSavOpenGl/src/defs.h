
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
#define MAX_SYMBOLS                   12
#define MAX_SYMBOL_CYCLE              5
#define REEL_START_VEL				  -0.8f //-1.7f
#define REEL_STATR_ACC                0.10f //0.15f
#define REEL_MAX_SPEED                15
#define SPIN_DURATION                 1200
#define MAX_JOG_COUNT                 19
#define REEL_STOP_DELAY               400

#define SYMB_W					      155
#define SYMB_H				     	  142

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
	REEL_INIT_SPIN=0,
	REEL_START_SPIN,
	REEL_SPINNING,
	REEL_STOP,
	REEL_EVALUATE,
};

enum
{
	REEL1=0,
	REEL2,
	REEL3,
	REEL4,
	REEL5,
	REELS,
};

// Symbol movement struct
struct SSymb
{
	int index;
	int defPos;
	int vector;
};

struct SPhysicalReel
{
	int count;
	int phyReel[];
};

#endif  // __Defines_H__