
/************************************************************************
*
*    PRODUCT:         Screen Saver Thread
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

// Define packed structure, no padding
#pragma pack(push, 1)

// Testing defines
//#define DEBUG
//#define FULL_SCREEN_DEBUG
//#define ON_OFF_TEST  // Causes the screen saver to turn on and off.

// For Debugging the view program
//#define VIEW_DEBUG

// Define the windows version
//#define _WIN32_WINNT 0x0400

// Define if this is the tool or the screen saver
// Needed for the installer too
#define SS2GO_TOOL

#define SEMAPHORE          "HowiesFunwareScreenSaversViewer"

#define PRODUCT_NAME           "Screen Savers To Go"

#define MAIN_APP_SETTINGS      "Settings.h"

#define INSTCLASSNAME         "S2GFunwareInstaller"

#define HELP_FILE_NAME        "\\SSToGo.hlp"
#define ORDER_FORM_TITLE      "Screen Savers To Go Order Form  www.howiesfunware.com"
#define ABOUT_CAPTION         "About Screen Savers To Go"
#define ABOUT_MSG_VIEW        "This screen saver was created using Howie's Funware, Screen Savers To Go software."
#define SHAREWARE_BUILD       "While Screen Savers To Go is in shareware mode, the screen savers that are built can only run on the computer that it is built on. To freely distribute the files Screen Savers To Go generates, you will need to register Screen Savers To Go by paying the registration fee. This can be done using the online order form at my web site at www.howiesfunware.com, or through the mail by using the built-in order form under the Help/Order Info menu."
#define PRO_EXAMPLE_CAPTION   "My Screen Saver"
#define PRO_EXAMPLE_EXE_NAME  "My Screen Saver.exe"
#define PRO_EXAMPLE_WEB       "www.howiesfunware.com"
#define WINDOW_NAME           "SSToGo"
#define FULL_VERSION_NUMBER   "210058-00249"
#define FAKE_VERSION_NUMBER   "L#!WC64&*j4#2!';Fc2bx^@%28"
#define INI_SECRET_REG_FILE   "ltmreg12.dlI"  // made to look like a dll

#define OPEN_FILE_CAPTION     "Open Screen Savers To Go Project File"
#define SAVE_FILE_CAPTION     "Save Screen Savers To Go Project File"
#define SAVEAS_FILE_CAPTION   "Save As Screen Savers To Go Project File"
#define PROJECT_FILE_MASK     "Project Files (*.s2g)\0*.s2g\0\0"
#define PROJECT_EXT           "s2g"

#define CONVERT_TO_REG_MODE_TXT "Registered Mode", "Your copy of the Screen Savers To Go program is now in Registered Mode. You'll notice it says Registered in the window's caption. Any screen saver you have created will still be in shareware mode. Load any existing screen saver project you may have created and click on the File menu, then the Save menu option, to convert your screen saver to Registered Mode. Only the newly-generated install file will install your screen saver in Registered Mode."

#define PRODUCT_COST          15.00
#define PRO_VER_NUM           "1.0.7"
#define PRODUCT_VERSION       "Ver 1.0.7"

// Future data array sizes
#define PRJPREF_FUTURE        20480
#define DATE_HEADER_FUTURE    20
#define PAGE_DATA_FUTURE      5120

// Change these to change the install messages
#define TYPEOFPROGRAM         "software"
#define TYPEOFPROGRAM2        "screen saver"

// Format IDs for checking when loading data
#define FORMAT_ID             0x962243C8
#define PERFS_CHECK_ID        0x10C6E0B1
#define PAGE_CHECK_ID         0xC63A4016


// Screen Saver Specific
#define PROGRAM PRODUCT_NAME
#define SHAREWAEW_TMP_RUN_MAX 200

#define CHECK_PLAYBACK_DONE_INTERVAL  7000


#define AOL_ADDRESS               "funware"
#define WEB_ADDRESS               "www.howiesfunware.com"
#define ONLINE_ORDER_WEB_ADDRESS  "https://www.howiesfunware.com/horder.html"
#define EMAIL_ADDRESS             "funware@aol.com"
#define CO_NAME                   "Howie's Funware"
#define CO_ADDRESS1               "1822 Forest Bend Lane"
#define CO_CITY                   "Keller"
#define CO_ZIP                    "76248"
#define REGISTERED_STR            "(Registered)"
#define SHAREWARE_STR             "(Shareware)"

#define PRO_EXAMPLE_ASK_4_REG_AFTER_PAGE 3

#define PAGE_FILE_TYPES        "ALL Bitmap Files (*.jpg,*.jpeg;*.gif,*.bmp)\0*.jpg;*.jpeg;*.gif;*.bmp\0JPEG Files (*.jpg,*.jpeg)\0*.jpg;*.jpeg\0Gif Files (*.gif)\0*.gif\0Windows Bitmap Files (*.bmp)\0*.bmp\0\0" 

#define SOUND_FILE_TYPES       "Sound Files (*.wav, *.mid, *.rmi)\0*.wav;*.mid;*.rmi\0WAVE Files (*.wav)\0*.wav\0MIDI Files (*.mid, *.rmi)\0*.mid, *.rmi\0\0"

#define DEFAULT_MUSIC_FILE_NAME "%sHowiesFunwareMusic%u.tmp"

// Order Form setup
#define ORDER_FORM_INFO        "To register %s ($%.2lf), fill out the order form, print it, then mail it in with your payment."
#define CREDIT_CARD_INFO       "To pay by credit card, fill out the information below. Please double-check your credit card number and expiration date before printing."
#define CREDIT_CARD_INFO_VIEW  "To pay by credit card through the mail, fill out the information below. Please be sure to fill out all the information correctly."
#define FLOPPY_DISK_INFO       "Include 3-1/2 floppy disk ($%.2lf extra)"
#define ORDER_FORM             "Order Form"
#define MAIL_ORDER_ONLY        "(Mail Order Only)"
#define MAX_OF_NAME            150
#define MAX_OF_ADDRESS         150
#define MAX_OF_CITY            70
#define MAX_OF_ZIP             50
#define MAX_OF_EMAIL           100
#define MAX_COPYRIGHT_INFO     300

#define ABOUT_MSG              "Howie's Funware Copyright 2001, All Rights Reserved  www.howiesfunware.com"
#define BITMAPS_TOO_BIG        "The bitmaps that are being added are rather big in size. This is just a friendly reminder in case you're not aware of that. Although bitmaps of any size can be used in this screen saver, there's a good chance that by creating screen savers with files this large, your final, downloadable executable will be larger than is necessary."

#define FIRST_TEMP_NUMBER      "  "

#define FULL_REG_VERSION       TRUE
#define SHAREWARE_VERSION      FALSE 

// Costs
#define SALES_TAX              0.08125
#define FLOPPY_DISK_COST       3.50

//Misc
#define ENABLE                 TRUE
#define DISABLE                FALSE
#define TOOLBARBMP             200
#define NUM_US_STATES          51
#define STATE_ORDER_INDEX		44
#define NO_COLOR_SET           0x7FFFFFFF
#define MAX_PROJECT_BMP        2
#define MAX_PROJECT_INFO_TXT   2
#define WM_MOUSEWHEEL          0x020A
#define SCROLL_BAR_PAGE_SIZE   20
#define VIEW_WND_FAT			6
#define VIEW_WND_X             6
#define VIEW_WND_Y             10
#define VIEW_WND_W             320
#define VIEW_WND_H             240
#define SS2GO_WND_W				630
#define SS2GO_WND_H				360
#define CTRL_OFFSET_X			10

#define MAX_VER_NO             11
#define MAX_TITLE              150
#define MAX_EXE_NAME           50
#define MAX_REG_NUM            30
#define MAX_CLICK_RECT         30

#define NUM_TBBUTTONS          6
#define NUM_TBSOUNDBUTTONS     3
#define TBBUTTON_HEIGHT        16
#define TBBUTTON_WIDTH         16
#define TBBUTTON_SPACER        10
#define TBBUTTON_ADDITION      6
#define MAX_STATUS_PANELS      3

#define MAX_PAGES_BEFOR_REG_NUM 4

#define STARTUP_INFO_TXT_FILE   "\\StartUpInfo.dat"
#define ORDERFRM_INFO_TXT_FILE  "\\OrderFrmInfo.dat"

#define ENABLE                 TRUE
#define DISABLE                FALSE

#define COMPRESSED_FILE_EXT    ".lzz"

#define VIEW_CLICK_WND_SIZE     13
#define DEFAULT_CAPTION_WIDTH   200
#define DEFAULT_CAPTION_HEIGHT  22
#define DEFAULT_CAPTION_Y       2

#define DEBUG_OFFSET   20
#define ENABLE         TRUE
#define DISABLE        FALSE
#define CENTER_WND_TO_PARENT  TRUE

#define VIEW_WND_OFFSET_X     100
#define VIEW_WND_OFFSET_Y     30

// Key Codes
#define RETURN_KEY              0xD
#define LINEFEED_KEY            0xA
#define NUM_0_KEY               0x30
#define NUM_9_KEY               0x39
#define BACKSPACE_KEY           0x8
#define DELETE_KEY              0x2E
#define PERIOD_KEY              0xBE
#define LEFT_ARROW_KEY          0x25
#define RIGHT_ARROW_KEY         0x27
#define UP_ARROW_KEY            0x26
#define DOWN_ARROW_KEY          0x28

// Data array sizes
#define MAX_INFO                200
#define MAX_STR_PATH            (MAX_PATH*2)  // Twice the string length just in case
#define MAX_REG_FEE             50
#define MAX_SALES_TAX           50
#define MAX_PREF_STRS           1
#define MAX_PAGE_STRS           4
#define SMALL_ARRAY_20          20
#define SMALL_ARRAY_50          50
#define SMALL_ARRAY_100         100
#define SMALL_ARRAY_200         200
#define MED_ARRAY_300           300
#define LARGE_ARRAY_500         500
#define MAX_PAGE_CAPTION        255
#define MAX_JUST_BTNS           3
#define MAX_STYLE_BTNS          2
#define MAX_BTN_GROUPS          2

#define COMPRESSED_DATA         TRUE
#define UNCOMPRESSED_DATA       FALSE
#define CENTER_WND_TO_PARENT    TRUE

#define TEXT_OFFSET             0.00005


////////////////////////////
//    Dialog Defines
////////////////////////////

// Command defines
#define MNU_FILE               100 
#define MNU_NEW                101
#define MNU_OPEN               102
#define MNU_SAVE               103
#define MNU_SAVEAS             104
#define MNU_CLOSE              105
#define MNU_EXIT               106
#define MNU_ABOUT              107
#define MNU_ORDER              108
#define MNU_PREFS              109
#define MNU_ADD_AVI            111
#define MNU_ADD_SOUND          112
#define MNU_ADD_MAIN_IMAGE     113
#define MNU_ADD_BK_IMAGE       114
#define MNU_DELETE_PAGE        115
#define MNU_MOVE_PAGE          116
#define MNU_PRINT_SETUP        117
#define MNU_PRINT              118
#define MNU_HLP_CONTENTS       119
#define MNU_HLP_INDEX          120
#define MNU_WEB_LINK           121
#define MNU_REMOVE_SOUND       122
#define MNU_BACKGROUND_COLOR   123
#define MNU_CHANGE_MAIN_IMAGE  124
#define MNU_RUN_SAVER          125
#define MNU_RUN_PREFS          126
#define MNU_RUN_INSTALLER      127
#define MNU_SOUND_ON		   128
#define MNU_TRANS_ON		   129
#define MNU_SIZE_ALL_IMAGES	   130
#define MNU_RANDOMIZE_MUSIC	   131
#define MNU_RANDOMIZE_SCREENS  132
#define MNU_FIND_SCREENSAVER   133
#define MNU_RANDOMIZE_BACKS    134

#define BTN_FLIP_LEFT          140
#define BTN_FLIP_RIGHT         141
#define BTN_SEP1               142
#define BTN_SOUND_PLAY         143
#define BTN_SOUND_PAUSE        144
#define BTN_SOUND_STOP         145


#define BTN_TXT_TOP_JUST_LEFT  170
#define BTN_TXT_TOP_JUST_CENT  171
#define BTN_TXT_TOP_JUST_RIGH  172
#define BTN_TXT_BOT_JUST_LEFT  173
#define BTN_TXT_BOT_JUST_CENT  174
#define BTN_TXT_BOT_JUST_RIGH  175

#define BTN_TOP_FONT_SET       180
#define BTN_TOP_FONT_CLR       181
#define BTN_BOT_FONT_SET       182
#define BTN_BOT_FONT_CLR       183

// Dialog box Defines
#define DLG_QUIT_BOX            2

// Button Defines
#define BTN_DONE               200
#define BTN_NO_PRINTER         201
#define BTN_OK                 202
#define BTN_CANCEL             203
#define BTN_ORDER_FORM         204
#define BTN_PRINT_ORDERFORM    205
#define BTN_PLAY_SOUND         206
#define BTN_ORDER_ONLINE       207
#define BTN_ORDER_THRO_MAIL    208
#define BTN_GOTO_WEB_SITE      209

// Text label control defines
#define TXT_REG_NUM            300
#define TXT_SHIP_HAND_COST     301
#define TXT_SHIP_HAND_STATIC   302
#define TXT_TITLE              303
#define TXT_IL_RESIDENT        304
#define TXT_TOTAL_COST         305
#define TXT_PRINTING           306
#define TXT_HLP_TITLE          307
#define TXT_MEDIA_DIR          308
#define TXT_ORDERING_INFO      309
#define TXT_PROGRAM_COST       310
#define TXT_CREDIT_CARD_INFO   311
#define TXT_STATE_TAX          312
#define TXT_ABOUT              313
#define TXT_CREDIT_CARD_EXP    314
#define TXT_CREDIT_CARD_NO     315

// Edit field defines
#define EDT_REG_NUMBER         400
#define EDT_HELPFUL_HINT       401
#define EDT_CITY               402
#define EDT_EMAIL              403
#define EDT_ADDRESS_2          404
#define EDT_ADDRESS_1          405
#define EDT_NAME               406
#define EDT_CREDIT_CARD_NO     407
#define EDT_EXP_DATE           408
#define EDT_APP_TITLE          409
#define EDT_EXE_NAME           410
#define EDT_CUST_PASS          411
#define EDT_USE_PASS_AFTER     412
#define EDT_INFO_NAME          413
#define EDT_INFO_ADDRESS1      414
#define EDT_INFO_ADDRESS2      415
#define EDT_EMAIL_ADDRESS      417
#define EDT_SALES_TAX          418
#define EDT_REG_FEE            419
#define EDT_INFOWND_TITLE      420
#define EDT_INFOWND_TEXT       421
#define EDT_INFOWND_CAPTION    422
#define EDT_ZIP                423
#define EDT_COPYRIGHT_INFO     424
#define EDT_TEXT_WND           425
#define EDIT_CAPTION           425
#define EDT_CREDIT_URL	       426

// Check box defines
#define CHK_DONT_SHOW_AGAIN    500
#define CHK_FLOPPY_DISK        502
#define CHK_CHECKSUM           503
#define CHK_USE_REGNO          504
#define CHK_VISA               507
#define CHK_MASTERCARD         508
#define CHK_DISCOVER           509
#define CHK_AMERICAN_EXP       510
#define CHK_SIZE_TO_FIT        511

// Radio Buttons
#define RDOBTN_VISA            600
#define RDOBTN_MASTERCARD      601
#define RDOBTN_DISCOVER        602
#define RDOBTN_AMER_EXP        603

// Misc
#define SHOW_HELPHINT_WND      900
#define GRP_CREDIT_CARDS       901
#define VIEW_WND               902
#define CBO_STATE              903
#define POST_MSG_AFTER_WM_SHOW 904
#define ID_TOOL_BAR            905
#define CBO_MOVE_PAGE          906
#define SOUND_CLICK_WND        907
#define CAPTION_CLICK_WND      908
#define STATUS_WND	           909
#define STC_PICTLOAD_IMAGE_WND 910
#define STC_PICTLOAD_INFO	   911
#define STC_VIEW_WND           912
#define STC_TOTAL_PAGES        913
#define BTN_SYSTEM_QUIT        2
#define PROG_BAR	           914
#define STC_PROGBAR_TXT	  	   915
#define STC_LOOK_FOR		   916


////////////////////////////
//    Enumerations
////////////////////////////

// String Table enums
enum
{
    REG_NUM_INFO_TEXT_VIEW_PROG=50,
    REG_NUM_INFO_TEXT_TOOL,
    REG_NUM_ERROR,
    REG_NUM_THANK_YOU,
};


enum
{
    PAGE_FLIP_LEFT=0,
    PAGE_FLIP_RIGHT,
};

enum
{
    NO_PAGE_TYPE=0,
    PAGE_TYPE_MAIN,
	PAGE_TYPE_RELOAD_MAIN,
	PAGE_TYPE_BACK,
    PAGE_TYPE_SOUND,
    PAGE_TYPE_AVI,
};

enum
{
	EDT_TEXT_TOP_MSG=0,
	EDT_TEXT_BOTTOM_MSG,
	MAX_EDT_TEXT,
};

enum
{
	EDT_CUR_PAGE=0,
	EDT_DELAY_IN_SEC,
	MAX_EDT_CONTRL,
};

enum
{
	BTN_JUST_TOP_LEFT=0,
	BTN_JUST_TOP_CENT,
	BTN_JUST_TOP_RIGH,
	BTN_JUST_BOT_LEFT,
	BTN_JUST_BOT_CENT,
	BTN_JUST_BOT_RIGH,
	MAX_JUST_BTN,	
};

enum
{
    ERASE = 0,
    MOTION,
};

enum
{
	PLAY_MUSIC = 0,
	LOAD_GRAPHICS,
	BLIT_TO_BUFFER,
	RESET_TRANSITION,
	BLIT_TO_SCREEN,
	MAX_BLIT_OPERATIONS,
};

enum
{
	BLIT_BUFFER_TO_SCREEN=0,
	TIME_OUT_SCREEN,
	RESTART_OPERATION,
	MAX_BUFFER_OPERATIONS,
};

enum
{
	TOP_FONT_DLG=0,
	TOP_COLOR_DLG,
	BOT_FONT_DLG,
	BOT_COLOR_DLG,
	MAX_FONT_DLG,
};

enum
{
	STC_DELAY_LBL=0,
	STC_MSG_TOP_LBL,
	STC_MSG_BOT_LBL,
	MAX_STATIC_LABELS,
};

enum
{
	RANDOM_TRANS=0,
	VERT_LINES_TRANS,
	HORZ_LINES_TRANS,
	DIAG_LINES_TRANS,
	MAX_TRANSITIONS,
};

#endif  // __GDefines__