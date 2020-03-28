
/************************************************************************
*
*    PRODUCT:         Screen Saver
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

// Testing defines
//#define FULL_SCREEN_DEBUG
//#define DEBUG
//#define ON_OFF_TEST  // Causes the screen saver to turn on and off.

// Define the windows version
#define _WIN32_WINNT 0x0400

// Define packed structure, no padding
//#pragma pack(push, 1)

// This keeps us from having to compile all the different render flavor functions.
// Uncomment the ones you don't need and comment the ones you use
#define NO_16_TO_16_POS
#define NO_16_TO_16_POS_OR
#define NO_16_TO_16_POS_TRANS
#define NO_16_TO_16_NEG
#define NO_16_TO_16_NEG_OR
#define NO_16_TO_16_NEG_TRANS
#define NO_16_TO_16_NO_Z_BUF
#define NO_16_TO_16_NO_Z_BUF_OR
#define NO_16_TO_16_NO_Z_BUF_TRANS
#define NO_16_TO_16_POS_SOLID
#define NO_16_TO_16_NEG_SOLID

//#define NO_8_TO_16_POS
#define NO_8_TO_16_POS_CHECK
#define NO_8_TO_16_POS_OR
#define NO_8_TO_16_POS_TRANS
//#define NO_8_TO_16_NEG
#define NO_8_TO_16_NEG_OR
#define NO_8_TO_16_NEG_TRANS
#define NO_8_TO_16_NO_Z_BUF
#define NO_8_TO_16_NO_Z_BUF_OR
#define NO_8_TO_16_NO_Z_BUF_TRANS
//#define NO_8_TO_16_POS_SOLID
#define NO_8_TO_16_NEG_SOLID

//#define NO_8_TO_8_POS
#define NO_8_TO_8_POS_CHECK
#define NO_8_TO_8_POS_OR
#define NO_8_TO_8_POS_TRANS
#define NO_8_TO_8_NEG
#define NO_8_TO_8_NEG_OR
#define NO_8_TO_8_NEG_TRANS
#define NO_8_TO_8_NO_Z_BUF
#define NO_8_TO_8_NO_Z_BUF_OR
#define NO_8_TO_8_NO_Z_BUF_TRANS
//#define NO_8_TO_8_POS_SOLID
#define NO_8_TO_8_NEG_SOLID


/********************************************
*      Order form specific defines
********************************************/
// Screen Saver Specific
#define INI_FILE_NAME            "Columbia Tribute-Prefs.ini"
#define PRODUCT_NAME             "Columbia Tribute Screen Saver"
#define PROGRAM                  "Columbia Tribute"
#define PRODUCT_VERSION          "Columbia Tribute  Ver 1.0.0"
#define ORDER_FORM_TITLE         "Columbia Tribute Order Form       www.howiesfunware.com"
#define AOL_ADDRESS              "funware"
#define WEB_ADDRESS              "www.howiesfunware.com"
#define ONLINE_ORDER_WEB_ADDRESS "https://www.howiesfunware.com/horder.html"
#define EMAIL_ADDRESS            "funware@aol.com"
#define MAIL_ORDER_ONLY          "(Mail Order Only)"
#define CO_NAME                  "Howie's Funware"
#define CO_ADDRESS1              "1822 Forest Bend Lane"
#define CO_CITY                  "Keller"
#define CO_ZIP                   "76248"
#define NUM_US_STATES            51
#define STATE_ORDER_INDEX        44
#define FLOPPY_DISK_INFO         "Include 3-1/2 floppy disk ($%.2lf extra)"
#define CREDIT_CARD_INFO         "To pay by credit card, fill out the information below. Please double-check your credit card number and expiration date before printing."

// Costs
#define PRODUCT_COST             10.00
#define SALES_TAX                0.08125
#define FLOPPY_DISK_COST         3.50

#define FULL_VERSION_NUMBER      "021042-70015"
#define FAKE_VERSION_NUMBER      "05W010-9A235"
#define FIRST_TEMP_NUMBER        "70G-520-WH17"
#define SECOND_TEMP_NUMBER       "0R1-7P3-Z004"
#define FIRST_CHECK_FILE         "a1dljx23.dli"
#define SECOND_CHECK_FILE        "l5w2v7cb.dli"
#define SHAREWAEW_TMP_RUN_MAX    200

