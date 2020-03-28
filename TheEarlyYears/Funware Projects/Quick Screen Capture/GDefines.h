
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
//#define _WIN32_WINNT 0x0400

// Define packed structure, no padding
//#pragma pack(push, 1)

#define WND_CAPTION               "Howie's Quick Screen Capture v1.1.1"
#define PRODUCT_NAME              "Howie's Quick Screen Capture"
#define WEB_ADDRESS               "www.howiesfunware.com"
#define CHECK_FOR_KEYPRESS_TIMER   3
#define RUNNING_TIMER              5
#define DEFAULT_METERS_PER_INCH    2834
#define INI_FILE_NAME              "QScreenCapt.ini"
#define FILE_NAME_SIZE             100
#define MAIN_APP_TOOLS             "gtools.h"
#define MAX_SOUND_TIME             750

// Menu defines
#define DLG_QUIT_BOX              2  // Value sent when the little X box is clicked
#define MNU_EXIT                  100
#define MNU_WEB_LINK              101
#define MNU_PREFS                 102
#define MNU_HELP                  103

// Misc controls
#define EDT_FILE_PATH             200
#define EDT_FILE_NAME             201
#define BTN_SEL_DIR               202
#define BTN_CANCEL                203
#define BTN_OK                    204
#define CBO_FILE_SAVE_OPT         205
#define CBO_BIT_DEPTH_OPT         206
#define CHK_GRB_ACTIVE_WND        207
#define CBO_GRAB_KEY              208
#define STC_DISPLAY_INFO          209
#define BTN_DONE                  210
#define EDT_HELPFUL_HINT          211
#define CHK_DONT_SHOW_AGAIN       212
#define TXT_HLP_TITLE             213
#define CHK_SOUND_EFFECT	      214
#define CHK_TIMER_CAPTURE		  215
#define CBO_TIMER_HOURS			  216
#define CBO_TIMER_MINUTES		  217
#define CBO_TIMER_SECONDS		  218

// Windows 98 GetSystemMetrics #defines
#define SM_XVIRTUALSCREEN         76
#define SM_YVIRTUALSCREEN         77
#define SM_CXVIRTUALSCREEN        78
#define SM_CYVIRTUALSCREEN        79
#define SM_CMONITORS              80

// Other defines
#define MAX_TIMER_HOURS           25
#define MAX_TIMER_MINUTES         60
#define MAX_TIMER_SECONDS         60


// File Save Options
enum
{
    FORMAT_BMP=0,
    FORMAT_PNG,
    FORMAT_TIF,
	FORMAT_TIF_CMP,
    FORMAT_JPG_SUP,
    FORMAT_JPG_HI,
    FORMAT_JPG_GOOD,
    FORMAT_JPG_NOR,
    FORMAT_JPG_AVG,
    FORMAT_JPG_BAD,
	MAX_FILE_FORMATS,
};

// File bit depth
enum
{
    BIT_DEPTH_SYS_DEF=0,
    BIT_DEPTH_8,
    BIT_DEPTH_16,
    BIT_DEPTH_24,
};


#endif  // __GDefines__
