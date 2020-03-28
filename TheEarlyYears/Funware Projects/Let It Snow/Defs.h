
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


#define CHECK_PLAYBACK_DONE_INTERVAL  7000L
#define MAX_PREVIEW_TIMER_COUNTS      2
#define MAX_NUMBER_OF_SONGS           51
#define NEIGHBORHOOD_W                1026
#define NEIGHBORHOOD_H                272
#define MAX_BLINK_NEIGHBORHOOD        7
#define MIN_BLINK_TIMEOUT_ON          3000L
#define RANDOM_BLINK_AMOUNT_ON        2000L
#define MIN_BLINK_TIMEOUT_OFF         1000L
#define RANDOM_BLINK_AMOUNT_OFF       2000L
#define MAX_CYCLE_RENDER_WAIT         40L

#define STAR_ARTX                     1021
#define STAR_ARTY                     728
#define STAR_ARTW                     82
#define STAR_ARTH                     75
#define STAR_DESTX                    40
#define STAR_DESTY                    40
#define MIN_STAR_BLINK_TIMEOUT_ON     200L
#define RANDOM_STAR_BLINK_AMOUNT_ON   400L
#define MIN_STAR_BLINK_TIMEOUT_OFF    4000L
#define RANDOM_STAR_BLINK_AMOUNT_OFF  3000L
#define MAX_DIFFERENT_SNOW            7
#define MAX_PIXEL_SPEEDSX             3
#define MAX_PIXEL_SPEEDSY             3
#define MIN_SNOW_BLINK                200L
#define RANDOM_SNOW_BLINK             400L
#define MAX_PEOPLE_ANIM               5
#define MAX_BOTTOM_EDGE               20L
#define MIN_BOTTOM_EDGE               20L
#define PEOPLE_PICK_TIME_MIN          5000L
#define PEOPLE_PICK_TIME_MAX          10000L
#define MAX_SMOKE_NEIGHBORHOOD        4
#define MAX_SMOKE_FRAMES              8
#define MAX_DIFFERENT_SMOKE           3
#define SMOKE_ANIM_DELAY              160
#define MAX_SANTA_FRAMES              7
#define SANTA_PIXEL_SPEED             2
#define SANTA_ANIM_DELAY              100
#define MAX_SANTA_START_DELAY         50000
#define MIN_SANTA_START_DELAY         20000

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
	BLINKER_OFF=0,
	BLINKER_ON,
	MAX_BLINKER_STATES,
};

enum
{
    MOVE_LEFT = 0,
    MOVE_RIGHT,
    MAX_NUM_OF_MOVEMENTS,
};

struct TSnow
{
    RECT DestRect;
    RECT ArtBlinkOn;
	RECT ArtBlinkOff;
    int PixelSpeedX;
	int PixelSpeedY;
	DWORD BlinkTimer;
	DWORD TimerInterval;
	int BlinkState;
};
typedef TSnow *PTSnow;

struct SStaticAnimRect
{
	int artX;
	int artY;
	int width;
	int height;
	int destX;
	int destY;
};

struct SStaticDest
{
	int destX;
	int destY;
};

struct TBlinker
{
    RECT DestRect;
    RECT ArtBlinkOn;
	RECT ArtBlinkOff;
    DWORD BlinkTimer;
    DWORD TimerIntervalOn;
	DWORD TimerIntervalOff;
	BOOL ChangeState;
	int BlinkState;
};
typedef TBlinker *PTBlinker;

// People Animations
struct SPeople
{
    RECT Dest;
    RECT Source;
    RECT Art;
    BOOL Active;
    int Movement;
    int NumFrames;
    int PixelSpeed;
    DWORD AnimTimer;
    DWORD AnimDelay;
    DWORD WalkTimer;
    DWORD WalkDelay;
    int FrameCounter;
};

// Smoke animations
struct SSmoke
{
    RECT Dest;
    RECT Source;
    int FrameCounter;
};
typedef SSmoke *PSSmoke;

struct SSanta
{
    RECT Dest;
    RECT Source;
	RECT Art;
    BOOL Active;
    int Movement;
    DWORD AnimTimer;
    DWORD AnimDelay;
    int FrameCounter;
};


#endif  // __Defines_H__