// String Sizes
#define MAX_TITLE                150
#define MAX_INFO                 200
#define MAX_REG_FEE              50
#define MAX_SALES_TAX            50
#define MAX_PREF_STRS            1
#define MAX_PAGE_STRS            4
#define SMALL_ARRAY_50           50
#define SMALL_ARRAY_100          100
#define MED_ARRAY_300            300
#define LARGE_ARRAY_500          500
#define MAX_STR_PATH             (MAX_PATH*2)  // Twice the string length just in case
#define MAX_OF_NAME              150
#define MAX_OF_ADDRESS           150
#define MAX_OF_CITY              70
#define MAX_OF_ZIP               50
#define MAX_OF_EMAIL             100

#define ORDER_FORM_INFO          "To register the %s ($%.2lf), fill out the order form, print it, then mail it in with your payment."

// Change these to change the install messages
#define TYPEOFPROGRAM            "Screen saver"
#define TYPEOFPROGRAM2           "screen saver"

//Misc
#define SEMAPHORE                "HowiesFunwareScreenSavers"
#define ENABLE                   TRUE
#define DISABLE                  FALSE
#define CENTER_WND_TO_PARENT     TRUE
#define MAIN_APP_TOOLS           "sstools.h"

// Key Codes
#define RETURN_KEY               0xD
#define LINEFEED_KEY             0xA 


////////////////////////////
//    Dialog Defines
////////////////////////////

// Dialog box Defines
#define DLG_QUIT_BOX             2

// Button Defines
#define BTN_DONE                 100
#define BTN_NO_PRINTER           101
#define BTN_OK                   102
#define BTN_CANCEL               103
#define BTN_ORDER_FORM           104
#define BTN_PRINT_ORDERFORM      105
#define BTN_PLAY_SOUND           106
#define BTN_ORDER_ONLINE         107
#define BTN_ORDER_THRO_MAIL      108
#define BTN_WEB_SITE             109

// Text label control defines
#define TXT_REG_NUM              200
#define TXT_SHIP_HAND_COST       201
#define TXT_SHIP_HAND_STATIC     202
#define TXT_TITLE                203
#define TXT_IL_RESIDENT          204
#define TXT_TOTAL_COST           205
#define TXT_PRINTING             206
#define TXT_CREDIT_CARD_INFO     207
#define TXT_STATE_TAX            208
#define TXT_HLP_TITLE            209
#define TXT_MEDIA_DIR            210
#define TXT_CREDIT_CARD_EXP      211
#define TXT_CREDIT_CARD_NO       212
#define TXT_SPEED                213
#define STC_RENDER_RES			 215

// Edit field defines
#define EDT_REG_NUMBER           300
#define EDT_HELPFUL_HINT         301
#define EDT_CITY                 302
#define EDT_EMAIL                303
#define EDT_ADDRESS_2            304
#define EDT_ADDRESS_1            305
#define EDT_NAME                 306
#define EDT_CREDIT_CARD_NO       307
#define EDT_EXP_DATE             308
#define EDT_ZIP                  309

// Check box defines
#define CHK_DONT_SHOW_AGAIN      400
#define CHK_IL_RESIDENT          401
#define CHK_FLOPPY_DISK          402
#define CHK_RAND_MUSIC           403
#define CHK_FILL_SCREEN	         404
#define CHK_TWINKLE_STARS	     405
#define CHK_ROTATE_FLAG	         406
#define CHK_FORCE_16BIT			 407

// Scroll bars and boxes
#define SCR_RENDER_RES	         501

// Radio Buttons
#define RDOBTN_VISA              600
#define RDOBTN_MASTERCARD        601
#define RDOBTN_DISCOVER          602
#define RDOBTN_AMER_EXP          603
#define RDO_MUSIC_OFF            604
#define RDO_DEFAULT_MUSIC        605
#define RDO_LOAD_YOUR_OWN        606

// Misc
#define SHOW_HELPHINT_WND        700
#define GRPBOX_MUSIC             701
#define CBO_STATE                702


#endif  // __GDefines__