
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

#define MAX_CYCLE_RENDER_WAIT         30
#define CHECK_PLAYBACK_DONE_INTERVAL  7000L
#define FISH_PICK_TIME_MIN            500L
#define FISH_PICK_TIME_MAX            500L
#define MAX_FLOAT_MOVEMENTS           68L
#define MAX_BUB_FLOAT_MOVEMENTS       98L
#define MIN_REQUIRED_FISH             6L
#define MAX_UP_DOWN_OFFSET            34L
#define MAX_LEFT_RIGHT_OFFSET         10L
#define MAX_BOTTOM_EDGE               5L
#define MIN_BOTTOM_EDGE               10L
#define MAX_PREVIEW_TIMER_COUNTS      2L

#define BACK_COLOR_X                  1159L
#define BACK_COLOR_Y                  0L
#define BACK_COLOR_SIZE               128L
#define BOSS_FISH                     29L
#define BOSS_FISH_PICK_TIME_MIN       60000L
#define BOSS_FISH_PICK_TIME_MAX       60000L
#define MAX_FISH_PIXEL_SPEED          3L
#define MIN_ANIMATION_DELAY           80L
#define MIN_FLOAT_COUNTER             1L
#define DARK_FISH_OFFSET              138

#define SAND_BAR_X                    1055L
#define SAND_BAR_Y                    1913L
#define SAND_BAR_W                    256L
#define SAND_BAR_H                    44L
#define SAND_BAR_LOW_H                34L

#define BOTTOM_PROP_OFFSET            158L

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
    MOVE_LEFT = 0,
    MOVE_RIGHT,
    MAX_NUM_OF_MOVEMENTS,
};

struct TFishData
{
    SMALL_RECT Art;
    short FrameCount;
    short BottomOnly;
    short PixelSpeed;
    short AnimDelay;
    short FloatIncCount;
};

struct TFishDataArry
{
    short count;
    TFishData FishData[];
};
typedef TFishDataArry *PTFishDataArry;

struct TRectArry
{
    short count;
    SMALL_RECT Art[];
};
typedef TRectArry *PTRectArry;


struct TBubble
{
    RECT Dest;
    RECT Art;
    int PixelSpeed;
    int FloatCounter;
    int FloatIncCount;
    int FloatSpeed;
    short *YFloat;
};
typedef TBubble *PTBubble;


// Manages the fish data
struct TFish
{
    RECT Dest;
    RECT Source;
    RECT Art;
    BOOL Active;
    BOOL BottomOnly;
    int Movement;
    int NumFrames;
    int PixelSpeed;
    DWORD AnimTimer;
    DWORD AnimDelay;
    int FrameCounter;
    int FloatCounter;
    int FloatIncCount;
    int FloatIncCounter;
    short *YFloat;
};


#endif  // __Defines_H__