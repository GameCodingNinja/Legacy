
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


// Define the windows version
//#define _WIN32_WINNT 0x0400

// Define packed structure, no padding
#pragma pack(push, 1)

// Define a padded structure
//#pragma pack(pop)

// Define if this is the tool or the viewer
#define EBOOK_TOOL

// This #define indicates the Picture Book version
//#define SHARE_THE_MEMORIES

// For Debugging the view program
//#define VIEW_DEBUG

// Forces file export for other view programs
//#define FORCE_FILE_EXPORT

// Application Specific
#ifdef SHARE_THE_MEMORIES
    #define PRODUCT_NAME          "Share the Memories"
    #define HELP_FILE_NAME        "\\SharetheMemories.hlp"
    #define ORDER_FORM_TITLE      "Share the Memories Order Form  www.howiesfunware.com"
    #define ABOUT_CAPTION         "About Share the Memories"
    #define ABOUT_MSG_VIEW        "This ePhoto Book was created using Howie's Funware, Share the Memories software."
    #define SHAREWARE_BUILD       "While Share the Memories is in shareware mode, the executables that are built can only run on the computer that it is build on and only on the C drive. To freely distrabute and use the executables Share the Memories generates, you will need to register Share the Memories by paying the registration fee. This can be done from my web site, www.howiesfunware.com or through the mail by using the built in order form under the Order Info menu."
    #define PRO_EXAMPLE_CAPTION   "My ePhoto Book"
    #define PRO_EXAMPLE_EXE_NAME  "My ePhoto Book.exe"
    #define PRO_EXAMPLE_WEB       "www.howiesfunware.com"
    #define WINDOW_NAME           "SharetheMemories"
    #define FULL_VERSION_NUMBER   "904113-19502"
    #define FAKE_VERSION_NUMBER   "L#!WC64&*j4#2!';Fc2bx^@%28"
    #define INI_SECRET_REG_FILE   "weqreg10.dlI"  // made to look like a dll
    #define UNKNOWN_COMANY_NAME   "ePhoto"
    #define UNKNOWN_PROGRAM_NAME  "eBook"
    #define OPEN_FILE_CAPTION     "Open Share the Memories Project File"
    #define SAVE_FILE_CAPTION     "Save Share the Memories Project File"
    #define SAVEAS_FILE_CAPTION   "Save As Share the Memories Project File"
	#define PROJECT_FILE_MASK     "Project Files (*.stm)\0*.stm\0\0"
	//#define PROJECT_EXT_MASK      "*.stm"
	#define PROJECT_EXT           "stm"

    #define INI_TOOL_FILE_CHECK   "ShareTheMemories_prefs.ini"
    #ifdef EBOOK_TOOL
    #define INI_FILE_NAME         INI_TOOL_FILE_CHECK
    #else
    #define INI_FILE_NAME         "STMView_prefs.ini"
    #endif

    #define PRODUCT_COST          15.00
    #define PRO_VER_NUM           "1.0.5"
    #define PRODUCT_VERSION       "Ver 1.0.5"

    // Future data array sizes
    #define PRJPREF_FUTURE        20480
    #define DATE_HEADER_FUTURE    20
    #define PAGE_DATA_FUTURE      5120

	// Format IDs for checking when loading data
	#define FORMAT_ID               0x962243C8
	#define PERFS_CHECK_ID          0x87D690F2
	#define PAGE_CHECK_ID           0xB6FF402E
