
/************************************************************************
*
*    PRODUCT:         Resource Manager
*
*    FILE NAME:       GDefines.h
*
*    DESCRIPTION:     Global defines
*
*    IMPLEMENTATION:  Header file for the project files 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __GDefines_H__
#define __GDefines_H__


////////////////////////////
//    #define Section
///////////////////////////

// Define the windows version
#define _WIN32_WINNT 0x0400

// Define packed structure, no padding
//#pragma pack(push, 1)

//Misc
#define MAIN_APP_TOOLS           "gtools.h"
#define PRODUCT_CAPTION          "WMS MNG Script Animator v1.1.1"
#define PRODUCT_NAME             "WMS MNG Script Animator"
#define INI_FILE_NAME            "MNG_Animator.ini"
#define OPEN_FILE_CAPTION        "Open Project File Path"
#define PROJECT_FILE_MASK        "MNG Files (*.mng)\0*.mng\0\0"
#define PAGE_FILE_TYPES         "ALL Bitmap Files (*.png,*.jpg,*.jpeg;*.gif,*.bmp)\0*.png;*.jpg;*.jpeg;*.gif;*.bmp\0PNG Files (*.png)\0*.png\0JPEG Files (*.jpg,*.jpeg)\0*.jpg;*.jpeg\0Gif Files (*.gif)\0*.gif\0Windows Bitmap Files (*.bmp)\0*.bmp\0\0" 
#define PROJECT_EXT              "mng"
#define SCRIPT_EXT              ".sas"
#define SCRIPT_EXT2             "*.sas"
#define SCREEN_W                 800
#define SCREEN_H                 600
#define MAX_SCRIP_NAME_SIZE      100
#define ANIM_TIMER_HANDLE        3
#define MAX_ASTERICK			 11

// Key Codes
#define RETURN_KEY               0xD
#define LINEFEED_KEY             0xA
#define NUM_0_KEY                0x30
#define NUM_9_KEY                0x39
#define BACKSPACE_KEY            0x8
#define DELETE_KEY               0x2E
#define PERIOD_KEY               0xBE
#define LEFT_ARROW_KEY           0x25
#define RIGHT_ARROW_KEY          0x27
#define UP_ARROW_KEY             0x26
#define DOWN_ARROW_KEY           0x28

// Menu defines
#define PROGRAM_EXIT             2  // Value sent when the little X box is clicked
#define MNU_EXIT                 100
#define MNU_OPEN_PROJ_PATH       101
#define MNU_SAVE_ACTIVE_SCRIPTS  102
#define MNU_LOAD_BACKGROUND      103
#define MNU_HELP                 104
#define MNU_CLEAR_BACKGROUND     105
#define MNU_SET_SOUND            106
#define MNU_CLEAR_SOUND          107
#define MNU_PLAY_SOUNDS_ANIM     108

// MNG Animator control defines
#define STC_PREVIEW_INFO         200
#define LST_MNG_FRAMES           201
#define LST_MNG_FILES            202
#define LST_AVAILABLE_MNG        203
#define BTN_ADD_FRAME            204
#define BTN_PLAY_SCRIPTS         205
#define BTN_ADD_SCRIPT           206
#define EDT_FPS                  208
#define BTN_CREATE_SCRIPT        209
#define STC_PICTLOAD_INFO        210
#define STC_FRAMES_PER_SEC       211
#define BTN_OK                   212
#define BTN_CANCEL               213
#define EDT_GET_SCRIPT           214
#define STC_SCRIPT_PATH          215
#define CHK_GOST_PREV_FRAME      216
#define CBO_MOVE_PAGE            217
#define STC_PICTLOAD_IMAGE_WND   218
#define STC_MNG_PATH             219
#define LST_PLAY_OPTIONS		 220

// Sound dialog box controls
#define BTN_PLAY_SOUND           260
#define BTN_ADD_SND				 261

// Sound dialog	subclassed controls
#define EDT_SOUND_VOLUME		 280
#define EDT_CHANNEL				 281
#define LST_FRAME_SND			 282

enum
{
	EDT_VOLUME=0,
	EDT_CHAN,
	LST_SND,
	MAX_SND_SUBCLASS_CTRLS,
};

// Starting from this point, these
// are the subclassed controls
#define EDT_XOFSET               400
#define EDT_YOFSET               401
#define EDT_DISPLAY_COUNT        402
#define EDT_Z_ORDER              403
#define EDT_BASE_XOFST           404
#define EDT_BASE_YOFST           405
#define EDT_BASE_ZOFST           406
#define STC_PREVIEW_WND          407
#define STC_VIEW_WND             408
#define LST_ANIM_FRAMES          409
#define LST_SCRIPTS_TO_ANIM      410
#define LST_MNG_SCRIPT_FILES     411

enum
{
    EDT_FRAME_X=0,
    EDT_FRAME_Y,
    EDT_DIS_COUNT,
    EDT_Z_ORD,
    EDT_BASE_X,
    EDT_BASE_Y,
    EDT_BASE_Z,
    STC_PRE_VIEW,
    STC_VIEW,
    LST_FRAMES,
    LST_SCR_TO_ANIM,
    LST_SCRIPT_FILES,    
    MAX_SUBCLASS_CTRLS,
};

enum
{
	PLAY_FORWARD_STAY=0,
	PLAY_FORWARD_HIDE,
	PLAY_FORWARD_LOOP,
	PLAY_REVERSE_STAY,
	PLAY_REVERSE_HIDE,
	PLAY_REVERSE_LOOP,
	PLAY_PING_PONG,
};

#endif  // __GDefines__
