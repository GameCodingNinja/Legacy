
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
#define MAX_BUBBLE_ART				  64
#define MAX_PIXEL_SPEED               3
#define MAX_PIXEL_SPEED_FLOAT         7
#define MAX_PIXEL_SPEED_DOUBLED       6
#define MAX_POP_ART                   8
#define MIN_POP_DELAY                 2000L
#define MAX_POP_DELAY                 5000L
#define POP_COUNT_CYCLES              20
#define MAX_POP_SOUNDS                4
#define BUBLE_COUNT_MULTI             16
#define BUBLE_SPEED_MULTI             10
#define MAX_BUBBLE_INDEX              63
#define FLOAT_COUNT					  98L

enum
{
    ERASE = 0,
    MOTION,
};

enum
{
	BUB=0,
	POP,
};

enum
{
	ERASE_BUFFER=0,
	UPDATE_ANIM,
	BLIT_TO_BUFFER,
	SHAREWARE_NOTICE,
	BLIT_TO_SCREEN,
	NUM_BLIT_OPERATIONS,
};

typedef struct tagRECTS
{
    short int X;
    short int Y;
    short int W;
    short int H;
} RECTS, *PRECTS;

typedef struct tagTBubble
{
    RECT Dest;
    RECT Art[2];
    int PixelSpeedX;
	int PixelSpeedY;
	int PixelTrimX;
	int PixelTrimY;
	int PopCount;
	int ArtIndex;
	int FloatUpCounter;
	BOOL PopErase;
	BOOL ToPop;
} TBubble, *PTBubble;

#endif  // __Defines_H__