#else
    #define PRODUCT_NAME          "eBook 4 Sale"
    #define HELP_FILE_NAME        "\\ebook4sale.hlp"
    #define ORDER_FORM_TITLE      "eBook 4 Sale Order Form             www.howiesfunware.com"
    #define ABOUT_CAPTION         "About eBook 4 Sale"
    #define ABOUT_MSG_VIEW        "This ebook was created using Howie's Funware, eBook 4 Sale software."
    #define SHAREWARE_BUILD       "While eBook 4 Sale is in shareware mode, the executables that are built can only run on the computer that it is build on and only on the C drive. To freely distrabute and use the executables eBook 4 Sale generates, you will need to register ebook 4 Sale by paying the registration fee. This can be done from my web site, www.howiesfunware.com or through the mail by using the built in order form under the Order Info menu."
    #define PRO_EXAMPLE_CAPTION   "My eBook"
    #define PRO_EXAMPLE_EXE_NAME  "My eBook.exe"
    #define WINDOW_NAME           "ebook4Sale"
    #define FULL_VERSION_NUMBER   "192799-23446"
    #define FAKE_VERSION_NUMBER   "Z#!EC6U&*j4#2!';Fc28x^@%28"
    #define INI_SECRET_REG_FILE   "regsaf10.dlI"  // made to look like a dll
    #define UNKNOWN_COMANY_NAME   "Unknown Vender"
    #define UNKNOWN_PROGRAM_NAME  "Unknown Program Name"
    //#define OPPS_NEED_TO_REGISTER "Opps, need to register eBook 4 Sale"
    //#define SORRY_CANT_USE_EXE    "Sorry, you can't use the executables eBook 4 Sale generates unless you are a registered customer."
    #define OPEN_FILE_CAPTION     "Open eBook 4 Sale Project File"
    #define SAVE_FILE_CAPTION     "Save eBook 4 Sale Project File"
    #define SAVEAS_FILE_CAPTION   "Save As eBook 4 Sale Project File"
	#define PROJECT_FILE_MASK     "Project Files (*.e4s)\0*.e4s\0\0"
	//#define PROJECT_EXT_MASK      "*.e4s"
	#define PROJECT_EXT           "e4s"

    #define INI_TOOL_FILE_CHECK   "eBook4Sale_prefs.ini"
    #ifdef EBOOK_TOOL
    #define INI_FILE_NAME         INI_TOOL_FILE_CHECK
    #else
    #define INI_FILE_NAME         "eBookView_prefs.ini"
    #endif

    #define PRODUCT_COST          50.00
    #define PRO_VER_NUM           "1.0.5"
    #define PRODUCT_VERSION       "Ver 1.0.5"

    // Future data array sizes
    #define PRJPREF_FUTURE        20480
    #define DATE_HEADER_FUTURE    20
    #define PAGE_DATA_FUTURE      5120

	// Format IDs for checking when loading data
	#define FORMAT_ID               0x562143C7
	#define PERFS_CHECK_ID          0x87D690F2
	#define PAGE_CHECK_ID           0xB6FF402E
#endif

#define OPPS_NEED_TO_REGISTER "Sorry, Shareware Restriction"
#define SORRY_CANT_USE_EXE    "This executable is restricted to the C drive of the computer it was built on. This software needs to be registered."


// Change these to change the install messages
#define TYPEOFPROGRAM             "software"
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

#define PRO_EXAMPLE_REG_NUM    "MyRegNumber-R53WE-02"
#define PRO_EXAMPLE_ASK_4_REG_AFTER_PAGE 3

#define PAGE_FILE_TYPES   "Bitmap & TXT Files (*.jpg,*.gif,*.bmp,*.txt)\0*.jpg;*.jpeg;*.gif;*.bmp;*.txt\0\0" 
//#define PAGE_FILE_EXT	  "*.jpg;*.jpeg;*.gif;*.bmp;*.txt"

#define SOUND_FILE_TYPES  "Sound Files (*.mp3, *.wav, *.mid, *.rmi)\0*.mp3;*.wav;*.mid;*.rmi\0\0" 
//#define SOUND_FILE_EXT	  "*.wav;*.mid;*.rmi"
#define BITMAP_FILE_TYPES "Bitmap Files (*.jpg,*.gif,*.bmp)\0*.jpg;*.gif;*.bmp\0\0"

#define DEFAULT_MUSIC_FILE_NAME "%sHowiesFunwareMusic%u.tmp"

// Order Form setup
#define ORDER_FORM_INFO        "To register the %s ($%.2lf), fill out the order form, print it, then mail it in with your payment."
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
#define BITMAPS_TOO_BIG        "The bitmaps that are being added are rather big in size. This is just a friendly reminder incase you're not aware. Bitmaps of any size can be used but there's a good chance you may not need them to be this big because your final executable could end up being huge."

