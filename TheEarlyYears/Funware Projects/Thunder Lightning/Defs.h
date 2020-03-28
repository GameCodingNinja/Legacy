
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


#define CHECK_PLAYBACK_DONE_INTERVAL    7000
#define MAX_PREVIEW_TIMER_COUNTS        2
#define MAX_CITY                        3
#define MAX_CITY_LIGHTS_ON              7
#define MAX_CITY_LIGHTS_OFF             7
#define MAX_HIGHLIGHT                   9
#define MAX_LIGHTNING                   24
#define MAX_BLOCK_LIGHTS                20
#define MAX_MISC                        2
#define MAX_MULTI_LIGHT_HITS            2
#define LIGHTNING_DUR_TIMEOUT           700
#define RAND_LIGHTNING_TIMEOUT          3000
#define FLASH_TIMEOUT                   300
#define MAX_CYCLE_RENDER_WAIT           100
#define BLACKOUT_MIN_TIMEOUT            15000
#define BLACKOUT_MAX_TIMEOUT            25000
#define BLACKOUT_FLICK_COUNT            6
#define FLICKER_TIMEOUT                 100
#define MAX_ON_OFF_LIGHTS               6
#define CITY_LIGHT_TIMEOUT              2000 // 3000
#define MIN_NEIGHBORHOOD_LIGHT_TIMEOUT  500
#define MAX_NEIGHBORHOOD_LIGHT_TIMEOUT  700 //2000
#define MAX_NEIGHBORHOOD_ON_OFF_LIGHTS  18
#define MAX_RAIN_SHADES					6
#define LIGHTNING_SND_DELAY             300
#define MAX_DIAGONAL_RAIN               2
#define MAX_DELAY_TIL_NEXT_BLACKOUT     3000
#define GRID_SIZE                       64//128
#define CAR_MULTI                       2.5f
#define CAR_MULTI_MOVE					1.5f
#define MAX_CARS                        5
#define HIGH_DELAY                      120000

enum
{
    ERASE = 0,
    MOTION,
};

enum
{
    PLAY_MUSIC=0,
    UPDATE_ANIM,
    BLIT_TO_BUFFER,
    SHAREWARE_NOTICE,
    BLIT_TO_SCREEN,
    NUM_BLIT_OPERATIONS,
};

enum
{
	RAIN_STRAIGHT=0,
	RAIN_LEFT,
	RAIN_RIGHT,
	MAX_RAIN_DIR,
};

enum
{
	SET_RAIN_STRAIGHT=0,
	SET_DIAGONAL,
	SET_RANDOM_RAIN
};

enum
{
	RAIN_AMOUNT_SPRINKLE=0,
	RAIN_AMOUNT_SHOWER,
	RAIN_AMOUNT_HEAVY,
	RAIN_AMOUNT_DELUGE,
	RAIN_AMOUNT_MONSOON,
	MAX_RAIN_AMOUNT,
};

enum
{
	LIGHTNING_RARELY=0,
	LIGHTNING_NORMAL,
	LIGHTNING_OFTEN,
	LIGHTNING_LOTS,
	MAX_LIGHT_SET,
};

enum
{
	FLASH_WHITE=0,
	FLASH_RED_GRAY,   
	FLASH_GREEN_GRAY, 
	FLASH_BLUE_GRAY,  
	FLASH_YELLOW_GRAY,
	FLASH_PURPLE_GRAY,
	MAX_FLASH_SHADES,
};

enum
{
	SND_RAIN=0,
	SND_BLACK_OUT,
	MAX_REG_SOUNDS,
};

enum
{
	SND_LOW_RAIN=0,
	SND_LOW_BLACK_OUT,
	SND_LOW_THUNDER1,
	SND_LOW_THUNDER2,
	SND_LOW_THUNDER3,
	MAX_LOW_SOUNDS,
	MAX_LOW_THUNDER_SOUNDS=3,
};

enum
{
	SND_THUNDER1=0,
	SND_THUNDER2,
	SND_THUNDER3,
	SND_THUNDER4,
	SND_THUNDER5,
	SND_THUNDER6,
	SND_THUNDER7,
	MAX_SND_THUNDER,
};

enum
{
	SND_BACK_SIREN=0,
	SND_BACK2,
	SND_BACK3,
	SND_BACK4,
	SND_BACK5,
	SND_BACK6,
	SND_BACK7,
	MAX_BACK_SND,
};

struct SRain
{
	int X1;
	int Y1;
	int X2;
	int	Y2;
	int length;
	WORD color;
};
typedef SRain *PSRain;


#endif  // __Defines_H__