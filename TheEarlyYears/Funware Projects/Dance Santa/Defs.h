
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
#define MAX_CYCLE_RENDER_WAIT         60
#define MAX_RES_CHECK                 9

#define MAX_SANTA_DANCES              12
#define MAX_SANTA_FRAMES              24
#define MAX_RUDOLF_DANCES             3
#define MAX_RUDOLF_FRAMES             16
#define MAX_PING_PONG                 20
#define MAX_SNOW					  900
#define Z_OFFSET                      700
#define Y_TOP_OFFSET                  1500
#define Y_BOT_OFFSET                  -700
#define MAX_Y_SNOW_HEIGHT             2200
#define HOUSE_Y_OFFSET				  -792
#define CHAR_X_OFFSET				  220
#define HOUSE_Z_OFFSET				  20
#define MAX_SNOW_X                    1700
#define MAX_SNOW_Z                    1500
// Offset the snow so it doesn't go through Santa or Rudolf
#define SNOW_CHAR_OFFSET_X			  300
#define SNOW_CHAR_OFFSET_Z			  50
#define SNOW_ROTATION_SPEED           12
#define SNOW_FLOAT_SPEED              12
#define MAX_SNOW_FLAKE_ART            5
#define MAX_LIGHT_DIS                 900000
#define MAX_Z_DIS                     400000
#define MAX_CAMERA_VIEWS              26
#define LIGHT_OFFSET_X                -600.0F
#define LIGHT_OFFSET_Y                -100.0F
#define LIGHT_OFFSET_Z                2000.0F
#define MAX_NUMBER_OF_SONGS           18
#define MAX_GLOW_COUNT				  64
#define NOSE_GLOW_OFFSET              404
#define MAX_NOSE_POLYS                39

// Camera Structure
struct SSnow
{
    float XPos, YPos, ZPos;
    int XRot, YRot, ZRot, artIndex;
    SSnow(){ artIndex = XRot = YRot = ZRot = 0; XPos = YPos = ZPos = 0.0F; }
};

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
	PING_POMG=0,
	LOOP,
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