#define FIRST_TEMP_NUMBER      "Not Available"

#define FULL_REG_VERSION       TRUE
#define SHAREWARE_VERSION      FALSE

#define MAIN_APP_HEADER        "ebook.h"
#define MAIN_APP_TOOLS         "gtools.h"
#define MAIN_APP_SETTINGS      "Settings.h" 

// Costs
#define SALES_TAX               0.08125
#define FLOPPY_DISK_COST        3.50

//Misc
#define SEMAPHORE               "HowiesFunwareArt4Sale"
#define FIVE_SECONDS            5000
#define ENABLE                  TRUE
#define DISABLE                 FALSE
#define TOOLBARBMP              200
#define NUM_US_STATES           51
#define STATE_ORDER_INDEX       44
#define NO_COLOR_SET            0x7FFFFFFF
#define MAX_PROJECT_BMP         2
#define MAX_PROJECT_INFO_TXT    2
#define WM_MOUSEWHEEL           0x020A
#define SCROLL_BAR_PAGE_SIZE    20
#define GET_FILE_DROP_COUNT     0xFFFFFFFF

#define MAX_VER_NO              11
#define MAX_TITLE               150
#define MAX_EXE_NAME            50
#define MAX_REG_NUM             30
#define MAX_CLICK_RECT          30

#define NUM_TBBUTTONS           8
#define NUM_TBSOUNDBUTTONS      3
#define TBBUTTON_HEIGHT         16
#define TBBUTTON_WIDTH          16
#define TBBUTTON_SPACER         10
#define TBBUTTON_ADDITION       7

#define MAX_PAGES_BEFOR_REG_NUM 4
#define ZOOM_SIZE_COUNT         18
#define ZOOM_100_PERCENT_INDEX  7
#define ZOOM_100_PERCENT_INDEX_STR  "7"

#define STARTUP_INFO_TXT_FILE   "\\StartUpInfo.dat"
#define ORDERFRM_INFO_TXT_FILE  "\\OrderFrmInfo.dat"

#define ENABLE                  TRUE
#define DISABLE                 FALSE

#define COMPRESSED_FILE_EXT     ".lzz"

#define VIEW_CLICK_WND_SIZE     13
#define DEFAULT_CAPTION_WIDTH   200
#define DEFAULT_CAPTION_HEIGHT  22
#define DEFAULT_CAPTION_Y       2

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
#define MAX_PAGE_CAPTION        250

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
#define MNU_BUILD_EXE          110
#define MNU_ADD_BK_IMAGE       111
#define MNU_ADD_AVI            112
#define MNU_ADD_SOUND          113
#define MNU_ADD_PAGE           114
#define MNU_ADD_STARTUP_INFO   115
#define MNU_ADD_OF_INFO        116
#define MNU_DELETE_PAGE        117
#define MNU_MOVE_PAGE          118
#define MNU_PRINT_SETUP        119
#define MNU_PRINT              120
#define MNU_HLP_CONTENTS       121
#define MNU_HLP_INDEX          122
#define MNU_WEB_LINK           123
#define MNU_REST_PROJECTS      124
#define MNU_RUN_PROJECT        125
#define MNU_SET_DEF_FONT       126
#define MNU_SET_BKG_COLOR      127
#define MNU_RESET_FONT_DEF     128
#define MNU_ADD_CAPTION        129
#define MNU_REMOVE_SOUND       130
#define MNU_EXPORT_PAGE_FILE   131
#define MNU_EXPORT_SOUND_FILE  132
#define MNU_PLAY_ON_PAGE_TURN  133
#define MNU_AUTO_SIZE		   134
#define MNU_FIND_PROJECT       135


#define BTN_FLIP_LEFT          150
#define BTN_FLIP_RIGHT         151
#define BTN_ZOOM_IN            152
#define BTN_ZOOM_OUT           153
#define BTN_SEP1               154
#define BTN_SOUND_PLAY         155
#define BTN_SOUND_PAUSE        156
#define BTN_SOUND_STOP         157
#define BTN_GOTO_WEB_SITE	   158
#define BTN_SYSTEM_QUIT        2

// Dialog box Defines
#define DLG_QUIT_BOX            2

// Button Defines
#define BTN_DONE               100
#define BTN_NO_PRINTER         101
#define BTN_OK                 102
#define BTN_CANCEL             103
#define BTN_ORDER_FORM         104
#define BTN_PRINT_ORDERFORM    105
#define BTN_PLAY_SOUND         106
#define BTN_ORDER_ONLINE       107
#define BTN_ORDER_THRO_MAIL    108

// Text label control defines
#define TXT_REG_NUM            200
#define TXT_SHIP_HAND_COST     201
#define TXT_SHIP_HAND_STATIC   202
#define TXT_TITLE              203
#define TXT_IL_RESIDENT        204
#define TXT_TOTAL_COST         205
#define TXT_PRINTING           206
#define TXT_HLP_TITLE          207
#define TXT_MEDIA_DIR          208
#define TXT_ORDERING_INFO      209
#define TXT_PROGRAM_COST       210
#define TXT_CREDIT_CARD_INFO   211
#define TXT_STATE_TAX          212
#define TXT_ABOUT              213
#define TXT_CREDIT_CARD_EXP    214
#define TXT_CREDIT_CARD_NO     215

// Edit field defines
#define EDT_REG_NUMBER         300
#define EDT_HELPFUL_HINT       301
#define EDT_CITY               302
#define EDT_EMAIL              303
#define EDT_ADDRESS_2          304
#define EDT_ADDRESS_1          305
#define EDT_NAME               306
#define EDT_CREDIT_CARD_NO     307
#define EDT_EXP_DATE           308
#define EDT_APP_TITLE          309
#define EDT_EXE_NAME           310
#define EDT_CUST_PASS          311
#define EDT_USE_PASS_AFTER     312
#define EDT_INFO_NAME          313
#define EDT_INFO_ADDRESS1      314
#define EDT_INFO_ADDRESS2      315
#define EDT_WEB_ADDRESS        316
#define EDT_EMAIL_ADDRESS      317
#define EDT_SALES_TAX          318
#define EDT_REG_FEE            319
#define EDT_INFOWND_TITLE      320
#define EDT_INFOWND_TEXT       321
#define EDT_INFOWND_CAPTION    322
#define EDT_ZIP                323
#define EDT_COPYRIGHT_INFO     324
#define EDT_TEXT_WND           325
#define EDIT_CAPTION           325
#define EDT_CREDIT_URL	       326

// Check box defines
#define CHK_DONT_SHOW_AGAIN    400
#define CHK_FLOPPY_DISK        402
#define CHK_CHECKSUM           403
#define CHK_USE_REGNO          404
#define CHK_ALLOW_PRINTING     405
#define CHK_STARTUP_SCREEN     406
#define CHK_VISA               407
#define CHK_MASTERCARD         408
#define CHK_DISCOVER           409
#define CHK_AMERICAN_EXP       410
#define CHK_PRINT_PAGE_NO      412
#define CHK_USE_BK_BITMAP      413
#define CHK_ALLOW_FILE_EXPORTS 414

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
#define STC_PICTLOAD_IMAGE_WND 909
#define STC_PICTLOAD_INFO	   910
#define PROG_BAR	           911
#define STC_PROGBAR_TXT	  	   912
#define STC_LOOK_FOR           913


////////////////////////////
//    Enumerations
////////////////////////////

// String Table enums
enum
{
    REG_NUM_INFO_TEXT_VIEW_PROG=0,
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
    ENLARGE_PAGE,
    REDUCE_PAGE,
};

enum
{
    NO_PAGE_TYPE=0,
    PAGE_TYPE_MAIN,
    PAGE_TYPE_SOUND,
    PAGE_TYPE_AVI,
};


#endif  // __GDefines__